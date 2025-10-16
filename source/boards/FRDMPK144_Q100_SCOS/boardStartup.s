/* boardStartup.s - board specific assembly startup code.
 *
 * This file contains the early startup code implemented in assembly.
 * It exports the following functions:
 *  boardResetStart: Reset vector.
 *  boardInvalidInterrupt: Handler for invalid / spurious interrupts if
 *                         ErrorHandling is set to MINIMAL.
 *  boardInvalidException: Handler for exceptions if ExceptionHandling is
 *                         disabled.
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: boardStartup.s 29524 2018-11-23 14:02:00Z aniv261689 $
*/

#ifndef OS_ASM
#define OS_ASM
#endif

#include <CORTEXM/Os_asm_CORTEXM.h>
#include <Os_configuration.h>


	OS_ASM_SECTION_TEXT
	OS_ASM_CODE_THUMB

	OS_global boardResetStart
	OS_global boardInvalidInterrupt
	OS_global boardInvalidException
	OS_global boardDisableInterrupts
	OS_global boardEnableInterrupts
	OS_extern BoardStart
	OS_extern CORTEXMExceptionTable
	OS_extern OS_kernStack0
	OS_extern OS_kernel_ptr

#ifdef OS_TESTSUITE_IS_RUNNING_
	OS_extern BTestOK
	OS_extern BPanic
#endif

	OS_align(4, 2)

	/* Data label: cover the following OS_ASM_LABEL declarations */
	OS_ASM_DATALABEL
	
	/* Dummy reference to prevent the linker from dropping OS_kernel_ptr
	 * This is needed for ORTI on single-core derivatives. */
OS_ASM_LABEL(DummyRef)
	OS_word		OS_kernel_ptr

	/* The value for the system handler control and status register */
OS_ASM_LABEL(SHCSR_Value)
	OS_word 0x00070000

	/* The address of the system handler control and status register */
OS_ASM_LABEL(SHCSR_Addr)
	OS_word OS_CORTEXM_SHCSR

	/* Address of vector offset register */
OS_ASM_LABEL(VTOR_Addr)
	OS_word 0xE000ED08

	/* Watchdog (WDOG) CS and TOVAL registers with initial values */
OS_ASM_LABEL(WDOGCS_Addr)
	OS_word 0x40052000
OS_ASM_LABEL(WDOGCS_InitVal)
	OS_word 0x000021A0
OS_ASM_LABEL(WDOGTOVAL_Addr)
	OS_word 0x40052008
OS_ASM_LABEL(WDOGTOVAL_InitVal)
	OS_word 0x00000500

	/* Address of exception table */
OS_ASM_LABEL(ExceptionTableAddr)
	OS_word CORTEXMExceptionTable

OS_ASM_LABEL(BoardspInitial)
	OS_word (OS_kernStack0 + OS_ISTACKLEN_0)

/*
 * boardInvalidInterrupt()
 *
 * The boardInvalidInterrupt() routine is used for uninitialized
 * vectors of the vector table if error handling is set to minimal.
*/
	OS_ASM_FUNC(boardInvalidInterrupt)
OS_ASM_LABEL(boardInvalidInterrupt)
#ifdef OS_TESTSUITE_IS_RUNNING_
#ifdef TESTCASE_InvalidInterrupt
	bl		BTestOK
#else  /* TESTCASE_InvalidInterrupt */
	bl		BPanic
#endif /* TESTCASE_InvalidInterrupt */
#endif /* OS_TESTSUITE_IS_RUNNING_ */
OS_ASM_LABEL(InvalidInterruptLoop)
	b		InvalidInterruptLoop
	OS_ASM_END_FUNC(boardInvalidInterrupt)

/*
 * boardInvalidException()
 *
 * The boardInvalidException() routine is used for uninitialized
 * vectors of the exception table if Exception handling is off.
 *
*/
	OS_ASM_FUNC(boardInvalidException)
OS_ASM_LABEL(boardInvalidException)
#ifdef OS_TESTSUITE_IS_RUNNING_
#ifdef TESTCASE_InvalidException
	bl		BTestOK
#else  /* TESTCASE_InvalidException */
	bl		BPanic
#endif /* TESTCASE_InvalidException */
#endif /* OS_TESTSUITE_IS_RUNNING_ */
OS_ASM_LABEL(InvalidExceptionLoop)
	b		InvalidExceptionLoop
	OS_ASM_END_FUNC(boardInvalidException)

/*
 * boardResetStart
 *
 * Startup routine, for hardware initialization in assembly (which cannot be done in C).
 *
*/
	OS_ASM_FUNC(boardResetStart)
OS_ASM_LABEL(boardResetStart)
	/* After reset this routine is called
	 * - in privileged thread mode and
	 * - with sp == msp (sp_main) already set to reset value
	 * - with interrupt priority set to all enabled BASEPRI=0
	 * - with interrupt flag set to all enabled PRIMASK=0
	*/

	/* Global interrupt disable */
	cpsid	i

	/*	Set WDOG update flag to allow updates in the future.
	 *	must be done during the first 8'd128 bus clocks.
	 *	See chapter 21.4.3 - Configuring the Watchdog in S32K14x Reference Manual.
	 */
	ldr		r0, WDOGTOVAL_Addr
	ldr		r1, WDOGTOVAL_InitVal
	str		r1, [r0]
	ldr		r0, WDOGCS_Addr
	ldr		r1, WDOGCS_InitVal
	str		r1, [r0]

	/* set sp to top of kernel stack. */
	ldr		sp, BoardspInitial
	
	/* Set VTOR to base of vector table address. */
	ldr		r0, VTOR_Addr
	ldr		r1, ExceptionTableAddr
	str		r1, [r0]

	/* enable bus fault, mem manage fault and usage fault to invoke separate exceptions */
	ldr		r0, SHCSR_Value
	ldr		r5, SHCSR_Addr					/* system handler control and status register */
	ldr		r1, [r5]
	orr		r1, r0, r1
	str		r1, [r5]

	bl		BoardStart					/* call C routine, never returns */
	OS_ASM_END_FUNC(boardResetStart)

	
	OS_ASM_FUNC(boardDisableInterrupts)
OS_ASM_LABEL(boardDisableInterrupts)
	cpsid	i
	bx lr
OS_ASM_END_FUNC(boardDisableInterrupts)


	OS_ASM_FUNC(boardEnableInterrupts)
OS_ASM_LABEL(boardEnableInterrupts)
	cpsie	i
	bx lr
OS_ASM_END_FUNC(boardEnableInterrupts)


OS_ASM_SRC_END
	
/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
