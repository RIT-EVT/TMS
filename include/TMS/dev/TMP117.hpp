#ifndef TMS_TMP117_HPP
#define TMS_TMP117_HPP

#include <EVT/io/I2C.hpp>

#define TEMPREG 0x00

namespace IO = EVT::core::IO;

namespace TMS {

/**
 * Temp sensor for TMS
 */
class TMP117 {
public:
    /**
     * Temp sensor constructor
     *
     * @param i2c used to read temperature
     * @param deviceID - device address
     * */
    TMP117(IO::I2C& i2c, uint8_t deviceID);

    /**
     * Reads the temperature
     *
     * @return temperature reading
     */
    uint16_t readTemp();

private:
    /**register for temperature values */
    uint8_t tempReg;
    /**device ID*/
    uint8_t deviceID;
    /**i2c declaration*/
    IO::I2C& i2c;
    /**length of the temperature register in bytes*/
    uint8_t regLength = 1;
    /**length of the value being read in bytes*/
    uint8_t valueLength = 2;
    /**16-bit temp value returned from readTemp() in degrees centi celsius*/
    uint16_t temp;
};

}

#endif//TMS_TMP117_HPP
