#ifndef TMS_INCLUDE_TMS_DEV_RADIATORFAN_HPP
#define TMS_INCLUDE_TMS_DEV_RADIATORFAN_HPP

#include "EVT/io/PWM.hpp"
#include <EVT/io/GPIO.hpp>
#include <type_traits>
#include <EVT/utils/time.hpp>
#define MAX_SPEED 100

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
     * @pre gpio's direction is set to OUTPUT
     */
    RadiatorFan(IO::PWM& pwm);

    /**
     * Sets the speed based on the duty cycle of the PWM
     *
     * @param speed The desired speed of the fan, corresponding to the duty cycle
     */
    void setSpeed(uint16_t speed);

    /**
     * Stop the pump
     */
    void stop();

private:
    /** PWM instance to control the fan */
    IO::PWM& pwm;
};

}// namespace TMS

#endif//TMS_INCLUDE_TMS_DEV_RADIATORFAN_HPP
