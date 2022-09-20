#include <TMS/TMS.hpp>

namespace TMS {

TMS::TMS(IO::GPIO& m1, IO::GPIO& m2, IO::ADC& thermADC) : thermistor(DEV::Thermistor(thermADC, conversion)),
                                                          mux1(m1), mux2(m2) {}

void TMS::updateTemps() {
    log::LOGGER.log(log::Logger::LogLevel::DEBUG, "Updating Temps");
    for (uint8_t i = 0; i < NUM_THERMISTORS; i++) {
        if (i & 1U) {
            mux1.writePin(IO::GPIO::State::HIGH);
        } else {
            mux1.writePin(IO::GPIO::State::LOW);
        }
        if (i & 2U) {
            mux2.writePin(IO::GPIO::State::HIGH);
        } else {
            mux2.writePin(IO::GPIO::State::LOW);
        }
        thermTemps[i] = thermistor.getTempCelcius();

        thermVoltages[i] = ((uint64_t)thermistor.getRawADC()) * 3300 / 4095;
    }
}

CO_OBJ_T* TMS::getObjectDictionary() {
    return &objectDictionary[0];
}
uint16_t TMS::getObjectDictionarySize() {
    return OBJECT_DICTIONARY_SIZE;
}

}// namespace TMS
