#ifndef TMS_INCLUDE_TMS_DEV_HEATPUMP_HPP
#define TMS_INCLUDE_TMS_DEV_HEATPUMP_HPP

#include <EVT/io/PWM.hpp>

// To wake the pump correctly, the initial duty cycle and frequency must have the PWM line held high
// for at least 3 ms, so this is set to 3.5 ms to ensure it doesn't fail
#define MIN_INIT_TIME .0035
#define MIN_INIT_SPEED 7
#define MAX_SPEED 100
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
     * Constructor for pump to operate with the given pwm and initial speed
     *
     * @param pwm PWM to be used to control the heat pump
     * @param initSpeed Initial speed (7-100) to start the heat pump at; must be at least 7
     * initially to ensure the init() method calculates an acceptable frequency (50-1000 Hz)
     */
    HeatPump(IO::PWM& pwm, uint8_t initSpeed);

    /**
     * Initialize the pump
     */
    void init();

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
    /** Speed to initialize the pump to */
    uint8_t initSpeed;
};

}// namespace TMS

#endif//TMS_INCLUDE_TMS_DEV_HEATPUMP_HPP
