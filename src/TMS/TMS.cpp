#include <TMS/TMS.hpp>

namespace TMS {

uint16_t TMS::sensorTemps[NUM_TEMP_SENSORS] = {};

TMS::TMS(TCA9545A& tca9545A) : tca9545A(tca9545A) {}

void TMS::updateTemps() {
    log::LOGGER.log(log::Logger::LogLevel::DEBUG, "Updating Temps");
    tca9545A.pollDevices();
}

CO_OBJ_T* TMS::getObjectDictionary() {
    return &objectDictionary[0];
}
uint8_t TMS::getNumElements() {
    return OBJECT_DICTIONARY_SIZE;
}
uint8_t TMS::getNodeID() {
    return TMS::NODE_ID;
}

void TMS::process(RadiatorFan* fans, HeatPump pump) {
    RadiatorFan fan1 = fans[0];
    //    RadiatorFan fan2 = fans[1];

    uint16_t highestTemp = 0;
    for (uint8_t i = 1; i < 4; i++) {
        uint16_t temp = sensorTemps[i];
        if (temp > highestTemp) {
            highestTemp = temp;
        }
    }
    highestTemp /= 100;
    log::LOGGER.log(log::Logger::LogLevel::DEBUG, "Highest Temp: %d\r\n", highestTemp);

    if (highestTemp >= 50) {
        // COOL_LEVEL_MAX
        log::LOGGER.log(log::Logger::LogLevel::DEBUG, "Setting Level: COOL_LEVEL_MAX");
        pumpSpeed = 100;
        fan1Speed = 100;
        fan2Speed = 100;
    } else if (highestTemp >= 40) {
        // COOL_LEVEL_2
        log::LOGGER.log(log::Logger::LogLevel::DEBUG, "Setting Level: COOL_LEVEL_2");
        pumpSpeed = 75;
        fan1Speed = 75;
        fan2Speed = 75;
    } else if (highestTemp >= 30) {
        // COOL_LEVEL_1
        log::LOGGER.log(log::Logger::LogLevel::DEBUG, "Setting Level: COOL_LEVEL_1");
        pumpSpeed = 50;
        fan1Speed = 50;
        fan2Speed = 50;
    } else {
        // IDLE
        log::LOGGER.log(log::Logger::LogLevel::DEBUG, "Setting Level: IDLE");
        pumpSpeed = 50;
        fan1Speed = 0;
        fan2Speed = 0;
    }

    pump.setSpeed(pumpSpeed);
    fan1.setSpeed(fan1Speed);
    //    fan2.setSpeed(fan2Speed);
}

}// namespace TMS
