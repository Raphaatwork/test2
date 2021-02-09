/**
  ******************************************************************************
  * @file       EEPROM_ApplicationMapped.h
  * @author     Tim Steinberg
  * @date       13.11.2020
  * @brief      Application usage of the EEPROM
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
  * | 001       | 2020-11-13    | Tim Steinberg         | Initial version / skeleton of file            |
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

#ifndef __EEPROM_APPLICATIONMAPPED_H
#define __EEPROM_APPLICATIONMAPPED_H

/* Includes */
#include "stm32l0xx_hal.h"
#include "MasterDefine.h"

/* Typedefinitions */

/* Variables */

/* Function definitions */
uint8_t  eeprom_getEACheckUint8_t();
uint16_t eeprom_getEACheckUint16_t();
uint32_t eeprom_getEACheckUint32_t();

uint8_t eeprom_getBatteryValue(void);
bool eeprom_setBatteryValue(uint8_t batteryVal);

uint8_t eeprom_getAlertValue(void);
bool eeprom_setAlertValue(uint8_t alertVal);

uint8_t eeprom_getErrorValue(void);
bool eeprom_setErrorValue(uint8_t errorVal);

uint32_t eeprom_getBatteryLowCounter(void);
void eeprom_setBatteryLowCounter(uint32_t batteryLowCounter);
void eeprom_incBatteryLowCounter(void);

BOOLEAN eeprom_getPairingState(void);
void eeprom_setPaired(void);
void eeprom_setUnPaired(void);

uint32_t eeprom_getAlertCounter();
void eeprom_setAlertCounter(uint32_t alertCounter);
void eeprom_incAlertCounter(void);

uint32_t eeprom_getLsiCalibration();
void eeprom_setLsiCalibration(uint32_t lsiCalibrationValue);

uint32_t eeprom_getNumberOfTransmissions868();
void eeprom_incNumberOfTransmissions868();
void eeprom_resetNumberOfTransmissions868();

uint32_t eeprom_getNumberOfTransmissionsBLE();
void eeprom_incNumberOfTransmissionsBLE();
void eeprom_resetNumberOfTransmissionsBLE();

uint8_t eeprom_getBatteryLowThresholdValue();

uint8_t eeprom_getRepetitionCountEmergency();
uint8_t eeprom_getRepetitionCountPairing();
uint8_t eeprom_getRepetitionCountBattery();

uint32_t eeprom_getWaitCycleCount();

//==========================================//
// Programming cycles accesses
//==========================================//

uint8_t         eeprom_getPCBVersion();
uint8_t         eeprom_getBOMVersion();
uint8_t         eeprom_setPCBVersion(uint8_t val);
uint8_t         eeprom_setBOMVersion(uint8_t val);
uint32_t        eeprom_getUID();
void            eeprom_setUID(uint32_t number);
uint32_t        eeprom_getSerialNumber();
void            eeprom_setSerialNumber(uint32_t number);
uint32_t        eeprom_getSoftwareVersion_Type();
uint32_t        eeprom_getSoftwareVersion_Status();
uint32_t        eeprom_getSoftwareVersion_Major();
uint32_t        eeprom_getSoftwareVersion_Minor();
uint32_t        eeprom_getSoftwareVersion_Build();
uint32_t        eeprom_getSoftwareVersion_CMI();

//==========================================//
// Flash / EEPROM CRC
//==========================================//

uint32_t eeprom_getFlashCRC();
bool eeprom_setFlashCRC(uint32_t value);
uint32_t eeprom_getEEPROMCRC();
bool eeprom_setEEPROMCRC(uint32_t value);

//==========================================//
// S2LP SYNTH
//==========================================//

uint8_t eeprom_getS2LP_Synth3();
uint8_t eeprom_getS2LP_Synth2();
uint8_t eeprom_getS2LP_Synth1();
uint8_t eeprom_getS2LP_Synth0();
bool eeprom_setS2LP_Synth_3_0(uint32_t value);

//==========================================//
// S2LP OUTPUTPOWER
//==========================================//

uint8_t eeprom_getS2LP_OutputPower8();
uint8_t eeprom_getS2LP_OutputPower7();
uint8_t eeprom_getS2LP_OutputPower6();
uint8_t eeprom_getS2LP_OutputPower5();
bool eeprom_setS2LP_OutputPower_8_5(uint32_t value);

uint8_t eeprom_getS2LP_OutputPower4();
uint8_t eeprom_getS2LP_OutputPower3();
uint8_t eeprom_getS2LP_OutputPower2();
uint8_t eeprom_getS2LP_OutputPower1();
bool eeprom_setS2LP_OutputPower_4_1(uint32_t value);

uint8_t eeprom_getS2LP_OutputPower0();
bool eeprom_setS2LP_OutputPower_0(uint32_t value);

//==========================================//
// S2LP CLOCKSOURCE SELECTOR
//==========================================//

uint8_t eeprom_getS2LP_ClocksourceSelector();
void eeprom_setS2LP_ClocksourceSelector(uint8_t val);

#endif