/**
  ******************************************************************************
  * @file       BehaviourStepV115_BC_Wait_TO.c
  * @author     Tim Steinberg
  * @date       29.06.2020
  * @brief      This code is for the wait time of the ongoing broadcast.
  *             Upon receiving Data transmit it will call ActionA
  *             Upon receiving BC End OK it will call Behaviour Next
  *             Upon receiving BC End NOT OK transmit it will call ActionB
  *             If the timer runs out, it will call ActionB
  *             
  *             ActioB must end the broadcast manually
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
  * | 001       | 2020-06-29    | Tim Steinberg         | Initial version / skeleton of file            |
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
#include "Handler_Timer.h"
#include "Debug.h"

#include "UserMethods_Characteristics.h"
#include "UserMethods_UART.h"

/* Typedefinitions */
typedef enum BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE {
  BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_START_BC_TIMER                 = 0x00,

  BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_DT_OR_BC_END_OR_TO    = 0x10,
  BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_CHECK_FOR_DT_OR_BC_END         = 0x11,
  
  BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_BC_END_OR_TO          = 0x20,
  BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_CHECK_FOR_BC_END               = 0x21,

  BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_NO_NEXT_STATE_OK               = 0xEE,
  BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_NO_NEXT_STATE_FAIL             = 0xFF,
} BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_TYPEDEF;

#define BEHAVIOURSTEP_BC_WAIT_TO_V115_TIMEOUTTIME_END_BROADCASTING        60000

/* Variables */
BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_TYPEDEF behaviourStep_bc_wait_to_v115_internalState = BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_START_BC_TIMER;

/* Function definitions */
BEHAVIOUR_STEP_RETURN_VALUES_TYPEDEF behaviourStep_bc_end_wait_to_v115_main(bool init){
  static TIMER_STRUCT_TYPEDEF timerTimeout;
  bool found;
  uint32_t temp;
  
  if (init){
    behaviourStep_bc_wait_to_v115_internalState = BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_START_BC_TIMER;
  }
  
  if (behaviourStep_bc_wait_to_v115_internalState == BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_START_BC_TIMER){
    DEBUG_LEESYS_STACK_VALUES(1, "BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_START_BC_TIMER\r\n");
    logic_resetEverything();
    timerHandler_timerStart(&timerTimeout, userMethods_characteristics_getTime(), BEHAVIOURSTEP_BC_WAIT_TO_V115_TIMEOUTTIME_END_BROADCASTING);
    
    behaviourStep_bc_wait_to_v115_internalState = BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_DT_OR_BC_END_OR_TO;
    return BEHAVIOUR_STEP_RETURN_NOTHING;
  }
  
  if (behaviourStep_bc_wait_to_v115_internalState == BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_DT_OR_BC_END_OR_TO){
    if (logic_countOfMessagesInInputbuffer() > 0){
      behaviourStep_bc_wait_to_v115_internalState = BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_CHECK_FOR_DT_OR_BC_END;
      return BEHAVIOUR_STEP_RETURN_NOTHING;
    }
    if (timerHandler_isTimerTimedOut(&timerTimeout, userMethods_characteristics_getTime()) == TRUE){
      // Safety-Timeout event
      behaviourStep_bc_wait_to_v115_internalState = BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_NO_NEXT_STATE_FAIL;
      return BEHAVIOUR_STEP_RETURN_ACTION_B;
    }
    return BEHAVIOUR_STEP_RETURN_NOTHING;
  }
  
  if (behaviourStep_bc_wait_to_v115_internalState == BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_CHECK_FOR_DT_OR_BC_END){
    DEBUG_LEESYS_STACK_VALUES(1, "BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_CHECK_FOR_DT_OR_BC_END\r\n");
    temp = logic_getSlotOfCommand_inputBuffer(UART_MSG_CMD_DATA_TRANSMIT_OK, &found);
    if (found){
      logic_deletePaketFromInputBuffer(temp);
      behaviourStep_bc_wait_to_v115_internalState = BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_BC_END_OR_TO;
      return BEHAVIOUR_STEP_RETURN_ACTION_A;
    }
    temp = logic_getSlotOfCommand_inputBuffer(UART_MSG_CMD_STOP_BC_OK, &found);
    if (found){
      logic_deletePaketFromInputBuffer(temp);
      behaviourStep_bc_wait_to_v115_internalState = BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_NO_NEXT_STATE_OK;
      return BEHAVIOUR_STEP_RETURN_NEXT_BEHAVIOUR_STEP;
    }
    temp = logic_getSlotOfCommand_inputBuffer(UART_MSG_CMD_STOP_BC_NOT_OK, &found);
    if (found){
      logic_deletePaketFromInputBuffer(temp);
      behaviourStep_bc_wait_to_v115_internalState = BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_NO_NEXT_STATE_FAIL;
      return BEHAVIOUR_STEP_RETURN_ACTION_B;
    }
    behaviourStep_bc_wait_to_v115_internalState = BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_NO_NEXT_STATE_FAIL;
    return BEHAVIOUR_STEP_RETURN_CRITICAL_ABORT;
  }

  if (behaviourStep_bc_wait_to_v115_internalState == BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_BC_END_OR_TO){
    if (logic_countOfMessagesInInputbuffer() > 0){
      behaviourStep_bc_wait_to_v115_internalState = BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_CHECK_FOR_BC_END;
      return BEHAVIOUR_STEP_RETURN_NOTHING;
    }
    if (timerHandler_isTimerTimedOut(&timerTimeout, userMethods_characteristics_getTime()) == TRUE){
      // Safety-Timeout event
      behaviourStep_bc_wait_to_v115_internalState = BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_NO_NEXT_STATE_FAIL;
      return BEHAVIOUR_STEP_RETURN_ACTION_B;
    }
    return BEHAVIOUR_STEP_RETURN_NOTHING;
  }
  
  if (behaviourStep_bc_wait_to_v115_internalState == BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_CHECK_FOR_BC_END){
    DEBUG_LEESYS_STACK_VALUES(1, "BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_CHECK_FOR_BC_END\r\n");
    temp = logic_getSlotOfCommand_inputBuffer(UART_MSG_CMD_STOP_BC_OK, &found);
    if (found){
      logic_deletePaketFromInputBuffer(temp);
      behaviourStep_bc_wait_to_v115_internalState = BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_NO_NEXT_STATE_OK;
      return BEHAVIOUR_STEP_RETURN_NEXT_BEHAVIOUR_STEP;
    }
    temp = logic_getSlotOfCommand_inputBuffer(UART_MSG_CMD_STOP_BC_NOT_OK, &found);
    if (found){
      logic_deletePaketFromInputBuffer(temp);
      behaviourStep_bc_wait_to_v115_internalState = BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_NO_NEXT_STATE_FAIL;
      return BEHAVIOUR_STEP_RETURN_ACTION_B;
    }
    behaviourStep_bc_wait_to_v115_internalState = BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_NO_NEXT_STATE_FAIL;
    return BEHAVIOUR_STEP_RETURN_CRITICAL_ABORT;
  }  
  
  return BEHAVIOUR_STEP_RETURN_CRITICAL_ABORT;
}
  
#if TEST_BEHAVIOURSTEP_BC_WAIT_TO_V115 >= 1

#include "UserMethods_UART.h"
#include "Device_Definitions.h"

int behaviourStep_bc_wait_to_v115_testFail(int i){
  return i;
}

uint8_t *behaviourStep_bc_wait_to_v115_testBuffer;
uint32_t behaviourStep_bc_wait_to_v115_lengthBuffer;

void behaviourStep_bc_wait_to_v115_uartFuncCallback(uint8_t *buf, uint32_t len){
  behaviourStep_bc_wait_to_v115_testBuffer = buf;
  behaviourStep_bc_wait_to_v115_lengthBuffer = len;
}

bool behaviourStep_bc_wait_to_v115_compareReception(uint8_t *buf, uint32_t len){
  if (len != behaviourStep_bc_wait_to_v115_lengthBuffer){
    return FALSE;
  }
  
  for (int i = 0; i < len; i++){
    if (buf[i] != behaviourStep_bc_wait_to_v115_testBuffer[i]){
      return FALSE;
    }
  }
  
  return TRUE;
}

int behaviourStep_bc_wait_to_v115_main_test(){
  logic_resetEverything();

// RSL does nothing
  if (behaviourStep_bc_end_wait_to_v115_main(TRUE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  if (behaviourStep_bc_wait_to_v115_internalState != BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_DT_OR_BC_END_OR_TO){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  
  if (behaviourStep_bc_end_wait_to_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  if (behaviourStep_bc_wait_to_v115_internalState != BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_DT_OR_BC_END_OR_TO){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  
  HAL_Delay(BEHAVIOURSTEP_BC_WAIT_TO_V115_TIMEOUTTIME_END_BROADCASTING);
  
  if (behaviourStep_bc_end_wait_to_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_ACTION_B){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  if (behaviourStep_bc_wait_to_v115_internalState != BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_NO_NEXT_STATE_FAIL){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  
// RSL sends DT -> nothing more
  if (behaviourStep_bc_end_wait_to_v115_main(TRUE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  if (behaviourStep_bc_wait_to_v115_internalState != BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_DT_OR_BC_END_OR_TO){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  
  if (behaviourStep_bc_end_wait_to_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  if (behaviourStep_bc_wait_to_v115_internalState != BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_DT_OR_BC_END_OR_TO){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_DATA_TRANSMIT_OK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_DATA_TRANSMIT_OK);
  logic_parseNachricht(HAL_GetTick());
  
  if (behaviourStep_bc_end_wait_to_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  if (behaviourStep_bc_wait_to_v115_internalState != BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_CHECK_FOR_DT_OR_BC_END){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  
  if (behaviourStep_bc_end_wait_to_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_ACTION_A){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  if (behaviourStep_bc_wait_to_v115_internalState != BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_BC_END_OR_TO){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  
  HAL_Delay(BEHAVIOURSTEP_BC_WAIT_TO_V115_TIMEOUTTIME_END_BROADCASTING);
  
  if (behaviourStep_bc_end_wait_to_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_ACTION_B){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  if (behaviourStep_bc_wait_to_v115_internalState != BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_NO_NEXT_STATE_FAIL){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  
// RSL sends BC End OK
  if (behaviourStep_bc_end_wait_to_v115_main(TRUE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  if (behaviourStep_bc_wait_to_v115_internalState != BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_DT_OR_BC_END_OR_TO){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  
  if (behaviourStep_bc_end_wait_to_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  if (behaviourStep_bc_wait_to_v115_internalState != BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_DT_OR_BC_END_OR_TO){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_STOP_BC_OK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_STOP_BC_OK);
  logic_parseNachricht(HAL_GetTick());
  
  if (behaviourStep_bc_end_wait_to_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  if (behaviourStep_bc_wait_to_v115_internalState != BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_CHECK_FOR_DT_OR_BC_END){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  
  if (behaviourStep_bc_end_wait_to_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NEXT_BEHAVIOUR_STEP){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  if (behaviourStep_bc_wait_to_v115_internalState != BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_NO_NEXT_STATE_OK){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  
// RSL sends DT -> BC end OK
  if (behaviourStep_bc_end_wait_to_v115_main(TRUE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  if (behaviourStep_bc_wait_to_v115_internalState != BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_DT_OR_BC_END_OR_TO){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  
  if (behaviourStep_bc_end_wait_to_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  if (behaviourStep_bc_wait_to_v115_internalState != BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_DT_OR_BC_END_OR_TO){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_DATA_TRANSMIT_OK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_DATA_TRANSMIT_OK);
  logic_parseNachricht(HAL_GetTick());
  
  if (behaviourStep_bc_end_wait_to_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  if (behaviourStep_bc_wait_to_v115_internalState != BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_CHECK_FOR_DT_OR_BC_END){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  
  if (behaviourStep_bc_end_wait_to_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_ACTION_A){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  if (behaviourStep_bc_wait_to_v115_internalState != BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_BC_END_OR_TO){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_STOP_BC_OK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_STOP_BC_OK);
  logic_parseNachricht(HAL_GetTick());
  
  if (behaviourStep_bc_end_wait_to_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  if (behaviourStep_bc_wait_to_v115_internalState != BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_CHECK_FOR_BC_END){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  
  if (behaviourStep_bc_end_wait_to_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NEXT_BEHAVIOUR_STEP){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  if (behaviourStep_bc_wait_to_v115_internalState != BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_NO_NEXT_STATE_OK){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  
// RSL sends BC end NOT OK
  if (behaviourStep_bc_end_wait_to_v115_main(TRUE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  if (behaviourStep_bc_wait_to_v115_internalState != BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_DT_OR_BC_END_OR_TO){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  
  if (behaviourStep_bc_end_wait_to_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  if (behaviourStep_bc_wait_to_v115_internalState != BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_DT_OR_BC_END_OR_TO){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_STOP_BC_NOT_OK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_STOP_BC_NOT_OK);
  logic_parseNachricht(HAL_GetTick());
  
  if (behaviourStep_bc_end_wait_to_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  if (behaviourStep_bc_wait_to_v115_internalState != BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_CHECK_FOR_DT_OR_BC_END){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  
  if (behaviourStep_bc_end_wait_to_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_ACTION_B){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  if (behaviourStep_bc_wait_to_v115_internalState != BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_NO_NEXT_STATE_FAIL){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  
// RSL sends DT -> BC end NOT OK
  if (behaviourStep_bc_end_wait_to_v115_main(TRUE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  if (behaviourStep_bc_wait_to_v115_internalState != BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_DT_OR_BC_END_OR_TO){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  
  if (behaviourStep_bc_end_wait_to_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  if (behaviourStep_bc_wait_to_v115_internalState != BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_DT_OR_BC_END_OR_TO){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_DATA_TRANSMIT_OK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_DATA_TRANSMIT_OK);
  logic_parseNachricht(HAL_GetTick());
  
  if (behaviourStep_bc_end_wait_to_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  if (behaviourStep_bc_wait_to_v115_internalState != BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_CHECK_FOR_DT_OR_BC_END){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  
  if (behaviourStep_bc_end_wait_to_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_ACTION_A){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  if (behaviourStep_bc_wait_to_v115_internalState != BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_WAIT_FOR_BC_END_OR_TO){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  
  userMethods_uartReceivedByte(HAL_GetTick(), UART_AWAITING_MAGIC);
  userMethods_uartReceivedByte(HAL_GetTick(), UART_MSG_CMD_STOP_BC_NOT_OK);
  userMethods_uartReceivedByte(HAL_GetTick(), 0x00);
  userMethods_uartReceivedByte(HAL_GetTick(), 0xAA ^ UART_AWAITING_MAGIC ^ UART_MSG_CMD_STOP_BC_NOT_OK);
  logic_parseNachricht(HAL_GetTick());
  
  if (behaviourStep_bc_end_wait_to_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_NOTHING){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  if (behaviourStep_bc_wait_to_v115_internalState != BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_CHECK_FOR_BC_END){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  
  if (behaviourStep_bc_end_wait_to_v115_main(FALSE) != BEHAVIOUR_STEP_RETURN_ACTION_B){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  if (behaviourStep_bc_wait_to_v115_internalState != BEHAVIOURSTEP_BC_WAIT_TO_V115_STATEMACHINE_NO_NEXT_STATE_FAIL){ return behaviourStep_bc_wait_to_v115_testFail(-1); }
  
  
  return behaviourStep_bc_wait_to_v115_testFail(0);
}

#endif
