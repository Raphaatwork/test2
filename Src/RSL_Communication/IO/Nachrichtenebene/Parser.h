/**
  ******************************************************************************
  * @file       Parser.h
  * @author     Tim Steinberg
  * @date       11.05.2020
  * @brief      This file contains a parser for a specific UART-RX-TX-PROTOCOL.
  *             The message structure for the messages are as following:
  *             - 1st Byte: Magic (0x02 = STM32 or Master, 0x03 = RSL or Slave)
  *             - 2nd Byte: Command (see command table)
  *             - 3rd Byte: Data length (depending on command, see message table)
  *             - n-1 Byte: CRC (for (i=0; i<n-1; i++) CRC ^= msg[i])
  *             -> In the message set CRC to 0x00 before calculating the CRC
  *                when building a message
  *             The ruleset for this protocol are as following:
  *             - Every command (except ACK/NAK) is ACKed or NAKed
  *             - Up to 2 repetitions (in case of NAK) are allowed
  *             - NAK only in case of error in transmission / reception
  *             - Reboot and Restart abort any other RX/TX-process
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
  * | 001       | 2020-05-11    | Tim Steinberg         | Initial version / skeleton of file            |
  * |-----------|---------------|-----------------------|-----------------------------------------------|
  * | 002       | 2020-05-25    | Tim Steinberg         | Added Tests                                   |
  * |-----------|---------------|-----------------------|-----------------------------------------------|
  * | 003       | 2020-06-15    | Tim Steinberg         | Added comments & doxygen commentaries         |
  * |-----------|---------------|-----------------------|-----------------------------------------------|
  * |           |               |                       |                                               |
  * |-----------|---------------|-----------------------|-----------------------------------------------|
  * |           |               |                       |                                               |
  * |-----------|---------------|-----------------------|-----------------------------------------------|
  *~~~
  */

#ifndef __PARSER_H
#define __PARSER_H

/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "Test_Selector.h"
#include "MessageIOBuffer.h"

/* Typedefinitions */

/**
 * @brief This typedef defines the return values of the parser functions
 *        MESSAGE_SUCCESSFULLY_RECEPTED = A message was successfully parsed
 *                                        and put in the assigned slot
 *        TOO_LESS_BYTES = Not enough bytes for a reception
 *        DROP_BYTE = Parser dropped a byte
 *        DO_NAK_HANDLING = Tell upper layer to send a NAK
 *        TIMEOUT = Some bytes came in but never put together a valid message
 */
typedef enum PARSER_RETURN_VALUES {
  PARSER_RETURN_MESSAGE_SUCCESSFULLY_RECEPTED   = 0x01, /**< A message was successfully received and put in given slot */
  PARSER_RETURN_TOO_LESS_BYTES                  = 0x02, /**< There are still bytes missing for a parsing */
  PARSER_RETURN_DROP_BYTE                       = 0x03, /**< Parser dropped a byte */
  PARSER_RETURN_DO_NAK_HANDLING                 = 0x0F, /**< Parser wants upper layer to do a nak handling */
  PARSER_RETURN_TIMEOUT                         = 0x1F, /**< Parser wants upper layer to do a timeout handling */
} PARSER_RETURN_VALUES_TYPEDEF;

/* Variables */

/* Function definitions */

/** @brief This method will start the integrity timer.
 *  @param time Actual time
 *  @return Nothing.
 */
void parser_timerMessageIntegrityStart(uint32_t time);

/** @brief This method will RESTART the timer, if it is running.
 *  @param time Actual time
 *  @return Nothing.
 */
void parser_timerMessageIntegrityStartISP(uint32_t time);

/** @brief This method makes the parser try to parse a message
 *  @param time The actual time (for entry in message buffer and for timeouts)
 *  @param *inputBuffer The pointer to the message input buffer
 *  @param inputBufferSlotId The slot that has to be used
 *  @return Take a look at typedef enum PARSER_RETURN_VALUES.
 */
PARSER_RETURN_VALUES_TYPEDEF parser_parseMessage(uint32_t time, MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *inputBuffer, uint32_t inputBufferSlotId);

/** @brief This method will reset the parser to starting setup.
 *  @return Nothing.
 */
void parser_resetEverything(void);

#if TEST_PARSER >= 1

/** @brief This method is the test for this unit
 *  @return The returnvalue. 0 == OK, <0 == FAILURE
 */
int parser_testsuite();

#endif

#endif