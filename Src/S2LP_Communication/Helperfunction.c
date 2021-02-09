/**
  ******************************************************************************
  * @file       Helperfunction.h
  * @author     Tim Steinberg
  * @date       09.12.2020
  * @brief      Functions that are being used in the 869MHz bitfield things
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
  * | 001       | 2020-12-09    | Tim Steinberg         | Initial version / skeleton of file            |
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

#include "ErrorCodes.h"
#include "MasterDefine.h"
#include "App_Misc.h"

////////////////////////////////////////////////
// FUNCTIONS FOR UINT8_T-ARRAYS
////////////////////////////////////////////////

#define GET_FULL_BYTES (position >> 3)
#define GET_POSITION_IN_BYTE (7 - (position & 0x00000007))

void setBitInPosition(uint8_t *array, uint32_t position){
  array[GET_FULL_BYTES] |= (1 << GET_POSITION_IN_BYTE);
}
                           
void unsetBitInPosition(uint8_t *array, uint32_t position){
  array[GET_FULL_BYTES] &= (~(1 << GET_POSITION_IN_BYTE));
}

uint8_t checkBitInPosition(uint8_t *array, uint32_t position){
  if ((array[GET_FULL_BYTES] & (1 << GET_POSITION_IN_BYTE)) > 0){
    return 0xFF;
  }else{
    return 0x00;
  }
}

void flipBitInPosition(uint8_t *array, uint32_t position){
  if (checkBitInPosition(array, position) == 0x00){
    setBitInPosition(array, position);
  }else{
    unsetBitInPosition(array, position);
  }
}

////////////////////////////////////////////////
// FUNCTIONS FOR BITFIELDS WITH TIMINGS
////////////////////////////////////////////////

#include "App_868MHz_MessageBuilder.h"

void initializeBitfield(uint8_t *arrayWithBits, uint32_t bitCount, bitfield *bits, uint16_t timeZero, uint16_t timeOne){
  for (uint32_t i = 0; i < bitCount; i++){
    bits->bitsToSend[i].bitfield = arrayWithBits[i];
  }
  bits->countOfBitsUsed = bitCount;
  
  for (uint32_t i = 0; i < bits->countOfBitsUsed; i++){
    // Is the actual bit space a 0 or 1?
    if (bits->bitsToSend[i >> 3].bitfield >> (7 - i & 0x07) == 0){
      // It's 0
      bits->bitsToSend[i >> 3].bitfieldTimes[7 - i & 0x07] = timeZero;
    }else{
      // It's 1
      bits->bitsToSend[i >> 3].bitfieldTimes[7 - i & 0x07] = timeOne;
    }
  }
}