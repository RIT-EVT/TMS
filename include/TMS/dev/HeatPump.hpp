#ifndef TMS_INCLUDE_TMS_DEV_HEATPUMP_HPP
#define TMS_INCLUDE_TMS_DEV_HEATPUMP_HPP

#include <EVT/io/PWM.hpp>

namespace IO = EVT::core::IO;

namespace TMS {

class HeatPump {
public:
    HeatPump(IO::PWM& Pwm, float initSpeed);

    void init();

    void setSpeed(float speed);

private:
    IO::PWM& pwm;
    double initSpeed;
};

}

#endif //TMS_INCLUDE_TMS_DEV_HEATPUMP_HPP
