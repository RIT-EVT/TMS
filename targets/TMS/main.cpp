/**
 * This is the primary file for running the Thermal Management System.
 */
#include <EVT/io/CANopen.hpp>
#include <EVT/io/UART.hpp>
#include <EVT/io/manager.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/io/types/CANMessage.hpp>
#include <EVT/utils/log.hpp>
#include <EVT/utils/types/FixedQueue.hpp>

#include <EVT/dev/platform/f3xx/f302x8/Timerf302x8.hpp>
#include <TMS/TMS.hpp>
#include <TMS/dev/HeatPump.hpp>
#include <TMS/dev/RadiatorFan.hpp>

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
            mode = CO_OPERATIONAL;
            break;
        case 0x80:
            mode = CO_PREOP;
            break;
        default:
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
void canInterruptHandler(IO::CANMessage& message, void* priv) {
    log::LOGGER.log(log::Logger::LogLevel::DEBUG, "CAN Message received.");

    // Handle NMT messages
    if (message.getId() == 0) {
        handleNMT(message);
        return;
    }

    EVT::core::types::FixedQueue<CANOPEN_QUEUE_SIZE, IO::CANMessage>* queue =
        (EVT::core::types::FixedQueue<CANOPEN_QUEUE_SIZE, IO::CANMessage>*) priv;
    if (queue == nullptr)
        return;
    if (!message.isCANExtended())
        queue->append(message);
}

///////////////////////////////////////////////////////////////////////////////
// CANopen specific Callbacks. Need to be defined in some location
///////////////////////////////////////////////////////////////////////////////
extern "C" void CONodeFatalError(void) {
    log::LOGGER.log(log::Logger::LogLevel::ERROR, "Fatal CANopen error");
}

extern "C" void COIfCanReceive(CO_IF_FRM* frm) {}

extern "C" int16_t COLssStore(uint32_t baudrate, uint8_t nodeId) { return 0; }

extern "C" int16_t COLssLoad(uint32_t* baudrate, uint8_t* nodeId) { return 0; }

extern "C" void CONmtModeChange(CO_NMT* nmt, CO_MODE mode) {
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Network Management state changed.");
}

extern "C" void CONmtHbConsEvent(CO_NMT* nmt, uint8_t nodeId) {}

extern "C" void CONmtHbConsChange(CO_NMT* nmt, uint8_t nodeId, CO_MODE mode) {}

extern "C" int16_t COParaDefault(CO_PARA* pg) { return 0; }

extern "C" void COPdoTransmit(CO_IF_FRM* frm) {}

extern "C" int16_t COPdoReceive(CO_IF_FRM* frm) { return 0; }

extern "C" void COPdoSyncUpdate(CO_RPDO* pdo) {}

extern "C" void COTmrLock(void) {}

extern "C" void COTmrUnlock(void) {}

int main() {
    // Initialize system
    IO::init();

    // Queue that will store CANopen messages
    EVT::core::types::FixedQueue<CANOPEN_QUEUE_SIZE, IO::CANMessage> canOpenQueue;

    // Initialize CAN, add an IRQ that will populate the above queue
    IO::CAN& can = IO::getCAN<IO::Pin::PA_12, IO::Pin::PA_11>();
    can.addIRQHandler(canInterruptHandler, reinterpret_cast<void*>(&canOpenQueue));

    // Initialize the timer
    DEV::Timerf302x8 timer(TIM2, 100);

    // Set up Logger
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);
    log::LOGGER.setUART(&uart);
    log::LOGGER.setLogLevel(log::Logger::LogLevel::DEBUG);
    log::LOGGER.log(log::Logger::LogLevel::DEBUG, "Logger initialized.");
    timer.stopTimer();

    // Set up TMS and necessary device drivers
    IO::ADC* adcs[4] = {
        &IO::getADC<IO::Pin::PC_0>(),
        &IO::getADC<IO::Pin::PC_1>(),
        &IO::getADC<IO::Pin::PB_0>(),
        &IO::getADC<IO::Pin::PA_4>(),
    };
    TMS::TMS tms(adcs);
    IO::PWM& pwm = IO::getPWM<IO::Pin::PA_1>();
    auto pump = TMS::HeatPump(pwm);
    TMS::RadiatorFan fans[] = {
        TMS::RadiatorFan(IO::getGPIO<IO::Pin::PC_2>()),
        TMS::RadiatorFan(IO::getGPIO<IO::Pin::PC_3>())};

    // Reserved memory for CANopen stack usage
    uint8_t sdoBuffer[1][CO_SDO_BUF_BYTE];
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
                fan.powerOn(0);
            }
            break;
        // Operational Mode
        case CO_OPERATIONAL:
            // Update the state of timer based events
            COTmrService(&canNode.Tmr);
            // Handle executing timer events that have elapsed
            COTmrProcess(&canNode.Tmr);

            // Activate the pump and fans -- will be replaced with more advanced cooling logic later
            pump.setSpeed(60);
            for (TMS::RadiatorFan fan : fans) {
                fan.powerOn(1);
            }
            break;
        default:
            log::LOGGER.log(log::Logger::LogLevel::ERROR, "Network Management state is not valid.");
        }

        time::wait(250);
    }
}
