/**
  ******************************************************************************
  * @file       Logic.h
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

#ifndef __LOGIC_H
#define __LOGIC_H

/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "Test_Selector.h"

/* Typedefinitions */
/** *@brief The return values of the logic function 
  * NOTHING        = nothing new happened
  * NOTHING_NAK    = nothing new happened, but send a NAK
  * NEW_MESSAGE    = we have a new message
  * MESSAGE_SENT   = a message was successfully transmitted
  * CRITICAL_ERROR = a critical error occured
  */
typedef enum LOGIC_RETURN_VALUES {
  LOGIC_RETURN_NOTHING                  = 0x00, /**< Nothing new happened */
  LOGIC_RETURN_NOTHING_NAK              = 0x01, /**< Nothing new happened, but a NAK was transmitted */
  LOGIC_RETURN_NEW_MESSAGE              = 0x80, /**< There is a new message in the MIOB */
  LOGIC_RETURN_MESSAGE_SENT             = 0x81, /**< A message was transmitted */
  LOGIC_RETURN_CRITICAL_ERROR           = 0xFF, /**< A critical error occured */
} LOGIC_RETURN_VALUES_TYPEDEF;

/* Variables */

/* Function definitions */

/** @brief This method will reset the logic systems values
 *  @return Nothing.
 */
void                            logic_reset();

/** @brief This method will count all timed out slots of the output buffer
 *  @param time The actual time
 *  @return Nothing.
 */
uint32_t                        logic_countOfTimeouts(uint32_t time);

/** @brief This method will handle all timeouts (by retransmitting them)
 *  @param time The actual time
 *  @return TRUE if all could be handled
 *  @return FALSE if there is a retransmit-overflow, meaning too often no ACK
 */
bool                            logic_handleTimeouts(uint32_t time);

/** @brief This method will return the count of messages in the input buffer
 *  @return The count of slots used.
 */
uint32_t                        logic_countOfMessagesInInputbuffer();

/** @brief This method will return the count of messages in the output buffer
 *  @return The count of slots used.
 */
uint32_t                        logic_countOfMessagesInOutputbuffer();

/** @brief This method will try to
 *              parse a message
 *              send ACK upon OK, NAK if NOK
 *              return to you the respective value
 *  @param time The actual time
 *  @return Take a look at typedef enum LOGIC_RETURN_VALUES.
 */
LOGIC_RETURN_VALUES_TYPEDEF     logic_parseNachricht(uint32_t time);

/** @brief This method will transmit your message, if it is OK
 *  @param cmd The byte containing the command
 *  @param lengthParam The byte containing the data length
 *  @param *param The array pointer containing the data
 *  @param time The actual time
 *  @param timeoutTime The time the communication partner can take to ACK this
 *  @return LOGIC_RETURN_MESSAGE_SENT if OK, LOGIC_RETURN_CRITICAL_ERROR else.
 */
LOGIC_RETURN_VALUES_TYPEDEF     logic_transmitMessage(uint8_t cmd, uint32_t lengthParam, uint8_t *param, uint32_t time, uint32_t timeoutTime);

/** @brief This method will reset the logic and it's subsystems values
 *  @return Nothing.
 */
void                            logic_resetEverything();

/** @brief This method will look into all slots of the input buffer to find your
 *         message.
 *  @param cmd The command you are looking for
 *  @param *found The pointer to a bool where the function can put the result
 *  @return The first number of the slot (slotID) that it got found.
 *  @return 0 is a possibly valid value, found tells you if it is valid
 */
uint32_t                        logic_getSlotOfCommand_inputBuffer(uint8_t cmd, bool *found);

/** @brief This method will look into all slots of the output buffer to find your
 *         message.
 *  @param cmd The command you are looking for
 *  @param *found The pointer to a bool where the function can put the result
 *  @return The first number of the slot (slotID) that it got found.
 *  @return 0 is a possibly valid value, found tells you if it is valid
 */
uint32_t                        logic_getSlotOfCommand_outputBuffer(uint8_t cmd, bool *found);

/** @brief This method will clear a specific slot from the output buffer.
 *  @param slotId The ID of the slot you want to clear.
 *  @return TRUE if cleared, FALSE else
 */
bool                            logic_deletePaketFromInputBuffer(uint32_t slotId);

/** @brief This method will clear a specific slot from the input buffer.
 *  @param slotId The ID of the slot you want to clear.
 *  @return TRUE if cleared, FALSE else
 */
bool                            logic_deletePaketFromOutputBuffer(uint32_t slotId);

/** @brief This method will get the newest message from the input buffer
 *  @param slotId Pointer to a uint32, in which ID of the slot shall be stored
 *  @param cmd Pointer to a byte where the method can store the cmd at
 *  @param paramLength Pointer to a byte where the method can store the datalength at
 *  @param param Pointer to a bytebuffer where the routine can copy the params to
 *  @return TRUE if OK, FALSE else
 */
bool                            logic_getNewestMessageFromInputBuffer(uint32_t *slotId, uint8_t *cmd, uint32_t *paramLength, uint8_t *param);

#if TEST_LOGIC >= 1

/** @brief This method is the test for this unit
 *  @return The returnvalue. 0 == OK, <0 == FAILURE
 */
int logic_testsuite();

#endif

#endif