/**
  ******************************************************************************
  * @file       S2LP.c
  * @author     Tim Steinberg
  * @date       10.12.2020
  * @brief      Code to control the S2LP in this product.
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
  * | 001       | 2020-12-10    | Tim Steinberg         | Initial version / skeleton of file            |
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
#include "stm32l0xx_hal.h"
#include "Masterdefine.h"
#include "GPIO.h"
#include "Watchdog.h"
#include "S2LP_Configuration.h"
#include "EEPROM_ApplicationMapped.h"
#include "Led.h"
#include "SPI.h"

/* Typedefinitions / Prototypes */
extern SPI_HandleTypeDef hspi1;

void S2LP_SpiWriteRegisters(uint8_t cRegAddress, uint8_t cNbBytes, uint8_t* pcBuffer);
void S2LP_SpiReadRegisters(uint8_t cRegAddress, uint8_t cNbBytes, uint8_t* pcBuffer);

/** @defgroup SPI_Headers
* @{
*/
#define HEADER_WRITE_MASK     0x00 /*!< Write mask for header byte*/
#define HEADER_READ_MASK      0x01 /*!< Read mask for header byte*/
#define HEADER_ADDRESS_MASK   0x00 /*!< Address mask for header byte*/
#define HEADER_COMMAND_MASK   0x80 /*!< Command mask for header byte*/

#define LINEAR_FIFO_ADDRESS 0xFF  /*!< Linear FIFO address*/

/** @defgroup SPI_Private_Macros
* @{
*/
#define BUILT_HEADER(add_comm, w_r) (add_comm | w_r)  /*!< macro to build the header byte*/
#define WRITE_HEADER    BUILT_HEADER(HEADER_ADDRESS_MASK, HEADER_WRITE_MASK) /*!< macro to build the write header byte*/
#define READ_HEADER     BUILT_HEADER(HEADER_ADDRESS_MASK, HEADER_READ_MASK)  /*!< macro to build the read header byte*/
#define COMMAND_HEADER  BUILT_HEADER(HEADER_COMMAND_MASK, HEADER_WRITE_MASK) /*!< macro to build the command header byte*/

/* Variables */
uint8_t S2LP_ErrorCode[2];

void S2LP_ZeroArray(uint8_t *array, uint32_t length){
  for (int i = 0; i < length; i++){
    array[i] = 0;
  }
}

/* Function definitions */
void S2LP_PowerUp(void)
{ 
  uint8_t addr;
  uint8_t noOfBytes;
  uint8_t *bytearr;
  uint8_t eepromArr[12];
  uint8_t readArr[255];
  
  switch(eeprom_getS2LP_ClocksourceSelector()){
    case CLOCKTYPE_SELECTOR_TCXO:
      gpio_tcxo_on(); // TCXO enable!
      break;
      
    case CLOCKTYPE_SELECTOR_XTAL:
      gpio_xtal_on(); // XTAL enable!
      break;
      
    default:
      gpio_ledOutputMode();
      led_red();
      break;
  }
  
  HAL_Delay(10);
  gpio_s2lp_pinCS_init();
  gpio_s2lp_pinShutdown_setToRunmode();
  HAL_Delay(1);
  
  bytearr = S2LP_getConfigBlock_A(&addr, &noOfBytes);
  S2LP_SpiWriteRegisters(addr, noOfBytes, bytearr);
  
  S2LP_ZeroArray(readArr, 255);
  S2LP_SpiReadRegisters(addr, noOfBytes, readArr);
  
  // Block B is residing in eeprom -> read and build array from there
  eepromArr[0] = eeprom_getS2LP_Synth3();
  eepromArr[1] = eeprom_getS2LP_Synth2();
  eepromArr[2] = eeprom_getS2LP_Synth1();
  eepromArr[3] = eeprom_getS2LP_Synth0();
  noOfBytes = 4;
  addr = 0x05;
  S2LP_SpiWriteRegisters(addr, noOfBytes, eepromArr);
  
  S2LP_ZeroArray(readArr, 255);
  S2LP_SpiReadRegisters(addr, noOfBytes, readArr);
  
  bytearr = S2LP_getConfigBlock_C(&addr, &noOfBytes);
  S2LP_SpiWriteRegisters(addr, noOfBytes, bytearr);
  
  S2LP_ZeroArray(readArr, 255);
  S2LP_SpiReadRegisters(addr, noOfBytes, readArr);
  
  bytearr = S2LP_getConfigBlock_D(&addr, &noOfBytes);
  S2LP_SpiWriteRegisters(addr, noOfBytes, bytearr);
  
  S2LP_ZeroArray(readArr, 255);
  S2LP_SpiReadRegisters(addr, noOfBytes, readArr);
  
  // Block E is residing in eeprom -> read and build array from there
  eepromArr[0] = eeprom_getS2LP_OutputPower8();
  eepromArr[1] = eeprom_getS2LP_OutputPower7();
  eepromArr[2] = eeprom_getS2LP_OutputPower6();
  eepromArr[3] = eeprom_getS2LP_OutputPower5();
  eepromArr[4] = eeprom_getS2LP_OutputPower4();
  eepromArr[5] = eeprom_getS2LP_OutputPower3();
  eepromArr[6] = eeprom_getS2LP_OutputPower2();
  eepromArr[7] = eeprom_getS2LP_OutputPower1();
  eepromArr[8] = eeprom_getS2LP_OutputPower0();
  noOfBytes = 9;
  addr = 0x5A;
  S2LP_SpiWriteRegisters(addr, noOfBytes, eepromArr);
  
  S2LP_ZeroArray(readArr, 255);
  S2LP_SpiReadRegisters(addr, noOfBytes, readArr);
  
  gpio_s2lp_signalGPO2Output_init();
  
}

void S2LP_Shutdown(void)
{
  gpio_s2lp_pinShutdown_setToShutdown();
  gpio_s2lp_signalGPO2Output_deInit();
  gpio_s2lp_pinCS_deInit();
  switch(eeprom_getS2LP_ClocksourceSelector()){
    case CLOCKTYPE_SELECTOR_TCXO:
      gpio_tcxo_off(); // TCXO enable!
      break;
      
    case CLOCKTYPE_SELECTOR_XTAL:
      gpio_xtal_off(); // XTAL enable!
      break;
      
    default:
      gpio_ledOutputMode();
      led_red();
      break;
  }
}

void S2LP_CS_LOW(void)
{
  gpio_s2lp_pinCS_setToTransmission();  // SPI1 CS LOW !
}

void S2LP_CS_HIGH(void)
{
  gpio_s2lp_pinCS_setToSilence();    // SPI1 CS HIGH ! 
}

/**
* @brief  Write single or multiple registers.
* @param  cRegAddress: base register's address to be write
* @param  cNbBytes: number of registers and bytes to be write
* @param  pcBuffer: pointer to the buffer of values have to be written into registers
* @retval Device status
*/
void S2LP_SpiWriteRegisters(uint8_t cRegAddress, uint8_t cNbBytes, uint8_t* pcBuffer)
{
  HAL_StatusTypeDef hstd = HAL_OK;
  uint8_t tx_buff[2] = {WRITE_HEADER, cRegAddress};
  uint8_t rx_buff[255];
  
  //__disable_irq();     // SPI_ENTER_CRITICAL
  
  /* Puts the SPI chip select low to start the transaction */
  S2LP_CS_LOW();
  
  hstd |= HAL_SPI_TransmitReceive(&hspi1, tx_buff, rx_buff, 2, 1000);
  hstd |= HAL_SPI_TransmitReceive(&hspi1, pcBuffer, &rx_buff[2], cNbBytes, 1000);
  
  /* Puts the SPI chip select high to end the transaction */
  S2LP_CS_HIGH();
  
  //__enable_irq();      // SPI_EXIT_CRITICAL();
      
  S2LP_ErrorCode[0] = rx_buff[0];
  S2LP_ErrorCode[1] = rx_buff[1];
  
  return;
}

/**
* @brief  Read single or multiple registers.
* @param  cRegAddress: base register's address to be read
* @param  cNbBytes: number of registers and bytes to be read
* @param  pcBuffer: pointer to the buffer of registers' values read
* @retval Device status
*/
void S2LP_SpiReadRegisters(uint8_t cRegAddress, uint8_t cNbBytes, uint8_t* pcBuffer)
{
  uint8_t tx_buff[255] = {READ_HEADER, cRegAddress};
  uint8_t rx_buff[2];
  
  //__disable_irq();     // SPI_ENTER_CRITICAL
  S2LP_CS_LOW();
  HAL_SPI_TransmitReceive(&hspi1, tx_buff, rx_buff, 2, 1000);
  HAL_SPI_TransmitReceive(&hspi1, tx_buff, pcBuffer, cNbBytes, 1000);  
  S2LP_CS_HIGH();
  //__enable_irq();      // SPI_EXIT_CRITICAL();
    
  S2LP_ErrorCode[0] = rx_buff[0];
  S2LP_ErrorCode[1] = rx_buff[1];
  
  return;
}

/**
* @brief  Send a command
* @param  cCommandCode: command code to be sent
* @retval Device status
*/
void S2LP_SpiCommandStrobes(uint8_t cCommandCode)
{
  HAL_StatusTypeDef hstd = HAL_OK;
  uint8_t tx_buff[2] = {COMMAND_HEADER,cCommandCode};
  uint8_t rx_buff[2];
  
  //__disable_irq();     // SPI_ENTER_CRITICAL
  S2LP_CS_LOW();
  hstd |= HAL_SPI_TransmitReceive(&hspi1, tx_buff, rx_buff, 2, 1000);  
  S2LP_CS_HIGH();
  //__enable_irq();      // SPI_EXIT_CRITICAL();
  
  S2LP_ErrorCode[0] = rx_buff[0];
  S2LP_ErrorCode[1] = rx_buff[1];
  
  return;
}

/**
* @brief  Configure the registers of the S2LP to send a Continue CW in TX mode.
* @retval ---
*/
void S2LP_SetRegistersCWMode(void)
{
  uint8_t tmp[1];
  // MOD2
  tmp[0] = 0x77; // Unmodulated! => Wave Carrier in CONTINU until a SABORT command!    
  S2LP_SpiWriteRegisters(0x10, 1, tmp);
  // PCKTCTRL1 // Set bits 2 & 3  PN9 for CW continue; Set bits 5 & 6 => CRC Mode 3 (0x1021);
  tmp[0] = 0x6C;
  S2LP_SpiWriteRegisters(0x30, 1, tmp);
}

/**
* @brief  Configure the registers of the S2LP to remove the Continue CW in TX mode.
* @retval ---
*/
void S2LP_ResetRegistersCWMode(void)
{
#warning "THIS FUNCTION SEEMS TO BE PART OF THE S87 AS WE DONT HAVE 25KBPS!"
  uint8_t tmp[1];
  // MOD2
  // In normal mode, restore 25kbps baudrate
  tmp[0] = 0x07; // Modulated 2-FSK! No Continue Wave Carrier!
  S2LP_SpiWriteRegisters(0x10, 1, tmp);
  // PCKTCTRL1 // Set bits 2 & 3 Normal Mode (No CW cont.); Set bits 5 & 6 => CRC Mode 3 (0x1021)
  tmp[0] = 0x60; //
  S2LP_SpiWriteRegisters(0x30, 1, tmp);
}

/****************************************************************************************/
/* ====> Set setLowPower                                                                */
/*                                                                                      */
/*       Set S2LP low or high power                                                     */
/*                                                                                      */
/*       Input: lowPower: 0: Set high power                                             */
/*       gl_systemStatus.                                                               */
/*                                                                                      */
/****************************************************************************************/

void S2LP_setLowPower(uint8_t lowPower)
{
  uint8_t S2LPReg[2];

#warning "TELEALARM DOES LOCK THE SPI HERE. AS WE DON'T HAVE ANY PARALLEL PROCESSES, WE SHOULD NOT NEED THAT"
  if(lowPower)
  {
//    S2LPReg[0] = 0x45;                                                          // -20dBm
//    S2LPReg[0] = 0x31;                                                          // -10dBm
    S2LPReg[0] = 0x1D;                                                          // 0dBm
    S2LP_SpiWriteRegisters(0x5A, 1, S2LPReg);                                   
    S2LPReg[0] = 0x07;
    S2LP_SpiWriteRegisters(0x62, 1, S2LPReg);                                   // Clear "max power flag"
  }
  else
  {
    S2LPReg[0] = 0x01;
    S2LP_SpiWriteRegisters(0x5A, 1, S2LPReg);
    S2LPReg[0] = 0x27;
    S2LP_SpiWriteRegisters(0x62, 1, S2LPReg);                                   // Set max power 14dBm 
  }
#warning "TELEALARM DOES UNLOCK THE SPI HERE. AS WE DON'T HAVE ANY PARALLEL PROCESSES, WE SHOULD NOT NEED THAT"
}

/****************************************************************************************/
/* ====> Set S2LP Frequency                                                             */
/*                                                                                      */
/*                                                                                      */
/*       Input: Address of the 4 bytes table to use for SYNT3, SYNT2, SYNT1, SYNT0      */
/*              register setting                                                        */
/*                                                                                      */
/****************************************************************************************/

void S2LP_setFreq(uint8_t *f)
{
#warning "TELEALARM DOES LOCK THE SPI HERE. AS WE DON'T HAVE ANY PARALLEL PROCESSES, WE SHOULD NOT NEED THAT"
  S2LP_SpiWriteRegisters(0x05, 4, f);
#warning "TELEALARM DOES UNLOCK THE SPI HERE. AS WE DON'T HAVE ANY PARALLEL PROCESSES, WE SHOULD NOT NEED THAT"
}

uint8_t S2LP_isError()
{
  return(!(S2LP_ErrorCode[0] & 0x10) || (S2LP_ErrorCode[0] & 0x01) || !(S2LP_ErrorCode[1] & 0x01));
}

void S2LP_SendCommandStrobes(void){
  HAL_Delay(10);
  S2LP_SpiCommandStrobes(0x62);
  HAL_Delay(1);
  S2LP_SpiCommandStrobes(0x66);
  HAL_Delay(1);
  S2LP_SpiCommandStrobes(0x60);
  HAL_Delay(1);
}

void S2LP_SetConfig_CWActiveMode(void){  
  SPI_S2LP_Init();
  S2LP_PowerUp();
  S2LP_SetRegistersCWMode();
  S2LP_SendCommandStrobes();
}

void S2LP_SetConfig_CWInactiveMode(void){
  S2LP_ResetRegistersCWMode();
  S2LP_Shutdown();
  SPI_S2LP_DeInit();
}

void S2LP_SetConfig_WorkingMode(void){  
  SPI_S2LP_Init();
  S2LP_PowerUp();
  S2LP_SendCommandStrobes();
}

void S2LP_SetConfig_SleepMode(void){
  S2LP_Shutdown();
  SPI_S2LP_DeInit();
}
