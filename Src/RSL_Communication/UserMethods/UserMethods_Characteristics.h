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

#ifndef __USERMETHODS_CHARACTERISTICS_H
#define __USERMETHODS_CHARACTERISTICS_H

/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

/* Typedefinitions */

/**
 * @brief This typedef defines the members of the pairing states.
 *        UNPAIRED = Nothing is paired with the RSL10.
 *        PAIRED = Something is paired with the RSL10.
 */
typedef enum USERMETHODS_CHARACTERISTICS_PAIRING_STATE {
  USERMETHODS_CHARACTERISTICS_PAIRING_STATE_UNPAIRED            = 0x00,
  USERMETHODS_CHARACTERISTICS_PAIRING_STATE_PAIRED              = 0x01,
} USERMETHODS_CHARACTERISTICS_PAIRING_STATE_TYPEDEF;

/**
 * @brief This typedef defines the members of the broadcasting states.
 *        INACTIVE = RSL10 is silent.
 *        ACTIVE = RSL10 is screaming.
 */
typedef enum USERMETHODS_CHARACTERISTICS_BROADCASTING_STATE {
  USERMETHODS_CHARACTERISTICS_BROADCASTING_STATE_INACTIVE       = 0x00,
  USERMETHODS_CHARACTERISTICS_BROADCASTING_STATE_ACTIVE         = 0x01,
} USERMETHODS_CHARACTERISTICS_BROADCASTING_STATE_TYPEDEF;

/**
 * @brief This typedef defines the members of the transmission states.
 *        OPEN = RSL10 does nothing
 *        BC STARTED/STOPPED = RSL10 broadcasts or ended the bc.
 *        TX OK/NOK = Something read data or not.
 */
typedef enum USERMETHODS_CHARACTERISTICS_TRANSMISSION_STATE {
  USERMETHODS_CHARACTERISTICS_TRANSMISSION_STATE_NONE                   = 0x00,
  USERMETHODS_CHARACTERISTICS_TRANSMISSION_STATE_DONE                   = 0x01,
} USERMETHODS_CHARACTERISTICS_TRANSMISSION_STATE_TYPEDEF;

/* Variables */

/* Function definitions */

void userMethods_feedWatchdog(void);

/***********************************************
 *  FUNCTIONGROUP TIME
 ***********************************************/

/** @brief This method will ask the system for the actual time (in miliseconds)
 *  @return The time in ms (see HAL_GetTick() ).
 */
uint32_t userMethods_characteristics_getTime(void);

/** @brief This method will wait for some time (in miliseconds)
 *  @param The time in ms (see HAL_Delay() ).
 *  @return Nothing.
 */
void userMethods_characteristics_waitTime(uint32_t waitTime);

/***********************************************
 *  FUNCTIONGROUP CHARACTERISTIC ALERT
 ***********************************************/

/** @brief This method will set the "ST-Side" characteristic for alarm to 1.
 *  @return Nothing.
 */
void userMethods_characteristics_setAlert(uint8_t alert);

/** @brief This method will set the "ST-Side" characteristic for alarm to 0.
 *  @return Nothing.
 */
void userMethods_characteristics_resetAlert(void);

/** @brief This method will ask the "ST-Side" characteristic alert.
 *  @return The value of the characeristic.
 */
uint8_t userMethods_characteristics_getAlert(void);

/***********************************************
 *  FUNCTIONGROUP CHARACTERISTIC ERROR
 ***********************************************/

/** @brief This method will set the "ST-Side" characteristic for error to 1.
 *  @return Nothing.
 */
void userMethods_characteristics_setError(uint8_t errorCode);

/** @brief This method will set the "ST-Side" characteristic for error to 0.
 *  @return Nothing.
 */
void userMethods_characteristics_resetError(void);

/** @brief This method will ask the "ST-Side" characteristic error.
 *  @return The value of the characeristic.
 */
uint8_t userMethods_characteristics_getError(void);

/***********************************************
 *  FUNCTIONGROUP CHARACTERISTIC BATTERY
 ***********************************************/

/** @brief This method will set the "ST-Side" characteristic for battery to the
 *         given value.
 *  @return Nothing.
 */
void userMethods_characteristics_setBattery(uint8_t batteryLevelPercent);

/** @brief This method will ask the "ST-Side" characteristic battery.
 *  @return The value of the characeristic.
 */
uint8_t userMethods_characteristics_getBattery(void);

/***********************************************
 *  FUNCTIONGROUP CHARACTERISTIC PAIRING
 ***********************************************/

/** @brief This method will set the "ST-Side" value for pairing to "paired".
 *  @return Nothing.
 */
void userMethods_characteristics_setPairingStatePaired(void);

/** @brief This method will set the "ST-Side" value for pairing to "unpaired".
 *  @return Nothing.
 */
void userMethods_characteristics_setPairingStateUnpaired(void);

/** @brief This method will ask the "ST-Side" value of pairing.
 *  @return The value of the characeristic.
 */
USERMETHODS_CHARACTERISTICS_PAIRING_STATE_TYPEDEF userMethods_characteristics_getPairingState(void);

/***********************************************
 *  FUNCTIONGROUP TRANSMISSION
 ***********************************************/

/** @brief This method will set the "ST-Side" value for transmission to "none".
 *              Meaning no data has been transmitted yet.
 *  @return Nothing.
 */
void userMethods_characteristics_setTransmissionState_None(void);

/** @brief This method will set the "ST-Side" value for transmission to "done".
 *              Meaning data has been transmitted.
 *  @return Nothing.
 */
void userMethods_characteristics_setTransmissionState_Done(void);

/** @brief This method will ask the "ST-Side" value of transmission.
 *  @return The value of the characeristic.
 */
USERMETHODS_CHARACTERISTICS_TRANSMISSION_STATE_TYPEDEF userMethods_characteristics_getTransmissionState(void);

#endif