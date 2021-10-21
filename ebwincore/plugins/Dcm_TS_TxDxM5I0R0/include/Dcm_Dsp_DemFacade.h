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

#ifndef DCM_DSP_DEMFACADE_H
#define DCM_DSP_DEMFACADE_H

/*==================[inclusions]=================================================================*/

#include <Std_Types.h>                                                 /* AUTOSAR standard types */
#include <Dcm_Int.h>

#if (DCM_DEM_CLIENT_REF_USED == STD_ON)
#include <Dem_Types.h>
#endif /* #if (DCM_DEM_CLIENT_REF_USED == STD_ON) */


#if (DCM_DSP_USE_DEMFACADE == STD_ON)

/*==================[macros]=====================================================================*/

#if (defined DCM_DSP_SVCID_READDTCINFO)
  #error "DCM_DSP_SVCID_READDTCINFO is already defined"
#endif
/** \brief Service ID for Read DTC Information */
#define DCM_DSP_SVCID_READDTCINFO   0x19U

#if (defined DCM_DSP_READDTCINFORMATION_GETDTCBYOCCURENCETIME)
  #error "DCM_DSP_READDTCINFORMATION_GETDTCBYOCCURENCETIME is already defined"
#endif
/* Is GetDTCByOccurentTime subfunctionality of ReadDTCInformation available */
#if ((DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0B == STD_ON) ||\
     (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0C == STD_ON) ||\
     (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0D == STD_ON) ||\
     (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0E == STD_ON))
#define DCM_DSP_READDTCINFORMATION_GETDTCBYOCCURENCETIME STD_ON
#else
#define DCM_DSP_READDTCINFORMATION_GETDTCBYOCCURENCETIME STD_OFF
#endif

/* Macros to identify requesting subservice */
#if (defined DCM_DSP_READDTCINFORMATION_0x01_ID)
  #error "DCM_DSP_READDTCINFORMATION_0x01_ID is already defined"
#endif
#define DCM_DSP_READDTCINFORMATION_0x01_ID   (0x01U)

#if (defined DCM_DSP_READDTCINFORMATION_0x02_ID)
  #error "DCM_DSP_READDTCINFORMATION_0x02_ID is already defined"
#endif
#define DCM_DSP_READDTCINFORMATION_0x02_ID   (0x02U)

#if (defined DCM_DSP_READDTCINFORMATION_0x04_ID)
  #error "DCM_DSP_READDTCINFORMATION_0x04_ID is already defined"
#endif
#define DCM_DSP_READDTCINFORMATION_0x04_ID   (0x04U)

#if (defined DCM_DSP_READDTCINFORMATION_0x06_ID)
  #error "DCM_DSP_READDTCINFORMATION_0x06_ID is already defined"
#endif
#define DCM_DSP_READDTCINFORMATION_0x06_ID   (0x06U)

#if (defined DCM_DSP_READDTCINFORMATION_0x07_ID)
  #error "DCM_DSP_READDTCINFORMATION_0x07_ID is already defined"
#endif
#define DCM_DSP_READDTCINFORMATION_0x07_ID   (0x07U)

#if (defined DCM_DSP_READDTCINFORMATION_0x08_ID)
  #error "DCM_DSP_READDTCINFORMATION_0x08_ID is already defined"
#endif
#define DCM_DSP_READDTCINFORMATION_0x08_ID   (0x08U)

#if (defined DCM_DSP_READDTCINFORMATION_0x09_ID)
  #error "DCM_DSP_READDTCINFORMATION_0x09_ID is already defined"
#endif
#define DCM_DSP_READDTCINFORMATION_0x09_ID   (0x09U)

#if (defined DCM_DSP_READDTCINFORMATION_0x0F_ID)
  #error "DCM_DSP_READDTCINFORMATION_0x0F_ID is already defined"
#endif
#define DCM_DSP_READDTCINFORMATION_0x0F_ID   (0x0FU)

#if (defined DCM_DSP_READDTCINFORMATION_0x0A_ID)
  #error "DCM_DSP_READDTCINFORMATION_0x0A_ID is already defined"
#endif
#define DCM_DSP_READDTCINFORMATION_0x0A_ID   (0x0AU)

#if (defined DCM_DSP_READDTCINFORMATION_0x11_ID)
  #error "DCM_DSP_READDTCINFORMATION_0x11_ID is already defined"
#endif
#define DCM_DSP_READDTCINFORMATION_0x11_ID   (0x11U)

#if (defined DCM_DSP_READDTCINFORMATION_0x12_ID)
  #error "DCM_DSP_READDTCINFORMATION_0x12_ID is already defined"
#endif
#define DCM_DSP_READDTCINFORMATION_0x12_ID   (0x12U)

#if (defined DCM_DSP_READDTCINFORMATION_0x13_ID)
  #error "DCM_DSP_READDTCINFORMATION_0x13_ID is already defined"
#endif
#define DCM_DSP_READDTCINFORMATION_0x13_ID   (0x13U)

#if (defined DCM_DSP_READDTCINFORMATION_0x14_ID)
  #error "DCM_DSP_READDTCINFORMATION_0x14_ID is already defined"
#endif
#define DCM_DSP_READDTCINFORMATION_0x14_ID   (0x14U)

#if (defined DCM_DSP_READDTCINFORMATION_0x15_ID)
  #error "DCM_DSP_READDTCINFORMATION_0x15_ID is already defined"
#endif
#define DCM_DSP_READDTCINFORMATION_0x15_ID   (0x15U)

#if (defined DCM_DSP_READDTCINFORMATION_0x17_ID)
  #error "DCM_DSP_READDTCINFORMATION_0x17_ID is already defined"
#endif
#define DCM_DSP_READDTCINFORMATION_0x17_ID   (0x17U)

#if (defined DCM_DSP_READDTCINFORMATION_0x18_ID)
  #error "DCM_DSP_READDTCINFORMATION_0x18_ID is already defined"
#endif
#define DCM_DSP_READDTCINFORMATION_0x18_ID   (0x18U)

#if (defined DCM_DSP_READDTCINFORMATION_0x19_ID)
  #error "DCM_DSP_READDTCINFORMATION_0x19_ID is already defined"
#endif
#define DCM_DSP_READDTCINFORMATION_0x19_ID   (0x19U)

/* Macro to be able to clear the subservice ID */
#if (defined DCM_DSP_READDTCINFORMATION_UNSPECIFIC_ID)
  #error "DCM_DSP_READDTCINFORMATION_UNSPECIFIC_ID is already defined"
#endif
#define DCM_DSP_READDTCINFORMATION_UNSPECIFIC_ID (0xFFU)

#if (defined DCM_SUBSERVICES_EDR_AND_FF_GROUP)
  #error "DCM_SUBSERVICES_EDR_AND_FF_GROUP is already defined"
#endif
/* This Services/SubServices needs functionality for assemble ExtendedData and FreezeFrame */
#if( (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x06 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x19 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x04 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x18 == STD_ON))
#define DCM_SUBSERVICES_EDR_AND_FF_GROUP    STD_ON
#else
#define DCM_SUBSERVICES_EDR_AND_FF_GROUP    STD_OFF
#endif

#if (defined DCM_SUBSERVICES_FILTERED_DTC_GROUP)
  #error "DCM_SUBSERVICES_FILTERED_DTC_GROUP is already defined"
#endif
/* This Services/SubServices needs functionality for assemble filtered DTC infos */
#if ((DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x01 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x02 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x07 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x08 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0A == STD_ON) || \
     (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x13 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x14 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x15 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x17 == STD_ON) || \
     (DCM_DSP_USE_SERVICE_0X03 == STD_ON) || \
     (DCM_DSP_USE_SERVICE_0X07 == STD_ON) || \
     (DCM_DSP_USE_SERVICE_0X0A == STD_ON) )
#define DCM_SUBSERVICES_FILTERED_DTC_GROUP    STD_ON
#else
#define DCM_SUBSERVICES_FILTERED_DTC_GROUP    STD_OFF
#endif

#if (defined DCM_DEMFACADE_LOCK_MECHANISM_ENABLE)
  #error "DCM_DEMFACADE_LOCK_MECHANISM_ENABLE is already defined"
#endif
#if( ((DCM_HAS_ROE_TYPE2 == STD_ON) && (DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U)) ||\
     (DCM_DSP_USE_SERVICE_0X85 == STD_ON) )
#define DCM_DEMFACADE_LOCK_MECHANISM_ENABLE    STD_ON
#else
#define DCM_DEMFACADE_LOCK_MECHANISM_ENABLE    STD_OFF
#endif

/* Macro for Read Event Data services ID */
#if (defined DCM_DSP_SERVICE_0X19)
  #error "DCM_DSP_SERVICE_0X19 is already defined"
#endif
#define DCM_DSP_SERVICE_0X19                 (0x19U)

#if (defined DCM_DSP_SERVICE_0XAF)
  #error "DCM_DSP_SERVICE_0XAF is already defined"
#endif
#define DCM_DSP_SERVICE_0XAF                 (0xAFU)

/** \brief This value defines that a requested parameter from DEM has not been obtained */
#if (defined DCM_NOT_OBTAINED)
  #error "DCM_NOT_OBTAINED is already defined"
#endif
#define DCM_NOT_OBTAINED (0U)

/** \brief This value defines that a requested parameter from DEM has been obtained */
#if (defined DCM_OBTAINED)
  #error "DCM_OBTAINED is already defined"
#endif
#define DCM_OBTAINED (1U)

/** \brief This value defines if a specific task from the Facade has not been executed */
#if (defined DCM_NOT_COMPLETED)
  #error "DCM_NOT_COMPLETED is already defined"
#endif
#define DCM_NOT_COMPLETED (0U)

/** \brief This value defines if a specific task from the Facade has been executed */
#if (defined DCM_COMPLETED)
  #error "DCM_COMPLETED is already defined"
#endif
#define DCM_COMPLETED (1U)

/** \brief This value defines if a specific task from the Facade has not been executed */
#if (defined DCM_NOT_REQUIRED)
  #error "DCM_NOT_REQUIRED is already defined"
#endif
#define DCM_NOT_REQUIRED (1U)

/** \brief This value defines if a specific task from the Facade has been executed */
#if (defined DCM_REQUIRED)
  #error "DCM_REQUIRED is already defined"
#endif
#define DCM_REQUIRED (0U)

/** \brief This macro is specific to service 0xAF, when All DTCs are requested.*/
#if (defined DCM_REQUEST_ALL_DTCS)
  #error "DCM_REQUEST_ALL_DTCS is already defined"
#endif
#define DCM_REQUEST_ALL_DTCS (0xFFFFFFU)

/**\brief This macro is specific for service 0x19 when requesting all Supported DTCs */
#if (defined DCM_REPORT_ALLSUPPORTEDDTC)
  #error "DCM_REPORT_ALLSUPPORTEDDTC is already defined"
#endif
#define DCM_REPORT_ALLSUPPORTEDDTC (0x00U)

#if (defined DCM_STATUS_INDICATOR)
  #error "DCM_STATUS_INDICATOR is already defined"
#endif
#define DCM_STATUS_INDICATOR (0x30U)

/* This is the size in bytes of (Mask(3bytes) + Status Mask(1byte) + Status Indicator(1byte))
 * in the response for service 0xAF */
#if (defined DCM_SIZE_OF_HEADER_INFO)
  #error "DCM_SIZE_OF_HEADER_INFO is already defined"
#endif
#define DCM_SIZE_OF_HEADER_INFO (5U)

/* This is the expected size of DTC and Status Information when reading DTC information */
#if (defined DCM_SIZE_OF_DTC_AND_STATUS)
  #error "DCM_SIZE_OF_DTC_AND_STATUS is already defined"
#endif
#define DCM_SIZE_OF_DTC_AND_STATUS (4U)

/* This is the expected size of DTC and Status Information when reading Severity DTC information */
#if (defined DCM_SIZE_OF_DTC_AND_STATUS_WITH_SEVERITY)
  #error "DCM_SIZE_OF_DTC_AND_STATUS_WITH_SEVERITY is already defined"
#endif
#define DCM_SIZE_OF_DTC_AND_STATUS_WITH_SEVERITY (6U)

/* This is the expected size of an OBD DTC when reading DTC information */
#if (defined DCM_SIZE_OF_OBD_DTC)
  #error "DCM_SIZE_OF_OBD_DTC is already defined"
#endif
#define DCM_SIZE_OF_OBD_DTC 2U

/* Macros defining return codes for facade interface */
#if (defined DCM_FACADE_OK)
  #error "DCM_FACADE_OK is already defined"
#endif
#define DCM_FACADE_OK                  (0x00U)

#if (defined DCM_FACADE_PENDING)
  #error "DCM_FACADE_PENDING is already defined"
#endif
#define DCM_FACADE_PENDING             (0x01U)

#if (defined DCM_FACADE_BUFFERFULL)
  #error "DCM_FACADE_BUFFERFULL is already defined"
#endif
#define DCM_FACADE_BUFFERFULL          (0x02U)

#if (defined DCM_FACADE_REQUESTOUTOFRANGE)
  #error "DCM_FACADE_REQUESTOUTOFRANGE is already defined"
#endif
#define DCM_FACADE_REQUESTOUTOFRANGE   (0x03U)

#if (defined DCM_FACADE_NOT_OK)
  #error "DCM_FACADE_NOT_OK is already defined"
#endif
#define DCM_FACADE_NOT_OK              (0x04U)

#if (defined DCM_FACADE_WRONGFILTER)
  #error "DCM_FACADE_WRONGFILTER is already defined"
#endif
#define DCM_FACADE_WRONGFILTER         (0x05U)

#if (defined DCM_FACADE_WRONGCONDITION)
  #error "DCM_FACADE_WRONGCONDITION is already defined"
#endif
#define DCM_FACADE_WRONGCONDITION      (0x06U)

#if (defined DCM_FACADE_NO_SUCH_ELEMENT)
  #error "DCM_FACADE_NO_SUCH_ELEMENT is already defined"
#endif
#define DCM_FACADE_NO_SUCH_ELEMENT      (0x07U)


#if (defined DCM_FACADE_INVALID_VALUE)
  #error "DCM_FACADE_INVALID_VALUE is already defined"
#endif
#define DCM_FACADE_INVALID_VALUE      (0x08U)

/* Definitions for Kind of data to obtain from DEM */
#if (defined DCM_FACADE_FREEZE_FRAME_DATA)
  #error "DCM_FACADE_FREEZE_FRAME_DATA is already defined"
#endif
#define DCM_FACADE_FREEZE_FRAME_DATA 0U

#if (defined DCM_FACADE_EXTENDED_DATA_RECORDS)
  #error "DCM_FACADE_EXTENDED_DATA_RECORDS is already defined"
#endif
#define DCM_FACADE_EXTENDED_DATA_RECORDS 1U

#if (defined DCM_AMOUNT_OF_BYTES_OCCUPIED_ON_RESPONSE)
  #error "DCM_AMOUNT_OF_BYTES_OCCUPIED_ON_RESPONSE is already defined"
#endif
#define DCM_AMOUNT_OF_BYTES_OCCUPIED_ON_RESPONSE (4U)


/* Macros for flow control of data supplier */
#if (defined DCM_DATA_SUPPLIER_NOT_REQUIRED)
  #error "DCM_DATA_SUPPLIER_NOT_REQUIRED is already defined"
#endif
#define DCM_DATA_SUPPLIER_NOT_REQUIRED 0U

#if (defined DCM_DATA_SUPPLIER_REQUIRED)
  #error "DCM_DATA_SUPPLIER_REQUIRED is already defined"
#endif
#define DCM_DATA_SUPPLIER_REQUIRED     1U

#if (defined DCM_DATA_SUPPLIER_IN_PROGRESS)
  #error "DCM_DATA_SUPPLIER_IN_PROGRESS is already defined"
#endif
#define DCM_DATA_SUPPLIER_IN_PROGRESS  2U

#if (defined DCM_DATA_SUPPLIER_DONE)
  #error "DCM_DATA_SUPPLIER_DONE is already defined"
#endif
#define DCM_DATA_SUPPLIER_DONE         3U

#if (defined DCM_DATA_SUPPLIER_FAILED)
  #error "DCM_DATA_SUPPLIER_FAILED is already defined"
#endif
#define DCM_DATA_SUPPLIER_FAILED       4U

/* Macros for RecordUpdate. */
#if (defined DCM_RECORDUPDATE_ENABLED)
#error "DCM_RECORDUPDATE_ENABLED is already defined"
#endif
#define DCM_RECORDUPDATE_ENABLED          0U

#if (defined DCM_RECORDUPDATE_DISABLED)
#error "DCM_RECORDUPDATE_DISABLED is already defined"
#endif
#define DCM_RECORDUPDATE_DISABLED         1U

#if (defined DCM_RECORDUPDATE_DOENABLE)
#error "DCM_RECORDUPDATE_DOENABLE is already defined"
#endif
#define DCM_RECORDUPDATE_DOENABLE         0U

/* Macros for operations of data supplier */
#if (defined DCM_DATA_SUPPLIER_SETUP)
  #error "DCM_DATA_SUPPLIER_SETUP is already defined"
#endif
#define DCM_DATA_SUPPLIER_SETUP    0U

#if (defined DCM_DATA_SUPPLIER_GET_SIZE)
  #error "DCM_DATA_SUPPLIER_GET_SIZE is already defined"
#endif
#define DCM_DATA_SUPPLIER_GET_SIZE 1U

#if (defined DCM_DATA_SUPPLIER_GET_DATA)
  #error "DCM_DATA_SUPPLIER_GET_DATA is already defined"
#endif
#define DCM_DATA_SUPPLIER_GET_DATA 2U

#if (defined DCM_DATA_SUPPLIER_CANCEL)
  #error "DCM_DATA_SUPPLIER_CANCEL is already defined"
#endif
#define DCM_DATA_SUPPLIER_CANCEL   3U

#if (DCM_DSP_USE_SERVICE_0X85 == STD_ON)
/* Macros defining return codes for ControlDTCSetting interface */
#if (defined DCM_CONTROL_DTC_SETTING_OK)
  #error "DCM_CONTROL_DTC_SETTING_OK is already defined"
#endif
#define DCM_CONTROL_DTC_SETTING_OK      0U

#if (defined DCM_CONTROL_DTC_SETTING_PENDING)
  #error "DCM_CONTROL_DTC_SETTING_PENDING is already defined"
#endif
#define DCM_CONTROL_DTC_SETTING_PENDING  1U

#if (defined DCM_CONTROL_DTC_UNKNOWN_VALUE)
  #error "DCM_CONTROL_DTC_UNKNOWN_VALUE is already defined"
#endif
#define DCM_CONTROL_DTC_UNKNOWN_VALUE   2U
#endif /* #if (DCM_DSP_USE_SERVICE_0X85 == STD_ON) */

/* Definitions of ClearDTCInfoState for Dcm_DemClientContextType */
#if (DCM_DSP_USE_SERVICE_0X14 == STD_ON) || (DCM_DSP_USE_SERVICE_0X04 == STD_ON)
#if (defined STATE_CLEARDTCINFO_GETDTCSELECTIONRESULTFORCLEARDTC)
  #error "STATE_CLEARDTCINFO_GETDTCSELECTIONRESULTFORCLEARDTC is already defined"
#endif
#define STATE_CLEARDTCINFO_GETDTCSELECTIONRESULTFORCLEARDTC   0U

#if (defined STATE_CLEARDTCINFO_CLEARDTC)
  #error "STATE_CLEARDTCINFO_CLEARDTC is already defined"
#endif
#define STATE_CLEARDTCINFO_CLEARDTC                          1U
#endif /* #if (DCM_DSP_USE_SERVICE_0X14 == STD_ON) || (DCM_DSP_USE_SERVICE_0X04 == STD_ON) */
/*==================[type declarations]===========================================================*/

/** \brief Pointer to DEM interface function to select data */
typedef P2FUNC(uint8, DCM_APPL_CODE, Dem_SelectDataDemFunctionType) (
  Dcm_DemClientIdType ClientId,
  uint8 DataNumber);

/** \brief Pointer to DEM interface function to get size of data */
typedef P2FUNC(uint8, DCM_APPL_CODE, Dem_GetSizeOfDataDemFunctionType) (
  Dcm_DemClientIdType ClientId,
  P2VAR(uint16, TYPEDEF, DCM_VAR) SizeOfExtendedDataRecord);

/** \brief Pointer to DEM interface function to get data */
typedef P2FUNC(uint8, DCM_APPL_CODE, Dem_GetDataDemFunctionType) (
  Dcm_DemClientIdType ClientId,
  P2VAR(uint8, TYPEDEF, DCM_VAR) DestBuffer,
  P2VAR(uint16, TYPEDEF, DCM_VAR) BufSize);

/** \brief This data structure is meant to handle the program flow and contain the input and
 * output data for processing of sub-function 0x06, 0x10 and 0x19 of service 0x19 relevant for
 * obtaining the Status information of a particular DTC */
typedef struct
{
  /* This element contains the DTC Mask from the service request */
  uint32 DTCMask;

  /* Available amount of data free on the response buffer */
  Dcm_MsgLenType BufferSize;

  /* This pointer contains the address where the answer for the sub-service will be stored */
  P2VAR(uint8, TYPEDEF, DCM_VAR) ResponseBuffer;

  /* This data is relevant for program flow control */
  boolean StatusOfDTC;
}
Dcm_GetDTCStatusContextType;

/** \brief This data structure is meant to handle the program flow
 * of sub-function 0x09 of service 0x19 relevant for
 * obtaining the severity and functional unit information of a particular DTC */
typedef struct
{
  /* Indicates if the Severity of DTC was obtained or not. */
  boolean SeverityOfDTC;
  /* Indicates if the Functional Unit of DTC was obtained or not. */
  boolean FunctionalUnitOfDTC;
}
Dcm_GetDTCSeverityContextType;

#if(DCM_DSP_USE_SERVICE_0XAF == STD_ON)
/** \brief This data structure is meant to handle the program flow and contain the input and
 * output data for processing of sub-function 0x06 and 0x04 of service 0xAF relevant for
 * obtaining the Status Indicators SI30 */
typedef struct
{
  /* Available amount of data free on the response buffer */
  Dcm_MsgLenType BufferSize;

  /* This pointer contains the address where the answer for the sub-service will be stored */
  P2VAR(uint8, TYPEDEF, DCM_VAR) ResponseBuffer;

  /* This data is relevant for program flow control */
  boolean StatusOfSI30ForCurrentDTC;

  /* This flag indicates if a data for SI was selected successfully. */
  boolean ReportOnSelectStatusIndicator;
}
Dcm_GetSI30ContextType;
#endif /* #if(DCM_DSP_USE_SERVICE_0XAF == STD_ON) */

#if (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON) || (DCM_SUBSERVICES_FILTERED_DTC_GROUP == STD_ON)
/** \brief This data structure is meant to handle the program flow and contain the input and
 * output data for processing of sub-function 0x01, 0x02, 0x04, 0x06, 0x18 and 0x19 of service 0x19
 * relevant to obtaining the Extended Data Record information */
typedef struct
{
  /* this element contains the DTC Mask from the service request */
  uint32 DTCMask;

  /* Available amount of data free on the response buffer */
  Dcm_MsgLenType BufferSize;

  /* Amount of data which will be written to the response buffer after reading Data from DEM */
  Dcm_MsgLenType AmountOfDataToWriteOnResponse;

  /* Last position filled on the response buffer */
  Dcm_MsgLenType NextPossitionToFillOnResponseBuffer;

  /* Amount of data collected from DEM so far */
  Dcm_MsgLenType AmountOfDataStoredOnCompleteResponse;

  /* Amount of data collected from DEM on current run */
  Dcm_MsgLenType AmountOfDataStoredOnResponseOnCurrentRun;

#if (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON)
  /* Size of one data page */
  Dcm_MsgLenType SizeOfOnePage;

  /* DEM Function Pointer for selecting data */
  Dem_SelectDataDemFunctionType SelectDataDemFunction;

  /* DEM Function Pointer for obtaining data size */
  Dem_GetSizeOfDataDemFunctionType GetSizeOfDataDemFunction;

  /* DEM Function Pointer for obtaining Data*/
  Dem_GetDataDemFunctionType GetDataDemFunction;
#endif /* #if (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON) */

  /* This pointer contains the address where the answer for the sub-service will be stored */
  P2VAR(uint8, TYPEDEF, DCM_VAR) ResponseBuffer;

#if (DCM_SUBSERVICES_FILTERED_DTC_GROUP == STD_ON)

  uint16 NumberOfFilteredDTCs;
  uint16 StoredDTCs;

#endif /* #if (DCM_SUBSERVICES_FILTERED_DTC_GROUP == STD_ON) */

#if (DCM_DSP_USE_SERVICE_0XAF == STD_ON)
  /* This variable contains the Service ID from which the current call has been executed */
  uint8 ServiceID;
#endif /* #if (DCM_DSP_USE_SERVICE_0XAF == STD_ON) */

  /* this element contains the DTC Status Mask from obtained from DEM */
  uint8 DTCStatusMask;

#if (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON)
  /* Requested Extended Data record / Freeze Frame Data record */
  uint8 RequestedDataRecordNumber;

  /* Kind of data to be retrieved */
  uint8 DataKind;

  /* This data is relevant to control flow control of the executer*/
  boolean ReportOnSizeOfDataToRead;

  /* This flag indicates if a data was selected successfully. */
  boolean ReportOnSelectData;

  /* This data is relevant for program flow control */
  boolean ReportOnReadOfExtendedDataRecord;

  /* This flag is used to control the Record Update */
  uint8 ControlRecordUpdate;

#endif /* #if (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON) */

  uint8 DTCFormat;

#if (DCM_SUBSERVICES_FILTERED_DTC_GROUP == STD_ON)
  /* This flag is relevant for control flow of the executer */
  boolean RequestOfReadOfTranslationType;

  /* This flag is relevant for control flow of the executer */
  boolean ReportOnReadOfNumberOfFilteredDtc;

  /* This flag is relevant for control flow of the executer */
  boolean ReportOnGetNumberOfFilters;

  /* This flag is relevant for control flow of the executer */
  boolean ReportOnSetDTCFilter;

  uint8 DTCKind;

  boolean FilterWithSeverity;

  uint8 DTCSeverityMask;

  boolean FilterForFaultDetectionCounter;

  uint8 SubServiceId;

  uint8 RequestedStatusMask;

#endif /* #if (DCM_SUBSERVICES_FILTERED_DTC_GROUP == STD_ON) */
}
Dcm_DiagnosticDataContextType;
#endif /* #if ( (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON) ||
                (DCM_SUBSERVICES_FILTERED_DTC_GROUP == STD_ON) )*/

/** \brief This type used to indicate the operation for a data supplier function call. */
typedef uint8 Dcm_DataSupplierOperationType;

/** \brief This data structure is meant to maintain all global variables relevant for DTC services */
typedef struct
{
  /* This value maintains the DTCMask selected */
  uint32 DTCMask;

  /* This variable contains the maximum amount of bytes which a single page can contain */
  Dcm_MsgLenType SizeOfOnePage;

  /* This variable holds the count of bytes which have already been written
   * by the sub-service handler into the current Page in the context of Page Buffering */
  Dcm_MsgLenType AmountOfDataWrittenOnCurrentPage;

  /* This value maintains the MemoryOrigin selected */
  uint16 MemoryOrigin;

  /* This variable contains the ServiceID which is currently being handled */
  uint8 ServiceID;

  /* This variable contains the SubserviceID which is currently being handled */
  uint8 SubServiceID;

  /* Kind of data to be retrieved */
  uint8 DataKind;

  /* Format of the DTC */
  uint8 DTCFormat;

  /* Requested filter for FaultDetectionCounter */
  boolean FilterForFaultDetectionCounter;

  /* This variable must retain the requested data record number
   * for sub-service 0x19-0x04 0x19-0x06 and 0x19-0x19 */
  uint8 DataRecordNumber;

  /* This variable contains the requested Mask
   * for sub-services 0x19-0x01, -0x02, -0x0A, -0x14, -0x15 and -0x17 */
  uint8 RequestedStatusMask;

  /* This variable contains the effective requested status Bits */
  uint8 RequestedStatusBits;

  /* This variable contains the requested SeverityMask
   * for sub-services 0x19-0x07, 0x19-0x08, 0x19-0x09 */
#if ( (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x07 == STD_ON) || \
      (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x08 == STD_ON) )
  uint8 RequestedSeverityMask;
#endif /* #if ( (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x07 == STD_ON) || \
                (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x08 == STD_ON) ) */

  /* Purpose of this variable is to avoid re-writing the DTC status on the response buffer */
  boolean DTCStatusWrittenInResponseBuffer;

  /* This flag will indicate if the page buffering mechanism is needed or not */
  boolean PageBufferingFlag;

  /* This flag will indicate if a page has already been sent on current page buffered transaction */
  boolean FirstPageSent;

  /* Process status regarding the memory origin */
  uint8 MemoryOriginState;

  /* Process status regarding the DEM availability mask */
  uint8 AvailabilityMaskState;

  /* Process status regarding the DTC format identifier */
  uint8 DtcFormatIdState;

  /* Process status regarding the count of filtered DTCs */
  uint8 FilteredDtcsCountState;

  /* Process status regarding the filtered DTCs */
  uint8 FilteredDtcsState;

#if (DCM_DSP_USE_SERVICE_0X14 == STD_ON) || (DCM_DSP_USE_SERVICE_0X04 == STD_ON)
  /* Process status regarding the clearing DTCs */
  uint8 ClearDTCInfoState;
#endif

#if(DCM_DEMFACADE_LOCK_MECHANISM_ENABLE == STD_ON)
  /* Mutex controlling access to DEM client instance */
  Dcm_MutexType Mutex;
#endif

#if((DCM_HAS_ROE_TYPE2 == STD_ON) && (DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0))
  /* This flag will indicate if all amount of data has been sent */
  boolean PageBufferingDone;
#endif

#if (DCM_DSP_USE_SERVICE_0XAF == STD_ON)
  /* This flag indicates if the request has been interrupted by pending or other reason */
  boolean CurrentDTCProcessingReady;
#endif /* #if (DCM_DSP_USE_SERVICE_0XAF == STD_ON) */

#if ( (DCM_SUBSERVICES_FILTERED_DTC_GROUP == STD_ON) || \
      (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON) )
  /** \brief This is the context variable for holding status and data information for the Read Number
   ** of Filtered DTC operation and for the Read Extended Data Record operation */
  /* Deviation MISRAC2012-3 */
  Dcm_DiagnosticDataContextType DiagnosticDataContext;

#endif /* #if ( (DCM_SUBSERVICES_FILTERED_DTC_GROUP == STD_ON) || \
                (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON) ) */
}
Dcm_DemClientContextType;

#if (DCM_DSP_USE_SERVICE_0XAF == STD_ON)
typedef struct
{
  /* this counter keeps the number of DTCs which where processed on the complete request */
  uint16 AmountOfDTCsProcessed;

  /* This value maintains the MemoryOrigin selected */
  uint16 MemoryOrigin;

  /* This variable maintains the sort of data to read from DEM FreezeFrame (FF)
   * or ExtendedDataRecord (EDR)*/
  uint8 DataKind;

  /* This array holds the relevant header data */
  uint8 HeaderData[5];

  /* This variable indicates if the AF header has been written for the first time */
  boolean WriteHeaderDataBeforeRecordNumber;

  /* This flag indicates if the request is for all DTCs or only for one */
  boolean RequestAllDTCs;

  /* This flag indicates that the data request has reached an end, for one or all DCTs */
  boolean ReachedEndOfRequest;

  /* This flag indicates that there are valid DTCs to be read */
  boolean AtLeastOneDTCExists;
}
Dcm_ReadeventEventDataServicesContextType;
#endif /*#if (DCM_DSP_USE_SERVICE_0XAF == STD_ON)*/

#if (DCM_DSP_USE_SERVICE_0X85 == STD_ON)
typedef uint8 Dcm_ReturnControlDTCSettingType;
#endif /* #if (DCM_DSP_USE_SERVICE_0X85 == STD_ON) */

/*==================[external constant declarations]==============================================*/

/*==================[external data declarations]==================================================*/

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

#if ( (DCM_SUBSERVICES_FILTERED_DTC_GROUP == STD_ON) || \
      (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON) || \
      (DCM_0XAF_EXT_SSVCH_ENABLED == STD_ON) )

#if (DCM_DSP_USE_SERVICE_0XAF == STD_ON)
/** \brief This is the context variable for the Service context */
extern VAR(Dcm_ReadeventEventDataServicesContextType, DCM_VAR) Dcm_ReadEventDataServicesContext;
#endif /*#if (DCM_DSP_USE_SERVICE_0XAF == STD_ON)*/

#endif /* #if ( (DCM_SUBSERVICES_FILTERED_DTC_GROUP == STD_ON) || \
                (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON) || \
                (DCM_0XAF_EXT_SSVCH_ENABLED == STD_ON) ) */

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/*==================[external function declarations]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief Initializes the DemFacade unit
 *
 *  \return void **/
extern FUNC(void, DCM_CODE) Dcm_Dsp_DemFacade_Init(void);

#if (DCM_SUBSERVICES_FILTERED_DTC_GROUP == STD_ON)
/** \brief Retrieves the DTCs matching the filter.
 **
 ** \param[in]    Dcm_DemClientIdType  ClientId      This parameter defines the Dem Client instance.
 ** \param[in]    Dcm_OpStatusType     OpStatus      Operation state of the function.
 ** \param[inout] Dcm_MsgContextType   *pMsgContext  Message related information for one diagnostic
 **                                                  protocol identifier.
 **
 ** \return                     Std_ReturnType
 ** \retval DCM_E_DONE          Operation completed
 ** \retval DCM_E_PENDING       Operation did not complete, it must be called again
 ** \retval DCM_E_PROCESS_PAGE  One page is ready to be given to the PduR
 ** \retval DCM_E_NOT_OK        Current page buffer is full for services not supporting page buffering
 **/
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DemFacade_GetDTCInfoByFilter_Process(
  Dcm_DemClientIdType ClientId,
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /* #if (DCM_SUBSERVICES_FILTERED_DTC_GROUP == STD_ON) )*/

#if (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON)
/** \brief Retrieves the selected DTC.
 **
 ** \param[in]    Dcm_DemClientIdType  ClientId      This parameter defines the Dem Client instance.
 ** \param[in]    Dcm_OpStatusType     OpStatus      Operation state of the function.
 ** \param[inout] Dcm_MsgContextType   *pMsgContext  Message related information for one diagnostic
 **                                                  protocol identifier.
 **
 ** \return                     Std_ReturnType
 ** \retval DCM_E_DONE          Operation completed
 ** \retval DCM_E_PENDING       Operation did not complete, it must be called again
 ** \retval DCM_E_PROCESS_PAGE  One page is ready to be given to the PduR
 ** \retval DCM_E_NOT_OK        Current page buffer is full for services not supporting page buffering
 **/
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DemFacade_GetDTCInfoBySelect_Process(
  Dcm_DemClientIdType ClientId,
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /* #if (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON)*/

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x09 == STD_ON)
/** \brief Retrieves the selected DTC and severity information.
 **
 ** \param[in]    Dcm_DemClientIdType  ClientId      This parameter defines the Dem Client instance.
 ** \param[in]    Dcm_OpStatusType     OpStatus      Operation state of the function.
 ** \param[inout] Dcm_MsgContextType   *pMsgContext  Message related information for one diagnostic
 **                                                  protocol identifier.
 **
 ** \return                     Std_ReturnType
 ** \retval DCM_E_DONE          Operation completed
 ** \retval DCM_E_PENDING       Operation did not complete, it must be called again
 ** \retval DCM_E_NOT_OK        Current page buffer is full for services not supporting page buffering
 **/
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DemFacade_GetDTCSeverityInfoBySelect_Process(
  Dcm_DemClientIdType ClientId,
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /* #if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x09 == STD_ON)*/

#if ((DCM_SUBSERVICES_FILTERED_DTC_GROUP == STD_ON) || (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON))
#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)

/** \brief The function to Update amount of data filled on response buffer
 **
 ** \param[in]  DcmRxPduId the Rx Pdu Id for obtaining the correct protocol configuration
 ** \param[in]  AmountOfFilledData amount of data successfully updated.
 **
 **/
extern FUNC(void, DCM_CODE) Dcm_DspInternal_UpdateAmountOfFilledDataOnResponseBuffer
(
  PduIdType DcmRxPduId,
  Dcm_MsgLenType AmountOfFilledData
);

/** \brief Used to enable process page function to be called from Call Service function of DSP SM
 **
 ** \param[in] pMsgContext Message related information for one diagnostic protocol identifier.
 ** \param[in] ProcessPageFuncId Process page function id
 **
 ** \retval None
 */
extern FUNC(void, DCM_CODE) Dcm_DspInternal_EnableProcessPage (
  P2CONST(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  uint8 ProcessPageFuncId);

#endif /*#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)*/
#endif /* #if ((DCM_SUBSERVICES_FILTERED_DTC_GROUP == STD_ON) ||
               (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON)) */

#if(DCM_DEMFACADE_LOCK_MECHANISM_ENABLE == STD_ON)
/** \brief Function to lock the Dem client instance when it needs to be used exclusively by one user
 **
 ** \param[in] Dcm_DemClientIdType ClientId  This parameter defines the Dem client instance
 **
 ** \retval DCM_E_OK                Dem client successfully locked
 ** \retval DCM_E_RESOURCE_LOCKED   Dem client already locked and not available
 **/
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DemFacade_LockClient(Dcm_DemClientIdType ClientId);

/** \brief Function to unlock the Dem client instance after it was locked
 **
 ** \param[in] Dcm_DemClientIdType ClientId  This parameter defines the Dem client instance
 **
 ** \retval None
 **/
extern FUNC(void, DCM_CODE) Dcm_Dsp_DemFacade_UnlockClient(Dcm_DemClientIdType ClientId);
#endif /* #if(DCM_DEMFACADE_LOCK_MECHANISM_ENABLE == STD_ON) */
#if ((DCM_SUBSERVICES_FILTERED_DTC_GROUP == STD_ON) || \
     (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON) || \
     (DCM_0XAF_EXT_SSVCH_ENABLED == STD_ON) || \
     (DCM_DSP_USE_SERVICE_0X14 == STD_ON) || (DCM_DSP_USE_SERVICE_0X04 == STD_ON) || \
     (DCM_DEMFACADE_LOCK_MECHANISM_ENABLE == STD_ON))
/** \brief Function to get the Dem client instance context
 **
 ** \param[in] Dcm_DemClientIdType ClientId  This parameter defines the Dem client instance
 **
 ** \retval reference to Dem client instance context
 **/
extern FUNC_P2VAR(Dcm_DemClientContextType, AUTOMATIC, DCM_CODE) Dcm_Dsp_DemFacade_GetClient(Dcm_DemClientIdType ClientId);
#endif /* #if ((DCM_SUBSERVICES_FILTERED_DTC_GROUP == STD_ON) || \
               (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON) || \
               (DCM_0XAF_EXT_SSVCH_ENABLED == STD_ON) || \
               (DCM_DSP_USE_SERVICE_0X14 == STD_ON) || (DCM_DSP_USE_SERVICE_0X04 == STD_ON) || \
               (DCM_DEMFACADE_LOCK_MECHANISM_ENABLE == STD_ON)) */
#if (DCM_DSP_USE_SERVICE_0X02 == STD_ON)

/** \brief Wrapper for the Dem interface Dem_DcmReadDataOfOBDFreezeFrame() to get data element
 *  per PID and index of the most important freeze frame for service $02.
 *
 *  \param[in]    PID                    This parameter is an identifier for a PID as defined
 *                                       in ISO15031-5.
 *  \param[in]    DataElementIndexOfPID  Data element index of this PID according to
 *                                       the Dcm configuration of service $02.
 *  \param[inout] DestBuffer             This parameter contains a byte pointer that points to
 *                                       the buffer, to which the data element of the PID shall
 *                                       be written to.
 *  \param[inout] BufSize                This parameter contains the maximum number of data bytes
 *                                       that can be written to the buffer.
 *  \return Std_ReturnType **/
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DemFacade_ReadDataOfOBDFreezeFrame(
  uint8 PID,
  uint8 DataElementIndexOfPID,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) DestBuffer,
  P2VAR(uint16, AUTOMATIC, DCM_VAR) BufSize
);

/** \brief Wrapper for the Dem interface Dem_DcmGetDTCOfOBDFreezeFrame() to get the OBD DTC for
 *  the given FrameNumber.
 *  Since only FrameNumber 0x00 is supported, this function does not have parameter FrameNumber.
 *  Since it is currently only called from service $02, this function does not have parameter DTCFormat.
 *
 *  \param[out] DTC  Diagnostic Trouble Code in ODB format. If the return value of
 *                   the function is other than E_OK this parameter does not contain valid data.
 *  \return Std_ReturnType **/
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DemFacade_GetDTCOfOBDFreezeFrame(
  P2VAR(uint32, AUTOMATIC, DCM_VAR) DTC
);

#endif /*(DCM_DSP_USE_SERVICE_0X02 == STD_ON) */

#if (DCM_DSP_READDTCINFORMATION_GETDTCBYOCCURENCETIME == STD_ON)
/** \brief Retrieves a DTC by occurrence time, if available.
 *
 *  \param[in] Dcm_DemClientIdType           ClientId     This parameter defines the Dem Client
 *                                                        instance
 *  \param[in] Dem_DTCRequestType            DTCRequest   This parameter defines the requested
 *                                                        occurence time type of the DTC
 *  \param[in] Dcm_OpStatusType              OpStatus     Operation status of the service handler
 *
 *  \param[out] Dcm_MsgType                  *RespBufPtr  Buffer where to store the constructed
 *                                                        response message
 *  \param[out] Dcm_MsgLenType               *RespLenPtr  Length of the constructed response with
 *                                                        or without DTC and status
 *  \param[out] Dcm_NegativeResponseCodeType *NrcPtr      Negative response code in case the
 *                                                        request fails
 *  \return                   Std_ReturnType
 *
 *  \retval DCM_E_OK          Positive response message with or without DTC and status
 *
 *  \retval DCM_E_NOT_OK      Negative response message
 *
 *  \retval DCM_E_PEDNING     The status of the DTC could not be retrieved in the current cycle
 *                            call the handler again in the next cycle
 *
 **/
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DemFacade_GetDTCInfoByOccurrence_Process(
  Dcm_DemClientIdType ClientId,
  Dem_DTCRequestType DTCRequest,
  Dcm_OpStatusType OpStatus,
  Dcm_MsgType RespBufPtr,
  P2VAR(Dcm_MsgLenType, AUTOMATIC, DCM_VAR) RespLenPtr,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) NrcPtr
);
#endif /* #if (DCM_DSP_READDTCINFORMATION_GETDTCBYOCCURENCETIME == STD_ON) */

#if ((DCM_DSP_USE_SERVICE_0X06 == STD_ON) && (DCM_OBDMID_SUPPORT_SWC == STD_OFF))

/** \brief Wrapper for the Dem interface Dem_DcmGetAvailableOBDMIDs(), which reports the value of a
 **        requested "availability-OBDMID" to the DCM upon a Service $06 request. Derived from
 **        that the tester displays the supported tests a mechanic can select from.
 **
 ** \param[in]  ObdMid          Availablity OBDMID ($00,$20, $40. . . )
 ** \param[out] pObdmidvalue    Bit coded information on the support of OBDMIDs.
 **
 ** \retval E_OK                Report of DTR result successful
 ** \retval E_NOT_OK            Report of DTR result failed
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DemFacade_GetAvailableOBDMIDs(
  uint8 ObdMid,
  P2VAR(uint32, AUTOMATIC, DCM_VAR) pObdMidValue);

/** \brief Wrapper for the Dem interface Dem_DcmGetNumTIDsOfOBDMID(), which gets the number of
 **        TIDs per (functional) OBDMID. This can be used by the DCM to iteratively request for
 **        OBD/TID result data within a loop from 0. . . .numberOfTIDs-1
 **
 ** \param[in]  ObdMid        OBDMID subject of the request to identify the number of assigned TIDs
 ** \param[out] pNumberOfTIDs Number of assigned TIDs for the requested OBDMID. Used as loop value
 **                           for the DCM to retrieve all OBD/TID result data.
 **
 ** \retval E_OK              Getting number of TIDs per OBDMID result successful
 ** \retval E_NOT_OK          Getting number of TIDs per OBDMID result failed
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DemFacade_GetNumTIDsOfOBDMID(
  uint8 ObdMid,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) pNumberOfTIDs);

/** \brief Wrapper for the Dem interface Dem_DcmGetDTRData(), which reports a DTR data along with
 **        TID-value, UaSID, test result with lower and upper limit.
 **
 ** \param[in]  ObdMid        Identification of a DTR element by assigned DTRId.
 ** \param[in]  TIDIndex      Index of the TID within the DEM. Runs from 0 to "numberOfTIDs"
 **                           obtained in the call to Dem_DcmGetNumTIDsOfOBDMID()
 **
 ** \param[out] pTIDValue     TID to be put on the tester reponse
 ** \param[out] pUaSID        UaSID to be put on the tester reponse
 ** \param[out] pTestValue    Latest test result
 ** \param[out] pLowLimValue  Lower limit value associated to the latest test result
 ** \param[out] pUppLimValue  Upper limit value associated to the latest test result
 **
 ** \retval E_OK              Report of DTR result successful
 ** \retval E_NOT_OK          Report of DTR result failed
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DemFacade_GetDTRData(
  uint8 ObdMid,
  uint8 TIDIndex,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) pTIDValue,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) pUaSID,
  P2VAR(uint16, AUTOMATIC, DCM_VAR) pTestValue,
  P2VAR(uint16, AUTOMATIC, DCM_VAR) pLowLimValue,
  P2VAR(uint16, AUTOMATIC, DCM_VAR) pUppLimValue);

#endif /* #if ((DCM_DSP_USE_SERVICE_0X06 == STD_ON) && (DCM_OBDMID_SUPPORT_SWC == STD_OFF)) */

#if (DCM_DSP_USE_SERVICE_0X85 == STD_ON)
/** \brief Used to call the Dem function that enable the ControlDTCSetting
 **
 ** \param [in] ReqDTCGroup    Specify the DTC group used in DEM function call
 **
 ** \retval DEM_CONTROL_DTC_SETTING_OK        EnableDTCSetting was successful
 ** \retval DEM_CONTROL_DTC_SETTING_N_OK      EnableDTCSetting not successful
 ** \retval DEM_CONTROL_DTC_WRONG_DTCGROUP    EnableDTCSetting not successful because group
 **                                           of DTC was wrong
 **/
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DemFacade_ControlDTCSetting_Enable(
  const Dcm_DemClientIdType DemClientId);

/** \brief Used to call the Dem function that disable the ControlDTCSetting
 **
 ** \param [in] ReqDTCGroup    Specify the DTC group used in DEM function call
 **
 ** \retval DEM_CONTROL_DTC_SETTING_OK        DisableDTCSetting was successful
 ** \retval DEM_CONTROL_DTC_SETTING_N_OK      DisableDTCSetting not successful
 ** \retval DEM_CONTROL_DTC_WRONG_DTCGROUP    DisableDTCSetting not successful because group
 **                                           of DTC was wrong
 **/
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DemFacade_ControlDTCSetting_Disable(
  const Dcm_DemClientIdType DemClientId);
#endif /* #if (DCM_DSP_USE_SERVICE_0X85 == STD_ON) */

#if (DCM_DSP_USE_SERVICE_0X14 == STD_ON) || (DCM_DSP_USE_SERVICE_0X04 == STD_ON)
/** \brief Setup ClearDTC processing
 **
 ** \param[in] DemClientId     This parameter defines the Dem Client instance
 ** \param[in] DTC             This parameter defines Diagnostic Trouble Code
 ** \param[in] DTCFormat       This parameter defines Format of DTC
 ** \param[in] DTCOrigin       This parameter defines Origin of DTC
 ** \param[out] NrcPtr         This parameter defines negative response code
 **
 ** \retval DCM_E_DONE         Operation completed
 **/
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DemFacade_ClearDTCInfo_Setup(
  Dcm_DemClientIdType DemClientId, uint32 DTC, uint8 DTCFormat, uint16 DTCOrigin,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) NrcPtr);
/** \brief Execute ClearDTC processing
 **
 ** \param[in] DemClientId     This parameter defines the Dem Client instance
 ** \param[out] NrcPtr         This parameter defines negative response code
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation could not be completed in the current cycle,
 **                            It shall be called again in the next cycle
 **/
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DemFacade_ClearDTCInfo_Process(
  Dcm_DemClientIdType DemClientId,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) NrcPtr);
/** \brief Cancel ClearDTC processing
 **
 ** \param[in] DemClientId     This parameter defines the Dem Client instance
 **
 ** \retval DCM_E_DONE         Operation completed
 **/
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DemFacade_ClearDTCInfo_Cancel(
  Dcm_DemClientIdType DemClientId);
#endif /* #if (DCM_DSP_USE_SERVICE_0X14 == STD_ON) || (DCM_DSP_USE_SERVICE_0X04 == STD_ON) */

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

#endif /* #if (DCM_DSP_USE_DEMFACADE == STD_ON) */

#endif /* ifndef DCM_DSP_DEMFACADE_H */
/*==================[end of file]================================================================*/
