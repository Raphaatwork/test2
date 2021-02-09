/**
  ******************************************************************************
  * @file       UserMethods_Characteristics.c
  * @author     Tim Steinberg
  * @date       04.06.2020
  * @brief      Functions regarding bluetooth values
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
  * | 001       | 2020-06-04    | Tim Steinberg         | Initial version / skeleton of file            |
  * |-----------|---------------|-----------------------|-----------------------------------------------|
  * | 002       | 2020-06-15    | Tim Steinberg         | Added comments & doxygen commentaries         |
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
#include "UserMethods_Characteristics.h"
#include "Watchdog.h"

#include "stm32l0xx_hal.h"
/* Typedefinitions */

/* Variables */

/** \brief userMethods_characteristics_alert This variable holds the ST value
  *        of the ST-used characteristic alert
  */
uint8_t userMethods_characteristics_alert = 0;

/** \brief userMethods_characteristics_error This variable holds the ST value
  *        of the ST-used characteristic error
  */
uint8_t userMethods_characteristics_error = 0;

/** \brief userMethods_characteristics_battery This variable holds the ST value
  *        of the ST-used characteristic battery
  */
uint8_t userMethods_characteristics_battery = 0;

/** \brief userMethods_characteristics_pairingState This variable holds the ST
  *        sided value of a pairing state.
  */
USERMETHODS_CHARACTERISTICS_PAIRING_STATE_TYPEDEF userMethods_characteristics_pairingState = USERMETHODS_CHARACTERISTICS_PAIRING_STATE_UNPAIRED;

/** \brief userMethods_characteristics_broadcastingState This variable holds the
  *        ST sided value of a broadcasting state.
  */
USERMETHODS_CHARACTERISTICS_BROADCASTING_STATE_TYPEDEF userMethods_characteristics_broadcastingState = USERMETHODS_CHARACTERISTICS_BROADCASTING_STATE_INACTIVE;

/** \brief userMethods_characteristics_transmissionState This variable holds the
  *        ST sided value of a transmission state. 
  */
USERMETHODS_CHARACTERISTICS_TRANSMISSION_STATE_TYPEDEF userMethods_characteristics_transmissionState = USERMETHODS_CHARACTERISTICS_TRANSMISSION_STATE_NONE;

/* Function definitions */

/***********************************************
 *  FUNCTIONGROUP CONTROL
 ***********************************************/

void userMethods_feedWatchdog(){
  watchdog_feed();
}

/***********************************************
 *  FUNCTIONGROUP TIME
 ***********************************************/

/** @brief This method will ask the system for the actual time (in miliseconds)
 *  @return The time in ms (see HAL_GetTick() ).
 */
uint32_t userMethods_characteristics_getTime(){
  return HAL_GetTick();
}

/** @brief This method will wait for some time (in miliseconds)
 *  @param The time in ms (see HAL_Delay() ).
 *  @return Nothing.
 */
void userMethods_characteristics_waitTime(uint32_t waitTime){
  HAL_Delay(waitTime);
  return;
}

/***********************************************
 *  FUNCTIONGROUP CHARACTERISTIC ALERT
 ***********************************************/

/** @brief This method will set the "ST-Side" characteristic for alarm to 1.
 *  @return Nothing.
 */
void userMethods_characteristics_setAlert(uint8_t alert){
  userMethods_characteristics_alert = alert;
}

/** @brief This method will set the "ST-Side" characteristic for alarm to 0.
 *  @return Nothing.
 */
void userMethods_characteristics_resetAlert(){
  userMethods_characteristics_alert = 0;
}

/** @brief This method will ask the "ST-Side" characteristic alert.
 *  @return The value of the characeristic.
 */
uint8_t userMethods_characteristics_getAlert(){
  return userMethods_characteristics_alert;
}

/***********************************************
 *  FUNCTIONGROUP CHARACTERISTIC ERROR
 ***********************************************/

/** @brief This method will set the "ST-Side" characteristic for error to 1.
 *  @return Nothing.
 */
void userMethods_characteristics_setError(uint8_t errorCode){
  userMethods_characteristics_error = errorCode;
}

/** @brief This method will set the "ST-Side" characteristic for error to 0.
 *  @return Nothing.
 */
void userMethods_characteristics_resetError(){
  userMethods_characteristics_error = 0;
}

/** @brief This method will ask the "ST-Side" characteristic error.
 *  @return The value of the characeristic.
 */
uint8_t userMethods_characteristics_getError(){
  return userMethods_characteristics_error;
}

/***********************************************
 *  FUNCTIONGROUP CHARACTERISTIC BATTERY
 ***********************************************/

/** @brief This method will set the "ST-Side" characteristic for battery to the
 *         given value.
 *  @return Nothing.
 */
void userMethods_characteristics_setBattery(uint8_t batteryLevelPercent){
  userMethods_characteristics_battery = batteryLevelPercent;
}

/** @brief This method will ask the "ST-Side" characteristic battery.
 *  @return The value of the characeristic.
 */
uint8_t userMethods_characteristics_getBattery(){
  return userMethods_characteristics_battery;
}

/***********************************************
 *  FUNCTIONGROUP CHARACTERISTIC PAIRING
 ***********************************************/

/** @brief This method will set the "ST-Side" value for pairing to "paired".
 *  @return Nothing.
 */
void userMethods_characteristics_setPairingStatePaired(){
  userMethods_characteristics_pairingState = USERMETHODS_CHARACTERISTICS_PAIRING_STATE_PAIRED;
}

/** @brief This method will set the "ST-Side" value for pairing to "unpaired".
 *  @return Nothing.
 */
void userMethods_characteristics_setPairingStateUnpaired(){
  userMethods_characteristics_pairingState = USERMETHODS_CHARACTERISTICS_PAIRING_STATE_UNPAIRED;
}

/** @brief This method will ask the "ST-Side" value of pairing.
 *  @return The value of the characeristic.
 */
USERMETHODS_CHARACTERISTICS_PAIRING_STATE_TYPEDEF userMethods_characteristics_getPairingState(){
  return userMethods_characteristics_pairingState;
}

/***********************************************
 *  FUNCTIONGROUP TRANSMISSION
 ***********************************************/

/** @brief This method will set the "ST-Side" value for transmission to "none".
 *              Meaning no data has been transmitted yet.
 *  @return Nothing.
 */
void userMethods_characteristics_setTransmissionState_None(){
  userMethods_characteristics_transmissionState = USERMETHODS_CHARACTERISTICS_TRANSMISSION_STATE_NONE;
}

/** @brief This method will set the "ST-Side" value for transmission to "done".
 *              Meaning data has been transmitted.
 *  @return Nothing.
 */
void userMethods_characteristics_setTransmissionState_Done(){
  userMethods_characteristics_transmissionState = USERMETHODS_CHARACTERISTICS_TRANSMISSION_STATE_DONE;
}

/** @brief This method will ask the "ST-Side" value of transmission.
 *  @return The value of the characeristic.
 */
USERMETHODS_CHARACTERISTICS_TRANSMISSION_STATE_TYPEDEF userMethods_characteristics_getTransmissionState(){
  return userMethods_characteristics_transmissionState;
}

