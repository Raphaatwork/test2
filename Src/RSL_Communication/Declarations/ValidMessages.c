/**
  ******************************************************************************
  * @file       ValidDeclarations.c
  * @author     Tim Steinberg
  * @date       19.05.2020
  * @brief      File that contains structs of valid messages and param lengths
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
  * | 001       |               | Tim Steinberg         | Initial version / skeleton of file            |
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
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "Message_Definitions.h"
#include "ValidMessages.h"
/* Typedefinitions */

/* Variables */
/*
#ifdef I_AM_STM32
  #define TRANSMISSION_ROWSIZE  4
  #define TRANSMISSION_COLCOUNT 15
  #define RECEPTION_ROWSIZE     4
  #define RECEPTION_COLCOUNT    25
#else
  #ifdef I_AM_RSL10
    #define TRANSMISSION_ROWSIZE  4
    #define TRANSMISSION_COLCOUNT 15
    #define RECEPTION_ROWSIZE     4
    #define RECEPTION_COLCOUNT    25
  #else
    #warning NO DEVICE ROLE DEFINED!
  #endif
#endif
*/

#ifdef I_AM_STM32
/**
 * @brief This 2D-Array contains the CMD-Byte, 
 *        the minimum allowed data length,
 *        the maximum allowed data length,
 *        spacer byte.
 *        This is for the transmission part (outgoing from STM32 side)
 */
  const uint8_t uartMsgCmdDefinedTransmission[][4] = 
                                                  {     {UART_MSG_CMD_PACK_REC_ACK                      , 0, 0, 0,}, /**< CMD ACK, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_PACK_REC_NAK                      , 0, 0, 0,}, /**< CMD NAK, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_BATTERY_STATE                     , 1, 1, 0,}, /**< CMD Battery, min. 1 bytes, max. 1 bytes */
                                                        {UART_MSG_CMD_ALERT                             , 1, 1, 0,}, /**< CMD Alert, min. 1 bytes, max. 1 bytes */
                                                        {UART_MSG_CMD_ERROR                             , 1, 1, 0,}, /**< CMD Error, min. 1 bytes, max. 1 bytes */
                                                        {UART_MSG_CMD_GO_TO_SLEEP                       , 0, 0, 0,}, /**< CMD Go to sleep, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_REBOOT                            , 0, 0, 0,}, /**< CMD Reboot, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_START_PAIRING                     , 0, 0, 0,}, /**< CMD Start pairing, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_REMOVE_PAIRED_DEVICE              , 0, 0, 0,}, /**< CMD Remove paired devices, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_START_BC                          , 0, 0, 0,}, /**< CMD Start broadcast, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_STOP_BC                           , 0, 0, 0,}, /**< CMD Stop broadcast, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_HCI_COMMAND                       , 0, 32,0,}, /**< CMD HCI, min. 0 bytes, max. 32 bytes */
                                                        {UART_MSG_CMD_CALIBRATION_COMMAND               , 0, 32,0,}, /**< CMD CAL, min. 0 bytes, max. 32 bytes */
                                                        {UART_MSG_CMD_START_FOTA                        , 0, 0, 0,}, /**< CMD FOTA start, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_STOP_FOTA                         , 0, 0, 0,}, /**< CMD FOTA stop, min. 0 bytes, max. 0 bytes */
                                                  };
/**
 * @brief This 2D-Array contains the CMD-Byte, 
 *        the minimum allowed data length,
 *        the maximum allowed data length,
 *        spacer byte.
 *        This is for the reception part (incoming from STM32 side)
 */
  const uint8_t uartMsgCmdDefinedReception[][4] = 
                                                  {     {UART_MSG_CMD_PACK_REC_ACK                      , 0, 0, 0,}, /**< CMD ACK, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_PACK_REC_NAK                      , 0, 0, 0,}, /**< CMD NAK, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_CHAR_ACK                          , 0, 0, 0,}, /**< CMD Characteristic update OK, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_CHAR_NAK                          , 0, 0, 0,}, /**< CMD Characteristic update NOT OK, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_GO_TO_SLEEP_OK                    , 0, 0, 0,}, /**< CMD Go to sleep OK, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_READY_AFTER_SLEEP                 , 0, 0, 0,}, /**< CMD Ready after sleep, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_GO_TO_SLEEP_MYSELF                , 0, 0, 0,}, /**< CMD Go to sleep myself, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_RESPONSE_TO_REBOOT                , 0, 0, 0,}, /**< CMD Reboot OK, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_READY_AFTER_BOOT_UP               , 0, 0, 0,}, /**< CMD Ready after boot, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_PAIRING_OK                        , 0, 0, 0,}, /**< CMD Pairing OK, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_PAIRING_NOT_OK                    , 0, 0, 0,}, /**< CMD Pairing NOT OK, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_PAIRED_DEVICE_REMOVED_OK          , 0, 0, 0,}, /**< CMD Remove devices OK, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_START_BC_OK                       , 0, 0, 0,}, /**< CMD Start broadcast OK, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_START_BC_NOT_OK                   , 0, 0, 0,}, /**< CMD Start broadcast NOT OK, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_DATA_TRANSMIT_OK                  , 0, 0, 0,}, /**< CMD Data transmit OK, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_STOP_BC_OK                        , 0, 0, 0,}, /**< CMD Stop broadcast OK, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_STOP_BC_NOT_OK                    , 0, 0, 0,}, /**< CMD Stop broadcast NOT OK, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_HCI_COMMAND_RESPONSE              , 0, 32,0,}, /**< CMD HCI response, min. 0 bytes, max. 32 bytes */
                                                        {UART_MSG_CMD_CALIBRATION_COMMAND_RESPONSE      , 0, 32,0,}, /**< CMD CAL response, min. 0 bytes, max. 32 bytes */
                                                        {UART_MSG_CMD_START_FOTA_OK                     , 0, 0, 0,}, /**< CMD Start FOTA OK, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_START_FOTA_NOK                    , 0, 0, 0,}, /**< CMD Start FOTA NOT OK, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_FOTA_ONGOING                      , 0, 0, 0,}, /**< CMD FOTA ongoing, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_FOTA_FINISHED                     , 0, 0, 0,}, /**< CMD FOTA finished, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_STOP_FOTA_OK                      , 0, 0, 0,}, /**< CMD Stop FOTA OK, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_STOP_FOTA_NOK                     , 0, 0, 0,}, /**< CMD Stop FOTA NOT OK, min. 0 bytes, max. 0 bytes */
                                                  };
#else
  #ifdef I_AM_RSL10
/**
 * @brief This 2D-Array contains the CMD-Byte, 
 *        the minimum allowed data length,
 *        the maximum allowed data length,
 *        spacer byte.
 *        This is for the transmission part (outgoing from RSL10 side)
 */
    const uint8_t uartMsgCmdDefinedReception[TRANSMISSION_COLCOUNT][TRANSMISSION_ROWSIZE] = 
                                                    {   {UART_MSG_CMD_PACK_REC_ACK                      , 0, 0, 0,}, /**< CMD ACK, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_PACK_REC_NAK                      , 0, 0, 0,}, /**< CMD NAK, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_BATTERY_STATE                     , 1, 1, 0,}, /**< CMD Battery, min. 1 bytes, max. 1 bytes */
                                                        {UART_MSG_CMD_ALERT                             , 1, 1, 0,}, /**< CMD Alert, min. 1 bytes, max. 1 bytes */ 
                                                        {UART_MSG_CMD_ERROR                             , 1, 1, 0,}, /**< CMD Error, min. 1 bytes, max. 1 bytes */
                                                        {UART_MSG_CMD_GO_TO_SLEEP                       , 0, 0, 0,}, /**< CMD Go to sleep, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_REBOOT                            , 0, 0, 0,}, /**< CMD Reboot, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_START_PAIRING                     , 0, 0, 0,}, /**< CMD Start pairing, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_REMOVE_PAIRED_DEVICE              , 0, 0, 0,}, /**< CMD Remove paired devices, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_START_BC                          , 0, 0, 0,}, /**< CMD Start broadcast, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_STOP_BC                           , 0, 0, 0,}, /**< CMD Stop broadcast, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_HCI_COMMAND                       , 0, 32,0,}, /**< CMD HCI, min. 0 bytes, max. 32 bytes */
                                                        {UART_MSG_CMD_CALIBRATION_COMMAND               , 0, 32,0,}, /**< CMD CAL, min. 0 bytes, max. 32 bytes */
                                                        {UART_MSG_CMD_START_FOTA                        , 0, 0, 0,}, /**< CMD FOTA start, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_STOP_FOTA                         , 0, 0, 0,}, /**< CMD FOTA stop, min. 0 bytes, max. 0 bytes */
                                                  };
/**
 * @brief This 2D-Array contains the CMD-Byte, 
 *        the minimum allowed data length,
 *        the maximum allowed data length,
 *        spacer byte.
 *        This is for the reception part (incoming from RSL10 side)
 */
    const uint8_t uartMsgCmdDefinedTransmission[RECEPTION_COLCOUNT][RECEPTION_ROWSIZE] = 
                                                    {   {UART_MSG_CMD_PACK_REC_ACK                      , 0, 0, 0,}, /**< CMD ACK, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_PACK_REC_NAK                      , 0, 0, 0,}, /**< CMD NAK, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_CHAR_ACK                          , 0, 0, 0,}, /**< CMD Characteristic update OK, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_CHAR_NAK                          , 0, 0, 0,}, /**< CMD Characteristic update NOT OK, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_GO_TO_SLEEP_OK                    , 0, 0, 0,}, /**< CMD Go to sleep OK, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_READY_AFTER_SLEEP                 , 0, 0, 0,}, /**< CMD Ready after sleep, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_GO_TO_SLEEP_MYSELF                , 0, 0, 0,}, /**< CMD Go to sleep myself, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_RESPONSE_TO_REBOOT                , 0, 0, 0,}, /**< CMD Reboot OK, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_READY_AFTER_BOOT_UP               , 0, 0, 0,}, /**< CMD Ready after boot, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_PAIRING_OK                        , 0, 0, 0,}, /**< CMD Pairing OK, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_PAIRING_NOT_OK                    , 0, 0, 0,}, /**< CMD Pairing NOT OK, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_PAIRED_DEVICE_REMOVED_OK          , 0, 0, 0,}, /**< CMD Remove devices OK, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_START_BC_OK                       , 0, 0, 0,}, /**< CMD Start broadcast OK, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_START_BC_NOT_OK                   , 0, 0, 0,}, /**< CMD Start broadcast NOT OK, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_DATA_TRANSMIT_OK                  , 0, 0, 0,}, /**< CMD Data transmit OK, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_STOP_BC_OK                        , 0, 0, 0,}, /**< CMD Stop broadcast OK, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_STOP_BC_NOT_OK                    , 0, 0, 0,}, /**< CMD Stop broadcast NOT OK, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_HCI_COMMAND_RESPONSE              , 0, 32,0,}, /**< CMD HCI response, min. 0 bytes, max. 32 bytes */
                                                        {UART_MSG_CMD_CALIBRATION_COMMAND_RESPONSE      , 0, 32,0,}, /**< CMD CAL response, min. 0 bytes, max. 32 bytes */
                                                        {UART_MSG_CMD_START_FOTA_OK                     , 0, 0, 0,}, /**< CMD Start FOTA OK, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_START_FOTA_NOK                    , 0, 0, 0,}, /**< CMD Start FOTA NOT OK, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_FOTA_ONGOING                      , 0, 0, 0,}, /**< CMD FOTA ongoing, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_FOTA_FINISHED                     , 0, 0, 0,}, /**< CMD FOTA finished, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_STOP_FOTA_OK                      , 0, 0, 0,}, /**< CMD Stop FOTA OK, min. 0 bytes, max. 0 bytes */
                                                        {UART_MSG_CMD_STOP_FOTA_NOK                     , 0, 0, 0,}, /**< CMD Stop FOTA NOT OK, min. 0 bytes, max. 0 bytes */
                                                  };
  #else
    #warning NO DEVICE ROLE DEFINED!
  #endif
#endif

/* Function definitions */
/** @brief This method will return the pointer to the reception 2D-byte array
 *  @return Pointer to the reception 2D-byte array
 */
uint8_t** messageParamValidity_reception_getPointerToBuffer(){
  return (uint8_t **)uartMsgCmdDefinedReception;
}

/** @brief This method will return the overall byte count of the 2D-byte array
 *  @return Overall byte count of the 2D-byte array
 */
uint32_t messageParamValidity_reception_getOverallSize(){
  return sizeof(uartMsgCmdDefinedReception);
}

/** @brief This method will return the column count of the 2D-byte array
 *  @return Column count of the 2D-byte array
 */
uint32_t messageParamValidity_reception_getColCount(){
  return sizeof(uartMsgCmdDefinedReception[0]);
}

/** @brief This method will return the row count of the 2D-byte array
 *  @return Row count of the 2D-byte array
 */
uint32_t messageParamValidity_reception_getRowCount(){
  return sizeof(uartMsgCmdDefinedReception) / sizeof(uartMsgCmdDefinedReception[0]);
}

/** @brief This method will return the element size in the 2D-byte array
 *  @return Element size in the 2D-byte array
 */
uint32_t messageParamValidity_reception_getElementSize(){
  return sizeof(uartMsgCmdDefinedReception[0][0]);
}


/** @brief This method will return the pointer to the reception 2D-byte array
 *  @return Pointer to the reception 2D-byte array
 */
uint8_t** messageParamValidity_transmission_getPointerToBuffer(){
  return (uint8_t **)uartMsgCmdDefinedTransmission;
}

/** @brief This method will return the overall byte count of the 2D-byte array
 *  @return Overall byte count of the 2D-byte array
 */
uint32_t messageParamValidity_transmission_getOverallSize(){
  return sizeof(uartMsgCmdDefinedTransmission);
}

/** @brief This method will return the column count of the 2D-byte array
 *  @return Column count of the 2D-byte array
 */
uint32_t messageParamValidity_transmission_getColCount(){
  return sizeof(uartMsgCmdDefinedTransmission[0]);
}

/** @brief This method will return the row count of the 2D-byte array
 *  @return Row count of the 2D-byte array
 */
uint32_t messageParamValidity_transmission_getRowCount(){
  return sizeof(uartMsgCmdDefinedTransmission) / sizeof(uartMsgCmdDefinedTransmission[0]);
}

/** @brief This method will return the element size in the 2D-byte array
 *  @return Element size in the 2D-byte array
 */
uint32_t messageParamValidity_transmission_getElementSize(){
  return sizeof(uartMsgCmdDefinedTransmission[0][0]);
}
