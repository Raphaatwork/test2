/**
  ******************************************************************************
  * @file       BehaviourController.c
  * @author     Tim Steinberg
  * @date       24.06.2020
  * @brief      Main controller for sync between behaviours and stack
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
  * | 001       | 2020-06-24    | Tim Steinberg         | Initial version / skeleton of file            |
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
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "MasterDefine.h"
#include "BehaviourDefines.h"
#include "BehaviourController.h"
#include "Test_Selector.h"

/* Typedefinitions */

/* Variables */
BEHAVIOUR_CONTROLLER_CALL_STRUCT_TYPEDEF *sequence = NULL;

/* Function definitions */

/** @brief      This method is the loader function for a new sequence. This 
 *              function must be called before you try to call the controllers
 *              main function (to load the sequence you want to execute).
 *  @param *newSequence Pointer to the new sequence
 *  @return Nothing
 */
void behaviourController_loadNewSequence(BEHAVIOUR_CONTROLLER_CALL_STRUCT_TYPEDEF *newSequence){
  // Set the local sequence pointer to the given new sequence
  sequence = newSequence;
  // Set the sequence step to the first behaviour
  sequence->numberOfActualBehaviour = sequence->initializingBehaviour;
  // And set the initer to TRUE
  sequence->initNext = TRUE;
}

/** @brief      Prior to this function CALL THE LOADER TO LOAD THE SEQUENCE. 
 *              The controller will execute the loaded behaviour.
 *  @return The result of the execution which might be
 *          BEHAVIOUR_CONTROLLER_RETURN_ONGOING                   = 0x00, // Tell upper layer that nothing happened 
 *          BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT         = 0x01, // Tell upper layer that nothing happened, but the next step was loaded (for debug purpose) 
 *          BEHAVIOUR_CONTROLLER_RETURN_FINISHED                  = 0x10, // Tell upper layer that the behaviour is finished 
 *          BEHAVIOUR_CONTROLLER_RETURN_CRITICAL_ERROR            = 0xFF, // Tell upper layer that things are bad 
 */
BEHAVIOUR_CONTROLLER_RETURN_VALUES_TYPEDEF behaviourController_main(){
  BEHAVIOUR_STEP_RETURN_VALUES_TYPEDEF behaviourSequenceReturnValue;
  BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF behaviourSequenceCommandReturnValue;
  // The "executive" part -> call the sequence main function
  behaviourSequenceReturnValue = sequence->stepsOfSequence[sequence->numberOfActualBehaviour].behaviourMainFunction(sequence->initNext);
  sequence->initNext = FALSE;
  // The "reactive" part -> call the sequence callback function (for that step)
  switch(behaviourSequenceReturnValue){
    /** This case will call the callback for the nothing case */
    case BEHAVIOUR_STEP_RETURN_NOTHING:
      behaviourSequenceCommandReturnValue = sequence->stepsOfSequence[sequence->numberOfActualBehaviour].behaviourCallbackFunctionNothing();
      break;
    /** This case will call the callback for the action a case */
    case BEHAVIOUR_STEP_RETURN_ACTION_A:
      behaviourSequenceCommandReturnValue = sequence->stepsOfSequence[sequence->numberOfActualBehaviour].behaviourCallbackFunctionAction_A();
      break;
    /** This case will call the callback for the action b case */
    case BEHAVIOUR_STEP_RETURN_ACTION_B:
      behaviourSequenceCommandReturnValue = sequence->stepsOfSequence[sequence->numberOfActualBehaviour].behaviourCallbackFunctionAction_B();
      break;
    /** This case will call the callback for the action c case */
    case BEHAVIOUR_STEP_RETURN_ACTION_C:
      behaviourSequenceCommandReturnValue = sequence->stepsOfSequence[sequence->numberOfActualBehaviour].behaviourCallbackFunctionAction_C();
      break;
    /** This case will call the callback for the action d case */
    case BEHAVIOUR_STEP_RETURN_ACTION_D:
      behaviourSequenceCommandReturnValue = sequence->stepsOfSequence[sequence->numberOfActualBehaviour].behaviourCallbackFunctionAction_D();
      break;
    /** This case will call the callback for the next step case */
    case BEHAVIOUR_STEP_RETURN_NEXT_BEHAVIOUR_STEP:
      behaviourSequenceCommandReturnValue = sequence->stepsOfSequence[sequence->numberOfActualBehaviour].behaviourCallbackFunctionNextBehaviour();
      break;
    /** This case will call the callback for the redo step case */
    case BEHAVIOUR_STEP_RETURN_REDO:
      behaviourSequenceCommandReturnValue = sequence->stepsOfSequence[sequence->numberOfActualBehaviour].behaviourCallbackFunctionRedo();
      break;
    /** This case will call the callback for the abort case */
    case BEHAVIOUR_STEP_RETURN_ABORT:
      behaviourSequenceCommandReturnValue = sequence->stepsOfSequence[sequence->numberOfActualBehaviour].behaviourCallbackFunctionAbort();
      break;
    /** This case will call the callback for the critical case */
    case BEHAVIOUR_STEP_RETURN_CRITICAL_ABORT:
      behaviourSequenceCommandReturnValue = sequence->stepsOfSequence[sequence->numberOfActualBehaviour].behaviourCallbackFunctionCriticalAbort();
      break;
    /** This case will handle all unknown cases */
    default :
      return BEHAVIOUR_CONTROLLER_RETURN_CRITICAL_ERROR;
      break;
  }
  // The "handling" part in the behaviour controller -> handle the return value by doing what it stands for and return the result to the above layer
  switch (behaviourSequenceCommandReturnValue.command){
    /** This case will handle the nothing return value from the sequence callback */
    case BEHAVIOUR_CONTROLLER_DO_NOTHING: 
      return BEHAVIOUR_CONTROLLER_RETURN_ONGOING;
      break;
    /** This case will handle the reload return value from the sequence callback */
    case BEHAVIOUR_CONTROLLER_RELOAD_BEHAVIOUR:
      sequence->initNext = TRUE;
      return BEHAVIOUR_CONTROLLER_RETURN_ONGOING;
      break;
    /** This case will handle the load next return value from the sequence callback */
    case BEHAVIOUR_CONTROLLER_LOAD_NEXT_BEHAVIOUR_STEP:
      if (behaviourSequenceCommandReturnValue.nextStep == BEHAVIOUR_SEQUENCING_STEP_UNDEFINED){
        return BEHAVIOUR_CONTROLLER_RETURN_CRITICAL_ERROR;
      }
      sequence->initNext = TRUE;
      sequence->numberOfActualBehaviour = behaviourSequenceCommandReturnValue.nextStep;
      return BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT;
      break;
    /** This case will handle the finished return value from the sequence callback */
    case BEHAVIOUR_CONTROLLER_FINISHED_BEHAVIOUR: 
      return BEHAVIOUR_CONTROLLER_RETURN_FINISHED;
      break;
    /** This case will handle the critical return value from the sequence callback */
    case BEHAVIOUR_CONTROLLER_RETURN_CRITICAL: 
      return BEHAVIOUR_CONTROLLER_RETURN_CRITICAL_ERROR;
      break;
    /** This case will handle all the cases with unknown return values */
    default :
      return BEHAVIOUR_CONTROLLER_RETURN_CRITICAL_ERROR;
      break;
  }
}

#if TEST_BEHAVIOUR_CONTROLLER >= 1

uint32_t testBehaviourFunctionCRepetitioncounter = 0x3C;
uint32_t testBehaviourStartingCounter = 0x00;

// This function will set up the whole thing to pass the test
BEHAVIOUR_STEP_RETURN_VALUES_TYPEDEF testSequence_Initializer(bool init){
  testBehaviourFunctionCRepetitioncounter = 0;
  testBehaviourStartingCounter++;
  return BEHAVIOUR_STEP_RETURN_NEXT_BEHAVIOUR_STEP;
}

BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF testSequence_Initializer_DoNothing(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF testSequence_Initializer_LoadNextStep(){
  if (testBehaviourStartingCounter <= 2){
    return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_LOAD_NEXT_BEHAVIOUR_STEP, .nextStep = BEHAVIOUR_SEQUENCING_STEP_1};
  }else{
    return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_LOAD_NEXT_BEHAVIOUR_STEP, .nextStep = BEHAVIOUR_SEQUENCING_STEP_4};
  }
}

BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF testSequence_Initializer_Redo(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF testSequence_Initializer_Abort(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF testSequence_Initializer_CriticalAbort(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// This function will make the sequence load step C
BEHAVIOUR_STEP_RETURN_VALUES_TYPEDEF testSequence_FunctionA(bool init){
  static uint32_t testBehaviourFunctionAInternalState = 0;
  if (init) testBehaviourFunctionAInternalState = 0;  
  if (++testBehaviourFunctionAInternalState == 10){
    return BEHAVIOUR_STEP_RETURN_NEXT_BEHAVIOUR_STEP;
  }
  return BEHAVIOUR_STEP_RETURN_NOTHING;
}

BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF testSequence_FunctionA_DoNothing(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF testSequence_FunctionA_LoadNextStep(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_LOAD_NEXT_BEHAVIOUR_STEP, .nextStep = BEHAVIOUR_SEQUENCING_STEP_3};
}

BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF testSequence_FunctionA_Redo(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF testSequence_FunctionA_Abort(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF testSequence_FunctionA_CriticalAbort(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// This function will end the sequence
BEHAVIOUR_STEP_RETURN_VALUES_TYPEDEF testSequence_FunctionB(bool init){
  static uint32_t testBehaviourFunctionBInternalState = 0;
  if (init) testBehaviourFunctionBInternalState = 0;  
  if (++testBehaviourFunctionBInternalState == 20){
    return BEHAVIOUR_STEP_RETURN_NEXT_BEHAVIOUR_STEP;
  }
  return BEHAVIOUR_STEP_RETURN_NOTHING;
}

BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF testSequence_FunctionB_DoNothing(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF testSequence_FunctionB_LoadNextStep(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_FINISHED_BEHAVIOUR, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF testSequence_FunctionB_Redo(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF testSequence_FunctionB_Abort(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF testSequence_FunctionB_CriticalAbort(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// This function will return REDO, so it will be run 3 times
BEHAVIOUR_STEP_RETURN_VALUES_TYPEDEF testSequence_FunctionC(bool init){
  static uint32_t testBehaviourFunctionCInternalState = 0;
  if (init) testBehaviourFunctionCInternalState = 0;  
  if (++testBehaviourFunctionCInternalState == 30){
    if (testBehaviourFunctionCRepetitioncounter >= 2){
      return BEHAVIOUR_STEP_RETURN_NEXT_BEHAVIOUR_STEP;
    }else{
      return BEHAVIOUR_STEP_RETURN_REDO;
    }
  }
  return BEHAVIOUR_STEP_RETURN_NOTHING;
}

BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF testSequence_FunctionC_DoNothing(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF testSequence_FunctionC_LoadNextStep(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_LOAD_NEXT_BEHAVIOUR_STEP, .nextStep = BEHAVIOUR_SEQUENCING_STEP_2};
}

BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF testSequence_FunctionC_Redo(){
  testBehaviourFunctionCRepetitioncounter++;
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_RELOAD_BEHAVIOUR, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF testSequence_FunctionC_Abort(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF testSequence_FunctionC_CriticalAbort(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

// This function will return REDO, so it will be run 3 times
BEHAVIOUR_STEP_RETURN_VALUES_TYPEDEF testSequence_FunctionD(bool init){
  return BEHAVIOUR_STEP_RETURN_CRITICAL_ABORT;
}

BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF testSequence_FunctionD_DoNothing(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF testSequence_FunctionD_LoadNextStep(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF testSequence_FunctionD_Redo(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF testSequence_FunctionD_Abort(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_DO_NOTHING, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF testSequence_FunctionD_CriticalAbort(){
  return (BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF){.command = BEHAVIOUR_CONTROLLER_RETURN_CRITICAL, .nextStep = BEHAVIOUR_SEQUENCING_STEP_UNDEFINED};
}

BEHAVIOUR_CONTROLLER_CALL_STRUCT_TYPEDEF behaviourControllerTestSequence = {
  .initializingBehaviour = BEHAVIOUR_SEQUENCING_STEP_START,
  .numberOfActualBehaviour = BEHAVIOUR_SEQUENCING_STEP_END,
  .initNext = FALSE,
  .stepsOfSequence = {
    // BEHAVIOUR_SEQUENCING_STEP_START
    {
      .behaviourMainFunction                    = testSequence_Initializer,
      .behaviourCallbackFunctionNothing         = testSequence_Initializer_DoNothing,
      /*
      .behaviourCallbackFunctionAction_A        = NULL,
      .behaviourCallbackFunctionAction_B        = NULL,
      .behaviourCallbackFunctionAction_C        = NULL,
      .behaviourCallbackFunctionAction_D        = NULL,
      */
      .behaviourCallbackFunctionNextBehaviour   = testSequence_Initializer_LoadNextStep,
      .behaviourCallbackFunctionRedo            = testSequence_Initializer_Redo,
      .behaviourCallbackFunctionAbort           = testSequence_Initializer_Abort,
      .behaviourCallbackFunctionCriticalAbort   = testSequence_Initializer_CriticalAbort,
    },
    // BEHAVIOUR_SEQUENCING_STEP_1
    {
      .behaviourMainFunction                    = testSequence_FunctionA,
      .behaviourCallbackFunctionNothing         = testSequence_FunctionA_DoNothing,
      /*
      .behaviourCallbackFunctionAction_A        = NULL,
      .behaviourCallbackFunctionAction_B        = NULL,
      .behaviourCallbackFunctionAction_C        = NULL,
      .behaviourCallbackFunctionAction_D        = NULL,
      */
      .behaviourCallbackFunctionNextBehaviour   = testSequence_FunctionA_LoadNextStep,
      .behaviourCallbackFunctionRedo            = testSequence_FunctionA_Redo,
      .behaviourCallbackFunctionAbort           = testSequence_FunctionA_Abort,
      .behaviourCallbackFunctionCriticalAbort   = testSequence_FunctionA_CriticalAbort,
    },
    // BEHAVIOUR_SEQUENCING_STEP_2
    {
      .behaviourMainFunction                    = testSequence_FunctionB,
      .behaviourCallbackFunctionNothing         = testSequence_FunctionB_DoNothing,
      /*
      .behaviourCallbackFunctionAction_A        = NULL,
      .behaviourCallbackFunctionAction_B        = NULL,
      .behaviourCallbackFunctionAction_C        = NULL,
      .behaviourCallbackFunctionAction_D        = NULL,
      */
      .behaviourCallbackFunctionNextBehaviour   = testSequence_FunctionB_LoadNextStep,
      .behaviourCallbackFunctionRedo            = testSequence_FunctionB_Redo,
      .behaviourCallbackFunctionAbort           = testSequence_FunctionB_Abort,
      .behaviourCallbackFunctionCriticalAbort   = testSequence_FunctionB_CriticalAbort,
    },
    // BEHAVIOUR_SEQUENCING_STEP_3
    {
      .behaviourMainFunction                    = testSequence_FunctionC,
      .behaviourCallbackFunctionNothing         = testSequence_FunctionC_DoNothing,
      /*
      .behaviourCallbackFunctionAction_A        = NULL,
      .behaviourCallbackFunctionAction_B        = NULL,
      .behaviourCallbackFunctionAction_C        = NULL,
      .behaviourCallbackFunctionAction_D        = NULL,
      */
      .behaviourCallbackFunctionNextBehaviour   = testSequence_FunctionC_LoadNextStep,
      .behaviourCallbackFunctionRedo            = testSequence_FunctionC_Redo,
      .behaviourCallbackFunctionAbort           = testSequence_FunctionC_Abort,
      .behaviourCallbackFunctionCriticalAbort   = testSequence_FunctionC_CriticalAbort,
    },
    // BEHAVIOUR_SEQUENCING_STEP_4
    {
      .behaviourMainFunction                    = testSequence_FunctionD,
      .behaviourCallbackFunctionNothing         = testSequence_FunctionD_DoNothing,
      /*
      .behaviourCallbackFunctionAction_A        = NULL,
      .behaviourCallbackFunctionAction_B        = NULL,
      .behaviourCallbackFunctionAction_C        = NULL,
      .behaviourCallbackFunctionAction_D        = NULL,
      */
      .behaviourCallbackFunctionNextBehaviour   = testSequence_FunctionD_LoadNextStep,
      .behaviourCallbackFunctionRedo            = testSequence_FunctionD_Redo,
      .behaviourCallbackFunctionAbort           = testSequence_FunctionD_Abort,
      .behaviourCallbackFunctionCriticalAbort   = testSequence_FunctionD_CriticalAbort,
    },
  },
};

//======================================//
// TEST RUN                             //
//======================================//

bool skipFunctionA(){
  for (int i = 1; i < 10; i++){
    if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING){
      return FALSE;
    }
  }
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){
    return FALSE;
  }
  return TRUE;
}

bool skipFunctionB(){
  for (int i = 1; i < 20; i++){
    if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING){
      return FALSE;
    }
  }
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_FINISHED){
    return FALSE;
  }
  return TRUE;
}

bool skipFunctionCRepetition(){
  for (int i = 1; i < 30; i++){
    if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING){
      return FALSE;
    }
  }
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING){
    return FALSE;
  }
  return TRUE;
}

bool skipFunctionCFinish(){
  for (int i = 1; i < 30; i++){
    if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING){
      return FALSE;
    }
  }
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){
    return FALSE;
  }
  return TRUE;
}

int behaviourController_testsuiteReturner(int retVal){
  return retVal;
}

int behaviourController_testsuite(void){ 
  behaviourController_loadNewSequence(&behaviourControllerTestSequence);
  
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){
    return behaviourController_testsuiteReturner(-1);
  }
  
  if (skipFunctionA() != TRUE){
    return behaviourController_testsuiteReturner(-1);
  }
  
  if (skipFunctionCRepetition() != TRUE){
    return behaviourController_testsuiteReturner(-1);
  }
  
  if (skipFunctionCRepetition() != TRUE){
    return behaviourController_testsuiteReturner(-1);
  }
  
  if (skipFunctionCFinish() != TRUE){
    return behaviourController_testsuiteReturner(-1);
  }
  
  if (skipFunctionB() != TRUE){
    return behaviourController_testsuiteReturner(-1);
  }
  
  behaviourController_loadNewSequence(&behaviourControllerTestSequence);
  
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){
    return behaviourController_testsuiteReturner(-1);
  }
  
  if (skipFunctionA() != TRUE){
    return behaviourController_testsuiteReturner(-1);
  }
  
  if (skipFunctionCRepetition() != TRUE){
    return behaviourController_testsuiteReturner(-1);
  }
  
  if (skipFunctionCRepetition() != TRUE){
    return behaviourController_testsuiteReturner(-1);
  }
  
  if (skipFunctionCFinish() != TRUE){
    return behaviourController_testsuiteReturner(-1);
  }
  
  if (skipFunctionB() != TRUE){
    return behaviourController_testsuiteReturner(-1);
  }
  
  behaviourController_loadNewSequence(&behaviourControllerTestSequence);
  
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT){
    return behaviourController_testsuiteReturner(-1);
  }
  
  if (behaviourController_main() != BEHAVIOUR_CONTROLLER_RETURN_CRITICAL_ERROR){
    return behaviourController_testsuiteReturner(-1);
  }

  return behaviourController_testsuiteReturner(0);
}

#endif
