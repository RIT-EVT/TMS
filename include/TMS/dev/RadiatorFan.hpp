#ifndef TMS_INCLUDE_TMS_DEV_RADIATORFAN_HPP
#define TMS_INCLUDE_TMS_DEV_RADIATORFAN_HPP

#include <EVT/io/GPIO.hpp>
#include <type_traits>

namespace IO = EVT::core::IO;

namespace TMS {

class RadiatorFan {
public:
    RadiatorFan(IO::GPIO& gpio);

    void powerOn(bool on);

private:
    IO::GPIO& gpio;
};

}// namespace TMS

#endif//TMS_INCLUDE_TMS_DEV_RADIATORFAN_HPP
