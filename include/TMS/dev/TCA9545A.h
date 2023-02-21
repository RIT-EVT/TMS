#ifndef TMS_TCA9545A_H
#define TMS_TCA9545A_H

#include <cstddef>
#include <EVT/io/I2C.hpp>

namespace IO = EVT::core::IO;

namespace TCA9545A {

constexpr uint8_t TCA9545A_I2C_ADDR = 0x73; // Address can change based on pins

/**
 * Represents the registers for setting the active bus on the TCA9545A
 */
enum TCA9545A_BUS {
    BUS_0 = 0x01,
    BUS_1 = 0x02,
    BUS_2 = 0x04,
    BUS_3 = 0x08,
};

/**
 * TCA9545A I2C Multiplexer Driver
 * https://www.ti.com/lit/ds/symlink/tca9545a.pdf?ts=1676581408943
 */
class TCA9545A {
public:
    TCA9545A(IO::I2C& i2c);

    /**
     * Sets the active bus on the TCA9545A
     *
     * @param[in] bus The bus to set active
     * @param[in] toggled Whether to toggle the bus or not
     * @return Result of the I2C write operation
     */
    IO::I2C::I2CStatus setBus(uint8_t bus, bool toggled);

    IO::I2C::I2CStatus toggleBus(uint8_t bus);

private:
    IO::I2C& i2c;

    /**
     * Writes a value to a register on the TCA9545A
     *
     * @param[in] reg The register to write to
     * @param[in] val The value to write to the register
     * @return Result of the I2C write operation
     */
    IO::I2C::I2CStatus writeRegister(uint8_t reg, uint8_t val);

    /**
     * Reads a value from a register on the TCA9545A
     * @param[in] reg The register to read from
     * @param[out] val The value read from the register
     * @return Result of the I2C read operation
     */
    IO::I2C::I2CStatus readRegister(uint8_t reg, uint8_t* val);
};
}

#endif//TMS_TCA9545A_H
