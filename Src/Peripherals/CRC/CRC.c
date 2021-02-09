/**
  ******************************************************************************
  * @file       CRC.c
  * @author     Tim Steinberg
  * @date       08.07.2020
  * @brief      NOT USED YET
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
  * | 001       | 2020-07-08    | Tim Steinberg         | Initial version / skeleton of file            |
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
#include "MasterDefine.h"
#include "Debug.h"
#include "ErrorHandling.h"

/* Typedefinitions / Prototypes */

/* Variables */
extern CRC_HandleTypeDef hcrc;

/* Function definitions */
void crc_deInit(void){
  HAL_CRC_DeInit(&hcrc);
  __HAL_RCC_CRC_CLK_DISABLE();
}

void crc_init_MPEG2(void)
{
  hcrc.Instance = CRC;
  hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_DISABLE;
  hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;
  hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
  hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
  hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_WORDS;
  hcrc.Init.CRCLength = CRC_POLYLENGTH_32B;
  hcrc.Init.GeneratingPolynomial = 0x04C11DB7;
  hcrc.Init.InitValue = 0xFFFFFFFF;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler_TxV2(INIT_CRC_FAILED);
  }
}

uint32_t crc_calcCrc32_MPEG2(uint32_t *ptrToData, uint32_t length){
  crc_init_MPEG2();
  hcrc.Instance->DR = 0x00000000;
  uint32_t crc = HAL_CRC_Calculate(&hcrc, ptrToData, length);
  crc_deInit();
  return crc;
}

uint16_t crc_calcCrc16_868MHzProtocol_softwareCrc(uint8_t *ptrToData, uint32_t length){
  uint32_t wLocalMask;
  uint16_t wLocalCrc16 = 0x0000;
  
  for (int i = 0; i < length; i++){
    wLocalMask = ptrToData[i] << 8;
    for (int j = 0; j < 8; j++){
      if((wLocalCrc16 ^ wLocalMask) & 0x8000)
      {
        wLocalCrc16 = (wLocalCrc16 << 1) ^ 0x1021;
      }
      else
      {
        wLocalCrc16 <<= 1;
      }
      wLocalMask <<= 1;
    }
  }
  
  return wLocalCrc16;
}

void crc_init_868MHzProtocol(void)
{
  hcrc.Instance = CRC;
  hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_DISABLE;
  hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_DISABLE;
  hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
  hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
  hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
  hcrc.Init.CRCLength = CRC_POLYLENGTH_16B;
  hcrc.Init.GeneratingPolynomial = 0x1021;
  hcrc.Init.InitValue = 0x0000;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler_TxV2(INIT_CRC_FAILED);
  }
}

uint16_t crc_calcCrc16_868MHzProtocol_hardwareCrc(uint8_t *ptrToData, uint32_t length){
  crc_init_868MHzProtocol();
  hcrc.Instance->DR = 0x00000000;
  uint32_t crc = HAL_CRC_Calculate(&hcrc, (uint32_t*)ptrToData, length);
  crc_deInit();
  return crc;
}

void crc_test(void){
  uint32_t testArray[] = {0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210};
  
  uint32_t crc = crc_calcCrc32_MPEG2((uint32_t*) testArray, 4);
  return;
}