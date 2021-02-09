/**
  ******************************************************************************
  * @file       BehaviourController.h
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

#ifndef __BEHAVIOUR_CONTROLLER_H
#define __BEHAVIOUR_CONTROLLER_H

/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "BehaviourDefines.h"
#include "Test_Selector.h"

/* Typedefinitions */

/* Variables */

/* Function definitions */
/** @brief      This method is the loader function for a new sequence. This 
 *              function must be called before you try to call the controllers
 *              main function (to load the sequence you want to execute).
 *  @param *newSequence Pointer to the new sequence
 *  @return Nothing
 */
void behaviourController_loadNewSequence(BEHAVIOUR_CONTROLLER_CALL_STRUCT_TYPEDEF *newSequence);

/** @brief      Prior to this function CALL THE LOADER TO LOAD THE SEQUENCE. 
 *              The controller will execute the loaded behaviour.
 *  @return The result of the execution which might be
 *          BEHAVIOUR_CONTROLLER_RETURN_ONGOING                   = 0x00, // Tell upper layer that nothing happened 
 *          BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT         = 0x01, // Tell upper layer that nothing happened, but the next step was loaded (for debug purpose) 
 *          BEHAVIOUR_CONTROLLER_RETURN_FINISHED                  = 0x10, // Tell upper layer that the behaviour is finished 
 *          BEHAVIOUR_CONTROLLER_RETURN_CRITICAL_ERROR            = 0xFF, // Tell upper layer that things are bad 
 */
BEHAVIOUR_CONTROLLER_RETURN_VALUES_TYPEDEF behaviourController_main(void);

#if TEST_BEHAVIOUR_CONTROLLER >= 1
  int behaviourController_testsuite(void);
#endif

#endif