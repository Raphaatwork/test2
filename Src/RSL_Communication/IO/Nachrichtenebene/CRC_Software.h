/**
  ******************************************************************************
  * @file       CRC_Software.h
  * @author     Tim Steinberg
  * @date       11.05.2020
  * @brief      This file contains a CRC-Builder for simple XOR of all bytes
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
  * | 001       | 2020-05-11    | Tim Steinberg         | Initial version / skeleton of file            |
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

#ifndef __CRC_SOFTWARE_H
#define __CRC_SOFTWARE_H

/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "Test_Selector.h"

/* Typedefinitions */

/**
 * @brief This typedef defines the return values of the CRC functions
 *        Valid = CRC is good
 *        Invalid = CRC is bad
 */
typedef enum CRC_SOFTWARE_RETURN_VALUES {
  CRC_SOFTWARE_VALID            = 0x00, /**< CRC is valid */
  CRC_SOFTWARE_INVALID          = 0xFF, /**< CRC is invalid */
} CRC_SOFTWARE_RETURN_VALUES_TYPEDEF;

/* Variables */

/* Function definitions */

/** @brief This method will build a CRC for the given field
 *  @param *package Pointer to a buffer
 *  @param length The count of bytes to be used in the buffer
 *  @return The CRC-Byte.
 */
uint8_t CRC_Software_buildCRC(uint8_t *package, uint32_t length);

/** @brief This method will check a CRC for the given field
 *  @param *package Pointer to a buffer
 *  @param length The count of bytes to be used in the buffer
 *  @return VALID or INVALID.
 */
CRC_SOFTWARE_RETURN_VALUES_TYPEDEF CRC_Software_checkCRC(uint8_t *package, uint32_t length);

#if TEST_CRC_SOFTWARE >= 1

/** @brief This method is the test for this unit
 *  @return The returnvalue. 0 == OK, <0 == FAILURE
 */
int CRC_Software_testsuite();

#endif

#endif