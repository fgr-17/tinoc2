/*****************************************************************************
 *   gpio.h:  Header file for NXP LPC13xx Family Microprocessors
 *
 *   Copyright(C) 2008, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2008.09.01  ver 1.00    Preliminary version, first Release
 *
******************************************************************************/
#ifndef __GPIO_H 
#define __GPIO_H


/*=======================================================================================
 * DEFINICION DE CONSTANTES :
 *=======================================================================================*/
/* Definición de puertos  */
#define PORT0		0
#define PORT1		1
#define PORT2		2
#define PORT3		3
/* Bit para la definición de dirección de pines entrada / salida */
#define GPIO_ENTRADA										0
#define GPIO_SALIDA 										1
/* Pines GPIO con interrupción : interrumpo por flanco o por nivel */
#define GPIO_IS_EDGE										0
#define GPIO_IS_LEVEL										1
/* Defino tipo de flanco o nivel con el que interrumpe :*/
#define GPIO_IEV_NIVEL_BAJO									0
#define GPIO_IEV_FLANCO_BAJADA								0
#define GPIO_IEV_NIVEL_ALTO									1
#define GPIO_IEV_FLANCO_SUBIDA								1
/* Defino si el flanco lo especifico con IEV o interrumpo por ambos flancos : */
#define GPIO_IBE_CONTROL_IEV								0
#define GPIO_IBE_CONTROL_BOTH								1


/*=======================================================================================
 * 	FUNCIONES EXTERNAS :
 *=======================================================================================*/
void GPIO_IRQHandler(void);
void GPIOInit( void );
void GPIOSetDir( uint32_t portNum, uint32_t bitPosi, uint32_t dir );
void GPIOSetValue( uint32_t portNum, uint32_t bitPosi, uint32_t bitVal );
void GPIOSetInterrupt( uint32_t portNum, uint32_t bitPosi, uint32_t sense,
		uint32_t single, uint32_t event );
void GPIOIntEnable( uint32_t portNum, uint32_t bitPosi );
void GPIOIntDisable( uint32_t portNum, uint32_t bitPosi );
uint32_t GPIOIntStatus( uint32_t portNum, uint32_t bitPosi );
void GPIOIntClear( uint32_t portNum, uint32_t bitPosi );

#endif /* end __GPIO_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
