
/** @file leds.h
 *
 * @brief Header del archivo leds.c
 * @date 	Feb 22, 2018
 * @author Roux, Federico G. (froux@citedef.gob.ar)
 */

#ifndef LEDS_LEDS_H_
#define LEDS_LEDS_H_


#ifndef INC_LPC1102_IOCON_H_
	/* Header necesario : */
	#include <LPC1102_IOCON.h>

#endif



/* =========================================================================================
 * 		Definiciones de constantes :
 *  =========================================================================================
 */

/** @defgroup DEFINESLED Definiciones del hardware asociado a los leds.
 *	@brief	Asociacion de cada LED a pines físicos del MCU
 *	@{
 */

/** @brief Registro IOCON asociado al LED1 */
#define LED1_IOCON 					LPC_IOCON_B4
/** @brief Función asociada al registro IOCON */
#define LED1_IOCON_FUNC 			LPC_IOCON_B4_FUNC_PIO0_11

/** @brief Bit asociado al LED1 */
#define LED1_DIR_BIT				( 11UL )
/** @brief Dirección del bit LED1 */
#define LED1_DIR_MAS				( 0x01 << LED1_DIR_BIT )
/** @brief Registro de dirección */
#define LED1_DIR_REG				LPC_GPIO0->DIR
/** @brief Registro IOCON asociado al LED1 */
#define LED1_MAS_REG				LPC_GPIO0->MASKED_ACCESS

/**
 * @}
 */


/** @defgroup PRIORIDADTAREAS Declaracion de constantes de prioridades
 *	@brief	Prioridad asignada a cada tarea
 *	@{
 */

/* @brief Prioridad tarea delay */
#define PRIORIDAD_TAREA_DELAY			( tskIDLE_PRIORITY + 2 )
/* @brief Prioridad tarea parpadear */
#define PRIORIDAD_TAREA_PARPADEAR		( tskIDLE_PRIORITY + 1 )

/**
 * @}
 */

 /** @brief Values passed to the two tasks just to check the task parameter functionality. */
 #define mainQUEUE_SEND_PARAMETER			( 0x1111UL )
 #define mainQUEUE_RECEIVE_PARAMETER		( 0x22UL )

 /** @brief The rate at which data is sent to the queue. The 200ms value is converted to ticks using the portTICK_PERIOD_MS constant. */
 #define mainQUEUE_SEND_FREQUENCY_MS			( 5000 / portTICK_PERIOD_MS )


 /** @brief The number of items the queue can hold.  This is 1 as the receive task
 will remove items as they are added, meaning the send task should always find
 the queue empty. */
 #define mainQUEUE_LENGTH					( 1 )




/* =========================================================================================
 * 		Funciones externas:
 *  =========================================================================================
 */

extern QueueHandle_t xQueue;
extern volatile QueueHandle_t xQueueToClassify;

extern uint32_t Inicializar_Leds ( void );
extern uint32_t Parpadear_Led ( void );

#endif /* LEDS_LEDS_H_ */


