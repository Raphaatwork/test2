/**
  ******************************************************************************
  * @file       App_Misc.c
  * @author     Tim Steinberg
  * @date       08.07.2020
  * @brief      A collection of several functions that are used in the main apps
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
  * | 001       | 2020-07-08    | Tim Steinberg         | Initial version / skeleton of file            |
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
#include "ErrorCodes.h"
#include "ErrorHandling.h"
#include "MasterDefine.h"

#include "ErrorHandling.h"
#include "EEPROM_ApplicationMapped.h"
#include "Watchdog.h"
#include "UART_RSL.h"
#include "Batterylevel.h"

#include "Handler_Timer.h"

#include "Led.h"
#include "Logic.h"
#include "BehaviourController.h"
#include "UserMethods_Characteristics.h"

#include "App_RSL.h"

/* Typedefinitions / Prototypes */
typedef enum APP_MISC_ALERT_LEVEL{
  APP_MISC_ALERT_LEVEL_NONE             = 0x00,
  APP_MISC_ALERT_LEVEL_NEW              = 0x01,
  APP_MISC_ALERT_LEVEL_OLD              = 0x10,
  APP_MISC_ALERT_LEVEL_NEW_AND_OLD      = 0x11,
} APP_MISC_ALERT_LEVEL_TYPEDEF; 

/* Variables */
TIMER_STRUCT_TYPEDEF timer_b_led_off = {
  .time_waitTime = 3000,
};
TIMER_STRUCT_TYPEDEF timer_b_led_on = {
  .time_waitTime = 200,
};

/* Function definitions */
void app_misc_alert_setNew(void){
  eeprom_setAlertValue(APP_MISC_ALERT_LEVEL_NEW);
}

void app_misc_alert_setNewToOld(void){
  eeprom_setAlertValue(APP_MISC_ALERT_LEVEL_NONE);
  Error_SetError_OldAlert();
}

void app_misc_alert_unset(void){
  eeprom_setAlertValue(APP_MISC_ALERT_LEVEL_NONE);
}

void app_misc_battery_measureNewBatteryValue(void){
  eeprom_setBatteryValue(batterylevel_getPercentage());
  if (eeprom_getBatteryValue() < eeprom_getBatteryLowThresholdValue()){
    eeprom_setBatteryLowCounter(eeprom_getBatteryLowCounter() + 1);
  }
}

void app_misc_battery_resetBatteryLowCounter(void){  
  // Load value, mask out lowest bits, return "cleaned" value
  eeprom_setBatteryLowCounter(eeprom_getBatteryLowCounter() & 0xFFFFFF00);
}

bool app_misc_battery_isBatteryNew(void){
  if (batterylevel_getPercentage() >= 100){
    return TRUE;
  }else{
    return FALSE;
  }
}

void app_misc_error_maskInNewErrorcode(ERRORCODES_TYPEDEF errorcode){
  switch(errorcode){    
    case ERRORCODES_FLASH_CORRUPT        :
      Error_SetError_FlashCorrupt();
      break;
      
    case ERRORCODES_NO_CONFIG_EEP_CORRUPT:
      Error_SetError_EEPCorrupt();
      break;
      
    case ERRORCODES_S2LP_ERROR           :
      Error_SetError_S2LP();
      break;
      
    case ERRORCODES_RSL_ERROR            :
      Error_SetError_RSL();
      break;
      
    case ERRORCODES_WD_TRIGGERED         :
      Error_SetError_WDTriggered();
      break;
      
    case ERRORCODES_UE_TRIGGERED         :
      Error_SetError_UETriggered();
      break;
      
    case ERRORCODES_OLDALERT             :
      Error_SetError_OldAlert();
      break;
      
    case ERRORCODES_FREE_B               :
      Error_SetError_FlashCorrupt();
      break;
  }
}

void app_misc_error_unset(void){
  Error_ResetErrorWithoutAlert();
}

void app_misc_errorAndOldAlert_unset(void){
  Error_ResetErrorWithAlert();
}

void app_misc_sync_eepromToCommstack(void){
  userMethods_characteristics_setAlert(eeprom_getAlertValue());
  userMethods_characteristics_setError(eeprom_getErrorValue());
  userMethods_characteristics_setBattery(batterylevel_getPercentage());
}

void app_misc_showErrorCritical(void){
  uint8_t repetitionCounter = 0;
  timer_b_led_off.time_waitTime = 50;
  timer_b_led_on.time_waitTime = 50;
  timerHandler_timerStop(&timer_b_led_on);
  timerHandler_timerStart(&timer_b_led_off, userMethods_characteristics_getTime(), 50);
  do{
    if (timerHandler_isTimerTimedOut(&timer_b_led_off, userMethods_characteristics_getTime()) == TRUE){
      timerHandler_timerRestart(&timer_b_led_on, userMethods_characteristics_getTime());
      timerHandler_timerStop(&timer_b_led_off);
      led_red();
    }
    if (timerHandler_isTimerTimedOut(&timer_b_led_on, userMethods_characteristics_getTime()) == TRUE){
      timerHandler_timerRestart(&timer_b_led_off, userMethods_characteristics_getTime());
      timerHandler_timerStop(&timer_b_led_on);
      repetitionCounter++;
      led_black();
    }
    watchdog_feed();
  }while(repetitionCounter < 40);
}

void app_misc_showError(void){
  uint8_t repetitionCounter = 0;
  timer_b_led_off.time_waitTime = 200;
  timer_b_led_on.time_waitTime = 200;
  timerHandler_timerStop(&timer_b_led_on);
  timerHandler_timerStart(&timer_b_led_off, userMethods_characteristics_getTime(), 200);
  do{
    if (timerHandler_isTimerTimedOut(&timer_b_led_off, userMethods_characteristics_getTime()) == TRUE){
      timerHandler_timerRestart(&timer_b_led_on, userMethods_characteristics_getTime());
      timerHandler_timerStop(&timer_b_led_off);
      led_red();
    }
    if (timerHandler_isTimerTimedOut(&timer_b_led_on, userMethods_characteristics_getTime()) == TRUE){
      timerHandler_timerRestart(&timer_b_led_off, userMethods_characteristics_getTime());
      timerHandler_timerStop(&timer_b_led_on);
      repetitionCounter++;
      led_black();
    }
    watchdog_feed();
  }while(repetitionCounter < 10);
}

void app_misc_showSuccess(void){
  uint8_t repetitionCounter = 0;
  timer_b_led_off.time_waitTime = 200;
  timer_b_led_on.time_waitTime = 200;
  timerHandler_timerStop(&timer_b_led_on);
  timerHandler_timerStart(&timer_b_led_off, userMethods_characteristics_getTime(), 200);
  do{
    if (timerHandler_isTimerTimedOut(&timer_b_led_off, userMethods_characteristics_getTime()) == TRUE){
      timerHandler_timerRestart(&timer_b_led_on, userMethods_characteristics_getTime());
      timerHandler_timerStop(&timer_b_led_off);
      led_green();
    }
    if (timerHandler_isTimerTimedOut(&timer_b_led_on, userMethods_characteristics_getTime()) == TRUE){
      timerHandler_timerRestart(&timer_b_led_off, userMethods_characteristics_getTime());
      timerHandler_timerStop(&timer_b_led_on);
      repetitionCounter++;
      led_black();
    }
    watchdog_feed();
  }while(repetitionCounter < 10);
}

void app_misc_showProblematicSuccess(void){
  uint8_t repetitionCounter = 0;
  timer_b_led_off.time_waitTime = 200;
  timer_b_led_on.time_waitTime = 200;
  timerHandler_timerStop(&timer_b_led_on);
  timerHandler_timerStart(&timer_b_led_off, userMethods_characteristics_getTime(), 200);
  do{
    if (timerHandler_isTimerTimedOut(&timer_b_led_off, userMethods_characteristics_getTime()) == TRUE){
      timerHandler_timerRestart(&timer_b_led_on, userMethods_characteristics_getTime());
      timerHandler_timerStop(&timer_b_led_off);
      led_green();
    }
    if (timerHandler_isTimerTimedOut(&timer_b_led_on, userMethods_characteristics_getTime()) == TRUE){
      timerHandler_timerRestart(&timer_b_led_off, userMethods_characteristics_getTime());
      timerHandler_timerStop(&timer_b_led_on);
      repetitionCounter++;
      led_red();
    }
    watchdog_feed();
  }while(repetitionCounter < 10);
  led_black();
}
