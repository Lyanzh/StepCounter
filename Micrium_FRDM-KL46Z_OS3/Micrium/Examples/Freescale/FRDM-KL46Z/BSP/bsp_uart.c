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
*                                             UART Driver
*
*                                        Freescale Kinetis KL40
*                                               on the
*                                        Freescale FRDM-KL46Z
*                                          Evaluation Board
*
* Filename      : bsp_uart.c
* Version       : V1.00
* Programmer(s) : Lin Yanzhou
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define   BSP_UART_MODULE
#include  <bsp_uart.h>
#include  <MKL46Z4.h>


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


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

                                                                /* ------------- UART1,UART2 ADDRESS MAP -------------*/
static const  UART_MemMapPtr UART_ARR[]    = {UART1_BASE_PTR, UART2_BASE_PTR};

                                                                /* ----------------- UART IRQ TABLE ------------------*/
static        CPU_INT08U table_irq_uart[3] = {12, 13, 14};

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

/*
*********************************************************************************************************
*                                        MCGIRCLK_OutEnable()
*
* Description : Enable clock MCGIRCLK,which frequency is 31.25Khz ~ 4Mhz.
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
static void MCGIRCLK_OutEnable(void)
{
    MCG_C1 |= MCG_C1_IRCLKEN_MASK|MCG_C1_IREFSTEN_MASK;
    MCG_SC |= MCG_SC_FCRDIV(0x0);                               /* Division factor of internal 4M quick reference clock */
																/* 31.25Khz ~ 4Mhz                                      */
    MCG_C4 |= MCG_C4_FCTRIM(0xA);
    MCG_C2 |= MCG_C2_IRCS_MASK;
    while (!(MCG_S & MCG_S_IRCST_MASK));
	}

/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                           BSP_UART_Init()
*
* Description : Initialize the UART module.
*
* Argument(s) : uartNo        UART channel  :  BSP_UART_0
*	                                             BSP_UART_1
*	                                             BSP_UART_2
*
*	              sel_clk       Clock of UART :  BSP_MCGIRCLK  (4000Khz)
*                                              BSP_MCGPLL    (48000Khz)
*                                              BSP_BUSCLK    (24000khz)
*
*               baud_rate     Baud rate of UART : 300, 600, 120, 2400, 4800, 9600, 19200, 115200...
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/
void  BSP_UART_Init (CPU_INT08U uartNo,
	                   CPU_INT32U sel_clk,
                     CPU_INT32U baud_rate) 
{	  
    register CPU_INT16U sbr;
    CPU_INT08U temp;  
    CPU_INT32U clk_Khz;
    UART0_MemMapPtr uartch1 = UART0_BASE_PTR;
    UART_MemMapPtr uartch2;
    uartch2 = UART_ARR[uartNo-1];                               /* Get the base address of UART1,UART2 modules          */

    if (uartNo == 0) {
        switch (sel_clk) {
        case BSP_MCGIRCLK:
            MCGIRCLK_OutEnable();                               /* Enable clock MCGIRCLK                                */
            SIM_SOPT2 |= SIM_SOPT2_UART0SRC(0x3);               /* Choose MCGIRCLK(4000Khz) for UART0                   */
            break;
				 
        case BSP_MCGPLL:
            SIM_SOPT2 |= SIM_SOPT2_UART0SRC(1);                 /* Choose MCGFLLCLK_khz(48000Khz) for UART0             */
            SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
            break;
					 
        default:
            break;
        }
		
        BSP_UART0_TXD_RXD;                                      /* GPIO multiplexing control for UART0                  */
		
        SIM_SCGC4   |= SIM_SCGC4_UART0_MASK;                    /* Enable UART0 clock                                   */
				
                                                                /* Make sure that the transmitter and receiver are 
                                                                 * disabled while we change settings.
                                                                 */ 
        uartch1->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);
			 
                                                                /* Configure the UART for 8-bit mode, no parity         */
        if (BSP_MCGIRCLK == sel_clk) {
	        clk_Khz = 4000;
        } else if (BSP_MCGPLL == sel_clk) {
            clk_Khz = 48000;
        }
                                                                /* Calculate baud settings                              */
        sbr  = (CPU_INT16U)((clk_Khz * 1000) / (baud_rate * 16));
	    
                                                                /* Save off the current value of the uartx_BDH except 
                                                                 * for the SBR field 
                                                                 */
		    temp = UART_BDH_REG(uartch1) & ~(UART_BDH_SBR(0x1F));
		
	      UART_BDH_REG(uartch1) = temp | UART_BDH_SBR(((sbr & 0x1F00) >> 8));
	      UART_BDL_REG(uartch1) = (CPU_INT08U)(sbr & UART_BDL_SBR_MASK);

                                                                /* Initialize control register and clear flag bit       */
	      UART0_C4  = 0x0F;
	      UART0_C1  = 0x00;
	      UART0_C3  = 0x00;
	      UART0_MA1 = 0x00;
	      UART0_MA2 = 0x00;
	      UART0_S1 |= 0x1F;
	      UART0_S2 |= 0xC0;
		
	      uartch1->C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK);   /* Enable receiver and transmitter                      */
    } else {
        switch (uartNo)
	      {
	      case BSP_UART_1:
	          BSP_UART1_TXD_RXD;                                /* GPIO multiplexing control for UART1                  */ 

            SIM_SCGC4 |= SIM_SCGC4_UART1_MASK;                  /* Enable UART1 clock                                   */
            break;
	      case BSP_UART_2: 
	          BSP_UART0_TXD_RXD;                                /* GPIO multiplexing control for UART2                  */
			
            SIM_SCGC4  |= SIM_SCGC4_UART2_MASK;                 /* Enable UART2 clock                                   */
            break;
	      default:
            break;
        }
		
        uartch2->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);
		
        //uartch2->C1 = 0;

                                                                /* Only Bus clock(24M) can be chosen for UART1
                                                                 * and UART2(KL46)
                                                                 */
        sbr = (CPU_INT16U)((24000*1000)/(baud_rate * 16));
		
        temp = UART_BDH_REG(uartch2) & ~(UART_BDH_SBR(0x1F));
		
        UART_BDH_REG(uartch2) = temp |  UART_BDH_SBR(((sbr & 0x1F00) >> 8));
        UART_BDL_REG(uartch2) = (CPU_INT08U)(sbr & UART_BDL_SBR_MASK);
		
        uartch2->C1 = 0x00;
        uartch2->C3 = 0x00;
        uartch2->S1 = 0x1F;
        uartch2->S2 = 0x00;
		
        uartch2->C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK);                           	
    }
}

/*
*********************************************************************************************************
*                                         BSP_UART_Send1()
*
* Description : Send one character.
*
* Argument(s) : uartNo        UART channel  :  BSP_UART_0
	                                           BSP_UART_1
	                                           BSP_UART_2
*
*	            ch            character to send
*
* Return(s)   : Sending result  :  0(Success)
*                                  1(Fail)
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/
CPU_INT08U  BSP_UART_Send1 (CPU_INT08U uartNo,
                            CPU_INT08U ch)
{
    CPU_INT32U t;       
    UART0_MemMapPtr uartch1 = UART0_BASE_PTR;
    UART_MemMapPtr  uartch2 = UART_ARR[uartNo-1];

    for (t = 0; t < 0xFBBB; t++) {
        if (0 == uartNo) {
            if ((uartch1->S1) & UART_S1_TDRE_MASK) {            /* Check whether the sending buffer is empty            */
				        uartch1->D = ch;                        /* Send the data                                        */
				        break;
			      }
        } else {
		        if ((uartch2->S1) & UART_S1_TDRE_MASK) {
				        uartch2->D = ch;
				        break;
			      }
        }
    }

    if (t >= 0xFBBB) {
        return 1;                                               /* Sending time-out,fail                                */
    } else {
        return 0;                                               /* Sending succeed                                      */
    }
}

/*
*********************************************************************************************************
*                                         BSP_UART_sendN()
*
* Description : Send characters which number is N.
*
* Argument(s) : uartNo        UART channel  :  BSP_UART_0
	                                           BSP_UART_1
	                                           BSP_UART_2
*
*               len           number of sending characters
*
*	            buff          sending buffer
*
* Return(s)   : Sending result  :  0(Success)
*                                  1(Fail)
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/
CPU_INT08U  BSP_UART_sendN (CPU_INT08U uartNo,
                            CPU_INT16U len,
                            CPU_INT08U* buff)
{
    CPU_INT16U i;
    for (i = 0; i < len; i++) {
        if (BSP_UART_Send1(uartNo, buff[i])) {
        	break;        
        }
    }
	
    if (i < len) {
        return 1;
    } else {
        return 0;
    }
}

/*
*********************************************************************************************************
*                                       BSP_UART_Send_String()
*
* Description : Send a string.
*
* Argument(s) : uartNo        UART channel  :  BSP_UART_0
	                                           BSP_UART_1
	                                           BSP_UART_2
*
*	            buff          sending buffer
*
* Return(s)   : Sending result  :  0(Success)
*                                  1(Fail)
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/
CPU_INT08U BSP_UART_Send_String(CPU_INT08U uartNo,
                                void      *buff)
{
    CPU_INT16U i = 0;
    CPU_INT08U *buff_ptr = (CPU_INT08U *)buff;
    for (i = 0; buff_ptr[i] != '\0'; i++) {
        if (BSP_UART_Send1(uartNo, buff_ptr[i])) {
            return 1;
        }
    }
    return 0;
}

/*
*********************************************************************************************************
*                                          BSP_UART_Re1()
*
* Description : Receive one character.
*
* Argument(s) : uartNo        UART channel  :  BSP_UART_0
	                                           BSP_UART_1
	                                           BSP_UART_2
*
*	            fp            pointer of sending result
*
* Return(s)   : Character received.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/
CPU_INT08U BSP_UART_Re1 (CPU_INT08U uartNo,
                         CPU_INT08U *fp)
{
    CPU_INT32U t;
    CPU_INT08U dat;
    UART0_MemMapPtr uartch1 = UART0_BASE_PTR;
    UART_MemMapPtr uartch2  = UART_ARR[uartNo-1];

    for (t = 0; t < 0xFBBB; t++) {
        if (0 == uartNo) {
            if ((uartch1->S1) & UART_S1_RDRF_MASK ) {           /* Check whether the receiving buffer is empty          */
                dat = uartch1->D;                               /* Receive data                                         */
                *fp= 0;
                break;
            }
    	  } else {
            if ((uartch2->S1) & UART_S1_RDRF_MASK ) {
                dat = uartch2->D;
                *fp= 0;
                break;
            }
    	  }
    }
	
    if(t >= 0xFBBB) {
        dat = 0xFF; 
        *fp = 1;
    }
    return dat;
}

/*
*********************************************************************************************************
*                                          BSP_UART_ReN()
*
* Description : Receive one character.
*
* Argument(s) : uartNo        UART channel  :  BSP_UART_0
	                                           BSP_UART_1
	                                           BSP_UART_2
*
*               len           number of characters received
*
*	            buff          receiving buff
*
* Return(s)   : Receiving result  :  0(Success)
*                                    1(Fail)
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/
CPU_INT08U BSP_UART_ReN (CPU_INT08U uartNo,
                         CPU_INT16U len,
                         CPU_INT08U* buff)
{
    CPU_INT16U i;
    CPU_INT08U flag = 0;
    
    for (i = 0; i < len && 0 == flag; i++) {
        buff[i] = BSP_UART_Re1(uartNo, &flag);
    }
    if (i < len) {
        return 1;
    } else {
        return 0;
    }
}

/*
*********************************************************************************************************
*                                        BSP_UART_Enable_Re_Int()
*
* Description : Enable receiving interrupt.
*
* Argument(s) : uartNo        UART channel  :  BSP_UART_0
*                                              BSP_UART_1
*                                              BSP_UART_2
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/
void BSP_UART_Enable_Re_Int(CPU_INT08U uartNo)
{
    UART0_MemMapPtr uartch1 = UART0_BASE_PTR;
    UART_MemMapPtr uartch2  = UART_ARR[uartNo-1];
                                                                /* Enable receiving interrupt                           */
    if (0 == uartNo) {
        uartch1->C2 |= UART_C2_RIE_MASK;
    } else { 
        uartch2->C2 |= UART_C2_RIE_MASK;
    }

    BSP_IntEn (table_irq_uart[uartNo]);                         /* Enable IRQ                                           */
}

/*
*********************************************************************************************************
*                                        BSP_UART_Disable_Re_Int()
*
* Description : Disable receiving interrupt.
*
* Argument(s) : uartNo        UART channel  :  BSP_UART_0
	                                           BSP_UART_1
	                                           BSP_UART_2
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/
void BSP_UART_Disable_Re_Int(CPU_INT08U uartNo)
{
    UART0_MemMapPtr uartch1 = UART0_BASE_PTR; 
    UART_MemMapPtr uartch2  = UART_ARR[uartNo-1];
                                                                /* Disable receiving interrupt                         */
    if (0 == uartNo) {
	      uartch1->C2 &= ~UART_C2_RIE_MASK;
    } else { 
        uartch2->C2 &= ~UART_C2_RIE_MASK;
    }

    BSP_IntDis (table_irq_uart[uartNo]);                        /* Disable IRQ                                          */
}

void uart_printf(CPU_CHAR *fmt,...)
{
    va_list ap;

    CPU_CHAR string[256];

    va_start(ap,fmt);

    vsprintf(string,fmt,ap);

    BSP_UART_Send_String(BSP_UART_DEFAULT, string);

    va_end(ap);
}
