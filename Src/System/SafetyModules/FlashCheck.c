/**
  ******************************************************************************
  * @file       FlashCheck.c
  * @author     Tim Steinberg
  * @date       30.11.2020
  * @brief      Check for the Flash-Integrity
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
  * | 001       | 2020-11-30    | Tim Steinberg         | Initial version / skeleton of file            |
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
#include "MasterDefine.h"
#include "Debug.h"
#include "ErrorHandling.h"

#include "EEPROM_ApplicationMapped.h"
#include "EEPROM_Map.h"
#include "CRC.h"

/* Typedefinitions / Prototypes */
#define LEESYS_FLASH_START     0x08000000
#define LEESYS_FLASH_END       0x0800FFFF
#define LEESYS_FLASH_SIZE      ((LEESYS_FLASH_END - LEESYS_FLASH_START) >> 2)

/* Variables */

/* Function definitions */
uint32_t flashCheck_BuildFlashCRC(void){
  return crc_calcCrc32_MPEG2((uint32_t*) LEESYS_FLASH_START, LEESYS_FLASH_SIZE);
}

bool flashCheck_CheckFlashCRC(void){
  uint32_t crcResult = flashCheck_BuildFlashCRC();
  uint32_t crcEEPValue = eeprom_getFlashCRC();
  if (crcResult == crcEEPValue){
    return TRUE;
  }else{
    return FALSE;
  }
}

bool flashCheck_UpdateEEPROMCRC(void){
  uint32_t crcResult = flashCheck_BuildFlashCRC();
  return eeprom_setFlashCRC(crcResult);
}