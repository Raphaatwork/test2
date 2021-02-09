/**
  ******************************************************************************
  * @file       Userbutton.c
  * @author     Tim Steinberg
  * @date       06.07.2020
  * @brief      Code to get the user input regarding the button
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
#include "Debug.h"
#include "ADC.h"
#include "Watchdog.h"
#include "GPIO.h"
#include "Userbutton.h"
#include "Led.h"

/* Typedefinitions / Prototypes */

////////////////////OLD VERSION WITH SMALL VOLTAGE SPLITTER////////////////////
/** ADC Measurings from ADC connected to button for different VBats
  * Measurements done with count of prototypes  : 1
  * Measurements done per voltage & prototype   : 10
  * Batt-Volt refence ±2% or ±50mV exact
  * INTERESTING CATCH: THE LONGER YOU PRESS, THE MORE THE VOLTAGE RISES!
  *~~~
  * |-----------|-------------------------------|---------------------------------|
  * | Batt-Volt | ADC-reading pressed in digits | ADC-reading unpressed in digits |
  * |-----------|-------------------------------|---------------------------------|
  * | 1.6       | Voltage too low to start debuggin, case may never occur         |
  * |-----------|-------------------------------|---------------------------------|
  * | 1.75      | 2441 - 2441                   | 1266 - 1269                     |
  * |-----------|-------------------------------|---------------------------------|
  * | 1.8       | 2435 - 2441                   | 1266 - 1271                     |
  * |-----------|-------------------------------|---------------------------------|
  * | 2.1       | 2455 - 2455                   | 1275 - 1286                     |
  * |-----------|-------------------------------|---------------------------------|
  * | 2.4       | 2469 - 2469                   | 1286 - 1290                     |
  * |-----------|-------------------------------|---------------------------------|
  * | 2.7       | 2474 - 2479                   | 1294 - 1298                     |
  * |-----------|-------------------------------|---------------------------------|
  * | 3.0       | 2494 - 2494                   | 1310 - 1313                     |
  * |-----------|-------------------------------|---------------------------------|
  * | 3.3       | 2514 - 2515                   | 1321 - 1345                     |
  * |-----------|-------------------------------|---------------------------------|
  * | 3.48      | 2533 - 2533                   | 1323 - 1330                     |
  * |-----------|-------------------------------|---------------------------------|
  *~~~
  * 
  */

////////////////////NEW VERSION WITH HIGHER VOLTAGE SPLITTER////////////////////
/** ADC Measurings from ADC connected to button for different VBats
  * Measurements done with count of prototypes  : 1
  * Measurements done per voltage & prototype   : 10
  * Batt-Volt refence ±2% or ±50mV exact
  * INTERESTING CATCH: THE LONGER YOU PRESS, THE MORE THE VOLTAGE RISES!
  *~~~
  * |-----------|-------------------------------|---------------------------------|
  * | Batt-Volt | ADC-reading pressed in digits | ADC-reading unpressed in digits |
  * |-----------|-------------------------------|---------------------------------|
  * | 1.6       | Voltage too low to start debuggin, case may never occur         |
  * |-----------|-------------------------------|---------------------------------|
  * | 1.75      | XXXX - XXXX                   | XXXX - XXXX                     |
  * |-----------|-------------------------------|---------------------------------|
  * | 1.8       | 2372 - 2378                   | 1256 - 1256                     |
  * |-----------|-------------------------------|---------------------------------|
  * | 1.9       | 2372 - 2378                   | 1256 - 1256                     |
  * |-----------|-------------------------------|---------------------------------|
  * | 2.0       | 2381 - 2384                   | 1256 - 1256                     |
  * |-----------|-------------------------------|---------------------------------|
  * | 2.1       | 2381 - 2384                   | 1256 - 1256                     |
  * |-----------|-------------------------------|---------------------------------|
  * | 2.2       | 2381 - 2398                   | 1256 - 1256                     |
  * |-----------|-------------------------------|---------------------------------|
  * | 2.3       | 2394 - 2398                   | 1256 - 1256                     |
  * |-----------|-------------------------------|---------------------------------|
  * | 2.4       | 2394 - 2398                   | 1256 - 1256                     |
  * |-----------|-------------------------------|---------------------------------|
  * | 2.5       | 2398 - 2401                   | 1256 - 1256                     |
  * |-----------|-------------------------------|---------------------------------|
  * | 2.6       | 2398 - 2401                   | 1256 - 1256                     |
  * |-----------|-------------------------------|---------------------------------|
  * | 2.7       | 2398 - 2401                   | 1256 - 1256                     |
  * |-----------|-------------------------------|---------------------------------|
  * | 2.8       | 2401 - 2407                   | 1256 - 1256                     |
  * |-----------|-------------------------------|---------------------------------|
  * | 2.9       | 2401 - 2407                   | 1256 - 1256                     |
  * |-----------|-------------------------------|---------------------------------|
  * | 3.0       | 2401 - 2401                   | 1256 - 1256                     |
  * |-----------|-------------------------------|---------------------------------|
  * | 3.1       | 2406 - 2406                   | 1256 - 1256                     |
  * |-----------|-------------------------------|---------------------------------|
  * | 3.2       | 2406 - 2406                   | 1256 - 1256                     |
  * |-----------|-------------------------------|---------------------------------|
  * | 3.3       | 2410 - 2410                   | 1256 - 1256                     |
  * |-----------|-------------------------------|---------------------------------|
  * | 3.48      | XXXX - XXXX                   | XXXX - XXXX                     |
  * |-----------|-------------------------------|---------------------------------|
  *~~~
  * 
  */
#define LED_PRESS_TIME_MAX_EMERGENCY            5000
#define LED_PRESS_TIME_MAX_PAIRING              10000

/* Variables */

/* Function definitions */
uint16_t UserButtonGetLevel(uint32_t battery, uint32_t multiplier, uint32_t shifter){
  uint32_t mult = battery * multiplier;
  return (uint16_t) (mult >> shifter);
}


SYSTEMBOOT_SOURCE_BUTTON_TYPEDEF UserButtonMeasurePresstime(void){
  uint32_t startTick = HAL_GetTick();
  SYSTEMBOOT_SOURCE_BUTTON_TYPEDEF buttonAction = SYSTEMBOOT_SOURCE_BUTTON_NOTHING;
  uint16_t adcMeasurement;
  uint32_t adcBattery;
  uint16_t adcBatteryPressed;
  uint16_t adcBatteryReleased;
  watchdog_feed();
  
  // Move button into free mode to allow for a slow rise of voltage
  gpio_userButtonMeasureMode();
  HAL_Delay(5);
  adcBattery            = adcBlocking_getBatteryVoltage_fourOutOfSixAveragedLinearizedOffsetted();
  adcBatteryPressed     = UserButtonGetLevel(adcBattery, 1331, 11);//(uint16_t) ((adcBattery * 1331) >> 11); // Have less than 64,99% of the battery voltage on the ADC pin to register pressed
  adcBatteryReleased    = UserButtonGetLevel(adcBattery, 1741, 11);//(uint16_t) ((adcBattery * 1741) >> 11); // Have more than 85,00% of the battery voltage on the ADC pin to register released
  
  // Do measurement to check the press state again
  adcMeasurement = adcBlocking_getButtonVoltage_fourOutOfSixAveraged();
  // If measurement indicating NO press, then this was a glitch
  if (adcMeasurement < adcBatteryPressed){
    do{
      // Watchdog feeding
      watchdog_feed();
      // Press time in emergency range?
      if (HAL_GetTick() - startTick < LED_PRESS_TIME_MAX_EMERGENCY){
        buttonAction = SYSTEMBOOT_SOURCE_BUTTON_EMERGENCY;
        led_red();
      }
      // Press time in pairing range?
      if ((HAL_GetTick() - startTick >= LED_PRESS_TIME_MAX_EMERGENCY) && (HAL_GetTick() - startTick < LED_PRESS_TIME_MAX_PAIRING)){
        buttonAction = SYSTEMBOOT_SOURCE_BUTTON_PAIRING;
        led_blue();
      }
      // Press time invalid?
      if (HAL_GetTick() - startTick >= LED_PRESS_TIME_MAX_PAIRING){
        buttonAction = SYSTEMBOOT_SOURCE_BUTTON_BARRELROLL;//SYSTEMBOOT_SOURCE_BUTTON_EMERGENCY;
        led_green();
      }
      adcMeasurement = adcBlocking_getButtonVoltage_fourOutOfSixAveraged();
    }while(adcMeasurement < adcBatteryReleased);
  }
  
  // Restrict button to prevent a new restart action finished
  gpio_userButtonUnarmedMode();
    
  // Turn of any kind of LED
  led_black();
    
  // Feed that hungry dog
  watchdog_feed();
  
  return buttonAction;
}