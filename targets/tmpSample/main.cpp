/**
 * tests measuring temperature with TMP117
 */

#include <EVT/io/I2C.hpp>
#include <EVT/io/UART.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/manager.hpp>
#include <dev/TMP117.hpp>

int main() {
    EVT::core::platform::init();

    IO::I2C& i2c = IO::getI2C<IO::Pin::PB_8, IO::Pin::PB_9>();
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);
    TMS::TMP117 tmp = TMS::TMP117(&i2c, 0x48);

    while (1) {
        uart.printf("temp values: ");
        uart.printf("%d\n\r", tmp.readTemp());
    }
}
