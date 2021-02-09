/**
  ******************************************************************************
  * @file       App_Misc.h
  * @author     Tim Steinberg
  * @date       08.07.2020
  * @brief      A collection of several functions that are used in the main apps
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
  * | 001       | 2020-07-08    | Tim Steinberg         | Initial version / skeleton of file            |
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

#ifndef __APP_MISC_H
#define __APP_MISC_H

/* Includes */
#include "ErrorCodes.h"

/* Typedefinitions */

/* Variables */

/* Function definitions */
void app_misc_alert_setNew(void);
void app_misc_alert_setNewToOld(void);
void app_misc_alert_unset(void);

void app_misc_battery_measureNewBatteryValue(void);
void app_misc_battery_resetBatteryLowCounter(void);
bool app_misc_battery_isBatteryNew(void);

void app_misc_error_maskInNewErrorcode(ERRORCODES_TYPEDEF errorcode);
void app_misc_error_unset(void);
void app_misc_errorAndOldAlert_unset(void);

void app_misc_sync_eepromToCommstack(void);

void app_misc_showErrorCritical(void);
void app_misc_showError(void);
void app_misc_showSuccess(void);
void app_misc_showProblematicSuccess(void);

#endif