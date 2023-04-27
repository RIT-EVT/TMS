#ifndef TMS_I2CDEVICE_HPP
#define TMS_I2CDEVICE_HPP

#include <cstddef>
#include <EVT/io/I2C.hpp>

namespace IO = EVT::core::IO;
namespace TMS {

/**
 * Interface for devices using I2C to call a common function
 */
class I2CDevice {
public:
    /**
     * Performs an action utilizing I2C
     *
     * @return Status of I2C call made
     */
    virtual IO::I2C::I2CStatus action() = 0;
};

}

#endif//TMS_I2CDEVICE_HPP
