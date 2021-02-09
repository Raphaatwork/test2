/**
  ******************************************************************************
  * @file       Tx_Misc.c
  * @author     Tim Steinberg
  * @date       17.11.2020
  * @brief      Supportive functions for the testing protocol
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
  * | 001       | 2020-11-17    | Tim Steinberg         | Initial version / skeleton of file            |
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
#include "Ringbuffer.h"
#include "UART_Tester.h"
#include "Tx_Interpreter_Messages.h"

/* Typedefinitions / Prototypes */

/* Variables */

/* Function definitions */

void TXM_FillCmdInField(uint16_t cmd, uint8_t *field, int *position){
  int pos = *position;
  field[pos++] = (uint8_t)(cmd >> 8);
  field[pos++] = (uint8_t)(cmd & 0xFF);
  *position = pos;
}

void TXM_AppendCRLFAndSend(uint8_t *field, uint32_t len){
  field[len] = TX_INTERPRETER_SYMBOL_CR; len++;
  field[len] = TX_INTERPRETER_SYMBOL_LF; len++;
  uart_tester_transmit(len, field);
}

void TXM_Negative(ringbuffer *txInterpreter_rb, uint8_t *field, uint16_t cmd){
  int position = 0;
  ringbufferClear(txInterpreter_rb);
  TXM_FillCmdInField(cmd, field, &position);
  TXM_AppendCRLFAndSend(field, position);
}

void TXM_RingbufferDrop2Bytes(ringbuffer *txInterpreter_rb){
  // Drop byte 1
  ringbufferGetChar(txInterpreter_rb);
  // Drop byte 2
  ringbufferGetChar(txInterpreter_rb);
}

void TXM_RingbufferDrop1Byte(ringbuffer *txInterpreter_rb){
  // Drop byte 1
  ringbufferGetChar(txInterpreter_rb);
}

void TXM_SendHeader(void){
  uint8_t preambleA[18] = "******************";
  uint8_t preambleB[18] = "*  TeleAlarm SA  *";
  uint8_t preambleC[18] = "*  Radio device  *";
  uint8_t preambleD[13] = "03.B00.01.797";
  uint8_t crlf[2] = {TX_INTERPRETER_SYMBOL_CR, TX_INTERPRETER_SYMBOL_LF};
  
  uart_tester_transmit(18, preambleA);
  uart_tester_transmit(2, crlf);
  uart_tester_transmit(18, preambleB);
  uart_tester_transmit(2, crlf);
  uart_tester_transmit(18, preambleC);
  uart_tester_transmit(2, crlf);
  uart_tester_transmit(18, preambleA);
  uart_tester_transmit(2, crlf);
  uart_tester_transmit(13, preambleD);
  uart_tester_transmit(2, crlf);
}

void TXM_SendPrompt(void){
  uint8_t prompt[18] = "TxGo> ";
  
  uart_tester_transmit(6, prompt);
}

void TXM_SendBoot(void){
  uint8_t txInterpreterMessageBooted[2] = {0x38, 0x30};
  uint8_t crlf[2] = {TX_INTERPRETER_SYMBOL_CR, TX_INTERPRETER_SYMBOL_LF};
  
  uart_tester_transmit(2, txInterpreterMessageBooted);
  uart_tester_transmit(2, crlf);
}