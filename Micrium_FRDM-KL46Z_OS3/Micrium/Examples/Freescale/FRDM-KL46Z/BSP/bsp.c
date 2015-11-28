/*
*********************************************************************************************************
*                                     MICIRUM BOARD SUPPORT PACKAGE
*
*                             (c) Copyright 2014; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                        BOARD SUPPORT PACKAGE
*
*                                        Freescale Kinetis KL40
*                                               on the
*                                        Freescale FRDM-KL46Z
*                                          Evaluation Board
*
* Filename      : bsp.c
* Version       : V1.00
* Programmer(s) : JPB
*                 SB
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define  BSP_MODULE
#include  <bsp.h>
#include  <bsp_lcd.h>
#include  <MKL46Z4.h>

#ifdef  BSP_CFG_OS
#include  <bsp_os.h>
#endif


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#define  BSP_CORE_DIV              (1u)
#define  BSP_BUS_DIV               (2u)
#define  BSP_FLEXBUS_DIV           (3u)
#define  BSP_FLASH_DIV             (6u)


/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

static  CPU_INT32U      BSP_CPU_ClkFreq_MHz;
static  BSP_TSI_SLIDER  BSP_TSI_Slider;


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  BSP_PLL_Init       (void);
static  void  BSP_IRC_Init       (void);
static  void  BSP_LED_Init       (void);
static  void  BSP_SW_Init        (void);
static  void  BSP_TSI_Init       (void);
static  void  BSP_TSI_SelfCal    (void);
static  void  BSP_TSI_IntHandler (void);
static  void  BSP_Timer_Init     (void);


/*
*********************************************************************************************************
*                                             REGISTERS
*********************************************************************************************************
*/

#define  DWT_CR      *(CPU_REG32 *)0xE0001000                   /* Data Watchpoint and Trace (DWT) Control Register.    */
#define  DBGMCU_CR   *(CPU_REG32 *)0xE0042004


/*
*********************************************************************************************************
*                                            REGISTER BITS
*********************************************************************************************************
*/

#define  DBGMCU_CR_TRACE_IOEN_MASK       0x10
#define  DBGMCU_CR_TRACE_MODE_ASYNC      0x00
#define  DBGMCU_CR_TRACE_MODE_SYNC_01    0x40
#define  DBGMCU_CR_TRACE_MODE_SYNC_02    0x80
#define  DBGMCU_CR_TRACE_MODE_SYNC_04    0xC0
#define  DBGMCU_CR_TRACE_MODE_MASK       0xC0


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/

#if ((CPU_CFG_TS_TMR_EN          != DEF_ENABLED) && \
     (APP_CFG_PROBE_OS_PLUGIN_EN == DEF_ENABLED) && \
     (OS_PROBE_HOOKS_EN          == 1))
#error  "CPU_CFG_TS_EN                  illegally #define'd in 'cpu.h'"
#error  "                              [MUST be  DEF_ENABLED] when    "
#error  "                               using uC/Probe COM modules    "
#endif


/*
*********************************************************************************************************
*                                               BSP_Init()
*
* Description : Initialize the Board Support Package (BSP).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) This function SHOULD be called before any other BSP function is called.
*
*               (2) CPU instruction / data tracing requires the use of the following pins :
*                   (a) (1) Aysynchronous     :  PB[3]
*                       (2) Synchronous 1-bit :  PE[3:2]
*                       (3) Synchronous 2-bit :  PE[4:2]
*                       (4) Synchronous 4-bit :  PE[6:2]
*
*                   (b) The application may wish to adjust the trace bus width depending on I/O
*                       requirements.
*********************************************************************************************************
*/

void  BSP_Init (void)
{
    BSP_IntInit();                                              /* Initialize Interrupts.                               */

    BSP_REG_SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK                  /* Enable all the port Clocks                           */
                      |   SIM_SCGC5_PORTB_MASK
                      |   SIM_SCGC5_PORTC_MASK
                      |   SIM_SCGC5_PORTD_MASK
                      |   SIM_SCGC5_PORTE_MASK);

#ifdef  BSP_CFG_OS
                                                                /* Init System tick times.                              */
                                                                /* Clear pending interrupt.                             */

    CPU_REG_NVIC_ST_RELOAD  = (2000000UL);                      /* Set period.                                          */
    CPU_REG_NVIC_ST_CTRL   |=  SysTick_CSR_CLKSOURCE_MASK;      /* Clock source - System Clock.                         */
    CPU_REG_NVIC_ST_CTRL   |=  SysTick_CSR_TICKINT_MASK;        /* Enable interrupt.                                    */
    CPU_REG_NVIC_ST_CTRL   |=  SysTick_CSR_ENABLE_MASK;         /* Start Sys Timer.                                     */
#endif


    BSP_CPU_ClkFreq_MHz = BSP_CPU_ClkFreq() / (CPU_INT32U)1000000;

    BSP_CPU_ClkFreq_MHz = BSP_CPU_ClkFreq_MHz;                  /* Surpress compiler warning BSP_CPU_ClkFreq_MHz.       */

    BSP_PLL_Init();

    BSP_LED_Init();                                             /* Initialize the I/Os for the LEDs.                    */

    BSP_SW_Init();                                              /* Initialize the I/Os for the switches.                */

    BSP_TSI_Init();                                             /* Initialize the TSI module.                           */

    BSP_IRC_Init();                                             /* Initialize the High-Speed Internal Reference Clock.  */

    BSP_Timer_Init();                                           /* Initialize the Low-Power timer.                      */

#ifdef TRACE_EN                                                 /* See project / compiler preprocessor options.         */
    DBGMCU_CR |=  DBGMCU_CR_TRACE_IOEN_MASK;                    /* Enable tracing (see Note #2).                        */
    DBGMCU_CR &= ~DBGMCU_CR_TRACE_MODE_MASK;                    /* Clr trace mode sel bits.                             */
    DBGMCU_CR |=  DBGMCU_CR_TRACE_MODE_SYNC_04;                 /* Cfg trace mode to synch 4-bit.                       */
#endif
}


/*
*********************************************************************************************************
*                                            BSP_CPU_ClkFreq()
*
* Description : Read CPU registers to determine the CPU clock frequency of the chip.
*
* Argument(s) : none.
*
* Return(s)   : The CPU clock frequency, in Hz.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_INT32U  BSP_CPU_ClkFreq (void)
{
  return ((CPU_INT32U)48000000UL);                              /* The board runs the CPU at 48MHz.                     */
}


/*
*********************************************************************************************************
*                                            BSP_Tick_Init()
*
* Description : Initialize all the peripherals that required OS Tick services (OS initialized)
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

#ifdef  BSP_CFG_OS
void BSP_Tick_Init (void)
{
    CPU_INT32U  cpu_clk_freq;
    CPU_INT32U  cnts;


    cpu_clk_freq = BSP_CPU_ClkFreq();                           /* Determine SysTick reference freq.                    */

#if (OS_VERSION >= 30000u)
    cnts  = cpu_clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;       /* Determine nbr SysTick increments.                    */
#else
    cnts  = cpu_clk_freq / (CPU_INT32U)OS_TICKS_PER_SEC;        /* Determine nbr SysTick increments.                    */
#endif

    OS_CPU_SysTickInit(cnts);                                   /* Init uC/OS periodic time src (SysTick).              */
}
#endif


/*
*********************************************************************************************************
*                                         PLL INITIALIZATION
*
* Description : This function is called to initialize the PLL.
*
* Arguments   : none
*********************************************************************************************************
*/

static  void  BSP_PLL_Init (void)
{
    CPU_REG32   reg_val;
    CPU_SR_ALLOC();


    CPU_CRITICAL_ENTER();

                                                                /* System clock divider configuration.                  */
    BSP_REG_SIM_CLKDIV1 = (BSP_SIM_CLKDIV1_OUTDIV1_DIV2 |       /* Clock 1 divide by 2.                                 */
                           BSP_SIM_CLKDIV1_OUTDIV4_DIV2);       /* Clock 4 divide by 2.                                 */

                                                                /* Multipurpose clock generator configuration.          */
    BSP_REG_MCG_C2 = (BSP_MCG_C2_LOCRE0          |              /* Generate a reset request on a loss of ext ref clk.   */
                      BSP_MCG_C2_RANGE_HIGH_FREQ |              /* Select high freq range for the crystal oscillator.   */
                      BSP_MCG_C2_EREFS0);                       /* Select oscillator as the source for the ext ref clk. */

    BSP_REG_MCG_C1 = (BSP_MCG_C1_CLKS_EXT_CLK    |              /* Selects the ext ref clk as the clk source for MCG.   */
                      BSP_MCG_C1_FRDIV_8);                      /* Divide the ext ref clk by 256 for the FLL.           */

    while (DEF_BIT_IS_SET(BSP_REG_MCG_S, BSP_MCG_S_IREFST)) {   /* Wait for Reference clock Status bit to clear.        */
        ;
    }

    do {                                                        /* Wait for clock status bits to show clock.            */
        reg_val = (BSP_REG_MCG_S & BSP_MCG_S_CLKST_MASK) >> 2u; /* source is ext ref clk.                               */
    } while (reg_val != 0x2);


                                                                /* --------------------- PLL0 CFG --------------------- */
    BSP_REG_MCG_C5  = BSP_MCG_C5_PRDIV0_4;                      /* Select PLL0 external reference divider.              */

    BSP_REG_MCG_C6 = (BSP_MCG_C6_PLLS    |                      /* Select the PLL output.                               */
                      BSP_MCG_C6_CME0    |                      /* Enable the loss of clock monitoring circuit.         */
                      BSP_MCG_C6_VDIV0_MUL_48);


    while (DEF_BIT_IS_CLR(BSP_REG_MCG_S, BSP_MCG_S_PLLST)) {    /* Wait for PLL status bit to set.                      */
        ;
    }

    while (DEF_BIT_IS_CLR(BSP_REG_MCG_S, BSP_MCG_S_LOCK0)) {    /* Wait for LOCK bit to set.                            */
        ;
    }

    DEF_BIT_CLR(BSP_REG_MCG_C1, BSP_MCG_C1_CLKS_MASK);          /* Clear CLKS to switch CLKS mux to PLL as MCG_OUT.     */

    do {
        reg_val = (BSP_REG_MCG_S & BSP_MCG_S_CLKST_MASK) >> 2u;
    } while (reg_val != 0x3);                                   /* Wait for clock status bits to update.                */


    SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;                      /* Set PLLFLLSEL to select the PLL for this clk src.    */

    CPU_CRITICAL_EXIT();
}



/*
*********************************************************************************************************
*                                             BSP_IRC_Init()
*
* Description : Initialize the High-Speed Internal Reference Clock (IRC).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_Init().
*
* Note(s)     : (1) This function starts the High-Speed IRC which is used to clock certain peripherals.
*                   The IRC is set to 4 MHz and should not be changed.
*********************************************************************************************************
*/

static  void  BSP_IRC_Init (void)
{
    BSP_REG_MCG_SC    &= ~MCG_SC_FCRDIV_MASK;                   /* MCGIRCLK = (4 MHz / 1)                               */

    BSP_REG_MCG_C2    |=  MCG_C2_IRCS_MASK;                     /* Select Fast internal reference clock (4 MHz IRC).    */

    BSP_REG_MCG_C1    |=  MCG_C1_IRCLKEN_MASK;                  /* Enable the internal reference clock for MCGIRCLK.    */

    while (DEF_BIT_IS_CLR(BSP_REG_MCG_S, MCG_S_IRCST_MASK)) {   /* Wait for IRC source to switch.                       */
        ;
    }
}


/*
*********************************************************************************************************
*********************************************************************************************************
*                                              LED FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            BSP_LED_Init()
*
* Description : Initialize the I/O for the LEDs.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  BSP_LED_Init (void)
{
                                                                /* Green LED.                                           */
    PORTD_PCR5   = (0|PORT_PCR_MUX(1));                         /* Alternative 1 (GPIO).                                */

                                                                /* Red LED.                                             */
    PORTE_PCR29  = (0|PORT_PCR_MUX(1));                         /* Alternative 1 (GPIO).                                */

                                                                /* Configure GPIO pins for output.                      */
    GPIOD_PDDR  |=  BSP_GPIOD_LED_GREEN;
    GPIOE_PDDR  |=  BSP_GPIOE_LED_RED;

    BSP_LED_Off(BSP_LED_ALL);
}

/*
*********************************************************************************************************
*                                             BSP_LED_On()
*
* Description : Turn on LEDs.
*
* Argument(s) : led             the led you want to control:
*                               BSP_LED_ALL     Turn on all LEDs
*                               BSP_LED_GREEN   Turn on the Green LED
*                               BSP_LED_RED     Turn on the RED LED
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_LED_On (CPU_INT08U led)
{
    switch(led) {
        case BSP_LED_GREEN:
             GPIOD_PCOR  |=  BSP_GPIOD_LED_GREEN;
             break;


        case BSP_LED_RED:
             GPIOE_PCOR  |=  BSP_GPIOE_LED_RED;
             break;


        case BSP_LED_ALL:
             GPIOD_PCOR  |=  BSP_GPIOD_LED_GREEN;
             GPIOE_PCOR  |=  BSP_GPIOE_LED_RED;
             break;


        default:
             return;
    }
}


/*
*********************************************************************************************************
*                                            BSP_LED_Off()
*
* Description : Turn off LEDs.
*
* Argument(s) : led             the led you want to control:
*                               BSP_LED_ALL     Turn off all LEDs
*                               BSP_LED_GREEN   Turn off the Green LED
*                               BSP_LED_RED     Turn off the RED LED
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_LED_Off (CPU_INT08U led)
{
    switch(led) {
        case BSP_LED_GREEN:
             GPIOD_PSOR  |=  BSP_GPIOD_LED_GREEN;
             break;


        case BSP_LED_RED:
             GPIOE_PSOR  |=  BSP_GPIOE_LED_RED;
             break;


        case BSP_LED_ALL:
             GPIOD_PSOR  |=  BSP_GPIOD_LED_GREEN;
             GPIOE_PSOR  |=  BSP_GPIOE_LED_RED;
             break;


        default:
             return;
    }
}


/*
*********************************************************************************************************
*                                           BSP_LED_Toggle()
*
* Description : Toggle LEDs.
*
* Argument(s) : led             the led you want to control:
*                               BSP_LED_ALL     Toggle all LEDs
*                               BSP_LED_GREEN   Toggle the Green LED
*                               BSP_LED_RED     Toggle the RED LED
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_LED_Toggle (CPU_INT08U led)
{
    switch(led) {
        case BSP_LED_GREEN:
             GPIOD_PTOR  |=  BSP_GPIOD_LED_GREEN;
             break;


        case BSP_LED_RED:
             GPIOE_PTOR  |=  BSP_GPIOE_LED_RED;
             break;


        case BSP_LED_ALL:
             GPIOD_PTOR  |=  BSP_GPIOD_LED_GREEN;
             GPIOE_PTOR  |=  BSP_GPIOE_LED_RED;
             break;


        default:
             return;
    }
}


/*
*********************************************************************************************************
*********************************************************************************************************
*                                              SW FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            BSP_SW_Init()
*
* Description : Initialize the I/O for the switches.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  BSP_SW_Init (void)
{
                                                                /* Switch 1                                             */
    PORTC_PCR3   = (PORT_PCR_MUX(1)  |                          /* Alternative 1 (GPIO).                                */
                    PORT_PCR_PE_MASK |                          /* Enable pullup resistor on the pin.                   */
                    PORT_PCR_PS_MASK);
                                                                /* Switch 2                                             */
    PORTC_PCR12  = (PORT_PCR_MUX(1)  |                          /* Alternative 1 (GPIO).                                */
                    PORT_PCR_PE_MASK |                          /* Enable pullup resistor on the pin.                   */
                    PORT_PCR_PS_MASK);

                                                                /* Configure GPIO pins for input.                       */
    GPIOC_PDDR  &=  ~BSP_GPIOC_SW_1;
    GPIOC_PDDR  &=  ~BSP_GPIOC_SW_2;
}


/*
*********************************************************************************************************
*                                            BSP_SW_Read()
*
* Description : Read
*
* Argument(s) : sw              the switch you want to read from:
*                               BSP_SW_1        Read switch 1
*                               BSP_SW_2        Read switch 2
*
* Return(s)   : DEF_ON          if sw is closed.
*
*               DEF_OFF         otherwise.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_INT08U  BSP_SW_Read (CPU_INT08S  sw)
{
    switch(sw) {
        case BSP_SW_1:
             if (DEF_BIT_IS_CLR(GPIOC_PDIR, BSP_GPIOC_SW_1)) {
                 return DEF_ON;
             }
             break;


        case BSP_SW_2:
             if (DEF_BIT_IS_CLR(GPIOC_PDIR, BSP_GPIOC_SW_2)) {
                 return DEF_ON;
             }
             break;

        default:
             break;
    }

    return  DEF_OFF;
}


/*
*********************************************************************************************************
*********************************************************************************************************
*                                          TSI MODULE FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             BSP_TSI_Init()
*
* Description : Initialize the TSI (Touch Sense Input) module.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  BSP_TSI_Init (void)
{
    BSP_TSI_Slider.EndFlg = DEF_FALSE;                          /* Initialize global variables.                         */

    BSP_TSI_Slider.Electrodes[0].PinNbr    = BSP_TSI_PIN9;
    BSP_TSI_Slider.Electrodes[1].PinNbr    = BSP_TSI_PIN10;

    BSP_TSI_Slider.Electrodes[0].Threshold = BSP_TSI_THRESHOLD;
    BSP_TSI_Slider.Electrodes[1].Threshold = BSP_TSI_THRESHOLD;

    SIM_SCGC5   |= SIM_SCGC5_TSI_MASK;                          /* Enable clock gating for TSI.                         */

                                                                /* Enable proper GPIO as TSI channels.                  */
    PORTB_PCR16  = PORT_PCR_MUX(0);                             /* PTB16 as TSI channel 9.                              */
    PORTB_PCR17  = PORT_PCR_MUX(0);                             /* PTB17 as TSI channel 10.                             */

    BSP_IntVectSet(BSP_INT_ID_TSI0, BSP_TSI_IntHandler);        /* Register the interrupt handler.                      */
    BSP_IntEn(BSP_INT_ID_TSI0);                                 /* Enable the TSI module interrupts.                    */

    TSI0_GENCS  |= (TSI_GENCS_ESOR_MASK
                |   TSI_GENCS_MODE(0)
                |   TSI_GENCS_REFCHRG(4)
                |   TSI_GENCS_DVOLT(0)
                |   TSI_GENCS_EXTCHRG(7)
                |   TSI_GENCS_PS(4)
                |   TSI_GENCS_NSCN(11)
                |   TSI_GENCS_TSIIEN_MASK
                |   TSI_GENCS_STPE_MASK);

    TSI0_GENCS  |=  TSI_GENCS_TSIEN_MASK;                       /* Enable the TSI module.                               */

    BSP_TSI_SelfCal();                                          /* Self calibration.                                    */
}


/*
*********************************************************************************************************
*                                             BSP_TSI_SelfCal()
*
* Description : Calibrates the TSI (Touch Sense Input) module.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_TSI_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  BSP_TSI_SelfCal (void)
{
    CPU_INT08U  cnt;
    CPU_INT08U  trigger_backup;


    TSI0_GENCS |=  TSI_GENCS_EOSF_MASK;                         /* Clear End of Scan Flag.                              */
    TSI0_GENCS &= ~TSI_GENCS_TSIEN_MASK;                        /* Disable TSI module.                                  */

    if (TSI0_GENCS & TSI_GENCS_STM_MASK) {
        trigger_backup = DEF_TRUE;                              /* Back-up TSI Trigger.                                 */
    } else {
        trigger_backup = DEF_FALSE;
    }

    TSI0_GENCS &= ~TSI_GENCS_STM_MASK;                          /* Use SW trigger.                                      */
    TSI0_GENCS &= ~TSI_GENCS_TSIIEN_MASK;                       /* Enable TSI interrupts.                               */

    TSI0_GENCS |=  TSI_GENCS_TSIEN_MASK;                        /* Enable TSI module.                                   */

    for (cnt = 0; cnt < BSP_TSI_MAX_ELECTRODES; cnt++) {        /* Get reading when the Electrode is not being touched. */
        TSI0_DATA  = ((BSP_TSI_Slider.Electrodes[cnt].PinNbr << TSI_DATA_TSICH_SHIFT));
        TSI0_DATA |= TSI_DATA_SWTS_MASK;
        while(!(TSI0_GENCS & TSI_GENCS_EOSF_MASK));
        TSI0_GENCS |= TSI_GENCS_EOSF_MASK;
        BSP_TSI_Slider.Electrodes[cnt].Baseline = (TSI0_DATA & TSI_DATA_TSICNT_MASK);
    }

    TSI0_GENCS &= ~TSI_GENCS_TSIEN_MASK;                        /* Disable TSI module.                                  */
    TSI0_GENCS |=  TSI_GENCS_TSIIEN_MASK;                       /* Enale TSI interrupt.                                 */

    if (trigger_backup) {                                       /* Restore trigger mode.                                */
        TSI0_GENCS |=  TSI_GENCS_STM_MASK;
    } else {
        TSI0_GENCS &= ~TSI_GENCS_STM_MASK;
    }

    TSI0_GENCS |= TSI_GENCS_TSIEN_MASK;                         /* Enable TSI module.                                   */

    TSI0_DATA   = ((BSP_TSI_Slider.Electrodes[cnt].PinNbr << TSI_DATA_TSICH_SHIFT));
    TSI0_DATA  |= TSI_DATA_SWTS_MASK;
}


/*
*********************************************************************************************************
*                                             BSP_TSI_SliderRead()
*
* Description : Reads the TSI (Touch Sense Input) module.
*
* Argument(s) : Pointers to the application variables to store the slider position
*               in terms of percentage and millimeters.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void BSP_TSI_SliderRead (CPU_INT08U *slider_pos_mm, CPU_INT08U *slider_pos_pct)
{
    if (BSP_TSI_Slider.EndFlg == DEF_TRUE) {

        BSP_TSI_Slider.EndFlg = DEF_FALSE;

        if ((BSP_TSI_Slider.Electrodes[0].Delta > BSP_TSI_Slider.Electrodes[0].Threshold) ||
            (BSP_TSI_Slider.Electrodes[1].Delta > BSP_TSI_Slider.Electrodes[1].Threshold)) {

            BSP_TSI_Slider.Electrodes[0].PosPct = (BSP_TSI_Slider.Electrodes[0].Delta * 100) /
                                                  (BSP_TSI_Slider.Electrodes[0].Delta +
                                                   BSP_TSI_Slider.Electrodes[1].Delta);

            BSP_TSI_Slider.Electrodes[1].PosPct = (BSP_TSI_Slider.Electrodes[1].Delta * 100) /
                                                  (BSP_TSI_Slider.Electrodes[0].Delta +
                                                   BSP_TSI_Slider.Electrodes[1].Delta);

            BSP_TSI_Slider.Electrodes[0].PosMm  = (BSP_TSI_Slider.Electrodes[0].PosPct *
                                                   BSP_TSI_SLIDER_LEN) / 100;

            BSP_TSI_Slider.Electrodes[1].PosMm  = (BSP_TSI_Slider.Electrodes[1].PosPct *
                                                   BSP_TSI_SLIDER_LEN) / 100;

            *slider_pos_pct = ((100 - BSP_TSI_Slider.Electrodes[0].PosPct) +
                                      BSP_TSI_Slider.Electrodes[1].PosPct) / 2;

            *slider_pos_mm  = ((BSP_TSI_SLIDER_LEN - BSP_TSI_Slider.Electrodes[0].PosMm) +
                                                     BSP_TSI_Slider.Electrodes[1].PosMm) / 2;

         } else {
            BSP_TSI_Slider.Electrodes[0].PosPct = BSP_TSI_NO_TOUCH;
            BSP_TSI_Slider.Electrodes[1].PosPct = BSP_TSI_NO_TOUCH;
            BSP_TSI_Slider.Electrodes[0].PosMm  = BSP_TSI_NO_TOUCH;
            BSP_TSI_Slider.Electrodes[1].PosMm  = BSP_TSI_NO_TOUCH;
            *slider_pos_pct                     = BSP_TSI_NO_TOUCH;
            *slider_pos_mm                      = BSP_TSI_NO_TOUCH;
         }
    }
}


/*
*********************************************************************************************************
*                                        BSP_TSI_IntHandler()
*
* Description : TSI interrupt handler.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_IntHandler().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  BSP_TSI_IntHandler (void)
{
    CPU_INT16S  delta;


    BSP_TSI_Slider.EndFlg = DEF_TRUE;

    TSI0_GENCS |= TSI_GENCS_EOSF_MASK;                          /* Clear End of Scan Flag.                              */

                                                                /* Save Counts for current electrode.                   */
    BSP_TSI_Slider.Electrodes[BSP_TSI_Slider.CurElectrode].RawCnt = (TSI0_DATA & TSI_DATA_TSICNT_MASK);

                                                                /* Obtains Counts Delta from callibration reference.    */
    delta = BSP_TSI_Slider.Electrodes[BSP_TSI_Slider.CurElectrode].RawCnt -
            BSP_TSI_Slider.Electrodes[BSP_TSI_Slider.CurElectrode].Baseline;

    if (delta < 0) {
        BSP_TSI_Slider.Electrodes[BSP_TSI_Slider.CurElectrode].Delta = 0;
    } else {
        BSP_TSI_Slider.Electrodes[BSP_TSI_Slider.CurElectrode].Delta = delta;
    }

    if (BSP_TSI_MAX_ELECTRODES > 1)  {                          /* Change to the next electrode to scan.                */
        if ((BSP_TSI_MAX_ELECTRODES - 1) > BSP_TSI_Slider.CurElectrode) {
            BSP_TSI_Slider.CurElectrode++;
        } else {
            BSP_TSI_Slider.CurElectrode = 0;
        }
        TSI0_DATA  = ((BSP_TSI_Slider.Electrodes[BSP_TSI_Slider.CurElectrode].PinNbr << TSI_DATA_TSICH_SHIFT));
        TSI0_DATA |= TSI_DATA_SWTS_MASK;
    }
}

/*
*********************************************************************************************************
*********************************************************************************************************
*                                            TIMER FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                           BSP_Timer_Init()
*
* Description : Initialize the Low-Power Timer (LPTMR0).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  BSP_Timer_Init (void)
{
    SIM_SCGC5 |=  SIM_SCGC5_LPTMR_MASK;                         /* Enable the clock for LPTMR0.                         */

    BSP_REG_LPTMR0_CSR =  0x00000000u;                          /* Disable LPTMR0.                                      */

    BSP_REG_LPTMR0_PSR = (LPTMR_PSR_PRESCALE(0x1u) |            /* Select (MCGIRCLK / 4) as the timer clock.            */
                          LPTMR_PSR_PCS(0x0u));

    BSP_REG_LPTMR0_CSR =  0x00000005u;                          /* Enable LPTMR0 in Free Running Counter mode.          */
}


/*
*********************************************************************************************************
*                                            BSP_Timer_Dly()
*
* Description : Delay CPU for the specified time.
*
* Argument(s) : ms          number of milliseconds to delay
*
*               us          number of microseconds to delay
*
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_Timer_Dly (CPU_INT16U  ms,
                     CPU_INT16U  us)
{
    CPU_INT32U  ticks;

    ticks = (ms * 1000u) + us;

    while (ticks > LPTMR_CNR_COUNTER_MASK) {
        BSP_REG_LPTMR0_CNR  =  0x00000000u;                     /* Register current timer value into the temporary reg. */
        BSP_REG_LPTMR0_CMR  =  BSP_REG_LPTMR0_CNR + LPTMR_CNR_COUNTER_MASK;
        BSP_REG_LPTMR0_CSR |=  LPTMR_CSR_TCF_MASK;

        while (DEF_BIT_IS_CLR(BSP_REG_LPTMR0_CSR, LPTMR_CSR_TCF_MASK)) {
            ;
        }

        ticks -=  LPTMR_CNR_COUNTER_MASK;
    }

    if (ticks > 0) {
        BSP_REG_LPTMR0_CNR  =  0x00000000u;
        BSP_REG_LPTMR0_CMR  =  BSP_REG_LPTMR0_CNR + ticks;
        BSP_REG_LPTMR0_CSR |=  LPTMR_CSR_TCF_MASK;

        while (DEF_BIT_IS_CLR(BSP_REG_LPTMR0_CSR, LPTMR_CSR_TCF_MASK)) {
            ;
        }
    }
}
