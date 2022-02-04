#include <TMS/dev/HeatPump.hpp>

namespace TMS {

HeatPump::HeatPump(IO::PWM& Pwm, float initSpeed) : pwm(Pwm) {
    // Must be > .027778
    this->initSpeed = initSpeed;
}

void HeatPump::init() {
    float initDutyCycle = initSpeed * .72 + .13;
    pwm.setPeriod(.003 / initDutyCycle);
    pwm.setDutyCycle(initDutyCycle);
}

void HeatPump::setSpeed(float speed) {
    if (speed < 0) {
        speed = 0;
    } else if (speed > 1) {
        speed = 1;
    }

    float dutyCycle = speed * .72 + .13;
    pwm.setDutyCycle(dutyCycle);
}
}