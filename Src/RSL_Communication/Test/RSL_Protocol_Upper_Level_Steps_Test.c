/**
  ******************************************************************************
  * @file       RSL_Protocol_Upper_Level_Steps_Test.c
  * @author     Tim Steinberg
  * @date       02.06.2020
  * @brief      Test option and runner for all steps
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
  * | 001       | 2020-06-02    | Tim Steinberg         | Initial version / skeleton of file            |
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

/* Includes */
#include "Test_Selector.h"
#include "Debug.h"

#include "BehaviourStepV115_Start.h"
#include "BehaviourStepV115_Sleep.h"
#include "BehaviourStepV115_Pairing.h"
#include "BehaviourStepV115_CharAlert.h"
#include "BehaviourStepV115_CharBattery.h"
#include "BehaviourStepV115_CharError.h"
#include "BehaviourStepV115_BC_Start.h"
#include "BehaviourStepV115_BC_End.h"
#include "BehaviourStepV115_BC_Wait_TO.h"
#include "BehaviourStepV115_RemovePD.h"

/* Typedefinitions */

/* Variables */

/* Function definitions */

void rslProtocolTestsuite_testsUpperLevel_steps(){
  
#if TEST_GROUP_UPPER_LEVEL_STEPS_ACTIVE >= 1
  int32_t retVal;
#endif 
  
//==============================================//
//      V115                                    //
//==============================================//
  
#if TEST_BEHAVIOURSTEP_START_V115 >= 1
  retVal = behaviourStep_start_v115_main_test();
  TRACE_TEST_VALUES(1, "TEST BehaviourStepV115_Start.c %i", retVal);
  if (retVal < 0){
    do{}while(1);
  }
#endif
  
#if TEST_BEHAVIOURSTEP_SLEEP_V115 >= 1
  retVal = behaviourStep_sleep_v115_main_test();
  TRACE_TEST_VALUES(1, "TEST BehaviourStepV115_Sleep.c %i", retVal);
  if (retVal < 0){
    do{}while(1);
  }
#endif
  
#if TEST_BEHAVIOURSTEP_PAIRING_V115 >= 1
  retVal = behaviourStep_pairing_v115_main_test();
  TRACE_TEST_VALUES(1, "TEST BehaviourStepV115_Pairing.c %i", retVal);
  if (retVal < 0){
    do{}while(1);
  }
#endif
  
#if TEST_BEHAVIOURSTEP_CHAR_ALERT_V115 >= 1
  retVal = behaviourStep_char_alert_v115_main_test();
  TRACE_TEST_VALUES(1, "TEST BehaviourStepV115_CharAlert.c %i", retVal);
  if (retVal < 0){
    do{}while(1);
  }
#endif
  
#if TEST_BEHAVIOURSTEP_CHAR_BATTERY_V115 >= 1
  retVal = behaviourStep_char_battery_v115_main_test();
  TRACE_TEST_VALUES(1, "TEST BehaviourStepV115_CharBattery.c %i", retVal);
  if (retVal < 0){
    do{}while(1);
  }
#endif
  
#if TEST_BEHAVIOURSTEP_CHAR_ERROR_V115 >= 1
  retVal = behaviourStep_char_error_v115_main_test();
  TRACE_TEST_VALUES(1, "TEST BehaviourStepV115_CharError.c %i", retVal);
  if (retVal < 0){
    do{}while(1);
  }
#endif
  
#if TEST_BEHAVIOURSTEP_BC_START_V115 >= 1
  retVal = behaviourStep_bc_start_v115_main_test();
  TRACE_TEST_VALUES(1, "TEST BehaviourStepV115_BC_Start.c %i", retVal);
  if (retVal < 0){
    do{}while(1);
  }
#endif
  
#if TEST_BEHAVIOURSTEP_BC_END_V115 >= 1
  retVal = behaviourStep_bc_end_v115_main_test();
  TRACE_TEST_VALUES(1, "TEST BehaviourStepV115_BC_End.c %i", retVal);
  if (retVal < 0){
    do{}while(1);
  }
#endif
  
#if TEST_BEHAVIOURSTEP_BC_WAIT_TO_V115 >= 1
  retVal = behaviourStep_bc_wait_to_v115_main_test();
  TRACE_TEST_VALUES(1, "TEST BehaviourStepV115_BC_Wait_TO.c %i", retVal);
  if (retVal < 0){
    do{}while(1);
  }
#endif
  
#if TEST_BEHAVIOURSTEP_REMOVEPD_V115 >= 1
  retVal = behaviourStep_removePD_v115_main_test();
  TRACE_TEST_VALUES(1, "TEST BehaviourStepV115_RemovePD.c %i", retVal);
  if (retVal < 0){
    do{}while(1);
  }
#endif
  
  
  
}
