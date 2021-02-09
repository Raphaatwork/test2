
/********************************************************************************
* Global definitions                                                 08-12-2017 *
*********************************************************************************/ 

#ifndef __MASTERDEFINE_H__
#define __MASTERDEFINE_H__

#include "stdint.h"
#include "stm32l0xx.h"

// Projects identifier. These project identifiers must be used in common code, for project dependant conditional compilation 
#define   PROJ_L051TX     1             // Project includes: S87, S85, S87L, B80A, B81, RAC80, UPCBA

// --- Transmitters ----

#define SAFE_FW_UPDATE
// Devices --------------------------------------------------------------------------------------------------------------------

#define   DEV_S87       0x00000004

enum devices {PR80,S85,S87,S87L,N86,UPCBA,RAC80,B80A,B81,LE80,AP80,NC8};
//#define NUM_OF_PRODUCT NC8+1;      // Number of product = last device + 1

// --> Watch Dog activation --------------------------------------------------------------
//#define WDOG_ACTIVATED

// --> Flash memory ----------------------------------------------------------------------
#define LOWER   0       // Lower half of Flash memory 
#define UPPER   1       // Upper half of Flash memory 

// Locations of FW info according to Projects
// FW Binary shall use at maximum half of the available flash size, to allow FW update
// Last 3 words of this half are reserved for CRC and FW version information

#define FW_VERSION_LOCATION     0x08007FF4
#define FW_BUILD_LOCATION       0x08007FF8
#define FW_CRC32_LOCATION       0x08007FFC


// --> Radio Frequencies ------------------------------------------------------------------------------------------------------
// To avoid conflicts, let's define different frequencies per developpers
#define FREQ_PROD           0                 // Normal setting 868.8
#define FREQ_LAURENT        1                 // 868.2
#define FREQ_SIMON          2                 // 868.0
#define FREQ_DANIEL         3                 // 868.5
#define FREQ_PROX           4                 // 869.85
#define FREQ                FREQ_DANIEL


/* --------------------------------------------------------------------------- Normally OFF defines - */

#define DEBUG_L051_SHELL_TIMER    P_ON        // Authorize shell timer command. Placed on "ON", this is usable for Test Systems
                                              // This #Define placed on "OFF" allows to save around 600 bytes of Flash memory!

#define DEBUG_NOSLEEP             P_OFF
#define DEBUG_100KBS              P_OFF       // 100kbps data rate test

#define DEBUG_POWER_MINUS_30DB    P_OFF       // Reduces transmission power by 30dB
#define DEBUG_L051_SHELL_PARAM    P_ON        // Authorize shell param command for L051 devices.
#define HAL_BASED_USART2_INIT     P_OFF       // Use HAL library for USART2 handling. If P_OFF, handling is done through registers
#define HAL_BASED_ADC_INIT        P_OFF       // Use HAL library for ADC handling. If P_OFF, handling is done through registers
#define HAL_BASED_SPI1_INIT       P_OFF       // Use HAL library for SPI handling. If P_OFF, handling is done through registers and simplified library (L051TX only)

#define EMULATOR                  P_OFF       // Only for Emulator debugging, to avoid "sleep mode" with emulator (not compatible)!

/* --> Types aliases ---------------------------------------------------------------------------------------------------------*/
typedef uint8_t             u8;
typedef uint16_t            u16;
typedef uint32_t            u32;
typedef int8_t              s8;
typedef int16_t             s16;
typedef int32_t             s32;
typedef volatile uint8_t    vu8;
typedef volatile uint16_t   vu16;
typedef volatile uint32_t   vu32;
typedef enum {FALSE = 0, TRUE = !FALSE, OFF = 0, ON = !OFF, PRESSED = 1, RELEASED = !PRESSED} bool;
typedef unsigned char UCHAR, BYTE, STATUS, BOOLEAN, NIBBLE;

#define P_ON    1             // Need a special ON/OFF for compiler directive because normal ON/OFF defines are
#define P_OFF   0             // based on enum type and not accepted in a compiler directive.

#define TX_MODE 0
#define RX_MODE 1

/* --> Basic defines ---------------------------------------------------------------------------------------- */
#define OK      TRUE
#define KO      FALSE
#define SOH     0x01
#define EOT     0x04
#define ACK     0x06
#define NAK     0x15

/* --> Battery management ----------------------------------------------------------------------------------- */
#define VREF 3000                   // VDDA REF for Internal Vref calibration [mV]
#define CR2_0PERCENT_V      2300    // Voltage in mV for 0%
#define CR2_25PERCENT_V     2600    // Voltage in mV for 25% (and "inflection point")
#define CR2_100PERCENT_V    2900    // Voltage in mV for 100%

// CR2430 (and CR2025 for S87L)
#define CR2430_0PERCENT_V   1800 //1800    // Voltage in mV for 0%
#define CR2430_10PERCENT_V  1900 //1900    // Voltage in mV for 10%
#define CR2430_100PERCENT_V 2800 //2800    // Voltage in mV for 100%
#if ((CR2430_0PERCENT_V != 1800) || (CR2430_10PERCENT_V != 1900) ||(CR2430_100PERCENT_V != 2800))
#warning "THESE VALUES WERE CHANGED AND THEY ARE NOT EQUIVALENT TO THE TELEALARM VALUE!"
#endif
// --> Parameters -------------------------------------------------------------------------------------------------------------
// --- Parameters types ---
#define T_UID   0       
#define T_U32   1
#define T_REV   2       // not used yet
#define T_U8    3
#define T_U16   4

// --- Parameters location ---
#define PARAM_NUM               72       // Number of parameters (see enum above)
#define PARAM_SIZE              154      // Size of Factory parameters

// Warning: If PARAM_SIZE > 255, Byte index type in "EepromParamInit()" must be u16
#define PARAM_LIVE_ADDR         DATA_EEPROM_BASE

// --- Parameters format ---
// The following structure defines the format of each parameters  
typedef struct paramNr868Format // Model of structure to define parameters format (std + ext)
{
  u8 length;                    // Parameter length
  u8 pos;                       // Position of payload in EEPROM from PARAM_LIVE_ADDR
  u8 type;                      // Parameter type
  u16 lowerLimit;               // Lower limit of the payload
  u16 upperLimit;               // Upper limit of the payload
  u32 device;                   // Bitfield of devices for which the parameter applies to 
  u8 permissions;               // Bitfield that defines the permissions related to the parameters
} PARAM_NR868_FORMAT;

// --- Clock Source Selector Typedef XTAL/TCXO ---
typedef enum CLOCKTYPE_SELECTOR// Model of structure to define parameters format (std + ext)
{
  CLOCKTYPE_SELECTOR_NONE = 0,
  CLOCKTYPE_SELECTOR_TCXO = 1,
  CLOCKTYPE_SELECTOR_XTAL = 2,
} CLOCKTYPE_SELECTOR_TYPEDEF;

// --- Parameter IDs ---
// WARNING: This must be enumerated as sorted in table PARAM_NR868[]

enum parameters
{  
  DFU,                           // ParamID   0    Device Firmware update
  U_ID,                          // ParamID   1    Unique ID
  A_ID,                          // ParamID   2    Alternate ID
  USE_A_ID,                      // ParamID   3    Use Alternate ID
  PRODUCT_TYPE,                  // ParamID   4    Product type
  HW_REV,                        // ParamID   5    HW revision
  FREQ_CAL,                      // ParamID   6    Frequency calibration
  SUP_INT,                       // ParamID   7    Supervision interval
  SUP_ACK,                       // ParamID   8    Supervision With ACK
  ARMING_PLANE_MODE,             // ParamID   9    Plane Mode Arming
  DEM_LOW,                       // ParamID   10   Dementia low limit
  DEM_HIGH,                      // ParamID   11   Dementia high limit
  LE80_MODE,                     // ParamID   12   LE80 mode
  TRIG_1,                        // ParamID   13   Trigger 1 Red Button
  TRIG_2,                        // ParamID   14   Trigger 2 Blue Button 
  TRIG_3,                        // ParamID   15   Trigger 3 Blue+Red Buttons
  TRIG_4,                        // ParamID   16   Trigger 4 Green Button
  TRIG_5,                        // ParamID   17   Trigger 5 contact 1 - Closing
  TRIG_6,                        // ParamID   18   Trigger 6 contact 1 - Opening
  TRIG_7,                        // ParamID   19   Trigger 7 contact 2 - Closing
  TRIG_8,                        // ParamID   20   Trigger 8 contact 2 - Opening
  TRIG_9,                        // ParamID   21   Trigger 9 contact 3 - Closing
  TRIG_10,                       // ParamID   22   Trigger 10 contact 3 - Opening
  TRIG_11,                       // ParamID   23   Trigger 11 contact 4 - Closing
  TRIG_12,                       // ParamID   24   Trigger 12 contact 4 - Opening
  BATT_LEV,                      // ParamID   25   Battery level
  LOW_BATT_CNT,                  // ParamID   26   Low Battery Counter
  PLANE_MODE_STATUS,             // ParamID   27   Plane mode status
  RSSI_THRESHOLD,                // ParamID   28   RSSI threshold
  LOW_BATT_THRESHOLD,            // ParamID   29   Low Battery Threshold
  DAILY,                         // ParamID   30   Daily
  PCBA_SN,                       // ParamID   31   PCBA Serial Number
  MULT_RECEIVERS,                // ParamID   32   Multiple receivers
  DEMENTIA,                      // ParamID   33   Dementia activated
  DEMENTIA_EXTENDED,             // ParamID   34   Dementia with extended range
  BUTTON_ACTIVE,                 // ParamID   35   Button active
  BUTTON_PRESSED_LOCATION,       // ParamID   36   Button pressed with location
  COUNTER_LOWBAT_INDIC,          // ParamID   37   Counter Low Bat indicator
  PRESENCE_SELECTABLE,           // ParamID   38   Presence Mode selectable
  ASSIST_ONLY_IF_PRESENCE,       // ParamID   39   Assistance only if Presence
  DEVICE_DEACTIVATION,           // ParamID   40   Device Deactivation
  INTERNAL_ANTENNA,              // ParamID   41   Internal Antenna Selected
  LE80_REC_SLOT1,                // ParamID   42   LE80-Record SLOT1
  LE80_REC_SLOT2,                // ParamID   43   LE80-Record SLOT2
  LE80_REC_SLOT3,                // ParamID   44   LE80-Record SLOT3
  LE80_REC_SLOT4,                // ParamID   45   LE80-Record SLOT4
  LE80_REC_SLOT5,                // ParamID   46   LE80-Record SLOT5
  LE80_REC_SLOT6,                // ParamID   47   LE80-Record SLOT6
  LE80_REC_SLOT7,                // ParamID   48   LE80-Record SLOT7
  LE80_REC_SLOT8,                // ParamID   49   LE80-Record SLOT8
  LE80_REC_SLOT9,                // ParamID   50   LE80-Record SLOT9
  LE80_REC_SLOT10,               // ParamID   51   LE80-Record SLOT10
  LE80_REC_SLOT11,               // ParamID   52   LE80-Record SLOT11
  LE80_REC_SLOT12,               // ParamID   53   LE80-Record SLOT12
  LE80_REC_SLOT13,               // ParamID   54   LE80-Record SLOT13
  LE80_REC_SLOT14,               // ParamID   55   LE80-Record SLOT14
  LE80_REC_SLOT15,               // ParamID   56   LE80-Record SLOT15
  LE80_REC_SLOT16,               // ParamID   57   LE80-Record SLOT16
  LE80_REC_SLOT17,               // ParamID   58   LE80-Record SLOT17
  LE80_REC_SLOT18,               // ParamID   59   LE80-Record SLOT18
  LE80_REC_SLOT19,               // ParamID   60   LE80-Record SLOT19
  LE80_REC_SLOT20,               // ParamID   61   LE80-Record SLOT20
  BADGE_WITH_ALARM,              // ParamID   62   BADGE - With Alarm
  LED_BRIGHTNESS,                // ParamID   63   LED Brightness
  LAST_LOCAL_POSITION,           // ParamID   64   Last Local Position
  LE80_REC_LOCAL_POSITION,       // ParamID   65   LE80-Record LOCAL POS
  OUTPUT_SEQUENCE,               // ParamID   66   Output Sequence
  ASSIST_AFTER_CLEAR,            // ParamID   67   Assistance after Clear Alarm
  REPETITION_CRIT_TECH_ALARM,    // ParamID   68   Repetition Crit Technical Alarm
  REPETITION_ALARMS_LEV1,        // ParamID   69   Repetition Alarms Level 1
  REPETITION_ALARMS_LEV2,        // ParamID   70   Repetition Alarms Level 2
  REPETITION_ALARMS_LEV3,        // ParamID   71   Repetition Alarms Level 3
};

// --- Param permissions ---
#define NONE            0x00    // User has no access to this parameter
#define READ            0x01    // User has read access to this parameter
#define WRITE           0x02    // User has write access to this parameter
#define FACTORY         0x80    // Parameter can be reset to Factory setting

// --- Param DFU ---
#define DFU_FW_UPD      1       // ON: 1st Reboot from a FW update 
#define DFU_KEEP_PARAM  2       // If set, keep parameters after a FW update 

#define DFU_FW_UPD_2    4       // ON: 2nd Reboot from a FW update
// Others Reboots:  Both Bits DFU_FW_UPD and DFU_FW_UPD_2 are reset
//#endif
/* --- SPI2 Command Macro ----------------------------------------------------------------------------------------------------*/

#define COMMAND_TX ((uint8_t) (0x60))           // Start Transmit! Valid only from READY
#define COMMAND_RX ((uint8_t) (0x61))           // Start Receive!  Valid only from READY
#define COMMAND_READY ((uint8_t) (0x62))        // Go to READY!    Valid only from STB, SLEEP or LOCK
#define COMMAND_STANDBY ((uint8_t) (0x63))      // Go to STANDBY(STB)! Only from READY
#define COMMAND_SLEEP ((uint8_t) (0x64))        // Go to SLEEP! Only from READY  
#define COMMAND_LOCKRX ((uint8_t) (0x65))       // Go to LOCK using RX Synth config! Valid from READY
#define COMMAND_LOCKTX ((uint8_t) (0x66))       // Go to LOCK using TX Synth config! Valid from READY
#define COMMAND_SABORT ((uint8_t) (0x67))       // Force Exit RX or TX state and go to READY! Only from RX or TX
#define COMMAND_SRES ((uint8_t) (0x70))         // Reset all digital part, except SPI registers.
#define COMMAND_FLUSHRXFIFO ((uint8_t) (0x71))  // Clean RX FIFO! Valid from all states 
#define COMMAND_FLUSHTXFIFO ((uint8_t) (0x72))  // Clean TX FIFO! Valid from all states

/* --> USART ------------------------------------------------------------------------------------------------ */
#define RXBUFSIZE 50                            // Standard size for most products
#define c_CR      0x0d
#define c_LF      0x0a
#define TXRESPSIZE 50
#define WELCOME1 "* NC RF DEVICE *\r\n"

#define SHELLCONFIRM_TIMEOUT      5000              // Time, from shell activation, after which system returns in stop mode in case of no command receive

/* --> Backbone  delegated tasks bits defines --------------------------------------------------------------- */
// Bit defines for gl_doEvent

#define DOEVENT_SHELL_HELP         0x00000001
#define DOEVENT_STOP               0x00000002
#define DOEVENT_RECV_MODE          0x00000004
#define DOEVENT_GET_STANDBY_RSSI   0x00000008
#define DOEVENT_CARRIER_ON         0x00000010
#define DOEVENT_EEPROM_WRITE       0x00000020                  
#define DOEVENT_SEND               0x00000040                  
#define DOEVENT_CYCLE_TEST         0x00000080  
#define DOEVENT_FACTORY_WRITE      0x00000100
#define DOEVENT_FW_START           0x00000200     
#define DOEVENT_FW_BLOC            0x00000400     
#define DOEVENT_FW_END             0x00000800 
#define DOEVENT_SERVER_MODE        0x00001000
#define DOEVENT_FW_PR80            0x00002000 
#define DOEVENT_FW_DEV             0x00004000 
#define DOEVENT_PR80_READ          0x00008000 
#define DOEVENT_PR80_WRITE         0x00010000 
#define DOEVENT_PR80_GETINFO       0x00020000
#define DOEVENT_PR80AUTH           0x00040000
#define DOEVENT_SEND_WITHACK       0x00080000
#define DOEVENT_PR80_GETINFO_ADDR  0x00100000


/* --> Wake-up interrupt sources defines -------------------------------------------------------------------- */
#define WKP_IT_PROXIMITY          0x01
#define WKP_IT_SHELL              0x02
#define WKP_IT_RTC                0x04
#define WKP_IT_INPUT1             0x08
#define WKP_IT_INPUT2             0x10
#define WKP_IT_INPUT3             0x20
#define WKP_IT_INPUT_TERMINAL     0x40
#define WKP_IT_RAC_BUTTON         0x80
#define WKP_IT_RSL10              0x100
#define TIMER_AWAKE               1200                                            // 500 ms to stay awake

/* --> Wake-up Interval define ------------------------------------------------------------------------------ */
#define LSI_CALIBR_PERIOD         720                     // LSI calibration period in unit of 5s
#define RTC_LSI_CLK               37000
#define RTC_WUP_0S1               231                     // 0.1s @FreLSI = 37kHz
#define RTC_WUP_INTERVAL          50 * RTC_WUP_0S1        // 5s
/* --> Watch-Dog TimeOut define ----------------------------------------------------------------------------- */
#define WATCHDOG_PRESCALER        IWDG_PRESCALER_128 // IWDG_PRESCALER_256=>~25sec  128=>12.5sec  64=>6sec  32=> 3sec... until 4=>~0.4sec

/* --> Timing defines --------------------------------------------------------------------------------------- */
#define DAILY_MESSAGE_INTERVAL    2520               // The Daily message interval is fixed to 21 hours (1 unit = 30sec).
#define DELAY_TRM_BUTTON_TRIG     700                // Delay in ms to LED "ON" before the transmission, after button pressed
#define DLY_FLASHS_LOWBAT_INDIC   3                  // Delay between indications when low bat 3 * 5sec = 15 sec
#define DLY_LONG_FLASHS_LB_INDIC  6                  // Long delay between indications when low bat = 30 seconds
#define TIME_BEFORE_LONG_LB_INDIC 17280              // Time before to pass from standard delay to long delay, Low Bat indicator = 3 days 
#define LOCAL_DECODING_TIMEOUT    4000               // Global timeout for the decoding after INT LOC Interrupt (in millisec). (S87L)
#define LOCAL_REDECOD_TOUT_FIELD  200                // Timeout re-decoding every RTC period (5 sec), when staying in the Field (in millisec). (S87L)
#define LOCAL_MAX_REDECOD_FIELD   3                  // Number of times max, re-decoding when staying in the Field (S87L)

/* --> Various defines --------------------------------------------------------------------------------------- */
#define NUMBER_REPETITIONS        3
#define REPETITION_INTERVAL       5                  // in minutes
#define NBR_MAX_TIMERS_SHELL_MGT  5                  // Numbers of timers which can be managed by the Shell
#define RSSI_NC4_LEV1             42                 // Corresponds to 42-146 = -104dBm 
#define RSSI_NC4_LEV2             57                 // Corresponds to 57-146 = -89dBm 
#define RSSI_NC4_LEV3             72                 // Corresponds to 72-146 = -74dBm 
#define TIMER2_ENDED              TIM2->SR & TIM_SR_UIF
#define PLANE_MODE_CUSTOMER_CNT   40                 // Number of hours set when "Arming Plane Mode" is progrmmed by the PR80 (max 63)
#define LOCAL_DECOD_SAME_POS      10                 // Number of times we have to catch consecutively the same Position to valid it (S87L)

/* --> System status (gl_systemStatus) ---------------------------------------------------------------------- */
#define STAT_NOSTOP_DEBUG         0x00000001        // System will not go in stop mode because of debug mode
#define STAT_NOSTOP_5S            0x00000002        // System will not go in stop mode for 5s after shell wake-up
#define STAT_NOSTOP_SHELL         0x00000004        // System will not go in stop mode because shell was used
#define STAT_PARAMS_UPDATED       0x00000008        // Parameters were just updated by NC configuration manager
#define STAT_FREE04               0x00000010        // FREE
#define STAT_SEND_MODE            0x00000020        // S2LP is currently in sending mode
#define STAT_FREE06               0x00000040        // FREE
#define STAT_FREE07               0x00000080        // FREE
#define STAT_SU_MODE              0x00000100        // Operating mode and parameter access mode: 0:User mode / 1: SuperUser mode
#define STAT_DATAGRAM_APP         0x00000200        // Datagram received and ACKed, application level
#define STAT_PROD_MODE            0x00000400        // Production mode (TEST_MODE_Pin low @ boot-up)
#define STAT_FREE11               0x00000800        // FREE
#define STAT_PROX_SESSION         0x00001000        // System is on a proxi session
#define STAT_NURSE_ACK_PENDING    0x00002000        // System needs to send a "clear alarm" message
#define STAT_CONTRX               0x00004000        // Continuous RX mode
#define STAT_NURSE_PROXI          0x00008000        // Nurse proxi phase
#define STAT_FREE16               0x00010000        // FREE
#define STAT_AP80_LINKDOWN        0x00020000        // RS Link is down
#define STAT_FREE_18              0x00040000        // FREE
#define STAT_FREE_19              0x00080000        // FREE
#define STAT_FWUPDATE_1ST_REBOOT  0x00100000        // In 1st reboot session
#define STAT_INFINITE_LOOP_RUNS   0x00200000        // Indicating that the Infinite loop is running
#define STAT_LE80_STANDBY_MODE    0x00400000        // LE80 in Standby Mode (Not programming or Sequence running) 
#define STAT_LE80_PROG_MODE       0x00800000        // LE80 in Programming Mode
#define STAT_WAKEUP_RIGHTNOW      0x01000000        // Ask for an immediate wake-up
#define STAT_ON_BAT               0x02000000        // N86 is powered by battery
#define STAT_LE80_RUNNING_MODE    0x04000000        // LE80 in Running Mode (Sequence played)



/* --> Type of event that caused the startup ---------------------------------------------------------------- */
#define STARTUP_BUTTON            1                 // Through MCLR pin (RC)
#define STARTUP_BATTERY           2                 // Power ON by Battery introduction 
#define STARTUP_FIRMWARE          3                 // Firmware Reset
#define STARTUP_WATCHDOG          4                 // Watch-Dog Reset!
#define STARTUP_FW_UPGRADE        5                 // Due to firmware upgrade

/* --> Pins definition by Product --------------------------------------------------------------------------- */

// ================ PORT A ===================

// Common all TXs
#define WAKE_UP_Pin GPIO_PIN_0
#define WAKE_UP_GPIO_Port GPIOA
#define GPIOO_PA05_Pin GPIO_PIN_5
#define GPIOO_PA05_GPIO_Port GPIOA
#define TEST_MODE_Pin GPIO_PIN_6
#define TEST_MODE_GPIO_Port GPIOA
#define LED_R_Pin GPIO_PIN_8
#define LED_R_GPIO_Port GPIOA
#define S2LP_GPIO_2_Pin GPIO_PIN_11
#define S2LP_GPIO_2_GPIO_Port GPIOA
#define S2LP_GPIO_1_Pin GPIO_PIN_12
#define S2LP_GPIO_1_GPIO_Port GPIOA
#define S2LP_CS_Pin GPIO_PIN_15
#define S2LP_CS_GPIO_Port GPIOA

// S85, S87, S87L (B81 & B80A)
#define BUTTON_LOCK_Pin GPIO_PIN_1
#define BUTTON_LOCK_GPIO_Port GPIOA
#define USART2_TX_Pin GPIO_PIN_2
#define USART2_TX_GPIO_Port GPIOA
#define USART2_RX_Pin GPIO_PIN_3
#define USART2_RX_GPIO_Port GPIOA
#define GPIOO_PA04_Pin GPIO_PIN_4
#define GPIOO_PA04_GPIO_Port GPIOA
#define DFU_SAFE_Pin GPIO_PIN_7         // Safety FW update
#define DFU_SAFE_GPIO_Port GPIOA
// S87 (B81)
#define GPIOO_PA09_Pin GPIO_PIN_9
#define GPIOO_PA09_GPIO_Port GPIOA
#define GPIOO_PA10_Pin GPIO_PIN_10
#define GPIOO_PA10_GPIO_Port GPIOA

// S87L (B80A)
#define DATA_LOC_Pin GPIO_PIN_9
#define DATA_LOC_GPIO_Port GPIOA
#define INT_LOC_Pin GPIO_PIN_10
#define INT_LOC_GPIO_Port GPIOA

// UPCBA and RAC80 common
#define OUTPUT_SIGNAL_Pin GPIO_PIN_3
#define OUTPUT_SIGNAL_GPIO_Port GPIOA
#define DFU_SAFE_UPCBA_RAC_Pin GPIO_PIN_4
#define DFU_SAFE_UPCBA_RAC_GPIO_Port GPIOA
#define GPIOO_PA07_Pin GPIO_PIN_7
#define GPIOO_PA07_GPIO_Port GPIOA
#define USART1_TX_Pin GPIO_PIN_9
#define USART1_TX_GPIO_Port GPIOA
#define USART1_RX_Pin GPIO_PIN_10
#define USART1_RX_GPIO_Port GPIOA

// UPCBA
#define W3_INPUT_Pin GPIO_PIN_1
#define W3_INPUT_GPIO_Port GPIOA
#define ALARM_INPUT_Pin GPIO_PIN_2
#define ALARM_INPUT_GPIO_Port GPIOA

// RAC80
#define TERMINAL_INPUT_Pin GPIO_PIN_1
#define TERMINAL_INPUT_GPIO_Port GPIOA
#define REED_INPUT_Pin GPIO_PIN_2
#define REED_INPUT_GPIO_Port GPIOA

// ================ PORT B ===================

// Common all TXs
#define TCXO_EN_Pin GPIO_PIN_0
#define TCXO_EN_GPIO_Port GPIOB
#define S2LP_SCK_Pin GPIO_PIN_3
#define S2LP_SCK_GPIO_Port GPIOB
#define S2LP_MISO_Pin GPIO_PIN_4
#define S2LP_MISO_GPIO_Port GPIOB
#define S2LP_MOSI_Pin GPIO_PIN_5
#define S2LP_MOSI_GPIO_Port GPIOB
#define LED_G_Pin GPIO_PIN_6
#define LED_G_GPIO_Port GPIOB
#define LED_B_Pin GPIO_PIN_7
#define LED_B_GPIO_Port GPIOB
#define S2LP_SHDWN_Pin GPIO_PIN_8
#define S2LP_SHDWN_GPIO_Port GPIOB

// S85, S87, S87L (B81 & B80A)
#define GPIOO_PB02_Pin GPIO_PIN_2
#define GPIOO_PB02_GPIO_Port GPIOB
// S85, S87
#define GPIOO_PB01_Pin GPIO_PIN_1
#define GPIOO_PB01_GPIO_Port GPIOB
// S87L (B80A)
#define LOC_EN_Pin GPIO_PIN_1
#define LOC_EN_GPIO_Port GPIOB

// UPCBA
#define ANT_SW_Pin GPIO_PIN_1
#define ANT_SW_GPIO_Port GPIOB
#define TRIG_LOCK_Pin GPIO_PIN_2
#define TRIG_LOCK_GPIO_Port GPIOB

// RAC80
#define EXT_POWER_Pin GPIO_PIN_1
#define EXT_POWER_GPIO_Port GPIOB
#define RAC_BUTTON_Pin GPIO_PIN_2
#define RAC_BUTTON_GPIO_Port GPIOB


// ================ PORT C ===================

// S85, S87, S87L (B81 & B80A)
#define GPIOO_PC14_Pin GPIO_PIN_14
#define GPIOO_PC14_GPIO_Port GPIOC
#define GPIOO_PC15_Pin GPIO_PIN_15
#define GPIOO_PC15_GPIO_Port GPIOC

// UPCBA
#define INPUT1_Pin GPIO_PIN_14
#define INPUT1_GPIO_Port GPIOC

#define W2_INPUT_Pin GPIO_PIN_15
#define W2_INPUT_GPIO_Port GPIOC

// RAC80
#define RAC_TRIG_LOCK_Pin GPIO_PIN_14
#define RAC_TRIG_LOCK_GPIO_Port GPIOC
#define GPIOO_PC15_Pin GPIO_PIN_15
#define GPIOO_PC15_GPIO_Port GPIOC



typedef struct                // Sequence LED RGB
{ 
  BOOLEAN     red;            // LED red ON/OFF
  BOOLEAN     green;          // LED green ON/OFF
  BOOLEAN     blue;           // LED blue ON/OFF
  u16         timerON;        // timer ON in ms
  u16         timerOFF;       // timer OFF in ms
  u16         timerON_Reload; // timer ON reload in ms, if continue seq.
  u16         timerOFF_Reload;// timer OFF reload in ms, if continue seq. 
}SEQUENCE_LED_RGB;


typedef struct                // Sequence LED GREEN SINGLE
{ 
  u16         timerON;        // timer ON in ms
  u16         timerOFF;       // timer OFF in ms
  u16         timerON_Reload; // timer ON reload in ms, if continue seq.
  u16         timerOFF_Reload;// timer OFF reload in ms, if continue seq. 
}SEQUENCE_LED_GREEN_SINGLE;

// ---------------------------------------------------------------------------  RF protocol related
#pragma pack(1)               // Byte align all elements
typedef struct                // Standard datagram for audio extender pairing protocol
{
  u32     time;               // time stamp (1ms step)
  u8      len;                // RX payload length
  u8      rssi;               // RX packet RSSI
  u8      payload[128];       // Payload
} RX_DATAGRAM;
#pragma pack()                // Standard alignment
#define PROX_DETECT_TIME          150                 // Min time to sustain "activated" status to validate proximity detection
#define PROX_DETECT_MAX_ON_TIME   300                 // Max time to sustain "activated" status to validate proximity detection
#define PROX_CARRIER_TIME         220                 // Proximity Carrier ON time

// Led behaviors
#define LED_OFF               0
#define LED_ON                1
#define LED_BLINK_1S          2
#define LED_BLINK_2S          3

// RGB led color
#define LED_BLACK             0
#define LED_RED               1
#define LED_GREEN             2
#define LED_YELLOW            3
#define LED_BLUE              4
#define LED_MAGENTA           5
#define LED_CYAN              6
#define LED_WHITE             7

#define PEAR_PLUGTIME_VALID   3         // Min time in 5s unit the pear must be plugged to activate "pear extracted" error


#endif //__MASTERDEFINE_H__
