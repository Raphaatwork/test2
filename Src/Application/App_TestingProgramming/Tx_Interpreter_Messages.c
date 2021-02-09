/**
  ******************************************************************************
  * @file       Tx_Interpreter_Messages.h
  * @author     Tim Steinberg
  * @date       12.11.2020
  * @brief      Valid message commands for the interpreter
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
  * | 001       | 2020-11-12    | Tim Steinberg         | Initial version / skeleton of file            |
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
#include "main.h"
#include "MasterDefine.h"
#include "Tx_Interpreter_Messages.h"
#include "Ringbuffer.h"

/* Typedefinitions / Prototypes */
typedef enum TX_INTERPRETER_MESSAGE_ANALYSIS_RESULT {
  TX_INTERPRETER_MESSAGE_ANALYSIS_RESULT_MESSAGE_RECEIVED = 0,
  TX_INTERPRETER_MESSAGE_ANALYSIS_RESULT_CMD_UNKNOWN = -10,
  TX_INTERPRETER_MESSAGE_ANALYSIS_RESULT_TOO_LESS_BYTES = -20,
  TX_INTERPRETER_MESSAGE_ANALYSIS_RESULT_TOO_MANY_BYTES = -21,
} TX_INTERPRETER_MESSAGE_ANALYSIS_RESULT_TYPEDEF;

/* Variables */

/* Function definitions */
// Lengths include cmd-bytes, <spacers>/<dots> etc. and <cr> (which are 1 byte if not otherwise declared)
const TX_INTERPRETER_MESSAGE_STRUCT_TYPEDEF txInterpreterCommandField[] = {
  { // MESSAGE 0
    //"02<cr>"
    .cmd                = TX_INTERPRETER_MESSAGE_COMMANDS_MEASUREBATTERYLEVEL,
    .lengthMin          = 3,
    .lengthMax          = 3,
    .nextElement        = 1,
  },
  { // MESSAGE 1
    //"04 1<cr>"
    .cmd                = TX_INTERPRETER_MESSAGE_COMMANDS_SETRFCARRIER_TCXO,
    .lengthMin          = 5,
    .lengthMax          = 5,
    .nextElement        = 1,
  },
  { // MESSAGE 2
    //"11 1<cr>" ... "11 1 XXXX<cr>"
    .cmd                = TX_INTERPRETER_MESSAGE_COMMANDS_SETRFCARRIER_XTAL,
    .lengthMin          = 5,
    .lengthMax          = 10,
    .nextElement        = 1,
  },
  { // MESSAGE 3
    //"07 1<cr>"
    .cmd                = TX_INTERPRETER_MESSAGE_COMMANDS_SETLED,
    .lengthMin          = 5,
    .lengthMax          = 5,
    .nextElement        = 1,
  },
  { // MESSAGE 4
    //"08 a<cr>"
    .cmd                = TX_INTERPRETER_MESSAGE_COMMANDS_RESETTEST,
    .lengthMin          = 5,
    .lengthMax          = 10,
    .nextElement        = 1,
  },
  { // MESSAGE 5
    //"0A 12345678<cr>
    .cmd                = TX_INTERPRETER_MESSAGE_COMMANDS_SET_U_ID,
    .lengthMin          = 12,
    .lengthMax          = 13,
    .nextElement        = 1,
  },
  { // MESSAGE 6
    //"0B 12 12345678<cr>"
    .cmd                = TX_INTERPRETER_MESSAGE_COMMANDS_SET_PARAM,
    .lengthMin          = 15,
    .lengthMax          = 15,
    .nextElement        = 1,
  },
  { // MESSAGE 7
    //"0D<cr>"
    .cmd                = TX_INTERPRETER_MESSAGE_COMMANDS_GETSOFTWAREVERSION,
    .lengthMin          = 3,
    .lengthMax          = 3,
    .nextElement        = 1,
  },
  { // MESSAGE 8
    //"0E 1234<cr>"
    .cmd                = TX_INTERPRETER_MESSAGE_COMMANDS_SETHARDWAREREVISION,
    .lengthMin          = 8,
    .lengthMax          = 8,
    .nextElement        = 1,
  },
  { // MESSAGE 9
    //"60<cr>"
    .cmd                = TX_INTERPRETER_MESSAGE_COMMANDS_PAIRING,
    .lengthMin          = 3,
    .lengthMax          = 3,
    .nextElement        = 1,
  },
  { // MESSAGE 10
    //"61 xx yy zz<cr>"
    .cmd                = TX_INTERPRETER_MESSAGE_SETCHARBROADCAST,
    .lengthMin          = 12,
    .lengthMax          = 12,
    .nextElement        = 1,
  },
  { // MESSAGE 11
    //"0C 0<cr>" .. "0C 4294967295<cr>"
    .cmd                = TX_INTERPRETER_MESSAGE_SET_SERIALNO,
    .lengthMin          = 5,
    .lengthMax          = 14,
    .nextElement        = 1,
  },
  { // MESSAGE 12
    //"62 XXXXXXXX<cr>"
    .cmd                = TX_INTERPRETER_MESSAGE_869MHZ_SET_FREQUENCY,
    .lengthMin          = 12,
    .lengthMax          = 12,
    .nextElement        = 1,
  },
  { // MESSAGE 13
    //"63 XXXXXXXXXXXXXXXXXX<cr>"
    .cmd                = TX_INTERPRETER_MESSAGE_869MHZ_SET_OUTPUTPOWER,
    .lengthMin          = 22,
    .lengthMax          = 22,
    .nextElement        = 1,
  },
  { // MESSAGE 14
    //"6C Y XX<cr>"
    .cmd                = TX_INTERPRETER_MESSAGE_GET_PARAM,
    .lengthMin          = 8,
    .lengthMax          = 8,
    .nextElement        = 0,
  },
};

int txInterpreterValidMessages_SolveCmdToArray(uint16_t cmd){
  int result = -1;
  int i = 0;
  do{
    if (txInterpreterCommandField[i].cmd == cmd){
      result = i;
      return i;
    }
    if (txInterpreterCommandField[i].nextElement == 1){
      i++;
    }else{
      return result;
    }
  }while(1);
}
  
int txInterpreterValidMessages_GetMinlen(uint16_t cmd){
  int pos = txInterpreterValidMessages_SolveCmdToArray(cmd);
  if (pos < 0){
    return -1;
  }
  return txInterpreterCommandField[pos].lengthMin;
}
  
int txInterpreterValidMessages_GetMaxlen(uint16_t cmd){
  int pos = txInterpreterValidMessages_SolveCmdToArray(cmd);
  if (pos < 0){
    return -1;
  }
  return txInterpreterCommandField[pos].lengthMax;
}

int txInterpreter_GetCommandLengthMinMax(uint16_t cmd, uint8_t *min, uint8_t *max){
  int retVal = txInterpreterValidMessages_GetMinlen(cmd);
  if (retVal == EOF){
    return EOF;
  }
  *min = (uint8_t) retVal;
  retVal = txInterpreterValidMessages_GetMaxlen(cmd);
  if (retVal == EOF){
    return EOF;
  }
  *max = (uint8_t) retVal;
  return 0;
}

int txInterpreter_CheckReceptionAndPositionOfCR(uint8_t minPos, uint8_t maxPos, ringbuffer *txInterpreter_rb){
  int count = ringbufferGetCount(txInterpreter_rb);
  int symbol;
  
  // Are there even enough bytes to check?
  if (count < minPos){
    return TX_INTERPRETER_MESSAGE_ANALYSIS_RESULT_TOO_LESS_BYTES;
  }
  // For every already received by do
  for (int i = minPos; i <= count; i++){
    // Did we leave the possible length of the message?
    if (i > maxPos){
      // Yes
      
      // Give that result to the upper layers
      return TX_INTERPRETER_MESSAGE_ANALYSIS_RESULT_TOO_MANY_BYTES;
    }
    // Is in the specified length a CR? Note: While we start counting with 1, the RB starts with 0 - so it's minus 1
    symbol = ringbufferPeekCharPosition(txInterpreter_rb, i - 1);
    if (symbol == TX_INTERPRETER_SYMBOL_CR){
      // Yes
      
      // Return that position to the upper layers
      return i;
    }
  }
  // We didn't find a CR, but the maximum length of the message is not received 
  // either. So there are still bytes in transmission. Tell that the upper layers
  return TX_INTERPRETER_MESSAGE_ANALYSIS_RESULT_TOO_LESS_BYTES;
}

int txInterpreter_CheckReceptionOfMessageComplete(uint16_t cmd, ringbuffer *txInterpreter_rb){
  uint8_t min;
  uint8_t max;
  int result;
  
  // Is this command existent, then result is 0
  if (txInterpreter_GetCommandLengthMinMax(cmd, &min, &max) != 0){
    return TX_INTERPRETER_MESSAGE_ANALYSIS_RESULT_CMD_UNKNOWN;
  }
  
  // Check if the length is valid and on the specified position is a CR
  result = txInterpreter_CheckReceptionAndPositionOfCR(min, max, txInterpreter_rb);
  switch(result){
    // No CR found in the already recepted bytes. Wait some more.
    case TX_INTERPRETER_MESSAGE_ANALYSIS_RESULT_TOO_LESS_BYTES:
      return 0;
      break;
    // The CR is not in the limited area, thus the message is wrong or unknown  
    case TX_INTERPRETER_MESSAGE_ANALYSIS_RESULT_TOO_MANY_BYTES:
      return -1;
      break;
      
    default:
      // CR was found in the limited area. Return that position to the upper layers
      return 1;
      break;
  }
}