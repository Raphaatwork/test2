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

#ifndef __TX_COMMANDEXECUTER_H
#define __TX_COMMANDEXECUTER_H

/* Includes */

/* Typedefinitions */

/* Variables */

/* Function definitions */

// Preconditions to call those functions:
// Complete message is already in ringbuffer
// Command known, valid and length of param valid

// Postconditions to call those functions:
// Complete message taken out of ringbuffer
// The interpreter has nothing to do anymore
void TXCE_MeasureBatteryLevel(ringbuffer *txInterpreter_rb);
void TXCE_GetSoftwareVersion(ringbuffer *txInterpreter_rb);
void TXCE_SetHardwareRevision(ringbuffer *txInterpreter_rb);
void TXCE_SetRFCarrier_TCXO(ringbuffer *txInterpreter_rb);
void TXCE_SetRFCarrier_XTAL(ringbuffer *txInterpreter_rb);
void TXCE_SetLED(ringbuffer *txInterpreter_rb);
void TXCE_ResetTest(ringbuffer *txInterpreter_rb);
void TXCE_SetUID(ringbuffer *txInterpreter_rb);
void TXCE_SetParam(ringbuffer *txInterpreter_rb);
void TXCE_GetParam(ringbuffer *txInterpreter_rb);
void TXCE_SetSerialNo(ringbuffer *txInterpreter_rb);
void TXCE_Pairing(ringbuffer *txInterpreter_rb);
void TXCE_SetCharStartBroadcast(ringbuffer *txInterpreter_rb);
void TXCE_SetS2LPSynth(ringbuffer *txInterpreter_rb);
void TXCE_SetS2LPOutputPower(ringbuffer *txInterpreter_rb);

#endif