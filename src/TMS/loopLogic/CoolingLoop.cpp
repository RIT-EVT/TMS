

#include <TMS/loopLogic/CoolingLoop.hpp>

TMS::CoolingLoop::CoolingLoop(uint32_t& tempAfterInverter, uint32_t& tempAfterMotor,
                              uint32_t& tempRadiator1, uint32_t& tempRadiator2, uint32_t& throttle,
                              uint16_t& pumpSpeed, bool& fanControl) : tempAfterInverter(tempAfterInverter),
                                                                       tempAfterMotor(tempAfterMotor),
                                                                       tempRadiator1(tempRadiator1),
                                                                       tempRadiator2(tempRadiator2),
                                                                       throttle(throttle),
                                                                       pumpSpeed(pumpSpeed),
                                                                       fanControl(fanControl) {
    tempIntegrator = 0;
}

void TMS::CoolingLoop::updateLoop() {
    if (tempRadiator2 - tempRadiator2 >= 10000 && tempAfterMotor >= 40) {
        fanControl = true;
    }
}
