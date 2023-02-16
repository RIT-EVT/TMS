#include <TMS/dev/TCA9545A.h>

namespace TCA9545A {

TCA9545A::TCA9545A(IO::I2C& i2c) : i2c(i2c) {};

IO::I2C::I2CStatus TCA9545A::setBus(uint8_t bus, bool toggled) {
    IO::I2C::I2CStatus ret;
    uint8_t val = toggled ? 1 : 0;
    switch (bus) {
    case 0:
        ret = writeRegister(TCA9545A_BUS::BUS_0, val);
        break;

    case 1:
        ret = writeRegister(TCA9545A_BUS::BUS_1, val);
        break;

    case 2:
        ret = writeRegister(TCA9545A_BUS::BUS_2, val);
        break;

    case 3:
        ret = writeRegister(TCA9545A_BUS::BUS_3, val);
        break;
    default:
        return IO::I2C::I2CStatus::ERROR;
    }

    return ret;
}

IO::I2C::I2CStatus TCA9545A::writeRegister(uint8_t reg, uint8_t val) {
    return this->i2c.writeReg(TCA9545A_I2C_ADDR, reg, val);
}

IO::I2C::I2CStatus TCA9545A::readRegister(uint8_t reg, uint8_t* val) {
    return this->i2c.readReg(TCA9545A_I2C_ADDR, reg, val);
}
} // namespace TCA9545A