#ifndef TMS_INCLUDE_TMS_TMS_HPP
#define TMS_INCLUDE_TMS_TMS_HPP

#include <Canopen/co_core.h>
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
     * Construct a TMS instance with thermistors using the ADC instances in thermADCs
     *
     */
    explicit TMS(TCA9545A& tca9545A);

    /**
     * Array to store the thermistor values
     */
    static uint16_t sensorTemps[NUM_TEMP_SENSORS];

    uint8_t pumpSpeed = 0;
    uint8_t fan1Speed = 0;
    uint8_t fan2Speed = 0;

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
     * TMS State Machine Hack
     */
    void process(RadiatorFan* fans, HeatPump pump);

private:
    /**
     * TCA9545A instance
     */
    TCA9545A& tca9545A;

    /**
     * Have to know the size of the object dictionary for initialization
     * process.
     */
    static constexpr uint16_t OBJECT_DICTIONARY_SIZE = 37;

    CO_OBJ_T objectDictionary[OBJECT_DICTIONARY_SIZE + 1] = {
        // Sync ID, defaults to 0x80
        {
            .Key = CO_KEY(0x1005, 0, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 0x80,
        },
        // Information about the hardware, hard coded sample values for now
        // 1: Vendor ID
        // 2: Product Code
        // 3: Revision Number
        // 4: Serial Number
        {
            .Key = CO_KEY(0x1018, 1, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 0x10},
        {
            .Key = CO_KEY(0x1018, 2, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 0x11,
        },
        {
            .Key = CO_KEY(0x1018, 3, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 0x12,
        },
        {
            .Key = CO_KEY(0x1018, 4, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 0x13,
        },

        // SDO CAN message IDS.
        // 1: Client -> Server ID, default is 0x600 + NODE_ID
        // 2: Server -> Client ID, default is 0x580 + NODE_ID
        {
            .Key = CO_KEY(0x1200, 1, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 0x600 + NODE_ID,
        },
        {
            .Key = CO_KEY(0x1200, 2, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 0x580 + NODE_ID,
        },

        // TPDO0 settings
        // 0: The TPDO number, default 0
        // 1: The COB-ID used by TPDO0, provided as a function of the TPDO number
        // 2: How the TPO is triggered, default to manual triggering
        // 3: Inhibit time, defaults to 0
        // 5: Timer trigger time in 1ms units, 0 will disable the timer based triggering
        {
            .Key = CO_KEY(0x1800, 0, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 0,
        },
        {
            .Key = CO_KEY(0x1800, 1, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) CO_COBID_TPDO_DEFAULT(0) + NODE_ID,
        },
        {
            .Key = CO_KEY(0x1800, 2, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 0xFE,
        },
        {
            .Key = CO_KEY(0x1800, 3, CO_UNSIGNED16 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 0,
        },
        {
            .Key = CO_KEY(0x1800, 5, CO_UNSIGNED16 | CO_OBJ_D__R_),
            .Type = CO_TEVENT,
            .Data = (uintptr_t) 1000,
        },

        // TPDO1 settings
        // 0: The TPDO number, default 0
        // 1: The COB-ID used by TPDO0, provided as a function of the TPDO number
        // 2: How the TPO is triggered, default to manual triggering
        // 3: Inhibit time, defaults to 0
        // 5: Timer trigger time in 1ms units, 0 will disable the timer based triggering
        {
            .Key = CO_KEY(0x1801, 0, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 1,
        },
        {
            .Key = CO_KEY(0x1801, 1, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) CO_COBID_TPDO_DEFAULT(1) + NODE_ID,
        },
        {
            .Key = CO_KEY(0x1801, 2, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 0xFE,
        },
        {
            .Key = CO_KEY(0x1801, 3, CO_UNSIGNED16 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 0,
        },
        {
            .Key = CO_KEY(0x1801, 5, CO_UNSIGNED16 | CO_OBJ_D__R_),
            .Type = CO_TEVENT,
            .Data = (uintptr_t) 1000,
        },

        // TPDO0 mapping, determines the PDO messages to send when TPDO0 is triggered
        // 0: The number of PDO messages associated with the TPDO
        // 1: Link to the first PDO message
        // n: Link to the nth PDO message
        {
            .Key = CO_KEY(0x1A00, 0, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 4,
        },
        {
            .Key = CO_KEY(0x1A00, 1, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2100, 0, 16),
        },
        {
            .Key = CO_KEY(0x1A00, 2, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2100, 1, 16),
        },
        {
            .Key = CO_KEY(0x1A00, 3, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2100, 2, 16),
        },
        {
            .Key = CO_KEY(0x1A00, 4, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2100, 3, 16),
        },

        // TPDO1 mapping, determines the PDO messages to send when TPDO0 is triggered
        // 0: The number of PDO messages associated with the TPDO
        // 1: Link to the first PDO message
        // n: Link to the nth PDO message
        {
            .Key = CO_KEY(0x1A01, 0, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 3,
        },
        {
            .Key = CO_KEY(0x1A01, 1, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2100, 4, 8),
        },
        {
            .Key = CO_KEY(0x1A01, 2, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2100, 5, 8),
        },
        {
            .Key = CO_KEY(0x1A01, 3, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2100, 6, 8),
        },

        // User defined data, this will be where we put elements that can be
        // accessed via SDO and depending on configuration PDO

        {
            .Key = CO_KEY(0x2100, 0, CO_UNSIGNED16 | CO_OBJ___PR_),
            .Type = nullptr,
            .Data = (uintptr_t) &sensorTemps[0],
        },
        {
            .Key = CO_KEY(0x2100, 1, CO_UNSIGNED16 | CO_OBJ___PR_),
            .Type = nullptr,
            .Data = (uintptr_t) &sensorTemps[1],
        },
        {
            .Key = CO_KEY(0x2100, 2, CO_UNSIGNED16 | CO_OBJ___PR_),
            .Type = nullptr,
            .Data = (uintptr_t) &sensorTemps[2],
        },
        {
            .Key = CO_KEY(0x2100, 3, CO_UNSIGNED16 | CO_OBJ___PR_),
            .Type = nullptr,
            .Data = (uintptr_t) &sensorTemps[3],
        },
        {
            .Key = CO_KEY(0x2100, 4, CO_UNSIGNED8 | CO_OBJ___PR_),
            .Type = nullptr,
            .Data = (uintptr_t) &pumpSpeed,
        },
        {
            .Key = CO_KEY(0x2100, 5, CO_UNSIGNED8 | CO_OBJ___PR_),
            .Type = nullptr,
            .Data = (uintptr_t) &fan1Speed,
        },
        {
            .Key = CO_KEY(0x2100, 6, CO_UNSIGNED8 | CO_OBJ___PR_),
            .Type = nullptr,
            .Data = (uintptr_t) &fan2Speed,
        },

        // End of dictionary marker
        CO_OBJ_DIR_ENDMARK,
    };
};

}// namespace TMS

#endif//TMS_INCLUDE_TMS_TMS_HPP
