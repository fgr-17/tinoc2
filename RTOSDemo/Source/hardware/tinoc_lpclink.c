/** @file tinoc_lpclink.c
 *
 * 	@brief 	Funciones de inicialización del hardware
 * 			específicas para la placa TINOC_LPCLINK
 *
 *
 * 	@date 	1 de set. de 2017
 * 	@author Roux, Federico G. (froux@citedef.gob.ar)
 */

/* =========================================================================================
 * 		Inclusion de Archivos :
 * =========================================================================================
 */

#include <stdint.h>
#include <LPC11xx.h>
#include <LPC1102_IOCON.h>
#include <hardware/tinoc_lpclink.h>

#include <flash/flash.h>

/* =========================================================================================
 * 		Prototipos de funciones :
 * =========================================================================================
 */

uint32_t Inicializar_Pines_GPIO (void);
uint32_t Activar_Pin_ISP (void);

/* =========================================================================================
 * 		Variables comunes a todo el archivo
 * =========================================================================================
 */

/**
 * @var IAP iap_entry;
 *
 * @brief variable para acceso a flash
 *
 */
IAP iap_entry;

/* =========================================================================================
 * 		Implementación de funciones :
 * =========================================================================================
 */

/**
 * 	@fn uint32_t Inicializar_Pines_GPIO (void)
 *
 * 	@brief Inicialización de pines destinados a GPIO utilizando las macros de LPC1102_IOCON.h
 *
 * 	@param[in] 	None
 * 	@param[out] 0			OK
 *
 */

uint32_t Inicializar_Pines_GPIO (void)
{
	// LPC_IOCON_D4 = LPC_IOCON_D4_FUNC_PIO1_3  | LPC_IOCON_MODE_PULLUP | LPC_IOCON_HYS_DISABLE | LPC_IOCON_OD_GPIOOUTPUT;
	// LPC_IOCON_A4 = LPC_IOCON_A4_FUNC_PIO0_10 | LPC_IOCON_MODE_PULLUP | LPC_IOCON_HYS_DISABLE | LPC_IOCON_OD_GPIOOUTPUT;
	LPC_IOCON_A2 = LPC_IOCON_A2_FUNC_PIO0_8 | LPC_IOCON_MODE_PULLUP | LPC_IOCON_HYS_DISABLE | LPC_IOCON_OD_GPIOOUTPUT;
	LPC_IOCON_A3 = LPC_IOCON_A3_FUNC_PIO0_9 | LPC_IOCON_MODE_PULLUP | LPC_IOCON_HYS_DISABLE | LPC_IOCON_OD_GPIOOUTPUT;
	LPC_IOCON_B4 = LPC_IOCON_B4_FUNC_PIO0_11 | LPC_IOCON_MODE_PULLUP | LPC_IOCON_HYS_DISABLE | LPC_IOCON_OD_GPIOOUTPUT;
	//LPC_IOCON_A4 = LPC_IOCON_A4_FUNC_PIO0_10 | LPC_IOCON_MODE_PULLUP | LPC_IOCON_HYS_DISABLE | LPC_IOCON_OD_GPIOOUTPUT;
	//LPC_IOCON_D4 = LPC_IOCON_D4_FUNC_PIO1_3 | LPC_IOCON_MODE_PULLUP | LPC_IOCON_HYS_DISABLE | LPC_IOCON_OD_GPIOOUTPUT;

	/* Configure GPIO for LED output. */
	LPC1102_A2_GPIO->DIR |= LPC1102_A2_BIT;
	LPC1102_A3_GPIO->DIR |= LPC1102_A3_BIT;
	LPC1102_B4_GPIO->DIR |= LPC1102_B4_BIT;
	//LPC1102_A4_GPIO->DIR |= LPC1102_A4_BIT;
	//LPC1102_D4_GPIO->DIR |= LPC1102_D4_BIT;

	return 0;
}

/**
 * 	@fn 		uint32_t Inicializar_Pines_GPIO (void)
 *
 * 	@brief 		Inicialización de pines destinados a GPIO utilizando las macros de LPC1102_IOCON.h
 *
 * 	@param[in] 	nada
 * 	@param[out] 0			OK
 *
 */

uint32_t Activar_Pin_ISP (void)
{
	LPC_IOCON_B4 = LPC_IOCON_B4_FUNC_PIO0_11 | LPC_IOCON_HYS_DISABLE | LPC_IOCON_ADMODE_DIGITAL_FUNC | LPC_IOCON_OD_GPIOOUTPUT; // LPC_IOCON_MODE_PULLUP
	LPC1102_B4_GPIO->DIR &= ~(LPC1102_B4_BIT);

	return 0;
}

/** @fn void ReinvokeISP(void)
 *
 * 	@brief Envia comando IAP para inicializar el modo ISP
 *
 * 	@param[in] 		nada
 * 	@param[out] 	nada
 *
 */

void ReinvokeISP(void)
{

	/* Seteo los pines de SWD */
	// LPC_IOCON_A4 = LPC_IOCON_A4_FUNC_SWCLK | LPC_IOCON_MODE_INACTIVE | LPC_IOCON_HYS_DISABLE;
	// LPC_IOCON_D4 = LPC_IOCON_D4_FUNC_SWDIO | LPC_IOCON_MODE_INACTIVE | LPC_IOCON_HYS_DISABLE;

	iap_entry = (IAP) IAP_LOCATION;
	uint32_t command[5], result[4];
	/* make sure 32-bit Timer 1 is turned on before
	calling ISP */
	LPC_SYSCON->SYSAHBCLKCTRL |= 0x00400;
	/* make sure GPIO clock is turned on before calling ISP */
	LPC_SYSCON->SYSAHBCLKCTRL |= 0x00040;
	/* make sure IO configuration clock is turned
	on before calling ISP */
	LPC_SYSCON->SYSAHBCLKCTRL |= 0x10000;
	/* make sure AHB clock divider is 1:1 */
	LPC_SYSCON->SYSAHBCLKDIV = 1;
	/* Send Reinvoke ISP command to ISP entry point*/
	command[0] = IAP_COM_REINVOKE_ISP;
	/* Set stack pointer to ROM value (reset default).
	This must be the last piece of code executed before
	calling ISP, because most C expressions and function
	returns will fail after the stack pointer is changed. */
	__set_MSP(*((uint32_t *)0x1FFF0000)); /* inline asm */
	/* Invoke ISP. We call "iap_entry" to invoke
	ISP because the ISP entry is done through the same
	command interface as IAP. */
	iap_entry(command, result);
	// Code will never return!
}
