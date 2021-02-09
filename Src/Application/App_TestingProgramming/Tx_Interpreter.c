/**
  ******************************************************************************
  * @file       Tx_Interpreter.c
  * @author     Tim Steinberg
  * @date       12.11.2020
  * @brief      Interpreter for the Tx-Series
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
#include "Runmode_Awake.h"
#include "Ringbuffer.h"
#include "UART_Tester.h"
#include "Tx_Interpreter_Messages.h"
#include "Tx_Misc.h"
#include "Tx_CommandExecuter.h"
#include "GPIO.h"
#include "ADC.h"
#include "RTC.h"
#include "Watchdog.h"

#include "EEPROMCheck.h"
#include "FlashCheck.h"

#include "Batterylevel.h"
#include "Runmode_Awake.h"

/* Typedefinitions / Prototypes */
#define TX_INTERPRETER_RBBUFFER_LENGTH 128

/* Variables */
uint8_t txInterpreter_rbBuffer[TX_INTERPRETER_RBBUFFER_LENGTH];

ringbuffer txInterpreter_rb = {
  .length = TX_INTERPRETER_RBBUFFER_LENGTH,
  .count = 0,
  .readptr = txInterpreter_rbBuffer, 
  .writeptr = txInterpreter_rbBuffer,
  .buffer = txInterpreter_rbBuffer,          
};

/* Function definitions */
void txInterpreter_uartReceptionCallback(){
  ringbufferPutChar(&txInterpreter_rb, uart_tester_getReceptedByte());
  uart_tester_startReception();
}

bool txInterpreter_CheckStartCondition(){
  gpio_activateTestPin();
  HAL_Delay(5);
  if (HAL_GPIO_ReadPin(GPI_TEST_MODE_GPIO_Port, GPI_TEST_MODE_Pin) == GPIO_PIN_RESET){
    gpio_deactivateTestPin();
    return TRUE;
  }
  return FALSE;
}

void txInterpreter_Setup(void){
  uart_tester_deInit();
  uart_tester_init();
}
  
void txInterpreter_DoTestMode(void){
  uint8_t answerField[128];
  int cmd;
  int temp;
  
  if (txInterpreter_CheckStartCondition() == FALSE){
    return;
  }
    
  watchdog_init_lowspeed();
  watchdog_feed();
  
  rtc_init();
  
  adc_doInitialCalibration();
  
  txInterpreter_Setup();
  
  ringbufferClear(&txInterpreter_rb);
  
  if (eepromCheck_UpdateEEPROMCRC() == FALSE){
    do{}while(1);
  }
  
#warning "DELETE THIS IF FOR RELEASE!"
  if (flashCheck_UpdateEEPROMCRC() == FALSE){
    do{}while(1);
  }

  if (flashCheck_CheckFlashCRC() == FALSE){
    do{}while(1);
  }
  
  TXM_SendBoot();
  gpio_userButtonArmedMode();
  
  do{
    watchdog_feed();
    
    // Are at least 2 bytes in buffer?
    if (ringbufferGetCount(&txInterpreter_rb) < 2){
      if (ringbufferGetCount(&txInterpreter_rb) == 1){
        if (ringbufferPeekCharPosition(&txInterpreter_rb, 0) == TX_INTERPRETER_SYMBOL_CR){
          ringbufferGetChar(&txInterpreter_rb);
          TXM_SendHeader();
          TXM_SendPrompt();
        }
      }
      continue;
    }
    
    // Peek the 2 bytes which the cmd is made of
    cmd = ringbufferPeekCharPosition(&txInterpreter_rb, 0) << 8;
    cmd += ringbufferPeekCharPosition(&txInterpreter_rb, 1);

    // Check if the message is existent and complete
    temp = txInterpreter_CheckReceptionOfMessageComplete((uint16_t) cmd, &txInterpreter_rb);
    if (temp == 0){
      //Not enough bytes yet
      continue;
    }
    if (temp < 0){
      //Something went wrong, clear buffer
      TXM_Negative(&txInterpreter_rb, answerField, (uint16_t)cmd);
      TXM_SendPrompt();
      continue;
    }
    
    HAL_Delay(10);
    
    switch(cmd){
      case TX_INTERPRETER_MESSAGE_MEASUREBATTERYLEVEL:
        TXCE_MeasureBatteryLevel(&txInterpreter_rb);
        break;
        
      case TX_INTERPRETER_MESSAGE_GETSOFTWAREVERSION:
        TXCE_GetSoftwareVersion(&txInterpreter_rb);
        break;
        
      case TX_INTERPRETER_MESSAGE_SETHARDWAREREVISION:
        TXCE_SetHardwareRevision(&txInterpreter_rb);
        break;
        
      case TX_INTERPRETER_MESSAGE_SETRFCARRIER_TCXO:
        TXCE_SetRFCarrier_TCXO(&txInterpreter_rb);
        break;
        
      case TX_INTERPRETER_MESSAGE_SETRFCARRIER_XTAL:
        TXCE_SetRFCarrier_XTAL(&txInterpreter_rb);
        break;
        
      case TX_INTERPRETER_MESSAGE_SETLED:
        TXCE_SetLED(&txInterpreter_rb);
        break;
        
      case TX_INTERPRETER_MESSAGE_RESETTEST:
        TXCE_ResetTest(&txInterpreter_rb);
        break;
        
      case TX_INTERPRETER_MESSAGE_SET_U_ID:
        TXCE_SetUID(&txInterpreter_rb);
        break;
        
      case TX_INTERPRETER_MESSAGE_SET_PARAM:
        TXCE_SetParam(&txInterpreter_rb);
        break;
        
      case TX_INTERPRETER_MESSAGE_GET_PARAM:
        TXCE_GetParam(&txInterpreter_rb);
        break;
        
      case TX_INTERPRETER_MESSAGE_SET_SERIALNO:
        TXCE_SetSerialNo(&txInterpreter_rb);
        break;
        
      case TX_INTERPRETER_MESSAGE_PAIRING:
        TXCE_Pairing(&txInterpreter_rb);
        break;
        
      case TX_INTERPRETER_MESSAGE_SETCHARBROADCAST:
        TXCE_SetCharStartBroadcast(&txInterpreter_rb);
        break;
      
      case TX_INTERPRETER_MESSAGE_869MHZ_SET_FREQUENCY:
        TXCE_SetS2LPSynth(&txInterpreter_rb);
        break;
      
      case TX_INTERPRETER_MESSAGE_869MHZ_SET_OUTPUTPOWER:
        TXCE_SetS2LPOutputPower(&txInterpreter_rb);
        break;

      default:
        
        break;
        
    }
    ringbufferClear(&txInterpreter_rb);
    TXM_SendPrompt();
  }while(1);
}
  
