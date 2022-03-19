#include <TMS/TMS.hpp>

namespace TMS {

TMS::TMS(EVT::core::IO::ADC* thermADCs[4]) : thermistors{
    DEV::Thermistor(*thermADCs[0], conversion),
    DEV::Thermistor(*thermADCs[1], conversion),
    DEV::Thermistor(*thermADCs[2], conversion),
    DEV::Thermistor(*thermADCs[3], conversion),
} {}

void TMS::updateTemps() {
    log::LOGGER.log(log::Logger::LogLevel::DEBUG, "Updating Temps");
    for (int i = 0; i < NUM_THERMISTORS; i++) {
        thermTemps[i] = thermistors[i].getTempCelcius();
    }
}

CO_OBJ_T* TMS::getObjectDictionary() {
    return &objectDictionary[0];
}
uint16_t TMS::getObjectDictionarySize() {
    return OBJECT_DICTIONARY_SIZE;
}

}// namespace TMS
