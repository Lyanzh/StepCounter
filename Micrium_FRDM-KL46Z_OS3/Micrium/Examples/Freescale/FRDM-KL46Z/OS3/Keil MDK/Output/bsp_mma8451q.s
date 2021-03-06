; generated by Component: ARM Compiler 5.05 update 2 (build 169) Tool: ArmCC [4d0f38]
; commandline ArmCC [--debug -c -S -o.\output\bsp_mma8451q.s --depend=.\output\bsp_mma8451q.d --cpu=Cortex-M0+ --apcs=interwork -O0 --diag_suppress=9931 -I..\ -I..\..\BSP -I..\..\BSP\KeilMDK -I..\..\BSP\OS\uCOS-III -I..\..\BSP\Freescale -I..\..\..\..\..\Software\uC-CPU -I..\..\..\..\..\Software\uC-CPU\ARM-Cortex-M0\RealView -I..\..\..\..\..\Software\uC-LIB -I..\..\..\..\..\Software\uC-LIB\Ports\ARM-Cortex-M0\Generic\RealView -I..\..\..\..\..\Software\uCOS-III\Source -I..\..\..\..\..\Software\uCOS-III\Ports\ARM-Cortex-M0\Generic\RealView -I"C:\Users\Administrator\Desktop\learning_20151113\KL46_StepCounter\Micrium_FRDM-KL46Z_OS3\Micrium\Examples\Freescale\FRDM-KL46Z\OS3\Keil MDK\RTE" -ID:\Keil_v5\ARM\PACK\Keil\Kinetis_KLxx_DFP\1.8.0\Device\Include -ID:\Keil_v5\ARM\CMSIS\Include -D__UVISION_VERSION=515 -DMKL46Z256xxx4 --omf_browse=.\output\bsp_mma8451q.crf ..\..\BSP\bsp_mma8451q.c]
        THUMB
        REQUIRE8
        PRESERVE8

        AREA ||.text||, CODE, READONLY, ALIGN=2

        REQUIRE _printf_percent
        REQUIRE _printf_d
        REQUIRE _printf_int_dec
pause PROC
        MOVS     r0,#0
        B        |L0.10|
|L0.4|
        NOP      
        ADDS     r1,r0,#1
        UXTB     r0,r1
|L0.10|
        CMP      r0,#0x28
        BLT      |L0.4|
        BX       lr
        ENDP

BSP_MMA8451_Write_Reg PROC
        PUSH     {r4-r6,lr}
        MOV      r4,r0
        MOV      r5,r1
        MOVS     r0,#0
        BL       BSP_I2C_Start
        MOVS     r1,#0x3a
        MOVS     r0,#0
        BL       BSP_I2C_Write_Byte
        MOVS     r0,#0
        BL       BSP_I2C_Wait
        MOVS     r0,#0
        BL       BSP_I2C_Get_Ack
        MOV      r1,r4
        MOVS     r0,#0
        BL       BSP_I2C_Write_Byte
        MOVS     r0,#0
        BL       BSP_I2C_Wait
        MOVS     r0,#0
        BL       BSP_I2C_Get_Ack
        MOV      r1,r5
        MOVS     r0,#0
        BL       BSP_I2C_Write_Byte
        MOVS     r0,#0
        BL       BSP_I2C_Wait
        MOVS     r0,#0
        BL       BSP_I2C_Get_Ack
        MOVS     r0,#0
        BL       BSP_I2C_Stop
        BL       pause
        POP      {r4-r6,pc}
        ENDP

BSP_MMA8451_Read_Reg PROC
        PUSH     {r4-r6,lr}
        MOV      r4,r0
        MOVS     r0,#0
        BL       BSP_I2C_Start
        MOVS     r1,#0x3a
        MOVS     r0,#0
        BL       BSP_I2C_Write_Byte
        MOVS     r0,#0
        BL       BSP_I2C_Wait
        MOVS     r0,#0
        BL       BSP_I2C_Get_Ack
        MOV      r1,r4
        MOVS     r0,#0
        BL       BSP_I2C_Write_Byte
        MOVS     r0,#0
        BL       BSP_I2C_Wait
        MOVS     r0,#0
        BL       BSP_I2C_Get_Ack
        MOVS     r0,#0
        BL       BSP_I2C_Repeated_Start
        MOVS     r1,#0x3b
        MOVS     r0,#0
        BL       BSP_I2C_Write_Byte
        MOVS     r0,#0
        BL       BSP_I2C_Wait
        MOVS     r0,#0
        BL       BSP_I2C_Get_Ack
        MOVS     r0,#0
        BL       BSP_I2C_Set_RX_Mode
        MOVS     r0,#0
        BL       BSP_I2C_Give_Nack
        MOVS     r0,#0
        BL       BSP_I2C_Read_Byte
        MOV      r5,r0
        MOVS     r0,#0
        BL       BSP_I2C_Wait
        MOVS     r0,#0
        BL       BSP_I2C_Stop
        MOVS     r0,#0
        BL       BSP_I2C_Read_Byte
        MOV      r5,r0
        BL       pause
        MOV      r0,r5
        POP      {r4-r6,pc}
        ENDP

BSP_MMA8451_Init PROC
        PUSH     {r4,lr}
        MOVS     r0,#0
        BL       BSP_I2C_Init
        MOVS     r0,#0x2a
        BL       BSP_MMA8451_Read_Reg
        MOV      r4,r0
        LSRS     r1,r4,#1
        LSLS     r1,r1,#1
        MOVS     r0,#0x2a
        BL       BSP_MMA8451_Write_Reg
        POP      {r4,pc}
        ENDP

mma8451_active PROC
        PUSH     {r4-r6,lr}
        MOV      r4,r0
        CMP      r4,#2
        BNE      |L0.268|
        CMP      r4,#4
        BNE      |L0.268|
        CMP      r4,#8
        BEQ      |L0.270|
|L0.268|
        MOVS     r4,#2
|L0.270|
        CMP      r4,#2
        BNE      |L0.294|
        MOVS     r0,#0xe
        BL       BSP_MMA8451_Read_Reg
        LSRS     r5,r0,#2
        LSLS     r5,r5,#2
        MOV      r1,r5
        MOVS     r0,#0xe
        BL       BSP_MMA8451_Write_Reg
        B        |L0.376|
|L0.294|
        CMP      r4,#4
        BNE      |L0.338|
        MOVS     r0,#0xe
        BL       BSP_MMA8451_Read_Reg
        LSRS     r5,r0,#2
        LSLS     r5,r5,#2
        MOV      r1,r5
        MOVS     r0,#0xe
        BL       BSP_MMA8451_Write_Reg
        MOVS     r0,#0xe
        BL       BSP_MMA8451_Read_Reg
        MOVS     r1,#1
        ORRS     r0,r0,r1
        MOV      r5,r0
        MOV      r1,r5
        MOVS     r0,#0xe
        BL       BSP_MMA8451_Write_Reg
        B        |L0.376|
|L0.338|
        MOVS     r0,#0xe
        BL       BSP_MMA8451_Read_Reg
        LSRS     r5,r0,#2
        LSLS     r5,r5,#2
        MOV      r1,r5
        MOVS     r0,#0xe
        BL       BSP_MMA8451_Write_Reg
        MOVS     r0,#0xe
        BL       BSP_MMA8451_Read_Reg
        MOVS     r1,#2
        ORRS     r0,r0,r1
        MOV      r5,r0
        MOV      r1,r5
        MOVS     r0,#0xe
        BL       BSP_MMA8451_Write_Reg
|L0.376|
        MOVS     r0,#0x2a
        BL       BSP_MMA8451_Read_Reg
        MOVS     r1,#1
        ORRS     r0,r0,r1
        MOV      r5,r0
        MOV      r1,r5
        MOVS     r0,#0x2a
        BL       BSP_MMA8451_Write_Reg
        POP      {r4-r6,pc}
        ENDP

accel_read PROC
        PUSH     {r4,lr}
        MOVS     r0,#0
        BL       BSP_MMA8451_Read_Reg
        LSLS     r0,r0,#28
        LSRS     r0,r0,#28
        CMP      r0,#0
        BEQ      |L0.666|
        MOVS     r0,#1
        BL       BSP_MMA8451_Read_Reg
        LSLS     r0,r0,#8
        SXTH     r0,r0
        LDR      r1,|L0.1068|
        STRH     r0,[r1,#0]
        MOVS     r0,#2
        BL       BSP_MMA8451_Read_Reg
        LDR      r1,|L0.1068|
        MOVS     r2,#0
        LDRSH    r2,[r1,r2]  ; accel_x
        ORRS     r0,r0,r2
        STRH     r0,[r1,#0]
        MOV      r0,r1
        MOVS     r1,#0
        LDRSH    r1,[r0,r1]  ; accel_x
        ASRS     r0,r1,#2
        LDR      r1,|L0.1068|
        STRH     r0,[r1,#0]
        MOVS     r0,#3
        BL       BSP_MMA8451_Read_Reg
        LSLS     r0,r0,#8
        SXTH     r0,r0
        LDR      r1,|L0.1072|
        STRH     r0,[r1,#0]
        MOVS     r0,#4
        BL       BSP_MMA8451_Read_Reg
        LDR      r1,|L0.1072|
        MOVS     r2,#0
        LDRSH    r2,[r1,r2]  ; accel_y
        ORRS     r0,r0,r2
        STRH     r0,[r1,#0]
        MOV      r0,r1
        MOVS     r1,#0
        LDRSH    r1,[r0,r1]  ; accel_y
        ASRS     r0,r1,#2
        LDR      r1,|L0.1072|
        STRH     r0,[r1,#0]
        MOVS     r0,#5
        BL       BSP_MMA8451_Read_Reg
        LSLS     r0,r0,#8
        SXTH     r0,r0
        LDR      r1,|L0.1076|
        STRH     r0,[r1,#0]
        MOVS     r0,#6
        BL       BSP_MMA8451_Read_Reg
        LDR      r1,|L0.1076|
        MOVS     r2,#0
        LDRSH    r2,[r1,r2]  ; accel_z
        ORRS     r0,r0,r2
        STRH     r0,[r1,#0]
        MOV      r0,r1
        MOVS     r1,#0
        LDRSH    r1,[r0,r1]  ; accel_z
        ASRS     r0,r1,#2
        LDR      r1,|L0.1076|
        STRH     r0,[r1,#0]
        MOVS     r0,#1
        BL       BSP_MMA8451_Read_Reg
        LSLS     r0,r0,#8
        SXTH     r0,r0
        LDR      r1,|L0.1080|
        STRH     r0,[r1,#0]
        MOVS     r0,#2
        BL       BSP_MMA8451_Read_Reg
        LDR      r1,|L0.1080|
        MOVS     r2,#0
        LDRSH    r2,[r1,r2]  ; Aresultx
        ORRS     r0,r0,r2
        STRH     r0,[r1,#0]
        MOV      r0,r1
        MOVS     r1,#0
        LDRSH    r1,[r0,r1]  ; Aresultx
        ASRS     r0,r1,#8
        LDR      r1,|L0.1080|
        STRH     r0,[r1,#0]
        MOVS     r0,#3
        BL       BSP_MMA8451_Read_Reg
        LSLS     r0,r0,#8
        SXTH     r0,r0
        LDR      r1,|L0.1084|
        STRH     r0,[r1,#0]
        MOVS     r0,#4
        BL       BSP_MMA8451_Read_Reg
        LDR      r1,|L0.1084|
        MOVS     r2,#0
        LDRSH    r2,[r1,r2]  ; Aresulty
        ORRS     r0,r0,r2
        STRH     r0,[r1,#0]
        MOV      r0,r1
        MOVS     r1,#0
        LDRSH    r1,[r0,r1]  ; Aresulty
        ASRS     r0,r1,#8
        LDR      r1,|L0.1084|
        STRH     r0,[r1,#0]
        MOVS     r0,#5
        BL       BSP_MMA8451_Read_Reg
        LSLS     r0,r0,#8
        SXTH     r0,r0
        LDR      r1,|L0.1088|
        STRH     r0,[r1,#0]
        MOVS     r0,#6
        BL       BSP_MMA8451_Read_Reg
        LDR      r1,|L0.1088|
        MOVS     r2,#0
        LDRSH    r2,[r1,r2]  ; Aresultz
        ORRS     r0,r0,r2
        STRH     r0,[r1,#0]
        MOV      r0,r1
        MOVS     r1,#0
        LDRSH    r1,[r0,r1]  ; Aresultz
        ASRS     r0,r1,#8
        LDR      r1,|L0.1088|
        STRH     r0,[r1,#0]
|L0.666|
        POP      {r4,pc}
        ENDP

accel_test PROC
        PUSH     {r4,lr}
        BL       accel_read
        LDR      r0,|L0.1080|
        MOVS     r1,#0
        LDRSH    r1,[r0,r1]  ; Aresultx
        LDR      r0,|L0.1092|
        STR      r1,[r0,#0]  ; Atestx
        LDR      r1,|L0.1084|
        MOVS     r0,#0
        LDRSH    r0,[r1,r0]  ; Aresulty
        LDR      r1,|L0.1096|
        STR      r0,[r1,#0]  ; Atesty
        LDR      r1,|L0.1088|
        MOVS     r0,#0
        LDRSH    r0,[r1,r0]  ; Aresultz
        LDR      r1,|L0.1100|
        STR      r0,[r1,#0]  ; Atestz
        ADR      r0,|L0.1104|
        BL       __2printf
        LDR      r0,|L0.1092|
        LDR      r1,[r0,#0]  ; Atestx
        ADR      r0,|L0.1128|
        BL       __2printf
        LDR      r0,|L0.1096|
        LDR      r1,[r0,#0]  ; Atesty
        ADR      r0,|L0.1140|
        BL       __2printf
        LDR      r0,|L0.1100|
        LDR      r1,[r0,#0]  ; Atestz
        ADR      r0,|L0.1152|
        BL       __2printf
        POP      {r4,pc}
        ENDP

iAcclReadX PROC
        PUSH     {r4,lr}
        MOVS     r0,#0
        BL       BSP_MMA8451_Read_Reg
        LSLS     r0,r0,#28
        LSRS     r0,r0,#28
        CMP      r0,#0
        BEQ      |L0.842|
        MOVS     r0,#1
        BL       BSP_MMA8451_Read_Reg
        LSLS     r0,r0,#8
        SXTH     r0,r0
        LDR      r1,|L0.1068|
        STRH     r0,[r1,#0]
        MOVS     r0,#2
        BL       BSP_MMA8451_Read_Reg
        LDR      r1,|L0.1068|
        MOVS     r2,#0
        LDRSH    r2,[r1,r2]  ; accel_x
        ORRS     r0,r0,r2
        STRH     r0,[r1,#0]
        MOV      r0,r1
        MOVS     r1,#0
        LDRSH    r1,[r0,r1]  ; accel_x
        ASRS     r0,r1,#2
        LDR      r1,|L0.1068|
        STRH     r0,[r1,#0]
        MOVS     r0,#1
        BL       BSP_MMA8451_Read_Reg
        LSLS     r0,r0,#8
        SXTH     r0,r0
        LDR      r1,|L0.1080|
        STRH     r0,[r1,#0]
        MOVS     r0,#2
        BL       BSP_MMA8451_Read_Reg
        LDR      r1,|L0.1080|
        MOVS     r2,#0
        LDRSH    r2,[r1,r2]  ; Aresultx
        ORRS     r0,r0,r2
        STRH     r0,[r1,#0]
        MOV      r0,r1
        MOVS     r1,#0
        LDRSH    r1,[r0,r1]  ; Aresultx
        ASRS     r0,r1,#8
        LDR      r1,|L0.1080|
        STRH     r0,[r1,#0]
|L0.842|
        LDR      r1,|L0.1080|
        MOVS     r0,#0
        LDRSH    r0,[r1,r0]  ; Aresultx
        POP      {r4,pc}
        ENDP

iAcclReadY PROC
        PUSH     {r4,lr}
        MOVS     r0,#0
        BL       BSP_MMA8451_Read_Reg
        LSLS     r0,r0,#28
        LSRS     r0,r0,#28
        CMP      r0,#0
        BEQ      |L0.950|
        MOVS     r0,#3
        BL       BSP_MMA8451_Read_Reg
        LSLS     r0,r0,#8
        SXTH     r0,r0
        LDR      r1,|L0.1072|
        STRH     r0,[r1,#0]
        MOVS     r0,#4
        BL       BSP_MMA8451_Read_Reg
        LDR      r1,|L0.1072|
        MOVS     r2,#0
        LDRSH    r2,[r1,r2]  ; accel_y
        ORRS     r0,r0,r2
        STRH     r0,[r1,#0]
        MOV      r0,r1
        MOVS     r1,#0
        LDRSH    r1,[r0,r1]  ; accel_y
        ASRS     r0,r1,#2
        LDR      r1,|L0.1072|
        STRH     r0,[r1,#0]
        MOVS     r0,#3
        BL       BSP_MMA8451_Read_Reg
        LSLS     r0,r0,#8
        SXTH     r0,r0
        LDR      r1,|L0.1084|
        STRH     r0,[r1,#0]
        MOVS     r0,#4
        BL       BSP_MMA8451_Read_Reg
        LDR      r1,|L0.1084|
        MOVS     r2,#0
        LDRSH    r2,[r1,r2]  ; Aresulty
        ORRS     r0,r0,r2
        STRH     r0,[r1,#0]
        MOV      r0,r1
        MOVS     r1,#0
        LDRSH    r1,[r0,r1]  ; Aresulty
        ASRS     r0,r1,#8
        LDR      r1,|L0.1084|
        STRH     r0,[r1,#0]
|L0.950|
        LDR      r1,|L0.1084|
        MOVS     r0,#0
        LDRSH    r0,[r1,r0]  ; Aresulty
        POP      {r4,pc}
        ENDP

iAcclReadZ PROC
        PUSH     {r4,lr}
        MOVS     r0,#0
        BL       BSP_MMA8451_Read_Reg
        LSLS     r0,r0,#28
        LSRS     r0,r0,#28
        CMP      r0,#0
        BEQ      |L0.1058|
        MOVS     r0,#5
        BL       BSP_MMA8451_Read_Reg
        LSLS     r0,r0,#8
        SXTH     r0,r0
        LDR      r1,|L0.1076|
        STRH     r0,[r1,#0]
        MOVS     r0,#6
        BL       BSP_MMA8451_Read_Reg
        LDR      r1,|L0.1076|
        MOVS     r2,#0
        LDRSH    r2,[r1,r2]  ; accel_z
        ORRS     r0,r0,r2
        STRH     r0,[r1,#0]
        MOV      r0,r1
        MOVS     r1,#0
        LDRSH    r1,[r0,r1]  ; accel_z
        ASRS     r0,r1,#2
        LDR      r1,|L0.1076|
        STRH     r0,[r1,#0]
        MOVS     r0,#5
        BL       BSP_MMA8451_Read_Reg
        LSLS     r0,r0,#8
        SXTH     r0,r0
        LDR      r1,|L0.1088|
        STRH     r0,[r1,#0]
        MOVS     r0,#6
        BL       BSP_MMA8451_Read_Reg
        LDR      r1,|L0.1088|
        MOVS     r2,#0
        LDRSH    r2,[r1,r2]  ; Aresultz
        ORRS     r0,r0,r2
        STRH     r0,[r1,#0]
        MOV      r0,r1
        MOVS     r1,#0
        LDRSH    r1,[r0,r1]  ; Aresultz
        ASRS     r0,r1,#8
        LDR      r1,|L0.1088|
        STRH     r0,[r1,#0]
|L0.1058|
        LDR      r1,|L0.1088|
        MOVS     r0,#0
        LDRSH    r0,[r1,r0]  ; Aresultz
        POP      {r4,pc}
        ENDP

        DCW      0x0000
|L0.1068|
        DCD      accel_x
|L0.1072|
        DCD      accel_y
|L0.1076|
        DCD      accel_z
|L0.1080|
        DCD      Aresultx
|L0.1084|
        DCD      Aresulty
|L0.1088|
        DCD      Aresultz
|L0.1092|
        DCD      Atestx
|L0.1096|
        DCD      Atesty
|L0.1100|
        DCD      Atestz
|L0.1104|
        DCB      "\n\r Acelerometer Values",0
        DCB      0
|L0.1128|
        DCB      "\n\rx = %d \t",0
        DCB      0
|L0.1140|
        DCB      "y = %d \t",0
        DCB      0
        DCB      0
        DCB      0
|L0.1152|
        DCB      "z = %d \n\r",0
        DCB      0
        DCB      0

        AREA ||.arm_vfe_header||, DATA, READONLY, NOALLOC, ALIGN=2

        DCD      0x00000000

        AREA ||.data||, DATA, ALIGN=2

accel_x
        DCB      0x00,0x00
accel_y
        DCB      0x00,0x00
accel_z
        DCB      0x00,0x00
Aresultx
        DCB      0x00,0x00
Aresulty
        DCB      0x00,0x00
Aresultz
        DCB      0x00,0x00
aux1
        DCB      0x00,0x00,0x00,0x00
Atestx
        DCD      0x00000000
Atesty
        DCD      0x00000000
Atestz
        DCD      0x00000000

        EXPORT BSP_MMA8451_Write_Reg [CODE]
        EXPORT BSP_MMA8451_Read_Reg [CODE]
        EXPORT BSP_MMA8451_Init [CODE]
        EXPORT mma8451_active [CODE]
        EXPORT accel_read [CODE]
        EXPORT accel_test [CODE]
        EXPORT iAcclReadX [CODE]
        EXPORT iAcclReadY [CODE]
        EXPORT iAcclReadZ [CODE]
        EXPORT accel_x [DATA,SIZE=2]
        EXPORT accel_y [DATA,SIZE=2]
        EXPORT accel_z [DATA,SIZE=2]
        EXPORT Aresultx [DATA,SIZE=2]
        EXPORT Aresulty [DATA,SIZE=2]
        EXPORT Aresultz [DATA,SIZE=2]
        EXPORT aux1 [DATA,SIZE=1]
        EXPORT Atestx [DATA,SIZE=4]
        EXPORT Atesty [DATA,SIZE=4]
        EXPORT Atestz [DATA,SIZE=4]

        IMPORT ||Lib$$Request$$armlib|| [CODE,WEAK]
        IMPORT BSP_I2C_Start [CODE]
        IMPORT BSP_I2C_Write_Byte [CODE]
        IMPORT BSP_I2C_Wait [CODE]
        IMPORT BSP_I2C_Get_Ack [CODE]
        IMPORT BSP_I2C_Stop [CODE]
        IMPORT BSP_I2C_Repeated_Start [CODE]
        IMPORT BSP_I2C_Set_RX_Mode [CODE]
        IMPORT BSP_I2C_Give_Nack [CODE]
        IMPORT BSP_I2C_Read_Byte [CODE]
        IMPORT BSP_I2C_Init [CODE]
        IMPORT __2printf [CODE]
        IMPORT _printf_percent [CODE]
        IMPORT _printf_d [CODE]
        IMPORT _printf_int_dec [CODE]

        KEEP pause

        ATTR FILESCOPE
        ATTR SETVALUE Tag_ABI_PCS_wchar_t,2
        ATTR SETVALUE Tag_ABI_enum_size,1
        ATTR SETVALUE Tag_ABI_optimization_goals,6
        ATTR SETSTRING Tag_conformance,"2.06"
        ATTR SETVALUE AV,18,1

        ASSERT {ENDIAN} = "little"
        ASSERT {INTER} = {TRUE}
        ASSERT {ROPI} = {FALSE}
        ASSERT {RWPI} = {FALSE}
        ASSERT {IEEE_FULL} = {FALSE}
        ASSERT {IEEE_PART} = {FALSE}
        ASSERT {IEEE_JAVA} = {FALSE}
        END
