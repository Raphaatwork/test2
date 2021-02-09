/********************************************************************************
*                 TeleAlarm SA - Care solutions                                 *
* Module:               AllParams.c                                             *
* Product:              NurseCall                                               *
* Development system:   IAR Embedded Workbench 8.2                              *
* Description:          Parameters definitions                                  *
* Date:                 02.10.2018                                              *
*                                                                               *
*********************************************************************************/

#include "MasterDefine.h"
#include "AllParams.h"
#include "String.h"
#include "Tools.h"

//#pragma location = PARAM_FACTORY_ADDR

__root const u8 paramFactory[PARAM_SIZE] = {
  0x00, 0x00,                                      // Device Firmware update
  0x00, 0x00, 0x00, 0x00,                          // Unique ID
  0x00, 0x00, 0x00, 0x00,                          // Alternate ID
  0x00,                                            // Use Alternate ID
  0x00, 0x00, 0x00, 0x00,                          // Product type
  0x00, 0x00,                                      // HW revision
  0x00, 0x00,                                      // Frequency calibration
  0x14, 0x00,                                      // Supervision interval
  0x00,                                            // Supervision With ACK
  0x00,                                            // Plane Mode Arming
  0x00, 0x00,                                      // Dementia low limit
  0x00, 0x00,                                      // Dementia high limit
  0x01,                                            // LE80 mode
  0x10,                                            // Trigger 1 Red Button
  0x50,                                            // Trigger 2 Blue Button 
  0x60,                                            // Trigger 3 Blue+Red Buttons
  0x70,                                            // Trigger 4 Green Button
  0x21,                                            // Trigger 5 contact 1 - Closing
  0xFF,                                            // Trigger 6 contact 1 - Opening
  0x22,                                            // Trigger 7 contact 2 - Closing
  0xFF,                                            // Trigger 8 contact 2 - Opening
  0x23,                                            // Trigger 9 contact 3 - Closing
  0xFF,                                            // Trigger 10 contact 3 - Opening
  0x01,                                            // Trigger 11 contact 4 - Closing
  0xFF,                                            // Trigger 12 contact 4 - Opening
  0x64,                                            // Battery level
  0x30,                                            // Low Battery Counter
  0x00,                                            // Plane mode status
  0x2E,                                            // RSSI threshold
  0x0A,                                            // Low Battery Threshold
  0x00, 0x00,                                      // Daily
  0x00, 0x00, 0x00, 0x00,                          // PCBA Serial Number
  0x00,                                            // Multiple receivers
  0x01,                                            // Dementia activated
  0x00,                                            // Dementia with extended range
  0x01,                                            // Button active
  0x01,                                            // Button pressed with location
  0x00, 0x00,                                      // Counter Low Bat indicator
  0x01,                                            // Presence Mode selectable
  0x00,                                            // Assistance only if Presence
  0x00,                                            // Device Deactivation
  0x00,                                            // Internal Antenna Selected
  0x00, 0x00, 0x00, 0x00,                          // LE80-Record SLOT1
  0x00, 0x00, 0x00, 0x00,                          // LE80-Record SLOT2
  0x00, 0x00, 0x00, 0x00,                          // LE80-Record SLOT3
  0x00, 0x00, 0x00, 0x00,                          // LE80-Record SLOT4
  0x00, 0x00, 0x00, 0x00,                          // LE80-Record SLOT5
  0x00, 0x00, 0x00, 0x00,                          // LE80-Record SLOT6
  0x00, 0x00, 0x00, 0x00,                          // LE80-Record SLOT7
  0x00, 0x00, 0x00, 0x00,                          // LE80-Record SLOT8
  0x00, 0x00, 0x00, 0x00,                          // LE80-Record SLOT9
  0x00, 0x00, 0x00, 0x00,                          // LE80-Record SLOT10
  0x00, 0x00, 0x00, 0x00,                          // LE80-Record SLOT11
  0x00, 0x00, 0x00, 0x00,                          // LE80-Record SLOT12
  0x00, 0x00, 0x00, 0x00,                          // LE80-Record SLOT13
  0x00, 0x00, 0x00, 0x00,                          // LE80-Record SLOT14
  0x00, 0x00, 0x00, 0x00,                          // LE80-Record SLOT15
  0x00, 0x00, 0x00, 0x00,                          // LE80-Record SLOT16
  0x00, 0x00, 0x00, 0x00,                          // LE80-Record SLOT17
  0x00, 0x00, 0x00, 0x00,                          // LE80-Record SLOT18
  0x00, 0x00, 0x00, 0x00,                          // LE80-Record SLOT19
  0x00, 0x00, 0x00, 0x00,                          // LE80-Record SLOT20
  0x00,                                            // BADGE - With Alarm
  0x08,                                            // LED Brightness
  0x00, 0x00,                                      // Last Local Position
  0x00, 0x00,                                      // LE80-Record LOCAL POS
  0x00,                                            // Output Sequence
  0x00,                                            // Assistance after Clear Alarm
  0x00,                                            // Repetition Crit Technical Alarm
  0x00,                                            // Repetition Alarms Level 1
  0x00,                                            // Repetition Alarms Level 2
  0x00,                                            // Repetition Alarms Level 3
};
//-----------
//
// PARAM_NR868_FORMAT description of the PARAM_NR868[] fields 
//  u8 length                   // Parameter length
//  u8 pos                      // Position of payload in EEPROM from PARAM_LIVE_ADDR
//  u8 type                     // Parameter type
//  u16 lowerLimit              // Lower limit of the payload
//  u16 upperLimit              // Upper limit of the payload
//  u32 device                  // Bitfield of devices for which the parameter applies to 
//  u8 permissions              // Bitfield that defines the permissions related to the parameters

// WARNING: This table must be sorted as enumerated in "enum parameters" 
// WARNING: Array to be filled by data generated by Excel  
const PARAM_NR868_FORMAT PARAM_NR868[] =         // NR868 complete list of parameters
{
  {2,0,T_U16,0,2047,0x00000FFF,0x00},                       // Device Firmware update
  {4,2,T_UID,0,0,0x00000FFF,0x01},                          // Unique ID
  {4,6,T_UID,0,0,0x00000FFE,0x83},                          // Alternate ID
  {1,10,T_U8,0,1,0x00000FFE,0x83},                          // Use Alternate ID
  {4,11,T_U32,0,0,0x00000FFF,0x01},                         // Product type
  {2,15,T_U16,0,65535,0x00000FFF,0x01},                     // HW revision
  {2,17,T_U16,0,65535,0x00000FFE,0x00},                     // Frequency calibration
  {2,19,T_U16,0,2880,0x0000007E,0x83},                      // Supervision interval
  {1,21,T_U8,0,1,0x0000007E,0x83},                          // Supervision With ACK
  {1,22,T_U8,0,1,0x0000018E,0x80},                          // Plane Mode Arming
  {2,23,T_U16,0,65535,0x00000088,0x80},                     // Dementia low limit
  {2,25,T_U16,0,65535,0x00000088,0x80},                     // Dementia high limit
  {1,27,T_U8,1,12,0x00000200,0x83},                         // LE80 mode
  {1,28,T_U8,0,255,0x0000001E,0x83},                        // Trigger 1 Red Button
  {1,29,T_U8,0,255,0x00000010,0x83},                        // Trigger 2 Blue Button 
  {1,30,T_U8,0,255,0x00000010,0x83},                        // Trigger 3 Blue+Red Buttons
  {1,31,T_U8,0,255,0x00000010,0x83},                        // Trigger 4 Green Button
  {1,32,T_U8,0,255,0x00000070,0x83},                        // Trigger 5 contact 1 - Closing
  {1,33,T_U8,0,255,0x00000070,0x83},                        // Trigger 6 contact 1 - Opening
  {1,34,T_U8,0,255,0x00000070,0x83},                        // Trigger 7 contact 2 - Closing
  {1,35,T_U8,0,255,0x00000070,0x83},                        // Trigger 8 contact 2 - Opening
  {1,36,T_U8,0,255,0x00000030,0x83},                        // Trigger 9 contact 3 - Closing
  {1,37,T_U8,0,255,0x00000030,0x83},                        // Trigger 10 contact 3 - Opening
  {1,38,T_U8,0,255,0x00000020,0x83},                        // Trigger 11 contact 4 - Closing
  {1,39,T_U8,0,255,0x00000020,0x83},                        // Trigger 12 contact 4 - Opening
  {1,40,T_U8,0,228,0x000001FE,0x00},                        // Battery level
  {1,41,T_U8,0,255,0x000001FE,0x80},                        // Low Battery Counter
  {1,42,T_U8,0,255,0x0000018E,0x80},                        // Plane mode status
  {1,43,T_U8,0,255,0x00000FFF,0x00},                        // RSSI threshold
  {1,44,T_U8,0,255,0x000001FE,0x80},                        // Low Battery Threshold
  {2,45,T_U16,0,65535,0x00000000,0x40},                     // Daily
  {4,47,T_U32,0,0,0x00000FFF,0x00},                         // PCBA Serial Number
  {1,51,T_U8,0,2,0x000000FE,0x83},                          // Multiple receivers
  {1,52,T_U8,0,1,0x00000008,0x83},                          // Dementia activated
  {1,53,T_U8,0,1,0x00000008,0x83},                          // Dementia with extended range
  {1,54,T_U8,0,1,0x00000008,0x83},                          // Button active
  {1,55,T_U8,0,1,0x00000008,0x83},                          // Button pressed with location
  {2,56,T_U16,0,65535,0x000001FE,0x00},                     // Counter Low Bat indicator
  {1,58,T_U8,0,1,0x00000010,0x83},                          // Presence Mode selectable
  {1,59,T_U8,0,1,0x00000010,0x83},                          // Assistance only if Presence
  {1,60,T_U8,0,255,0x00000040,0x00},                        // Device Deactivation
  {1,61,T_U8,0,1,0x00000820,0x83},                          // Internal Antenna Selected
  {4,62,T_U32,0,0,0x00000200,0x83},                         // LE80-Record SLOT1
  {4,66,T_U32,0,0,0x00000200,0x83},                         // LE80-Record SLOT2
  {4,70,T_U32,0,0,0x00000200,0x83},                         // LE80-Record SLOT3
  {4,74,T_U32,0,0,0x00000200,0x83},                         // LE80-Record SLOT4
  {4,78,T_U32,0,0,0x00000200,0x83},                         // LE80-Record SLOT5
  {4,82,T_U32,0,0,0x00000200,0x83},                         // LE80-Record SLOT6
  {4,86,T_U32,0,0,0x00000200,0x83},                         // LE80-Record SLOT7
  {4,90,T_U32,0,0,0x00000200,0x83},                         // LE80-Record SLOT8
  {4,94,T_U32,0,0,0x00000200,0x83},                         // LE80-Record SLOT9
  {4,98,T_U32,0,0,0x00000200,0x83},                         // LE80-Record SLOT10
  {4,102,T_U32,0,0,0x00000200,0x83},                        // LE80-Record SLOT11
  {4,106,T_U32,0,0,0x00000200,0x83},                        // LE80-Record SLOT12
  {4,110,T_U32,0,0,0x00000200,0x83},                        // LE80-Record SLOT13
  {4,114,T_U32,0,0,0x00000200,0x83},                        // LE80-Record SLOT14
  {4,118,T_U32,0,0,0x00000200,0x83},                        // LE80-Record SLOT15
  {4,122,T_U32,0,0,0x00000200,0x83},                        // LE80-Record SLOT16
  {4,126,T_U32,0,0,0x00000200,0x83},                        // LE80-Record SLOT17
  {4,130,T_U32,0,0,0x00000200,0x83},                        // LE80-Record SLOT18
  {4,134,T_U32,0,0,0x00000200,0x83},                        // LE80-Record SLOT19
  {4,138,T_U32,0,0,0x00000200,0x83},                        // LE80-Record SLOT20
  {1,142,T_U8,0,1,0x00000180,0x83},                         // BADGE - With Alarm
  {1,143,T_U8,0,8,0x00000410,0x83},                         // LED Brightness
  {2,144,T_U16,0,65535,0x00000008,0x00},                    // Last Local Position
  {2,146,T_U16,0,65535,0x00000200,0x83},                    // LE80-Record LOCAL POS
  {1,148,T_U8,0,255,0x00000060,0x83},                       // Output Sequence
  {1,149,T_U8,0,1,0x0000006E,0x83},                         // Assistance after Clear Alarm
  {1,150,T_U8,0,1,0x0000003E,0x83},                         // Repetition Crit Technical Alarm
  {1,151,T_U8,0,1,0x0000003E,0x83},                         // Repetition Alarms Level 1
  {1,152,T_U8,0,1,0x0000003E,0x83},                         // Repetition Alarms Level 2
  {1,153,T_U8,0,1,0x0000003E,0x83},                         // Repetition Alarms Level 3
};                                    

/****************************************************************************************/
/* ====> GetParamSize                                                                   */
/*                                                                                      */
/*       Return the size of a paramter                                                  */
/*                                                                                      */
/*       INPUT:   u8 ID : ID of the parameter to get the site                           */
/*                                                                                      */
/*       OUTPUT: size of the value of the paramerter                                    */
/*                                                                                      */
/****************************************************************************************/
u8 GetParamSize(u8 ID)
{
  u8 retVal = 99;
  if (ID < PARAM_NUM)
  {
    return PARAM_NR868[ID].length;
  }
  return retVal;
}

