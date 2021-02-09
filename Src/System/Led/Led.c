/**
  ******************************************************************************
  * @file       LED.c
  * @author     Tim Steinberg
  * @date       06.07.2020
  * @brief      Control for the LED
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
#include "main.h"
#include "Debug.h"

/* Typedefinitions / Prototypes */

/* Variables */

/* Function definitions */
void Led_RGB_OFF(void)
{
  TRACE_PROCEDURE_CALLS(1, "Led_RGB_OFF(void)\r\n");
  // Clear each color
  HAL_GPIO_WritePin(GPO_LED_RED_GPIO_Port, GPO_LED_RED_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPO_LED_BLUE_GPIO_Port, GPO_LED_BLUE_Pin, GPIO_PIN_SET);  
  HAL_GPIO_WritePin(GPO_LED_GREEN_GPIO_Port, GPO_LED_GREEN_Pin, GPIO_PIN_SET);
}

void Led_RGB_ON(GPIO_PinState red, GPIO_PinState green, GPIO_PinState blue)
{
  TRACE_PROCEDURE_CALLS(1, "Led_RGB_OFF(void)\r\n");
  // Clear each color
  HAL_GPIO_WritePin(GPO_LED_RED_GPIO_Port, GPO_LED_RED_Pin, red);
  HAL_GPIO_WritePin(GPO_LED_BLUE_GPIO_Port, GPO_LED_BLUE_Pin, blue);  
  HAL_GPIO_WritePin(GPO_LED_GREEN_GPIO_Port, GPO_LED_GREEN_Pin, green);
}

/** @brief This is a stack call to set the LEDs regarding some states.
 *         While it looks this way, those functions will NOT be used in final
 *         versions and stay only for TESTING AND DEVELOPMENT PURPOSE!
 *  @return Nothing.
 */
void led_red(){
  Led_RGB_ON(GPIO_PIN_RESET, GPIO_PIN_SET, GPIO_PIN_SET);
}

/** @brief This is a stack call to set the LEDs regarding some states.
 *         While it looks this way, those functions will NOT be used in final
 *         versions and stay only for TESTING AND DEVELOPMENT PURPOSE!
 *  @return Nothing.
 */
void led_green(){
  Led_RGB_ON(GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_SET);
}

/** @brief This is a stack call to set the LEDs regarding some states.
 *         While it looks this way, those functions will NOT be used in final
 *         versions and stay only for TESTING AND DEVELOPMENT PURPOSE!
 *  @return Nothing.
 */
void led_blue(){
  Led_RGB_ON(GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_RESET);
}

/** @brief This is a stack call to set the LEDs regarding some states.
 *         While it looks this way, those functions will NOT be used in final
 *         versions and stay only for TESTING AND DEVELOPMENT PURPOSE!
 *  @return Nothing.
 */
void led_magenta(){
  Led_RGB_ON(GPIO_PIN_RESET, GPIO_PIN_SET, GPIO_PIN_RESET);
}

/** @brief This is a stack call to set the LEDs regarding some states.
 *         While it looks this way, those functions will NOT be used in final
 *         versions and stay only for TESTING AND DEVELOPMENT PURPOSE!
 *  @return Nothing.
 */
void led_turquoise(){
  Led_RGB_ON(GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_RESET);
}

/** @brief This is a stack call to set the LEDs regarding some states.
 *         While it looks this way, those functions will NOT be used in final
 *         versions and stay only for TESTING AND DEVELOPMENT PURPOSE!
 *  @return Nothing.
 */
void led_yellow(){
  Led_RGB_ON(GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_SET);
}

/** @brief This is a stack call to set the LEDs regarding some states.
 *         While it looks this way, those functions will NOT be used in final
 *         versions and stay only for TESTING AND DEVELOPMENT PURPOSE!
 *  @return Nothing.
 */
void led_white(){
  Led_RGB_ON(GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET);
}

/** @brief This is a stack call to set the LEDs regarding some states.
 *         While it looks this way, those functions will NOT be used in final
 *         versions and stay only for TESTING AND DEVELOPMENT PURPOSE!
 *  @return Nothing.
 */
void led_black(){
  Led_RGB_ON(GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET);
}