/********************************************************************************
* LEDs - Management                                                  17-05-2018 *
*                                                                               *
*********************************************************************************/

#ifndef TXV2_DEBUG_H
#define TXV2_DEBUG_H

/********************************************************************************/
/* ====> Functions prototypes                                                   */
/********************************************************************************/

#include "String.h"
#include "Stdio.h"

#define DEBUG_PROCESSOR_HALTABLE 0

// DEBUG MAIN OUTPUT
#ifndef DEBUG_LEESYS
  #define DEBUG_LEESYS 1
#else
  #error DEBUG_LEESYS already defined!
#endif

#if DEBUG_LEESYS >= 1
  #define TRACE(x ...)   printf(x);
#else
  #define TRACE(x ...)
#endif /* DEBUG_LEESYS */



// DEBUG THE PROCEDURE CALLS
#ifndef DEBUG_LEESYS_PROCEDURE_CALLS
  #define DEBUG_LEESYS_PROCEDURE_CALLS 0
#else
  #error DEBUG_LEESYS_PROCEDURE_CALLS already defined!
#endif

#if DEBUG_LEESYS_PROCEDURE_CALLS >= 1
  #define TRACE_PROCEDURE_CALLS(y, x ...)   if (y >= DEBUG_LEESYS_PROCEDURE_CALLS) TRACE(x);
#else
  #define TRACE_PROCEDURE_CALLS(y, x ...)
#endif /* DEBUG_LEESYS_PROCEDURE_CALLS */



// DEBUG ERROR OCCURANCES
#ifndef DEBUG_LEESYS_ERROR_OCCURANCES
  #define DEBUG_LEESYS_ERROR_OCCURANCES 0
#else
  #error DEBUG_LEESYS_ERROR_OCCURANCES already defined!
#endif

#if DEBUG_LEESYS_ERROR_OCCURANCES >= 1
  #define TRACE_ERROR_OCCURANCES(y, x ...)   if (y >= DEBUG_LEESYS_ERROR_OCCURANCES) TRACE(x);
#else
  #define TRACE_ERROR_OCCURANCES(y, x ...)
#endif /* DEBUG_LEESYS_ERROR_OCCURANCES */



//DEBUG IMPORTANT VALUES, E.G. SENSOR VALUES
#ifndef DEBUG_LEESYS_SENSOR_VALUES
  #define DEBUG_LEESYS_SENSOR_VALUES 0
#else
  #error DEBUG_LEESYS_SENSOR_VALUES already defined!
#endif

#if DEBUG_LEESYS_SENSOR_VALUES >= 1
  #define TRACE_SENSOR_VALUES(y, x ...)   if (y >= DEBUG_LEESYS_SENSOR_VALUES) TRACE(x);
#else
  #define TRACE_SENSOR_VALUES(y, x ...)
#endif /* DEBUG_LEESYS_SENSOR_VALUES */



//DEBUG INFORMATION TO TESTS
#ifndef DEBUG_LEESYS_TEST_VALUES
  #define DEBUG_LEESYS_TEST_VALUES 0
#else
  #error DEBUG_LEESYS_TEST_VALUES already defined!
#endif

#if DEBUG_LEESYS_TEST_VALUES >= 1
  #define TRACE_TEST_VALUES(y, x ...)   if (y >= DEBUG_LEESYS_TEST_VALUES) TRACE(x);
#else
  #define TRACE_TEST_VALUES(y, x ...)
#endif /* DEBUG_LEESYS_TEST_VALUES */



//DEBUG INFORMATION TO IO
#ifndef DEBUG_LEESYS_IO_VALUES
  #define DEBUG_LEESYS_IO_VALUES 0
#else
  #error DEBUG_LEESYS_IO_VALUES already defined!
#endif

#if DEBUG_LEESYS_IO_VALUES >= 1
  #define TRACE_IO_VALUES(y, x ...)   if (y >= DEBUG_LEESYS_IO_VALUES) TRACE(x);
#else
  #define TRACE_IO_VALUES(y, x ...)
#endif /* DEBUG_LEESYS_IO_VALUES */



//DEBUG INFORMATION TO IO
#ifndef DEBUG_LEESYS_IO_RECEPTION_VALUES
  #define DEBUG_LEESYS_IO_RECEPTION_VALUES 0
#else
  #error DEBUG_LEESYS_IO_RECEPTION_VALUES already defined!
#endif

#if DEBUG_LEESYS_IO_RECEPTION_VALUES >= 1
  #define TRACE_IO_RECEPTION_VALUES(y, x ...)   if (y >= DEBUG_LEESYS_IO_RECEPTION_VALUES) TRACE(x);
#else
  #define TRACE_IO_RECEPTION_VALUES(y, x ...)
#endif /* DEBUG_LEESYS_IO_RECEPTION_VALUES */



//DEBUG INFORMATION TO IO
#ifndef DEBUG_LEESYS_STACK
  #define DEBUG_LEESYS_STACK 1
#else
  #error DEBUG_LEESYS_STACK already defined!
#endif

#if DEBUG_LEESYS_STACK >= 2
  #define DEBUG_LEESYS_STACK_VALUES(y, x ...)   if (y >= DEBUG_LEESYS_STACK) TRACE(x);
#else
  #define DEBUG_LEESYS_STACK_VALUES(y, x ...)
#endif /* DEBUG_LEESYS_STACK */

#endif