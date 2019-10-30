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
#include "semphr.h"

#include <UART/uart.h>
//#include <UART/colas.h>

#include <LPC11xx.h>
#include <LPC1102_IOCON.h>
#include <hardware/hardware_aplicacion.h>
#include <comunicaciones/comunicaciones.h>
#include <leds/leds.h>
#include <protocolo/protocolo.h>


#define DEBUG_MODE

/* =========================================================================================
 * 		Prototipos de funciones :
 *  =========================================================================================
 */


uint32_t Inicializar_Leds ( void );
uint32_t Parpadear_Led ( void ) ;

/* =========================================================================================
 * 		Variables comunes del archivo
 *  =========================================================================================
 */

/**
 ** @var 	xQueue
 ** @brief 	The queue used by both tasks.
 **/

QueueHandle_t xQueue = NULL;


/**
 ** @var 	t_canal
 ** @brief 	Contiene el id de dispositivo (por el momento).
 **/
volatile t_terminal t0;																	// Estructura de datos asociada a la terminal

/* =========================================================================================
 * 		Implementación de funciones :
 *  =========================================================================================
 */

/**
 * @fn uint32_t Inicializar_T0 (void)
 *
 * 	@brief Inicialización de valores de id de dispositivo
 *
 * 	@param[in] 	nada
 * 	@param[out] 0	OK
 *
 */

uint32_t Inicializar_T0 ( void )
{
	//t0.ID=0;
	t0.ID=0x30;
	return 0;
}


/**
 * 	@fn void Tarea_Delay( void *pvParameters )
 *
 * 	@brief 	Tarea que inicia la cuenta del tick y envia mensaje
 * 			a través de la queue
 *
 * 	@param[in] 	nada
 * 	@param[out] 0	OK
 *
 */

void prvDelayTask( void *pvParameters )
{
TickType_t xNextWakeTime;

	t_paq_buf_cmp paq;


	/* Check the task parameter is as expected. */
//  	configASSERT( ( ( unsigned long ) pvParameters ) == mainQUEUE_SEND_PARAMETER );

	/* Initialise xNextWakeTime - this only needs to be done once. */
	xNextWakeTime = xTaskGetTickCount();

	for( ;; )
	{
		/* Place this task in the blocked state until it is time to run again.
		The block time is specified in ticks, the constant used converts ticks
		to ms.  While in the Blocked state this task will not consume any CPU
		time. */
		vTaskDelayUntil( &xNextWakeTime, mainQUEUE_SEND_FREQUENCY_MS );


		strcpy(paq.buffer, "abcdefg");

		xQueueSend(xQueueToSendReply, &paq, portMAX_DELAY);


		/* Send to the queue - causing the queue receive task to unblock and
		toggle the LED.  0 is used as the block time so the sending operation
		will not block - it shouldn't need to block as the queue should always
		be empty at this point in the code. */
		//xQueueSend( xQueue, &ulValueToSend, 0U );
		//UARTSend(".", 1);
		Parpadear_Led();
	}
}


/**
 * 	@fn 	void Tarea_Procesamiento_Serie( void *pvParameters )
 *
 * 	@brief 	Tarea que Verifica el Checksum del mensaje para ver si llego correctamente
 * 			 y si es un comando valido. Si es correcto lo pasa a ser clasificado
 *
 * 	@param[in] 	nada
 *
 *
 * 	@param[out] 0			OK
 *
 */

void prvProcessSerialMsjTask( void *pvParameters )
{
	//unsigned long ulReceivedValue;
	t_paq_buf_cmp msjReceived;

	for( ;; )
	{

		//EN ESTA TAREA SE RECIBEN LOS MENSAJES SERIE Y SE ANALIZA SI ES PARA EL Y EL CHECKSUM

		xQueueReceive(xQueueReceived, &msjReceived.buffer, portMAX_DELAY );
//#if defined(DEBUG_MODE)
//		char str[] = "[ProcessSerialMsj] Received: ";
//		UARTSend(str,strlen(str));
//		UARTSend(msjReceived.buffer, 8);
//		UARTSend("\n",1);
//#endif

		//1) Copio lo almacenado en la cola de datos al buffer

		//2) Chequeo que el ID me corresponda a esta terminal
		if(t0.ID != msjReceived.bytes_campos.DST){
			//return PAQ_CLASIFICAR_RECIBIDO_PAQ_ERROR_DST;
#if defined(DEBUG_MODE)
			char str[] = "[ProcessSerialMsj] Not for me, DST: ";
			UARTSend(str,strlen(str));
			UARTSend(&msjReceived.bytes_campos.DST, 1);
			UARTSend("\n",1);
#endif

		}
		else{

#if defined(DEBUG_MODE)
			char str[] = "[ProcessSerialMsj] Msj for me.\n";
			UARTSend(str,strlen(str));
#endif
			//3) Compruebo que el checksum este bien
			if(PAQ_Validar_Checksum(msjReceived) != PAQ_VALIDAR_CHECKSUM_VALIDO){
				//return PAQ_CLASIFICAR_RECIBIDO_PAQ_ERROR_CHK;
			}else{
//				//4) Compruebo que el comando esté OK
//				if(PAQ_Validar_Comando(msjReceived) != PAQ_VALIDAR_COMANDO_VALIDO){
//						//return PAQ_CLASIFICAR_RECIBIDO_PAQ_ERROR_CMD;
//				}else{
//					//5) Si esta bien, devuelvo lo paso a clasificacion
					//return PAQ_CLASIFICAR_RECIBIDO_OK;
					xQueueSendToBack(xQueueToClassify, &msjReceived.buffer, portMAX_DELAY);
//				}
			}
		}
		//RS485_HABILITAR_TX();
		Parpadear_Led();
	}
}


/**
 * 	@fn 	void Tarea_Clasificar_Mensaje( void *pvParameters )
 *
 * 	@brief 	Tarea que Clasifica los paquetes
 *
 * 	@param[in] 	nada
 *
 *
 * 	@param[out] 0			OK
 *
 */

void prvClassifyMsjTask( void *pvParameters )
{
	t_paq_buf_cmp msjReceived;
	int paq_clasificado = 0;
	t_paq_buf_cmp msjResponse;

	for( ;; )
	{

		xQueueReceive(xQueueToClassify, &msjReceived.buffer, portMAX_DELAY );

#if defined(DEBUG_MODE)
		char str[] = "[ClassifyMsj] Received: ";
		UARTSend(str,strlen(str));
		UARTSend(msjReceived.buffer,PQ_LARGO_BYTES);
		UARTSend("\n",1);
#endif

		paq_clasificado = PAQ_Clasificar_Recibido (msjReceived);

		switch(paq_clasificado)
		{
//			case PAQ_CLASIFICAR_RECIBIDO_PAQ_ERROR_DST :										// Lo que recibí no era para mí
//				return;

//			case PAQ_CLASIFICAR_RECIBIDO_PAQ_ERROR_CHK :										// Recibí con un error de checksum
//				msjResponse = PAQ_Armar_Respuesta_Estandar(msjReceived,CMD_ECK);
//				return;

//			case PAQ_CLASIFICAR_RECIBIDO_TIMEOUT :												// Recibí el frame incompleto (menos de 8 bytes)
//				msjResponse = PAQ_Armar_Respuesta_Estandar(msjReceived,CMD_ETO);
//				return;

//			case PAQ_CLASIFICAR_RECIBIDO_PAQ_NO_EXTRAIDO :										// Recibí algo pero no lo puedo ver por error interno
//				msjResponse = PAQ_Armar_Respuesta_Estandar(msjReceived,CMD_EIN);
//				return;

			case PAQ_CLASIFICAR_RECIBIDO_PAQ_ERROR_CMD :										// Recibí un comando no válido
				msjResponse = PAQ_Armar_Respuesta_Estandar(msjReceived,CMD_ENE);
				//xQueueSendToBack(xQueueToSend2, &msjResponse.buffer, portMAX_DELAY);
				break;

			case PAQ_CLASIFICAR_RECIBIDO_OK	:

				xQueueSendToBack(xQueueToProcess, &msjReceived.buffer, portMAX_DELAY);
				break;

			default :
//				PAQ_Error(1);
				return;
		}
//		for (int ix=0;ix<8;ix++)
//			msjResponse.buffer[ix]=0x30+ix;

		//xQueueSendToBack(xQueueToSend, &msjResponse.buffer, portMAX_DELAY);

	}
}

void prvProcessCommand ( void *pvParameters )
{
	t_paq_buf_cmp msjReceived;
	int paq_clasificado = 0;
	t_paq_buf_cmp msjResponse;
	t_est_control est_control;
	t_paq_dato dato_respuesta;


	for( ;; )
	{
		xQueueReceive(xQueueToProcess, &msjReceived.buffer, portMAX_DELAY );

#if defined(DEBUG_MODE)
		char str[] = "[ProcessCommand] Received: ";
		UARTSend(str,strlen(str));
		UARTSend(msjReceived.buffer,PQ_LARGO_BYTES);
		UARTSend("\n",1);
#endif

		paq_clasificado = PAQ_CLASIFICAR_RECIBIDO_PAQ_ERROR_ECX;							// Pongo por default en fuera de contexto

		/////// CHEQUEAR PRIMERO COMANDO DEL RESET GENERAL.
		/////// HACER RESET DEL MICRO
		if(msjReceived.bytes_campos.CMD == CMD_RESET)
		{
			// Resetear el sistema
		}
		switch (est_control)
		{
			case CONTROL_ESPERO_INICIALIZACION :													// Estado inicial de la máq.

				if(msjReceived.bytes_campos.CMD == CMD_INIC)
				{
					est_control = CONTROL_ESPERO_INICIALIZACION;									// paso al siguiente estado
					paq_clasificado = PAQ_CLASIFICAR_RECIBIDO_OK;
				}
				break;

			default :
				est_control = CONTROL_ESPERO_INICIALIZACION;
				break;
		}



		switch(paq_clasificado)
		{
			case PAQ_ENVIAR_DATO :
				dato_respuesta.bytes.b0= 0x00;
				dato_respuesta.bytes.b1= 0x00;
				dato_respuesta.bytes.b2= 0x00;
				dato_respuesta.bytes.b3= 0x00;
				dato_respuesta.qword= 0x00;
				dato_respuesta.words.w0= 0x00;
				dato_respuesta.words.w1= 0x00;
				msjResponse = PAQ_Armar_Respuesta_CMD_DAT(msjReceived,dato_respuesta);
				return;

			case PAQ_CLASIFICAR_RECIBIDO_OK	:
				msjResponse = PAQ_Armar_Respuesta_Estandar(msjReceived,CMD_OK);
				break;

			case PAQ_CLASIFICAR_RECIBIDO_PAQ_ERROR_ECX :
				msjResponse = PAQ_Armar_Respuesta_Estandar(msjReceived,CMD_ECX);
				break;

			default :
				//PAQ_Error(2);
			return;
		}

		xQueueSendToBack(xQueueToSendReply, &msjResponse.buffer, portMAX_DELAY);


	}
}

