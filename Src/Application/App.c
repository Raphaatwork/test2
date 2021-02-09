/**
  ******************************************************************************
  * @file       App.c
  * @author     Tim Steinberg
  * @date       06.07.2020
  * @brief      The application for TxV2
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
#include "DebugMain.h"
#include "MasterDefine.h"
#include "ErrorHandling.h"
#include "Bootsource.h"

#include "Runmode_Awake.h"
#include "Runmode_Sleep.h"
#include "Tx_Interpreter.h"

#include "CRC.h"
#include "Led.h"
#include "GPIO.h"
#include "EEPROM_ApplicationMapped.h"
#include "RTC.h"
#include "ADC.h"
#include "Watchdog.h"
#include "App_Misc.h"
#include "App_868MHz.h"
#include "App_RSL_Interaction_Broadcast.h"
#include "App_RSL_Interaction_Emergency.h"
#include "App_RSL_Interaction_Pairing.h"
#include "App_Misc.h"

#include "EEPROMCheck.h"
#include "FlashCheck.h"

/* Typedefinitions / Prototypes */
void app_TXV2_checkIntegrity(void);

/* Variables */

/* Function definitions */
void app_TXV2_checkIntegrity(void){
  watchdog_feed();
  if (flashCheck_CheckFlashCRC() == FALSE){
    Error_SetError_FlashCorrupt();
  }
  
  watchdog_feed();
  if (eepromCheck_CheckEEPROMCRC() == FALSE){
    Error_SetError_EEPCorrupt();
  }
}

void app_TXV2_boot(void){
  
  watchdog_feed();
  runmode_awake();
  watchdog_feed();
  gpio_userButtonUnarmedMode();
  watchdog_feed();
  
  txInterpreter_DoTestMode();
  // pin dearm
  watchdog_feed();

  SYSTEMBOOT_SOURCE_TYPEDEF bootsource = Boot_CheckStartupEvent();
  //bootsource = BOOT_STARTUP_BUTTON_EMERGENCY;
  
  watchdog_feed();
    
  // PRECONDITION TO ENTER APP
  // -> System clock set up
  // -> System itself set up
  // -> LED PINs initialized, turned off
  // -> Watchdog running, armed
  // -> RTC not running, not armed
  // -> LSI not calibrated
  
  // Check the integrity of the whole system
  //app_TXV2_checkIntegrity();
#warning "SECURITY SUBSYSTEM DISABLED BY COMMENTING THIS"
  
  switch(bootsource){
    
    case BOOT_STARTUP_UNKNOWN           :      
      // An unknown cause led to a restart
      watchdog_feed();
      app_misc_error_maskInNewErrorcode(ERRORCODES_UE_TRIGGERED);
  
      led_magenta();
      HAL_Delay(400);
      led_black();
      break;
      
    case BOOT_STARTUP_WATCHDOG          :      
      // The watchdog led to a restart. TxV2 was hanging somewhere
      watchdog_feed();
      app_misc_error_maskInNewErrorcode(ERRORCODES_WD_TRIGGERED);
  
      led_turquoise();
      HAL_Delay(400);
      led_black();
      break;
      
    case BOOT_STARTUP_BATTERY           :
      // The battery was freshly introduced
      watchdog_feed();
      led_red();
      HAL_Delay(400);
      led_green();
      HAL_Delay(400);
      led_blue();
      HAL_Delay(400);
      led_black();
      watchdog_feed();
      
      runmode_sleep_prepare();
      rtc_lsi_calibration();
      watchdog_feed();

      adc_doInitialCalibration();
      watchdog_feed();
      
      if (app_misc_battery_isBatteryNew() == TRUE){
        app_misc_battery_resetBatteryLowCounter();
      }
      watchdog_feed();
      
      // Set button to AMRED
      gpio_userButtonArmedMode(); 

      // Measure new battery value
      app_misc_battery_measureNewBatteryValue();
      watchdog_feed();
      
      // Do a 868 MHz action
#warning "KEEP THIS RIGHT"
      if (eeprom_getBatteryValue() < 1){
        // Send battery low state
        app_868mhz_transmitMessage(FALSE, TRUE);
      }else{
        // Send nothing
        app_868mhz_transmitMessage(FALSE, FALSE);
      }
    
      // If not paired, then don't do bluetooth
      if (eeprom_getPairingState() == TRUE){
        app_rsl_interaction_broadcast_main();
      }
      break;
      
    //========================================================================//
    // The following cases are only reached, if the button was pressed
    // The userbutton will be restricted for these cases
    //========================================================================//
      
    case BOOT_STARTUP_BUTTON_NOTHING    :
      // The button was pressed too short
      watchdog_feed();
      break;
      
    case BOOT_STARTUP_BUTTON_BARRELROLL :
      // The button was pressed too long
      watchdog_feed();
      //#include "App_Button_Barrelroll.h"
      //app_button_barrelroll();
      break;
      
    case BOOT_STARTUP_BUTTON_EMERGENCY  :
      watchdog_feed();
      // Do a 868 MHz action (ALERT YES, BATTERY ALERT NO)
      app_868mhz_transmitMessage(TRUE, FALSE);
      
      // If not paired, then don't do bluetooth
      if (eeprom_getPairingState() == TRUE){
        // Yes, we are paired
        
        // Do emergency
        app_rsl_interaction_emergency_main();
      }
      break;
      
    case BOOT_STARTUP_BUTTON_PAIRING    :
      watchdog_feed();
      // Do pairing
      app_rsl_interaction_pairing_main();
      break;
      
  }
  
  // POSTCONDITION AFTER LEAVING APP
  // -> System clock set up
  // -> System itself set up
  // -> LED PINs initialized
  // -> Watchdog running, armed
  // -> RTC not running and not armed
  // -> LSI not calibrated
  
  watchdog_feed();
  led_black();
  gpio_userButtonArmedMode();  
  gpio_ledAnalogMode();
  
  runmode_sleep_prepare();
}

#include "App_Misc.h"
#include "App_RSL_Interaction.h"
#include "Batterylevel.h"
    
void app_TXV2_main(void){ 
  watchdog_feed();
  //            1x sleep = 20 seconds
  //            3x sleep = 1 minute
  //           15x sleep = 5 minutes
  //          180x sleep = 1 hour
  //        4.320x sleep = 1 day
  //       30.240x sleep = 7 days
  //      129.600x sleep = 30 days
  //   11.037.600x sleep = 365 days
  for (int i = 0; i < eeprom_getWaitCycleCount(); i++){ // 3 = 1 minute, 15 = 5 minutes, 180 = 60 minutes, 
    #if DEBUG_PROCESSOR_HALTABLE > 0
      HAL_Delay(50);
    #else
      runmode_sleep(20);
    #endif
    watchdog_feed();
  }
  
  runmode_awake();
  watchdog_feed();
  
  app_TXV2_checkIntegrity();

  eeprom_setBatteryValue((uint8_t) batterylevel_getPercentage());
  watchdog_feed();
    
  #warning "RAPHAELS 868 MHZ PART BATTERY"
  
  // If not paired, then don't do bluetooth
  if (eeprom_getPairingState() == TRUE){
    // Yes, we are paired

    // Broadcast battery
    app_rsl_interaction_broadcast_main();
  }
  
  watchdog_feed();
}