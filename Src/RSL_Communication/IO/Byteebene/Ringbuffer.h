/**
  ******************************************************************************
  * @file       Ringbuffer.h
  * @author     Tim Steinberg
  * @date       28.11.2016
  * @brief      A ringbuffer for uint8_t types
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
  * | 001       | 2016-11-28    | Tim Steinberg         | Initial version / skeleton of file            |
  * |-----------|---------------|-----------------------|-----------------------------------------------|
  * |           |               |                       |                                               |
  * |-----------|---------------|-----------------------|-----------------------------------------------|
  * |           |               |                       |                                               |
  * |-----------|---------------|-----------------------|-----------------------------------------------|
  * |           |               |                       |                                               |
  * |-----------|---------------|-----------------------|-----------------------------------------------|
  * |           |               |                       |                                               |
  * |-----------|---------------|-----------------------|-----------------------------------------------|
  *~~~
  */

#ifndef __RINGBUFFER_H
#define __RINGBUFFER_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "Test_Selector.h"

/**
 * @brief This structure is the ringerbuffer. It's a kind of circle FIFO and should be POSIX conform, except the look ahead function.
 * 
 * This buffer knows it's own length and count of used byte. You might be able to calculate both from the read and write pointer (by aligning it to 128 or whatever 2^x you want it to be),
 * but these 8 bytes are totally worth the overhead and safes us from wasting cycles. And you can have as many of them as you wish, until your RAM is a goner.
 */
typedef struct {
  uint32_t length;              /**< The length of this buffer.*/
  volatile uint32_t count;      /**< The used bytes of this buffer. */
  volatile uint8_t *readptr;    /**< Where do we read actually? */
  volatile uint8_t *writeptr;   /**< Where do we write actually? */
  uint8_t *buffer;              /**< The pointer to the buffer itself (it can be located somewhere else). */
} ringbuffer;

#ifdef EOF
  #if EOF != (-1)
    #error WARNING EOF HAS BEEN DEFINED WITH SOMETHING ELSE THAN -1!
  #else
    #define EOF (-1)
  #endif
#endif

/** @brief This method is used to create a completely new ringbuffer in runtime.
 *  @param length refers to the length of the ringbuffer.
 *  @return ringbuffer* which is the pointer to the object itself.
 */
ringbuffer*     ringbufferCreateCompleteObject  (uint32_t length);

/** @brief This method is used to create a completely new ringbuffer in runtime.
 *  @param length refers to the length of the ringbuffer.
 *  @param *buf is a struct which contains the pointers, but not the buffer field.
 *  @return length of the buffer field.
 *  @return -1 upon fail
 */
int             ringbufferCreateBuffer          (ringbuffer *buf, uint32_t length);

/** @brief This method will completely free the object.
 *  @param *buf is the pointer to the ringbuffer object.
 */
void            ringbufferFreeCompleteObject    (ringbuffer *buf);

/** @brief This method will only free the buffer.
 *  @param *buf is the pointer to the ringbuffer object.
 */
void            ringbufferFreeBuffer            (ringbuffer *buf);

/** @brief This method will put a byte into buffer.
 *  @param *buf is the pointer to the ringbuffer object.
 *  @param data is the byte to fit in.
 *  @return EOF if buffer is nullpointer.
 *  @return EOF if buffer full.
 *  @return 0 upon success.
 */
int             ringbufferPutChar               (ringbuffer *buf, uint8_t data);

/** @brief This method get a byte out of the ringbuffer.
 *  @param *buf is the pointer to the ringbuffer object.
 *  @return EOF if buffer is nullpointer.
 *  @return EOF if buffer is less filled then you want to read.
 *  @return [0;255] upon success, which is the byte you wanted.
 */
int             ringbufferGetChar               (ringbuffer *buf);

/** @brief This method get a byte out of the ringbuffer.
 *  @param *buf is the pointer to the ringbuffer object.
 *  @param position is the offset regarding to the first byte you would've get
 *  if you called ringbufferGetChar(*buf).
 *  @return EOF if buffer is nullpointer.
 *  @return EOF if buffer count of bytes is smaller than your intended position.
 *  @return [0;255] upon success, which is the byte you wanted.
 */
int             ringbufferPeekCharPosition      (ringbuffer *buf, uint32_t position);

/** @brief This method will clear the buffer by setting the read and write pointer.
 *  It will NOT write 0x00 over every byte the buffer is made of.
 *  @param *buf is the pointer to the ringbuffer object.
 */
void            ringbufferClear                 (ringbuffer *buf);

/** @brief This method will give you the count of bytes in the ringbuffer.
 *  @param *buf is the pointer to the ringbuffer object.
 *  @return count of bytes in buffer.
 */
uint32_t        ringbufferGetCount              (ringbuffer *buf);

/** @brief This method get a number of bytes out of the ringbuffer.
 *  @param *buf is the pointer to the ringbuffer object.
 *  @param *buffer is the pointer to a field where to put the read bytes at.
 *  @param numberOfBytes is how many bytes you intend to read.
 *  @return EOF if buffer is nullpointer.
 *  @return EOF if buffer is less filled then you want to read.
 *  @return 0 upon success.
 */
int             ringbufferGetString             (ringbuffer *buf, uint8_t *buffer, uint32_t numberOfBytes);

#if TEST_RINGBUFFER >= 1
/** @brief This method is there to test the ringbuffer by using it with sample data
 *  and interaction.
 *  @return 0 upon success.
 *  @return -1 else.
 */
int             ringbufferTestsuite             (void);
#endif



#endif
