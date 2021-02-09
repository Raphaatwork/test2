/********************************************************************************
*                 Leesys - Producing care solutions                             *
* Module:               GlobalVariables.c                                       *
* Product:              TxV2                                                    *
* Development system:   IAR Embedded Workbench 8.42.2                           *
* Description:          File containing the global variables                    *
*                                                                               *
*********************************************************************************/

/********************************************************************************/
/* File Revision history                                                        */
/*                                                                              */
/* File Creation: 03-March-2020 / TS                                            */
/*                                                                              */
/********************************************************************************/

/* Includes ------------------------------------------------------------------*/

#include "stm32l0xx_hal.h"
#include "MasterDefine.h"

// CPU System / status ********************************************************
#if(DEBUG_NOSLEEP == P_OFF)
vu32 gl_systemStatus = 0;                                     // Live status of the system (Bitfield, see MasterDefine.h)
#else
vu32 gl_systemStatus = STAT_NOSTOP_DEBUG;                     // Set no stop mode in case of debug
#endif
//u32 gl_curentWkupPeriod = 50 * RTC_WUP_0S1;                   // Current CPU wake-up period (default is 5s)
//vu32 gl_LSIcalibr0S1 = RTC_WUP_0S1;                           // Corrected timer value for 0.1s wake-up (default value before calibration)
//u32 gl_eepromGpBase;                                          // Base address of 16 bytes general purpose EEPROM storage

// Battery Management ********************************************************
//uint16_t gl_vRefintFactory=0;   // Internal Vref (1.22V) factory calibration 10bits
//uint16_t gl_BatteryMeasure=0;   // Battery level measured [mV]
//uint16_t gl_BatteryLevel=0;     // Battery level transmitted during TX (0-100%)

// Inter-task communication ***************************************************
vu32   gl_ITSource = 0;                                        // Wake-up interrupt source       

