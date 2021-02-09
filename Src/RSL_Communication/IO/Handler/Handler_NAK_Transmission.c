/**
  ******************************************************************************
  * @file       Handler_NAK_Transmission.c
  * @author     Tim Steinberg
  * @date       11.05.2020
  * @brief      Handler for incorrect reception nacknowledgement and failure counting
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
#include "Test_Selector.h"
#include "Message_Definitions.h"
#include "Handler_NAK_Transmission.h"
#include "userMethods_UART.h"

/* Typedefinitions */

/* Variables */
/** \brief handlerNAK_state The handler state object and its initial value */
NAK_TRANSMISSION_STATE_VALUES_TYPEDEF handlerNAK_state = NAK_TRANSMISSION_STATE_NO_PROBLEM;
/** \brief nakHandler_counter The counter for repeated NAK transmission*/
uint32_t nakHandler_counter = 0;
/** \brief msgBufferNAK The buffer for the NAK message*/
uint8_t msgBufferNAK[] = { UART_TRANSMISSION_MAGIC, UART_MSG_CMD_PACK_REC_NAK, 0x00, 0xAA ^ UART_TRANSMISSION_MAGIC ^ UART_MSG_CMD_PACK_REC_NAK};

#if TEST_HANDLER_NAK_TRANSMISSION >= 1
  /** \brief transmissionCounter counter for testing purpose (to see, if called)*/
  uint8_t transmissionCounter;
  
  /** @brief This method is used by tests to increment the counter
   *  @return Nothing.
   */
  void handlerNAK_incTransmissionCounter(){
    transmissionCounter++;
  }
#endif

/* Function definitions */
  
/** @brief This method is used to handle a new occurance of a NAK. It will
 *         send a NAK or it will change the state to "too many naks" which
 *         will be used to reset the whole system.
 *  @return Nothing.
 */
void handlerNAK_sendNAK(){  
  if (++nakHandler_counter >= 3){
    handlerNAK_state = NAK_TRANSMISSION_STATE_TOO_MANY;
  }else{
    #if TEST_HANDLER_NAK_TRANSMISSION >= 1
      handlerNAK_incTransmissionCounter();
    #endif
    userMethods_uartTransmit(4, msgBufferNAK);
  }
}

/** @brief This method is used to reset the NAK counter
 *  @return Nothing.
 */
void handlerNAK_resetCounter(){
  nakHandler_counter = 0;
}

/** @brief This method is used get the count of NAKs
 *  @return The count.
 */
uint8_t handlerNAK_getCounter(){
  return nakHandler_counter;
}

/** @brief This method is used get the NAK handler state
 *  @return The state.
 */
NAK_TRANSMISSION_STATE_VALUES_TYPEDEF handlerNAK_getState(){
  return handlerNAK_state;
}

/** @brief This method is used to reset the NAK handler state
 *  @return Nothing.
 */
void handlerNAK_resetState(){
  handlerNAK_state = NAK_TRANSMISSION_STATE_NO_PROBLEM;
}

#if TEST_HANDLER_NAK_TRANSMISSION >= 1

/** @brief This method will free variables and set back things to return from 
 *         the test.
 *  @param retVal The value you want to return.
 *  @return The returnvalue you enter.
 */
int handlerNAK_testsuiteReturner(int retVal){
  return retVal;
}

/** @brief This method is the test for this unit
 *  @return The returnvalue. 0 == OK, <0 == FAILURE
 */
int handlerNAK_testsuite(){
  handlerNAK_resetCounter();
  handlerNAK_resetState();
  transmissionCounter = 0;
  
  // Check settings
  if (handlerNAK_getState() != NAK_TRANSMISSION_STATE_NO_PROBLEM){
    return handlerNAK_testsuiteReturner(-1);
  }
  if (handlerNAK_getCounter() != 0){
    return handlerNAK_testsuiteReturner(-1);
  }
  if (transmissionCounter != 0){
    return handlerNAK_testsuiteReturner(-1);
  }
  
  // Send first NAK
  handlerNAK_sendNAK();
  if (handlerNAK_getState() != NAK_TRANSMISSION_STATE_NO_PROBLEM){
    return handlerNAK_testsuiteReturner(-1);
  }
  if (handlerNAK_getCounter() != 1){
    return handlerNAK_testsuiteReturner(-1);
  }
  if (transmissionCounter != 1){
    return handlerNAK_testsuiteReturner(-1);
  }
  
  // Send second NAK
  handlerNAK_sendNAK();
  if (handlerNAK_getState() != NAK_TRANSMISSION_STATE_NO_PROBLEM){
    return handlerNAK_testsuiteReturner(-1);
  }
  if (handlerNAK_getCounter() != 2){
    return handlerNAK_testsuiteReturner(-1);
  }
  if (transmissionCounter != 2){
    return handlerNAK_testsuiteReturner(-1);
  }
  
  // Send third NAK (= now it all went wrong => handle reset of device)
  handlerNAK_sendNAK();
  if (handlerNAK_getState() != NAK_TRANSMISSION_STATE_TOO_MANY){
    return handlerNAK_testsuiteReturner(-1);
  }
  if (handlerNAK_getCounter() != 3){
    return handlerNAK_testsuiteReturner(-1);
  }
  if (transmissionCounter != 2){
    return handlerNAK_testsuiteReturner(-1);
  }
  
  // Send FOURTH NAK (= now it all went wrong => handle reset of device)
  handlerNAK_sendNAK();
  if (handlerNAK_getState() != NAK_TRANSMISSION_STATE_TOO_MANY){
    return handlerNAK_testsuiteReturner(-1);
  }
  if (handlerNAK_getCounter() != 4){
    return handlerNAK_testsuiteReturner(-1);
  }
  if (transmissionCounter != 2){
    return handlerNAK_testsuiteReturner(-1);
  }
  
  // JUST reset the counter, the state MUST stay
  handlerNAK_resetCounter();
  transmissionCounter = 0;
  if (handlerNAK_getState() != NAK_TRANSMISSION_STATE_TOO_MANY){
    return handlerNAK_testsuiteReturner(-1);
  }
  if (handlerNAK_getCounter() != 0){
    return handlerNAK_testsuiteReturner(-1);
  }
  
  // Now reset the state as well
  handlerNAK_resetState();
  if (handlerNAK_getState() != NAK_TRANSMISSION_STATE_NO_PROBLEM){
    return handlerNAK_testsuiteReturner(-1);
  }
  if (handlerNAK_getCounter() != 0){
    return handlerNAK_testsuiteReturner(-1);
  }
  
  // Set NAK Count to 1
  handlerNAK_sendNAK();
  if (handlerNAK_getState() != NAK_TRANSMISSION_STATE_NO_PROBLEM){
    return handlerNAK_testsuiteReturner(-1);
  }
  if (handlerNAK_getCounter() != 1){
    return handlerNAK_testsuiteReturner(-1);
  }
  if (transmissionCounter != 1){
    return handlerNAK_testsuiteReturner(-1);
  }
  
  // Reset it to 0
  handlerNAK_resetCounter();
  transmissionCounter = 0;
  if (handlerNAK_getState() != NAK_TRANSMISSION_STATE_NO_PROBLEM){
    return handlerNAK_testsuiteReturner(-1);
  }
  if (handlerNAK_getCounter() != 0){
    return handlerNAK_testsuiteReturner(-1);
  }
  
  // Set NAK Count to 4
  handlerNAK_sendNAK();
  handlerNAK_sendNAK();
  handlerNAK_sendNAK();
  handlerNAK_sendNAK();
  if (handlerNAK_getState() != NAK_TRANSMISSION_STATE_TOO_MANY){
    return handlerNAK_testsuiteReturner(-1);
  }
  if (handlerNAK_getCounter() != 4){
    return handlerNAK_testsuiteReturner(-1);
  }
  if (transmissionCounter != 2){
    return handlerNAK_testsuiteReturner(-1);
  }
  
  // Send a fifth, because the counter must still work, it will just not transmit
  handlerNAK_sendNAK();
  if (handlerNAK_getState() != NAK_TRANSMISSION_STATE_TOO_MANY){
    return handlerNAK_testsuiteReturner(-1);
  }
  if (handlerNAK_getCounter() != 5){
    return handlerNAK_testsuiteReturner(-1);
  }
  if (transmissionCounter != 2){
    return handlerNAK_testsuiteReturner(-1);
  }
  
  // Just reset the state, the counter must still work
  handlerNAK_sendNAK();
  handlerNAK_resetState();
  if (handlerNAK_getState() != NAK_TRANSMISSION_STATE_NO_PROBLEM){
    return handlerNAK_testsuiteReturner(-1);
  }
  if (handlerNAK_getCounter() != 6){
    return handlerNAK_testsuiteReturner(-1);
  }
  if (transmissionCounter != 2){
    return handlerNAK_testsuiteReturner(-1);
  }
  
  // Another sendNAK should immediately reset the state
  handlerNAK_sendNAK();
  if (handlerNAK_getState() != NAK_TRANSMISSION_STATE_TOO_MANY){
    return handlerNAK_testsuiteReturner(-1);
  }
  if (handlerNAK_getCounter() != 7){
    return handlerNAK_testsuiteReturner(-1);
  }
  if (transmissionCounter != 2){
    return handlerNAK_testsuiteReturner(-1);
  }
      
  // Reset it to 0
  handlerNAK_resetCounter();
  handlerNAK_resetState();
  transmissionCounter = 0;
  if (handlerNAK_getState() != NAK_TRANSMISSION_STATE_NO_PROBLEM){
    return handlerNAK_testsuiteReturner(-1);
  }
  if (handlerNAK_getCounter() != 0){
    return handlerNAK_testsuiteReturner(-1);
  }
  if (transmissionCounter != 0){
    return handlerNAK_testsuiteReturner(-1);
  }
  
  return 0;
}
#endif