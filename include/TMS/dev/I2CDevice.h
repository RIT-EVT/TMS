#ifndef TMS_I2CDEVICE_H
#define TMS_I2CDEVICE_H

#include <cstddef>
#include <EVT/io/I2C.hpp>

namespace IO = EVT::core::IO;
namespace I2CDevice {

class I2CDevice {
public:
    /**
     * Constructor for an I2CDevice
     *
     * @param addr Address of the device for debug purposes
     */
    I2CDevice(uint8_t addr) : addr(addr){};

    /**
     * Performs an action utilizing I2C
     *
     * @return Status of I2C call made
     */
    virtual IO::I2C::I2CStatus action() = 0;

    uint8_t getAddr() {
        return addr;
    }

private:
    uint8_t addr;
};

}

#endif//TMS_I2CDEVICE_H
