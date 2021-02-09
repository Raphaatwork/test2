/**
  ******************************************************************************
  * @file       BehaviourDefines.h
  * @author     Tim Steinberg
  * @date       28.05.2020
  * @brief      Definitions and Typedefs for the behaviour and their control
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
  * | 001       | 2020-05-28    | Tim Steinberg         | Initial version / skeleton of file            |
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

#ifndef __BEHAVIOUR_DEFINES_H
#define __BEHAVIOUR_DEFINES_H

/* Includes */
#include "MasterDefine.h"

/* Typedefinitions */
/**
 * @brief       This typedef enum contains the defines to enumerate all steps
 *              and allow for a human readable sequencing
 */
typedef enum BEHAVIOUR_SEQUENCING_STEPS {
  BEHAVIOUR_SEQUENCING_STEP_START                       = 0x00, /**< The 1st behaviourstep indexer, of the behehaviourstep which is automatically loaded if you load the behaviour.*/
  BEHAVIOUR_SEQUENCING_STEP_1                           = 0x01, /**< The 2nd behaviourstep indexer.*/
  BEHAVIOUR_SEQUENCING_STEP_2                           = 0x02, /**< The 3rd behaviourstep indexer.*/
  BEHAVIOUR_SEQUENCING_STEP_3                           = 0x03, /**< The 4th behaviourstep indexer.*/
  BEHAVIOUR_SEQUENCING_STEP_4                           = 0x04, /**< The 5th behaviourstep indexer.*/
  BEHAVIOUR_SEQUENCING_STEP_5                           = 0x05, /**< The 6th behaviourstep indexer.*/
  BEHAVIOUR_SEQUENCING_STEP_6                           = 0x06, /**< The 7th behaviourstep indexer.*/
  BEHAVIOUR_SEQUENCING_STEP_7                           = 0x07, /**< The 8th behaviourstep indexer.*/
  BEHAVIOUR_SEQUENCING_STEP_8                           = 0x08, /**< The 9th behaviourstep indexer.*/
  BEHAVIOUR_SEQUENCING_STEP_9                           = 0x09, /**< The 10th behaviourstep indexer.*/
  BEHAVIOUR_SEQUENCING_STEP_10                          = 0x0A, /**< The 11th behaviourstep indexer.*/
  BEHAVIOUR_SEQUENCING_STEP_11                          = 0x0B, /**< The 12th behaviourstep indexer.*/
  BEHAVIOUR_SEQUENCING_STEP_12                          = 0x0C, /**< The 13th behaviourstep indexer.*/
  BEHAVIOUR_SEQUENCING_STEP_13                          = 0x0D, /**< The 14th behaviourstep indexer.*/
  BEHAVIOUR_SEQUENCING_STEP_14                          = 0x0E, /**< The 15th behaviourstep indexer.*/
  BEHAVIOUR_SEQUENCING_STEP_15                          = 0x0F, /**< The 16th behaviourstep indexer.*/
  BEHAVIOUR_SEQUENCING_STEP_16                          = 0x10, /**< The 17th behaviourstep indexer.*/
  BEHAVIOUR_SEQUENCING_STEP_17                          = 0x11, /**< The 18th behaviourstep indexer.*/
  BEHAVIOUR_SEQUENCING_STEP_18                          = 0x12, /**< The 19th behaviourstep indexer.*/
  BEHAVIOUR_SEQUENCING_STEP_19                          = 0x13, /**< The 20th behaviourstep indexer.*/
  BEHAVIOUR_SEQUENCING_STEP_20                          = 0x14, /**< The 21st behaviourstep indexer.*/
  BEHAVIOUR_SEQUENCING_STEP_21                          = 0x15, /**< The 22nd behaviourstep indexer.*/
  BEHAVIOUR_SEQUENCING_STEP_22                          = 0x16, /**< The 23rd behaviourstep indexer.*/
  BEHAVIOUR_SEQUENCING_STEP_23                          = 0x17, /**< The 24th behaviourstep indexer.*/
  BEHAVIOUR_SEQUENCING_STEP_24                          = 0x18, /**< The 25th behaviourstep indexer.*/
  BEHAVIOUR_SEQUENCING_STEP_25                          = 0x19, /**< The 26th behaviourstep indexer.*/
  BEHAVIOUR_SEQUENCING_STEP_26                          = 0x1A, /**< The 27th behaviourstep indexer.*/
  BEHAVIOUR_SEQUENCING_STEP_27                          = 0x1B, /**< The 28th behaviourstep indexer.*/
  BEHAVIOUR_SEQUENCING_STEP_28                          = 0x1C, /**< The 29th behaviourstep indexer.*/
  BEHAVIOUR_SEQUENCING_STEP_29                          = 0x1D, /**< The 30th behaviourstep indexer.*/
  BEHAVIOUR_SEQUENCING_STEP_30                          = 0x1E, /**< The 31st behaviourstep indexer.*/
  BEHAVIOUR_SEQUENCING_STEP_31                          = 0x1F, /**< The 32nd behaviourstep indexer. Add more if you need it, but you probably won't*/
  BEHAVIOUR_SEQUENCING_STEP_END                         = 0xFF, /**< The indexer of the last step. This tells the controller that your behaviour ends with it.*/
  BEHAVIOUR_SEQUENCING_STEP_UNDEFINED                   = -1,   /**< Marker for NOT allowed jump.*/
} BEHAVIOUR_SEQUENCING_STEPS_TYPEDEF;

/**
 * @brief       This typedef enum contains the defines for the return values
 *              that a behaviour step delivers to the behaviour controller.
 */
typedef enum BEHAVIOUR_STEP_RETURN_VALUES {
  BEHAVIOUR_STEP_RETURN_NOTHING                              = 0x00, /**< The behaviourstep is still onging, just call it more.*/
  BEHAVIOUR_STEP_RETURN_ACTION_A                             = 0x01, /**< The behaviourstep is still onging, it is like NOTHING, just an extra call for more options.*/
  BEHAVIOUR_STEP_RETURN_ACTION_B                             = 0x02, /**< The behaviourstep is still onging, it is like NOTHING, just an extra call for more options.*/
  BEHAVIOUR_STEP_RETURN_ACTION_C                             = 0x03, /**< The behaviourstep is still onging, it is like NOTHING, just an extra call for more options.*/
  BEHAVIOUR_STEP_RETURN_ACTION_D                             = 0x04, /**< The behaviourstep is still onging, it is like NOTHING, just an extra call for more options.*/
  BEHAVIOUR_STEP_RETURN_NEXT_BEHAVIOUR_STEP                  = 0x10, /**< The behaviourstep is finished and the behaviourcontroller can load the next step.*/
  BEHAVIOUR_STEP_RETURN_REDO                                 = 0x20, /**< The behaviourstep ran into a problem (e.g. RSL not responding) and wishes to be called again. Check the repetition limit though!*/
  BEHAVIOUR_STEP_RETURN_ABORT                                = 0xFD, /**< The behaviourstep ran into a minor problem and wants you to load another sequence to fix this - e.g. reboot.*/
  BEHAVIOUR_STEP_RETURN_CRITICAL_ABORT                       = 0xFE, /**< The behaviourstep ran into a mayor problem. Please reset hard and try again or put to sleep.*/
} BEHAVIOUR_STEP_RETURN_VALUES_TYPEDEF;

/**
 * @brief       This typedef enum contains the defines for the return values
 *              that a behaviour step callback returns to the behaviour
 *              controller in order to tell it what to do next.
 */
typedef enum BEHAVIOUR_CONTROLLER_COMMANDS {
  BEHAVIOUR_CONTROLLER_DO_NOTHING                       = 0x00, /**< Tell the behaviour controller to do nothing */
  BEHAVIOUR_CONTROLLER_RELOAD_BEHAVIOUR                 = 0x10, /**< Tell the behaviour controller to reload the step */
  BEHAVIOUR_CONTROLLER_LOAD_NEXT_BEHAVIOUR_STEP         = 0x20, /**< Tell the behaviour controller to load the next step */
  BEHAVIOUR_CONTROLLER_FINISHED_BEHAVIOUR               = 0x30, /**< Tell the behaviour controller that we are finished */
  BEHAVIOUR_CONTROLLER_RETURN_CRITICAL                  = 0xFF, /**< Tell the behavioru controller that things are bad */
} BEHAVIOUR_CONTROLLER_COMMANDS_TYPEDEF;

/**
 * @brief       This typedef enum contains the defines for the return values
 *              that a behaviour controllers returns to the app.
 */
typedef enum BEHAVIOUR_CONTROLLER_RETURN_VALUES {
  BEHAVIOUR_CONTROLLER_RETURN_ONGOING                   = 0x00, /**< Tell upper layer that nothing happened */
  BEHAVIOUR_CONTROLLER_RETURN_ONGOING_LOAD_NEXT         = 0x01, /**< Tell upper layer that nothing happened, but the next step was loaded (for debug purpose) */
  BEHAVIOUR_CONTROLLER_RETURN_FINISHED                  = 0x10, /**< Tell upper layer that the behaviour is finished */
  BEHAVIOUR_CONTROLLER_RETURN_CRITICAL_ERROR            = 0xFF, /**< Tell upper layer that things are bad */
} BEHAVIOUR_CONTROLLER_RETURN_VALUES_TYPEDEF;

/**
 * @brief       This typedef struct defines a structure which is used as return
 *              value in the callbacks of the behaviour handling.
 *              command is the "decider" for what to do
 *              nextStep is only used for commands that do lead to a next step
 */
typedef struct BEHAVIOUR_CONTROLLER_COMMAND_STRUCT {
  BEHAVIOUR_CONTROLLER_COMMANDS_TYPEDEF command;/**< Returned command */
  BEHAVIOUR_SEQUENCING_STEPS_TYPEDEF nextStep;  /**< The next step ID. Leave BEHAVIOUR_SEQUENCING_STEP_UNDEFINED if not used by command */
} BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF;

/**
 * @brief       The main function. Will be called first by the behaviour
 *              controller. The return value of this function will decide,
 *              which callback function will be executed afterwards.
 *              return-type:         BEHAVIOUR_STEP_RETURN_VALUES_TYPEDEF
 *              param-types:         bool
 *              name of type:        BEHAVIOUR_MAIN_FUNC
 */
typedef BEHAVIOUR_STEP_RETURN_VALUES_TYPEDEF (*BEHAVIOUR_MAIN_FUNC)(bool init);

/**
 * @brief       The callback function of the behaviour(-step). Will be called by 
 *              the behaviour controller after the main function.
 *              return-type:         BEHAVIOUR_CONTROLLER_RETURN_STRUCT_TYPEDEF
 *              param-types:         BEHAVIOUR_RETURN_VALUES_TYPEDEF
 *              name of type:        BEHAVIOUR_CALLBACK_FUNC
 */
typedef BEHAVIOUR_CONTROLLER_COMMAND_STRUCT_TYPEDEF (*BEHAVIOUR_CALLBACK_FUNC)(void);

/**
 * @brief       The behaviour steps integral function. The main function is the
 *              "executive" part of each step, while the callbacks are the
 *              "reactive" part of each step.
 *              If you want to build your own sequencing then you only
 *              need to integrate functions that are within the scope of the
 *              main functions return values. If you don't have any actions,
 *              then they'll never be called - so you can leave them open.
 */
typedef struct BEHAVIOR_SEQUENCE_STEP_STRUCT {
  //BEHAVIOUR_SEQUENCING_STEPS_TYPEDEF numberOfNextStep;
  BEHAVIOUR_MAIN_FUNC           behaviourMainFunction;                  /**< The "main" function of the sequence. This is where things are done */
  BEHAVIOUR_CALLBACK_FUNC       behaviourCallbackFunctionNothing;       /**< The callback for the return value "BEHAVIOUR_STEP_RETURN_NOTHING" */
  BEHAVIOUR_CALLBACK_FUNC       behaviourCallbackFunctionAction_A;      /**< The callback for the return value "BEHAVIOUR_STEP_RETURN_ACTION_A" */
  BEHAVIOUR_CALLBACK_FUNC       behaviourCallbackFunctionAction_B;      /**< The callback for the return value "BEHAVIOUR_STEP_RETURN_ACTION_B" */
  BEHAVIOUR_CALLBACK_FUNC       behaviourCallbackFunctionAction_C;      /**< The callback for the return value "BEHAVIOUR_STEP_RETURN_ACTION_C" */
  BEHAVIOUR_CALLBACK_FUNC       behaviourCallbackFunctionAction_D;      /**< The callback for the return value "BEHAVIOUR_STEP_RETURN_ACTION_D" */
  BEHAVIOUR_CALLBACK_FUNC       behaviourCallbackFunctionNextBehaviour; /**< The callback for the return value "BEHAVIOUR_STEP_RETURN_NEXT_BEHAVIOUR_STEP" */
  BEHAVIOUR_CALLBACK_FUNC       behaviourCallbackFunctionRedo;          /**< The callback for the return value "BEHAVIOUR_STEP_RETURN_REDO" */
  BEHAVIOUR_CALLBACK_FUNC       behaviourCallbackFunctionAbort;         /**< The callback for the return value "BEHAVIOUR_STEP_RETURN_ABORT" */
  BEHAVIOUR_CALLBACK_FUNC       behaviourCallbackFunctionCriticalAbort; /**< The callback for the return value "BEHAVIOUR_STEP_RETURN_CRITICAL_ABORT" */
} BEHAVIOR_SEQUENCE_STEP_STRUCT_TYPEDEF;

/**
 * @brief       The behaviours integral struct. This struct defines
 *              with which function to start, how many steps there are,
 *              the init flag and the behaviour steps themselves.
 */
typedef struct BEHAVIOUR_CONTROLLER_CALL_STRUCT {
  BEHAVIOUR_SEQUENCING_STEPS_TYPEDEF initializingBehaviour;
  BEHAVIOUR_SEQUENCING_STEPS_TYPEDEF numberOfActualBehaviour;
  bool initNext;
  BEHAVIOR_SEQUENCE_STEP_STRUCT_TYPEDEF stepsOfSequence[];
} BEHAVIOUR_CONTROLLER_CALL_STRUCT_TYPEDEF;

/* Variables */

/* Function definitions */

#endif