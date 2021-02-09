/**
  ******************************************************************************
  * @file       Bootsource.c
  * @author     Tim Steinberg
  * @date       06.07.2020
  * @brief      This code will get the source of the (re-)boot.
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
#include "main.h"
#include "MasterDefine.h"
#include "Debug.h"
#include "Userbutton.h"
#include "Bootsource.h"

/* Typedefinitions / Prototypes */

/* Variables */

/* Function definitions */

BOOLEAN Boot_CheckButtonDebounced(void){
  if(HAL_GPIO_ReadPin(GPO_BUTTON_PULL_GPIO_Port, GPO_BUTTON_PULL_Pin) == 0){
    for (int i = 0; i < 10000; i++){
      if(HAL_GPIO_ReadPin(GPO_BUTTON_PULL_GPIO_Port, GPO_BUTTON_PULL_Pin) != 0){
        return FALSE;
      }
    }
    return TRUE;
  }
  return FALSE;
  
  /*
  if(HAL_GPIO_ReadPin(GPO_BUTTON_PULL_GPIO_Port, GPO_BUTTON_PULL_Pin) == 0){
    HAL_Delay(1);
    if(HAL_GPIO_ReadPin(GPO_BUTTON_PULL_GPIO_Port, GPO_BUTTON_PULL_Pin) == 0){
      return TRUE;
    }
  }
  return FALSE;
*/
}

SYSTEMBOOT_SOURCE_TYPEDEF Boot_CheckStartupEvent(void){
  TRACE_PROCEDURE_CALLS(1, "Boot_CheckStartupEvent(void)\r\n");

  // Initialization, set source to default value
  SYSTEMBOOT_SOURCE_TYPEDEF source = BOOT_STARTUP_UNKNOWN; 

  // Check if the POR flag is set -> power got plugged in (battery)
  if(((RCC->CSR) & 0x08000000) != 0){    
    source = BOOT_STARTUP_BATTERY;
  }else{
    // Check if the IWDG was the cause
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST) != RESET){
      source = BOOT_STARTUP_WATCHDOG;
    }else{
      // If not, Check if Button pressed with debouncing
      
      // With the UPCBA and the RAC80, each status can be used (closing contact and opening contact) for the RESET
      // Then, Reset => An event occured! Meaning No glitch can be detected...
      if(Boot_CheckButtonDebounced()){
        switch(UserButtonMeasurePresstime()){
          case SYSTEMBOOT_SOURCE_BUTTON_NOTHING         :
            source = BOOT_STARTUP_BUTTON_NOTHING;
            break;
          case SYSTEMBOOT_SOURCE_BUTTON_EMERGENCY       :
            source = BOOT_STARTUP_BUTTON_EMERGENCY;
            break;
          case SYSTEMBOOT_SOURCE_BUTTON_PAIRING         :
            source = BOOT_STARTUP_BUTTON_PAIRING;
            break;
          case SYSTEMBOOT_SOURCE_BUTTON_BARRELROLL      :
            source = BOOT_STARTUP_BUTTON_BARRELROLL;
            break;
        }
      }else{
        // No valid/known cause of reset detected
        // FW Reset or Glitch (electrostatic discharge for example)
        source = BOOT_STARTUP_UNKNOWN;
      }
    }
  }
  // Clear All Registers RESET Flags
  RCC->CSR = RCC->CSR | 0x00800000;
  return source;
}