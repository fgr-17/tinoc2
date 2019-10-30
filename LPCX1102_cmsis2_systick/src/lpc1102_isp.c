// ******************************************************************
// lpc1102_isp.c - Version 121126
//
// Copyright (c) 2011-12 Code Red Technologies Ltd.
// Based on original code and documentation by NXP.
//
// This file provides functions for the NXP LPC1102 to check if the
// user has requested the MCU to enter the ISP bootloader at startup,
// and if so, then the ISP is invoked.
//
// Note that the code defaults to using P1.6 as the ISP request pin.
// You will need to modify the code if your hardware requirements
// mean that an alternative pin needs to be used.
//
// For more information, please see  NXP's application note:
// - AN11015 "Adding ISP to LPC1102 systems"
// ******************************************************************

// This code will only get built into the project if the compiler
// define "__ENABLE_LPC1102_ISP" is set. This will be done
// automatically by the Code Red IDE LPC1102 project wizard if the
// appropriate tick box is enabled.
// It also checks to makes sure the target MCU is LPC1102, as LPC1104
// has a dedicated ISP pin and so this code is not then needed.
#if defined(__ENABLE_LPC1102_ISP) && defined (LPC1102)

#include "LPC11xx.h"                        /* LPC11xx definitions */

typedef void (*IAP)(uint32_t [], uint32_t []);

void ReinvokeISP(void)
{
	// 'iap_entry' and 'command[5]' marked as 'static' to
	// prevent issues when optimisation enabled.
	static IAP iap_entry = (IAP)0x1fff1ff1;
	static uint32_t command[5], result[4];

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
	command[0] = 57;
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

// The CheckForISP() function will be invoked by the reset
// handler code in cr_startup_lpc1102.c.
void CheckForISP(void)
{
	/* Enable AHB clock to the GPIO domain. */
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);

	// Note: Default pin setup is input, pullup
	// If P1.6 is pulled low, then reinvoke ISP
	if( !(LPC_GPIO1->DATA & (1<<6)))
		  ReinvokeISP();
}

#endif // __USE_LPC1102_ISP && LPC1102
