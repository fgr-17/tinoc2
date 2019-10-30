/*
 * LPC1102_pinout.h
 *
 *  Created on: 16 de ago. de 2017
 *      Author: piro8
 */

#ifndef INC_LPC1102_IOCON_H_
#define INC_LPC1102_IOCON_H_

/** @file LPC1102_IOCON.h
 **
 ** @brief Archivo de configuracion de registro IOCON
 ** @brief Manejo de campos y funciones del registro IOCON\n
 ** @brief UM10429 Capitulo 7: LPC1102/04 I/O Configuration
 ** @author Roux, Federico G. (froux@favaloro.edu.ar)
 **/


/** @defgroup LPC_IOCON_LPC1102_DEFINES Defines IOCON LPC1102
 *	@brief	Defines asociados al set de registros IOCON del LPC1102
 *	@{
 */


/** @ingroup  LPC_IOCON_LPC1102_DEFINES
 *  @defgroup LPC_IOCON_LPC1102_BITS
 *  @brief Bits del registro IOCON LPC1102
 *
 *	@{
 */

/** @brief Bit de función */
#define LPC_IOCON_FUNC_BIT				(0)
/** @brief Bits de Modo */
#define LPC_IOCON_MODE_BIT				(3)
/** @brief Bit de histeresis */
#define LPC_IOCON_HYS_BIT				(5)
/** @brief Bit de modo analogico o digital*/
#define LPC_IOCON_ADMODE_BIT			(7)
/** @brief Bit de Open Drain */
#define LPC_IOCON_OD_BIT				(10)

/** @} fin de LPC_IOCON_LPC1102_BITS */


/** @ingroup  LPC_IOCON_LPC1102_DEFINES
 *  @defgroup LPC_IOCON_LPC1102_MASCARAS
 *  @brief Mascaras del registro IOCON LPC1102
 *
 *	@{
 */

/** Mascara de bits de función */
#define LPC_IOCON_FUNC_MAS					(0x00000007UL) << LPC_IOCON_FUNC_BIT
/** Mascara de bits de modo */
#define LPC_IOCON_MODE_MAS					(0x00000003UL) << LPC_IOCON_MODE_BIT
/** Mascara de bits de histeresis */
#define LPC_IOCON_HYS_MAS					(0x00000001UL) << LPC_IOCON_HYS_BIT
/** Mascara de bits de opendrain */
#define LPC_IOCON_OD_MAS					(0x00000001UL) << LPC_IOCON_OD_BIT

/** @} fin de LPC_IOCON_LPC1102_MASCARAS */


/** @ingroup  LPC_IOCON_LPC1102_DEFINES
 *  @defgroup LPC_IOCON_LPC1102_PINES
 *  @brief Funciones de cada pin asociados a los registros IOCON
 *	@{
 */


/** @name C1 - RESET/P0.0
 * @{
 */

#define LPC_IOCON_C1_FUNC_RESET			(0x00000000UL) << LPC_IOCON_FUNC_BIT
#define LPC_IOCON_C1_FUNC_PIO0_0		(0x00000001UL) << LPC_IOCON_FUNC_BIT

/** @} */

/** @name A2 - P0.8/MISO/CT16B0_MAT0
 * @{
 */

#define LPC_IOCON_A2_FUNC_PIO0_8		(0x00000000UL) << LPC_IOCON_FUNC_BIT
#define LPC_IOCON_A2_FUNC_MISO0			(0x00000001UL) << LPC_IOCON_FUNC_BIT
#define LPC_IOCON_A2_FUNC_CT16B0_MAT0	(0x00000002UL) << LPC_IOCON_FUNC_BIT

/** @} */

/** @name A3 - P0.9/MOSI/CT16B0_MAT1
 * @{
 */

#define LPC_IOCON_A3_FUNC_PIO0_9		(0x00000000UL) << LPC_IOCON_FUNC_BIT
#define LPC_IOCON_A3_FUNC_MOSI0			(0x00000001UL) << LPC_IOCON_FUNC_BIT
#define LPC_IOCON_A3_FUNC_CT16B0_MAT1	(0x00000002UL) << LPC_IOCON_FUNC_BIT

/** @} */

/** @name A4 - SWCLK/P0.10/SCK0/CT16B0_MAT2
 * @{
 */

#define LPC_IOCON_A4_FUNC_SWCLK			(0x00000000UL) << LPC_IOCON_FUNC_BIT
#define LPC_IOCON_A4_FUNC_PIO0_10		(0x00000001UL) << LPC_IOCON_FUNC_BIT
#define LPC_IOCON_A4_FUNC_SCK0			(0x00000002UL) << LPC_IOCON_FUNC_BIT
#define LPC_IOCON_A4_FUNC_CT16B0_MAT2	(0x00000003UL) << LPC_IOCON_FUNC_BIT

/** @} */

/** @name B4 - R/P0.11/AD0/CT32B0_MAT3
 * @{
 */

#define LPC_IOCON_B4_FUNC_R				(0x00000000UL) << LPC_IOCON_FUNC_BIT
#define LPC_IOCON_B4_FUNC_PIO0_11		(0x00000001UL) << LPC_IOCON_FUNC_BIT
#define LPC_IOCON_B4_FUNC_AD0			(0x00000002UL) << LPC_IOCON_FUNC_BIT
#define LPC_IOCON_B4_FUNC_CT16B0_MAT3	(0x00000003UL) << LPC_IOCON_FUNC_BIT

/** @} */


/** @name B3 - R/P1.0/AD1/CT32B1_CAP0
 * @{
 */

#define LPC_IOCON_B3_FUNC_R				(0x00000000UL) << LPC_IOCON_FUNC_BIT
#define LPC_IOCON_B3_FUNC_PIO1_0		(0x00000001UL) << LPC_IOCON_FUNC_BIT
#define LPC_IOCON_B3_FUNC_AD1			(0x00000002UL) << LPC_IOCON_FUNC_BIT
#define LPC_IOCON_B3_FUNC_CT32B1_CAP0	(0x00000003UL) << LPC_IOCON_FUNC_BIT

/** @} */

/** @name C4 - R/P1.1/AD2/CT32B1_MAT0
 * @{
 */

#define LPC_IOCON_C4_FUNC_R				(0x00000000UL) << LPC_IOCON_FUNC_BIT
#define LPC_IOCON_C4_FUNC_PIO1_1		(0x00000001UL) << LPC_IOCON_FUNC_BIT
#define LPC_IOCON_C4_FUNC_AD2			(0x00000002UL) << LPC_IOCON_FUNC_BIT
#define LPC_IOCON_C4_FUNC_CT32B1_MAT0	(0x00000003UL) << LPC_IOCON_FUNC_BIT

/** @} */

/** @name C3 - R/P1.2/AD3/CT32B1_MAT1
 * @{
 */

#define LPC_IOCON_C3_FUNC_R				(0x00000000UL) << LPC_IOCON_FUNC_BIT
#define LPC_IOCON_C3_FUNC_PIO1_2		(0x00000001UL) << LPC_IOCON_FUNC_BIT
#define LPC_IOCON_C3_FUNC_AD3			(0x00000002UL) << LPC_IOCON_FUNC_BIT
#define LPC_IOCON_C3_FUNC_CT32B1_MAT1	(0x00000003UL) << LPC_IOCON_FUNC_BIT

/** @} */

/** @name D4 - SWDIO/P1.3/AD4/CT32B1_MAT2
 * @{
 */

#define LPC_IOCON_D4_FUNC_SWDIO			(0x00000000UL) << LPC_IOCON_FUNC_BIT
#define LPC_IOCON_D4_FUNC_PIO1_3		(0x00000001UL) << LPC_IOCON_FUNC_BIT
#define LPC_IOCON_D4_FUNC_AD4			(0x00000002UL) << LPC_IOCON_FUNC_BIT
#define LPC_IOCON_D4_FUNC_CT32B1_MAT2	(0x00000003UL) << LPC_IOCON_FUNC_BIT

/** @} */

/** @name C2 - P1.6/RXD/CT32B0_MAT0
 *  @{
 */

#define LPC_IOCON_C2_FUNC_PIO1_3		(0x00000000UL) << LPC_IOCON_FUNC_BIT
#define LPC_IOCON_C2_FUNC_RXD			(0x00000001UL) << LPC_IOCON_FUNC_BIT
#define LPC_IOCON_C4_FUNC_CT32B0_MAT0	(0x00000002UL) << LPC_IOCON_FUNC_BIT
/** @} */
/** @name D1 - P1.7/TXD/CT32B0_MAT1
 *  @{
 */
#define LPC_IOCON_D1_FUNC_PIO1_7		(0x00000000UL) << LPC_IOCON_FUNC_BIT
#define LPC_IOCON_D1_FUNC_TXD			(0x00000001UL) << LPC_IOCON_FUNC_BIT
#define LPC_IOCON_D1_FUNC_CT32B0_MAT1	(0x00000002UL) << LPC_IOCON_FUNC_BIT
/** @} */


/** @} fin de LPC_IOCON_LPC1102_PINES */




/** @ingroup  LPC_IOCON_LPC1102_DEFINES
 *  @defgroup LPC_IOCON_LPC1102_MODE
 *  @brief Modos del registro IOCON comunes a todos los pines
 *
 *	@{
 */

/** @brief Inactivo : sin resistencia pull-down/pull-up */
#define LPC_IOCON_MODE_INACTIVE			(0x00000000UL) << LPC_IOCON_MODE_BIT
/** @brief Resistencia de Pull-Down habilitada */
#define LPC_IOCON_MODE_PULLDOWN			(0x00000001UL) << LPC_IOCON_MODE_BIT
/** @brief Resistencia de Pull-Up habilitada */
#define LPC_IOCON_MODE_PULLUP			(0x00000002UL) << LPC_IOCON_MODE_BIT
/** @brief Modo repetidor */
#define LPC_IOCON_MODE_REPEATER			(0x00000003UL) << LPC_IOCON_MODE_BIT

/** @} fin de LPC_IOCON_LPC1102_MODE */


/** @ingroup  LPC_IOCON_LPC1102_DEFINES
 *  @defgroup LPC_IOCON_LPC1102_HYS_OD
 *  @brief Bits de histéresis y OpenDrain
 *
 *	@{
 */

/** @name Bit de Histéresis
 *  @{
 */
/** @brief Histéresis Deshabilitada */
#define LPC_IOCON_HYS_DISABLE			(0x00000000UL) << LPC_IOCON_HYS_BIT
/** @brief Histéresis Habilitada */
#define LPC_IOCON_HYS_ENABLE			(0x00000001UL) << LPC_IOCON_HYS_BIT

/** @} */
/** @name Bit de Analog/Digital Mode para pines del AD
 *  @{
 */

/** @brief Modo Entrada Analógica */
#define LPC_IOCON_ADMODE_ANALOG_INPUT	(0x00UL) << LPC_IOCON_ADMODE_BIT
/** @brief Modo Función digital */
#define LPC_IOCON_ADMODE_DIGITAL_FUNC	(0x01UL) << LPC_IOCON_ADMODE_BIT


/** @} */
/** @name Bit de Open Drain
 *  @{
 */
/** @brief Salida GPIO estándar */
#define LPC_IOCON_OD_GPIOOUTPUT			(0x00000000UL) << LPC_IOCON_OD_BIT
/** @brief Salida Open Drain */
#define LPC_IOCON_OD_ODOUTPUT			(0x00000001UL) << LPC_IOCON_OD_BIT
/** @} */
/** @} fin de LPC_IOCON_LPC1102_HYS_OD */


/** @ingroup  LPC_IOCON_LPC1102_DEFINES
 *  @defgroup LPC_IOCON_LPC1102_ASOCIACIONES
 *  @brief Asociacion del nombre del registro en la estructura LPC_IOCON
 *  con los nombres del manual de <letra><nro>
 *
 *	@{
 */

#define LPC_IOCON_C1					LPC_IOCON->RESET_PIO0_0
#define LPC_IOCON_A2					LPC_IOCON->PIO0_8
#define LPC_IOCON_A3					LPC_IOCON->PIO0_9
#define LPC_IOCON_A4					LPC_IOCON->SWCLK_PIO0_10
#define LPC_IOCON_B4					LPC_IOCON->R_PIO0_11
#define LPC_IOCON_B3					LPC_IOCON->R_PIO1_0
#define LPC_IOCON_C4					LPC_IOCON->R_PIO1_1
#define LPC_IOCON_C3					LPC_IOCON->R_PIO1_2
#define LPC_IOCON_D4					LPC_IOCON->SWDIO_PIO1_3
#define LPC_IOCON_C2					LPC_IOCON->PIO1_6
#define LPC_IOCON_D1					LPC_IOCON->PIO1_7


/** @} fin de LPC_IOCON_LPC1102_ASOCIACIONES */

/** @ingroup  LPC_IOCON_LPC1102_DEFINES
 *  @defgroup LPC_IOCON_LPC1102_SCK_LOC
 *  @brief Registro que define en qué pin se asocia la función SCK
 *  @brief Para el LPC1102 solo se usaria en el valor por reset
 *
 *	@{
 */

/** @brief Bit del registro para modificar el pin SCKLOC*/
#define LPC_IOCON_SCKLOC_BIT			(0)
/** @brief Asocio la función de SCK al Pin P0.10 A4 (por default en LPC1102)*/
#define LPC_IOCON_SCKLOC_P0_10			(0x00000000UL) << LPC_IOCON_SCKLOC_BIT
/** @brief Asocio la función de SCK al Pin P0.6 A1 solo en LPC1104 */
#define LPC_IOCON_SCKLOC_P0_6			(0x00000002UL) << LPC_IOCON_SCKLOC_BIT

/** @} fin de LPC_IOCON_LPC1102_SCK_LOC */

/** @} */


/**
 * Funciones externas :
 */

/** @brief */
typedef enum {C1, A2, A3, A4, B4, B3, C4, C3, D4, C2, D1} t_pin;


#define DEFINIR_PIN_CONFIGURACION_IO_ERROR_PIN			1

extern uint8_t Definir_Pin_Configuracion_IO(t_pin pin, uint32_t funcion, uint32_t modo, uint32_t histeresis, uint32_t opendrain);



#endif /* INC_LPC1102_IOCON_H_ */
