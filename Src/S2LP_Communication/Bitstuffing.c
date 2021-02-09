/**
  ******************************************************************************
  * @file       Bitstuffing.c
  * @author     Tim Steinberg
  * @date       09.12.2020
  * @brief      Function for HDLC transformation of 869MHz
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


uint8_t bitstuffing(uint8_t *array, uint32_t lengthBitsUsed, uint8_t *targetArray, uint32_t *lengthTargetBits){
  uint8_t countOfConsequtiveOnes = 0;
  uint32_t targetPosition = 0;
  // For every bit do
  for (uint32_t i = 0; i < lengthBitsUsed; i++){
    // Abfang von Überlauf
    if (targetPosition == *lengthTargetBits){
      return 0x01;
    }
    // Is 0 or 1?
    if (checkBitInPosition(array, i) == 0xFF){
      // Is 1
      
      // Count up 1-count
      countOfConsequtiveOnes++;
      // Is 1-count 5?
      if (countOfConsequtiveOnes == 5){
        // Is 5
        
        // Set bit
        setBitInPosition(targetArray, targetPosition++);
        // Plug in the filling 0
        unsetBitInPosition(targetArray, targetPosition++);
        // Reset the 1-count
        countOfConsequtiveOnes = 0;
      }else{
        // Is smaller
        
        // Set bit and count up target position
        setBitInPosition(targetArray, targetPosition++);
      }
    }else{
      // Is 0
      
      // Unset that bit and count up target position
      unsetBitInPosition(targetArray, targetPosition++);
      // Reset the 1-count
      countOfConsequtiveOnes = 0;
    }
  }
  *lengthTargetBits = targetPosition;
  return 0x00;
}