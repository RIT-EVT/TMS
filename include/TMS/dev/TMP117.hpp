//
// Created by Ishan on 1/23/2023.
//

#include "EVT/io/I2C.hpp"

#ifndef TMS_TMP117_HPP
#define TMS_TMP117_HPP
#endif//TMS_TMP117_HPP

namespace IO = EVT::core::IO;

namespace TMS {

class TMP117 {
public:
    TMP117(IO::I2C& i2c, uint8_t deviceID);
    uint16_t readTemp();

private:
    constexpr static uint8_t* tempReg = 0x00;
    uint8_t deviceID;
    IO::I2C& i2c;
    uint8_t regLength = 1;
    uint8_t valueLength = 2;
    uint16_t temp;
};
}
