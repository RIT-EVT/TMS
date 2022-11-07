#ifndef TMS_INCLUDE_TMS_DEV_HEATPUMP_HPP
#define TMS_INCLUDE_TMS_DEV_HEATPUMP_HPP

#include <EVT/io/PWM.hpp>
#include <EVT/utils/time.hpp>

// We want to maximize frequency, the max frequency can be 950 HZ.
// NOTE: Datasheet says 1000 Hz but I have put it down 50 Hz for some errors that can occur will running at max
// To achieve this period must be T = 1/950 seconds which is (~ 1053 microseconds or 1.053 ms)
#define PERIOD 1053
#define MAX_SPEED 100
#define STOP_DUTY_CYCLE 10
#define SPEED_TO_DUTY_CYCLE(speed) ((speed * 72 / 100) + 13)//d = (85 - 13)(s / 100) + 13

namespace IO = EVT::core::IO;
namespace time = EVT::core::time;


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
    void setSpeed(uint16_t speed);

    /**
     * Stop the pump
     */
    void stop();

private:
    /** PWM instance to control the pump */
    IO::PWM& pwm;
};

}// namespace TMS

#endif//TMS_INCLUDE_TMS_DEV_HEATPUMP_HPP
