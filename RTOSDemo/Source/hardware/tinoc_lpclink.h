/** @file 	tinoc_lpclink.h
 **
 ** @brief 	header del archivo tinoc_lpclink.c
 ** @date 	1 de set. de 2017
 * 	@author Roux, Federico G. (froux@citedef.gob.ar)
 **/

#ifndef HARDWARE_TINOC_LPCLINK_H_
#define HARDWARE_TINOC_LPCLINK_H_


/* =========================================================================================
 * 		Definiciones de la placa :
 *  =========================================================================================
 */

/** @defgroup DEFINELPLINK Declaraciones de la placa TINOC_LPCLINK
 *	@brief	Constantes asignadas a los pines de la placa
 *	@{
 */


/**	@defgroup PINC1 Pin C1
 * 	@{
 */

/** @brief 	Defino el bit del puerto ocupado por C1  */
#define LPC1102_C1_BIT			( 1UL << 0UL )
/** @brief Defino el port del puerto ocupado por C1 */
#define LPC1102_C1_PORT			( 0 )
/** @brief Defino el registro GPIO ocupado por C1 */
#define LPC1102_C1_GPIO			LPC_GPIO0

/**
 * @}
 */


/**	@defgroup PINA2 Pin A2
 * 	@{
 */

/** @brief Defino el port y bit del puerto ocupado por A2 */
#define LPC1102_A2_BIT			( 1UL << 8UL )
/** @brief Defino el port del puerto ocupado por A2 */
#define LPC1102_A2_PORT			( 0 )
/** @brief Defino el registro GPIO ocupado por A2 */
#define LPC1102_A2_GPIO			LPC_GPIO0

/**
 * @}
 */


/**	@defgroup PINA3 Pin A3
 * 	@{
 */


/** @brief Defino el port y bit del puerto ocupado por A3 */
#define LPC1102_A3_BIT			( 1UL << 9UL )
/** @brief Defino el port del puerto ocupado por A3 */
#define LPC1102_A3_PORT			( 0 )
/** @brief Defino el registro GPIO ocupado por A3 */
#define LPC1102_A3_GPIO			LPC_GPIO0

/**
 * @}
 */


/**	@defgroup PINA4 Pin A4
 * 	@{
 */



/** @brief Defino el port y bit del puerto ocupado por A4 */
#define LPC1102_A4_BIT			( 1UL << 10UL )
/** @brief Defino el port del puerto ocupado por A4 */
#define LPC1102_A4_PORT			( 0 )
/** @brief Defino el registro GPIO ocupado por A4 */
#define LPC1102_A4_GPIO			LPC_GPIO0

/**
 * @}
 */


/**	@defgroup PINB4 Pin B4
 * 	@{
 */

/** @brief Defino el port y bit del puerto ocupado por B4 */
#define LPC1102_B4_BIT			( 1UL << 11UL )
/** @brief Defino el port del puerto ocupado por B4 */
#define LPC1102_B4_PORT			( 0 )
/** @brief Defino el registro GPIO ocupado por B4 */
#define LPC1102_B4_GPIO			LPC_GPIO0

/**
 * @}
 */


/**	@defgroup PINB3 Pin B3
 * 	@{
 */


/** @brief Defino el port y bit del puerto ocupado por B3 */
#define LPC1102_B3_BIT			( 1UL << 0UL )
/** @brief Defino el port del puerto ocupado por B3 */
#define LPC1102_B3_PORT			( 1 )
/** @brief Defino el registro GPIO ocupado por B3 */
#define LPC1102_B3_GPIO			LPC_GPIO1

/**
 * @}
 */


/**	@defgroup PINC4 Pin C4
 * 	@{
 */

/** @brief Defino el port y bit del puerto ocupado por C4 */
#define LPC1102_C4_BIT			( 1UL << 1UL )
/** @brief Defino el port del puerto ocupado por C4 */
#define LPC1102_C4_PORT			( 1 )
/** @brief Defino el registro GPIO ocupado por C4 */
#define LPC1102_C4_GPIO			LPC_GPIO1

/**
 * @}
 */


/**	@defgroup PINC3 Pin C3
 * 	@{
 */

/** @brief Defino el port y bit del puerto ocupado por C3 */
#define LPC1102_C3_BIT			( 1UL << 2UL )
/** @brief Defino el port del puerto ocupado por C3 */
#define LPC1102_C3_PORT			( 1 )
/** @brief Defino el registro GPIO ocupado por C3 */
#define LPC1102_C3_GPIO			LPC_GPIO1

/**
 * @}
 */


/**	@defgroup PIND4 Pin D4
 * 	@{
 */

/** @brief Defino el port y bit del puerto ocupado por D4 */
#define LPC1102_D4_BIT			( 1UL << 3UL )
/** @brief Defino el port del puerto ocupado por D4 */
#define LPC1102_D4_PORT			( 1 )
/** @brief Defino el registro GPIO ocupado por D4 */
#define LPC1102_D4_GPIO			LPC_GPIO1

/**
 * @}
 */


/**	@defgroup PINC2 Pin C2
 * 	@{
 */

/** @brief Defino el port y bit del puerto ocupado por C2 */
#define LPC1102_C2_BIT			( 1UL << 6UL )
/** @brief Defino el port del puerto ocupado por C2 */
#define LPC1102_C2_PORT			( 1 )
/** @brief Defino el registro GPIO ocupado por C2 */
#define LPC1102_C2_GPIO			LPC_GPIO1

/**
 * @}
 */


/**	@defgroup PIND1 Pin D1
 * 	@{
 */

/** @brief Defino bit del puerto ocupado por D1 */
#define LPC1102_D1_BIT			( 1UL << 7UL )
/** @brief Defino el port del puerto ocupado por D1 */
#define LPC1102_D1_PORT			( 1 )
/** @brief Defino el registro GPIO ocupado por D1 */
#define LPC1102_D1_GPIO			LPC_GPIO1

/**
 * @}
 */




/**
 * @}
 */





/* =========================================================================================
 * 		Funciones externas :
 * =========================================================================================
 */

extern uint32_t Inicializar_Pines_GPIO (void);
extern uint32_t Activar_Pin_ISP (void);
extern void ReinvokeISP(void);

#endif /* HARDWARE_TINOC_LPCLINK_H_ */
