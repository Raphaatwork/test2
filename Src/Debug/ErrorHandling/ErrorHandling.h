/**
  ******************************************************************************
  * @file       ErrorHandling.h
  * @author     Tim Steinberg
  * @date       13.07.2020
  * @brief      Code to safe Errorcodes in designated EEPROM-area
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
  * | 001       | 2020-07-13    | Tim Steinberg         | Initial version / skeleton of file            |
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

#ifndef TXV2_ERRORHANDLING_H
#define TXV2_ERRORHANDLING_H

/* Includes */
#include "stm32l0xx_hal.h"
#include "MasterDefine.h"
#include "EEPROM_Access.h"

/* Typedefinitions */
typedef enum ERROR_CODES {
  NO_ERROR                              = 0x00000000,
  // ACCESS TO PERIPHERALS
  ACCESS_EEPROM_ACCESS_FAILED           = 0x00010000,
  ACCESS_WATCHDOG_FEED_FAILED           = 0x00010001,
  ACCESS_RTC_CALIBRATION_FAILED         = 0x00010002,
  
  // INIT OF PERIPHERALS
  INIT_WATCHDOG_FAILED                  = 0x00020000,
  INIT_UART_RSL_FAILED                  = 0x00020001,
  INIT_ADC_FAILED                       = 0x00020002,
  INIT_ADC_CHANNEL_FAILED               = 0x00020003,
  INIT_RTC_FAILED                       = 0x00020004,
  INIT_CRC_FAILED                       = 0x00020005,
  INIT_SPI_FAILED                       = 0x00020006,
  
  UNKNOWN                               = 0xFFFFFFFE,
} ERROR_CODES_TYPEDEF;
/* Variables */

/* Function definitions */

//==========================================//
// Set functions
//==========================================//

void Error_SetError_FlashCorrupt(void);
void Error_SetError_EEPCorrupt(void);
void Error_SetError_S2LP(void);
void Error_SetError_RSL(void);
void Error_SetError_WDTriggered(void);
void Error_SetError_UETriggered(void);
void Error_SetError_OldAlert(void);

//==========================================//
// Reset functions
//==========================================//

void Error_ResetErrorWithAlert(void);
void Error_ResetErrorWithoutAlert(void);

//==========================================//
// Unset functions
//==========================================//

void Error_UnsetError_FlashCorrupt(void);
void Error_UnsetError_EEPCorrupt(void);
void Error_UnsetError_S2LP(void);
void Error_UnsetError_RSL(void);
void Error_UnsetError_WDTriggered(void);
void Error_UnsetError_UETriggered(void);
void Error_UnsetError_OldAlert(void);

void Error_Handler_TxV2(ERROR_CODES_TYPEDEF errorCode);

#endif
