/**
 * Tests the HeatPump class by allowing the user to input values via UART to set the pump speed.
 */
#include <EVT/io/GPIO.hpp>
#include <EVT/io/PWM.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/manager.hpp>
#include <EVT/utils/time.hpp>
#include <cstdlib>
#include <dev/HeatPump.hpp>

namespace IO = EVT::core::IO;
namespace time = EVT::core::time;

int main() {
    EVT::core::platform::init();

    char buf[100];

    IO::PWM& pwm = IO::getPWM<IO::Pin::PA_6>();
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);
    TMS::HeatPump pump = TMS::HeatPump(pwm);

    while (1) {
        uart.printf("Enter Speed (0-100): ");
        uart.gets(buf, 100);
        uart.printf("\n\r");

        uint8_t speed = strtol(buf, nullptr, 10);
        pump.setSpeed(speed);
    }
}
