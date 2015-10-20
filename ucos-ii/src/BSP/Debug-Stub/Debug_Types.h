/*
 * Debug_Types.h
 *
 *  Created on: ??�/??�/????
 *      Author: YUSSRY
 */
#include "cpu.h"

#ifndef DEBUG_TYPES_H_
#define DEBUG_TYPES_H_

typedef  CPU_INT32U Debug_MemWidth;
typedef  Debug_MemWidth Debug_addr_t;
typedef  CPU_INT08U Debug_Len_t;

typedef  CPU_INT08U Debug_TID_t;
typedef  CPU_INT08U Debug_Signal_t;
typedef  CPU_INT32U Debug_RegisterWidth;
typedef  Debug_RegisterWidth Debug_RegID_t;
typedef  Debug_RegisterWidth  Debug_RegisterVal;
//#define  Debug_Num_registers 17

//typedef  CPU_INT32S Debug_MemWidth;   /*future discussion about negatives in parsing RSP fields*/


typedef  CPU_INT08U Debug_RDP_header_t;

typedef  CPU_INT08U Debug_RSP_trailer_t;

/*
*********************************************************************************************************
*                                             ERROR CODES
*                                             from 20-220
*********************************************************************************************************
*/
#define  DEBUG_SUCCESS                            0
#define  DEBUG_ERR_INVALID_REGID                  20
#define  DEBUG_ERR_INVALID_HEXChar                21
#define  DEBUG_ERR_INOUTBUF_OVERFLOW              22

#define  Debug_PacketErrorCode                    1

/***Generic Errors***/
/*E06: Bad arguments to command*/
#define  DEBUG_Bad_COMMAND_ARGS                   6
                                           /***memory errors***/
/*E30: Bad address in memory operation*/
#define  DEBUG_MEM_BAD_ADDRESS                    48
/*E35: General error on memory operation*/
#define  DEBUG_MEM_ERROR                          53
                                            /***Breakpoint Error Codes***/

#define  DEBUG_Bad_COMMAND_ARGS_Address           7
#define  DEBUG_BRKPT_ERROR_UnableSET              50

#define  DEBUG_ERROR                              220


#endif /* DEBUG_TYPES_H_ */


