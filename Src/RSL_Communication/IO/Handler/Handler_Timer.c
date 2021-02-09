/**
  ******************************************************************************
  * @file    	Handler_Timer.c
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

/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "Test_Selector.h"
#include "MasterDefine.h"
#include "Variable_Definitions.h"
#include "Handler_Timer.h"

/* Typedefinitions */

/* Variables */

/* Function definitions */

/** @brief This method will start the given timer with the set values
 *  @param *timer Pointer to the timer object.
 *  @param time The actual time you want it to start at
 *  @param timeouttime The (wait)time it has to wait until it times out
 *  @return Nothing.
 */
void timerHandler_timerStart(TIMER_STRUCT_TYPEDEF *timer, uint32_t time, uint32_t timeouttime){
  timer->time_targetTime = time + timeouttime;
  timer->flag_active = TIMER_FLAG_ACTIVE;
  timer->time_waitTime = timeouttime;
  timer->time_startTime = time;
}

/** @brief This method will check the active flag of the timer.
 *  @param *timer Pointer to the timer object.
 *  @return The active flag.
 */
TIMER_FLAG_ACTIVE_VALUES_TYPEDEF timerHandler_isTimerActive(TIMER_STRUCT_TYPEDEF *timer){
  return timer->flag_active;
}

/** @brief This method will check whether the timer has timed out or not
 *  @param *timer Pointer to the timer object.
 *  @param time The actual time
 *  @return TRUE (== TIMED OUT) OR FALSE (== NOT TIMED OUT).
 */
bool timerHandler_isTimerTimedOut(TIMER_STRUCT_TYPEDEF *timer, uint32_t time){
  if (timer->flag_active == TIMER_FLAG_INACTIVE){
    return FALSE;
  }
  
  // Is the case a turnaround?
  if (timer->time_startTime > timer->time_targetTime){
    // Yes
    
    // Is the time NOT between set time and timeout time?
    if ((time < timer->time_startTime) && (time > timer->time_targetTime)){
      return TRUE;
    }
  }else{
    // No
    
    // Is the time BEFORE the setting point?
    if (time < timer->time_startTime){
      return TRUE;
    }
    // Is the target time passed=
    if (time > timer->time_targetTime){
      return TRUE;
    }
  }
  return FALSE;
}

/** @brief This method will stop a timer
 *  @param *timer Pointer to the timer object.
 *  @return Nothing.
 */
void timerHandler_timerStop(TIMER_STRUCT_TYPEDEF  *timer){
  timer->flag_active = TIMER_FLAG_INACTIVE;
}

/** @brief This method will restart a timer. Take note that the last entered
 *         waittime will be used.
 *  @param *timer Pointer to the timer object.
 *  @param time The actual time
 *  @return Nothing.
 */
void timerHandler_timerRestart(TIMER_STRUCT_TYPEDEF *timer, uint32_t time){
  timerHandler_timerStart(timer, time, timer->time_waitTime);
}

#if TEST_HANDLER_TIMER >= 1

/** @brief This method will free variables and set back things to return from 
 *         the test.
 *  @param retVal The value you want to return.
 *  @return The returnvalue you enter.
 */
int timerHandler_testsuiteReturner(int32_t retVal){
  return retVal;
}

/** @brief This method is the test for this unit
 *  @return The returnvalue. 0 == OK, <0 == FAILURE
 */
int timerHandler_testsuite(){
  TIMER_STRUCT_TYPEDEF testTimer = {
    .time_targetTime = 0,
    .time_waitTime = 0,
    .flag_active = TIMER_FLAG_INACTIVE,    
  };
  
  //===================== ORDINARY - TIMER TIMEOUT
  
  timerHandler_timerStart(&testTimer, 0x10000000, 0x64); //time = 268.435.456 target = 268.435.556
  if (testTimer.flag_active != TIMER_FLAG_ACTIVE){
    return timerHandler_testsuiteReturner(-1);
  }
  if (testTimer.time_targetTime != 0x10000064){
    return timerHandler_testsuiteReturner(-1);
  }
  if (testTimer.time_waitTime != 0x64){
    return timerHandler_testsuiteReturner(-1);
  }
  if (timerHandler_isTimerActive(&testTimer) != TIMER_FLAG_ACTIVE){
    return timerHandler_testsuiteReturner(-1);
  }
  if (timerHandler_isTimerTimedOut(&testTimer, 0x10000000) == TRUE){
    return timerHandler_testsuiteReturner(-1);
  }
  
  // Check if timer reacts before time point passes
  if (timerHandler_isTimerTimedOut(&testTimer, 0x00000000) == FALSE){
    return timerHandler_testsuiteReturner(-1);
  }
  if (timerHandler_isTimerTimedOut(&testTimer, 0x10000063) == TRUE){
    return timerHandler_testsuiteReturner(-1);
  }
  if (timerHandler_isTimerTimedOut(&testTimer, 0x10000064) == TRUE){
    return timerHandler_testsuiteReturner(-1);
  }
  if (timerHandler_isTimerTimedOut(&testTimer, 0x10000065) == FALSE){
    return timerHandler_testsuiteReturner(-1);
  }
  if (timerHandler_isTimerTimedOut(&testTimer, 0xFFFFFFFF) == FALSE){
    return timerHandler_testsuiteReturner(-1);
  }
  
  // Check if timer stop works
  timerHandler_timerStop(&testTimer);
  if (timerHandler_isTimerActive(&testTimer) == TIMER_FLAG_ACTIVE){
    return timerHandler_testsuiteReturner(-1);
  }
  
  //===================== CLOSE EDGE CASES BEFORE OVERFLOW - TIMER TIMEOUT

  timerHandler_timerStart(&testTimer, 0xFFFFF000, 0xC8); //time = 4.294.963.200 target = 4.294.963.400
  if (testTimer.flag_active != TIMER_FLAG_ACTIVE){
    return timerHandler_testsuiteReturner(-1);
  }
  if (testTimer.time_targetTime != 0xFFFFF0C8){
    return timerHandler_testsuiteReturner(-1);
  }
  if (testTimer.time_waitTime != 0xC8){
    return timerHandler_testsuiteReturner(-1);
  }
  if (timerHandler_isTimerActive(&testTimer) != TIMER_FLAG_ACTIVE){
    return timerHandler_testsuiteReturner(-1);
  }
  if (timerHandler_isTimerTimedOut(&testTimer, 0xFFFFF000) == TRUE){
    return timerHandler_testsuiteReturner(-1);
  }
  
  // Check if timer reacts before time point passes
  if (timerHandler_isTimerTimedOut(&testTimer, 0x00000000) == FALSE){
    return timerHandler_testsuiteReturner(-1);
  }
  if (timerHandler_isTimerTimedOut(&testTimer, 0xFFFFF0C7) == TRUE){
    return timerHandler_testsuiteReturner(-1);
  }
  if (timerHandler_isTimerTimedOut(&testTimer, 0xFFFFF0C8) == TRUE){
    return timerHandler_testsuiteReturner(-1);
  }
  if (timerHandler_isTimerTimedOut(&testTimer, 0xFFFFF0C9) == FALSE){
    return timerHandler_testsuiteReturner(-1);
  }
  if (timerHandler_isTimerTimedOut(&testTimer, 0xFFFFFFFF) == FALSE){
    return timerHandler_testsuiteReturner(-1);
  }
  
  // Check if timer stop works
  timerHandler_timerStop(&testTimer);
  if (timerHandler_isTimerActive(&testTimer) == TIMER_FLAG_ACTIVE){
    return timerHandler_testsuiteReturner(-1);
  }
  
  //===================== CLOSE EDGE CASES AFTER OVERFLOW - TIMER TIMEOUT

  timerHandler_timerStart(&testTimer, 0x00000F00, 0x32); //time = 3.840 target = 3.890
  if (testTimer.flag_active != TIMER_FLAG_ACTIVE){
    return timerHandler_testsuiteReturner(-1);
  }
  if (testTimer.time_targetTime != 0x00000F32){
    return timerHandler_testsuiteReturner(-1);
  }
  if (testTimer.time_waitTime != 0x32){
    return timerHandler_testsuiteReturner(-1);
  }
  if (timerHandler_isTimerActive(&testTimer) != TIMER_FLAG_ACTIVE){
    return timerHandler_testsuiteReturner(-1);
  }
  if (timerHandler_isTimerTimedOut(&testTimer, 0x00000F00) == TRUE){
    return timerHandler_testsuiteReturner(-1);
  }
  
  // Check if timer reacts before time point passes
  if (timerHandler_isTimerTimedOut(&testTimer, 0x00000000) == FALSE){
    return timerHandler_testsuiteReturner(-1);
  }
  if (timerHandler_isTimerTimedOut(&testTimer, 0x00000F31) == TRUE){
    return timerHandler_testsuiteReturner(-1);
  }
  if (timerHandler_isTimerTimedOut(&testTimer, 0x00000F32) == TRUE){
    return timerHandler_testsuiteReturner(-1);
  }
  if (timerHandler_isTimerTimedOut(&testTimer, 0x00000F33) == FALSE){
    return timerHandler_testsuiteReturner(-1);
  }
  if (timerHandler_isTimerTimedOut(&testTimer, 0xFFFFFFFF) == FALSE){
    return timerHandler_testsuiteReturner(-1);
  }
  
  // Check if timer stop works
  timerHandler_timerStop(&testTimer);
  if (timerHandler_isTimerActive(&testTimer) == TIMER_FLAG_ACTIVE){
    return timerHandler_testsuiteReturner(-1);
  }
  
  //===================== OVERFLOW CASES - TIMER TIMEOUT

  timerHandler_timerStart(&testTimer, 0xFFFFFF00, 0x1000); //time = 4.294.967.040 target = 3.840
  if (testTimer.flag_active != TIMER_FLAG_ACTIVE){
    return timerHandler_testsuiteReturner(-1);
  }
  if (testTimer.time_targetTime != 0x00000F00){
    return timerHandler_testsuiteReturner(-1);
  }
  if (testTimer.time_waitTime != 0x1000){
    return timerHandler_testsuiteReturner(-1);
  }
  if (timerHandler_isTimerActive(&testTimer) != TIMER_FLAG_ACTIVE){
    return timerHandler_testsuiteReturner(-1);
  }
  if (timerHandler_isTimerTimedOut(&testTimer, 0xFFFFFF00) == TRUE){
    return timerHandler_testsuiteReturner(-1);
  }
  
  // Check if timer reacts before time point passes
  if (timerHandler_isTimerTimedOut(&testTimer, 0xFFFFFEFF) == FALSE){
    return timerHandler_testsuiteReturner(-1);
  }
  if (timerHandler_isTimerTimedOut(&testTimer, 0xFFFFFFFF) == TRUE){
    return timerHandler_testsuiteReturner(-1);
  }
  if (timerHandler_isTimerTimedOut(&testTimer, 0x00000000) == TRUE){
    return timerHandler_testsuiteReturner(-1);
  }
  if (timerHandler_isTimerTimedOut(&testTimer, 0x00000EFF) == TRUE){
    return timerHandler_testsuiteReturner(-1);
  }
  if (timerHandler_isTimerTimedOut(&testTimer, 0x00000F00) == TRUE){
    return timerHandler_testsuiteReturner(-1);
  }
  if (timerHandler_isTimerTimedOut(&testTimer, 0x00000F01) == FALSE){
    return timerHandler_testsuiteReturner(-1);
  }
  if (timerHandler_isTimerTimedOut(&testTimer, 0x10000000) == FALSE){
    return timerHandler_testsuiteReturner(-1);
  }
  
  // Check if timer stop works
  timerHandler_timerStop(&testTimer);
  if (timerHandler_isTimerActive(&testTimer) == TIMER_FLAG_ACTIVE){
    return timerHandler_testsuiteReturner(-1);
  }
  
  //===================== DIFFERENT - TIMER RESTART
  
  timerHandler_timerStart(&testTimer, 0xE0000000, 0x100F0000); //time = 3.758.096.384 target = 4.026.593.280
  
  if (testTimer.time_targetTime != 0xF00F0000){
    return timerHandler_testsuiteReturner(-1);
  }
  if (testTimer.time_waitTime != 0x100F0000){
    return timerHandler_testsuiteReturner(-1);
  }
  
  timerHandler_timerRestart(&testTimer, 0xF00F0000); //time = 4.026.593.280 target = 269.418.496
  if (testTimer.time_targetTime != 0x001E0000){
    return timerHandler_testsuiteReturner(-1);
  }
  if (testTimer.time_waitTime != 0x100F0000){
    return timerHandler_testsuiteReturner(-1);
  }
  
  timerHandler_timerRestart(&testTimer, 0x001E0000); //time = 1.966.080 target = 271.384.576

  if (testTimer.time_targetTime != 0x102D0000){
    return timerHandler_testsuiteReturner(-1);
  }
  if (testTimer.time_waitTime != 0x100F0000){
    return timerHandler_testsuiteReturner(-1);
  }
  
  timerHandler_timerStart(&testTimer, 0xFFFF0000, 0x100F0000); //time = 4.294.901.760 target = 269.352.960
  if (testTimer.time_targetTime != 0x100E0000){
    return timerHandler_testsuiteReturner(-1);
  }
  if (testTimer.time_waitTime != 0x100F0000){
    return timerHandler_testsuiteReturner(-1);
  }
  timerHandler_timerRestart(&testTimer, 0x100E0000); //time = 269.352.960 target = 538.771.456

  if (testTimer.time_targetTime != 0x201D0000){
    return timerHandler_testsuiteReturner(-1);
  }
  if (testTimer.time_waitTime != 0x100F0000){
    return timerHandler_testsuiteReturner(-1);
  }
  
  
  return 0;
}
#endif