/**
  ******************************************************************************
  * @file       RSL_Protocol_Lower_Level_Test.c
  * @author     Tim Steinberg
  * @date       19.05.2020
  * @brief      Test option and runner for all units belonging to the lower
  *             level of the RSL protocol.
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

/* Includes */
#include "Test_Selector.h"
#include "Debug.h"

#include "userMethods_UART.h"
#include "Ringbuffer.h"
#include "RingbufferWrapper.h"
#include "CRC_Software.h"
#include "Handler_Timer.h"
#include "Handler_NAK_Transmission.h"
#include "Parser.h"
#include "MessageIOBuffer.h"
#include "Logic.h"
/* Typedefinitions */

/* Variables */

/* Function definitions */

void rslProtocolTestsuite_testsLowerLevel(){
  
#if TEST_GROUP_LOWER_LEVEL_ACTIVE >= 1
  int32_t retVal;
#endif 
  
#if TEST_USERMETHODS_UART >= 1
  retVal = userMethods_testsuite();
  TRACE_TEST_VALUES(1, "TEST UserMethods_UART.c %i", retVal);
  if (retVal < 0){
    do{}while(1);
  }
#endif
  
#if TEST_RINGBUFFER >= 1
  retVal = ringbufferTestsuite();
  TRACE_TEST_VALUES(1, "TEST Ringbuffer.c %i", retVal);
  if (retVal < 0){
    do{}while(1);
  }
#endif
  
#if TEST_RECEPTION_BUFFER >= 1
  retVal = ringbufferWrapper_testsuite();
  TRACE_TEST_VALUES(1, "TEST RingbufferWrapper.c %i", retVal);
  if (retVal < 0){
    do{}while(1);
  }
#endif
  
#if TEST_CRC_SOFTWARE >= 1
  retVal = CRC_Software_testsuite();
  TRACE_TEST_VALUES(1, "TEST CRC_Software.c %i", retVal);
  if (retVal < 0){
    do{}while(1);
  }
#endif
  
#if TEST_HANDLER_TIMER >= 1
  retVal = timerHandler_testsuite();
  TRACE_TEST_VALUES(1, "TEST Handler_Timer.c %i", retVal);
  if (retVal < 0){
    do{}while(1);
  }
#endif
  
#if TEST_HANDLER_NAK_TRANSMISSION >= 1
  retVal = handlerNAK_testsuite();
  TRACE_TEST_VALUES(1, "TEST Handler_NAK_Transmission.c %i", retVal);
  if (retVal < 0){
    do{}while(1);
  }
#endif
  
#if TEST_PARSER >= 1
  retVal = parser_testsuite();
  TRACE_TEST_VALUES(1, "TEST Parser.c %i", retVal);
  if (retVal < 0){
    do{}while(1);
  }
#endif
  
#if TEST_MESSAGEIOBUFFER >= 1
  retVal = messageIOBuffer_testsuite();
  TRACE_TEST_VALUES(1, "TEST MessageIOBuffer.c %i", retVal);
  if (retVal < 0){
    do{}while(1);
  }
#endif
  
#if TEST_LOGIC >= 1
  retVal = logic_testsuite();
  TRACE_TEST_VALUES(1, "TEST Logic.c %i", retVal);
  if (retVal < 0){
    do{}while(1);
  }
#endif
  
}
