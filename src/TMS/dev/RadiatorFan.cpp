#include <TMS/dev/RadiatorFan.hpp>

namespace TMS {

RadiatorFan::RadiatorFan(IO::PWM& pwm) : pwm(pwm) {
    this->pwm.setDutyCycle(0);// setting the duty cycle to 100% to initially start the pump
    this->pwm.setPeriod(PERIOD);
}

void RadiatorFan::setSpeed(uint16_t speed) {
    if (speed > MAX_SPEED) {
        speed = MAX_SPEED;
    }
    pwm.setDutyCycle(speed);
}
}// namespace TMS
