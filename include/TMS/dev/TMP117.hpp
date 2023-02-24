#include <EVT/io/I2C.hpp>
#ifndef TMS_TMP117_HPP
#define TMS_TMP117_HPP

namespace IO = EVT::core::IO;

namespace TMS {

/**class for TMP117 driver*/
class TMP117 {
public:
    /**temp sensor*/
    TMP117(IO::I2C& i2c, uint8_t deviceID);
    /**reads the temperature*/
    uint16_t readTemp();

private:
    constexpr static uint8_t tempReg = 0x00;
    uint8_t deviceID;
    IO::I2C& i2c;
    uint8_t regLength = 1;
    uint8_t valueLength = 2;
    uint16_t temp;
};
}

#endif//TMS_TMP117_HPP
