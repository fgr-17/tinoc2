
#ifndef UART_UART_H_
#define UART_UART_H_

/** @file 	uart.h
 *
 * @brief 	Header del archivo uart.c
 * @date 	Feb 22, 2018
 * @author Roux, Federico G. (froux@citedef.gob.ar)
 *
 */



/*  =========================================================================================
 *			DEFINICION DE CONSTANTES :
 *  =========================================================================================
 */

/**
 * @defgroup DEFINESUART	Definiciones asociadas al manejo de la UART
 *
 * @brief Definiciones de constantes de registros y funciones de la UART
 *
 * @author Ing. Roux, Federico G. (froux@citedef.gob.ar)
 *
 *
 * @{
 */

/** @brief 0 if TX uses polling, 1 interrupt driven. */
#define TX_INTERRUPT		1

/**
 * @defgroup REGISTRO_IER Registro IER
 * @brief  	Definiciones asociadas al registro Interrupt Enable Register (U0IER)\n
 * 		 	Descripciones copiada directamente del manual UM10398 pag. 202.
 *
 *
\tThe UART RLS interrupt (U0IIR[3:1] = 011) is the highest priority interrupt and is set
whenever any one of four error conditions occur on the UART RX input: overrun error
(OE), parity error (PE), framing error (FE) and break interrupt (BI). The UART Rx error
condition that set the interrupt can be observed via U0LSR[4:1]. The interrupt is cleared
upon a U0LSR read.\n
\tThe UART RDA interrupt (U0IIR[3:1] = 010) shares the second level priority with the CTI
interrupt (U0IIR[3:1] = 110). The RDA is activated when the UART Rx FIFO reaches the
trigger level defined in U0FCR7:6 and is reset when the UART Rx FIFO depth falls below
the trigger level. When the RDA interrupt goes active, the CPU can read a block of data
defined by the trigger level.\n
\tThe CTI interrupt (U0IIR[3:1] = 110) is a second level interrupt and is set when the UART
Rx FIFO contains at least one character and no UART Rx FIFO activity has occurred in
3.5 to 4.5 character times. Any UART Rx FIFO activity (read or write of UART RSR) will
clear the interrupt. This interrupt is intended to flush the UART RBR after a message has
been received that is not a multiple of the trigger level size. For example, if a peripheral
wished to send a 105 character message and the trigger level was 10 characters, the
CPU would receive 10 RDA interrupts resulting in the transfer of 100 characters and 1 to 5
CTI interrupts (depending on the service routine) resulting in the transfer of the remaining
5 characters.
 *\n\n\n VER TABLA DE PRIORIDADES PAGINA 204 UM10398
 *
 * @{
 */

/** @brief  RBR Interrupt Enable. Enables the Receive Data Available
			interrupt for UART. It also controls the Character Receive
			Time-out inturrupt. Reset Value : 0\n
			0 : Disable the RDA interrupt\n
			1 : Enable the RDA interrupt\n
  */
#define IER_RBR		(0x01 << 0)
/** @brief 	THRE Interrupt Enable. Enables the THRE interrupt for
			UART. The status of this interrupt can be read from
			U0LSR[5].\n
			0 Disable the THRE interrupt.\n
			1 Enable the THRE interrupt.\n
 *
 */
#define IER_THRE	(0x01 << 1)
/**
 * @brief 	RX Line Interrupt Enable. Enables the UART RX line
			status interrupts. The status of this interrupt can be read
			from U0LSR[4:1].\n
			0 Disable the RX line status interrupts.\n
			1 Enable the RX line status interrupts.\n
 */
#define IER_RLS		(0x01 << 2)

/**
 * @}
 */

/**
 * @defgroup REGISTRO_IIR Registro IIR
 * @brief  	Definiciones asociadas al registro Interrupt Enable Register (U0IER)\n
 * 		 	Descripciones copiada directamente del manual UM10398 pag. 203
 * @{
 */

/**
 * @brief 	Interrupt status. Note that U0IIR[0] is active low. The
			pending interrupt can be determined by evaluating
			U0IIR[3:1].\n
			0 At least one interrupt is pending. \n
			1 No interrupt is pending. \n

 */
#define IIR_PEND	(0x01 << 0)

/**
 * @defgroup INTID Bits INTID
 * @brief 	Interrupt identification.\n <b> U0IER[3:1] </b> identifies an interrupt 0
			corresponding to the UART Rx FIFO. All other combinations
			of U0IER[3:1] not listed below are reserved (100,101,111).

 * @{
 */
/** @brief 1 - Receive Line Status (RLS). */
#define IIR_RLS		0x03
/** @brief 2a - Receive Data Available (RDA).*/
#define IIR_RDA		0x02
/** @brief  2b - Character Time-out Indicator (CTI). */
#define IIR_CTI		0x06
/** @brief 3 - THRE Interrupt. */
#define IIR_THRE	0x01

/**
 * @}
 */


/**
 * @}
 */


/**
 * @defgroup REGISTRO_LSR Registro LSR
 * @brief  	Definiciones asociadas al registro Line Status Register (U0LSR)\n
 * 		 	Descripciones copiada directamente del manual UM10398 pag. 210\n
 * 		 	The U0LSR is a Read Only register that provides status information on the UART TX and
			RX blocks.\n
 * @{
 */

/**
 * @brief 	Receiver Data Ready. U0LSR[0] is set when the U0RBR holds 0
			an unread character and is cleared when the UART RBR FIFO
			is empty.\n
			0 U0RBR is empty.\n
			1 U0RBR contains valid data.\n
 */
#define LSR_RDR		0x01
/**
 * @brief 	Overrun Error. The overrun error condition is set as soon as it 0
			occurs. A U0LSR read clears U0LSR[1]. U0LSR[1] is set when
			UART RSR has a new character assembled and the UART
			RBR FIFO is full. In this case, the UART RBR FIFO will not be
			overwritten and the character in the UART RSR will be lost.\n
			0 Overrun error status is inactive.\n
			1 Overrun error status is active.\n
 */
#define LSR_OE		0x02
/**
 * @brief 	Parity Error. When the parity bit of a received character is in
			the wrong state, a parity error occurs. A U0LSR read clears
			U0LSR[2]. Time of parity error detection is dependent on
			U0FCR[0].\n
			0 Parity error status is inactive.\n
			1 Parity error status is active.\n
 */
#define LSR_PE		0x04
/**
 * @brief 	Framing Error. When the stop bit of a received character is a 0
			logic 0, a framing error occurs. A U0LSR read clears
			U0LSR[3]. The time of the framing error detection is
			dependent on U0FCR0. Upon detection of a framing error, the
			RX will attempt to re-synchronize to the data and assume that
			the bad stop bit is actually an early start bit. However, it cannot
			be assumed that the next received byte will be correct even if
			there is no Framing Error.
			Note: A framing error is associated with the character at the
			top of the UART RBR FIFO.\n

			0 Framing error status is inactive.\n
			1 Framing error status is active.\n
 */

#define LSR_FE		0x08

/**
 * @brief 	Break Interrupt. When RXD1 is held in the spacing state (all
			zeros) for one full character transmission (start, data, parity,
			stop), a break interrupt occurs. Once the break condition has
			been detected, the receiver goes idle until RXD1 goes to
			marking state (all ones). A U0LSR read clears this status bit.
			The time of break detection is dependent on U0FCR[0].\n
			0
			Note: The break interrupt is associated with the character at
			the top of the UART RBR FIFO.\n

			0 Break interrupt status is inactive.\n
			1 Break interrupt status is active.\n
 */
#define LSR_BI		0x10
/**
 * @brief 	Transmitter Holding Register Empty. THRE is set immediately 1
			upon detection of an empty UART THR and is cleared on a
			U0THR write.\n

			0 U0THR contains valid data.\n
			1 U0THR is empty.\n
 */
#define LSR_THRE	0x20

/**
 * @brief 	Transmitter Empty. TEMT is set when both U0THR and
			U0TSR are empty; TEMT is cleared when either the U0TSR or
			the U0THR contain valid data. This bit is updated as soon as
			50 % of the first stop bit has been transmitted or a byte has
			been written into the THR.\n

			0 U0THR and/or the U0TSR contains valid data.\n
			1 U0THR and the U0TSR are empty.\n
 */
#define LSR_TEMT	0x40

/**
 * @brief 	Error in RX FIFO. U0LSR[7] is set when a character with a RX 0
			error such as framing error, parity error or break interrupt, is
			loaded into the U0RBR. This bit is cleared when the U0LSR
			register is read and there are no subsequent errors in the
			UART FIFO.\n

			0 U0RBR contains no UART RX errors or U0FCR[0]=0.\n
			1 UART RBR contains at least one UART RX error.\n
 */
#define LSR_RXFE	0x80

/**
 * @}
 */

/** @brief Tamaño del buffer de almacenamiento intermedio */
#define BUFSIZE		0x40

	/** @defgroup RS485MODE RS485 mode definition.
	 *  @brief 		The U0RS485CTRL register controls
	 *  			the configuration of the UART in RS-485/EIA-485 mode.
	 *
	 *  @{
	 */

	/** @brief	Habilita el modo multipunto. Se envían las direcciones
	 * 			al bus con error de paridad para generar una IRQ de la
	 * 			UART */
	#define RS485_NMMEN		(0x1 << 0)
	/** @brief Habilita la recepción de datos */
	#define RS485_RXDIS		(0x1 << 1)
	/** @brief Deteccion automática de dirección (AAD) habilitada */
	#define RS485_AADEN		(0x1 << 2)
	/** @brief Determina el pin  de direccion -> 0 : RTS negado P1.5 (J21) // 1 : DTR negado P3.0 (J25)*/
	#define RS485_SEL		(0x1 << 3)
	/** @brief Habilita el control automático de dirección */
	#define RS485_DCTRL		(0x1 << 4)
	/** @brief 0 : El pin de control se pone en 0 cuando se esta por transmitir. Se pone en 1 al terminar la transmision
			   1 : El pin de control se pone en 1 cuando se esta por transmitir. Se pone en 0 al terminar la transmision
	*/
	#define RS485_OINV		(0x1 << 5)
	/**
	 * @}
	 */

	/** @defgroup LCRBITS  BITS DEL REGISTRO LCR - UM10398 pág. 105/106

	 	 @brief The U0LCR determines the format of the data character that is to be transmitted or
				received.
		@{
	 */


	/** @brief Largo de la palabra transmitida : 5 bits.*/
	#define U0LCR_WORD_LENGTH_SELECT_5BIT	(0x00 << 0)
	/** @brief Largo de la palabra transmitida : 6 bits */
	#define U0LCR_WORD_LENGTH_SELECT_6BIT	(0x01 << 0)
	/** @brief Largo de la palabra transmitida : 7 bits */
	#define U0LCR_WORD_LENGTH_SELECT_7BIT	(0x02 << 0)
	/** @brief Largo de la palabra transmitida : 8 bits */
	#define U0LCR_WORD_LENGTH_SELECT_8BIT	(0x03 << 0)

	/** @brief Selecciono 1 bits de stop. Es un cero por default */
	#define U0LCR_STOP_BIT_SELECT_1BIT		(0x00 << 2)
	/** @brief Selecciono 2 bits de stop. Sino queda un bit */
	#define U0LCR_STOP_BIT_SELECT_2BIT		(0x01 << 2)
	/** @brief Habilita la generacion y chequeo paridad */
	#define U0LCR_PARITY_ENABLE				(0x01 << 3)

	/** @brief Paridad par */
	#define U0LCR_PARITY_SELECT_ODD			(0x00 << 4)
	/** @brief Paridad impar */
	#define U0LCR_PARITY_SELECT_EVEN		(0x01 << 4)
	/** @brief Paridad 1 forzado */
	#define U0LCR_PARITY_SELECT_1STICK		(0x02 << 4)
	/** @brief Paridad 0 forzado */
	#define U0LCR_PARITY_SELECT_0STICK		(0x03 << 4)

	/** @brief Habilita Break Transmission : TXD se fuerza a cero cuando este bit se encuentra en uno */
	#define U0LCR_BREAK_CONTROL				(0x01 << 6)
	/** @brief Habilita el acceso a Divisor Latches */
	#define U0LCR_DLAB						(0x01 << 7)

	/**
	 * @}
	 */

/*  BITS DEL REGISTRO FCR  - UM10398 pág.105*/

	/** @defgroup LCRBITS  BITS DEL REGISTRO FCR  - UM10398 pág.105

		@brief 	The U0FCR controls the operation of the UART RX and TX FIFOs.
	@{
	*/

	/** @brief Habilita las FIFO de Tx y Rx. Al modificar este bit se limpian las FIFO. Habilita el acceso de los bits U0FCR[7:1] */
	#define U0FCR_FIFO_ENABLE				(0x01 << 0)
	/** @brief Limpia todos los bits de la fifo Rx */
	#define U0FCR_RX_FIFO_RESET				(0x01 << 1)
 	/** @brief Limpia todos los bits de la fifo Tx */
	#define U0FCR_TX_FIFO_RESET				(0x01 << 2)

	/** @brief Trigger Level 0 : 1 caracter */
	#define U0FCR_RX_TRIGGER_LEVEL_1		(0x00 << 6)
	/** @brief Trigger Level 1 : 4 caracter */
	#define U0FCR_RX_TRIGGER_LEVEL_2		(0x01 << 6)
	/** @brief Trigger Level 2 : 8 caracter */
	#define U0FCR_RX_TRIGGER_LEVEL_3		(0x02 << 6)
	/** @brief Trigger Level 3 : 16 caracter */
	#define U0FCR_RX_TRIGGER_LEVEL_4		(0x03 << 6)

	/**
	 * @}
	 */

/** @brief DEFINICIONES POR DEFAULT */
#define BAUD_RATE_DEFAULT			9600

#ifndef BAUD_RATE
	/** @brief Velocidad de transmisión UART */
	#define 	BAUD_RATE 			BAUD_RATE_DEFAULT
#endif


#define _RS485_USAR_RTS_DIR
#ifdef _RS485_USAR_RTS_DIR
	/** @brief P1.5/neg(RTS)/CT32B0_CAP0 UM10398 pág. 79 */
	#define RTS_PORT			1
	/** @brief P1.5/neg(RTS)/CT32B0_CAP0 UM10398 pág. 79 */
	#define RTS_PIN 			5

#else

	#ifdef _RS485_USAR_RTS_DIR
		#define DTS_PORT			0							// P0.7/CTS
		#define DTS_PIN				7							// P0.7/CTS
	#endif

#endif

/** @brief puerto utilizado para manejo de dirección del puerto RS485 */
#define RS485_DIR_PORT											0 /*1 - TINOC por reset / 0 - Placa Micro*/
/** @brief pin utilizado para manejo de dirección del puerto RS485 */
#define RS485_DIR_PIN											0 /*1 - TINOC por reset / 0 - Placa Micro*/

/** @brief Inicializo pin de salida para direccionar RS485 */
#define RS485_DEFINIR_PIN()										GPIOSetDir( RS485_DIR_PORT, RS485_DIR_PIN, 1 )
/** @brief Seteo el pin en bajo para habilitar la recepción */
#define RS485_HABILITAR_TX()									GPIOSetValue(RS485_DIR_PORT, RS485_DIR_PIN, 1)
/** @brief Seteo el pin en bajo para habilitar la recepción */
#define RS485_HABILITAR_RX()									GPIOSetValue(RS485_DIR_PORT, RS485_DIR_PIN, 0)

#define receiveQUEUE_LENGTH				( 10 )			//Tamaño de la cola de recepcion
#define sendQUEUE_LENGTH				( 5 )			//Tamaño de la cola de salida

/**
 * @}
 */

/*  =========================================================================================
 * 				Prototipos de funciones externas
 *  =========================================================================================
 */

void UART_IRQHandler(void);

extern void ModemInit(void);
extern void UARTInit(uint32_t baudrate);

extern int Inic_UART_RS485 (void);
extern int UART_RS485_Habilitar_Recepcion (void);
extern int UART_RS485_Deshabilitar_Recepcion (void);

extern void UARTSend(uint8_t *BufferPtr, uint32_t Length);


/*  =========================================================================================
 * 						Variables globales
 *  =========================================================================================
 */
#include <stdbool.h>
extern volatile uint32_t UARTStatus;
extern volatile bool  tx_libre;
extern volatile uint8_t  UARTBuffer[BUFSIZE];
extern volatile uint32_t UARTCount;
// extern volatile uint32_t contador_timeout;
// extern volatile uint32_t contador_vueltas_timeout;

//extern t_cola cola_rx;												// Cola de recepcion de la UART  	//*
//extern t_cola cola_tx;												// Cola de transmision de la UART	//*


extern volatile QueueHandle_t xQueueReceived;
extern volatile QueueHandle_t xQueueToSend;
extern volatile QueueHandle_t xQueueToProcess;
extern volatile QueueHandle_t xQueueToSend2;
extern volatile QueueHandle_t xQueueToSendReply;

extern volatile SemaphoreHandle_t semUartTx;

/* @brief Prioridad tarea prvSendSerial */
extern void prvUARTSendReplyTask( void *pvParameters );

#endif /* UART_UART_H_ */
