#ifndef __DEVICE_DEFINITIONS_H
#define __DEVICE_DEFINITIONS_H

#define DEVICE_TYPE_STM32       1
#define DEVICE_TYPE_RSL10       2
#define DEVICE_TYPE_NONE        3

#define I_AM_STM32
//#define I_AM_RSL10

#ifdef DEVICE_TYPE
  #error "DEVICE TYPE IS ALREADY DEFINED!"
#else
  #if defined(I_AM_STM32) &&  defined(I_AM_RSL10)
    #error "YOU CAN'T BE BOTH DEVICE TYPES AT ONCE!"
  #else
    #ifdef I_AM_STM32
      #define DEVICE_TYPE   DEVICE_TYPE_STM32
    #else
      #ifdef I_AM_RSL10
        #define DEVICE_TYPE DEVICE_TYPE_RSL10
      #else
        #define DEVICE_TYPE DEVICE_TYPE_NONE
      #endif
    #endif
  #endif
#endif

#ifdef I_AM_STM32
  #define UART_AWAITING_MAGIC                                                   0x03
  #define UART_TRANSMISSION_MAGIC                                               0x02
#else
  #ifdef I_AM_RSL10
    #define UART_AWAITING_MAGIC                                                 0x02
    #define UART_TRANSMISSION_MAGIC                                             0x03
  #else
    #error NO DEVICE ROLE DEFINED!
  #endif
#endif

#endif