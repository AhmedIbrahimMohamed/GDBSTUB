/*
*********************************************************************************************************
*                                               uC/OS-II
*                                         The Real-Time Kernel
*
*                             (c) Copyright 2012, Micrium, Inc.; Weston, FL
*                                          All Rights Reserved
*
*
* File : APP.C
* By   : JPB
*
* LICENSING TERMS:
* ---------------
*           uC/OS-II is provided in source form for FREE short-term evaluation, for educational use or
*           for peaceful research.  If you plan or intend to use uC/OS-II in a commercial application/
*           product then, you need to contact Micrium to properly license uC/OS-II for its use in your
*           application/product.   We provide ALL the source code for your convenience and to help you
*           experience uC/OS-II.  The fact that the source is provided does NOT mean that you can use
*           it commercially without paying a licensing fee.
*
*           Knowledge of the source code may NOT be used to develop a similar product.
*
*           Please help us continue to provide the embedded community with the finest software available.
*           Your honesty is greatly appreciated.
*
*           You can contact us at www.micrium.com, or by phone at +1 (954) 217-2036.
*
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <includes.h>


/*
*********************************************************************************************************
*                                             LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL VARIABLES
*********************************************************************************************************
*/

static  OS_STK       AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE]; /* Startup Task Stack                                */
static  OS_STK       AppTask1Stk[APP_CFG_TASK_1_STK_SIZE];         /* Task #1      Stack                                */
static  OS_STK       AppTask2Stk[APP_CFG_TASK_2_STK_SIZE];         /* Task #2      Stack                                */
static  OS_STK       AppTask3Stk[APP_CFG_TASK_3_STK_SIZE];         /* Task #3"stub"      Stack                                */
        OS_EVENT    *AppMutexPrint;                                /* UART mutex.                                       */


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  AppTaskCreate      (void);

static  void  AppMutexCreate     (void);

static  void  AppTaskStart       (void *p_arg);
static  void  AppTaskStub        (void *p_arg);
static  void  AppTask2           (void *p_arg);
static  void  AppTask1           (void *p_arg);

static  void  AppPrint           (char *str);
static  void  AppPrintWelcomeMsg (void);
        void  print              (char *str);


/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : Startup Code.
*
* Note(s)     : none.
*********************************************************************************************************
*/

int  main (void)
{

#if (CPU_CFG_NAME_EN == DEF_ENABLED)
    CPU_ERR  cpu_err;
#endif


    BSP_Init();

    CPU_Init();                                                 /* Initialize the uC/CPU services                       */

#if (CPU_CFG_NAME_EN == DEF_ENABLED)
    CPU_NameSet((CPU_CHAR *)CSP_DEV_NAME,
                (CPU_ERR  *)&cpu_err);
#endif

    CPU_IntDis();

    //AppPrintWelcomeMsg();

    OSInit();                                                   /* Initialize uC/OS-II.                                 */

    OSTaskCreateExt(AppTaskStart,                               /* Create the start task                                */
                   (void       *)0,
                   (OS_STK     *)&AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE - 1],
                   (OS_PRIO     )APP_CFG_TASK_START_PRIO,
                   (OS_PRIO     )APP_CFG_TASK_START_PRIO,
                   (OS_STK     *)&AppTaskStartStk[0],
                   (CPU_INT32U  )APP_CFG_TASK_START_STK_SIZE,
                   (void       *)0,
                   (CPU_INT16U  )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

    OSStart();                                                  /* Start multitasking (i.e. give control to uC/OS-II).  */

    return 0;
}


/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  AppTaskStart (void *p_arg)
{
    (void)p_arg;


    //print("Task Start Created\r\n");

    OS_CSP_TickInit();                                          /* Initialize the Tick interrupt                        */

    Mem_Init();                                                 /* Initialize memory management module                  */
    Math_Init();                                                /* Initialize mathematical module                       */

#if (OS_TASK_STAT_EN > 0u)
    OSStatInit();                                               /* Determine CPU capacity                               */
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif


    AppTaskCreate();                                            /* Create Application tasks                             */

    AppMutexCreate();                                           /* Create Mutual Exclusion Semaphores                   */

    while (DEF_ON) {                                            /* Task body, always written as an infinite loop.       */

    	OSTimeDlyHMSM(0, 0, 0, 100);                            /* Waits 100 milliseconds.                              */

    	AppPrint(".");                                          /* Prints a dot every 100 milliseconds.                 */
    }
}


/*
*********************************************************************************************************
*                                       CREATE APPLICATION TASKS
*
* Description : Creates the application tasks.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : AppTaskStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  AppTaskCreate (void)
{

#if (OS_USE_GDB == 1)

	OSTaskCreateExt(AppTaskStub,                                   /* Create the Task #3"stub".                                  */
   	                       (void       *)0,
   	                       (OS_STK     *)&AppTask3Stk[APP_CFG_TASK_1_STK_SIZE - 1],
   	                       (OS_PRIO     )APP_CFG_TASK_3_PRIO,
   	                       (OS_PRIO     )APP_CFG_TASK_3_PRIO,
   	                       (OS_STK     *)&AppTask3Stk[0],
   	                       (CPU_STK_SIZE)APP_CFG_TASK_3_STK_SIZE,
   	                       (void       *)0,
   	                       (CPU_INT16U  )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
#endif
    OSTaskCreateExt(AppTask1,                                   /* Create the Task #1.                                  */
                   (void       *)0,
                   (OS_STK     *)&AppTask1Stk[APP_CFG_TASK_1_STK_SIZE - 1],
                   (OS_PRIO     )APP_CFG_TASK_1_PRIO,
                   (OS_PRIO     )APP_CFG_TASK_1_PRIO,
                   (OS_STK     *)&AppTask1Stk[0],
                   (CPU_STK_SIZE)APP_CFG_TASK_1_STK_SIZE,
                   (void       *)0,
                   (CPU_INT16U  )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

    OSTaskCreateExt(AppTask2,                                   /* Create the Task #2.                                  */
                   (void       *)0,
                   (OS_STK     *)&AppTask2Stk[APP_CFG_TASK_2_STK_SIZE - 1],
                   (OS_PRIO     )APP_CFG_TASK_2_PRIO,
                   (OS_PRIO     )APP_CFG_TASK_2_PRIO,
                   (OS_STK     *)&AppTask2Stk[0],
                   (CPU_STK_SIZE)APP_CFG_TASK_2_STK_SIZE,
                   (void       *)0,
                   (CPU_INT16U  )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

   }


/*
*********************************************************************************************************
*                                       CREATE APPLICATION MUTEXES
*
* Description : Creates the application mutexes.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : AppTaskStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  AppMutexCreate (void)
{
	CPU_INT08U  err;


    AppMutexPrint = OSMutexCreate(20, &err);                    /* Creates the UART mutex.                              */
}


/*
*********************************************************************************************************
*                                              TASK #1
*
* Description : This is an example of an application task that prints "1" every second to the UART.
*
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  AppTask1 (void *p_arg)
{
    (void)p_arg;

    AppPrint("Task #1 Started\r\n");

    while (DEF_ON) {                                            /* Task body, always written as an infinite loop.       */

    	//AppPrint("1");
    	OSTimeDlyHMSM(0, 0, 1, 0);                              /* Waits for 1-second.                                  */

    	AppPrint("1");                                          /* Prints 1 to the UART.                                */

    }
}


/*
*********************************************************************************************************
*                                               TASK #2
*
* Description : This is an example of an application task that prints "2" every 2 seconds to the UART.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  AppTask2 (void *p_arg)
{
    (void)p_arg;

    AppPrint("Task #2 Started\r\n");

    while (DEF_ON) {                                            /* Task body, always written as an infinite loop.       */

    	OSTimeDlyHMSM(0, 0, 2, 0);                              /* Waits for 2-seconds.                                 */

    	AppPrint("2");                                          /* Prints 2 to the UART.                                */

    }
}



/*
*********************************************************************************************************
*                                               TASK #3"Stub"
*
* Description : This is an example of an application task that acts as gdbstub getting GDB commands
*              ,retrieving information and respond with appropriate data .
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/
#if (OS_USE_GDB == 1)

static  void  AppTaskStub (void *p_arg)
{
    (void)p_arg;
    /*define some variables and arrays to test them with <m> and <M> command packets*/

    CPU_INT64U dword = 0x70348E905984fd1c;
    //dword is stored at 0x11b208
    //$m11b200,40#53
    //$M110b208,8:5555555555555555#c9
    //
    Debug_MemWidth word = 0x70348E90;
    //word at address :0x11b204
    //$M11b204,4:xxxxxxxx#
    //$m11b200,40#53
    CPU_INT08U HEXBuf[]={'A','B','c','f','5','8','1','0','g','5','9','a','D','C','6','7',
    	                      'A','B','c','f','5','8','9','0','4','5','9','a','D','C','6','7'};
    //HexBuf at address 0x11b1e4
    //$m11b1c0,40#85

    CPU_INT32U registers [4] = {0x70348E90, 0x753DfBca,0x70348E90,0x2004A06c};
//registers at address 0x11b1d4
    //$m11b1c0,40#85
    //set registers[0] = 0x11111111
    //$M11b1d4,4:11111111#fc

    Debug_Main_Init();
    //AppPrint("\nStub Task created \n");

    while (DEF_ON) {                                            /* Task body, always written as an infinite loop.       */

//    	OSTimeDlyHMSM(0, 0, 3, 0);
    	Debug_RSP_Get_Packet();                          /* Waits for a packet from host GDB.                                 */
    	Debug_RSP_Process_Packet();                     /* process and respond to packet.                                */


    	//Debug_RSP_Get_Packet();
     	//Debug_RSP_Process_Packet();                     /* process and respond to packet.                                */


    }
}
#endif

/*
*********************************************************************************************************
*                                            PRINT THROUGH UART
*
* Description : Prints a string through the UART. It makes use of a mutex to
*               access this shared resource.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : application functions.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  AppPrint (char *str)
{
	CPU_INT08U  err;


#if (OS_USE_GDB == 1)
	Debug_RSP_Console_Packet(str);
#else

	OSMutexPend(AppMutexPrint, 0, &err);                        /* Wait for the shared resource to be released.         */

	print(str);                                                 /* Access the shared resource.                          */

    OSMutexPost(AppMutexPrint);                                 /* Releases the shared resource.                        */
#endif
}


/*
*********************************************************************************************************
*                                        PRINT WELCOME THROUGH UART
*
* Description : Prints a welcome message through the UART.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : application functions.
*
* Note(s)     : Because the welcome message gets displayed before
*               the multi-tasking has started, it is safe to access
*               the shared resource directly without any mutexes.
*********************************************************************************************************
*/

static  void  AppPrintWelcomeMsg (void)
{
    print("\f\f\r\n");
    print("Micrium\r\n");
    print("uCOS-II\r\n\r\n");
    print("This application runs three different tasks:\r\n\r\n");
    print("1. Task Start: Initializes the OS and creates tasks and\r\n");
    print("               other kernel objects such as semaphores.\r\n");
    print("               This task remains running and printing a\r\n");
    print("               dot '.' every 100 milliseconds.\r\n");
    print("2. Task #1   : Prints '1' every 1-second.\r\n");
    print("3. Task #2   : Prints '2' every 2-seconds.\r\n\r\n");
}
