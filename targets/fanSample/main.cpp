/**
 * Tests the RadiatorFan class by allowing the user to turn the fan on and off using UART.
 */
#include <EVT/io/GPIO.hpp>
#include <EVT/io/PWM.hpp>
#include <EVT/io/manager.hpp>
#include <TMS/dev/RadiatorFan.hpp>

namespace IO = EVT::core::IO;

int main() {
    IO::init();

    char buf[100];

    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);
    TMS::RadiatorFan fan = TMS::RadiatorFan(IO::getGPIO<IO::Pin::PC_0>());

    while (1) {
        uart.printf("Enter 0 to turn the fan off or 1 to turn it on: ");
        uart.gets(buf, 100);
        uart.printf("\n\r");

        bool on = strtol(buf, nullptr, 10);
        fan.powerOn(on);
    }
}
