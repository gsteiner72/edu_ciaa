/* Copyright 2015, Eduardo Filomena, Juan Manuel Reta
 * All rights reserved.
 *
 * This file is part of CIAA Firmware.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/** \brief Blinking Bare Metal example source file
 **
 ** This is a mini example of the CIAA Firmware.
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */

/** \addtogroup Examples CIAA Firmware Examples
 ** @{ */
/** \addtogroup Baremetal Bare Metal example source file
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 *
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * yyyymmdd v0.0.1 initials initial version
 */

/*==================[inclusions]=============================================*/
#include "main.h"       /* <= own header */
#include "led.h"
#include "teclado.h"
#include "timer.h"
#include "adc.h"
#include "dac.h"
#include "uart.h"

#ifndef CPU
#error CPU shall be defined
#endif
#if (lpc4337 == CPU)
#include "chip.h"
#elif (mk60fx512vlq15 == CPU)
#else
#endif


/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
/** \brief Main function
 *
 * This is the main entry point of the software.
 *
 * \returns 0
 *
 * \remarks This function never returns. Return value is only to avoid compiler
 *          warnings or errors.
 */
unsigned char cad1[]="Aumento la ganancia\r\n";
unsigned char cad2[]="Disminuyo la ganancia\r\n";
unsigned char cad3[]="MUTE\r\n";
unsigned char cad4[]="Valor ADC:0000\r\n";


int factor;
int valuetosend;
int main(void)
{
   /* perform the needed initialization here */
	GPIO_Init();
	Led_Color_Init();
	Teclado_Init();
	ADC_Init();
	ADC_Interrup();
	DAC_Init();
	Timer_Init();
	Timer_Set(100);
	UART_Init();

	int tecla,old_tecla;
	old_tecla=0;
	int value;
	while(1) {
		tecla = key();
		if(tecla!=old_tecla) {
			switch(tecla) {
				case TECLA_1:
					factor++;
					if(factor>20) factor=20;
					EnviarCadena(cad1);
					break;
				case TECLA_2:
					factor--;
					if(factor<1) factor=1;
					EnviarCadena(cad2);
					break;
				case TECLA_3:
					factor=0;
					EnviarCadena(cad3);
					break;
				case TECLA_4:
					EnviarADC();
					break;

			}
			old_tecla=tecla;
		}
	    value = valuetosend * factor;
	    value /= 10;
	    if(value>1023) value = 1023;
		DAC_Value(value);


    }
   return 0;
}
void EnviarCadena(unsigned char *cadena) {
	int caracter=0;
	while(cadena[caracter]!=0) {
		UART_Send(cadena[caracter++]);
	}
}
void EnviarADC() {
	int i,valor;
	valor = valuetosend;
	for(i=0;i<4;i++) {
		if(valor==0) {
			cad4[13-i] = '0';
		} else {
			cad4[13-i] = '0' + valor % 10;
			valor = valor /10;
		}
	}
	EnviarCadena(cad4);
}

void Timer_IRQ(void) {
	ADC_Start();
	Led_Color_Toggle(LED_1);
	Timer_Clear_IRQ();
}

void ADC0_IRQ(void) {
	int value;
    value = ADC_GetValue();
    valuetosend = value;
}



/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
