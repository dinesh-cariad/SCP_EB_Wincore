/**
 * \file
 *
 * \brief AUTOSAR NvM
 *
 * This file contains the implementation of the AUTOSAR
 * module NvM.
 *
 * \version 6.17.22
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef NVM_TRACE_H
#define NVM_TRACE_H
/*==================[inclusions]============================================*/

#include <Std_Types.h>

/* Check if BASE_DBG_ENABLE is defined and set to STD_ON.
 * If so, include Base_Dbg.h which includes a custom header file */
#if (defined BASE_DBG_ENABLE)
#if (BASE_DBG_ENABLE == STD_ON)

#include <Base_Dbg.h>

#endif
#endif
#include <NvM_Trace_Internal.h>

/*==================[macros]================================================*/

#ifndef DBG_NVM_JOBERRORNOTIFICATION_ENTRY
/** \brief Entry point of function NvM_JobErrorNotification() */
#define DBG_NVM_JOBERRORNOTIFICATION_ENTRY()
#endif

#ifndef DBG_NVM_INIT_ENTRY
/** \brief Entry point of function NvM_Init() */
#define DBG_NVM_INIT_ENTRY()
#endif

#ifndef DBG_NVM_GLOBALWORKINGSTATUS
/** \brief Change of NvM_GlobalWorkingStatus */
#define DBG_NVM_GLOBALWORKINGSTATUS(a,b)
#endif

#ifndef DBG_NVM_INIT_EXIT
/** \brief Exit point of function NvM_Init() */
#define DBG_NVM_INIT_EXIT()
#endif

#ifndef DBG_NVM_MAINFUNCTION_ENTRY
/** \brief Entry point of function NvM_MainFunction() */
#define DBG_NVM_MAINFUNCTION_ENTRY()
#endif

#ifndef DBG_NVM_GLOBALERRORSTATUS
/** \brief Change of NvM_GlobalErrorStatus */
#define DBG_NVM_GLOBALERRORSTATUS(a,b)
#endif

#ifndef DBG_NVM_GLOBALGENERICSTATUS
/** \brief Change of NvM_GlobalErrorStatus */
#define DBG_NVM_GLOBALGENERICSTATUS(a,b)
#endif

#ifndef DBG_NVM_MAINFUNCTION_EXIT
/** \brief Exit point of function NvM_MainFunction() */
#define DBG_NVM_MAINFUNCTION_EXIT()
#endif

#ifndef DBG_NVM_ASR40_RESTOREBLOCKDEFAULTS_ENTRY
/** \brief Entry point of function NvM_ASR40_RestoreBlockDefaults() */
#define DBG_NVM_ASR40_RESTOREBLOCKDEFAULTS_ENTRY(a,b)
#endif

#ifndef DBG_NVM_ASR40_RESTOREBLOCKDEFAULTS_EXIT
/** \brief Exit point of function NvM_ASR40_RestoreBlockDefaults() */
#define DBG_NVM_ASR40_RESTOREBLOCKDEFAULTS_EXIT(a,b,c)
#endif

#ifndef DBG_NVM_ASR40_GETERRORSTATUS_ENTRY
/** \brief Entry point of function NvM_ASR40_GetErrorStatus() */
#define DBG_NVM_ASR40_GETERRORSTATUS_ENTRY(a,b)
#endif

#ifndef DBG_NVM_ASR40_GETERRORSTATUS_EXIT
/** \brief Exit point of function NvM_ASR40_GetErrorStatus() */
#define DBG_NVM_ASR40_GETERRORSTATUS_EXIT(a,b,c)
#endif

#ifndef DBG_NVM_ASR40_ERASENVBLOCK_ENTRY
/** \brief Entry point of function NvM_ASR40_EraseNvBlock() */
#define DBG_NVM_ASR40_ERASENVBLOCK_ENTRY(a)
#endif

#ifndef DBG_NVM_ASR40_ERASENVBLOCK_EXIT
/** \brief Exit point of function NvM_ASR40_EraseNvBlock() */
#define DBG_NVM_ASR40_ERASENVBLOCK_EXIT(a,b)
#endif

#ifndef DBG_NVM_ASR40_INVALIDATENVBLOCK_ENTRY
/** \brief Entry point of function NvM_ASR40_InvalidateNvBlock() */
#define DBG_NVM_ASR40_INVALIDATENVBLOCK_ENTRY(a)
#endif

#ifndef DBG_NVM_ASR40_INVALIDATENVBLOCK_EXIT
/** \brief Exit point of function NvM_ASR40_InvalidateNvBlock() */
#define DBG_NVM_ASR40_INVALIDATENVBLOCK_EXIT(a,b)
#endif

#ifndef DBG_NVM_ASR40_SETBLOCKPROTECTION_ENTRY
/** \brief Entry point of function NvM_ASR40_SetBlockProtection() */
#define DBG_NVM_ASR40_SETBLOCKPROTECTION_ENTRY(a,b)
#endif

#ifndef DBG_NVM_ASR40_SETBLOCKPROTECTION_EXIT
/** \brief Exit point of function NvM_ASR40_SetBlockProtection() */
#define DBG_NVM_ASR40_SETBLOCKPROTECTION_EXIT(a,b,c)
#endif

#ifndef DBG_NVM_ASR40_SETRAMBLOCKSTATUS_ENTRY
/** \brief Entry point of function NvM_ASR40_SetRamBlockStatus() */
#define DBG_NVM_ASR40_SETRAMBLOCKSTATUS_ENTRY(a,b)
#endif

#ifndef DBG_NVM_ASR40_SETRAMBLOCKSTATUS_EXIT
/** \brief Exit point of function NvM_ASR40_SetRamBlockStatus() */
#define DBG_NVM_ASR40_SETRAMBLOCKSTATUS_EXIT(a,b,c)
#endif

#ifndef DBG_NVM_SETBLOCKLOCKSTATUS_ENTRY
/** \brief Entry point of function NvM_SetBlockLockStatus() */
#define DBG_NVM_SETBLOCKLOCKSTATUS_ENTRY(a,b)
#endif

#ifndef DBG_NVM_SETBLOCKLOCKSTATUS_EXIT
/** \brief Exit point of function NvM_SetBlockLockStatus() */
#define DBG_NVM_SETBLOCKLOCKSTATUS_EXIT(a,b)
#endif

#ifndef DBG_NVM_GETVERSIONINFO_ENTRY
/** \brief Entry point of function NvM_GetVersionInfo() */
#define DBG_NVM_GETVERSIONINFO_ENTRY(a)
#endif

#ifndef DBG_NVM_GETVERSIONINFO_EXIT
/** \brief Exit point of function NvM_GetVersionInfo() */
#define DBG_NVM_GETVERSIONINFO_EXIT(a)
#endif

#ifndef DBG_NVM_JOBENDNOTIFICATION_ENTRY
/** \brief Entry point of function NvM_JobEndNotification() */
#define DBG_NVM_JOBENDNOTIFICATION_ENTRY()
#endif

#ifndef DBG_NVM_JOBENDNOTIFICATION_EXIT
/** \brief Exit point of function NvM_JobEndNotification() */
#define DBG_NVM_JOBENDNOTIFICATION_EXIT()
#endif

#ifndef DBG_NVM_JOBERRORNOTIFICATION_EXIT
/** \brief Exit point of function NvM_JobErrorNotification() */
#define DBG_NVM_JOBERRORNOTIFICATION_EXIT()
#endif

#ifndef DBG_NVM_GLOBALCALLLEVEL
/** \brief Change of NvM_GlobalCallLevel */
#define DBG_NVM_GLOBALCALLLEVEL(a,b)
#endif

#ifndef DBG_NVM_READALL_ENTRY
/** \brief Entry point of function NvM_ReadAll() */
#define DBG_NVM_READALL_ENTRY()
#endif

#ifndef DBG_NVM_READALL_EXIT
/** \brief Exit point of function NvM_ReadAll() */
#define DBG_NVM_READALL_EXIT()
#endif

#ifndef DBG_NVM_VALIDATEALL_ENTRY
/** \brief Entry point of function NvM_ValidateAll() */
#define DBG_NVM_VALIDATEALL_ENTRY()
#endif

#ifndef DBG_NVM_VALIDATEALL_EXIT
/** \brief Exit point of function NvM_ValidateAll() */
#define DBG_NVM_VALIDATEALL_EXIT()
#endif

#ifndef DBG_NVM_WRITEALL_ENTRY
/** \brief Entry point of function NvM_WriteAll() */
#define DBG_NVM_WRITEALL_ENTRY()
#endif

#ifndef DBG_NVM_WRITEALL_EXIT
/** \brief Exit point of function NvM_WriteAll() */
#define DBG_NVM_WRITEALL_EXIT()
#endif

#ifndef DBG_NVM_CANCELWRITEALL_ENTRY
/** \brief Entry point of function NvM_CancelWriteAll() */
#define DBG_NVM_CANCELWRITEALL_ENTRY()
#endif

#ifndef DBG_NVM_CANCELWRITEALL_EXIT
/** \brief Exit point of function NvM_CancelWriteAll() */
#define DBG_NVM_CANCELWRITEALL_EXIT()
#endif

#ifndef DBG_NVM_ASR40_SETDATAINDEX_ENTRY
/** \brief Entry point of function NvM_ASR40_SetDataIndex */
#define DBG_NVM_ASR40_SETDATAINDEX_ENTRY(a,b)
#endif

#ifndef DBG_NVM_ASR40_SETDATAINDEX_EXIT
/** \brief Exit point of function NvM_ASR40_SetDataIndex */
#define DBG_NVM_ASR40_SETDATAINDEX_EXIT(a,b,c)
#endif

#ifndef DBG_NVM_ASR40_GETDATAINDEX_ENTRY
/** \brief Entry point of function NvM_ASR40_GetDataIndex */
#define DBG_NVM_ASR40_GETDATAINDEX_ENTRY(a,b)
#endif

#ifndef DBG_NVM_ASR40_GETDATAINDEX_EXIT
/** \brief Exit point of function NvM_ASR40_GetDataIndex */
#define DBG_NVM_ASR40_GETDATAINDEX_EXIT(a,b,c)
#endif

#ifndef DBG_NVM_ASR40_CANCELJOBS_ENTRY
/** \brief Entry point of function NvM_CancelJobs */
#define DBG_NVM_ASR40_CANCELJOBS_ENTRY(a)
#endif

#ifndef DBG_NVM_ASR40_CANCELJOBS_EXIT
/** \brief Exit point of function NvM_CancelJobs */
#define DBG_NVM_ASR40_CANCELJOBS_EXIT(a,b)
#endif

#ifndef DBG_NVM_ASR40_READBLOCK_ENTRY
/** \brief Entry point of function NvM_ASR40_ReadBlock */
#define DBG_NVM_ASR40_READBLOCK_ENTRY(a,b)
#endif

#ifndef DBG_NVM_ASR40_READBLOCK_EXIT
/** \brief Exit point of function NvM_ASR40_ReadBlock */
#define DBG_NVM_ASR40_READBLOCK_EXIT(a,b,c)
#endif

#ifndef DBG_NVM_ASR40_WRITEBLOCK_ENTRY
/** \brief Entry point of function NvM_ASR40_WriteBlock */
#define DBG_NVM_ASR40_WRITEBLOCK_ENTRY(a,b)
#endif

#ifndef DBG_NVM_ASR40_WRITEBLOCK_EXIT
/** \brief Exit point of function NvM_ASR40_WriteBlock */
#define DBG_NVM_ASR40_WRITEBLOCK_EXIT(a,b,c)
#endif

#ifndef DBG_NVM_ASR42_READPRAMBLOCK_ENTRY
/** \brief Entry point of function NvM_ASR42_ReadPRAMBlock */
#define DBG_NVM_ASR42_READPRAMBLOCK_ENTRY(a)
#endif

#ifndef DBG_NVM_ASR42_READPRAMBLOCK_EXIT
/** \brief Exit point of function NvM_ASR42_ReadPRAMBlock */
#define DBG_NVM_ASR42_READPRAMBLOCK_EXIT(a,b)
#endif

#ifndef DBG_NVM_ASR42_WRITEPRAMBLOCK_ENTRY
/** \brief Entry point of function NvM_ASR42_WritePRAMBlock */
#define DBG_NVM_ASR42_WRITEPRAMBLOCK_ENTRY(a)
#endif

#ifndef DBG_NVM_ASR42_WRITEPRAMBLOCK_EXIT
/** \brief Exit point of function NvM_ASR42_WritePRAMBlock */
#define DBG_NVM_ASR42_WRITEPRAMBLOCK_EXIT(a,b)
#endif

#ifndef DBG_NVM_ASR42_RESTOREPRAMBLOCKDEFAULTS_ENTRY
/** \brief Entry point of function NvM_ASR42_RestorePRAMBlockDefaults() */
#define DBG_NVM_ASR42_RESTOREPRAMBLOCKDEFAULTS_ENTRY(a)
#endif

#ifndef DBG_NVM_ASR42_RESTOREPRAMBLOCKDEFAULTS_EXIT
/** \brief Exit point of function NvM_ASR42_RestorePRAMBlockDefaults() */
#define DBG_NVM_ASR42_RESTOREPRAMBLOCKDEFAULTS_EXIT(a,b)
#endif

#ifndef DBG_NVM_FIRSTINITALL_ENTRY
/** \brief Entry point of function NvM_FirstInitAll() */
#define DBG_NVM_FIRSTINITALL_ENTRY()
#endif

#ifndef DBG_NVM_FIRSTINITALL_EXIT
/** \brief Exit point of function NvM_FirstInitAll() */
#define DBG_NVM_FIRSTINITALL_EXIT()
#endif

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef NVM_TRACE_H */
/*==================[end of file]===========================================*/
