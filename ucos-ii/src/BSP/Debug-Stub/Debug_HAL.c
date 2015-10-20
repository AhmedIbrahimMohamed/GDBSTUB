/*
 * Debug_HAL.c
 *
 *  Created on: ??þ/??þ/????
 *      Author: YUSSRY
 */



/*
*********************************************************************************************************
*                                     MICIRUM BOARD SUPPORT PACKAGE
*
*                             (c) Copyright 2012; Micrium, Inc.; Weston, FL
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
*                                         Xilinx Zynq XC7Z020
*                                               on the
*
*                                             Xilinx ZC702
*                                          Evaluation Board
*
* Filename      : bsp.c
* Version       : V1.00
* Programmer(s) : JPB
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define Debug_HAL_IMPORT
#include "Debug_HAL.h"



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


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

 _gdb_arch arch_gdb_ops = {
	.gdb_bpt_instr		=  {0xfe, 0xde, 0xff, 0xe7}  //Little-Endian
};

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             REGISTERS
*********************************************************************************************************
*/




/*
*********************************************************************************************************
*                                            REGISTER BITS
*********************************************************************************************************
*/




/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/





/*
*********************************************************************************************************
*                                               void Debug_HAL_init()
*
* Description :
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : .
*
* Note(s)     : (1)
*
*               (2)
*********************************************************************************************************
*/

void Debug_HAL_init(void)
{

	/*Initialize Registers Buffer*/
	CPU_INT08U RegOffset;
	for(RegOffset =R0_Offset ; RegOffset< Debug_HAL_Regs_IDMAX ; RegOffset++)
		Debug_HAL_RegsBuffer[RegOffset] = 0;

}


/*
*********************************************************************************************************
*                                       Register Functions
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               void Debug_HAL_Regs_Readall()
*
* Description :Reads target registers that GDB needs in <g> command packet . Registers are Thread-specific and is stored in Debug_HAL_RegsBuffer[]
*
* Argument(s) : ThreadID : thread id for which this function read its register context.
*
* Return(s)   : none.
*
* Caller(s)   : Debug_Main_Read_Registers() in Debug_Main Module
*
* Note(s)     : (1) ThreadID could be thread priority , thread ID  depending on the target system(RTOS)
*
*               (2)refer to OSTaskStkInit() in os_cpu_c.c to know how registers are stored in task stack.
*********************************************************************************************************
*/

void Debug_HAL_Regs_Readall(Debug_TID_t ThreadID)
{

	/*ThreadID here means priority*/

	CPU_INT08U count ;
	//Debug_TID_t current_thread = current_thread_OF_Focus;

	Debug_TCB *Deubg_TCBCurr = (Debug_TCB *)Debug_RTOS_ThreadListPtr[ThreadID] ; /*get a pointer to the thread stack from the Task Control Block */
	Debug_RegisterWidth *Debug_SP = Deubg_TCBCurr->OSTCBStkPtr;

	/***************Note : Debug_SP is incremented or decremented based on stack-growth setting"OS_STK_GROWTH macro" in os_cpu.h *********************/
	Debug_HAL_RegsBuffer[SP_Offset] = Debug_SP;                                        /*Get the SP*/
	Debug_HAL_RegsBuffer[CPSR_Offset] = *(Debug_SP++);                                 /*Get the CPSR*/

	for(count = R0_Offset; count <= R12_Offset;count++)                                    /*get R0-R12*/
		Debug_HAL_RegsBuffer[count] = *(Debug_SP++);

	Debug_HAL_RegsBuffer[LR_Offset] = *(Debug_SP++);                                       /*Get R14=LR , R13=SP is skipped
	                                                                                     as it is not stored in task stack*/

	Debug_HAL_RegsBuffer[PC_Offset] = *(Debug_SP);                                       /*Get R15=PC */


}

/*
*********************************************************************************************************
*                                                Debug_HAL_Regs_Writeall()
*
* Description : Writes target registers with GDB values in <G> command packet . Registers values
*               are Thread-specific and is stored in Debug_HAL_RegsBuffer[]
*
* Argument(s) : ThreadID : thread id for which this function modify its register context.
*
* Return(s)   : none.
*
* Caller(s)   : Debug_Main_Write_Registers() in Debug_Main Module
*
* Note(s)     : (1)ThreadID could be thread priority , thread ID  depending on the target system(RTOS)
*
*               (2)refer to OSTaskStkInit() in os_cpu_c.c to know how registers are stored in task stack.
*********************************************************************************************************
*/
void Debug_HAL_Regs_Writeall(Debug_TID_t ThreadID)
{
	CPU_INT08U count = 0 ;
	//Debug_TID_t current_thread = current_thread_OF_Focus;

	Debug_TCB *Deubg_TCBCurr = (Debug_TCB *)Debug_RTOS_ThreadListPtr[ThreadID] ; /*get a pointer to the thread stack from the Task Control Block */
	Debug_RegisterWidth *Debug_SP = Deubg_TCBCurr->OSTCBStkPtr;

	/***************Note : Debug_SP is incremented or decremented based on stack-growth setting"OS_STK_GROWTH macro" in os_cpu.h *********************/


	*(Debug_SP++) = Debug_HAL_RegsBuffer[CPSR_Offset];                                 /*set the CPSR*/

	for(count = R0_Offset ; count <= R12_Offset ; count++)                                    /*set R12-R0*/
		*(Debug_SP++) = Debug_HAL_RegsBuffer[count] ;

	Deubg_TCBCurr->OSTCBStkPtr = Debug_HAL_RegsBuffer[SP_Offset];                                 /*set the R13 = SP*/

	*(Debug_SP++) = Debug_HAL_RegsBuffer[LR_Offset] ;                                       /*set R14=LR , R13=SP is skipped
		                                                                                     as it is not stored in task stack*/

	*(Debug_SP++) = Debug_HAL_RegsBuffer[PC_Offset]  ;                                       /*set R15=PC */






}

/*
*********************************************************************************************************
*                                                Debug_HAL_Regs_ReadOne()
*
* Description :Read one register from Thread register context
*
* Argument(s) : ThreadID : thread id for which this function get  register regno.
*               regno    : Register ID as specified by GDB in <p> command packet.
*               RegValue : The returned/read register value.
*
* Return(s)   : DEBUG_SUCCESS : if no error
*               DEBUG_ERR_INVALID_REGID if register ID "regno" is not valid.
*
* Caller(s)   :
*
* Note(s)     : (1)Debug_Main_Read_Register() in Debug_Main module
*
*               (2)
*********************************************************************************************************
*/
CPU_INT08U Debug_HAL_Regs_ReadOne(Debug_TID_t ThreadID,Debug_RegID_t regno ,Debug_RegisterWidth *RegValue )
{

	/***************Note : Debug_SP is incremented or decremented based on stack-growth setting"OS_STK_GROWTH macro" in os_cpu.h *********************/

	/*
	*********************************************************************************************************
	*                                             1st Method to read one register
	*********************************************************************************************************
	*/
	if(regno > Debug_HAL_Regs_IDMAX)
		return DEBUG_ERR_INVALID_REGID;

	Debug_TCB *Deubg_TCBCurr = (Debug_TCB *)Debug_RTOS_ThreadListPtr[ThreadID] ; /*get a pointer to the thread stack from the Task Control Block */
		Debug_RegisterWidth *Debug_SP = Deubg_TCBCurr->OSTCBStkPtr;

	if (regno == CPSR_Offset)   /*CPSR*/
		 *RegValue = *(Debug_SP) ;

	else if (regno >= R0_Offset && regno <= R12_Offset )
		*RegValue = *(Debug_SP +(regno+1));
	else if (regno == SP_Offset)
		*RegValue = Debug_SP;
	else if(regno == LR_Offset || regno == PC_Offset)
		*RegValue = *(Debug_SP + regno);

	/*Floating point register IDs are ignored and they are always 0's*/

	    /*
		*********************************************************************************************************
		*                                            2nd Method to read one register
		*********************************************************************************************************
		*/

	/*just read all registers and return the needed one*/

	/*Debug_HAL_Regs_Readall(ThreadID);
	*RegValue = Debug_HAL_RegsBuffer[regno];
*/
	return DEBUG_SUCCESS;

}

/*
*********************************************************************************************************
*                                                Debug_HAL_Regs_WriteOne()
*
* Description :Write one register to Thread register context
*
* Argument(s) : ThreadID : thread id for which this function set register regno.
*               regno    : Register ID as specified by GDB in <P> command packet.
*               RegValue : The register value to be written to thread register regno.
*
* Return(s)   : DEBUG_SUCCESS : if no error
*               DEBUG_ERR_INVALID_REGID if register ID "regno" is not valid.
*
* Caller(s)   :
*
* Note(s)     : (1)Debug_Main_Write_Register() in Debug_Main module
*
*               (2)
*********************************************************************************************************
*/



CPU_INT08U Debug_HAL_Regs_WriteOne(Debug_TID_t ThreadID, Debug_RegID_t regno, Debug_RegisterWidth RegValue)
{

	if(regno > Debug_HAL_Regs_IDMAX)
			return DEBUG_ERR_INVALID_REGID;

	/***************Note : Debug_SP is incremented or decremented based on stack-growth setting"OS_STK_GROWTH macro" in os_cpu.h *********************/

	/*
	*********************************************************************************************************
	*                                             1st Method to write one register
	*********************************************************************************************************
	*/


	Debug_TCB *Deubg_TCBCurr = (Debug_TCB *)Debug_RTOS_ThreadListPtr[ThreadID] ; /*get a pointer to the thread stack from the Task Control Block */
	Debug_RegisterWidth *Debug_SP = Deubg_TCBCurr->OSTCBStkPtr;
	if (regno == CPSR_Offset)   /*CPSR*/
		*(Debug_SP) = RegValue;

	else if (regno >= R0_Offset && regno <= R12_Offset )
		*(Debug_SP +(regno+1)) = RegValue;
	else if (regno == SP_Offset)
		Deubg_TCBCurr->OSTCBStkPtr = RegValue;
	else if(regno == LR_Offset || regno == PC_Offset)
		*(Debug_SP + regno) = RegValue;

	/*Floating point register IDs are ignored and they are always 0's*/


	/*
	*********************************************************************************************************
	*                                             2nd Method to write one register
	*                                               read-modify-write sequence
	*********************************************************************************************************
	*/


	/*Read all registers*/
/*
	 Debug_HAL_Regs_Readall(ThreadID);
	Modify the required register in register buffer
	 Debug_HAL_RegsBuffer[regno] = RegValue ;
	 Write back the register buffer
	 Debug_HAL_Regs_Writeall(ThreadID) ;
*/

	return DEBUG_SUCCESS;

	}

/*
*********************************************************************************************************
*                                                Debug_HAL_Get_StopSignal()
*
* Description :

*
* Argument(s) : ThreadID :
*               signo    :
*
*
* Return(s)   :
*
* Caller(s)   :  Debug_Main_Report_Halt_Reason() in Debug_Main Module
*
* Note(s)     : (1)
*
*               (2)
*********************************************************************************************************
*/
void Debug_HAL_Get_StopSignal(Debug_TID_t ThreadID,Debug_Signal_t *signo)
{
	 /*For this implementation , we only support one signal which is a breakpoint signal SIGTRAP*/
	*signo = SIGTRAP;

}
