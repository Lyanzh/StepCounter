/*
*********************************************************************************************************
*                                     MICIRUM BOARD SUPPORT PACKAGE
*
*                             (c) Copyright 2013; Micrium, Inc.; Weston, FL
*
*                   All rights reserved.  Protected by international copyright laws.
*                   Knowledge of the source code may not be used to write a similar
*                   product.  This file may only be used in accordance with a license
*                   and should not be redistributed in any way.
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
* Filename      : bsp_int.c
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

#define  BSP_INT_MODULE
#include <bsp.h>

#ifdef  BSP_CFG_OS
#include <bsp_os.h>
#endif


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#define  BSP_INT_SRC_NBR                                 106


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

static  CPU_FNCT_VOID  BSP_IntVectTbl[BSP_INT_SRC_NBR];


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  BSP_IntHandler      (CPU_DATA  int_id);
static  void  BSP_IntHandlerDummy (void);


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                              BSP_IntClr()
*
* Description : Clear interrupt.
*
* Argument(s) : int_id      Interrupt to clear.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) An interrupt does not need to be cleared within the interrupt controller.
*********************************************************************************************************
*/

void  BSP_IntClr (CPU_DATA  int_id)
{

}


/*
*********************************************************************************************************
*                                              BSP_IntDis()
*
* Description : Disable interrupt.
*
* Argument(s) : int_id      Interrupt to disable.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_IntDis (CPU_DATA  int_id)
{
    if (int_id < BSP_INT_SRC_NBR) {
        CPU_IntSrcDis(int_id + 16);
    }
}


/*
*********************************************************************************************************
*                                           BSP_IntDisAll()
*
* Description : Disable ALL interrupts.
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

void  BSP_IntDisAll (void)
{
    CPU_IntDis();
}


/*
*********************************************************************************************************
*                                               BSP_IntEn()
*
* Description : Enable interrupt.
*
* Argument(s) : int_id      Interrupt to enable.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_IntEn (CPU_DATA  int_id)
{
    if (int_id < BSP_INT_SRC_NBR) {
        CPU_IntSrcEn(int_id + 16);
    }
}


/*
*********************************************************************************************************
*                                            BSP_IntVectSet()
*
* Description : Assign ISR handler.
*
* Argument(s) : int_id      Interrupt for which vector will be set.
*
*               isr         Handler to assign
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_IntVectSet (CPU_DATA       int_id,
                      CPU_FNCT_VOID  isr)
{
    CPU_SR_ALLOC();


    if (int_id < BSP_INT_SRC_NBR) {
        CPU_CRITICAL_ENTER();
        BSP_IntVectTbl[int_id] = isr;
        CPU_CRITICAL_EXIT();
    }
}


/*
*********************************************************************************************************
*                                            BSP_IntPrioSet()
*
* Description : Assign ISR priority.
*
* Argument(s) : int_id      Interrupt for which vector will be set.
*
*               prio        Priority to assign
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_IntPrioSet (CPU_DATA    int_id,
                      CPU_INT08U  prio)
{
    CPU_SR_ALLOC();


    if (int_id < BSP_INT_SRC_NBR) {
        CPU_CRITICAL_ENTER();
        CPU_IntSrcPrioSet(int_id + 16, prio);
        CPU_CRITICAL_EXIT();
    }
}


/*
*********************************************************************************************************
*********************************************************************************************************
*                                           INTERNAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                              BSP_IntInit()
*
* Description : Initialize interrupts:
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

void  BSP_IntInit (void)
{
    CPU_DATA  int_id;


    for (int_id = 0; int_id < BSP_INT_SRC_NBR; int_id++) {
        BSP_IntVectSet(int_id, BSP_IntHandlerDummy);
    }
}


/*
*********************************************************************************************************
*                                        BSP_IntHandler####()
*
* Description : Handle an interrupt.
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

void  BSP_IntHandlerDMA0             (void)  { BSP_IntHandler(BSP_INT_ID_DMA0);                      }
void  BSP_IntHandlerDMA1             (void)  { BSP_IntHandler(BSP_INT_ID_DMA1);                      }
void  BSP_IntHandlerDMA2             (void)  { BSP_IntHandler(BSP_INT_ID_DMA2);                      }
void  BSP_IntHandlerDMA3             (void)  { BSP_IntHandler(BSP_INT_ID_DMA3);                      }
void  BSP_IntHandlerRSVD4            (void)  { BSP_IntHandler(BSP_INT_ID_RSVD4);                     }
void  BSP_IntHandlerFTFA             (void)  { BSP_IntHandler(BSP_INT_ID_FTFA);                      }
void  BSP_IntHandlerLVD              (void)  { BSP_IntHandler(BSP_INT_ID_LVD);                       }
void  BSP_IntHandlerLLWU             (void)  { BSP_IntHandler(BSP_INT_ID_LLWU);                      }
void  BSP_IntHandlerI2C0             (void)  { BSP_IntHandler(BSP_INT_ID_I2C0);                      }
void  BSP_IntHandlerI2C1             (void)  { BSP_IntHandler(BSP_INT_ID_I2C1);                      }
void  BSP_IntHandlerSPI0             (void)  { BSP_IntHandler(BSP_INT_ID_SPI0);                      }
void  BSP_IntHandlerSPI1             (void)  { BSP_IntHandler(BSP_INT_ID_SPI1);                      }
void  BSP_IntHandlerUART0_RX_TX      (void)  { BSP_IntHandler(BSP_INT_ID_UART0_RX_TX);               }
void  BSP_IntHandlerUART1_RX_TX      (void)  { BSP_IntHandler(BSP_INT_ID_UART1_RX_TX);               }
void  BSP_IntHandlerUART2_RX_TX      (void)  { BSP_IntHandler(BSP_INT_ID_UART2_RX_TX);               }
void  BSP_IntHandlerADC0             (void)  { BSP_IntHandler(BSP_INT_ID_ADC0);                      }
void  BSP_IntHandlerCMP0             (void)  { BSP_IntHandler(BSP_INT_ID_CMP0);                      }
void  BSP_IntHandlerTPM0             (void)  { BSP_IntHandler(BSP_INT_ID_TPM0);                      }
void  BSP_IntHandlerTPM1             (void)  { BSP_IntHandler(BSP_INT_ID_TPM1);                      }
void  BSP_IntHandlerTPM2             (void)  { BSP_IntHandler(BSP_INT_ID_TPM2);                      }
void  BSP_IntHandlerRTC_ALARM        (void)  { BSP_IntHandler(BSP_INT_ID_RTC_ALARM);                 }
void  BSP_IntHandlerRTC_SECONDS      (void)  { BSP_IntHandler(BSP_INT_ID_RTC_SECONDS);               }
void  BSP_IntHandlerPIT              (void)  { BSP_IntHandler(BSP_INT_ID_PIT);                       }
void  BSP_IntHandlerI2S0             (void)  { BSP_IntHandler(BSP_INT_ID_I2S0);                      }
void  BSP_IntHandlerUSB_OTG          (void)  { BSP_IntHandler(BSP_INT_ID_USB_OTG);                   }
void  BSP_IntHandlerDAC0             (void)  { BSP_IntHandler(BSP_INT_ID_DAC0);                      }
void  BSP_IntHandlerTSI0             (void)  { BSP_IntHandler(BSP_INT_ID_TSI0);                      }
void  BSP_IntHandlerMCG              (void)  { BSP_IntHandler(BSP_INT_ID_MCG);                       }
void  BSP_IntHandlerLPTMR0           (void)  { BSP_IntHandler(BSP_INT_ID_LPTMR0);                    }
void  BSP_IntHandlerSLCD             (void)  { BSP_IntHandler(BSP_INT_ID_SLCD);                      }
void  BSP_IntHandlerPORT_A           (void)  { BSP_IntHandler(BSP_INT_ID_PORT_A);                    }
void  BSP_IntHandlerPORT_C_D         (void)  { BSP_IntHandler(BSP_INT_ID_PORT_C_D);                  }


/*
*********************************************************************************************************
*********************************************************************************************************
*                                           LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                          BSP_IntHandler()
*
* Description : Central interrupt handler.
*
* Argument(s) : int_id          Interrupt that will be handled.
*
* Return(s)   : none.
*
* Caller(s)   : ISR handlers.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  BSP_IntHandler (CPU_DATA  int_id)
{
    CPU_FNCT_VOID  isr;

#ifdef  BSP_CFG_OS
    CPU_SR_ALLOC();


    CPU_CRITICAL_ENTER();                                       /* Tell the OS that we are starting an ISR            */

    OSIntEnter();

    CPU_CRITICAL_EXIT();
#endif

    if (int_id < BSP_INT_SRC_NBR) {
        isr = BSP_IntVectTbl[int_id];
        if (isr != (CPU_FNCT_VOID)0) {
            isr();
        }
    }

#ifdef  BSP_CFG_OS
    OSIntExit();                                                /* Tell the OS that we are leaving the ISR            */
#endif
}


/*
*********************************************************************************************************
*                                        BSP_IntHandlerDummy()
*
* Description : Dummy interrupt handler.
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

static  void  BSP_IntHandlerDummy (void)
{

}
