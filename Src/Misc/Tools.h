/********************************************************************************
* Tools. header file                                                            *
*                                                                               *
*********************************************************************************/


#ifndef TXV2_TOOLS_H
#define TXV2_TOOLS_H


// $LastChangedDate: 2018-07-04 16:06:06 +0200 (Wed, 04 Jul 2018) $
// $LastChangedRevision: 1591 $
// $LastChangedBy: jpg1ch $
// $HeadURL: https://lcf-svn:8443/svn/Sources_HTS_NG/Firmware/trunk/Src/Tools.h $

#include "MasterDefine.h"

/****************************************************************************************/
/* ====> Functions prototypes                                                           */
/****************************************************************************************/

uint8_t posValHexToDez(uint8_t byte);
void ultoaf(u32 lo, char *s, u8 n);			  // This one returns fixed n number of digits
void ultoa(u32 lo, char *s);
unsigned long atoul(char *s, char n);                     // ASCII to unsigned long converter
void u82hex(u8 theByte, char *result);
u16 rnd();                                                // Generate a pseudo-random number
u16 rnd_bounded(u16 n);
u8 hex2u8(char* s);
void rssi2a(u8 rssi, char *s);
int u32toString(u32 val, char *string);
u32 Stringtou32(char* string);
void getCpuid(u32* here);
u32 getCpuidCrc(void);
u32 hash(u32 n);
u8 CompareUID(u32 uid32, u8* uid8);

#endif
