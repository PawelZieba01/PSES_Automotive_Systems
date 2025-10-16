/* board_reset_vector.c
 *
 *	This file contains the reset vector for S32K14X:
 *	reset vector is two 32-bit words, linked to a hardware-specific location,
 *	that tell the hardware:
 *		1) Reset value of main stack pointer
 *		2) Address of the boardResetStart function
 *
 *	Usually this is not needed, because by default the reset vector is part of the interrupt table
 *	(See CORTEXM-exceptiontable.s). However, because of the S32K14X memory layout the interrupt
 *	table is relocated and the reset vector must be provided separately.
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: board_reset_vector.c 28810 2018-06-12 08:20:22Z beoe851 $
*/


#include <Os_kernel.h>
#include <Os_config.h>


/* the function to be executed on reset or power-on	*/
extern void boardResetStart(void);

/* reference the interrupt table so that linker does not optimize it out */
extern void *CORTEXMInterruptTable;

/* Const array of two 32-bit words */
const os_uint32_t reset_vector[3] = {
	(os_uint32_t)OS_kernStack0 + OS_ISTACKLEN_0,	/* stack pointer as in CORTEXM-exceptiontable.s */
	(os_uint32_t)&boardResetStart,
	(os_uint32_t)&CORTEXMInterruptTable				/* not used but needed, see above */
};
