/**
  ******************************************************************************
  * @file       App_868MHz_Sequencer.h
  * @author     Tim Steinberg
  * @date       10.12.2020
  * @brief      Sequencer for the S2LP module
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
  * | 001       | 2020-12-10    | Tim Steinberg         | Initial version / skeleton of file            |
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

#include "stm32l051xx.h"
#include "main.h"

#include "GPIO.h"
#include "Watchdog.h"
#include "App_868MHz_Sequencer.h"
#include "App_868MHz_MessageBuilder.h"

/* Typedefinitions / Prototypes */

/* Variables */
static uint32_t waitTimeCycles;
static uint8_t result;
static uint32_t i;

/* Function definitions */
void setOutputToHigh(void){
  ((GPIO_TypeDef*)GPO_2_S2LP_GPIO_Port)->BSRR = GPO_2_S2LP_Pin;
}

void setOutputToLow(void){
  ((GPIO_TypeDef*)GPO_2_S2LP_GPIO_Port)->BRR = GPO_2_S2LP_Pin;
}

static inline void setOutputOn(void){
  ((GPIO_TypeDef*)GPO_2_S2LP_GPIO_Port)->BSRR = GPO_2_S2LP_Pin;
}

static inline void setOutputOff(void){
  ((GPIO_TypeDef*)GPO_2_S2LP_GPIO_Port)->BRR = GPO_2_S2LP_Pin;
}

static inline void waitCycle(void){
  asm("NOP");
  asm("NOP");
  asm("NOP");
  asm("NOP");
  asm("NOP");
  asm("NOP");
  asm("NOP");
  asm("NOP");
  asm("NOP");
  asm("NOP");
  asm("NOP");
  asm("NOP");
  asm("NOP");
  asm("NOP");
  asm("NOP");
  asm("NOP");
  asm("NOP");
  asm("NOP");
  asm("NOP");
  asm("NOP");
}

inline void app_868MHz_sequencer_output(bitfield *bits){
  i = 0;
  //waitTimeCyclesNext = bits->bitsToSend[0].bitfieldTimes[0] - 1;
  //result = (bits->bitsToSend[0].bitfield >> 7) & 0x01;
  while(i < bits->countOfBitsUsed){
    
    waitTimeCycles = bits->bitsToSend[i >> 3].bitfieldTimes[7 - i & 0x00000007]; //waitTimeCyclesNext;
    //waitTimeCyclesNext = bits->bitsToSend[i >> 3].bitfieldTimes[i & 0x00000007];
    result = (bits->bitsToSend[i >> 3].bitfield >> (7 - i & 0x00000007)) & 0x01;
    if(result != 0)
    {
      // Set
      setOutputOn();
    }
    if(result == 0)
    {
      // Reset
      setOutputOff();
    }
    i++;
    while(waitTimeCycles > 0){
      waitCycle();
      waitTimeCycles -= 1;
    }
    
  }
}