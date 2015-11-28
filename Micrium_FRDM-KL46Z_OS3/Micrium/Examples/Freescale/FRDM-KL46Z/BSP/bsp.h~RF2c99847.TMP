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
*                                        BOARD SUPPORT PACKAGE
*
*                                        Freescale Kinetis KL40
*                                               on the
*                                        Freescale FRDM-KL46Z
*                                          Evaluation Board
*
* Filename      : bsp.h
* Version       : V1.00
* Programmer(s) : JPB
*                 SB
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

#ifndef  BSP_PRESENT
#define  BSP_PRESENT


/*
*********************************************************************************************************
*                                                 EXTERNS
*********************************************************************************************************
*/

#ifdef   BSP_MODULE
#define  BSP_EXT
#else
#define  BSP_EXT  extern
#endif


/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/

#include  <stdarg.h>
#include  <stdio.h>

#include  <cpu.h>
#include  <cpu_core.h>

#include  <lib_ascii.h>
#include  <lib_def.h>
#include  <lib_mem.h>
#include  <lib_str.h>

#include  <app_cfg.h>
#include  <bsp_cfg.h>


/*
*********************************************************************************************************
*                                           NAME DEFINITIONS
*********************************************************************************************************
*/

#define  BSP_LED_ALL               0
#define  BSP_LED_GREEN             1
#define  BSP_LED_RED               2

#define  BSP_SW_1                  1
#define  BSP_SW_2                  2


/*
*********************************************************************************************************
*                                             REGISTERS
*********************************************************************************************************
*/
                                                                /* ------ MULTIPURPOSE CLOCK GENERATOR REGISTERS ------ */
#define  BSP_REG_MCG_BASE_ADDR                  0x40064000u
#define  BSP_REG_MCG_C1                        (*(CPU_REG08 *)(BSP_REG_MCG_BASE_ADDR + 0x00u))
#define  BSP_REG_MCG_C2                        (*(CPU_REG08 *)(BSP_REG_MCG_BASE_ADDR + 0x01u))
#define  BSP_REG_MCG_C5                        (*(CPU_REG08 *)(BSP_REG_MCG_BASE_ADDR + 0x04u))
#define  BSP_REG_MCG_C6                        (*(CPU_REG08 *)(BSP_REG_MCG_BASE_ADDR + 0x05u))
#define  BSP_REG_MCG_S                         (*(CPU_REG08 *)(BSP_REG_MCG_BASE_ADDR + 0x06u))
#define  BSP_REG_MCG_SC                        (*(CPU_REG08 *)(BSP_REG_MCG_BASE_ADDR + 0x08u))
#define  BSP_REG_MCG_C11                       (*(CPU_REG08 *)(BSP_REG_MCG_BASE_ADDR + 0x10u))

                                                                /* -------- SYSTEM INTEGRATION MODULE REGISTERS ------- */
#define  BSP_REG_SIM_BASE_ADDR                  0x40047000u
#define  BSP_REG_SIM_SCGC5                     (*(CPU_REG32 *)(BSP_REG_SIM_BASE_ADDR + 0x1038u))
#define  BSP_REG_SIM_CLKDIV1                   (*(CPU_REG32 *)(BSP_REG_SIM_BASE_ADDR + 0x1044u))

                                                                /* ------------- LOW-POWER TIMER REGISTERS ------------ */
#define  BSP_REG_LPTMR0_BASE_ADDR               0x40040000u
#define  BSP_REG_LPTMR0_CSR                    (*(CPU_REG32 *)(BSP_REG_LPTMR0_BASE_ADDR + 0x0u))
#define  BSP_REG_LPTMR0_PSR                    (*(CPU_REG32 *)(BSP_REG_LPTMR0_BASE_ADDR + 0x4u))
#define  BSP_REG_LPTMR0_CMR                    (*(CPU_REG32 *)(BSP_REG_LPTMR0_BASE_ADDR + 0x8u))
#define  BSP_REG_LPTMR0_CNR                    (*(CPU_REG32 *)(BSP_REG_LPTMR0_BASE_ADDR + 0xCu))


/*
*********************************************************************************************************
*                                            REGISTER BITS
*********************************************************************************************************
*/

#define  DBGMCU_CR_TRACE_IOEN_MASK       0x10                   /* ??? Located in the PPB area only in ST           ??? */
#define  DBGMCU_CR_TRACE_MODE_ASYNC      0x00                   /* ??? Located in the PPB area only in ST           ??? */
#define  DBGMCU_CR_TRACE_MODE_SYNC_01    0x40                   /* ??? Located in the PPB area only in ST           ??? */
#define  DBGMCU_CR_TRACE_MODE_SYNC_02    0x80                   /* ??? Located in the PPB area only in ST           ??? */
#define  DBGMCU_CR_TRACE_MODE_SYNC_04    0xC0                   /* ??? Located in the PPB area only in ST           ??? */
#define  DBGMCU_CR_TRACE_MODE_MASK       0xC0                   /* ??? Located in the PPB area only in ST           ??? */

#define  BSP_SIM_CLKDIV1_OUTDIV1_DIV2          DEF_BIT_MASK(1u, 28u)
#define  BSP_SIM_CLKDIV1_OUTDIV4_DIV2          DEF_BIT_MASK(1u, 16u)

#define  BSP_MCG_C1_CLKS_INT_CLK               DEF_BIT_MASK(1u, 6u)
#define  BSP_MCG_C1_CLKS_EXT_CLK               DEF_BIT_MASK(2u, 6u)
#define  BSP_MCG_C1_CLKS_MASK                  DEF_BIT_FIELD(3u, 6u)
#define  BSP_MCG_C1_FRDIV_1                    DEF_BIT_NONE
#define  BSP_MCG_C1_FRDIV_2                    DEF_BIT_MASK(1u, 3u)
#define  BSP_MCG_C1_FRDIV_4                    DEF_BIT_MASK(2u, 3u)
#define  BSP_MCG_C1_FRDIV_8                    DEF_BIT_MASK(3u, 3u)
#define  BSP_MCG_C1_FRDIV_16                   DEF_BIT_MASK(4u, 3u)
#define  BSP_MCG_C1_FRDIV_32                   DEF_BIT_MASK(5u, 3u)
#define  BSP_MCG_C1_FRDIV_64                   DEF_BIT_MASK(6u, 3u)
#define  BSP_MCG_C1_FRDIV_128                  DEF_BIT_MASK(7u, 3u)
#define  BSP_MCG_C1_IREFS                      DEF_BIT_02
#define  BSP_MCG_C1_IRCLKEN                    DEF_BIT_01
#define  BSP_MCG_C1_IREFSTEN                   DEF_BIT_00

#define  BSP_MCG_C2_LOCRE0                     DEF_BIT_MASK(1u, 7u)
#define  BSP_MCG_C2_RANGE_LOW_FREQ             DEF_BIT_NONE
#define  BSP_MCG_C2_RANGE_HIGH_FREQ            DEF_BIT_MASK(1u, 4u)
#define  BSP_MCG_C2_EREFS0                     DEF_BIT_MASK(1u, 2u)

#define  BSP_MCG_C5_PLLREFSEL0                 DEF_BIT_07
#define  BSP_MCG_C5_PLLCLKEN0                  DEF_BIT_06
#define  BSP_MCG_C5_PLLSTEN0                   DEF_BIT_05
#define  BSP_MCG_C5_PRDIV0_1                   DEF_BIT_NONE
#define  BSP_MCG_C5_PRDIV0_2                   DEF_BIT_MASK(1u, 0u)
#define  BSP_MCG_C5_PRDIV0_3                   DEF_BIT_MASK(2u, 0u)
#define  BSP_MCG_C5_PRDIV0_4                   DEF_BIT_MASK(3u, 0u)
#define  BSP_MCG_C5_PRDIV0_5                   DEF_BIT_MASK(4u, 0u)
#define  BSP_MCG_C5_PRDIV0_6                   DEF_BIT_MASK(5u, 0u)
#define  BSP_MCG_C5_PRDIV0_7                   DEF_BIT_MASK(6u, 0u)
#define  BSP_MCG_C5_PRDIV0_8                   DEF_BIT_MASK(7u, 0u)

#define  BSP_MCG_C6_LOLIE0                     DEF_BIT_07
#define  BSP_MCG_C6_PLLS                       DEF_BIT_06
#define  BSP_MCG_C6_CME0                       DEF_BIT_05

#define  BSP_MCG_C6_VDIV0_MUL_24               DEF_BIT_NONE
#define  BSP_MCG_C6_VDIV0_MUL_25               DEF_BIT_MASK( 1u, 0u)
#define  BSP_MCG_C6_VDIV0_MUL_26               DEF_BIT_MASK( 2u, 0u)
#define  BSP_MCG_C6_VDIV0_MUL_27               DEF_BIT_MASK( 3u, 0u)
#define  BSP_MCG_C6_VDIV0_MUL_28               DEF_BIT_MASK( 4u, 0u)
#define  BSP_MCG_C6_VDIV0_MUL_29               DEF_BIT_MASK( 5u, 0u)
#define  BSP_MCG_C6_VDIV0_MUL_30               DEF_BIT_MASK( 6u, 0u)
#define  BSP_MCG_C6_VDIV0_MUL_31               DEF_BIT_MASK( 7u, 0u)
#define  BSP_MCG_C6_VDIV0_MUL_32               DEF_BIT_MASK( 8u, 0u)
#define  BSP_MCG_C6_VDIV0_MUL_33               DEF_BIT_MASK( 9u, 0u)
#define  BSP_MCG_C6_VDIV0_MUL_34               DEF_BIT_MASK(10u, 0u)
#define  BSP_MCG_C6_VDIV0_MUL_35               DEF_BIT_MASK(11u, 0u)
#define  BSP_MCG_C6_VDIV0_MUL_36               DEF_BIT_MASK(12u, 0u)
#define  BSP_MCG_C6_VDIV0_MUL_37               DEF_BIT_MASK(13u, 0u)
#define  BSP_MCG_C6_VDIV0_MUL_38               DEF_BIT_MASK(14u, 0u)
#define  BSP_MCG_C6_VDIV0_MUL_39               DEF_BIT_MASK(15u, 0u)
#define  BSP_MCG_C6_VDIV0_MUL_40               DEF_BIT_MASK(16u, 0u)
#define  BSP_MCG_C6_VDIV0_MUL_41               DEF_BIT_MASK(17u, 0u)
#define  BSP_MCG_C6_VDIV0_MUL_42               DEF_BIT_MASK(18u, 0u)
#define  BSP_MCG_C6_VDIV0_MUL_43               DEF_BIT_MASK(19u, 0u)
#define  BSP_MCG_C6_VDIV0_MUL_44               DEF_BIT_MASK(20u, 0u)
#define  BSP_MCG_C6_VDIV0_MUL_45               DEF_BIT_MASK(21u, 0u)
#define  BSP_MCG_C6_VDIV0_MUL_46               DEF_BIT_MASK(22u, 0u)
#define  BSP_MCG_C6_VDIV0_MUL_47               DEF_BIT_MASK(23u, 0u)
#define  BSP_MCG_C6_VDIV0_MUL_48               DEF_BIT_MASK(24u, 0u)
#define  BSP_MCG_C6_VDIV0_MUL_49               DEF_BIT_MASK(25u, 0u)
#define  BSP_MCG_C6_VDIV0_MUL_50               DEF_BIT_MASK(26u, 0u)
#define  BSP_MCG_C6_VDIV0_MUL_51               DEF_BIT_MASK(27u, 0u)
#define  BSP_MCG_C6_VDIV0_MUL_52               DEF_BIT_MASK(28u, 0u)
#define  BSP_MCG_C6_VDIV0_MUL_53               DEF_BIT_MASK(29u, 0u)
#define  BSP_MCG_C6_VDIV0_MUL_54               DEF_BIT_MASK(30u, 0u)
#define  BSP_MCG_C6_VDIV0_MUL_55               DEF_BIT_MASK(31u, 0u)


#define  BSP_MCG_C11_PLLCS                     DEF_BIT_04

#define  BSP_MCG_S_LOLS0                       DEF_BIT_07
#define  BSP_MCG_S_LOCK0                       DEF_BIT_06
#define  BSP_MCG_S_PLLST                       DEF_BIT_05
#define  BSP_MCG_S_IREFST                      DEF_BIT_04
#define  BSP_MCG_S_CLKST_FLL                   DEF_BIT_NONE
#define  BSP_MCG_S_CLKST_IREF_CLK              DEF_BIT_MASK(1u, 2u)
#define  BSP_MCG_S_CLKST_EREF_CLK              DEF_BIT_MASK(2u, 2u)
#define  BSP_MCG_S_CLKST_PLL                   DEF_BIT_MASK(3u, 2u)
#define  BSP_MCG_S_CLKST_MASK                  DEF_BIT_FIELD(3u, 2u)
#define  BSP_MCG_S_OSCINIT0                    DEF_BIT_01
#define  BSP_MCG_S_IRCST                       DEF_BIT_00


/*
*********************************************************************************************************
*                                          TPM DEFINITIONS
*********************************************************************************************************
*/

#define  TPM_Cn_MODE                          (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK)
#define  TPM_MODULE                            1000
#define  TPM_INIT_VAL                            25


/*
*********************************************************************************************************
*                                          GPIO PIN DEFINITIONS
*********************************************************************************************************
*/

                                                                /* -------------------- GPIOA PINS -------------------- */
#define  BSP_GPIOA_PIN_00                    DEF_BIT_00
#define  BSP_GPIOA_PIN_01                    DEF_BIT_01
#define  BSP_GPIOA_PIN_02                    DEF_BIT_02
#define  BSP_GPIOA_PIN_03                    DEF_BIT_03
#define  BSP_GPIOA_PIN_04                    DEF_BIT_04
#define  BSP_GPIOA_PIN_05                    DEF_BIT_05
#define  BSP_GPIOA_PIN_06                    DEF_BIT_06
#define  BSP_GPIOA_PIN_07                    DEF_BIT_07
#define  BSP_GPIOA_PIN_08                    DEF_BIT_08
#define  BSP_GPIOA_PIN_09                    DEF_BIT_09
#define  BSP_GPIOA_PIN_10                    DEF_BIT_10
#define  BSP_GPIOA_PIN_11                    DEF_BIT_11
#define  BSP_GPIOA_PIN_12                    DEF_BIT_12
#define  BSP_GPIOA_PIN_13                    DEF_BIT_13
#define  BSP_GPIOA_PIN_14                    DEF_BIT_14
#define  BSP_GPIOA_PIN_15                    DEF_BIT_15
#define  BSP_GPIOA_PIN_16                    DEF_BIT_16
#define  BSP_GPIOA_PIN_17                    DEF_BIT_17
#define  BSP_GPIOA_PIN_18                    DEF_BIT_18
#define  BSP_GPIOA_PIN_19                    DEF_BIT_19
#define  BSP_GPIOA_PIN_20                    DEF_BIT_20
#define  BSP_GPIOA_PIN_21                    DEF_BIT_21
#define  BSP_GPIOA_PIN_22                    DEF_BIT_22
#define  BSP_GPIOA_PIN_23                    DEF_BIT_23
#define  BSP_GPIOA_PIN_24                    DEF_BIT_24
#define  BSP_GPIOA_PIN_25                    DEF_BIT_25
#define  BSP_GPIOA_PIN_26                    DEF_BIT_26
#define  BSP_GPIOA_PIN_27                    DEF_BIT_27
#define  BSP_GPIOA_PIN_28                    DEF_BIT_28
#define  BSP_GPIOA_PIN_29                    DEF_BIT_29
#define  BSP_GPIOA_PIN_30                    DEF_BIT_30
#define  BSP_GPIOA_PIN_31                    DEF_BIT_31


                                                                /* -------------------- GPIOB PINS -------------------- */
#define  BSP_GPIOB_PIN_00                    DEF_BIT_00
#define  BSP_GPIOB_PIN_01                    DEF_BIT_01
#define  BSP_GPIOB_PIN_02                    DEF_BIT_02
#define  BSP_GPIOB_PIN_03                    DEF_BIT_03
#define  BSP_GPIOB_PIN_04                    DEF_BIT_04
#define  BSP_GPIOB_PIN_05                    DEF_BIT_05
#define  BSP_GPIOB_PIN_06                    DEF_BIT_06
#define  BSP_GPIOB_PIN_07                    DEF_BIT_07
#define  BSP_GPIOB_PIN_08                    DEF_BIT_08
#define  BSP_GPIOB_PIN_09                    DEF_BIT_09
#define  BSP_GPIOB_PIN_10                    DEF_BIT_10
#define  BSP_GPIOB_PIN_11                    DEF_BIT_11
#define  BSP_GPIOB_PIN_12                    DEF_BIT_12
#define  BSP_GPIOB_PIN_13                    DEF_BIT_13
#define  BSP_GPIOB_PIN_14                    DEF_BIT_14
#define  BSP_GPIOB_PIN_15                    DEF_BIT_15
#define  BSP_GPIOB_PIN_16                    DEF_BIT_16
#define  BSP_GPIOB_PIN_17                    DEF_BIT_17
#define  BSP_GPIOB_PIN_18                    DEF_BIT_18
#define  BSP_GPIOB_PIN_19                    DEF_BIT_19
#define  BSP_GPIOB_PIN_20                    DEF_BIT_20
#define  BSP_GPIOB_PIN_21                    DEF_BIT_21
#define  BSP_GPIOB_PIN_22                    DEF_BIT_22
#define  BSP_GPIOB_PIN_23                    DEF_BIT_23
#define  BSP_GPIOB_PIN_24                    DEF_BIT_24
#define  BSP_GPIOB_PIN_25                    DEF_BIT_25
#define  BSP_GPIOB_PIN_26                    DEF_BIT_26
#define  BSP_GPIOB_PIN_27                    DEF_BIT_27
#define  BSP_GPIOB_PIN_28                    DEF_BIT_28
#define  BSP_GPIOB_PIN_29                    DEF_BIT_29
#define  BSP_GPIOB_PIN_30                    DEF_BIT_30
#define  BSP_GPIOB_PIN_31                    DEF_BIT_31

                                                                /* -------------------- GPIOC PINS -------------------- */
#define  BSP_GPIOC_PIN_00                    DEF_BIT_00
#define  BSP_GPIOC_PIN_01                    DEF_BIT_01
#define  BSP_GPIOC_PIN_02                    DEF_BIT_02
#define  BSP_GPIOC_SW_1                      DEF_BIT_03         /* Switch 1.                                            */
#define  BSP_GPIOC_PIN_04                    DEF_BIT_04
#define  BSP_GPIOC_PIN_05                    DEF_BIT_05
#define  BSP_GPIOC_PIN_06                    DEF_BIT_06
#define  BSP_GPIOC_PIN_07                    DEF_BIT_07
#define  BSP_GPIOC_PIN_08                    DEF_BIT_08
#define  BSP_GPIOC_PIN_09                    DEF_BIT_09
#define  BSP_GPIOC_PIN_10                    DEF_BIT_10
#define  BSP_GPIOC_PIN_11                    DEF_BIT_11
#define  BSP_GPIOC_SW_2                      DEF_BIT_12         /* Switch 2.                                            */
#define  BSP_GPIOC_PIN_13                    DEF_BIT_13
#define  BSP_GPIOC_PIN_14                    DEF_BIT_14
#define  BSP_GPIOC_PIN_15                    DEF_BIT_15
#define  BSP_GPIOC_PIN_16                    DEF_BIT_16
#define  BSP_GPIOC_PIN_17                    DEF_BIT_17
#define  BSP_GPIOC_PIN_18                    DEF_BIT_18
#define  BSP_GPIOC_PIN_19                    DEF_BIT_19
#define  BSP_GPIOC_PIN_20                    DEF_BIT_20
#define  BSP_GPIOC_PIN_21                    DEF_BIT_21
#define  BSP_GPIOC_PIN_22                    DEF_BIT_22
#define  BSP_GPIOC_PIN_23                    DEF_BIT_23
#define  BSP_GPIOC_PIN_24                    DEF_BIT_24
#define  BSP_GPIOC_PIN_25                    DEF_BIT_25
#define  BSP_GPIOC_PIN_26                    DEF_BIT_26
#define  BSP_GPIOC_PIN_27                    DEF_BIT_27
#define  BSP_GPIOC_PIN_28                    DEF_BIT_28
#define  BSP_GPIOC_PIN_29                    DEF_BIT_29
#define  BSP_GPIOC_PIN_30                    DEF_BIT_30
#define  BSP_GPIOC_PIN_31                    DEF_BIT_31

                                                                /* -------------------- GPIOD PINS -------------------- */
#define  BSP_GPIOD_PIN_00                    DEF_BIT_00
#define  BSP_GPIOD_PIN_01                    DEF_BIT_01
#define  BSP_GPIOD_PIN_02                    DEF_BIT_02
#define  BSP_GPIOD_PIN_03                    DEF_BIT_03
#define  BSP_GPIOD_PIN_04                    DEF_BIT_04
#define  BSP_GPIOD_LED_GREEN                 DEF_BIT_05         /* Green LED.                                           */
#define  BSP_GPIOD_PIN_06                    DEF_BIT_06
#define  BSP_GPIOD_PIN_07                    DEF_BIT_07
#define  BSP_GPIOD_PIN_08                    DEF_BIT_08
#define  BSP_GPIOD_PIN_09                    DEF_BIT_09
#define  BSP_GPIOD_PIN_10                    DEF_BIT_10
#define  BSP_GPIOD_PIN_11                    DEF_BIT_11
#define  BSP_GPIOD_PIN_12                    DEF_BIT_12
#define  BSP_GPIOD_PIN_13                    DEF_BIT_13
#define  BSP_GPIOD_PIN_14                    DEF_BIT_14
#define  BSP_GPIOD_PIN_15                    DEF_BIT_15
#define  BSP_GPIOD_PIN_16                    DEF_BIT_16
#define  BSP_GPIOD_PIN_17                    DEF_BIT_17
#define  BSP_GPIOD_PIN_18                    DEF_BIT_18
#define  BSP_GPIOD_PIN_19                    DEF_BIT_19
#define  BSP_GPIOD_PIN_20                    DEF_BIT_20
#define  BSP_GPIOD_PIN_21                    DEF_BIT_21
#define  BSP_GPIOD_PIN_22                    DEF_BIT_22
#define  BSP_GPIOD_PIN_23                    DEF_BIT_23
#define  BSP_GPIOD_PIN_24                    DEF_BIT_24
#define  BSP_GPIOD_PIN_25                    DEF_BIT_25
#define  BSP_GPIOD_PIN_26                    DEF_BIT_26
#define  BSP_GPIOD_PIN_27                    DEF_BIT_27
#define  BSP_GPIOD_PIN_28                    DEF_BIT_28
#define  BSP_GPIOD_PIN_29                    DEF_BIT_29
#define  BSP_GPIOD_PIN_30                    DEF_BIT_30
#define  BSP_GPIOD_PIN_31                    DEF_BIT_31

                                                                /* -------------------- GPIOE PINS -------------------- */
#define  BSP_GPIOE_PIN_00                    DEF_BIT_00
#define  BSP_GPIOE_PIN_01                    DEF_BIT_01
#define  BSP_GPIOE_PIN_02                    DEF_BIT_02
#define  BSP_GPIOE_PIN_03                    DEF_BIT_03
#define  BSP_GPIOE_PIN_04                    DEF_BIT_04
#define  BSP_GPIOE_PIN_05                    DEF_BIT_05
#define  BSP_GPIOE_PIN_06                    DEF_BIT_06
#define  BSP_GPIOE_PIN_07                    DEF_BIT_07
#define  BSP_GPIOE_PIN_08                    DEF_BIT_08
#define  BSP_GPIOE_PIN_09                    DEF_BIT_09
#define  BSP_GPIOE_PIN_10                    DEF_BIT_10
#define  BSP_GPIOE_PIN_11                    DEF_BIT_11
#define  BSP_GPIOE_PIN_12                    DEF_BIT_12
#define  BSP_GPIOE_PIN_13                    DEF_BIT_13
#define  BSP_GPIOE_PIN_14                    DEF_BIT_14
#define  BSP_GPIOE_PIN_15                    DEF_BIT_15
#define  BSP_GPIOE_PIN_16                    DEF_BIT_16
#define  BSP_GPIOE_PIN_17                    DEF_BIT_17
#define  BSP_GPIOE_PIN_18                    DEF_BIT_18
#define  BSP_GPIOE_PIN_19                    DEF_BIT_19
#define  BSP_GPIOE_PIN_20                    DEF_BIT_20
#define  BSP_GPIOE_PIN_21                    DEF_BIT_21
#define  BSP_GPIOE_PIN_22                    DEF_BIT_22
#define  BSP_GPIOE_PIN_23                    DEF_BIT_23
#define  BSP_GPIOE_PIN_24                    DEF_BIT_24
#define  BSP_GPIOE_PIN_25                    DEF_BIT_25
#define  BSP_GPIOE_PIN_26                    DEF_BIT_26
#define  BSP_GPIOE_PIN_27                    DEF_BIT_27
#define  BSP_GPIOE_PIN_28                    DEF_BIT_28
#define  BSP_GPIOE_LED_RED                   DEF_BIT_29         /* Red LED.                                             */
#define  BSP_GPIOE_PIN_30                    DEF_BIT_30
#define  BSP_GPIOE_PIN_31                    DEF_BIT_31


/*
*********************************************************************************************************
*                                               INT DEFINES
*********************************************************************************************************
*/

#define  BSP_INT_ID_DMA0                                  0u    /* DMA Channel 0 Transfer Complete                      */
#define  BSP_INT_ID_DMA1                                  1u    /* DMA Channel 1 Transfer Complete                      */
#define  BSP_INT_ID_DMA2                                  2u    /* DMA Channel 2 Transfer Complete                      */
#define  BSP_INT_ID_DMA3                                  3u    /* DMA Channel 3 Transfer Complete                      */
#define  BSP_INT_ID_RSVD4                                 4u
#define  BSP_INT_ID_FTFA                                  5u    /* Flash memory.                                        */
#define  BSP_INT_ID_LVD                                   6u    /* Low-voltage detect, low-voltage warning.             */
#define  BSP_INT_ID_LLWU                                  7u    /* Low Leakage Wakeup                                   */
#define  BSP_INT_ID_I2C0                                  8u    /* I2C0 interrupt                                       */
#define  BSP_INT_ID_I2C1                                  9u    /* I2C1 interrupt                                       */
#define  BSP_INT_ID_SPI0                                 10u    /* Single interrupt vector for all sources              */
#define  BSP_INT_ID_SPI1                                 11u    /* Single interrupt vector for all sources              */
#define  BSP_INT_ID_UART0_RX_TX                          12u    /* UART0 Receive/Transmit interrupt                     */
#define  BSP_INT_ID_UART1_RX_TX                          13u    /* UART1 Receive/Transmit interrupt                     */
#define  BSP_INT_ID_UART2_RX_TX                          14u    /* UART2 Receive/Transmit interrupt                     */
#define  BSP_INT_ID_ADC0                                 15u    /* ADC0 interrupt                                       */
#define  BSP_INT_ID_CMP0                                 16u    /* CMP0 interrupt                                       */
#define  BSP_INT_ID_TPM0                                 17u    /* TPM0 fault, overflow and channels interrupt          */
#define  BSP_INT_ID_TPM1                                 18u    /* TPM1 fault, overflow and channels interrupt          */
#define  BSP_INT_ID_TPM2                                 19u    /* TPM2 fault, overflow and channels interrupt          */
#define  BSP_INT_ID_RTC_ALARM                            20u    /* RTC alarm interrupt                                  */
#define  BSP_INT_ID_RTC_SECONDS                          21u    /* RTC seconds interrupt                                */
#define  BSP_INT_ID_PIT                                  22u    /* PIT timer interrupt                                  */
#define  BSP_INT_ID_I2S0                                 23u    /* I2S0 interrupt                                       */
#define  BSP_INT_ID_USB_OTG                              24u    /* USB OTG interrupt                                    */
#define  BSP_INT_ID_DAC0                                 25u    /* DAC0 interrupt                                       */
#define  BSP_INT_ID_TSI0                                 26u    /* TSI0 interrupt                                       */
#define  BSP_INT_ID_MCG                                  27u    /* MCG Interrupt                                        */
#define  BSP_INT_ID_LPTMR0                               28u    /* LPTMR0 interrupt                                     */
#define  BSP_INT_ID_SLCD                                 29u    /* SLCD interrupt                                       */
#define  BSP_INT_ID_PORT_A                               30u    /* Port A interrupt                                     */
#define  BSP_INT_ID_PORT_C_D                             31u    /* Port C and Port D interrupt                          */


/*
*********************************************************************************************************
*                                               TSI MODULE
*********************************************************************************************************
*/

#define  BSP_TSI_MAX_ELECTRODES     2u                          /* The TSI module supports up to 16 input capacitive    */
                                                                /* touch sensing pins.  This board uses 2 of them.      */

#define  BSP_TSI_PIN9               9u                          /* This board uses TSI pins 9 and 10.                   */
#define  BSP_TSI_PIN10             10u

#define  BSP_TSI_THRESHOLD        100u

#define  BSP_TSI_NO_TOUCH           0u
#define  BSP_TSI_SLIDER_LEN        40u                          /* Length of the electrode in millimeters.              */


typedef  struct {
    CPU_INT08U   PinNbr;                                        /* Pin number in the TSI module.                        */
    CPU_INT16U   Threshold;
    CPU_INT16U   RawCnt;                                        /* Electrode reading.                                   */
    CPU_INT16U   Delta;                                         /* Difference between readings.                         */
    CPU_INT16U   Baseline;                                      /* Electrode reading when not being touched.            */
    CPU_INT08U   PosPct;                                        /* Position in terms of percentage.                     */
    CPU_INT08U   PosMm;                                         /* Position in terms of millimeters.                    */
} BSP_TSI_ELECTRODE;


typedef  struct {
    CPU_INT08U         CurElectrode;                            /* TSI pin number of current electrode being scanned.   */
    CPU_INT08U         EndFlg;
    BSP_TSI_ELECTRODE  Electrodes[BSP_TSI_MAX_ELECTRODES];
} BSP_TSI_SLIDER;


/*
*********************************************************************************************************
*                                            GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                                 MACRO'S
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void        BSP_Init        (void);

void        BSP_IntDisAll   (void);

void        BSP_Tick_Init   (void);

CPU_INT32U  BSP_CPU_ClkFreq (void);


/*
*********************************************************************************************************
*                                           INTERRUPT SERVICES
*********************************************************************************************************
*/

void         BSP_IntInit    (void);

void         BSP_IntEn      (CPU_DATA       int_id);

void         BSP_IntDis     (CPU_DATA       int_id);

void         BSP_IntClr     (CPU_DATA       int_id);

void         BSP_IntVectSet (CPU_DATA       int_id,
                             CPU_FNCT_VOID  isr);

void         BSP_IntPrioSet (CPU_DATA       int_id,
                             CPU_INT08U     prio);

void         BSP_IntHandlerDMA0             (void);
void         BSP_IntHandlerDMA1             (void);
void         BSP_IntHandlerDMA2             (void);
void         BSP_IntHandlerDMA3             (void);
void         BSP_IntHandlerFTFA             (void);
void         BSP_IntHandlerLVD              (void);
void         BSP_IntHandlerLLWU             (void);
void         BSP_IntHandlerI2C0             (void);
void         BSP_IntHandlerI2C1             (void);
void         BSP_IntHandlerSPI0             (void);
void         BSP_IntHandlerSPI1             (void);

void         BSP_IntHandlerRSVD4            (void);

void         BSP_IntHandlerUART0_RX_TX      (void);
void         BSP_IntHandlerUART1_RX_TX      (void);
void         BSP_IntHandlerUART2_RX_TX      (void);
void         BSP_IntHandlerADC0             (void);
void         BSP_IntHandlerCMP0             (void);
void         BSP_IntHandlerTPM0             (void);
void         BSP_IntHandlerTPM1             (void);
void         BSP_IntHandlerTPM2             (void);
void         BSP_IntHandlerRTC_ALARM        (void);
void         BSP_IntHandlerRTC_SECONDS      (void);
void         BSP_IntHandlerPIT              (void);

void         BSP_IntHandlerI2S0             (void);

void         BSP_IntHandlerUSB_OTG          (void);
void         BSP_IntHandlerDAC0             (void);
void         BSP_IntHandlerTSI0             (void);
void         BSP_IntHandlerMCG              (void);
void         BSP_IntHandlerLPTMR0           (void);
void         BSP_IntHandlerSLCD             (void);
void         BSP_IntHandlerPORT_A           (void);
void         BSP_IntHandlerPORT_C_D         (void);


/*
*********************************************************************************************************
*                                     PERIPHERAL POWER/CLOCK SERVICES
*********************************************************************************************************
*/

CPU_INT32U  BSP_PeriphClkFreqGet (CPU_DATA    pwr_clk_id);

void        BSP_PeriphEn         (CPU_DATA    pwr_clk_id);

void        BSP_PeriphDis        (CPU_DATA    pwr_clk_id);


/*
*********************************************************************************************************
*                                              LED SERVICES
*********************************************************************************************************
*/

void  BSP_LED_On     (CPU_INT08U led);

void  BSP_LED_Off    (CPU_INT08U led);

void  BSP_LED_Toggle (CPU_INT08U led);


/*
*********************************************************************************************************
*                                             SW SERVICES
*********************************************************************************************************
*/

CPU_INT08U  BSP_SW_Read (CPU_INT08S  sw);


/*
*********************************************************************************************************
*                                              TSI SERVICES
*********************************************************************************************************
*/

void        BSP_TSI_SliderRead (CPU_INT08U *slider_pos_mm, CPU_INT08U *slider_pos_pct);


/*
*********************************************************************************************************
*                                             TIMER SERVICES
*********************************************************************************************************
*/

void  BSP_Timer_Dly (CPU_INT16U  ms,
                     CPU_INT16U  us);


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif                                                          /* End of module include.                               */
