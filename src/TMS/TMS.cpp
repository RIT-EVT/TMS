#include <TMS/TMS.hpp>

namespace TMS {

TMS::TMS(IO::GPIO& m1, IO::GPIO& m2, IO::ADC& thermADC, TCA9545A tca9545A) : thermistor(DEV::Thermistor(thermADC, conversion)),
                                                                             mux1(m1), mux2(m2), tca9545A(tca9545A) {}

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

}// namespace TMS
