/**
  ******************************************************************************
  * @file       Batterylevel.c
  * @author     Tim Steinberg
  * @date       07.07.2020
  * @brief      Code to read out the actual battery level
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
  * | 001       | 2020-07-07    | Tim Steinberg         | Initial version / skeleton of file            |
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
#include "stm32l0xx_hal.h"
#include "MasterDefine.h"
#include "ADC.h"

/* Typedefinitions / Prototypes */
/** ADC Measurings for battery for different Vbat
  * Measurements done with count of prototypes  : 1
  * Measurements done per voltage & prototype   : 10
  * Batt-Volt refence ±2% or ±50mV exact
  *~~~
  * |-----------|-------------------------------|
  * | Batt-Volt | Battery reading in percent    |
  * |-----------|-------------------------------|
  * | < 1790 mV |   0                           |
  * |-----------|-------------------------------|
  * | 1.7       |   0                           |
  * |-----------|-------------------------------|
  * | 1.8       |   0                           |
  * |-----------|-------------------------------|
  * | 1.9       |   7                           |
  * |-----------|-------------------------------|
  * | 2.0       |  17                           |
  * |-----------|-------------------------------|
  * | 2.1       |  27                           |
  * |-----------|-------------------------------|
  * | 2.2       |  36                           |
  * |-----------|-------------------------------|
  * | 2.3       |  47                           |
  * |-----------|-------------------------------|
  * | 2.4       |  56                           |
  * |-----------|-------------------------------|
  * | 2.5       |  64                           |
  * |-----------|-------------------------------|
  * | 2.6       |  74                           |
  * |-----------|-------------------------------|
  * | 2.7       |  84                           |
  * |-----------|-------------------------------|
  * | 2.8       |  94                           |
  * |-----------|-------------------------------|
  * | 2.9       | 100                           |
  * |-----------|-------------------------------|
  * | 3.0       | 100                           |
  * |-----------|-------------------------------|
  * | 3.1       | 100                           |
  * |-----------|-------------------------------|
  * | 3.2       | 100                           |
  * |-----------|-------------------------------|
  * | 3.3       | 100                           |
  * |-----------|-------------------------------|
  * | 3.48      | 100                           |
  * |-----------|-------------------------------|
  *~~~
  * 
  */
    
#define BATTERY_INTERVAL_WIDTH (CR2430_100PERCENT_V - CR2430_0PERCENT_V)
/* Variables */

/* Function definitions */
uint16_t batterylevel_mV(void){
  uint16_t calValue;
  
  calValue = adcBlocking_getBatteryVoltage_fourOutOfSixAveragedLinearizedOffsetted();
  
  return calValue;
}

uint16_t batterylevel_getPercentage(void){
  uint16_t calValue;
  
  calValue = batterylevel_mV();
  
  if (calValue > CR2430_100PERCENT_V){
    calValue = CR2430_100PERCENT_V;
  }
  
  if (calValue < CR2430_0PERCENT_V){
    calValue = CR2430_0PERCENT_V;
  }

  calValue = ((calValue - CR2430_0PERCENT_V) * 100) / BATTERY_INTERVAL_WIDTH;
  
  return calValue;
}

#include "S2LP.h"
#include "App_868MHz_Sequencer.h"

/** ADC Measurings for battery for different Vbat
  * Measurements done with count of prototypes  : 1
  * Measurements done per voltage & prototype   : 10
  * Batt-Volt refence ±2% or ±50mV exact
  *~~~
  * |-----------|-------------------------------|
  * | Batt-Volt | Battery reading in percent    |
  * |-----------|-------------------------------|
  * | < 1790 mV |   0                           |
  * |-----------|-------------------------------|
  * | 1.7       |   0                           |
  * |-----------|-------------------------------|
  * | 1.8       |   0                           |
  * |-----------|-------------------------------|
  * | 1.9       |   7                           |
  * |-----------|-------------------------------|
  * | 2.0       |  17                           |
  * |-----------|-------------------------------|
  * | 2.1       |  27                           |
  * |-----------|-------------------------------|
  * | 2.2       |  36                           |
  * |-----------|-------------------------------|
  * | 2.3       |  47                           |
  * |-----------|-------------------------------|
  * | 2.4       |  56                           |
  * |-----------|-------------------------------|
  * | 2.5       |  64                           |
  * |-----------|-------------------------------|
  * | 2.6       |  74                           |
  * |-----------|-------------------------------|
  * | 2.7       |  82                           |
  * |-----------|-------------------------------|
  * | 2.8       |  93                           |
  * |-----------|-------------------------------|
  * | 2.9       | 100                           |
  * |-----------|-------------------------------|
  * | 3.0       | 100                           |
  * |-----------|-------------------------------|
  * | 3.1       | 100                           |
  * |-----------|-------------------------------|
  * | 3.2       | 100                           |
  * |-----------|-------------------------------|
  * | 3.3       | 100                           |
  * |-----------|-------------------------------|
  * | 3.48      | 100                           |
  * |-----------|-------------------------------|
  *~~~
  * 
  */

uint16_t batterylevel_measureWithLoad(void){
  uint16_t calValue;
  uint64_t sum = 0;
  
  S2LP_SetConfig_WorkingMode();
  for (int i = 0; i < 750; i++){
    setOutputToHigh();
    for (int j = 0; j < 4; j++){
      sum += batterylevel_getPercentage();
    }
    setOutputToLow();
    for (int j = 0; j < 4; j++){
      sum += batterylevel_getPercentage();
    }
  }
  
  sum /= (750*8);
  
  calValue = sum;
  
  S2LP_SetConfig_SleepMode();
  
  return calValue;
}
