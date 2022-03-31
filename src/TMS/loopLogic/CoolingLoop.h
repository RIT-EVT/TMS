//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: CoolingLoop.h
//
// Code generated for Simulink model 'CoolingLoop'.
//
// Model version                  : 1.6
// Simulink Coder version         : 9.6 (R2021b) 14-May-2021
// C/C++ source code generated on : Mon Mar 28 20:00:23 2022
//
// Target selection: ert.tlc
// Embedded hardware selection: STMicroelectronics->ST10/Super10
// Code generation objectives:
//    1. Execution efficiency
//    2. RAM efficiency
// Validation result: Not run
//
#ifndef RTW_HEADER_CoolingLoop_h_
#define RTW_HEADER_CoolingLoop_h_
#include <cstring>
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"

// Model Code Variants

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetStopRequested
#define rtmGetStopRequested(rtm)       ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequested
#define rtmSetStopRequested(rtm, val)  ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetStopRequestedPtr
#define rtmGetStopRequestedPtr(rtm)    (&((rtm)->Timing.stopRequestedFlag))
#endif

#ifndef rtmGetT
#define rtmGetT(rtm)                   (rtmGetTPtr((rtm))[0])
#endif

#ifndef rtmGetTPtr
#define rtmGetTPtr(rtm)                ((rtm)->Timing.t)
#endif

#ifndef ODE3_INTG
#define ODE3_INTG

// ODE3 Integration Data
struct ODE3_IntgData {
  real_T *y;                           // output
  real_T *f[3];                        // derivatives
};

#endif

// Class declaration for model CoolingLoop
class CoolingLoopModelClass
{
  // public data and function members
 public:
  // Block signals and states (default storage) for system '<Root>'
  struct DW {
    real_T Sum1;                       // '<Root>/Sum1'
  };

  // Continuous states (default storage)
  struct X {
    real_T Integrator_CSTATE;          // '<Root>/Integrator'
  };

  // State derivatives (default storage)
  struct XDot {
    real_T Integrator_CSTATE;          // '<Root>/Integrator'
  };

  // State disabled
  struct XDis {
    boolean_T Integrator_CSTATE;       // '<Root>/Integrator'
  };

  // External inputs (root inport signals with default storage)
  struct ExtU {
    real_T TempAfterInverter;          // '<Root>/Temp After Inverter'
    real_T TempAfterMotor;             // '<Root>/Temp After Motor'
    real_T TempRadiator1;              // '<Root>/Temp Radiator 1'
    real_T TempRadiator2;              // '<Root>/Temp Radiator 2'
    real_T Throttle;                   // '<Root>/Throttle'
  };

  // External outputs (root outports fed by signals with default storage)
  struct ExtY {
    real_T PumpSpeed;                  // '<Root>/Pump Speed'
    boolean_T FanControl;              // '<Root>/Fan Control'
  };

  // Real-time Model Data Structure
  struct RT_MODEL {
    const char_T *errorStatus;
    RTWSolverInfo solverInfo;
    X *contStates;
    int_T *periodicContStateIndices;
    real_T *periodicContStateRanges;
    real_T *derivs;
    boolean_T *contStateDisabled;
    boolean_T zCCacheNeedsReset;
    boolean_T derivCacheNeedsReset;
    boolean_T CTOutputIncnstWithState;
    real_T odeY[1];
    real_T odeF[3][1];
    ODE3_IntgData intgData;

    //
    //  Sizes:
    //  The following substructure contains sizes information
    //  for many of the model attributes such as inputs, outputs,
    //  dwork, sample times, etc.

    struct {
      int_T numContStates;
      int_T numPeriodicContStates;
      int_T numSampTimes;
    } Sizes;

    //
    //  Timing:
    //  The following substructure contains information regarding
    //  the timing information for the model.

    struct {
      uint32_T clockTick0;
      time_T stepSize0;
      uint32_T clockTick1;
      SimTimeStep simTimeStep;
      boolean_T stopRequestedFlag;
      time_T *t;
      time_T tArray[2];
    } Timing;
  };

  // Copy Constructor
  CoolingLoopModelClass(CoolingLoopModelClass const&) =delete;

  // Assignment Operator
  CoolingLoopModelClass& operator= (CoolingLoopModelClass const&) & = delete;

  // Real-Time Model get method
  CoolingLoopModelClass::RT_MODEL * getRTM();

  // External inputs
  ExtU rtU;

  // External outputs
  ExtY rtY;

  // model initialize function
  void initialize();

  // model step function
  void step();

  // Constructor
  CoolingLoopModelClass();

  // Destructor
  ~CoolingLoopModelClass();

  // private data and function members
 private:
  // Block states
  DW rtDW;

  // Block continuous states
  X rtX;

  // Continuous states update member function
  void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si );

  // Derivatives member function
  void CoolingLoop_derivatives();

  // Real-Time Model
  RT_MODEL rtM;
};

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<Root>/Gain' : Eliminated nontunable gain of 1
//  Block '<Root>/MinMax' : Eliminated since input is scalar


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
//  '<Root>' : 'CoolingLoop'

#endif                                 // RTW_HEADER_CoolingLoop_h_

//
// File trailer for generated code.
//
// [EOF]
//
