/**
  ******************************************************************************
  * @file       RTC.c
  * @author     Tim Steinberg
  * @date       07.07.2020
  * @brief      Control for RTC init and wakeup
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
#include "ErrorHandling.h"
#include "Debug.h"
#include "EEPROM_ApplicationMapped.h"

/* Typedefinitions / Prototypes */

/* Variables */
extern RTC_HandleTypeDef hrtc;
volatile uint8_t rtcWakeupIntFired;

/* Function definitions */
uint32_t rtc_adcCalGet(void){
  return HAL_RTCEx_BKUPRead(&hrtc, 0);
}

void rtc_adcCalSet(uint32_t adc_calVal){
  HAL_RTCEx_BKUPWrite(&hrtc, 0, adc_calVal);
}

void rtc_init(void){
  /** Initialize RTC Only 
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler_TxV2(INIT_RTC_FAILED);
  }
}

void rtc_setWakeUpInSeconds(uint32_t seconds){
  // Set the wakeup timer -> WARNING THE MAXIMUM TIME FOR THE WATCHDOG TO RUN OUT IS 28,3 SECONDS!
  HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 2312 * seconds, RTC_WAKEUPCLOCK_RTCCLK_DIV16); 

  // Clear the wakeup flag
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU); 
  
  // Enable the Power Down in Run mode
  //__HAL_FLASH_POWER_DOWN_ENABLE(); //DO NOT USE THIS LINE AS LONG AS YOU DO NOT RUN FROM RAM!
  
  // Enter stopmode Nnow
  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);  
  
  ////////////////
  // STOP MODE
  ////////////////
  
  // Disable the Power Down in Run mode
  //__HAL_FLASH_POWER_DOWN_DISABLE(); //DO NOT USE THIS LINE AS LONG AS YOU DO NOT RUN FROM RAM!
    
  // Disable RTC Wakeup
  HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
}

void rtc_wakeUpIntFire(void){
  rtcWakeupIntFired = 0x01;
}

void rtc_lsi_calibration(void){
  TRACE_PROCEDURE_CALLS(1, "rtc_lsi_calibration(void)\r\n");
  uint32_t timeStamp;

  // Load last calibrated value
  uint32_t calibrationValue = eeprom_getLsiCalibration();
  // Is value realistic ? take it : load default
  if ( (calibrationValue < 50) || (calibrationValue > 400) ){
    calibrationValue = 231;
  }
  uint32_t wakeupTime = calibrationValue;
  // Disable Wakeup Counter
  HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
  // Set the flag to 0
  rtcWakeupIntFired = 0x00;
  // Plan wake-up in 0.1s from now
  HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, wakeupTime, RTC_WAKEUPCLOCK_RTCCLK_DIV16);
  // Get "now"
  timeStamp = HAL_GetTick(); 
  do
  {   
    // Wait until wake-up
  }
  while(rtcWakeupIntFired == 0x00);
  if (timeStamp == 0){
    Error_Handler_TxV2(ACCESS_RTC_CALIBRATION_FAILED);
  }
  // Real time of theoretical 0.1s
  timeStamp = HAL_GetTick() - timeStamp; 
  // Calculate corrected wake-up timer value for 0.1s
  wakeupTime = (calibrationValue * 100) / timeStamp; 
  // Safe value back to EEPROM
  eeprom_setLsiCalibration(wakeupTime);
}