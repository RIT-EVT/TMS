#ifndef TMS_TCA9545A_HPP
#define TMS_TCA9545A_HPP

#include <EVT/io/I2C.hpp>
#include <TMS/dev/I2CDevice.h>
#include <cstddef>

namespace IO = EVT::core::IO;

namespace TMS {

constexpr uint8_t TCA9545A_DEFAULT_I2C_ADDR = 0x73;

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
 * https://www.ti.com/lit/ds/symlink/tca9545a.pdf
 */
class TCA9545A {
public:
    /**
     * Constructor for the TCA9545A driver
     *
     * @param[in] i2c I2C instance for communicating with TCA
     * @param[in] addr address of TCA
     * @param[in] buses array of buses containing I2CDevices
     */
    TCA9545A(IO::I2C& i2c, uint8_t addr = TCA9545A_DEFAULT_I2C_ADDR,
             I2CDevice** buses[4] = nullptr, uint8_t numDevices[4] = nullptr);

    /**
     * Sets the active bus on the TCA9545A
     *
     * @param[in] bus The bus to set active
     * @param[in] toggled Whether to toggle the bus or not
     * @return Result of the I2C write operation
     */
    IO::I2C::I2CStatus setBus(uint8_t bus, bool toggled);

    /**
     * Runs all actions on all I2CDevices stored in the bus
     */
    void pollDevices();

private:
    static constexpr uint8_t I2C_MUX_BUS_SIZE = 4;
    I2CDevice** busDevices[I2C_MUX_BUS_SIZE];
    IO::I2C& i2c;
    uint8_t numDevices[4];
    uint8_t slaveAddress;

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
     *
     * @param[in] reg The register to read from
     * @param[out] val The value read from the register
     * @return Result of the I2C read operation
     */
    IO::I2C::I2CStatus readRegister(uint8_t reg, uint8_t* val);
};

}// namespace TMS

#endif//TMS_TCA9545A_HPP
