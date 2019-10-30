/** @file uart.c
 *
 * @brief UART API file for NXP LPC11xx Family Microprocessors\n
 * 	   Archivo modificado para trabajar como driver dentro del\n
 * 	   protocolo de comunicacion PCP1\n\n
 * 	   <b>History</b>
 *		@arg 2008.08.21  ver 1.00 - Preliminary version, first Release
 *		@arg 2014-2015 : modificacions para SATFIG
 *		@arg 02/2018 : documentacion
 *
 *
 *	@author Copyright(C) 2008, NXP Semiconductor
 * 	@author Modificado por Roux, Federico G. (froux@citedef.gob.ar)
 */


// Code Red - updated for CMSIS 1.30

#include <protocolo/protocolo.h>
#include <stdbool.h>
#include "LPC11xx.h"
#include "gpio/gpio.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

#ifndef _USAR_COLAS_INLINE
	#define _USAR_COLAS_INLINE
	//#include "UART/colas.h"
#endif


#include "uart.h"

// #include <maq_estados\maq_estados.h>


#define DEBUG_MODE

/* ===============================================================================
 * 								PROTOTIPO DE FUNCIONES
 * ===============================================================================*/
void UARTInit(uint32_t baudrate);

int Inic_UART_RS485 (void);
int UART_RS485_Habilitar_Recepcion (void);
int UART_RS485_Deshabilitar_Recepcion (void);
void UART_Reset (int baudrate);
// void UARTSend(uint8_t *BufferPtr, uint32_t Length);

/* ===============================================================================
 * 								VARIABLES GLOBALES
 * ===============================================================================*/

/** @brief variable temporal para sacar algunos datos de la fifo */
volatile uint32_t UARTStatus;
/** @brief flag de señalizacion de buffer de tx libre */
volatile bool  tx_libre = true;
/** @brief buffer de software la UART */
volatile uint8_t  UARTBuffer[BUFSIZE];
/** @brief indice del contador del buffer */
volatile uint32_t UARTCount = 0;
// volatile uint32_t contador_timeout = 0;

// volatile uint32_t contador_vueltas_timeout = 0;

/** @brief Cola de recepcion de la UART */
//t_cola cola_rx;
t_paq_buf_cmp paq_rx_data;
/** @brief Cola de transmision de la UART */
//t_cola cola_tx;
t_paq_buf_cmp paq_rx_data;

/** @brief flag para indicar que ocurrió un timeout para procesar afuera */
bool timeout_pendiente;

/** @brief 	Handler de la queue utilizada para la recepcion de mensajes */
volatile QueueHandle_t xQueueReceived = NULL;
volatile QueueHandle_t xQueueToClassify = NULL;
volatile QueueHandle_t xQueueToSend = NULL;
volatile QueueHandle_t xQueueToProcess = NULL;
volatile QueueHandle_t xQueueToSendReply = NULL;

volatile SemaphoreHandle_t semUartTx;




/* ===============================================================================
 * 							IMPLEMENTACION DE FUNCIONES
 * ===============================================================================*/

/** ===============================================================================
 * @fn					void UART_IRQHandler(void)
 * @brief				UART interrupt handler
 * @param				None
 * @return 				None
 *  ===============================================================================
 */

void UART_IRQHandler(void)
{


  BaseType_t xHigherPriorityTaskWoken;

  /* No tasks have yet been unblocked. */

  xHigherPriorityTaskWoken = pdFALSE;

  uint8_t IIRValue, LSRValue;
  static uint8_t Dummy = 0;
  uint16_t cuenta_soltar_RTS = 0x0000;

  IIRValue = LPC_UART->IIR;															// Guardo el valor del IIR (Interrupt ID Register): IRQs pendientes de la uart

  IIRValue >>= 1;																	// Descarto el bit 0 de IntStatus. Se supone que está en cero al entrar al handler
  IIRValue &= 0x07;																	// Chequeo los bits de IntId para identificar la causa de la IRQ.

  switch(IIRValue)
  {

  case IIR_RLS: 																	// Receive Line Status: Mayor prioridad, cualquiera de las 4 condiciones de error

	LSRValue = LPC_UART->LSR;														// Guardo el valor del Line Status Register
    if (LSRValue & (LSR_OE | LSR_PE | LSR_FE | LSR_RXFE | LSR_BI))  				// Evalúo si se dio el caso de alguno de los 4 errores o Break Interrupt
    {																				// Se dio el caso de alguno de los 4 errores o BREAK INTERRUPT (BI)
      UARTStatus = LSRValue;														// Leo el LSR para limpiar la IRQ.
      Dummy = LPC_UART->RBR;														// Leo el Recieve Buffer Register (RBR) para limpiar la interrupcion
      return;
    }
    if (LSRValue & LSR_RDR)															// Receive Data Ready : la FIFO tiene datos válidos a ser leídos. No hubo error
    {
      UARTBuffer[UARTCount++] = LPC_UART->RBR;										// Leo la FIFO y lo guardo en un buffer propio
      if (UARTCount == BUFSIZE)														// Pregunto si llegué al final de mi buffer
      {
        UARTCount = 0;																// Buffer completo. Lo re inicializo en cero
      }
    }
    break;

  case IIR_RDA:																		// RDA: Receive Data Available. Se superó el umbral de bytes recibidos

	LSRValue = LPC_UART->LSR;														// Leo el LSR previo a leer el buffer RBR
	paq_rx_data.buffer[0]=LPC_UART->RBR;
	paq_rx_data.buffer[1]=LPC_UART->RBR;
	paq_rx_data.buffer[2]=LPC_UART->RBR;
	paq_rx_data.buffer[3]=LPC_UART->RBR;
	paq_rx_data.buffer[4]=LPC_UART->RBR;
	paq_rx_data.buffer[5]=LPC_UART->RBR;
	paq_rx_data.buffer[6]=LPC_UART->RBR;
	paq_rx_data.buffer[7]=LPC_UART->RBR;
	xQueueSendToBackFromISR(xQueueReceived, &paq_rx_data, &xHigherPriorityTaskWoken);						// Envio los datos recibidos a la xQueueReceived de datos Recibidos

	// est_enviar_datos = ESPERAR_TX_RESPUESTA;										// Seteo la máquina de estados para enviar respuesta
    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );

	break;

  case IIR_CTI:																		// CTI : Character Time-out indicator. Un byte permanece por mucho tiempo en la FIFO

	LSRValue = LPC_UART->LSR;														// Leo el LSR previo a leer el buffer RBR
	Dummy = LPC_UART->RBR;
	Dummy = LPC_UART->RBR;
	Dummy = LPC_UART->RBR;
	Dummy = LPC_UART->RBR;
	Dummy = LPC_UART->RBR;
	Dummy = LPC_UART->RBR;
	Dummy = LPC_UART->RBR;

	/********************************************/
	//Escribir_Cola_Inline(&cola_rx, LPC_UART->RBR);									// Agrego dato a la cola de recepción. Función inline
	//UART_Reset(9600);			// todo : verificar esta linea

	/********************************************/
	// uint8_t trashcan_uint8_t;
	// for (int ix=0;ix<8;ix++) trashcan_uint8_t=LPC_UART->RBR;
	//cola_rx.estado_buffer = BUFFER_TIMEOUT;											// Señalizo el timeout del buffer
	//cola_rx.datos_nuevos = 1;														// seteo el flag de datos nuevos.
	timeout_pendiente = true;
	break;

  case IIR_THRE:																	// THRE : transmit holding register empty

	LSRValue = LPC_UART->LSR;														// Chequeo el LSR para ver si el Buffer de Tx tiene datos válidos o no

	/* AGREGAR DELAY PARA SOLTAR EL RTS : */


	if (LSRValue & LSR_THRE)														// Chequeo el bit de Tx Holding Register Empty U0THR = 0 -> Datos validos
	{																	// U0THR está vacío
		/*
		for ( cuenta_soltar_RTS = 0x0000; cuenta_soltar_RTS < 0xFFFF; cuenta_soltar_RTS++); // todo : emprolijar esta linea // HACER EN FREERTOS PARA RS485
		RS485_HABILITAR_RX();
		*/
		tx_libre = true;
		xSemaphoreGiveFromISR(semUartTx, &xHigherPriorityTaskWoken);

    }
    break;

  }

  portYIELD_FROM_ISR( xHigherPriorityTaskWoken );

}

/** ===============================================================================
 * @fn			void UART_Reset (int baudrate)
 * @brief				Reinicio el periférico
 * @param[in]			baudrate
 * @return 				None
 * ===============================================================================
 */

void UART_Reset (int baudrate)
{

	uint8_t fcr;
	uint32_t Fdiv;
	uint32_t regVal;

	tx_libre = true;																	// Señalizo el buffer de Tx como vacío
	UARTCount = 0;																	// Señalizo la cuenta del buffer en cero

	/*
	NVIC_DisableIRQ(UART_IRQn);														// Deshabilito la IRQ de la UART

	////////////////////////////////////////////////////
	// CONFIGURACION DE PINES DE ENTRADAS Y SALIDAS :
	////////////////////////////////////////////////////
	LPC_IOCON->PIO1_6 &= ~0x07;    													// UART I/O config
	LPC_IOCON->PIO1_6 |= 0x01;     													// UART RXD
	LPC_IOCON->PIO1_7 &= ~0x07;
	LPC_IOCON->PIO1_7 |= 0x01;     													// UART TXD
*/
	////////////////////////////////////////////////////
	// 		CONFIGURACION DE REGISTROS DEL CLOCK :
	////////////////////////////////////////////////////
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<12);											// Enable UART clock
	LPC_SYSCON->UARTCLKDIV = 0x1;     												// divided by 1

	// LPC_UART->LCR = 0x83;
	LPC_UART->LCR = U0LCR_WORD_LENGTH_SELECT_8BIT | U0LCR_DLAB;						// LCR : Line Control Register : 8 bits, no Parity, 1 Stop bit

	regVal = LPC_SYSCON->UARTCLKDIV;												// Guardo el valor de la división del clock

	Fdiv = (((SystemCoreClock/LPC_SYSCON->SYSAHBCLKDIV)/regVal)/16)/baudrate ;		// Asigno el valor de baud rate

	LPC_UART->DLM = Fdiv / 256;														// DLM : Divisor Latch MSB
	LPC_UART->DLL = Fdiv % 256;														// DLL : Divisor Latch LSB

	// LPC_UART->LCR = 0x03;														// 8N1 y desactivo el DLAB para no tener acceso a DLM y DLL
	LPC_UART->LCR = U0LCR_WORD_LENGTH_SELECT_8BIT;									// Deshabilito el acceso a los registros de división

	// LPC_UART->FCR = 0x07;														// FCR : Habilito y reseteo las FIFO de Tx y Rx
	fcr =	U0FCR_FIFO_ENABLE | 													// Habilito la fifo y los demas bits
			U0FCR_RX_FIFO_RESET |													// Limpio la fifo de rx
			U0FCR_TX_FIFO_RESET |													// Limpio la fifo de tx
			U0FCR_RX_TRIGGER_LEVEL_3;												// Nivel de trigger 4 bits.

	LPC_UART->FCR = fcr;

	regVal = LPC_UART->LSR;															// Leo el Line Stauts Register para limpiarlo

	while ( (LPC_UART->LSR & (LSR_THRE|LSR_TEMT)) != (LSR_THRE|LSR_TEMT) );			// Espero a tener el Buffer de Tx vacío

	while ( LPC_UART->LSR & LSR_RDR )												// Espero a tener el Buffer Rx vacío
	{
		regVal = LPC_UART->RBR;														// Leo los bytes pendientes de la fifo Rx
	}

	NVIC_EnableIRQ(UART_IRQn);														// Vuelvo a habilitar las interrupciones

	LPC_UART->IER = IER_RBR | IER_THRE | IER_RLS;									// IER : Interrupt Enable Register

	//Inicializar_Cola(&cola_rx);														// Inicializo cola de recepcion
	//Inicializar_Cola(&cola_tx);														// Inicializo cola de transmision

	Inic_UART_RS485();
	RS485_HABILITAR_RX();
	return;
}

/** ===============================================================================
 * @fn void UARTInit(uint32_t baudrate)
 *
 * @brief Initialize UART0 port, setup pin select, clock, parity, stop bits, FIFO, etc.
 *
 * @param[in] uint32_t baudrate
 * @return None
 *  ===============================================================================
 */

void UARTInit(uint32_t baudrate)
{
	uint8_t fcr;
	uint32_t Fdiv;
	uint32_t regVal;

	tx_libre = true;																	// Señalizo el buffer de Tx como vacío
	UARTCount = 0;																	// Señalizo la cuenta del buffer en cero

	NVIC_DisableIRQ(UART_IRQn);														// Deshabilito la IRQ de la UART

	////////////////////////////////////////////////////
	// CONFIGURACION DE PINES DE ENTRADAS Y SALIDAS :
	////////////////////////////////////////////////////
	LPC_IOCON->PIO1_6 &= ~0x07;    													// UART I/O config
	LPC_IOCON->PIO1_6 |= 0x01;     													// UART RXD
	LPC_IOCON->PIO1_7 &= ~0x07;
	LPC_IOCON->PIO1_7 |= 0x01;     													// UART TXD
	//Cambio de pines para manejo de la direccion de MAX485,
	// ESTA ES PLACA MICRO, en la otra placa es otro pin (1_1) (C1 deja de ser reset)
	LPC_IOCON->RESET_PIO0_0 &= ~0x07;
	LPC_IOCON->RESET_PIO0_0	|=0x1;

	////////////////////////////////////////////////////
	// 		CONFIGURACION DE REGISTROS DEL CLOCK :
	////////////////////////////////////////////////////
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<12);											// Enable UART clock
	LPC_SYSCON->UARTCLKDIV = 0x1;     												// divided by 1

	// LPC_UART->LCR = 0x83;
	LPC_UART->LCR = U0LCR_WORD_LENGTH_SELECT_8BIT | U0LCR_DLAB;						// LCR : Line Control Register : 8 bits, no Parity, 1 Stop bit

	regVal = LPC_SYSCON->UARTCLKDIV;												// Guardo el valor de la división del clock

	Fdiv = (((SystemCoreClock/LPC_SYSCON->SYSAHBCLKDIV)/regVal)/16)/baudrate ;		// Asigno el valor de baud rate

	LPC_UART->DLM = Fdiv / 256;														// DLM : Divisor Latch MSB
	LPC_UART->DLL = Fdiv % 256;														// DLL : Divisor Latch LSB

	// LPC_UART->LCR = 0x03;														// 8N1 y desactivo el DLAB para no tener acceso a DLM y DLL
	LPC_UART->LCR = U0LCR_WORD_LENGTH_SELECT_8BIT;									// Deshabilito el acceso a los registros de división

	// LPC_UART->FCR = 0x07;														// FCR : Habilito y reseteo las FIFO de Tx y Rx
	fcr =	U0FCR_FIFO_ENABLE | 													// Habilito la fifo y los demas bits
			U0FCR_RX_FIFO_RESET |													// Limpio la fifo de rx
			U0FCR_TX_FIFO_RESET |													// Limpio la fifo de tx
			U0FCR_RX_TRIGGER_LEVEL_3;												// Nivel de trigger 4 bits.

	LPC_UART->FCR = fcr;

	regVal = LPC_UART->LSR;															// Leo el Line Stauts Register para limpiarlo

	while ( (LPC_UART->LSR & (LSR_THRE|LSR_TEMT)) != (LSR_THRE|LSR_TEMT) );			// Espero a tener el Buffer de Tx vacío

	while ( LPC_UART->LSR & LSR_RDR )												// Espero a tener el Buffer Rx vacío
	{
		regVal = LPC_UART->RBR;														// Leo los bytes pendientes de la fifo Rx
	}

	NVIC_EnableIRQ(UART_IRQn);														// Vuelvo a habilitar las interrupciones

#if TX_INTERRUPT																	// Habilito interrupcion por IRQ.
	LPC_UART->IER = IER_RBR | IER_THRE | IER_RLS;									// IER : Interrupt Enable Register
#else
	LPC_UART->IER = IER_RBR | IER_RLS;												// Enable UART interrupt
#endif

	//Inicializar_Cola(&cola_rx);														// Inicializo cola de recepcion
	//Inicializar_Cola(&cola_tx);														// Inicializo cola de transmision

	return;
}

/** ==========================================================================
 * @fn	int Inic_UART_RS485 (void)
 *
 * @brief Inicializo la UART para funcionar en RS485
 *
 * @param[in] None
 * @return None
 *  ==========================================================================
 */

int Inic_UART_RS485 (void)
{

	LPC_UART->RS485CTRL = RS485_RXDIS;														// Deshabilito la recepción a través del pin elegido

#ifdef _RS485_USAR_RTS_DIR

	LPC_UART->RS485CTRL &= !RS485_SEL;														// Utilizo el pin neg(RTS) para direccionar la comunicacion
	RS485_DEFINIR_PIN();																	// Inicializo pin de salida para direccionar RS485

#else
	#ifdef _RS485_USAR_DTR_DIR
		LPC_UART->RS485CTRL |= RS485_SEL;													// Utilizo el pin neg(DTR) para direccionar la comunicacion
	#endif
#endif

	LPC_UART->RS485CTRL &= !RS485_RXDIS;													// Habilito la recepcion de datos
	RS485_HABILITAR_RX();																	// Seteo el pin en bajo para habilitar la recepción

	return 0;


}

/** ==========================================================================
 * @fn int UART_RS485_Habilitar_Recepcion (void)
 *
 * @brief 	Habilito la recepcion para la norma RS485 poniendo en alto
 * 			el pin definido por RS485_DIR_PORT y RS485_DIR_PIN
 *
 * @param[in] None
 * @return None *
 *  ==========================================================================
 */
int UART_RS485_Habilitar_Recepcion (void)
{
	LPC_UART->RS485CTRL &= !RS485_RXDIS;											// Habilito la recepcion de datos
	//GPIOSetValue(RS485_DIR_PORT, RS485_DIR_PIN, 0);
	RS485_HABILITAR_RX();
	return 0;
}

/** ==========================================================================
 * @fn 		int UART_RS485_Deshabilitar_Recepcion (void)
 *
 * @brief 	Habilito la recepcion para la norma RS485 poniendo en alto
 * 			el pin definido por RS485_DIR_PORT y RS485_DIR_PIN
 *
 * @param[in] None
 * @return None
 *  ==========================================================================
 */
int UART_RS485_Deshabilitar_Recepcion (void)
{
	LPC_UART->RS485CTRL |= RS485_RXDIS;											// Deshabilito la recepcion de datos
	//GPIOSetValue(RS485_DIR_PORT, RS485_DIR_PIN, 1);
	RS485_HABILITAR_TX();
	return 0;
}

/*****************************************************************************
** Function name:		UARTSend
**
** Descriptions:		Send a block of data to the UART 0 port based on the data length
**
** parameters:		buffer pointer, and data length
** Returned value:	None
**
*****************************************************************************/


void UARTSend(uint8_t *BufferPtr, uint32_t Length)
{
   while (Length != 0){
		if (tx_libre == true){
			RS485_HABILITAR_TX();								//Habilito pin para transmision de datos
			for(int i=0; i<16; i++){
				LPC_UART->THR = *BufferPtr;					// Escribo el byte del buffer sobre el registro de salida THR
				tx_libre = false;								// not empty in the THR until it shifts out
				BufferPtr++;									// Incremento el puntero al buffer
				Length--;
				if (Length == 0) break;							//Si ya no hay datos para mandar salgo del loop de escritura del THR
			}
		}
    }
	return;
}



/*****************************************************************************
** Function name:		prvUARTSendReplyTask
**
** Descriptions:		Send a Reply
**
** parameters:		Data enters as xQueueReceive
** Returned value:	None
**
*****************************************************************************/

#define DELAY_TX_485		100

void prvUARTSendReplyTask( void *pvParameters )
{
	t_paq_buf_cmp msjReceived;

	TickType_t delayTx485 = DELAY_TX_485;

	int8_t i;
/*
	for( ;; )
	{

		xQueueReceive(xQueueToSendReply, &msjReceived.buffer, portMAX_DELAY );
#if defined(DEBUG_MODE)
		char str[] = "[UARTSend] Received: ";
		UARTSend(str,strlen(str));
		UARTSend(msjReceived.buffer,PQ_LARGO_BYTES);
		UARTSend("\n",1);
#endif

		int sizeofmsj = PQ_LARGO_BYTES;
		while(LPC_UART->LSR&LSR_THRE); // Wait for Previous transmission
		do
		{
			tx_libre = false;								// not empty in the THR until it shifts out
			LPC_UART->THR = msjReceived.buffer[PQ_LARGO_BYTES-sizeofmsj];					// Escribo el byte del buffer sobre el registro de salida THR

			sizeofmsj--;
		} while (sizeofmsj);

		//xQueueSendToBack(xQueueToSend, &msjResponse.buffer, portMAX_DELAY);

	}

*/

	for( ;; )
	{
		// destrabo la tarea por cola de datos de transmisión
		xQueueReceive(xQueueToSendReply, &msjReceived.buffer, portMAX_DELAY );

		RS485_HABILITAR_TX();
		// descargo buffer en la FIFO de TX (funciona mientras que PQ_LARGO_BYTES <= 16)
		for(i = 0; i < PQ_LARGO_BYTES; i++)
			LPC_UART->THR = msjReceived.buffer[i];

		// espero que la IRQ s
		xSemaphoreTake(semUartTx, portMAX_DELAY);

		vTaskDelay(delayTx485);

		RS485_HABILITAR_RX();
	}
}

/******************************************************************************
**                            End Of File
******************************************************************************/
