/*
 * LPC1102_IOCON.c
 *
 *  Created on: 22 de ago. de 2017
 *      Author: piro8
 */

#include <stdint.h>

#include <LPC11xx.h>
#include <core_cm0.h>


#include <LPC1102_IOCON.h>

/** @file LPC1102_IOCON.c
 **
 ** @brief Archivo de funciones del registro IOCON
 ** @brief Manejo de campos y funciones del registro IOCON\n
 ** @brief UM10429 Capitulo 7: LPC1102/04 I/O Configuration
 ** @author Roux, Federico G. (froux@favaloro.edu.ar)
 **/

uint8_t Definir_Pin_Configuracion_IO(t_pin pin, uint32_t funcion, uint32_t modo, uint32_t histeresis, uint32_t opendrain);


/** @function Definir_Pin_Configuracion_IO()
 *
 * 	@brief Definición de la configuración de un pin
 *
 * 	@param[in] pin 			Listado de pines disponibles del microcontrolador. Ver : @link LPC_IOCON_LPC1102_PINES @endlink
 * 	@param[in] modo 		Modo de resistencia: inactivo, pull-up, pull-down, repetidor. Ver:
 * 							@link LPC_IOCON_LPC1102_MODE @endlink
 * 	@param[in] histeresis	Bit de activacion de histeresis. Ver : @link LPC_IOCON_LPC1102_HYS_OD @endlink
 *	@param[in] opendrain 	Bit de activacion de salida opendrain. Ver : @link LPC_IOCON_LPC1102_HYS_OD @endlink
 *
 */

uint8_t Definir_Pin_Configuracion_IO(t_pin pin, uint32_t funcion, uint32_t modo, uint32_t histeresis, uint32_t opendrain)
{
	LPC_IOCON_B4 ^= ~LPC_IOCON_FUNC_MAS;
	LPC_IOCON_B4 |= funcion;

	LPC_IOCON_B4 ^= ~LPC_IOCON_MODE_MAS;
	LPC_IOCON_B4 |= modo;

	LPC_IOCON_B4 ^= ~LPC_IOCON_HYS_MAS;
	LPC_IOCON_B4 |= histeresis;

	LPC_IOCON_B4 ^= ~LPC_IOCON_OD_MAS;
	LPC_IOCON_B4 |= opendrain;

	switch(pin)
	{
	case C1:
		break;
	case A2:
		break;
	case A3:
		break;
	case  A4:
		break;
	case  B4:

		break;
	case  B3:
		break;
	case  C4:
		break;
	case  C3:
		break;
	case  D4:
		break;
	case  C2:
		break;
	case  D1:
		break;
	default:
		return DEFINIR_PIN_CONFIGURACION_IO_ERROR_PIN;

	}




	return 0;
}

