/**
  ******************************************************************************
  * @file       App_RSL_Interaction.c
  * @author     Tim Steinberg
  * @date       10.11.2020
  * @brief      Top level interaction for the RSL. 
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
  * | 001       | 2020-11-10    | Tim Steinberg         | Initial version / skeleton of file            |
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
#include "EEPROM_Access.h"
#include "UserMethods_Characteristics.h"
#include "App_Misc.h"
#include "App_RSL.h"
#include "Led.h"

/* Typedefinitions / Prototypes */

/* Variables */

/* Function definitions */
void app_rsl_interaction_emergency_bluetooth(void){
  USERMETHODS_CHARACTERISTICS_TRANSMISSION_STATE_TYPEDEF retVal;
  
  app_misc_battery_measureNewBatteryValue();
  app_misc_alert_setNew();
  app_misc_sync_eepromToCommstack();
  
  if (app_rsl_updateAllCharacteristics(eeprom_getRepetitionCountEmergency(), led_red) == FALSE){
    app_misc_showErrorCritical();
    return;
  }
  
  userMethods_characteristics_setTransmissionState_None();
  
  if (app_rsl_emergency(eeprom_getRepetitionCountEmergency(), led_red) == FALSE){
    app_misc_showErrorCritical();
    return;
  }
  
  if ((retVal = userMethods_characteristics_getTransmissionState()) == USERMETHODS_CHARACTERISTICS_TRANSMISSION_STATE_DONE){
    app_misc_alert_unset();
    app_misc_error_unset();
  }else{
    app_misc_alert_setNewToOld();
  }
  
  if (app_rsl_updateAllCharacteristics(eeprom_getRepetitionCountEmergency(), led_red) == FALSE){
    app_misc_showErrorCritical();
    return;
  }
  
  if (retVal == USERMETHODS_CHARACTERISTICS_TRANSMISSION_STATE_DONE){
    app_misc_showSuccess();
  }
}

void app_rsl_interaction_pairing_bluetooth(void){
  USERMETHODS_CHARACTERISTICS_PAIRING_STATE_TYPEDEF retValPairing;
  
  app_misc_battery_measureNewBatteryValue();
  app_misc_sync_eepromToCommstack();
  
  if (app_rsl_updateAllCharacteristics(eeprom_getRepetitionCountPairing(), led_blue) == FALSE){
    app_misc_showErrorCritical();
    return;
  }
  
  userMethods_characteristics_setTransmissionState_None();
  userMethods_characteristics_setPairingStateUnpaired();
  
  if (app_rsl_pairing(eeprom_getRepetitionCountPairing(), led_blue) == FALSE){
    app_misc_showErrorCritical();
    return;
  }
  
  if ((retValPairing = userMethods_characteristics_getPairingState()) == USERMETHODS_CHARACTERISTICS_PAIRING_STATE_PAIRED){
    eeprom_setPaired();
  }else{
    app_misc_showError();
  }
  
  if (userMethods_characteristics_getTransmissionState() == USERMETHODS_CHARACTERISTICS_TRANSMISSION_STATE_DONE){
    app_misc_alert_unset();
    app_misc_error_unset();
    if (app_rsl_updateAllCharacteristics(eeprom_getRepetitionCountPairing(), led_blue) == FALSE){
      app_misc_showErrorCritical();
    }
  }
  
  if (retValPairing == USERMETHODS_CHARACTERISTICS_PAIRING_STATE_PAIRED){
    app_misc_showSuccess();
  }
}

void app_rsl_interaction_battery_bluetooth(void){
  USERMETHODS_CHARACTERISTICS_TRANSMISSION_STATE_TYPEDEF retVal;
  
  app_misc_battery_measureNewBatteryValue();
  app_misc_sync_eepromToCommstack();
  
  if (app_rsl_updateAllCharacteristics(eeprom_getRepetitionCountBattery(), led_green) == FALSE){
    app_misc_showErrorCritical();
    return;
  }
  
  userMethods_characteristics_setTransmissionState_None();
  
  if (app_rsl_emergency(eeprom_getRepetitionCountBattery(), led_green) == FALSE){
    app_misc_showErrorCritical();
    return;
  }
  
  if ((retVal = userMethods_characteristics_getTransmissionState()) == USERMETHODS_CHARACTERISTICS_TRANSMISSION_STATE_DONE){
    app_misc_alert_unset();
    app_misc_error_unset();
  }else{
    app_misc_alert_setNewToOld();
  }
  
  if (app_rsl_updateAllCharacteristics(eeprom_getRepetitionCountBattery(), led_green) == FALSE){
    app_misc_showErrorCritical();
    return;
  }
  
  if (retVal == USERMETHODS_CHARACTERISTICS_TRANSMISSION_STATE_DONE){
    app_misc_showSuccess();
  }
}
