//*****************************************************************************
//   +--+
//   | ++----+
//   +-++    |
//     |     |
//   +-+--+  |
//   | +--+--+
//   +----+    Copyright (c) 2011-12 Code Red Technologies Ltd.
//
// LED flashing SysTick application for LPCXPresso1102 board
//
// 2012.01.04  - Minor mod to remove use of SYSTICKCLKDIV register (which does
//               not exist on LPC11 parts) in setting up Systick timer.
//
// Software License Agreement
//
// The software is owned by Code Red Technologies and/or its suppliers, and is
// protected under applicable copyright laws.  All rights are reserved.  Any
// use in violation of the foregoing restrictions may subject the user to criminal
// sanctions under applicable laws, as well as to civil liability for the breach
// of the terms and conditions of this license.
//
// THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
// OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
// USE OF THIS SOFTWARE FOR COMMERCIAL DEVELOPMENT AND/OR EDUCATION IS SUBJECT
// TO A CURRENT END USER LICENSE AGREEMENT (COMMERCIAL OR EDUCATIONAL) WITH
// CODE RED TECHNOLOGIES LTD.
//
//*****************************************************************************
#include "LPC11xx.h"                        /* LPC11xx definitions */
#include "gpio.h"

#include <cr_section_macros.h>
#include <NXP/crp.h>


/** \mainpage LPCX1102_cmsis2_systick
 *
 *  Ejemplo baremetal blinky sobre el led P0.11, es decir, B4.
 *  No utiliza las definiciones de LPC_IOCON de CMSIS.
 *
 *  Se utiliza para hacer chequeo de funcionamiento de los dispositivos.
 *
 *
 */


// Variable to store CRP value in. Will be placed automatically
// by the linker when "Enable Code Read Protect" selected.
// See crp.h header for more information
__CRP const unsigned int CRP_WORD = CRP_NO_CRP ;


// LPCXpresso1102 eval board LED
#define LED_PORT 0		// Port for led
#define LED_BIT 11		// Bit on port for led
#define LED_ON 1		// Level to set port to turn on led
#define LED_OFF 0		// Level to set port to turn off led

// Time period to wait between toggling LED. 2000 msecs = 2 secs.
#define DELAY_LEN 2000

volatile uint32_t msTicks;                            /* counts 1ms timeTicks */
/*----------------------------------------------------------------------------
  SysTick_Handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void) {
  msTicks++;                        /* increment counter necessary in Delay() */
}

/*------------------------------------------------------------------------------
  delays number of tick Systicks (happens every 1 ms)
 *------------------------------------------------------------------------------*/
__INLINE static void Delay (uint32_t dlyTicks) {
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks);
}

/* Main Program */

int main (void) {

  /* Initialize GPIO (sets up clock) */
  GPIOInit();

  /* Set port for LED to output GPIO0_11 */
   LPC_IOCON->R_PIO0_11  &= ~0x07;
   LPC_IOCON->R_PIO0_11  |= 0x01;

  /* Set port for LED to output */
  GPIOSetDir(LED_PORT, LED_BIT, 1 );

  if (SysTick_Config(SystemCoreClock / 1000)) { /* Setup SysTick Timer for 1 msec interrupts  */
    while (1);                                  /* Capture error */
  }


  while (1)                                /* Loop forever */
  {
	  // Turn LED on, then wait
	  GPIOSetValue( LED_PORT, LED_BIT, LED_ON );
	  Delay (DELAY_LEN);

	  // Turn LED off, then wait
	  GPIOSetValue( LED_PORT, LED_BIT, LED_OFF );
	  Delay (DELAY_LEN);
  }
}
