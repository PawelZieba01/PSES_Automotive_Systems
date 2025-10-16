/* board.c - board specific code.
 *
 * This file contains board-specific startup code.
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: board.c 32046 2020-06-30 06:38:43Z hagr271648 $
*/

#include <board.h>
#include <Os.h>
#include <Os_tool.h>
#include <Os_kernel.h>

/* Static function declarations */
static void BoardDisableWatchdog(void);
static void BoardEnablePeripherals(void);
static void BoardSetupClocks(void);
static void BoardInitMemorySections(void);
static void BoardDisableMPU(void);

/* Watchdog */
static struct board_wdt_s * const board_wdt = (struct board_wdt_s *)BOARD_WDT_ADDR;

/* System Clock Generator (SCG) Control Registers Address */
static struct board_scg_ctrl_s * const board_scg_ctrl = (struct board_scg_ctrl_s *)SCG_CTRL_ADDR;

/* System Oscillator Control Status Register */
static struct board_scg_clk_s * const board_scg_sosc = (struct board_scg_clk_s *)SCG_SOSC_ADDR;

/* System PLL Control Status Register */
static struct board_scg_clk_s * const board_scg_pll = (struct board_scg_clk_s *)SCG_SPLL_ADDR;

/* Fast IRC Control Status Register */
static struct board_scg_clk_s * const board_scg_firc = (struct board_scg_clk_s *)SCG_FIRC_ADDR;

/* Pre-set meaningful colors for the LEDs */
static const os_uint32_t led_colors[] = {
		BOARD_LED_R,
		BOARD_LED_G,
		BOARD_LED_B,
		BOARD_LED_C,
		BOARD_LED_M,
		BOARD_LED_Y,
		BOARD_LED_W,
		BOARD_LED_K,
};

/* CHECK: RULE 401 OFF (linker generated symbols) */
/* linker generated symbol: begin of anonymous DATA section (load address) */
extern os_uint32_t OS_TOOL_INITDATA;
/* linker generated symbol: begin of anonymous DATA section (execution address) */
extern os_uint32_t OS_TOOL_STARTDATA;
/* linker generated symbol: end of anonymous DATA section (execution address) */
extern os_uint32_t OS_TOOL_ENDDATA;
/* linker generated symbol: start of BSS section (VMA) */
extern os_uint32_t OS_TOOL_STARTBSS;
/* linker generated symbol: end of BSS section (VMA) */
extern os_uint32_t OS_TOOL_ENDBSS;
/* CHECK: RULE 401 ON */

/* Initialize memory sections (data, bss). */
void BoardInitMemorySections(void)
{
	register os_uint32_t *ram;
	register os_uint32_t *flash;


	for	(ram = &OS_TOOL_STARTDATA, flash = &OS_TOOL_INITDATA;
		 ram < &OS_TOOL_ENDDATA;
		 ram++, flash++)
	{
		*ram = *flash;
	}
	for (ram = &OS_TOOL_STARTBSS;
		 ram < &OS_TOOL_ENDBSS;
		 ram++)
	{
		*ram = 0;
	}
}

/* Disables the watchdog. */
void BoardDisableWatchdog(void)
{
	/* unlock the WDOG registres */
	board_wdt->cnt = BOARD_WDT_UNLOCK_SEQ;
	/* we have 8'd128 bus clocks to set win, toval and cs */
	board_wdt->toval = BOARD_WDT_TOVAL;
	/* disable WDOG, keep updates allowed, use default clock, set 32-bit command words */
	board_wdt->cs = (BOARD_WDT_CS_UPD | BOARD_WDT_CS_CLK | BOARD_WDT_CS_CMD32EN);
}

/* Enable the peripherals in PCC. */
void BoardEnablePeripherals(void)
{
	os_reg32_t * const pcc_ptd = (os_reg32_t *)BOARD_PCC_PTD_ADDR;
	os_reg32_t * const pcc_ftm0 = (os_reg32_t *)BOARD_PCC_FTM0_ADDR;
	os_reg32_t * const pcc_ftm1 = (os_reg32_t *)BOARD_PCC_FTM1_ADDR;
	os_reg32_t * const pcc_ftm2 = (os_reg32_t *)BOARD_PCC_FTM2_ADDR;
	os_reg32_t * const pcc_ftm3 = (os_reg32_t *)BOARD_PCC_FTM3_ADDR;
#if BOARD_N_FTM > 4
	os_reg32_t * const pcc_ftm4 = (os_reg32_t *)BOARD_PCC_FTM4_ADDR;
#endif
#if BOARD_N_FTM > 5
	os_reg32_t * const pcc_ftm5 = (os_reg32_t *)BOARD_PCC_FTM5_ADDR;
#endif
#if BOARD_N_FTM > 6
	os_reg32_t * const pcc_ftm6 = (os_reg32_t *)BOARD_PCC_FTM6_ADDR;
#endif
#if BOARD_N_FTM > 7
	os_reg32_t * const pcc_ftm7 = (os_reg32_t *)BOARD_PCC_FTM7_ADDR;
#endif
	
	*pcc_ptd	= 0xC0000000; 	/* enable clock-gated portd for the LEDs */
	*pcc_ftm0 	= 0xC0000000;	/* enable clock-gated ftm0 */
	*pcc_ftm1 	= 0xC0000000;	/* enable clock-gated ftm1 */
	*pcc_ftm2	= 0xC0000000;	/* enable clock-gated ftm2 */
	*pcc_ftm3	= 0xC0000000;	/* enable clock-gated ftm3 */
#if BOARD_N_FTM > 4
	*pcc_ftm4	= 0xC0000000;	/* enable clock-gated ftm4 */
#endif
#if BOARD_N_FTM > 5
	*pcc_ftm5	= 0xC0000000;	/* enable clock-gated ftm5 */
#endif
#if BOARD_N_FTM > 6
	*pcc_ftm6	= 0xC0000000;	/* enable clock-gated ftm6 */
#endif
#if BOARD_N_FTM > 7
	*pcc_ftm7	= 0xC0000000;	/* enable clock-gated ftm7 */
#endif
}

void BoardSetupClocks(void)
{
	/* Setup clocks and power up PLL
	* Default configuration after reset is FIRC 48 MHz. This means that:
	* Fast Internal Reference Clock is the active system clock.
	* 
	* FIRC can only provide frequencies in the range of 48-60 MHz,
	* we need to switch to System PLL clock to run in
	* "Normal" (80 MHz) or "High-Speed" (112 MHz) modes.
	* 
	* --------------------------------------------------------
	* 				SLOW-RUN	NORMAL-RUN		HIGH-SPEED-RUN
	* CORE_CLK		48 MHz		80    MHz		112 MHz
	* SYS_CLK		48 MHz		80    MHz		112 MHz
	* BUS_CLK		48 MHz		40    MHz		56  MHz
	* FLASH_CLK		24 MHz		26,67 MHz		28  MHz
	* --------------------------------------------------------
	*
	* Selecting a different clock source when in RUN requires clock source
	* to be enabled first and be valid before system clocks switch to that
	* clock source. If system clock divide ratios also change when selecting
	* a different clock mode when in RUN, new system clock divide ratios
	* will not take affect until new clock source is valid.
	*/

	/* configure SOSC */
	board_scg_sosc->csr &= ~SCG_SOSCCSR_SOSCEN;	/* SOSC has to be disabled first */
	board_scg_sosc->cfg = SCG_SOSCCFG;
	board_scg_sosc->div = SCG_SOSCDIV;
	board_scg_sosc->csr = SCG_SOSCCSR;
	while ((board_scg_sosc->csr & SCG_SOSCCSR_SOSCVLD) == 0)
	{
		/* wait for SOSC clock to become ready. */
	}

	/* configure and enable SPLL */
	board_scg_pll->csr &= ~SCG_SPLLCSR_SPLLEN;
	board_scg_pll->cfg = SCG_SPLLCFG;
	board_scg_pll->div = SCG_SPLLDIV;
	board_scg_pll->csr = SCG_SPLLCSR;
	while ((board_scg_pll->csr & SCG_SPLLCSR_SPLLVLD) == 0)
	{
		/* wait for SPLL clock to become ready. */
	}
	
	/* set up SCG to use SPLL */
	board_scg_ctrl->rccr = SCG_RCCR_NR;
	
	/* set CLKOUT to SPLL */
	board_scg_ctrl->clkoutcnfg = SCG_CLKOUTCNFG;
	
	/* disable the FIRC clock */
	board_scg_firc->csr = 0x0u;
}

/* MPU should be disabled at startup.
*/
void BoardDisableMPU(void)
{
	os_nxpmpu_t* const os_mpu = (os_nxpmpu_t*)OS_NXPMPU_ADDR;
	os_uint32_t i;
	
	/* Disable MPU.
	*/
	os_mpu->cesr &= ~(OS_NXPMPU_ENABLE);

	/* Disable MPU regions 1 to 7.
	*/
	for (i = 1; i < OS_NXPMPU_RGD_N; ++i)
	{
		OS_NXPMPU_SetRegion(i, 0u, OS_NXPMPU_ALIGN_END(0u), 0u, 0u);
	}
}

/* Initialize the LEDs */
void BoardInitLeds(void)
{
	/*	Set operation mode of pins: 0,15,16 on Port-D to control RGB-leds by
	 *	setting multiplexer switch to 0x100u */
	os_reg32_t * const ptd_mxctl = (os_reg32_t *)BOARD_PORTD_MX_CTL_BASEADDR;
	ptd_mxctl[0]	= 0x100u;	/*	Enable Blue LED:	ptd0	*/
	ptd_mxctl[15]	= 0x100u;	/*	Enable Red LED:		ptd15	*/
	ptd_mxctl[16]	= 0x100u;	/*	Enable Green LED:	ptd16	*/
}

/* Make LEDs represent the lower three bits of status. */
void BoardProgramLeds(os_uint32_t status)
{
	os_reg32_t * const board_leds = (os_reg32_t *)BOARD_GPIO_REG_LEDS_ADDR;
	os_uint32_t index = (status & 0x7u);
	if (led_colors[index] != *board_leds)
	{
		*board_leds = led_colors[index];
	}
}

/*
 * BoardStart() - called from assembler
*/
void BoardStart(void)
{
	/* Note: You may only use the stack and rely on initialized data,
	 * after BoardInitMemorySections() was executed.
	*/
	BoardDisableWatchdog();
	BoardInitMemorySections();
	/* From here on you can rely on data and stack. */

	BoardEnablePeripherals();

	BoardSetupClocks();

#if (OS_CORTEXM_FPU != OS_CORTEXM_FPU_NONE)
	OS_CORTEXM_EnableVFPCoprocessor();
#endif

	BoardDisableMPU();

	boardEnableInterrupts();

	/* When all that is done, we can start the OS */
	main();

	/* NOT REACHED */
}

#if (OS_TOOL == OS_gnu)
/* To avoid drawing in the C-library, we need to provide a memcpy
 * function for gcc-generated code.
 * Probably, you may want to use a proper C-library instead.
*/
extern void *memcpy(void *, const void *, os_size_t);

#define unlikely(expr) __builtin_expect(!!(expr), 0)

void *memcpy(void *dest, const void *src, os_size_t n)
{
	os_size_t i;
	void * tmp = dest;

	/* check assumptions */
	if (unlikely(   (((os_uint32_t) dest & 3) != 0)
		         || (((os_uint32_t) src & 3) != 0)
		         || (((os_uint32_t) n & 3) != 0)
	   )        )
	{
		/* We need to use the slow version... */
		const os_uint8_t * s8 = (const os_uint8_t *) src;
		os_uint8_t * d8 = (os_uint8_t *) dest;

		for (i = 0; i < n; i++)
		{
			*d8++ = *s8++;
		}
	}
	else
	{
		/* Use unrolled 32-bit accesses */
		const os_uint32_t * s32 = (const os_uint32_t *) src;
		os_uint32_t * d32 = (os_uint32_t *) dest;

		n /= 4u;
		switch (n & 3)
		{
			case 3:
				*d32++ = *s32++;
			case 2:
				*d32++ = *s32++;
			case 1:
				*d32++ = *s32++;
			default:
				break;
		}
		n /= 4u;

		for (i = 0; i < n; i++)
		{
			*d32++ = *s32++;
			*d32++ = *s32++;
			*d32++ = *s32++;
			*d32++ = *s32++;
		}
	}

	return tmp;

}

void __aeabi_memcpy(void *dest, const void *src, os_size_t n)
{
	memcpy(dest, src, n);
}

void __aeabi_memcpy4(void *dest, const void *src, os_size_t n)
{
	memcpy(dest, src, n);
}

void __aeabi_memcpy8(void *dest, const void *src, os_size_t n)
{
	memcpy(dest, src, n);
}
#endif

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
