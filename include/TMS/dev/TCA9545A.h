#ifndef TMS_TCA9545A_H
#define TMS_TCA9545A_H

#include <EVT/io/I2C.hpp>

namespace IO = EVT::core::IO;

namespace TCA9545A {

constexpr uint8_t TCA9545A_I2C_ADDR = 0x00;

enum TCA9545A_BUS {
    BUS_0 = 0x01,
    BUS_1 = 0x02,
    BUS_2 = 0x04,
    BUS_3 = 0x08,

};

class TCA9545A {
public:
    TCA9545A(IO::I2C& i2c);

    IO::I2C::I2CStatus setBus(uint8_t bus, bool toggled);

    IO::I2C::I2CStatus toggleBus(uint8_t bus);

private:
    IO::I2C& i2c;

    IO::I2C::I2CStatus writeRegister(uint8_t reg, uint8_t val);

    IO::I2C::I2CStatus readRegister();
};
}

#endif//TMS_TCA9545A_H
