#ifndef TMS_TMP117_HPP
#define TMS_TMP117_HPP

#include <EVT/io/I2C.hpp>

#define TEMP_REG 0x00

namespace IO = EVT::core::IO;

namespace TMS {

/**
 * Temp sensor for TMS
 * https://www.ti.com/lit/ds/symlink/tmp117.pdf
 */
class TMP117 {
public:
    /**
     * Temp sensor constructor
     *
     * @param i2c used to read temperature
     * @param i2cSlaveAddress address to ID the sensor on the I2C bus
     * */
    TMP117(IO::I2C* i2c, uint8_t i2cSlaveAddress);

    /**
     * default constructor for instantiation of uninitialized instances
     */
    TMP117();

    /**
     * Reads the temperature
     *
     * @return temperature reading in degrees centi celsius
     */
    uint16_t readTemp();

private:
    /**
     * Register for temperature values
     * TODO: Make the register input for I2C.readReg() a const pointer, so this can be const
     */
    uint8_t tempReg;

    /**
     * Device ID
     */
    uint8_t i2cSlaveAddress;

    /**
     * I2C instance
     */
    IO::I2C* i2c;
};

}// namespace TMS

#endif//TMS_TMP117_HPP
