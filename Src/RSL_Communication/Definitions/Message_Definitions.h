#ifndef __MESSAGE_DEFINITIONS_H
#define __MESSAGE_DEFINITIONS_H

#include "Device_Definitions.h"

#define UART_MSG_CMD_BATTERY_STATE                                              0x00
#define UART_MSG_CMD_ALERT                                                      0x01
#define UART_MSG_CMD_ERROR                                                      0x04

#define UART_MSG_CMD_CHAR_ACK                                                   0x08
#define UART_MSG_CMD_CHAR_NAK                                                   0x09

#define UART_MSG_CMD_HCI_COMMAND                                                0x10
#define UART_MSG_CMD_HCI_COMMAND_RESPONSE                                       0x11

#define UART_MSG_CMD_CALIBRATION_COMMAND                                        0x18
#define UART_MSG_CMD_CALIBRATION_COMMAND_RESPONSE                               0x19

#define UART_MSG_CMD_START_FOTA                                                 0x20
#define UART_MSG_CMD_START_FOTA_OK                                              0x21
#define UART_MSG_CMD_START_FOTA_NOK                                             0x22

#define UART_MSG_CMD_FOTA_ONGOING                                               0x23
#define UART_MSG_CMD_FOTA_FINISHED                                              0x24

#define UART_MSG_CMD_STOP_FOTA                                                  0x28
#define UART_MSG_CMD_STOP_FOTA_OK                                               0x29
#define UART_MSG_CMD_STOP_FOTA_NOK                                              0x2A

#define UART_MSG_CMD_PACK_REC_ACK                                               0x50
#define UART_MSG_CMD_PACK_REC_NAK                                               0x51

#define UART_MSG_CMD_GO_TO_SLEEP                                                0x60
#define UART_MSG_CMD_GO_TO_SLEEP_OK                                             0x61
#define UART_MSG_CMD_READY_AFTER_SLEEP                                          0x62
#define UART_MSG_CMD_GO_TO_SLEEP_MYSELF                                         0x63

#define UART_MSG_CMD_REBOOT                                                     0x68
#define UART_MSG_CMD_RESPONSE_TO_REBOOT                                         0x69
#define UART_MSG_CMD_READY_AFTER_BOOT_UP                                        0x6A

#define UART_MSG_CMD_START_PAIRING                                              0x70
#define UART_MSG_CMD_PAIRING_OK                                                 0x71
#define UART_MSG_CMD_PAIRING_NOT_OK                                             0x72

#define UART_MSG_CMD_REMOVE_PAIRED_DEVICE                                       0x78
#define UART_MSG_CMD_PAIRED_DEVICE_REMOVED_OK                                   0x79

#define UART_MSG_CMD_START_BC                                                   0x80
#define UART_MSG_CMD_START_BC_OK                                                0x81
#define UART_MSG_CMD_START_BC_NOT_OK                                            0x82

#define UART_MSG_CMD_DATA_TRANSMIT_OK                                           0x84

#define UART_MSG_CMD_STOP_BC                                                    0x88
#define UART_MSG_CMD_STOP_BC_OK                                                 0x89
#define UART_MSG_CMD_STOP_BC_NOT_OK                                             0x8A

/**
 * @brief       This typedef enum contains all possible message commands
 */
typedef enum UART_MESSAGE_TYPES {
  UART_MESSAGE_TYPE_BATTERY_STATE                                               = UART_MSG_CMD_BATTERY_STATE,
  UART_MESSAGE_TYPE_ALERT                                                       = UART_MSG_CMD_ALERT,
  UART_MESSAGE_TYPE_ERROR                                                       = UART_MSG_CMD_ERROR,
  UART_MESSAGE_TYPE_CHAR_ACK                                                    = UART_MSG_CMD_CHAR_ACK,
  UART_MESSAGE_TYPE_CHAR_NAK                                                    = UART_MSG_CMD_CHAR_NAK,
  UART_MESSAGE_TYPE_HCI_COMMAND                                                 = UART_MSG_CMD_HCI_COMMAND,
  UART_MESSAGE_TYPE_HCI_COMMAND_RESPONSE                                        = UART_MSG_CMD_HCI_COMMAND_RESPONSE,
  UART_MESSAGE_TYPE_CALIBRATION_COMMAND                                         = UART_MSG_CMD_CALIBRATION_COMMAND,
  UART_MESSAGE_TYPE_CALIBRATION_COMMAND_RESPONSE                                = UART_MSG_CMD_CALIBRATION_COMMAND_RESPONSE,
  UART_MESSAGE_TYPE_START_FOTA                                                  = UART_MSG_CMD_START_FOTA,
  UART_MESSAGE_TYPE_START_FOTA_OK                                               = UART_MSG_CMD_START_FOTA_OK,
  UART_MESSAGE_TYPE_START_FOTA_NOK                                              = UART_MSG_CMD_START_FOTA_NOK,
  UART_MESSAGE_TYPE_FOTA_ONGOING                                                = UART_MSG_CMD_FOTA_ONGOING,
  UART_MESSAGE_TYPE_FOTA_FINISHED                                               = UART_MSG_CMD_FOTA_FINISHED,
  UART_MESSAGE_TYPE_STOP_FOTA                                                   = UART_MSG_CMD_STOP_FOTA,
  UART_MESSAGE_TYPE_STOP_FOTA_OK                                                = UART_MSG_CMD_STOP_FOTA_OK,
  UART_MESSAGE_TYPE_STOP_FOTA_NOK                                               = UART_MSG_CMD_STOP_FOTA_NOK,
  UART_MESSAGE_TYPE_PACK_REC_ACK                                                = UART_MSG_CMD_PACK_REC_ACK,
  UART_MESSAGE_TYPE_PACK_REC_NAK                                                = UART_MSG_CMD_PACK_REC_NAK,
  UART_MESSAGE_TYPE_GO_TO_SLEEP                                                 = UART_MSG_CMD_GO_TO_SLEEP,
  UART_MESSAGE_TYPE_GO_TO_SLEEP_OK                                              = UART_MSG_CMD_GO_TO_SLEEP_OK,
  UART_MESSAGE_TYPE_READY_AFTER_SLEEP                                           = UART_MSG_CMD_READY_AFTER_SLEEP,
  UART_MESSAGE_TYPE_GO_TO_SLEEP_MYSELF                                          = UART_MSG_CMD_GO_TO_SLEEP_MYSELF,
  UART_MESSAGE_TYPE_REBOOT                                                      = UART_MSG_CMD_REBOOT,
  UART_MESSAGE_TYPE_RESPONSE_TO_REBOOT                                          = UART_MSG_CMD_RESPONSE_TO_REBOOT,
  UART_MESSAGE_TYPE_READY_AFTER_BOOT_UP                                         = UART_MSG_CMD_READY_AFTER_BOOT_UP,
  UART_MESSAGE_TYPE_START_PAIRING                                               = UART_MSG_CMD_START_PAIRING,
  UART_MESSAGE_TYPE_PAIRING_OK                                                  = UART_MSG_CMD_PAIRING_OK,
  UART_MESSAGE_TYPE_PAIRING_NOT_OK                                              = UART_MSG_CMD_PAIRING_NOT_OK,
  UART_MESSAGE_TYPE_REMOVE_PAIRED_DEVICE                                        = UART_MSG_CMD_REMOVE_PAIRED_DEVICE,
  UART_MESSAGE_TYPE_PAIRED_DEVICE_REMOVED_OK                                    = UART_MSG_CMD_PAIRED_DEVICE_REMOVED_OK,
  UART_MESSAGE_TYPE_START_BC                                                    = UART_MSG_CMD_START_BC,
  UART_MESSAGE_TYPE_START_BC_OK                                                 = UART_MSG_CMD_START_BC_OK,
  UART_MESSAGE_TYPE_START_BC_NOT_OK                                             = UART_MSG_CMD_START_BC_NOT_OK,
  UART_MESSAGE_TYPE_DATA_TRANSMIT_OK                                            = UART_MSG_CMD_DATA_TRANSMIT_OK,
  UART_MESSAGE_TYPE_STOP_BC                                                     = UART_MSG_CMD_STOP_BC,
  UART_MESSAGE_TYPE_STOP_BC_OK                                                  = UART_MSG_CMD_STOP_BC_OK,
  UART_MESSAGE_TYPE_STOP_BC_NOT_OK                                              = UART_MSG_CMD_STOP_BC_NOT_OK,
} UART_MESSAGE_TYPES_TYPEDEF;
  
#define UART_PACKAGE_HEADER_SIZE                                                0x02
#define UART_PACKAGE_DATA_SIZE                                                  0x01
#define UART_PACKAGE_CRC_SIZE                                                   0x01

#define UART_PACKAGE_POSITION_CMD_BYTE                                          0x01
#define UART_PACKAGE_POSITION_DATA_LENGTH_BYTE                                  0x02
#define UART_PACKAGE_POSITION_FIRST_PARAM_BYTE                                  0x03

#define UART_PACKAGE_NAK_SIZE                                                   UART_PACKAGE_HEADER_SIZE + UART_PACKAGE_DATA_SIZE + UART_PACKAGE_CRC_SIZE
#define UART_PACKAGE_ACK_SIZE                                                   UART_PACKAGE_HEADER_SIZE + UART_PACKAGE_DATA_SIZE + UART_PACKAGE_CRC_SIZE


#endif