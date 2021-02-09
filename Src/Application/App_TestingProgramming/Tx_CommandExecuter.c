/**
  ******************************************************************************
  * @file       Tx_CommandExecuter.c
  * @author     Tim Steinberg
  * @date       16.11.2020
  * @brief      Execution of received testing stuff.
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
  * | 001       | 2020-11-16    | Tim Steinberg         | Initial version / skeleton of file            |
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
#include "Tx_Interpreter.h"
#include "Tx_Interpreter_Messages.h"
#include "Tx_Misc.h"
#include "Tools.h"
#include "EEPROM_ApplicationMapped.h"
#include "Batterylevel.h"
#include "Led.h"
#include "GPIO.h"
#include "ADC.h"

/* Typedefinitions / Prototypes */

/* Variables */

/* Function definitions */
void TXCE_MeasureBatteryLevel(ringbuffer *txInterpreter_rb){
  uint8_t answerField[128];
  uint32_t temp32;
  int pos;
  
  // Drop the cmd bytes
  TXM_RingbufferDrop2Bytes(txInterpreter_rb);
  
  // Drop the CR
  ringbufferGetChar(txInterpreter_rb);
  
  pos = 0;
  TXM_FillCmdInField(TX_INTERPRETER_MESSAGE_MEASUREBATTERYLEVEL_ANS, answerField, &pos);
  answerField[pos] = TX_INTERPRETER_SYMBOL_SPACE; pos++;
  
  temp32 = batterylevel_mV();
  ultoaf(temp32, (char*) &(answerField[pos]), 4); pos += 4;
  
  TXM_AppendCRLFAndSend(answerField, pos);
}

void TXCE_GetSoftwareVersion(ringbuffer *txInterpreter_rb){
  uint8_t answerField[128];
  int pos;
  
  // Drop the cmd bytes
  TXM_RingbufferDrop2Bytes(txInterpreter_rb);
  
  // Drop the CR
  ringbufferGetChar(txInterpreter_rb);
  
  pos = 0;
  TXM_FillCmdInField(TX_INTERPRETER_MESSAGE_GETSOFTWAREVERSION_ANS, answerField, &pos);
  answerField[pos] = TX_INTERPRETER_SYMBOL_SPACE; pos++;
  
#warning I dont exactly know in which format they want to have this. I assume that they want to see decimal numbers without stuffed '0's
  pos += u32toString((uint32_t)eeprom_getSoftwareVersion_Type(), (char*) &answerField[pos]);
  answerField[pos] = TX_INTERPRETER_SYMBOL_DOT; pos++;
  if (eeprom_getSoftwareVersion_Status() == TX_INTERPRETER_SYMBOL_A){
    answerField[pos] = TX_INTERPRETER_SYMBOL_A; pos++;
  }else{
    if (eeprom_getSoftwareVersion_Status() == TX_INTERPRETER_SYMBOL_B){
      answerField[pos] = TX_INTERPRETER_SYMBOL_B; pos++;
    }else{
      answerField[pos] = '?'; pos++; // Show sign ?
    }
  }
  pos += u32toString((uint32_t)eeprom_getSoftwareVersion_Major(), (char*) &answerField[pos]);
  answerField[pos] = TX_INTERPRETER_SYMBOL_DOT; pos++;
  pos += u32toString((uint32_t)eeprom_getSoftwareVersion_Minor(), (char*) &answerField[pos]);
  answerField[pos] = TX_INTERPRETER_SYMBOL_DOT; pos++;
  pos += u32toString((uint32_t)eeprom_getSoftwareVersion_Build(), (char*) &answerField[pos]);
  answerField[pos] = TX_INTERPRETER_SYMBOL_DOT; pos++;
  pos += u32toString((uint32_t)eeprom_getSoftwareVersion_CMI(), (char*) &answerField[pos]);
  
  TXM_AppendCRLFAndSend(answerField, pos);
}

void TXCE_SetHardwareRevision(ringbuffer *txInterpreter_rb){
  uint8_t answerField[128];
  uint8_t temp8Arr[2];
  uint8_t temp8;
  uint8_t pcbVer;
  uint8_t bomVer;
  int pos;
  
  // Drop the cmd bytes
  TXM_RingbufferDrop2Bytes(txInterpreter_rb);
  
  // Drop the space after cmd byte
  ringbufferGetChar(txInterpreter_rb);
  
  // Both incoming values are decimal coded digits. 
  // Since d13 means d13 and not h13 (wich is 0001 0011 and thus 19)
  // we will have to use a calculator function to switch that.
  // Afterwards we have to write that value in the EEPROM.
  temp8Arr[0] = ringbufferGetChar(txInterpreter_rb);
  temp8Arr[1] = ringbufferGetChar(txInterpreter_rb);
  temp8 = hex2u8((char*) temp8Arr);
  pcbVer = temp8;
  
  eeprom_setPCBVersion(pcbVer);
  
  temp8Arr[0] = ringbufferGetChar(txInterpreter_rb);
  temp8Arr[1] = ringbufferGetChar(txInterpreter_rb);
  temp8 = hex2u8((char*) temp8Arr);
  bomVer = temp8;
  
  eeprom_setBOMVersion(bomVer);

  // Drop the CR
  ringbufferGetChar(txInterpreter_rb);
  
  pos = 0;
  TXM_FillCmdInField(TX_INTERPRETER_MESSAGE_SETHARDWAREREVISION_ANS, answerField, &pos);
  
  TXM_AppendCRLFAndSend(answerField, pos);
}

#include "S2LP.h"
#include "SPI.h"

void TXCE_SetRFCarrier_TCXO(ringbuffer *txInterpreter_rb){
  uint8_t answerField[128];
  uint8_t temp8         = 0;
  int pos               = 0;
  
  // Drop the cmd bytes
  TXM_RingbufferDrop2Bytes(txInterpreter_rb);
  
  // Drop the space after cmd byte
  ringbufferGetChar(txInterpreter_rb);
  
  // Get the parameter
  temp8 = ringbufferGetChar(txInterpreter_rb);
  
  // Drop the CR
  ringbufferGetChar(txInterpreter_rb);
  
  // Set the clocksoruce to TCXO
  eeprom_setS2LP_ClocksourceSelector(CLOCKTYPE_SELECTOR_TCXO);

  switch(temp8){
    case 0x30:
      // Turn off RF CW
      S2LP_SetConfig_CWInactiveMode();
      break;
      
    case 0x31:
      // Turn on RF CW
      S2LP_SetConfig_CWActiveMode();
      break;
      
    default:
      TXM_Negative(txInterpreter_rb, answerField, TX_INTERPRETER_MESSAGE_SETRFCARRIER_TCXO);
      return;
      break;
  }
  
  TXM_FillCmdInField(TX_INTERPRETER_MESSAGE_SETRFCARRIER_TCXO_ANS, answerField, &pos);
  TXM_AppendCRLFAndSend(answerField, pos);
}

void TXCE_SetRFCarrier_XTAL(ringbuffer *txInterpreter_rb){
  uint8_t answerField[128];
  uint8_t cmd;
  uint8_t temp8;
  uint16_t temp16;
  uint8_t temp8Arr[2];
  uint32_t freq, freqR;
  int pos = 0;
  
  // Drop the cmd bytes
  TXM_RingbufferDrop2Bytes(txInterpreter_rb);
  
  // Drop the space before cmd byte
  ringbufferGetChar(txInterpreter_rb);
  
  // Get the parameter
  cmd = ringbufferGetChar(txInterpreter_rb);
  
  // Set the clocksoruce to XTAL
  eeprom_setS2LP_ClocksourceSelector(CLOCKTYPE_SELECTOR_XTAL);
  
  if ((cmd == 0x31) || (cmd == 0x32)){
    // Drop the space after cmd byte
    ringbufferGetChar(txInterpreter_rb);
  
    if (ringbufferGetCount(txInterpreter_rb) < 5){
      TXM_Negative(txInterpreter_rb, answerField, TX_INTERPRETER_MESSAGE_SETRFCARRIER_XTAL);
      return;
    }
    // MSByte
    temp8Arr[0] = ringbufferGetChar(txInterpreter_rb);
    temp8Arr[1] = ringbufferGetChar(txInterpreter_rb);
    temp8 = hex2u8((char*) temp8Arr);
    temp16 = temp8 << 8;
    // LSByte
    temp8Arr[0] = ringbufferGetChar(txInterpreter_rb);
    temp8Arr[1] = ringbufferGetChar(txInterpreter_rb);
    temp8 = hex2u8((char*) temp8Arr);
    temp16 += temp8;
  }
  
  // Drop the CR
  ringbufferGetChar(txInterpreter_rb);
  
  switch(cmd){
    case 0x30:
      // Turn on RF CW
      S2LP_SetConfig_CWActiveMode();
      break;
      
    case 0x31:
      // Get f from EEPROM in reverse (due to add)
      freqR =  eeprom_getS2LP_Synth3() << 24;
      freqR += eeprom_getS2LP_Synth2() << 16;
      freqR += eeprom_getS2LP_Synth1() << 8;
      freqR += eeprom_getS2LP_Synth0() << 0;
      // Add 23.8 Hz steps to reverse f
      freqR += temp16;
      // Get f from EEPROM
      freq =  (freqR & 0x000000FF) << 24;
      freq += (freqR & 0x0000FF00) << 8;
      freq += (freqR & 0x00FF0000) >> 8;
      freq += (freqR & 0xFF000000) >> 24;
      // Safe back to EEPROM
      eeprom_setS2LP_Synth_3_0(freq);
      // Turn off RF CW
      S2LP_SetConfig_CWInactiveMode();
      // Turn on RF CW
      S2LP_SetConfig_CWActiveMode();
      break;
      
    case 0x32:
      // Get f from EEPROM in reverse (due to add)
      freqR =  eeprom_getS2LP_Synth3() << 24;
      freqR += eeprom_getS2LP_Synth2() << 16;
      freqR += eeprom_getS2LP_Synth1() << 8;
      freqR += eeprom_getS2LP_Synth0() << 0;
      // Add 23.8 Hz steps to reverse f
      freqR -= temp16;
      // Get f from EEPROM
      freq =  (freqR & 0x000000FF) << 24;
      freq += (freqR & 0x0000FF00) << 8;
      freq += (freqR & 0x00FF0000) >> 8;
      freq += (freqR & 0xFF000000) >> 24;
      // Safe back to EEPROM
      eeprom_setS2LP_Synth_3_0(freq);
      // Turn off RF CW
      S2LP_SetConfig_CWInactiveMode();
      // Turn on RF CW
      S2LP_SetConfig_CWActiveMode();
      break;
      
    case 0x33:
      // We're already working in EEPROM, so no need to safe it back
      // Turn off RF CW
      S2LP_SetConfig_CWInactiveMode();
      break;
      
    default:
      TXM_Negative(txInterpreter_rb, answerField, TX_INTERPRETER_MESSAGE_SETRFCARRIER_XTAL);
      return;
      break;
  }
  
  TXM_FillCmdInField(TX_INTERPRETER_MESSAGE_SETRFCARRIER_XTAL_ANS, answerField, &pos);
  TXM_AppendCRLFAndSend(answerField, pos);
}

void TXCE_SetLED(ringbuffer *txInterpreter_rb){
  uint8_t answerField[128];
  uint8_t temp8;
  int pos;
  
  // Drop the cmd bytes
  TXM_RingbufferDrop2Bytes(txInterpreter_rb);
  
  // Drop the space after cmd byte
  ringbufferGetChar(txInterpreter_rb);
  
  // Get the parameter
  temp8 = ringbufferGetChar(txInterpreter_rb);
  switch(temp8){
    
    case '0':
      led_black();
      gpio_ledAnalogMode();
      break;
      
    case '1':
      gpio_ledOutputMode();
      led_red();
      break;
      
    case '2':
      gpio_ledOutputMode();
      led_blue();
      break;
      
    case '3':
      gpio_ledOutputMode();
      led_green();
      break;
      
    default:
      TXM_Negative(txInterpreter_rb, answerField, TX_INTERPRETER_MESSAGE_SETLED);
      return;
      break;
  }
  
  // Drop the CR
  ringbufferGetChar(txInterpreter_rb);
  
  pos = 0;
  TXM_FillCmdInField(TX_INTERPRETER_MESSAGE_SETLED_ANS, answerField, &pos);
  
  TXM_AppendCRLFAndSend(answerField, pos);
}

void TXCE_ResetTestDigital(ringbuffer *txInterpreter_rb, uint8_t paramPort){
  
}

void TXCE_ResetTestAnalog(ringbuffer *txInterpreter_rb, uint8_t paramPort, uint8_t paramPinHigh, uint8_t paramPinLow, uint8_t paramSetting){
  uint8_t answerField[128];
  uint32_t temp32;
  int pos;
  
  pos = 0;
  switch(paramPort){
    case 'a':
      
      switch(paramPinHigh){
        case '0':
      
          switch(paramPinLow){
            case '1':
            
              switch(paramSetting){
                case '0':
                  gpio_userButtonUnarmedMode();
                  break;
                  
                case '1':
                  gpio_userButtonMeasureMode();
                  break;
                  
                case '2':
                  gpio_userButtonArmedMode();
                  break;
                  
                default: // If you try to make a setting that does not exist
                  TXM_FillCmdInField(TX_INTERPRETER_MESSAGE_COMMANDS_RESETTEST, answerField, &pos);
                  TXM_AppendCRLFAndSend(answerField, pos);
                  return;
                  break;
              } // End switch paramSetting
              
              // Wait for the button to reach a stable voltage
              HAL_Delay(10);
              temp32 = adcBlocking_getButtonVoltage_fourOutOfSixAveraged();
              break;
              
            default: // If you try to reach a pin number that is not existing
              TXM_FillCmdInField(TX_INTERPRETER_MESSAGE_COMMANDS_RESETTEST, answerField, &pos);
              TXM_AppendCRLFAndSend(answerField, pos);
              return;
              break;
          } // End switch paramPinLow
          break;
          
        default: // If you try to reach a decade where there is no ping (e.g. 9)
          TXM_FillCmdInField(TX_INTERPRETER_MESSAGE_COMMANDS_RESETTEST, answerField, &pos);
          TXM_AppendCRLFAndSend(answerField, pos);
          return;
          break;
      } // End switch paramPinHigh
      break;
      
    default: // If the inserted port is not existing
      TXM_FillCmdInField(TX_INTERPRETER_MESSAGE_COMMANDS_RESETTEST, answerField, &pos);
      TXM_AppendCRLFAndSend(answerField, pos);
      return;
      break;
  } // End switch paramPort
  
  TXM_FillCmdInField(TX_INTERPRETER_MESSAGE_RESETTEST_ANS, answerField, &pos);
  answerField[pos] = TX_INTERPRETER_SYMBOL_SPACE; pos++;
  answerField[pos] = paramPort; pos++;
  answerField[pos] = TX_INTERPRETER_SYMBOL_SPACE; pos++;
  answerField[pos] = paramPinHigh; pos++;
  answerField[pos] = paramPinLow; pos++;
  answerField[pos] = TX_INTERPRETER_SYMBOL_SPACE; pos++;
  answerField[pos] = paramSetting; pos++;
  answerField[pos] = TX_INTERPRETER_SYMBOL_SPACE; pos++;
  ultoaf(temp32, (char*) &(answerField[pos]), 4); pos += 4;
  TXM_AppendCRLFAndSend(answerField, pos);
}

#include "Userbutton.h"

void TXCE_ResetTest(ringbuffer *txInterpreter_rb){
  uint8_t temp81;
  uint32_t buttonVal;
  uint32_t battery;
  uint32_t upper, lower, deadoff;
  uint8_t answerField[128];
  int pos = 0;

  // Drop the cmd bytes
  TXM_RingbufferDrop2Bytes(txInterpreter_rb);
  
  // Drop the space
  ringbufferGetChar(txInterpreter_rb);
  
  // Get the port parameter
  temp81 = ringbufferGetChar(txInterpreter_rb);
  
  // Analog or digital?
  if (ringbufferPeekCharPosition(txInterpreter_rb, 0) == TX_INTERPRETER_SYMBOL_CR){
    // Digital
    
    // Drop the CR
    ringbufferGetChar(txInterpreter_rb);
    
    if (temp81 != TX_INTERPRETER_SYMBOL_a){
      TXM_FillCmdInField(TX_INTERPRETER_MESSAGE_COMMANDS_RESETTEST, answerField, &pos);
      TXM_AppendCRLFAndSend(answerField, pos);
      return;
    }
    
    pos = 0;
    TXM_FillCmdInField(TX_INTERPRETER_MESSAGE_RESETTEST_ANS, answerField, &pos);
    answerField[pos] = TX_INTERPRETER_SYMBOL_SPACE; pos++;
    
    gpio_userButtonArmedMode();
    HAL_Delay(100);
    
    battery = batterylevel_mV();
    buttonVal = adcBlocking_getButtonVoltage_fourOutOfSixAveraged();
    
    upper = UserButtonGetLevel(battery, 1741, 11); // Upper level set to 85,00%, Userbutton set to more than 80%
    lower = UserButtonGetLevel(battery, 1229, 11); // Lower level set to 60,00%, Userbutton set to less than 70%
    deadoff = UserButtonGetLevel(battery, 819, 11); // Dead level set to 39,99%, Userbutton does not check this, since it can't start then anyway. 
    
    // Is button released?
    if (buttonVal > upper){
      answerField[pos] = TX_INTERPRETER_SYMBOL_0; pos++;
      answerField[pos] = TX_INTERPRETER_SYMBOL_0; pos++;
      answerField[pos] = TX_INTERPRETER_SYMBOL_0; pos++;
      answerField[pos] = TX_INTERPRETER_SYMBOL_0; pos++;
      TXM_AppendCRLFAndSend(answerField, pos);
      return;
    }
    // Is button pressed?
    if ((buttonVal < lower) && (buttonVal > deadoff)){
      answerField[pos] = TX_INTERPRETER_SYMBOL_0; pos++;
      answerField[pos] = TX_INTERPRETER_SYMBOL_0; pos++;
      answerField[pos] = TX_INTERPRETER_SYMBOL_0; pos++;
      answerField[pos] = TX_INTERPRETER_SYMBOL_1; pos++;
      TXM_AppendCRLFAndSend(answerField, pos);
      return;
    }
    // Is button dead?
    answerField[pos] = TX_INTERPRETER_SYMBOL_F; pos++;
    answerField[pos] = TX_INTERPRETER_SYMBOL_F; pos++;
    answerField[pos] = TX_INTERPRETER_SYMBOL_F; pos++;
    answerField[pos] = TX_INTERPRETER_SYMBOL_F; pos++;
    
    TXM_AppendCRLFAndSend(answerField, pos); 
    return;
  }
  
  TXM_Negative(txInterpreter_rb, answerField, TX_INTERPRETER_MESSAGE_RESETTEST);
}

void TXCE_SetUID(ringbuffer *txInterpreter_rb){
  uint8_t answerField[128];
  uint8_t temp8Arr[2];
  uint8_t uid[4];
  int pos               = 0; 
  int ans               = 0; (void) ans;

  // Drop the cmd bytes
  TXM_RingbufferDrop2Bytes(txInterpreter_rb);
  
  // Drop the space
  ringbufferGetChar(txInterpreter_rb);
  
  // Check if we got 8 or 9 digits. Only 8 are allowed
  if (ringbufferGetCount(txInterpreter_rb) == 10){
    // Yes, we got 9
    
    // Check that the last one is a CR
    if (ringbufferPeekCharPosition(txInterpreter_rb, 9) == TX_INTERPRETER_SYMBOL_CR){
      // Yes, the last one is a CR, so it's intended
      
      // Drop the leading digit (which is 1 for LeesysPlant)
      ringbufferGetChar(txInterpreter_rb);
    }else{
      TXM_Negative(txInterpreter_rb, answerField, TX_INTERPRETER_MESSAGE_SET_U_ID);
      return ;
    }
  }
  
  // Get first hexbyte
  temp8Arr[0] = ringbufferGetChar(txInterpreter_rb);
  temp8Arr[1] = ringbufferGetChar(txInterpreter_rb);
  uid[3] = hex2u8((char*) temp8Arr);
  
  // Get second hexbyte
  temp8Arr[0] = ringbufferGetChar(txInterpreter_rb);
  temp8Arr[1] = ringbufferGetChar(txInterpreter_rb);
  uid[2] = hex2u8((char*) temp8Arr);
  
  // Get third hexbyte
  temp8Arr[0] = ringbufferGetChar(txInterpreter_rb);
  temp8Arr[1] = ringbufferGetChar(txInterpreter_rb);
  uid[1] = hex2u8((char*) temp8Arr);
  
  // Get fourth hexbyte
  temp8Arr[0] = ringbufferGetChar(txInterpreter_rb);
  temp8Arr[1] = ringbufferGetChar(txInterpreter_rb);
  uid[0] = hex2u8((char*) temp8Arr);
  
  // Drop the CR
  ringbufferGetChar(txInterpreter_rb);
  
  eeprom_setUID(*(uint32_t*) uid);  
  TXM_FillCmdInField(TX_INTERPRETER_MESSAGE_SET_U_ID_ANS, answerField, &pos);
  TXM_AppendCRLFAndSend(answerField, pos);
}

void TXCE_SetSerialNumber(ringbuffer *txInterpreter_rb){
  uint8_t answerField[128];
  uint8_t temp;
  uint32_t serialNoSum;
  uint8_t serialNo[4];
  int pos               = 0; 
  int ans               = 0; (void) ans;

  // Drop the cmd bytes
  TXM_RingbufferDrop2Bytes(txInterpreter_rb);
  
  // Drop the space
  ringbufferGetChar(txInterpreter_rb);
  
  // Set actual sum to 0
  serialNoSum = 0;
  
  // Do while there is still a digit in the ringbuffer
  while(ringbufferGetCount(txInterpreter_rb) > 1){
    // Multiply sum with 10 to add next digit
    serialNoSum *= 10;
    
    temp = ringbufferGetChar(txInterpreter_rb);
    if ((temp < 0x30) || (temp > 0x39)){
      TXM_Negative(txInterpreter_rb, answerField, TX_INTERPRETER_MESSAGE_SET_SERIALNO);
      return;
    }
    serialNoSum += temp;
  }
  
  // Drop the CR
  ringbufferGetChar(txInterpreter_rb);
  
  // Turn endianess from little to big endian
  serialNo[0] = (uint8_t) ((serialNoSum & 0xFF000000) >> 24);
  serialNo[1] = (uint8_t) ((serialNoSum & 0x00FF0000) >> 16);
  serialNo[2] = (uint8_t) ((serialNoSum & 0x0000FF00) >>  8);
  serialNo[3] = (uint8_t) ((serialNoSum & 0x000000FF) >>  0);
  
  // Write serial number in eep
  eeprom_setSerialNumber(*(uint32_t*) serialNo);
  
  TXM_FillCmdInField(TX_INTERPRETER_MESSAGE_SET_SERIALNO_ANS, answerField, &pos);
  TXM_AppendCRLFAndSend(answerField, pos);
}
  
#include "EEPROM_Access.h"

void TXCE_SetSerialNo(ringbuffer *txInterpreter_rb){
  uint8_t answerField[128];
  uint32_t value = 0;
  uint8_t posVal;
  int pos = 0;
  
  // Drop the cmd bytes
  TXM_RingbufferDrop2Bytes(txInterpreter_rb);
  
  // Drop the space
  ringbufferGetChar(txInterpreter_rb);
  
  // Get hexbyte and add multiplied - 10 times
  for (int i = 1000000000; i >= 1; i /= 10){
    posVal = posValHexToDez(ringbufferGetChar(txInterpreter_rb));
    if (posVal == 0xFF){
      TXM_Negative(txInterpreter_rb, answerField, TX_INTERPRETER_MESSAGE_SET_SERIALNO);
      return;
    }
    value += posVal * i;
  }
  
  // Drop the CR
  ringbufferGetChar(txInterpreter_rb);
  
  // Write value
  eeprom_setSerialNumber(value);
  
  TXM_FillCmdInField(TX_INTERPRETER_MESSAGE_SET_SERIALNO_ANS, answerField, &pos);
  TXM_AppendCRLFAndSend(answerField, pos);
}

void TXCE_GetParam(ringbuffer *txInterpreter_rb){
  uint8_t answerField[128];
  uint8_t temp8Arr[2];
  uint8_t formattype;
  uint8_t paramID;
  uint32_t value;
  int pos               = 0;
  
  // Drop the cmd bytes
  TXM_RingbufferDrop2Bytes(txInterpreter_rb);
  
  // Drop the space
  ringbufferGetChar(txInterpreter_rb);
  
  // Get the format type
  formattype = ringbufferGetChar(txInterpreter_rb);
  
  // Drop the space
  ringbufferGetChar(txInterpreter_rb);
  
  // Get the address number as hexbyte
  temp8Arr[0]   = ringbufferGetChar(txInterpreter_rb);
  temp8Arr[1]   = ringbufferGetChar(txInterpreter_rb);
  paramID       = hex2u8((char*) temp8Arr);
  
  // Drop the CR
  ringbufferGetChar(txInterpreter_rb);
  
  switch(formattype){
    case TX_INTERPRETER_SYMBOL_b:
      TXM_FillCmdInField(TX_INTERPRETER_MESSAGE_SET_PARAM_ANS, answerField, &pos);
      answerField[pos] = TX_INTERPRETER_SYMBOL_SPACE; pos++;
      value = eeprom_getWord_byId(paramID);
      for (int i = 0; i < 32; i++){
        if ( ( (value >> (31 - i)) & 0x01) > 0){
          answerField[pos] = 0x31; pos++;
        }else{
          answerField[pos] = 0x30; pos++;
        }
      }
      break;
    
    case TX_INTERPRETER_SYMBOL_d:
      TXM_FillCmdInField(TX_INTERPRETER_MESSAGE_SET_PARAM_ANS, answerField, &pos);
      answerField[pos] = TX_INTERPRETER_SYMBOL_SPACE; pos++;
      value = eeprom_getWord_byId(paramID);
      ultoaf(value, (char*) &(answerField[pos]), 10); pos += 10;
      break;
    
    case TX_INTERPRETER_SYMBOL_h:
      TXM_FillCmdInField(TX_INTERPRETER_MESSAGE_SET_PARAM_ANS, answerField, &pos);
      answerField[pos] = TX_INTERPRETER_SYMBOL_SPACE; pos++;
      value = eeprom_getWord_byId(paramID);
      u82hex( (value & 0xFF000000) >> 24 , (char*) &(answerField[pos])); pos += 2;
      u82hex( (value & 0x00FF0000) >> 16 , (char*) &(answerField[pos])); pos += 2;
      u82hex( (value & 0x0000FF00) >> 8  , (char*) &(answerField[pos])); pos += 2;
      u82hex( (value & 0x000000FF) >> 0  , (char*) &(answerField[pos])); pos += 2;
      break;
      
    default:
      TXM_Negative(txInterpreter_rb, answerField, TX_INTERPRETER_MESSAGE_GET_PARAM);
      return;
      break;
  }

  TXM_AppendCRLFAndSend(answerField, pos);
}

void TXCE_SetParam(ringbuffer *txInterpreter_rb){
  uint8_t answerField[128];
  uint8_t temp8Arr[2];
  uint8_t value[4];
  uint8_t paramID;
  int pos               = 0;
  
  // Drop the cmd bytes
  TXM_RingbufferDrop2Bytes(txInterpreter_rb);
  
  // Drop the space
  ringbufferGetChar(txInterpreter_rb);
  
  // Get the parameter id as hexbyte
  temp8Arr[0]   = ringbufferGetChar(txInterpreter_rb);
  temp8Arr[1]   = ringbufferGetChar(txInterpreter_rb);
  paramID       = hex2u8((char*) temp8Arr);
  
  // Drop the space
  ringbufferGetChar(txInterpreter_rb);
  
  // Get first hexbyte
  temp8Arr[0]   = ringbufferGetChar(txInterpreter_rb);
  temp8Arr[1]   = ringbufferGetChar(txInterpreter_rb);
  value[3]      = hex2u8((char*) temp8Arr);
  
  // Get second hexbyte
  temp8Arr[0]   = ringbufferGetChar(txInterpreter_rb);
  temp8Arr[1]   = ringbufferGetChar(txInterpreter_rb);
  value[2]      = hex2u8((char*) temp8Arr);
  
  // Get third hexbyte
  temp8Arr[0]   = ringbufferGetChar(txInterpreter_rb);
  temp8Arr[1]   = ringbufferGetChar(txInterpreter_rb);
  value[1]      = hex2u8((char*) temp8Arr);
  
  // Get fourth hexbyte
  temp8Arr[0]   = ringbufferGetChar(txInterpreter_rb);
  temp8Arr[1]   = ringbufferGetChar(txInterpreter_rb);
  value[0]      = hex2u8((char*) temp8Arr);
  
  // Drop the CR
  ringbufferGetChar(txInterpreter_rb);
  
  eeprom_writeWord_byId_WithValueCheck(*(uint32_t*)value, paramID);
  
  TXM_FillCmdInField(TX_INTERPRETER_MESSAGE_SET_PARAM_ANS, answerField, &pos);
  TXM_AppendCRLFAndSend(answerField, pos);
}

#include "App_RSL_Interaction_Pairing.h"

void TXCE_Pairing(ringbuffer *txInterpreter_rb){ 
  uint8_t answerField[128];
  int pos               = 0;
  
  // Drop the cmd bytes
  TXM_RingbufferDrop2Bytes(txInterpreter_rb);
  
  // Drop the CR
  ringbufferGetChar(txInterpreter_rb);
  
  TXM_FillCmdInField(TX_INTERPRETER_MESSAGE_PAIRING_ANS, answerField, &pos);
  TXM_AppendCRLFAndSend(answerField, pos);
  
  app_rsl_interaction_pairing_main();
}

#include "UserMethods_Characteristics.h"
#include "App_RSL.h"
#include "BehaviourV115_Battery.h"
#include "BehaviourV115_SetAllCharacteristics.h"
#include "Led.h"

void TXCE_SetCharStartBroadcast(ringbuffer *txInterpreter_rb){ 
  uint8_t answerField[128];
  uint8_t temp8Arr[2];
  uint8_t alert;
  uint8_t battery;
  uint8_t error;
  int pos               = 0;
  
  // Drop the cmd bytes
  TXM_RingbufferDrop2Bytes(txInterpreter_rb);
  
  // Drop the Space
  ringbufferGetChar(txInterpreter_rb);
  
  // Get first hexbyte (alert)
  temp8Arr[0]   = ringbufferGetChar(txInterpreter_rb);
  temp8Arr[1]   = ringbufferGetChar(txInterpreter_rb);
  alert         = hex2u8((char*) temp8Arr);
  
  // Drop the Space
  ringbufferGetChar(txInterpreter_rb);
  
  // Get first hexbyte (battery)
  temp8Arr[0]   = ringbufferGetChar(txInterpreter_rb);
  temp8Arr[1]   = ringbufferGetChar(txInterpreter_rb);
  battery       = hex2u8((char*) temp8Arr);
  
  // Drop the Space
  ringbufferGetChar(txInterpreter_rb);
  
  // Get first hexbyte (error)
  temp8Arr[0]   = ringbufferGetChar(txInterpreter_rb);
  temp8Arr[1]   = ringbufferGetChar(txInterpreter_rb);
  error         = hex2u8((char*) temp8Arr);
  
  // Drop the CR
  ringbufferGetChar(txInterpreter_rb);
  
  TXM_FillCmdInField(TX_INTERPRETER_MESSAGE_SETCHARBROADCAST_ANS, answerField, &pos);
  TXM_AppendCRLFAndSend(answerField, pos);
  
  userMethods_characteristics_setAlert(alert);
  userMethods_characteristics_setBattery(battery);
  userMethods_characteristics_setError(error);
  app_rsl_handler_executeCommunication(50, 50, led_red, behaviourV115_setAllCharacteristics_prepare_and_get_struct);
  app_rsl_handler_executeCommunication(50, 50, led_red, behaviourV115_battery_prepare_and_get_struct);
  userMethods_characteristics_setAlert(0);
  userMethods_characteristics_setBattery(0);
  userMethods_characteristics_setError(0);
  app_rsl_handler_executeCommunication(50, 50, led_red, behaviourV115_setAllCharacteristics_prepare_and_get_struct);
  led_black();
}

void TXCE_SetS2LPSynth(ringbuffer *txInterpreter_rb){ 
  uint8_t answerField[128];
  int pos               = 0;
  uint8_t temp8Arr[2];
  uint32_t synth;
  
  // Drop the cmd bytes
  TXM_RingbufferDrop2Bytes(txInterpreter_rb);
  
  // Drop the Space
  ringbufferGetChar(txInterpreter_rb);
  
  // Set SYNTH 3
  temp8Arr[0]   = ringbufferGetChar(txInterpreter_rb);
  temp8Arr[1]   = ringbufferGetChar(txInterpreter_rb);
  synth         = hex2u8((char*) temp8Arr);
  
  // Set SYNTH 2
  temp8Arr[0]   = ringbufferGetChar(txInterpreter_rb);
  temp8Arr[1]   = ringbufferGetChar(txInterpreter_rb);
  synth         += hex2u8((char*) temp8Arr) << 8;
  
  // Set SYNTH 1
  temp8Arr[0]   = ringbufferGetChar(txInterpreter_rb);
  temp8Arr[1]   = ringbufferGetChar(txInterpreter_rb);
  synth         += hex2u8((char*) temp8Arr) << 16;
  
  // Set SYNTH 0
  temp8Arr[0]   = ringbufferGetChar(txInterpreter_rb);
  temp8Arr[1]   = ringbufferGetChar(txInterpreter_rb);
  synth         += hex2u8((char*) temp8Arr) << 24;
  
  eeprom_setS2LP_Synth_3_0(synth);
    
  // Drop the CR
  ringbufferGetChar(txInterpreter_rb);
  
  TXM_FillCmdInField(TX_INTERPRETER_MESSAGE_869MHZ_SET_FREQUENCY_ANS, answerField, &pos);
  TXM_AppendCRLFAndSend(answerField, pos);
}

void TXCE_SetS2LPOutputPower(ringbuffer *txInterpreter_rb){ 
  uint8_t answerField[128];
  int pos               = 0;
  uint8_t temp8Arr[2];
  uint32_t op85, op41, op0;
  
  // Drop the cmd bytes
  TXM_RingbufferDrop2Bytes(txInterpreter_rb);
  
  // Drop the Space
  ringbufferGetChar(txInterpreter_rb);
  
  // Set outputpower 8
  temp8Arr[0]   = ringbufferGetChar(txInterpreter_rb);
  temp8Arr[1]   = ringbufferGetChar(txInterpreter_rb);
  op85          = hex2u8((char*) temp8Arr);
  
  // Set outputpower 7
  temp8Arr[0]   = ringbufferGetChar(txInterpreter_rb);
  temp8Arr[1]   = ringbufferGetChar(txInterpreter_rb);
  op85          += hex2u8((char*) temp8Arr) << 8;
  
  // Set outputpower 6
  temp8Arr[0]   = ringbufferGetChar(txInterpreter_rb);
  temp8Arr[1]   = ringbufferGetChar(txInterpreter_rb);
  op85          += hex2u8((char*) temp8Arr) << 16;
  
  // Set outputpower 5
  temp8Arr[0]   = ringbufferGetChar(txInterpreter_rb);
  temp8Arr[1]   = ringbufferGetChar(txInterpreter_rb);
  op85          += hex2u8((char*) temp8Arr) << 24;
  
  //======
  
  // Set outputpower 4
  temp8Arr[0]   = ringbufferGetChar(txInterpreter_rb);
  temp8Arr[1]   = ringbufferGetChar(txInterpreter_rb);
  op41          = hex2u8((char*) temp8Arr);
  
  // Set outputpower 3
  temp8Arr[0]   = ringbufferGetChar(txInterpreter_rb);
  temp8Arr[1]   = ringbufferGetChar(txInterpreter_rb);
  op41          += hex2u8((char*) temp8Arr) << 8;
  
  // Set outputpower 2
  temp8Arr[0]   = ringbufferGetChar(txInterpreter_rb);
  temp8Arr[1]   = ringbufferGetChar(txInterpreter_rb);
  op41          += hex2u8((char*) temp8Arr) << 16;
  
  // Set outputpower 1
  temp8Arr[0]   = ringbufferGetChar(txInterpreter_rb);
  temp8Arr[1]   = ringbufferGetChar(txInterpreter_rb);
  op41          += hex2u8((char*) temp8Arr) << 24;
  
  //======
  
  // Set outputpower 0
  temp8Arr[0]   = ringbufferGetChar(txInterpreter_rb);
  temp8Arr[1]   = ringbufferGetChar(txInterpreter_rb);
  op0           = hex2u8((char*) temp8Arr);
  
  eeprom_setS2LP_OutputPower_8_5(op85);
  eeprom_setS2LP_OutputPower_4_1(op41);
  eeprom_setS2LP_OutputPower_0(op0);
    
  // Drop the CR
  ringbufferGetChar(txInterpreter_rb);
  
  TXM_FillCmdInField(TX_INTERPRETER_MESSAGE_869MHZ_SET_OUTPUTPOWER_ANS, answerField, &pos);
  TXM_AppendCRLFAndSend(answerField, pos);
}
