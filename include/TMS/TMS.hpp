#ifndef TMS_INCLUDE_TMS_TMS_HPP
#define TMS_INCLUDE_TMS_TMS_HPP

#include <Canopen/co_core.h>
#include <EVT/dev/Thermistor.hpp>
#include <EVT/io/GPIO.hpp>
#include <EVT/utils/log.hpp>
#include "TMS/dev/TCA9545A.h"

#define NUM_THERMISTORS 4

namespace DEV = EVT::core::DEV;
namespace log = EVT::core::log;
namespace IO = EVT::core::IO;

namespace TMS {

class TMS {
public:
    /**
     * Construct a TMS instance with thermistors using the ADC instances in thermADCs
     *
     * @param thermADCs Array of pointers to ADCs used to create thermistor instances
     */
    TMS(IO::GPIO& m1, IO::GPIO& m2, EVT::core::IO::ADC& thermADC, TCA9545A tca9545A);

    /**
     * The node ID used to identify the device on the CAN network.
     */
    static constexpr uint8_t NODE_ID = 0x02;

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

private:
    /**
     * Array to store the thermistor objects
     */
    DEV::Thermistor thermistor;

    IO::GPIO& mux1;
    IO::GPIO& mux2;

    /**
     * TCA9545A instance
     */
     TCA9545A& tca9545A;

    /**
     * Array to store the thermistor values
     */
    uint32_t thermTemps[NUM_THERMISTORS];

    uint32_t thermVoltages[NUM_THERMISTORS];

    /**
     * Hardcoded conversion function from voltage to temperature in Celsius
     *
     * V(x): Converts from ADC units to voltage
     * V(x) = (3.3/4095)x
     *
     * C(x): Converts from voltage to Celsius (supplied by TMS EE team)
     * C(x) = 18.352x^3-69.323x^2+121.29x-7.3735
     *
     * m(x): Converts from Celsius to Millicelsius
     * m(x) = 1000x
     *
     * m(C(V(x))): Converts from ADC units to Millicelsius
     * m(C(V(x))) = (9604/10^9)x^3 - (4502/10^5)x^2 + (9774/10^2)x - 7374
     *
     * @param adcInput Raw voltage in ADC units out of a maximum of 4095
     * @return Temperature of the thermistor in Celsius
     */
    static uint32_t conversion(uint32_t adcInput) {
        auto adc64 = (uint64_t) adcInput;
        auto outTemp = (uint32_t) ((adc64 * adc64 * adc64) / 1000000000 * 9604
                                   - (adc64 * adc64) / 100000 * 4502
                                   + adc64 * 9774 / 100
                                   - 7374);
        log::LOGGER.log(log::Logger::LogLevel::DEBUG, "  -  Temp: %08x", outTemp);

        return outTemp;
    }

    /**
     * Have to know the size of the object dictionary for initialization
     * process.
     */
    static constexpr uint16_t OBJECT_DICTIONARY_SIZE = 47;

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
            .Data = (uintptr_t) 2000,
        },

        // TPDO1 settings
        // 0: The TPDO number, default 0
        // 1: The COB-ID used by TPDO1, provided as a function of the TPDO number
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
            .Data = (uintptr_t) 2000,
        },

        // TPDO2 settings
        // 0: The TPDO number, default 0
        // 1: The COB-ID used by TPDO1, provided as a function of the TPDO number
        // 2: How the TPO is triggered, default to manual triggering
        // 3: Inhibit time, defaults to 0
        // 5: Timer trigger time in 1ms units, 0 will disable the timer based triggering
        {
            .Key = CO_KEY(0x1802, 0, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 2,
        },
        {
            .Key = CO_KEY(0x1802, 1, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) CO_COBID_TPDO_DEFAULT(2) + NODE_ID,
        },
        {
            .Key = CO_KEY(0x1802, 2, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 0xFE,
        },
        {
            .Key = CO_KEY(0x1802, 3, CO_UNSIGNED16 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 0,
        },
        {
            .Key = CO_KEY(0x1802, 5, CO_UNSIGNED16 | CO_OBJ_D__R_),
            .Type = CO_TEVENT,
            .Data = (uintptr_t) 2000,
        },

        // TPDO3 settings
        // 0: The TPDO number, default 0
        // 1: The COB-ID used by TPDO1, provided as a function of the TPDO number
        // 2: How the TPO is triggered, default to manual triggering
        // 3: Inhibit time, defaults to 0
        // 5: Timer trigger time in 1ms units, 0 will disable the timer based triggering
        {
            .Key = CO_KEY(0x1803, 0, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 3,
        },
        {
            .Key = CO_KEY(0x1803, 1, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) CO_COBID_TPDO_DEFAULT(3) + NODE_ID,
        },
        {
            .Key = CO_KEY(0x1803, 2, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 0xFE,
        },
        {
            .Key = CO_KEY(0x1803, 3, CO_UNSIGNED16 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 0,
        },
        {
            .Key = CO_KEY(0x1803, 5, CO_UNSIGNED16 | CO_OBJ_D__R_),
            .Type = CO_TEVENT,
            .Data = (uintptr_t) 2000,
        },

        // TPDO0 mapping, determines the PDO messages to send when TPDO0 is triggered
        // 0: The number of PDO messages associated with the TPDO
        // 1: Link to the first PDO message
        // n: Link to the nth PDO message
        {
            .Key = CO_KEY(0x1A00, 0, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 2,
        },
        {
            .Key = CO_KEY(0x1A00, 1, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2100, 0, 8),
        },
        {
            .Key = CO_KEY(0x1A00, 2, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2100, 1, 8),
        },

        // TPDO1 mapping, determines the PDO messages to send when TPDO1 is triggered
        // 0: The number of PDO messages associated with the TPDO
        // 1: Link to the first PDO message
        // n: Link to the nth PDO message
        {
            .Key = CO_KEY(0x1A01, 0, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 2,
        },
        {
            .Key = CO_KEY(0x1A01, 1, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2100, 2, 8),
        },
        {
            .Key = CO_KEY(0x1A01, 2, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2100, 3, 8),
        },

        // TPDO2 mapping, determines the PDO messages to send when TPDO2 is triggered
        // 0: The number of PDO messages associated with the TPDO
        // 1: Link to the first PDO message
        // n: Link to the nth PDO message
        {
            .Key = CO_KEY(0x1A02, 0, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 2,
        },
        {
            .Key = CO_KEY(0x1A02, 1, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2100, 4, 8),
        },
        {
            .Key = CO_KEY(0x1A02, 2, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2100, 5, 8),
        },

        // TPDO3 mapping, determines the PDO messages to send when TPDO3 is triggered
        // 0: The number of PDO messages associated with the TPDO
        // 1: Link to the first PDO message
        // n: Link to the nth PDO message
        {
            .Key = CO_KEY(0x1A03, 0, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 2,
        },
        {
            .Key = CO_KEY(0x1A03, 1, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2100, 6, 8),
        },
        {
            .Key = CO_KEY(0x1A03, 2, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2100, 7, 8),
        },

        // User defined data, this will be where we put elements that can be
        // accessed via SDO and depending on configuration PDO

        /**
        * The discrepency with the case and voltage index is due to an apparent miswiring in the TMS itself
        * The conversion to read temperatures in the correct order is as follows:
        * [Temp 1] is actually from [Temp 2]
        * [Temp 2] is actually from [Temp 3]
        * [Temp 3] is actually from [Temp 4]
        * [Temp 4] is actually from [Temp 1]
        */
        {
            .Key = CO_KEY(0x2100, 0, CO_UNSIGNED32 | CO_OBJ___PR_),
            .Type = nullptr,
            .Data = (uintptr_t) &thermTemps[1],
        },
        {
            .Key = CO_KEY(0x2100, 1, CO_UNSIGNED32 | CO_OBJ___PR_),
            .Type = nullptr,
            .Data = (uintptr_t) &thermTemps[2],
        },
        {
            .Key = CO_KEY(0x2100, 2, CO_UNSIGNED32 | CO_OBJ___PR_),
            .Type = nullptr,
            .Data = (uintptr_t) &thermTemps[3],
        },
        {
            .Key = CO_KEY(0x2100, 3, CO_UNSIGNED32 | CO_OBJ___PR_),
            .Type = nullptr,
            .Data = (uintptr_t) &thermTemps[0],
        },
        {
            .Key = CO_KEY(0x2100, 4, CO_UNSIGNED32 | CO_OBJ___PR_),
            .Type = nullptr,
            .Data = (uintptr_t) &thermVoltages[1],
        },
        {
            .Key = CO_KEY(0x2100, 5, CO_UNSIGNED32 | CO_OBJ___PR_),
            .Type = nullptr,
            .Data = (uintptr_t) &thermVoltages[2],
        },
        {
            .Key = CO_KEY(0x2100, 6, CO_UNSIGNED32 | CO_OBJ___PR_),
            .Type = nullptr,
            .Data = (uintptr_t) &thermVoltages[3],
        },
        {
            .Key = CO_KEY(0x2100, 7, CO_UNSIGNED32 | CO_OBJ___PR_),
            .Type = nullptr,
            .Data = (uintptr_t) &thermVoltages[0],
        },

        // End of dictionary marker
        CO_OBJ_DIR_ENDMARK,
    };
};

}// namespace TMS

#endif//TMS_INCLUDE_TMS_TMS_HPP
