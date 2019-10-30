/** @file 	leds.c
 **
 ** @brief 	Funciones de manejo de leds
 ** 		específicas para la aplicacion
 ** @date 	Feb 22, 2018
 ** @author Roux, Federico G. (froux@favaloro.edu.ar)
 **/

/* =========================================================================================
 * 		Inclusion de Archivos :
 *  =========================================================================================
 */

#include <stdint.h>

#include "string.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <LPC11xx.h>
#include <LPC1102_IOCON.h>
#include <hardware/hardware_aplicacion.h>
#include <leds/leds.h>
#include <protocolo/protocolo.h>

/* =========================================================================================
 * 		Prototipos de funciones :
 *  =========================================================================================
 */

void Tarea_Delay( void *pvParameters );
void Tarea_Parpadear( void *pvParameters );

uint32_t Inicializar_Leds ( void );
uint32_t Parpadear_Led ( void ) ;


/* =========================================================================================
 * 		Implementación de funciones :
 *  =========================================================================================
 */

/**
 * @fn uint32_t Inicializar_Leds (void)
 *
 * 	@brief Inicialización de pines destinados a leds
 *
 * 	@param[in] 	nada
 * 	@param[out] 0	OK
 *
 */

uint32_t Inicializar_Leds ( void )
{


	GPIOSetDir( 0, 7, 1); // led salida.
	GPIOSetValue(0, 7, 0); //apagado
	return 0;
}


/**
 *  @fn uint32_t Parpadear_Led (void)
 *
 * 	@brief Cambio el estado del pin seteado como led
 *
 * 	@param[in] 	nada
 * 	@param[out] 0			OK
 *
 */

uint32_t Parpadear_Led ( void )
{

	static unsigned long ulLEDState = 0UL;

	if( ulLEDState == 0UL )
	{
		GPIOSetValue(0, 7, 0); //apagado
	}
	else
	{
		GPIOSetValue(0, 7, 1); //encendido
	}

	ulLEDState = !ulLEDState;
	return 0;
}



