/**
  ******************************************************************************
  * @file    	Handler_Timer.h
  * @author	Tim Steinberg
  * @date      	18.05.2020
  * @brief   	Code to handle timing in software
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
  * | 001       | 2020-05-18    | Tim Steinberg         | Initial version / skeleton of file            |
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

#ifndef __HANDLER_TIMER_H
#define __HANDLER_TIMER_H

/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "Test_Selector.h"
#include "MasterDefine.h"

/* Typedefinitions */

/**
 * @brief This typedef defines the state values of a timer.
 *        ACTIVE = Timer is running
 *        INACTIVE = Timer is paused
 */
typedef enum TIMER_FLAG_ACTIVE_VALUES {
  TIMER_FLAG_ACTIVE             = 0x01, /**< The timer is active */
  TIMER_FLAG_INACTIVE           = 0x00, /**< The timer is inactive */
} TIMER_FLAG_ACTIVE_VALUES_TYPEDEF;

/**
 * @brief This struct definition contains all variables needed for the timer
 */
typedef struct TIMER_STRUCT {
  uint32_t time_targetTime;                     /**< Target time.*/
  uint32_t time_waitTime;                       /**< Waiting time till timeout.*/
  TIMER_FLAG_ACTIVE_VALUES_TYPEDEF flag_active; /**< Timer activity state.*/
  uint32_t time_startTime;                      /**< Time when timer was started.*/
} TIMER_STRUCT_TYPEDEF;

/* Variables */

/* Function definitions */

/** @brief This method will start the given timer with the set values
 *  @param *timer Pointer to the timer object.
 *  @param time The actual time you want it to start at
 *  @param timeouttime The (wait)time it has to wait until it times out
 *  @return Nothing.
 */
void                                    timerHandler_timerStart(TIMER_STRUCT_TYPEDEF *timer, uint32_t time, uint32_t timeouttime);

/** @brief This method will check the active flag of the timer.
 *  @param *timer Pointer to the timer object.
 *  @return The active flag.
 */
TIMER_FLAG_ACTIVE_VALUES_TYPEDEF        timerHandler_isTimerActive(TIMER_STRUCT_TYPEDEF *timer);

/** @brief This method will check whether the timer has timed out or not
 *  @param *timer Pointer to the timer object.
 *  @param time The actual time
 *  @return TRUE (== TIMED OUT) OR FALSE (== NOT TIMED OUT).
 */
bool                                    timerHandler_isTimerTimedOut(TIMER_STRUCT_TYPEDEF *timer, uint32_t time);

/** @brief This method will stop a timer
 *  @param *timer Pointer to the timer object.
 *  @return Nothing.
 */
void                                    timerHandler_timerStop(TIMER_STRUCT_TYPEDEF  *timer);

/** @brief This method will restart a timer. Take note that the last entered
 *         waittime will be used.
 *  @param *timer Pointer to the timer object.
 *  @param time The actual time
 *  @return Nothing.
 */
void                                    timerHandler_timerRestart(TIMER_STRUCT_TYPEDEF *timer, uint32_t time);

#if TEST_HANDLER_TIMER >= 1

/** @brief This method is the test for this unit
 *  @return The returnvalue. 0 == OK, <0 == FAILURE
 */
int timerHandler_testsuite();

#endif

#endif