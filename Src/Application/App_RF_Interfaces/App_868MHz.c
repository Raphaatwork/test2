/**
  ******************************************************************************
  * @file       App_868MHz.c
  * @author     Tim Steinberg
  * @date       10.11.2020
  * @brief      Code for Interaction with the S2LP
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
  * | 001       | 2020-11-10    | Tim Steinberg         | Initial version / skeleton of file            |
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
#include "MasterDefine.h"
#include "Runmode_Awake.h"
#include "GPIO.h"
#include "Watchdog.h"
#include "EEPROM_ApplicationMapped.h"
#include "S2LP.h"
#include "App_868MHz_MessageBuilder.h"
#include "App_868MHz_Sequencer.h"

/* Typedefinitions / Prototypes */

/* Variables */
bitfieldTimed bitArrayPreamble[2];
bitfield bitsPreamble = {
  .bitsToSend = bitArrayPreamble,
  .countOfBits = 16,
  .countOfBitsUsed = 0,
};

bitfieldTimed bitArrayMessage[100];
bitfield bitsMessage = {
  .bitsToSend = bitArrayMessage,
  .countOfBits = 100,
  .countOfBitsUsed = 0,
};

void app_868mhz_s2lp_setupWorking(void){
  S2LP_SetConfig_WorkingMode();
   
  // ENTERING CRITICAL SECTION
  watchdog_feed();
  //__disable_irq();
  
}

void app_868mhz_s2lp_setupShutdown(void){
  watchdog_feed();
  //__enable_irq(); 
  // LEAVING CRITICAL SECTION
  
  S2LP_SetConfig_SleepMode();
}

void interMessageDelay(void){
  runmode_awake_configClocktreeMidspeed();
  HAL_Delay(800);
  runmode_awake_configClocktreeHighspeedHSI();
}

static void app_868mhz_slip(){
  watchdog_feed();
  app_868mhz_s2lp_setupWorking();
  
  app_868MHz_sequencer_output(&bitsPreamble); 
  
  app_868MHz_sequencer_output(&bitsMessage); 
  app_868MHz_sequencer_output(&bitsMessage); 
  app_868MHz_sequencer_output(&bitsMessage); 
  app_868MHz_sequencer_output(&bitsMessage); 
  app_868MHz_sequencer_output(&bitsMessage); 
  app_868MHz_sequencer_output(&bitsMessage); 
  app_868MHz_sequencer_output(&bitsMessage); 
  app_868MHz_sequencer_output(&bitsMessage); 
  app_868MHz_sequencer_output(&bitsMessage); 
  app_868MHz_sequencer_output(&bitsMessage); 
  
  app_868MHz_sequencer_output(&bitsMessage); 
  app_868MHz_sequencer_output(&bitsMessage); 
  app_868MHz_sequencer_output(&bitsMessage); 
  app_868MHz_sequencer_output(&bitsMessage); 
  app_868MHz_sequencer_output(&bitsMessage); 
  app_868MHz_sequencer_output(&bitsMessage); 
  app_868MHz_sequencer_output(&bitsMessage); 
  app_868MHz_sequencer_output(&bitsMessage); 
  app_868MHz_sequencer_output(&bitsMessage);
  
  watchdog_feed();
  app_868mhz_s2lp_setupShutdown();
}

void app_868mhz_transmitDynamicMessage(uint8_t *message){
  watchdog_feed();
  app_868MHz_buildMessage_preamble(&bitsPreamble);
  app_868MHz_buildMessage_emergency(message, 7, &bitsMessage);
  
  app_868mhz_slip();
  interMessageDelay();
  app_868mhz_slip();
  interMessageDelay();
  app_868mhz_slip();
}
  
/* Function definitions */
void app_868mhz_transmitMessage(BOOLEAN alertOnOff, BOOLEAN batteryLow){
  uint8_t messageField[7];
  uint32_t uID;
      
  /* REAL VALUES ASSIGNMENT */
  watchdog_feed();
  
  messageField[0] = 0x7E;
  if (batteryLow == TRUE){
    messageField[1] = 0x80;
  }
  if (alertOnOff == TRUE){
    messageField[1] |= 0x01; //messageField[1] |= 0x0F; //0x01 = "Alarm Button", 0x0F = "TxV2"
  }else{
    messageField[1] |= 0x71;
  }
  uID = eeprom_getUID();
  messageField[2] = (uint8_t)((uID & 0xFF));
  messageField[3] = (uint8_t)((uID >> 8) & 0xFF);
  messageField[4] = (uint8_t)((uID >> 16) & 0xFF);
  messageField[5] = (uint8_t)((uID >> 24) & 0xFF);
  messageField[6] = 0x00;

  watchdog_feed();
  app_868MHz_buildMessage_preamble(&bitsPreamble);
  app_868MHz_buildMessage_emergency(messageField, 7, &bitsMessage);
  
  app_868mhz_slip();
  interMessageDelay();
  app_868mhz_slip();
  interMessageDelay();
  app_868mhz_slip();
  
  /*  
  do{
    NVIC_SystemReset();
  }while(1);
  */
}

void app_868MHz_test_NOT(void);
void app_868MHz_test_NOT_LB(void);
void app_868MHz_test_Datecode(void);
void app_868MHz_test_Serialnumber(void);
void app_868MHz_test_Mixed(void);

void app_868MHz_test(void){
  app_868MHz_test_NOT();
  app_868MHz_test_NOT_LB();
  app_868MHz_test_Datecode();
  app_868MHz_test_Serialnumber();
  app_868MHz_test_Mixed();
}

void app_868MHz_test_NOT(void){
  const uint8_t messages[][7] = {
    {0x7E, 0x01, 0x03, 0xE0, 0x12, 0xFF, 0x00},
    {0x01, 0x01, 0x03, 0xE0, 0x12, 0xFF, 0x00},
    {0x30, 0x01, 0x03, 0xE0, 0x12, 0xFF, 0x00},
    {0x35, 0x01, 0x03, 0xE0, 0x12, 0xFF, 0x00},
    {0x60, 0x01, 0x03, 0xE0, 0x12, 0xFF, 0x00},
    {0x67, 0x01, 0x03, 0xE0, 0x12, 0xFF, 0x00},
    {0x6F, 0x01, 0x03, 0xE0, 0x12, 0xFF, 0x00},
    {0x70, 0x01, 0x03, 0xE0, 0x12, 0xFF, 0x00},
    {0x7F, 0x01, 0x03, 0xE0, 0x12, 0xFF, 0x00},
    {0x80, 0x01, 0x03, 0xE0, 0x12, 0xFF, 0x00},
    {0x81, 0x01, 0x03, 0xE0, 0x12, 0xFF, 0x00},
    {0x99, 0x01, 0x03, 0xE0, 0x12, 0xFF, 0x00},
    {0xAA, 0x01, 0x03, 0xE0, 0x12, 0xFF, 0x00},
    {0xFE, 0x01, 0x03, 0xE0, 0x12, 0xFF, 0x00},
    {0xFF, 0x01, 0x03, 0xE0, 0x12, 0xFF, 0x00},
  };
  for (int i = 0; i < 15; i++){
    for (int j = 0; j < 3; j++){
      app_868mhz_transmitDynamicMessage((uint8_t*) messages[i]);
      HAL_Delay(11000);
    }
    HAL_Delay(3000);
  }
}

void app_868MHz_test_NOT_LB(void){
  const uint8_t messages[][7] = {
    {0x01, 0x81, 0x03, 0xE0, 0x12, 0xFF, 0x00},
    {0x11, 0x81, 0x03, 0xE0, 0x12, 0xFF, 0x00},
    {0x30, 0x81, 0x03, 0xE0, 0x12, 0xFF, 0x00},
    {0x60, 0x81, 0x03, 0xE0, 0x12, 0xFF, 0x00},
    {0x7F, 0x81, 0x03, 0xE0, 0x12, 0xFF, 0x00},
    {0x80, 0x81, 0x03, 0xE0, 0x12, 0xFF, 0x00},
    {0x99, 0x81, 0x03, 0xE0, 0x12, 0xFF, 0x00},
    {0xAA, 0x81, 0x03, 0xE0, 0x12, 0xFF, 0x00},
    {0xFE, 0x81, 0x03, 0xE0, 0x12, 0xFF, 0x00},
    {0xFF, 0x81, 0x03, 0xE0, 0x12, 0xFF, 0x00},
  };
  for (int i = 0; i < 10; i++){
    for (int j = 0; j < 3; j++){
      app_868mhz_transmitDynamicMessage((uint8_t*) messages[i]);
      HAL_Delay(11000);
    }
    HAL_Delay(3000);
  }
}

void app_868MHz_test_Datecode(void){
  const uint8_t messages[][7] = {
    {0x7E, 0x01, 0x03, 0x71, 0x12, 0xFF, 0x00},
    {0x7E, 0x01, 0x01, 0xC6, 0x12, 0xFF, 0x00},
    {0x7E, 0x01, 0x00, 0x89, 0x12, 0xFF, 0x00},
    {0x7E, 0x01, 0x01, 0x51, 0x12, 0xFF, 0x00},
    
  };
  for (int i = 0; i < 4; i++){
    for (int j = 0; j < 2; j++){
      // Call for you to learn this transmitter to TA
      app_868mhz_transmitDynamicMessage((uint8_t*) messages[i]);
      HAL_Delay(3000);
    }
    HAL_Delay(3000);
    for (int j = 0; j < 3; j++){
      app_868mhz_transmitDynamicMessage((uint8_t*) messages[i]);
      HAL_Delay(11000);
    }
    HAL_Delay(3000);
  }
}

void app_868MHz_test_Serialnumber(void){
  const uint8_t messages[][7] = {
    {0x7E, 0x01, 0x03, 0xE0, 0x00, 0x01, 0x00},
    {0x7E, 0x01, 0x03, 0xE0, 0x70, 0x00, 0x00},
    {0x7E, 0x01, 0x03, 0xE0, 0x12, 0x00, 0x00},
    {0x7E, 0x01, 0x03, 0xE0, 0x00, 0xFF, 0x00},
    
  };
  for (int i = 0; i < 4; i++){
    for (int j = 0; j < 2; j++){
      // Call for you to learn this transmitter to TA
      app_868mhz_transmitDynamicMessage((uint8_t*) messages[i]);
      HAL_Delay(3000);
    }
    HAL_Delay(3000);
    for (int j = 0; j < 3; j++){
      app_868mhz_transmitDynamicMessage((uint8_t*) messages[i]);
      HAL_Delay(11000);
    }
    HAL_Delay(3000);
  }
}

void app_868MHz_test_Mixed(void){
  const uint8_t messages[][7] = {
    {0x80, 0x01, 0x00, 0x89, 0x00, 0x01, 0x00},
    {0xFF, 0x81, 0x01, 0xC6, 0x0F, 0x0F, 0x00},
  };
  for (int i = 0; i < 2; i++){
    for (int j = 0; j < 2; j++){
      // Call for you to learn this transmitter to TA
      app_868mhz_transmitDynamicMessage((uint8_t*) messages[i]);
      HAL_Delay(3000);
    }
    HAL_Delay(3000);
    for (int j = 0; j < 3; j++){
      app_868mhz_transmitDynamicMessage((uint8_t*) messages[i]);
      HAL_Delay(11000);
    }
    HAL_Delay(3000);
  }
}