#ifndef TMS_INCLUDE_TMS_DEV_RADIATORFAN_HPP
#define TMS_INCLUDE_TMS_DEV_RADIATORFAN_HPP

#include <EVT/io/GPIO.hpp>
#include <EVT/io/PWM.hpp>
#include <EVT/utils/time.hpp>
#include <type_traits>

#define MAX_SPEED 100
#define FAN_PERIOD 100

namespace IO = EVT::core::IO;
namespace time = EVT::core::time;

namespace TMS {

/**
 * Represents a radiator fan controlled with PWM using and H Bridge
 * Datasheet: https://www.monolithicpower.com/en/documentview/productdocument/index/version/2/document_type/Datasheet/lang/en/sku/MP6551GQB/document_id/9795/
 */
class RadiatorFan {
public:
    /**
     * Constructor for the radiator fan to operate with the given GPIO
     *
     * @param pwm PWM instance to control the fan
     * @param enable Enable pin to turn the fan on and off
     * @param in2 Second input pin, determines direction of current on output
     */
    RadiatorFan(IO::PWM& pwm, IO::GPIO& enable, IO::GPIO& in2);

    /**
     * Sets the speed based on the duty cycle of the PWM
     *
     * @param speed The desired speed of the fan, corresponding to the duty cycle
     */
    void setSpeed(uint16_t speed);

private:
    /**
     * PWM instance to control the fan
     */
    IO::PWM& pwm;

    /**
     * GPIO instance to turn on the fan
     */
    IO::GPIO& enable;

    /**
     * Second input pin, determines direction of current on output
     */
    IO::GPIO& in2;
};

}// namespace TMS

#endif//TMS_INCLUDE_TMS_DEV_RADIATORFAN_HPP
