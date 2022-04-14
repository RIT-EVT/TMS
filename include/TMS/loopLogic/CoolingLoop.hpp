

#ifndef TMS_INCLUDE_TMS_LOOPLOGIC_COOLINGLOOP_HPP
#define TMS_INCLUDE_TMS_LOOPLOGIC_COOLINGLOOP_HPP

#include <cstdint>


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
};

}

#endif//TMS_INCLUDE_TMS_LOOPLOGIC_COOLINGLOOP_HPP
