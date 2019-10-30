
/** @file leds.h
 *
 * @brief Header del archivo leds.c
 * @date 	Feb 22, 2018
 * @author Roux, Federico G. (froux@citedef.gob.ar)
 */

#ifndef COMS_COMS_H_
#define COMS_COMS_H_


#ifndef INC_LPC1102_IOCON_H_
	/* Header necesario : */
	#include <LPC1102_IOCON.h>

#endif



/* =========================================================================================
 * 		Definiciones de constantes :
 *  =========================================================================================
 */


/** @defgroup PRIORIDADTAREAS Declaracion de constantes de prioridades
 *	@brief	Prioridad asignada a cada tarea
 *	@{
 */

/* @brief Prioridad tarea prvDelayTask */
#define PRIORIDAD_TAREA_prvDelayTask				( tskIDLE_PRIORITY + 2 )
/* @brief Prioridad tarea prvProcessSerialMsjTask */
#define PRIORIDAD_TAREA_prvProcessSerialMsjTask		( tskIDLE_PRIORITY + 1 )
/* @brief Prioridad tarea prvClassifyMsjTask */
#define PRIORIDAD_TAREA_prvClassifyMsjTask			( tskIDLE_PRIORITY + 1 )
/* @brief Prioridad tarea prvProcessCommand */
#define PRIORIDAD_TAREA_prvProcessCommand			( tskIDLE_PRIORITY + 1 )
/* @brief Prioridad tarea prvSendSerial */
#define PRIORIDAD_TAREA_prvSendSerial				( tskIDLE_PRIORITY + 1 )




/**
 * @}
 */

 /** @brief Values passed to the two tasks just to check the task parameter functionality. */
 #define mainQUEUE_SEND_PARAMETER			( 0x1111UL )
 #define mainQUEUE_RECEIVE_PARAMETER		( 0x22UL )

 /** @brief The rate at which data is sent to the queue. The 200ms value is converted to ticks using the portTICK_PERIOD_MS constant. */
 #define mainQUEUE_SEND_FREQUENCY_MS			( 1000 / portTICK_PERIOD_MS )


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

extern void prvDelayTask( void *pvParameters );

extern void prvProcessSerialMsjTask( void *pvParameters );

extern void prvClassifyMsjTask( void *pvParameters );

extern void prvProcessCommand( void *pvParameters );

extern void prvSendSerial( void *pvParameters );

extern uint32_t Inicializar_T0 ( void );

#endif /* COMS_COMS_H_ */



/************************************************************************************
 * 							DEFINICION DE TIPOS MAQ ESTADOS|						*
 ************************************************************************************/

typedef enum {CONTROL_ESPERO_INICIALIZACION} t_est_control;
