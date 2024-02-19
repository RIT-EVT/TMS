/**
 * This is the primary file for running the Thermal Management System.
 */
#include <EVT/io/CANopen.hpp>
#include <EVT/io/UART.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/io/types/CANMessage.hpp>
#include <EVT/manager.hpp>
#include <EVT/utils/log.hpp>
#include <EVT/utils/types/FixedQueue.hpp>

#include "../../libs/EVT-core/samples/canopen/canopen_tpdo/TPDOCanNode.hpp"
#include <TMS/TMS.hpp>
#include <TMS/dev/HeatPump.hpp>
#include <TMS/dev/I2CDevice.hpp>
#include <TMS/dev/RadiatorFan.hpp>
#include <TMS/dev/TMP117.hpp>
#include <TMS/dev/TMP117I2CDevice.hpp>

namespace IO = EVT::core::IO;
namespace DEV = EVT::core::DEV;
namespace time = EVT::core::time;
namespace log = EVT::core::log;

// Global CAN Node reference
CO_NODE canNode;

void handleNMT(IO::CANMessage& message) {
    log::LOGGER.log(log::Logger::LogLevel::DEBUG, "Network Management message recognized.");
    uint8_t* payload = message.getPayload();
    uint8_t targetID = payload[1];
    if (targetID == TMS::TMS::NODE_ID || targetID == 0x00) {
        CO_MODE mode;
        switch (payload[0]) {
        case 0x01:
            log::LOGGER.log(log::Logger::LogLevel::DEBUG, "NMT State: Operational");
            mode = CO_OPERATIONAL;
            break;
        case 0x80:
            log::LOGGER.log(log::Logger::LogLevel::DEBUG, "NMT State: Preoperational");
            mode = CO_PREOP;
            break;
        default:
            log::LOGGER.log(log::Logger::LogLevel::DEBUG, "NMT State: Invalid");
            mode = CO_INVALID;
        }
        if (canNode.Nmt.Mode != mode)
            CONmtSetMode(&canNode.Nmt, mode);
    }
}

/**
 * Interrupt handler for incoming CAN messages.
 *
 * @param priv[in] The private data (FixedQueue<CANOPEN_QUEUE_SIZE, CANMessage>)
 */
void canInterrupt(IO::CANMessage& message, void* priv) {
    log::LOGGER.log(log::Logger::LogLevel::DEBUG, "CAN Message received.");

    // Handle NMT messages
    if (message.getId() == 0) {
        handleNMT(message);
        return;
    }
    auto* queue = (EVT::core::types::FixedQueue<CANOPEN_QUEUE_SIZE, IO::CANMessage>*) priv;
    if (queue != nullptr)
        queue->append(message);
}

int main() {
    // Initialize system
    EVT::core::platform::init();

    // Queue that will store CANopen messages
    EVT::core::types::FixedQueue<CANOPEN_QUEUE_SIZE, IO::CANMessage> canOpenQueue;

    // Initialize CAN, add an IRQ that will populate the above queue
    IO::CAN& can = IO::getCAN<IO::Pin::PA_12, IO::Pin::PA_11>();
    can.addIRQHandler(canInterrupt, reinterpret_cast<void*>(&canOpenQueue));

    // Initialize the timer
    DEV::Timer& timer = DEV::getTimer<DEV::MCUTimer::Timer16>(100);

    // Set up Logger
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);
    log::LOGGER.setUART(&uart);
    log::LOGGER.setLogLevel(log::Logger::LogLevel::DEBUG);
    log::LOGGER.log(log::Logger::LogLevel::DEBUG, "Logger initialized.");
    timer.stopTimer();

    TMS::HeatPump pump(IO::getPWM<IO::Pin::PA_6>());

    IO::I2C& i2c = IO::getI2C<IO::Pin::PB_8, IO::Pin::PB_9>();

    //array storing I2CDevices
    TMS::TMP117I2CDevice devices[3];

    //BUS POINTERS
    //array of buses
    TMS::TMP117I2CDevice** buses[4];
    //buses
    TMS::TMP117I2CDevice* bus0[0];
    TMS::TMP117I2CDevice* bus1[3];
    TMS::TMP117I2CDevice* bus2[1];
    TMS::TMP117I2CDevice* bus3[0];

    //set each index in buses array to be a bus
    buses[0] = bus0;
    buses[1] = bus1;
    buses[2] = bus2;
    buses[3] = bus3;

    //TODO: figure out why stuff is "implicitly deleted"
    // Set up TMS and necessary device drivers
    TMS::TMP117 tmpDevices[4];

    tmpDevices[0] = TMS::TMP117(&i2c, 0x48);
    devices[0] = TMS::TMP117I2CDevice(&tmpDevices[0], &TMS::TMS::sensorTemps[0]);
    bus2[0] = &devices[0];

    tmpDevices[1] = TMS::TMP117(&i2c, 0x4A);
    devices[1] = TMS::TMP117I2CDevice(&tmpDevices[1], &TMS::TMS::sensorTemps[2]);
    bus1[0] = &devices[1];

    tmpDevices[2] = TMS::TMP117(&i2c, 0x4B);
    devices[2] = TMS::TMP117I2CDevice(&tmpDevices[2], &TMS::TMS::sensorTemps[3]);
    bus1[1] = &devices[2];

    uint8_t numDevices[4] = {0, 2, 1, 0};

    TMS::TCA9545A tca(i2c, 0x70, reinterpret_cast<TMS::I2CDevice***>(buses), numDevices);
    TMS::TMS tms(tca);

    TMS::RadiatorFan fans[1] = {
        TMS::RadiatorFan(IO::getPWM<IO::Pin::PA_0>(),
                         IO::getGPIO<IO::Pin::PA_1>(IO::GPIO::Direction::OUTPUT),
                         IO::getGPIO<IO::Pin::PB_10>(IO::GPIO::Direction::OUTPUT)),
    };

    // Reserved memory for CANopen stack usage
    uint8_t sdoBuffer[CO_SSDO_N * CO_SDO_BUF_BYTE];
    CO_TMR_MEM appTmrMem[4];

    // Attempt to join the CAN network
    IO::CAN::CANStatus result = can.connect();

    if (result != IO::CAN::CANStatus::OK) {
        uart.printf("Failed to connect to CAN network\r\n");
        return 1;
    }

    // Make drivers
    CO_IF_DRV canStackDriver;

    CO_IF_CAN_DRV canDriver;
    CO_IF_TIMER_DRV timerDriver;
    CO_IF_NVM_DRV nvmDriver;

    IO::getCANopenCANDriver(&can, &canOpenQueue, &canDriver);
    IO::getCANopenTimerDriver(&timer, &timerDriver);
    IO::getCANopenNVMDriver(&nvmDriver);

    canStackDriver.Can = &canDriver;
    canStackDriver.Timer = &timerDriver;
    canStackDriver.Nvm = &nvmDriver;

    CO_NODE_SPEC canSpec = {
        .NodeId = TMS::TMS::NODE_ID,
        .Baudrate = IO::CAN::DEFAULT_BAUD,
        .Dict = tms.getObjectDictionary(),
        .DictLen = tms.getObjectDictionarySize(),
        .EmcyCode = nullptr,
        .TmrMem = appTmrMem,
        .TmrNum = 16,
        .TmrFreq = 100,
        .Drv = &canStackDriver,
        .SdoBuf = reinterpret_cast<uint8_t*>(&sdoBuffer[0]),
    };

    CONodeInit(&canNode, &canSpec);
    CONodeStart(&canNode);

    log::LOGGER.log(log::Logger::LogLevel::DEBUG, "Entering loop");
    EVT::core::platform::init();

    //create the TPDO node
    TPDOCanNode testCanNode;


    ///////////////////////////////////////////////////////////////////////////
    // Setup CAN configuration, this handles making drivers, applying settings.
    // And generally creating the CANopen stack node which is the interface
    // between the application (the code we write) and the physical CAN network
    ///////////////////////////////////////////////////////////////////////////

    // Will store CANopen messages that will be populated by the EVT-core CAN
    // interrupt


    // Initialize CAN, add an IRQ which will add messages to the queue above
    can.addIRQHandler(canInterrupt, reinterpret_cast<void*>(&canOpenQueue));




    //test that the board is connected to the can network
    if (result != IO::CAN::CANStatus::OK) {
        uart.printf("Failed to connect to CAN network\r\n");
        return 1;
    }

    // Initialize all the CANOpen drivers.
    IO::initializeCANopenDriver(&canOpenQueue, &can, &timer, &canStackDriver, &nvmDriver, &timerDriver, &canDriver);

    // Initialize the CANOpen node we are using.
    IO::initializeCANopenNode(&canNode, &testCanNode, &canStackDriver, sdoBuffer, appTmrMem);

    // Set the node to operational mode
    CONmtSetMode(&canNode.Nmt, CO_OPERATIONAL);

    time::wait(500);

    //print any CANopen errors
    uart.printf("Error: %d\r\n", CONodeGetErr(&canNode));

    ///////////////////////////////////////////////////////////////////////////
    // Main loop
    ///////////////////////////////////////////////////////////////////////////

    while (1) {
        // Update the thermistor temperatures
        tms.updateTemps();
        // Process incoming CAN messages
        CONodeProcess(&canNode);

        switch (CONmtGetMode(&canNode.Nmt)) {
        // Auxiliary Mode
        case CO_PREOP:
            // Turn the pump and fans off
            pump.stop();
            for (TMS::RadiatorFan fan : fans) {
                fan.setSpeed(0);
            }
            break;
        // Operational Mode
        case CO_OPERATIONAL:
            // Update the state of timer based events
            COTmrService(&canNode.Tmr);
            // Handle executing timer events that have elapsed
            COTmrProcess(&canNode.Tmr);

            // Activate the pump and fans -- will be replaced with more advanced cooling logic later
            tms.process(fans, pump);
            break;
        default:
            log::LOGGER.log(log::Logger::LogLevel::ERROR, "Network Management state is not valid.");
        }

        time::wait(250);
    }
}
