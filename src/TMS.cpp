#include <TMS.hpp>

namespace TMS {

uint16_t TMS::sensorTemps[NUM_TEMP_SENSORS] = {};

TMS::TMS(TCA9545A& tca9545A, HeatPump pump, RadiatorFan fans[2]) : tca9545A(tca9545A), pump(pump),
                                                                   fans{fans[0], fans[1]} {}

CO_OBJ_T* TMS::getObjectDictionary() {
    return &objectDictionary[0];
}
uint8_t TMS::getNumElements() {
    return OBJECT_DICTIONARY_SIZE;
}
uint8_t TMS::getNodeID() {
    return TMS::NODE_ID;
}

void TMS::process() {
    log::LOGGER.log(log::Logger::LogLevel::DEBUG, "Updating Temps");
    tca9545A.pollDevices();

    switch (mode) {
    // Auxiliary Mode
    case CO_PREOP:
        // Turn the pump and fans off
        pump.stop();
        for (RadiatorFan fan : fans) {
            fan.setSpeed(0);
        }

        break;
    // Operational Mode
    case CO_OPERATIONAL:
        // Set the cooling controls
        applyThermalModel();

        break;
    default:
        log::LOGGER.log(log::Logger::LogLevel::ERROR, "Network Management state is not valid.");
    }
}

void TMS::setMode(CO_MODE newMode) {
    mode = newMode;
}

void TMS::applyThermalModel() {
    // Initialize the input struct
    DEV1ThermalModel::ExtU_DEV1ThermalModel_T inputs = {
        .temp1 = sensorTemps[1],
        .temp2 = sensorTemps[2],
        .boardTemp = sensorTemps[0],
    };

    // Run the model
    thermalModel.setExternalInputs(&inputs);
    thermalModel.step();
    DEV1ThermalModel::ExtY_DEV1ThermalModel_T outputs = thermalModel.getExternalOutputs();

    // Extract the outputs
    pumpSpeed = outputs.pumpSpeed;
    fan1Speed = outputs.fanSpeed1;
    fan2Speed = outputs.fanSpeed2;

    // Command devices to execute the control policy
    pump.setSpeed(pumpSpeed);
    fans[0].setSpeed(fan1Speed);
    //fans[1].setSpeed(fan2Speed);
}

}// namespace TMS
