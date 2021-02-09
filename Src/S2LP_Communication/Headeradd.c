/**
  ******************************************************************************
  * @file       Headeradd.c
  * @author     Tim Steinberg
  * @date       09.12.2020
  * @brief      Function to add header and footer bitfields
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

uint32_t concatBitfields(uint8_t *arrayTarget, uint32_t arrayTargetLengthBits, uint8_t *arrayFront, uint8_t arrayFrontLengthBits, uint8_t *arrayBack, uint8_t arrayBackLengthBits){
  if (arrayTargetLengthBits < arrayFrontLengthBits + arrayBackLengthBits){
    return 0;
  }  
  arrayTargetLengthBits = 0;
  for (uint32_t i = 0; i < arrayFrontLengthBits; i++){
    if (checkBitInPosition(arrayFront, i) > 0){
      setBitInPosition(arrayTarget, i);
    }else{
      unsetBitInPosition(arrayTarget, i);
    }
  }
  for (uint32_t i = 0; i < arrayBackLengthBits; i++){
    if (checkBitInPosition(arrayBack, i) > 0){
      setBitInPosition(arrayTarget, i + arrayFrontLengthBits);
    }else{
      unsetBitInPosition(arrayTarget, i + arrayFrontLengthBits);
    }
  }
  return arrayFrontLengthBits + arrayBackLengthBits;
}