#include <TMS/dev/RadiatorFan.hpp>

namespace TMS {

RadiatorFan::RadiatorFan(IO::PWM& pwm): pwm(pwm) {
    this->pwm.setDutyCycle(0);
    this->pwm.setPeriod(PERIOD);
}

    void RadiatorFan::setSpeed(uint16_t speed) {
        if (speed == 0){
            gpio.writePin(EVT::core::IO::GPIO::State::LOW);
        }
        else{
            gpio.writePin(EVT::core::IO::GPIO::State::HIGH);
            if (speed > MAX_SPEED) {
                speed = MAX_SPEED;
            }
            pwm.setDutyCycle(speed);
            }
    }

    }// namespace TMS
