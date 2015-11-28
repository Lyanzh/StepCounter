/*
*********************************************************************************************************
*                                     MICIRUM BOARD SUPPORT PACKAGE
*
*                             (c) Copyright 2013; Micrium, Inc.; Weston, FL
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
*                                             UART Driver
*
*                                        Freescale Kinetis KL40
*                                               on the
*                                        Freescale FRDM-KL46Z
*                                          Evaluation Board
*
* Filename      : bsp_uart.h
* Version       : V1.00
* Programmer(s) : Lin Yanzhou
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                 MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
*               BSP present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef  BSP_UART_PRESENT
#define  BSP_UART_PRESENT


/*
*********************************************************************************************************
*                                                 EXTERNS
*********************************************************************************************************
*/

#ifdef   BSP_UART_MODULE
#define  BSP_UART_EXT
#else
#define  BSP_UART_EXT  extern
#endif


/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/

#include  <bsp.h>

/*
*********************************************************************************************************
*                                            UART OPTION DEFINES
*********************************************************************************************************
*/
                                                                /* ------- UART TXD RXD MULTIPLEXING CONTROL --------*/
#define  BSP_UART0_TXD_RXD	    PORTA_PCR14 = PORT_PCR_MUX(0x3); \
                                PORTA_PCR15 = PORT_PCR_MUX(0x3)
								
#define  BSP_UART1_TXD_RXD      PORTE_PCR0  = PORT_PCR_MUX(0x3); \
                                PORTE_PCR1  = PORT_PCR_MUX(0x3)

#define  BSP_UART2_TXD_RXD	    PORTE_PCR22 = PORT_PCR_MUX(0x4); \
                                PORTE_PCR23 = PORT_PCR_MUX(0x4)

                                                                /* ------------------ UART CHANNELS ------------------*/
#define  BSP_UART_0             0
#define  BSP_UART_1             1
#define  BSP_UART_2             2
#define  BSP_UART_DEFAULT       BSP_UART_0

                                                                /* ------------------ UART SOURCE --------------------*/
#define  BSP_MCGIRCLK           0
#define  BSP_MCGPLL             1
#define  BSP_BUSCLK	            2

/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void        BSP_UART_Init           (CPU_INT08U uartNo,
									                   CPU_INT32U sel_clk,
									                   CPU_INT32U baud_rate);

CPU_INT08U  BSP_UART_Send1          (CPU_INT08U uartNo,
                                     CPU_INT08U ch);

CPU_INT08U  BSP_UART_sendN          (CPU_INT08U uartNo,
                                     CPU_INT16U len,
                                     CPU_INT08U* buff);

CPU_INT08U  BSP_UART_Send_String    (CPU_INT08U uartNo,
	                                   void       *buff);

CPU_INT08U  BSP_UART_Re1            (CPU_INT08U uartNo,
	                                   CPU_INT08U *fp);

CPU_INT08U  BSP_UART_ReN            (CPU_INT08U uartNo,
	                                   CPU_INT16U len,
	                                   CPU_INT08U* buff);

void        BSP_UART_Enable_Re_Int  (CPU_INT08U uartNo);

void        BSP_UART_Disable_Re_Int (CPU_INT08U uartNo);

void        uart_printf             (CPU_CHAR   *fmt,...);

#endif                                                          /* End of module include.                               */
