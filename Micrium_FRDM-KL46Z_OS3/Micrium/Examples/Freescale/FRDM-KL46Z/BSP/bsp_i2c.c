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
*                                              I2C Driver
*
*                                        Freescale Kinetis KL40
*                                               on the
*                                        Freescale FRDM-KL46Z
*                                          Evaluation Board
*
* Filename      : bsp_i2c.c
* Version       : V1.00
* Programmer(s) : Lin Yanzhou
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define  BSP_I2C_MODULE
#include  <bsp_i2c.h>
#include  <MKL46Z4.h>

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#define BSP_I2C0        I2C0_BASE_PTR
#define BSP_I2C1        I2C1_BASE_PTR

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


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

I2C_MemMapPtr i2c_get_base_address(CPU_INT08U I2C_No);

/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/

//???????
void BSP_I2C_Set_TX_Mode(CPU_INT08U I2C_No)
{
    BSET(I2C_C1_TX_SHIFT, I2C_C1_REG(i2c_get_base_address(I2C_No)));//??i2c TXAK
}

//???????
void BSP_I2C_Set_RX_Mode(CPU_INT08U I2C_No)
{
    BCLR(I2C_C1_TX_SHIFT, I2C_C1_REG(i2c_get_base_address(I2C_No))); //?i2c TXAK
}

//???????
void BSP_I2C_Set_Slave_Mode(CPU_INT08U I2C_No)
{
    BCLR(I2C_C1_MST_SHIFT, I2C_C1_REG(i2c_get_base_address(I2C_No))); //?i2c MST
}

//???????
void BSP_I2C_Set_Master_Mode(CPU_INT08U I2C_No)
{
    BSET(I2C_C1_MST_SHIFT, I2C_C1_REG(i2c_get_base_address(I2C_No))); //??i2c MST
}

// i2c general

//???????
void BSP_I2C_Give_Nack(CPU_INT08U I2C_No)
{
    BSET(I2C_C1_TXAK_SHIFT, I2C_C1_REG(i2c_get_base_address(I2C_No))); //??i2c TXAK
}

//??????
void BSP_I2C_Give_Ack(CPU_INT08U I2C_No)
{
    BCLR(I2C_C1_TXAK_SHIFT, I2C_C1_REG(i2c_get_base_address(I2C_No))); //?i2c TXAK
}

//??????
void BSP_I2C_Repeated_Start(CPU_INT08U I2C_No)
{
    BSET(I2C_C1_RSTA_SHIFT, I2C_C1_REG(i2c_get_base_address(I2C_No))); //??i2c RSTA
}

//?
void BSP_I2C_Write_Byte(CPU_INT08U I2C_No,
	                      CPU_INT08U Data)
{
    I2C_D_REG(i2c_get_base_address(I2C_No)) = Data;
}

//?
CPU_INT08U BSP_I2C_Read_Byte(CPU_INT08U I2C_No)
{
    return I2C_D_REG(i2c_get_base_address(I2C_No));
}

//????
void BSP_I2C_Start(CPU_INT08U I2C_No)
{
    BSP_I2C_Set_Master_Mode(I2C_No);
    BSP_I2C_Set_TX_Mode(I2C_No);
}

//????
void BSP_I2C_Stop(CPU_INT08U I2C_No)
{
    BSP_I2C_Set_Slave_Mode(I2C_No);
    BSP_I2C_Set_RX_Mode(I2C_No);
}

//??
void BSP_I2C_Wait(CPU_INT08U I2C_No)
{
    // wait flag
    while (BGET(I2C_S_IICIF_SHIFT, I2C_S_REG(i2c_get_base_address(I2C_No))) == 0);
    //?1??
    BSET(I2C_S_IICIF_SHIFT, I2C_S_REG(i2c_get_base_address(I2C_No)));
}

//??????
CPU_INT16U BSP_I2C_Get_Ack(CPU_INT08U I2C_No)
{
    if (BGET(I2C_S_RXAK_SHIFT, I2C_S_REG(i2c_get_base_address(I2C_No))) == 0) {
        return 1;//TRUE;
    } else {
        return 0;//FALSE;
    }
}

/*
*********************************************************************************************************
*                                           BSP_I2C_Init()
*
* Description : Initialize the I2C module.
*
* Argument(s) : I2C_No        I2C channel  :  BSP_I2C_0
*	                                          BSP_I2C_1
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/
void BSP_I2C_Init(CPU_INT08U I2C_No)
{
    I2C_MemMapPtr num;
	
    if (I2C_No < 0 || I2C_No > 1) {
        I2C_No = 0;
    }
                                 
    num = i2c_get_base_address(I2C_No);                          /* Get the base address of I2C module                  */
    if (BSP_I2C0 == num) {
        SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;
        SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTE_MASK;

        BSP_I2C0_SCL_SDA;                                        /* GPIO multiplexing control for I2C0                  */
		
        I2C0_F  = 0x14;                                          /* Baud rate of I2C0 is 300k                           */
        BSET(I2C_C1_IICEN_SHIFT, I2C0_C1);                       /* Enable I2C0 module                                  */
    } else {
    	  SIM_SCGC4 |= SIM_SCGC4_I2C1_MASK;
        SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;

        BSP_I2C1_SCL_SDA;                                       /* GPIO multiplexing control for I2C1                   */

        I2C0_F = 0x14;                                          /* Baud rate of I2C1 is 300k                            */
        BSET(I2C_C1_IICEN_SHIFT,I2C0_C1);                       /* Enable I2C1 module                                   */
    }
}

/*
*********************************************************************************************************
*                                          BSP_I2C_Deinit()
*
* Description : Disable the I2C module.
*
* Argument(s) : I2C_No        I2C channel  :  BSP_I2C_0
*	                                          BSP_I2C_1
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/
void BSP_I2C_Deinit(CPU_INT08U I2C_No)
{
    if (I2C_No < 0 || I2C_No > 1) {
        I2C_No = 0;
    }
    if (0 == I2C_No) {
        I2C0_C1 = 0x00;
		    SIM_SCGC4 &= ~SIM_SCGC4_I2C0_MASK;
    } else {
		    I2C1_C1 = 0x00;
		    SIM_SCGC4 &= ~SIM_SCGC4_I2C1_MASK;
    }
}

I2C_MemMapPtr i2c_get_base_address(CPU_INT08U I2C_No)
{
    if (I2C_No < 0 || I2C_No > 1) {
        I2C_No = 0;
    }
    if (0 == I2C_No) {
    	return I2C0_BASE_PTR;
    } else {
    	return I2C1_BASE_PTR;
    }
}
