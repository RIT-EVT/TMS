//
// Created by Ishan on 1/30/2023.
//
#include "EVT/io/i2c.hpp"
#include "EVT/io/UART.hpp"
#include "TMS/dev/TMP117.hpp"
#include <EVT/io/manager.hpp>
#include <EVT/io/pin.hpp>

int main() {
    IO::init();

    IO::I2C& i2c = IO::getI2C<IO::Pin::PB_8, IO::Pin::PB_9>();
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);
    TMS::TMP117 tmp = TMS::TMP117(i2c);

    while(1){
        uart.printf("temp values: ");
        uart.printf("%#X\n\r", tmp.readTemp());


    }

}