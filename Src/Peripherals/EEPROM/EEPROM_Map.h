/**
  ******************************************************************************
  * @file       EEPROM_Map.h
  * @author     Tim Steinberg
  * @date       06.07.2020
  * @brief      Addresses of all variables safed in EEPROM.
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

#ifndef __EEPROM_MAP_H
#define __EEPROM_MAP_H

/* Includes */
#include "stm32l0xx_hal.h"

/* Typedefinitions */
#define EEPROM_MAP_OFFSET_IDENT                         0
#define EEPROM_MAP_OFFSET_BATTERY                       4
#define EEPROM_MAP_OFFSET_ALERT                         8
#define EEPROM_MAP_OFFSET_ERROR                         12
#define EEPROM_MAP_OFFSET_SPACER_A                      16
#define EEPROM_MAP_OFFSET_BATTERYLOWCOUNTER             20
#define EEPROM_MAP_OFFSET_PAIRING_STATE                 24
#define EEPROM_MAP_OFFSET_ALERTCOUNTER                  28
#define EEPROM_MAP_OFFSET_LSICALIBRATION                32
#define EEPROM_MAP_OFFSET_S2LP_SYNTH                    36
#define EEPROM_MAP_OFFSET_S2LP_OUTPUTPOWER_8_5          40
#define EEPROM_MAP_OFFSET_S2LP_OUTPUTPOWER_4_1          44
#define EEPROM_MAP_OFFSET_S2LP_OUTPUTPOWER_0            48
#define EEPROM_MAP_OFFSET_S2LP_CLOCKSOURCE_SELECTOR     52
#define EEPROM_MAP_OFFSET_BATTERYLOWTHRESHOLDVALUE      56
#define EEPROM_MAP_OFFSET_SPACER_G                      60
#define EEPROM_MAP_OFFSET_RETRYCOUNTEMERGENCY           64
#define EEPROM_MAP_OFFSET_RETRYCOUNTPAIRING             68
#define EEPROM_MAP_OFFSET_RETRYCOUNTBATTERY             72
#define EEPROM_MAP_OFFSET_NUMBEROFTRANSMISSIONS868      76
#define EEPROM_MAP_OFFSET_NUMBEROFTRANSMISSIONSBLE      80
#define EEPROM_MAP_OFFSET_NUMBEROFWAITCYCLES            84
#define EEPROM_MAP_OFFSET_SPACER_H                      88
#define EEPROM_MAP_OFFSET_SPACER_I                      92
#define EEPROM_MAP_OFFSET_SPACER_J                      96
#define EEPROM_MAP_OFFSET_SPACER_K                      100
#define EEPROM_MAP_OFFSET_SPACER_L                      104
#define EEPROM_MAP_OFFSET_SPACER_M                      108
#define EEPROM_MAP_OFFSET_SPACER_N                      112
#define EEPROM_MAP_OFFSET_SPACER_O                      116
#define EEPROM_MAP_OFFSET_SPACER_P                      120
#define EEPROM_MAP_OFFSET_SPACER_Q                      124

// DYNAMICS

#define EEPROM_MAP_OFFSET_SOFTWARE_VERSION              128
#define EEPROM_MAP_OFFSET_SOFTWARE_STATUS               132
#define EEPROM_MAP_OFFSET_SOFTWARE_MAJOR                136
#define EEPROM_MAP_OFFSET_SOFTWARE_MINOR                140
#define EEPROM_MAP_OFFSET_SOFTWARE_BUILD                144
#define EEPROM_MAP_OFFSET_SOFTWARE_CMI                  148
#define EEPROM_MAP_OFFSET_SERIALNUMBER                  152
#define EEPROM_MAP_OFFSET_HARDWARE_PCB_VERSION          156
#define EEPROM_MAP_OFFSET_HARDWARE_BOM_VERSION          160
#define EEPROM_MAP_OFFSET_U_ID                          164
#define EEPROM_MAP_OFFSET_SPACER_1                      168
#define EEPROM_MAP_OFFSET_SPACER_2                      172
#define EEPROM_MAP_OFFSET_SPACER_3                      176
#define EEPROM_MAP_OFFSET_SPACER_4                      180
#define EEPROM_MAP_OFFSET_SPACER_5                      184
#define EEPROM_MAP_OFFSET_SPACER_6                      188
#define EEPROM_MAP_OFFSET_SPACER_7                      192
#define EEPROM_MAP_OFFSET_SPACER_8                      196
#define EEPROM_MAP_OFFSET_SPACER_9                      200
#define EEPROM_MAP_OFFSET_SPACER_10                     204
#define EEPROM_MAP_OFFSET_SPACER_11                     208
#define EEPROM_MAP_OFFSET_SPACER_12                     212
#define EEPROM_MAP_OFFSET_SPACER_13                     216
#define EEPROM_MAP_OFFSET_SPACER_14                     220
#define EEPROM_MAP_OFFSET_SPACER_15                     224
#define EEPROM_MAP_OFFSET_SPACER_16                     228
#define EEPROM_MAP_OFFSET_SPACER_17                     232
#define EEPROM_MAP_OFFSET_SPACER_18                     236
#define EEPROM_MAP_OFFSET_SPACER_19                     240
#define EEPROM_MAP_OFFSET_SPACER_20                     244
#define EEPROM_MAP_OFFSET_SPACER_21                     248
#define EEPROM_MAP_OFFSET_SPACER_22                     252

  // END OF USED EEPROM SPACE

#define EEPROM_MAP_OFFSET_FLASH_CRC                     256
#define EEPROM_MAP_OFFSET_EEPROM_CRC                    260

/* Variables */

/* Function definitions */
uint32_t eepromMemoryMap_getEEPROMBaseAddress(void);

#endif