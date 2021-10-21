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
#ifndef MEMIF_TYPES_H
#define MEMIF_TYPES_H

/*==================[inclusions]============================================*/

/* Include module version information and publish them to drivers which only
 * include MemIf_Types.h */
#include <MemIf_Version.h>
/* AUTOSAR standard types */
#include <Std_Types.h>
/*==================[type definitions]======================================*/

/* !LINKSTO MemIf009,1 */
/** \brief Type definition of the MemIf status type
 **
 ** This type denotes the current status of the underlying abstraction module
 ** and device driver. It shall be used as the return value of the
 ** corresponding driver's "GetStatus" function. */
typedef uint8 MemIf_StatusType;

/** \brief Type definition of the MemIf job result type
 **
 ** This type denotes the result of the last job. */
typedef uint8 MemIf_JobResultType;

/** \brief Type definition for the operation mode
 **
 ** The operation mode of the underlying abstraction modules and device
 ** drivers. */
typedef uint8 MemIf_ModeType;

/*==================[macros]================================================*/

#if (defined MEMIF_BROADCAST_ID)
#error MEMIF_BROADCAST_ID is already defined
#endif /* if (defined MEMIF_BROADCAST_ID) */
/** \brief Device ID for "broadcast"
 **
 ** In case the parameter given as device ID is MEMIF_BROADCAST_ID,
 ** the memory abstraction interface shall iterate over all underlying
 ** devices and return their combined status. */
#define MEMIF_BROADCAST_ID 255U

#if (defined MEMIF_UNINIT)
#error MEMIF_UNINIT is already defined
#endif /* if (defined MEMIF_UNINIT) */
  /** \brief The underlying abstraction module or device driver has not been
   * initialized (yet). */
#define  MEMIF_UNINIT ((MemIf_StatusType)0U)

#if (defined MEMIF_IDLE)
#error MEMIF_IDLE is already defined
#endif /* if (defined MEMIF_IDLE) */
  /** \brief The underlying abstraction module or device driver is currently
   * idle. */
#define  MEMIF_IDLE ((MemIf_StatusType)1U)

#if (defined MEMIF_BUSY)
#error MEMIF_BUSY is already defined
#endif /* if (defined MEMIF_BUSY) */
  /** \brief The underlying abstraction module or device driver is currently
   * busy. */
#define  MEMIF_BUSY ((MemIf_StatusType)2U)

#if (defined MEMIF_BUSY_INTERNAL)
#error MEMIF_BUSY_INTERNAL is already defined
#endif /* if (defined MEMIF_BUSY_INTERNAL) */
  /** \brief The underlying abstraction module is busy with internal
   * management operations.
   *
   * The underlying device driver can be busy or idle. */
#define  MEMIF_BUSY_INTERNAL ((MemIf_StatusType)3U)

#if (defined MEMIF_JOB_OK)
#error MEMIF_JOB_OK is already defined
#endif /* if (defined MEMIF_JOB_OK) */
  /** \brief The job has been finished successfully. */
#define  MEMIF_JOB_OK ((MemIf_JobResultType)0U)

#if (defined MEMIF_JOB_FAILED)
#error MEMIF_JOB_FAILED is already defined
#endif /* if (defined MEMIF_JOB_FAILED) */
  /** \brief The job has not been finished successfully. */
#define  MEMIF_JOB_FAILED ((MemIf_JobResultType)1U)

#if (defined MEMIF_JOB_PENDING)
#error MEMIF_JOB_PENDING is already defined
#endif /* if (defined MEMIF_JOB_PENDING) */
  /** \brief The job has not yet been finished. */
#define  MEMIF_JOB_PENDING ((MemIf_JobResultType)2U)

#if (defined MEMIF_JOB_CANCELED)
#error MEMIF_JOB_CANCELED is already defined
#endif /* if (defined MEMIF_JOB_CANCELED) */
  /** \brief The job has been canceled. */
#define  MEMIF_JOB_CANCELED ((MemIf_JobResultType)3U)

#if (defined MEMIF_BLOCK_INCONSISTENT)
#error MEMIF_BLOCK_INCONSISTENT is already defined
#endif /* if (defined MEMIF_BLOCK_INCONSISTENT) */
  /** \brief The requested block is inconsistent, it may contain corrupted
   * data. */
#define  MEMIF_BLOCK_INCONSISTENT ((MemIf_JobResultType)4U)

#if (defined MEMIF_BLOCK_INVALID)
#error MEMIF_BLOCK_INVALID is already defined
#endif /* if (defined MEMIF_BLOCK_INVALID) */
  /** \brief The requested block has been marked as invalid, the requested
   * operation can not be performed. */
#define  MEMIF_BLOCK_INVALID ((MemIf_JobResultType)5U)
/* !LINKSTO MemIf.EB.CustomReturnTypes,1 */
#if (defined MEMIF_JOB_OK_SIZE_INCREASED)
#error MEMIF_JOB_OK_SIZE_INCREASED is already defined
#endif /* if (defined MEMIF_JOB_OK_SIZE_INCREASED) */
  /** \brief The requested block is ok but the block length size is increased
    *        compared to the one configured.  */
#define  MEMIF_JOB_OK_SIZE_INCREASED ((MemIf_JobResultType)6U)

#if (defined MEMIF_JOB_OK_SIZE_DECREASED)
#error MEMIF_JOB_OK_SIZE_DECREASED is already defined
#endif /* if (defined MEMIF_JOB_OK_SIZE_DECREASED) */
  /** \brief The requested block is ok but the block length size is decreased
   *          compared to the one configured.  */
#define  MEMIF_JOB_OK_SIZE_DECREASED ((MemIf_JobResultType)7U)

#if (defined MEMIF_MODE_SLOW)
#error MEMIF_MODE_SLOW is already defined
#endif /* if (defined MEMIF_MODE_SLOW) */
  /** \brief The underlying memory abstraction modules and drivers are working
   * in slow mode. */
#define  MEMIF_MODE_SLOW ((MemIf_ModeType)0U)

#if (defined MEMIF_MODE_FAST)
#error MEMIF_MODE_FAST is already defined
#endif /* if (defined MEMIF_MODE_FAST) */
  /** \brief The underlying memory abstraction modules and drivers are working
   * in fast mode. */
#define  MEMIF_MODE_FAST ((MemIf_ModeType)1U)


#endif /* MEMIF_TYPES_H */
/*==================[end of file]===========================================*/
