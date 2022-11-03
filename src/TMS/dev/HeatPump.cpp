#include <TMS/dev/HeatPump.hpp>
#include <EVT/utils/time.hpp>

namespace time = EVT::core::time;


namespace TMS {

HeatPump::HeatPump(IO::PWM& pwm) : pwm(pwm) {
    this->pwm.setDutyCycle(100); // setting the duty cycle to 100% to initially start the pump
    time::wait(3); // turning on the pump for 3 milliseconds (must do according to data sheet)
    this->pwm.setPeriod(PERIOD);
    stop();
}

void HeatPump::setSpeed(uint16_t speed) {
    if (speed > MAX_SPEED) {
        speed = MAX_SPEED;
    } else if (speed == 0) {
        stop();
        return;
    }

    pwm.setDutyCycle(SPEED_TO_DUTY_CYCLE(speed));
    isInitialized = true;
}

void HeatPump::stop() {
    pwm.setDutyCycle(STOP_DUTY_CYCLE);
    isInitialized = false;
}

}// namespace TMS
