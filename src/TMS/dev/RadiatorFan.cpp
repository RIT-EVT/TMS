#include <TMS/dev/RadiatorFan.hpp>

namespace TMS {

RadiatorFan::RadiatorFan(IO::PWM& pwm, IO::GPIO& enable, IO::GPIO& in2): pwm(pwm), enable(enable), in2(in2) {
    this->pwm.setDutyCycle(0);
    this->pwm.setPeriod(FAN_PERIOD);
    this->enable.writePin(IO::GPIO::State::LOW);

    this->in2.writePin(IO::GPIO::State::LOW);
}

void RadiatorFan::setSpeed(uint16_t speed) {
    if (speed == 0) {
        enable.writePin(EVT::core::IO::GPIO::State::LOW);
    }
    else {
        enable.writePin(EVT::core::IO::GPIO::State::HIGH);
        if (speed > MAX_SPEED) {
            speed = MAX_SPEED;
        }

        // Limit speed to 50% duty cycle
        speed /= 2;
    }

    pwm.setDutyCycle(speed);
}

}// namespace TMS
