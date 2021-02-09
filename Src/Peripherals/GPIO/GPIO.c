/**
  ******************************************************************************
  * @file       GPIO.c
  * @author     Tim Steinberg
  * @date       06.07.2020
  * @brief      Code to handle several special GPIO and their modes.
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
  * | 001       | 2020-07-06    | Tim Steinberg         | Initial version / skeleton of file            |
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
#include "Debug.h"
/* Typedefinitions / Prototypes */

/* Variables */

/* Function definitions */
// This functions arms the user button and does allow the button usage!
void gpio_userButtonArmedMode(void){
  TRACE_PROCEDURE_CALLS(1, "gpio_UserButtonArmedMode(void)\r\n");
  GPIO_InitTypeDef GPIO_InitStruct = {0};
    
  __HAL_RCC_GPIOA_CLK_ENABLE();
    
  GPIO_InitStruct.Pin = GPO_BUTTON_PULL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPO_BUTTON_PULL_GPIO_Port, &GPIO_InitStruct);
  
  // The voltage will change with the following line
  HAL_GPIO_WritePin(GPO_BUTTON_PULL_GPIO_Port, GPO_BUTTON_PULL_Pin, GPIO_PIN_SET);
}

// This functions is for the ADC measurement it allows for a slow rise of voltage
void gpio_userButtonMeasureMode(void){
  TRACE_PROCEDURE_CALLS(1, "gpio_UserButtonMeasureMode(void)\r\n");
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  __HAL_RCC_GPIOA_CLK_ENABLE();
  
  GPIO_InitStruct.Pin = GPO_BUTTON_PULL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPO_BUTTON_PULL_GPIO_Port, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPO_BUTTON_PULL_GPIO_Port, GPO_BUTTON_PULL_Pin, GPIO_PIN_SET);
}

// This function restricts the userbutton and does NOT allow for reset/button usage!
void gpio_userButtonUnarmedMode(void){
  TRACE_PROCEDURE_CALLS(1, "gpio_UserButtonUnarmedMode(void)\r\n");
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  __HAL_RCC_GPIOA_CLK_ENABLE();
  
  GPIO_InitStruct.Pin = GPO_BUTTON_PULL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN; //GPIO_PULLDOWN; //GPIO_NOPULL;
  HAL_GPIO_Init(GPO_BUTTON_PULL_GPIO_Port, &GPIO_InitStruct);
  
  HAL_GPIO_WritePin(GPO_BUTTON_PULL_GPIO_Port, GPO_BUTTON_PULL_Pin, GPIO_PIN_RESET);
}

// Switch GPIO LED in output mode
void gpio_ledOutputMode(void){
  TRACE_PROCEDURE_CALLS(1, "gpio_ledOutputMode(void)\r\n");
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  
  HAL_GPIO_WritePin(GPO_LED_RED_GPIO_Port, GPO_LED_RED_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPO_LED_GREEN_GPIO_Port, GPO_LED_GREEN_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPO_LED_BLUE_GPIO_Port, GPO_LED_BLUE_Pin, GPIO_PIN_SET);
  
  GPIO_InitStruct.Pin = GPO_LED_RED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPO_LED_RED_GPIO_Port, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = GPO_LED_GREEN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPO_LED_GREEN_GPIO_Port, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = GPO_LED_BLUE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPO_LED_BLUE_GPIO_Port, &GPIO_InitStruct);
}

// Switch GPIO LED in analog mode
void gpio_ledAnalogMode(void){
  TRACE_PROCEDURE_CALLS(1, "gpio_ledAnalogMode(void)\r\n");
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  __HAL_RCC_GPIOA_CLK_ENABLE();
  
  GPIO_InitStruct.Pin = GPO_LED_RED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPO_LED_RED_GPIO_Port, &GPIO_InitStruct);
  
  HAL_GPIO_WritePin(GPO_LED_RED_GPIO_Port, GPO_LED_RED_Pin, GPIO_PIN_SET);
  
  __HAL_RCC_GPIOB_CLK_ENABLE();
  
  GPIO_InitStruct.Pin = GPO_LED_GREEN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPO_LED_GREEN_GPIO_Port, &GPIO_InitStruct);
  
  HAL_GPIO_WritePin(GPO_LED_GREEN_GPIO_Port, GPO_LED_GREEN_Pin, GPIO_PIN_SET);
  
  GPIO_InitStruct.Pin = GPO_LED_BLUE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPO_LED_BLUE_GPIO_Port, &GPIO_InitStruct);
  
  HAL_GPIO_WritePin(GPO_LED_BLUE_GPIO_Port, GPO_LED_BLUE_Pin, GPIO_PIN_SET);
  
  __HAL_RCC_GPIOA_CLK_DISABLE();
  __HAL_RCC_GPIOB_CLK_DISABLE();
}

// Set Wakeup for RSL10 active
void gpio_rsl10WakeupActive(void){
  TRACE_PROCEDURE_CALLS(1, "gpio_rsl10WakeupActive(void)\r\n");
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  __HAL_RCC_GPIOB_CLK_ENABLE();
  
  GPIO_InitStruct.Pin = GPO_RSL_WAKE_UP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPO_RSL_WAKE_UP_GPIO_Port, &GPIO_InitStruct);
  
  HAL_GPIO_WritePin(GPO_RSL_WAKE_UP_GPIO_Port, GPO_RSL_WAKE_UP_Pin, GPIO_PIN_SET);
}

// Set Wakeup for RSL10 inactive
void gpio_rsl10WakeupInactive(void){
  TRACE_PROCEDURE_CALLS(1, "gpio_rsl10WakeupInactive(void)\r\n");
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  HAL_GPIO_WritePin(GPO_RSL_WAKE_UP_GPIO_Port, GPO_RSL_WAKE_UP_Pin, GPIO_PIN_RESET);
  
  __HAL_RCC_GPIOB_CLK_ENABLE();
  
  GPIO_InitStruct.Pin = GPO_RSL_WAKE_UP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPO_RSL_WAKE_UP_GPIO_Port, &GPIO_InitStruct);
}

// Set Reset for RSL10 active
void gpio_rsl10ResetActive(void){

  TRACE_PROCEDURE_CALLS(1, "gpio_rsl10ResetActive(void)\r\n");
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  __HAL_RCC_GPIOB_CLK_ENABLE();
  
  GPIO_InitStruct.Pin = GPO_RSL_RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPO_RSL_RESET_GPIO_Port, &GPIO_InitStruct);
  
  HAL_GPIO_WritePin(GPO_RSL_RESET_GPIO_Port, GPO_RSL_RESET_Pin, GPIO_PIN_RESET);
}

// Set Reset for RSL10 inactive
void gpio_rsl10ResetInactive(void){

  TRACE_PROCEDURE_CALLS(1, "gpio_rsl10ResetInactive(void)\r\n");
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  __HAL_RCC_GPIOB_CLK_ENABLE();
  
  HAL_GPIO_WritePin(GPO_RSL_RESET_GPIO_Port, GPO_RSL_RESET_Pin, GPIO_PIN_SET);
  
  GPIO_InitStruct.Pin = GPO_RSL_RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPO_RSL_RESET_GPIO_Port, &GPIO_InitStruct);
}

// PA11 INPUT

// PA12 OUTPUT

void gpio_initAllAnalog(void){
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pins : GPO_TCXO_EN_Pin GPO_BUTTON_PULL_Pin GPO_T20_GATE_Pin GPO_2_S2LP_Pin
                           GPO_1_S2LP_Pin GPO_S2LP_CS_Pin */
  GPIO_InitStruct.Pin = GPO_TCXO_EN_Pin|GPO_BUTTON_PULL_Pin|GPO_T20_GATE_Pin|GPO_2_S2LP_Pin
                          |GPO_1_S2LP_Pin|GPO_S2LP_CS_Pin|GPO_LED_RED_Pin|GPI_TEST_MODE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : GPO_S2LP_SHDWN_Pin */
  GPIO_InitStruct.Pin = GPO_S2LP_SHDWN_Pin|GPO_LED_GREEN_Pin|GPO_LED_BLUE_Pin|GPO_RSL_WAKE_UP_Pin|GPO_RSL_RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void gpio_initSleepMode(void){
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  /*
  // Not connected pins => Analog with no pull
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  */
  
  /*
  // Disconnect Debugger-Port
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  */

  // Not used pins => Analog with no pull
  GPIO_InitStruct.Pin = GPO_TCXO_EN_Pin|GPO_T20_GATE_Pin|
                          GPO_2_S2LP_Pin|GPO_1_S2LP_Pin|GPO_S2LP_CS_Pin|
                          USART1_TX_RSL10_Pin|USART1_RX_RSL10_Pin|
                          GPO_LED_RED_Pin|
                          GPI_TEST_MODE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPO_LED_GREEN_Pin|GPO_LED_BLUE_Pin|
                          SPI1_SCK_S2LP_Pin|SPI1_MISO_S2LP_Pin|SPI1_MOSI_S2LP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  /*
  // Shut down the S2LP
  GPIO_InitStruct.Pin = GPO_S2LP_SHDWN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPO_S2LP_SHDWN_GPIO_Port, &GPIO_InitStruct);
  
  // Set the wakeup pin for the RSL10   
  GPIO_InitStruct.Pin = GPO_RSL_WAKE_UP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPO_RSL_WAKE_UP_GPIO_Port, &GPIO_InitStruct);
  
  // Set the reset pin for the RSL10
  GPIO_InitStruct.Pin = GPO_RSL_RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPO_RSL_RESET_GPIO_Port, &GPIO_InitStruct);
  
  // Set the reset pin for STM32 to ARMED
  GPIO_InitStruct.Pin = GPO_BUTTON_PULL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPO_BUTTON_PULL_GPIO_Port, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPO_BUTTON_PULL_GPIO_Port, GPO_BUTTON_PULL_Pin, GPIO_PIN_SET);
  */
}

//////////////////////////////////////////////
// S2LP PINS
//////////////////////////////////////////////

void gpio_s2lp_signalGPO1Output_init(void){
  TRACE_PROCEDURE_CALLS(1, "gpio_s2lp_signal_init(void)\r\n");
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  __HAL_RCC_GPIOA_CLK_ENABLE();
  
  GPIO_InitStruct.Pin = GPO_1_S2LP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPO_1_S2LP_GPIO_Port, &GPIO_InitStruct);
}

void gpio_s2lp_signalGPO2Output_init(void){
  TRACE_PROCEDURE_CALLS(1, "gpio_s2lp_signal_init(void)\r\n");
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  __HAL_RCC_GPIOA_CLK_ENABLE();
  
  GPIO_InitStruct.Pin = GPO_2_S2LP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPO_2_S2LP_GPIO_Port, &GPIO_InitStruct);
}

void gpio_s2lp_signalGPO1Output_deInit(void){
  TRACE_PROCEDURE_CALLS(1, "gpio_s2lp_signal_deInit(void)\r\n");
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  __HAL_RCC_GPIOA_CLK_ENABLE();
  
  GPIO_InitStruct.Pin = GPO_1_S2LP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPO_1_S2LP_GPIO_Port, &GPIO_InitStruct);
}

void gpio_s2lp_signalGPO2Output_deInit(void){
  TRACE_PROCEDURE_CALLS(1, "gpio_s2lp_signal_deInit(void)\r\n");
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  __HAL_RCC_GPIOA_CLK_ENABLE();
  
  GPIO_InitStruct.Pin = GPO_2_S2LP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPO_2_S2LP_GPIO_Port, &GPIO_InitStruct);
}

void gpio_s2lp_pinShutdown_setToShutdown(void){
  TRACE_PROCEDURE_CALLS(1, "gpio_s2lp_signal_deInit(void)\r\n");
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  __HAL_RCC_GPIOB_CLK_ENABLE();
  
  GPIO_InitStruct.Pin = GPO_S2LP_SHDWN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPO_S2LP_SHDWN_GPIO_Port, &GPIO_InitStruct);
}

void gpio_s2lp_pinShutdown_setToRunmode(void){
  TRACE_PROCEDURE_CALLS(1, "gpio_s2lp_signal_deInit(void)\r\n");
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  __HAL_RCC_GPIOB_CLK_ENABLE();
  
  GPIO_InitStruct.Pin = GPO_S2LP_SHDWN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPO_S2LP_SHDWN_GPIO_Port, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPO_S2LP_SHDWN_GPIO_Port, GPO_S2LP_SHDWN_Pin, GPIO_PIN_RESET);
}

void gpio_s2lp_pinCS_init(void){
  TRACE_PROCEDURE_CALLS(1, "gpio_s2lp_pinCS_init(void)\r\n");
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  __HAL_RCC_GPIOA_CLK_ENABLE();
  
  GPIO_InitStruct.Pin = GPO_S2LP_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPO_S2LP_CS_GPIO_Port, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPO_S2LP_CS_GPIO_Port, GPO_S2LP_CS_Pin, GPIO_PIN_SET);
}

void gpio_s2lp_pinCS_deInit(void){
  TRACE_PROCEDURE_CALLS(1, "gpio_s2lp_pinCS_deInit(void)\r\n");
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  __HAL_RCC_GPIOA_CLK_ENABLE();
  
  GPIO_InitStruct.Pin = GPO_S2LP_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPO_S2LP_CS_GPIO_Port, &GPIO_InitStruct);
}

void gpio_s2lp_pinCS_setToTransmission(void){
  HAL_GPIO_WritePin(GPO_S2LP_CS_GPIO_Port, GPO_S2LP_CS_Pin, GPIO_PIN_RESET);
}

void gpio_s2lp_pinCS_setToSilence(void){
  HAL_GPIO_WritePin(GPO_S2LP_CS_GPIO_Port, GPO_S2LP_CS_Pin, GPIO_PIN_SET);  
}

//////////////////////////////////////////////
// TCXO PIN
//////////////////////////////////////////////

void gpio_tcxo_on(void){
  TRACE_PROCEDURE_CALLS(1, "gpio_tcxo_on(void)\r\n");
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  __HAL_RCC_GPIOA_CLK_ENABLE();
  
  GPIO_InitStruct.Pin = GPO_TCXO_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPO_TCXO_EN_GPIO_Port, &GPIO_InitStruct);
 
  HAL_GPIO_WritePin(GPO_TCXO_EN_GPIO_Port, GPO_TCXO_EN_Pin, GPIO_PIN_SET);
}

void gpio_tcxo_off(void){
  TRACE_PROCEDURE_CALLS(1, "gpio_tcxo_off(void)\r\n");
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  __HAL_RCC_GPIOA_CLK_ENABLE();
  
  GPIO_InitStruct.Pin = GPO_TCXO_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPO_TCXO_EN_GPIO_Port, &GPIO_InitStruct);
}

//////////////////////////////////////////////
// XTAL PIN
//////////////////////////////////////////////

void gpio_xtal_on(void){
  TRACE_PROCEDURE_CALLS(1, "gpio_xtal_on(void)\r\n");
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  __HAL_RCC_GPIOA_CLK_ENABLE();
  
  GPIO_InitStruct.Pin = GPO_XTAL_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPO_XTAL_EN_GPIO_Port, &GPIO_InitStruct);
 
  HAL_GPIO_WritePin(GPO_XTAL_EN_GPIO_Port, GPO_XTAL_EN_Pin, GPIO_PIN_SET);
}

void gpio_xtal_off(void){
  TRACE_PROCEDURE_CALLS(1, "gpio_xtal_off(void)\r\n");
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  __HAL_RCC_GPIOA_CLK_ENABLE();
  
  GPIO_InitStruct.Pin = GPO_XTAL_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPO_XTAL_EN_GPIO_Port, &GPIO_InitStruct);
}

//////////////////////////////////////////////
// TEST PIN
//////////////////////////////////////////////

void gpio_activateTestPin(void){
  TRACE_PROCEDURE_CALLS(1, "gpio_activateTestPin(void)\r\n");
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  __HAL_RCC_GPIOA_CLK_ENABLE();
  
  GPIO_InitStruct.Pin = GPI_TEST_MODE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPI_TEST_MODE_GPIO_Port, &GPIO_InitStruct);
}

void gpio_deactivateTestPin(void){
  TRACE_PROCEDURE_CALLS(1, "gpio_deactivateTestPin(void)\r\n");
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  __HAL_RCC_GPIOA_CLK_ENABLE();
  
  GPIO_InitStruct.Pin = GPI_TEST_MODE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPI_TEST_MODE_GPIO_Port, &GPIO_InitStruct);
}

void gpio_PortAInput(void){
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  __HAL_RCC_GPIOA_CLK_ENABLE();
  uint16_t pinMask = ~(GPO_BUTTON_PULL_Pin|GPIO_PIN_13|GPIO_PIN_14);
  GPIO_InitStruct.Pin = (GPIO_PIN_All & pinMask);
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void gpio_PortBInput(void){
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  __HAL_RCC_GPIOB_CLK_ENABLE();
  
  GPIO_InitStruct.Pin = GPIO_PIN_All;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void gpio_PortCInput(void){
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  __HAL_RCC_GPIOC_CLK_ENABLE();
  
  GPIO_InitStruct.Pin = GPIO_PIN_All;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void gpio_PortDInput(void){
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  __HAL_RCC_GPIOD_CLK_ENABLE();
  
  GPIO_InitStruct.Pin = GPIO_PIN_All;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

void gpio_PortHInput(void){
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  __HAL_RCC_GPIOH_CLK_ENABLE();
  
  GPIO_InitStruct.Pin = GPIO_PIN_All;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
}