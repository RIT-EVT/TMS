//
// Created by Ishan on 1/23/2023.
//

#include <TMS/dev/TMP117.hpp>
#include "EVT/io/I2C.hpp"

namespace TMS {

TMP117::TMP117(IO::I2C& i2c): i2c(i2c){

};

uint16_t TMP117::readTemp() {
    i2c.readReg(deviceID, tempReg, regLength, tempBytes, valueLength);

    byte1 = tempBytes[1];
    byte1<<=8;


    return byte1|tempBytes[0];
//    return byte1;
}

}