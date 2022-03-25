#ifndef TMS_INCLUDE_TMS_DEV_HEATPUMP_HPP
#define TMS_INCLUDE_TMS_DEV_HEATPUMP_HPP

#include <EVT/io/PWM.hpp>

// Maximum period is .02, so this value is set as close to the max as can be safe, which allows the
// minimum of the init speed to be as low as possible
#define PERIOD .0195
// Value decided by the duty cycle and period to ensure the pump awakens with the appropriate high
// signal of >= 3 ms
#define MIN_INIT_SPEED 7// d = t / P = .0035 / .0195 = .179; s = (d - .13) / .0072 = 6.87 ~ 7
#define MAX_SPEED 100
#define STOP_DUTY_CYCLE .1
#define SPEED_TO_DUTY_CYCLE(speed) (speed * .0072 + .13)

namespace IO = EVT::core::IO;

namespace TMS {

/**
 * Represents the heat pump used for DEV1
 * Datasheet: https://www.tecomotive.com/en/products/CWA100-3.html
 */
class HeatPump {
public:
    /**
     * Constructor for pump to operate with the given pwm
     *
     * @param pwm PWM to be used to control the heat pump
     */
    HeatPump(IO::PWM& pwm);

    /**
     * Set the speed of the pump
     *
     * @param speed Speed (0-100) to set the pump to
     */
    void setSpeed(uint8_t speed);

    /**
     * Stop the pump
     */
    void stop();

private:
    /** PWM instance to control the pump */
    IO::PWM& pwm;
    /**
     * Whether or not the pump is initialized
     *
     * Necessary because the speed of the pump needs to be set differently depending on whether it
     * has been initialized
     */
    bool isInitialized;
};

}// namespace TMS

#endif//TMS_INCLUDE_TMS_DEV_HEATPUMP_HPP
