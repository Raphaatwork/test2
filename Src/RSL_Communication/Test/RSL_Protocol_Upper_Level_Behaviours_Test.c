/**
  ******************************************************************************
  * @file       RSL_Protocol_Upper_Level_Behaviours_Test.c
  * @author     Tim Steinberg
  * @date       03.06.2020
  * @brief      Test option and runner for all behaviours
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
  * | 001       | 2020-06-03    | Tim Steinberg         | Initial version / skeleton of file            |
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

#include "UART_RSL.h"

#include "BehaviourController.h"

#include "BehaviourV115_Pairing.h"
#include "BehaviourV115_Alert.h"
#include "BehaviourV115_Error.h"
#include "BehaviourV115_Battery.h"
#include "BehaviourV115_ResetAlert.h"
#include "BehaviourV115_SetAllCharacteristics.h"
    
/* Typedefinitions */

/* Variables */

/* Function definitions */

void rslProtocolTestsuite_testsUpperLevel_behaviours(){
  
#if TEST_GROUP_UPPER_LEVEL_BEHAVIOURS_ACTIVE >= 1
  int32_t retVal;
  Uart_Rsl_StartReception();
#endif 
  
#if TEST_BEHAVIOUR_CONTROLLER >= 1
  retVal = behaviourController_testsuite();
  TRACE_TEST_VALUES(1, "TEST BehaviourController.c %i", retVal);
  if (retVal < 0){
    do{}while(1);
  }
#endif
  
//==============================================//
//      V115                                    //
//==============================================//
  
#if TEST_BEHAVIOUR_PAIRING_V115 >= 1
  retVal = behaviourV115_pairing_testsuite();
  TRACE_TEST_VALUES(1, "TEST BehaviourV115_Pairing.c %i", retVal);
  if (retVal < 0){
    do{}while(1);
  }
#endif
  
#if TEST_BEHAVIOUR_ALERT_V115 >= 1
  retVal = behaviourV115_alert_testsuite();
  TRACE_TEST_VALUES(1, "TEST BehaviourV115_Alert.c %i", retVal);
  if (retVal < 0){
    do{}while(1);
  }
#endif
  
#if TEST_BEHAVIOUR_BATTERY_V115 >= 1
  retVal = behaviourV115_battery_testsuite();
  TRACE_TEST_VALUES(1, "TEST BehaviourV115_Battery.c %i", retVal);
  if (retVal < 0){
    do{}while(1);
  }
#endif
  
#if TEST_BEHAVIOUR_ERROR_V115 >= 1
  retVal = behaviourV115_error_testsuite();
  TRACE_TEST_VALUES(1, "TEST BehaviourV115_Error.c %i", retVal);
  if (retVal < 0){
    do{}while(1);
  }
#endif
  
#if TEST_BEHAVIOUR_RESETALERT_V115 >= 1
  retVal = behaviourV115_resetAlert_testsuite();
  TRACE_TEST_VALUES(1, "TEST BehaviourV115_ResetAlert.c %i", retVal);
  if (retVal < 0){
    do{}while(1);
  }
#endif
  
#if TEST_BEHAVIOUR_SETALLCHARACTERISTICS_V115 >= 1
  retVal = behaviourV115_resetAlert_testsuite();
  TRACE_TEST_VALUES(1, "TEST BehaviourV115_ResetAlert.c %i", retVal);
  if (retVal < 0){
    do{}while(1);
  }
#endif
  
}
