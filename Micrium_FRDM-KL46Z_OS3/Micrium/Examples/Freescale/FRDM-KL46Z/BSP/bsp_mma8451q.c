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
*                                            LCD_S401M16KR
*                                     Four-Digit LCD Panel Driver
*
*                                        Freescale Kinetis KL40
*                                               on the
*                                        Freescale FRDM-KL46Z
*                                          Evaluation Board
*
* Filename      : bsp_lcd.c
* Version       : V1.00
* Programmer(s) : SB
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define  BSP_MMA8451_MODULE
#include  <bsp_mma8451q.h>
#include  <MKL46Z4.h>

/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/

#define MMA8451_I2C_ADDRESS (0x1d<<1)

#define I2C_READ  1
#define I2C_WRITE 0

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

//Variables acelerometro//
CPU_INT16S  accel_x, accel_y, accel_z;
CPU_INT16S  Aresultx, Aresulty, Aresultz;
CPU_CHAR    aux1;
CPU_INT32S  Atestx, Atesty, Atestz;

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/

void BSP_MMA8451_Init(void)  //Standby
{
    CPU_INT08U tmp;
	
    BSP_I2C_Init(0);  //Initialize I2C modules
    tmp = BSP_MMA8451_Read_Reg(CTRL_REG1);
    BSP_MMA8451_Write_Reg(CTRL_REG1, tmp & ~ACTIVE_MASK);  //Standby??
}

void mma8451_active (CPU_INT08U dynamicrange)
{
    if (dynamicrange != 2 || dynamicrange != 4 || dynamicrange != 8) {  //??ActiveMode???2??4??8?????2 
        dynamicrange = 2;
    }
		
    if (2 == dynamicrange) {
        BSP_MMA8451_Write_Reg(XYZ_DATA_CFG_REG, (BSP_MMA8451_Read_Reg(XYZ_DATA_CFG_REG) & ~FS_MASK));//Write the 2g dynamic range value into register 0x0E
    } else if (4 == dynamicrange) {
        BSP_MMA8451_Write_Reg(XYZ_DATA_CFG_REG, (BSP_MMA8451_Read_Reg(XYZ_DATA_CFG_REG) & ~FS_MASK));
        BSP_MMA8451_Write_Reg(XYZ_DATA_CFG_REG, (BSP_MMA8451_Read_Reg(XYZ_DATA_CFG_REG) | FULL_SCALE_4G));//Write the 4g dynamic range value into register 0x0E
    } else {
        BSP_MMA8451_Write_Reg(XYZ_DATA_CFG_REG, (BSP_MMA8451_Read_Reg(XYZ_DATA_CFG_REG) & ~FS_MASK));
        BSP_MMA8451_Write_Reg(XYZ_DATA_CFG_REG, (BSP_MMA8451_Read_Reg(XYZ_DATA_CFG_REG) | FULL_SCALE_8G));//Write the 8g dynamic range value into register 0x0E
    }
		
    BSP_MMA8451_Write_Reg(CTRL_REG1, (BSP_MMA8451_Read_Reg(CTRL_REG1) | ACTIVE_MASK));  //Set the Active bit in CTRL Reg 1
}

// this delay is very important, it may cause w-r operation failure.
static void pause(void)
{
    CPU_INT08U n;
    for (n=0; n<40; n++) {
        __asm{nop};
    }
}

CPU_INT08U BSP_MMA8451_Read_Reg(CPU_INT08U addr)
{
    CPU_INT08U result;

    BSP_I2C_Start(0);  //????
    BSP_I2C_Write_Byte(0, MMA8451_I2C_ADDRESS | I2C_WRITE);  //????
    
    BSP_I2C_Wait(0);  //??
    BSP_I2C_Get_Ack(0);  //??????

    BSP_I2C_Write_Byte(0, addr);  //??????
    BSP_I2C_Wait(0);  //??
    BSP_I2C_Get_Ack(0);  //??????

    BSP_I2C_Repeated_Start(0);  //????
    BSP_I2C_Write_Byte(0, MMA8451_I2C_ADDRESS | I2C_READ);  //??????
    BSP_I2C_Wait(0);  //??
    BSP_I2C_Get_Ack(0);  //??????

    BSP_I2C_Set_RX_Mode(0);  //????

    BSP_I2C_Give_Nack(0);  //???????,??????
    result = BSP_I2C_Read_Byte(0);  //????
    BSP_I2C_Wait(0);  //??

    BSP_I2C_Stop(0);  //????
    result = BSP_I2C_Read_Byte(0);  //????
    pause();
    return result;
}

void BSP_MMA8451_Write_Reg(CPU_INT08U addr, CPU_INT08U data)
{
    BSP_I2C_Start(0);  //????

    BSP_I2C_Write_Byte(0, MMA8451_I2C_ADDRESS|I2C_WRITE);  //??????
    BSP_I2C_Wait(0);
    BSP_I2C_Get_Ack(0);

    BSP_I2C_Write_Byte(0, addr);  //??????
    BSP_I2C_Wait(0);
    BSP_I2C_Get_Ack(0);

    BSP_I2C_Write_Byte(0, data);  //???
    BSP_I2C_Wait(0);
    BSP_I2C_Get_Ack(0);

    BSP_I2C_Stop(0);
    pause();
}

///****Funciones del Acelerometro******//

void accel_read(void)
{
    if ((BSP_MMA8451_Read_Reg(0x00)&0xf) != 0) {
        accel_x   = BSP_MMA8451_Read_Reg(0x01)<<8;
        accel_x  |= BSP_MMA8451_Read_Reg(0x02);
        accel_x >>= 2;

        accel_y   = BSP_MMA8451_Read_Reg(0x03)<<8;
        accel_y  |= BSP_MMA8451_Read_Reg(0x04);
        accel_y >>= 2;

        accel_z   = BSP_MMA8451_Read_Reg(0x05)<<8;
        accel_z  |= BSP_MMA8451_Read_Reg(0x06);
        accel_z >>= 2;

        Aresultx   = BSP_MMA8451_Read_Reg(0x01)<<8;
        Aresultx  |= BSP_MMA8451_Read_Reg(0x02);
        Aresultx >>= 8;

        Aresulty   = BSP_MMA8451_Read_Reg(0x03)<<8;
        Aresulty  |= BSP_MMA8451_Read_Reg(0x04);
        Aresulty >>= 8;

        Aresultz   = BSP_MMA8451_Read_Reg(0x05)<<8;
        Aresultz  |= BSP_MMA8451_Read_Reg(0x06);
        Aresultz >>= 8;
    }
}

void accel_test(void)
{
    accel_read();
    Atestx = Aresultx;
    Atesty = Aresulty;
    Atestz = Aresultz;

    printf("\n\r Acelerometer Values");
    printf("\n\rx = %d \t",Atestx);
    printf("y = %d \t",Atesty);
    printf("z = %d \n\r",Atestz);
  
}

int iAcclReadX(void)
{
    if ((BSP_MMA8451_Read_Reg(0x00)&0xf) != 0) {
        accel_x   = BSP_MMA8451_Read_Reg(0x01)<<8;
        accel_x  |= BSP_MMA8451_Read_Reg(0x02);
        accel_x >>= 2;
      
        Aresultx   = BSP_MMA8451_Read_Reg(0x01)<<8;
        Aresultx  |= BSP_MMA8451_Read_Reg(0x02);
        Aresultx >>= 8;
    }
    return Aresultx;
}

int iAcclReadY(void)
{
    if ((BSP_MMA8451_Read_Reg(0x00)&0xf) != 0) {
        accel_y   = BSP_MMA8451_Read_Reg(0x03)<<8;
        accel_y  |= BSP_MMA8451_Read_Reg(0x04);
        accel_y >>= 2;
        
        Aresulty   = BSP_MMA8451_Read_Reg(0x03)<<8;
        Aresulty  |= BSP_MMA8451_Read_Reg(0x04);
        Aresulty >>= 8;
    }
    return Aresulty;
}

int iAcclReadZ(void) {
    if ((BSP_MMA8451_Read_Reg(0x00)&0xf) != 0) {
        accel_z   = BSP_MMA8451_Read_Reg(0x05)<<8;
        accel_z  |= BSP_MMA8451_Read_Reg(0x06);
        accel_z >>= 2;

        Aresultz   = BSP_MMA8451_Read_Reg(0x05)<<8;
        Aresultz  |= BSP_MMA8451_Read_Reg(0x06);
        Aresultz >>= 8;   
    }
    return Aresultz;
}

