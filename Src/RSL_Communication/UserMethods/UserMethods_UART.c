/**
  ******************************************************************************
  * @file       UserMethods_UART.c
  * @author     Tim Steinberg
  * @date      	18.05.2020
  * @brief      Wrapper for user methods regarding UART-Handling
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
  * | 002       | 2020-06-15    | Tim Steinberg         | Added comments, test and the debug CB         |
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
#include <stdint.h>

#include "Test_Selector.h"
#include "Handler_Timer.h"
#include "RingbufferWrapper.h"
#include "Parser.h"
#include "UserMethods_UART.h"
#include "UART_RSL.h"

#include "stm32l0xx_hal.h"
/* Typedefinitions */

/* Variables */
/** \brief debugCallbackTransmission A function pointer to a test method that
  *        needs the things the STM sends (instead of use a UART for transmit 
  */
uartCallback debugCallbackTransmission  = NULL;

/* Function definitions */

/** @brief This method will be called when the stack wants to transfer bytes.
 *  @param lengthBuffer is the count of bytes to transmit.
 *  @param *buffer is the pointer to the position within the byte field to be transmitted.
 *  @return The count of bytes that was transmitted.
 */
uint32_t userMethods_uartTransmit(uint32_t lengthBuffer, uint8_t *buffer){
#if TEST_ACTIVATE_UART_CALLBACK == 1
  if (debugCallbackTransmission != NULL){
    debugCallbackTransmission(buffer, lengthBuffer);
  }
  #warning THE UART WILL SEND NOTHING DUE TO TEST MODE BEING ACTIVE
#else
  uart_rsl_transmit(lengthBuffer, buffer);
#endif
  return lengthBuffer;
}

/** @brief This method must be called by the user, if a byte was recepted.
 *         The function will reset an onging integrity timer and push the byte
 *         in the reception buffer. It will start another reception.
 *  @param time is the actual tick (-> HAL_GetTick() )
 *  @return Nothing.
 */
void userMethods_uartReceptionCallback(uint32_t time){
  parser_timerMessageIntegrityStartISP(time);
  ringbufferWrapper_putByte(uart_rsl_getReceptedByte());
  uart_rsl_startReception();
}

/** @brief This method must be called by the user, if there was a uart error.
 *         This function will start another reception.
 *  @param time is the actual tick (-> HAL_GetTick() )
 *  @return Nothing.
 */
void userMethods_uartError(uint32_t time){
  parser_timerMessageIntegrityStartISP(time);
  uart_rsl_startReception();
}

/** @brief This will be called, if the user should stop recept bytes.
 *  @return Nothing.
 */
void userMethods_stopReception(){
  
}

/** @brief This will be called, if the user must resume recept bytes.
 *  @return Nothing.
 */
void userMethods_startReception(){
  
}

#if TEST_ACTIVATE_UART_CALLBACK == 1

void userMethods_txRegisterCallbackForTransmission(uartCallback func){
  debugCallbackTransmission = func;
}

#endif

#if TEST_USERMETHODS_UART >= 1
int userMethods_testsuite(){
  // Test if you can sent a "very long" buffer and get the number of bytes returned
  uint8_t buffer[257];
  for (int i = 0; i < 257; i++){
    buffer[i] = (uint8_t) (i & 0xFF);
  }
  if (userMethods_uartTransmit(257, buffer) != 257){
    return -1;
  }
  
  // There is no sense in testing more, due to the whole thing being user dependent
  return 0;
}
#endif
