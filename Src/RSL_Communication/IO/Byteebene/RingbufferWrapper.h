/**
  ******************************************************************************
  * @file       RingbufferWrapper.h
  * @author     Tim Steinberg
  * @date       11.05.2020
  * @brief      This is an extension for the ringbuffer made by me. It adds a
  *             statemachine to handle overflow and no-initialization as well as
  *             different return states to help you analyze in case of
  *             "something went wrong".
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

#ifndef __RECEPTION_BUFFER_H
#define __RECEPTION_BUFFER_H

/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "Test_Selector.h"

/* Typedefinitions */
/**
 * @brief This typedef defines the inner state of the buffer.
 * 
 * RECEPTION_BUFFER_STATE_OK - reception buffer has no error detected
 * RECEPTION_BUFFER_STATE_OVERFLOW - reception buffer detected overflow
 */
typedef enum RINGBUFFER_WRAPPER_STATE_VALUES {
  RINGBUFFER_WRAPPER_STATE_OK             = 0x00, /**< Buffer is OK */
  RINGBUFFER_WRAPPER_STATE_OVERFLOW       = 0xFF, /**< Buffer flowed over */
} RINGBUFFER_WRAPPER_STATE_VALUES_TYPEDEF;

/* Variables */

/* Function definitions */
  
/** @brief This method asks the count of bytes in the ringbuffer.
 *  @return The count of bytes in the buffer.
 */
uint32_t                                        ringbufferWrapper_getCount(void);

/** @brief This method clears the ringbuffer and resets the state.
 *  @return Nothing.
 */
void                                            ringbufferWrapper_clear(void);

/** @brief This method is used to peek inside the ringbuffer and return the
 *         byte at the position asked.
 *  @param position Is the position you want to take a look at.
 *  @return The byte you ask for. WARNING: No prevention regarding position!
 */
uint8_t                                         ringbufferWrapper_peekByte(uint32_t position);

/** @brief This method will drop the first byte from the ringbuffer.
 *  @return Nothing.
 */
void                                            ringbufferWrapper_dropByte(void);

/** @brief This method will drop the first COUNT bytes from the ringbuffer.
 *  @param count The number of bytes you want to drop.
 *  @return Nothing.
 */
void                                            ringbufferWrapper_dropBytesCount(uint32_t count);

/** @brief This method will get the first byte from the ringbuffer (peek+drop)
 *  @return Nothing.
 */
uint8_t                                         ringbufferWrapper_getByte(void);

/** @brief This method will put a byte in the ringbuffer. If you put in too many
 *         the system will set the overflow flag.
 *  @param byte The byte you want to put in.
 *  @return Nothing.
 */
void                                            ringbufferWrapper_putByte(uint8_t byte);

/** @brief This method will return the flag state.
 *  @return The flag state of the OVERFLOW flag.
 */
RINGBUFFER_WRAPPER_STATE_VALUES_TYPEDEF         ringbufferWrapper_flagState(void);

#if TEST_RINGBUFFER_WRAPPER >= 1

/** @brief This method is the test for this unit
 *  @return The returnvalue. 0 == OK, <0 == FAILURE
 */
int ringbufferWrapper_testsuite();

#endif

#endif