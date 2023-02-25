
/**
 * This example shows off a basic use of the Fan with a PWM signal. You will need to use a logic
 * analyzer to see the generated square wave.
 */
#include <EVT/io/PWM.hpp>
#include <EVT/io/manager.hpp>
#include <EVT/utils/time.hpp>

#include <TMS/dev/RadiatorFan.hpp>

namespace IO = EVT::core::IO;
namespace time = EVT::core::time;

int main() {
    // Initialize system
    IO::init();

    IO::PWM& pwm = IO::getPWM<IO::Pin::PC_0>();

    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);
    TMS::RadiatorFan fan = TMS::RadiatorFan(pwm);

    // Simple code to test the fan at different speeds

    fan.setSpeed(30);
    EVT::core::time::wait(5000);
    fan.setSpeed(0);
    EVT::core::time::wait(5000);
    fan.setSpeed(20);
    EVT::core::time::wait(5000);
    fan.setSpeed(0);
    EVT::core::time::wait(5000);
    fan.setSpeed(10);

}








