/**
  ******************************************************************************
  * @file       MessageIOBuffer.h
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

#ifndef __MESSAGE_IO_BUFFER_H
#define __MESSAGE_IO_BUFFER_H

/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "Test_Selector.h"
#include "Handler_Timer.h"
#include "MasterDefine.h"

/* Typedefinitions */

/**
 * @brief       This struct typedef is the body of a slot.
 *              It holds several thing that are needed to store and handle
 *              incoming and outgoing messages.
 */
typedef struct BUFFER_STRUCT {
  TIMER_STRUCT_TYPEDEF timer;   /**< Timer used to detect timeouts */
  uint8_t transmissionCount;    /**< Counter to count the transmissions */
  uint32_t usedLength;          /**< Used length of the buffer */
  uint8_t *buffer;              /**< Pointer to the buffer to store the message */
  uint32_t age;                 /**< The age of the message */
  bool valid;                   /**< Validity flag of the message slot */
} BUFFER_STRUCT_TYPEDEF;

/**
 * @brief       This struct typedef is the main body of the message input output
 *              buffer. It holds everything needed to store a number of messages.
 */
typedef struct MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT {
  uint32_t slotCount;           /**< Count of slots in this buffer */
  uint32_t maxBufferLength;     /**< Maximum length for the buffers in the slots */
  BUFFER_STRUCT_TYPEDEF *slot;  /**< Pointer array to the slots */
} MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF;

/* Variables */
#define MESSAGE_IO_BUFFER_EOF -1

/* Function definitions */

// MESSAGE-BUFFER-SLOT->BUFFER-WIDE FUNCTIONS

/** @brief This method will reset a single slot to the default values.
 *  @param *slot Pointer to the slot you want to reset.
 *  @return Nothing
 */
void                    messageIOBuffer_resetSlot(BUFFER_STRUCT_TYPEDEF *slot);

/** @brief This method will add a byte to specific slot
 *  @param *buffer Pointer to the MIOB
 *  @param slotId The ID of the slot you want to add a byte to
 *  @param byte The byte you want to add
 *  @return TRUE if OK, FALSE else
 */
bool                    messageIOBuffer_addByteToBuffer_bySlotId(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer,  uint32_t slotId, uint8_t byte);

/** @brief This method will peek at a byte at the buffer within a specific slot
 *  @param *buffer Pointer to the MIOB
 *  @param slotId The ID of the slot you want to get a byte from
 *  @param position The position of the byte within the bufferarray
 *  @return -1 if something went wrong, [0x00 - 0xFF] else
 */
int                     messageIOBuffer_byteAtPositionInBuffer_bySlotId(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer,  uint32_t slotId, uint32_t position);

/** @brief This method will return the pointer to the buffer of the MIOB slot
 *  @param *buffer Pointer to the MIOB
 *  @param slotId The slot ID you want to modify
 *  @return NULL if the slot ID does not exist, a pointer else
 */
uint8_t*                messageIOBuffer_getBuffer_bySlotId(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer, uint32_t slotId);

// SINGLE MESSAGE-BUFFER-SLOT FUNCTIONS

/** @brief This method will set the used bytes of the specific slots buffer
 *  @param *buffer Pointer to the MIOB
 *  @param slotId The ID of the slot you want to manipulate
 *  @param length The new length
 *  @return TRUE if it worked, FALSE else
 */
bool                    messageIOBuffer_setMessageLength_bySlotId(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer,  uint32_t slotId, uint32_t length);

/** @brief This method will read out the used bytes of the specific slots buffer
 *  @param *buffer Pointer to the MIOB
 *  @param slotId The ID of the slot you want get info from
 *  @return >= 0 if it worked, <0 else
 */
int32_t                 messageIOBuffer_getMessageLength_bySlotId(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer,  uint32_t slotId);

/** @brief This method will check whether a slot timed out
 *  @param *buffer Pointer to the MIOB
 *  @param slotId The specific slots ID
 *  @param time The actual time
 *  @return TRUE if it timed out, FALSE if not AND if you asked for a invalid 
 *          slot AND if your slot id is out of bounds
 */
bool                    messageIOBuffer_isSlotTimedOut_bySlotId(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer, uint32_t slotId, uint32_t time);

/** @brief This method reset the timer for the given Slot-ID.
 *         It will check if the message slot is valid and active!
 *  @param *buffer Pointer to the MIOB
 *  @param slotId The slot ID you want to modify
 *  @param time The actual time
 *  @return -1 if not valid, transmissionCount else (count of transmissions)
 */
int32_t                 messageIOBuffer_resetMessageTimeout_bySlotID(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer, uint32_t slotId, uint32_t time);

/** @brief This method will increase the transmission count of a message by 1.
 *         It will check if the message slot is valid and active!
 *  @param *buffer Pointer to the MIOB
 *  @param slotId The slot ID you want to modify
 *  @return -1 if not valid, transmissionCount else (count of transmissions)
 */
int32_t                 messageIOBuffer_incTransmissionCount_bySlotId(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer, uint32_t slotId);

/** @brief This method check if a message slot is valid.
 *  @param *buffer Pointer to the MIOB
 *  @param slotId The slot ID you want to modify
 *  @return FALSE if not valid, TRUE else
 */
bool                    messageIOBuffer_isSlotValid_bySlotID(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer, uint32_t slotId);

/** @brief This method will validate a slot. It will increase the age of all
 *         other slots with valid messages by 1. The new message will be set to
 *         0 transmissions and have its timeout timer started.
 *  @param *buffer Pointer to the MIOB
 *  @param slotId The ID of the slot you want to edit
 *  @param time The actual time (to reset the timeout-timer)
 *  @param timeoutTime The time after which the timeout will occur
 *  @return TRUE if it was successful, FALSE else
 */
bool                    messageIOBuffer_validateMessage_bySlotId(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer,  uint32_t slotId, uint32_t time, uint32_t timeoutTime);

/** @brief This method will reset the specific slot and decrease all other 
 *         message ages by 1. WARNING: YOU MIGHT GET A PROBLEM IF YOU TRY TO
 *         DELETE THE OLDEST MESSAGES AS THE NEWER ONES MIGHT TURN AROUND <0
 *         BE SURE TO KNOW WHAT YOU ARE DOING!
 *  @param *buffer Pointer to the MIOB
 *  @param slotId The ID of the slot you want to delete
 *  @return TRUE if it worked, FALSE else
 */
bool                    messageIOBuffer_deleteMessage_bySlotId(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer,  uint32_t slotId);

/** @brief This method will handle a retransmission of a message in a specific
 *         slot. It will increase the "age" of all messages by and afterwards
 *         set the age of the specific message to 1.
 *  @param *buffer Pointer to the MIOB
 *  @param slotId The ID of the slot you want to edit
 *  @param time The actual time (to reset the timeout-timer)
 *  @param timeoutTime The time after which the timeout will occur
 *  @return TRUE if it was successful, FALSE else
 */
bool                    messageIOBuffer_setValuesRetransmit_bySlotId(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer,  uint32_t slotId, uint32_t time, uint32_t timeoutTime);

// MULTI MESSAGE-BUFFER-SLOT FUNCTIONS

/** @brief This method will ask the message buffer for the count of timed out
 *         messages in used slots
 *  @param *buffer Pointer to the MIOB
 *  @param time The actual time
 *  @return The count of timed out messages
 */
uint32_t                messageIOBuffer_countOfTimeoutedMessages(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer, uint32_t time);

/** @brief This method check the MIOB for a free slot
 *  @param *buffer Pointer to the MIOB
 *  @return >= 0 for a free slot, -1 if full
 */
int32_t                 messageIOBuffer_getFreeSlot(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer);

/** @brief This method will clear all the slots.
 *  @param *buffer Pointer to the MIOB
 *  @return Nothing
 */
void                    messageIOBuffer_clearAllSlots(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer);

/** @brief This method will return the count of used messages
 *  @return The count of used slots.
 */
uint32_t                messageIOBuffer_countOfSlotsUsed(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer);

/** @brief This method will reset the slot with the highest age. It will NOT
 *         touch other messages!
 *  @param *buffer Pointer to the MIOB
 *  @return TRUE if it worked, FALSE else
 */
bool                    messageIOBuffer_deleteMessage_oldest(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer);

/** @brief This method will check if the oldest message timed out
 *  @param *buffer Pointer to the MIOB
 *  @param time The actual time
 *  @return TRUE == message timed out, FALSE else
 */
bool                    messageIOBuffer_isOldestMessageTimeout(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer, uint32_t time);

/** @brief This method will return the slotID of the oldest slot.
 *  @return -1 if there is no valid slot, slotID else.
 */
int32_t                 messageIOBuffer_getSlotIdOldestElement(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer);

/** @brief This method check the MIOB whether there is a message with a specific
 *         command byte.
 *  @param *buffer Pointer to the MIOB
 *  @param cmd The cmd you want to check for
 *  @return TRUE == there is at least 1, FALSE if there is none
 */
bool                    messageIOBuffer_doesMessageWithCommandExist(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer, uint8_t cmd);

/** @brief This method reset the timer for the given Slot-ID.
 *         It will check if the message slot is valid and active!
 *  @param *buffer Pointer to the MIOB
 *  @param slotId The slot ID you want to modify
 *  @param time The actual time
 *  @return -1 if not valid, transmissionCount else (count of transmissions)
 */
int32_t                 messageIOBuffer_getOldestSlotWithCommand(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer, uint8_t cmd);

/** @brief This method will return the id of the slot with the newest entry.
 *  @param *buffer Pointer to the MIOB
 *  @return -1 if there is no message at all, >= 0 else
 */
int32_t                 messageIOBuffer_getSlotIdNewestMessage(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer);

// MESSAGE-BUFFER-WIDE-FUNCTIONS

/** @brief This method will ask the MIOB for the count of slots it has.
 *  @param *buffer Pointer to the MIOB
 *  @return The count of slots.
 */
uint32_t                messageIOBuffer_getSlotCount(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer);

/** @brief This method will ask the MIOB for the smallest buffer size it
 *          contains
 *  @param *buffer Pointer to the MIOB
 *  @return The count of bytes for the smallest buffer
 */
uint32_t                messageIOBuffer_getMaxBufferLength(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer);

/** @brief This method will ask the MIOB for the pointer of the first slot
 *  @param *buffer Pointer to the MIOB
 *  @return The pointer to the first slot
 */
BUFFER_STRUCT_TYPEDEF*  messageIOBuffer_getSlotStruct(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *buffer);


#if TEST_MESSAGEIOBUFFER >= 1
int messageIOBuffer_testsuite();
#endif

#endif