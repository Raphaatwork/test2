/**
  ******************************************************************************
  * @file       S2LP_Configuration.c
  * @author     Tim Steinberg
  * @date       10.12.2020
  * @brief      Configuration for the S2LP
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

/* Typedefinitions / Prototypes */

/* Variables */

/* Function definitions */

//VALUE, // ADDRESS DESCRITPION

uint8_t S2LP_configuration_GPIO_MODE_BlockA[] = {
  //0x11, // 0x01 GPIO1Conf -> GPIO1_CONF Input Config. DIRECT MODULATION!!!
  0x11, // 0x02 GPIO2Conf -> GPIO2_CONF Input Config. DIRECT MODULATION!!!
};

uint8_t S2LP_configuration_GPIO_MODE_BlockB[] = {
  0x62, // 0x05 SYNT3 -> Frequency 869.2125Mhz
  0x2C, // 0x06 SYNT2
  0x4B, // 0x07 SYNT1
  0xCB, // 0x08 SYNT0 // Was previously 0xBE
};
 
uint8_t S2LP_configuration_GPIO_MODE_BlockC[] = {
  0xA3, // 0x0E MOD4
  0x6E, // 0x0F MOD3
  0x03, // 0x10 MOD2 -> MODE TYPE 2 FSK
  0x20, // 0x11 MOD1
  0xFC, // 0x12 MOD0
  0x85, // 0x13 CHFLT
};

uint8_t S2LP_configuration_GPIO_MODE_BlockD[] = {  
  0x08, // 0x30 PCKTCTRL1 -> Direct through GPIO
};

uint8_t S2LP_configuration_GPIO_MODE_BlockE[] = {  
  0x02, // 0x5A PA_Power8
  0x02, // 0x5B PA_Power7
  0x02, // 0x5C PA_Power6
  0x02, // 0x5D PA_Power5
  0x02, // 0x5E PA_Power4
  0x02, // 0x5F PA_Power3
  0x02, // 0x60 PA_Power2
  0x02, // 0x61 PA_Power1
  0x40, // 0x62 PA_Power0
};


uint8_t* S2LP_getConfigBlock_A(uint8_t *addr, uint8_t *noBytes){
  *addr         = 0x02;
  *noBytes      = 1;
  return        S2LP_configuration_GPIO_MODE_BlockA;
}

uint8_t* S2LP_getConfigBlock_B(uint8_t *addr, uint8_t *noBytes){
  *addr         = 0x05;
  *noBytes      = 4;
  return        S2LP_configuration_GPIO_MODE_BlockB;
}

uint8_t* S2LP_getConfigBlock_C(uint8_t *addr, uint8_t *noBytes){
  *addr         = 0x0E;
  *noBytes      = 6;
  return        S2LP_configuration_GPIO_MODE_BlockC;
}

uint8_t* S2LP_getConfigBlock_D(uint8_t *addr, uint8_t *noBytes){
  *addr         = 0x30;
  *noBytes      = 1;
  return        S2LP_configuration_GPIO_MODE_BlockD;
}

uint8_t* S2LP_getConfigBlock_E(uint8_t *addr, uint8_t *noBytes){
  *addr         = 0x5A;
  *noBytes      = 9;
  return        S2LP_configuration_GPIO_MODE_BlockE;
}

