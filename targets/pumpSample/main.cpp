#include <cstdlib>
#include <EVT/io/GPIO.hpp>
#include <EVT/io/PWM.hpp>
#include <EVT/io/manager.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/utils/time.hpp>
#include <TMS/dev/HeatPump.hpp>

namespace IO = EVT::core::IO;
namespace time = EVT::core::time;

int main() {
    IO::init();

    char buf[100];

    IO::PWM& pwm = IO::getPWM<IO::Pin::PC_0>();
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);
    TMS::HeatPump pump = TMS::HeatPump(pwm, .1);

    pump.init();

    while (1) {
        uart.printf("Enter Speed (0-1): ");
        uart.gets(buf, 100);
        uart.printf("\n\r");

        float speed = strtof(buf, nullptr);
        pump.setSpeed(speed);
    }
}
