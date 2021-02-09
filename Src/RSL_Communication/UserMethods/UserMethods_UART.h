/**
  ******************************************************************************
  * @file       UserMethods_UART.h
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

#ifndef __USER_METHODS_UART_H
#define __USER_METHODS_UART_H

/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "Test_Selector.h"

/* Typedefinitions */
/**
 * @brief This typedef defines a type which will be used for tests and their 
 * calls. The function will feed the transmitted data back to the tests, which 
 * can then compare the transmission to their exptected data.
 */
typedef void(*uartCallback)(uint8_t*, uint32_t len);

/* Variables */

/* Function definitions */

/** @brief This method will be called when the stack wants to transfer bytes.
 *  @param lengthBuffer is the count of bytes to transmit.
 *  @param *buffer is the pointer to the position within the byte field to be transmitted.
 *  @return The count of bytes that was transmitted.
 */
uint32_t userMethods_uartTransmit(uint32_t lengthBuffer, uint8_t *buffer);

/** @brief This method must be called by the user, if a byte was recepted.
 *         The function will reset an onging integrity timer and push the byte
 *         in the reception buffer. It will start another reception.
 *  @param time is the actual tick (-> HAL_GetTick() )
 *  @return Nothing.
 */
void userMethods_uartReceptionCallback(uint32_t time);

/** @brief This method must be called by the user, if there was a uart error.
 *         This function will start another reception.
 *  @param time is the actual tick (-> HAL_GetTick() )
 *  @return Nothing.
 */
void userMethods_uartError(uint32_t time);

/** @brief This will be called, if the user should stop recept bytes.
 *  @return Nothing.
 */
void userMethods_stopReception();

/** @brief This will be called, if the user must resume recept bytes.
 *  @return Nothing.
 */
void userMethods_startReception();

#if TEST_ACTIVATE_UART_CALLBACK == 1
/** @brief This will be by tests to register their callback for transmission.
 *         Please take note, that thus NO data will be transmitted by UART.
 *  @return Nothing.
 */
void userMethods_txRegisterCallbackForTransmission(uartCallback func);
#endif

#if TEST_USERMETHODS_UART >= 1
/** @brief This will be called by the test iterator and is the main test for
 *         this unit.
 *  @return Nothing.
 */
int userMethods_testsuite();
#endif

#endif