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

/** \brief Blinking Bare Metal driver led
 **
 **
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */

/** \addtogroup Examples CIAA Firmware Examples
 ** @{ */
/** \addtogroup Baremetal Bare Metal LED Driver
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

#ifndef CPU
#error CPU shall be defined
#endif
#if (lpc4337 == CPU)
#include "chip.h"
#elif (mk60fx512vlq15 == CPU)
#else
#endif
#include "teclado.h"
void Teclado_Init() {
	Chip_SCU_PinMux(1,0,MD_PUP|MD_EZI|MD_ZI,FUNC0); /* remapea P1_0 en GPIO 0[4],sW1 */
	Chip_SCU_PinMux(1,1,MD_PUP|MD_EZI|MD_ZI,FUNC0); /* remapea P1_1 en GPIO 0[8],SW2 */
	Chip_SCU_PinMux(1,2,MD_PUP|MD_EZI|MD_ZI,FUNC0); /* remapea P1_2 en GPIO 0[9],SW3 */
	Chip_SCU_PinMux(1,6,MD_PUP|MD_EZI|MD_ZI,FUNC0); /* remapea P1_6 en GPIO 1[9],SW4 */
	Chip_GPIO_SetDir(LPC_GPIO_PORT, 0,(1<<4)|(1<<8)|(1<<9),0);
	Chip_GPIO_SetDir(LPC_GPIO_PORT, 1,(1<<9),0);
}
void Tecla_Port(int tecla,int *port,int *pin) {
	switch(tecla)
	{
		case TECLA_1:
			*port = 0;
			*pin  = 4;
			break;
		case TECLA_2:
			*port = 0;
			*pin =  8;
			break;
		case TECLA_3:
			*port = 0;
			*pin =  9;
			break;
		case TECLA_4:
			*port = 1;
			*pin =  9;
			break;
		default:
			*port = 1;
			*pin =  9;
			break;
	}
}
int Tecla_C(int tecla,int rebote, int lib) {
	int out;
	int i;
	out = Tecla(tecla);
	if(out && rebote) {
		for(i=0;i<10000;i++) {
			asm("nop");
		}
		out = Tecla(tecla);
		if(out && lib) {
			while(!Tecla(tecla));
		}
	}
	return out;
}
int Tecla(int tecla) {
	int port,pin,out;
	Tecla_Port(tecla,&port,&pin);
	out = !Chip_GPIO_ReadPortBit(LPC_GPIO_PORT,port,pin);
	return out;
}
char kbhit() {
	char teclas;
	teclas = 0;
	if(Tecla(TECLA_1))
		teclas|= 1<< TECLA_1;
	if(Tecla(TECLA_2))
		teclas|= 1<< TECLA_2;
	if(Tecla(TECLA_3))
		teclas|= 1<< TECLA_3;
	if(Tecla(TECLA_4))
		teclas|= 1<< TECLA_4;
    return teclas;
}
int key() {
	int tecla;
	tecla = -1;
	if(Tecla(TECLA_1))
		tecla = TECLA_1;
	if(Tecla(TECLA_2))
		tecla = TECLA_2;
	if(Tecla(TECLA_3))
		tecla = TECLA_3;
	if(Tecla(TECLA_4))
		tecla = TECLA_4;
    return tecla;
}


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




/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

