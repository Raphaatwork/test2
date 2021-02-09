/**
  ******************************************************************************
  * @file       EEPROM_Access.c
  * @author     Tim Steinberg
  * @date       06.07.2020
  * @brief      Access to a range of variables that are saved in EEPROM.
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
#include "EEPROM_Map.h"
#include "MasterDefine.h"
#include "Watchdog.h"

#include "ErrorHandling.h"
#include "EEPROMCheck.h"

/* Typedefinitions / Prototypes */

#define EEPROM_WRITE_RETRY_MAXIMUM_COUNT        8

/* Variables */

/* Function definitions */

//==========================================//
// Internal functions
//==========================================//

BOOLEAN eeprom_unlock(){
  uint8_t retryCounter = 0;
  HAL_StatusTypeDef hstd = HAL_OK;
  do{
    hstd = HAL_FLASHEx_DATAEEPROM_Unlock();
  }while((retryCounter < 10) && (hstd != HAL_OK));
  if (hstd == HAL_OK){
    return TRUE;
  }else{
    return FALSE;
  }
}

BOOLEAN eeprom_lock(){
  uint8_t retryCounter = 0;
  HAL_StatusTypeDef hstd = HAL_OK;
  do{
    hstd = HAL_FLASHEx_DATAEEPROM_Lock();
  }while((retryCounter < 10) && (hstd != HAL_OK));
  if (hstd == HAL_OK){
    return TRUE;
  }else{
    return FALSE;
  }
}

//==========================================//
// Access methods
//==========================================//

BOOLEAN eeprom_writeErase(uint32_t offset){
  uint8_t retryCounter = 0;
  HAL_StatusTypeDef hstd = HAL_OK;
  do{
    hstd = HAL_FLASHEx_DATAEEPROM_Erase(eepromMemoryMap_getEEPROMBaseAddress() + offset);
  }while((retryCounter < 10) && (hstd != HAL_OK));
  if (hstd == HAL_OK){
    return TRUE;
  }else{
    return FALSE;
  }
}

BOOLEAN eeprom_writeByte(uint32_t value, uint32_t offset){
  uint8_t retryCounter = 0;
  HAL_StatusTypeDef hstd = HAL_OK;
  do{
    hstd = HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, eepromMemoryMap_getEEPROMBaseAddress() + offset, value);
  }while((retryCounter < 10) && (hstd != HAL_OK));
  if (hstd == HAL_OK){
    return TRUE;
  }else{
    return FALSE;
  }
}

BOOLEAN eeprom_writeHalfWord(uint32_t value, uint32_t offset){
  uint8_t retryCounter = 0;
  HAL_StatusTypeDef hstd = HAL_OK;
  do{
    hstd = HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_HALFWORD, eepromMemoryMap_getEEPROMBaseAddress() + offset, value);
  }while((retryCounter < 10) && (hstd != HAL_OK));
  if (hstd == HAL_OK){
    return TRUE;
  }else{
    return FALSE;
  }
}

BOOLEAN eeprom_writeWord(uint32_t value, uint32_t offset){
  uint8_t retryCounter = 0;
  HAL_StatusTypeDef hstd = HAL_OK;
  do{
    hstd = HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD, eepromMemoryMap_getEEPROMBaseAddress() + offset, value);
  }while((retryCounter < 10) && (hstd != HAL_OK));
  if (hstd == HAL_OK){
    return TRUE;
  }else{
    return FALSE;
  }
}

//==========================================//
// Complete write methods
//==========================================//

bool eeprom_writeByte_withValueCheck(uint8_t value, uint32_t offset){
  BOOLEAN success = FALSE;
  uint8_t retryCounter = 0;
  uint8_t temp8 = *((uint8_t*) (eepromMemoryMap_getEEPROMBaseAddress() + offset));
  // Compare step - don't E/W if already contained
  if (temp8 == value){
    return TRUE;
  }
  // Write step
  do{
    watchdog_feed();
    eeprom_unlock();
    eeprom_writeErase(offset);
    eeprom_writeByte(value, offset);
    if (*((uint8_t*) (eepromMemoryMap_getEEPROMBaseAddress() + offset)) == value){
      if (eepromCheck_UpdateEEPROMCRC() == FALSE){
        Error_SetError_EEPCorrupt();
        return FALSE;
      }
      return TRUE;
    }
  }while((!success) || (++retryCounter >= EEPROM_WRITE_RETRY_MAXIMUM_COUNT));
  return FALSE;
}

bool eeprom_writeHalfword_withValueCheck(uint16_t value, uint32_t offset){
  BOOLEAN success = FALSE;
  uint8_t retryCounter = 0;
  uint16_t temp16 = *((uint16_t*) (eepromMemoryMap_getEEPROMBaseAddress() + offset));
  // Compare step - don't E/W if already contained
  if (temp16 == value){
    return TRUE;
  }
  // Write step
  do{
    watchdog_feed();
    eeprom_unlock();
    eeprom_writeErase(offset);
    eeprom_writeHalfWord(value, offset);
    eeprom_lock();
    if (*((uint16_t*) (eepromMemoryMap_getEEPROMBaseAddress() + offset)) == value){
      if (eepromCheck_UpdateEEPROMCRC() == FALSE){
        Error_SetError_EEPCorrupt();
        return FALSE;
      }
      return TRUE;
    }
  }while((!success) || (++retryCounter >= EEPROM_WRITE_RETRY_MAXIMUM_COUNT));
  return FALSE;
}

bool eeprom_writeWord_withValueCheck(uint32_t value, uint32_t offset){
  BOOLEAN success = FALSE;
  uint8_t retryCounter = 0;
  uint32_t tmp32 = *((uint32_t*) (eepromMemoryMap_getEEPROMBaseAddress() + offset));
  // Compare step - don't E/W if already contained
  if (tmp32 == value){
    return TRUE;
  }
  // Write step
  do{
    watchdog_feed();
    eeprom_unlock();
    eeprom_writeErase(offset);
    eeprom_writeWord(value, offset);
    eeprom_lock();
    if (*((uint32_t*) (eepromMemoryMap_getEEPROMBaseAddress() + offset)) == value){
      if (eepromCheck_UpdateEEPROMCRC() == FALSE){
        Error_SetError_EEPCorrupt();
        return FALSE;
      }
      return TRUE;
    }
  }while((!success) || (++retryCounter >= EEPROM_WRITE_RETRY_MAXIMUM_COUNT));
  return FALSE;
}

//==========================================//
// Mapper for param ID access
//==========================================//

void eeprom_writeWord_byId_WithValueCheck(uint32_t value, uint8_t id){
  uint32_t addrOffset = id << 2;
  eeprom_writeWord_withValueCheck(value, addrOffset);
}

uint32_t eeprom_getWord_byId(uint8_t id){
  uint32_t addrOffset = id << 2;
  return (*((uint32_t*) (eepromMemoryMap_getEEPROMBaseAddress() + addrOffset )));
}