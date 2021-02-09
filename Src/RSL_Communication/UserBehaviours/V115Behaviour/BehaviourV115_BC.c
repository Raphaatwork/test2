/**
  ******************************************************************************
  * @file       BehaviourV115_BC.c
  * @author     Tim Steinberg
  * @date       25.06.2020
  * @brief      Behaviour sequencer for broadcast
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
#include "BehaviourStepV115_BC_Start.h"
#include "BehaviourStepV115_BC_Wait_TO.h"
#include "BehaviourStepV115_BC_End.h"

#include "UserMethods_Characteristics.h"

/* Typedefinitions / Prototypes */

/* Variables */
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_start_doNothing(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_start_Action_A(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_start_Action_B(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_start_Action_C(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_start_Action_D(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_start_loadNextStep(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_start_redo(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_start_abort(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_start_criticalAbort(void);

BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_startBC_doNothing(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_startBC_Action_A(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_startBC_Action_B(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_startBC_Action_C(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_startBC_Action_D(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_startBC_loadNextStep(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_startBC_redo(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_startBC_abort(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_startBC_criticalAbort(void);

BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_endBCWaitTO_doNothing(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_endBCWaitTO_Action_A(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_endBCWaitTO_Action_B(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_endBCWaitTO_Action_C(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_endBCWaitTO_Action_D(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_endBCWaitTO_loadNextStep(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_endBCWaitTO_redo(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_endBCWaitTO_abort(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_endBCWaitTO_criticalAbort(void);

BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_endBC_doNothing(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_endBC_Action_A(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_endBC_Action_B(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_endBC_Action_C(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_endBC_Action_D(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_endBC_loadNextStep(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_endBC_redo(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_endBC_abort(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_endBC_criticalAbort(void);

BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_sleep_doNothing(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_sleep_Action_A(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_sleep_Action_B(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_sleep_Action_C(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_sleep_Action_D(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_sleep_loadNextStep(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_sleep_redo(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_sleep_abort(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_sleep_criticalAbort(void);

/* Variables */
BEHAVIOUR_CONTROLLER_CALL_STRUCT_TYPEDEF behaviourV115_bc = {
  .initializingBehaviour = BEHAVIOUR_SEQUENCING_STEP_START,
  .numberOfActualBehaviour = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED,
  .initNext = FALSE,
  .stepsOfSequence = {
    {
      .behaviourMainFunction                     = behaviourStep_start_v115_main,
      .behaviourCallbackFunctionNothing          = behaviourV115_bc_start_doNothing,
      .behaviourCallbackFunctionAction_A         = behaviourV115_bc_sleep_Action_A,
      .behaviourCallbackFunctionAction_B         = behaviourV115_bc_sleep_Action_B,
      .behaviourCallbackFunctionAction_C         = behaviourV115_bc_sleep_Action_C,
      .behaviourCallbackFunctionAction_D         = behaviourV115_bc_sleep_Action_D,
      .behaviourCallbackFunctionNextBehaviour    = behaviourV115_bc_start_loadNextStep,
      .behaviourCallbackFunctionRedo             = behaviourV115_bc_start_redo,
      .behaviourCallbackFunctionAbort            = behaviourV115_bc_start_abort,
      .behaviourCallbackFunctionCriticalAbort    = behaviourV115_bc_start_criticalAbort
    },
    {
      .behaviourMainFunction                     = behaviourStep_bc_start_v115_main,
      .behaviourCallbackFunctionNothing          = behaviourV115_bc_startBC_doNothing,
      .behaviourCallbackFunctionAction_A         = behaviourV115_bc_startBC_Action_A,
      .behaviourCallbackFunctionAction_B         = behaviourV115_bc_startBC_Action_B,
      .behaviourCallbackFunctionAction_C         = behaviourV115_bc_startBC_Action_C,
      .behaviourCallbackFunctionAction_D         = behaviourV115_bc_startBC_Action_D,
      .behaviourCallbackFunctionNextBehaviour    = behaviourV115_bc_startBC_loadNextStep,
      .behaviourCallbackFunctionRedo             = behaviourV115_bc_startBC_redo,
      .behaviourCallbackFunctionAbort            = behaviourV115_bc_startBC_abort,
      .behaviourCallbackFunctionCriticalAbort    = behaviourV115_bc_startBC_criticalAbort,
    },
    {
      .behaviourMainFunction                     = behaviourStep_bc_end_wait_to_v115_main,
      .behaviourCallbackFunctionNothing          = behaviourV115_bc_endBCWaitTO_doNothing,
      .behaviourCallbackFunctionAction_A         = behaviourV115_bc_endBCWaitTO_Action_A,
      .behaviourCallbackFunctionAction_B         = behaviourV115_bc_endBCWaitTO_Action_B,
      .behaviourCallbackFunctionAction_C         = behaviourV115_bc_endBCWaitTO_Action_C,
      .behaviourCallbackFunctionAction_D         = behaviourV115_bc_endBCWaitTO_Action_D,
      .behaviourCallbackFunctionNextBehaviour    = behaviourV115_bc_endBCWaitTO_loadNextStep,
      .behaviourCallbackFunctionRedo             = behaviourV115_bc_endBCWaitTO_redo,
      .behaviourCallbackFunctionAbort            = behaviourV115_bc_endBCWaitTO_abort,
      .behaviourCallbackFunctionCriticalAbort    = behaviourV115_bc_endBCWaitTO_criticalAbort,
    },
    {
      .behaviourMainFunction                     = behaviourStep_bc_end_v115_main,
      .behaviourCallbackFunctionNothing          = behaviourV115_bc_endBC_doNothing,
      .behaviourCallbackFunctionAction_A         = behaviourV115_bc_endBC_Action_A,
      .behaviourCallbackFunctionAction_B         = behaviourV115_bc_endBC_Action_B,
      .behaviourCallbackFunctionAction_C         = behaviourV115_bc_endBC_Action_C,
      .behaviourCallbackFunctionAction_D         = behaviourV115_bc_endBC_Action_D,
      .behaviourCallbackFunctionNextBehaviour    = behaviourV115_bc_endBC_loadNextStep,
      .behaviourCallbackFunctionRedo             = behaviourV115_bc_endBC_redo,
      .behaviourCallbackFunctionAbort            = behaviourV115_bc_endBC_abort,
      .behaviourCallbackFunctionCriticalAbort    = behaviourV115_bc_endBC_criticalAbort,
    },
    {
      .behaviourMainFunction                     = behaviourStep_sleep_v115_main,
      .behaviourCallbackFunctionNothing          = behaviourV115_bc_sleep_doNothing,
      .behaviourCallbackFunctionAction_A         = behaviourV115_bc_sleep_Action_A,
      .behaviourCallbackFunctionAction_B         = behaviourV115_bc_sleep_Action_B,
      .behaviourCallbackFunctionAction_C         = behaviourV115_bc_sleep_Action_C,
      .behaviourCallbackFunctionAction_D         = behaviourV115_bc_sleep_Action_D,
      .behaviourCallbackFunctionNextBehaviour    = behaviourV115_bc_sleep_loadNextStep,
      .behaviourCallbackFunctionRedo             = behaviourV115_bc_sleep_redo,
      .behaviourCallbackFunctionAbort            = behaviourV115_bc_sleep_abort,
      .behaviourCallbackFunctionCriticalAbort    = behaviourV115_bc_sleep_criticalAbort,
    },
  },
};

uint32_t bc_start_retry_counter;
uint32_t bc_updateCharacteristic_retry_counter;
uint32_t bc_startBC_retry_counter;
uint32_t bc_BCWaitTO_retry_counter;
uint32_t bc_endBC_retry_counter;
uint32_t bc_sleep_retry_counter;

/* Function definitions */
BEHAVIOUR_CONTROLLER_CALL_STRUCT_TYPEDEF* behaviourV115_bc_prepare_and_get_struct(){
  bc_start_retry_counter = 0;
  bc_startBC_retry_counter = 0;
  bc_BCWaitTO_retry_counter = 0;
  bc_endBC_retry_counter = 0;
  bc_sleep_retry_counter = 0;
  userMethods_characteristics_setTransmissionState_None();
  return &behaviourV115_bc;
}

//==============================//
//  Battery -> Start            //
//==============================//

// Used to tell behaviourcontroller, that nothing is to do right now. Called because no message yet and such
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_start_doNothing(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_start_Action_A(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_start_Action_B(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_start_Action_C(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_start_Action_D(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Used to tell behaviourcontroller, that the next step has to be loaded
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_start_loadNextStep(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_LOAD_NEXT_BEHAVIOUR_STEP, .nextStep = BEHAVIOUR_SEQUENCING_STEP_1};
}

// Used to tell behaviourcontroller, that something went wrong and should repeat the whole process
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_start_redo(){
  bc_start_retry_counter++;
  if (bc_start_retry_counter < 3){
    return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_RELOAD_BEHAVIOUR, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
  }else{
    return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_RETURN_CRITICAL, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
  }
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_start_abort(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// If the RSL10 answers nothing at all, wrong messages or spams "READY AFTER BOOT UP OK" -> something is wrong
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_start_criticalAbort(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_RETURN_CRITICAL, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

//==============================//
//  Battery -> Start Broadcast  //
//==============================//

// Used to tell behaviourcontroller, that nothing is to do right now. Called because no message yet and such
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_startBC_doNothing(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_startBC_Action_A(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_startBC_Action_B(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_startBC_Action_C(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_startBC_Action_D(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Used to tell behaviourcontroller, that the next step has to be loaded
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_startBC_loadNextStep(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_LOAD_NEXT_BEHAVIOUR_STEP, .nextStep = BEHAVIOUR_SEQUENCING_STEP_2};
}

// Used to tell behaviourcontroller, that something went wrong and should repeat the whole process
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_startBC_redo(){
  bc_startBC_retry_counter++;
  if (bc_startBC_retry_counter < 3){
    return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_RELOAD_BEHAVIOUR, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
  }else{
    return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_RETURN_CRITICAL, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
  }
}

// Not used by behaviourStep_startBC_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_startBC_abort(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// If the RSL10 answers nothing at all, wrong messages or spams "READY AFTER BOOT UP OK" -> something is wrong
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_startBC_criticalAbort(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_RETURN_CRITICAL, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

//==============================//
//  Battery -> WaitBCEndTO      //
//==============================//

// Used to tell behaviourcontroller, that nothing is to do right now. Called because no message yet and such
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_endBCWaitTO_doNothing(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_endBCWaitTO_Action_A(){
  userMethods_characteristics_setTransmissionState_Done();
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_endBCWaitTO_Action_B(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_LOAD_NEXT_BEHAVIOUR_STEP, .nextStep = BEHAVIOUR_SEQUENCING_STEP_3};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_endBCWaitTO_Action_C(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_endBCWaitTO_Action_D(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Used to tell behaviourcontroller, that the next step has to be loaded
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_endBCWaitTO_loadNextStep(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_LOAD_NEXT_BEHAVIOUR_STEP, .nextStep = BEHAVIOUR_SEQUENCING_STEP_4};
}

// Used to tell behaviourcontroller, that something went wrong and should repeat the whole process
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_endBCWaitTO_redo(){
  bc_BCWaitTO_retry_counter++;
  if (bc_BCWaitTO_retry_counter < 3){
    return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_RELOAD_BEHAVIOUR, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
  }else{
    return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_RETURN_CRITICAL, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
  }
}

// Not used by behaviourStep_endBCWait_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_endBCWaitTO_abort(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// If the RSL10 answers nothing at all, wrong messages or spams "READY AFTER BOOT UP OK" -> something is wrong
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_endBCWaitTO_criticalAbort(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_RETURN_CRITICAL, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

//==============================//
//  Battery -> EndBC            //
//==============================//

// Used to tell behaviourcontroller, that nothing is to do right now. Called because no message yet and such
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_endBC_doNothing(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_endBC_Action_A(){
  userMethods_characteristics_setTransmissionState_Done();
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_endBC_Action_B(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_endBC_Action_C(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_endBC_Action_D(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Used to tell behaviourcontroller, that the next step has to be loaded
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_endBC_loadNextStep(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_LOAD_NEXT_BEHAVIOUR_STEP, .nextStep = BEHAVIOUR_SEQUENCING_STEP_4};
}

// Used to tell behaviourcontroller, that something went wrong and should repeat the whole process
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_endBC_redo(){
  bc_endBC_retry_counter++;
  if (bc_endBC_retry_counter < 3){
    return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_RELOAD_BEHAVIOUR, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
  }else{
    return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_RETURN_CRITICAL, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
  }
}

// Not used by behaviourStep_endBCWait_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_endBC_abort(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// If the RSL10 answers nothing at all, wrong messages or spams "READY AFTER BOOT UP OK" -> something is wrong
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_endBC_criticalAbort(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_RETURN_CRITICAL, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

//==============================//
//  Battery -> Sleep            //
//==============================//

// Used to tell behaviourcontroller, that nothing is to do right now. Called because no message yet and such
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_sleep_doNothing(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_sleep_Action_A(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_sleep_Action_B(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_sleep_Action_C(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_sleep_Action_D(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Used to tell the behaviourcontroller, that the actual step ended. In this case this was the last behaviourstep
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_sleep_loadNextStep(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_FINISHED_BEHAVIOUR, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_sleep_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_sleep_redo(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_sleep_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_sleep_abort(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_bc_sleep_criticalAbort(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_RETURN_CRITICAL, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}
