/**
 * \file
 *
 * \brief AUTOSAR MemIf
 *
 * This file contains the implementation of the AUTOSAR
 * module MemIf.
 *
 * \version 5.11.11
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
/*  MISRA-C:2012 Deviation List
 *
 *  MISRAC2012-1) Deviated Rule: 8.9 (advisory)
 *    "An object should be defined at block scope if its identifier only appears in a single function."
 *
 *    Reason: Even thou is only used once the object needs a global scope.
 */
/*==================[inclusions]============================================*/

/* !LINKSTO MemIf037,1 */
#include <MemIf_Trace.h>
#include <Std_Types.h>         /* AUTOSAR standard types */
#include <TSAutosar.h>         /* EB specific standard types */

#include <MemIf.h>             /* our own API */

#if (MEMIF_DEV_ERROR_DETECT == STD_ON)
# include <Det.h>              /* declaration of Det_ReportError() */
#endif

#if ((MEMIF_MAXIMUM_NUMBER_OF_DEVICES == 1U)                                    \
   && (MEMIF_DEV_ERROR_DETECT == STD_OFF)                               \
   && (MEMIF_VERSION_INFO_API == STD_OFF))
# include <TSCompiler.h>
/* For the given configuration this translation unit is empty.  The following
 * macro prevents compiler errors or warnings in this case */
TS_PREVENTEMPTYTRANSLATIONUNIT
#endif

/*==================[macros]================================================*/

/*------------------[AUTOSAR vendor identification check]-------------------*/

#ifndef MEMIF_VENDOR_ID /* configuration check */
#error MEMIF_VENDOR_ID must be defined
#endif

#if (MEMIF_VENDOR_ID != 1U) /* vendor check */
#error MEMIF_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#ifndef MEMIF_AR_RELEASE_MAJOR_VERSION /* configuration check */
#error MEMIF_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (MEMIF_AR_RELEASE_MAJOR_VERSION != 4U)
#error MEMIF_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef MEMIF_AR_RELEASE_MINOR_VERSION /* configuration check */
#error MEMIF_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (MEMIF_AR_RELEASE_MINOR_VERSION != 0U )
#error MEMIF_AR_RELEASE_MINOR_VERSION wrong (!= 0U)
#endif

#ifndef MEMIF_AR_RELEASE_REVISION_VERSION /* configuration check */
#error MEMIF_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (MEMIF_AR_RELEASE_REVISION_VERSION != 3U )
#error MEMIF_AR_RELEASE_REVISION_VERSION wrong (!= 3U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#ifndef MEMIF_SW_MAJOR_VERSION /* configuration check */
#error MEMIF_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (MEMIF_SW_MAJOR_VERSION != 5U)
#error MEMIF_SW_MAJOR_VERSION wrong (!= 5U)
#endif

#ifndef MEMIF_SW_MINOR_VERSION /* configuration check */
#error MEMIF_SW_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (MEMIF_SW_MINOR_VERSION < 11U)
#error MEMIF_SW_MINOR_VERSION wrong (< 11U)
#endif

#ifndef MEMIF_SW_PATCH_VERSION /* configuration check */
#error MEMIF_SW_PATCH_VERSION must be defined
#endif

/* patch version check */
#if (MEMIF_SW_PATCH_VERSION < 11U)
#error MEMIF_SW_PATCH_VERSION wrong (< 11U)
#endif

/*------------------[DET calls]---------------------------------------------*/

#if (defined MEMIF_DET_REPORT_ERROR)
#error MEMIF_DET_REPORT_ERROR is already defined
#endif /* if (defined MEMIF_DET_REPORT_ERROR) */
#define MEMIF_DET_REPORT_ERROR(ApiId,ErrId)                             \
  ((void)Det_ReportError(MEMIF_MODULE_ID, MEMIF_INSTANCE_ID, (ApiId), (ErrId)))

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

#if (MEMIF_MAXIMUM_NUMBER_OF_DEVICES > 1U)

#define MEMIF_START_SEC_CONST_UNSPECIFIED
#include <MemIf_MemMap.h>

/* !LINKSTO MemIf020,1 */
/* function pointer array definitions */

CONST(MemIf_ReadFctPtrType, MEMIF_CONST)
  MemIf_ReadFctPtr[MEMIF_MAXIMUM_NUMBER_OF_DEVICES]      = MEMIF_READ_FCTPTR;

CONST(MemIf_WriteFctPtrType, MEMIF_CONST)
  MemIf_WriteFctPtr[MEMIF_MAXIMUM_NUMBER_OF_DEVICES]     = MEMIF_WRITE_FCTPTR;

CONST(MemIf_CancelFctPtrType, MEMIF_CONST)
  MemIf_CancelFctPtr[MEMIF_MAXIMUM_NUMBER_OF_DEVICES]    = MEMIF_CANCEL_FCTPTR;

CONST(MemIf_GetJobResultFctPtrType, MEMIF_CONST)
  MemIf_GetJobResultFctPtr[MEMIF_MAXIMUM_NUMBER_OF_DEVICES]
  = MEMIF_GETJOBRESULT_FCTPTR;

CONST(MemIf_InvalidateBlockFctPtrType, MEMIF_CONST)
  MemIf_InvalidateBlockFctPtr[MEMIF_MAXIMUM_NUMBER_OF_DEVICES]
  = MEMIF_INVALIDATEBLOCK_FCTPTR;

CONST(MemIf_EraseImmedBlockFctPtrType, MEMIF_CONST)
  MemIf_EraseImmediateBlockFctPtr[MEMIF_MAXIMUM_NUMBER_OF_DEVICES]
  = MEMIF_ERASEIMMEDIATEBLOCK_FCTPTR;

#define MEMIF_STOP_SEC_CONST_UNSPECIFIED
#include <MemIf_MemMap.h>

#endif

/*==================[internal constants]====================================*/

#if (MEMIF_MAXIMUM_NUMBER_OF_DEVICES > 1U)

#define MEMIF_START_SEC_CONST_UNSPECIFIED
#include <MemIf_MemMap.h>

/* function pointer array definitions for function pointer which are only
 * accessed by the functions in this translation unit */
/* Deviation MISRAC2012-1 */
STATIC CONST(MemIf_SetModeFctPtrType, MEMIF_CONST)
  MemIf_SetModeFctPtr[MEMIF_MAXIMUM_NUMBER_OF_DEVICES]   = MEMIF_SETMODE_FCTPTR;
/* Deviation MISRAC2012-1 */
STATIC CONST(MemIf_GetStatusFctPtrType, MEMIF_CONST)
  MemIf_GetStatusFctPtr[MEMIF_MAXIMUM_NUMBER_OF_DEVICES] = MEMIF_GETSTATUS_FCTPTR;

#define MEMIF_STOP_SEC_CONST_UNSPECIFIED
#include <MemIf_MemMap.h>

#endif

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/* AUTOSAR Memory Mapping - start section for code */
#define MEMIF_START_SEC_CODE
#include <MemIf_MemMap.h>

#if (MEMIF_DEV_ERROR_DETECT == STD_ON) && (MEMIF_MAXIMUM_NUMBER_OF_DEVICES > 1U)

FUNC(void, MEMIF_CODE) MemIf_Cancel(uint8 DeviceIndex)
{

  DBG_MEMIF_CANCEL_ENTRY(DeviceIndex);
  if (DeviceIndex >= MEMIF_MAXIMUM_NUMBER_OF_DEVICES)
  {
    MEMIF_DET_REPORT_ERROR(MEMIF_CANCEL_ID, MEMIF_E_PARAM_DEVICE);
  }
  else
  {
    if(NULL_PTR == MemIf_SetModeFctPtr[DeviceIndex])
    {
      MEMIF_DET_REPORT_ERROR(MEMIF_CANCEL_ID, MEMIF_E_PARAM_DEVICE);
    }
    else
    {
      MemIf_CancelFctPtr[DeviceIndex]();
    }
  }

  DBG_MEMIF_CANCEL_EXIT(DeviceIndex);
}

FUNC(Std_ReturnType, MEMIF_CODE) MemIf_EraseImmediateBlock(
  uint8  DeviceIndex,
  uint16 BlockNumber)
{
  Std_ReturnType ret;

  DBG_MEMIF_ERASEIMMEDIATEBLOCK_ENTRY(DeviceIndex,BlockNumber);
  if (DeviceIndex >= MEMIF_MAXIMUM_NUMBER_OF_DEVICES)
  {
    MEMIF_DET_REPORT_ERROR(MEMIF_ERASEIMMEDIATEBLOCK_ID, MEMIF_E_PARAM_DEVICE);
    ret = E_NOT_OK;
  }
  else
  {
    if(NULL_PTR == MemIf_SetModeFctPtr[DeviceIndex])
    {
      MEMIF_DET_REPORT_ERROR(MEMIF_ERASEIMMEDIATEBLOCK_ID, MEMIF_E_PARAM_DEVICE);
      ret = E_NOT_OK;
    }
    else
    {
      ret = MemIf_EraseImmediateBlockFctPtr[DeviceIndex](BlockNumber);
    }
  }
  DBG_MEMIF_ERASEIMMEDIATEBLOCK_EXIT(ret,DeviceIndex,BlockNumber);
  return ret;
}

FUNC(MemIf_JobResultType, MEMIF_CODE) MemIf_GetJobResult(uint8 DeviceIndex)
{
  MemIf_JobResultType ret;

  DBG_MEMIF_GETJOBRESULT_ENTRY(DeviceIndex);
  if (DeviceIndex >= MEMIF_MAXIMUM_NUMBER_OF_DEVICES)
  {
    MEMIF_DET_REPORT_ERROR(MEMIF_GETJOBRESULT_ID, MEMIF_E_PARAM_DEVICE);
    ret = MEMIF_JOB_FAILED;
  }
  else
  {
    if(NULL_PTR == MemIf_SetModeFctPtr[DeviceIndex])
    {
      MEMIF_DET_REPORT_ERROR(MEMIF_GETJOBRESULT_ID, MEMIF_E_PARAM_DEVICE);
      ret = MEMIF_JOB_FAILED;
    }
    else
    {
      ret = MemIf_GetJobResultFctPtr[DeviceIndex]();
    }
  }

  DBG_MEMIF_GETJOBRESULT_EXIT(ret,DeviceIndex);
  return ret;
}

FUNC(Std_ReturnType, MEMIF_CODE) MemIf_InvalidateBlock(
  uint8  DeviceIndex,
  uint16 BlockNumber)
{
  Std_ReturnType ret;

  DBG_MEMIF_INVALIDATEBLOCK_ENTRY(DeviceIndex,BlockNumber);
  if (DeviceIndex >= MEMIF_MAXIMUM_NUMBER_OF_DEVICES)
  {
    MEMIF_DET_REPORT_ERROR(MEMIF_INVALIDATEBLOCK_ID, MEMIF_E_PARAM_DEVICE);
    ret = E_NOT_OK;
  }
  else
  {
    if(NULL_PTR == MemIf_SetModeFctPtr[DeviceIndex])
    {
      MEMIF_DET_REPORT_ERROR(MEMIF_INVALIDATEBLOCK_ID, MEMIF_E_PARAM_DEVICE);
      ret = E_NOT_OK;
    }
    else
    {
      ret = MemIf_InvalidateBlockFctPtr[DeviceIndex](BlockNumber);
    }
  }

  DBG_MEMIF_INVALIDATEBLOCK_EXIT(ret,DeviceIndex,BlockNumber);
  return ret;
}

FUNC(Std_ReturnType, MEMIF_CODE) MemIf_Read(
  uint8                                    DeviceIndex,
  uint16                                   BlockNumber,
  uint16                                   BlockOffset,
  P2VAR(uint8, AUTOMATIC, MEMIF_APPL_DATA) DataBufferPtr,
  uint16                                   Length)
{
  Std_ReturnType ret;

  DBG_MEMIF_READ_ENTRY(DeviceIndex,BlockNumber,BlockOffset,DataBufferPtr,Length);
  if (DeviceIndex >= MEMIF_MAXIMUM_NUMBER_OF_DEVICES)
  {
    MEMIF_DET_REPORT_ERROR(MEMIF_READ_ID, MEMIF_E_PARAM_DEVICE);
    ret = E_NOT_OK;
  }
  else
  {
    if(NULL_PTR == MemIf_SetModeFctPtr[DeviceIndex])
    {
      MEMIF_DET_REPORT_ERROR(MEMIF_READ_ID, MEMIF_E_PARAM_DEVICE);
      ret = E_NOT_OK;
    }
    else
    {
      ret = MemIf_ReadFctPtr[DeviceIndex](
      BlockNumber, BlockOffset, DataBufferPtr, Length);
    }
  }

  DBG_MEMIF_READ_EXIT(ret,DeviceIndex,BlockNumber,BlockOffset,DataBufferPtr,Length);
  return ret;
}

FUNC(Std_ReturnType, MEMIF_CODE) MemIf_Write(
  uint8                                    DeviceIndex,
  uint16                                   BlockNumber,
  P2CONST(uint8, AUTOMATIC, MEMIF_APPL_DATA) DataBufferPtr)
{
  Std_ReturnType ret;

  DBG_MEMIF_WRITE_ENTRY(DeviceIndex,BlockNumber,DataBufferPtr);
  if (DeviceIndex >= MEMIF_MAXIMUM_NUMBER_OF_DEVICES)
  {
    MEMIF_DET_REPORT_ERROR(MEMIF_WRITE_ID, MEMIF_E_PARAM_DEVICE);
    ret = E_NOT_OK;
  }
  else
  {
    if(NULL_PTR == MemIf_SetModeFctPtr[DeviceIndex])
    {
      MEMIF_DET_REPORT_ERROR(MEMIF_WRITE_ID, MEMIF_E_PARAM_DEVICE);
      ret = E_NOT_OK;
    }
    else
    {
      ret = MemIf_WriteFctPtr[DeviceIndex](BlockNumber,DataBufferPtr);
    }
  }

  DBG_MEMIF_WRITE_EXIT(ret,DeviceIndex,BlockNumber,DataBufferPtr);
  return ret;
}

#endif  /* (MEMIF_DEV_ERROR_DETECT == STD_ON)
         * && (MEMIF_MAXIMUM_NUMBER_OF_DEVICES > 1U) */


#if (MEMIF_MAXIMUM_NUMBER_OF_DEVICES > 1U)

FUNC(MemIf_StatusType, MEMIF_CODE) MemIf_GetStatus(uint8 DeviceIndex)
{
  MemIf_StatusType ret = MEMIF_UNINIT;

  DBG_MEMIF_GETSTATUS_ENTRY(DeviceIndex);

#if (MEMIF_DEV_ERROR_DETECT == STD_ON)
  if ((DeviceIndex >= MEMIF_MAXIMUM_NUMBER_OF_DEVICES)
      && (DeviceIndex != MEMIF_BROADCAST_ID))
  {
    MEMIF_DET_REPORT_ERROR(MEMIF_GETSTATUS_ID, MEMIF_E_PARAM_DEVICE);
    ret = MEMIF_UNINIT;
  }
  else
#endif /* #if (MEMIF_DEV_ERROR_DETECT == STD_ON) */
  {
    if (DeviceIndex == MEMIF_BROADCAST_ID)
    {
      uint8_least i;

      ret = MEMIF_IDLE;

      for (i = 0U; i < MEMIF_MAXIMUM_NUMBER_OF_DEVICES; ++i)
      {
        if( NULL_PTR != MemIf_GetStatusFctPtr[i])
        {
          const MemIf_StatusType status = MemIf_GetStatusFctPtr[i]();
          if (status == MEMIF_UNINIT)
          {
            ret = MEMIF_UNINIT;
            break;
          }

          if (ret != MEMIF_BUSY)
          {
            if (status == MEMIF_BUSY)
            {
              ret = MEMIF_BUSY;
            }
            if (status == MEMIF_BUSY_INTERNAL)
            {
              ret = MEMIF_BUSY_INTERNAL;
            }
          }
        }
      } /* end for */
    }
    else /* DeviceIndex != MEMIF_BROADCAST_ID */
    {
    #if (MEMIF_DEV_ERROR_DETECT == STD_ON)
      if( NULL_PTR == MemIf_GetStatusFctPtr[DeviceIndex])
      {
        MEMIF_DET_REPORT_ERROR(MEMIF_GETSTATUS_ID, MEMIF_E_PARAM_DEVICE);
        ret = MEMIF_UNINIT;
      }
      else
    #endif /* #if (MEMIF_DEV_ERROR_DETECT == STD_ON) */
      {
        ret = MemIf_GetStatusFctPtr[DeviceIndex]();
      }
    }
  }


  DBG_MEMIF_GETSTATUS_EXIT(ret,DeviceIndex);
  return ret;
}

FUNC(void, MEMIF_CODE) MemIf_SetMode(MemIf_ModeType Mode)
{
  uint8_least i;

  DBG_MEMIF_SETMODE_ENTRY(Mode);

  for (i = 0U; i < MEMIF_MAXIMUM_NUMBER_OF_DEVICES; ++i)
  {
    if( NULL_PTR != MemIf_SetModeFctPtr[i] )
      {
        MemIf_SetModeFctPtr[i](Mode);
      }
  }

  DBG_MEMIF_SETMODE_EXIT(Mode);
}

#endif  /* (MEMIF_MAXIMUM_NUMBER_OF_DEVICES > 1U) */


#if (MEMIF_VERSION_INFO_API == STD_ON)

FUNC(void, MEMIF_CODE) MemIf_GetVersionInfo(
  P2VAR(Std_VersionInfoType, AUTOMATIC, MEMIF_APPL_DATA) VersionInfoPtr)
{
  DBG_MEMIF_GETVERSIONINFO_ENTRY(VersionInfoPtr);
#if (MEMIF_DEV_ERROR_DETECT == STD_ON)
  if (NULL_PTR == VersionInfoPtr)
  {
    MEMIF_DET_REPORT_ERROR(MEMIF_GETVERSIONINFO_ID, MEMIF_E_PARAM_NULL_PTR);
  }
  else
#endif
  {
    VersionInfoPtr->vendorID         = MEMIF_VENDOR_ID;
    VersionInfoPtr->moduleID         = MEMIF_MODULE_ID;
    VersionInfoPtr->sw_major_version = MEMIF_SW_MAJOR_VERSION;
    VersionInfoPtr->sw_minor_version = MEMIF_SW_MINOR_VERSION;
    VersionInfoPtr->sw_patch_version = MEMIF_SW_PATCH_VERSION;
  }

  DBG_MEMIF_GETVERSIONINFO_EXIT(VersionInfoPtr);
}

#endif  /* (MEMIF_VERSION_INFO_API == STD_ON) */

#define MEMIF_STOP_SEC_CODE
#include <MemIf_MemMap.h>

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
