/**
  ******************************************************************************
  * @file       Parser.c
  * @author     Tim Steinberg
  * @date       11.05.2020
  * @brief      This file contains a parser for a specific UART-RX-TX-PROTOCOL.
  *             The message structure for the messages are as following:
  *             - 1st Byte: Magic (0x02 = STM32 or Master, 0x03 = RSL or Slave)
  *             - 2nd Byte: Command (see command table)
  *             - 3rd Byte: Data length (depending on command, see message table)
  *             - n-1 Byte: CRC (for (i=0; i<=n-1; i++) CRC ^= msg[i])
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

/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "Test_Selector.h"
#include "MasterDefine.h"
#include "Device_Definitions.h"
#include "Message_Definitions.h"
#include "ValidMessages.h"
#include "MessageIOBuffer.h"
#include "Handler_Timer.h"
#include "RingbufferWrapper.h"
#include "CRC_Software.h"
#include "Parser.h"
#include "Stack_Definitions.h"
        
/* Typedefinitions */

/**
 * @brief This typedef defines the internal return values for the MCD checker
 *        MCG_BYTES_VALID = The M/C/D bytes are valid
 *        MCG_BYTES_INVALID = The M/C/D bytes are NOT valid
 */
typedef enum PARSER_ARE_MCD_BYTES_VALID_VALUES {
  MCG_BYTES_VALID                               = 0x00,
  MCG_BYTES_INVALID                             = 0x01,
} PARSER_ARE_MCD_BYTES_VALID_VALUES_TYPEDEF;

/**
 * @brief This typedef defines the internal state values of the parser
 *        NO_FAILURE = The parser is in "valid" mode (no bad occurances)
 *        FAILURE = The parser had to drop bytes or do a nak handling, thus
 *                  as long as we don't have a valid parsing, it can not do a 
 *                  NAK handling again.
 */
typedef enum PARSER_FAILURE_FLAG_VALUES {
  PARSER_FAILURE_FLAG_NO_FAILURE                = 0x00,
  PARSER_FAILURE_FLAG_FAILURE                   = 0x01,
} PARSER_FAILURE_FLAG_VALUES_TYPEDEF;

/* Variables */

/**
 * @brief parserFailureFlag stores the internal state of the parser regarding
 *        errors and their occurances.
 */
PARSER_FAILURE_FLAG_VALUES_TYPEDEF parserFailureFlag = PARSER_FAILURE_FLAG_NO_FAILURE;

/**
 * @brief parserTimer is the parser internal message-integrity-timer. It is only
 *        started, when the parser tries to parse a message. If it's started
 *        then the reception can restart the timer. It is on hold when the
 *        parser successfully recepts a message. Until then make sure to call
 *        the parser often enough, since it only drops a single byte each run
 *        for failure. If it times out, the buffer gets flushed.
 */
TIMER_STRUCT_TYPEDEF parserTimer = {
  .time_targetTime = 0,
  .time_waitTime = 100,
  .flag_active = TIMER_FLAG_INACTIVE,
};

/* Function definitions */

/** @brief This method will start the integrity timer.
 *  @param time Actual time
 *  @return Nothing.
 */
void parser_timerMessageIntegrityStart(uint32_t time){
  timerHandler_timerStart(&parserTimer, time, PARSER_MESSAGE_INTEGRITY_TIMEOUT_LENGTH_MS);
}

/** @brief This method will RESTART the timer, if it is running.
 *  @param time Actual time
 *  @return Nothing.
 */
void parser_timerMessageIntegrityStartISP(uint32_t time){
  if (timerHandler_isTimerActive(&parserTimer) == TIMER_FLAG_ACTIVE){
    timerHandler_timerStart(&parserTimer, time, PARSER_MESSAGE_INTEGRITY_TIMEOUT_LENGTH_MS);
  }
}

/** @brief This method will reset the parser to starting setup.
 *  @return Nothing.
 */
void parser_resetEverything(){
  // Reset parser related stuff
  parserFailureFlag = PARSER_FAILURE_FLAG_NO_FAILURE;
  parserTimer.time_targetTime = 0;
  parserTimer.time_waitTime = 100;
  parserTimer.flag_active = TIMER_FLAG_INACTIVE;
  
  // Reset reception buffer and its related stuff
  ringbufferWrapper_clear();
}

/** @brief This method check the M/C/D bytes for validity in the message table
 *  @param magicByte Byte that contains the magic
 *  @param cmdByte Byte that contains the command
 *  @param dataLengthByte Byte that contains the data length
 *  @return VALID or INVALID.
 */
PARSER_ARE_MCD_BYTES_VALID_VALUES_TYPEDEF parser_areMCDBytesValid(uint8_t magicByte, uint8_t cmdByte, uint8_t dataLengthByte){
  PARSER_ARE_MCD_BYTES_VALID_VALUES_TYPEDEF retVal = MCG_BYTES_INVALID;
  uint32_t colcount     = messageParamValidity_reception_getColCount();
  uint32_t rowcount     = messageParamValidity_reception_getRowCount();
  uint8_t **bufPtr      = messageParamValidity_reception_getPointerToBuffer();
  uint32_t mover;
  
  if (magicByte == (uint8_t) UART_AWAITING_MAGIC){
    for (uint32_t i = (uint32_t) 0; i < rowcount; i++){
      mover = i * colcount;
      if (((uint8_t*) bufPtr)[mover] == cmdByte){
        /* It is contained*/ 
        
        /* Check if the data length is in the correct interval as well */
        if ((dataLengthByte >= ((uint8_t*) bufPtr)[mover + 1]) && (dataLengthByte <= ((uint8_t*) bufPtr)[mover + 2])){
          /* It is correct */
          retVal = MCG_BYTES_VALID;
           break;
        }
      }
    }
  }
  return retVal;
}

/** @brief This method will handle the byte drops and tells the upper layer to
 *         either do NAK handling or not.
 *  @return TRUE or FALSE.
 */
bool parser_handleByteDropAndErrorFlag(void){
  // Drop a single byte
    ringbufferWrapper_dropByte();
    
    // Are we already in an erroneous parsing mode?
    // If we are, that means we already sent NAK and dropped bytes
    // We must not send another NAK!
    if (parserFailureFlag == PARSER_FAILURE_FLAG_NO_FAILURE){
      // No
      
      // Set a flag that we just entered an erroneous parsing mode
      parserFailureFlag = PARSER_FAILURE_FLAG_FAILURE;
      
      return TRUE;
    }
    
    // We must return. In the next loop we can try and look, if the next message has begun
    // Due to our sending of NAK, we can expect the message to be there again - hopefully complete
    return FALSE;
}

/** @brief This method makes the parser try to parse a message
 *  @param time The actual time (for entry in message buffer and for timeouts)
 *  @param *inputBuffer The pointer to the message input buffer
 *  @param inputBufferSlotId The slot that has to be used
 *  @return Take a look at typedef enum PARSER_RETURN_VALUES.
 */
PARSER_RETURN_VALUES_TYPEDEF parser_parseMessage(uint32_t time, MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *inputBuffer, uint32_t inputBufferSlotId){
  uint32_t packageLength;
  
  // Check if the reception buffer got an "overflow" state
  if (ringbufferWrapper_flagState() == RINGBUFFER_WRAPPER_STATE_OVERFLOW){
    // Yes
    
    // So clear everything, due to receptions being trash
    ringbufferWrapper_clear();
    
    // Is the error flag not set?
    if (parserFailureFlag == PARSER_FAILURE_FLAG_NO_FAILURE){
      // Yes
      
      // Then tell the upper layer to re-request whatever was being sent
      return PARSER_RETURN_DO_NAK_HANDLING;
    }
    
    // So whatever was in the buffer is trash and seems to be already
    // re-requested... so whatever we got now seems to be trash as well
    // Thus we just wait for the messages to time out on the other side so
    // everything returns in it's intended order
    parserFailureFlag = PARSER_FAILURE_FLAG_NO_FAILURE;
    return PARSER_RETURN_TIMEOUT;
  }
  
  // Is there a timeout running regarding complete-reception-integrity?
  if (timerHandler_isTimerActive(&parserTimer) == TIMER_FLAG_INACTIVE){
    // No
    
    // Is there something in ringbuffer?
    if (ringbufferWrapper_getCount() > 0){
      // Yes
      
      // Start the timer
      parser_timerMessageIntegrityStart(time);
    }else{
      // No
      
      // Then just return
      return PARSER_RETURN_TOO_LESS_BYTES;
    }
  }
  
  // Has the timer passed the timeout point?
  if (timerHandler_isTimerTimedOut(&parserTimer, time) == TRUE){
    // Yes
    
    // We can not call reset everything, because the failure is of essence in
    // the further process
    
    // We received bytes, but we couldn't complete a message in time
    // Thus we missed out bytes and now we got incomplete messages
    // Thus clear the ringbuffer
    ringbufferWrapper_clear();
    
    // we must reset the timer to make sure, that the next reception starts
    // the timer anew.
    parserTimer.time_targetTime = 0;
    parserTimer.time_waitTime = PARSER_MESSAGE_INTEGRITY_TIMEOUT_LENGTH_MS;
    parserTimer.flag_active = TIMER_FLAG_INACTIVE;
    
    // Are we already in a problematic situation regarding reception?
    if (parserFailureFlag == PARSER_FAILURE_FLAG_FAILURE){
      // Yes
      
      // Then we now cleared the buffer and should be able to receive messages
      parserFailureFlag = PARSER_FAILURE_FLAG_NO_FAILURE;
      return PARSER_RETURN_TIMEOUT;
    }
    
    return PARSER_RETURN_DO_NAK_HANDLING;
  }
  
  // Do we have enough bytes to understand at least the header?
  if (ringbufferWrapper_getCount() < 4){
    // No
    
    // Then there is nothing that we can do right now
    return PARSER_RETURN_TOO_LESS_BYTES;
  }
  
  // Are the magic, command and datalength byte valid?
  if (parser_areMCDBytesValid(ringbufferWrapper_peekByte(0), ringbufferWrapper_peekByte(1), ringbufferWrapper_peekByte(2)) != MCG_BYTES_VALID){
    // No
    
    // Call handler for drop a byte AND check if we need to tell upper layer
    // to send NAK or not
    if (parser_handleByteDropAndErrorFlag() == TRUE){
      return PARSER_RETURN_DO_NAK_HANDLING;
    }else{
      return PARSER_RETURN_DROP_BYTE;
    }
  }
  
  // Calculate the whole length of the message
  packageLength = (uint8_t) UART_PACKAGE_HEADER_SIZE + (uint8_t) UART_PACKAGE_DATA_SIZE + ringbufferWrapper_peekByte(2) + (uint8_t) UART_PACKAGE_CRC_SIZE;
  
  // Is everything already there?
  if (ringbufferWrapper_getCount() < packageLength){
    // No
    
    // Since we still miss some bytes, we can return and wait for more
    return PARSER_RETURN_TOO_LESS_BYTES;
  }
  
  // Just peek-copy the buffer in the assigned buffer field
  for (uint32_t i = (uint32_t) 0; i < packageLength; i++){
    messageIOBuffer_addByteToBuffer_bySlotId(inputBuffer, inputBufferSlotId, ringbufferWrapper_peekByte(i));
  }
  
  // Is the crc valid?
  if (CRC_Software_checkCRC(messageIOBuffer_getBuffer_bySlotId(inputBuffer, inputBufferSlotId), packageLength) == CRC_SOFTWARE_INVALID){
    // No
    
    // Reset used buffer length
    messageIOBuffer_setMessageLength_bySlotId(inputBuffer, inputBufferSlotId, 0);
    
    // Call handler for drop a byte AND check if we need to tell upper layer
    // to send NAK or not
    if (parser_handleByteDropAndErrorFlag() == TRUE){
      return PARSER_RETURN_DO_NAK_HANDLING;
    }else{
      return PARSER_RETURN_DROP_BYTE;
    }
  }
  
  // Since we received something valid, we must reset the failure flag...
  parserFailureFlag = PARSER_FAILURE_FLAG_NO_FAILURE;
  
  // ...stop the timer as well...
  timerHandler_timerStop(&parserTimer);
  
  // Since our message is correct, we must now take out the bytes for real
  ringbufferWrapper_dropBytesCount(packageLength);
    
  // Now that everything is handled, we can validate our reception
  messageIOBuffer_validateMessage_bySlotId(inputBuffer, inputBufferSlotId, time, 1000);
  
  return PARSER_RETURN_MESSAGE_SUCCESSFULLY_RECEPTED;
}

#if TEST_PARSER >= 1

/** @brief This method will free variables and set back things to return from 
 *         the test.
 *  @param retVal The value you want to return.
 *  @return The returnvalue you enter.
 */
int parser_testsuiteReturner(int retVal){
  return retVal;
}

void parser_slotInit(BUFFER_STRUCT_TYPEDEF *slot, uint8_t *buffer){
  slot->transmissionCount = 0;
  slot->usedLength = 0;
  slot->buffer = buffer;
  slot->age = 0;
  slot->valid = FALSE;
}

void parser_resetEverythingAndMiob(MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF *miob){
  for (int i = 0; i < miob->slotCount; i++){
    miob->slot[i].transmissionCount = 0;
    miob->slot[i].usedLength = 0;
    miob->slot[i].age = 0;
    miob->slot[i].valid = FALSE;
  }
  parser_resetEverything();
}

/** @brief This method is the test for this unit
 *  @return The returnvalue. 0 == OK, <0 == FAILURE
 */
int parser_testsuite(){
  int32_t i;
  int32_t tempInt32_t;
  PARSER_RETURN_VALUES_TYPEDEF parserRetVal;
  
  // Although this test should only test the buffer, due to it using the
  // message IO-struct it will test *SOME* of it's features, but passively
  
  /* Test order & setup:
   * 1.) test reset: ensure a stable reset point
   * 2.) test empty conditions twice: ensure that just asking does not change anything
   * 3.) test correct messages: ensure correct parsing under optimal conditions
   * 4.) test wrong CRC: ensure that CRC check works as intended
   * 5.) test wrong M/C/D-Bytes: ensure correct behaviour and return values
   * 6.) test timeouting: ensure correct behaviour and return values
   * 7.) test mixed behaviour - correct/wrong/correct: ensure correct behaviours
   * 8.) test mixed behaviour - correct/incomplete/correct: ensure correct behaviours
   * 9.) test overflowing conditions: ensure correct behaviour under spamming conditions
   */
  
  // MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF init
  
  uint8_t bufferArrays[PARSER_TEST_SLOTCOUNT][PARSER_TEST_BUFFERSIZE];
  BUFFER_STRUCT_TYPEDEF slots[PARSER_TEST_SLOTCOUNT];
  
  for (i = 0; i < PARSER_TEST_SLOTCOUNT; i++){
    parser_slotInit(&slots[i], bufferArrays[i]);
  }
  
  MESSAGE_INPUT_OUTPUT_BUFFER_STRUCT_TYPEDEF miob = {
    .slotCount = PARSER_TEST_SLOTCOUNT,
    .maxBufferLength = PARSER_TEST_BUFFERSIZE,
    .slot = slots,
  };
  
  // Test reset
  parser_resetEverything();
  
  // Test under empty conditions -> return value must be "too less bytes", timer must NOT be started
  tempInt32_t = messageIOBuffer_getFreeSlot(&miob);
  if (tempInt32_t < 0){
    return parser_testsuiteReturner(-1);
  }
  parserRetVal = parser_parseMessage(0, &miob, tempInt32_t);
  if (parserRetVal != PARSER_RETURN_TOO_LESS_BYTES){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.flag_active != TIMER_FLAG_INACTIVE){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.time_targetTime != 0){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.time_waitTime != 100){
    return parser_testsuiteReturner(-1);
  }
  if (parserFailureFlag != PARSER_FAILURE_FLAG_NO_FAILURE){
    return parser_testsuiteReturner(-1);
  }
  
  // Test same thing again. Due to nothing that happened, states must be EXACT the same
  tempInt32_t = messageIOBuffer_getFreeSlot(&miob);
  if (tempInt32_t < 0){
    return parser_testsuiteReturner(-1);
  }
  parserRetVal = parser_parseMessage(0, &miob, tempInt32_t);
  if (parserRetVal != PARSER_RETURN_TOO_LESS_BYTES){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.flag_active != TIMER_FLAG_INACTIVE){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.time_targetTime != 0){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.time_waitTime != 100){
    return parser_testsuiteReturner(-1);
  }
  if (parserFailureFlag != PARSER_FAILURE_FLAG_NO_FAILURE){
    return parser_testsuiteReturner(-1);
  }
  
  //=============== PARSE OF A CORRECT MESSAGE WITHOUT ANY PARAMETER
  ringbufferWrapper_putByte(UART_AWAITING_MAGIC);
  ringbufferWrapper_putByte(UART_MSG_CMD_PACK_REC_ACK);
  ringbufferWrapper_putByte(0x00);
  ringbufferWrapper_putByte(0xF9);
  tempInt32_t = messageIOBuffer_getFreeSlot(&miob);
  if (tempInt32_t < 0){
    return parser_testsuiteReturner(-1);
  }
  parserRetVal = parser_parseMessage(123, &miob, tempInt32_t);
  if (parserRetVal != PARSER_RETURN_MESSAGE_SUCCESSFULLY_RECEPTED){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.flag_active != TIMER_FLAG_INACTIVE){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.time_targetTime != (123 + PARSER_MESSAGE_INTEGRITY_TIMEOUT_LENGTH_MS)){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.time_waitTime != 100){
    return parser_testsuiteReturner(-1);
  }
  if (parserFailureFlag != PARSER_FAILURE_FLAG_NO_FAILURE){
    return parser_testsuiteReturner(-1);
  }
  
  //=============== PARSE OF A CORRECT MESSAGE WITH 1 PARAMETER 
  //                *THIS CAN ONLY BE TESTED IN RSL MODE, SO WE HAVE TO DO IT THIS WAY*
  ringbufferWrapper_putByte(UART_AWAITING_MAGIC);
  ringbufferWrapper_putByte(UART_MSG_CMD_HCI_COMMAND_RESPONSE);
  ringbufferWrapper_putByte(0x01);
  ringbufferWrapper_putByte(0x64);
  ringbufferWrapper_putByte(0xDD);
  tempInt32_t = messageIOBuffer_getFreeSlot(&miob);
  if (tempInt32_t < 0){
    return parser_testsuiteReturner(-1);
  }
  parserRetVal = parser_parseMessage(123, &miob, tempInt32_t);
  if (parserRetVal != PARSER_RETURN_MESSAGE_SUCCESSFULLY_RECEPTED){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.flag_active != TIMER_FLAG_INACTIVE){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.time_targetTime != (123 + PARSER_MESSAGE_INTEGRITY_TIMEOUT_LENGTH_MS)){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.time_waitTime != 100){
    return parser_testsuiteReturner(-1);
  }
  if (parserFailureFlag != PARSER_FAILURE_FLAG_NO_FAILURE){
    return parser_testsuiteReturner(-1);
  }
  
  //=============== PARSE OF A CORRECT MESSAGE WITH >1 PARAMETER
  ringbufferWrapper_putByte(UART_AWAITING_MAGIC);
  ringbufferWrapper_putByte(UART_MSG_CMD_HCI_COMMAND_RESPONSE);
  ringbufferWrapper_putByte(0x20);
  for (i = 0; i < 32; i++){
    ringbufferWrapper_putByte((uint8_t) (i & 0xFF));
  }
  ringbufferWrapper_putByte(0x98);
  tempInt32_t = messageIOBuffer_getFreeSlot(&miob);
  if (tempInt32_t < 0){
    return parser_testsuiteReturner(-1);
  }
  parserRetVal = parser_parseMessage(123, &miob, tempInt32_t);
  if (parserRetVal != PARSER_RETURN_MESSAGE_SUCCESSFULLY_RECEPTED){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.flag_active != TIMER_FLAG_INACTIVE){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.time_targetTime != (123 + PARSER_MESSAGE_INTEGRITY_TIMEOUT_LENGTH_MS)){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.time_waitTime != 100){
    return parser_testsuiteReturner(-1);
  }
  if (parserFailureFlag != PARSER_FAILURE_FLAG_NO_FAILURE){
    return parser_testsuiteReturner(-1);
  }
  
  //=============== PARSE OF A WRONG-CRC MESSAGE WITHOUT ANY PARAMETER
  ringbufferWrapper_putByte(UART_AWAITING_MAGIC);
  ringbufferWrapper_putByte(UART_MSG_CMD_PACK_REC_ACK);
  ringbufferWrapper_putByte(0x00);
  ringbufferWrapper_putByte(0xF8);
  tempInt32_t = messageIOBuffer_getFreeSlot(&miob);
  if (tempInt32_t < 0){
    return parser_testsuiteReturner(-1);
  }
  parserRetVal = parser_parseMessage(123, &miob, tempInt32_t);
  if (parserRetVal != PARSER_RETURN_DO_NAK_HANDLING){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.flag_active != TIMER_FLAG_ACTIVE){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.time_targetTime != (123 + PARSER_MESSAGE_INTEGRITY_TIMEOUT_LENGTH_MS)){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.time_waitTime != 100){
    return parser_testsuiteReturner(-1);
  }
  if (parserFailureFlag != PARSER_FAILURE_FLAG_FAILURE){
    return parser_testsuiteReturner(-1);
  }
  parser_resetEverythingAndMiob(&miob);
  
  //=============== PARSE OF A WRONG-CRC MESSAGE WITH 1 PARAMETER
  ringbufferWrapper_putByte(UART_AWAITING_MAGIC);
  ringbufferWrapper_putByte(UART_MSG_CMD_HCI_COMMAND_RESPONSE);
  ringbufferWrapper_putByte(0x01);
  ringbufferWrapper_putByte(0x64);
  ringbufferWrapper_putByte(0xDE);
  tempInt32_t = messageIOBuffer_getFreeSlot(&miob);
  if (tempInt32_t < 0){
    return parser_testsuiteReturner(-1);
  }
  parserRetVal = parser_parseMessage(123, &miob, tempInt32_t);
  if (parserRetVal != PARSER_RETURN_DO_NAK_HANDLING){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.flag_active != TIMER_FLAG_ACTIVE){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.time_targetTime != (123 + PARSER_MESSAGE_INTEGRITY_TIMEOUT_LENGTH_MS)){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.time_waitTime != 100){
    return parser_testsuiteReturner(-1);
  }
  if (parserFailureFlag != PARSER_FAILURE_FLAG_FAILURE){
    return parser_testsuiteReturner(-1);
  }
  parser_resetEverythingAndMiob(&miob);
  
  //=============== PARSE OF A WRONG-CRC MESSAGE WITH >1 PARAMETER
  ringbufferWrapper_putByte(UART_AWAITING_MAGIC);
  ringbufferWrapper_putByte(UART_MSG_CMD_HCI_COMMAND_RESPONSE);
  ringbufferWrapper_putByte(0x20);
  for (i = 0; i < 32; i++){
    ringbufferWrapper_putByte((uint8_t) (i & 0xFF));
  }
  ringbufferWrapper_putByte(0x97);
  tempInt32_t = messageIOBuffer_getFreeSlot(&miob);
  if (tempInt32_t < 0){
    return parser_testsuiteReturner(-1);
  }
  parserRetVal = parser_parseMessage(123, &miob, tempInt32_t);
  if (parserRetVal != PARSER_RETURN_DO_NAK_HANDLING){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.flag_active != TIMER_FLAG_ACTIVE){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.time_targetTime != (123 + PARSER_MESSAGE_INTEGRITY_TIMEOUT_LENGTH_MS)){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.time_waitTime != 100){
    return parser_testsuiteReturner(-1);
  }
  if (parserFailureFlag != PARSER_FAILURE_FLAG_FAILURE){
    return parser_testsuiteReturner(-1);
  }
  parser_resetEverythingAndMiob(&miob);
  
  //=============== PARSE OF A WRONG-DL-BYTE MESSAGE WITHOUT ANY PARAMETER
  ringbufferWrapper_putByte(UART_AWAITING_MAGIC);
  ringbufferWrapper_putByte(UART_MSG_CMD_PACK_REC_ACK);
  ringbufferWrapper_putByte(0x01);
  ringbufferWrapper_putByte(0xF9);
  tempInt32_t = messageIOBuffer_getFreeSlot(&miob);
  if (tempInt32_t < 0){
    return parser_testsuiteReturner(-1);
  }
  parserRetVal = parser_parseMessage(123, &miob, tempInt32_t);
  if (parserRetVal != PARSER_RETURN_DO_NAK_HANDLING){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.flag_active != TIMER_FLAG_ACTIVE){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.time_targetTime != (123 + PARSER_MESSAGE_INTEGRITY_TIMEOUT_LENGTH_MS)){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.time_waitTime != 100){
    return parser_testsuiteReturner(-1);
  }
  if (parserFailureFlag != PARSER_FAILURE_FLAG_FAILURE){
    return parser_testsuiteReturner(-1);
  }
  parser_resetEverythingAndMiob(&miob);
  
  //=============== PARSE OF A WRONG-DL-BYTE MESSAGE WITH 1 PARAMETER (TOO SMALL)
  ringbufferWrapper_putByte(UART_AWAITING_MAGIC);
  ringbufferWrapper_putByte(UART_MSG_CMD_HCI_COMMAND_RESPONSE);
  ringbufferWrapper_putByte(0x00);
  ringbufferWrapper_putByte(0x64);
  ringbufferWrapper_putByte(0xDD);
  tempInt32_t = messageIOBuffer_getFreeSlot(&miob);
  if (tempInt32_t < 0){
    return parser_testsuiteReturner(-1);
  }
  parserRetVal = parser_parseMessage(123, &miob, tempInt32_t);
  if (parserRetVal != PARSER_RETURN_DO_NAK_HANDLING){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.flag_active != TIMER_FLAG_ACTIVE){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.time_targetTime != (123 + PARSER_MESSAGE_INTEGRITY_TIMEOUT_LENGTH_MS)){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.time_waitTime != 100){
    return parser_testsuiteReturner(-1);
  }
  if (parserFailureFlag != PARSER_FAILURE_FLAG_FAILURE){
    return parser_testsuiteReturner(-1);
  }
  parser_resetEverythingAndMiob(&miob);
  
  //=============== PARSE OF A WRONG-DL-BYTE MESSAGE WITH >1 PARAMETER (TOO BIG)
  ringbufferWrapper_putByte(UART_AWAITING_MAGIC);
  ringbufferWrapper_putByte(UART_MSG_CMD_HCI_COMMAND_RESPONSE);
  ringbufferWrapper_putByte(0x21);
  for (i = 0; i < 32; i++){
    ringbufferWrapper_putByte((uint8_t) (i & 0xFF));
  }
  ringbufferWrapper_putByte(0x98);
  tempInt32_t = messageIOBuffer_getFreeSlot(&miob);
  if (tempInt32_t < 0){
    return parser_testsuiteReturner(-1);
  }
  parserRetVal = parser_parseMessage(123, &miob, tempInt32_t);
  if (parserRetVal != PARSER_RETURN_DO_NAK_HANDLING){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.flag_active != TIMER_FLAG_ACTIVE){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.time_targetTime != (123 + PARSER_MESSAGE_INTEGRITY_TIMEOUT_LENGTH_MS)){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.time_waitTime != 100){
    return parser_testsuiteReturner(-1);
  }
  if (parserFailureFlag != PARSER_FAILURE_FLAG_FAILURE){
    return parser_testsuiteReturner(-1);
  }
  parser_resetEverythingAndMiob(&miob);

  //=============== PARSE OF A WRONG-CMD-BYTE MESSAGE WITHOUT ANY PARAMETER
  ringbufferWrapper_putByte(UART_AWAITING_MAGIC);
  ringbufferWrapper_putByte(UART_MSG_CMD_REBOOT);
  ringbufferWrapper_putByte(0x00);
  ringbufferWrapper_putByte(0xC1);
  tempInt32_t = messageIOBuffer_getFreeSlot(&miob);
  if (tempInt32_t < 0){
    return parser_testsuiteReturner(-1);
  }
  parserRetVal = parser_parseMessage(123, &miob, tempInt32_t);
  if (parserRetVal != PARSER_RETURN_DO_NAK_HANDLING){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.flag_active != TIMER_FLAG_ACTIVE){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.time_targetTime != (123 + PARSER_MESSAGE_INTEGRITY_TIMEOUT_LENGTH_MS)){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.time_waitTime != 100){
    return parser_testsuiteReturner(-1);
  }
  if (parserFailureFlag != PARSER_FAILURE_FLAG_FAILURE){
    return parser_testsuiteReturner(-1);
  }
  parser_resetEverythingAndMiob(&miob);
  
  //=============== PARSE OF A WRONG-CMD-BYTE MESSAGE WITH 1 PARAMETER
  //                *THIS CAN ONLY BE TESTED IN RSL MODE, SO WE HAVE TO DO IT THIS WAY*
  ringbufferWrapper_putByte(UART_AWAITING_MAGIC);
  ringbufferWrapper_putByte(UART_MSG_CMD_ALERT);
  ringbufferWrapper_putByte(0x01);
  ringbufferWrapper_putByte(0x64);
  ringbufferWrapper_putByte(0xCD);
  tempInt32_t = messageIOBuffer_getFreeSlot(&miob);
  if (tempInt32_t < 0){
    return parser_testsuiteReturner(-1);
  }
  parserRetVal = parser_parseMessage(123, &miob, tempInt32_t);
  if (parserRetVal != PARSER_RETURN_DO_NAK_HANDLING){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.flag_active != TIMER_FLAG_ACTIVE){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.time_targetTime != (123 + PARSER_MESSAGE_INTEGRITY_TIMEOUT_LENGTH_MS)){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.time_waitTime != 100){
    return parser_testsuiteReturner(-1);
  }
  if (parserFailureFlag != PARSER_FAILURE_FLAG_FAILURE){
    return parser_testsuiteReturner(-1);
  }
  parser_resetEverythingAndMiob(&miob);
  
  //=============== PARSE OF A WRONG-CMD-BYTE MESSAGE WITH >1 PARAMETER
  ringbufferWrapper_putByte(UART_AWAITING_MAGIC);
  ringbufferWrapper_putByte(0x05);
  ringbufferWrapper_putByte(0x20);
  for (i = 0; i < 32; i++){
    ringbufferWrapper_putByte((uint8_t) (i & 0xFF));
  }
  ringbufferWrapper_putByte(0x8C);
  tempInt32_t = messageIOBuffer_getFreeSlot(&miob);
  if (tempInt32_t < 0){
    return parser_testsuiteReturner(-1);
  }
  parserRetVal = parser_parseMessage(123, &miob, tempInt32_t);
  if (parserRetVal != PARSER_RETURN_DO_NAK_HANDLING){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.flag_active != TIMER_FLAG_ACTIVE){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.time_targetTime != (123 + PARSER_MESSAGE_INTEGRITY_TIMEOUT_LENGTH_MS)){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.time_waitTime != 100){
    return parser_testsuiteReturner(-1);
  }
  if (parserFailureFlag != PARSER_FAILURE_FLAG_FAILURE){
    return parser_testsuiteReturner(-1);
  }
  parser_resetEverythingAndMiob(&miob);
  
  //=============== PARSE OF A WRONG-MAGIC-BYTE MESSAGE WITHOUT ANY PARAMETER
  ringbufferWrapper_putByte(UART_TRANSMISSION_MAGIC);
  ringbufferWrapper_putByte(UART_MSG_CMD_PACK_REC_ACK);
  ringbufferWrapper_putByte(0x00);
  ringbufferWrapper_putByte(0xF9);
  tempInt32_t = messageIOBuffer_getFreeSlot(&miob);
  if (tempInt32_t < 0){
    return parser_testsuiteReturner(-1);
  }
  parserRetVal = parser_parseMessage(123, &miob, tempInt32_t);
  if (parserRetVal != PARSER_RETURN_DO_NAK_HANDLING){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.flag_active != TIMER_FLAG_ACTIVE){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.time_targetTime != (123 + PARSER_MESSAGE_INTEGRITY_TIMEOUT_LENGTH_MS)){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.time_waitTime != 100){
    return parser_testsuiteReturner(-1);
  }
  if (parserFailureFlag != PARSER_FAILURE_FLAG_FAILURE){
    return parser_testsuiteReturner(-1);
  }
  parser_resetEverythingAndMiob(&miob);
  
  //=============== PARSE OF A WRONG-MAGIC-BYTE MESSAGE WITH 1 PARAMETER
  //                *THIS CAN ONLY BE TESTED IN RSL MODE, SO WE HAVE TO DO IT THIS WAY*
  ringbufferWrapper_putByte(0x00);
  ringbufferWrapper_putByte(UART_MSG_CMD_HCI_COMMAND_RESPONSE);
  ringbufferWrapper_putByte(0x01);
  ringbufferWrapper_putByte(0x64);
  ringbufferWrapper_putByte(0xDD);
  tempInt32_t = messageIOBuffer_getFreeSlot(&miob);
  if (tempInt32_t < 0){
    return parser_testsuiteReturner(-1);
  }
  parserRetVal = parser_parseMessage(123, &miob, tempInt32_t);
  if (parserRetVal != PARSER_RETURN_DO_NAK_HANDLING){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.flag_active != TIMER_FLAG_ACTIVE){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.time_targetTime != (123 + PARSER_MESSAGE_INTEGRITY_TIMEOUT_LENGTH_MS)){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.time_waitTime != 100){
    return parser_testsuiteReturner(-1);
  }
  if (parserFailureFlag != PARSER_FAILURE_FLAG_FAILURE){
    return parser_testsuiteReturner(-1);
  }
  parser_resetEverythingAndMiob(&miob);
  
  //=============== PARSE OF A WRONG-MAGIC-BYTE MESSAGE WITH >1 PARAMETER
  ringbufferWrapper_putByte(0x01);
  ringbufferWrapper_putByte(UART_MSG_CMD_HCI_COMMAND_RESPONSE);
  ringbufferWrapper_putByte(0x20);
  for (i = 0; i < 32; i++){
    ringbufferWrapper_putByte((uint8_t) (i & 0xFF));
  }
  ringbufferWrapper_putByte(0x98);
  tempInt32_t = messageIOBuffer_getFreeSlot(&miob);
  if (tempInt32_t < 0){
    return parser_testsuiteReturner(-1);
  }
  parserRetVal = parser_parseMessage(123, &miob, tempInt32_t);
  if (parserRetVal != PARSER_RETURN_DO_NAK_HANDLING){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.flag_active != TIMER_FLAG_ACTIVE){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.time_targetTime != (123 + PARSER_MESSAGE_INTEGRITY_TIMEOUT_LENGTH_MS)){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.time_waitTime != 100){
    return parser_testsuiteReturner(-1);
  }
  if (parserFailureFlag != PARSER_FAILURE_FLAG_FAILURE){
    return parser_testsuiteReturner(-1);
  }
  parser_resetEverythingAndMiob(&miob);
  
  //=============== TEST THE TIMEOUT (INCOMPLETE MESSAGE, CRC MISSING)
  ringbufferWrapper_putByte(UART_AWAITING_MAGIC);
  ringbufferWrapper_putByte(UART_MSG_CMD_HCI_COMMAND_RESPONSE);
  ringbufferWrapper_putByte(0x20);
  for (i = 0; i < 32; i++){
    ringbufferWrapper_putByte((uint8_t) (i & 0xFF));
  }
  // THE CRC IS MISSING
  tempInt32_t = messageIOBuffer_getFreeSlot(&miob);
  if (tempInt32_t < 0){
    return parser_testsuiteReturner(-1);
  }
  // FIRST RUN - NO TIMEOUT
  parserRetVal = parser_parseMessage(123, &miob, tempInt32_t);
  if (parserRetVal != PARSER_RETURN_TOO_LESS_BYTES){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.flag_active != TIMER_FLAG_ACTIVE){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.time_targetTime != 123 + PARSER_MESSAGE_INTEGRITY_TIMEOUT_LENGTH_MS){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.time_waitTime != PARSER_MESSAGE_INTEGRITY_TIMEOUT_LENGTH_MS){
    return parser_testsuiteReturner(-1);
  }
  if (parserFailureFlag != PARSER_FAILURE_FLAG_NO_FAILURE){
    return parser_testsuiteReturner(-1);
  }
  // 2ND RUN - TIMEOUT, but due to NO sending of NAK yet, do NAK handling!
  parserRetVal = parser_parseMessage(123 + PARSER_MESSAGE_INTEGRITY_TIMEOUT_LENGTH_MS + 1, &miob, tempInt32_t);
  if (parserRetVal != PARSER_RETURN_DO_NAK_HANDLING){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.flag_active != TIMER_FLAG_INACTIVE){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.time_targetTime != 0){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.time_waitTime != 100){
    return parser_testsuiteReturner(-1);
  }
  // Due to the parser doing a nak + reset, it IS no failure, because next reception may be correct
  if (parserFailureFlag != PARSER_FAILURE_FLAG_NO_FAILURE){
    return parser_testsuiteReturner(-1);
  }
  parser_resetEverythingAndMiob(&miob);
  
  //=============== TEST THE TIMEOUT AFTER NAKing (WRONG MESSAGE)
  ringbufferWrapper_putByte(UART_AWAITING_MAGIC);
  ringbufferWrapper_putByte(UART_MSG_CMD_PACK_REC_ACK);
  ringbufferWrapper_putByte(0x00);
  ringbufferWrapper_putByte(0xF8);
  tempInt32_t = messageIOBuffer_getFreeSlot(&miob);
  if (tempInt32_t < 0){
    return parser_testsuiteReturner(-1);
  }
  // First parse
  parserRetVal = parser_parseMessage(123, &miob, tempInt32_t);
  if (parserRetVal != PARSER_RETURN_DO_NAK_HANDLING){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.flag_active != TIMER_FLAG_ACTIVE){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.time_targetTime != (123 + PARSER_MESSAGE_INTEGRITY_TIMEOUT_LENGTH_MS)){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.time_waitTime != 100){
    return parser_testsuiteReturner(-1);
  }
  if (parserFailureFlag != PARSER_FAILURE_FLAG_FAILURE){
    return parser_testsuiteReturner(-1);
  }
  // 2ND RUN - TIMEOUT, but due to sending of NAK, it's timeout!
  parserRetVal = parser_parseMessage(123 + PARSER_MESSAGE_INTEGRITY_TIMEOUT_LENGTH_MS + 1, &miob, tempInt32_t);
  if (parserRetVal != PARSER_RETURN_TIMEOUT){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.flag_active != TIMER_FLAG_INACTIVE){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.time_targetTime != 0){
    return parser_testsuiteReturner(-1);
  }
  if (parserTimer.time_waitTime != 100){
    return parser_testsuiteReturner(-1);
  }
  // Due to the parser doing a nak + reset, it IS no failure, because next reception may be correct
  if (parserFailureFlag != PARSER_FAILURE_FLAG_NO_FAILURE){
    return parser_testsuiteReturner(-1);
  }
  parser_resetEverythingAndMiob(&miob);
  
  /*
   * 7.) test mixed behaviour - correct/wrong/correct: ensure correct behaviours
   * 8.) test mixed behaviour - correct/incomplete/correct: ensure correct behaviours
   * 9.) test overflowing conditions: ensure correct behaviour under spamming conditions
   */
  //=============== TEST MIXED BEHAVIOUR: PUSH C/W/C IN BUFFER AND PARSE THEM
  ringbufferWrapper_putByte(UART_AWAITING_MAGIC);
  ringbufferWrapper_putByte(UART_MSG_CMD_PACK_REC_ACK);
  ringbufferWrapper_putByte(0x00);
  ringbufferWrapper_putByte(0xF9);
  ringbufferWrapper_putByte(UART_AWAITING_MAGIC);
  ringbufferWrapper_putByte(UART_MSG_CMD_PACK_REC_ACK);
  ringbufferWrapper_putByte(0x00);
  ringbufferWrapper_putByte(0xF8);
  ringbufferWrapper_putByte(UART_AWAITING_MAGIC);
  ringbufferWrapper_putByte(UART_MSG_CMD_PACK_REC_ACK);
  ringbufferWrapper_putByte(0x00);
  ringbufferWrapper_putByte(0xF9);
  // 1ST MESSAGE
  tempInt32_t = messageIOBuffer_getFreeSlot(&miob);
  if (tempInt32_t < 0){
    return parser_testsuiteReturner(-1);
  }
  // 1st message - 1st parse - must be PARSER_RETURN_MESSAGE_SUCCESSFULLY_RECEPTED
  parserRetVal = parser_parseMessage(123, &miob, tempInt32_t);
  if (parserRetVal != PARSER_RETURN_MESSAGE_SUCCESSFULLY_RECEPTED){
    return parser_testsuiteReturner(-1);
  }
  if (parserFailureFlag != PARSER_FAILURE_FLAG_NO_FAILURE){
    return parser_testsuiteReturner(-1);
  }
  // 2ND MESSAGE
  tempInt32_t = messageIOBuffer_getFreeSlot(&miob);
  if (tempInt32_t < 0){
    return parser_testsuiteReturner(-1);
  }
  // 2nd message - 1st parse - must be PARSER_RETURN_DO_NAK_HANDLING
  parserRetVal = parser_parseMessage(123, &miob, tempInt32_t);
  if (parserRetVal != PARSER_RETURN_DO_NAK_HANDLING){
    return parser_testsuiteReturner(-1);
  }
  if (parserFailureFlag != PARSER_FAILURE_FLAG_FAILURE){
    return parser_testsuiteReturner(-1);
  }
  // 2nd message - 2nd parse - must be PARSER_RETURN_DROP_BYTE
  parserRetVal = parser_parseMessage(123, &miob, tempInt32_t);
  if (parserRetVal != PARSER_RETURN_DROP_BYTE){
    return parser_testsuiteReturner(-1);
  }
  if (parserFailureFlag != PARSER_FAILURE_FLAG_FAILURE){
    return parser_testsuiteReturner(-1);
  }
  // 2nd message - 3rd parse - must be PARSER_RETURN_DROP_BYTE
  parserRetVal = parser_parseMessage(123, &miob, tempInt32_t);
  if (parserRetVal != PARSER_RETURN_DROP_BYTE){
    return parser_testsuiteReturner(-1);
  }
  if (parserFailureFlag != PARSER_FAILURE_FLAG_FAILURE){
    return parser_testsuiteReturner(-1);
  }
  // 2nd message - 4th parse - must be PARSER_RETURN_DROP_BYTE
  parserRetVal = parser_parseMessage(123, &miob, tempInt32_t);
  if (parserRetVal != PARSER_RETURN_DROP_BYTE){
    return parser_testsuiteReturner(-1);
  }
  if (parserFailureFlag != PARSER_FAILURE_FLAG_FAILURE){
    return parser_testsuiteReturner(-1);
  }
  // 3RD MESSAGE
  tempInt32_t = messageIOBuffer_getFreeSlot(&miob);
  if (tempInt32_t < 0){
    return parser_testsuiteReturner(-1);
  }
  // 3rd message - 1st parse - must be PARSER_RETURN_MESSAGE_SUCCESSFULLY_RECEPTED
  parserRetVal = parser_parseMessage(123, &miob, tempInt32_t);
  if (parserRetVal != PARSER_RETURN_MESSAGE_SUCCESSFULLY_RECEPTED){
    return parser_testsuiteReturner(-1);
  }
  if (parserFailureFlag != PARSER_FAILURE_FLAG_NO_FAILURE){
    return parser_testsuiteReturner(-1);
  }
  parser_resetEverythingAndMiob(&miob);
  
  //=============== TEST MIXED BEHAVIOUR: PUSH C/IC/C IN BUFFER AND PARSE THEM
  ringbufferWrapper_putByte(UART_AWAITING_MAGIC);
  ringbufferWrapper_putByte(UART_MSG_CMD_PACK_REC_ACK);
  ringbufferWrapper_putByte(0x00);
  ringbufferWrapper_putByte(0xF9);
  ringbufferWrapper_putByte(UART_AWAITING_MAGIC);
  ringbufferWrapper_putByte(UART_MSG_CMD_HCI_COMMAND_RESPONSE);
  ringbufferWrapper_putByte(0x01);
  ringbufferWrapper_putByte(0xB8);
  ringbufferWrapper_putByte(UART_AWAITING_MAGIC);
  ringbufferWrapper_putByte(UART_MSG_CMD_PACK_REC_ACK);
  ringbufferWrapper_putByte(0x00);
  ringbufferWrapper_putByte(0xF9);
  // 1ST MESSAGE
  tempInt32_t = messageIOBuffer_getFreeSlot(&miob);
  if (tempInt32_t < 0){
    return parser_testsuiteReturner(-1);
  }
  // 1st message - 1st parse - must be PARSER_RETURN_MESSAGE_SUCCESSFULLY_RECEPTED
  parserRetVal = parser_parseMessage(123, &miob, tempInt32_t);
  if (parserRetVal != PARSER_RETURN_MESSAGE_SUCCESSFULLY_RECEPTED){
    return parser_testsuiteReturner(-1);
  }
  if (parserFailureFlag != PARSER_FAILURE_FLAG_NO_FAILURE){
    return parser_testsuiteReturner(-1);
  }
  // 2ND MESSAGE
  tempInt32_t = messageIOBuffer_getFreeSlot(&miob);
  if (tempInt32_t < 0){
    return parser_testsuiteReturner(-1);
  }
  // 2nd message - 1st parse - must be PARSER_RETURN_DO_NAK_HANDLING
  parserRetVal = parser_parseMessage(123, &miob, tempInt32_t);
  if (parserRetVal != PARSER_RETURN_DO_NAK_HANDLING){
    return parser_testsuiteReturner(-1);
  }
  if (parserFailureFlag != PARSER_FAILURE_FLAG_FAILURE){
    return parser_testsuiteReturner(-1);
  }
  // 2nd message - 2nd parse - must be PARSER_RETURN_DROP_BYTE
  parserRetVal = parser_parseMessage(123, &miob, tempInt32_t);
  if (parserRetVal != PARSER_RETURN_DROP_BYTE){
    return parser_testsuiteReturner(-1);
  }
  if (parserFailureFlag != PARSER_FAILURE_FLAG_FAILURE){
    return parser_testsuiteReturner(-1);
  }
  // 2nd message - 3rd parse - must be PARSER_RETURN_DROP_BYTE
  parserRetVal = parser_parseMessage(123, &miob, tempInt32_t);
  if (parserRetVal != PARSER_RETURN_DROP_BYTE){
    return parser_testsuiteReturner(-1);
  }
  if (parserFailureFlag != PARSER_FAILURE_FLAG_FAILURE){
    return parser_testsuiteReturner(-1);
  }
  // 2nd message - 4th parse - must be PARSER_RETURN_DROP_BYTE
  parserRetVal = parser_parseMessage(123, &miob, tempInt32_t);
  if (parserRetVal != PARSER_RETURN_DROP_BYTE){
    return parser_testsuiteReturner(-1);
  }
  if (parserFailureFlag != PARSER_FAILURE_FLAG_FAILURE){
    return parser_testsuiteReturner(-1);
  }
  // 3RD MESSAGE
  tempInt32_t = messageIOBuffer_getFreeSlot(&miob);
  if (tempInt32_t < 0){
    return parser_testsuiteReturner(-1);
  }
  // 3rd message - 1st parse - must be PARSER_RETURN_MESSAGE_SUCCESSFULLY_RECEPTED
  parserRetVal = parser_parseMessage(123, &miob, tempInt32_t);
  if (parserRetVal != PARSER_RETURN_MESSAGE_SUCCESSFULLY_RECEPTED){
    return parser_testsuiteReturner(-1);
  }
  if (parserFailureFlag != PARSER_FAILURE_FLAG_NO_FAILURE){
    return parser_testsuiteReturner(-1);
  }
  parser_resetEverythingAndMiob(&miob);
  
  //=============== TEST MIXED BEHAVIOUR: PUSH SPAM IN BUFFER AND PARSE THEM
  for (i = 0; i < 129; i++){
    ringbufferWrapper_putByte(0x00);
  }
  ringbufferWrapper_putByte(UART_AWAITING_MAGIC);
  ringbufferWrapper_putByte(UART_MSG_CMD_PACK_REC_ACK);
  ringbufferWrapper_putByte(0x00);
  ringbufferWrapper_putByte(0xF9);
  // 1st parse must be NAK
  parserRetVal = parser_parseMessage(123, &miob, tempInt32_t);
  if (parserRetVal != PARSER_RETURN_DO_NAK_HANDLING){
    return parser_testsuiteReturner(-1);
  }
  if (parserFailureFlag != PARSER_FAILURE_FLAG_FAILURE){
    return parser_testsuiteReturner(-1);
  }
  // 2nd to 129th byte drop
  for (i = 1; i < 129; i++){
      parserRetVal = parser_parseMessage(123, &miob, tempInt32_t);
    if (parserRetVal != PARSER_RETURN_DROP_BYTE){
      return parser_testsuiteReturner(-1);
    }
    if (parserFailureFlag != PARSER_FAILURE_FLAG_FAILURE){
      return parser_testsuiteReturner(-1);
    }
  }
  // 130th must be a successfull msg
  parserRetVal = parser_parseMessage(123, &miob, tempInt32_t);
  if (parserRetVal != PARSER_RETURN_MESSAGE_SUCCESSFULLY_RECEPTED){
    return parser_testsuiteReturner(-1);
  }
  if (parserFailureFlag != PARSER_FAILURE_FLAG_NO_FAILURE){
    return parser_testsuiteReturner(-1);
  }
  parser_resetEverythingAndMiob(&miob);
  
  //=============== TEST OVERFLOW - FROM OK CONDITION
  for (i = 0; i < 300; i++){
    ringbufferWrapper_putByte(0x00);
  }
  parserRetVal = parser_parseMessage(123, &miob, tempInt32_t);
  if (parserRetVal != PARSER_RETURN_DO_NAK_HANDLING){
    return parser_testsuiteReturner(-1);
  }
  // The flag must be reset, because another failure might occur!
  if (parserFailureFlag != PARSER_FAILURE_FLAG_NO_FAILURE){
    return parser_testsuiteReturner(-1);
  }
  parser_resetEverythingAndMiob(&miob);
  
  //=============== TEST OVERFLOW - FROM FAILURE CONDITION
  ringbufferWrapper_putByte(UART_AWAITING_MAGIC);
  ringbufferWrapper_putByte(UART_MSG_CMD_PACK_REC_ACK);
  ringbufferWrapper_putByte(0x00);
  ringbufferWrapper_putByte(0xF8);
  parserRetVal = parser_parseMessage(123, &miob, tempInt32_t);
  if (parserRetVal != PARSER_RETURN_DO_NAK_HANDLING){
    return parser_testsuiteReturner(-1);
  }
  if (parserFailureFlag != PARSER_FAILURE_FLAG_FAILURE){
    return parser_testsuiteReturner(-1);
  }
  for (i = 0; i < 300; i++){
    ringbufferWrapper_putByte(0x00);
  }
  // As the NAK condition was already sent, we should not request another
  // as such the return value is timeout
  parserRetVal = parser_parseMessage(123, &miob, tempInt32_t);
  if (parserRetVal != PARSER_RETURN_TIMEOUT){
    return parser_testsuiteReturner(-1);
  }
  // The flag must be reset, because another failure might occur!
  if (parserFailureFlag != PARSER_FAILURE_FLAG_NO_FAILURE){
    return parser_testsuiteReturner(-1);
  }
  parser_resetEverythingAndMiob(&miob);
  
  return 0;
}
#endif