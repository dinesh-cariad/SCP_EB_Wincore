/**
 * \file
 *
 * \brief AUTOSAR Dcm
 *
 * This file contains the implementation of the AUTOSAR
 * module Dcm.
 *
 * \version 5.0.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef DCM_DSP_DATATRANSFER_SERVICES_H
#define DCM_DSP_DATATRANSFER_SERVICES_H

/* !LINKSTO Dcm.Dsn.File.DataTransferServices.PublicApi,1 */
/* This file contains the public API of the data transfer services functions. */

/*  MISRA-C:2004 Deviation List
 *
 *  MISRA-1) Deviated Rule: 8.9 (required)
 *     An identifier with external linkage shall have exactly one external definition.
 *
 *     Reason:
 *     The definition of the declared external variable is, for some unit tests, in the test source
 *     code as a dummy.
 *     The test source code is not included in the MISRA analysis and therefore it is
 *     not visible to the MISRA checker.
 */

 /* MISRA-C:2012 Deviation List
  *
  *  MISRAC2012-1) Deviated Rule: 8.6 (required)
  *     "An identifier with external linkage shall have exactly one external definition."
  *
  *     Reason:
  *     The definition of the declared external variable is, for some unit tests, in the test source
  *     code as a dummy.
  *     The test source code is not included in the MISRA analysis and therefore it is
  *     not visible to the MISRA checker.
  */

/*==================[inclusions]=================================================================*/

#include <Dcm_Int.h>

#if (DCM_DSP_USE_DATATRANSFERSERVICES == STD_ON)

/*==================[macros]=====================================================================*/

#if (defined DCM_DATA_TRANSFER_CONTEXT)
  #error "DCM_DATA_TRANSFER_CONTEXT is already defined"
#endif
#if (DCM_NUM_PROTOCOL > 1U)
#define DCM_DATA_TRANSFER_CONTEXT(instIdx) Dcm_DataTransferContext[(instIdx)]
#else
#define DCM_DATA_TRANSFER_CONTEXT(instIdx) Dcm_DataTransferContext[0U]
#endif /* #if (DCM_NUM_PROTOCOL > 1U) */

/*==================[type definitions]===========================================================*/

/** \brief Different States of Data Transfer. To specify whether Upload or Download is active. */

/* No Upload or Download is registered */
#if (defined DCM_TRANSFER_IDLE)
  #error "DCM_TRANSFER_IDLE is already defined"
#endif
#define DCM_TRANSFER_IDLE 0U

/* Download is registered by the service '0x34' */
#if (defined DCM_TRANSFER_DOWNLOAD)
  #error "DCM_TRANSFER_DOWNLOAD is already defined"
#endif
#define DCM_TRANSFER_DOWNLOAD 1U

/* Upload is registered by the service '0x35' */
#if (defined DCM_TRANSFER_UPLOAD)
  #error "DCM_TRANSFER_UPLOAD is already defined"
#endif
#define DCM_TRANSFER_UPLOAD 2U

typedef uint8 Dcm_DataTransferStateType;

/** \brief This type contains the information for data transfer services (0x34,0x35,0x36,0x37)
 ** module. */
typedef struct
{
  /** \brief Address to/from data to be download/upload */
  uint32 MemAddres;
  /** \brief Size of data to be download/upload */
  uint32 MemSize;
  /** \brief Total Size of data transfered in the transfer request */
  uint32 MemTransfered;
  /** \brief Total Size of data transfered in the current transfer request */
  uint32 CurrMemTransfered;
  /** \brief Max. Number of bytes for one Dcm_ReadMemory/Dcm_WriteMemory */
  uint32 BlockLength;
  /** \brief Specifies whether Upload or Download is active */
  Dcm_DataTransferStateType TransState;
  /** \brief For error handling in transfer data services */
  uint8 BlockSequenceCounter;
  /** \brief Stores the RequestDownload DataFormatIdentifier */
  uint8 RequestDownloadFormatId;
  /** \brief Stores the RequestUpload DataFormatIdentifier */
  uint8 RequestUploadFormatId;
}
Dcm_DataTransferType;

/** \brief  This type contains the configuration information for RequestDownload(0x34),
     RequestUpload(0x35) services.
*/
typedef struct
{
  /** \brief Lower Range of memory for Downloading/Uploading */
  uint32 MemRangeLow;
  /** \brief Upper Range of memory for Downloading/Uploading */
  uint32 MemRangeHigh;
  /** \brief Pointer to array of allowed security levels for Download/Upload memory */
  P2CONST(Dcm_SecLevelType, TYPEDEF, DCM_CONST) DldSecLevels;
  /** \brief Number of security levels for which this Memory can be Downloaded/Uploaded */
  uint8 NumDldSecLevels;
}
Dcm_TransferConfigType;

/*==================[external function declarations]=============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief Initializes the control structures used by the RequestDownload (0x34),
 **        RequestUpload (0x35), TransferData (0x36) and RequestTransferExit (0x37)
 **        service handlers
 */
extern FUNC(void, DCM_CODE) Dcm_UploadDownloadInit(void);

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/** \brief Context variable used to hold information regarding the processing of
           the service RequestDownload(0x34), RequestUpload(0x35). */
/* Deviation MISRAC2012-1 */
/* Deviation MISRA-1 */
extern VAR(Dcm_DataTransferType, DCM_VAR) Dcm_DataTransferContext[DCM_NUM_PROTOCOL];

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/*==================[internal data]==============================================================*/

/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/

#endif /* #if (DCM_DSP_USE_DATATRANSFERSERVICES == STD_ON) */

#endif /* ifndef DCM_DSP_DATATRANSFER_SERVICES_H */
/*==================[end of file]================================================================*/
