/**
  ******************************************************************************
  * @file       Runmode_Sleep.c
  * @author     Tim Steinberg
  * @date       30.07.2020
  * @brief      Set up runmode for sleeping
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
  * | 001       | 2020-07-30    | Tim Steinberg         | Initial version / skeleton of file            |
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

#include "Debug.h"
#include "ErrorHandling.h"

#include "Led.h"
#include "GPIO.h"
#include "EEPROM_Access.h"
#include "RTC.h"
#include "Watchdog.h"

/* Typedefinitions / Prototypes */

/* Variables */

/* Function definitions */

void runmode_sleep_configClocktree(void){
  __HAL_RCC_HSI_DISABLE();
  
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.HSEState = RCC_HSE_OFF;
  RCC_OscInitStruct.MSIState = RCC_MSI_OFF;
  RCC_OscInitStruct.LSEState = RCC_LSE_OFF;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_OFF;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler_TxV2(UNKNOWN);
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = 0;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV512;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV16;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV16;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler_TxV2(UNKNOWN);
  }
  PeriphClkInit.PeriphClockSelection = 0;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler_TxV2(UNKNOWN);
  }
}

void runmode_sleep_configurePeripheralsClockGating(void){
  __HAL_RCC_GPIOA_CLK_SLEEP_DISABLE();
  __HAL_RCC_GPIOB_CLK_SLEEP_DISABLE();
  __HAL_RCC_GPIOC_CLK_SLEEP_DISABLE();
  __HAL_RCC_GPIOD_CLK_SLEEP_DISABLE();
  __HAL_RCC_GPIOH_CLK_SLEEP_DISABLE();
  
  __HAL_RCC_CRC_CLK_SLEEP_DISABLE();
  __HAL_RCC_MIF_CLK_SLEEP_DISABLE();
  __HAL_RCC_SRAM_CLK_SLEEP_DISABLE();
  __HAL_RCC_DMA1_CLK_SLEEP_DISABLE();
    
  __HAL_RCC_SYSCFG_CLK_SLEEP_DISABLE();
  __HAL_RCC_DBGMCU_CLK_SLEEP_DISABLE();
  __HAL_RCC_WWDG_CLK_SLEEP_DISABLE();
  __HAL_RCC_PWR_CLK_SLEEP_DISABLE();
  
  __HAL_RCC_TIM2_CLK_DISABLE();   
  __HAL_RCC_TIM6_CLK_DISABLE();   
  __HAL_RCC_SPI2_CLK_DISABLE();   
  __HAL_RCC_USART2_CLK_DISABLE(); 
  __HAL_RCC_USART1_CLK_DISABLE();
  __HAL_RCC_LPUART1_CLK_DISABLE();
  __HAL_RCC_I2C1_CLK_DISABLE();   
  __HAL_RCC_I2C2_CLK_DISABLE();
  __HAL_RCC_LPTIM1_CLK_DISABLE();
  
  __HAL_RCC_DMA1_CLK_DISABLE();
  __HAL_RCC_MIF_CLK_DISABLE();
  __HAL_RCC_CRC_CLK_DISABLE();
  
  __HAL_RCC_FIREWALL_CLK_DISABLE();
  __HAL_RCC_WWDG_CLK_DISABLE();
  __HAL_RCC_DBGMCU_CLK_DISABLE();
  __HAL_RCC_SYSCFG_CLK_DISABLE();
  
  __HAL_RCC_GPIOA_CLK_DISABLE();
  __HAL_RCC_GPIOB_CLK_DISABLE();
  __HAL_RCC_GPIOC_CLK_DISABLE();
  __HAL_RCC_GPIOD_CLK_DISABLE();
  __HAL_RCC_GPIOH_CLK_DISABLE(); 
}

void runmode_sleep_prepare(void){
  rtc_init();
  rtc_lsi_calibration();
}

void runmode_sleep(uint32_t timeInSeconds){
  gpio_initSleepMode();
  
  runmode_sleep_configClocktree();
  
  runmode_sleep_configurePeripheralsClockGating();
  
  // Suspend Clocking
  HAL_SuspendTick();
  
  // Enable Ultra low power mode
  HAL_PWREx_EnableUltraLowPower();
  
  // Enable the fast wake up from Ultra low power mode
  HAL_PWREx_EnableFastWakeUp();
  
  // Select HSI as system clock source after Wake Up from Stop mode
  __HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_MSI);
  
  // Feed the barky
  watchdog_feed();
  
  // Change speed of watchdog to lowspeed
  watchdog_init_lowspeed();
  
  // Feed the barky
  watchdog_feed();

  
  // Go to sleep
  rtc_setWakeUpInSeconds(timeInSeconds);
}
