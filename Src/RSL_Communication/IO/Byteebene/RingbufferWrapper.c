/**
  ******************************************************************************
  * @file       RingbufferWrapper.c
  * @author     Tim Steinberg
  * @date       11.05.2020
  * @brief      This is an extension for the ringbuffer made by me. It adds a
  *             statemachine to handle overflow and no-initialization as well as
  *             different return states to help you analyze in case of
  *             "something went wrong".
  *             Please take note, that this buffer wrapper does NOT care about
  *             correctness as much as the ringbuffer does. E.g. if you ask for
  *             a byte while the buffer is empty, you will get no protest/errorö.
  *             If you use this unit YOU must be sure in what YOU do.
  *             Optimized for the use of the LEESYS Stack.
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
#include "Ringbuffer.h"
#include "RingbufferWrapper.h"
#include "Stack_Definitions.h"
/* Typedefinitions */

/* Variables */
/** \brief ringbufferArray[RINGBUFFER_SIZE]
  *        ringbufferArray is the buffer that will be used inside the ringbuffer
  *        RINGBUFFER_SIZE length of the buffer, defined in "Stack_Definitions.h"
*/
uint8_t ringbufferArray[RINGBUFFER_SIZE];

/** \brief rb The ringbuffer object itself with the standard values. */
ringbuffer rb = {
  .length = RINGBUFFER_SIZE,    /**< Size = Size of the related buffer.*/
  .count = 0,                   /**< Set initial reception count to 0.*/
  .readptr = ringbufferArray,   /**< Set initial readpointer to first byte.*/
  .writeptr = ringbufferArray,  /**< Set initial writepointer to first byte.*/
  .buffer = ringbufferArray,    /**< Set buffer pointer to first byte.*/
};

/** \brief receptionBuffer_state
  *        The internal state object of the buffer.
  */
RINGBUFFER_WRAPPER_STATE_VALUES_TYPEDEF ringbufferWrapper_state = RINGBUFFER_WRAPPER_STATE_OK;

/* Function definitions */

/** @brief This method asks the count of bytes in the ringbuffer.
 *  @return The count of bytes in the buffer.
 */
uint32_t ringbufferWrapper_getCount(void){
  // Just forward the result, nothing much to wrap here
  return ringbufferGetCount(&rb);
}

/** @brief This method clears the ringbuffer and resets the state.
 *  @return Nothing.
 */
void ringbufferWrapper_clear(void){
  // Clear the buffer
  ringbufferClear(&rb);
  // Reset the state
  ringbufferWrapper_state = RINGBUFFER_WRAPPER_STATE_OK;
}

/** @brief This method is used to peek inside the ringbuffer and return the
 *         byte at the position asked.
 *  @param position Is the position you want to take a look at.
 *  @return The byte you ask for. WARNING: No prevention regarding position!
 */
uint8_t ringbufferWrapper_peekByte(uint32_t position){
  // Just forward the result, nothing much to wrap here
  return ((uint8_t) (ringbufferPeekCharPosition(&rb, position) & 0xFF));
}

/** @brief This method will drop the first byte from the ringbuffer.
 *  @return Nothing.
 */
void ringbufferWrapper_dropByte(void){
  // Just forward the result, nothing much to wrap here
  ringbufferGetChar(&rb);
}

/** @brief This method will drop the first COUNT bytes from the ringbuffer.
 *  @param count The number of bytes you want to drop.
 *  @return Nothing.
 */
void ringbufferWrapper_dropBytesCount(uint32_t count){  
  // We might use get string, but we'd have to create a buffer here
  while(count > 0){
    ringbufferGetChar(&rb);
    count --;
  }
}

/** @brief This method will get the first byte from the ringbuffer (peek+drop)
 *  @return Nothing.
 */
uint8_t ringbufferWrapper_getByte(void){
  // Just forward the result, nothing much to wrap here
  return ((uint8_t) (ringbufferGetChar(&rb) & 0xFF));
}

/** @brief This method will put a byte in the ringbuffer. If you put in too many
 *         the system will set the overflow flag.
 *  @param byte The byte you want to put in.
 *  @return Nothing.
 */
void ringbufferWrapper_putByte(uint8_t byte){
  // Put the byte and check if the result was an EOF
  if (ringbufferPutChar(&rb, byte) == EOF){
    // Yes
    
    // Set state to overflow
    ringbufferWrapper_state = RINGBUFFER_WRAPPER_STATE_OVERFLOW;
  }
}

/** @brief This method will return the flag state.
 *  @return The flag state of the OVERFLOW flag.
 */
RINGBUFFER_WRAPPER_STATE_VALUES_TYPEDEF    ringbufferWrapper_flagState(void){
  return ringbufferWrapper_state;
}

#if TEST_RINGBUFFER_WRAPPER >= 1

/** @brief This method will free variables and set back things to return from 
 *         the test.
 *  @param retVal The value you want to return.
 *  @return The returnvalue you enter.
 */
int32_t ringbufferWrapper_returner(int32_t retVal){
  return retVal;
}

/** @brief This method is the test for this unit
 *  @return The returnvalue. 0 == OK, <0 == FAILURE
 */
int ringbufferWrapper_testsuite(){
  int32_t i = 0;
  // Due to this unit having NO sanity checks, a user might want to check some
  // values manually. This is what those variables are for. DON'T use optimization
  // if you want to see anything!
  uint8_t debugUint8_t;
  uint32_t debugUint32_t;
  
  ringbufferWrapper_clear();
  
  // Check getCount and putByte for NOT overflowing conditions
  // Put 0...127 in buffer
  for (i = 0; i < 128; i++){
    ringbufferWrapper_putByte((uint8_t) (i & 0xFF));
  }
  // count must be 128
  if (ringbufferWrapper_getCount() != 128){
    return ringbufferWrapper_returner(-1);
  }
  
  // Check peek for NOT overflowing conditions without delete
  // Check elements 0 (=0) and 63 (=63)
  debugUint8_t = ringbufferWrapper_peekByte(0);
  if (debugUint8_t != 0){
    return ringbufferWrapper_returner(-1);
  }
  debugUint8_t = ringbufferWrapper_peekByte(63);
  if (debugUint8_t != 63){
    return ringbufferWrapper_returner(-1);
  }
  
  // Check dropByte for NOT overflowing conditions
  // Drop 64 of 128 (=64 remaining) elements. 1st is now 64, 63rd is now 127
  for (i = 0; i < 64; i++){
    ringbufferWrapper_dropByte();
  }
  if (ringbufferWrapper_getCount() != 64){
    return ringbufferWrapper_returner(-1);
  }
  
  // Check peekByte for NOT overflowing conditions with delete
  // In the buffer is now 64...127 as elements
  debugUint8_t = ringbufferWrapper_peekByte(0);
  if (debugUint8_t != 64){
    return ringbufferWrapper_returner(-1);
  }
  debugUint8_t = ringbufferWrapper_peekByte(63);
  if (debugUint8_t != 127){
    return ringbufferWrapper_returner(-1);
  }
  
  // Drop 32 of our 64 (=32 remaining) elements. 1st is now 96, 31st is now 127
  ringbufferWrapper_dropBytesCount(32);
  debugUint32_t = ringbufferWrapper_getCount();
  if (debugUint32_t != 32){
    return ringbufferWrapper_returner(-1);
  }
  
  // Check clear for NOT overflowing conditions
  // Kick them all and reset pointers AND reset flag
  ringbufferWrapper_clear();
  debugUint8_t = ringbufferWrapper_getCount();
  if (debugUint8_t != 0){
    return ringbufferWrapper_returner(-1);
  }
  
  // Check newer put and order for NOT overflowing conditions
  // Put 32 to 128 in buffer (96 elements, 1st = 32, 95th is 127)
  for (i = 32; i < 128; i++){
    ringbufferWrapper_putByte((uint8_t) (i & 0xFF));
  }
  debugUint8_t = ringbufferWrapper_peekByte(0);
  if (debugUint8_t != 32){
    return ringbufferWrapper_returner(-1);
  }
  debugUint8_t = ringbufferWrapper_peekByte(95);
  if (debugUint8_t != 127){
    return ringbufferWrapper_returner(-1);
  }
  
  // Check flag, it must be not set, because we had NO overflow
  if (ringbufferWrapper_flagState() != RINGBUFFER_WRAPPER_STATE_OK){
    return ringbufferWrapper_returner(-1);
  }
  if (ringbufferWrapper_getCount() != 96){
    return ringbufferWrapper_returner(-1);
  }
  
  // Now let's overflow this
  // Now we put in 0 to 160, so we have [32...127 followed by 0...160]
  // = 256 elements
  for (i = 0; i < 160; i++){
    ringbufferWrapper_putByte((uint8_t) (i & 0xFF));
  }
  // It's full, but NOT overflowed yet
  if (ringbufferWrapper_flagState() != RINGBUFFER_WRAPPER_STATE_OK){
    return ringbufferWrapper_returner(-1);
  }
  if (ringbufferWrapper_getCount() != 256){
    return ringbufferWrapper_returner(-1);
  }
  // Now overfill it
  ringbufferWrapper_putByte(64);
  if (ringbufferWrapper_flagState() != RINGBUFFER_WRAPPER_STATE_OVERFLOW){
    return ringbufferWrapper_returner(-1);
  }
  if (ringbufferWrapper_getCount() != 256){
    return ringbufferWrapper_returner(-1);
  }
  
  // Get out a byte, check the count, check the flag (must be still set)
  for(i = 32; i < 128; i++){
    debugUint8_t = ringbufferWrapper_getByte();
    if (debugUint8_t != i){
      return ringbufferWrapper_returner(-1);
    }
  }
  for(i = 0; i < 160; i++){
    debugUint8_t = ringbufferWrapper_getByte();
    if (debugUint8_t != i){
      return ringbufferWrapper_returner(-1);
    }
  }
  if (ringbufferWrapper_flagState() != RINGBUFFER_WRAPPER_STATE_OVERFLOW){
    return ringbufferWrapper_returner(-1);
  }
  
  // Check the clear function
  ringbufferWrapper_clear();
  debugUint8_t = ringbufferWrapper_getCount();
  if (debugUint8_t != 0){
    return ringbufferWrapper_returner(-1);
  }
  if (ringbufferWrapper_flagState() != RINGBUFFER_WRAPPER_STATE_OK){
    return ringbufferWrapper_returner(-1);
  }
  
  return ringbufferWrapper_returner(0);
}
#endif