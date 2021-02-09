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

/* Includes */
#include "EEPROM_Map.h"
#include "EEPROM_Access.h"
#include "MasterDefine.h"
#include "Watchdog.h"

/* Typedefinitions / Prototypes */

/* Variables */

/* Function definitions */
//==========================================//
// Endianess & Access check
//==========================================//

uint8_t eeprom_getEACheckUint8_t(){
  return *((uint8_t*)eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_IDENT);
}

uint16_t eeprom_getEACheckUint16_t(){
  return *((uint16_t*)eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_IDENT);
}

uint32_t eeprom_getEACheckUint32_t(){
  return *((uint32_t*)eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_IDENT);
}

//==========================================//
// Battery values
//==========================================//

uint8_t eeprom_getBatteryValue(){
  return *((uint8_t*)eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_BATTERY);
}

bool eeprom_setBatteryValue(uint8_t batteryVal){
  return eeprom_writeByte_withValueCheck(batteryVal, EEPROM_MAP_OFFSET_BATTERY);
}

//==========================================//
// Alert values
//==========================================//

uint8_t eeprom_getAlertValue(){
  return *((uint8_t*)eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_ALERT);
}

bool eeprom_setAlertValue(uint8_t alertVal){
  return eeprom_writeByte_withValueCheck(alertVal, EEPROM_MAP_OFFSET_ALERT);
}

//==========================================//
// Error values
//==========================================//

uint8_t eeprom_getErrorValue(){
  return *((uint8_t*)eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_ERROR);
}

bool eeprom_setErrorValue(uint8_t errorVal){
  return eeprom_writeByte_withValueCheck(errorVal, EEPROM_MAP_OFFSET_ERROR);
}

//==========================================//
// Battery low counter
//==========================================//

uint32_t eeprom_getBatteryLowCounter(){
  return *((uint32_t*)(eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_BATTERYLOWCOUNTER));
}

void eeprom_setBatteryLowCounter(uint32_t batteryLowCounter){
  eeprom_writeWord_withValueCheck(batteryLowCounter, EEPROM_MAP_OFFSET_BATTERYLOWCOUNTER);
  return;
}

void eeprom_incBatteryLowCounter(void){
  uint32_t battLowCount = eeprom_getBatteryLowCounter() + 1;
  eeprom_writeWord_withValueCheck(battLowCount, EEPROM_MAP_OFFSET_BATTERYLOWCOUNTER);
  return;
}

//==========================================//
// Pairing state
//==========================================//

BOOLEAN eeprom_getPairingState(void){
  uint32_t addr = eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_PAIRING_STATE;
  uint32_t cmpVal = *((uint32_t*)(addr));
  if (cmpVal == 0xDEADBEEF){
    return FALSE;
  }
  return TRUE;
}

void eeprom_setPaired(void){
  eeprom_writeWord_withValueCheck(0xABBA1337, EEPROM_MAP_OFFSET_PAIRING_STATE);
  return;
}

void eeprom_setUnPaired(void){
  eeprom_writeWord_withValueCheck(0xDEADBEEF, EEPROM_MAP_OFFSET_PAIRING_STATE);
  return;
}

//==========================================//
// Alert counter
//==========================================//

uint32_t eeprom_getAlertCounter(){
  return *((uint32_t*)(eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_ALERTCOUNTER));
}

void eeprom_setAlertCounter(uint32_t alertCounter){
  eeprom_writeWord_withValueCheck(alertCounter, EEPROM_MAP_OFFSET_ALERTCOUNTER);
  return;
}

void eeprom_incAlertCounter(void){
  uint32_t alertCount = eeprom_getAlertCounter() + 1;
  eeprom_writeWord_withValueCheck(alertCount, EEPROM_MAP_OFFSET_ALERTCOUNTER);
  return;
}

//==========================================//
// LSI Calibration
//==========================================//

uint32_t eeprom_getLsiCalibration(){
  return *((uint32_t*)(eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_LSICALIBRATION));
}

void eeprom_setLsiCalibration(uint32_t lsiCalibrationValue){
  eeprom_writeWord_withValueCheck(lsiCalibrationValue, EEPROM_MAP_OFFSET_LSICALIBRATION);
  return;
}

//==========================================//
// NumberOfTransmissions
//==========================================//

uint32_t eeprom_getNumberOfTransmissions868(){
  return *((uint32_t*)(eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_NUMBEROFTRANSMISSIONS868));
}

void eeprom_incNumberOfTransmissions868(){
  uint32_t count = eeprom_getNumberOfTransmissions868();
  count++;
  eeprom_writeWord_withValueCheck(count, EEPROM_MAP_OFFSET_NUMBEROFTRANSMISSIONS868);
  return;
}

void eeprom_resetNumberOfTransmissions868(){
  eeprom_writeWord_withValueCheck(0, EEPROM_MAP_OFFSET_NUMBEROFTRANSMISSIONS868);
  return;
}

uint32_t eeprom_getNumberOfTransmissionsBLE(){
  return *((uint32_t*)(eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_NUMBEROFTRANSMISSIONSBLE));
}

void eeprom_incNumberOfTransmissionsBLE(){
  uint32_t count = eeprom_getNumberOfTransmissionsBLE();
  count++;
  eeprom_writeWord_withValueCheck(count, EEPROM_MAP_OFFSET_NUMBEROFTRANSMISSIONSBLE);
  return;
}

void eeprom_resetNumberOfTransmissionsBLE(){
  eeprom_writeWord_withValueCheck(0, EEPROM_MAP_OFFSET_NUMBEROFTRANSMISSIONSBLE);
  return;
}

//==========================================//
// SerialNumber & UID
//==========================================//

uint32_t eeprom_getUID(){
  return *((uint32_t*)(eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_U_ID));
}
  
void eeprom_setUID(uint32_t number){
  eeprom_writeWord_withValueCheck(number, EEPROM_MAP_OFFSET_U_ID);
}

uint32_t eeprom_getSerialNumber(){
  return *((uint32_t*)(eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_SERIALNUMBER));
}
  
void eeprom_setSerialNumber(uint32_t number){
  eeprom_writeWord_withValueCheck(number, EEPROM_MAP_OFFSET_SERIALNUMBER);
}

//==========================================//
// Battery Low Threshold Value
//==========================================//

uint8_t eeprom_getBatteryLowThresholdValue(){
  return *((uint8_t*)(eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_BATTERYLOWTHRESHOLDVALUE));
}

//==========================================//
// Repetition values
//==========================================//


uint8_t eeprom_getRepetitionCountEmergency(){
  return *((uint8_t*)(eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_RETRYCOUNTEMERGENCY));
}

uint8_t eeprom_getRepetitionCountPairing(){
  return *((uint8_t*)(eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_RETRYCOUNTPAIRING));
}

uint8_t eeprom_getRepetitionCountBattery(){
  return *((uint8_t*)(eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_RETRYCOUNTBATTERY));
}

//==========================================//
// Wait Cycle Options
//==========================================//

uint32_t eeprom_getWaitCycleCount(){
  return *((uint32_t*)(eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_NUMBEROFWAITCYCLES));
}

//==========================================//
// Programming cycles accesses
//==========================================//

uint8_t eeprom_getPCBVersion(){
  return *((uint8_t*)(eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_HARDWARE_PCB_VERSION));
}

uint8_t eeprom_getBOMVersion(){
  return *((uint8_t*)(eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_HARDWARE_BOM_VERSION));
}

void eeprom_setPCBVersion(uint8_t val){
  eeprom_writeByte_withValueCheck(val, EEPROM_MAP_OFFSET_HARDWARE_PCB_VERSION);
  return;
}

void eeprom_setBOMVersion(uint8_t val){
  eeprom_writeByte_withValueCheck(val, EEPROM_MAP_OFFSET_HARDWARE_BOM_VERSION);
  return;
}

uint32_t eeprom_getSoftwareVersion_Type(){
  return *((uint32_t*)(eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_SOFTWARE_VERSION));
}

uint32_t eeprom_getSoftwareVersion_Status(){
  return *((uint32_t*)(eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_SOFTWARE_STATUS));
}

uint32_t eeprom_getSoftwareVersion_Major(){
  return *((uint32_t*)(eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_SOFTWARE_MAJOR));
}

uint32_t eeprom_getSoftwareVersion_Minor(){
  return *((uint32_t*)(eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_SOFTWARE_MINOR));
}

uint32_t eeprom_getSoftwareVersion_Build(){
  return *((uint32_t*)(eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_SOFTWARE_BUILD));
}

uint32_t eeprom_getSoftwareVersion_CMI(){
  return *((uint32_t*)(eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_SOFTWARE_CMI));
}

//==========================================//
// Flash / EEPROM CRC
//==========================================//

uint32_t eeprom_getFlashCRC(){
  return *((uint32_t*)(eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_FLASH_CRC));
}

bool eeprom_setFlashCRC(uint32_t value){
  return eeprom_writeWord_withValueCheck(value, EEPROM_MAP_OFFSET_FLASH_CRC);
}

uint32_t eeprom_getEEPROMCRC(){
  return *((uint32_t*)(eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_EEPROM_CRC));
}

bool eeprom_setEEPROMCRC(uint32_t value){
  return eeprom_writeWord_withValueCheck(value, EEPROM_MAP_OFFSET_EEPROM_CRC);
}

//==========================================//
// S2LP SYNTH
//==========================================//

uint8_t eeprom_getS2LP_Synth3(){
  return *((uint8_t*)(eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_S2LP_SYNTH + 0));
}

uint8_t eeprom_getS2LP_Synth2(){
  return *((uint8_t*)(eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_S2LP_SYNTH + 1));
}

uint8_t eeprom_getS2LP_Synth1(){
  return *((uint8_t*)(eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_S2LP_SYNTH + 2));
}

uint8_t eeprom_getS2LP_Synth0(){
  return *((uint8_t*)(eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_S2LP_SYNTH + 3));
}

bool eeprom_setS2LP_Synth_3_0(uint32_t value){
  return eeprom_writeWord_withValueCheck(value, EEPROM_MAP_OFFSET_S2LP_SYNTH);
}

//==========================================//
// S2LP OUTPUTPOWER
//==========================================//

uint8_t eeprom_getS2LP_OutputPower8(){
  return *((uint8_t*)(eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_S2LP_OUTPUTPOWER_8_5 + 0));
}

uint8_t eeprom_getS2LP_OutputPower7(){
  return *((uint8_t*)(eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_S2LP_OUTPUTPOWER_8_5 + 1));
}

uint8_t eeprom_getS2LP_OutputPower6(){
  return *((uint8_t*)(eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_S2LP_OUTPUTPOWER_8_5 + 2));
}

uint8_t eeprom_getS2LP_OutputPower5(){
  return *((uint8_t*)(eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_S2LP_OUTPUTPOWER_8_5 + 3));
}

bool eeprom_setS2LP_OutputPower_8_5(uint32_t value){
  return eeprom_writeWord_withValueCheck(value, EEPROM_MAP_OFFSET_S2LP_OUTPUTPOWER_8_5);
}

//======

uint8_t eeprom_getS2LP_OutputPower4(){
  return *((uint8_t*)(eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_S2LP_OUTPUTPOWER_4_1 + 0));
}

uint8_t eeprom_getS2LP_OutputPower3(){
  return *((uint8_t*)(eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_S2LP_OUTPUTPOWER_4_1 + 1));
}

uint8_t eeprom_getS2LP_OutputPower2(){
  return *((uint8_t*)(eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_S2LP_OUTPUTPOWER_4_1 + 2));
}

uint8_t eeprom_getS2LP_OutputPower1(){
  return *((uint8_t*)(eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_S2LP_OUTPUTPOWER_4_1 + 3));
}

bool eeprom_setS2LP_OutputPower_4_1(uint32_t value){
  return eeprom_writeWord_withValueCheck(value, EEPROM_MAP_OFFSET_S2LP_OUTPUTPOWER_4_1);
}

//======

uint8_t eeprom_getS2LP_OutputPower0(){
  return *((uint8_t*)(eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_S2LP_OUTPUTPOWER_0 + 0));
}

bool eeprom_setS2LP_OutputPower_0(uint32_t value){
  return eeprom_writeWord_withValueCheck(value, EEPROM_MAP_OFFSET_S2LP_OUTPUTPOWER_0);
}

//==========================================//
// S2LP CLOCKSOURCE SELECTOR
//==========================================//

uint8_t eeprom_getS2LP_ClocksourceSelector(){
  return *((uint8_t*)(eepromMemoryMap_getEEPROMBaseAddress() + EEPROM_MAP_OFFSET_S2LP_CLOCKSOURCE_SELECTOR));
}

void eeprom_setS2LP_ClocksourceSelector(uint8_t val){
  eeprom_writeByte_withValueCheck(val, EEPROM_MAP_OFFSET_S2LP_CLOCKSOURCE_SELECTOR);
  return;
}

