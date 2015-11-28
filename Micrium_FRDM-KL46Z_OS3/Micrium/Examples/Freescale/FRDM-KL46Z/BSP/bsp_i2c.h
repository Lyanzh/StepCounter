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
*                                             I2C Driver
*
*                                        Freescale Kinetis KL40
*                                               on the
*                                        Freescale FRDM-KL46Z
*                                          Evaluation Board
*
* Filename      : bsp_i2c.h
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

#ifndef  BSP_I2C_PRESENT
#define  BSP_I2C_PRESENT


/*
*********************************************************************************************************
*                                                 EXTERNS
*********************************************************************************************************
*/

#ifdef   BSP_I2C_MODULE
#define  BSP_I2C_EXT
#else
#define  BSP_I2C_EXT  extern
#endif

/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/

#include  <bsp.h>

/*
*********************************************************************************************************
*                                         LCD CTL OPTION DEFINES
*********************************************************************************************************
*/
                                                                /* -------- I2C SCL SDA MULTIPLEXING CONTROL --------*/
#define  BSP_I2C0_SCL_SDA	    PORTE_PCR24 = PORT_PCR_MUX(5); \
                                PORTE_PCR25 = PORT_PCR_MUX(5)
								
#define  BSP_I2C1_SCL_SDA       PORTC_PCR10 = PORT_PCR_MUX(2); \
                                PORTC_PCR11 = PORT_PCR_MUX(2)

                                                                /* ------------------- I2C CHANNELS ------------------*/
#define  BSP_I2C_0              0
#define  BSP_I2C_1              1
#define  BSP_I2C_DEFAULT        BSP_I2C_0

/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void        BSP_I2C_Set_TX_Mode      (CPU_INT08U I2C_No);

void        BSP_I2C_Set_RX_Mode      (CPU_INT08U I2C_No);

void        BSP_I2C_Set_Slave_Mode   (CPU_INT08U I2C_No);

void        BSP_I2C_Set_Master_Mode  (CPU_INT08U I2C_No);

void        BSP_I2C_Give_Nack        (CPU_INT08U I2C_No);

void        BSP_I2C_Give_Ack         (CPU_INT08U I2C_No);

void        BSP_I2C_Repeated_Start   (CPU_INT08U I2C_No);

void        BSP_I2C_Write_Byte       (CPU_INT08U I2C_No,
	                                  CPU_INT08U Data);

CPU_INT08U  BSP_I2C_Read_Byte        (CPU_INT08U I2C_No);

void        BSP_I2C_Start            (CPU_INT08U I2C_No);

void        BSP_I2C_Stop             (CPU_INT08U I2C_No);

void        BSP_I2C_Wait             (CPU_INT08U I2C_No);

CPU_INT16U  BSP_I2C_Get_Ack          (CPU_INT08U I2C_No);

void        BSP_I2C_Init             (CPU_INT08U I2C_No);

void        BSP_I2C_Deinit           (CPU_INT08U I2C_No);

#endif                                                          /* End of module include.                               */
