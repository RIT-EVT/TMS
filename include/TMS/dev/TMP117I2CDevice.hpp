#ifndef TMS_TMP117I2CDEVICE_HPP
#define TMS_TMP117I2CDEVICE_HPP
#include <TMS/dev/I2CDevice.h>
#include <EVT/io/I2C.hpp>
#include <TMS/dev/TMP117.hpp>

namespace TMS {

class TMP117I2CDevice : I2CDevice {
    public:
        /**
         * second constructor to make TMS main work
         */
        TMP117I2CDevice();
        /**
         * TMP117 constructor
         * @param tmp117 pointer to tmp object
         * @param tempValue where the temp value is stored
         */
        TMP117I2CDevice(TMS::TMP117* tmp117, uint16_t* tempValue);
        /**
         * returns I2CStatus of TMP readTemp()
         * @return I2CStatus
         */
        IO::I2C::I2CStatus action() override;
    private:
        TMP117* tmp117;
        uint16_t* tempValue;
};

}// namespace TMS

#endif//TMS_TMP117I2CDEVICE_HPP
