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
uint16_t TMS::getObjectDictionarySize() {
    return OBJECT_DICTIONARY_SIZE;
}
void TMS::process(RadiatorFan* fans, HeatPump pump) {
    RadiatorFan fan1 = fans[0];
    RadiatorFan fan2 = fans[1];

    uint16_t highestTemp = 0;
    for(uint16_t temp: sensorTemps) {
        if (temp > highestTemp) {
            highestTemp = temp;
        }
    }
    highestTemp /= 100;
    log::LOGGER.log(log::Logger::LogLevel::DEBUG, "Highest Temp: %d\r\n", highestTemp);

    if (highestTemp >= 50) {
        // COOL_LEVEL_MAX
        log::LOGGER.log(log::Logger::LogLevel::DEBUG, "Setting Level: COOL_LEVEL_MAX");
        pump.setSpeed(100);
        fan1.setSpeed(100);
        fan2.setSpeed(100);
    } else if (highestTemp >= 40) {
        // COOL_LEVEL_2
        log::LOGGER.log(log::Logger::LogLevel::DEBUG, "Setting Level: COOL_LEVEL_2");
        pump.setSpeed(75);
        fan1.setSpeed(50);
        fan2.setSpeed(50);
    } else if (highestTemp >= 30) {
        // COOL_LEVEL_1
        log::LOGGER.log(log::Logger::LogLevel::DEBUG, "Setting Level: COOL_LEVEL_1");
        pump.setSpeed(50);
        fan1.setSpeed(25);
        fan2.setSpeed(25);
    } else {
        // IDLE
        log::LOGGER.log(log::Logger::LogLevel::DEBUG, "Setting Level: IDLE");
        pump.setSpeed(50);
        fan1.setSpeed(0);
        fan2.setSpeed(0);
    }
}

}// namespace TMS
