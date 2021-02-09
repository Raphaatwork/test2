/**
  ******************************************************************************
  * @file       App_PowerModeTest.c
  * @author     Tim Steinberg
  * @date       30.07.2020
  * @brief      Tests for sleepmode/wakemode and peripheral inits/deinits
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
    
#include "Debug.h"
#include "ADC.h"
#include "UART_RSL.h"
#include "Watchdog.h"
#include "GPIO.h"
#include "Userbutton.h"
#include "Led.h"
#include "Batterylevel.h"
    
/* Typedefinitions / Prototypes */

/* Variables */

/* Function definitions */
    
/**
================================================================================
|| Peripherals || Current in sleepmode                                        ||
================================================================================
|| None        || 1,4 µA                                                      ||
================================================================================
|| ADC-Button  || 1,4 µA                                                      ||
================================================================================
|| ADC-Battery || 1,4 µA                                                      ||
================================================================================
|| UART        || 1,4 µA                                                      ||
================================================================================
|| GPIO        || 1,4 µA                                                      ||
================================================================================
||             ||                                                             ||
================================================================================
||             ||                                                             ||
================================================================================
||             ||                                                             ||
================================================================================
||             ||                                                             ||
================================================================================
||             ||                                                             ||
================================================================================

*/
    
void app_powerModeTest(void){
  /// ADC ACTIVATION BUTTON
  //adcBlocking_button_init();
  //adcBlocking_getFourOutOfSixAveragedLinearizedOffsetted();
  //adcBlocking_button_deInit();
  
  /// ADC ACTIVATION BATTERY
  //batterylevel_getPercentage();
  
  /// UART ACTIVATION
  //uint8_t test[4] = "TEST";
  //uart_rsl_init();
  //uart_rsl_startReception();
  //uart_rsl_transmit(4, test);
  //uart_rsl_deInit();
  
  /// GPIO settings
  //gpio_userButtonMeasureMode();
  //gpio_userButtonUnarmedMode();
  //gpio_rsl10WakeupActive();
  //gpio_rsl10WakeupInactive();
  //gpio_rsl10ResetActive();
  //gpio_rsl10ResetInactive();
  //gpio_ledOutputMode();
  //gpio_ledAnalogMode();
}