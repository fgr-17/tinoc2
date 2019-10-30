
/**************************************************************************//**
 * @file     main.c
 * @brief    Archivo principal del proyecto
 * @version  V0.1
 * @date     2016
 *
 * @note
 * FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
 *
 * @par
 * ARM Limited (ARM) is supplying this software for use with Cortex-M
 * processor based microcontrollers.  This file can be freely distributed
 * within development tools that are supporting such ARM based processors.
 *
 * @par
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * @brief
 * This project provides two demo applications.  A simple blinky style project,
 * and a more comprehensive test and demo application.  The
 * mainCREATE_SIMPLE_BLINKY_DEMO_ONLY setting (defined in this file) is used to
 * select between the two.  The simply blinky demo is implemented and described
 * in main_blinky.c.  The more comprehensive test and demo application is
 * implemented and described in main_full.c.
 *
 * This file implements the code that is not demo specific, including the
 * hardware setup and FreeRTOS hook functions.  It also contains a dummy
 * interrupt service routine called Dummy_IRQHandler() that is provided as an
 * example of how to use interrupt safe FreeRTOS API functions (those that end
 * in "FromISR").
 *
 *     VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.
 *
 * This file is part of the FreeRTOS distribution.

FreeRTOS is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License (version 2) as published by the
Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

***************************************************************************
>>!   NOTE: The modification to the GPL is included to allow you to     !<<
>>!   distribute a combined work that includes FreeRTOS without being   !<<
>>!   obliged to provide the source code for proprietary components     !<<
>>!   outside of the FreeRTOS kernel.                                   !<<
***************************************************************************

FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  Full license text is available on the following
link: http://www.freertos.org/a00114.html

 ***************************************************************************
 *                                                                       *
 *    FreeRTOS provides completely free yet professionally developed,    *
 *    robust, strictly quality controlled, supported, and cross          *
 *    platform software that is more than just the market leader, it     *
 *    is the industry's de facto standard.                               *
 *                                                                       *
 *    Help yourself get started quickly while simultaneously helping     *
 *    to support the FreeRTOS project by purchasing a FreeRTOS           *
 *    tutorial book, reference manual, or both:                          *
 *    http://www.FreeRTOS.org/Documentation                              *
 *                                                                       *
 ***************************************************************************

	http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
	the FAQ page "My application does not run, what could be wrong?".  Have you
	defined configASSERT()?

	http://www.FreeRTOS.org/support - In return for receiving this top quality
	embedded software for free we request you assist our global community by
	participating in the support forum.

	http://www.FreeRTOS.org/training - Investing in training allows your team to
	be as productive as possible as early as possible.  Now you can receive
	FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
	Ltd, and the world's leading authority on the world's leading RTOS.

	http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
	including FreeRTOS+Trace - an indispensable productivity tool, a DOS
	compatible FAT file system, and our tiny thread aware UDP/IP stack.

	http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
	Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

	http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
	Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
	licenses offer ticketed support, indemnification and commercial middleware.

	http://www.SafeRTOS.com - High Integrity Systems also provide a safety
	engineered and independently SIL3 certified version for use in safety and
	mission critical applications that require provable dependability.

	1 tab == 4 spaces!
 *
 ******************************************************************************/

/** \mainpage RTOSDemo
 *
 *  Programa del TINOC2 basado en ejemplo de freeRTOS sobre el microcontrolador
 *  LPC1102.
 *
 *  Autores:
 *
 *  	- GARCIA, Pablo (Div. Telemetría. CITEDEF)
 *  	- ROUX, Federico (Div. Pirotecnia. CITEDEF)
 *
 */

/* ============================================================================
 * 		INCLUSION DE ARCHIVOS :
 * ============================================================================
 */

/* Standard includes. */
#include "string.h"

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* Hardware specific includes. */
#include "LPC11xx.h"

#include <UART/uart.h>
//#include <UART/colas.h>
#include <leds/leds.h>
#include <comunicaciones/comunicaciones.h>
#include <protocolo/protocolo.h>

/* Set mainCREATE_SIMPLE_BLINKY_DEMO_ONLY to one to run the simple blinky demo,
or 0 to run the more comprehensive test and demo application. */
// #define mainCREATE_SIMPLE_BLINKY_DEMO_ONLY	1

/* The bit on port 0 to which the LED is wired. */
// #define mainLED_BIT		( 1UL << 11UL )

/* The configCHECK_FOR_STACK_OVERFLOW setting in FreeRTOSConifg can be used to
check task stacks for overflows.  It does not however check the stack used by
interrupts.  This demo has a simple addition that will also check the stack used
by interrupts if mainCHECK_INTERRUPT_STACK is set to 1.  Note that this check is
only performed from the tick hook function (which runs in an interrupt context).
It is a good debugging aid - but won't catch interrupt stack problems until the
tick interrupt next executes. */
#define mainCHECK_INTERRUPT_STACK			1
#if mainCHECK_INTERRUPT_STACK == 1
	const unsigned char ucExpectedInterruptStackValues[] = { 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC };
#endif


/*************************************************************
 * 		PROTOTIPOS DE FUNCIONES
 *************************************************************/

static void prvSetupHardware( void );


/*************************************************************
 * 		IMPLEMENTACION DE FUNCIONES
 *************************************************************/

/**
 ******************************************************************************************
 ** @function 	int main( void )
 ** @brief  	Programa ppal
 ** @author 	NXP
 ******************************************************************************************
 **/

int main( void )
{
	/*Queue de Tarea_Delay, test*/
	xQueue = xQueueCreate( mainQUEUE_LENGTH, sizeof( unsigned long ) );

	/*Inicializacion de la Queue de recepcion de mensajes serie*/
	xQueueReceived = xQueueCreate( receiveQUEUE_LENGTH, sizeof( t_paq_buf_cmp ) );		//Inicio la cola de recepcion de mensajes (utilizada para encolar el mensaje obtenido en el IRQ Handler de UART

	/*Inicializacion de la Queue de recepcion de mensajes a procesar*/
	xQueueToClassify = xQueueCreate( receiveQUEUE_LENGTH, sizeof( t_paq_buf_cmp ) );

	/*Inicializacion de la Queue de salida de mensajes serie*/
	xQueueToSend = xQueueCreate( sendQUEUE_LENGTH, sizeof( t_paq_buf_cmp ) );		//Inicio la cola de salida de mensajes (utilizada para encolar el mensaje a la UART

	/*Inicializacion de la Queue de procesamiento de comandos*/
	xQueueToProcess = xQueueCreate( sendQUEUE_LENGTH, sizeof( t_paq_buf_cmp ) );		//Inicio la cola de salida de mensajes (utilizada para encolar el mensaje a la UART

	/*Inicializacion de la Queue de procesamiento de comandos*/
	//xQueueToSend2 = xQueueCreate( sendQUEUE_LENGTH, sizeof( t_paq_buf_cmp ) );		//Inicio la cola de salida de mensajes (utilizada para encolar el mensaje a la UART

	/*Inicializacion de la Queue de Respuestas*/
	xQueueToSendReply = xQueueCreate( sendQUEUE_LENGTH, sizeof( t_paq_buf_cmp ) );		//Inicio la cola de salida de mensajes (utilizada para encolar el mensaje a la UART


	semUartTx = xSemaphoreCreateBinary();


	/* Prepare the hardware to run this demo. */
	prvSetupHardware();
	Inicializar_T0();
	UARTInit(9600);
	Inic_UART_RS485 ();
	// UARTSend("-Tinoc Started-\n",16);
	//RS485_HABILITAR_RX();

	/* Set port for LED to output GPIO0_11 */
	// LPC_IOCON->R_PIO0_11  &= ~0x07;
	// LPC_IOCON->R_PIO0_11  |= 0x01;
	Inicializar_Leds();


	if( xQueue != NULL )
	{

		/*REFERENCIA. */
		/*Como Referencia:
		 * BaseType_t xTaskCreate(    TaskFunction_t pvTaskCode, // The function that implements the task.
		 *                          const char * const pcName, // The text name assigned to the task - for debug only as it is not used by the kernel.
		 *                          unsigned short usStackDepth, // The size of the stack to allocate to the task.
		 *                          void *pvParameters, // The parameter passed to the task - just to check the functionality.
		 *                          UBaseType_t uxPriority, // The priority assigned to the task.
		 *                          TaskHandle_t *pxCreatedTask // The task handle is not required, so NULL is passed.
		 *                        );*/

		/*Inicio tarea de procesamiento de datos serie*/
		xTaskCreate( prvDelayTask, "DELAY", configMINIMAL_STACK_SIZE, ( void * ) mainQUEUE_RECEIVE_PARAMETER, PRIORIDAD_TAREA_prvDelayTask, NULL );

		/*Inicio tarea de procesamiento de datos serie*/
		xTaskCreate( prvProcessSerialMsjTask, "MSJ_PROC", configMINIMAL_STACK_SIZE, ( void * ) NULL, PRIORIDAD_TAREA_prvProcessSerialMsjTask, NULL );

		/*Inicio tarea de clasificacion de mensajes serie*/
		xTaskCreate( prvClassifyMsjTask, "MSJ_CLAS", configMINIMAL_STACK_SIZE, ( void * ) NULL, PRIORIDAD_TAREA_prvClassifyMsjTask, NULL );

		/*Inicio tarea de procesamiento de comando*/
		xTaskCreate( prvProcessCommand, "CMD_PROC", configMINIMAL_STACK_SIZE, ( void * ) NULL, PRIORIDAD_TAREA_prvProcessCommand, NULL );

		/*Inicio tarea de procesamiento de comando*/
		xTaskCreate( prvUARTSendReplyTask, "REPLY", configMINIMAL_STACK_SIZE, ( void * ) NULL, PRIORIDAD_TAREA_prvSendSerial, NULL );





		/* Start the tasks and timer running. */
		vTaskStartScheduler();
	}

	/* If all is well, the scheduler will now be running, and the following
	line will never be reached.  If the following line does execute, then
	there was insufficient FreeRTOS heap memory available for the idle and/or
	timer tasks	to be created.  See the memory management section on the
	FreeRTOS web site for more details. */
	for( ;; );

	return 0;
}


/**
 ******************************************************************************************
 ** @function static void prvSetupHardware( void )
 ** @brief  Perform any application specific hardware configuration.  The clocks,
 ** @brief memory, etc. are configured before main() is called.
 ** @author 	NXP
 ******************************************************************************************
 **/

static void prvSetupHardware( void )
{
extern unsigned long _vStackTop[], _pvHeapStart[];
unsigned long ulInterruptStackSize;

	/* Enable AHB clock for GPIO. */
	LPC_SYSCON->SYSAHBCLKCTRL |= ( 1 << 6 );



	/* Configure GPIO for LED output. */
	// xGPIO0->DIR |= mainLED_BIT;

	/* The size of the stack used by main and interrupts is not defined in
	the linker, but just uses whatever RAM is left.  Calculate the amount of
	RAM available for the main/interrupt/system stack, and check it against
	a reasonable number.  If this assert is hit then it is likely you don't
	have enough stack to start the kernel, or to allow interrupts to nest.
	Note - this is separate to the stacks that are used by tasks.  The stacks
	that are used by tasks are automatically checked if
	configCHECK_FOR_STACK_OVERFLOW is not 0 in FreeRTOSConfig.h - but the stack
	used by interrupts is not.  Reducing the conifgTOTAL_HEAP_SIZE setting will
	increase the stack available to main() and interrupts. */
	ulInterruptStackSize = ( ( unsigned long ) _vStackTop ) - ( ( unsigned long ) _pvHeapStart );
	configASSERT( ulInterruptStackSize > 350UL );

	/* Fill the stack used by main() and interrupts to a known value, so its
	use can be manually checked. */
	memcpy( ( void * ) _pvHeapStart, ucExpectedInterruptStackValues, sizeof( ucExpectedInterruptStackValues ) );
}


/** @defgroup HOOKFUNCTIONS Funciones "Hook" del FreeRTOS
 *	@brief	Funciones handler de eventos de excepcion
 *	@{
 */

/**
 ******************************************************************************************
 ** @function 	void vApplicationMallocFailedHook( void )
 ** @brief  	vApplicationMallocFailedHook() will only be called if
 ** @brief		configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
 ** @brief		function that will get called if a call to pvPortMalloc() fails.
 ** @brief		pvPortMalloc() is called internally by the kernel whenever a task, queue,
 ** @brief		timer or semaphore is created.  It is also called by various parts of the
 ** @brief		demo application.  If heap_1.c or heap_2.c are used, then the size of the
 ** @brief		heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
 ** @brief		FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
 ** @brief		to query the size of free heap space that remains (although it does not
 ** @brief		provide information on how the remaining heap might be fragmented).
 ** @brief 		memory, etc. are configured before main() is called.
 ** @author 	NXP
 ******************************************************************************************
 **/

void vApplicationMallocFailedHook( void )
{
	taskDISABLE_INTERRUPTS();
	for( ;; );
}

/**
 *****************************************************************************************
 ** @function 	void vApplicationIdleHook( void )
 ** @brief  	vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
 ** @brief  	to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
 ** @brief  	task.  It is essential that code added to this hook function never attempts
 ** @brief  	to block in any way (for example, call xQueueReceive() with a block time
 ** @brief  	specified, or call vTaskDelay()).  If the application makes use of the
 ** @brief  	vTaskDelete() API function (as this demo application does) then it is also
 ** @brief  	important that vApplicationIdleHook() is permitted to return to its calling
 ** @brief  	function, because it is the responsibility of the idle task to clean up
 ** @brief  	memory allocated by the kernel to any task that has since been deleted.
 ** @author 	NXP
 *****************************************************************************************
 **/

void vApplicationIdleHook( void )
{
}

/**
 *****************************************************************************************
 ** @function 	void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
 ** @brief  	Run time stack overflow checking is performed if
 ** @brief  	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
 ** @brief  	function is called if a stack overflow is detected.
 ** @author 	NXP
 *****************************************************************************************
 **/


void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}

/**
 *****************************************************************************************
 ** @function 	vApplicationTickHook
 **
 ** @brief  	This function will be called by each tick interrupt if
				configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
				added here, but the tick hook is called from an interrupt context, so
				code must not attempt to block, and only the interrupt safe FreeRTOS API
				functions can be used (those that end in FromISR()).
				Manually check the last few bytes of the interrupt stack to check they
				have not been overwritten.  Note - the task stacks are automatically
				checked for overflow if configCHECK_FOR_STACK_OVERFLOW is set to 1 or 2
				in FreeRTOSConifg.h, but the interrupt stack is not.
 ** @author 	NXP
 *****************************************************************************************
 **/


void vApplicationTickHook( void )
{
#if mainCHECK_INTERRUPT_STACK == 1
extern unsigned long _pvHeapStart[];

	configASSERT( memcmp( ( void * ) _pvHeapStart, ucExpectedInterruptStackValues, sizeof( ucExpectedInterruptStackValues ) ) == 0U );
#endif /* mainCHECK_INTERRUPT_STACK */
}


/** @} */

#ifdef JUST_AN_EXAMPLE_ISR

void Dummy_IRQHandler(void)
{
long lHigherPriorityTaskWoken = pdFALSE;

	/* Clear the interrupt if necessary. */
	Dummy_ClearITPendingBit();

	/* This interrupt does nothing more than demonstrate how to synchronise a
	task with an interrupt.  A semaphore is used for this purpose.  Note
	lHigherPriorityTaskWoken is initialised to zero.  Only FreeRTOS API functions
	that end in "FromISR" can be called from an ISR. */
	xSemaphoreGiveFromISR( xTestSemaphore, &lHigherPriorityTaskWoken );

	/* If there was a task that was blocked on the semaphore, and giving the
	semaphore caused the task to unblock, and the unblocked task has a priority
	higher than the current Running state task (the task that this interrupt
	interrupted), then lHigherPriorityTaskWoken will have been set to pdTRUE
	internally within xSemaphoreGiveFromISR().  Passing pdTRUE into the
	portEND_SWITCHING_ISR() macro will result in a context switch being pended to
	ensure this interrupt returns directly to the unblocked, higher priority,
	task.  Passing pdFALSE into portEND_SWITCHING_ISR() has no effect. */
	portEND_SWITCHING_ISR( lHigherPriorityTaskWoken );
}

#endif /* JUST_AN_EXAMPLE_ISR */




