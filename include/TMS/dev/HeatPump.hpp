#ifndef TMS_INCLUDE_TMS_DEV_HEATPUMP_HPP
#define TMS_INCLUDE_TMS_DEV_HEATPUMP_HPP

#include <EVT/io/PWM.hpp>

#define MIN_INIT_SPEED 7
#define MAX_SPEED 100
#define MIN_INIT_TIME .0035
#define SPEED_TO_DUTY_CYCLE(speed) (speed * .0072 + .13)

namespace IO = EVT::core::IO;

namespace TMS {

class HeatPump {
public:
    HeatPump(IO::PWM& pwm, uint8_t initSpeed);

    void init();

    void setSpeed(uint8_t speed);

    void stop();

private:
    IO::PWM& pwm;
    uint8_t initSpeed;
};

}// namespace TMS

#endif//TMS_INCLUDE_TMS_DEV_HEATPUMP_HPP
