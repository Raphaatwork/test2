/**
  ******************************************************************************
  * @file       Test_Selector.h
  * @author     Tim Steinberg
  * @date       19.05.2020
  * @brief      Selector for which tests to run
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
  * | 001       | 2020-05-19    | Tim Steinberg         | Initial version / skeleton of file            |
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

// Let this rest here, else it will always complain about "undefined functions"
#define TEST_USERMETHODS_UART                           0
#define TEST_RINGBUFFER                                 0
#define TEST_RINGBUFFER_WRAPPER                         0
#define TEST_CRC_SOFTWARE                               0
#define TEST_HANDLER_TIMER                              0
#define TEST_HANDLER_NAK_TRANSMISSION                   0
#define TEST_PARSER                                     0
#define TEST_MESSAGEIOBUFFER                            0
#define TEST_LOGIC                                      0

#define TEST_GROUP_LOWER_LEVEL_ACTIVE                   ( (TEST_USERMETHODS_UART >= 1) || (TEST_RINGBUFFER >= 1) || (TEST_RINGBUFFER_WRAPPER >= 1) || (TEST_CRC_SOFTWARE >= 1) || (TEST_HANDLER_TIMER >= 1) || (TEST_HANDLER_NAK_TRANSMISSION >= 1) || (TEST_PARSER >= 1) || (TEST_MESSAGEIOBUFFER >= 1) || (TEST_LOGIC >= 1) )

#define TEST_BEHAVIOURSTEP_START_V115                   0
#define TEST_BEHAVIOURSTEP_SLEEP_V115                   0
#define TEST_BEHAVIOURSTEP_PAIRING_V115                 0
#define TEST_BEHAVIOURSTEP_CHAR_ALERT_V115              0
#define TEST_BEHAVIOURSTEP_CHAR_BATTERY_V115            0
#define TEST_BEHAVIOURSTEP_CHAR_ERROR_V115              0
#define TEST_BEHAVIOURSTEP_BC_START_V115                0
#define TEST_BEHAVIOURSTEP_BC_END_V115                  0
#define TEST_BEHAVIOURSTEP_BC_WAIT_TO_V115              0
#define TEST_BEHAVIOURSTEP_REMOVEPD_V115                0

#define TEST_GROUP_UPPER_LEVEL_STEPS_ACTIVE             ( (TEST_BEHAVIOURSTEP_REMOVEPD_V115 >= 1) || (TEST_BEHAVIOURSTEP_BC_WAIT_TO_V115 >= 1) || (TEST_BEHAVIOURSTEP_BC_END_V115 >= 1) || (TEST_BEHAVIOURSTEP_BC_START_V115 >= 1) || (TEST_BEHAVIOURSTEP_CHAR_ERROR_V115 >= 1) || (TEST_BEHAVIOURSTEP_CHAR_BATTERY_V115 >= 1) || (TEST_BEHAVIOURSTEP_CHAR_ALERT_V115 >= 1) || (TEST_BEHAVIOURSTEP_PAIRING_V115 >= 1) || (TEST_BEHAVIOURSTEP_SLEEP_V115 >= 1) || (TEST_BEHAVIOURSTEP_START_V115 >= 1) )                    

#define TEST_BEHAVIOUR_CONTROLLER                       0
                                                        
#define TEST_BEHAVIOUR_PAIRING_V115                     0
#define TEST_BEHAVIOUR_ALERT_V115                       0
#define TEST_BEHAVIOUR_BATTERY_V115                     0
#define TEST_BEHAVIOUR_ERROR_V115                       0
#define TEST_BEHAVIOUR_RESETALERT_V115                  0
#define TEST_BEHAVIOUR_SETALLCHARACTERISTICS_V115       0

#define TEST_GROUP_UPPER_LEVEL_BEHAVIOURS_ACTIVE        ( (TEST_BEHAVIOUR_SETALLCHARACTERISTICS_V115 >= 1) || (TEST_BEHAVIOUR_RESETALERT_V115 >= 1) || (TEST_BEHAVIOUR_ERROR_V115 >= 1) || (TEST_BEHAVIOUR_BATTERY_V115 >= 1) || (TEST_BEHAVIOUR_ALERT_V115 >= 1) || (TEST_BEHAVIOUR_PAIRING_V115 >= 1) || (TEST_BEHAVIOUR_CONTROLLER >= 1) )

#if TEST_GROUP_LOWER_LEVEL_ACTIVE >= 1
  //#warning TEST MAY TAKE UP TO 10 SECONDS
#endif

#if TEST_GROUP_UPPER_LEVEL_STEPS_ACTIVE >= 1
  #if TEST_BEHAVIOURSTEP_WAITFORBCENDORTRANSMISSION >= 1
    //#warning TEST MAY TAKE UP TO 160 SECONDS
  #else
    //#warning TEST MAY TAKE UP TO 60 SECONDS
  #endif
  
#endif

#if TEST_GROUP_UPPER_LEVEL_BEHAVIOURS_ACTIVE >= 1
  //#warning TEST MAY TAKE UP TO 120 SECONDS
#endif

#ifndef TEST_ACTIVATE_UART_CALLBACK
  #if (TEST_GROUP_UPPER_LEVEL_BEHAVIOURS_ACTIVE || TEST_GROUP_UPPER_LEVEL_STEPS_ACTIVE || TEST_GROUP_LOWER_LEVEL_ACTIVE)
    #define TEST_ACTIVATE_UART_CALLBACK 1
  #endif
#endif

#ifndef __TEST_SELECTOR_H
#define __TEST_SELECTOR_H

/* Includes */

/* Typedefinitions */

/* Variables */

/* Function definitions */    

#endif