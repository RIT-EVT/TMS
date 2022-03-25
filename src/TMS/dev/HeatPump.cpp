#include <TMS/dev/HeatPump.hpp>

namespace TMS {

HeatPump::HeatPump(IO::PWM& pwm) : pwm(pwm) {
    this->pwm.setPeriod(PERIOD);
    stop();
}

void HeatPump::setSpeed(uint8_t speed) {
    if (speed > MAX_SPEED) {
        speed = MAX_SPEED;
    } else if (speed == 0) {
        stop();
        return;
    } else if (!isInitialized && speed < MIN_INIT_SPEED) {
        // Ensures the pump initializes properly
        speed = MIN_INIT_SPEED;
    }

    pwm.setDutyCycle(SPEED_TO_DUTY_CYCLE(speed));
    isInitialized = true;
}

void HeatPump::stop() {
    pwm.setDutyCycle(STOP_DUTY_CYCLE);
    isInitialized = false;
}

}// namespace TMS
