
/**
 * This example shows off a basic use of the Fan with a PWM signal. You will need to use a logic
 * analyzer to see the generated square wave.
 */
#include <EVT/io/PWM.hpp>
#include <EVT/manager.hpp>
#include <EVT/utils/time.hpp>

#include <TMS/dev/RadiatorFan.hpp>

namespace IO = EVT::core::IO;
namespace time = EVT::core::time;

int main() {
    // Initialize system
    EVT::core::platform::init();

    IO::GPIO& fan1EN = IO::getGPIO<IO::Pin::PA_1>(IO::GPIO::Direction::OUTPUT);
    IO::GPIO& fan1IN2 = IO::getGPIO<IO::Pin::PB_10>(IO::GPIO::Direction::OUTPUT);
    IO::PWM& pwm = IO::getPWM<IO::Pin::PA_0>();
    TMS::RadiatorFan fan1(pwm, fan1EN, fan1IN2);

    IO::GPIO& fan2EN = IO::getGPIO<IO::Pin::PC_0>(IO::GPIO::Direction::OUTPUT);
    IO::GPIO& fan2IN2 = IO::getGPIO<IO::Pin::PC_1>(IO::GPIO::Direction::OUTPUT);
    IO::PWM& pwm2 = IO::getPWM<IO::Pin::PC_2>();
    TMS::RadiatorFan fan2(pwm2, fan2EN, fan2IN2);

    while (1) {
        fan1.setSpeed(100);
        fan2.setSpeed(100);
        time::wait(2000);

        fan1.setSpeed(0);
        fan2.setSpeed(0);
        time::wait(2000);
    }
}
