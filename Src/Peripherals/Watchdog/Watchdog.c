/**
  ******************************************************************************
  * @file       Watchdog.c
  * @author     Tim Steinberg
  * @date       06.07.2020
  * @brief      Control, Init, Deinit and Feed functions.
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

#include "Debug.h"
#include "ErrorHandling.h"
/* Typedefinitions / Prototypes */

/* Variables */
extern IWDG_HandleTypeDef hiwdg;

/* Function definitions */
void watchdog_init_highspeed(void){
  
#if DEBUG_PROCESSOR_HALTABLE > 0
  return;
#endif
  
  TRACE_PROCEDURE_CALLS(1, "watchdog_init_highspeed()\r\n");
  // Configure wakeup every 4,000 seconds.
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_64;
  hiwdg.Init.Window = 0x0909;
  hiwdg.Init.Reload = 0x0909;
  
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    Error_Handler_TxV2(INIT_WATCHDOG_FAILED);
  }
}

void watchdog_init_lowspeed(void){
  
#if DEBUG_PROCESSOR_HALTABLE > 0
  return;
#endif
  
  TRACE_PROCEDURE_CALLS(1, "watchdog_init_slowspeed()\r\n");
  // Configure wakeup every 28,33[297] seconds. More is NOT possible.
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_256;
  hiwdg.Init.Window = 0x0FFF;
  hiwdg.Init.Reload = 0x0FFF;
  
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    Error_Handler_TxV2(INIT_WATCHDOG_FAILED);
  }
}

void watchdog_feed(void)
{
  TRACE_PROCEDURE_CALLS(1, "watchdog_feed(void)\r\n");
  
  // Refresh the watchdog
  if(HAL_IWDG_Refresh(&hiwdg) != HAL_OK)
  {
    Error_Handler_TxV2(ACCESS_WATCHDOG_FEED_FAILED);
  }
}