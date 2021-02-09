/**
  ******************************************************************************
  * @file       ADC.c
  * @author     Tim Steinberg
  * @date       06.07.2020
  * @brief      ADC control, init, deinit
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
  * | 001       | 2020-07-06    | Tim Steinberg         | Initial version / skeleton of file            |
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
#include "Debug.h"
#include "ErrorHandling.h"
#include "RTC.h"

/* Typedefinitions / Prototypes */
void adcBlocking_button_init(void);
void adcBlocking_button_deInit(void);
void adcBlocking_batterylevel_init(void);
void adcBlocking_batterylevel_deInit(void);
uint16_t adcBlocking_getSingleMeasurementRaw(void);
uint16_t adcBlocking_getFourOutOfSixAveragedRaw(void);
uint16_t adcBlocking_getFactoryCalibrationValue(void);

/* Variables */
extern ADC_HandleTypeDef hadc;
extern DMA_HandleTypeDef hdma_adc;
uint32_t adcFactoryCalibrationValue;

/* Function definitions */
void adc_doInitialCalibration(void){
  adcBlocking_batterylevel_init();
  HAL_Delay(10);
  HAL_ADCEx_Calibration_Start(&hadc, ADC_SINGLE_ENDED);
  uint32_t adc_cal = HAL_ADCEx_Calibration_GetValue(&hadc, ADC_SINGLE_ENDED);
  rtc_adcCalSet(adc_cal);
  adcBlocking_batterylevel_deInit();
}

uint16_t adcBlocking_getFactoryCalibrationValue(void){
  return *((uint16_t*) ((uint32_t) 0x1FF80078));
}

void adcBlocking_button_init(void){
  TRACE_PROCEDURE_CALLS(1, "adcBlocking_button_init(void)\r\n");
    
  __HAL_RCC_ADC1_CLK_ENABLE();
  
  ADC_ChannelConfTypeDef sConfig = {0};

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) */
  hadc.Instance = ADC1;
  hadc.Init.OversamplingMode = DISABLE;
  hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.SamplingTime = ADC_SAMPLETIME_160CYCLES_5; //ADC_SAMPLETIME_1CYCLE_5;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ContinuousConvMode = DISABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.DMAContinuousRequests = DISABLE;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerFrequencyMode = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    Error_Handler_TxV2(INIT_ADC_FAILED);
  }
  
  // Lösche alle Kanäle aus der Kanalkonfiguration
  hadc.Instance->CHSELR &= (~ADC_CHANNEL_MASK);
  
  /** Configure for the selected ADC regular channel to be converted. */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler_TxV2(INIT_ADC_CHANNEL_FAILED);
  }
  
  // Lese alte Kalibrationswerte aus und verwende nochmal
  uint32_t adc_cal = rtc_adcCalGet();
  HAL_ADCEx_Calibration_SetValue(&hadc, ADC_SINGLE_ENDED, adc_cal);
}

void adcBlocking_button_deInit(void){
  TRACE_PROCEDURE_CALLS(1, "adcBlocking_button_deInit(void)\r\n");
  
  HAL_ADC_DeInit(&hadc);
  __HAL_RCC_ADC1_CLK_DISABLE();
}

  //#define ADC_RANK_CHANNEL_NUMBER                 ((uint32_t)0x00001000U)  /*!< Enable the rank of the selected channels. Number of ranks in the sequence is defined by number of channels enabled, rank of each channel is defined by channel number (channel 0 fixed on rank 0, channel 1 fixed on rank1, ...) */

void adcBlocking_batterylevel_init(void){
  TRACE_PROCEDURE_CALLS(1, "adcBlocking_batterylevel_init(void)\r\n");

  __HAL_RCC_ADC1_CLK_ENABLE();
  
  ADC_ChannelConfTypeDef sConfig = {0};
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) */
  hadc.Instance = ADC1;
  hadc.Init.OversamplingMode = DISABLE;
  hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.SamplingTime = ADC_SAMPLETIME_160CYCLES_5; //ADC_SAMPLETIME_1CYCLE_5;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ContinuousConvMode = DISABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.DMAContinuousRequests = DISABLE;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerFrequencyMode = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    Error_Handler_TxV2(INIT_ADC_FAILED);
  }
  
  // Lösche alle Kanäle aus der Kanalkonfiguration
  hadc.Instance->CHSELR &= (~ADC_CHANNEL_MASK);
  
  /** Configure for the selected ADC regular channel to be converted. */
  sConfig.Channel = ADC_CHANNEL_VREFINT;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler_TxV2(INIT_ADC_CHANNEL_FAILED);
  }
  
  // Lese alte Kalibrationswerte aus und verwende nochmal
  uint32_t adc_cal = rtc_adcCalGet();
  HAL_ADCEx_Calibration_SetValue(&hadc, ADC_SINGLE_ENDED, adc_cal);
}

void adcBlocking_batterylevel_deInit(void){
  TRACE_PROCEDURE_CALLS(1, "adcBlocking_batterylevel_deInit(void)\r\n");
  
  HAL_ADC_DeInit(&hadc);
  __HAL_RCC_ADC1_CLK_DISABLE();
}

uint16_t adcBlocking_getSingleMeasurementRaw(void){
  TRACE_PROCEDURE_CALLS(1, "adcBlocking_getMeasurement(void)\r\n");
  uint16_t val;
  HAL_ADC_Start(&hadc);
  //HAL_ADC_PollForConversion(&hadc, 10);
  //val = (uint16_t)HAL_ADC_GetValue(&hadc); // First "Dummy" measure! (ST problem ?...)
  HAL_ADC_PollForConversion(&hadc, 10);
  val = (uint16_t)HAL_ADC_GetValue(&hadc); 
  HAL_ADC_Stop(&hadc);
  return val;
}

uint16_t adcBlocking_getFourOutOfSixAveragedRaw(void){
  uint16_t min = 0xFFFF;
  uint16_t max = 0x0000;
  uint32_t sum = 0;
  uint16_t temp;
  
  for (int i = 0; i < 6; i++){
    temp = adcBlocking_getSingleMeasurementRaw(); 
    if (min > temp){
      min = temp;
    }
    if (max < temp){
      max = temp;
    }
    sum += temp;
  }
  
  sum -= min;
  sum -= max;
  // Shift right times 2 because it's a sum of 4 summands
  sum = sum >> 2;
  
  return (uint16_t)sum;
}

uint16_t adcBlocking_getBatteryVoltage_fourOutOfSixAveragedLinearizedOffsetted(void){
  uint32_t temp;
  
  adcBlocking_batterylevel_init();
  // Get average from 4 oo 6
  temp = adcBlocking_getFourOutOfSixAveragedRaw();
  
  adcBlocking_batterylevel_deInit();
  
  // Compensate difference from target voltage as TeleAlarm does
  // VREF = Value where the calibrated value was taken
  // gl_vRefintFactory = (*((uint16_t*) ((uint32_t) 0x1FF80078))) / 4; in TeleAlarm ressources
  // adcFactoryCalibrationValue = (*((uint16_t*) ((uint32_t) 0x1FF80078))); redefined, no shift since all values use 12 bit
  
  temp = (VREF * adcBlocking_getFactoryCalibrationValue()) / temp;
  
  // Compensate unlinearity as TeleAlarm does
  temp = (((temp * 523) >> 9)); // * 513 followed by RSH 9 equals a factor of 1.021484375
  // Compensate offset as TeleAlarm does
  if(temp > 2410)
  {
    temp = temp - 10;
  }
  // < 2.39V
  else if(temp < 2390)
  {
    temp = temp + 10;
  }
  
  return (uint16_t)temp;
}

uint16_t adcBlocking_getButtonVoltage_fourOutOfSixAveraged(void){
  uint16_t temp;
  uint16_t battVal = adcBlocking_getBatteryVoltage_fourOutOfSixAveragedLinearizedOffsetted();
  
  adcBlocking_button_init();
  
  temp = adcBlocking_getFourOutOfSixAveragedRaw();
  
  adcBlocking_button_deInit();
  
  temp = (battVal * temp) >> 12;
  
  return temp;
}