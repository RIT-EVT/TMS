//
// Created by Ishan on 1/23/2023.
//

#include <TMS/dev/TMP117.hpp>
#include <EVT/io/I2C.hpp>

namespace TMS {

TMP117::TMP117(IO::I2C& i2c, uint8_t deviceID): deviceID(deviceID), i2c(i2c) { }

uint16_t TMP117::readTemp() {
    uint16_t input;
    uint8_t tempBytes[2];
    uint16_t byte1;

    i2c.readReg(deviceID, tempReg, regLength, tempBytes, valueLength);

    byte1 = tempBytes[0];
    byte1<<=8;
    input = byte1|tempBytes[1];

    //degrees centi celsius???
    temp = ((uint64_t)input)*78125/100000;

    return temp;
}

}