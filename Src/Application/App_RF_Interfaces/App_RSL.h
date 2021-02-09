/**
  ******************************************************************************
  * @file       APP_RSL.h
  * @author     Tim Steinberg
  * @date       08.11.2020
  * @brief      Handler for RSL10 communication stack
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
  * | 001       | 08.11.2020    | Tim Steinberg         | Initial version / skeleton of file            |
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

#ifndef __APP_RSL_H
#define __APP_RSL_H

/* Includes */
#include "BehaviourController.h"

/* Typedefinitions */
typedef enum APP_RSL_INTERNAL_RETURN_VALUES{
  APP_RSL_INTERNAL_RETURN_VALUES_OK     = 0,
  APP_RSL_INTERNAL_RETURN_VALUES_REPEAT = 1,
} APP_RSL_INTERNAL_RETURN_VALUES_TYPEDEF;

typedef enum APP_RSL_COMMUNICATION_RETURN_VALUES{
  APP_RSL_COMMUNICATION_RETURN_VALUE_NO_READOUT = 0,
  APP_RSL_COMMUNICATION_RETURN_VALUE_READOUT    = 1,
} APP_RSL_COMMUNICATION_RETURN_VALUES_TYPEDEF;
/* Variables */

/* Function definitions */
APP_RSL_INTERNAL_RETURN_VALUES_TYPEDEF app_rsl_handler_executeCommunication(uint32_t ledOnTime, uint32_t ledOffTime, void (*ledOnFunction)(), BEHAVIOUR_CONTROLLER_CALL_STRUCT_TYPEDEF* (*commfunction)());
bool app_rsl_updateAllCharacteristics(void (*ledOnFunction)());
bool app_rsl_broadcast(void (*ledOnFunction)());
bool app_rsl_emergency(void (*ledOnFunction)());
bool app_rsl_pairing(void (*ledOnFunction)());

#endif