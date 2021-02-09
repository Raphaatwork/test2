/**
  ******************************************************************************
  * @file       MessageIOBuffer.c
  * @author     Tim Steinberg
  * @date       18.05.2020
  * @brief      Message storage for a stack of messages
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
  * | 002       | 2020-05-20    | Tim Steinberg         | Added testsuite                               |
  * |-----------|---------------|-----------------------|-----------------------------------------------|
  * | 003       | 2020-06-16    | Tim Steinberg         | Added comments & doxygen commentaries         |
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
#include "Message_Definitions.h"
#include "Handler_Timer.h"
#include "MessageIOBuffer.h"
#include "Stack_Definitions.h"

/* Typedefinitions */

/* Variables */

/* Function definitions */

/** @brief This method will reset a single slot to the default values.
 *  @param *slot Pointer to the slot you want to reset.
 *  @return Nothing
 */
void messageIOBuffer_resetSlot(BUFFER_STRUCT_TYPEDEF *slot){
  slot->timer.flag_active = TIMER_FLAG_INACTIVE;
  slot->timer.time_targetTime = 0;
  slot->timer.time_waitTime = 0;
  slot->transmissionCount = 0;
  slot->usedLength = 0;
  slot->age = 0;
  slot->valid = FALSE;
}

/** @brief This method will ask the MIOB for the count of slots it has.
 *  @param *buffer Pointer to the MIOB
 *  @return The count of slots.
 */
uint32_t messageIOBuffer_getSlotCount(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer){
  return buffer->slotCount;
}

/** @brief This method will ask the MIOB for the smallest buffer size it
 *          contains
 *  @param *buffer Pointer to the MIOB
 *  @return The count of bytes for the smallest buffer
 */
uint32_t messageIOBuffer_getMaxBufferLength(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer){
  return buffer->maxBufferLength;
}

/** @brief This method will ask the MIOB for the pointer of the first slot
 *  @param *buffer Pointer to the MIOB
 *  @return The pointer to the first slot
 */
BUFFER_STRUCT_TYPEDEF* messageIOBuffer_getSlotStruct(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer){
  return buffer->slot;
}

/** @brief This method will add a byte to specific slot
 *  @param *buffer Pointer to the MIOB
 *  @param slotId The ID of the slot you want to add a byte to
 *  @param byte The byte you want to add
 *  @return TRUE if OK, FALSE else
 */
bool messageIOBuffer_addByteToBuffer_bySlotId(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer,  uint32_t slotId, uint8_t byte){
  if (buffer->slotCount < slotId){
    return FALSE;
  }
  
  if (buffer->slot[slotId].usedLength == buffer->maxBufferLength){
    return FALSE;
  }
  
  buffer->slot[slotId].buffer[buffer->slot[slotId].usedLength] = byte;
  
  buffer->slot[slotId].usedLength += 1;
  
  return TRUE;
}

/** @brief This method will peek at a byte at the buffer within a specific slot
 *  @param *buffer Pointer to the MIOB
 *  @param slotId The ID of the slot you want to get a byte from
 *  @param position The position of the byte within the bufferarray
 *  @return -1 if something went wrong, [0x00 - 0xFF] else
 */
int messageIOBuffer_byteAtPositionInBuffer_bySlotId(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer,  uint32_t slotId, uint32_t position){
  if (buffer->slotCount < slotId){
    return MESSAGE_IO_BUFFER_EOF;
  }
  
  if (buffer->slot[slotId].usedLength < position){
    return MESSAGE_IO_BUFFER_EOF;
  }
  
  return buffer->slot[slotId].buffer[position];
}

/** @brief This method will handle a retransmission of a message in a specific
 *         slot. It will increase the "age" of all messages by and afterwards
 *         set the age of the specific message to 1.
 *  @param *buffer Pointer to the MIOB
 *  @param slotId The ID of the slot you want to edit
 *  @param time The actual time (to reset the timeout-timer)
 *  @param timeoutTime The time after which the timeout will occur
 *  @return TRUE if it was successful, FALSE else
 */
bool messageIOBuffer_setValuesRetransmit_bySlotId(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer,  uint32_t slotId, uint32_t time, uint32_t timeoutTime){
  if (buffer->slotCount < slotId){
    return FALSE;
  }
  
  uint32_t counter = 0;
  
  while(counter < buffer->slotCount){

    if (buffer->slot[counter].valid == TRUE){
      buffer->slot[counter].age += 1;
    }
    
    counter++;
  }
  
  buffer->slot[slotId].valid = TRUE;
  buffer->slot[slotId].age = 1;
  timerHandler_timerStart(&(buffer->slot[slotId].timer), time, timeoutTime);
  return TRUE;
}

/** @brief This method will validate a slot. It will increase the age of all
 *         other slots with valid messages by 1. The new message will be set to
 *         0 transmissions and have its timeout timer started.
 *  @param *buffer Pointer to the MIOB
 *  @param slotId The ID of the slot you want to edit
 *  @param time The actual time (to reset the timeout-timer)
 *  @param timeoutTime The time after which the timeout will occur
 *  @return TRUE if it was successful, FALSE else
 */
bool messageIOBuffer_validateMessage_bySlotId(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer,  uint32_t slotId, uint32_t time, uint32_t timeoutTime){
  if (buffer->slotCount < slotId){
    return FALSE;
  }
  
  uint32_t counter = 0;
  
  while(counter < buffer->slotCount){

    if (buffer->slot[counter].valid == TRUE){
      buffer->slot[counter].age += 1;
    }
    
    counter++;
  }
  
  buffer->slot[slotId].valid = TRUE;
  buffer->slot[slotId].age = 1;
  timerHandler_timerStart(&(buffer->slot[slotId].timer), time, timeoutTime);
  buffer->slot[slotId].transmissionCount = 0;
  return TRUE;
}

/** @brief This method will read out the used bytes of the specific slots buffer
 *  @param *buffer Pointer to the MIOB
 *  @param slotId The ID of the slot you want get info from
 *  @return >= 0 if it worked, <0 else
 */
int32_t messageIOBuffer_getMessageLength_bySlotId(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer,  uint32_t slotId){
  if (buffer->slotCount < slotId){
    return MESSAGE_IO_BUFFER_EOF;
  }
  
  if (buffer->slot[slotId].valid == FALSE){
    return MESSAGE_IO_BUFFER_EOF;
  }
  
  return buffer->slot[slotId].usedLength;
}

/** @brief This method will set the used bytes of the specific slots buffer
 *  @param *buffer Pointer to the MIOB
 *  @param slotId The ID of the slot you want to manipulate
 *  @param length The new length
 *  @return TRUE if it worked, FALSE else
 */
bool messageIOBuffer_setMessageLength_bySlotId(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer,  uint32_t slotId, uint32_t length){
  if (buffer->slotCount < slotId){
    return FALSE;
  }
  
  if (buffer->maxBufferLength < length){
    return FALSE;
  }
  
  buffer->slot[slotId].usedLength = length;
  
  return TRUE;
}

/** @brief This method will reset the specific slot and decrease all other 
 *         message ages by 1. WARNING: YOU MIGHT GET A PROBLEM IF YOU TRY TO
 *         DELETE THE OLDEST MESSAGES AS THE NEWER ONES MIGHT TURN AROUND <0
 *         BE SURE TO KNOW WHAT YOU ARE DOING!
 *  @param *buffer Pointer to the MIOB
 *  @param slotId The ID of the slot you want to delete
 *  @return TRUE if it worked, FALSE else
 */
bool messageIOBuffer_deleteMessage_bySlotId(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer,  uint32_t slotId){
  if (buffer->slotCount < slotId){
    return FALSE;
  }
  
  if (buffer->slot[slotId].valid == FALSE){
    return FALSE;
  }
  
  uint32_t counter = 0;
  
  while(counter < buffer->slotCount){
    if (buffer->slot[counter].age > buffer->slot[slotId].age){
      buffer->slot[counter].age -= 1;
    }
    
    counter++;
  }
  
  messageIOBuffer_resetSlot(&buffer->slot[slotId]);
  
  return TRUE;
}

/** @brief This method will reset the slot with the highest age. It will NOT
 *         touch other messages!
 *  @param *buffer Pointer to the MIOB
 *  @return TRUE if it worked, FALSE else
 */
bool messageIOBuffer_deleteMessage_oldest(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer){
  int32_t temp = messageIOBuffer_getSlotIdOldestElement(buffer);
  
  if (temp >= 0){
    messageIOBuffer_resetSlot(&buffer->slot[temp]);
    return TRUE;
  }
    
  return FALSE;
}

/** @brief This method will check whether a slot timed out
 *  @param *buffer Pointer to the MIOB
 *  @param slotId The specific slots ID
 *  @param time The actual time
 *  @return TRUE if it timed out, FALSE if not AND if you asked for a invalid 
 *          slot AND if your slot id is out of bounds
 */
bool messageIOBuffer_isSlotTimedOut_bySlotId(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer, uint32_t slotId, uint32_t time){
  if (buffer->slotCount < slotId){
    return FALSE;
  }
  
  if (buffer->slot[slotId].valid == FALSE){
    return FALSE;
  }
  
  return timerHandler_isTimerTimedOut(&(buffer->slot[slotId].timer), time);
}

/** @brief This method will ask the message buffer for the count of timed out
 *         messages in used slots
 *  @param *buffer Pointer to the MIOB
 *  @param time The actual time
 *  @return The count of timed out messages
 */
uint32_t messageIOBuffer_countOfTimeoutedMessages(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer, uint32_t time){
  uint32_t counter = 0;
  uint32_t timeoutCounter = 0;
  
  while(counter < buffer->slotCount){
    if (timerHandler_isTimerTimedOut(&(buffer->slot[counter].timer), time) == TRUE){
      timeoutCounter += 1;
    }
    
    counter += 1;
  }
  
  return timeoutCounter;
}

/** @brief This method will check if the oldest message timed out
 *  @param *buffer Pointer to the MIOB
 *  @param time The actual time
 *  @return TRUE == message timed out, FALSE else
 */
bool messageIOBuffer_isOldestMessageTimeout(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer, uint32_t time){
  int32_t temp = messageIOBuffer_getSlotIdOldestElement(buffer);
  
  if (temp >= 0){
    return timerHandler_isTimerTimedOut(&(buffer->slot[temp].timer), time);
  }
  
  return FALSE;
}

/** @brief This method reset the timer for the given Slot-ID.
 *         It will check if the message slot is valid and active!
 *  @param *buffer Pointer to the MIOB
 *  @param slotId The slot ID you want to modify
 *  @param time The actual time
 *  @return -1 if not valid, transmissionCount else (count of transmissions)
 */
int32_t messageIOBuffer_resetMessageTimeout_bySlotID(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer, uint32_t slotId, uint32_t time){
  if (buffer->slotCount < slotId){
    return MESSAGE_IO_BUFFER_EOF;
  }
  
  if (buffer->slot[slotId].valid == FALSE){
    return MESSAGE_IO_BUFFER_EOF;
  }
  
  timerHandler_timerRestart(&(buffer->slot[slotId].timer), time);
  
  buffer->slot[slotId].transmissionCount += 1;
  
  return buffer->slot[slotId].transmissionCount;
}

/** @brief This method will return the slotID of the oldest slot.
 *  @return -1 if there is no valid slot, slotID else.
 */
int32_t messageIOBuffer_getSlotIdOldestElement(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer){
  uint32_t counter = 0;
  int32_t temp = 0;
  int32_t retVal = -1;
  
  while (counter < buffer->slotCount){
    if (buffer->slot[counter].age > temp){
      temp = buffer->slot[counter].age;
      retVal = counter;
    }
    
    counter++;
  }
  return retVal;
}

/** @brief This method will increase the transmission count of a message by 1.
 *         It will check if the message slot is valid and active!
 *  @param *buffer Pointer to the MIOB
 *  @param slotId The slot ID you want to modify
 *  @return -1 if not valid, transmissionCount else (count of transmissions)
 */
int32_t messageIOBuffer_incTransmissionCount_bySlotId(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer, uint32_t slotId){
  if (buffer->slot[slotId].valid == FALSE){
    return MESSAGE_IO_BUFFER_EOF;
  }
  
  buffer->slot[slotId].transmissionCount += 1;
  
  return buffer->slot[slotId].transmissionCount;
}

/** @brief This method check if a message slot is valid.
 *  @param *buffer Pointer to the MIOB
 *  @param slotId The slot ID you want to modify
 *  @return FALSE if not valid, TRUE else
 */
bool messageIOBuffer_isSlotValid_bySlotID(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer, uint32_t slotId){
  if (buffer->slot[slotId].valid == FALSE){
    return FALSE;
  }
  
  return buffer->slot[slotId].valid;
}

/** @brief This method check the MIOB for a free slot
 *  @param *buffer Pointer to the MIOB
 *  @return >= 0 for a free slot, -1 if full
 */
int32_t messageIOBuffer_getFreeSlot(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer){
  uint32_t temp = 0;
  
  while(temp < buffer->slotCount){
    if (buffer->slot[temp].valid == FALSE){
      return temp;
    }
    
    temp += 1;
  }
  
  return MESSAGE_IO_BUFFER_EOF;
}

/** @brief This method will clear all the slots.
 *  @param *buffer Pointer to the MIOB
 *  @return Nothing
 */
void messageIOBuffer_clearAllSlots(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer){
  uint32_t counter = 0;
  
  while(counter < buffer->slotCount){
    messageIOBuffer_resetSlot(&buffer->slot[counter]);
    counter += 1;
  }
}

/** @brief This method will return the count of used messages
 *  @return The count of used slots.
 */
uint32_t messageIOBuffer_countOfSlotsUsed(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer){
  uint32_t counter = 0;
  uint32_t temp = 0;
  
  while(counter < buffer->slotCount){
    if (buffer->slot[counter].valid == TRUE){
      temp += 1;
    }
    
    counter += 1;
  }
  return temp;
}

/** @brief This method check the MIOB whether there is a message with a specific
 *         command byte.
 *  @param *buffer Pointer to the MIOB
 *  @param cmd The cmd you want to check for
 *  @return TRUE == there is at least 1, FALSE if there is none
 */
bool messageIOBuffer_doesMessageWithCommandExist(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer, uint8_t cmd){
  uint32_t counter = 0;
   
  while(counter < buffer->slotCount){
    if (buffer->slot[counter].valid == TRUE){
      if (buffer->slot[counter].buffer[UART_PACKAGE_POSITION_CMD_BYTE] == cmd){
        return TRUE;
      }
    }
    counter += 1;
  }
  
  return FALSE;
}

/** @brief This method reset the timer for the given Slot-ID.
 *         It will check if the message slot is valid and active!
 *  @param *buffer Pointer to the MIOB
 *  @param slotId The slot ID you want to modify
 *  @param time The actual time
 *  @return -1 if not valid, transmissionCount else (count of transmissions)
 */
int32_t messageIOBuffer_getOldestSlotWithCommand(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer, uint8_t cmd){
  int32_t temp = -1;
  uint32_t counter = 0;
  int32_t age = 0;
  
  while(counter < buffer->slotCount){
    if (buffer->slot[counter].buffer[UART_PACKAGE_POSITION_CMD_BYTE] == cmd){
      if (buffer->slot[counter].age > age){
        temp = counter;
        age = buffer->slot[counter].age;
      }
    }
    
    counter++;
  }
  
  return temp;
}

/** @brief This method will return the pointer to the buffer of the MIOB slot
 *  @param *buffer Pointer to the MIOB
 *  @param slotId The slot ID you want to modify
 *  @return NULL if the slot ID does not exist, a pointer else
 */
uint8_t* messageIOBuffer_getBuffer_bySlotId(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer, uint32_t slotId){
  if (buffer->slotCount < slotId){
    return NULL;
  }
  return buffer->slot[slotId].buffer;
}

/** @brief This method will return the id of the slot with the newest entry.
 *  @param *buffer Pointer to the MIOB
 *  @return -1 if there is no message at all, >= 0 else
 */
int32_t messageIOBuffer_getSlotIdNewestMessage(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer){
  int32_t counter = 0;
  uint32_t temp = 0xFFFFFFFF;
  int32_t retVal = -1;
  
  while(counter < buffer->slotCount){
    if (buffer->slot[counter].valid == TRUE){
      if (buffer->slot[counter].age < temp){
        retVal = counter;
        temp = buffer->slot[counter].age;
      }
    }
    
    counter += 1;
  }
  
  return retVal;
}

#if TEST_MESSAGEIOBUFFER >= 1

void messageIOBuffer_slotInit(BUFFER_STRUCT_TYPEDEF *slot, uint8_t *buffer){
  slot->transmissionCount = 0;
  slot->usedLength = 0;
  slot->buffer = buffer;
  slot->age = 0;
  slot->valid = FALSE;
}

void messageIOBuffer_reset(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *miob){
  miob->maxBufferLength = MESSAGEIOBUFFER_TEST_BUFFERSIZE;
  miob->slotCount = MESSAGEIOBUFFER_TEST_SLOTCOUNT;
  for (int i = 0; i < MESSAGEIOBUFFER_TEST_SLOTCOUNT; i++){
    miob->slot[i].transmissionCount = 0;
    miob->slot[i].usedLength = 0;
    miob->slot[i].age = 0;
    miob->slot[i].valid = FALSE;
  }
}

int messageIOBuffer_testsuiteReturner(int retVal){
  return retVal;
}

int messageIOBuffer_testsuite(){
  uint32_t i;
  uint8_t bufferArrays[MESSAGEIOBUFFER_TEST_SLOTCOUNT][MESSAGEIOBUFFER_TEST_BUFFERSIZE];
  BUFFER_STRUCT_TYPEDEF slots[MESSAGEIOBUFFER_TEST_SLOTCOUNT];
  
  for (i = 0; i < MESSAGEIOBUFFER_TEST_SLOTCOUNT; i++){
    messageIOBuffer_slotInit(&slots[i], bufferArrays[i]);
  }
  
  MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF miob = {
    .slotCount = MESSAGEIOBUFFER_TEST_SLOTCOUNT,
    .maxBufferLength = MESSAGEIOBUFFER_TEST_BUFFERSIZE,
    .slot = slots,
  };
    
  messageIOBuffer_reset(&miob);

// TEST THE BASIC ACCESS FUNCTIONS
  
  if (messageIOBuffer_getSlotCount(&miob) != MESSAGEIOBUFFER_TEST_SLOTCOUNT){
    return messageIOBuffer_testsuiteReturner(-1);
  }

  if (messageIOBuffer_getMaxBufferLength(&miob) != MESSAGEIOBUFFER_TEST_BUFFERSIZE){
    return messageIOBuffer_testsuiteReturner(-1);
  }

  if (messageIOBuffer_getSlotStruct(&miob) != &slots[0]){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  
// TEST SLOT-BUFFER-WIDE FUNCTIONS
  //uint8_t*                messageIOBuffer_getBuffer_bySlotId(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer, uint32_t slotId);
  for (i = 0; i < MESSAGEIOBUFFER_TEST_SLOTCOUNT; i++){
    if (messageIOBuffer_getBuffer_bySlotId(&miob, i) != bufferArrays[i]){
      return messageIOBuffer_testsuiteReturner(-1);
    }
  }
  //bool                    messageIOBuffer_addByteToBuffer_bySlotId(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer,  uint32_t slotId, uint8_t byte);
  uint8_t *myBuf = messageIOBuffer_getBuffer_bySlotId(&miob, 6);
  for (i = 0; i < MESSAGEIOBUFFER_TEST_BUFFERSIZE; i++){
    if (messageIOBuffer_addByteToBuffer_bySlotId(&miob, 6, (uint8_t) (i & 0xFF)) != TRUE){
      return messageIOBuffer_testsuiteReturner(-1);
    }
  }
  for (i = 0; i < MESSAGEIOBUFFER_TEST_BUFFERSIZE >> 1; i++){
    if (messageIOBuffer_addByteToBuffer_bySlotId(&miob, 5, (uint8_t) (i & 0xFF)) != TRUE){
      return messageIOBuffer_testsuiteReturner(-1);
    }
  }
  for (i = 0; i < MESSAGEIOBUFFER_TEST_BUFFERSIZE >> 2; i++){
    if (messageIOBuffer_addByteToBuffer_bySlotId(&miob, 4, (uint8_t) (i & 0xFF)) != TRUE){
      return messageIOBuffer_testsuiteReturner(-1);
    }
  }
  for (i = 0; i < MESSAGEIOBUFFER_TEST_BUFFERSIZE >> 3; i++){
    if (messageIOBuffer_addByteToBuffer_bySlotId(&miob, 3, (uint8_t) (i & 0xFF)) != TRUE){
      return messageIOBuffer_testsuiteReturner(-1);
    }
  }
  for (i = 0; i < MESSAGEIOBUFFER_TEST_BUFFERSIZE >> 4; i++){
    if (messageIOBuffer_addByteToBuffer_bySlotId(&miob, 2, (uint8_t) (i & 0xFF)) != TRUE){
      return messageIOBuffer_testsuiteReturner(-1);
    }
  }
  for (i = 0; i < MESSAGEIOBUFFER_TEST_BUFFERSIZE >> 5; i++){
    if (messageIOBuffer_addByteToBuffer_bySlotId(&miob, 1, (uint8_t) (i & 0xFF)) != TRUE){
      return messageIOBuffer_testsuiteReturner(-1);
    }
  }
  
  //int                     messageIOBuffer_byteAtPositionInBuffer_bySlotId(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer,  uint32_t slotId, uint32_t position);
  for (i = 0; i < MESSAGEIOBUFFER_TEST_BUFFERSIZE; i++){
    if (messageIOBuffer_byteAtPositionInBuffer_bySlotId(&miob, 6, i) != (uint8_t) (i & 0xFF)){
      return messageIOBuffer_testsuiteReturner(-1);
    }
  }
  for (i = 0; i < MESSAGEIOBUFFER_TEST_BUFFERSIZE >> 1; i++){
    if (messageIOBuffer_byteAtPositionInBuffer_bySlotId(&miob, 5, i) != (uint8_t) (i & 0xFF)){
      return messageIOBuffer_testsuiteReturner(-1);
    }
  }
  for (i = 0; i < MESSAGEIOBUFFER_TEST_BUFFERSIZE >> 2; i++){
    if (messageIOBuffer_byteAtPositionInBuffer_bySlotId(&miob, 4, i) != (uint8_t) (i & 0xFF)){
      return messageIOBuffer_testsuiteReturner(-1);
    }
  }
  for (i = 0; i < MESSAGEIOBUFFER_TEST_BUFFERSIZE >> 3; i++){
    if (messageIOBuffer_byteAtPositionInBuffer_bySlotId(&miob, 3, i) != (uint8_t) (i & 0xFF)){
      return messageIOBuffer_testsuiteReturner(-1);
    }
  }
  for (i = 0; i < MESSAGEIOBUFFER_TEST_BUFFERSIZE >> 4; i++){
    if (messageIOBuffer_byteAtPositionInBuffer_bySlotId(&miob, 2, i) != (uint8_t) (i & 0xFF)){
      return messageIOBuffer_testsuiteReturner(-1);
    }
  }
  for (i = 0; i < MESSAGEIOBUFFER_TEST_BUFFERSIZE >> 5; i++){
    if (messageIOBuffer_byteAtPositionInBuffer_bySlotId(&miob, 1, i) != (uint8_t) (i & 0xFF)){
      return messageIOBuffer_testsuiteReturner(-1);
    }
  }

// TEST SLOT-WIDE FUNCTIONS
  //bool                    messageIOBuffer_isSlotValid_bySlotID(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer, uint32_t slotId);
  for (i = 0; i < miob.slotCount; i++){
    if (messageIOBuffer_isSlotValid_bySlotID(&miob, i) != FALSE){
      return messageIOBuffer_testsuiteReturner(-1);
    }
  }
  //bool                    messageIOBuffer_validateMessage_bySlotId(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer,  uint32_t slotId, uint32_t time, uint32_t timeoutTime);
  if (messageIOBuffer_validateMessage_bySlotId(&miob, 1, 100, 1000) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_validateMessage_bySlotId(&miob, 2, 100, 1000) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_validateMessage_bySlotId(&miob, 3, 100, 1000) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_validateMessage_bySlotId(&miob, 4, 100, 1000) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_validateMessage_bySlotId(&miob, 5, 100, 1000) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_validateMessage_bySlotId(&miob, 6, 100, 1000) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  // Check set
  for (i = 1; i < 7; i++){
    if (messageIOBuffer_isSlotValid_bySlotID(&miob, i) != TRUE){
      return messageIOBuffer_testsuiteReturner(-1);
    }
  }
  //int32_t                 messageIOBuffer_getMessageLength_bySlotId(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer,  uint32_t slotId);
  if (messageIOBuffer_getMessageLength_bySlotId(&miob, 1) != (MESSAGEIOBUFFER_TEST_BUFFERSIZE >> 5)){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_getMessageLength_bySlotId(&miob, 2) != (MESSAGEIOBUFFER_TEST_BUFFERSIZE >> 4)){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_getMessageLength_bySlotId(&miob, 3) != (MESSAGEIOBUFFER_TEST_BUFFERSIZE >> 3)){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_getMessageLength_bySlotId(&miob, 4) != (MESSAGEIOBUFFER_TEST_BUFFERSIZE >> 2)){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_getMessageLength_bySlotId(&miob, 5) != (MESSAGEIOBUFFER_TEST_BUFFERSIZE >> 1)){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_getMessageLength_bySlotId(&miob, 6) != MESSAGEIOBUFFER_TEST_BUFFERSIZE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  
  //bool                    messageIOBuffer_setMessageLength_bySlotId(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer,  uint32_t slotId, uint32_t length);
  // set length to 0 must be valid
  if (messageIOBuffer_setMessageLength_bySlotId(&miob, 6, 0) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_getMessageLength_bySlotId(&miob, 6) != 0){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  // set length to MAX must be valid
  if (messageIOBuffer_setMessageLength_bySlotId(&miob, 6, MESSAGEIOBUFFER_TEST_BUFFERSIZE) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_getMessageLength_bySlotId(&miob, 6) != MESSAGEIOBUFFER_TEST_BUFFERSIZE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  // set length to >MAX must be invalid and rejected
  if (messageIOBuffer_setMessageLength_bySlotId(&miob, 6, MESSAGEIOBUFFER_TEST_BUFFERSIZE + 1) != FALSE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_getMessageLength_bySlotId(&miob, 6) != MESSAGEIOBUFFER_TEST_BUFFERSIZE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  
  //bool                    messageIOBuffer_isSlotTimedOut_bySlotId(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer, uint32_t slotId, uint32_t time);
  // Check NO turnaround, but after-turnaround close edge
  if (messageIOBuffer_validateMessage_bySlotId(&miob, 6, 100, 1000) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_isSlotTimedOut_bySlotId(&miob, 6, 99) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_isSlotTimedOut_bySlotId(&miob, 6, 100) != FALSE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_isSlotTimedOut_bySlotId(&miob, 6, 1099) != FALSE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_isSlotTimedOut_bySlotId(&miob, 6, 1100) != FALSE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_isSlotTimedOut_bySlotId(&miob, 6, 1101) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  // Check somewhere in the middle
  if (messageIOBuffer_validateMessage_bySlotId(&miob, 6, 0x0F000000, 0x1000) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_isSlotTimedOut_bySlotId(&miob, 6, 0x0EFFFFFF) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_isSlotTimedOut_bySlotId(&miob, 6, 0x0F000000) != FALSE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_isSlotTimedOut_bySlotId(&miob, 6, 0x0F000FFF) != FALSE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_isSlotTimedOut_bySlotId(&miob, 6, 0x0F001000) != FALSE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_isSlotTimedOut_bySlotId(&miob, 6, 0x0F001001) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  // Close to turnaround 
  if (messageIOBuffer_validateMessage_bySlotId(&miob, 6, 0xFFFFF000, 0x0100) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_isSlotTimedOut_bySlotId(&miob, 6, 0xFFFFEFFF) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_isSlotTimedOut_bySlotId(&miob, 6, 0xFFFFF000) != FALSE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_isSlotTimedOut_bySlotId(&miob, 6, 0xFFFFF0FF) != FALSE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_isSlotTimedOut_bySlotId(&miob, 6, 0xFFFFF100) != FALSE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_isSlotTimedOut_bySlotId(&miob, 6, 0xFFFFF101) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  // In turnaround 
  if (messageIOBuffer_validateMessage_bySlotId(&miob, 6, 0xFFFFFF00, 0x0200) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_isSlotTimedOut_bySlotId(&miob, 6, 0xFFFFFEFF) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_isSlotTimedOut_bySlotId(&miob, 6, 0xFFFFFFFF) != FALSE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_isSlotTimedOut_bySlotId(&miob, 6, 0x00000000) != FALSE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_isSlotTimedOut_bySlotId(&miob, 6, 0x00000100) != FALSE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_isSlotTimedOut_bySlotId(&miob, 6, 0x00000101) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }

  //int32_t                 messageIOBuffer_resetMessageTimeout_bySlotID(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer, uint32_t slotId, uint32_t time);
  // Resent an invalid buffer is NOT allowed -> must fail
  if (messageIOBuffer_resetMessageTimeout_bySlotID(&miob, 9, 1000) != MESSAGE_IO_BUFFER_EOF){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_validateMessage_bySlotId(&miob, 6, 1000, 1000) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_resetMessageTimeout_bySlotID(&miob, 6, 1000) != 1){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_resetMessageTimeout_bySlotID(&miob, 6, 1000) != 2){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_resetMessageTimeout_bySlotID(&miob, 6, 1000) != 3){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_isSlotTimedOut_bySlotId(&miob, 6, 999) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_isSlotTimedOut_bySlotId(&miob, 6, 1000) != FALSE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_isSlotTimedOut_bySlotId(&miob, 6, 1999) != FALSE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_isSlotTimedOut_bySlotId(&miob, 6, 2000) != FALSE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_isSlotTimedOut_bySlotId(&miob, 6, 2001) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  
  //int32_t                 messageIOBuffer_incTransmissionCount_bySlotId(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer, uint32_t slotId);
  if (messageIOBuffer_incTransmissionCount_bySlotId(&miob, 6) != 4){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_incTransmissionCount_bySlotId(&miob, 9) != MESSAGE_IO_BUFFER_EOF){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  
  //bool                    messageIOBuffer_deleteMessage_bySlotId(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer,  uint32_t slotId);
  if (messageIOBuffer_deleteMessage_bySlotId(&miob, 6) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_deleteMessage_bySlotId(&miob, 9) != FALSE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  
// TEST BUFFER-WIDE FUNCTIONS
  messageIOBuffer_clearAllSlots(&miob);
  //Test conditions after clear
  if (messageIOBuffer_countOfTimeoutedMessages(&miob, 100) != 0){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_countOfSlotsUsed(&miob) != 0){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_deleteMessage_oldest(&miob) != FALSE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_isOldestMessageTimeout(&miob, 100) != FALSE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  for (i = 0; i < 255; i++){
    if (messageIOBuffer_doesMessageWithCommandExist(&miob, (uint8_t) (i & 0xFF)) != FALSE){
      return messageIOBuffer_testsuiteReturner(-1);
    }
  }
  for (i = 0; i < 255; i++){
    if (messageIOBuffer_getOldestSlotWithCommand(&miob, (uint8_t) (i & 0xFF)) != -1){
      return messageIOBuffer_testsuiteReturner(-1);
    }
  }
  if (messageIOBuffer_getSlotIdNewestMessage(&miob) != -1){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  
  // Set some test slots
  if (messageIOBuffer_addByteToBuffer_bySlotId(&miob, 0, 0x03) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_addByteToBuffer_bySlotId(&miob, 0, 0x50) != TRUE){ // UART_MSG_CMD_PACK_REC_ACK
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_addByteToBuffer_bySlotId(&miob, 0, 0x00) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_addByteToBuffer_bySlotId(&miob, 0, 0xF8) != TRUE){ // CRC wrong
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_validateMessage_bySlotId(&miob, 0, 1000, 1000) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_addByteToBuffer_bySlotId(&miob, 1, 0x03) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_addByteToBuffer_bySlotId(&miob, 1, 0x50) != TRUE){ // UART_MSG_CMD_PACK_REC_ACK
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_addByteToBuffer_bySlotId(&miob, 1, 0x00) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_addByteToBuffer_bySlotId(&miob, 1, 0xF9) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_validateMessage_bySlotId(&miob, 1, 1000, 1000) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }

  //uint32_t                messageIOBuffer_countOfTimeoutedMessages(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer, uint32_t time);
  if (messageIOBuffer_countOfTimeoutedMessages(&miob, 0) != 2){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_countOfTimeoutedMessages(&miob, 1000) != 0){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_countOfTimeoutedMessages(&miob, 2000) != 0){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_countOfTimeoutedMessages(&miob, 2001) != 2){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  
  //int32_t                 messageIOBuffer_getFreeSlot(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer);
  if (messageIOBuffer_countOfSlotsUsed(&miob) != 2){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_getFreeSlot(&miob) != 2){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  
  //bool                    messageIOBuffer_deleteMessage_oldest(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer);
  if (messageIOBuffer_deleteMessage_oldest(&miob) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_isSlotValid_bySlotID(&miob, 0) != FALSE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_isSlotValid_bySlotID(&miob, 1) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_getSlotIdOldestElement(&miob) != 1){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_getSlotIdNewestMessage(&miob) != 1){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  
  if (messageIOBuffer_addByteToBuffer_bySlotId(&miob, 0, 0x03) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_addByteToBuffer_bySlotId(&miob, 0, 0x50) != TRUE){ // UART_MSG_CMD_PACK_REC_ACK
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_addByteToBuffer_bySlotId(&miob, 0, 0x00) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_addByteToBuffer_bySlotId(&miob, 0, 0xF8) != TRUE){ // CRC wrong
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_validateMessage_bySlotId(&miob, 0, 1000, 1000) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  
  if (messageIOBuffer_isSlotValid_bySlotID(&miob, 0) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_getSlotIdOldestElement(&miob) != 1){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_getSlotIdNewestMessage(&miob) != 0){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  
  if (messageIOBuffer_addByteToBuffer_bySlotId(&miob, 3, 0x03) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_addByteToBuffer_bySlotId(&miob, 3, 0x50) != TRUE){ // UART_MSG_CMD_PACK_REC_ACK
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_addByteToBuffer_bySlotId(&miob, 3, 0x00) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_addByteToBuffer_bySlotId(&miob, 3, 0xF8) != TRUE){ // CRC wrong
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_validateMessage_bySlotId(&miob, 3, 1000, 1000) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  
  if (messageIOBuffer_addByteToBuffer_bySlotId(&miob, 5, 0x03) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_addByteToBuffer_bySlotId(&miob, 5, 0x50) != TRUE){ // UART_MSG_CMD_PACK_REC_ACK
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_addByteToBuffer_bySlotId(&miob, 5, 0x00) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_addByteToBuffer_bySlotId(&miob, 5, 0xF8) != TRUE){ // CRC wrong
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_validateMessage_bySlotId(&miob, 5, 1000, 1000) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  
  if (messageIOBuffer_getOldestSlotWithCommand(&miob, 0x50) != 1){ // CRC wrong
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_getSlotIdNewestMessage(&miob) != 5){ // CRC wrong
    return messageIOBuffer_testsuiteReturner(-1);
  }
  
  messageIOBuffer_clearAllSlots(&miob);
  if (messageIOBuffer_addByteToBuffer_bySlotId(&miob, 0, 0x03) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_addByteToBuffer_bySlotId(&miob, 0, 0x50) != TRUE){ // UART_MSG_CMD_PACK_REC_ACK
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_addByteToBuffer_bySlotId(&miob, 0, 0x00) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_addByteToBuffer_bySlotId(&miob, 0, 0xF8) != TRUE){ // CRC wrong
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_validateMessage_bySlotId(&miob, 0, 1000, 1000) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_addByteToBuffer_bySlotId(&miob, 1, 0x03) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_addByteToBuffer_bySlotId(&miob, 1, 0x50) != TRUE){ // UART_MSG_CMD_PACK_REC_ACK
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_addByteToBuffer_bySlotId(&miob, 1, 0x00) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_addByteToBuffer_bySlotId(&miob, 1, 0xF8) != TRUE){ // CRC wrong
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_validateMessage_bySlotId(&miob, 1, 2000, 1000) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_addByteToBuffer_bySlotId(&miob, 3, 0x03) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_addByteToBuffer_bySlotId(&miob, 3, 0x50) != TRUE){ // UART_MSG_CMD_PACK_REC_ACK
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_addByteToBuffer_bySlotId(&miob, 3, 0x00) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_addByteToBuffer_bySlotId(&miob, 3, 0xF8) != TRUE){ // CRC wrong
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_validateMessage_bySlotId(&miob, 3, 3000, 1000) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_isOldestMessageTimeout(&miob, 999) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_isOldestMessageTimeout(&miob, 1000) != FALSE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_isOldestMessageTimeout(&miob, 1999) != FALSE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_isOldestMessageTimeout(&miob, 2000) != FALSE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  if (messageIOBuffer_isOldestMessageTimeout(&miob, 2001) != TRUE){
    return messageIOBuffer_testsuiteReturner(-1);
  }
  
  return 0;
}
#endif