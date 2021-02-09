/**
  ******************************************************************************
  * @file       App_868MHz_MessageBuilder.c
  * @author     Tim Steinberg
  * @date       09.12.2020
  * @brief      Unit to build messages for the 869.25MHz protocol
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
  * | 001       | 2020-12-09    | Tim Steinberg         | Initial version / skeleton of file            |
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

#ifndef __APP_868MHZ_MESSAGEBUILDER_H
#define __APP_868MHZ_MESSAGEBUILDER_H

/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

/* Typedefinitions */
typedef struct {
  uint8_t bitfield;
  uint16_t bitfieldTimes[8];
} bitfieldTimed;

typedef struct {
  uint32_t countOfBits;
  uint32_t countOfBitsUsed;
  bitfieldTimed *bitsToSend;
} bitfield;

/* Variables */

/* Function definitions */
void    app_868MHz_invertBitfield(bitfield *bits);
int     app_868MHz_buildMessage_preamble(bitfield *bits);
int     app_868MHz_buildMessage_emergency(uint8_t *message, uint32_t messageLength, bitfield *bits);

#endif