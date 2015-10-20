/*
 * Debug_Main.c

 *
 *  Created on: ??þ/??þ/????
 *      Author: YUSSRY
 */


/*
 * template.c
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

#define Debug_Main_IMPORT
#include "Debug_Main.h"
#include "Debug_RSP.h"
#include "Debug_HAL.h"
#include "Debug_RTOS.h"

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

/*Register-related Enumeration types*/
/*Registers' ID as proposed by GDB for ARM-architecture in arm-core.xml file in "<GDB-Source-Tree>/gdb/features"  */
typedef enum
		{
			R0,
			R1,
			R2,
			R3,
			R4,
			R5,
			R6,
			R7,
			R8,
			R9,
			R10,
			R11,
			R12,
			SP,
			LR,
			PC,
			CPSR=25
		}Register_ID;

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

/*Register -related manipulating functions*/
/*may need to be migrated into separate module Ex: Debug_Registers.h*/
/*
static void Debug_Main_get_thread_registers(Debug_TID_t ThreadID);
static void Debug_Main_set_thread_registers(Debug_TID_t ThreadID);
static void Debug_Main_get_thread_register(Debug_RegID_t regstring_ID, Debug_TID_t ThreadID);
static void Debug_Main_set_thread_register(Debug_RegID_t regstring_ID, Debug_TID_t ThreadID);
*/

/*Memory -Related Mainpulating Functions*/
/*may need to be migrated to separate module Ex: Debug_Memory.h*/


static void Debug_Main_Init_CommandFunctions();
static CPU_INT32U InsertBpInsideBplist(Debug_MemWidth *bPAddress);
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
*                                               Debug_Main_Init()
*
* Description :Initialize Debug_Main module
*               - Register its capabilities to RSP commands
*               -
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : not known yet "may be the debug-stub task body"
*
* Note(s)     : (1)
*
*               (2)
*********************************************************************************************************
*/




void Debug_Main_Init()
 {
   Debug_RSP_Init();
   Debug_Main_Init_CommandFunctions();
   Debug_RTOS_Init();
   Debug_HAL_init();
   /*initialize the commands data structure*/

   Command_opts_gG.RegisterarrayPtr = Debug_HAL_RegsBuffer;    /*Connect between register buffer used in both RSP module  and HAL Module*/
   Command_opts_gG.bytecount = Debug_HAL_Regs_BYTES;

   Command_opts_mM.MemoryArrayptr = Debug_HAL_MemoryBuffer;    /*Connect between memory buffer used in both RSP module  and Main Module*/

 }



/*Main debugger-Stub functions*/
/*should not be migrated to other module*/

/*
*********************************************************************************************************
*                                               Debug_Main_Get_Last_Signal()
*
* Description :
*               -
*               -
*
* Argument(s) : ThreadID : the target thread on whhich this command handling apply
*               CommandParam : parameters as received in corresponding command packet
*
* Return(s)   : none.
*
* Caller(s)   : Debug_RSP_Process_Packet() via Debug_RSP_Commands_Functions callbacks
*
* Note(s)     : (1)
*
*               (2)
*********************************************************************************************************
*/
CPU_INT08U Debug_Main_Report_Halt_Reason(Debug_TID_t ThreadID,void *CommandParam)
{

	/*IF needed, CommandParam is custed to the corresponding commands-packet data-structure to be manipulated as defined in Dedug_RSP.h*/
	Debug_RSP_Info_HaltSig  *Sig_options = (Debug_RSP_Info_HaltSig  *) CommandParam;
	//Debug_HAL_Get_StopSignal(ThreadID, &(Command_opts_HaltSig.Signum));

	Debug_HAL_Get_StopSignal(ThreadID, &(Sig_options->Signum));
	return DEBUG_SUCCESS;

	}
/*
*********************************************************************************************************
*                                               Debug_Main_Read_Register()
*
* Description : Read a single register from a thread ThreadID register Context
*
*
* Argument(s) : ThreadID : the target thread on which this command handling apply
*               CommandParam : parameters as received in corresponding command packet
*
* Return(s)   : DEBUG_SUCCESS if no error
*               DEBUG_ERR_INVALID_REGID if the neede register ID is invalid
*
* Caller(s)   : Debug_RSP_Process_Packet() via Debug_RSP_Commands_Functions callbacks
*
* Note(s)     : (1)
*
*               (2)
*********************************************************************************************************
*/
CPU_INT08U Debug_Main_Read_Register(Debug_TID_t ThreadID,void *CommandParam)
{


	/*IF needed, CommandParam is casted to the corresponding commands-packet data-structure to be manipulated as defined in Dedug_RSP.h*/
	Debug_RSP_Info_pP * p_options = (Debug_RSP_Info_pP *)  CommandParam;

   return (Debug_HAL_Regs_ReadOne(ThreadID, p_options->RegID, &(p_options->RegVal)));



	}

/*
*********************************************************************************************************
*                                               Debug_Main_Write_Register()
*
* Description :
*               -
*               -
*
* Argument(s) : ThreadID : the target thread on whhich this command handling apply
*               CommandParam : parameters as received in corresponding command packet
*
* Return(s)   : none.
*
* Caller(s)   : Debug_RSP_Process_Packet() via Debug_RSP_Commands_Functions callbacks
*
* Note(s)     : (1)
*
*               (2)
*********************************************************************************************************
*/

CPU_INT08U Debug_Main_Write_Register(Debug_TID_t ThreadID,void *CommandParam)
{
	/*IF needed, CommandParam is casted to the corresponding commands-packet data-structure to be manipulated as defined in Dedug_RSP.h*/

	Debug_RSP_Info_pP * P_options = (Debug_RSP_Info_pP *)  CommandParam;

	return ((Debug_HAL_Regs_WriteOne(ThreadID, P_options->RegID, P_options->RegVal)));

}

/*
*********************************************************************************************************
*                                               Debug_Main_Read_Registers()
*
* Description :
*               -
*               -
*
* Argument(s) : ThreadID : the target thread on whhich this command handling apply
*               CommandParam : parameters as received in corresponding command packet
*
* Return(s)   : none.
*
* Caller(s)   : Debug_RSP_Process_Packet() via Debug_RSP_Commands_Functions callbacks
*
* Note(s)     : (1)
*
*               (2)
*********************************************************************************************************
*/
CPU_INT08U Debug_Main_Read_Registers(Debug_TID_t ThreadID,void *CommandParam)
{
	/*IF needed, CommandParam is casted to the corresponding commands-packet data-structure to be manipulated as defined in Dedug_RSP.h*/
	Debug_HAL_Regs_Readall(ThreadID);

	return DEBUG_SUCCESS;

}

/*
*********************************************************************************************************
*                                              Debug_Main_Write_Registers()
*
* Description :
*               -
*               -
*
* Argument(s) : ThreadID : the target thread on whhich this command handling apply
*               CommandParam : parameters as received in corresponding command packet
*
* Return(s)   : none.
*
* Caller(s)   : Debug_RSP_Process_Packet() via Debug_RSP_Commands_Functions callbacks
*
* Note(s)     : (1)
*
*               (2)
*********************************************************************************************************
*/

CPU_INT08U Debug_Main_Write_Registers(Debug_TID_t ThreadID,void *CommandParam)
{
	/*IF needed, CommandParam is casted to the corresponding commands-packet data-structure to be manipulated as defined in Dedug_RSP.h*/
	Debug_HAL_Regs_Writeall(ThreadID);
	return DEBUG_SUCCESS;
}


/*
*********************************************************************************************************
*                                               ()
*
* Description :
*               -
*               -
*
* Argument(s) : ThreadID : the target thread on whhich this command handling apply
*               CommandParam : parameters as received in corresponding command packet
*
* Return(s)   : none.
*
* Caller(s)   : Debug_RSP_Process_Packet() via Debug_RSP_Commands_Functions callbacks
*
* Note(s)     : (1)
*
*               (2)
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               Debug_Main_Read_memory()
*
* Description :
*               -
*               -
*
* Argument(s) : ThreadID : the target thread on whhich this command handling apply
*               CommandParam : parameters as received in corresponding command packet
*
* Return(s)   : DEBUG_SUCCESS if no error
*               DEBUG_ERROR   if required length exceeds Debug_RSP_IN_OUTBUFMax
*
* Caller(s)   : Debug_RSP_Process_Packet() via Debug_RSP_Commands_Functions callbacks
*
* Note(s)     : (1)
*
*               (2)
*********************************************************************************************************
*/

CPU_INT08U Debug_Main_Read_memory(Debug_TID_t ThreadID,void *CommandParam)
{
	/*IF needed, CommandParam is casted to the corresponding commands-packet data-structure to be manipulated as defined in Dedug_RSP.h*/
	Debug_RSP_Info_mM *m_options = (Debug_RSP_Info_mM *)CommandParam;
	Debug_MemWidth *src = m_options->start_address;
	Debug_MemWidth *dst =  Debug_HAL_MemoryBuffer;
	//CPU_INT08U *dst = &Debug_RSP_Payload_OutBuf[0];
	/*read in Temp Memory Buffer*/

	//Debug_MemWidth *dst =(CPU_INT08U *) m_options->MemoryArrayptr;
	Debug_Len_t count = 0;

	while(/*count < (Debug_RSP_IN_OUTBUFMax/2) &&*/ count < m_options->Count)
	{
		*(dst++) = *(src++);
       count += 4;
	}

	/*Needed Bytes exceeds the Maximum length of the outbuffer/2 as it's bytes not hexchars*/
	//if(count >= (Debug_RSP_IN_OUTBUFMax/2))
		//return DEBUG_ERROR;

	return DEBUG_SUCCESS;

}

/*
*********************************************************************************************************
*                                              Debug_Main_Write_memory ()
*
* Description :
*               -
*               -
*
* Argument(s) : ThreadID : the target thread on whhich this command handling apply
*               CommandParam : parameters as received in corresponding command packet
*
* Return(s)   : DEBUG_SUCCESS  : if Memory values were written successfully.
*              DEBUG_MEM_ERROR :if stub could not write memory
*
* Caller(s)   : Debug_RSP_Process_Packet() via Debug_RSP_Commands_Functions callbacks
*
* Note(s)     : (1)
*
*               (2)
*********************************************************************************************************
*/

CPU_INT08U Debug_Main_Write_memory(Debug_TID_t ThreadID,void *CommandParam)
{
	/*IF needed, CommandParam is casted to the corresponding commands-packet data-structure to be manipulated as defined in Dedug_RSP.h*/
	Debug_RSP_Info_mM *M_options = (Debug_RSP_Info_mM *)CommandParam;
	CPU_INT08U *src = (CPU_INT08U *)M_options->MemoryArrayptr;
	CPU_INT08U *dst = M_options->start_address;
	Debug_Len_t count = 0;

	while( count < M_options->Count)
	{
       *(dst++) = *(src++);
       count ++;
	}
      count = 0;
   src = (CPU_INT08U *)M_options->MemoryArrayptr;
   dst = M_options->start_address;

    while( count < M_options->Count)
    	{
           if(*(dst++) != *(src++))
        	   return DEBUG_MEM_ERROR;
    	}

    /*Flush the cache */
   		/*Ensure Values written successfully*/
    extern void Xil_L1DCacheInvalidateRange(unsigned int , unsigned );
   	Xil_L1DCacheInvalidateRange(M_options->start_address,M_options->Count);

    return DEBUG_SUCCESS;


}

/*
*********************************************************************************************************
*                                               Debug_Main_Step_machine_instruction()
*
* Description :
*               -
*               -
*
* Argument(s) : ThreadID : the target thread on whhich this command handling apply
*               CommandParam : parameters as received in corresponding command packet
*
* Return(s)   : none.
*
* Caller(s)   : Debug_RSP_Process_Packet() via Debug_RSP_Commands_Functions callbacks
*
* Note(s)     : (1)
*
*               (2)
*********************************************************************************************************
*/

CPU_INT08U Debug_Main_Step_machine_instruction(Debug_TID_t ThreadID,void *CommandParam)
{
	/*IF needed, CommandParam is custed to the corresponding commands-packet data-structure to be manipulated as defined in Dedug_RSP.h*/
	return DEBUG_SUCCESS;

}

/*
*********************************************************************************************************
*                                              Debug_Main_Resume ()
*
* Description :
*               -
*               -
*
* Argument(s) : ThreadID : the target thread on whhich this command handling apply
*               CommandParam : parameters as received in corresponding command packet
*
* Return(s)   : none.
*
* Caller(s)   : Debug_RSP_Process_Packet() via Debug_RSP_Commands_Functions callbacks
*
* Note(s)     : (1)
*
*               (2)
*********************************************************************************************************
*/

CPU_INT08U Debug_Main_Resume( Debug_TID_t ThreadID,void *CommandParam)
{
	/*IF needed, CommandParam is casted to the corresponding commands-packet data-structure to be manipulated as defined in Dedug_RSP.h*/
	return DEBUG_SUCCESS;

}

/*
*********************************************************************************************************
*                                               Debug_Main_InsertBreakPoint()
*
* Description :
*               -
*               -
*
* Argument(s) : ThreadID : the target thread on whhich this command handling apply
*               CommandParam : parameters as received in corresponding command packet
*
* Return(s)   : none.
*
* Caller(s)   : Debug_RSP_Process_Packet() via Debug_RSP_Commands_Functions callbacks
*
* Note(s)     : (1)
*
*               (2)
*********************************************************************************************************
*/

CPU_INT08U Debug_Main_InsertBreakPoint( Debug_TID_t ThreadID,void *CommandParam)
{
	//ToDo: Validate_Break_Address
			// InsertBpInsideBplist
		int error = 0;
		Debug_RSP_Info_znZn * BP_options=(Debug_RSP_Info_znZn *)CommandParam;
		error = InsertBpInsideBplist(BP_options->BkptAddress);
		//ToDO: check Error.
		if (error != DEBUG_SUCCESS)
			return DEBUG_BRKPT_ERROR_UnableSET;

		return DEBUG_SUCCESS;

}

/*
*********************************************************************************************************
*                                               Debug_Main_RemoveBreakPoint()
*
* Description :
*               -
*               -
*
* Argument(s) : ThreadID : the target thread on whhich this command handling apply
*               CommandParam : parameters as received in corresponding command packet
*
* Return(s)   : none.
*
* Caller(s)   : Debug_RSP_Process_Packet() via Debug_RSP_Commands_Functions callbacks
*
* Note(s)     : (1)
*
*               (2)
*********************************************************************************************************
*/

CPU_INT08U Debug_Main_RemoveBreakPoint( Debug_TID_t ThreadID,void *CommandParam)
{
	/*IF needed, CommandParam is custed to the corresponding commands-packet data-structure to be manipulated as defined in Dedug_RSP.h*/
	int i;

	Debug_RSP_Info_znZn * BP_options=(Debug_RSP_Info_znZn *)CommandParam;

	if(BP_options->Kind==BP_BREAKPOINT)// SoftWare BreakPoint
	{
		for (i = 0; i < GDB_MAX_BREAKPOINTS; i++) {
			if ((Gdb_BreakList[i].state == BP_SET) &&
					(Gdb_BreakList[i].bpt_addr == BP_options->BkptAddress)) {
				Gdb_BreakList[i].state = BP_REMOVED;
				return DEBUG_SUCCESS;
			}
		}
	}
    return -1;

	//return DEBUG_SUCCESS;

}
CPU_INT32U Gdb_Arch_Remove_BreakPoint(unsigned long ,char * );

CPU_INT08U Deactivate_SW_BreakPoints()
{
	unsigned long addr;
	    int error = 0;
	    int i;

	    for (i = 0; i < GDB_MAX_BREAKPOINTS; i++) {
	        if (Gdb_BreakList[i].state != BP_ACTIVE)
	            continue;
	        addr = Gdb_BreakList[i].bpt_addr;
	        error = Gdb_Arch_Remove_BreakPoint(addr,
	                (char *)(Gdb_BreakList[i].saved_instr));
	        if (error)
	            return error;

	        extern void Xil_ICacheInvalidateLine(unsigned int adr);
	       	        Xil_ICacheInvalidateLine(addr);
	        Gdb_BreakList[i].state = BP_SET;
	    }
	return 0;
}

CPU_INT32U BreakPointMemWrite(void * srcAddress,void  * desAddress,unsigned int size)
{
		int i = 0;
	    char *dst_ptr = (char *)desAddress;
	    char *src_ptr = (char *)srcAddress;
	    for (i = 0; i<size; i++) {
	           *(dst_ptr++) = *(src_ptr++);
	       }
	    //TODO: Check Memory Fault handler

	return 0;
}
CPU_INT32U Gdb_Arch_Remove_BreakPoint(unsigned long bPAddress,char * savedInst)
{
	//BreakPointMemWrite()
	int err;

	    err = BreakPointMemWrite((void *)bPAddress, (void *)savedInst, BREAK_INSTR_SIZE);
		if (err)
			return err;

	    return 0;
}

CPU_INT32U Gdb_Arch_Set_BreakPoint(unsigned long bPAddress,char * savedInst)
{
	//BreakPointMemWrite()
	int err;

	    err = BreakPointMemWrite((void *)bPAddress, (void *)savedInst, BREAK_INSTR_SIZE);
		if (err)
			return err;

	    return BreakPointMemWrite( (void *)arch_gdb_ops.gdb_bpt_instr,(void *)bPAddress,
					  BREAK_INSTR_SIZE);
}
CPU_INT32U Activate_Sw_BreakPoints()
{
	 unsigned long addr;
	    int error = 0;
	    int i;

	    for (i = 0; i < GDB_MAX_BREAKPOINTS; i++) {
	        if (Gdb_BreakList[i].state !=BP_SET)
	            continue;

	        addr = Gdb_BreakList[i].bpt_addr;
	        error = Gdb_Arch_Set_BreakPoint(addr,
	                (char *)(Gdb_BreakList[i].saved_instr));
	        if (error)
	            return error;
	        //TODO: invalidateIcash first
	        extern void Xil_ICacheInvalidateLine(unsigned int adr);
	        Xil_ICacheInvalidateLine(addr);
	        Gdb_BreakList[i].state = BP_ACTIVE;
	    }
	    return 0;
	/*Debug_RSP_Info_mM Command_opts_mM;
	Debug_Main_Write_memory()*/
}

/*
*********************************************************************************************************
*                                               Debug_Main_Init_CommandFunctions()
*
* Description : It assign each command packet to its corresponding handling routine
*               this handling routine is provided-by/implemented in Debug_Main module
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Debug_Main_Init()
*
* Note(s)     : (1)
*
*               (2)
*********************************************************************************************************
*/

static void Debug_Main_Init_CommandFunctions()
 {
   Debug_RSP_Commands_Functions[Debug_RSP_m]             = Debug_Main_Read_memory;  /*read memory range $m,<addr>,<len>#CS*/
   Debug_RSP_Commands_Functions[Debug_RSP_M]             = Debug_Main_Write_memory;  /*write memory range $M,<addr>,<len>#CS*/
   Debug_RSP_Commands_Functions[Debug_RSP_g]             = Debug_Main_Read_Registers ;
   Debug_RSP_Commands_Functions[Debug_RSP_G]             = Debug_Main_Write_Registers;
   Debug_RSP_Commands_Functions[Debug_RSP_p]             = Debug_Main_Read_Register;
   Debug_RSP_Commands_Functions[Debug_RSP_P]             = Debug_Main_Write_Register;
   Debug_RSP_Commands_Functions[Debug_RSP_c]             = Debug_Main_Resume;
   Debug_RSP_Commands_Functions[Debug_RSP_s]             = Debug_Main_Step_machine_instruction;
   Debug_RSP_Commands_Functions[Debug_RSP_z0]            = Debug_Main_RemoveBreakPoint;
   Debug_RSP_Commands_Functions[Debug_RSP_Z0]            = Debug_Main_InsertBreakPoint;
   Debug_RSP_Commands_Functions[Debug_RSP_z1]            = Debug_Main_RemoveBreakPoint;
   Debug_RSP_Commands_Functions[Debug_RSP_Z1]            = Debug_Main_InsertBreakPoint;
   Debug_RSP_Commands_Functions[Debug_RSP_ReportHaltReason] = Debug_Main_Report_Halt_Reason;
 }



static CPU_INT32U InsertBpInsideBplist(Debug_MemWidth *bPAddress)
{
	int breakno = -1;
	    int i;

	    for (i = 0; i < GDB_MAX_BREAKPOINTS; i++) {
	        if ((Gdb_BreakList[i].state == BP_SET) &&
	                (Gdb_BreakList[i].bpt_addr == (*bPAddress)))
	            return -1;
	    }
	    for (i = 0; i < GDB_MAX_BREAKPOINTS; i++) {
	        if (Gdb_BreakList[i].state == BP_REMOVED) {
	            breakno = i;
	            break;
	        }
	    }

	    if (breakno == -1) {
	        for (i = 0; i < GDB_MAX_BREAKPOINTS; i++) {
	            if (Gdb_BreakList[i].state == BP_UNDEFINED) {
	                breakno = i;
	                break;
	            }
	        }
	    }
	    if (breakno == -1)
	           return -1;

	    Gdb_BreakList[breakno].state = BP_SET;
	    Gdb_BreakList[breakno].type = BP_BREAKPOINT;
	    Gdb_BreakList[breakno].bpt_addr = (*bPAddress);
	return DEBUG_SUCCESS;
}
int Gdb_Handle_Exception(int signo)
{
	//TODO:
	Deactivate_SW_BreakPoints();
	Mem_Set(Debug_RSP_Payload_OutBuf,0,Debug_RSP_IN_OUTBUFMax);
	 char *ptr;
	 ptr=Debug_RSP_Payload_OutBuf;
	 *ptr++ = 'T';
	  Debug_Word2Hex(&signo,&ptr);
	  Debug_RSP_Put_Packet(&Debug_RSP_Payload_OutBuf,0);

	return 0;
}
