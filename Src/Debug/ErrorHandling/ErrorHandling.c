/********************************************************************************
*                 Leesys - Helping care solutions                               *
* Module:               ErrorHandling.c                                         *
* Product:              TxV2                                                    *
* Development system:   IAR Embedded Workbench 8.42.2                           *
* Description:          Error handling                                          *
*                                                                               *
*********************************************************************************/

/* Includes ------------------------------------------------------------------*/

#include "stm32l0xx_hal.h"
#include "ErrorCodes.h"
#include "ErrorHandling.h"
#include "EEPROM_ApplicationMapped.h"

#define ERROR_FLAGS_ONLINE 1

//==========================================//
// Set functions
//==========================================//

void Error_SetError_FlashCorrupt(void){
#if ERROR_FLAGS_ONLINE == 1
  uint8_t ec = eeprom_getErrorValue() | ERROR_CODE_SET_MASK_FLASH_CORRUPT;
  eeprom_setErrorValue(ec);
#else
  #warning "FLASH CORRUPT ERROR NOT ACTIVATATED!"
#endif
}

void Error_SetError_EEPCorrupt(void){
#if ERROR_FLAGS_ONLINE == 1
  uint8_t ec = eeprom_getErrorValue() | ERROR_CODE_SET_MASK_NO_CONFIG_EEP_CORRUPT;
  eeprom_setErrorValue(ec);
#else
  #warning "EEP CORRUPT ERROR NOT ACTIVATATED!"
#endif
}

void Error_SetError_S2LP(void){
#if ERROR_FLAGS_ONLINE == 1
  uint8_t ec = eeprom_getErrorValue() | ERROR_CODE_SET_MASK_S2LP_ERROR;
  eeprom_setErrorValue(ec);
#else
  #warning "S2LP COMMUNICATION ERROR NOT ACTIVATATED!"
#endif
}

void Error_SetError_RSL(void){
#if ERROR_FLAGS_ONLINE == 1
  uint8_t ec = eeprom_getErrorValue() | ERROR_CODE_SET_MASK_RSL_ERROR;
  eeprom_setErrorValue(ec);
#else
  #warning "RSL COMMUNICATION ERROR NOT ACTIVATATED!"
#endif
}

void Error_SetError_WDTriggered(void){
#if ERROR_FLAGS_ONLINE == 1
  uint8_t ec = eeprom_getErrorValue() | ERROR_CODE_SET_MASK_WD_TRIGGERED;
  eeprom_setErrorValue(ec);
#else
  #warning "WD RESTART CAUSE ERROR NOT ACTIVATATED!"
#endif
}

void Error_SetError_UETriggered(void){
#if ERROR_FLAGS_ONLINE == 1
  uint8_t ec = eeprom_getErrorValue() | ERROR_CODE_SET_MASK_UE_TRIGGERED;
  eeprom_setErrorValue(ec);
#else
  #warning "UE RESTART CAUSE ERROR NOT ACTIVATATED!"
#endif
}

void Error_SetError_OldAlert(void){
  uint8_t ec = eeprom_getErrorValue() | ERROR_CODE_SET_OLDALERT;
  eeprom_setErrorValue(ec);
}

//==========================================//
// Reset functions
//==========================================//

void Error_ResetErrorWithAlert(void){
  eeprom_setErrorValue(ERROR_CODE_CLEAR);
}

void Error_ResetErrorWithoutAlert(void){
  uint8_t ec = eeprom_getErrorValue();
  eeprom_setErrorValue(ec &= ERROR_CODE_SET_OLDALERT);
}

//==========================================//
// Unset functions
//==========================================//

void Error_UnsetError_FlashCorrupt(void){
  uint8_t ec = eeprom_getErrorValue();
  eeprom_setErrorValue(ec &= ERROR_CODE_RESET_MASK_FLASH_CORRUPT);
}

void Error_UnsetError_EEPCorrupt(void){
  uint8_t ec = eeprom_getErrorValue();
  eeprom_setErrorValue(ec &= ERROR_CODE_RESET_MASK_NO_CONFIG_EEP_CORRUPT);
}

void Error_UnsetError_S2LP(void){
  uint8_t ec = eeprom_getErrorValue();
  eeprom_setErrorValue(ec &= ERROR_CODE_RESET_MASK_S2LP_ERROR);
}

void Error_UnsetError_RSL(void){
  uint8_t ec = eeprom_getErrorValue();
  eeprom_setErrorValue(ec &= ERROR_CODE_RESET_MASK_RSL_ERROR);
}

void Error_UnsetError_WDTriggered(void){
  uint8_t ec = eeprom_getErrorValue();
  eeprom_setErrorValue(ec &= ERROR_CODE_RESET_MASK_WD_TRIGGERED);
}

void Error_UnsetError_UETriggered(void){
  uint8_t ec = eeprom_getErrorValue();
  eeprom_setErrorValue(ec &= ERROR_CODE_RESET_MASK_UE_TRIGGERED);
}

void Error_UnsetError_OldAlert(void){
  uint8_t ec = eeprom_getErrorValue();
  eeprom_setErrorValue(ec &= ERROR_CODE_RESET_OLDALERT);
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler_TxV2(ERROR_CODES_TYPEDEF errorCode)
{
  /* USER CODE BEGIN Error_Handler_Debug */

  /* USER CODE END Error_Handler_Debug */
}