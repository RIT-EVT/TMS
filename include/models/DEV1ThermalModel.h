//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: DEV1ThermalModel.h
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
#ifndef RTW_HEADER_DEV1ThermalModel_h_
#define RTW_HEADER_DEV1ThermalModel_h_
#include <stdbool.h>
#include <stdint.h>

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

// Class declaration for model DEV1ThermalModel
namespace TMS
{
    class DEV1ThermalModel final
    {
        // public data and function members
      public:
        // Block states (default storage) for system '<Root>'
        struct DW_DEV1ThermalModel_T {
            uint8_t is_active_c3_DEV1ThermalModel;// '<Root>/Thermal Model Stateflow' 
            uint8_t is_c3_DEV1ThermalModel;// '<Root>/Thermal Model Stateflow'
        };

        // External inputs (root inport signals with default storage)
        struct ExtU_DEV1ThermalModel_T {
            uint16_t temp1;            // '<Root>/temp1'
            uint16_t temp2;            // '<Root>/temp2'
            uint16_t boardTemp;        // '<Root>/boardTemp'
        };

        // External outputs (root outports fed by signals with default storage)
        struct ExtY_DEV1ThermalModel_T {
            uint8_t pumpSpeed;         // '<Root>/pumpSpeed'
            uint8_t fanSpeed1;         // '<Root>/fanSpeed1'
            uint8_t fanSpeed2;         // '<Root>/fanSpeed2'
        };

        // Real-time Model Data Structure
        struct RT_MODEL_DEV1ThermalModel_T {
            const char * volatile errorStatus;
        };

        // Copy Constructor
        DEV1ThermalModel(DEV1ThermalModel const&) = delete;

        // Assignment Operator
        DEV1ThermalModel& operator= (DEV1ThermalModel const&) & = delete;

        // Move Constructor
        DEV1ThermalModel(DEV1ThermalModel &&) = delete;

        // Move Assignment Operator
        DEV1ThermalModel& operator= (DEV1ThermalModel &&) = delete;

        // Real-Time Model get method
        DEV1ThermalModel::RT_MODEL_DEV1ThermalModel_T * getRTM();

        // Root inports set method
        void setExternalInputs(const ExtU_DEV1ThermalModel_T
                               *pExtU_DEV1ThermalModel_T)
        {
            DEV1ThermalModel_U = *pExtU_DEV1ThermalModel_T;
        }

        // Root outports get method
        const ExtY_DEV1ThermalModel_T &getExternalOutputs() const
        {
            return DEV1ThermalModel_Y;
        }

        // model initialize function
        static void initialize();

        // model step function
        void step();

        // model terminate function
        static void terminate();

        // Constructor
        DEV1ThermalModel();

        // Destructor
        ~DEV1ThermalModel();

        // private data and function members
      private:
        // External inputs
        ExtU_DEV1ThermalModel_T DEV1ThermalModel_U;

        // External outputs
        ExtY_DEV1ThermalModel_T DEV1ThermalModel_Y;

        // Block states
        DW_DEV1ThermalModel_T DEV1ThermalModel_DW;

        // Real-Time Model
        RT_MODEL_DEV1ThermalModel_T DEV1ThermalModel_M;
    };
}

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<Root>/Scope' : Unused code path elimination
//  Block '<Root>/Scope1' : Unused code path elimination


//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Use the MATLAB hilite_system command to trace the generated code back
//  to the model.  For example,
//
//  hilite_system('<S3>')    - opens system 3
//  hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'DEV1ThermalModel'
//  '<S1>'   : 'DEV1ThermalModel/Thermal Model Stateflow'

#endif                                 // RTW_HEADER_DEV1ThermalModel_h_

//
// File trailer for generated code.
//
// [EOF]
//
