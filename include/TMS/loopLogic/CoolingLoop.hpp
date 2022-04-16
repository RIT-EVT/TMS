

#ifndef TMS_INCLUDE_TMS_LOOPLOGIC_COOLINGLOOP_HPP
#define TMS_INCLUDE_TMS_LOOPLOGIC_COOLINGLOOP_HPP

#include <EVT/dev/RTCTimer.hpp>
#include <EVT/dev/platform/f3xx/f302x8/RTC302x8.hpp>
#include <cstdint>

#define TIME_DELAY 1000
#define INTEGRATOR_INCREMENT 100000
#define INTEGRATOR_MAX (INTEGRATOR_INCREMENT * 200)
#define INTEGRATOR_MIN (-INTEGRATOR_INCREMENT * 50)
#define TARGET_TEMP 30000

namespace DEV = EVT::core::DEV;
namespace time = EVT::core::time;

namespace TMS {

class CoolingLoop {
public:
    typedef struct LoopInput_S {
        uint32_t tempAfterInverter;
        uint32_t tempAfterMotor;
        uint32_t tempRadiator1;
        uint32_t tempRadiator2;
        uint32_t throttle;
    } LoopInput;

    typedef struct LoopOutput_S {
        uint16_t pumpSpeed;
        bool fanControl;
    } LoopOutput;

    CoolingLoop(uint32_t& tempAfterInverter, uint32_t& tempAfterMotor, uint32_t& tempRadiator1,
                uint32_t& tempRadiator2, uint32_t& throttle, uint16_t& pumpSpeed, bool& fanControl);

    void updateLoop();

private:
    uint32_t& tempAfterInverter;
    uint32_t& tempAfterMotor;
    uint32_t& tempRadiator1;
    uint32_t& tempRadiator2;
    uint32_t& throttle;
    uint16_t& pumpSpeed;
    bool& fanControl;

    int32_t tempIntegrator;
    uint32_t lastAttemptTime;
};

}// namespace TMS

#endif//TMS_INCLUDE_TMS_LOOPLOGIC_COOLINGLOOP_HPP
