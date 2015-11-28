/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                              (c) Copyright 2014; Micrium, Inc.; Weston, FL
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
*                                            STARTUP CODE
*
*                                        Freescale Kinetis KL40
*                                               on the
*                                        Freescale FRDM-KL46Z
*                                          Evaluation Board
*
* Filename      : cstartup.c
* Version       : V1.00
* Programmer(s) : JPB
*                 SB
*********************************************************************************************************
*/

#include <includes.h>


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#define  WDOG_REG_BASE_ADDR                               0x40048100u
#define  REG_WDOG_COP_REG                    (* (CPU_REG16 *) (WDOG_REG_BASE_ADDR + 0x00u))


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/

typedef  union {
    CPU_FNCT_VOID   Fnct;
    void           *Ptr;
} APP_INTVECT_ELEM;


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

#pragma language=extended
#pragma segment="CSTACK"

static  void  App_Reset_ISR       (void);

static  void  App_NMI_ISR         (void);

static  void  App_Fault_ISR       (void);

static  void  App_Spurious_ISR    (void);

extern  void  __iar_program_start (void);


/*
*********************************************************************************************************
*                                  EXCEPTION / INTERRUPT VECTOR TABLE
*
* Note(s) : (1) The Cortex-M0+ may have up to 32 external interrupts, which are the final entries in the
*               vector table.  The KL46Z has 32 external interrupt vectors.
*********************************************************************************************************
*/

__root  const  APP_INTVECT_ELEM  __vector_table[] @ ".intvec" = {
    { .Ptr = (void *)__sfe( "CSTACK" )},                        /* 000 Initial stack pointer.                           */
    App_Reset_ISR,                                              /* 001 Initial program counter.                         */
    App_NMI_ISR,                                                /* 002 Non-maskable interrupt.                          */
    App_Fault_ISR,                                              /* 003 Hard fault exception.                            */
    App_Spurious_ISR,                                           /* 004 Reserved interrupt 4.                            */
    App_Spurious_ISR,                                           /* 005 Reserved interrupt 5.                            */
    App_Spurious_ISR,                                           /* 006 Reserved interrupt 6.                            */
    App_Spurious_ISR,                                           /* 007 Reserved interrupt 7.                            */
    App_Spurious_ISR,                                           /* 008 Reserved interrupt 8.                            */
    App_Spurious_ISR,                                           /* 009 Reserved interrupt 9.                            */
    App_Spurious_ISR,                                           /* 010 Reserved interrupt 10.                           */
    App_Spurious_ISR,                                           /* 011 A supervisor call exception.                     */
    App_Spurious_ISR,                                           /* 012 Reserved interrupt 12.                           */
    App_Spurious_ISR,                                           /* 013 Reserved interrupt 13.                           */

#ifdef  BSP_CFG_OS
    OS_CPU_PendSVHandler,                                       /* 014 PendSV exception.                                */
    OS_CPU_SysTickHandler,                                      /* 015 SysTick Interrupt.                               */
#else
    App_Spurious_ISR,                                           /* 014 Reserved interrupt 14.                            */
    App_Spurious_ISR,                                           /* 015 Reserved interrupt 15.                           */
#endif

    BSP_IntHandlerDMA0,                                         /* 016 IRQ[  0] DMA Channel  0 Transfer Complete.       */
    BSP_IntHandlerDMA1,                                         /* 017 IRQ[  1] DMA Channel 1 Transfer Complete         */
    BSP_IntHandlerDMA2,                                         /* 018 IRQ[  2] DMA Channel 2 Transfer Complete         */
    BSP_IntHandlerDMA3,                                         /* 019 IRQ[  3] DMA Channel 3 Transfer Complete         */
    BSP_IntHandlerRSVD4,
    BSP_IntHandlerFTFA,                                         /* 021 IRQ[  5] Flash.                                  */
    BSP_IntHandlerLVD,                                          /* 022 IRQ[  6] Low voltage detect.                     */
    BSP_IntHandlerLLWU,                                         /* 023 IRQ[  7] Low Leakage Wakeup                      */
    BSP_IntHandlerI2C0,                                         /* 024 IRQ[  8] I2C0 interrupt                          */
    BSP_IntHandlerI2C1,                                         /* 025 IRQ[  9] I2C1 interrupt                          */
    BSP_IntHandlerSPI0,                                         /* 026 IRQ[ 10] Single interrupt vector for all sources */
    BSP_IntHandlerSPI1,                                         /* 027 IRQ[ 11] Single interrupt vector for all sources */
    BSP_IntHandlerUART0_RX_TX,                                  /* 028 IRQ[ 12] UART0 Receive/Transmit interrupt        */
    BSP_IntHandlerUART1_RX_TX,                                  /* 029 IRQ[ 13] UART1 Receive/Transmit interrupt        */
    BSP_IntHandlerUART2_RX_TX,                                  /* 030 IRQ[ 14] UART2 Receive/Transmit interrupt        */
    BSP_IntHandlerADC0,                                         /* 031 IRQ[ 15] ADC0 interrupt                          */
    BSP_IntHandlerCMP0,                                         /* 032 IRQ[ 16] CMP0 interrupt                          */
    BSP_IntHandlerTPM0,                                         /* 033 IRQ[ 17] TPM0 interrupt                          */
    BSP_IntHandlerTPM1,                                         /* 034 IRQ[ 18] TPM1 interrupt                          */
    BSP_IntHandlerTPM2,                                         /* 035 IRQ[ 19] TPM2 interrupt                          */
    BSP_IntHandlerRTC_ALARM,                                    /* 036 IRQ[ 20] RTC alarm interrupt                     */
    BSP_IntHandlerRTC_SECONDS,                                  /* 037 IRQ[ 21] RTC seconds interrupt                   */
    BSP_IntHandlerPIT,                                          /* 038 IRQ[ 22] Single interrupt vector for all channels*/
    BSP_IntHandlerI2S0,                                         /* 039 IRQ[ 23] Single interrupt vector for all sources */
    BSP_IntHandlerUSB_OTG,                                      /* 040 IRQ[ 24] USB OTG interrupt                       */
    BSP_IntHandlerDAC0,                                         /* 041 IRQ[ 25] DAC0 interrupt                          */
    BSP_IntHandlerTSI0,                                         /* 042 IRQ[ 26] TSI0 interrupt                          */
    BSP_IntHandlerMCG,                                          /* 043 IRQ[ 27] MCG Interrupt                           */
    BSP_IntHandlerLPTMR0,                                       /* 044 IRQ[ 28] LPTMR0 interrupt                        */
    BSP_IntHandlerSLCD,                                         /* 045 IRQ[ 29] LCD Controller interrupt                */
    BSP_IntHandlerPORT_A,                                       /* 046 IRQ[ 30] Port A interrupt                        */
    BSP_IntHandlerPORT_C_D                                      /* 047 IRQ[ 31] Single interrupt vector for Ports C & D */
};


/*
*********************************************************************************************************
*                                            App_Reset_ISR()
*
* Description : Handle Reset.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_Reset_ISR (void)
{
#if __ARMVFP__                                                  /* Enable access to Floating-point coprocessor.         */
    CPU_REG_NVIC_CPACR = CPU_REG_NVIC_CPACR_CP10_FULL_ACCESS | CPU_REG_NVIC_CPACR_CP11_FULL_ACCESS;

    DEF_BIT_CLR(CPU_REG_SCB_FPCCR, DEF_BIT_31);                 /* Disable automatic FP register content                */
    DEF_BIT_CLR(CPU_REG_SCB_FPCCR, DEF_BIT_30);                 /* Disable Lazy context switch                          */
#endif

    REG_WDOG_COP_REG = 0x00u;                                   /* Disable the watchdog timer.                          */

    __iar_program_start();
}


/*
*********************************************************************************************************
*                                            App_NMI_ISR()
*
* Description : Handle Non-Maskable Interrupt (NMI).
* Argument(s) : none.
* Return(s)   : none.
* Caller(s)   : This is an ISR.
*
* Note(s)     : (1) Since the NMI is not being used, this serves merely as a catch for a spurious
*                   exception.
*********************************************************************************************************
*/

static  void  App_NMI_ISR (void)
{
    while (DEF_TRUE) {
        ;
    }
}


/*
*********************************************************************************************************
*                                             App_Fault_ISR()
*
* Description : Handle hard fault.
* Argument(s) : none.
* Return(s)   : none.
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_Fault_ISR (void)
{
    while (DEF_TRUE) {
        ;
    }
}


/*
*********************************************************************************************************
*                                           App_Spurious_ISR()
*
* Description : Handle spurious interrupt.
* Argument(s) : none.
* Return(s)   : none.
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_Spurious_ISR (void)
{
    while (DEF_TRUE) {
        ;
    }
}