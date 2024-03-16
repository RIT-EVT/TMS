//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: DEV1ThermalModel.cpp
//
// Code generated for Simulink model 'DEV1ThermalModel'.
//
// Model version                  : 1.11
// Simulink Coder version         : 23.2 (R2023b) 01-Aug-2023
// C/C++ source code generated on : Sat Mar 16 18:41:35 2024
//
// Target selection: ert.tlc
// Embedded hardware selection: STMicroelectronics->ST10/Super10
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include <models/DEV1ThermalModel.h>
#include <stdint.h>

// Named constants for Chart: '<Root>/Thermal Model Stateflow'
const uint8_t DEV1ThermalM_IN_COOL_LEVEL_IDLE{ 3U };

const uint8_t DEV1ThermalMo_IN_COOL_LEVEL_MAX{ 4U };

const uint8_t DEV1ThermalMode_IN_COOL_LEVEL_1{ 1U };

const uint8_t DEV1ThermalMode_IN_COOL_LEVEL_2{ 2U };

namespace TMS
{
    // Model step function
    void DEV1ThermalModel::step()
    {
        uint16_t rtb_Max;

        // MinMax: '<Root>/Max' incorporates:
        //   Inport: '<Root>/temp1'
        //   Inport: '<Root>/temp2'

        if (DEV1ThermalModel_U.temp1 >= DEV1ThermalModel_U.temp2) {
            rtb_Max = DEV1ThermalModel_U.temp1;
        } else {
            rtb_Max = DEV1ThermalModel_U.temp2;
        }

        // End of MinMax: '<Root>/Max'

        // Chart: '<Root>/Thermal Model Stateflow'
        // Gateway: Thermal Model Stateflow
        // During: Thermal Model Stateflow
        if (static_cast<uint16_t>
                (DEV1ThermalModel_DW.is_active_c3_DEV1ThermalModel) == 0U) {
            // Entry: Thermal Model Stateflow
            DEV1ThermalModel_DW.is_active_c3_DEV1ThermalModel = 1U;

            // Entry Internal: Thermal Model Stateflow
            // Transition: '<S1>:13'
            DEV1ThermalModel_DW.is_c3_DEV1ThermalModel =
                DEV1ThermalM_IN_COOL_LEVEL_IDLE;

            // Outport: '<Root>/pumpSpeed'
            // Entry 'COOL_LEVEL_IDLE': '<S1>:8'
            DEV1ThermalModel_Y.pumpSpeed = 50U;
            DEV1ThermalModel_Y.fanSpeed1 = 0U;
        } else {
            switch (DEV1ThermalModel_DW.is_c3_DEV1ThermalModel) {
              case DEV1ThermalMode_IN_COOL_LEVEL_1:
                // During 'COOL_LEVEL_1': '<S1>:6'
                if (rtb_Max > 5000U) {
                    // Transition: '<S1>:26'
                    DEV1ThermalModel_DW.is_c3_DEV1ThermalModel =
                        DEV1ThermalMo_IN_COOL_LEVEL_MAX;

                    // Outport: '<Root>/pumpSpeed'
                    // Entry 'COOL_LEVEL_MAX': '<S1>:5'
                    DEV1ThermalModel_Y.pumpSpeed = 100U;
                    DEV1ThermalModel_Y.fanSpeed1 = 100U;
                } else if (rtb_Max < 3500U) {
                    // Transition: '<S1>:30'
                    DEV1ThermalModel_DW.is_c3_DEV1ThermalModel =
                        DEV1ThermalMode_IN_COOL_LEVEL_2;

                    // Outport: '<Root>/pumpSpeed'
                    // Entry 'COOL_LEVEL_2': '<S1>:7'
                    DEV1ThermalModel_Y.pumpSpeed = 50U;
                    DEV1ThermalModel_Y.fanSpeed1 = 50U;
                } else {
                    // Outport: '<Root>/pumpSpeed'
                    DEV1ThermalModel_Y.pumpSpeed = 75U;
                    DEV1ThermalModel_Y.fanSpeed1 = 75U;
                }
                break;

              case DEV1ThermalMode_IN_COOL_LEVEL_2:
                // During 'COOL_LEVEL_2': '<S1>:7'
                if (rtb_Max > 4000U) {
                    // Transition: '<S1>:15'
                    DEV1ThermalModel_DW.is_c3_DEV1ThermalModel =
                        DEV1ThermalMode_IN_COOL_LEVEL_1;

                    // Outport: '<Root>/pumpSpeed'
                    // Entry 'COOL_LEVEL_1': '<S1>:6'
                    DEV1ThermalModel_Y.pumpSpeed = 75U;
                    DEV1ThermalModel_Y.fanSpeed1 = 75U;
                } else if (rtb_Max < 2500U) {
                    // Transition: '<S1>:32'
                    DEV1ThermalModel_DW.is_c3_DEV1ThermalModel =
                        DEV1ThermalM_IN_COOL_LEVEL_IDLE;

                    // Outport: '<Root>/pumpSpeed'
                    // Entry 'COOL_LEVEL_IDLE': '<S1>:8'
                    DEV1ThermalModel_Y.pumpSpeed = 50U;
                    DEV1ThermalModel_Y.fanSpeed1 = 0U;
                } else {
                    // Outport: '<Root>/pumpSpeed'
                    DEV1ThermalModel_Y.pumpSpeed = 50U;
                    DEV1ThermalModel_Y.fanSpeed1 = 50U;
                }
                break;

              case DEV1ThermalM_IN_COOL_LEVEL_IDLE:
                // During 'COOL_LEVEL_IDLE': '<S1>:8'
                if (rtb_Max > 3000U) {
                    // Transition: '<S1>:14'
                    DEV1ThermalModel_DW.is_c3_DEV1ThermalModel =
                        DEV1ThermalMode_IN_COOL_LEVEL_2;

                    // Outport: '<Root>/pumpSpeed'
                    // Entry 'COOL_LEVEL_2': '<S1>:7'
                    DEV1ThermalModel_Y.pumpSpeed = 50U;
                    DEV1ThermalModel_Y.fanSpeed1 = 50U;
                } else {
                    // Outport: '<Root>/pumpSpeed'
                    DEV1ThermalModel_Y.pumpSpeed = 50U;
                    DEV1ThermalModel_Y.fanSpeed1 = 0U;
                }
                break;

              default:
                // During 'COOL_LEVEL_MAX': '<S1>:5'
                if (rtb_Max < 4500U) {
                    // Transition: '<S1>:28'
                    DEV1ThermalModel_DW.is_c3_DEV1ThermalModel =
                        DEV1ThermalMode_IN_COOL_LEVEL_1;

                    // Outport: '<Root>/pumpSpeed'
                    // Entry 'COOL_LEVEL_1': '<S1>:6'
                    DEV1ThermalModel_Y.pumpSpeed = 75U;
                    DEV1ThermalModel_Y.fanSpeed1 = 75U;
                } else {
                    // Outport: '<Root>/pumpSpeed'
                    DEV1ThermalModel_Y.pumpSpeed = 100U;
                    DEV1ThermalModel_Y.fanSpeed1 = 100U;
                }
                break;
            }
        }

        // End of Chart: '<Root>/Thermal Model Stateflow'

        // Outport: '<Root>/fanSpeed2'
        DEV1ThermalModel_Y.fanSpeed2 = DEV1ThermalModel_Y.fanSpeed1;
    }

    // Model initialize function
    void DEV1ThermalModel::initialize()
    {
        // (no initialization code required)
    }

    // Model terminate function
    void DEV1ThermalModel::terminate()
    {
        // (no terminate code required)
    }

    // Constructor
    DEV1ThermalModel::DEV1ThermalModel() :
        DEV1ThermalModel_U(),
        DEV1ThermalModel_Y(),
        DEV1ThermalModel_DW(),
        DEV1ThermalModel_M()
    {
        // Currently there is no constructor body generated.
    }

    // Destructor
    // Currently there is no destructor body generated.
    DEV1ThermalModel::~DEV1ThermalModel() = default;

    // Real-Time Model get method
    DEV1ThermalModel::RT_MODEL_DEV1ThermalModel_T * DEV1ThermalModel::getRTM()
    {
        return (&DEV1ThermalModel_M);
    }
}

//
// File trailer for generated code.
//
// [EOF]
//
