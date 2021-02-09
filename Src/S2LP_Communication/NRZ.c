/**
  ******************************************************************************
  * @file       NRZ.h
  * @author     Tim Steinberg
  * @date       09.12.2020
  * @brief      Function for NRZI transformation of 869MHz
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
#include "Helperfunction.h"
#include "NRZ.h"

void nrz_i(uint8_t *array, uint32_t arrayBitCount, NRZ_INITIAL_SIGN_TYPEDEF preliminarySign){
  uint8_t signTurn = preliminarySign;
  switch(preliminarySign){
    case SIGN_ZERO:
      signTurn = 0;
      break;
      
    case SIGN_ONE:
      signTurn = 1;
      break;
      
    case SIGN_AUTO_INVERSE:
      if (checkBitInPosition(array, 0) == 0x00){
        signTurn = 1;
      }else{
        signTurn = 0;
      }
      break;
      
    case SIGN_AUTO:
      if (checkBitInPosition(array, 0) == 0x00){
        signTurn = 0;
      }else{
        signTurn = 1;
      }
      break;
  }
  for (uint32_t i = 0; i < arrayBitCount; i++){
    if (checkBitInPosition(array, i) == 0x00){
      if (signTurn == 0x00){
        setBitInPosition(array, i);
        signTurn = 0x01;
      }else{
        unsetBitInPosition(array, i);
        signTurn = 0x00;
      }
    }else{
      if (signTurn == 0x00){
        unsetBitInPosition(array, i);
        signTurn = 0x00;
      }else{
        setBitInPosition(array, i);
        signTurn = 0x01;
      }
    }
    flipBitInPosition(array, i);
  }
}

void nrz_i_standard(uint8_t *array, uint32_t arrayBitCount, NRZ_INITIAL_SIGN_TYPEDEF preliminarySign){
  uint8_t signTurn = preliminarySign;
  uint8_t signTurnMem = 0x01;
  if (preliminarySign == SIGN_AUTO){
    // No auto selection!
    preliminarySign = SIGN_ONE;
  }
  for (uint32_t i = 0; i < arrayBitCount; i++){
    if (checkBitInPosition(array, i) == 0x00){
      signTurnMem ^= 0x01;
    }
    if (signTurn == 0x00){
      flipBitInPosition(array, i);
    }
    flipBitInPosition(array, i);
    signTurn = signTurnMem;
  }
}