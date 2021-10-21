/**
 * \file
 *
 * \brief AUTOSAR Eep
 *
 * This file contains the implementation of the AUTOSAR
 * module Eep.
 *
 * \version 1.0.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]============================================*/
#include <Eep_Trace.h>          /* Debug&Trace header */
#include <Eep.h>                /* public API and module config */

#if(EEP_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>
#endif /* EEP_DEV_ERROR_DETECT == STD_ON */

#include <Dem.h>                /* public API for Dem module */
#include <stdio.h>              /* for file operations */
#include <string.h>             /* for memcmp() function */

/*
 * Misra-C:2004 Deviations:
 *
 * MISRA-1) Deviated Rule: 20.2 (required)
 *   The names of standard library macros, objects and functions shall
 *   not be reused.
 *
 *   Reason:
 *   'FILE' is used as typedef.
 *
 * MISRA-2) Deviated Rule: 17.4 (required)
 *   Array indexing shall be the only allowed form of pointer arithmetic
 *
 *
 *   Reason:
 *   This is low-level memory module and pointer arithmetic is
 *   necessary.
 *
 * MISRA-3) Deviated Rule: 20.9 (required)
 *   The input/output library <stdio.h> shall not be used in production
 *   code.
 *
 *   Reason:
 *   The functionality of EEPROM is in this module implementation
 *   emulated in I/O file system, so usage of <stdio.h> is
 *   necessary.
 *
 * MISRA-4) Deviated Rule: 11.5 (required)
 *   A cast shall not be performed that removes any const or volatile
 *   qualification from the type addressed by a pointer.
 *
 *   Reason:
 *   'const' qualification is intentionaly removed as there is shared
 *   static function that process both const/non-const data
 *
*/

/*
 * Compiler Warnings:
 *
 * COMPILER-1) passing argument discards qualifiers from pointer target type
 *
 *
 *
 *   Reason:
 *   Eep_Write, Eep_Compare uses same static function Eep_CopyJobValues
 *   as Eep_Read, Eep_Erase.
 *
*/

/*==================[macros]================================================*/
/*------------------[AUTOSAR vendor identification check]-------------------*/

#ifndef EEP_VENDOR_ID /* configuration check */
#error EEP_VENDOR_ID must be defined
#endif

#if (EEP_VENDOR_ID != 1U) /* vendor check */
#error EEP_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#ifndef EEP_AR_RELEASE_MAJOR_VERSION /* configuration check */
#error EEP_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (EEP_AR_RELEASE_MAJOR_VERSION != 4U)
#error EEP_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef EEP_AR_RELEASE_MINOR_VERSION /* configuration check */
#error EEP_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (EEP_AR_RELEASE_MINOR_VERSION != 0U )
#error EEP_AR_RELEASE_MINOR_VERSION wrong (!= 0U)
#endif

#ifndef EEP_AR_RELEASE_REVISION_VERSION /* configuration check */
#error EEP_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (EEP_AR_RELEASE_REVISION_VERSION != 3U )
#error EEP_AR_RELEASE_REVISION_VERSION wrong (!= 3U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#ifndef EEP_SW_MAJOR_VERSION /* configuration check */
#error EEP_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (EEP_SW_MAJOR_VERSION != 1U)
#error EEP_SW_MAJOR_VERSION wrong (!= 1U)
#endif

#ifndef EEP_SW_MINOR_VERSION /* configuration check */
#error EEP_SW_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (EEP_SW_MINOR_VERSION < 0U)
#error EEP_SW_MINOR_VERSION wrong (< 0U)
#endif

#ifndef EEP_SW_PATCH_VERSION /* configuration check */
#error EEP_SW_PATCH_VERSION must be defined
#endif

/* patch version check */
#if (EEP_SW_PATCH_VERSION < 9U)
#error EEP_SW_PATCH_VERSION wrong (< 9U)
#endif

/* defines how many bytes are erased in one call of Eep_MainFunction() */
#define EEP_ERASE_BYTES_ONECYCLE    4U

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/
#define EEP_START_SEC_CODE
#include <Eep_MemMap.h>

STATIC FUNC(Std_ReturnType, EEP_CODE) Eep_CopyJobValues(
  VAR(Eep_AddressType, EEP_VAR) EepromAddress,
  P2VAR(uint8, AUTOMATIC, EEP_APPL_CONST) DataBufferPtr,
  VAR(Eep_LengthType, EEP_VAR) Length,
  VAR(Eep_JobType, EEP_VAR) Job
);
STATIC FUNC(void, EEP_CODE) Eep_ProcessJobEndNotification(void);
STATIC FUNC(void, EEP_CODE) Eep_ProcessJobErrorNotification(void);
STATIC FUNC(void, EEP_CODE) Eep_JobWriteProcess(void);
STATIC FUNC(void, EEP_CODE) Eep_JobReadProcess(void);
STATIC FUNC(void, EEP_CODE) Eep_JobEraseProcess(void);
STATIC FUNC(void, EEP_CODE) Eep_JobCompareProcess(void);

#define EEP_STOP_SEC_CODE
#include <Eep_MemMap.h>
/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/
#define EEP_START_SEC_CONST_UNSPECIFIED
#include <Eep_MemMap.h>

/* defines eeprom erase pattern */
STATIC CONST(uint8, EEP_CONST) Eep_BlankArray[EEP_ERASE_BYTES_ONECYCLE]={0xFF,0xFF,0xFF,0xFF};

#define EEP_STOP_SEC_CONST_UNSPECIFIED
#include <Eep_MemMap.h>

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/
#define EEP_START_SEC_VAR_FAST_UNSPECIFIED
#include <Eep_MemMap.h>

/* stores current module configuration used and passed via Eep_Init() */
STATIC P2CONST(Eep_ConfigType, AUTOMATIC, EEP_APPL_DATA) Eep_ConfigPtr = NULL_PTR;

/* stores FILE handler where Eeprom space is emulated */
/* Deviation MISRA-1 */
STATIC P2VAR(FILE, AUTOMATIC, EEP_APPL_DATA) Eep_FilePtr = NULL_PTR;

/* used for Eep_Compare() purposes as internal data buffer, it's size has to
   be properly setup! */
STATIC VAR(uint8, EEP_VAR) Eep_DataBuffer[EEP_DATABUFFER_SIZE];

/* stores job Eeprom start address */
STATIC VAR(Eep_AddressType, EEP_VAR) Eep_JobEepromAddressStart;

/* stores job user data buffer address */
STATIC P2VAR(uint8, AUTOMATIC, EEP_APPL_DATA) Eep_JobDataBufferPtr;

/* stores job length */
STATIC VAR(Eep_LengthType, EEP_VAR) Eep_JobLength;

/* stores job type: EEP_JOB_READ, EEP_JOB_WRITE, EEP_JOB_ERASE, EEP_JOB_COMPARE */
STATIC VAR(Eep_JobType, EEP_VAR) Eep_Job;

/* these variables are set in Eep_Init(), Eep_SetMode() according to
   default/required Eep mode */
STATIC VAR(uint32, EEP_VAR) Eep_ReadBlockSize;
STATIC VAR(uint32, EEP_VAR) Eep_WriteBlockSize;

/* stores Eep state */
STATIC VAR(MemIf_StatusType, EEP_VAR) Eep_Status = MEMIF_UNINIT;

/* stores Eep mode */
STATIC VAR(MemIf_ModeType, EEP_VAR) Eep_Mode = MEMIF_MODE_SLOW;

/* stores Eep job result */
STATIC VAR(MemIf_JobResultType, EEP_VAR) Eep_JobResult = MEMIF_JOB_OK;

#define EEP_STOP_SEC_VAR_FAST_UNSPECIFIED
#include <Eep_MemMap.h>

#define EEP_START_SEC_CODE
#include <Eep_MemMap.h>

/*==================[internal function definitions]=========================*/
STATIC FUNC(void, EEP_CODE) Eep_ProcessJobEndNotification(void)
{
  DBG_EEP_PROCESSJOBENDNOTIFICATION_ENTRY();

  if(Eep_ConfigPtr->jobEndNotificationPtr!=NULL_PTR)
  {
    /* Call EepJobEndNotification function if configured */
    Eep_ConfigPtr->jobEndNotificationPtr();
  }
  else
  {
    /* Callback notification configured as null pointer */
  }

  DBG_EEP_PROCESSJOBENDNOTIFICATION_EXIT();
}

STATIC FUNC(void, EEP_CODE) Eep_ProcessJobErrorNotification(void)
{
  DBG_EEP_PROCESSJOBERRORNOTIFICATION_ENTRY();

  if(Eep_ConfigPtr->jobErrorNotificationPtr!=NULL_PTR)
  {
    /* Call EepJobErrorNotification function if configured */
    Eep_ConfigPtr->jobErrorNotificationPtr();
  }
  else
  {
    /* Callback notification configured as null pointer */
  }

  DBG_EEP_PROCESSJOBERRORNOTIFICATION_EXIT();
}

STATIC FUNC(Std_ReturnType, EEP_CODE) Eep_CopyJobValues(
  VAR(Eep_AddressType, EEP_VAR) EepromAddress,
  P2VAR(uint8, AUTOMATIC, EEP_APPL_CONST) DataBufferPtr,
  VAR(Eep_LengthType, EEP_VAR) Length,
  VAR(Eep_JobType, EEP_VAR) Job
)
{
  VAR(Std_ReturnType, EEP_VAR) RetVal=E_NOT_OK;

  DBG_EEP_COPYJOBVALUES_ENTRY(EepromAddress, DataBufferPtr, Length, Job);

  /* move to required position within file */
  if(fseek(Eep_FilePtr, EepromAddress, SEEK_SET)==0)
  {
    /* copy job values, and initiate the job */
    Eep_JobEepromAddressStart = EepromAddress;
    Eep_JobDataBufferPtr = DataBufferPtr;
    Eep_JobLength = Length;
    Eep_Job = Job;
    Eep_Status = MEMIF_BUSY;
    Eep_JobResult = MEMIF_JOB_PENDING;
    RetVal = E_OK;
  }
  else
  {
    /* file failure, finalize the job */
    Eep_Status = MEMIF_IDLE;
    Eep_JobResult = MEMIF_JOB_FAILED;
    RetVal = E_NOT_OK;
  }

  DBG_EEP_COPYJOBVALUES_EXIT(RetVal, EepromAddress, DataBufferPtr, Length, Job);

  return RetVal;
}

STATIC FUNC(void, EEP_CODE) Eep_JobWriteProcess(void)
{
  DBG_EEP_JOBWRITEPROCESS_ENTRY();

  if((Eep_JobLength/Eep_WriteBlockSize)>0)
  {
    /* there are at least Eep_WriteBlockSize bytes to write */
    if(fwrite(Eep_JobDataBufferPtr, 1, Eep_WriteBlockSize, Eep_FilePtr) == Eep_WriteBlockSize)
    {
      fflush(Eep_FilePtr);
      /* file access OK */
      /* move to next data and decrease rest of the length */
      /* Deviation MISRA-2 */
      Eep_JobDataBufferPtr += Eep_WriteBlockSize;
      Eep_JobLength -= Eep_WriteBlockSize;

      /* are there any more bytes to write? */
      if(((Eep_JobLength/Eep_WriteBlockSize)==0) && ((Eep_JobLength%Eep_WriteBlockSize)==0))
      {
        /* no, finalize the job */
        Eep_Status = MEMIF_IDLE;
        Eep_JobResult = MEMIF_JOB_OK;
        Eep_ProcessJobEndNotification();
      }
    }
    else
    {
      /* file failure, finalize the job */
      Eep_Status = MEMIF_IDLE;
      Eep_JobResult = MEMIF_JOB_FAILED;
      Dem_ReportErrorStatus(DemConf_DemEventParameter_EEP_E_WRITE_FAILED, DEM_EVENT_STATUS_FAILED);
      Eep_ProcessJobErrorNotification();
    }
  }
  else
  {
    /* there are less than Eep_WriteBlockSize bytes to write */
    if(fwrite(Eep_JobDataBufferPtr, 1, Eep_JobLength%Eep_WriteBlockSize, Eep_FilePtr) == (Eep_JobLength%Eep_WriteBlockSize))
    {
      fflush(Eep_FilePtr);
      /* file access OK */
      /* finalize the job, as this was last write operation */
      Eep_Status = MEMIF_IDLE;
      Eep_JobResult = MEMIF_JOB_OK;
      Eep_ProcessJobEndNotification();
    }
    else
    {
      /* file failure, finalize the job */
      Eep_Status = MEMIF_IDLE;
      Eep_JobResult = MEMIF_JOB_FAILED;
      Dem_ReportErrorStatus(DemConf_DemEventParameter_EEP_E_WRITE_FAILED, DEM_EVENT_STATUS_FAILED);
      Eep_ProcessJobErrorNotification();
    }
  }

  DBG_EEP_JOBWRITEPROCESS_EXIT();
}

STATIC FUNC(void, EEP_CODE) Eep_JobReadProcess(void)
{
  DBG_EEP_JOBREADPROCESS_ENTRY();

  if((Eep_JobLength/Eep_ReadBlockSize)>0)
  {
    /* there are at least Eep_ReadBlockSize bytes to read */
    if(fread(Eep_JobDataBufferPtr, 1, Eep_ReadBlockSize, Eep_FilePtr) == Eep_ReadBlockSize)
    {
      /* file access OK */
      /* move to next data and decrease rest of the length */
      /* Deviation MISRA-2 */
      Eep_JobDataBufferPtr += Eep_ReadBlockSize;
      Eep_JobLength -= Eep_ReadBlockSize;

      /* are there any more bytes to read? */
      if(((Eep_JobLength/Eep_ReadBlockSize)==0) && ((Eep_JobLength%Eep_ReadBlockSize)==0))
      {
        /* no, finalize the job */
        Eep_Status = MEMIF_IDLE;
        Eep_JobResult = MEMIF_JOB_OK;
        Eep_ProcessJobEndNotification();
      }
    }
    else
    {
      /* file failure, finalize the job */
      Eep_Status = MEMIF_IDLE;
      Eep_JobResult = MEMIF_JOB_FAILED;
      Dem_ReportErrorStatus(DemConf_DemEventParameter_EEP_E_READ_FAILED, DEM_EVENT_STATUS_FAILED);
      Eep_ProcessJobErrorNotification();
    }
  }
  else
  {
    /* there are less than Eep_ReadBlockSize bytes to read */
    if(fread(Eep_JobDataBufferPtr, 1, Eep_JobLength%Eep_ReadBlockSize, Eep_FilePtr) == (Eep_JobLength%Eep_ReadBlockSize))
    {
      /* file access OK */
      /* finalize the job, as this was last read operation */
      Eep_Status = MEMIF_IDLE;
      Eep_JobResult = MEMIF_JOB_OK;
      Eep_ProcessJobEndNotification();
    }
    else
    {
      /* file failure, finalize the job */
      Eep_Status = MEMIF_IDLE;
      Eep_JobResult = MEMIF_JOB_FAILED;
      Dem_ReportErrorStatus(DemConf_DemEventParameter_EEP_E_READ_FAILED, DEM_EVENT_STATUS_FAILED);
      Eep_ProcessJobErrorNotification();
    }
  }

  DBG_EEP_JOBREADPROCESS_EXIT();
}

STATIC FUNC(void, EEP_CODE) Eep_JobEraseProcess(void)
{
  DBG_EEP_JOBERASEPROCESS_ENTRY();

  if((Eep_JobLength/EEP_ERASE_BYTES_ONECYCLE)>0)
  {
    /* there are at least EEP_ERASE_BYTES_ONECYCLE bytes to erase */
    if(fwrite(Eep_BlankArray, 1, EEP_ERASE_BYTES_ONECYCLE, Eep_FilePtr) == EEP_ERASE_BYTES_ONECYCLE)
    {
      fflush(Eep_FilePtr);
      /* file access OK */
      /* decrease rest of the length */
      Eep_JobLength -= EEP_ERASE_BYTES_ONECYCLE;

      /* are there any more bytes to erase? */
      if(((Eep_JobLength/EEP_ERASE_BYTES_ONECYCLE)==0) && ((Eep_JobLength%EEP_ERASE_BYTES_ONECYCLE)==0))
      {
        /* no, finalize the job */
        Eep_Status = MEMIF_IDLE;
        Eep_JobResult = MEMIF_JOB_OK;
        Eep_ProcessJobEndNotification();
      }
    }
    else
    {
      /* file failure, finalize the job */
      Eep_Status = MEMIF_IDLE;
      Eep_JobResult = MEMIF_JOB_FAILED;
      Dem_ReportErrorStatus(DemConf_DemEventParameter_EEP_E_ERASE_FAILED, DEM_EVENT_STATUS_FAILED);
      Eep_ProcessJobErrorNotification();
    }
  }
  else
  {
    /* there are less than EEP_ERASE_BYTES_ONECYCLE bytes to erase */
    if(fwrite(Eep_BlankArray, 1, Eep_JobLength%EEP_ERASE_BYTES_ONECYCLE, Eep_FilePtr) == (Eep_JobLength%EEP_ERASE_BYTES_ONECYCLE))
    {
      fflush(Eep_FilePtr);
      /* file access OK */
      /* finalize the job, as this was last erase operation */
      Eep_Status = MEMIF_IDLE;
      Eep_JobResult = MEMIF_JOB_OK;
      Eep_ProcessJobEndNotification();
    }
    else
    {
      /* file failure, finalize the job */
      Eep_Status = MEMIF_IDLE;
      Eep_JobResult = MEMIF_JOB_FAILED;
      Dem_ReportErrorStatus(DemConf_DemEventParameter_EEP_E_ERASE_FAILED, DEM_EVENT_STATUS_FAILED);
      Eep_ProcessJobErrorNotification();
    }
  }

  DBG_EEP_JOBERASEPROCESS_EXIT();
}

STATIC FUNC(void, EEP_CODE) Eep_JobCompareProcess(void)
{

  DBG_EEP_JOBCOMPAREPROCESS_ENTRY();

  if((Eep_JobLength/Eep_ReadBlockSize)>0)
  {
    /* there are at least Eep_ReadBlockSize bytes to compare */
    if(fread(Eep_DataBuffer, 1, Eep_ReadBlockSize, Eep_FilePtr) == Eep_ReadBlockSize)
    {
      /* file access OK */
      /* now compare data */
      if(memcmp(Eep_DataBuffer, Eep_JobDataBufferPtr, Eep_ReadBlockSize)==0)
      {
        /* compare OK */
        /* move to next data and decrease rest of the length */
        /* Deviation MISRA-2 */
        Eep_JobDataBufferPtr += Eep_ReadBlockSize;
        Eep_JobLength -= Eep_ReadBlockSize;

        /* are there any more bytes to compare? */
        if(((Eep_JobLength/Eep_ReadBlockSize)==0) && ((Eep_JobLength%Eep_ReadBlockSize)==0))
        {
          /* no, finalize the job */
          Eep_Status = MEMIF_IDLE;
          Eep_JobResult = MEMIF_JOB_OK;
          Eep_ProcessJobEndNotification();
        }
      }
      else
      {
        /* compare failed, finalize the job */
        Eep_Status = MEMIF_IDLE;
        Eep_JobResult = MEMIF_BLOCK_INCONSISTENT;
        Eep_ProcessJobErrorNotification();
      }
    }
    else
    {
      /* file failure, finalize the job */
      Eep_Status = MEMIF_IDLE;
      Eep_JobResult = MEMIF_JOB_FAILED;
      Dem_ReportErrorStatus(DemConf_DemEventParameter_EEP_E_COMPARE_FAILED, DEM_EVENT_STATUS_FAILED);
      Eep_ProcessJobErrorNotification();
    }
  }
  else
  {
    /* there are less than Eep_ReadBlockSize bytes to compare */
    if(fread(Eep_DataBuffer, 1, Eep_JobLength%Eep_ReadBlockSize, Eep_FilePtr) == (Eep_JobLength%Eep_ReadBlockSize))
    {
      /* file access OK */
      /* now compare data */
      if(memcmp(Eep_DataBuffer, Eep_JobDataBufferPtr, Eep_JobLength%Eep_ReadBlockSize)==0)
      {
        /* compare OK */
        /* finalize the job, as this was last compare operation */
        Eep_Status = MEMIF_IDLE;
        Eep_JobResult = MEMIF_JOB_OK;
        Eep_ProcessJobEndNotification();
      }
      else
      {
        /* compare failed, finalize the job */
        Eep_Status = MEMIF_IDLE;
        Eep_JobResult = MEMIF_BLOCK_INCONSISTENT;
        Eep_ProcessJobErrorNotification();
      }
    }
    else
    {
      /* file failure, finalize the job */
      Eep_Status = MEMIF_IDLE;
      Eep_JobResult = MEMIF_JOB_FAILED;
      Dem_ReportErrorStatus(DemConf_DemEventParameter_EEP_E_COMPARE_FAILED, DEM_EVENT_STATUS_FAILED);
      Eep_ProcessJobErrorNotification();
    }
  }

  DBG_EEP_JOBCOMPAREPROCESS_EXIT();
}
/*==================[external function definitions]=========================*/

FUNC(void, EEP_CODE) Eep_Init(
  P2CONST(Eep_ConfigType, AUTOMATIC, EEP_APPL_CONST) ConfigPtr)
{
  VAR(uint32, EEP_VAR) i=0;
  VAR(Std_ReturnType, EEP_VAR) RetVal=E_NOT_OK;

  DBG_EEP_INIT_ENTRY(ConfigPtr);

#if(EEP_DEV_ERROR_DETECT == STD_ON)
  if(ConfigPtr==NULL_PTR)
  {
    Det_ReportError(EEP_MODULE_ID, EEP_INSTANCE_ID, EEP_SID_INIT, EEP_E_PARAM_CONFIG);
  }
  else
#endif/* EEP_DEV_ERROR_DETECT == STD_ON */
  {
    /* store pointer to current provided module configuration set */
    Eep_ConfigPtr = ConfigPtr;

    /* try to re-open file for both read/write operations (and check if exists at all?) */
    /* Deviation MISRA-3 */
    Eep_FilePtr=fopen(EEP_FILE_NAME, "rb+");
    if(Eep_FilePtr==NULL_PTR)
    {
      /* re-open file failed, so file does not exist yet, hence try to create it */
      /* Deviation MISRA-3 */
      Eep_FilePtr=fopen(EEP_FILE_NAME, "wb");
      if(Eep_FilePtr!=NULL_PTR)
      {
        /* file was created OK, we can continue */
        /* make it's size according to eepromSize and write the blank-pattern to it */
        for(i=0; i<Eep_ConfigPtr->eepromSize; i++)
        {
          fwrite(Eep_BlankArray, 1, 1, Eep_FilePtr);
          fflush(Eep_FilePtr);
        }

        /* try to close recently created file */
        if(fclose(Eep_FilePtr)!=0)
        {
          RetVal = E_NOT_OK;
        }
        else
        {
          /* try to re-open file for both read/write operations */
          /* Deviation MISRA-3 */
          Eep_FilePtr=fopen(EEP_FILE_NAME, "rb+");
          if(Eep_FilePtr==NULL_PTR)
          {
            RetVal = E_NOT_OK;
          }
          else
          {
            RetVal = E_OK;
          }
        }
      }
      else
      {
        /* file was not created OK */
        RetVal = E_NOT_OK;
      }
    }
    else
    {
      /* file already exist and was re-opened OK */
      RetVal = E_OK;
    }

    /* setup initial Eep default mode to configured value */
    Eep_Mode = Eep_ConfigPtr->defaultMode;

    if(Eep_Mode == MEMIF_MODE_FAST)
    {
      Eep_ReadBlockSize = Eep_ConfigPtr->eepFastReadBlockSize;
      Eep_WriteBlockSize = Eep_ConfigPtr->eepFastWriteBlockSize;
    }
    else
    {
      Eep_ReadBlockSize = Eep_ConfigPtr->eepNormalReadBlockSize;
      Eep_WriteBlockSize = Eep_ConfigPtr->eepNormalWriteBlockSize;
    }

    if(RetVal==E_OK)
    {
      /* there seems to be everything fine, so we can continue */
      Eep_Status = MEMIF_IDLE;
      Eep_JobResult = MEMIF_JOB_OK;
    }
    else
    {
      /* there are some problems with file, and as there is no way how to report failure in this
         phase, hence at least Eep status setup to MEMIF_BUSY to indicate Eep can not accept any
         job further on */
      Eep_Status = MEMIF_BUSY;
      Eep_JobResult = MEMIF_JOB_OK;
    }

  }

  DBG_EEP_INIT_EXIT(ConfigPtr);
}

FUNC(void, EEP_CODE) Eep_SetMode(
    MemIf_ModeType Mode)
{
  DBG_EEP_SETMODE_ENTRY(Mode);

#if(EEP_DEV_ERROR_DETECT == STD_ON)
  if(Eep_Status==MEMIF_BUSY)
  {
    Det_ReportError(EEP_MODULE_ID, EEP_INSTANCE_ID, EEP_SID_SET_MODE, EEP_E_BUSY);
  }
  else if(Eep_Status==MEMIF_UNINIT)
  {
    Det_ReportError(EEP_MODULE_ID, EEP_INSTANCE_ID, EEP_SID_SET_MODE, EEP_E_UNINIT);
  }
  else
#endif/* EEP_DEV_ERROR_DETECT == STD_ON */
  {
    Eep_Mode = Mode;
    if(Eep_Mode == MEMIF_MODE_FAST)
    {
      Eep_ReadBlockSize = Eep_ConfigPtr->eepFastReadBlockSize;
      Eep_WriteBlockSize = Eep_ConfigPtr->eepFastWriteBlockSize;
    }
    else
    {
      Eep_ReadBlockSize = Eep_ConfigPtr->eepNormalReadBlockSize;
      Eep_WriteBlockSize = Eep_ConfigPtr->eepNormalWriteBlockSize;
    }
  }

  DBG_EEP_SETMODE_EXIT(Mode);
}

FUNC(Std_ReturnType, EEP_CODE) Eep_Read(
    Eep_AddressType EepromAddress,
    P2VAR(uint8, AUTOMATIC, EEP_APPL_CONST) DataBufferPtr,
    Eep_LengthType Length)
{
  VAR(Std_ReturnType, EEP_VAR) RetVal;

  DBG_EEP_READ_ENTRY(EepromAddress, DataBufferPtr, Length);

#if(EEP_DEV_ERROR_DETECT == STD_ON)
  if(Eep_Status==MEMIF_BUSY)
  {
    Det_ReportError(EEP_MODULE_ID, EEP_INSTANCE_ID, EEP_SID_READ, EEP_E_BUSY);
    RetVal = E_NOT_OK;
  }
  else if(Eep_Status==MEMIF_UNINIT)
  {
    Det_ReportError(EEP_MODULE_ID, EEP_INSTANCE_ID, EEP_SID_READ, EEP_E_UNINIT);
    RetVal = E_NOT_OK;
  }
  else if(DataBufferPtr==NULL_PTR)
  {
    Det_ReportError(EEP_MODULE_ID, EEP_INSTANCE_ID, EEP_SID_READ, EEP_E_PARAM_DATA);
    RetVal = E_NOT_OK;
  }
  else if(EepromAddress>Eep_ConfigPtr->eepromSize)
  {
    Det_ReportError(EEP_MODULE_ID, EEP_INSTANCE_ID, EEP_SID_READ, EEP_E_PARAM_ADDRESS);
    RetVal = E_NOT_OK;
  }
  else if((Length<1) || (Length>(Eep_ConfigPtr->eepromSize-EepromAddress)))
  {
    Det_ReportError(EEP_MODULE_ID, EEP_INSTANCE_ID, EEP_SID_READ, EEP_E_PARAM_LENGTH);
    RetVal = E_NOT_OK;
  }
  else
#endif/* EEP_DEV_ERROR_DETECT == STD_ON */
  {
    RetVal = Eep_CopyJobValues(EepromAddress, DataBufferPtr, Length, EEP_JOB_READ);
  }

  DBG_EEP_READ_EXIT(RetVal, EepromAddress, DataBufferPtr, Length);
  return RetVal;
}

FUNC(Std_ReturnType, EEP_CODE) Eep_Write(
    Eep_AddressType EepromAddress,
    P2CONST(uint8, AUTOMATIC, EEP_APPL_CONST) DataBufferPtr,
    Eep_LengthType Length)
{
  VAR(Std_ReturnType, EEP_VAR) RetVal;

  DBG_EEP_WRITE_ENTRY(EepromAddress, DataBufferPtr, Length);

#if(EEP_DEV_ERROR_DETECT == STD_ON)
  if(Eep_Status==MEMIF_BUSY)
  {
    Det_ReportError(EEP_MODULE_ID, EEP_INSTANCE_ID, EEP_SID_WRITE, EEP_E_BUSY);
    RetVal = E_NOT_OK;
  }
  else if(Eep_Status==MEMIF_UNINIT)
  {
    Det_ReportError(EEP_MODULE_ID, EEP_INSTANCE_ID, EEP_SID_WRITE, EEP_E_UNINIT);
    RetVal = E_NOT_OK;
  }
  else if(DataBufferPtr==NULL_PTR)
  {
    Det_ReportError(EEP_MODULE_ID, EEP_INSTANCE_ID, EEP_SID_WRITE, EEP_E_PARAM_DATA);
    RetVal = E_NOT_OK;
  }
  else if(EepromAddress>Eep_ConfigPtr->eepromSize)
  {
    Det_ReportError(EEP_MODULE_ID, EEP_INSTANCE_ID, EEP_SID_WRITE, EEP_E_PARAM_ADDRESS);
    RetVal = E_NOT_OK;
  }
  else if((Length<1) || (Length>(Eep_ConfigPtr->eepromSize-EepromAddress)))
  {
    Det_ReportError(EEP_MODULE_ID, EEP_INSTANCE_ID, EEP_SID_WRITE, EEP_E_PARAM_LENGTH);
    RetVal = E_NOT_OK;
  }
  else
#endif/* EEP_DEV_ERROR_DETECT == STD_ON */
  {
    /* Deviation MISRA-4 */
    /* Warning COMPILER-1 */
    RetVal = Eep_CopyJobValues(EepromAddress, DataBufferPtr, Length, EEP_JOB_WRITE);
  }

  DBG_EEP_WRITE_EXIT(RetVal, EepromAddress, DataBufferPtr, Length);
  return RetVal;
}

FUNC(Std_ReturnType, EEP_CODE) Eep_Erase(
    Eep_AddressType EepromAddress,
    Eep_LengthType Length)
{
  VAR(Std_ReturnType, EEP_VAR) RetVal;
  P2VAR(uint8, AUTOMATIC, EEP_APPL_CONST) DataBufferPtr = NULL_PTR; /* dummy variable for
  Eep_CopyJobValues(), as Eep_Erase() does not need this parameter */

  DBG_EEP_ERASE_ENTRY(EepromAddress, Length);

#if(EEP_DEV_ERROR_DETECT == STD_ON)
  if(Eep_Status==MEMIF_BUSY)
  {
    Det_ReportError(EEP_MODULE_ID, EEP_INSTANCE_ID, EEP_SID_ERASE, EEP_E_BUSY);
    RetVal = E_NOT_OK;
  }
  else if(Eep_Status==MEMIF_UNINIT)
  {
    Det_ReportError(EEP_MODULE_ID, EEP_INSTANCE_ID, EEP_SID_ERASE, EEP_E_UNINIT);
    RetVal = E_NOT_OK;
  }
  else if(EepromAddress>Eep_ConfigPtr->eepromSize)
  {
    Det_ReportError(EEP_MODULE_ID, EEP_INSTANCE_ID, EEP_SID_ERASE, EEP_E_PARAM_ADDRESS);
    RetVal = E_NOT_OK;
  }
  else if((Length<1) || (Length>(Eep_ConfigPtr->eepromSize-EepromAddress)))
  {
    Det_ReportError(EEP_MODULE_ID, EEP_INSTANCE_ID, EEP_SID_ERASE, EEP_E_PARAM_LENGTH);
    RetVal = E_NOT_OK;
  }
  else
#endif/* EEP_DEV_ERROR_DETECT == STD_ON */
  {
    RetVal = Eep_CopyJobValues(EepromAddress, DataBufferPtr, Length, EEP_JOB_ERASE);
  }

  DBG_EEP_ERASE_EXIT(RetVal, EepromAddress, Length);
  return RetVal;
}

FUNC(Std_ReturnType, EEP_CODE) Eep_Compare(
    Eep_AddressType EepromAddress,
    P2CONST(uint8, AUTOMATIC, EEP_APPL_CONST) DataBufferPtr,
    Eep_LengthType Length)
{
  VAR(Std_ReturnType, EEP_VAR) RetVal;

  DBG_EEP_COMPARE_ENTRY(EepromAddress, DataBufferPtr, Length);

#if(EEP_DEV_ERROR_DETECT == STD_ON)
  if(Eep_Status==MEMIF_BUSY)
  {
    Det_ReportError(EEP_MODULE_ID, EEP_INSTANCE_ID, EEP_SID_COMPARE, EEP_E_BUSY);
    RetVal = E_NOT_OK;
  }
  else if(Eep_Status==MEMIF_UNINIT)
  {
    Det_ReportError(EEP_MODULE_ID, EEP_INSTANCE_ID, EEP_SID_COMPARE, EEP_E_UNINIT);
    RetVal = E_NOT_OK;
  }
  else if(DataBufferPtr==NULL_PTR)
  {
    Det_ReportError(EEP_MODULE_ID, EEP_INSTANCE_ID, EEP_SID_COMPARE, EEP_E_PARAM_DATA);
    RetVal = E_NOT_OK;
  }
  else if(EepromAddress>Eep_ConfigPtr->eepromSize)
  {
    Det_ReportError(EEP_MODULE_ID, EEP_INSTANCE_ID, EEP_SID_COMPARE, EEP_E_PARAM_ADDRESS);
    RetVal = E_NOT_OK;
  }
  else if((Length<1) || (Length>(Eep_ConfigPtr->eepromSize-EepromAddress)))
  {
    Det_ReportError(EEP_MODULE_ID, EEP_INSTANCE_ID, EEP_SID_COMPARE, EEP_E_PARAM_LENGTH);
    RetVal = E_NOT_OK;
  }
  else
#endif/* EEP_DEV_ERROR_DETECT == STD_ON */
  {
    /* Deviation MISRA-4 */
    /* Warning COMPILER-1 */
    RetVal = Eep_CopyJobValues(EepromAddress, DataBufferPtr, Length, EEP_JOB_COMPARE);
  }

  DBG_EEP_COMPARE_EXIT(RetVal, EepromAddress, DataBufferPtr, Length);
  return RetVal;
}

FUNC(void, EEP_CODE) Eep_Cancel(
    void)
{
  VAR(Eep_Status, EEP_VAR) = MEMIF_IDLE;

  DBG_EEP_CANCEL_ENTRY();

  if(Eep_JobResult==MEMIF_JOB_PENDING)
  {
    Eep_JobResult=MEMIF_JOB_CANCELED;
  }
  else
  {
    /* otherwise leave Eep_JobResult unchanged */
  }

  Eep_ProcessJobErrorNotification();

  DBG_EEP_CANCEL_EXIT();
}

FUNC(MemIf_StatusType, EEP_CODE) Eep_GetStatus(
    void)
{
  DBG_EEP_GETSTATUS_ENTRY();
  DBG_EEP_GETSTATUS_EXIT(Eep_Status);
  return Eep_Status;
}

FUNC(MemIf_JobResultType, EEP_CODE) Eep_GetJobResult(
    void)
{
  DBG_EEP_GETJOBRESULT_ENTRY();
  DBG_EEP_GETJOBRESULT_EXIT(Eep_JobResult);
  return Eep_JobResult;
}

#if(EEP_VERSION_INFO_API==STD_ON)
FUNC(void, EEP_CODE) Eep_GetVersionInfo(
   CONSTP2VAR(Std_VersionInfoType, AUTOMATIC, EEP_APPL_DATA) VersionInfoPtr)
{
  DBG_EEP_GETVERSIONINFO_ENTRY(VersionInfoPtr);

#if(EEP_DEV_ERROR_DETECT == STD_ON)
    if(VersionInfoPtr==NULL_PTR)
    {
      Det_ReportError(EEP_MODULE_ID, EEP_INSTANCE_ID, EEP_SID_GET_VERSION_INFO, EEP_E_PARAM_DATA);
    }
    else
#endif/* EEP_DEV_ERROR_DETECT == STD_ON */
    {
      VersionInfoPtr->vendorID = EEP_VENDOR_ID;
      VersionInfoPtr->moduleID = EEP_MODULE_ID;
      VersionInfoPtr->sw_major_version = EEP_SW_MAJOR_VERSION;
      VersionInfoPtr->sw_minor_version = EEP_SW_MINOR_VERSION;
      VersionInfoPtr->sw_patch_version = EEP_SW_PATCH_VERSION;
    }

  DBG_EEP_GETVERSIONINFO_EXIT(VersionInfoPtr);
}
#endif/* EEP_VERSION_INFO_API==STD_ON */

FUNC(void, EEP_CODE) Eep_MainFunction(
   void)
{
  DBG_EEP_MAINFUNCTION_ENTRY();

  if(Eep_JobResult==MEMIF_JOB_PENDING)
  {
    /* there is some job pending, process it */
    switch(Eep_Job)
    {
      case EEP_JOB_WRITE:
        /* there is unfinished write job */
        Eep_JobWriteProcess();
      break;
      case EEP_JOB_READ:
        /* there is unfinished read job */
        Eep_JobReadProcess();
      break;
      case EEP_JOB_ERASE:
        /* there is unfinished erase job */
        Eep_JobEraseProcess();
      break;
      case EEP_JOB_COMPARE:
        /* there is unfinished compare job */
        Eep_JobCompareProcess();
      break;
      default:
      break;
    }
  }
  else
  {
    /* there is no job pending, so nothing to do here, just return */
  }

  DBG_EEP_MAINFUNCTION_EXIT();
}

#define EEP_STOP_SEC_CODE
#include <Eep_MemMap.h>
/*==================[end of file]===========================================*/
