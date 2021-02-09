/**
  ******************************************************************************
  * @file       App_RSL_Interaction_Emergency.c
  * @author     Tim Steinberg
  * @date       11.11.2020
  * @brief      Statemachine to handle an emergency from the application view
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
  * | 001       | 2020-11-11    | Tim Steinberg         | Initial version / skeleton of file            |
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
#include "EEPROM_ApplicationMapped.h"
#include "UserMethods_Characteristics.h"
#include "App_Misc.h"
#include "App_RSL.h"
#include "Led.h"

/* Typedefinitions / Prototypes */
typedef enum APP_RSL_INTERACTION_EMERGENCY_STATES{
  APP_RSL_INTERACTION_EMERGENCY_STATES_INIT_UPDATE        = 0x00,
  APP_RSL_INTERACTION_EMERGENCY_STATES_UPDATE_PRE         = 0x10,
  APP_RSL_INTERACTION_EMERGENCY_STATES_INIT_EMERGENCY     = 0x20,
  APP_RSL_INTERACTION_EMERGENCY_STATES_EMERGENCY          = 0x30,
  APP_RSL_INTERACTION_EMERGENCY_STATES_EVAL_EMERGENCY     = 0x40,
  APP_RSL_INTERACTION_EMERGENCY_STATES_UPDATE_POST        = 0x60,
  APP_RSL_INTERACTION_EMERGENCY_STATES_USERMESSAGE        = 0x70,
} APP_RSL_INTERACTION_EMERGENCY_STATES_TYPEDEF; 


/* Variables */

/* Function definitions */


APP_RSL_INTERACTION_EMERGENCY_STATES_TYPEDEF emergencyStatemachine;

void app_rsl_interaction_emergency_main(void){ 
  uint32_t stateRetryCounter;
  bool afterEmergencyUpdateState = TRUE;
  USERMETHODS_CHARACTERISTICS_TRANSMISSION_STATE_TYPEDEF retVal;
  emergencyStatemachine = APP_RSL_INTERACTION_EMERGENCY_STATES_INIT_UPDATE;
  
  do{
    switch(emergencyStatemachine){
      
      case APP_RSL_INTERACTION_EMERGENCY_STATES_INIT_UPDATE:
        // Set the new parameter
        app_misc_battery_measureNewBatteryValue();
        app_misc_alert_setNew();
        app_misc_sync_eepromToCommstack();
        
        // Transition to next state
        stateRetryCounter = 0;
        emergencyStatemachine = APP_RSL_INTERACTION_EMERGENCY_STATES_UPDATE_PRE;
        break;
        
      case APP_RSL_INTERACTION_EMERGENCY_STATES_UPDATE_PRE:
        // Try update all characteristics
        if (app_rsl_updateAllCharacteristics(led_red) == FALSE){
          // Failed
          
          // Do we still have tries?
          if (++stateRetryCounter >= eeprom_getRepetitionCountEmergency()){
            // No
            
            // Show critcal error and return
            app_misc_showErrorCritical();
            return;
          }
        }else{
          // Success 
          
          // Transition to next state
          stateRetryCounter = 0;
          emergencyStatemachine = APP_RSL_INTERACTION_EMERGENCY_STATES_INIT_EMERGENCY;
        }
        break;
        
      case APP_RSL_INTERACTION_EMERGENCY_STATES_INIT_EMERGENCY:
        // Set up the connection info
        userMethods_characteristics_setTransmissionState_None();
        
        // Transition to next state
        stateRetryCounter = 0;
        emergencyStatemachine = APP_RSL_INTERACTION_EMERGENCY_STATES_EMERGENCY;
        break;
        
      case APP_RSL_INTERACTION_EMERGENCY_STATES_EMERGENCY:
        // Try do pairing
        if (app_rsl_emergency(led_red) == FALSE){
          // Failed
          
          // Do we still have tries?
          if (++stateRetryCounter >= eeprom_getRepetitionCountEmergency()){
            // No
            
            // Show critcal error and return
            app_misc_showErrorCritical();
            return;
          }
        }else{
          // Success
          eeprom_incNumberOfTransmissionsBLE();
          
          // Was there a readout?
          if ((retVal = userMethods_characteristics_getTransmissionState()) == USERMETHODS_CHARACTERISTICS_TRANSMISSION_STATE_NONE){
            // No
            
            // Do we still have tries?
            if (++stateRetryCounter >= eeprom_getRepetitionCountEmergency()){
              // No
              
              // Go to next state
              stateRetryCounter = 0;
              emergencyStatemachine = APP_RSL_INTERACTION_EMERGENCY_STATES_EVAL_EMERGENCY;
            }else{
              // Yes
              
              // Stay in this state and try again
              emergencyStatemachine = APP_RSL_INTERACTION_EMERGENCY_STATES_EMERGENCY;
            }
          }else{
            // Yes there was a readout
            
            // Transition to next state
            stateRetryCounter = 0;
            emergencyStatemachine = APP_RSL_INTERACTION_EMERGENCY_STATES_EVAL_EMERGENCY;
          }
        }
        break;
        
      case APP_RSL_INTERACTION_EMERGENCY_STATES_EVAL_EMERGENCY:
        if ((retVal = userMethods_characteristics_getTransmissionState()) == USERMETHODS_CHARACTERISTICS_TRANSMISSION_STATE_DONE){
          app_misc_alert_unset();
          app_misc_errorAndOldAlert_unset();
        }else{
          app_misc_alert_setNewToOld();
        }
        // Transition to next state
        stateRetryCounter = 0;
        emergencyStatemachine = APP_RSL_INTERACTION_EMERGENCY_STATES_UPDATE_POST;
        break;
        
      case APP_RSL_INTERACTION_EMERGENCY_STATES_UPDATE_POST:
        // Try update all characteristics
        app_misc_sync_eepromToCommstack();
        if (app_rsl_updateAllCharacteristics(led_red) == FALSE){
          // Failed
          
          // Do we still have tries?
          if (++stateRetryCounter >= eeprom_getRepetitionCountEmergency()){
            // No
            
            // Set flag for problematic afterupdate
            afterEmergencyUpdateState = FALSE;
          }
        }else{
          // Success 
          
          // Transition to next state
          stateRetryCounter = 0;
          emergencyStatemachine = APP_RSL_INTERACTION_EMERGENCY_STATES_USERMESSAGE;
        }
        break;
        
      case APP_RSL_INTERACTION_EMERGENCY_STATES_USERMESSAGE:
        // Was emergency successful?
        if (retVal == USERMETHODS_CHARACTERISTICS_TRANSMISSION_STATE_DONE){
          // Yes
          
          // Could the device afterwards update the parameter OR was it not needed?
          if (afterEmergencyUpdateState == TRUE){
            // Yes
            
            // Show user success
            app_misc_showSuccess();
          }else{
            // No - there was a problem with updating this
            
            // Show user failure in that
            app_misc_showProblematicSuccess();
          }
        }else{
          // No
          
          // Show user error
          app_misc_showError();
        }
        return;
        break;
      
    }
  }while(1);
  
}
