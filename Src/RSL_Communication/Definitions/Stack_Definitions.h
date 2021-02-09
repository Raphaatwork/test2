/**
  ******************************************************************************
  * @file       Stack_Definitions.h
  * @author     Tim Steinberg
  * @date       26.05.2020
  * @brief      Defines for several settings regarding the stack
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
  * | 001       | 2020-05-26    | Tim Steinberg         | Initial version / skeleton of file            |
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

#ifndef __STACK_DEFINITIONS_H
#define __STACK_DEFINITIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>

#define RINGBUFFER_SIZE         256

#define LOGIC_MIB_SLOTCOUNT     5
#define LOGIC_MIB_BUFFERSIZE    40

#define LOGIC_MOB_SLOTCOUNT     5
#define LOGIC_MOB_BUFFERSIZE    40

#define PARSER_MESSAGE_INTEGRITY_TIMEOUT_LENGTH_MS 100

#if PARSER_MESSAGE_INTEGRITY_TIMEOUT_LENGTH_MS < 10
  #warning MAKE SURE THAT YOU CALL THE PARSER OFTEN ENOUGH!
  #warning IF YOU TAKE MORE TIME BETWEEN TWO CALLS THAN THE TIME GIVEN,
  #warning THEN YOU MIGHT ALWAYS [TAUTOLOGICAL] GET ERRONEOUS MESSAGES
#endif

#if PARSER_MESSAGE_INTEGRITY_TIMEOUT_LENGTH_MS > 1000
  #warning ARE YOU SURE ABOUT THIS SETTING?
  #warning THAT MEANS YOU LOOK AFTER NEW MESSAGES SLOWER THAN ONCE A SEC!
#endif

#if RINGBUFFER_SIZE < 64
  #warning YOU MIGHT HAVE PROBLEMS WITH SLOWER SETTINGS, BECAUSE OF OVERFLOWS
#endif

#if LOGIC_MIB_SLOTCOUNT < 5
  #warning YOU MIGHT HAVE PROBLEMS IF YOU HAVE DEEP LOGICAL INTERLEAVING
#endif

#if LOGIC_MOB_SLOTCOUNT < 5
  #warning YOU MIGHT HAVE PROBLEMS IF YOU HAVE DEEP LOGICAL INTERLEAVING
#endif

#if LOGIC_MIB_BUFFERSIZE < 36
  #warning YOU MIGHT HAVE PROBLEMS IF YOU HAVE LONG MESSAGES
#endif

#if LOGIC_MOB_BUFFERSIZE < 36
  #warning YOU MIGHT HAVE PROBLEMS IF YOU HAVE LONG MESSAGES
#endif

#if LOGIC_MIB_BUFFERSIZE != LOGIC_MOB_BUFFERSIZE
  #warning DO YOU REALLY HAVE ASYMETRICAL MESSAGE BUILDUPS?
#endif

#if TEST_MESSAGEIOBUFFER >= 1

  #define MESSAGEIOBUFFER_TEST_BUFFERSIZE  40
  #define MESSAGEIOBUFFER_TEST_SLOTCOUNT   10

  #if MESSAGEIOBUFFER_TEST_SLOTCOUNT < 9
    #error YOU CAN NOT RUN THE TESTS WITH THESE LOW SLOTCOUNTS
  #endif

  #if (MESSAGEIOBUFFER_TEST_BUFFERSIZE >> 5) == 0
    #error THIS BUFFER IS TOO SHORT FOR THE TEST
  #endif

#endif

#if TEST_PARSER >= 1
  #define PARSER_TEST_BUFFERSIZE  40
  #define PARSER_TEST_SLOTCOUNT   10
#endif

#endif 