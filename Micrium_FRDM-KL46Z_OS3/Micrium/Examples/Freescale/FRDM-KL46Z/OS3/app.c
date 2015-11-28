/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2014; Micrium, Inc.; Weston, FL
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
*                                            EXAMPLE CODE
*
*                                        Freescale Kinetis KL40
*                                               on the
*                                        Freescale FRDM-KL46Z
*                                          Evaluation Board
*
* Filename      : app.c
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

#include  <app_cfg.h>
#include  <includes.h>


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

static  OS_TCB        App_TaskStartTCB;                       /* Here we allocate storage for the OS_TCBs of each task. */
static  CPU_STK_SIZE  App_TaskStartStk[APP_CFG_TASK_START_STK_SIZE];    /* A stack is allocated for each task.          */

static  OS_TCB        App_Task_LCD_TCB;
static  CPU_STK_SIZE  App_Task_LCD_Stk[APP_CFG_TASK_LCD_STK_SIZE];

static  OS_TCB        App_TaskTSI_SliderTCB;
static  CPU_STK_SIZE  App_TaskTSI_SliderStk[APP_CFG_TASK_TSI_SLIDER_STK_SIZE];

static  OS_TCB        App_Task_LED_TCB;
static  CPU_STK_SIZE  App_Task_LED_Stk[APP_CFG_TASK_LED_STK_SIZE];

static  OS_TCB        App_Task_KEY_TCB;
static  CPU_STK_SIZE  App_Task_KEY_Stk[APP_CFG_TASK_KEY_STK_SIZE];

static  OS_MUTEX      AppMutex;                                   /* A mutual exclusion semaphore.                    */

static  OS_Q          AppQ;                                       /* A message queue.                                 */

static  CPU_INT08U    App_LCD_Dig1;
static  CPU_INT08U    App_LCD_Dig2;
static  CPU_INT08U    App_LCD_Dig3;
static  CPU_INT08U    App_LCD_Dig4;

static  CPU_INT08U    App_TSI_SliderPosMm;
static  CPU_INT08U    App_TSI_SliderPosPct;


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  App_ObjCreate       (void);
static  void  App_TaskCreate      (void);

static  void  App_TaskStart       (void *p_arg);                /* The prototype of the tasks are declared.             */
static  void  App_Task_LCD        (void *p_arg);
static  void  App_TaskTSI_Slider  (void *p_arg);
static  void  App_Task_LED        (void *p_arg);
static  void  App_Task_KEY        (void *p_arg);


/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This the main standard entry point.
*
* Note(s)     : none.
*********************************************************************************************************
*/

int  main (void)
{
    OS_ERR  err;

#if (CPU_CFG_NAME_EN == DEF_ENABLED)
    CPU_ERR     cpu_err;
#endif


    CPU_Init();                                                 /* Initialize the CPU abstraction layer.                */

    Mem_Init();                                                 /* Initialize the Memory Management Module.             */

    Math_Init();                                                /* Initialize the Mathematical Module.                  */

#if (CPU_CFG_NAME_EN == DEF_ENABLED)
    CPU_NameSet((CPU_CHAR *)"MKL46Z256VLL4",
                (CPU_ERR  *)&cpu_err);
#endif

    BSP_IntDisAll();                                            /* Disable all interrupts.                              */

#if (defined(TRACE_CFG_EN) && (TRACE_CFG_EN > 0u))
    TRACE_INIT();                                               /* Initialize the µC/Trace recorder.                    */
    TRACE_START();                                              /* Start recording.                                     */
#endif

    OSInit(&err);                                               /* Initialize "uC/OS-III, The Real-Time Kernel".        */
	
    OSMutexCreate((OS_MUTEX  *)&AppMutex,
                 (CPU_CHAR   *)"My App. Mutex",
                 (OS_ERR     *)&err);
  
    OSQCreate    ((OS_Q      *)&AppQ,
                 (CPU_CHAR   *)"My App Queue",
                 (OS_MSG_QTY  )10,
                 (OS_ERR     *)&err);

    OSTaskCreate((OS_TCB     *)&App_TaskStartTCB,               /* Create the startup task.                             */
                 (CPU_CHAR   *)"Startup Task",
                 (OS_TASK_PTR ) App_TaskStart,
                 (void       *) 0,
                 (OS_PRIO     ) APP_CFG_TASK_START_PRIO,
                 (CPU_STK    *)&App_TaskStartStk[0],
                 (CPU_STK     )(APP_CFG_TASK_START_STK_SIZE / 10u),
                 (CPU_STK_SIZE) APP_CFG_TASK_START_STK_SIZE,
                 (OS_MSG_QTY  ) 0,
                 (OS_TICK     ) 0,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

    OSStart(&err);                                              /* Start multitasking (i.e. give control to uC/OS-III). */

    while(DEF_ON) {                                             /* Should Never Get Here                                */
    };
}


/*
*********************************************************************************************************
*                                          App_TaskStart()
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskStart()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Notes       : (1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                   used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  App_TaskStart (void *p_arg)
{
    OS_ERR  os_err;

    (void)p_arg;                                                /* See Note #1.                                         */

    BSP_Init();                                                 /* Start BSP and tick initialization.                   */

    BSP_Tick_Init();                                            /* Start Tick Initialization.                           */
	
    BSP_UART_Init(BSP_UART_DEFAULT,                             /* Start UART Initialization                            */
	                BSP_BUSCLK,
	                9600);

    BSP_LCD_Init();                                             /* Initialize the seven-segment display panel.          */

#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&os_err);                            /* Compute CPU capacity with no task running            */
#endif

#ifdef  CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif

    APP_TRACE_INFO(("Creating Application Events...\n\r"));
    App_ObjCreate();                                            /* Create Applicaton kernel objects.                    */

    APP_TRACE_INFO(("Creating Application Tasks...\n\r"));
    App_TaskCreate();                                           /* Create Application tasks.                            */

    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */

        OSTimeDlyHMSM(0, 0, 2, 0,
                      OS_OPT_TIME_HMSM_STRICT, &os_err);
//		  BSP_UART_Send_String(BSP_UART_1,
//				             "Hello\n");
        uart_printf("Hello world\n");
        OSTimeDly(10, OS_OPT_TIME_HMSM_STRICT, &os_err);
        uart_printf("os_err = %d\n", os_err);
			  
//			  while (BSP_SW_Read (BSP_SW_1) != DEF_ON) {
//					  OSTimeDly(1, OS_OPT_TIME_HMSM_STRICT, &os_err);
//				}
//				BSP_LED_Toggle(BSP_LED_RED);
//				
//				BSP_UART_Send_String(BSP_UART_1,
//				                     "Hello\n");
//				
//				while (BSP_SW_Read (BSP_SW_1) == DEF_OFF) {
//			      OSTimeDly(1, OS_OPT_TIME_HMSM_STRICT, &os_err);
//				}
    }
}


/*
*********************************************************************************************************
*                                      App_ObjCreate()
*
* Description:  Creates the application kernel objects.
*
* Argument(s) :  none.
*
* Return(s)   :  none.
*
* Caller(s)   :  App_TaskStart().
*
* Note(s)     :  none.
*********************************************************************************************************
*/

static  void  App_ObjCreate (void)
{
}


/*
*********************************************************************************************************
*                                      App_TaskCreate()
*
* Description :  This function creates the application tasks.
*
* Argument(s) :  none.
*
* Return(s)   :  none.
*
* Caller(s)   :  App_TaskStart().
*
* Note(s)     :  none.
*********************************************************************************************************
*/

static  void  App_TaskCreate (void)
{
    OS_ERR  err;


    OSTaskCreate((OS_TCB     *)&App_Task_LCD_TCB,               /* Create the LCD task.                                    */
                 (CPU_CHAR   *)"LCD Task",
                 (OS_TASK_PTR ) App_Task_LCD,
                 (void       *) 0,
                 (OS_PRIO     ) APP_CFG_TASK_LCD_PRIO,
                 (CPU_STK    *)&App_Task_LCD_Stk[0],
                 (CPU_STK     )(APP_CFG_TASK_LCD_STK_SIZE / 10u),
                 (CPU_STK_SIZE) APP_CFG_TASK_LCD_STK_SIZE,
                 (OS_MSG_QTY  ) 0,
                 (OS_TICK     ) 0,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

    OSTaskCreate((OS_TCB     *)&App_TaskTSI_SliderTCB,          /* Create the TSI Slider task.                            */
                 (CPU_CHAR   *)"TSI Slider Task",
                 (OS_TASK_PTR ) App_TaskTSI_Slider,
                 (void       *) 0,
                 (OS_PRIO     ) APP_CFG_TASK_TSI_SLIDER_PRIO,
                 (CPU_STK    *)&App_TaskTSI_SliderStk[0],
                 (CPU_STK     )(APP_CFG_TASK_TSI_SLIDER_STK_SIZE / 10u),
                 (CPU_STK_SIZE) APP_CFG_TASK_TSI_SLIDER_STK_SIZE,
                 (OS_MSG_QTY  ) 0,
                 (OS_TICK     ) 0,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
								 
    OSTaskCreate((OS_TCB     *)&App_Task_LED_TCB,              /* Create the LED task.                                   */
                 (CPU_CHAR   *)"LED Task",
                 (OS_TASK_PTR ) App_Task_LED,
                 (void       *) 0,
                 (OS_PRIO     ) APP_CFG_TASK_LED_PRIO,
                 (CPU_STK    *)&App_Task_LED_Stk[0],
                 (CPU_STK     )(APP_CFG_TASK_LED_STK_SIZE / 10u),
                 (CPU_STK_SIZE) APP_CFG_TASK_LED_STK_SIZE,
                 (OS_MSG_QTY  ) 0,
                 (OS_TICK     ) 0,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
				 
								 
    OSTaskCreate((OS_TCB     *)&App_Task_KEY_TCB,              /* Create the KEY task.                                   */
                 (CPU_CHAR   *)"KEY Task",
                 (OS_TASK_PTR ) App_Task_KEY,
                 (void       *) 0,
                 (OS_PRIO     ) APP_CFG_TASK_KEY_PRIO,
                 (CPU_STK    *)&App_Task_KEY_Stk[0],
                 (CPU_STK     )(APP_CFG_TASK_KEY_STK_SIZE / 10u),
                 (CPU_STK_SIZE) APP_CFG_TASK_KEY_STK_SIZE,
                 (OS_MSG_QTY  ) 0,
                 (OS_TICK     ) 0,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
}


/*
*********************************************************************************************************
*                                            App_Task_LCD()
*
* Description : This is an example of an application task.
*
* Argument(s) : p_arg   is the argument passed to 'App_Task_LCD()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Notes       : (1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                   used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  App_Task_LCD (void *p_arg)
{
     OS_ERR      os_err;


    (void)p_arg;                                                /* See Note #1                                              */

    App_LCD_Dig1 =  0u;
    App_LCD_Dig2 =  0u;
    App_LCD_Dig3 =  0u;
    App_LCD_Dig4 =  0u;
    //BSP_LCD_OPT_COL = 1u;

    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.           */

        BSP_LCD_Write(App_LCD_Dig1,
                      App_LCD_Dig2,
                      App_LCD_Dig3,
                      App_LCD_Dig4,
                      BSP_LCD_OPT_COL);
			
			  App_LCD_Dig1++;
			  
			  if (App_LCD_Dig1 == 10) {
					  App_LCD_Dig1 = 0;
				}

        OSTimeDlyHMSM(0, 0, 1, 0,
                      OS_OPT_TIME_HMSM_STRICT, &os_err);        /* Suspend the task execution for 400 milliseconds.         */
    }
}


/*
*********************************************************************************************************
*                                          App_TaskTSI_Slider()
*
* Description : This is an example of an application task.
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskTSI_Slider()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Notes       : (1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                   used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  App_TaskTSI_Slider (void *p_arg)
{
     OS_ERR  os_err;

    (void)p_arg;                                                /* See Note #1                                              */

    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.           */

                                                                /* Read the position of the TSI slider.                     */
        BSP_TSI_SliderRead(&App_TSI_SliderPosMm, &App_TSI_SliderPosPct);

        OSTimeDlyHMSM(0, 0, 0, 50,
                      OS_OPT_TIME_HMSM_STRICT, &os_err);        /* Suspend the task execution for 50 milliseconds.          */

    }
}


/*
*********************************************************************************************************
*                                          App_Task_LED()
*
* Description : This is an example of an application task.
*
* Argument(s) : p_arg   is the argument passed to 'App_Task_KEY()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Notes       : (1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                   used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/
static  void App_Task_LED (void *p_arg)
{
    OS_ERR       os_err;
	
    void         *p_msg;
    OS_MSG_SIZE  msg_size;
	
    CPU_TS       ts;
    CPU_TS       ts_delta;
	
    (void)       p_arg;
	
    p_arg = p_arg;
    while (DEF_TRUE) {
		
        p_msg = OSQPend((OS_Q        *)&AppQ,                  /* starts by waiting for messages to be sent through the message queue AppQ. */
                        (OS_TICK      )0,
                        (OS_OPT       )OS_OPT_PEND_BLOCKING,
                        (OS_MSG_SIZE *)&msg_size,              /* The size of the message.                                                  */
                        (CPU_TS      *)&ts,                    /* contain the timestamp of when the message was sent.                       */
                        (OS_ERR      *)&os_err);
		
        ts_delta = OS_TS_GET() - ts;
		
        BSP_LED_Toggle(BSP_LED_GREEN);

        OSTimeDlyHMSM(0, 0, 1, 0,
                      OS_OPT_TIME_HMSM_STRICT, &os_err);
    }
}


/*
*********************************************************************************************************
*                                          App_Task_KEY()
*
* Description : This is an example of an application task.
*
* Argument(s) : p_arg   is the argument passed to 'App_Task_KEY()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Notes       : (1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                   used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/
static  void App_Task_KEY (void *p_arg)
{
    OS_ERR os_err;
	
    CPU_TS  ts;
	
    (void)p_arg;
	
    while (DEF_TRUE) {
		
        OSTimeDly ((OS_TICK     )1,
                   (OS_OPT      )OS_OPT_TIME_DLY,
                   (OS_ERR     *)&os_err);
			
        while (BSP_SW_Read (BSP_SW_1) == DEF_ON) {
            OSTimeDly(1, OS_OPT_TIME_HMSM_STRICT, &os_err);
        }
		
	    	//OS_CRITICAL_ENTER();
		
        OSQPost((OS_Q      *)&AppQ,                    /* sends a message to another task using the message queue AppQ. */
                (void      *)1,                        /* sends a fixed message of value "1".                           */
                (OS_MSG_SIZE)sizeof(void *),
                (OS_OPT     )OS_OPT_POST_FIFO,
                (OS_ERR    *)&os_err);
				
        OSMutexPend((OS_MUTEX  *)&AppMutex,            /* waits on the mutual exclusion semaphore.                      */
                    (OS_TICK    )0,
                    (OS_OPT     )OS_OPT_PEND_BLOCKING,
                    (CPU_TS    *)&ts,
                    (OS_ERR    *)&os_err);
				
        /* Access shared resource */
				
        OSMutexPost((OS_MUTEX  *)&AppMutex,            /* must call OSMutexPost() to release the mutex.                 */
                    (OS_OPT     )OS_OPT_POST_NONE,
                    (OS_ERR    *)&os_err);
		
	    	//BSP_LED_Toggle(BSP_LED_RED);
	    	//OSTimeDlyResume(&App_Task_LED_TCB, &os_err);

				
        while (BSP_SW_Read (BSP_SW_1) == DEF_OFF) {
            OSTimeDly(1, OS_OPT_TIME_HMSM_STRICT, &os_err);
	    }
		
		
        OSTimeDlyHMSM(0, 0, 0, 50,
        OS_OPT_TIME_HMSM_STRICT, &os_err);        /* Suspend the task execution for 50 milliseconds.          */
	  }
}
