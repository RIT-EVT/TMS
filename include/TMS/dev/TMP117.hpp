#ifndef TMS_TMP117_HPP
#define TMS_TMP117_HPP

#include <EVT/io/I2C.hpp>

#define TEMPREG 0x00

namespace IO = EVT::core::IO;

namespace TMS {

/**
 * Temp sensor for TMS on DEV1
 */
class TMP117 {
public:
    /**Temp sensor constructor
     * @param i2c used to read temperature
     * @param deviceID - device address
     * */
    TMP117(IO::I2C& i2c, uint8_t deviceID);
    /**reads the temperature*/
    uint16_t readTemp();

private:
    uint8_t tempReg;
    uint8_t deviceID;
    IO::I2C& i2c;
    uint8_t regLength = 1;
    uint8_t valueLength = 2;
    uint16_t temp;
};
}

#endif//TMS_TMP117_HPP
