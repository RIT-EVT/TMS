#include <TMS/dev/HeatPump.hpp>

namespace TMS {

HeatPump::HeatPump(IO::PWM& pwm, uint8_t initSpeed) : pwm(pwm) {
    // Must be >= 7 to ensure acceptable frequency
    if (initSpeed < MIN_INIT_SPEED) {
        initSpeed = MIN_INIT_SPEED;
    } else if (initSpeed > MAX_SPEED) {
        initSpeed = MAX_SPEED;
    }
    this->initSpeed = initSpeed;
}

void HeatPump::init() {
    float initDutyCycle = SPEED_TO_DUTY_CYCLE(initSpeed);
    pwm.setPeriod(MIN_INIT_TIME / initDutyCycle);
    pwm.setDutyCycle(initDutyCycle);
}

void HeatPump::setSpeed(uint8_t speed) {
    if (speed > MAX_SPEED) {
        speed = MAX_SPEED;
    }

    float dutyCycle = SPEED_TO_DUTY_CYCLE(speed);
    pwm.setDutyCycle(dutyCycle);
}

void HeatPump::stop() {
    pwm.setDutyCycle(0);
}

}// namespace TMS
