#include <TMS/dev/TCA9545A.h>

namespace TCA9545A {

TCA9545A::TCA9545A(IO::I2C& i2c) : i2c(i2c) {};

IO::I2C::I2CStatus TCA9545A::setBus(uint8_t bus, bool toggled) {
    uint8_t val = static_cast<uint8_t>(toggled);
    switch (bus) {
    case 0:
        return writeRegister(TCA9545A_BUS::BUS_0, val);
    case 1:
        return writeRegister(TCA9545A_BUS::BUS_1, val);
    case 2:
        return writeRegister(TCA9545A_BUS::BUS_2, val);
    case 3:
        return writeRegister(TCA9545A_BUS::BUS_3, val);
    default:
        return IO::I2C::I2CStatus::ERROR;
    }
}

IO::I2C::I2CStatus TCA9545A::writeRegister(uint8_t reg, uint8_t val) {
    return i2c.writeReg(TCA9545A_I2C_ADDR, reg, val);
}

IO::I2C::I2CStatus TCA9545A::readRegister(uint8_t reg, uint8_t* val) {
    return i2c.readReg(TCA9545A_I2C_ADDR, reg, val);
}
} // namespace TCA9545A
