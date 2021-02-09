/**
  ******************************************************************************
  * @file       GPIO.c
  * @author     Tim Steinberg
  * @date       06.07.2020
  * @brief      Code to handle several special GPIO and their modes.
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
  * | 001       | 2020-07-06    | Tim Steinberg         | Initial version / skeleton of file            |
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

#ifndef __GPIO_H
#define __GPIO_H

/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

/* Typedefinitions */

/* Variables */

/* Function definitions */

void gpio_userButtonArmedMode(void);
void gpio_userButtonMeasureMode(void);
void gpio_userButtonUnarmedMode(void);
void gpio_ledOutputMode(void);
void gpio_ledAnalogMode(void);
void gpio_rsl10WakeupActive(void);
void gpio_rsl10WakeupInactive(void);
void gpio_rsl10ResetActive(void);
void gpio_rsl10ResetInactive(void);

void gpio_initAllAnalog(void);
void gpio_initSleepMode(void);

void gpio_activateTestPin(void);
void gpio_deactivateTestPin(void);

void gpio_s2lp_signalGPO1Output_init(void);
void gpio_s2lp_signalGPO2Output_init(void);
void gpio_s2lp_signalGPO1Output_deInit(void);
void gpio_s2lp_signalGPO2Output_deInit(void);
void gpio_s2lp_signalGPO1Output_set(uint8_t level);
void gpio_s2lp_signalGPO2Output_set(uint8_t level);

void gpio_s2lp_pinShutdown_setToShutdown(void);
void gpio_s2lp_pinShutdown_setToRunmode(void);
void gpio_s2lp_pinCS_init(void);
void gpio_s2lp_pinCS_deInit(void);
void gpio_s2lp_pinCS_setToTransmission(void);
void gpio_s2lp_pinCS_setToSilence(void);

void gpio_tcxo_on(void);
void gpio_tcxo_off(void);
void gpio_xtal_on(void);
void gpio_xtal_off(void);

void gpio_PortAInput(void);
void gpio_PortBInput(void);
void gpio_PortCInput(void);
void gpio_PortDInput(void);
void gpio_PortHInput(void);

#endif