/*
 * Debug_HAL.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: YUSSRY
 */

#ifndef DEBUG_HAL_H_
#define DEBUG_HAL_H_

/*
 * Debug_RTOS.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: YUSSRY
 */

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include "Debug_Types.h"
#include "Debug_RTOS.h"
//#define Structures_IMPORT
#include "Shared/DataStructures.h"


/*
*********************************************************************************************************
 *                                                 EXTERNS
*********************************************************************************************************
*/
#ifdef Debug_HAL_IMPORT
	#define EXTERN
#else
	#define EXTERN extern
#endif

/*
*********************************************************************************************************
*                                        DEFAULT CONFIGURATION
*********************************************************************************************************
*/

#define Debug_HAL_Regs_IDMAX    25                          /*Maximum ID for registers ,  CPSR*/
#define Debug_HAL_MEMWORDMAX   	50  						/*100 * 4 = 400  byte*/

		/*
		****************************************************************************************
		*                                       Registers' CONFIGURATION
		*****************************************************************************************
		*/

#define  Debug_HAL_Regs_GPNUM    17  										 /*as in org.gnu.gdb.arm.core feature of GDB standard features*/
#define  Debug_HAL_Regs_FPNUM	 9    										/*as in org.gnu.gdb.arm.fpa feature of GDB standard features*/
#define  Debug_HAL_Regs_MAX     (Debug_HAL_Regs_GPNUM + Debug_HAL_Regs_FPNUM)
#define  Debug_HAL_Regs_BYTES   (Debug_HAL_Regs_MAX <<2 )


/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/


EXTERN Debug_RegisterWidth Debug_HAL_RegsBuffer[Debug_HAL_Regs_MAX]
#ifdef Debug_HAL_IMPORT
                        = {0}
#endif
;
EXTERN Debug_MemWidth      Debug_HAL_MemoryBuffer[Debug_HAL_MEMWORDMAX]
#ifdef Debug_HAL_IMPORT
                        = {0}
#endif
;

/*Signals numbering as defined by GDB source code in <GDB-SourceTree>/include/gdb/gdb.def*/
enum {

	SIGTRAP = 5
}Debug_HAL_Regs_GDBNumbers;

/*Registers' order in RSP packet as defined by GDB standard feature org.gnu.gdb.arm.core , org.gnu.gdb.arm.fpa */

 enum {
	R0_Offset,
	R1_Offset,
	R2_Offset,
	R3_Offset,
	R4_Offset,
	R5_Offset,
	R6_Offset,
	R7_Offset,
	R8_Offset,
	R9_Offset,
	R10_Offset,
	R11_Offset,
	R12_Offset,
	SP_Offset,
	LR_Offset,
	PC_Offset,   /*15*/
	F0_Offset,
	F1_Offset,
	F2_Offset,
	F3_Offset,
	F4_Offset,
	F5_Offset,
	F6_Offset,
	F7_Offset,
	FPS_Offset,
	CPSR_Offset /*25*/

} Debug_HAL_Regs_GDBOrder;

//EXTERN Debug_HAL_Regs_GDBOrder GDB_RegOrder;
/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/
EXTERN void Debug_HAL_init(void);

EXTERN void Debug_HAL_Regs_Readall(Debug_TID_t ThreadID);
EXTERN void Debug_HAL_Regs_Writeall(Debug_TID_t ThreadID);

EXTERN CPU_INT08U Debug_HAL_Regs_ReadOne(Debug_TID_t ThreadID, Debug_RegID_t regno, Debug_RegisterWidth *RegValue );
EXTERN CPU_INT08U Debug_HAL_Regs_WriteOne(Debug_TID_t ThreadID, Debug_RegID_t regno, Debug_RegisterWidth RegValue);
EXTERN void Debug_HAL_Get_StopSignal(Debug_TID_t ThreadID,Debug_Signal_t *signo);


/*
*********************************************************************************************************
*                                          CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                              MODULE END
*********************************************************************************************************
*/


#undef Debug_HAL_IMPORT

#undef EXTERN

#endif /* DEBUG_HAL_H_ */
