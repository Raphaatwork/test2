/**
  ******************************************************************************
  * @file       App_Button_Barrelroll.c
  * @author     Tim Steinberg
  * @date       07.07.2020
  * @brief      Application to handle button press too long
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
#include "Led.h"
#include "UserMethods_UART.h"
#include "UserMethods_RSL10_Control.h"
#include "UART_RSL.h"
#include "EEPROM_ApplicationMapped.h"

/* Typedefinitions / Prototypes */

/* Variables */

/* Function definitions */
const uint8_t ack[4]    = {0x02, 0x50, 0x00, 0xF8};
const uint8_t DT[4]     = {0x02, 0x78, 0x00, 0xD0};

void app_button_barrelroll(void){
  led_turquoise();
  rsl10Control_setResetActive();
  HAL_Delay(1);
  rsl10Control_setResetInactive();
  HAL_Delay(2000);
  
  uart_rsl_init();
  
  userMethods_uartTransmit(4, (uint8_t*) ack);
  HAL_Delay(1000);
  userMethods_uartTransmit(4, (uint8_t*) DT);
  
  eeprom_setUnPaired();
  
  uart_rsl_deInit();
  led_black();
  return;
}
