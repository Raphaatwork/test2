/**
  ******************************************************************************
  * @file       EEPROM_Map.c
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

/* Includes */
#include "stm32l0xx_hal.h"

/* Typedefinitions / Prototypes */

#define PAIRED 0

/* Variables */

// WARNING: TAKE NOTE THAT THE ERASING IS IN WORD MODE. YOU CAN NOT CHANGE THIS!
const uint8_t eepromMemoryMap[2048] @ 0x08080000 = {
  
  // STATIC AREA !!! DO NOT CHANGE EVEN WITH SOFTWARE UPDATE, JUST CHANGE IN PRODUCTION STEP !!!
  
  0x78, 0x56, 0x34, 0x12, // 0x08080000 - 0x08080003 Identifier EEPROM + Endianness-check (0x12345678)
//                    ^ First Byte (MSB)
//              ^ Second Byte (2nd SB)
//        ^ Third Byte (3rd SB)
//  ^ Fourth Byte (LSB)
//^====================^        uint32_t field area (if you access an uint32_t this is the area you access)
//^=========^                   uint16_t field area (if you access an uint16_t this is the area you access)
//^===^                         uint8_t  field area (if you access an uint8_t  this is the area you access)
  0x00, 0x00, 0x00, 0x00, // 0x08080004 - 0x08080007 Battery value
  0x00, 0x00, 0x00, 0x00, // 0x08080008 - 0x0808000B Alert value
  0x00, 0x00, 0x00, 0x00, // 0x0808000C - 0x0808000F Error value
  0x00, 0x00, 0x00, 0x00, // 0x08080000 - 0x08080013 Spacer A
  0x00, 0x00, 0x00, 0x00, // 0x08080014 - 0x08080017 Battery Low Counter
#if PAIRED >= 1
  0x37, 0x13, 0xBA, 0xAB, // 0x08080018 - 0x0808001B The one for being paired
#else
  0xEF, 0xBE, 0xAD, 0xDE, // 0x08080018 - 0x0808001B Pairing state (not once paired = 0xDEADBEEF, paired at least once 0xABBA1337
#endif
  0x00, 0x00, 0x00, 0x00, // 0x0808001C - 0x0808001F Alert Counter
  0xE7, 0x00, 0x00, 0x00, // 0x08080020 - 0x08080023 LSI Calibration, INITIAL VALUE 231 (0xE7)
  0x62, 0x2C, 0x4B, 0xCB, // 0x08080024 - 0x08080027 S2LP SYNTH    3-2-1-0
  0x05, 0x00, 0x00, 0x00, // 0x08080028 - 0x0808002B S2LP PA_Power 8-7-6-5
  0x00, 0x00, 0x00, 0x00, // 0x0808002C - 0x0808002F S2LP PA_Power 4-3-2-1
  0x00, 0x00, 0x00, 0x00, // 0x08080030 - 0x08080033 S2LP PA_Power 0
  0x01, 0x00, 0x00, 0x00, // 0x08080034 - 0x08080037 S2LP CLOCKSOURCE SELECTOR
  0x14, 0x00, 0x00, 0x00, // 0x08080038 - 0x0808003B Battery Low Threshold Value
  0x00, 0x00, 0x00, 0x00, // 0x0808003C - 0x0808003F Spacer G
  0x03, 0x00, 0x00, 0x00, // 0x08080040 - 0x08080043 Count of transmission for emergency
  0x01, 0x00, 0x00, 0x00, // 0x08080044 - 0x08080047 Count of transmission for pairing
  0x01, 0x00, 0x00, 0x00, // 0x08080048 - 0x0808004B Count of transmission for battery
  0x00, 0x00, 0x00, 0x00, // 0x0808004C - 0x0808004F NumberOfTransmissions868
  0x00, 0x00, 0x00, 0x00, // 0x08080050 - 0x08080053 NumberOfTransmissionsBLE
  0xB4, 0x00, 0x00, 0x00, // 0x08080054 - 0x08080057 NumberOfWaitingsTillBatterySend
  0x00, 0x00, 0x00, 0x00, // 0x08080058 - 0x0808005B Spacer H
  0x00, 0x00, 0x00, 0x00, // 0x0808005C - 0x0808005F Spacer I
  0x00, 0x00, 0x00, 0x00, // 0x08080060 - 0x08080063 Spacer J
  0x00, 0x00, 0x00, 0x00, // 0x08080064 - 0x08080067 Spacer K
  0x00, 0x00, 0x00, 0x00, // 0x08080068 - 0x0808006B Spacer L
  0x00, 0x00, 0x00, 0x00, // 0x0808006C - 0x0808006F Spacer M
  0x00, 0x00, 0x00, 0x00, // 0x08080070 - 0x08080073 Spacer N
  0x00, 0x00, 0x00, 0x00, // 0x08080074 - 0x08080077 Spacer O
  0x00, 0x00, 0x00, 0x00, // 0x08080078 - 0x0808007B Spacer P
  0x00, 0x00, 0x00, 0x00, // 0x0808007C - 0x0808007F Spacer Q
  
  // DYNAMIC AREA FOR SOFTWARE UPDATES
  
  0x01, 0x00, 0x00, 0x00, // 0x08080080 - 0x08080083 Software Version Type
  0x42, 0x00, 0x00, 0x00, // 0x08080084 - 0x08080087 Software Status Type
  0x03, 0x00, 0x00, 0x00, // 0x08080088 - 0x0808008B Software Major Version
  0x04, 0x00, 0x00, 0x00, // 0x0808008C - 0x0808008F Software Minor Version
  0x05, 0x00, 0x00, 0x00, // 0x08080090 - 0x08080093 Software Build Number
  0x06, 0x00, 0x00, 0x00, // 0x08080094 - 0x08080097 Software CMI
  0x00, 0x00, 0x00, 0x00, // 0x08080098 - 0x0808009B SerialNumber
  0x00, 0x00, 0x00, 0x00, // 0x0808009C - 0x0808009F Hardware PCB Version
  0x00, 0x00, 0x00, 0x00, // 0x080800A0 - 0x080800A3 Hardware BOM Version
  0x03, 0xE0, 0x12, 0xFF, // 0x080800A4 - 0x080800A7 U_ID uint8_t hex digits
  0x00, 0x00, 0x00, 0x00, // 0x080800A8 - 0x080800AB Spacer 1
  0x00, 0x00, 0x00, 0x00, // 0x080800AC - 0x080800AF Spacer 2
  0x00, 0x00, 0x00, 0x00, // 0x080800B0 - 0x080800B3 Spacer 3
  0x00, 0x00, 0x00, 0x00, // 0x080800B4 - 0x080800B7 Spacer 4
  0x00, 0x00, 0x00, 0x00, // 0x080800B8 - 0x080800BB Spacer 5
  0x00, 0x00, 0x00, 0x00, // 0x080800BC - 0x080800BF Spacer 6
  0x00, 0x00, 0x00, 0x00, // 0x080800C0 - 0x080800C3 Spacer 7
  0x00, 0x00, 0x00, 0x00, // 0x080800C4 - 0x080800C7 Spacer 8
  0x00, 0x00, 0x00, 0x00, // 0x080800C8 - 0x080800CB Spacer 9
  0x00, 0x00, 0x00, 0x00, // 0x080800CB - 0x080800CF Spacer 10
  0x00, 0x00, 0x00, 0x00, // 0x080800D0 - 0x080800D3 Spacer 11
  0x00, 0x00, 0x00, 0x00, // 0x080800D4 - 0x080800D7 Spacer 12
  0x00, 0x00, 0x00, 0x00, // 0x080800D8 - 0x080800DB Spacer 13
  0x00, 0x00, 0x00, 0x00, // 0x080800DC - 0x080800DF Spacer 14
  0x00, 0x00, 0x00, 0x00, // 0x080800E0 - 0x080800E3 Spacer 15
  0x00, 0x00, 0x00, 0x00, // 0x080800E4 - 0x080800E7 Spacer 16
  0x00, 0x00, 0x00, 0x00, // 0x080800E8 - 0x080800EB Spacer 17
  0x00, 0x00, 0x00, 0x00, // 0x080800EC - 0x080800EF Spacer 18
  0x00, 0x00, 0x00, 0x00, // 0x080800F0 - 0x080800F3 Spacer 19
  0x00, 0x00, 0x00, 0x00, // 0x080800F4 - 0x080800F7 Spacer 20
  0x00, 0x00, 0x00, 0x00, // 0x080800F8 - 0x080800FB Spacer 21
  0x00, 0x00, 0x00, 0x00, // 0x080800FC - 0x080800FF Spacer 22
  
  // END OF USED EEPROM SPACE
  
  0x00, 0x00, 0x00, 0x00, // 0x08080100 - 0x08080103 FLASH CRC <<write once for every software version>>
  0xA7, 0xCF, 0x76, 0xBE, // 0x08080104 - 0x08080107 EEPROM CRC 
};

/* Function definitions */
uint32_t eepromMemoryMap_getEEPROMBaseAddress(void){
  return (uint32_t) eepromMemoryMap;
}