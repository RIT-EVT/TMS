#ifndef TMS_HPP
#define TMS_HPP

#include <EVT/io/CANDevice.hpp>
#include <EVT/io/CANOpenMacros.hpp>
#include <co_core.h>
#include <EVT/dev/Thermistor.hpp>
#include <EVT/io/GPIO.hpp>
#include <EVT/utils/log.hpp>
#include <TMS/dev/HeatPump.hpp>
#include <TMS/dev/RadiatorFan.hpp>
#include <TMS/dev/TCA9545A.hpp>

#define NUM_TEMP_SENSORS 4

namespace DEV = EVT::core::DEV;
namespace log = EVT::core::log;
namespace IO = EVT::core::IO;

namespace TMS {

class TMS {
public:
    /**
     * Construct a TMS instance
     *
     * @param tca9545A I2C MUX instance to use for getting temp sensor data
     */
    explicit TMS(TCA9545A& tca9545A);

    /**
     * Array to store the thermistor values
     * Must be public, so they can be written to by the TMP117Device class
     * TODO: Refactor code to make this private
     */
    static uint16_t sensorTemps[NUM_TEMP_SENSORS];

    /**
     * The node ID used to identify the device on the CAN network.
     */
    static constexpr uint8_t NODE_ID = 0x08;

    /**
     * Update the saved thermistor temperature values with the latest data from the thermistors
     */
    void updateTemps();

    /**
     * Get a pointer to the start of the CANopen object dictionary.
     *
     * @return Pointer to the start of the CANopen object dictionary.
     */
    CO_OBJ_T* getObjectDictionary();

    /**
     * Get the number of elements in the object dictionary.
     *
     * @return The number of elements in the object dictionary
     */
    uint16_t getObjectDictionarySize();

    /**
     * Update fan and pump speeds
     * TODO: Modify to meet EVT standards
     *
     * @param fans Array of radiator fan instances
     * @param pump Heat pump
     */
    void process(RadiatorFan* fans, HeatPump pump);

private:
    /**
     * TCA9545A instance
     */
    TCA9545A& tca9545A;

    /**
     * Current heat pump speed
     */
    uint8_t pumpSpeed = 0;

    /**
     * Fan 1 speed
     */
    uint8_t fan1Speed = 0;

    /**
     * Fan 2 speed
     */
    uint8_t fan2Speed = 0;

    /**
     * Have to know the size of the object dictionary for initialization
     * process.
     */
    static constexpr uint16_t OBJECT_DICTIONARY_SIZE = 41;

    CO_OBJ_T objectDictionary[OBJECT_DICTIONARY_SIZE + 1] = {
        MANDATORY_IDENTIFICATION_ENTRIES_1000_1014,
        HEARTBEAT_PRODUCER_1017(2000),
        IDENTITY_OBJECT_1018,
        SDO_CONFIGURATION_1200, // Mandatory Keys

        TRANSMIT_PDO_SETTINGS_OBJECT_18XX(0,0,NODE_ID,TRANSMIT_PDO_TRIGGER_TIMER),
        TRANSMIT_PDO_SETTINGS_OBJECT_18XX(1,1,NODE_ID,TRANSMIT_PDO_TRIGGER_TIMER),

        TRANSMIT_PDO_MAPPING_START_KEY_1AXX(0,4),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0,1,PDO_MAPPING_UNSIGNED16),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0,2,PDO_MAPPING_UNSIGNED16),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0,3,PDO_MAPPING_UNSIGNED16),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0,4,PDO_MAPPING_UNSIGNED16),

        TRANSMIT_PDO_MAPPING_START_KEY_1AXX(1,3),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(1,1,PDO_MAPPING_UNSIGNED16),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(1,2,PDO_MAPPING_UNSIGNED16),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(1,3,PDO_MAPPING_UNSIGNED16),

        DATA_LINK_START_KEY_21XX(0,4),
        DATA_LINK_21XX(0,0,CO_TUNSIGNED16,&sensorTemps[0]),
        DATA_LINK_21XX(0,1,CO_TUNSIGNED16,&sensorTemps[1]),
        DATA_LINK_21XX(0,2,CO_TUNSIGNED16,&sensorTemps[2]),
        DATA_LINK_21XX(0,3,CO_TUNSIGNED16,&sensorTemps[3]),

        DATA_LINK_START_KEY_21XX(1,3),
        DATA_LINK_21XX(1,0,CO_TUNSIGNED16,&pumpSpeed),
        DATA_LINK_21XX(1,1,CO_TUNSIGNED16,&fan1Speed),
        DATA_LINK_21XX(1,2,CO_TUNSIGNED16, &fan2Speed),

        // End of dictionary marker
        CO_OBJ_DICT_ENDMARK,
    };
};

}// namespace TMS

#endif//TMS_HPP
