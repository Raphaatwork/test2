/**
  ******************************************************************************
  * @file       Handler_NAK_Transmission.h
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

#ifndef __HANDLER_NAK_TRANSMISSION_H
#define __HANDLER_NAK_TRANSMISSION_H

/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "Test_Selector.h"

/* Typedefinitions */

/**
 * @brief This typedef defines the result of a NAK transmission comparison
 *        NO_PROBLEM = We are still good
 *        TOO_MANY = We have too many successive NAKS...
 */
typedef enum NAK_TRANSMISSION_STATE_VALUES {
  NAK_TRANSMISSION_STATE_NO_PROBLEM                     = 0x00, /**< NAK count still in valid numbers */
  NAK_TRANSMISSION_STATE_TOO_MANY                       = 0x01, /**< NAK count hit limit */
} NAK_TRANSMISSION_STATE_VALUES_TYPEDEF;

/* Variables */

/* Function declarations */
  
/** @brief This method is used to handle a new occurance of a NAK. It will
 *         send a NAK or it will change the state to "too many naks" which
 *         will be used to reset the whole system.
 *  @return Nothing.
 */
void handlerNAK_sendNAK();

/** @brief This method is used to reset the NAK counter
 *  @return Nothing.
 */
void handlerNAK_resetCounter();

/** @brief This method is used get the count of NAKs
 *  @return The count.
 */
uint8_t handlerNAK_getCounter();

/** @brief This method is used get the NAK handler state
 *  @return The state.
 */
NAK_TRANSMISSION_STATE_VALUES_TYPEDEF handlerNAK_getState();

/** @brief This method is used to reset the NAK handler state
 *  @return Nothing.
 */
void handlerNAK_resetState();

#if TEST_HANDLER_NAK_TRANSMISSION >= 1

/** @brief This method is the test for this unit
 *  @return The returnvalue. 0 == OK, <0 == FAILURE
 */
int handlerNAK_testsuite();

#endif

#endif
