/**
  ******************************************************************************
  * @file       Ringbuffer.c
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

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "Ringbuffer.h"
#include "Test_Selector.h"

// Create Functions
/** @brief This method is used to create a completely new ringbuffer in runtime.
 *  @param length refers to the length of the ringbuffer.
 *  @return ringbuffer* which is the pointer to the object itself.
 */
ringbuffer* ringbufferCreateCompleteObject(uint32_t length){
  ringbuffer* tmp;
  // Allocate new ringbuffer object
  tmp = (ringbuffer*) malloc(sizeof(ringbuffer));
  // Success?
  if (tmp != NULL){
    // Yes
    
    // Allocate new buffer for ringbuffer object
    tmp->buffer = (uint8_t*) malloc(length);
    // Success?
    if (tmp->buffer != NULL){
      // Yes
      
      // Set the ringbuffer object initial values
      tmp->count = 0;
      tmp->length = length;
      tmp->readptr = (volatile uint8_t*) tmp->buffer;
      tmp->writeptr = (volatile uint8_t*) tmp->buffer;
      return tmp;
    }else{
      // No, buffer allocation failed
      
      // Free ringbuffer object
      free(tmp);
      // Return null to show that there is no ringbuffer object
      return NULL;
    }
  }else{ 
    // No, ringbuffer object allocation failed
    
    // Return null to show that there is no ringbuffer object
    return NULL;
  }
}

/** @brief This method is used to create a completely new ringbuffer in runtime.
 *  @param length refers to the length of the ringbuffer.
 *  @param *buf is a struct which contains the pointers, but not the buffer field.
 *  @return length of the buffer field.
 *  @return -1 upon fail
 */
int ringbufferCreateBuffer(ringbuffer *buf, uint32_t length){
  buf->buffer = (uint8_t*) malloc(length);
  if (buf->buffer != NULL){
    return length;
  }else{
    return -1;
  }
}

// Free Functions
/** @brief This method will completely free the object.
 *  @param *buf is the pointer to the ringbuffer object.
 */
void ringbufferFreeCompleteObject(ringbuffer *buf){
  free(buf->buffer);
  free(buf);
}

/** @brief This method will only free the buffer.
 *  @param *buf is the pointer to the ringbuffer object.
 */
void ringbufferFreeBuffer(ringbuffer *buf){
  free(buf->buffer);
}

// Change Functions
/* Those are not implemented yet, due to their questionable use and possible waste of ressources on q µC */

// Worker Functions
/** @brief This method will give you the count of bytes in the ringbuffer.
 *  @param *buf is the pointer to the ringbuffer object.
 *  @return count of bytes in buffer.
 */
uint32_t ringbufferGetCount(ringbuffer *buf){
  return buf->count;
}

/** @brief This method will put a byte into buffer.
 *  @param *buf is the pointer to the ringbuffer object.
 *  @param data is the byte to fit in.
 *  @return EOF if buffer is nullpointer.
 *  @return EOF if buffer full.
 *  @return 0 upon success.
 */
int ringbufferPutChar(ringbuffer *buf, uint8_t data){
  // Check if buffer valid
  if (buf->buffer == NULL){
    return EOF;
  }
  // Check for buffer full
  if (buf->count == buf->length){
    return EOF;
  }
  // Write through
  *(buf->writeptr) = data;
  // Move writeptr
  buf->writeptr += 1;
  if ( (buf->writeptr) >= ( buf->buffer + buf->length) ){
    buf->writeptr = buf->buffer;
  }
  // Set count
  buf->count += 1;
  return 0;
}

/** @brief This method get a number of bytes out of the ringbuffer.
 *  @param *buf is the pointer to the ringbuffer object.
 *  @param *buffer is the pointer to a field where to put the read bytes at.
 *  @param numberOfBytes is how many bytes you intend to read.
 *  @return EOF if buffer is nullpointer.
 *  @return EOF if buffer is less filled then you want to read.
 *  @return 0 upon success.
 */
int ringbufferGetString(ringbuffer *buf, uint8_t *buffer, uint32_t numberOfBytes){
  uint32_t i;
  // Check if buffer valid
  if (buf->buffer == NULL){
    return EOF;
  }
  // Check for buffer size validity
  if (buf->count < numberOfBytes){
    return EOF;
  }else{
    // Get values
    for (i=0; i < numberOfBytes; i++){
      buffer[i] = (int) *(buf->readptr);
      // Move pointer
      buf->readptr += 1;
      if ( (buf->readptr) >= ( (buf->buffer) + buf->length) ){
        buf->readptr = buf->buffer;
      }
      // Set count
      buf->count -= 1;
    }
  }
  return 0;
}

/** @brief This method get a byte out of the ringbuffer.
 *  @param *buf is the pointer to the ringbuffer object.
 *  @return EOF if buffer is nullpointer.
 *  @return EOF if buffer is less filled then you want to read.
 *  @return [0;255] upon success, which is the byte you wanted.
 */
int ringbufferGetChar(ringbuffer *buf){
  int retval;
  // Check if buffer valid
  if (buf->buffer == NULL){
    return EOF;
  }
  // Check for buffer empty
  if (buf->count == 0){
    return EOF;
  }else{
    // Get value
    retval = (int) *(buf->readptr);
    // Move pointer
    buf->readptr += 1;
    if ( (buf->readptr) >= ( (buf->buffer) + buf->length) ){
      buf->readptr = buf->buffer;
    }
    // Set count
    buf->count -= 1;
  }
  return retval;
}

/** @brief This method get a byte out of the ringbuffer.
 *  @param *buf is the pointer to the ringbuffer object.
 *  @param position is the offset regarding to the first byte you would've get
 *  if you called ringbufferGetChar(*buf).
 *  @return EOF if buffer is nullpointer.
 *  @return EOF if buffer count of bytes is smaller than your intended position.
 *  @return [0;255] upon success, which is the byte you wanted.
 */
int ringbufferPeekCharPosition(ringbuffer *buf, uint32_t position){
  // Check if buffer valid
  if (buf->buffer == NULL){
    return EOF;
  }
  // Check for validity
  if ((buf->count) < (position)){
    return EOF;
  }
  // Check for tournaround
  if ( (( ((uint32_t) (buf->readptr)) - ((uint32_t) (buf->buffer))) + position) >= (buf->length)){ // turnaround
    return (int) *((uint8_t*) ( buf->readptr + position - buf->length ));
  } else { // still in front of intended readptr
    return (int) *((uint8_t*) ( buf->readptr + position ));
  }
}

/** @brief This method will clear the buffer by setting the read and write pointer.
 *  It will NOT write 0x00 over every byte the buffer is made of.
 *  @param *buf is the pointer to the ringbuffer object.
 */
void ringbufferClear(ringbuffer *buf){
  buf->readptr = buf->buffer;
  buf->writeptr = buf->buffer;
  buf->count = 0;
}











/* Test area - include those, to make the machine test the validity of the above code */
#if TEST_RINGBUFFER >= 1

/** @brief This method is there to test the ringbuffer by using it with sample data
 *  and interaction.
 *  @return 0 upon success.
 *  @return -1 else.
 */
int ringbufferTestsuiteReturner(ringbuffer **rba, ringbuffer *rbb, int retVal){
  ringbufferFreeCompleteObject(*rba);
  ringbufferFreeBuffer(rbb);
  return retVal;
}

int ringbufferTestsuite(){
  ringbuffer *testobject; // test complete create function member
  ringbuffer testsubject; // test allocate function member
  
  // test constructors
  testobject = ringbufferCreateCompleteObject(8);
  if (testobject == NULL){
    return ringbufferTestsuiteReturner(&testobject, &testsubject, -1);
  }
  if (ringbufferCreateBuffer(&testsubject, 8) != 8){
    return ringbufferTestsuiteReturner(&testobject, &testsubject, -1);
  }else{
    testsubject.count = 0;
    testsubject.readptr = &testsubject.buffer[0];
    testsubject.writeptr = &testsubject.buffer[0];
    testsubject.length = 8;
  }
  
  // test validity of constructions and initial setters
  if (ringbufferGetChar(testobject) != EOF){
    return ringbufferTestsuiteReturner(&testobject, &testsubject, -1);
  }
  if (ringbufferGetChar(&testsubject) != EOF){
    return ringbufferTestsuiteReturner(&testobject, &testsubject, -1);
  }
  // THIS DOES WORK, SINCE IT REFEERS TO THE ACTUAL RPTR POSITION!
  // if (ringbufferPeekCharPosition(testobject, 0) != EOF){
  //   return ringbufferTestsuiteReturner(&testobject, &testsubject, -1);
  // }
  if (ringbufferPeekCharPosition(testobject, 1) != EOF){
    return ringbufferTestsuiteReturner(&testobject, &testsubject, -1);
  }
  if (ringbufferPeekCharPosition(testobject, 8) != EOF){
    return ringbufferTestsuiteReturner(&testobject, &testsubject, -1);
  }
  if (ringbufferPeekCharPosition(testobject, 9) != EOF){
    return ringbufferTestsuiteReturner(&testobject, &testsubject, -1);
  }
  // THIS DOES WORK, SINCE IT REFEERS TO THE ACTUAL RPTR POSITION!
  // if (ringbufferPeekCharPosition(&testsubject, 0) != EOF){
  //   return ringbufferTestsuiteReturner(&testobject, &testsubject, -1);
  // }
  if (ringbufferPeekCharPosition(&testsubject, 1) != EOF){
    return ringbufferTestsuiteReturner(&testobject, &testsubject, -1);
  }
  if (ringbufferPeekCharPosition(&testsubject, 8) != EOF){
    return ringbufferTestsuiteReturner(&testobject, &testsubject, -1);
  }
  if (ringbufferPeekCharPosition(&testsubject, 9) != EOF){
    return ringbufferTestsuiteReturner(&testobject, &testsubject, -1);
  }
  
  // test put in and get out x24 [tripple the max filling]
  for (int i=0;i<24;i++){
    if (ringbufferPutChar(testobject, i) == EOF){
      return ringbufferTestsuiteReturner(&testobject, &testsubject, -1);
    }
    if (ringbufferGetChar(testobject) != i){
      return ringbufferTestsuiteReturner(&testobject, &testsubject, -1);
    }
  }
  for (int i=0;i<24;i++){
    if (ringbufferPutChar(&testsubject, i) == EOF){
      return ringbufferTestsuiteReturner(&testobject, &testsubject, -1);
    }
    if (ringbufferGetChar(&testsubject) != i){
      return ringbufferTestsuiteReturner(&testobject, &testsubject, -1);
    }
  }
  
  // test overflow protection
  for (int i=0;i<=7;i++){
    if (ringbufferPutChar(testobject, i) == EOF){
      return ringbufferTestsuiteReturner(&testobject, &testsubject, -1);
    }
  }
  for (int i=0;i<=7;i++){
    if (ringbufferPutChar(&testsubject, i) == EOF){
      return ringbufferTestsuiteReturner(&testobject, &testsubject, -1);
    }
  }
  if (ringbufferPutChar(testobject, 8) != EOF){
    return ringbufferTestsuiteReturner(&testobject, &testsubject, -1);
  }
  if (ringbufferPutChar(&testsubject, 8) != EOF){
    return ringbufferTestsuiteReturner(&testobject, &testsubject, -1);
  }
  
  // buffer is now filled from 0 to 7, peektest without turnaround!
  for (int i=0;i<=7;i++){
    if (ringbufferPeekCharPosition(testobject, i) != i){
      return ringbufferTestsuiteReturner(&testobject, &testsubject, -1);
    }
  }
  for (int i=0;i<=7;i++){
    if (ringbufferPeekCharPosition(&testsubject, i) != i){
      return ringbufferTestsuiteReturner(&testobject, &testsubject, -1);
    }
  }
  
  // clear buffer
  ringbufferClear(testobject);
  ringbufferClear(&testsubject);
  if (testobject->count != 0){
    return ringbufferTestsuiteReturner(&testobject, &testsubject, -1);
  }
  if (testsubject.count != 0){
    return ringbufferTestsuiteReturner(&testobject, &testsubject, -1);
  }
  if (ringbufferGetChar(testobject) != EOF){
    return ringbufferTestsuiteReturner(&testobject, &testsubject, -1);
  }
  if (ringbufferGetChar(&testsubject) != EOF){
    return ringbufferTestsuiteReturner(&testobject, &testsubject, -1);
  }
  
  // Move pointers for 3 byte, then fill 0-7 and peek them -> peek with turnaround
  for (int i=0;i<3;i++){
    if (ringbufferPutChar(testobject, i) == EOF){
      return ringbufferTestsuiteReturner(&testobject, &testsubject, -1);
    }
    if (ringbufferGetChar(testobject) != i){
      return ringbufferTestsuiteReturner(&testobject, &testsubject, -1);
    }
  }
  for (int i=0;i<3;i++){
    if (ringbufferPutChar(&testsubject, i) == EOF){
      return ringbufferTestsuiteReturner(&testobject, &testsubject, -1);
    }
    if (ringbufferGetChar(&testsubject) != i){
      return ringbufferTestsuiteReturner(&testobject, &testsubject, -1);
    }
  }
  for (int i=0;i<=7;i++){
    if (ringbufferPutChar(testobject, i) == EOF){
      return ringbufferTestsuiteReturner(&testobject, &testsubject, -1);
    }
  }
  for (int i=0;i<=7;i++){
    if (ringbufferPutChar(&testsubject, i) == EOF){
      return ringbufferTestsuiteReturner(&testobject, &testsubject, -1);
    }
  }
  for (int i=0;i<=7;i++){
    if (ringbufferPeekCharPosition(testobject, i) != i){
      return ringbufferTestsuiteReturner(&testobject, &testsubject, -1);
    }
  }
  for (int i=0;i<=7;i++){
    if (ringbufferPeekCharPosition(&testsubject, i) != i){
      return ringbufferTestsuiteReturner(&testobject, &testsubject, -1);
    }
  }
  
  //Test StringCopy function
  uint8_t bTmp[8];
  if (ringbufferGetCount(testobject) != 8){
    return ringbufferTestsuiteReturner(&testobject, &testsubject, -1);
  }
  if (ringbufferGetCount(&testsubject) != 8){
    return ringbufferTestsuiteReturner(&testobject, &testsubject, -1);
  }
  ringbufferGetString(testobject, bTmp, 8);
  for (int i=0;i<8;i++){
    if (bTmp[i] != i){
      return ringbufferTestsuiteReturner(&testobject, &testsubject, -1);
    }
  }
  ringbufferGetString(&testsubject, bTmp, 8);
  for (int i=0;i<8;i++){
    if (bTmp[i] != i){
      return ringbufferTestsuiteReturner(&testobject, &testsubject, -1);
    }
  }
  
  return ringbufferTestsuiteReturner(&testobject, &testsubject, 0);
}

#endif
