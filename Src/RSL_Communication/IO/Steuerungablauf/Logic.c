/**
  ******************************************************************************
  * @file       Logic.c
  * @author     Tim Steinberg
  * @date       18.05.2020
  * @brief      Control logic for UART protocol
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
  * | 001       | 2020-05-18    | Tim Steinberg         | Initial version / skeleton of file            |
  * |-----------|---------------|-----------------------|-----------------------------------------------|
  * | 002       | 2020-06-15    | Tim Steinberg         | Added comments & doxygen commentaries         |
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
#include "Debug.h"
#include "Test_Selector.h"
#include "Message_Definitions.h"
#include "Parser.h"
#include "MessageIOBuffer.h"
#include "UserMethods_UART.h"
#include "Handler_NAK_Transmission.h"
#include "CRC_Software.h"
#include "ValidMessages.h"
#include "Logic.h"
#include "Stack_Definitions.h"

/* Typedefinitions */

/* Variables */
/** *@brief The memory for the input buffers */
uint8_t                 bufferArraysInput       [LOGIC_MIB_SLOTCOUNT][LOGIC_MIB_BUFFERSIZE];
/** *@brief The memory for the input slots */
BUFFER_STRUCT_TYPEDEF   slotsInput              [LOGIC_MIB_SLOTCOUNT];
/** *@brief The memory for the input timers */
TIMER_STRUCT_TYPEDEF    timerInput              [LOGIC_MIB_SLOTCOUNT];
/** *@brief The memory for the output buffers */
uint8_t                 bufferArraysOutput      [LOGIC_MOB_SLOTCOUNT][LOGIC_MOB_BUFFERSIZE];
/** *@brief The memory for the output slots */
BUFFER_STRUCT_TYPEDEF   slotsOutput             [LOGIC_MOB_SLOTCOUNT];
/** *@brief The memory for the output timers */
TIMER_STRUCT_TYPEDEF    timerOutput             [LOGIC_MOB_SLOTCOUNT];

/** *@brief The message input buffer struct itself */
MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF messageInputBuffer;
/** *@brief The message output buffer struct itself */
MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF messageOutputBuffer;

/** *@brief Buffer for the ACK message */
uint8_t msgACK[4] = {UART_TRANSMISSION_MAGIC, UART_MSG_CMD_PACK_REC_ACK, 0x00, 0xAA ^ UART_TRANSMISSION_MAGIC ^ UART_MSG_CMD_PACK_REC_ACK};

/* Function prototypes */
void logic_slotInit(BUFFER_STRUCT_TYPEDEF *slot, uint8_t *buffer);
uint32_t logic_getSlotOfCommand(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buf, uint8_t cmd, bool *found);


/* Function definitions */

/** @brief This method will init a slot
 *  @param *slot Pointer to the slot that shall be initialized
 *  @param *buffer Pointer to the buffer for this slot
 *  @return Nothing.
 */
void logic_slotInit(BUFFER_STRUCT_TYPEDEF *slot, uint8_t *buffer){
  messageIOBuffer_resetSlot(slot);
  slot->buffer = buffer;
}

/** @brief This method will return the count of messages in the input buffer
 *  @return The count of slots used.
 */
uint32_t logic_countOfMessagesInInputbuffer(){
  return messageIOBuffer_countOfSlotsUsed(&messageInputBuffer);
}

/** @brief This method will return the count of messages in the output buffer
 *  @return The count of slots used.
 */
uint32_t logic_countOfMessagesInOutputbuffer(){
  return messageIOBuffer_countOfSlotsUsed(&messageOutputBuffer);
}
   
/** @brief This method will reset the logic systems values
 *  @return Nothing.
 */
void logic_reset(){
  messageInputBuffer.slotCount = LOGIC_MIB_SLOTCOUNT;
  messageInputBuffer.maxBufferLength = LOGIC_MIB_BUFFERSIZE;
  
  for (int i = 0; i < LOGIC_MIB_SLOTCOUNT; i++){
    logic_slotInit(&slotsInput[i], &bufferArraysInput[i][0]);
    messageInputBuffer.slot = slotsInput;
  }
  
  messageOutputBuffer.slotCount = LOGIC_MOB_SLOTCOUNT;
  messageOutputBuffer.maxBufferLength = LOGIC_MOB_BUFFERSIZE;
  
  for (int i = 0; i < LOGIC_MOB_SLOTCOUNT; i++){
    logic_slotInit(&slotsOutput[i], &bufferArraysOutput[i][0]);
    messageOutputBuffer.slot = slotsOutput;
  }
}

/** @brief This method will count all timed out slots of the output buffer
 *  @param time The actual time
 *  @return Nothing.
 */
uint32_t logic_countOfTimeouts(uint32_t time){
  uint32_t counterSlots = 0;
  uint32_t counterTimeouts = 0;
  
  while(counterSlots < messageIOBuffer_getSlotCount(&messageOutputBuffer)){
    if (messageIOBuffer_isSlotTimedOut_bySlotId(&messageOutputBuffer, counterSlots, time) == TRUE){
      counterTimeouts++;
    }
    counterSlots++;
  }
  
  return counterTimeouts;
}

/** @brief This method will handle all timeouts (by retransmitting them)
 *  @param time The actual time
 *  @return TRUE if all could be handled
 *  @return FALSE if there is a retransmit-overflow, meaning too often no ACK
 */
bool logic_handleTimeouts(uint32_t time){
  uint32_t counterSlots = 0;
  uint32_t transmissionCount;
  
  while(counterSlots < messageIOBuffer_getSlotCount(&messageOutputBuffer)){
    if (messageIOBuffer_isSlotTimedOut_bySlotId(&messageOutputBuffer, counterSlots, time) == FALSE){
      counterSlots++;
      continue;
    }
    
    transmissionCount = messageIOBuffer_incTransmissionCount_bySlotId(&messageOutputBuffer, counterSlots);
    
    if (transmissionCount <= 2){
      // Resent the message
      userMethods_uartTransmit(
        messageIOBuffer_getMessageLength_bySlotId(&messageOutputBuffer, counterSlots), 
        messageIOBuffer_getBuffer_bySlotId(&messageOutputBuffer, counterSlots)
      );
      // Reset the timer
      messageIOBuffer_setValuesRetransmit_bySlotId(&messageOutputBuffer, counterSlots, time, messageOutputBuffer.slot[counterSlots].timer.time_waitTime);
    }else{
      return FALSE;
    }
    counterSlots++;
  }
  return TRUE;
}

/** @brief This method will try to
 *              parse a message
 *              send ACK upon OK, NAK if NOK
 *              return to you the respective value
 *  @param time The actual time
 *  @return Take a look at typedef enum LOGIC_RETURN_VALUES.
 */
LOGIC_RETURN_VALUES_TYPEDEF logic_parseNachricht(uint32_t time){
  int32_t temp = messageIOBuffer_getFreeSlot(&messageInputBuffer);
  PARSER_RETURN_VALUES_TYPEDEF returnValueParser;
  uint32_t slotIdOldestElement;
  uint32_t transmissionLength;
  
  if (temp < 0){
    return LOGIC_RETURN_CRITICAL_ERROR;
  }
  
  returnValueParser = parser_parseMessage(time, &messageInputBuffer, temp);
  
  switch(returnValueParser){
    case PARSER_RETURN_TOO_LESS_BYTES:
      TRACE_IO_VALUES(1, "LOGIC->PARSER_RETURN_TOO_LESS_BYTES\r\n");
      return LOGIC_RETURN_NOTHING;
      break;
      
    case PARSER_RETURN_DROP_BYTE:
      TRACE_IO_VALUES(1, "LOGIC->DROP PARSER_RETURN_DROP_BYTE\r\n");
      return LOGIC_RETURN_NOTHING;
      break;
      
    case PARSER_RETURN_TIMEOUT:
      TRACE_IO_VALUES(1, "LOGIC->PARSER_RETURN_TIMEOUT\r\n");
      return LOGIC_RETURN_NOTHING;
      break;
      
    case PARSER_RETURN_DO_NAK_HANDLING:
      TRACE_IO_VALUES(1, "LOGIC->PARSER_RETURN_DO_NAK_HANDLING\r\n");
      handlerNAK_sendNAK();
      if (handlerNAK_getState() == NAK_TRANSMISSION_STATE_TOO_MANY){
        return LOGIC_RETURN_CRITICAL_ERROR;
      }else{
        return LOGIC_RETURN_NOTHING_NAK;
      }
      break;
      
    case PARSER_RETURN_MESSAGE_SUCCESSFULLY_RECEPTED:
      handlerNAK_resetCounter();
      switch(messageIOBuffer_getBuffer_bySlotId(&messageInputBuffer, temp)[UART_PACKAGE_POSITION_CMD_BYTE]){
        case UART_MSG_CMD_PACK_REC_ACK:
          TRACE_IO_VALUES(1, "LOGIC->UART_MSG_CMD_PACK_REC_ACK\r\n"); 
          // was there an oldest message? if not, then this ACK came without any reason
          if (messageIOBuffer_deleteMessage_oldest(&messageOutputBuffer) == FALSE){
            return LOGIC_RETURN_CRITICAL_ERROR;
          }
          messageIOBuffer_deleteMessage_bySlotId(&messageInputBuffer, temp);
          return LOGIC_RETURN_NOTHING;
          break;
          
        case UART_MSG_CMD_PACK_REC_NAK:
          TRACE_IO_VALUES(1, "LOGIC->UART_MSG_CMD_PACK_REC_NAK\r\n"); 
          slotIdOldestElement = messageIOBuffer_getSlotIdOldestElement(&messageOutputBuffer);
          messageIOBuffer_resetMessageTimeout_bySlotID(&messageOutputBuffer, slotIdOldestElement, time);
          if (messageIOBuffer_incTransmissionCount_bySlotId(&messageOutputBuffer, slotIdOldestElement) >= 3){
            return LOGIC_RETURN_CRITICAL_ERROR;
          }
          transmissionLength = userMethods_uartTransmit(messageIOBuffer_getMessageLength_bySlotId(&messageOutputBuffer, slotIdOldestElement), messageIOBuffer_getBuffer_bySlotId(&messageOutputBuffer, slotIdOldestElement));
          if (transmissionLength != messageIOBuffer_getMessageLength_bySlotId(&messageOutputBuffer, slotIdOldestElement)){
            return LOGIC_RETURN_CRITICAL_ERROR;
          }
          return LOGIC_RETURN_NOTHING;
          break;
          
        default:
          TRACE_IO_VALUES(1, "LOGIC->LOGIC_RETURN_NEW_MESSAGE\r\n");
          userMethods_uartTransmit(4, msgACK);
          return LOGIC_RETURN_NEW_MESSAGE; 
          break;
          
      }
      break;
  }
  
  TRACE_IO_VALUES(1, "LOGIC->LOGIC_RETURN_CRITICAL_ERROR\r\n"); 
  return LOGIC_RETURN_CRITICAL_ERROR;
}

/** @brief This method will see if the M/C/D bytes of your message are OK
 *  @param magicByte The byte containing your magic
 *  @param cmdByte The byte containing your command
 *  @param dataLengthByte The byte containing the data length
 *  @return TRUE if OK, FALSE else.
 */
bool logic_areMCDBytesValid(uint8_t magicByte, uint8_t cmdByte, uint8_t dataLengthByte){
  bool retVal = FALSE;
  
  uint32_t colcount     = messageParamValidity_transmission_getColCount();
  uint32_t rowcount     = messageParamValidity_transmission_getRowCount();
  uint8_t **bufPtr      = messageParamValidity_transmission_getPointerToBuffer();
  uint32_t mover;
  
  if (magicByte == (uint8_t) UART_TRANSMISSION_MAGIC){
    for (uint32_t i = (uint32_t) 0; i < rowcount; i++){
      mover = i * colcount;
      if (((uint8_t*) bufPtr)[mover] == cmdByte){
        /* It is contained*/ 
        
        /* Check if the data length is in the correct interval as well */
        if ((dataLengthByte >= ((uint8_t*) bufPtr)[mover + 1]) && (dataLengthByte <= ((uint8_t*) bufPtr)[mover + 2])){
          /* It is correct */
          retVal = TRUE;
          break;
        }
      }
    }
  }
  
  return retVal;
}

/** @brief This method will transmit your message, if it is OK
 *  @param cmd The byte containing the command
 *  @param lengthParam The byte containing the data length
 *  @param *param The array pointer containing the data
 *  @param time The actual time
 *  @param timeoutTime The time the communication partner can take to ACK this
 *  @return LOGIC_RETURN_MESSAGE_SENT if OK, LOGIC_RETURN_CRITICAL_ERROR else.
 */
LOGIC_RETURN_VALUES_TYPEDEF logic_transmitMessage(uint8_t cmd, uint32_t lengthParam, uint8_t *param, uint32_t time, uint32_t timeoutTime){
  int32_t temp = messageIOBuffer_getFreeSlot(&messageOutputBuffer);
  uint8_t length;
  uint32_t buildLength;
  uint32_t transmissionLength;
  
  if (lengthParam > 0xFF){
    return LOGIC_RETURN_CRITICAL_ERROR;
  }
  length = (uint8_t) (lengthParam & 0xFF);
  
  if (temp < 0){
    return LOGIC_RETURN_CRITICAL_ERROR;
  }
  
  if (logic_areMCDBytesValid(UART_TRANSMISSION_MAGIC, cmd, length) != TRUE){
    return LOGIC_RETURN_CRITICAL_ERROR;
  }
  
  messageIOBuffer_addByteToBuffer_bySlotId(&messageOutputBuffer, temp, UART_TRANSMISSION_MAGIC);
  messageIOBuffer_addByteToBuffer_bySlotId(&messageOutputBuffer, temp, cmd);
  messageIOBuffer_addByteToBuffer_bySlotId(&messageOutputBuffer, temp, length);
  for (uint32_t i = (uint32_t) 0; i < length; i++){
    messageIOBuffer_addByteToBuffer_bySlotId(&messageOutputBuffer, temp, param[i]);
  }
  buildLength = UART_PACKAGE_HEADER_SIZE + UART_PACKAGE_DATA_SIZE + length;
  messageIOBuffer_addByteToBuffer_bySlotId(&messageOutputBuffer, temp, 
    CRC_Software_buildCRC(
      messageIOBuffer_getBuffer_bySlotId(&messageOutputBuffer, temp), 
      buildLength
    )
  );
  
  buildLength = buildLength + UART_PACKAGE_CRC_SIZE;
  transmissionLength = userMethods_uartTransmit(buildLength, messageIOBuffer_getBuffer_bySlotId(&messageOutputBuffer, temp));
  
  if (transmissionLength != buildLength){
    return LOGIC_RETURN_CRITICAL_ERROR;
  }
  
  messageIOBuffer_validateMessage_bySlotId(&messageOutputBuffer, temp, time, timeoutTime);
  
  return LOGIC_RETURN_MESSAGE_SENT;
}

/** @brief This method will reset the logic and it's subsystems values
 *  @return Nothing.
 */
void logic_resetEverything(){
  userMethods_stopReception();
  logic_reset();
  messageIOBuffer_clearAllSlots(&messageOutputBuffer);
  parser_resetEverything();
  handlerNAK_resetCounter();
  handlerNAK_resetState();
  messageIOBuffer_clearAllSlots(&messageInputBuffer);
  userMethods_startReception();
}

/** @brief This method will look into all slots of a buffer to find a message
 *  @param *buf The pointer to the message input or output buffer
 *  @param cmd The command you are looking for
 *  @param *found The pointer to a bool where the function can put the result
 *  @return The first number of the slot (slotID) that it got found.
 *  @return 0 is a possibly valid value, found tells you if it is valid
 */
uint32_t logic_getSlotOfCommand(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buf, uint8_t cmd, bool *found){
  int32_t temp = messageIOBuffer_getOldestSlotWithCommand(buf, cmd);
  *found = FALSE;
  if (temp >= 0){
    *found = TRUE;
    return (uint32_t) temp;
  }
  return 0;
}

/** @brief This method will look into all slots of the input buffer to find your
 *         message.
 *  @param cmd The command you are looking for
 *  @param *found The pointer to a bool where the function can put the result
 *  @return The first number of the slot (slotID) that it got found.
 *  @return 0 is a possibly valid value, found tells you if it is valid
 */
uint32_t logic_getSlotOfCommand_inputBuffer(uint8_t cmd, bool *found){
  return logic_getSlotOfCommand(&messageInputBuffer, cmd, found);
}

/** @brief This method will look into all slots of the output buffer to find your
 *         message.
 *  @param cmd The command you are looking for
 *  @param *found The pointer to a bool where the function can put the result
 *  @return The first number of the slot (slotID) that it got found.
 *  @return 0 is a possibly valid value, found tells you if it is valid
 */
uint32_t logic_getSlotOfCommand_outputBuffer(uint8_t cmd, bool *found){
  return logic_getSlotOfCommand(&messageOutputBuffer, cmd, found);
}

/** @brief This method will clear a specific slot from the output buffer.
 *  @param slotId The ID of the slot you want to clear.
 *  @return TRUE if cleared, FALSE else
 */
bool logic_deletePaketFromOutputBuffer(uint32_t slotId){
  return messageIOBuffer_deleteMessage_bySlotId(&messageOutputBuffer, slotId);
}

/** @brief This method will clear a specific slot from the input buffer.
 *  @param slotId The ID of the slot you want to clear.
 *  @return TRUE if cleared, FALSE else
 */
bool logic_deletePaketFromInputBuffer(uint32_t slotId){
  return messageIOBuffer_deleteMessage_bySlotId(&messageInputBuffer, slotId);
}

/** @brief This method will get the newest message from the input buffer
 *  @param slotId Pointer to a uint32, in which ID of the slot shall be stored
 *  @param cmd Pointer to a byte where the method can store the cmd at
 *  @param paramLength Pointer to a byte where the method can store the datalength at
 *  @param param Pointer to a bytebuffer where the routine can copy the params to
 *  @return TRUE if OK, FALSE else
 */
bool logic_getNewestMessageFromInputBuffer(uint32_t *slotId, uint8_t *cmd, uint32_t *paramLength, uint8_t *param){  
  int32_t temp = messageIOBuffer_getSlotIdNewestMessage(&messageInputBuffer);
  
  if (temp < 0){
    return FALSE;
  }
  
  *slotId = temp;
  *cmd = messageIOBuffer_byteAtPositionInBuffer_bySlotId(&messageInputBuffer, temp, UART_PACKAGE_POSITION_CMD_BYTE);
  *paramLength = messageIOBuffer_byteAtPositionInBuffer_bySlotId(&messageInputBuffer, temp, UART_PACKAGE_POSITION_DATA_LENGTH_BYTE);
  for (int i = 0; i < *paramLength; i++){
    param[i] = messageIOBuffer_byteAtPositionInBuffer_bySlotId(&messageInputBuffer, temp, UART_PACKAGE_POSITION_FIRST_PARAM_BYTE + i);
  }
  
  return TRUE;
}

#if TEST_LOGIC >= 1

// We need some more power for some qewl magic at this point (pushing in some messages)
#include "RingbufferWrapper.h"
#include "UserMethods_UART.h"

int logic_testsuiteReturner(int retVal){
  return retVal;
}

int logic_checkResetConditions(){
  int i;
  
  if (messageInputBuffer.slotCount != LOGIC_MIB_SLOTCOUNT){
    return -1;
  }
  
  if (messageInputBuffer.maxBufferLength != LOGIC_MIB_BUFFERSIZE){
    return -1;
  }
  
  if (messageOutputBuffer.slotCount != LOGIC_MOB_SLOTCOUNT){
    return -1;
  }
  
  if (messageOutputBuffer.maxBufferLength != LOGIC_MOB_BUFFERSIZE){
    return -1;
  }
  
  for (i = 0; i <messageInputBuffer.slotCount; i++){
    if (&messageInputBuffer.slot[i] != &slotsInput[i]){
      return -1;
    }
    if (messageInputBuffer.slot[i].age != 0){
      return -1;
    }
    if (messageInputBuffer.slot[i].transmissionCount != 0){
      return -1;
    }
    if (messageInputBuffer.slot[i].usedLength != 0){
      return -1;
    }
    if (messageInputBuffer.slot[i].valid != FALSE){
      return -1;
    }
    if (messageInputBuffer.slot[i].buffer != &bufferArraysInput[i][0]){
      return -1;
    }
    if (messageInputBuffer.slot[i].timer.flag_active != TIMER_FLAG_INACTIVE){
      return -1;
    }
    if (messageInputBuffer.slot[i].timer.time_targetTime != 0){
      return -1;
    }
    if (messageInputBuffer.slot[i].timer.time_waitTime != 0){
      return -1;
    }
  }
  
  for (i = 0; i <messageOutputBuffer.slotCount; i++){
    if (&messageOutputBuffer.slot[i] != &slotsOutput[i]){
      return -1;
    }
    if (messageOutputBuffer.slot[i].age != 0){
      return -1;
    }
    if (messageOutputBuffer.slot[i].transmissionCount != 0){
      return -1;
    }
    if (messageOutputBuffer.slot[i].usedLength != 0){
      return -1;
    }
    if (messageOutputBuffer.slot[i].valid != FALSE){
      return -1;
    }
    if (messageOutputBuffer.slot[i].buffer != &bufferArraysOutput[i][0]){
      return -1;
    }
    if (messageOutputBuffer.slot[i].timer.flag_active != TIMER_FLAG_INACTIVE){
      return -1;
    }
    if (messageOutputBuffer.slot[i].timer.time_targetTime != 0){
      return -1;
    }
    if (messageOutputBuffer.slot[i].timer.time_waitTime != 0){
      return -1;
    }
  }
  return 0;
}

/** @brief This method is the test for this unit
 *  @return The returnvalue. 0 == OK, <0 == FAILURE
 */
int logic_testsuite(){
  uint32_t i;
  uint8_t testParam[256];
  
  for (i = 0; i < 256; i++){
    testParam[i] = (uint8_t) (i & 0xFF);
  }
  
  // Test system reset
  logic_reset();
  
  // Check the conditions
  if (logic_checkResetConditions() != 0){
    return logic_testsuiteReturner(-1);
  }
  
  // System is now in reset mode, we shall push in some messages and test the 
  // parser returns and if they do their magic with the logic correctly
  // Correct message (not ACK/NAK)
  ringbufferWrapper_putByte(UART_AWAITING_MAGIC);
  ringbufferWrapper_putByte(UART_MSG_CMD_HCI_COMMAND_RESPONSE);
  ringbufferWrapper_putByte(0x01);
  ringbufferWrapper_putByte(0x01);
  ringbufferWrapper_putByte(0xB8);
  // Wrong CRC message
  ringbufferWrapper_putByte(UART_AWAITING_MAGIC);
  ringbufferWrapper_putByte(UART_MSG_CMD_PACK_REC_ACK);
  ringbufferWrapper_putByte(0x00);
  ringbufferWrapper_putByte(0xF8);
  // Correct message (not ACK/NAK)
  ringbufferWrapper_putByte(UART_AWAITING_MAGIC);
  ringbufferWrapper_putByte(UART_MSG_CMD_HCI_COMMAND_RESPONSE);
  ringbufferWrapper_putByte(0x01);
  ringbufferWrapper_putByte(0x01);
  ringbufferWrapper_putByte(0xB8);
  
  // We have 14 Bytes in Buffer. 5 correct 4 wrong 5 correct
  
  // Since we parse the first msg successfully, there are 9 bytes left
  if (logic_parseNachricht(0) != LOGIC_RETURN_NEW_MESSAGE){
    return logic_testsuiteReturner(-1);
  }
  // We try a parse and this will 1.) let us transmit a nak and 2.) drop a byte
  if (logic_parseNachricht(0) != LOGIC_RETURN_NOTHING_NAK){
    return logic_testsuiteReturner(-1);
  }
  // We try a parse again. We're in failure mode, ergo don't send NAK und just drop 2nd byte
  if (logic_parseNachricht(0) != LOGIC_RETURN_NOTHING){
    return logic_testsuiteReturner(-1);
  }
  // We try a parse again. We're in failure mode, ergo don't send NAK und just drop 3rd byte
  if (logic_parseNachricht(0) != LOGIC_RETURN_NOTHING){
    return logic_testsuiteReturner(-1);
  }
  // We try a parse again. We're in failure mode, ergo don't send NAK und just drop 4th byte
  if (logic_parseNachricht(0) != LOGIC_RETURN_NOTHING){
    return logic_testsuiteReturner(-1);
  }
  // Now we reached the 2nd correct msg, this must lead to a success
  if (logic_parseNachricht(0) != LOGIC_RETURN_NEW_MESSAGE){
    return logic_testsuiteReturner(-1);
  }
  
  // Check some analytic functions
  if (logic_countOfMessagesInInputbuffer() != 2){
    return logic_testsuiteReturner(-1);
  }
  
  if (logic_countOfMessagesInOutputbuffer() != 0){
    return logic_testsuiteReturner(-1);
  }
  
  // Send something
  if (logic_transmitMessage(UART_MSG_CMD_HCI_COMMAND, 1, testParam, 2000, 1000) != LOGIC_RETURN_MESSAGE_SENT){
    return logic_testsuiteReturner(-1);
  }
  
  // Now the Out Buffer must have 1 used slot
  if (logic_countOfMessagesInOutputbuffer() != 1){
    return logic_testsuiteReturner(-1);
  }
  
  // Send another one
  if (logic_transmitMessage(UART_MSG_CMD_HCI_COMMAND, 1, testParam, 1000, 1000) != LOGIC_RETURN_MESSAGE_SENT){
    return logic_testsuiteReturner(-1);
  }
  
  // Now the Out Buffer must have 2 used slots
  if (logic_countOfMessagesInOutputbuffer() != 2){
    return logic_testsuiteReturner(-1);
  }
  
  // ACK the first of them
  ringbufferWrapper_putByte(UART_AWAITING_MAGIC);
  ringbufferWrapper_putByte(UART_MSG_CMD_PACK_REC_ACK);
  ringbufferWrapper_putByte(0x00);
  ringbufferWrapper_putByte(0xF9);
  
  // Since we parse the first msg successfully, there are 9 bytes left
  if (logic_parseNachricht(0) != LOGIC_RETURN_NOTHING){
    return logic_testsuiteReturner(-1);
  }
  
  // Now the Out Buffer must have 1 used slot (slot 1, not slot 0!)
  if (logic_countOfMessagesInOutputbuffer() != 1){
    return logic_testsuiteReturner(-1);
  }
  
  // Check timeouting, the left message is the one with 1000 as entry time!
  if (logic_countOfTimeouts(0) != 1){
    return logic_testsuiteReturner(-1);
  }
  if (logic_countOfTimeouts(1000) != 0){
    return logic_testsuiteReturner(-1);
  }
  if (logic_countOfTimeouts(2000) != 0){
    return logic_testsuiteReturner(-1);
  }
  if (logic_countOfTimeouts(2001) != 1){
    return logic_testsuiteReturner(-1);
  }
  
  // Do a handle, thus inc transmissionCount to 2
  if (logic_handleTimeouts(10000) != TRUE){
    return logic_testsuiteReturner(-1);
  }
  if (logic_countOfTimeouts(9999) != 1){
    return logic_testsuiteReturner(-1);
  }
  if (logic_countOfTimeouts(10000) != 0){
    return logic_testsuiteReturner(-1);
  }
  if (logic_countOfTimeouts(11000) != 0){
    return logic_testsuiteReturner(-1);
  }
  if (logic_countOfTimeouts(11001) != 1){
    return logic_testsuiteReturner(-1);
  }
  
  // Do a handle, thus inc transmissionCount to 3
  if (logic_handleTimeouts(100000) != TRUE){
    return logic_testsuiteReturner(-1);
  }
  if (logic_countOfTimeouts(99999) != 1){
    return logic_testsuiteReturner(-1);
  }
  if (logic_countOfTimeouts(100000) != 0){
    return logic_testsuiteReturner(-1);
  }
  if (logic_countOfTimeouts(101000) != 0){
    return logic_testsuiteReturner(-1);
  }
  if (logic_countOfTimeouts(101001) != 1){
    return logic_testsuiteReturner(-1);
  }
  
  // transmissionCount >= 3, thus can't handle
  if (logic_handleTimeouts(1000000) != FALSE){
    return logic_testsuiteReturner(-1);
  }
  
  // delete the output message
  if (logic_deletePaketFromOutputBuffer(1) != TRUE){
    return 0;
  }
  
  // delete message 0 from input buffer
  if (logic_deletePaketFromInputBuffer(0) != TRUE){
    return 0;
  }
  // delete message 1 from input buffer
  if (logic_deletePaketFromInputBuffer(1) != TRUE){
    return 0;
  }
  
  // Check the conditions, we should now be in reset conditions
  if (logic_checkResetConditions() != 0){
    return logic_testsuiteReturner(-1);
  }
  
  // The timer is reset. We now test the parser TO. MIOB is completely reset, thus do nuff
  if (logic_parseNachricht(0) != LOGIC_RETURN_NOTHING){
    return logic_testsuiteReturner(-1);
  }
  ringbufferWrapper_putByte(UART_AWAITING_MAGIC);
  // Now there is a byte in there, so the parser should start the integrity timer
  if (logic_parseNachricht(0) != LOGIC_RETURN_NOTHING){
    return logic_testsuiteReturner(-1);
  }
  // Check out how it reacts in a max case
  if (logic_parseNachricht(0 + PARSER_MESSAGE_INTEGRITY_TIMEOUT_LENGTH_MS) != LOGIC_RETURN_NOTHING){
    return logic_testsuiteReturner(-1);
  }
  // Now it should time out
  if (logic_parseNachricht(1 + PARSER_MESSAGE_INTEGRITY_TIMEOUT_LENGTH_MS) != LOGIC_RETURN_NOTHING_NAK){
    return logic_testsuiteReturner(-1);
  }
  // The timer should be set inactive AND the ringbuffer should be empty, thus asking it again should be NOTHING
  if (logic_parseNachricht(1 + PARSER_MESSAGE_INTEGRITY_TIMEOUT_LENGTH_MS) != LOGIC_RETURN_NOTHING){
    return logic_testsuiteReturner(-1);
  }
  ringbufferWrapper_putByte(UART_AWAITING_MAGIC);
  // The process should be exact the same again: parser should start the integrity timer
  if (logic_parseNachricht(0) != LOGIC_RETURN_NOTHING){
    return logic_testsuiteReturner(-1);
  }
  // Check out how it reacts in a max case
  if (logic_parseNachricht(0 + PARSER_MESSAGE_INTEGRITY_TIMEOUT_LENGTH_MS) != LOGIC_RETURN_NOTHING){
    return logic_testsuiteReturner(-1);
  }
  // Now it should time out
  if (logic_parseNachricht(1 + PARSER_MESSAGE_INTEGRITY_TIMEOUT_LENGTH_MS) != LOGIC_RETURN_NOTHING_NAK){
    return logic_testsuiteReturner(-1);
  }
  // The timer should be set inactive AND the ringbuffer should be empty, thus asking it again should be NOTHING
  if (logic_parseNachricht(1 + PARSER_MESSAGE_INTEGRITY_TIMEOUT_LENGTH_MS) != LOGIC_RETURN_NOTHING){
    return logic_testsuiteReturner(-1);
  }
  ringbufferWrapper_putByte(UART_AWAITING_MAGIC);
  // The process should be exact the same again: parser should start the integrity timer
  if (logic_parseNachricht(0) != LOGIC_RETURN_NOTHING){
    return logic_testsuiteReturner(-1);
  }
  // Check out how it reacts in a max case
  if (logic_parseNachricht(0 + PARSER_MESSAGE_INTEGRITY_TIMEOUT_LENGTH_MS) != LOGIC_RETURN_NOTHING){
    return logic_testsuiteReturner(-1);
  }
  // Now it should time out AND because we got 3 times the wrong case it should alert the upper layer
  if (logic_parseNachricht(1 + PARSER_MESSAGE_INTEGRITY_TIMEOUT_LENGTH_MS) != LOGIC_RETURN_CRITICAL_ERROR){
    return logic_testsuiteReturner(-1);
  }
  
  // Assuming the upper layers logic, the return critical error leads to a complete reset. Let's do this.
  logic_resetEverything();
  // Check the conditions, we should now be in reset conditions
  if (logic_checkResetConditions() != 0){
    return logic_testsuiteReturner(-1);
  }
  
  // Correct message (not ACK/NAK)
  ringbufferWrapper_putByte(UART_AWAITING_MAGIC);
  ringbufferWrapper_putByte(UART_MSG_CMD_HCI_COMMAND_RESPONSE);
  ringbufferWrapper_putByte(0x01);
  ringbufferWrapper_putByte(0x01);
  ringbufferWrapper_putByte(0xB8);
  // Wrong CRC message
  ringbufferWrapper_putByte(UART_AWAITING_MAGIC);
  ringbufferWrapper_putByte(UART_MSG_CMD_PACK_REC_ACK);
  ringbufferWrapper_putByte(0x00);
  ringbufferWrapper_putByte(0xF8);
  // Wrong DL message
  ringbufferWrapper_putByte(UART_AWAITING_MAGIC);
  ringbufferWrapper_putByte(UART_MSG_CMD_PACK_REC_ACK);
  ringbufferWrapper_putByte(0x01);
  ringbufferWrapper_putByte(0xF8);
  // Wrong CMD message
  ringbufferWrapper_putByte(UART_AWAITING_MAGIC);
  ringbufferWrapper_putByte(0x05);
  ringbufferWrapper_putByte(0x00);
  ringbufferWrapper_putByte(0xAC);
  // Wrong Magic message
  ringbufferWrapper_putByte(UART_TRANSMISSION_MAGIC);
  ringbufferWrapper_putByte(UART_MSG_CMD_PACK_REC_ACK);
  ringbufferWrapper_putByte(0x00);
  ringbufferWrapper_putByte(0xF8);
  // Correct message (not ACK/NAK)
  ringbufferWrapper_putByte(UART_AWAITING_MAGIC);
  ringbufferWrapper_putByte(UART_MSG_CMD_HCI_COMMAND_RESPONSE);
  ringbufferWrapper_putByte(0x01);
  ringbufferWrapper_putByte(0x01);
  ringbufferWrapper_putByte(0xB8);
  
  // This message must result in a new message event
  if (logic_parseNachricht(0) != LOGIC_RETURN_NEW_MESSAGE){
    return logic_testsuiteReturner(-1);
  }
  // This message is the first fail. The logic must nak, but once a fail is not that important
  if (logic_parseNachricht(0) != LOGIC_RETURN_NOTHING_NAK){
    return logic_testsuiteReturner(-1);
  }
  // 16 error-message-bytes, 1 is out due to NAK, but now 15 times "Nothing" must follow up
  for (i = 0; i < 15; i++){
    // Since we ignore time at this, a timeout in integrity does not happen...
    if (logic_parseNachricht(0) != LOGIC_RETURN_NOTHING){
      return logic_testsuiteReturner(-1);
    }
  }
  // The last bytes are a good message, thus must result in a new message event
  if (logic_parseNachricht(0) != LOGIC_RETURN_NEW_MESSAGE){
    return logic_testsuiteReturner(-1);
  }
  
  return 0;
}
#endif