/**
  ******************************************************************************
  * @file       ErrorCodes.c
  * @author     Tim Steinberg
  * @date       08.07.2020
  * @brief      Error flaggings for different purpose
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

#ifndef __ERRORCODES_H
#define __ERRORCODES_H


typedef enum ERRORCODES{
  ERRORCODES_FLASH_CORRUPT                      = 0x01,
  ERRORCODES_NO_CONFIG_EEP_CORRUPT              = 0x02,
  ERRORCODES_S2LP_ERROR                         = 0x04,
  ERRORCODES_RSL_ERROR                          = 0x08,
  ERRORCODES_WD_TRIGGERED                       = 0x10,
  ERRORCODES_UE_TRIGGERED                       = 0x20,
  ERRORCODES_OLDALERT                           = 0x40,
  ERRORCODES_FREE_B                             = 0x80,
} ERRORCODES_TYPEDEF; 

#define ERROR_CODE_CLEAR                                                        0x00

// STM32 finds that there is some problem with flash 
#define ERROR_CODE_SET_MASK_FLASH_CORRUPT                                       0x01
#define ERROR_CODE_RESET_MASK_FLASH_CORRUPT                                     (~0x01)

// STM32 finds that there is some problem with EEPROM 
#define ERROR_CODE_SET_MASK_NO_CONFIG_EEP_CORRUPT                               0x02
#define ERROR_CODE_RESET_MASK_NO_CONFIG_EEP_CORRUPT                             (~0x02)

// S2LP not answering 
#define ERROR_CODE_SET_MASK_S2LP_ERROR                                          0x04
#define ERROR_CODE_RESET_MASK_S2LP_ERROR                                        (~0x04)

// RSL10 not answering 
#define ERROR_CODE_SET_MASK_RSL_ERROR                                           0x08
#define ERROR_CODE_RESET_MASK_RSL_ERROR                                         (~0x08)

// STM32 was restarted by watchdog - should never occur though 
#define ERROR_CODE_SET_MASK_WD_TRIGGERED                                        0x10
#define ERROR_CODE_RESET_MASK_WD_TRIGGERED                                      (~0x10)

// STM32 was restarted by an unknown cause - should never occur though 
#define ERROR_CODE_SET_MASK_UE_TRIGGERED                                        0x20
#define ERROR_CODE_RESET_MASK_UE_TRIGGERED                                      (~0x20)

// RESERVED FOR WHATEVER YOU NEED THAT 
#define ERROR_CODE_SET_OLDALERT                                                 0x40
#define ERROR_CODE_RESET_OLDALERT                                               (~0x40)

// RESERVED FOR WHATEVER YOU NEED THAT 
#define ERROR_CODE_SET_MASK_B                                                   0x80
#define ERROR_CODE_RESET_MASK_B                                                 (~0x80)



#endif