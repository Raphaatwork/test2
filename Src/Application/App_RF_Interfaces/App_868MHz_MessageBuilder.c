/**
  ******************************************************************************
  * @file       App_868MHz_MessageBuilder.c
  * @author     Tim Steinberg
  * @date       09.12.2020
  * @brief      Unit to build messages for the 869.25MHz protocol
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

/* Includes */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "CRC.h"
#include "Bitstuffing.h"
#include "Helperfunction.h"
#include "Headeradd.h"
#include "NRZ.h"

#include "App_868MHz_MessageBuilder.h"

/* Typedefinitions / Prototypes */

/* Variables */

/* Function definitions */
#define ARRAYLENGTH_RAW_INPUT                   9
#define ARRAYLENGTH_CODED_INPUT                 12
#define ARRAYLENGTH_CODED_CONCAT_INPUT          14
#define ARRAYLENGTH_HEADER                      2
#define HEADERSIZE_BITS                         12
#define BYTE_SIZE                               8
#define WAIT_CYCLES_FOR_400_MUS                 210

void app_868MHz_invertBitfield(bitfield *bits){  
  for (int i = 0; i < bits->countOfBitsUsed; i++){
    bits->bitsToSend[i].bitfield = ~bits->bitsToSend[i].bitfield;
  }
}

int app_868MHz_buildMessage_preamble(bitfield *bits){
  bits->bitsToSend[0].bitfield = 0x55;
  bits->bitsToSend[0].bitfieldTimes[0] = WAIT_CYCLES_FOR_400_MUS;
  bits->bitsToSend[0].bitfieldTimes[1] = WAIT_CYCLES_FOR_400_MUS;
  bits->bitsToSend[0].bitfieldTimes[2] = WAIT_CYCLES_FOR_400_MUS;
  bits->bitsToSend[0].bitfieldTimes[3] = WAIT_CYCLES_FOR_400_MUS;
  bits->bitsToSend[0].bitfieldTimes[4] = WAIT_CYCLES_FOR_400_MUS;
  bits->bitsToSend[0].bitfieldTimes[5] = WAIT_CYCLES_FOR_400_MUS;
  bits->bitsToSend[0].bitfieldTimes[6] = WAIT_CYCLES_FOR_400_MUS;
  bits->bitsToSend[0].bitfieldTimes[7] = WAIT_CYCLES_FOR_400_MUS;  
  bits->countOfBitsUsed = 8;
  return 0;
}

int app_868MHz_buildMessage_emergency(uint8_t *message, uint32_t messageLength, bitfield *bits){
  // The input we excpet as per TA Radio Protocol - Social Alarm from 19.12.2014 V1.6 is 7 Bytes
  uint8_t tempRaw[ARRAYLENGTH_RAW_INPUT];
  // We have - as per same document - expect 2 bytes CRC, so we need those as well as the changed through NRZ and HDLC
  uint8_t tempCoded[ARRAYLENGTH_CODED_INPUT];
  // The header - as per same document - is said to be 12 Bit long, which is 1.5 Bytes (or rounded 2)
  uint8_t tempHeader[ARRAYLENGTH_HEADER] = {0x80, 0x10};
  // The overall message will thus be at least 9 byte, but more like 10 or 11. 
  uint8_t tempCodedConcat[ARRAYLENGTH_CODED_CONCAT_INPUT];
  uint32_t lengthUsed = ARRAYLENGTH_CODED_INPUT * BYTE_SIZE;
  uint16_t crc;
  
  crc = crc_calcCrc16_868MHzProtocol_softwareCrc(message, messageLength);
  for (int i = 0; i < messageLength; i++){
    tempRaw[i] = message[i];
  }
  tempRaw[7] = (uint8_t) ((crc & 0xFF00) >> 8);
  tempRaw[8] = (uint8_t) ((crc & 0x00FF) >> 0);
  
  bitstuffing(tempRaw, 9 * BYTE_SIZE, tempCoded, &lengthUsed);
  
  nrz_i(tempCoded, lengthUsed, SIGN_ZERO);

  if (bits->countOfBits < concatBitfields(tempCodedConcat, ARRAYLENGTH_CODED_CONCAT_INPUT * BYTE_SIZE, tempHeader, HEADERSIZE_BITS, tempCoded, lengthUsed)){
    return -1;
  }
  
  initializeBitfield(tempCodedConcat, HEADERSIZE_BITS + lengthUsed, bits, WAIT_CYCLES_FOR_400_MUS, WAIT_CYCLES_FOR_400_MUS);
  
  return 0;
}

/* After the Bitstuffing the field must be :
                                      01111101 = 0x7D
                                      00010110 = 0x16
                                      01111101 = 0x7D
                                      11000100 = 0xC4
                                      01000000 = 0x40
                                      10001111 = 0x8F
                                      00010001 = 0x11
                                      01100010 = 0x62
                                      01010001 = 0x51
                                      11       = 0xC0
    The field length used must be 74 due to 2 bits being stuffed
  */
  /* After the NRZ-I the field must be :
                                      00000011 = 0x03
                                      01001110 = 0x4E
                                      11111100 = 0xFC
                                      00101101 = 0x2D
                                      00101010 = 0x2A
                                      01011111 = 0x5F
                                      01001011 = 0x4B
                                      00010110 = 0x16
                                      11001011 = 0xCB
                                      11       = 0xC0
    The field length used must still be 74
  */