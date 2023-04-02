/**
 * Sample used to show the use of the TCA9545A I2C Multiplexer
 */

#include <EVT/io/I2C.hpp>
#include <EVT/io/UART.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/manager.hpp>
#include <TMS/dev/TMP117.hpp>
#include <TMS/dev/TCA9545A.h>
#include <TMS/dev/TMP117I2CDevice.hpp>

namespace time = EVT::core::time;

int main() {
    EVT::core::platform::init();

    IO::I2C& i2c = IO::getI2C<IO::Pin::PB_8, IO::Pin::PB_9>();
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    uint16_t tempValue[4] = { };

    TMS::TMP117 tmp0 = TMS::TMP117(&i2c, 0x48);
    TMS::TMP117 tmp1 = TMS::TMP117(&i2c, 0x49);
    TMS::TMP117 tmp2 = TMS::TMP117(&i2c, 0x4A);
    TMS::TMP117 tmp3 = TMS::TMP117(&i2c, 0x4B);

    TMS::TMP117I2CDevice dev0(&tmp0, &tempValue[0]);
    TMS::TMP117I2CDevice dev1(&tmp1, &tempValue[1]);
    TMS::TMP117I2CDevice dev2(&tmp2, &tempValue[2]);
    TMS::TMP117I2CDevice dev3(&tmp3, &tempValue[3]);

    uint8_t numDevices[4] = { 0, 3, 1, 0 };
    TMS::I2CDevice* bus0[0] = {  };
    TMS::I2CDevice* bus1[3] = { &dev1, &dev2, &dev3 };
    TMS::I2CDevice* bus2[1] = { &dev0 };
    TMS::I2CDevice* bus3[0] = {  };
    TMS::I2CDevice** buses[4] = {bus0, bus1, bus2, bus3};

    TMS::TCA9545A tca(i2c, 0x70, reinterpret_cast<TMS::I2CDevice***>(buses), numDevices);

    while (1) {
        tca.pollDevices();
        uart.printf("temp values: \n\r");
        uart.printf("0x48: %d\n\r", tempValue[0]);
        uart.printf("0x49: %d\n\r", tempValue[1]);
        uart.printf("0x4A: %d\n\r", tempValue[2]);
        uart.printf("0x4B: %d\n\r", tempValue[3]);
        time::wait(500);
    }
}
