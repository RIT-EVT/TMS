#include <EVT/io/I2C.hpp>
#include <TMS/dev/TMP117.hpp>

namespace TMS {

TMP117::TMP117(IO::I2C* i2c, uint8_t i2cSlaveAddress) : i2cSlaveAddress(i2cSlaveAddress), i2c(i2c) {
    tempReg = TEMP_REG;
}

TMP117::TMP117() : i2cSlaveAddress(0), i2c(nullptr) {}

uint16_t TMP117::readTemp() {
    uint16_t input;
    uint8_t tempBytes[2];
    uint16_t temp;

    i2c->readReg(i2cSlaveAddress, &tempReg, 1, tempBytes, 2);

    input = ((uint16_t) tempBytes[0]) << 8 | tempBytes[1];

    /**
     * degrees centi celsius
     * multiplied by 78125 because the sensor output increases by .0078125 degrees celsius - brings it to fixed point within 64 bits
     * divided by 100000 so it fits in a 16 bit int
     */
    temp = ((uint64_t) input) * 78125 / 100000;

    return temp;
}
}// namespace TMS
