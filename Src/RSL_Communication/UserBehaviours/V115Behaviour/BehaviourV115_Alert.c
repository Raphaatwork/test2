/**
  ******************************************************************************
  * @file       BehaviourV115_Alert.c
  * @author     Tim Steinberg
  * @date       25.06.2020
  * @brief      Behaviour sequencer for alerting
  ******************************************************************************
  * Redistribution in source and binary forms, with or without modification,
  * are not permitted. Use in source code needs the written approval of the author.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  ******************************************************************************
  *~~~
  * |-----------|---------------|-----------------------|-----------------------------------------------|
  * | Version   | Date          | Author                | Comments and changes                          |
  * |-----------|---------------|-----------------------|-----------------------------------------------|
  * | 001       | 2020-06-25    | Tim Steinberg         | Initial version / skeleton of file            |
  * |-----------|---------------|-----------------------|-----------------------------------------------|
  * |           |               |                       |                                               |
  * |-----------|---------------|-----------------------|-----------------------------------------------|
  * |           |               |                       |                                               |
  * |-----------|---------------|-----------------------|-----------------------------------------------|
  * |           |               |                       |                                               |
  * |-----------|---------------|-----------------------|-----------------------------------------------|
  * |           |               |                       |                                               |
  * |-----------|---------------|-----------------------|-----------------------------------------------|
  *~~~
  */

/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "Test_Selector.h"
#include "BehaviourDefines.h"

#include "BehaviourStepV115_Start.h"
#include "BehaviourStepV115_Sleep.h"
#include "BehaviourStepV115_CharAlert.h"
#include "BehaviourStepV115_BC_Start.h"
#include "BehaviourStepV115_BC_Wait_TO.h"
#include "BehaviourStepV115_BC_End.h"

#include "UserMethods_Characteristics.h"

/* Typedefinitions / Prototypes */

/* Variables */
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_start_doNothing(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_start_Action_A(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_start_Action_B(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_start_Action_C(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_start_Action_D(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_start_loadNextStep(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_start_redo(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_start_abort(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_start_criticalAbort(void);

BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_updateCharacteristic_doNothing(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_updateCharacteristic_Action_A(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_updateCharacteristic_Action_B(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_updateCharacteristic_Action_C(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_updateCharacteristic_Action_D(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_updateCharacteristic_loadNextStep(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_updateCharacteristic_redo(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_updateCharacteristic_abort(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_updateCharacteristic_criticalAbort(void);

BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_startBC_doNothing(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_startBC_Action_A(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_startBC_Action_B(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_startBC_Action_C(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_startBC_Action_D(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_startBC_loadNextStep(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_startBC_redo(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_startBC_abort(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_startBC_criticalAbort(void);

BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_endBCWaitTO_doNothing(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_endBCWaitTO_Action_A(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_endBCWaitTO_Action_B(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_endBCWaitTO_Action_C(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_endBCWaitTO_Action_D(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_endBCWaitTO_loadNextStep(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_endBCWaitTO_redo(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_endBCWaitTO_abort(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_endBCWaitTO_criticalAbort(void);

BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_endBC_doNothing(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_endBC_Action_A(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_endBC_Action_B(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_endBC_Action_C(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_endBC_Action_D(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_endBC_loadNextStep(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_endBC_redo(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_endBC_abort(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_endBC_criticalAbort(void);

BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_sleep_doNothing(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_sleep_Action_A(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_sleep_Action_B(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_sleep_Action_C(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_sleep_Action_D(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_sleep_loadNextStep(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_sleep_redo(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_sleep_abort(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_sleep_criticalAbort(void);

/* Variables */
BEHAVIOUR_CONTROLLER_CALL_STRUCT_TYPEDEF behaviourV115_alert = {
  .initializingBehaviour = BEHAVIOUR_SEQUENCING_STEP_START,
  .numberOfActualBehaviour = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED,
  .initNext = FALSE,
  .stepsOfSequence = {
    {
      .behaviourMainFunction                     = behaviourStep_start_v115_main,
      .behaviourCallbackFunctionNothing          = behaviourV115_alert_start_doNothing,
      .behaviourCallbackFunctionAction_A         = behaviourV115_alert_sleep_Action_A,
      .behaviourCallbackFunctionAction_B         = behaviourV115_alert_sleep_Action_B,
      .behaviourCallbackFunctionAction_C         = behaviourV115_alert_sleep_Action_C,
      .behaviourCallbackFunctionAction_D         = behaviourV115_alert_sleep_Action_D,
      .behaviourCallbackFunctionNextBehaviour    = behaviourV115_alert_start_loadNextStep,
      .behaviourCallbackFunctionRedo             = behaviourV115_alert_start_redo,
      .behaviourCallbackFunctionAbort            = behaviourV115_alert_start_abort,
      .behaviourCallbackFunctionCriticalAbort    = behaviourV115_alert_start_criticalAbort
    },
    {
      .behaviourMainFunction                     = behaviourStep_char_alert_v115_main,
      .behaviourCallbackFunctionNothing          = behaviourV115_alert_updateCharacteristic_doNothing,
      .behaviourCallbackFunctionAction_A         = behaviourV115_alert_updateCharacteristic_Action_A,
      .behaviourCallbackFunctionAction_B         = behaviourV115_alert_updateCharacteristic_Action_B,
      .behaviourCallbackFunctionAction_C         = behaviourV115_alert_updateCharacteristic_Action_C,
      .behaviourCallbackFunctionAction_D         = behaviourV115_alert_updateCharacteristic_Action_D,
      .behaviourCallbackFunctionNextBehaviour    = behaviourV115_alert_updateCharacteristic_loadNextStep,
      .behaviourCallbackFunctionRedo             = behaviourV115_alert_updateCharacteristic_redo,
      .behaviourCallbackFunctionAbort            = behaviourV115_alert_updateCharacteristic_abort,
      .behaviourCallbackFunctionCriticalAbort    = behaviourV115_alert_updateCharacteristic_criticalAbort,
    },
    {
      .behaviourMainFunction                     = behaviourStep_bc_start_v115_main,
      .behaviourCallbackFunctionNothing          = behaviourV115_alert_startBC_doNothing,
      .behaviourCallbackFunctionAction_A         = behaviourV115_alert_startBC_Action_A,
      .behaviourCallbackFunctionAction_B         = behaviourV115_alert_startBC_Action_B,
      .behaviourCallbackFunctionAction_C         = behaviourV115_alert_startBC_Action_C,
      .behaviourCallbackFunctionAction_D         = behaviourV115_alert_startBC_Action_D,
      .behaviourCallbackFunctionNextBehaviour    = behaviourV115_alert_startBC_loadNextStep,
      .behaviourCallbackFunctionRedo             = behaviourV115_alert_startBC_redo,
      .behaviourCallbackFunctionAbort            = behaviourV115_alert_startBC_abort,
      .behaviourCallbackFunctionCriticalAbort    = behaviourV115_alert_startBC_criticalAbort,
    },
    {
      .behaviourMainFunction                     = behaviourStep_bc_end_wait_to_v115_main,
      .behaviourCallbackFunctionNothing          = behaviourV115_alert_endBCWaitTO_doNothing,
      .behaviourCallbackFunctionAction_A         = behaviourV115_alert_endBCWaitTO_Action_A,
      .behaviourCallbackFunctionAction_B         = behaviourV115_alert_endBCWaitTO_Action_B,
      .behaviourCallbackFunctionAction_C         = behaviourV115_alert_endBCWaitTO_Action_C,
      .behaviourCallbackFunctionAction_D         = behaviourV115_alert_endBCWaitTO_Action_D,
      .behaviourCallbackFunctionNextBehaviour    = behaviourV115_alert_endBCWaitTO_loadNextStep,
      .behaviourCallbackFunctionRedo             = behaviourV115_alert_endBCWaitTO_redo,
      .behaviourCallbackFunctionAbort            = behaviourV115_alert_endBCWaitTO_abort,
      .behaviourCallbackFunctionCriticalAbort    = behaviourV115_alert_endBCWaitTO_criticalAbort,
    },
    {
      .behaviourMainFunction                     = behaviourStep_bc_end_v115_main,
      .behaviourCallbackFunctionNothing          = behaviourV115_alert_endBC_doNothing,
      .behaviourCallbackFunctionAction_A         = behaviourV115_alert_endBC_Action_A,
      .behaviourCallbackFunctionAction_B         = behaviourV115_alert_endBC_Action_B,
      .behaviourCallbackFunctionAction_C         = behaviourV115_alert_endBC_Action_C,
      .behaviourCallbackFunctionAction_D         = behaviourV115_alert_endBC_Action_D,
      .behaviourCallbackFunctionNextBehaviour    = behaviourV115_alert_endBC_loadNextStep,
      .behaviourCallbackFunctionRedo             = behaviourV115_alert_endBC_redo,
      .behaviourCallbackFunctionAbort            = behaviourV115_alert_endBC_abort,
      .behaviourCallbackFunctionCriticalAbort    = behaviourV115_alert_endBC_criticalAbort,
    },
    {
      .behaviourMainFunction                     = behaviourStep_sleep_v115_main,
      .behaviourCallbackFunctionNothing          = behaviourV115_alert_sleep_doNothing,
      .behaviourCallbackFunctionAction_A         = behaviourV115_alert_sleep_Action_A,
      .behaviourCallbackFunctionAction_B         = behaviourV115_alert_sleep_Action_B,
      .behaviourCallbackFunctionAction_C         = behaviourV115_alert_sleep_Action_C,
      .behaviourCallbackFunctionAction_D         = behaviourV115_alert_sleep_Action_D,
      .behaviourCallbackFunctionNextBehaviour    = behaviourV115_alert_sleep_loadNextStep,
      .behaviourCallbackFunctionRedo             = behaviourV115_alert_sleep_redo,
      .behaviourCallbackFunctionAbort            = behaviourV115_alert_sleep_abort,
      .behaviourCallbackFunctionCriticalAbort    = behaviourV115_alert_sleep_criticalAbort,
    },
  },
};

uint32_t alert_start_retry_counter;
uint32_t alert_updateCharacteristic_retry_counter;
uint32_t alert_startBC_retry_counter;
uint32_t alert_BCWaitTO_retry_counter;
uint32_t alert_endBC_retry_counter;
uint32_t alert_sleep_retry_counter;

/* Function definitions */
BEHAVIOUR_CONTROLLER_CALL_STRUCT_TYPEDEF* behaviourV115_alert_prepare_and_get_struct(){
  alert_start_retry_counter = 0;
  alert_updateCharacteristic_retry_counter = 0;
  alert_startBC_retry_counter = 0;
  alert_BCWaitTO_retry_counter = 0;
  alert_endBC_retry_counter = 0;
  alert_sleep_retry_counter = 0;
  userMethods_characteristics_setTransmissionState_None();
  return &behaviourV115_alert;
}

//==============================//
//  Alert -> Start              //
//==============================//

// Used to tell behaviourcontroller, that nothing is to do right now. Called because no message yet and such
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_start_doNothing(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_start_Action_A(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_start_Action_B(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_start_Action_C(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_start_Action_D(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Used to tell behaviourcontroller, that the next step has to be loaded
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_start_loadNextStep(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_LOAD_NEXT_BEHAVIOUR_STEP, .nextStep = BEHAVIOUR_SEQUENCING_STEP_1};
}

// Used to tell behaviourcontroller, that something went wrong and should repeat the whole process
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_start_redo(){
  alert_start_retry_counter++;
  if (alert_start_retry_counter < 3){
    return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_RELOAD_BEHAVIOUR, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
  }else{
    return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_RETURN_CRITICAL, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
  }
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_start_abort(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// If the RSL10 answers nothing at all, wrong messages or spams "READY AFTER BOOT UP OK" -> something is wrong
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_start_criticalAbort(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_RETURN_CRITICAL, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

//=================================//
//  Alert -> Update Characteristic //
//=================================//

// Used to tell behaviourcontroller, that nothing is to do right now. Called because no message yet and such
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_updateCharacteristic_doNothing(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_updateCharacteristic_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_updateCharacteristic_Action_A(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_updateCharacteristic_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_updateCharacteristic_Action_B(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_updateCharacteristic_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_updateCharacteristic_Action_C(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_updateCharacteristic_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_updateCharacteristic_Action_D(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Used to tell behaviourcontroller, that the next step has to be loaded
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_updateCharacteristic_loadNextStep(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_LOAD_NEXT_BEHAVIOUR_STEP, .nextStep = BEHAVIOUR_SEQUENCING_STEP_2};
}

// Used to tell behaviourcontroller, that something went wrong and should repeat the whole process
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_updateCharacteristic_redo(){
  alert_updateCharacteristic_retry_counter++;
  if (alert_updateCharacteristic_retry_counter < 3){
    return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_RELOAD_BEHAVIOUR, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
  }else{
    return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_RETURN_CRITICAL, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
  }
}

// Not used by behaviourStep_updateCharacteristic_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_updateCharacteristic_abort(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// If the RSL10 answers nothing at all, wrong messages or spams "READY AFTER BOOT UP OK" -> something is wrong
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_updateCharacteristic_criticalAbort(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_RETURN_CRITICAL, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

//==============================//
//  Alert -> Start Broadcast    //
//==============================//

// Used to tell behaviourcontroller, that nothing is to do right now. Called because no message yet and such
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_startBC_doNothing(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_startBC_Action_A(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_startBC_Action_B(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_startBC_Action_C(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_startBC_Action_D(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Used to tell behaviourcontroller, that the next step has to be loaded
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_startBC_loadNextStep(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_LOAD_NEXT_BEHAVIOUR_STEP, .nextStep = BEHAVIOUR_SEQUENCING_STEP_3};
}

// Used to tell behaviourcontroller, that something went wrong and should repeat the whole process
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_startBC_redo(){
  alert_startBC_retry_counter++;
  if (alert_startBC_retry_counter < 3){
    return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_RELOAD_BEHAVIOUR, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
  }else{
    return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_RETURN_CRITICAL, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
  }
}

// Not used by behaviourStep_startBC_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_startBC_abort(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// If the RSL10 answers nothing at all, wrong messages or spams "READY AFTER BOOT UP OK" -> something is wrong
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_startBC_criticalAbort(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_RETURN_CRITICAL, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

//==============================//
//  Alert -> WaitBCEndTO        //
//==============================//

// Used to tell behaviourcontroller, that nothing is to do right now. Called because no message yet and such
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_endBCWaitTO_doNothing(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_endBCWaitTO_Action_A(){
  userMethods_characteristics_setTransmissionState_Done();
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_endBCWaitTO_Action_B(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_LOAD_NEXT_BEHAVIOUR_STEP, .nextStep = BEHAVIOUR_SEQUENCING_STEP_4};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_endBCWaitTO_Action_C(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_endBCWaitTO_Action_D(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Used to tell behaviourcontroller, that the next step has to be loaded
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_endBCWaitTO_loadNextStep(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_LOAD_NEXT_BEHAVIOUR_STEP, .nextStep = BEHAVIOUR_SEQUENCING_STEP_5};
}

// Used to tell behaviourcontroller, that something went wrong and should repeat the whole process
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_endBCWaitTO_redo(){
  alert_BCWaitTO_retry_counter++;
  if (alert_BCWaitTO_retry_counter < 3){
    return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_RELOAD_BEHAVIOUR, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
  }else{
    return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_RETURN_CRITICAL, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
  }
}

// Not used by behaviourStep_endBCWait_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_endBCWaitTO_abort(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// If the RSL10 answers nothing at all, wrong messages or spams "READY AFTER BOOT UP OK" -> something is wrong
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_endBCWaitTO_criticalAbort(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_RETURN_CRITICAL, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

//==============================//
//  Alert -> EndBC              //
//==============================//

// Used to tell behaviourcontroller, that nothing is to do right now. Called because no message yet and such
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_endBC_doNothing(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_endBC_Action_A(){
  userMethods_characteristics_setTransmissionState_Done();
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_endBC_Action_B(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_endBC_Action_C(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_endBC_Action_D(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Used to tell behaviourcontroller, that the next step has to be loaded
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_endBC_loadNextStep(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_LOAD_NEXT_BEHAVIOUR_STEP, .nextStep = BEHAVIOUR_SEQUENCING_STEP_5};
}

// Used to tell behaviourcontroller, that something went wrong and should repeat the whole process
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_endBC_redo(){
  alert_endBC_retry_counter++;
  if (alert_endBC_retry_counter < 3){
    return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_RELOAD_BEHAVIOUR, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
  }else{
    return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_RETURN_CRITICAL, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
  }
}

// Not used by behaviourStep_endBCWait_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_endBC_abort(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// If the RSL10 answers nothing at all, wrong messages or spams "READY AFTER BOOT UP OK" -> something is wrong
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_endBC_criticalAbort(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_RETURN_CRITICAL, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

//==============================//
//  Alert -> Sleep              //
//==============================//

// Used to tell behaviourcontroller, that nothing is to do right now. Called because no message yet and such
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_sleep_doNothing(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_sleep_Action_A(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_sleep_Action_B(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_sleep_Action_C(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_sleep_Action_D(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Used to tell the behaviourcontroller, that the actual step ended. In this case this was the last behaviourstep
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_sleep_loadNextStep(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_FINISHED_BEHAVIOUR, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_sleep_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_sleep_redo(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_sleep_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_sleep_abort(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_alert_sleep_criticalAbort(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_RETURN_CRITICAL, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}


#if TEST_BEHAVIOUR_ALERT_V115 >= 1

#include "UserMethods_UART.h"
#include "MasterDefine.h"
#include "Message_Definitions.h"
#include "BehaviourController.h"
#include "Logic.h"

uint8_t *behaviourV115_alert_testBuffer;
uint32_t behaviourV115_alert_lengthBuffer;

void behaviourV115_alert_uartFuncCallback(uint8_t *buf, uint32_t len){
  behaviourV115_alert_testBuffer = buf;
  behaviourV115_alert_lengthBuffer = len;
}

bool behaviourV115_alert_compareReception(uint8_t *buf, uint32_t len){
  if (len != behaviourV115_alert_lengthBuffer){
    return FALSE;
  }
  
  for (int i = 0; i < len; i++){
    if (buf[i] != behaviourV115_alert_testBuffer[i]){
      return FALSE;
    }
  }
  
  return TRUE;
}

int behaviourV115_alert_testsuiteReturner(int retVal){
  userMethods_characteristics_resetAlert();
  return retVal;
}

int behaviourV115_alert_testsuite(void){ 
  uint8_t expectedTransmissionUpdateCharacteristic[] = {UART_TRANSMISSION_MAGIC, UART_MSG_CMD_ALERT, 0x01, 0x01, 0xAA ^ UART_TRANSMISSION_MAGIC ^ UART_MSG_CMD_ALERT ^ 0x01 ^ 0x01};
  uint8_t expectedTransmissionStartBC[] = {UART_TRANSMISSION_MAGIC, UART_MSG_CMD_START_BC, 0x00, 0xAA ^ UART_TRANSMISSION_MAGIC ^ UART_MSG_CMD_START_BC ^ 0x00};
  uint8_t expectedTransmissionEndBC[] = {UART_TRANSMISSION_MAGIC, UART_MSG_CMD_STOP_BC, 0x00, 0xAA ^ UART_TRANSMISSION_MAGIC ^ UART_MSG_CMD_STOP_BC ^ 0x00};
  uint8_t expectedTransmissionSleeping[] = {UART_TRANSMISSION_MAGIC, UART_MSG_CMD_GO_TO_SLEEP, 0x00, 0xAA ^ UART_TRANSMISSION_MAGIC ^ UART_MSG_CMD_GO_TO_SLEEP ^ 0x00};
  userMethods_txRegisterCallbackForTransmission(behaviourV115_alert_uartFuncCallback);
  
  userMethods_characteristics_setAlert();


  
  logic_resetEverything();
  behaviourController_loadNewSequence(behaviourV115_alert_prepare_and_get_struct());
// Test what happens,... if RSL10 fails to do anything
  do{}while(behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_CRITICAL_ERROR);
  


  logic_resetEverything();
  behaviourController_loadNewSequence(behaviourV115_alert_prepare_and_get_struct());
// Test what happens,... if RSL10 send "Go to sleep OK" to greet us after sleep
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAKEUP_PIN
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_GO_TO_SLEEP_OK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_GO_TO_SLEEP_OK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAS
  
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING){ return behaviourV115_alert_testsuiteReturner(-1);}
  
  do{}while(behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_CRITICAL_ERROR);
  


  logic_resetEverything();
  behaviourController_loadNewSequence(behaviourV115_alert_prepare_and_get_struct());
// Test what happens,... if RSL10 sends "Ready after bootup" 4x [always]
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAKEUP_PIN
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_BOOT_UP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_BOOT_UP);
  logic_parseNachricht(HAL_GetTick());
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_BOOT_UP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_BOOT_UP);
  logic_parseNachricht(HAL_GetTick());
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_BOOT_UP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_BOOT_UP);
  logic_parseNachricht(HAL_GetTick());
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_BOOT_UP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_BOOT_UP);
  logic_parseNachricht(HAL_GetTick());
  
  for (int i = 0; i < 9; i++){
    behaviourController_main();
  }
  // This step must be a REDO
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING){ return behaviourV115_alert_testsuiteReturner(-1);}
  
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAKEUP_PIN
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_BOOT_UP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_BOOT_UP);
  logic_parseNachricht(HAL_GetTick());
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_BOOT_UP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_BOOT_UP);
  logic_parseNachricht(HAL_GetTick());
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_BOOT_UP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_BOOT_UP);
  logic_parseNachricht(HAL_GetTick());
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_BOOT_UP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_BOOT_UP);
  logic_parseNachricht(HAL_GetTick());
  
  for (int i = 0; i < 9; i++){
    behaviourController_main();
  }
  // This step must be a REDO
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING){ return behaviourV115_alert_testsuiteReturner(-1);}
  
   behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAKEUP_PIN
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_BOOT_UP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_BOOT_UP);
  logic_parseNachricht(HAL_GetTick());
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_BOOT_UP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_BOOT_UP);
  logic_parseNachricht(HAL_GetTick());
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_BOOT_UP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_BOOT_UP);
  logic_parseNachricht(HAL_GetTick());
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_BOOT_UP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_BOOT_UP);
  logic_parseNachricht(HAL_GetTick());
  
  for (int i = 0; i < 9; i++){
    behaviourController_main();
  }
  // This step must be a FAIL
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_CRITICAL_ERROR){ return behaviourV115_alert_testsuiteReturner(-1);}
  


  logic_resetEverything();
  behaviourController_loadNewSequence(behaviourV115_alert_prepare_and_get_struct());
// Test what happens,... if RSL10 sends "Ready after wakeup" -> no ACK for CharUpdate
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAKEUP_PIN
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_SLEEP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_SLEEP);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAS
  
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1);}
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT
  if (behaviourV115_alert_compareReception(expectedTransmissionUpdateCharacteristic, 5) != TRUE){ return behaviourV115_alert_testsuiteReturner(-1); }
  
  do{}while(behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_CRITICAL_ERROR);
  


  logic_resetEverything();
  behaviourController_loadNewSequence(behaviourV115_alert_prepare_and_get_struct());
// Test what happens,... if RSL10 sends "Ready after bootup" -> ACK the alert, but nothing more
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAKEUP_PIN
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_SLEEP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_SLEEP);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAS
  
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1);}
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT
  if (behaviourV115_alert_compareReception(expectedTransmissionUpdateCharacteristic, 5) != TRUE){ return behaviourV115_alert_testsuiteReturner(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_PACK_REC_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_PACK_REC_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  // This will take some time (5s default)
  do{}while(behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_CRITICAL_ERROR);
  


  logic_resetEverything();
  behaviourController_loadNewSequence(behaviourV115_alert_prepare_and_get_struct());
// Test what happens,... if RSL10 sends "Ready after wakeup" -> ACK for CharUpdate -> sends CharUpdate NOT OK
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAKEUP_PIN
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_SLEEP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_SLEEP);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAS
  
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1);}
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT
  if (behaviourV115_alert_compareReception(expectedTransmissionUpdateCharacteristic, 5) != TRUE){ return behaviourV115_alert_testsuiteReturner(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_PACK_REC_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_PACK_REC_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT_WAIT_ACK
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_CHAR_NAK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_CHAR_NAK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  
  // This step must be a REDO
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING){ return behaviourV115_alert_testsuiteReturner(-1); } // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_CHECK_FOR_MESSAGE_PAIRING_ANSWER
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT
  if (behaviourV115_alert_compareReception(expectedTransmissionUpdateCharacteristic, 5) != TRUE){ return behaviourV115_alert_testsuiteReturner(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_PACK_REC_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_PACK_REC_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT_WAIT_ACK
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_CHAR_NAK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_CHAR_NAK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  
  // This step must be a REDO
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING){ return behaviourV115_alert_testsuiteReturner(-1); } // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_CHECK_FOR_MESSAGE_PAIRING_ANSWER
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT
  if (behaviourV115_alert_compareReception(expectedTransmissionUpdateCharacteristic, 5) != TRUE){ return behaviourV115_alert_testsuiteReturner(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_PACK_REC_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_PACK_REC_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT_WAIT_ACK
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_CHAR_NAK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_CHAR_NAK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  
  // This step must be a FAIL
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_CRITICAL_ERROR){ return behaviourV115_alert_testsuiteReturner(-1); } // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_CHECK_FOR_MESSAGE_PAIRING_ANSWER
  


  logic_resetEverything();
  behaviourController_loadNewSequence(behaviourV115_alert_prepare_and_get_struct());
// Test what happens,... if RSL10 sends "Ready after wakeup" -> ACK for CharUpdate -> sends CharUpdate OK but nothing more
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAKEUP_PIN
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_SLEEP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_SLEEP);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAS
  
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1);}
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT
  if (behaviourV115_alert_compareReception(expectedTransmissionUpdateCharacteristic, 5) != TRUE){ return behaviourV115_alert_testsuiteReturner(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_PACK_REC_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_PACK_REC_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT_WAIT_ACK
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_CHAR_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_CHAR_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1); } // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  
  // This will take some time (5s default)
  do{}while(behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_CRITICAL_ERROR);
  


  logic_resetEverything();
  behaviourController_loadNewSequence(behaviourV115_alert_prepare_and_get_struct());
// Test what happens,... if RSL10 sends "Ready after wakeup" -> ACK for CharUpdate -> sends CharUpdate OK ->ACK for BC Start, but nothing more
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAKEUP_PIN
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_SLEEP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_SLEEP);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAS
  
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1);}
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT
  if (behaviourV115_alert_compareReception(expectedTransmissionUpdateCharacteristic, 5) != TRUE){ return behaviourV115_alert_testsuiteReturner(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_PACK_REC_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_PACK_REC_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT_WAIT_ACK
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_CHAR_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_CHAR_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1); } // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_SEND_BROADCAST_START
  if (behaviourV115_alert_compareReception(expectedTransmissionStartBC, 4) != TRUE){ return behaviourV115_alert_testsuiteReturner(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_PACK_REC_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_PACK_REC_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_SEND_BROADCAST_START_WAIT_ACK
  
  // This will take some time (5s default)
  do{}while(behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_CRITICAL_ERROR);
  
  
  
  logic_resetEverything();
  behaviourController_loadNewSequence(behaviourV115_alert_prepare_and_get_struct());
// Test what happens,... if RSL10 sends "Ready after wakeup" -> ACK for CharUpdate -> sends CharUpdate OK ->ACK for BC Start -> Start BC NOT OK
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAKEUP_PIN
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_SLEEP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_SLEEP);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAS
  
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1);}
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT
  if (behaviourV115_alert_compareReception(expectedTransmissionUpdateCharacteristic, 5) != TRUE){ return behaviourV115_alert_testsuiteReturner(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_PACK_REC_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_PACK_REC_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT_WAIT_ACK
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_CHAR_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_CHAR_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1); } // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_SEND_BROADCAST_START
  if (behaviourV115_alert_compareReception(expectedTransmissionStartBC, 4) != TRUE){ return behaviourV115_alert_testsuiteReturner(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_PACK_REC_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_PACK_REC_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_SEND_BROADCAST_START_WAIT_ACK
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_START_BC_NOT_OK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_START_BC_NOT_OK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  
  // This must be a redo
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING){ return behaviourV115_alert_testsuiteReturner(-1); } // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_CHECK_FOR_BROADCAST_START_ANSWER
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_SEND_BROADCAST_START
  if (behaviourV115_alert_compareReception(expectedTransmissionStartBC, 4) != TRUE){ return behaviourV115_alert_testsuiteReturner(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_PACK_REC_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_PACK_REC_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_SEND_BROADCAST_START_WAIT_ACK
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_START_BC_NOT_OK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_START_BC_NOT_OK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  
  // This must be a redo
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING){ return behaviourV115_alert_testsuiteReturner(-1); } // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_CHECK_FOR_BROADCAST_START_ANSWER
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_SEND_BROADCAST_START
  if (behaviourV115_alert_compareReception(expectedTransmissionStartBC, 4) != TRUE){ return behaviourV115_alert_testsuiteReturner(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_PACK_REC_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_PACK_REC_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_SEND_BROADCAST_START_WAIT_ACK
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_START_BC_NOT_OK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_START_BC_NOT_OK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  
  // This must be a FAIL
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_CRITICAL_ERROR){ return behaviourV115_alert_testsuiteReturner(-1); } // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_CHECK_FOR_BROADCAST_START_ANSWER


  
  logic_resetEverything();
  behaviourController_loadNewSequence(behaviourV115_alert_prepare_and_get_struct());
// Test what happens,... if RSL10 sends "Ready after wakeup" -> ACK for CharUpdate -> sends CharUpdate OK ->ACK for BC Start -> Start BC OK -> timeout for BC End
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAKEUP_PIN
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_SLEEP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_SLEEP);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAS
  
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1);}
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT
  if (behaviourV115_alert_compareReception(expectedTransmissionUpdateCharacteristic, 5) != TRUE){ return behaviourV115_alert_testsuiteReturner(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_PACK_REC_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_PACK_REC_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT_WAIT_ACK
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_CHAR_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_CHAR_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1); } // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_SEND_BROADCAST_START
  if (behaviourV115_alert_compareReception(expectedTransmissionStartBC, 4) != TRUE){ return behaviourV115_alert_testsuiteReturner(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_PACK_REC_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_PACK_REC_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_SEND_BROADCAST_START_WAIT_ACK
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_START_BC_OK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_START_BC_OK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1); } // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_CHECK_FOR_BROADCAST_START_ANSWER
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_START_BC_TIMER
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_DT_OR_BC_END_OR_TO
  
  HAL_Delay(45000); // Timeout for Timer
  
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1); } // Load BC End
  
  
  
  
  
  logic_resetEverything();
  behaviourController_loadNewSequence(behaviourV115_alert_prepare_and_get_struct());
// Test what happens,... if RSL10 sends "Ready after wakeup" -> ACK for CharUpdate -> sends CharUpdate OK ->ACK for BC Start -> Start BC OK -> sends only DT
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAKEUP_PIN
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_SLEEP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_SLEEP);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAS
  
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1);}
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT
  if (behaviourV115_alert_compareReception(expectedTransmissionUpdateCharacteristic, 5) != TRUE){ return behaviourV115_alert_testsuiteReturner(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_PACK_REC_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_PACK_REC_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT_WAIT_ACK
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_CHAR_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_CHAR_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1); } // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_SEND_BROADCAST_START
  if (behaviourV115_alert_compareReception(expectedTransmissionStartBC, 4) != TRUE){ return behaviourV115_alert_testsuiteReturner(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_PACK_REC_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_PACK_REC_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_SEND_BROADCAST_START_WAIT_ACK
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_START_BC_OK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_START_BC_OK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1); } // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_CHECK_FOR_BROADCAST_START_ANSWER
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_START_BC_TIMER
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_DT_OR_BC_END_OR_TO
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_DATA_TRANSMIT_OK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_DATA_TRANSMIT_OK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_CHECK_FOR_DT_OR_BC_END -> ActionA
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_BC_END_OR_TO
  
  HAL_Delay(45000); // Timeout for Timer
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1); } // Load BC End
  

 
  logic_resetEverything();
  behaviourController_loadNewSequence(behaviourV115_alert_prepare_and_get_struct());
// Test what happens,... if RSL10 sends "Ready after wakeup" -> ACK for CharUpdate -> sends CharUpdate OK ->ACK for BC Start -> Start BC OK -> sends BC NOT OK
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAKEUP_PIN
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_SLEEP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_SLEEP);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAS
  
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1);}
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT
  if (behaviourV115_alert_compareReception(expectedTransmissionUpdateCharacteristic, 5) != TRUE){ return behaviourV115_alert_testsuiteReturner(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_PACK_REC_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_PACK_REC_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT_WAIT_ACK
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_CHAR_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_CHAR_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1); } // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_SEND_BROADCAST_START
  if (behaviourV115_alert_compareReception(expectedTransmissionStartBC, 4) != TRUE){ return behaviourV115_alert_testsuiteReturner(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_PACK_REC_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_PACK_REC_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_SEND_BROADCAST_START_WAIT_ACK
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_START_BC_OK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_START_BC_OK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1); } // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_CHECK_FOR_BROADCAST_START_ANSWER
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_START_BC_TIMER
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_DT_OR_BC_END_OR_TO
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_STOP_BC_NOT_OK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_STOP_BC_NOT_OK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_DT_OR_BC_END_OR_TO
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1); } // Load BC End
  
  
  
  logic_resetEverything();
  behaviourController_loadNewSequence(behaviourV115_alert_prepare_and_get_struct());
// Test what happens,... if RSL10 sends "Ready after wakeup" -> ACK for CharUpdate -> sends CharUpdate OK ->ACK for BC Start -> Start BC OK -> sends BC NOT OK -> STOP BC -> no answer
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAKEUP_PIN
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_SLEEP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_SLEEP);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAS
  
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1);}
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT
  if (behaviourV115_alert_compareReception(expectedTransmissionUpdateCharacteristic, 5) != TRUE){ return behaviourV115_alert_testsuiteReturner(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_PACK_REC_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_PACK_REC_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT_WAIT_ACK
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_CHAR_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_CHAR_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1); } // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_SEND_BROADCAST_START
  if (behaviourV115_alert_compareReception(expectedTransmissionStartBC, 4) != TRUE){ return behaviourV115_alert_testsuiteReturner(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_PACK_REC_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_PACK_REC_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_SEND_BROADCAST_START_WAIT_ACK
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_START_BC_OK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_START_BC_OK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1); } // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_CHECK_FOR_BROADCAST_START_ANSWER
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_START_BC_TIMER
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_DT_OR_BC_END_OR_TO
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_STOP_BC_NOT_OK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_STOP_BC_NOT_OK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_DT_OR_BC_END_OR_TO
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1); } // Load BC End
  
  // NOW IN END BC MODE
  behaviourController_main(); // BEHAVIOURSTEP_BC_END_V115_STATEMACHINE_SEND_BROADCAST_END
  if (behaviourV115_alert_compareReception(expectedTransmissionEndBC, 4) != TRUE){ return behaviourV115_alert_testsuiteReturner(-1); }
  
  do{}while(behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_CRITICAL_ERROR);
  
  
 
  logic_resetEverything();
  behaviourController_loadNewSequence(behaviourV115_alert_prepare_and_get_struct());
// Test what happens,... if RSL10 sends "Ready after wakeup" -> ACK for CharUpdate -> sends CharUpdate OK ->ACK for BC Start -> Start BC OK -> sends BC NOT OK -> STOP BC -> ACK -> nothing more
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAKEUP_PIN
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_SLEEP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_SLEEP);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAS
  
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1);}
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT
  if (behaviourV115_alert_compareReception(expectedTransmissionUpdateCharacteristic, 5) != TRUE){ return behaviourV115_alert_testsuiteReturner(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_PACK_REC_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_PACK_REC_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT_WAIT_ACK
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_CHAR_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_CHAR_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1); } // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_SEND_BROADCAST_START
  if (behaviourV115_alert_compareReception(expectedTransmissionStartBC, 4) != TRUE){ return behaviourV115_alert_testsuiteReturner(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_PACK_REC_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_PACK_REC_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_SEND_BROADCAST_START_WAIT_ACK
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_START_BC_OK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_START_BC_OK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1); } // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_CHECK_FOR_BROADCAST_START_ANSWER
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_START_BC_TIMER
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_DT_OR_BC_END_OR_TO
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_STOP_BC_NOT_OK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_STOP_BC_NOT_OK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_DT_OR_BC_END_OR_TO
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1); } // Load BC End
  
  // NOW IN END BC MODE
  behaviourController_main(); // BEHAVIOURSTEP_BC_END_V115_STATEMACHINE_SEND_BROADCAST_END
  if (behaviourV115_alert_compareReception(expectedTransmissionEndBC, 4) != TRUE){ return behaviourV115_alert_testsuiteReturner(-1); }
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_END_V115_STATEMACHINE_SEND_BROADCAST_END_WAIT_ACK
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_PACK_REC_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_PACK_REC_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); //BEHAVIOURSTEP_BC_END_V115_STATEMACHINE_SEND_BROADCAST_END_WAIT_ACK 
  do{}while(behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_CRITICAL_ERROR);
  
  
  
  logic_resetEverything();
  behaviourController_loadNewSequence(behaviourV115_alert_prepare_and_get_struct());
// Test what happens,... if RSL10 sends "Ready after wakeup" -> ACK for CharUpdate -> sends CharUpdate OK ->ACK for BC Start -> Start BC OK -> sends BC NOT OK -> STOP BC -> ACK -> nothing more
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAKEUP_PIN
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_SLEEP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_SLEEP);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAS
  
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1);}
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT
  if (behaviourV115_alert_compareReception(expectedTransmissionUpdateCharacteristic, 5) != TRUE){ return behaviourV115_alert_testsuiteReturner(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_PACK_REC_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_PACK_REC_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT_WAIT_ACK
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_CHAR_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_CHAR_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1); } // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_SEND_BROADCAST_START
  if (behaviourV115_alert_compareReception(expectedTransmissionStartBC, 4) != TRUE){ return behaviourV115_alert_testsuiteReturner(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_PACK_REC_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_PACK_REC_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_SEND_BROADCAST_START_WAIT_ACK
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_START_BC_OK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_START_BC_OK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1); } // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_CHECK_FOR_BROADCAST_START_ANSWER
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_START_BC_TIMER
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_DT_OR_BC_END_OR_TO
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_STOP_BC_NOT_OK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_STOP_BC_NOT_OK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_DT_OR_BC_END_OR_TO
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1); } // Load BC End
  
  // NOW IN END BC MODE
  behaviourController_main(); // BEHAVIOURSTEP_BC_END_V115_STATEMACHINE_SEND_BROADCAST_END
  if (behaviourV115_alert_compareReception(expectedTransmissionEndBC, 4) != TRUE){ return behaviourV115_alert_testsuiteReturner(-1); }
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_END_V115_STATEMACHINE_SEND_BROADCAST_END_WAIT_ACK
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_PACK_REC_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_PACK_REC_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); //BEHAVIOURSTEP_BC_END_V115_STATEMACHINE_SEND_BROADCAST_END_WAIT_ACK 
  behaviourController_main(); //BEHAVIOURSTEP_BC_END_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_STOP_BC_OK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_STOP_BC_OK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); //BEHAVIOURSTEP_BC_END_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1);} //BEHAVIOURSTEP_BC_END_V115_STATEMACHINE_CHECK_FOR_BROADCAST_START_ANSWER

  
  
  logic_resetEverything();
  behaviourController_loadNewSequence(behaviourV115_alert_prepare_and_get_struct());
// Test what happens,... if RSL10 sends "Ready after wakeup" -> ACK for CharUpdate -> sends CharUpdate OK ->ACK for BC Start -> Start BC OK -> sends DT -> BC NOT OK
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAKEUP_PIN
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_SLEEP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_SLEEP);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAS
  
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1);}
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT
  if (behaviourV115_alert_compareReception(expectedTransmissionUpdateCharacteristic, 5) != TRUE){ return behaviourV115_alert_testsuiteReturner(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_PACK_REC_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_PACK_REC_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT_WAIT_ACK
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_CHAR_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_CHAR_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1); } // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_SEND_BROADCAST_START
  if (behaviourV115_alert_compareReception(expectedTransmissionStartBC, 4) != TRUE){ return behaviourV115_alert_testsuiteReturner(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_PACK_REC_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_PACK_REC_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_SEND_BROADCAST_START_WAIT_ACK
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_START_BC_OK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_START_BC_OK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1); } // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_CHECK_FOR_BROADCAST_START_ANSWER
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_START_BC_TIMER
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_DT_OR_BC_END_OR_TO
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_DATA_TRANSMIT_OK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_DATA_TRANSMIT_OK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_CHECK_FOR_DT_OR_BC_END -> ActionA
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_BC_END_OR_TO
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_STOP_BC_NOT_OK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_STOP_BC_NOT_OK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_DT_OR_BC_END_OR_TO
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1); } // Load BC End
  

  
  logic_resetEverything();
  behaviourController_loadNewSequence(behaviourV115_alert_prepare_and_get_struct());
// Test what happens,... if RSL10 sends "Ready after wakeup" -> ACK for CharUpdate -> sends CharUpdate OK ->ACK for BC Start -> Start BC OK -> sends BC OK
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAKEUP_PIN
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_SLEEP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_SLEEP);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAS
  
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1);}
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT
  if (behaviourV115_alert_compareReception(expectedTransmissionUpdateCharacteristic, 5) != TRUE){ return behaviourV115_alert_testsuiteReturner(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_PACK_REC_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_PACK_REC_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT_WAIT_ACK
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_CHAR_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_CHAR_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1); } // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_SEND_BROADCAST_START
  if (behaviourV115_alert_compareReception(expectedTransmissionStartBC, 4) != TRUE){ return behaviourV115_alert_testsuiteReturner(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_PACK_REC_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_PACK_REC_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_SEND_BROADCAST_START_WAIT_ACK
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_START_BC_OK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_START_BC_OK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1); } // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_CHECK_FOR_BROADCAST_START_ANSWER
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_START_BC_TIMER
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_DT_OR_BC_END_OR_TO
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_STOP_BC_OK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_STOP_BC_OK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_DT_OR_BC_END_OR_TO
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1); } // Load Sleep
  
  
  
  logic_resetEverything();
  behaviourController_loadNewSequence(behaviourV115_alert_prepare_and_get_struct());
// Test what happens,... if RSL10 sends "Ready after wakeup" -> ACK for CharUpdate -> sends CharUpdate OK ->ACK for BC Start -> Start BC OK -> sends DT -> BC OK 
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAKEUP_PIN
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_SLEEP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_SLEEP);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAS
  
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1);}
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT
  if (behaviourV115_alert_compareReception(expectedTransmissionUpdateCharacteristic, 5) != TRUE){ return behaviourV115_alert_testsuiteReturner(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_PACK_REC_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_PACK_REC_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT_WAIT_ACK
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_CHAR_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_CHAR_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1); } // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_SEND_BROADCAST_START
  if (behaviourV115_alert_compareReception(expectedTransmissionStartBC, 4) != TRUE){ return behaviourV115_alert_testsuiteReturner(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_PACK_REC_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_PACK_REC_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_SEND_BROADCAST_START_WAIT_ACK
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_START_BC_OK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_START_BC_OK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1); } // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_CHECK_FOR_BROADCAST_START_ANSWER
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_START_BC_TIMER
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_DT_OR_BC_END_OR_TO
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_DATA_TRANSMIT_OK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_DATA_TRANSMIT_OK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_CHECK_FOR_DT_OR_BC_END -> ActionA
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_BC_END_OR_TO
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_STOP_BC_OK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_STOP_BC_OK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_BC_END_OR_TO
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1); } // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_CHECK_FOR_BC_END -> load sleep
  
  
  
  logic_resetEverything();
  behaviourController_loadNewSequence(behaviourV115_alert_prepare_and_get_struct());
// Test what happens,... if RSL10 sends "Ready after wakeup" -> ACK for CharUpdate -> sends CharUpdate OK ->ACK for BC Start -> Start BC OK -> sends DT -> BC OK  -> No ACK for "Go to sleep"
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAKEUP_PIN
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_SLEEP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_SLEEP);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAS
  
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1);}
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT
  if (behaviourV115_alert_compareReception(expectedTransmissionUpdateCharacteristic, 5) != TRUE){ return behaviourV115_alert_testsuiteReturner(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_PACK_REC_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_PACK_REC_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT_WAIT_ACK
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_CHAR_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_CHAR_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1); } // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_SEND_BROADCAST_START
  if (behaviourV115_alert_compareReception(expectedTransmissionStartBC, 4) != TRUE){ return behaviourV115_alert_testsuiteReturner(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_PACK_REC_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_PACK_REC_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_SEND_BROADCAST_START_WAIT_ACK
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_START_BC_OK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_START_BC_OK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1); } // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_CHECK_FOR_BROADCAST_START_ANSWER
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_START_BC_TIMER
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_DT_OR_BC_END_OR_TO
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_DATA_TRANSMIT_OK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_DATA_TRANSMIT_OK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_CHECK_FOR_DT_OR_BC_END -> ActionA
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_BC_END_OR_TO
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_STOP_BC_OK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_STOP_BC_OK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_BC_END_OR_TO
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1); } // Load Sleep
  
  behaviourController_main(); // BEHAVIOURSTEP_SLEEP_V115_STATEMACHINE_SETUP_SEND_GO_TO_SLEEP
  if (behaviourV115_alert_compareReception(expectedTransmissionSleeping, 4) != TRUE){ return behaviourV115_alert_testsuiteReturner(-1); }
  
  do{}while(behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_CRITICAL_ERROR);
  
  
  logic_resetEverything();
  behaviourController_loadNewSequence(behaviourV115_alert_prepare_and_get_struct());
// Test what happens,... if RSL10 sends "Ready after wakeup" -> ACK for CharUpdate -> sends CharUpdate OK ->ACK for BC Start -> Start BC OK -> sends DT -> BC OK -> ACK for "Go to sleep", but nothing more  
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAKEUP_PIN
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_SLEEP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_SLEEP);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAS
  
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1);}
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT
  if (behaviourV115_alert_compareReception(expectedTransmissionUpdateCharacteristic, 5) != TRUE){ return behaviourV115_alert_testsuiteReturner(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_PACK_REC_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_PACK_REC_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT_WAIT_ACK
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_CHAR_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_CHAR_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1); } // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_SEND_BROADCAST_START
  if (behaviourV115_alert_compareReception(expectedTransmissionStartBC, 4) != TRUE){ return behaviourV115_alert_testsuiteReturner(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_PACK_REC_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_PACK_REC_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_SEND_BROADCAST_START_WAIT_ACK
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_START_BC_OK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_START_BC_OK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1); } // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_CHECK_FOR_BROADCAST_START_ANSWER
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_START_BC_TIMER
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_DT_OR_BC_END_OR_TO
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_DATA_TRANSMIT_OK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_DATA_TRANSMIT_OK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_CHECK_FOR_DT_OR_BC_END -> ActionA
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_BC_END_OR_TO
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_STOP_BC_OK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_STOP_BC_OK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_BC_END_OR_TO
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1); } // Load Sleep
  
  behaviourController_main(); // BEHAVIOURSTEP_SLEEP_V115_STATEMACHINE_SETUP_SEND_GO_TO_SLEEP
  if (behaviourV115_alert_compareReception(expectedTransmissionSleeping, 4) != TRUE){ return behaviourV115_alert_testsuiteReturner(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_PACK_REC_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_PACK_REC_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_SLEEP_V115_STATEMACHINE_SETUP_WAIT_FOR_ACK
  behaviourController_main(); // BEHAVIOURSTEP_SLEEP_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE
  
  do{}while(behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_CRITICAL_ERROR);
  
  
  logic_resetEverything();
  behaviourController_loadNewSequence(behaviourV115_alert_prepare_and_get_struct());
// Test what happens,... if RSL10 sends "Ready after wakeup" -> ACK for CharUpdate -> sends CharUpdate OK ->ACK for BC Start -> Start BC OK -> sends DT -> BC OK -> ACK for "Go to sleep" -> "Go to sleep OK"
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAKEUP_PIN
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_SLEEP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_SLEEP);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAS
  
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1);}
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT
  if (behaviourV115_alert_compareReception(expectedTransmissionUpdateCharacteristic, 5) != TRUE){ return behaviourV115_alert_testsuiteReturner(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_PACK_REC_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_PACK_REC_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT_WAIT_ACK
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_CHAR_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_CHAR_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1); } // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_SEND_BROADCAST_START
  if (behaviourV115_alert_compareReception(expectedTransmissionStartBC, 4) != TRUE){ return behaviourV115_alert_testsuiteReturner(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_PACK_REC_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_PACK_REC_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_SEND_BROADCAST_START_WAIT_ACK
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_START_BC_OK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_START_BC_OK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1); } // BEHAVIOURSTEP_BC_START_V115_STATEMACHINE_CHECK_FOR_BROADCAST_START_ANSWER
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_START_BC_TIMER
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_DT_OR_BC_END_OR_TO
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_DATA_TRANSMIT_OK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_DATA_TRANSMIT_OK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_CHECK_FOR_DT_OR_BC_END -> ActionA
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_BC_END_OR_TO
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_STOP_BC_OK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_STOP_BC_OK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_BC_END_OR_TO
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_alert_testsuiteReturner(-1); } // Load Sleep
  
  behaviourController_main(); // BEHAVIOURSTEP_SLEEP_V115_STATEMACHINE_SETUP_SEND_GO_TO_SLEEP
  if (behaviourV115_alert_compareReception(expectedTransmissionSleeping, 4) != TRUE){ return behaviourV115_alert_testsuiteReturner(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_PACK_REC_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_PACK_REC_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_SLEEP_V115_STATEMACHINE_SETUP_WAIT_FOR_ACK
  behaviourController_main(); // BEHAVIOURSTEP_SLEEP_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_GO_TO_SLEEP_OK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_GO_TO_SLEEP_OK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_SLEEP_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_FINISHED){ return behaviourV115_alert_testsuiteReturner(-1); } // BEHAVIOURSTEP_SLEEP_V115_STATEMACHINE_SETUP_CHECK_FOR_MESSAGE_GOTOSLEEPOK
  
  
  
  return behaviourV115_alert_testsuiteReturner(0);
}

#endif

