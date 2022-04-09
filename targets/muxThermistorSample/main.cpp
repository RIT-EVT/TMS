/**
* Tests iterating over the MUX to get thermistor temperatures
*/
#include <EVT/io/GPIO.hpp>
#include <EVT/io/manager.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/utils/time.hpp>
#include <TMS/TMS.hpp>
#include <EVT/io/ADC.hpp>
#include <cstdlib>
#include <EVT/utils/time.hpp>
#include <EVT/io/PWM.hpp>

namespace IO = EVT::core::IO;
namespace time = EVT::core::time;

static uint32_t conversion(uint32_t adcInput) {
    return adcInput;
}

int main() {
   IO::init();

   char buf[100];

   IO::UART& uart = IO::getUART<IO::Pin::PB_10, IO::Pin::PB_11>(9600);

   IO::GPIO* mux1 = &IO::getGPIO<IO::Pin::PB_2>();
   IO::GPIO* mux2 = &IO::getGPIO<IO::Pin::PB_8>();

   IO::GPIO& fanCntrl = IO::getGPIO<IO::Pin::PC_14>();


   IO::ADC& adc = IO::getADC<IO::Pin::PA_4>();

   IO::PWM& pwm = IO::getPWM<IO::Pin::PB_14>();
   pwm.setPeriod(10000);
   pwm.setDutyCycle(50);

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

       fanCntrl.writePin(IO::GPIO::State::HIGH);
       time::wait(500);
       fanCntrl.writePin(IO::GPIO::State::LOW);
       uart.printf("Temp value for %d: %dmv\n\r", thermNumber,
                                                  (int)(adc.read() * 1000));

       uart.printf("Press enter to move to the next thermistor.\n\r");
       uart.gets(buf, 100);
       uart.printf("\n\r");
       thermNumber = (thermNumber + 1) % 4;
   }
}
