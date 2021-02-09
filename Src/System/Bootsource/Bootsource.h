/**
  ******************************************************************************
  * @file       Bootsource.c
  * @author     Tim Steinberg
  * @date       06.07.2020
  * @brief      This code will get the source of the (re-)boot.
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

#ifndef __BOOTSOURCE_H
#define __BOOTSOURCE_H

/* Includes */

/* Typedefinitions */
typedef enum SYSTEMBOOT_SOURCE{
  BOOT_STARTUP_UNKNOWN          = 0,
  BOOT_STARTUP_BATTERY          = 1,
  BOOT_STARTUP_WATCHDOG         = 2,
  BOOT_STARTUP_BUTTON_NOTHING   = 3,
  BOOT_STARTUP_BUTTON_EMERGENCY = 4,
  BOOT_STARTUP_BUTTON_PAIRING   = 5,
  BOOT_STARTUP_BUTTON_BARRELROLL= 6,
} SYSTEMBOOT_SOURCE_TYPEDEF;

/* Variables */

/* Function definitions */
SYSTEMBOOT_SOURCE_TYPEDEF Boot_CheckStartupEvent(void);

#endif