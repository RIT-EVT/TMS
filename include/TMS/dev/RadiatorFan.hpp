#ifndef TMS_INCLUDE_TMS_DEV_RADIATORFAN_HPP
#define TMS_INCLUDE_TMS_DEV_RADIATORFAN_HPP

#include "EVT/io/PWM.hpp"
#include <EVT/io/GPIO.hpp>
#include <EVT/utils/time.hpp>
#include <type_traits>
#define MAX_SPEED 100
#define PERIOD 30000

namespace IO = EVT::core::IO;
namespace time = EVT::core::time;

namespace TMS {

/**
 * Represents a radiator fan used for DEV1
 */
class RadiatorFan {
public:
    /**
     * Constructor for the radiator fan to operate with the given GPIO
     *
     * @param pwm PWM instance to control the fan
     */
    RadiatorFan(IO::PWM& pwm);

    /**
     * Sets the speed based on the duty cycle of the PWM
     *
     * @param speed The desired speed of the fan, corresponding to the duty cycle
     */
    void setSpeed(uint16_t speed);

private:
    /** PWM instance to control the fan */
    IO::PWM& pwm;
};

}// namespace TMS

#endif//TMS_INCLUDE_TMS_DEV_RADIATORFAN_HPP
