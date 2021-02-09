/**
  ******************************************************************************
  * @file       APP_RSL.c
  * @author     Tim Steinberg
  * @date       08.11.2020
  * @brief      Handler for RSL10 communication stack.
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
  * | 001       | 08.11.2020    | Tim Steinberg         | Initial version / skeleton of file            |
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
#include "MasterDefine.h"

#include "EEPROM_Access.h"
#include "Watchdog.h"
#include "UART_RSL.h"
#include "Handler_Timer.h"
#include "LED.h"

#include "Logic.h"
#include "BehaviourController.h"
#include "UserMethods_Characteristics.h"
#include "BehaviourV115_BC.h"
#include "BehaviourV115_Battery.h"
#include "BehaviourV115_Pairing.h"
#include "BehaviourV115_Alert.h"
#include "BehaviourV115_AlertBase.h"
#include "BehaviourV115_SetAllCharacteristics.h"

#include "app_RSL.h"


#define LED_IN_PROGRESS_TIME_ON         500
#define LED_IN_PROGRESS_TIME_OFF        5000

/* Typedefinitions / Prototypes */

/* Variables */
TIMER_STRUCT_TYPEDEF app_rsl_timer_led_off;
TIMER_STRUCT_TYPEDEF app_rsl_timer_led_on;

/* Function definitions */
APP_RSL_INTERNAL_RETURN_VALUES_TYPEDEF app_rsl_handler_executeCommunication(uint32_t ledOnTime, uint32_t ledOffTime, void (*ledOnFunction)(), BEHAVIOUR_CONTROLLER_CALL_STRUCT_TYPEDEF* (*commfunction)()){
  APP_RSL_INTERNAL_RETURN_VALUES_TYPEDEF retVal;
  BEHAVIOUR_CONTROLLER_RETURN_VALUES_TYPEDEF returnValue;
  
  uart_rsl_init();
  logic_resetEverything();
  app_rsl_timer_led_off.time_waitTime = ledOffTime;
  app_rsl_timer_led_on.time_waitTime = ledOnTime;
  timerHandler_timerStop(&app_rsl_timer_led_on);
  timerHandler_timerStart(&app_rsl_timer_led_off, userMethods_characteristics_getTime(), ledOffTime);
  led_black();
  behaviourController_loadNewSequence(commfunction());
  
  do{
    // Do LED handling
    if (timerHandler_isTimerTimedOut(&app_rsl_timer_led_off, userMethods_characteristics_getTime()) == TRUE){
      timerHandler_timerRestart(&app_rsl_timer_led_on, userMethods_characteristics_getTime());
      timerHandler_timerStop(&app_rsl_timer_led_off);
      if (ledOnFunction != NULL){
        ledOnFunction();
      }
    }
    if (timerHandler_isTimerTimedOut(&app_rsl_timer_led_on, userMethods_characteristics_getTime()) == TRUE){
      timerHandler_timerRestart(&app_rsl_timer_led_off, userMethods_characteristics_getTime());
      timerHandler_timerStop(&app_rsl_timer_led_on);
      led_black();
    }
    // Feed watchdog to prevent restart
    watchdog_feed();
    // Check for communication
    logic_parseNachricht(HAL_GetTick());
    // Execute step
    returnValue = behaviourController_main();
    // Check the return value of the execute step
    if ((returnValue == BEHAVIOUR_CONTROLLER_RETURN_ONGOING) || (returnValue == BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT)){
      continue;
    }
    if (returnValue == BEHAVIOUR_CONTROLLER_RETURN_FINISHED){
      retVal = APP_RSL_INTERNAL_RETURN_VALUES_OK;
      
      do{
        watchdog_feed();
        if (timerHandler_isTimerTimedOut(&app_rsl_timer_led_off, userMethods_characteristics_getTime()) == TRUE){
          timerHandler_timerRestart(&app_rsl_timer_led_on, userMethods_characteristics_getTime());
          timerHandler_timerStop(&app_rsl_timer_led_off);
          if (ledOnFunction != NULL){
            ledOnFunction();
          }
        }
        if (timerHandler_isTimerTimedOut(&app_rsl_timer_led_on, userMethods_characteristics_getTime()) == TRUE){
          timerHandler_timerRestart(&app_rsl_timer_led_off, userMethods_characteristics_getTime());
          timerHandler_timerStop(&app_rsl_timer_led_on);
          break;
        }
      }while(1);
      
      break;
    }
    if (returnValue == BEHAVIOUR_CONTROLLER_RETURN_CRITICAL_ERROR){
      retVal = APP_RSL_INTERNAL_RETURN_VALUES_REPEAT;
      
      break;
    }
  }while(1);
  uart_rsl_deInit();
  return retVal;
}

bool app_rsl_updateAllCharacteristics(void (*ledOnFunction)()){
  APP_RSL_INTERNAL_RETURN_VALUES_TYPEDEF retVal;
  
  retVal = app_rsl_handler_executeCommunication(LED_IN_PROGRESS_TIME_ON, LED_IN_PROGRESS_TIME_OFF, ledOnFunction, behaviourV115_setAllCharacteristics_prepare_and_get_struct);
  
  switch(retVal){
    
    case APP_RSL_INTERNAL_RETURN_VALUES_OK:
      return TRUE;
      break;
      
    case APP_RSL_INTERNAL_RETURN_VALUES_REPEAT:
      return FALSE;
      break;
      
    default:
      return FALSE;
      break;
  }
}

bool app_rsl_broadcast(void (*ledOnFunction)()){
  APP_RSL_INTERNAL_RETURN_VALUES_TYPEDEF retVal;
  
  retVal = app_rsl_handler_executeCommunication(LED_IN_PROGRESS_TIME_ON, LED_IN_PROGRESS_TIME_OFF, ledOnFunction, behaviourV115_battery_prepare_and_get_struct);
  
  switch(retVal){
    
    case APP_RSL_INTERNAL_RETURN_VALUES_OK:
      return TRUE;
      break;
      
    case APP_RSL_INTERNAL_RETURN_VALUES_REPEAT:
      return FALSE;
      break;
      
    default:
      return FALSE;
      break;
  }
}

bool app_rsl_emergency(void (*ledOnFunction)()){
  APP_RSL_INTERNAL_RETURN_VALUES_TYPEDEF retVal;
  
  retVal = app_rsl_handler_executeCommunication(LED_IN_PROGRESS_TIME_ON, LED_IN_PROGRESS_TIME_OFF, ledOnFunction, behaviourV115_alertBase_prepare_and_get_struct);
  
  switch(retVal){
    
    case APP_RSL_INTERNAL_RETURN_VALUES_OK:
      return TRUE;
      break;
      
    case APP_RSL_INTERNAL_RETURN_VALUES_REPEAT:
      return FALSE;
      break;
      
    default:
      return FALSE;
      break;
  }
}

bool app_rsl_pairing(void (*ledOnFunction)()){
  APP_RSL_INTERNAL_RETURN_VALUES_TYPEDEF retVal;
  
  retVal = app_rsl_handler_executeCommunication(LED_IN_PROGRESS_TIME_ON, LED_IN_PROGRESS_TIME_OFF, ledOnFunction, behaviourV115_pairing_prepare_and_get_struct);
  
  switch(retVal){
    
    case APP_RSL_INTERNAL_RETURN_VALUES_OK:
      return TRUE;
      break;
      
    case APP_RSL_INTERNAL_RETURN_VALUES_REPEAT:
      return FALSE;
      break;
      
    default:
      return FALSE;
      break;
  }
}