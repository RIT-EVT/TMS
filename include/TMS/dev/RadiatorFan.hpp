#ifndef TMS_INCLUDE_TMS_DEV_RADIATORFAN_HPP
#define TMS_INCLUDE_TMS_DEV_RADIATORFAN_HPP

#include <EVT/io/GPIO.hpp>
#include <type_traits>

namespace IO = EVT::core::IO;

namespace TMS {

/**
 * Represents a radiator fan used for DEV1
 */
class RadiatorFan {
public:
    /**
     * Constructor for the radiator fan to operate with the given GPIO
     *
     * @param gpio GPIO to be used to control the fan
     * @pre gpio's direction is set to OUTPUT
     */
    RadiatorFan(IO::GPIO& gpio);

    /**
     * Turns the fan on or off
     *
     * @param on Whether the fan should be turned on or off
     */
    void powerOn(bool on);

private:
    IO::GPIO& gpio;
};

}// namespace TMS

#endif//TMS_INCLUDE_TMS_DEV_RADIATORFAN_HPP
