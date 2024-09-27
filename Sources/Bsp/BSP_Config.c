#include <BSP_Config.h>

/***************************************************
 * disable the known warnings in this file
 **************************************************/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-sign"


/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @addtogroup macro_group
 * @{
 */

#define WAIT_AML_GET_CYCLES_FOR_MS(ms, freq)  ((((uint32_t)freq) / 1000U) * (ms))            /*!< Gets needed cycles for specified delay in milliseconds, calculation is based on core clock frequency. */
#define WAIT_AML_GET_CYCLES_FOR_US(us, freq)  ((((uint32_t)freq) / 1000U) * (us) / 1000U)    /*!< Gets needed cycles for specified delay in microseconds, calculation is based on core clock frequency. */
#define WAIT_AML_GET_CYCLES_FOR_NS(ns, freq)  ((((uint32_t)freq) / 1000000U) * (ns) / 1000U) /*!< Gets needed cycles for specified delay in nanoseconds, calculation is based on core clock frequency. */
/*! @} */


#if defined(__thumb__) && !defined(__thumb2__) /* Thumb instruction set only */
/*!
 * @brief Waits for exact number of cycles which can be expressed as multiple of 4.
 *
 * MOV - 1 cycle
 * SUB - 1 cycle
 * BNE - 1 cycle or 2 cycles if jump is realized
 *
 * Output list (empty) - which registers are output and how to map them to C code.
 * Input list (Cycles) - which registers are input and how to map them to C code.
 * Clobber list (r0, r1, cc) - which registers might have changed during
 * execution of asm code (compiler will have to reload them).
 *
 * @param Cycles | Number of cycles to wait.
 */
#define WAIT_AML_WAIT_FOR_MUL4_CYCLES(cycles) \
  __asm( \
    "mov r0, %[cycles] \n\t" \
    "0: \n\t"                \
      "sub r0, #4 \n\t"      \
      "nop \n\t"             \
    "bne 0b \n\t"            \
     :                       \
     : [cycles] "r" (cycles) \
     : "r0", "r1", "cc"      \
  )

#else /* Thumb2 or A32 instruction set */

/*!
 * @brief Waits for exact number of cycles which can be expressed as multiple of 4.
 *
 * @param Cycles | Number of cycles to wait.
 */
#define WAIT_AML_WAIT_FOR_MUL4_CYCLES(cycles) \
  __asm( \
    "movs r0, %[cycles] \n"  \
    "0: \n"                  \
      "subs r0, r0, #4 \n"   \
      "nop \n\t"             \
    "bne 0b \n"              \
     :                       \
     : [cycles] "r" (cycles) \
     : "r0", "r1", "cc"      \
  )

#endif

 mutex_t UART_Print_Mutex;/*the mutex for UART print peripheral lock*/


 /*FUNCTION**********************************************************************
  *
  * Function Name : WAIT_AML_WaitCycles
  * Description   : Waits for specified amount of cycles which is given by 32bit
  *                 value range. Assumption for this function is that target
  *                 architecture is using 32bit general purpose registers.
  *
  *END**************************************************************************/
 void CPU_DealyCycles(uint32_t cycles)
 {
     /* Advance to next multiple of 4. Value 0x04U ensures that the number
      * is not zero. */
     cycles = (cycles & 0xFFFFFFFCU) | 0x04U;

     WAIT_AML_WAIT_FOR_MUL4_CYCLES(cycles);
 }

 /*FUNCTION**********************************************************************
  *
  * Function Name : WAIT_AML_WaitSec
  * Description   : Waits for specified amount of seconds.
  *
  *END**************************************************************************/
 void CPU_DealySec(uint16_t delay)
 {
     for (; delay > 0U; delay--)
     {
    	 CPU_DealyMs(1000U);
     }
 }

 /*FUNCTION**********************************************************************
  *
  * Function Name : WAIT_AML_WaitMs
  * Description   : Waits for specified amount of milliseconds.
  *
  *END**************************************************************************/
 void CPU_DealyMs(uint16_t delay)
 {
	 uint32_t freq,cycles;

	 CLOCK_SYS_GetFreq(CORE_CLK, &freq);/*get the core clock*/

	 cycles = (uint32_t) WAIT_AML_GET_CYCLES_FOR_MS(delay, freq );

     /* Advance to multiple of 4. */
     cycles = cycles & 0xFFFFFFFCU;

     WAIT_AML_WAIT_FOR_MUL4_CYCLES(cycles);

 }

 /*FUNCTION**********************************************************************
  *
  * Function Name : WAIT_AML_WaitUs
  * Description   : Waits for specified amount of microseconds.
  *
  *END**************************************************************************/
 void CPU_DealyUs(uint16_t delay)
 {
	 uint32_t freq,cycles;

	 CLOCK_SYS_GetFreq(CORE_CLK, &freq); /*get the core clock*/

	 cycles = (uint32_t) WAIT_AML_GET_CYCLES_FOR_US(delay, freq );

     /* Advance to next multiple of 4. Value 0x04U ensures that the number
      * is not zero. */
     cycles = (cycles & 0xFFFFFFFCU) | 0x04U;
     WAIT_AML_WAIT_FOR_MUL4_CYCLES(cycles);
 }


/*******************************************************************************
*
* Function: 	void McuClockConfig(void)
*
* Description:  This function installs the pre-defined clock configuration table
* 				to the clock manager. For more details see configuration
* 				in Processor Expert.
*
*******************************************************************************/
void McuClockConfig(void)
{
	/* Clock configuration for MCU and MCU's peripherals */
	CLOCK_SYS_Init(g_clockManConfigsArr,
	               CLOCK_MANAGER_CONFIG_CNT,
	               g_clockManCallbacksArr,
	               CLOCK_MANAGER_CALLBACK_CNT);

	/* Clock configuration update */
	CLOCK_SYS_UpdateConfiguration(0, CLOCK_MANAGER_POLICY_FORCIBLE);
}

/*******************************************************************************************
* basic clock and pin function initialization
* it should be called before any other MCU peripheral initialization and task creation
*******************************************************************************************/

void MCU_SystemInit(void)
{
	/*
	* initialize the system clock(SPLL, SysCLK, CoreCLK) and peripheral clock(function clock and clock gate enable) as clock_manager component configured
	*/
	CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT, g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
	CLOCK_SYS_UpdateConfiguration(0,CLOCK_MANAGER_POLICY_AGREEMENT);
	/*
	* configure the PORT and GPIO(PIN) function as Pin_Settings component configured:
	* interrupt, filter, direction, and other properties(.eg Pull up/down/enable, drive strength, etc. )
	*/
	PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);

	uSoftwarePowerControl(1);
	/*
	 * initialize the MCU power mode and switch to RUN mode
	 */
//	McuPowerConfig();
	/*
	 * enable the CPU cache to ensure the max. CPU performance
	 */
//	McuCacheConfig();
}

/*******************************************************************************
*
* Function: 	void McuPowerConfig(void)
*
* Description:  This function configures the Power manager for operation.
* 				For more details see configuration in Processor Expert.
*
*******************************************************************************/
void McuPowerConfig(void)
{
	/* initialize the MCU power mode configuration */
	POWER_SYS_Init(&powerConfigsArr, 2, &powerStaticCallbacksConfigsArr,0);
	/* Set MCU power mode to RUN mode */
	POWER_SYS_SetMode(HSRUN,POWER_MANAGER_POLICY_AGREEMENT);
}

/*******************************************************************************
*
* Function: 	void McuIntConfig(void)
*
* Description:  Enables an interrupt for a given IRQ number.
* 				For more details see configuration in Processor Expert.
*
*******************************************************************************/
void McuIntConfig(void)
{
	INT_SYS_EnableIRQ(WDOG_EWM_IRQn);
	INT_SYS_EnableIRQ(DMA1_IRQn);	//DMA channel 1 interrupt enabled for CRC data calculation
}

/*******************************************************************************
*
* Function: 	void McuSimConfig(void)
*
* Description:  This function configures SIM module
*
*******************************************************************************/
void McuSimConfig(void)
{
	SIM->CHIPCTL &= ~SIM_CHIPCTL_CLKOUTEN_MASK;
	SIM->CHIPCTL &= ~SIM_CHIPCTL_CLKOUTSEL_MASK;

	SIM->CHIPCTL |= ~SIM_CHIPCTL_CLKOUTSEL(SLOW_CLK);
	SIM->CHIPCTL |= SIM_CHIPCTL_CLKOUTSEL_MASK;
}


/*******************************************************************************
*
* Function: 	void McuCacheConfig(void)
*
* Description:  This function enables Cache memory
*
*******************************************************************************/
void McuCacheConfig(void)
{
    // Enable Cache !
    // Flush and enable I cache and write buffer
    LMEM->PCCCR = LMEM_PCCCR_ENCACHE_MASK | LMEM_PCCCR_INVW1_MASK
    		    | LMEM_PCCCR_INVW0_MASK   | LMEM_PCCCR_GO_MASK;
}

/*******************************************************************************
*
* Function: 	void McuDmaConfig(void)
*
* Description:  This function eDMA module
* Notice: user needs to enable the exact DMA channel for some peripheral driver in app
*
*******************************************************************************/
void McuDmaConfig(void)
{
	EDMA_DRV_Init(&dmaController1_State, &dmaController1_InitConfig0, \
			edmaChnStateArray, edmaChnConfigArray, EDMA_CONFIGURED_CHANNELS_COUNT);

}

/*******************************************************************************
*
* Function: 	void McuFlashPrefetchConfig(void)
*
* Description:  This function disable the Flash data and instruction prefetch
* Notice: this is necessary for Flash program option
*
*******************************************************************************/
void McuFlashPrefetchConfig(void)
{
    /* Disable cache to ensure that all flash operations will take effect instantly,
     * this is device dependent and necessary for Flash program option*/
#ifdef S32K144_SERIES
    MSCM->OCMDR[0u] |= MSCM_OCMDR_OCM1(0xFu);
    MSCM->OCMDR[1u] |= MSCM_OCMDR_OCM1(0xFu);
    MSCM->OCMDR[2u] |= MSCM_OCMDR_OCM1(0xFu);
#endif /* S32K144_SERIES */
}
/* End of file */
