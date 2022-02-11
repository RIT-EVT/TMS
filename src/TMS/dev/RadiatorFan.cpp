#include <TMS/dev/RadiatorFan.hpp>

namespace TMS {

RadiatorFan::RadiatorFan(IO::GPIO& gpio) : gpio(gpio) {
    this->powerOn(false);
}

void RadiatorFan::powerOn(bool on) {
    if (on) {
        gpio.writePin(IO::GPIO::State::HIGH);
    } else {
        gpio.writePin(IO::GPIO::State::LOW);
    }
}

}// namespace TMS
