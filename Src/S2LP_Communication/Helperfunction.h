/**
  ******************************************************************************
  * @file       Helperfunction.h
  * @author     Tim Steinberg
  * @date       09.12.2020
  * @brief      Functions that are being used in the 869MHz bitfield things
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

#ifndef __HELPERFUNCTION_H
#define __HELPERFUNCTION_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

////////////////////////////////////////////////
// FUNCTIONS FOR UINT8_T-ARRAYS
////////////////////////////////////////////////

void setBitInPosition(uint8_t *array, uint32_t position);
                           
void unsetBitInPosition(uint8_t *array, uint32_t position);

uint8_t checkBitInPosition(uint8_t *array, uint32_t position);

void flipBitInPosition(uint8_t *array, uint32_t position);

////////////////////////////////////////////////
// FUNCTIONS FOR BITFIELDS WITH TIMINGS
////////////////////////////////////////////////

#include "App_868MHz_MessageBuilder.h"

void initializeBitfield(uint8_t *arrayWithBits, uint32_t bitCount, bitfield *bits, uint16_t timeZero, uint16_t timeOne);

#endif