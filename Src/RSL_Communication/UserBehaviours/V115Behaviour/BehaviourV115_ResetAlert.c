/**
  ******************************************************************************
  * @file       BehaviourV115_ResetAlert.c
  * @author     Tim Steinberg
  * @date       02.07.2020
  * @brief      Behaviour sequencer for alert reset
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
  * | 001       | 2020-07-02    | Tim Steinberg         | Initial version / skeleton of file            |
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

#include "UserMethods_Characteristics.h"

/* Typedefinitions / Prototypes */
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_start_doNothing(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_start_Action_A(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_start_Action_B(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_start_Action_C(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_start_Action_D(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_start_loadNextStep(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_start_redo(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_start_abort(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_start_criticalAbort(void);

BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_updateCharacteristic_doNothing(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_updateCharacteristic_Action_A(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_updateCharacteristic_Action_B(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_updateCharacteristic_Action_C(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_updateCharacteristic_Action_D(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_updateCharacteristic_loadNextStep(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_updateCharacteristic_redo(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_updateCharacteristic_abort(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_updateCharacteristic_criticalAbort(void);

BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_sleep_doNothing(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_sleep_Action_A(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_sleep_Action_B(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_sleep_Action_C(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_sleep_Action_D(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_sleep_loadNextStep(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_sleep_redo(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_sleep_abort(void);
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_sleep_criticalAbort(void);

/* Variables */
BEHAVIOUR_CONTROLLER_CALL_STRUCT_TYPEDEF behaviourV115_resetAlert = {
  .initializingBehaviour = BEHAVIOUR_SEQUENCING_STEP_START,
  .numberOfActualBehaviour = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED,
  .initNext = FALSE,
  .stepsOfSequence = {
    {
      .behaviourMainFunction                     = behaviourStep_start_v115_main,
      .behaviourCallbackFunctionNothing          = behaviourV115_resetAlert_start_doNothing,
      .behaviourCallbackFunctionAction_A         = behaviourV115_resetAlert_sleep_Action_A,
      .behaviourCallbackFunctionAction_B         = behaviourV115_resetAlert_sleep_Action_B,
      .behaviourCallbackFunctionAction_C         = behaviourV115_resetAlert_sleep_Action_C,
      .behaviourCallbackFunctionAction_D         = behaviourV115_resetAlert_sleep_Action_D,
      .behaviourCallbackFunctionNextBehaviour    = behaviourV115_resetAlert_start_loadNextStep,
      .behaviourCallbackFunctionRedo             = behaviourV115_resetAlert_start_redo,
      .behaviourCallbackFunctionAbort            = behaviourV115_resetAlert_start_abort,
      .behaviourCallbackFunctionCriticalAbort    = behaviourV115_resetAlert_start_criticalAbort
    },
    {
      .behaviourMainFunction                     = behaviourStep_char_alert_v115_main,
      .behaviourCallbackFunctionNothing          = behaviourV115_resetAlert_updateCharacteristic_doNothing,
      .behaviourCallbackFunctionAction_A         = behaviourV115_resetAlert_updateCharacteristic_Action_A,
      .behaviourCallbackFunctionAction_B         = behaviourV115_resetAlert_updateCharacteristic_Action_B,
      .behaviourCallbackFunctionAction_C         = behaviourV115_resetAlert_updateCharacteristic_Action_C,
      .behaviourCallbackFunctionAction_D         = behaviourV115_resetAlert_updateCharacteristic_Action_D,
      .behaviourCallbackFunctionNextBehaviour    = behaviourV115_resetAlert_updateCharacteristic_loadNextStep,
      .behaviourCallbackFunctionRedo             = behaviourV115_resetAlert_updateCharacteristic_redo,
      .behaviourCallbackFunctionAbort            = behaviourV115_resetAlert_updateCharacteristic_abort,
      .behaviourCallbackFunctionCriticalAbort    = behaviourV115_resetAlert_updateCharacteristic_criticalAbort,
    },
    {
      .behaviourMainFunction                     = behaviourStep_sleep_v115_main,
      .behaviourCallbackFunctionNothing          = behaviourV115_resetAlert_sleep_doNothing,
      .behaviourCallbackFunctionAction_A         = behaviourV115_resetAlert_sleep_Action_A,
      .behaviourCallbackFunctionAction_B         = behaviourV115_resetAlert_sleep_Action_B,
      .behaviourCallbackFunctionAction_C         = behaviourV115_resetAlert_sleep_Action_C,
      .behaviourCallbackFunctionAction_D         = behaviourV115_resetAlert_sleep_Action_D,
      .behaviourCallbackFunctionNextBehaviour    = behaviourV115_resetAlert_sleep_loadNextStep,
      .behaviourCallbackFunctionRedo             = behaviourV115_resetAlert_sleep_redo,
      .behaviourCallbackFunctionAbort            = behaviourV115_resetAlert_sleep_abort,
      .behaviourCallbackFunctionCriticalAbort    = behaviourV115_resetAlert_sleep_criticalAbort,
    },
  },
};

uint32_t resetAlert_start_retry_counter;
uint32_t resetAlert_updateCharacteristic_retry_counter;
uint32_t resetAlert_sleep_retry_counter;

/* Function definitions */
BEHAVIOUR_CONTROLLER_CALL_STRUCT_TYPEDEF* behaviourV115_resetAlert_prepare_and_get_struct(){
  resetAlert_start_retry_counter = 0;
  resetAlert_updateCharacteristic_retry_counter = 0;
  resetAlert_sleep_retry_counter = 0;
  return &behaviourV115_resetAlert;
}



//======================================//
//  ResetAlert -> Start                 //
//======================================//

// Used to tell behaviourcontroller, that nothing is to do right now. Called because no message yet and such
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_start_doNothing(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_start_Action_A(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_start_Action_B(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_start_Action_C(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_start_Action_D(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Used to tell behaviourcontroller, that the next step has to be loaded
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_start_loadNextStep(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_LOAD_NEXT_BEHAVIOUR_STEP, .nextStep = BEHAVIOUR_SEQUENCING_STEP_1};
}

// Used to tell behaviourcontroller, that something went wrong and should repeat the whole process
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_start_redo(){
  resetAlert_start_retry_counter++;
  if (resetAlert_start_retry_counter < 3){
    return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_RELOAD_BEHAVIOUR, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
  }else{
    return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_RETURN_CRITICAL, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
  }
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_start_abort(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// If the RSL10 answers nothing at all, wrong messages or spams "READY AFTER BOOT UP OK" -> something is wrong
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_start_criticalAbort(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_RETURN_CRITICAL, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}



//======================================//
//  ResetAlert -> Update Characteristic //
//======================================//

// Used to tell behaviourcontroller, that nothing is to do right now. Called because no message yet and such
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_updateCharacteristic_doNothing(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_updateCharacteristic_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_updateCharacteristic_Action_A(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_updateCharacteristic_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_updateCharacteristic_Action_B(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_updateCharacteristic_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_updateCharacteristic_Action_C(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_updateCharacteristic_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_updateCharacteristic_Action_D(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Used to tell behaviourcontroller, that the next step has to be loaded
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_updateCharacteristic_loadNextStep(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_LOAD_NEXT_BEHAVIOUR_STEP, .nextStep = BEHAVIOUR_SEQUENCING_STEP_2};
}

// Used to tell behaviourcontroller, that something went wrong and should repeat the whole process
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_updateCharacteristic_redo(){
  resetAlert_updateCharacteristic_retry_counter++;
  if (resetAlert_updateCharacteristic_retry_counter < 3){
    return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_RELOAD_BEHAVIOUR, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
  }else{
    return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_RETURN_CRITICAL, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
  }
}

// Not used by behaviourStep_updateCharacteristic_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_updateCharacteristic_abort(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// If the RSL10 answers nothing at all, wrong messages or spams "READY AFTER BOOT UP OK" -> something is wrong
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_updateCharacteristic_criticalAbort(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_RETURN_CRITICAL, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}



//======================================//
//  ResetAlert -> Sleep                 //
//======================================//

// Used to tell behaviourcontroller, that nothing is to do right now. Called because no message yet and such
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_sleep_doNothing(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_sleep_Action_A(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_sleep_Action_B(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_sleep_Action_C(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_start_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_sleep_Action_D(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Used to tell the behaviourcontroller, that the actual step ended. In this case this was the last behaviourstep
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_sleep_loadNextStep(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_FINISHED_BEHAVIOUR, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_sleep_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_sleep_redo(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// Not used by behaviourStep_sleep_v115_main
BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_sleep_abort(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourV115_resetAlert_sleep_criticalAbort(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_RETURN_CRITICAL, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}



#if TEST_BEHAVIOUR_RESETALERT_V115 >= 1

#include "UserMethods_UART.h"
#include "MasterDefine.h"
#include "Message_Definitions.h"
#include "BehaviourController.h"
#include "Logic.h"

uint8_t *behaviourV115_resetAlert_testBuffer;
uint32_t behaviourV115_resetAlert_lengthBuffer;

void behaviourV115_resetAlert_uartFuncCallback(uint8_t *buf, uint32_t len){
  behaviourV115_resetAlert_testBuffer = buf;
  behaviourV115_resetAlert_lengthBuffer = len;
}

bool behaviourV115_resetAlert_compareReception(uint8_t *buf, uint32_t len){
  if (len != behaviourV115_resetAlert_lengthBuffer){
    return FALSE;
  }
  
  for (int i = 0; i < len; i++){
    if (buf[i] != behaviourV115_resetAlert_testBuffer[i]){
      return FALSE;
    }
  }
  
  return TRUE;
}

int behaviourV115_resetAlert_testsuiteReturner(int retVal){
  userMethods_characteristics_resetAlert();
  return retVal;
}

int behaviourV115_resetAlert_testsuite(void){ 
  uint8_t expectedTransmissionUpdateCharacteristic[] = {UART_TRANSMISSION_MAGIC, UART_MSG_CMD_ALERT, 0x01, 0x00, 0xAA ^ UART_TRANSMISSION_MAGIC ^ UART_MSG_CMD_ALERT ^ 0x01 ^ 0x00};
  uint8_t expectedTransmissionSleeping[] = {UART_TRANSMISSION_MAGIC, UART_MSG_CMD_GO_TO_SLEEP, 0x00, 0xAA ^ UART_TRANSMISSION_MAGIC ^ UART_MSG_CMD_GO_TO_SLEEP ^ 0x00};
  userMethods_txRegisterCallbackForTransmission(behaviourV115_resetAlert_uartFuncCallback);
  
  userMethods_characteristics_resetAlert();
  
  logic_resetEverything();
  behaviourController_loadNewSequence(behaviourV115_resetAlert_prepare_and_get_struct());
// Test what happens,... if RSL10 fails to do anything
  do{}while(behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_CRITICAL_ERROR);
  


  logic_resetEverything();
  behaviourController_loadNewSequence(behaviourV115_resetAlert_prepare_and_get_struct());
// Test what happens,... if RSL10 send "Go to sleep OK" to greet us after sleep
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAKEUP_PIN
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_GO_TO_SLEEP_OK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_GO_TO_SLEEP_OK);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAS
  
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING){ return behaviourV115_resetAlert_testsuiteReturner(-1);}
  
  do{}while(behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_CRITICAL_ERROR);
  


  logic_resetEverything();
  behaviourController_loadNewSequence(behaviourV115_resetAlert_prepare_and_get_struct());
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
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING){ return behaviourV115_resetAlert_testsuiteReturner(-1);}
  
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
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING){ return behaviourV115_resetAlert_testsuiteReturner(-1);}
  
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
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_CRITICAL_ERROR){ return behaviourV115_resetAlert_testsuiteReturner(-1);}
  


  logic_resetEverything();
  behaviourController_loadNewSequence(behaviourV115_resetAlert_prepare_and_get_struct());
// Test what happens,... if RSL10 sends "Ready after wakeup" -> no ACK for CharUpdate
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAKEUP_PIN
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_SLEEP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_SLEEP);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAS
  
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_resetAlert_testsuiteReturner(-1);}
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT
  if (behaviourV115_resetAlert_compareReception(expectedTransmissionUpdateCharacteristic, 5) != TRUE){ return behaviourV115_resetAlert_testsuiteReturner(-1); }
  
  do{}while(behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_CRITICAL_ERROR);
  


  logic_resetEverything();
  behaviourController_loadNewSequence(behaviourV115_resetAlert_prepare_and_get_struct());
// Test what happens,... if RSL10 sends "Ready after bootup" -> ACK the resetAlert, but nothing more
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAKEUP_PIN
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_SLEEP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_SLEEP);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAS
  
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_resetAlert_testsuiteReturner(-1);}
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT
  if (behaviourV115_resetAlert_compareReception(expectedTransmissionUpdateCharacteristic, 5) != TRUE){ return behaviourV115_resetAlert_testsuiteReturner(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_PACK_REC_ACK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_PACK_REC_ACK);
  logic_parseNachricht(HAL_GetTick());
  
  // This will take some time (5s default)
  do{}while(behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_CRITICAL_ERROR);
  


  logic_resetEverything();
  behaviourController_loadNewSequence(behaviourV115_resetAlert_prepare_and_get_struct());
// Test what happens,... if RSL10 sends "Ready after wakeup" -> ACK for CharUpdate -> sends CharUpdate NOT OK
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAKEUP_PIN
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_SLEEP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_SLEEP);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAS
  
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_resetAlert_testsuiteReturner(-1);}
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT
  if (behaviourV115_resetAlert_compareReception(expectedTransmissionUpdateCharacteristic, 5) != TRUE){ return behaviourV115_resetAlert_testsuiteReturner(-1); }
  
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
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING){ return behaviourV115_resetAlert_testsuiteReturner(-1); } // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_CHECK_FOR_MESSAGE_PAIRING_ANSWER
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT
  if (behaviourV115_resetAlert_compareReception(expectedTransmissionUpdateCharacteristic, 5) != TRUE){ return behaviourV115_resetAlert_testsuiteReturner(-1); }
  
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
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING){ return behaviourV115_resetAlert_testsuiteReturner(-1); } // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_CHECK_FOR_MESSAGE_PAIRING_ANSWER
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT
  if (behaviourV115_resetAlert_compareReception(expectedTransmissionUpdateCharacteristic, 5) != TRUE){ return behaviourV115_resetAlert_testsuiteReturner(-1); }
  
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
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_CRITICAL_ERROR){ return behaviourV115_resetAlert_testsuiteReturner(-1); } // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_CHECK_FOR_MESSAGE_PAIRING_ANSWER
  


  logic_resetEverything();
  behaviourController_loadNewSequence(behaviourV115_resetAlert_prepare_and_get_struct());
// Test what happens,... if RSL10 sends "Ready after wakeup" -> ACK for CharUpdate -> sends CharUpdate OK but nothing more
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAKEUP_PIN
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_SLEEP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_SLEEP);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAS
  
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_resetAlert_testsuiteReturner(-1);}
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT
  if (behaviourV115_resetAlert_compareReception(expectedTransmissionUpdateCharacteristic, 5) != TRUE){ return behaviourV115_resetAlert_testsuiteReturner(-1); }
  
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
  
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_resetAlert_testsuiteReturner(-1); } // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  
  // This will take some time (5s default)
  do{}while(behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_CRITICAL_ERROR);
  
  
  
  logic_resetEverything();
  behaviourController_loadNewSequence(behaviourV115_resetAlert_prepare_and_get_struct());
// Test what happens,... if RSL10 sends "Ready after wakeup" -> ACK for CharUpdate -> sends CharUpdate OK but nothing more
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAKEUP_PIN
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_SLEEP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_SLEEP);
  logic_parseNachricht(HAL_GetTick());
  
  behaviourController_main(); // BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAS
  
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_resetAlert_testsuiteReturner(-1);}
  
  //Start state ended, now Characteristic update state
  
  behaviourController_main(); // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_SEND_CHAR_UP_ALERT
  if (behaviourV115_resetAlert_compareReception(expectedTransmissionUpdateCharacteristic, 5) != TRUE){ return behaviourV115_resetAlert_testsuiteReturner(-1); }
  
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
  
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){ return behaviourV115_resetAlert_testsuiteReturner(-1); } // BEHAVIOURSTEP_CHAR_ALERT_V115_STATEMACHINE_WAIT_FOR_MESSAGE
  
  //Characteristic update state ended, now sleep state
  
  behaviourController_main(); // BEHAVIOURSTEP_SLEEP_V115_STATEMACHINE_SETUP_SEND_GO_TO_SLEEP
  if (behaviourV115_resetAlert_compareReception(expectedTransmissionSleeping, 4) != TRUE){ return behaviourV115_resetAlert_testsuiteReturner(-1); }
  
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
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_FINISHED){ return behaviourV115_resetAlert_testsuiteReturner(-1); } // BEHAVIOURSTEP_SLEEP_V115_STATEMACHINE_SETUP_CHECK_FOR_MESSAGE_GOTOSLEEPOK
  
  return behaviourV115_resetAlert_testsuiteReturner(0);
}

#endif
