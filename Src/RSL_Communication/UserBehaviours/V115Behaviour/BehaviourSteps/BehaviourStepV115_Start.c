/**
  ******************************************************************************
  * @file       BehaviourStepV115_Start.c
  * @author     Tim Steinberg
  * @date       18.06.2020
  * @brief      Code to start the RSL.
  *                     It will try to pull the wakeup line
  *                     If this leads to no answer, it will pull the reset line
  *             If successful the RSL is started. UNDEFINED else.
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
  * | 001       | 2020-06-18    | Tim Steinberg         | Initial version / skeleton of file            |
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
#include "MasterDefine.h"
#include "Message_Definitions.h"
#include "BehaviourDefines.h"
#include "Logic.h"
#include "UserMethods_RSL10_Control.h"
#include "Handler_Timer.h"
#include "Debug.h"
    
#include "Test_Selector.h"

#include "UserMethods_Characteristics.h"
#include "UserMethods_UART.h"

/* Typedefinitions */
typedef enum BEHAVIOURSTEP_START_V115_STATEMACHINE {
  BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAKEUP_PIN                         = 0x00,
  BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAS               = 0x10,
  BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_CHECK_READY_AFTER_SLEEP            = 0x11,
     
  BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_RESET_PIN                          = 0x20,
  BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAB_R1            = 0x21,
  BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_CHECK_FOR_READY_AFTER_BOOT_R1      = 0x22,
   
  BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAB_R2            = 0x31,
  BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_CHECK_FOR_READY_AFTER_BOOT_R2      = 0x32,
     
  BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAB_R3            = 0x41,
  BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_CHECK_FOR_READY_AFTER_BOOT_R3      = 0x42,
  
  BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAB_FAILURE       = 0x51,
  
  BEHAVIOURSTEP_START_V115_STATEMACHINE_NO_NEXT_STATE_OK                         = 0xEE,
  BEHAVIOURSTEP_START_V115_STATEMACHINE_NO_NEXT_STATE_FAIL                       = 0xFF,
} BEHAVIOURSTEP_START_V115_STATEMACHINE_TYPEDEF;

#define BEHAVIOURSTEP_START_V115_TIMER_WAIT_TIME_WAKEUP                         3000
#define BEHAVIOURSTEP_START_V115_TIMER_WAIT_TIME_RESET_INITIAL                  6000
#define BEHAVIOURSTEP_START_V115_TIMER_WAIT_TIME_RESET                          1500

/* Variables */
BEHAVIOURSTEP_START_V115_STATEMACHINE_TYPEDEF behaviourStep_start_v115_internalState = BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAKEUP_PIN;

/* Function definitions */
BEHAVIOUR_STEP_RETURN_VALUES_TYPEDEF behaviourStep_start_v115_main(bool init){
  static TIMER_STRUCT_TYPEDEF timerTimeout;
  bool found;
  uint32_t temp;
  
  if (init){
    behaviourStep_start_v115_internalState = BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAKEUP_PIN;
  }
  
  if (behaviourStep_start_v115_internalState == BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAKEUP_PIN){
    DEBUG_LEESYS_STACK_VALUES(1, "BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAKEUP_PIN\r\n");
    
    // Clear everything as we expect a completely new communication sequence
    logic_resetEverything();
    
    // Pull wakeup line
    //rsl10Control_setWakeupActive();
    HAL_Delay(1);
    //rsl10Control_setWakeupInactive();
    
    // Set up timer for timeout
    timerHandler_timerStart(&timerTimeout, userMethods_characteristics_getTime(), BEHAVIOURSTEP_START_V115_TIMER_WAIT_TIME_WAKEUP);
    
    behaviourStep_start_v115_internalState = BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAS;
    return BEHAVIOUR_STEP_RETURN_NOTHING;
  }
  
  if (behaviourStep_start_v115_internalState == BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAS){
    // Is there a message?
    if (logic_countOfMessagesInInputbuffer() > 0){
      behaviourStep_start_v115_internalState = BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_CHECK_READY_AFTER_SLEEP;
      return BEHAVIOUR_STEP_RETURN_NOTHING;
    }
    // Did the time to send something run out?
    if (timerHandler_isTimerTimedOut(&timerTimeout, userMethods_characteristics_getTime()) == TRUE){
      // Safety-Timeout event
      behaviourStep_start_v115_internalState = BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_RESET_PIN;
    }
    return BEHAVIOUR_STEP_RETURN_NOTHING;
  }
  
  if (behaviourStep_start_v115_internalState == BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_CHECK_READY_AFTER_SLEEP){
    DEBUG_LEESYS_STACK_VALUES(1, "BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_CHECK_READY_AFTER_SLEEP\r\n");
    temp = logic_getSlotOfCommand_inputBuffer(UART_MSG_CMD_READY_AFTER_SLEEP, &found);
    if (found){
      // We got a ready after sleep, we'll just skip the statemachine here and return
      logic_deletePaketFromInputBuffer(temp);
      behaviourStep_start_v115_internalState = BEHAVIOURSTEP_START_V115_STATEMACHINE_NO_NEXT_STATE_OK;
      return BEHAVIOUR_STEP_RETURN_NEXT_BEHAVIOUR_STEP;
    }else{
      behaviourStep_start_v115_internalState = BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_RESET_PIN;
    }
    return BEHAVIOUR_STEP_RETURN_NOTHING;
  }
  
  if (behaviourStep_start_v115_internalState == BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_RESET_PIN){
    DEBUG_LEESYS_STACK_VALUES(1, "BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_RESET_PIN\r\n");
    // Set reset pin
    rsl10Control_setResetActive();
    HAL_Delay(1);
    // Clear everything as we expect a completely new communication sequence
    logic_resetEverything();
    rsl10Control_setResetInactive();
    
    // Set up timer for timeout
    timerHandler_timerStart(&timerTimeout, userMethods_characteristics_getTime(), BEHAVIOURSTEP_START_V115_TIMER_WAIT_TIME_RESET_INITIAL);
    
    behaviourStep_start_v115_internalState = BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAB_R1;
    return BEHAVIOUR_STEP_RETURN_NOTHING;
  }
  
  if (behaviourStep_start_v115_internalState == BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAB_R1){
    // Is there a message?
    if (logic_countOfMessagesInInputbuffer() > 0){
      behaviourStep_start_v115_internalState = BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_CHECK_FOR_READY_AFTER_BOOT_R1;
      return BEHAVIOUR_STEP_RETURN_NOTHING;
    }
    // Did the time to send something run out?
    if (timerHandler_isTimerTimedOut(&timerTimeout, userMethods_characteristics_getTime()) == TRUE){
      // Safety-Timeout event
      behaviourStep_start_v115_internalState = BEHAVIOURSTEP_START_V115_STATEMACHINE_NO_NEXT_STATE_FAIL;
      return BEHAVIOUR_STEP_RETURN_CRITICAL_ABORT;
    }
    return BEHAVIOUR_STEP_RETURN_NOTHING;
  }
  
  if (behaviourStep_start_v115_internalState == BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_CHECK_FOR_READY_AFTER_BOOT_R1){
    DEBUG_LEESYS_STACK_VALUES(1, "BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_CHECK_FOR_READY_AFTER_BOOT_R1\r\n");
    temp = logic_getSlotOfCommand_inputBuffer(UART_MSG_CMD_READY_AFTER_BOOT_UP, &found);
    if (found){
      logic_deletePaketFromInputBuffer(temp);
      behaviourStep_start_v115_internalState = BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAB_R2;
      // We got a ready after bootup, so everything is fine (for now). 2 things may happen now:
      // 1.) RSL accepts ACK and does not send anymore
      // 2.) RSL continues to send more "Ready after bootup"s
      // 2.1.) 1 or 2 more is fine
      // 2.2.) 3 or more is not okay
      // Thus we need a timer. The RSL will continue to send every 1000ms, so 1100 should be fine
      timerHandler_timerStart(&timerTimeout, userMethods_characteristics_getTime(), BEHAVIOURSTEP_START_V115_TIMER_WAIT_TIME_RESET);
      return BEHAVIOUR_STEP_RETURN_NOTHING;
    }else{
      behaviourStep_start_v115_internalState = BEHAVIOURSTEP_START_V115_STATEMACHINE_NO_NEXT_STATE_FAIL;
      return BEHAVIOUR_STEP_RETURN_CRITICAL_ABORT;
    }
  }
  
  if (behaviourStep_start_v115_internalState == BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAB_R2){
    // Is there a message?
    if (logic_countOfMessagesInInputbuffer() > 0){
      behaviourStep_start_v115_internalState = BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_CHECK_FOR_READY_AFTER_BOOT_R2;
      return BEHAVIOUR_STEP_RETURN_NOTHING;
    }
    // Did the time to send something run out?
    if (timerHandler_isTimerTimedOut(&timerTimeout, userMethods_characteristics_getTime()) == TRUE){
      // Safety-Timeout event
      behaviourStep_start_v115_internalState = BEHAVIOURSTEP_START_V115_STATEMACHINE_NO_NEXT_STATE_OK;
      return BEHAVIOUR_STEP_RETURN_NEXT_BEHAVIOUR_STEP;
    }
    return BEHAVIOUR_STEP_RETURN_NOTHING;
  }
  
  if (behaviourStep_start_v115_internalState == BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_CHECK_FOR_READY_AFTER_BOOT_R2){
    DEBUG_LEESYS_STACK_VALUES(1, "BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_CHECK_FOR_READY_AFTER_BOOT_R2\r\n");
    temp = logic_getSlotOfCommand_inputBuffer(UART_MSG_CMD_READY_AFTER_BOOT_UP, &found);
    if (found){
      logic_deletePaketFromInputBuffer(temp);
      behaviourStep_start_v115_internalState = BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAB_R3;
      // We got a ready after bootup, so everything is fine (for now). 2 things may happen now:
      // 1.) RSL accepts ACK and does not send anymore
      // 2.) RSL continues to send more "Ready after bootup"s
      // 2.1.) 1 or 2 more is fine
      // 2.2.) 3 or more is not okay
      // Thus we need a timer. The RSL will continue to send every 1000ms, so 1100 should be fine
      timerHandler_timerStart(&timerTimeout, userMethods_characteristics_getTime(), BEHAVIOURSTEP_START_V115_TIMER_WAIT_TIME_RESET);
      return BEHAVIOUR_STEP_RETURN_NOTHING;
    }else{
      behaviourStep_start_v115_internalState = BEHAVIOURSTEP_START_V115_STATEMACHINE_NO_NEXT_STATE_FAIL;
      return BEHAVIOUR_STEP_RETURN_CRITICAL_ABORT;
    }
  }
  
  if (behaviourStep_start_v115_internalState == BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAB_R3){
    // Is there a message?
    if (logic_countOfMessagesInInputbuffer() > 0){
      behaviourStep_start_v115_internalState = BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_CHECK_FOR_READY_AFTER_BOOT_R3;
      return BEHAVIOUR_STEP_RETURN_NOTHING;
    }
    // Did the time to send something run out?
    if (timerHandler_isTimerTimedOut(&timerTimeout, userMethods_characteristics_getTime()) == TRUE){
      // Safety-Timeout event
      behaviourStep_start_v115_internalState = BEHAVIOURSTEP_START_V115_STATEMACHINE_NO_NEXT_STATE_OK;
      return BEHAVIOUR_STEP_RETURN_NEXT_BEHAVIOUR_STEP;
    }
    return BEHAVIOUR_STEP_RETURN_NOTHING;
  }
  
  if (behaviourStep_start_v115_internalState == BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_CHECK_FOR_READY_AFTER_BOOT_R3){
    DEBUG_LEESYS_STACK_VALUES(1, "BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_CHECK_FOR_READY_AFTER_BOOT_R3\r\n");
    temp = logic_getSlotOfCommand_inputBuffer(UART_MSG_CMD_READY_AFTER_BOOT_UP, &found);
    if (found){
      logic_deletePaketFromInputBuffer(temp);
      behaviourStep_start_v115_internalState = BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAB_FAILURE;
      // We got a ready after bootup, so everything is fine (for now). 2 things may happen now:
      // 1.) RSL accepts ACK and does not send anymore
      // 2.) RSL continues to send more "Ready after bootup"s
      // 2.1.) 1 or 2 more is fine
      // 2.2.) 3 or more is not okay
      // Thus we need a timer. The RSL will continue to send every 1000ms, so 1100 should be fine
      timerHandler_timerStart(&timerTimeout, userMethods_characteristics_getTime(), BEHAVIOURSTEP_START_V115_TIMER_WAIT_TIME_RESET);
      return BEHAVIOUR_STEP_RETURN_NOTHING;
    }else{
      behaviourStep_start_v115_internalState = BEHAVIOURSTEP_START_V115_STATEMACHINE_NO_NEXT_STATE_FAIL;
      return BEHAVIOUR_STEP_RETURN_CRITICAL_ABORT;
    }
  }
  
  if (behaviourStep_start_v115_internalState == BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAB_FAILURE){
    // Is there a message?
    if (logic_countOfMessagesInInputbuffer() > 0){
      // If we got one, the start failed, because he did not stop at all
      behaviourStep_start_v115_internalState = BEHAVIOURSTEP_START_V115_STATEMACHINE_NO_NEXT_STATE_FAIL;
      return BEHAVIOUR_STEP_RETURN_REDO;
    }
    // Did the time to send something run out?
    if (timerHandler_isTimerTimedOut(&timerTimeout, userMethods_characteristics_getTime()) == TRUE){
      // Safety-Timeout event
      behaviourStep_start_v115_internalState = BEHAVIOURSTEP_START_V115_STATEMACHINE_NO_NEXT_STATE_OK;
      return BEHAVIOUR_STEP_RETURN_NEXT_BEHAVIOUR_STEP;
    }
    return BEHAVIOUR_STEP_RETURN_NOTHING;
  }
  
  return BEHAVIOUR_STEP_RETURN_CRITICAL_ABORT;
}

#if TEST_BEHAVIOURSTEP_START_V115 >= 1

#include "UserMethods_UART.h"
#include "Device_Definitions.h"

int behaviourStep_start_v115_testFail(int i){
  return i;
}

int behaviourStep_start_v115_main_test(){
  logic_resetEverything();
  // Test RSL does nuff
  if (behaviourStep_start_v115_main(TRUE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAS){ return behaviourStep_start_v115_testFail(-1); }
  
  if (behaviourStep_start_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAS){ return behaviourStep_start_v115_testFail(-1); }
  
  HAL_Delay(BEHAVIOURSTEP_START_V115_TIMER_WAIT_TIME_WAKEUP);
  
  if (behaviourStep_start_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_RESET_PIN){ return behaviourStep_start_v115_testFail(-1); }
  
  if (behaviourStep_start_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAB_R1){ return behaviourStep_start_v115_testFail(-1); }
  
  if (behaviourStep_start_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAB_R1){ return behaviourStep_start_v115_testFail(-1); }
  
  HAL_Delay(BEHAVIOURSTEP_START_V115_TIMER_WAIT_TIME_RESET_INITIAL);
  
  if (behaviourStep_start_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_CRITICAL_ABORT){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_NO_NEXT_STATE_FAIL){ return behaviourStep_start_v115_testFail(-1); }
  
  // Test RSL answers 4x
  if (behaviourStep_start_v115_main(TRUE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAS){ return behaviourStep_start_v115_testFail(-1); }
  
  if (behaviourStep_start_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAS){ return behaviourStep_start_v115_testFail(-1); }
  
  HAL_Delay(BEHAVIOURSTEP_START_V115_TIMER_WAIT_TIME_WAKEUP);
  
  if (behaviourStep_start_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_RESET_PIN){ return behaviourStep_start_v115_testFail(-1); }
  
  if (behaviourStep_start_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAB_R1){ return behaviourStep_start_v115_testFail(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_BOOT_UP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_BOOT_UP);
  logic_parseNachricht(HAL_GetTick());

  if (behaviourStep_start_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_CHECK_FOR_READY_AFTER_BOOT_R1){ return behaviourStep_start_v115_testFail(-1); }
  
  if (behaviourStep_start_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAB_R2){ return behaviourStep_start_v115_testFail(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_BOOT_UP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_BOOT_UP);
  logic_parseNachricht(HAL_GetTick());

  if (behaviourStep_start_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_CHECK_FOR_READY_AFTER_BOOT_R2){ return behaviourStep_start_v115_testFail(-1); }
  
  if (behaviourStep_start_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAB_R3){ return behaviourStep_start_v115_testFail(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_BOOT_UP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_BOOT_UP);
  logic_parseNachricht(HAL_GetTick());

  if (behaviourStep_start_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_CHECK_FOR_READY_AFTER_BOOT_R3){ return behaviourStep_start_v115_testFail(-1); }
  
  if (behaviourStep_start_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAB_FAILURE){ return behaviourStep_start_v115_testFail(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_BOOT_UP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_BOOT_UP);
  logic_parseNachricht(HAL_GetTick());
  
  if (behaviourStep_start_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_REDO){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_NO_NEXT_STATE_FAIL){ return behaviourStep_start_v115_testFail(-1); }
  
  // Test RSL answers 3x
  if (behaviourStep_start_v115_main(TRUE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAS){ return behaviourStep_start_v115_testFail(-1); }
  
  if (behaviourStep_start_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAS){ return behaviourStep_start_v115_testFail(-1); }
  
  HAL_Delay(BEHAVIOURSTEP_START_V115_TIMER_WAIT_TIME_WAKEUP);
  
  if (behaviourStep_start_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_RESET_PIN){ return behaviourStep_start_v115_testFail(-1); }
  
  if (behaviourStep_start_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAB_R1){ return behaviourStep_start_v115_testFail(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_BOOT_UP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_BOOT_UP);
  logic_parseNachricht(HAL_GetTick());

  if (behaviourStep_start_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_CHECK_FOR_READY_AFTER_BOOT_R1){ return behaviourStep_start_v115_testFail(-1); }
  
  if (behaviourStep_start_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAB_R2){ return behaviourStep_start_v115_testFail(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_BOOT_UP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_BOOT_UP);
  logic_parseNachricht(HAL_GetTick());

  if (behaviourStep_start_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_CHECK_FOR_READY_AFTER_BOOT_R2){ return behaviourStep_start_v115_testFail(-1); }
  
  if (behaviourStep_start_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAB_R3){ return behaviourStep_start_v115_testFail(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_BOOT_UP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_BOOT_UP);
  logic_parseNachricht(HAL_GetTick());

  if (behaviourStep_start_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_CHECK_FOR_READY_AFTER_BOOT_R3){ return behaviourStep_start_v115_testFail(-1); }
  
  if (behaviourStep_start_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAB_FAILURE){ return behaviourStep_start_v115_testFail(-1); }
  
  HAL_Delay(BEHAVIOURSTEP_START_V115_TIMER_WAIT_TIME_RESET);

  if (behaviourStep_start_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NEXT_BEHAVIOUR_STEP){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_NO_NEXT_STATE_OK){ return behaviourStep_start_v115_testFail(-1); }
  
  // Test RSL answers 2x
  if (behaviourStep_start_v115_main(TRUE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAS){ return behaviourStep_start_v115_testFail(-1); }
  
  if (behaviourStep_start_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAS){ return behaviourStep_start_v115_testFail(-1); }
  
  HAL_Delay(BEHAVIOURSTEP_START_V115_TIMER_WAIT_TIME_WAKEUP);
  
  if (behaviourStep_start_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_RESET_PIN){ return behaviourStep_start_v115_testFail(-1); }
  
  if (behaviourStep_start_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAB_R1){ return behaviourStep_start_v115_testFail(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_BOOT_UP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_BOOT_UP);
  logic_parseNachricht(HAL_GetTick());

  if (behaviourStep_start_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_CHECK_FOR_READY_AFTER_BOOT_R1){ return behaviourStep_start_v115_testFail(-1); }
  
  if (behaviourStep_start_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAB_R2){ return behaviourStep_start_v115_testFail(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_BOOT_UP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_BOOT_UP);
  logic_parseNachricht(HAL_GetTick());

  if (behaviourStep_start_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_CHECK_FOR_READY_AFTER_BOOT_R2){ return behaviourStep_start_v115_testFail(-1); }
  
  if (behaviourStep_start_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAB_R3){ return behaviourStep_start_v115_testFail(-1); }
  
  HAL_Delay(BEHAVIOURSTEP_START_V115_TIMER_WAIT_TIME_RESET);

  if (behaviourStep_start_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NEXT_BEHAVIOUR_STEP){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_NO_NEXT_STATE_OK){ return behaviourStep_start_v115_testFail(-1); }
  
  // Test RSL answers 1x
  if (behaviourStep_start_v115_main(TRUE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAS){ return behaviourStep_start_v115_testFail(-1); }
  
  if (behaviourStep_start_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAS){ return behaviourStep_start_v115_testFail(-1); }
  
  HAL_Delay(BEHAVIOURSTEP_START_V115_TIMER_WAIT_TIME_WAKEUP);
  
  if (behaviourStep_start_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_RESET_PIN){ return behaviourStep_start_v115_testFail(-1); }
  
  if (behaviourStep_start_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAB_R1){ return behaviourStep_start_v115_testFail(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_BOOT_UP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_BOOT_UP);
  logic_parseNachricht(HAL_GetTick());

  if (behaviourStep_start_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_CHECK_FOR_READY_AFTER_BOOT_R1){ return behaviourStep_start_v115_testFail(-1); }
  
  if (behaviourStep_start_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAB_R2){ return behaviourStep_start_v115_testFail(-1); }
  
  HAL_Delay(BEHAVIOURSTEP_START_V115_TIMER_WAIT_TIME_RESET);

  if (behaviourStep_start_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NEXT_BEHAVIOUR_STEP){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_NO_NEXT_STATE_OK){ return behaviourStep_start_v115_testFail(-1); }
  
  // Test RSL answers ready after sleep
  if (behaviourStep_start_v115_main(TRUE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAS){ return behaviourStep_start_v115_testFail(-1); }
  
  if (behaviourStep_start_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_WAIT_FOR_MESSAGE_RAS){ return behaviourStep_start_v115_testFail(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_READY_AFTER_SLEEP);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_READY_AFTER_SLEEP);
  logic_parseNachricht(HAL_GetTick());

  if (behaviourStep_start_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_SETUP_CHECK_READY_AFTER_SLEEP){ return behaviourStep_start_v115_testFail(-1); }
  
  if (behaviourStep_start_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NEXT_BEHAVIOUR_STEP){ return behaviourStep_start_v115_testFail(-1); }
  if (behaviourStep_start_v115_internalState != BEHAVIOURSTEP_START_V115_STATEMACHINE_NO_NEXT_STATE_OK){ return behaviourStep_start_v115_testFail(-1); }
  
  return 0;
}

#endif
