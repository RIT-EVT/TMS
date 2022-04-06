/**
* Tests iterating over the MUX to get thermistor temperatures
*/
#include <EVT/io/GPIO.hpp>
#include <EVT/io/manager.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/utils/time.hpp>
#include <TMS/TMS.hpp>
#include <cstdlib>

namespace IO = EVT::core::IO;
namespace time = EVT::core::time;

static uint32_t conversion(uint32_t adcInput) {
    auto adc64 = (uint64_t) adcInput;
    auto outTemp = (uint32_t) ((adc64 * adc64 * adc64) / 1000000000 * 9604
                               - (adc64 * adc64) / 100000 * 4502
                               + adc64 * 9774 / 100
                               - 7374);
    log::LOGGER.log(log::Logger::LogLevel::DEBUG, "  -  Temp: %08x", outTemp);

    return outTemp;
}

int main() {
   IO::init();

   char buf[100];

   IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

   IO::GPIO* mux1 = &IO::getGPIO<IO::Pin::PB_2>();
   IO::GPIO* mux2 = &IO::getGPIO<IO::Pin::PB_8>();
   IO::GPIO* mux3 = &IO::getGPIO<IO::Pin::PB_9>();


   // Set up TMS and necessary device drivers
   DEV::Thermistor therm = DEV::Thermistor(IO::getADC<IO::Pin::PA_4>(), conversion);

   uint8_t thermNumber = 0;
   while (1) {
       if (thermNumber & 1U) {
           mux1->writePin(IO::GPIO::State::HIGH);
       } else {
           mux1->writePin(IO::GPIO::State::LOW);
       }
       if (thermNumber & 2U) {
           mux2->writePin(IO::GPIO::State::HIGH);
       } else {
           mux2->writePin(IO::GPIO::State::LOW);
       }
       if (thermNumber & 4U) {
           mux3->writePin(IO::GPIO::State::HIGH);
       } else {
           mux3->writePin(IO::GPIO::State::LOW);
       }

       uart.printf("Temp value: %d", therm.getTempCelcius());

       uart.printf("Press enter to move to the next thermistor.");
       uart.gets(buf, 100);
       uart.printf("\n\r");
       thermNumber = (thermNumber + 1) % 8;
   }
}
