/*
 * protocolo.h
 *
 *  Created on: 20/10/2014
 *      Author: Pirotecnia-06
 */

#ifndef PROTOCOLO_H_
#define PROTOCOLO_H_

#include "stdint.h"

//#include <UART/colas.h>

/****************************************************************************************************
 * Definición de los diferentes bytes correspondientes a un paquete.
 ****************************************************************************************************/

#define PQ_LARGO_BYTES									8							// Cantidad de bytes total del paquete

	#define PQ_BYTE_DES									0							// ID del destinatario del paquete
	#define PQ_BYTE_ORG									1							// ID de quién envió el paquete
	#define PQ_BYTE_CMD									2							// Byte de comando
	#define PQ_LARGO_DATOS								4							// Cantidad de bytes de datos
		#define PQ_BYTE_D3								3							// 1er byte de datos
		#define PQ_BYTE_D2								4							// 2do byte de datos
		#define PQ_BYTE_D1								5							// 3er byte de datos
		#define PQ_BYTE_D0								6							// 4to byte de datos
	#define PQ_BYTE_CHK									7							// Byte de checksum

/****************************************************************************************************
 * Definición de constantes de retorno de la fc. PAQ_Extraer_Info_Cola()
 ****************************************************************************************************/

#define PAQ_EXTRAER_INFO_COLA_OK						0
#define PAQ_EXTRAER_INFO_COLA_ERROR						-1

/****************************************************************************************************
 * Definición de constantes de retorno de la fc. PAQ_Clasificar_Recibido()
 ****************************************************************************************************/

#define PAQ_CLASIFICAR_RECIBIDO_OK						 0
#define PAQ_CLASIFICAR_RECIBIDO_PAQ_NO_EXTRAIDO			-1
#define PAQ_CLASIFICAR_RECIBIDO_PAQ_ERROR_DST			-2
#define PAQ_CLASIFICAR_RECIBIDO_PAQ_ERROR_CHK			-3
#define PAQ_CLASIFICAR_RECIBIDO_PAQ_ERROR_CMD 			-4
#define PAQ_CLASIFICAR_RECIBIDO_PAQ_ERROR_ECX 			-5
#define PAQ_CLASIFICAR_RECIBIDO_TIMEOUT					-6
#define PAQ_ERROR_ECX									-7
#define PAQ_ENVIAR_DATO									-8

/****************************************************************************************************
 * Definición de constantes de retorno de la fc. PAQ_Validar_Checksum()
 ****************************************************************************************************/

#define PAQ_VALIDAR_CHECKSUM_VALIDO						0
#define PAQ_VALIDAR_CHECKSUM_NO_VALIDO					-1

/****************************************************************************************************
 * Definición de constantes de retorno de la fc. PAQ_Validar_Comando()
 ****************************************************************************************************/

#define PAQ_VALIDAR_COMANDO_VALIDO						0
#define PAQ_VALIDAR_COMANDO_NO_VALIDO					-1

/****************************************************************************************************
 * Definición de constantes de retorno de la fc. PAQ_Agregar_Checksum()
 ****************************************************************************************************/

#define PAQ_AGREGAR_CHECKSUM_OK							0

/****************************************************************************************************
 * Definición de constantes de retorno de la fc. PAQ_Enviar_Cola_Salida()
 ****************************************************************************************************/

#define PAQ_ENVIAR_COLA_SALIDA_OK						0
#define PAQ_ENVIAR_COLA_SALIDA_ERR_COLA					-1
#define PAQ_ENVIAR_COLA_SALIDA_THR_OCUPADO				-2
#define PAQ_ENVIAR_COLA_SALIDA_COLA_VACIA				-3

/****************************************************************************************************
 * Definición de comandos válidos
 ****************************************************************************************************/

#define CMD_OK											0x40										// Respuesta a un frame correcto
#define CMD_DAT											0x41										// Pedido y respuestra de datos

#define CMD0											0x50
#define CMD1											0x51
#define CMD2											0x52
#define CMD3											0x53
#define CMD4											0x54
#define CMD5											0x55
#define CMD6											0x56
#define CMD7											0x57
#define CMD8											0x58
#define CMD9											0x59
#define CMD10											0x5A
#define CMD11											0x5B
#define CMD12											0x5C
#define CMD13											0x5D
#define CMD14											0x5E
#define CMD15											0x5F
#define CMD16											0x60
#define CMD17											0x61
#define CMD18											0x62
#define CMD19											0x63
#define CMD20											0x64

// Redefinición de comandos ajustados al programa :

#define CMD_RESET 										CMD0								// Reset por soft del sistema
#define CMD_INIC										CMD1								// Inicializo la máquina de estado
#define CMD_MOTINI										CMD2
#define CMD_MOTFIN										CMD3
#define CMD_MOTMODVEL									CMD4
#define CMD_MOTMODGIR									CMD5
#define CMD_MOTFREN										CMD6
#define CMD_MOTCONT										CMD7
#define CMD_GFINI										CMD8
#define CMD_GFFIN										CMD9
#define CMD_GFABRIR									 	CMD10
#define CMD_GFCERRAR									CMD11



#define CMD_ECX											0x80										// Respuesta a un frame con error de contexto
#define CMD_ENE											0x81										// Respuesta a un frame con error de comando inexistente
#define CMD_ECK											0x82										// Respuesta a un frame con error de checksum
#define CMD_EID											0x83										// Respuesta a un frame con error de ID de origen (no tenía privilegio para pedir el comando)
#define CMD_EIN											0x84										// Respuesta a un frame cuando tengo un error interno de manejo de datos
#define CMD_ETO											0x85										// Respuesta a un frame cuando está incompleto y se activó el timeout

/****************************************************************************************************
 * \type t_paq_dato estructura con el dato asociado a un paquete
 ****************************************************************************************************/

typedef struct
{
	uint8_t b0;
	uint8_t b1;
	uint8_t b2;
	uint8_t b3;
} t_4bytes;

typedef struct
{
	uint16_t w0;
	uint16_t w1;
}t_2words;

typedef union
{
	t_4bytes bytes;
	t_2words words;
	uint32_t qword;
}t_paq_dato;

/****************************************************************************************************
 * \type t_paq_campos estructura con los diferentes campos de un paquete
 ****************************************************************************************************/

typedef struct
{
	uint8_t DST;														// byte de ID del equipo de destino
	uint8_t ORG;														// byte de ID del equipo de origen
	uint8_t CMD;														// byte de comando
	// t_paq_dato dato;													// bytes de datos accedidos de diferentes manera
	uint8_t	D3;
	uint8_t	D2;
	uint8_t	D1;
	uint8_t	D0;
	uint8_t CHK;														// byte de checksum
} t_paq_campos;

/****************************************************************************************************
 * \type t_paq_buf_cmp	union con buffer y campos del paquete
 ****************************************************************************************************/

typedef union
{
	uint8_t buffer[PQ_LARGO_BYTES];										// buffer de datos para acceder como bytes
	t_paq_campos bytes_campos;											// buffer de datos para acceder como campos
} t_paq_buf_cmp;

///****************************************************************************************************
// * \type t_canal contiene todos los campos para el manejo de un canal de comunicación				*
// ****************************************************************************************************/
//
//typedef struct
//{
////	uint8_t ID_propio;													// ID propio
//// 	uint8_t ID_origen;													// ID del equipo que envió el paquete
//
//	uint8_t paq_pendiente;												// flag de dato pendiente
//	t_cola* cola;														// struct de cola
//	t_paq_buf_cmp buf_cmp;												// buffer de datos con campos asociados
//} t_canal;

/****************************************************************************************************
 * \type t_canal contiene todos los campos para el manejo de una terminal							*
 ****************************************************************************************************/

typedef struct
{
	uint8_t ID;
}t_terminal;


/************************************************************************************
 * 								FUNCIONES EXTERNAS 									*
 ************************************************************************************/

//extern void Inicializar_Terminal (volatile t_terminal* t);
//extern void Inicializar_Canal_RX (void);
//extern void Inicializar_Canal_TX (void);

//extern int PAQ_Enviar_Cola_Salida (void);
//extern int PAQ_Extraer_Info_Cola (void);
extern int PAQ_Clasificar_Recibido (t_paq_buf_cmp paq_buf_cmp);
extern t_paq_buf_cmp PAQ_Armar_Respuesta_Estandar (t_paq_buf_cmp paq_buf_cmp,uint8_t cmd_respuesta);
t_paq_buf_cmp PAQ_Armar_Respuesta_CMD_DAT (t_paq_buf_cmp paq_buf_cmp,t_paq_dato dato);
uint8_t PAQ_Calcular_Checksum (t_paq_buf_cmp paq_buf_cmp);
extern int PAQ_Validar_Checksum (t_paq_buf_cmp paq_buf_cmp);
extern int PAQ_Validar_Comando (t_paq_buf_cmp paq_buf_cmp);
//extern void PAQ_Rutina_Principal (void);
//extern void PAQ_Error (int error_sel);

/************************************************************************************
 * 								VARIABLES GLOBALES									*
 ************************************************************************************/

//extern volatile t_canal canal_rx;																// Estructura de datos asociada al canal de recepcion
extern volatile t_terminal t0;																	// Estructura de datos asociada a la terminal

extern volatile t_paq_dato global_dato;
extern volatile int global_paq_clasificado;


#endif /* PROTOCOLO_H_ */
