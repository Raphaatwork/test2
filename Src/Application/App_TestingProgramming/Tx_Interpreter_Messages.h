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

#ifndef __TX_INTERPRETER_MESSAGES_H
#define __TX_INTERPRETER_MESSAGES_H

/* Includes */
#include "Ringbuffer.h"

/* Typedefinitions */

#define TX_INTERPRETER_SYMBOL_CR                                0x0D
#define TX_INTERPRETER_SYMBOL_LF                                0x0A
#define TX_INTERPRETER_SYMBOL_SPACE                             0x20
#define TX_INTERPRETER_SYMBOL_MINUS                             0x2D
#define TX_INTERPRETER_SYMBOL_DOT                               0x2E
#define TX_INTERPRETER_SYMBOL_0                                 0x30
#define TX_INTERPRETER_SYMBOL_1                                 0x31
#define TX_INTERPRETER_SYMBOL_A                                 0x41
#define TX_INTERPRETER_SYMBOL_B                                 0x42
#define TX_INTERPRETER_SYMBOL_C                                 0x43
#define TX_INTERPRETER_SYMBOL_D                                 0x44
#define TX_INTERPRETER_SYMBOL_E                                 0x45
#define TX_INTERPRETER_SYMBOL_F                                 0x46
#define TX_INTERPRETER_SYMBOL_a                                 0x61
#define TX_INTERPRETER_SYMBOL_b                                 0x62
#define TX_INTERPRETER_SYMBOL_c                                 0x63
#define TX_INTERPRETER_SYMBOL_d                                 0x64
#define TX_INTERPRETER_SYMBOL_e                                 0x65
#define TX_INTERPRETER_SYMBOL_f                                 0x66
#define TX_INTERPRETER_SYMBOL_g                                 0x67
#define TX_INTERPRETER_SYMBOL_h                                 0x68

////////////////////////////////////////////////////////////
// Input commands
////////////////////////////////////////////////////////////
#define TX_INTERPRETER_MESSAGE_MEASUREBATTERYLEVEL              0x3032
#define TX_INTERPRETER_MESSAGE_SETRFCARRIER_TCXO                0x3034
#define TX_INTERPRETER_MESSAGE_SETRFCARRIER_XTAL                0x3131
#define TX_INTERPRETER_MESSAGE_SETLED                           0x3037
#define TX_INTERPRETER_MESSAGE_RESETTEST                        0x3038
#define TX_INTERPRETER_MESSAGE_SET_U_ID                         0x3041
#define TX_INTERPRETER_MESSAGE_SET_PARAM                        0x3042
#define TX_INTERPRETER_MESSAGE_SET_SERIALNO                     0x3043
#define TX_INTERPRETER_MESSAGE_GETSOFTWAREVERSION               0x3044
#define TX_INTERPRETER_MESSAGE_SETHARDWAREREVISION              0x3045
#define TX_INTERPRETER_MESSAGE_TXTEST                           0x3133
#define TX_INTERPRETER_MESSAGE_PAIRING                          0x3630
#define TX_INTERPRETER_MESSAGE_SETCHARBROADCAST                 0x3631
#define TX_INTERPRETER_MESSAGE_869MHZ_SET_FREQUENCY             0x3632
#define TX_INTERPRETER_MESSAGE_869MHZ_SET_OUTPUTPOWER           0x3633
#define TX_INTERPRETER_MESSAGE_GET_PARAM                        0x3643

typedef enum TX_INTERPRETER_MESSAGE_COMMANDS {
  TX_INTERPRETER_MESSAGE_COMMANDS_MEASUREBATTERYLEVEL           = TX_INTERPRETER_MESSAGE_MEASUREBATTERYLEVEL,
  TX_INTERPRETER_MESSAGE_COMMANDS_SETRFCARRIER_TCXO             = TX_INTERPRETER_MESSAGE_SETRFCARRIER_TCXO,
  TX_INTERPRETER_MESSAGE_COMMANDS_SETRFCARRIER_XTAL             = TX_INTERPRETER_MESSAGE_SETRFCARRIER_XTAL,
  TX_INTERPRETER_MESSAGE_COMMANDS_SETLED                        = TX_INTERPRETER_MESSAGE_SETLED,
  TX_INTERPRETER_MESSAGE_COMMANDS_RESETTEST                     = TX_INTERPRETER_MESSAGE_RESETTEST,
  TX_INTERPRETER_MESSAGE_COMMANDS_SET_U_ID                      = TX_INTERPRETER_MESSAGE_SET_U_ID,
  TX_INTERPRETER_MESSAGE_COMMANDS_SET_PARAM                     = TX_INTERPRETER_MESSAGE_SET_PARAM, 
  TX_INTERPRETER_MESSAGE_COMMANDS_SET_SERIALNO                  = TX_INTERPRETER_MESSAGE_SET_SERIALNO,
  TX_INTERPRETER_MESSAGE_COMMANDS_GETSOFTWAREVERSION            = TX_INTERPRETER_MESSAGE_GETSOFTWAREVERSION,
  TX_INTERPRETER_MESSAGE_COMMANDS_SETHARDWAREREVISION           = TX_INTERPRETER_MESSAGE_SETHARDWAREREVISION,
  TX_INTERPRETER_MESSAGE_COMMANDS_TXTEST                        = TX_INTERPRETER_MESSAGE_TXTEST, 
  TX_INTERPRETER_MESSAGE_COMMANDS_PAIRING                       = TX_INTERPRETER_MESSAGE_PAIRING,
  TX_INTERPRETER_MESSAGE_COMMANDS_SETCHARBROADCAST              = TX_INTERPRETER_MESSAGE_SETCHARBROADCAST,  
  TX_INTERPRETER_MESSAGE_COMMANDS_869MHZ_SET_F                  = TX_INTERPRETER_MESSAGE_869MHZ_SET_FREQUENCY,
  TX_INTERPRETER_MESSAGE_COMMANDS_869MHZ_SET_P                  = TX_INTERPRETER_MESSAGE_869MHZ_SET_OUTPUTPOWER,
  TX_INTERPRETER_MESSAGE_COMMANDS_GET_PARAM                     = TX_INTERPRETER_MESSAGE_GET_PARAM,
} TX_INTERPRETER_MESSAGE_COMMANDS_TYPEDEF;

typedef struct __attribute__ ((packed)) TX_INTERPRETER_MESSAGE_STRUCT {
  uint16_t cmd;
  uint8_t lengthMin;
  uint8_t lengthMax;
  uint8_t nextElement;
} TX_INTERPRETER_MESSAGE_STRUCT_TYPEDEF;

////////////////////////////////////////////////////////////
// Output commands
////////////////////////////////////////////////////////////
#define TX_INTERPRETER_MESSAGE_MEASUREBATTERYLEVEL_ANS          0x3832
#define TX_INTERPRETER_MESSAGE_SETRFCARRIER_TCXO_ANS            0x3834
#define TX_INTERPRETER_MESSAGE_SETRFCARRIER_XTAL_ANS            0x3931
#define TX_INTERPRETER_MESSAGE_SETLED_ANS                       0x3837
#define TX_INTERPRETER_MESSAGE_RESETTEST_ANS                    0x3838
#define TX_INTERPRETER_MESSAGE_SET_U_ID_ANS                     0x3841
#define TX_INTERPRETER_MESSAGE_SET_PARAM_ANS                    0x3842
#define TX_INTERPRETER_MESSAGE_SET_SERIALNO_ANS                 0x3843
#define TX_INTERPRETER_MESSAGE_GETSOFTWAREVERSION_ANS           0x3844
#define TX_INTERPRETER_MESSAGE_SETHARDWAREREVISION_ANS          0x3845
#define TX_INTERPRETER_MESSAGE_TXTEST_ANS                       0x3933
#define TX_INTERPRETER_MESSAGE_PAIRING_ANS                      0x4530
#define TX_INTERPRETER_MESSAGE_SETCHARBROADCAST_ANS             0x4531
#define TX_INTERPRETER_MESSAGE_869MHZ_SET_FREQUENCY_ANS         0x4532
#define TX_INTERPRETER_MESSAGE_869MHZ_SET_OUTPUTPOWER_ANS       0x4533

/* Variables */

/* Function definitions */
int txInterpreter_CheckReceptionOfMessageComplete(uint16_t cmd, ringbuffer *txInterpreter_rb);
  
#endif
