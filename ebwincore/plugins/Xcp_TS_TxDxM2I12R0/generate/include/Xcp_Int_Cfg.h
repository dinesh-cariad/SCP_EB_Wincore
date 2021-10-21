/**
 * \file
 *
 * \brief AUTOSAR Xcp
 *
 * This file contains the implementation of the AUTOSAR
 * module Xcp.
 *
 * \version 2.12.0
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
/* This file contains the generated Xcp module internal configuration. */
[!/*The indentation of the generator code is transmitted to the output file if INDENT is
not used here.*/!]
[!INDENT "0"!]
[!INCLUDE "../../generate_macros/Xcp_Vars.m"!][!//
#ifndef XCP_INT_CFG_H
#define XCP_INT_CFG_H
[!//
[!AUTOSPACING!]
[!IF "text:tolower(XcpGeneral/XcpMasterBlockModeSupported) = 'true'"!]
[!VAR "XcpMaxOfMaxBs" = "0"!]
[!LOOP "XcpConfig/*[1]/XcpConnectionCfg/*"!]
[!IF "../../../../XcpGeneral/XcpAddressGranularity = 'BYTE'"!]
[!VAR "XcpMaxBs" = "num:integer(ceiling(255 div (XcpMaxCto - 2)))"!]
[!ELSEIF "../../../../XcpGeneral/XcpAddressGranularity = 'WORD'"!]
[!VAR "XcpMaxBs" = "num:integer(ceiling(255 div (floor((XcpMaxCto - 2) div 2 ))))"!]
[!ELSE!]
[!VAR "XcpMaxBs" = "num:integer(ceiling(255 div (floor((XcpMaxCto - 4) div 4 ))))"!]
[!ENDIF!]
[!//
[!IF "XcpMaxBS < $XcpMaxBs"!]
[!VAR "XcpMaxBs" = "XcpMaxBS"!] [!/* Allow the user to configure a smaller value than the maximum possible value which would not result in a waste of resources */!]
[!ENDIF!]
[!//
[!IF "$XcpMaxBs > $XcpMaxOfMaxBs"!]
[!VAR "XcpMaxOfMaxBs" = "$XcpMaxBs"!]
[!ENDIF!]
[!ENDLOOP!]
[!//
[!ENDIF!] [!/* XcpGeneral/XcpMasterBlockModeSupported = 'true' */!][!//
[!//
[!IF "($XcpMasterBlockModePgmSupported = 'true')"!]
[!VAR "XcpMaxOfMaxBsPgm" = "0"!]
[!LOOP "XcpConfig/*[1]/XcpConnectionCfg/*"!]
[!IF "../../../../XcpGeneral/XcpAddressGranularity = 'BYTE'"!]
[!VAR "XcpMaxBSPgmVar" = "num:integer(ceiling(255 div (XcpMaxCtoPgm - 2)))"!]
[!ELSEIF "../../../../XcpGeneral/XcpAddressGranularity = 'WORD'"!]
[!VAR "XcpMaxBSPgmVar" = "num:integer(ceiling(255 div (floor((XcpMaxCtoPgm - 2) div 2 ))))"!]
[!ELSE!]
[!VAR "XcpMaxBSPgmVar" = "num:integer(ceiling(255 div (floor((XcpMaxCtoPgm - 4) div 4 ))))"!]
[!ENDIF!]
[!//
[!IF "XcpMaxBSPgm < $XcpMaxBSPgmVar"!]
[!VAR "XcpMaxBSPgmVar" = "XcpMaxBSPgm"!] [!/* Allow the user to configure a smaller value than the maximum possible value which would not result in a waste of resources */!]
[!ENDIF!]
[!//
[!IF "$XcpMaxBSPgmVar > $XcpMaxOfMaxBsPgm"!]
[!VAR "XcpMaxOfMaxBsPgm" = "$XcpMaxBSPgmVar"!]
[!ENDIF!]
[!ENDLOOP!]
[!ENDIF!] [!/* $XcpMasterBlockModePgmSupported = 'true' */!][!//
[!//
[!IF "$XcpTimestampIsEnabled = 'true'"!]
  [!IF "XcpGeneral/XcpUserTimestampSupported = 'false'"!]
    [!IF "node:refvalid(XcpGeneral/XcpCounterRef) and node:exists(as:ref(XcpGeneral/XcpCounterRef)/OsSecondsPerTick)"!]
      [!VAR "OsSecondsPerTick"="node:refs(XcpGeneral/XcpCounterRef)/OsSecondsPerTick"!]
      [!IF "floor($OsSecondsPerTick) = $OsSecondsPerTick"!] [!/* If OsSecondsPerTick is an integer - we have a multiple of seconds */!]
        [!VAR "TimeStampTicks"="num:i(num:i(text:replaceAll(num:f($OsSecondsPerTick), '\.', '')) div 10)"!]
        [!VAR "TimeStampUnit"="'TIMESTAMP_UNIT_1S'"!]
      [!ELSE!] [!/* OsSecondsPerTick is not an integer - we must use ticks of up to 65535 and units smaller than seconds (us, ms, ns, ps) */!]
        [!VAR "TimeStampTicks"="num:i(text:replaceAll(num:f($OsSecondsPerTick), '\.', ''))"!]
        [!VAR "TimeStampUnit"="(var:set('varName', string-length(substring-after(num:f($OsSecondsPerTick), '.'))) | text:split('TIMESTAMP_UNIT_1S TIMESTAMP_UNIT_100MS TIMESTAMP_UNIT_10MS TIMESTAMP_UNIT_1MS TIMESTAMP_UNIT_100US TIMESTAMP_UNIT_10US TIMESTAMP_UNIT_1US TIMESTAMP_UNIT_100NS TIMESTAMP_UNIT_10NS TIMESTAMP_UNIT_1NS TIMESTAMP_UNIT_100PS TIMESTAMP_UNIT_10PS TIMESTAMP_UNIT_1PS')[position() = $varName + 1])[2]"!]
      [!ENDIF!]
      [!IF "$TimeStampTicks > 65535"!][!ERROR!]Calculated timestamp ticks exceeded allowed value of 65535. Please reduce the OsSecondsPerTick value from the referenced OS counter (XcpCounterRef).[!ENDERROR!][!ENDIF!]
    [!ELSE!] [!/* XcpCounterRef is not correctly configured, set default values */!]
      [!VAR "TimeStampTicks"= "1"!]
      [!VAR "TimeStampUnit"= "'0U'"!]
    [!ENDIF!] [!/* Validity check for XcpCounterRef */!][!//
  [!ELSE!] [!/* XcpGeneral/XcpUserTimestampSupported = 'true' */!]
    [!VAR "TimeStampUnit"="XcpGeneral/XcpTimestampUnit"!]
    [!VAR "TimeStampTicks"="XcpGeneral/XcpTimestampTicks"!]
  [!ENDIF!] [!/* XcpGeneral/XcpUserTimestampSupported = 'false' */!][!//
[!ELSE!] [!/* Timestamp is disabled, set default values */!]
   [!VAR "TimeStampTicks"= "1"!]
   [!VAR "TimeStampUnit"= "'0U'"!]
[!ENDIF!]
[!//
[!IF "$TimeStampUnit != '0U'"!]
[!VAR "TimeStampUnit"="concat('(uint8)(XCP_',$TimeStampUnit,'_MASK << XCP_TIMESTAMP_UNIT_POS)')"!]
[!ENDIF!]
[!//
/*==================[includes]===============================================*/

#include <Std_Types.h>      /* AUTOSAR standard types */
#include <TSAutosar.h>      /* EB specific standard types */
#include <Xcp_Cfg.h>
#include <Xcp_Int_Stc.h>    /* Internal static header file */

[!IF "$XcpMemoryAccessAreas"!][!//
#include <Rte_Type.h>
[!ENDIF!][!//

/*==================[macros]=================================================*/

#if (defined XCP_SYNCH_RECEIVED_CALLOUT_ENABLED)
#error XCP_SYNCH_RECEIVED_CALLOUT_ENABLED already defined
#endif
/** \brief Indicates the value of configuration parameter XcpSynchReceivedCallout */
#define XCP_SYNCH_RECEIVED_CALLOUT_ENABLED [!//
[!IF "text:tolower(XcpGeneral/XcpSynchReceivedCallout) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined XCP_DAQ_SUPPORTED)
#error XCP_DAQ_SUPPORTED already defined
#endif
/** \brief Switch indicating if configuration parameter XcpDaqSupported is enabled */
#define XCP_DAQ_SUPPORTED [!//
[!IF "text:tolower(XcpGeneral/XcpDaqSupported) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined XCP_STIM_SUPPORTED)
#error XCP_STIM_SUPPORTED already defined
#endif
/** \brief Switch indicating if configuration parameter XcpStimSupported is enabled */
#define XCP_STIM_SUPPORTED [!//
[!IF "text:tolower(XcpGeneral/XcpStimSupported) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined XCP_DEV_ERROR_DETECT)
#error XCP_DEV_ERROR_DETECT already defined
#endif
/** \brief Switch, indicating whether the detection of development errors is
 ** activated or deactivated for XCP */
#define XCP_DEV_ERROR_DETECT         [!//
[!IF "text:tolower(XcpGeneral/XcpDevErrorDetect) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined XCP_MAIN_FUNCTION_PERIOD)
#error XCP_MAIN_FUNCTION_PERIOD already defined
#endif
/** \brief The period between successive ticks in seconds. */
#define XCP_MAIN_FUNCTION_PERIOD     [!//
[!"XcpGeneral/XcpMainFunctionPeriod"!]

#if (defined XCP_PROD_ERR_HANDLING_RETRY_FAILED)
#error XCP_PROD_ERR_HANDLING_RETRY_FAILED already defined
#endif
/** \brief Switch for DEM to DET reporting for Xcp Retry failure */
#define XCP_PROD_ERR_HANDLING_RETRY_FAILED    [!//
[!IF "ReportToDem/XcpRetryFailedReportToDem = 'DEM'"!][!//
TS_PROD_ERR_REP_TO_DEM
[!ELSEIF "ReportToDem/XcpRetryFailedReportToDem = 'DET'"!][!//
TS_PROD_ERR_REP_TO_DET
[!ELSE!][!//
TS_PROD_ERR_DISABLE
[!ENDIF!][!//

#if (defined XCP_PROD_ERR_HANDLING_PDU_BUFFER_LOCKED)
#error XCP_PROD_ERR_HANDLING_PDU_BUFFER_LOCKED already defined
#endif
/** \brief Switch for DEM to DET reporting for PDU Buffer Locked */
#define XCP_PROD_ERR_HANDLING_PDU_BUFFER_LOCKED   [!//
[!IF "ReportToDem/XcpPduBufferLockedReportToDem = 'DEM'"!][!//
TS_PROD_ERR_REP_TO_DEM
[!ELSEIF "ReportToDem/XcpPduBufferLockedReportToDem = 'DET'"!][!//
TS_PROD_ERR_REP_TO_DET
[!ELSE!][!//
TS_PROD_ERR_DISABLE
[!ENDIF!][!//

#if (defined XCP_PROD_ERR_HANDLING_ILLEGAL_MEMORY_ACCESS)
#error XCP_PROD_ERR_HANDLING_ILLEGAL_MEMORY_ACCESS already defined
#endif
/** \brief Switch for DEM to DET reporting for Xcp Illegal Memory Access */
#define XCP_PROD_ERR_HANDLING_ILLEGAL_MEMORY_ACCESS    [!//
[!IF "ReportToDem/XcpIllegalMemoryAccessReportToDem = 'DEM'"!][!//
TS_PROD_ERR_REP_TO_DEM
[!ELSEIF "ReportToDem/XcpIllegalMemoryAccessReportToDem = 'DET'"!][!//
TS_PROD_ERR_REP_TO_DET
[!ELSE!][!//
TS_PROD_ERR_DISABLE
[!ENDIF!][!//

#if (defined XCP_PROD_ERR_HANDLING_RESP_CTO_QUEUE_FULL)
#error XCP_PROD_ERR_HANDLING_RESP_CTO_QUEUE_FULL already defined
#endif
/** \brief Switch for DEM to DET reporting for response CTO Queue full */
#define XCP_PROD_ERR_HANDLING_RESP_CTO_QUEUE_FULL    [!//
[!IF "ReportToDem/XcpRespCTOQueueFullReportToDem = 'DEM'"!][!//
TS_PROD_ERR_REP_TO_DEM
[!ELSEIF "ReportToDem/XcpRespCTOQueueFullReportToDem = 'DET'"!][!//
TS_PROD_ERR_REP_TO_DET
[!ELSE!][!//
TS_PROD_ERR_DISABLE
[!ENDIF!][!//


#if (defined XCP_PROD_ERR_HANDLING_PDU_LOST)
#error XCP_PROD_ERR_HANDLING_PDU_LOST already defined
#endif
/** \brief Switch for DEM to DET reporting for response PDU Lost */
#define XCP_PROD_ERR_HANDLING_PDU_LOST    [!//
[!IF "ReportToDem/XcpPDULostReportToDem = 'DEM'"!][!//
TS_PROD_ERR_REP_TO_DEM
[!ELSEIF "ReportToDem/XcpPDULostReportToDem = 'DET'"!][!//
TS_PROD_ERR_REP_TO_DET
[!ELSE!][!//
TS_PROD_ERR_DISABLE
[!ENDIF!][!//

#if (defined XCP_PROD_ERR_HANDLING_PDU_BUFFER_FULL)
#error XCP_PROD_ERR_HANDLING_PDU_BUFFER_FULL already defined
#endif
/** \brief Switch for DEM to DET reporting for response PDU Buffer Full */
#define XCP_PROD_ERR_HANDLING_PDU_BUFFER_FULL    [!//
[!IF "ReportToDem/XcpPDUBufferFullReportToDem = 'DEM'"!][!//
TS_PROD_ERR_REP_TO_DEM
[!ELSEIF "ReportToDem/XcpPDUBufferFullReportToDem = 'DET'"!][!//
TS_PROD_ERR_REP_TO_DET
[!ELSE!][!//
TS_PROD_ERR_DISABLE
[!ENDIF!][!//


#if (defined XCP_PROD_ERR_HANDLING_STIMULATION_DATA_LOST)
#error XCP_PROD_ERR_HANDLING_STIMULATION_DATA_LOST already defined
#endif
/** \brief Switch for DEM to DET reporting for response Stimulation Data Lost */
#define XCP_PROD_ERR_HANDLING_STIMULATION_DATA_LOST    [!//
[!IF "ReportToDem/XcpStimulationDataLostReportToDem = 'DEM'"!][!//
TS_PROD_ERR_REP_TO_DEM
[!ELSEIF "ReportToDem/XcpStimulationDataLostReportToDem = 'DET'"!][!//
TS_PROD_ERR_REP_TO_DET
[!ELSE!][!//
TS_PROD_ERR_DISABLE
[!ENDIF!][!//

#if (defined XCP_PROD_ERR_HANDLING_ACQUISITION_DATA_LOST)
#error XCP_PROD_ERR_HANDLING_ACQUISITION_DATA_LOST already defined
#endif
/** \brief Switch for DEM to DET reporting for response Acquisition Data Lost */
#define XCP_PROD_ERR_HANDLING_ACQUISITION_DATA_LOST    [!//
[!IF "ReportToDem/XcpAcquisitionDataLostReportToDem = 'DEM'"!][!//
TS_PROD_ERR_REP_TO_DEM
[!ELSEIF "ReportToDem/XcpAcquisitionDataLostReportToDem = 'DET'"!][!//
TS_PROD_ERR_REP_TO_DET
[!ELSE!][!//
TS_PROD_ERR_DISABLE
[!ENDIF!][!//


#if (defined XCP_MEMORY_AREAS_ACCESS_SUPPORTED)
#error XCP_MEMORY_AREAS_ACCESS_SUPPORTED already defined
#endif
/** \brief Switch, indicating whether memory areas support is enabled or not  */
#define XCP_MEMORY_AREAS_ACCESS_SUPPORTED         [!//
[!IF "text:tolower(XcpConfig/*[1]/XcpMemoryAccessArea/XcpMemoryAccessAreasSupport) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


[!IF "node:exists(XcpGeneral/XcpIdentificationFieldType)"!]
#if (defined XCP_IDENTIFICATION_FIELD_TYPE)
#error XCP_IDENTIFICATION_FIELD_TYPE already defined
#endif
/** \brief Definition of value indicating the type of Packet Identification Field the slave
 ** will use while transferring DAQ Packets to the master.
 **/
#define XCP_IDENTIFICATION_FIELD_TYPE [!//
XCP_[!"XcpGeneral/XcpIdentificationFieldType"!]_IF_MASK
[!ENDIF!]


#if (defined XCP_GET_ID_TYPE1_CALLOUT_ENABLED)
#error XCP_GET_ID_TYPE1_CALLOUT_ENABLED already defined
#endif
/** \brief Switch, indicating whether the GET_ID command for type 1 uses informations from
 *  the configuration or gets the information from an user defined callout
 */
#define XCP_GET_ID_TYPE1_CALLOUT_ENABLED          [!//
[!IF "node:exists(XcpGeneral/XcpGetIdType1Callout) and (XcpGeneral/XcpGetIdType1Callout = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!IF "node:exists(XcpGeneral/XcpGetIdType1Callout) and (XcpGeneral/XcpGetIdType1Callout = 'true')"!]
#if (defined XCP_GET_ID_TYPE1_MAX_LENGTH)
#error XCP_GET_ID_TYPE1_MAX_LENGTH already defined
#endif
/** \brief Definition of the maximum Length of the identification information
 */
#define XCP_GET_ID_TYPE1_MAX_LENGTH  [!"num:i(XcpGeneral/XcpGetIdType1MaxLength)"!]U

[!ELSE!][!//
#if (defined XCP_GET_ID_TYPE1_ASAM_FILENAME_LENGTH)
#error XCP_GET_ID_TYPE1_ASAM_FILENAME_LENGTH already defined
#endif
/** \brief Definition of Length of the identification information TYPE 1
 * This is the length of ASCII value of ASAM-MC2 filename without path
 * and extension.
 */
[!IF "node:exists(XcpGeneral/XcpASAMMC2Filename)"!][!//
#define XCP_GET_ID_TYPE1_ASAM_FILENAME_LENGTH  [!"num:i(string-length(normalize-space(XcpGeneral/XcpASAMMC2Filename)))"!]U
[!ELSE!][!//
#define XCP_GET_ID_TYPE1_ASAM_FILENAME_LENGTH  0x00U
[!ENDIF!][!//
[!ENDIF!][!//

[!IF "((node:exists(XcpGeneral/XcpGetIdType1Callout) = 'false') or (XcpGeneral/XcpGetIdType1Callout = 'false')) and node:exists(XcpGeneral/XcpASAMMC2Filename)"!][!//
#if (defined XCP_GET_ID_TYPE1_ASAM_FILENAME)
#error XCP_GET_ID_TYPE1_ASAM_FILENAME already defined
#endif
/** \brief Definition of the value indicating Identification information TYPE 1(One).
 * Type 1(One) identification information is ASAM-MC2 filename without path
 * and extension
 */
#define XCP_GET_ID_TYPE1_ASAM_FILENAME  "[!"normalize-space(XcpGeneral/XcpASAMMC2Filename)"!]"
[!ENDIF!][!//

#if (defined XCP_MAX_CTO)
#error XCP_MAX_CTO already defined
#endif
/** \brief The maximum XcpMaxCto value of all configured connections */
#define XCP_MAX_CTO [!"num:max(XcpConfig/*[1]/XcpConnectionCfg/*/XcpMaxCto)"!]U

#if (defined XCP_MIN_OF_ALL_MAX_CTO)
#error XCP_MIN_OF_ALL_MAX_CTO already defined
#endif
/** \brief The minimum XcpMaxCto value of all configured connections */
#define XCP_MIN_OF_ALL_MAX_CTO [!"num:min(XcpConfig/*[1]/XcpConnectionCfg/*/XcpMaxCto)"!]U

#if (defined XCP_MAX_CTO_PGM)
#error XCP_MAX_CTO_PGM already defined
#endif
/** \brief The maximum length of XCP command transfer objects (CTO) packet in bytes during a flash
 ** programming sequence */
[!IF "text:tolower(XcpGeneral/XcpPgmSupported) = 'true'"!]
#define XCP_MAX_CTO_PGM             [!"num:max(XcpConfig/*[1]/XcpConnectionCfg/*/XcpMaxCtoPgm)"!]U
[!ELSE!]
#define XCP_MAX_CTO_PGM             0U
[!ENDIF!]

[!IF "text:tolower(XcpGeneral/XcpDaqSupported) = 'true'"!]
#if (defined XCP_DAQ_CONFIG_TYPE)
#error XCP_DAQ_CONFIG_TYPE already defined
#endif
/** \brief Switch, indicating whether the DAQ lists that are not PREDEFINED shall be configured
 ** statically or dynamically.
 **/
#define XCP_DAQ_CONFIG_TYPE          [!//
XCP_[!"$XcpDaqConfigType"!]_MASK
[!ENDIF!]

#if (defined XCP_MIN_DAQ)
#error XCP_MIN_DAQ already defined
#endif
/** \brief The number of predefined, read only DAQ lists on the XCP slave. */
#define XCP_MIN_DAQ                  [!//
[!"num:integer($XcpMinDaq)"!]U

#if (defined XCP_DAQ_COUNT)
#error XCP_DAQ_COUNT already defined
#endif
/** \brief The maximum number of configurable dynamic DAQ lists */
#define XCP_DAQ_COUNT                  [!//
[!"num:integer($XcpDaqCount)"!]U

#if (defined XCP_ODT_COUNT)
#error XCP_ODT_COUNT already defined
#endif
/** \brief Maximum number of ODTs allowed per dynamic DAQ list. */
#define XCP_ODT_COUNT                  [!//
[!"num:integer($XcpOdtCount)"!]U


#if (defined XCP_MAX_MF_PROCESSED_EVENTS)
#error XCP_MAX_MF_PROCESSED_EVENTS already defined
#endif
/** \brief The number of event channels processed with the XCP mainFunction. */
#define XCP_MAX_MF_PROCESSED_EVENTS        [!//
[!IF "XcpGeneral/XcpEventTriggerMainFunc = 'false'"!]
[!"num:i(count(XcpConfig/*[1]/XcpEventChannel/*))"!]U
[!ELSE!] [!//
[!"num:i(count(XcpConfig/*[1]/XcpEventChannel/*[XcpEventChannelTimeCycle = 0]))"!]U
[!ENDIF!]

#if (defined XCP_MAX_EVENT_CHANNEL)
#error XCP_MAX_EVENT_CHANNEL already defined
#endif
/** \brief The number of event channels on the XCP slave. */
#define XCP_MAX_EVENT_CHANNEL        [!//
[!"num:integer($XcpMaxEventChannel)"!]U

#if (defined XCP_MAX_CYCLIC_EVENT_CHANNEL)
#error XCP_MAX_CYCLIC_EVENT_CHANNEL already defined
#endif
/** \brief The number of cyclic event channels on the XCP slave. */
#define XCP_MAX_CYCLIC_EVENT_CHANNEL [!//
[!"num:i($CyclicEventCount)"!]U

#if (defined XCP_PRESCALER_SUPPORTED)
#error XCP_PRESCALER_SUPPORTED already defined
#endif
/** \brief Switch, indicating whether all DAQ lists support the prescaler for reducing the
 ** transmission period. */
#define XCP_PRESCALER_SUPPORTED      [!//
[!IF "(XcpGeneral/XcpDaqSupported = 'true') and (text:tolower(XcpGeneral/XcpPrescalerSupported) = 'true')"!]XCP_PRESCALER_MASK[!ELSE!](0U)[!ENDIF!]

#if (defined XCP_TIMESTAMP_SUPPORTED)
#error XCP_TIMESTAMP_SUPPORTED already defined
#endif
/** \brief Switch, indicating whether the slave supports time stamped data acquisition and
 ** stimulation. */
#define XCP_TIMESTAMP_SUPPORTED      [!//
[!IF "$XcpTimestampIsEnabled = 'true'"!]XCP_TIMESTAMP_MASK[!ELSE!](0U)[!ENDIF!]


#if (defined XCP_TIMESTAMP_TICKS)
#error XCP_TIMESTAMP_TICKS already defined
#endif
/** \brief Definition of value indicating Timestamp units per tick. */
#define XCP_TIMESTAMP_TICKS          [!"num:integer($TimeStampTicks)"!]U

#if (defined XCP_TIMESTAMP_TYPE)
#error XCP_TIMESTAMP_TYPE already defined
#endif
/** \brief The timestamp type (the number of bytes that the timestamp will have)*/
#define XCP_TIMESTAMP_TYPE           [!//
[!IF "$XcpTimestampIsEnabled = 'true'"!]XCP_[!"XcpGeneral/XcpTimestampType"!]_TS_MASK[!ELSE!]XCP_NO_TIME_STAMP_TS_MASK[!ENDIF!]

#if (defined XCP_TIMESTAMP_UNIT)
#error XCP_TIMESTAMP_UNIT already defined
#endif
/** \brief Definition of value indicating the unit of the time stamp. */
#define XCP_TIMESTAMP_UNIT           [!"$TimeStampUnit"!]

[!IF "$XcpTimestampIsEnabled = 'true'"!]
  [!IF "XcpGeneral/XcpUserTimestampSupported = 'false'"!]
#if (defined XCP_OS_COUNTER)
#error XCP_OS_COUNTER already defined
#endif
/** \brief OS Counter which is used by XCP. */
#define XCP_OS_COUNTER               [!//
[!"name(as:ref(XcpGeneral/XcpCounterRef))"!]
  [!ENDIF!]
[!ENDIF!]

[!IF "text:tolower($StoreDaq)  = 'true'"!]
[!VAR "BlockIdRef" = "as:ref(./XcpGeneral/XcpStoreDaqNvMBlock)/NvMNvramBlockIdentifier"!]
[!VAR "BlockIdLength" = "as:ref(./XcpGeneral/XcpStoreDaqNvMBlock)/NvMNvBlockLength "!]

#if (defined XCP_STORE_DAQ_NVM_BLOCK_ID)
#error XCP_STORE_DAQ_NVM_BLOCK_ID already defined
#endif
/** \brief The NvM block ID used to store the DAQ lists
 **
 **/
#define XCP_STORE_DAQ_NVM_BLOCK_ID   [!"$BlockIdRef"!]U

#if (defined XCP_STORE_DAQ_NVM_BLOCK_LENGTH)
#error XCP_STORE_DAQ_NVM_BLOCK_LENGTH already defined
#endif
/** \brief The length of the NvM block used to store the DAQ lists
 **
 **/
#define XCP_STORE_DAQ_NVM_BLOCK_LENGTH   [!"$BlockIdLength"!]U
[!ENDIF!]

#if (defined XCP_RESOURCE_CAL_PAG)
#error XCP_RESOURCE_CAL_PAG already defined
#endif
/** \brief Switch, indicating whether calibration/paging functionality is available. */
#define XCP_RESOURCE_CAL_PAG         [!//
[!IF "text:tolower(XcpGeneral/XcpCalPagSupported) = 'true'"!]XCP_RESOURCE_CAL_PAG_MASK[!ELSE!](0U)[!ENDIF!]

#if (defined XCP_RESOURCE_DAQ)
#error XCP_RESOURCE_DAQ already defined
#endif
/** \brief Switch, indicating whether synchronous data acquisition functionality is available. */
#define XCP_RESOURCE_DAQ             [!//
[!IF "text:tolower(XcpGeneral/XcpDaqSupported) = 'true'"!]XCP_RESOURCE_DAQ_MASK[!ELSE!](0U)[!ENDIF!]

#if (defined XCP_RESOURCE_STIM)
#error XCP_RESOURCE_STIM already defined
#endif
/** \brief Switch, indicating whether synchronous data stimulation functionality is available. */
#define XCP_RESOURCE_STIM            [!//
[!IF "node:exists(XcpGeneral/XcpStimSupported) and text:tolower(XcpGeneral/XcpStimSupported) = 'true'"!]XCP_RESOURCE_STIM_MASK[!ELSE!](0U)[!ENDIF!]

#if (defined XCP_RESOURCE_PGM)
#error XCP_RESOURCE_PGM already defined
#endif
/** \brief Value of the PGM resource as needed by the protection status (see GET_STATUS command) */
#define XCP_RESOURCE_PGM             [!//
[!IF "text:tolower(XcpGeneral/XcpPgmSupported) = 'true'"!]XCP_RESOURCE_PGM_MASK[!ELSE!](0U)[!ENDIF!]

#if (defined XCP_PGM_SUPPORTED)
#error XCP_PGM_SUPPORTED already defined
#endif
/** \brief Switch, indicating whether programming functionality is available.*/
#define XCP_PGM_SUPPORTED             [!//
STD_[!IF "text:tolower(XcpGeneral/XcpPgmSupported) = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]

#if (defined XCP_ADDRESS_GRANULARITY)
#error XCP_ADDRESS_GRANULARITY already defined
#endif
/** \brief Bit values related to the address granularity of the
 ** COMM_MODE_BASIC parameter in CONNECT command parameter.
 **/
#define XCP_ADDRESS_GRANULARITY      [!//
XCP_BYTE_AG_MASK

#if (defined XCP_ADDRESS_GRANULARITY_SIZE)
#error XCP_ADDRESS_GRANULARITY_SIZE already defined
#endif
/** \brief The size of an element contained at a single address in the slave. */
#define XCP_ADDRESS_GRANULARITY_SIZE [!//
XCP_BYTE_AG_SIZE

#if (defined XCP_MASTER_BLOCK_MODE)
#error XCP_MASTER_BLOCK_MODE already defined
#endif
/** \brief Switch, indicating whether master block mode functionality is available. */
#define XCP_MASTER_BLOCK_MODE        [!//
[!IF "text:tolower(XcpGeneral/XcpMasterBlockModeSupported) = 'true'"!]XCP_MASTER_BLOCK_MODE_MASK[!ELSE!](0U)[!ENDIF!]

#if (defined XCP_MASTER_BLOCK_MODE_SUPPORTED)
#error XCP_MASTER_BLOCK_MODE_SUPPORTED already defined
#endif
/** \brief Switch, indicating whether master block mode functionality is available. */
#define XCP_MASTER_BLOCK_MODE_SUPPORTED        [!//
[!IF "text:tolower(XcpGeneral/XcpMasterBlockModeSupported) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined XCP_MAX_BS)
#error XCP_MAX_BS already defined
#endif
/** \brief Definition of the maximum number of consecutive command packets in a block sequence.
 **  The value represents the initial DOWNLOAD command plus the consecutive DOWNLOAD_NEXT commands.
 **/
#define XCP_MAX_BS                   [!//
[!IF "text:tolower(XcpGeneral/XcpMasterBlockModeSupported) = 'true'"!][!"$XcpMaxOfMaxBs"!]U[!ELSE!](0U)[!ENDIF!]

#if (defined XCP_MIN_ST)
#error XCP_MIN_ST already defined
#endif
/** \brief The required minimum separation time between the packets of a block
 ** transfer from the master device to the slave device in units of 100 microseconds. */
#define XCP_MIN_ST                   [!//
[!IF "text:tolower(XcpGeneral/XcpMasterBlockModeSupported) = 'true'"!][!"XcpGeneral/XcpMinST"!]U[!ELSE!](0U)[!ENDIF!]

#if (defined XCP_MASTER_BLOCK_MODE_PGM)
#error XCP_MASTER_BLOCK_MODE_PGM already defined
#endif
/** \brief Switch, indicating whether master block mode for programming is available. */
#define XCP_MASTER_BLOCK_MODE_PGM        [!//
[!IF "($XcpMasterBlockModePgmSupported = 'true')"!]XCP_MASTER_BLOCK_MODE_MASK[!ELSE!](0U)[!ENDIF!]

#if (defined XCP_MASTER_BLOCK_MODE_PGM_SUPPORTED)
#error XCP_MASTER_BLOCK_MODE_PGM_SUPPORTED already defined
#endif
/** \brief Switch, indicating whether master block mode for programming is available. */
#define XCP_MASTER_BLOCK_MODE_PGM_SUPPORTED        [!//
[!IF "($XcpMasterBlockModePgmSupported = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined XCP_MAX_BS_PGM)
#error XCP_MAX_BS_PGM already defined
#endif
/** \brief Definition of the maximum number of consecutive command packets in a block sequence
 **  for flash programming.
 **  The value represents the initial PROGRAM command plus the consecutive PROGRAM_NEXT commands.
 **/
#define XCP_MAX_BS_PGM                   [!//
[!IF "($XcpMasterBlockModePgmSupported = 'true')"!][!"$XcpMaxOfMaxBsPgm"!]U[!ELSE!](0U)[!ENDIF!]

#if (defined XCP_MIN_ST_PGM)
#error XCP_MIN_ST_PGM already defined
#endif
/** \brief The required minimum separation time, for flash programming, between the packets of a block
 ** transfer from the master device to the slave device in units of 100 microseconds. */
#define XCP_MIN_ST_PGM                   [!//
[!IF "($XcpMasterBlockModePgmSupported = 'true')"!][!"XcpGeneral/XcpMinSTPgm"!]U[!ELSE!](0U)[!ENDIF!]

#if (defined XCP_TIMESTAMP_FIXED)
#error XCP_TIMESTAMP_FIXED already defined
#endif
/** \brief Switch, indicating whether time stamp fixed functionality is available. */
[!IF "(node:exists(XcpGeneral/XcpTimestampFixed) = 'true')"!]
#define XCP_TIMESTAMP_FIXED          [!//
[!IF "text:tolower(XcpGeneral/XcpTimestampFixed) = 'true'"!]XCP_TIMESTAMP_FIXED_MASK[!ELSE!](0U)[!ENDIF!]
[!ELSE!]
#define XCP_TIMESTAMP_FIXED    0U
[!ENDIF!]

#if (defined XCP_GRANULARITY_ODT_ENTRY_SIZE_DAQ)
#error XCP_GRANULARITY_ODT_ENTRY_SIZE_DAQ already defined
#endif
/** \brief Definition of value indicating granularity for size of ODT entry (DIRECTION = DAQ). */
#define XCP_GRANULARITY_ODT_ENTRY_SIZE_DAQ [!//
XCP_[!"XcpGeneral/XcpGranularityOdtEntrySizeDaq"!]_AG_SIZE

#if (defined XCP_GRANULARITY_ODT_ENTRY_SIZE_STIM)
#error XCP_GRANULARITY_ODT_ENTRY_SIZE_STIM already defined
#endif
/** \brief Definition of value indicating granularity for size of ODT entry (DIRECTION = STIM). */
#define XCP_GRANULARITY_ODT_ENTRY_SIZE_STIM [!//
XCP_[!"XcpGeneral/XcpGranularityOdtEntrySizeStim"!]_AG_SIZE

[!VAR "XcpMaxEntriesPerOdt" = "num:i(0)"!]
  [!LOOP "node:order(XcpConfig/*[1]/XcpDaqList/*, 'XcpDaqListNumber')"!]
    [!IF "(XcpDaqListNumber < ../../../../XcpGeneral/XcpMinDaq)"!]
      [!VAR "ODTEntryCnt" = "num:i(0)"!]
      [!LOOP "node:order(XcpOdt/*, 'XcpOdtNumber')"!]
        [!VAR "ODTEntryCnt" = "num:i(count(XcpOdtEntry/*))"!]
        [!IF "$XcpMaxEntriesPerOdt < $ODTEntryCnt"!]
          [!VAR "XcpMaxEntriesPerOdt" = "$ODTEntryCnt"!]
        [!ENDIF!]
      [!ENDLOOP!]
    [!ENDIF!]
  [!ENDLOOP!]
[!IF "XcpGeneral/XcpDaqConfigType = 'DAQ_STATIC'"!]
  [!LOOP "node:order(XcpConfig/*[1]/XcpDaqList/*, 'XcpDaqListNumber')"!]
   [!IF "XcpDaqListNumber >= ../../../../XcpGeneral/XcpMinDaq"!]
      [!IF "$XcpMaxEntriesPerOdt < ./XcpMaxOdtEntries"!]
        [!VAR "XcpMaxEntriesPerOdt" = "num:i(./XcpMaxOdtEntries)"!]
      [!ENDIF!]
    [!ENDIF!]
  [!ENDLOOP!]
[!ELSEIF "XcpGeneral/XcpDaqConfigType = 'DAQ_DYNAMIC'"!]
  [!IF "$XcpMaxEntriesPerOdt < num:i(XcpGeneral/XcpOdtEntriesCount)"!]
    [!VAR "XcpMaxEntriesPerOdt" = "num:i(XcpGeneral/XcpOdtEntriesCount)"!]
  [!ENDIF!]
[!ELSE!] [!// nothing to do. the daqList resource is not configured
[!ENDIF!]
#if (defined XCP_MAX_ENTRIES_PER_ODT)
#error XCP_MAX_ENTRIES_PER_ODT already defined
#endif
/** \brief The absolute maximum number of ODT Entries in an ODT
 **/
#define XCP_MAX_ENTRIES_PER_ODT   [!" num:i($XcpMaxEntriesPerOdt)"!]U

#if (defined XCP_OVERLOAD_INDICATION_TYPE)
#error XCP_OVERLOAD_INDICATION_TYPE already defined
#endif
/** \brief Definition of value indicating the method used to indicate overload situation to
 ** master. */
#define XCP_OVERLOAD_INDICATION_TYPE [!//
[!IF "text:tolower(XcpGeneral/XcpSupportForOverloadMSB) = 'true'"!]XCP_PID_OL_MASK[!//
[!ELSEIF "text:tolower(XcpGeneral/XcpEventPacketEnabled) = 'true'"!]XCP_EVENT_OL_MASK[!//
[!ELSE!]XCP_NO_OVERLOAD_INDICATION_OL_MASK[!ENDIF!]

#if (defined XCP_OPTIMISATION_TYPE)
#error XCP_OPTIMISATION_TYPE already defined
#endif
/** \brief Definition of value indicating the type of optimisation method the master preferably
 ** should use. */
#define XCP_OPTIMISATION_TYPE        [!//
XCP_DEFAULT_OM_MASK

#if (defined XCP_ADDRESS_EXTENSION_TYPE)
#error XCP_ADDRESS_EXTENSION_TYPE already defined
#endif
/** \brief Switch, indicating whether the address extension
 ** of all entries within one ODT or within one DAQ must be the same. */
#define XCP_ADDRESS_EXTENSION_TYPE   [!//
XCP_FREE_AE_MASK

#if (defined XCP_MAXIMUM_ODT)
#error XCP_MAXIMUM_ODT already defined
#endif
/** \brief The number of ODTs allowed for a DAQ.
 **
 ** \note Macro is introduced to have limit on number of ODTs allowed
 ** for a DAQ list.
 **/
[!IF "XcpGeneral/XcpDaqSupported = 'true'"!]
#define XCP_MAXIMUM_ODT             [!//
  [!VAR "Max" = "0"!]
  [!VAR "MaxOdtPerDaqList" = "0"!]
  [!LOOP "XcpConfig/*[1]/XcpDaqList/*"!]
     [!IF "XcpDaqListNumber < $XcpMinDaq"!]
      [!VAR "MaxOdtPerDaqList" = "count(./XcpOdt/*)"!]
      [!IF "$Max < $MaxOdtPerDaqList"!]
        [!VAR "Max" = "$MaxOdtPerDaqList"!]
      [!ENDIF!]
    [!ELSE!]
      [!IF "$Max < XcpMaxOdt"!]
        [!VAR "Max" = "XcpMaxOdt"!]
      [!ENDIF!]
    [!ENDIF!]
  [!ENDLOOP!]
  [!IF "$XcpDaqConfigType = 'DAQ_DYNAMIC'"!]
    [!IF "$Max < $XcpOdtCount"!]
      [!VAR "Max" = "$XcpOdtCount"!]
    [!ENDIF!]
  [!ENDIF!]
  [!"num:i($Max)"!]U
[!ELSE!] [!/* DAQ List support is disabled */!]
#define XCP_MAXIMUM_ODT              0U
[!ENDIF!]

#if (defined XCP_TX_RETRY_COUNT)
#error XCP_TX_RETRY_COUNT already defined
#endif
/** \brief The number of times the data will be retried for transmission.
 ** This retry mechanism is meant for the case when <TP>If_Transmit() rejects
 ** our transmission request.
 **
 ** \note Macro is introduced to have limit on number of transmission retries
 ** when transmission fails.
 **
 **/
#define XCP_TX_RETRY_COUNT           [!//
[!"XcpGeneral/XcpTxRetryCount"!]U


#if (defined XCP_TX_BUS_RETRY_COUNT)
#error XCP_TX_BUS_RETRY_COUNT already defined
#endif
/** \brief The number of times the PDU will be retried for transmission.
 ** Whenever we are waiting for the Tx Confirmation, if the bus timeout mechanism
 ** expires, we retry the entire transmission process for the configured amount of times.
 **
 ** \note Not to be confused with XCP_TX_RETRY_COUNT which retries only the <TP>If_Transmit() requests
 ** when they are rejected.
 **/
#define XCP_TX_BUS_RETRY_COUNT           [!//
[!"XcpGeneral/XcpTxBusRetry"!]U

#if (defined XCP_TX_BUS_TO_COUNTER_MAX_VAL)
#error XCP_TX_BUS_TO_COUNTER_MAX_VAL already defined
#endif
/** \brief The number of Xcp_MainFunction calls until the XCP slave is autonomously DISCONNECTED.
 **
 ** Macro is introduced to have the Bus Timeout limit in case the XCP TX processor remains
 ** stuck in one of the following states: TX Trigger/ TX Confirmation, after the pending message
 ** transmission was accepted by the underlying communication.
 **
 **/
#define XCP_TX_BUS_TO_COUNTER_MAX_VAL [!//
[!"num:i(round(num:f(num:div(XcpGeneral/XcpTxBusTimeout, XcpGeneral/XcpMainFunctionPeriod))))"!]U

#if (defined XCP_FLEXRAY_SYNC_TIMEOUT_SUPPORT)
#error XCP_FLEXRAY_SYNC_TIMEOUT_SUPPORT already defined
#endif
/** \brief Switch, indicating whether FlexRay hardware buffers timeout is enabled.
 **/
#define XCP_FLEXRAY_SYNC_TIMEOUT_SUPPORT [!//
STD_[!IF "node:exists(XcpGeneral/XcpFlexRaySyncTimeout) = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]

[!IF "node:exists(XcpGeneral/XcpFlexRaySyncTimeout) = 'true'"!]
#if (defined XCP_FLEXRAY_SYNC_TO_COUNTER_MAX_VAL)
#error XCP_FLEXRAY_SYNC_TO_COUNTER_MAX_VAL already defined
#endif
/** \brief The number of Xcp_MainFunction calls until the XCP slave autonomously disconnects itself
 ** in case a FlexRay hardware buffer initialization error has occured
 **/
#define XCP_FLEXRAY_SYNC_TO_COUNTER_MAX_VAL [!//
[!"$XcpFlexRaySyncTimeoutCycles"!]U
[!ENDIF!]

#if (defined XCP_NO_DAQ_DTOS)
#error XCP_NO_DAQ_DTOS already defined
#endif
/** \brief Value indicating the number of all DTOs
 **/
#define XCP_NO_DAQ_DTOS              [!"num:integer($XcpTotalNumberOfOdts)"!]U

#if (defined XCP_MAX_ODT_IN_DAQ)
#error XCP_MAX_ODT_IN_DAQ already defined
#endif
/** \brief Value indicating the number of maximum ODT in a DAQ list
 **/
#define XCP_MAX_ODT_IN_DAQ           [!"num:integer($MaxOdtInList)"!]U


[!VAR "XcpEventChannelMFEnabled"="(XcpGeneral/XcpEventTriggerMainFunc = 'true') and (node:exists(XcpConfig/*[1]/XcpEventChannel)) and (num:i(count(XcpConfig/*[1]/XcpEventChannel/*)) > 0)"!]
[!VAR "NumberOfEventMainFunctions" = "0"!]
[!IF "$XcpEventChannelMFEnabled"!]
  [!LOOP "XcpConfig/*[1]/XcpEventChannel/*"!]
    [!IF "XcpEventChannelTimeCycle > 0"!]
      [!VAR "NumberOfEventMainFunctions" = "$NumberOfEventMainFunctions + 1"!]
    [!ENDIF!]
  [!ENDLOOP!]
[!ENDIF!]
#if (defined XCP_NUMBER_OF_EVENT_MAIN_FUNCTIONS)
#error XCP_NUMBER_OF_EVENT_MAIN_FUNCTIONS already defined
#endif
/** \brief Value indicating the number of event main functions
 **/
#define XCP_NUMBER_OF_EVENT_MAIN_FUNCTIONS           [!"num:integer($NumberOfEventMainFunctions)"!]U


#if (defined XCP_SET_MTA_ENDIANNESS)
#error XCP_SET_MTA_ENDIANNESS already defined
#endif
/** \brief Switch, indicating whether SET_MTA address is processed considering the platform endianes. */
#define XCP_SET_MTA_ENDIANNESS        [!//
[!IF "XcpGeneral/XcpSetMtaEndianness = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/*------------------[PDU configuration options]------------------------------*/

[!IF "(XcpGeneral/XcpOnCanEnabled) or (XcpGeneral/XcpOnCanFDEnabled)"!]
#if (defined XCP_NO_OF_CANIF_RX_PDU_IDS)
#error XCP_NO_OF_CANIF_RX_PDU_IDS already defined
#endif
/** \brief Macro defining the maximum number of configured CanIf Rx Pdus **/
#define XCP_NO_OF_CANIF_RX_PDU_IDS                         [!"num:i($NoOfCanIfRxPdus)"!]U

#if (defined XCP_NO_OF_CANIF_TX_PDU_IDS)
#error XCP_NO_OF_CANIF_TX_PDU_IDS already defined
#endif
/** \brief Macro defining the maximum number of configured CanIf Tx Pdus **/
#define XCP_NO_OF_CANIF_TX_PDU_IDS                         [!"num:i($NoOfCanIfTxPdus)"!]U
[!ENDIF!]

[!IF "XcpGeneral/XcpOnFlexRayEnabled"!]
#if (defined XCP_NO_OF_FRIF_RX_PDU_IDS)
#error XCP_NO_OF_FRIF_RX_PDU_IDS already defined
#endif
/** \brief Macro defining the maximum number of configured FrIf Rx Pdus **/
#define XCP_NO_OF_FRIF_RX_PDU_IDS                          [!"num:i($NoOfFrIfRxPdus)"!]U

#if (defined XCP_NO_OF_FRIF_TX_PDU_IDS)
#error XCP_NO_OF_FRIF_TX_PDU_IDS already defined
#endif
/** \brief Macro defining the maximum number of configured FrIf Tx Pdus **/
#define XCP_NO_OF_FRIF_TX_PDU_IDS                          [!"num:i($NoOfFrIfTxPdus)"!]U
[!ENDIF!]

[!IF "XcpGeneral/XcpOnEthernetEnabled"!]
#if (defined XCP_NO_OF_SOAD_RX_PDU_IDS)
#error XCP_NO_OF_SOAD_RX_PDU_IDS already defined
#endif
/** \brief Macro defining the maximum number of configured SoAd Rx Pdus **/
#define XCP_NO_OF_SOAD_RX_PDU_IDS                          [!"num:i($NoOfSoAdRxPdus)"!]U

#if (defined XCP_NO_OF_SOAD_TX_PDU_IDS)
#error XCP_NO_OF_SOAD_TX_PDU_IDS already defined
#endif
/** \brief Macro defining the maximum number of configured SoAd Tx Pdus **/
#define XCP_NO_OF_SOAD_TX_PDU_IDS                          [!"num:i($NoOfSoAdTxPdus)"!]U
[!ENDIF!]

[!IF "XcpGeneral/XcpOnCddEnabled"!]
#if (defined XCP_NO_OF_CDD_RX_PDU_IDS)
#error XCP_NO_OF_CDD_RX_PDU_IDS already defined
#endif
/** \brief Macro defining the maximum number of configured CDD Rx Pdus **/
#define XCP_NO_OF_CDD_RX_PDU_IDS                         [!"num:i($NoOfCddRxPdus)"!]U

#if (defined XCP_NO_OF_CDD_TX_PDU_IDS)
#error XCP_NO_OF_CDD_TX_PDU_IDS already defined
#endif
/** \brief Macro defining the maximum number of configured CDD Tx Pdus **/
#define XCP_NO_OF_CDD_TX_PDU_IDS                         [!"num:i($NoOfCddTxPdus)"!]U
[!ENDIF!]

#if (defined XCP_NO_OF_RX_PDU_IDS)
#error XCP_NO_OF_RX_PDU_IDS already defined
#endif
/** \brief Macro defining the maximum number of configured Rx Pdus **/
#define XCP_NO_OF_RX_PDU_IDS                               [!"num:i(count(XcpConfig/*[1]/XcpPdu/*[@name='XcpRxPdu']))"!]U

#if (defined XCP_NO_OF_TX_PDU_IDS)
#error XCP_NO_OF_TX_PDU_IDS already defined
#endif
 /** \brief Macro defining the maximum number of configured Tx Pdus **/
#define XCP_NO_OF_TX_PDU_IDS                               [!"num:i(count(XcpConfig/*[1]/XcpPdu/*[@name='XcpTxPdu']))"!]U

#if (defined XCP_TX_BUFFER_SIZE)
#error XCP_TX_BUFFER_SIZE already defined
#endif
/** \brief Macro defining the size of transmit buffer */
#define XCP_TX_BUFFER_SIZE                                 [!"num:i($TxPduBufferSize)"!]U

#if (defined XCP_RX_BUFFER_SIZE)
#error XCP_RX_BUFFER_SIZE already defined
#endif
/** \brief Macro defining the size of receive buffer */
#define XCP_RX_BUFFER_SIZE                                 [!"num:i($RxPduBufferSize)"!]U

#if (defined XCP_MAX_TX_PDU_PER_CON)
#error XCP_MAX_TX_PDU_PER_CON already defined
#endif
/** \brief Macro defining the maximum tx pdus per Xcp connection */
#define XCP_MAX_TX_PDU_PER_CON                             [!"num:i($MaxNoOfTxPdu)"!]U

#if (defined XCP_MAX_RX_PDU_PER_CON)
#error XCP_MAX_RX_PDU_PER_CON already defined
#endif
/** \brief Macro defining the maximum Rx pdus per Xcp connection */
#define XCP_MAX_RX_PDU_PER_CON                             [!"num:i($MaxNoOfRxPdu)"!]U

#if (defined XCP_NO_OF_CONNECTIONS)
#error XCP_NO_OF_CONNECTIONS already defined
#endif
/** \brief Macro defining the maximum number of configured Xcp connections */
#define XCP_NO_OF_CONNECTIONS                              [!"num:i($NoOfConnections)"!]U


#if (defined XCP_NO_OF_FLEXRAY_CONN_WITH_SEQ_CORR)
#error XCP_NO_OF_FLEXRAY_CONN_WITH_SEQ_CORR already defined
#endif
/** \brief Macro defining the number of configured Xcp connections over the FlexRay that have
 ** Sequence correction enabled. */
#define XCP_NO_OF_FLEXRAY_CONN_WITH_SEQ_CORR               [!"num:i($NoOfXcpFlexRayCorrCon)"!]U


#if (defined XCP_NO_OF_ETHERNET_CONN)
#error XCP_NO_OF_ETHERNET_CONN already defined
#endif
/** \brief Macro defining the number of configured Xcp connections over the Ethernet. */
#define XCP_NO_OF_ETHERNET_CONN                            [!"num:i($NoOfXcpEthCon)"!]U

[!IF "text:tolower(XcpGeneral/XcpOnEthernetEnabled)  = 'true'"!]
  [!IF "$SuportForSoAdSocketProtocolTcp = 'true'"!]
#if (defined XCP_NO_OF_TCPIP_SOCKET_CON)
#error XCP_NO_OF_TCPIP_SOCKET_CON already defined
#endif
/** \brief Macro defining the maximum number of configured TCPIP socket connections. */
#define XCP_NO_OF_TCPIP_SOCKET_CON                         [!"num:i($NoOfTcpIpConnections)"!]U
  [!ENDIF!]
[!ENDIF!]

#if (defined XCP_SOAD_AUTO_OPEN_SO_CON)
#error XCP_SOAD_AUTO_OPEN_SO_CON already defined
#endif
/** \brief Switch, indicating whether SoAd_OpenSoCon() will be called during Xcp_Init() */
#define XCP_SOAD_AUTO_OPEN_SO_CON        [!//
[!IF "$AutomaticOpenSoCon = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


#if (defined XCP_SEED_AND_KEY_ENABLED)
#error XCP_SEED_AND_KEY_ENABLED already defined
#endif
/** \brief Switch, indicating whether resource protection with Seed & Key mechanism
 ** is activated or deactivated for XCP */
#define XCP_SEED_AND_KEY_ENABLED         [!//
[!IF "text:tolower(XcpGeneral/XcpSeedAndKeyEnabled) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined XCP_EVENT_PACKET_ENABLED)
#error XCP_EVENT_PACKET_ENABLED already defined
#endif
/** \brief Switch, indicating whether the transmission of Event packets from the slave device to
 ** the master device is activated or deactivated */
#define XCP_EVENT_PACKET_ENABLED         [!//
[!IF "text:tolower(XcpGeneral/XcpEventPacketEnabled) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!IF "text:tolower(XcpGeneral/XcpEventPacketEnabled) = 'true'"!]
  [!IF "(XcpGeneral/XcpStimSupported = 'true') or (XcpGeneral/XcpStoreDaqSupported = 'true')"!]
    [!VAR "XcpMaxEvCto"="8"!]
  [!ELSE!]
    [!VAR "XcpMaxEvCto"="2"!]
  [!ENDIF!]

#if (defined XCP_MAX_EV_CTO)
#error XCP_MAX_EV_CTO already defined
#endif
/** \brief The maximum length of an event CTO package */
#define XCP_MAX_EV_CTO                   [!"num:i($XcpMaxEvCto)"!]U
[!ENDIF!]

[!IF "node:exists(XcpGeneral/XcpEventPacketEnabled) = 'true'"!]
  [!IF "(node:exists(XcpGeneral/XcpStimSupported)= 'true') and (text:tolower(XcpGeneral/XcpStimSupported) = 'true')"!]
    [!VAR "XcpEvCtoQueueSize"="num:max(XcpConfig/*/XcpEventChannel/*/XcpEventChannelMaxDaqList)"!]
    [!IF "$XcpEvCtoQueueSize < 2"!]
      [!VAR "XcpEvCtoQueueSize"="2"!]
    [!ENDIF!]
  [!ELSE!]
    [!VAR "XcpEvCtoQueueSize"="2"!]
  [!ENDIF!]
#if (defined XCP_EV_CTO_QUEUE_SIZE)
#error XCP_EV_CTO_QUEUE_SIZE already defined
#endif
/** \brief Macro to hold the size of the Event CTO queue based on the value of maximum DAQ list
 ** allocated per Event Channel when STIM is supported else the Event CTO packets will be processed
 ** in a single cyclic function, so size of the Event CTO queue shall be considered as 2 */
#define XCP_EV_CTO_QUEUE_SIZE            [!"num:i($XcpEvCtoQueueSize)"!]U
[!ENDIF!]

#if (defined XCP_EVENT_CHANNEL_TOTAL_DAQ)
#error XCP_EVENT_CHANNEL_TOTAL_DAQ already defined
#endif
/** \brief The total number of DaqLists that can be associated to events channels on the XCP slave. */
#define XCP_EVENT_CHANNEL_TOTAL_DAQ        [!//
[!"num:i(sum(XcpConfig/*[1]/XcpEventChannel/*/XcpEventChannelMaxDaqList))"!]U

#if (defined XCP_NUM_EVENT_CHANNEL_PRECFG)
#error XCP_NUM_EVENT_CHANNEL_PRECFG already defined
#endif
/** \brief The total number of preconfigured DaqLists that are associated to events channels */
#define XCP_NUM_EVENT_CHANNEL_PRECFG       [!//
[!"num:i(count(XcpConfig/*[1]/XcpEventChannel/*/XcpEventChannelTriggeredDaqListRef/*))"!]U

#if (defined XCP_MODIFY_BITS_SUPPORTED)
#error XCP_MODIFY_BITS_SUPPORTED already defined
#endif
/** \brief MODIFY_BITS supported */
#define XCP_MODIFY_BITS_SUPPORTED         STD_[!IF "XcpGeneral/XcpModifyBitsSupported = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]

#if (defined XCP_WRITE_DAQ_MULTIPLE_SUPPORT)
#error XCP_WRITE_DAQ_MULTIPLE_SUPPORT already defined
#endif
/** \brief Enable/disable the WRITE_DAQ_MULTIPLE feature. */
#define XCP_WRITE_DAQ_MULTIPLE_SUPPORT   [!//
[!IF "XcpGeneral/XcpWriteDaqMultipleSupported = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined XCP_BUILD_CHECKSUM_SUPPORT)
#error XCP_BUILD_CHECKSUM_SUPPORT already defined
#endif
/** \brief Enable/disable the BUILD_CHECKSUM feature. */
#define XCP_BUILD_CHECKSUM_SUPPORT   [!//
[!IF "XcpGeneral/XcpBuildChecksumSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined XCP_BUILD_CHECKSUM_TYPE)
#error XCP_BUILD_CHECKSUM_TYPE already defined
#endif
[!IF "XcpGeneral/XcpBuildChecksumSupport = 'true'"!]
/** \brief The type of the checksum that the BUILD_CHECKSUM feature will use */
#define XCP_BUILD_CHECKSUM_TYPE   [!//
[!"XcpGeneral/XcpBuildChecksumType"!]
[!ENDIF!]

#if (defined XCP_USER_TIMESTAMP_SUPPORTED)
#error XCP_USER_TIMESTAMP_SUPPORTED already defined
#endif
/** \brief Enable/Disable the Xcp User Timestamp */
#define XCP_USER_TIMESTAMP_SUPPORTED  STD_[!IF "(node:exists(XcpGeneral/XcpUserTimestampSupported) = 'true') and (XcpGeneral/XcpUserTimestampSupported = 'true')"!]ON[!ELSE!]OFF[!ENDIF!]

#if (defined XCP_SLAVE_BLOCK_MODE)
#error XCP_SLAVE_BLOCK_MODE already defined
#endif
/** \brief Switch, indicating whether slave block mode functionality is available. */
#define XCP_SLAVE_BLOCK_MODE             [!//
[!IF "text:tolower(XcpGeneral/XcpSlaveBlockModeSupported) = 'true'"!]XCP_SLAVE_BLOCK_MODE_MASK[!ELSE!](0U)[!ENDIF!]

#if (defined XCP_MULTI_PDUS_IN_FRAME_SUPPORT)
#error XCP_MULTI_PDUS_IN_FRAME_SUPPORT already defined
#endif
/** \brief Switch for enabling support of multiple XCP messages in one frame */
#define XCP_MULTI_PDUS_IN_FRAME_SUPPORT  STD_[!IF "$MultiplePdusInOneFrame = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]

#if (defined XCP_FLEXRAY_SEQUENCE_CORRECTION_SUPPORT)
#error XCP_FLEXRAY_SEQUENCE_CORRECTION_SUPPORT already defined
#endif
/** \brief Switch for enabling support of multiple XCP messages in one frame */
#define XCP_FLEXRAY_SEQUENCE_CORRECTION_SUPPORT  STD_[!IF "$FlexRaySequenceCorrection = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]

#if (defined XCP_TRANSMIT_FROM_CONFIRMATION)
#error XCP_TRANSMIT_FROM_CONFIRMATION already defined
#endif
/** \brief Switch indicating whether transmission from TxConfirmation is enabled. */
#define XCP_TRANSMIT_FROM_CONFIRMATION           [!//
[!IF "num:i(count(XcpConfig/*/XcpConnectionCfg/*/XcpTxPduConnectionInfo/*[XcpPduSupportTxFromTxConfirmation = 'true'])) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined XCP_RECEIVE_FROM_INDICATION)
#error XCP_RECEIVE_FROM_INDICATION already defined
#endif
/** \brief Switch indicating whether reception from RxIndication is enabled. */
#define XCP_RECEIVE_FROM_INDICATION           [!//
[!IF "num:i(count(XcpConfig/*/XcpConnectionCfg/*/XcpRxPduConnectionInfo/*[XcpPduSupportRxFromRxIndication = 'true'])) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/* Validation: If XcpMulticastCommands is set as true, at least one IPv4 UDP connection needs to be configured */
[!IF "text:tolower(XcpGeneral/XcpMulticastCommands) = 'true' and $IsUpdIPv4 = 'false'"!]
  [!/* !LINKSTO Xcp.Configuration.XcpMulticastCommands.GenerationError,1 */!]
  [!ERROR!]Support for Multicast commands is enabled but no Ethernet IPv4 UDP connection is configured. Please configure first an Ethernet IPv4 UDP connection.[!ENDERROR!]
[!ENDIF!]

#if (defined XCP_IMPLEMENT_MULTICAST_COMMANDS)
#error XCP_IMPLEMENT_MULTICAST_COMMANDS already defined
#endif
/** \brief Switch indicating whether multicast commands are implemented */
#define XCP_IMPLEMENT_MULTICAST_COMMANDS      [!//
[!IF "text:tolower(XcpGeneral/XcpMulticastCommands) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!IF "text:tolower(XcpGeneral/XcpMulticastCommands) = 'true'"!]
#if (defined XCP_MULTICAST_TO_UNICAST_RETRY_COUNT)
#error XCP_MULTICAST_TO_UNICAST_RETRY_COUNT already defined
#endif
/** \brief The number of times the change of SoAd Tx socket address from Multicast to Unicast will be retried
 **/
/* !LINKSTO Xcp.XcpMulticastCommands.XcpMulticastToUnicastRetry,1 */
#define XCP_MULTICAST_TO_UNICAST_RETRY_COUNT           [!"num:i(XcpGeneral/XcpMulticastToUnicastRetry)"!]U
[!ENDIF!]

#if (defined XCP_GET_CONNECTION_MAX_CTO)
#error XCP_GET_CONNECTION_MAX_CTO already defined
#endif
#define XCP_GET_CONNECTION_MAX_CTO Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Max_Cto

[!IF "text:tolower(XcpGeneral/XcpMasterBlockModeSupported) = 'true'"!]
#if (defined XCP_GET_CONNECTION_MAX_BS)
#error XCP_GET_CONNECTION_MAX_BS already defined
#endif
/** \brief The value of the configured XcpMaxBS for each connection */
#define XCP_GET_CONNECTION_MAX_BS Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Max_BS
[!ELSE!]
#define XCP_GET_CONNECTION_MAX_BS 0U
[!ENDIF!]

#if (defined XCP_GET_CONNECTION_MAX_CTO_PGM)
#error XCP_GET_CONNECTION_MAX_CTO_PGM already defined
#endif
[!IF "text:tolower(XcpGeneral/XcpPgmSupported) = 'true'"!]
/** \brief The value of the configured XcpMaxCtoPgm for each connection */
#define XCP_GET_CONNECTION_MAX_CTO_PGM Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Max_Cto_Pgm
#if (defined XCP_GET_CONNECTION_MAX_BS_PGM)
#error XCP_GET_CONNECTION_MAX_BS_PGM already defined
#endif
[!IF "XcpGeneral/XcpMasterBlockModePgmSupported = 'true'"!]
/** \brief The value of the configured XcpMaxBSPgm for each connection */
#define XCP_GET_CONNECTION_MAX_BS_PGM Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Max_BS_Pgm
[!ELSE!]
#define XCP_GET_CONNECTION_MAX_BS_PGM 0U
[!ENDIF!]
[!ELSE!]
#define XCP_GET_CONNECTION_MAX_CTO_PGM 0U
[!ENDIF!]

#if (defined XCP_GET_MAX_OF_MAX_DLC)
#error XCP_GET_MAX_OF_MAX_DLC already defined
#endif

[!IF "count(XcpConfig/*[1]/XcpConnectionCfg/*[XcpConnectionInterfaceType = 'XcpConnectionOverCANFD' and XcpConnectionInterfaceType/XcpCanFdMaxDlcRequired = 'true']) > 0"!]
#define XCP_GET_MAX_OF_MAX_DLC [!"num:max(XcpConfig/*[1]/XcpConnectionCfg/*[XcpConnectionInterfaceType = 'XcpConnectionOverCANFD' and XcpConnectionInterfaceType/XcpCanFdMaxDlcRequired = 'true']/XcpConnectionInterfaceType/XcpCanFdMaxDlc)"!]U
[!ELSE!]
#define XCP_GET_MAX_OF_MAX_DLC 0U
[!ENDIF!]

[!IF "$XcpMemoryAccessAreas"!][!//
/** \brief The size of the memory proxy internal buffer */
#define XCP_MEMORY_PROXY_BUFFER_SIZE [!"num:i(XcpBswDistribution/XcpMemoryProxyBufferSize)"!]U

#if (defined XCP_NUMBER_OF_QUEUED_EVENTS)
#error XCP_NUMBER_OF_QUEUED_EVENTS already defined
#endif
/** \brief size of buffer for queued requests to MemProxy
  */
#define XCP_NUMBER_OF_QUEUED_EVENTS  (uint8)[!"num:i(XcpBswDistribution/XcpMemProxyNumberOfRequestQueued)"!]U
[!ENDIF!][!//

[!IF "$XcpBswDistributionEnabled"!][!//
#if (defined XCP_NUM_OF_OS_APPS)
#error XCP_NUM_OF_OS_APPS already defined
#endif
/** \brief The number of OS applications */
#define XCP_NUM_OF_OS_APPS [!"$XcpNumOsApps"!]U

#if (defined XCP_NUM_OF_OS_CORES)
#error XCP_NUM_OF_OS_CORES already defined
#endif

#define XCP_NUM_OF_OS_CORES [!"num:i(as:modconf('Os')[1]/OsOS/OsNumberOfCores)"!]U

[!ENDIF!][!//

/*==================[type definitions]=======================================*/

 /** \brief Definition for the Bus Timeout Counter value data type used to count the amount of
  ** Xcp_MainFunction cycles until the TX pending message is send by the underlying communication
  ** from the moment the transmission was accepted. */
 typedef uint[!"$BusToTypeSize"!] Xcp_BusTimeoutCounterValueType;

 /** \brief Definition for the FlexRay Hardware Buffers Timeout Counter value data type used to
  ** count the amount of Xcp_MainFunction cycles since a FlexRay hardware buffer initialization
  ** error has occurred. */
 typedef uint[!"$FlexRayToTypeSize"!] Xcp_FlexRayTimeoutCounterValueType;

/*==================[external function declarations]=========================*/

[!IF "RamOptimizations/XcpEnableDaqOptimization = 'true' and ($XcpNoOfNonDynamicODTEntries > 0 or XcpGeneral/XcpDaqConfigType = 'DAQ_DYNAMIC')"!][!//

#define XCP_START_SEC_CODE
#include <Xcp_MemMap.h>

/** \brief Helper function that retrieves a pointer to the data associated with an ODT entry.
 ** This function is required when the RAM optimizations feature is enabled in order obtain either a pointer
 ** to Xcp_OdtAddressTableRAM/Xcp_OdtLengthTableRAM/Xcp_OdtBitOffsetTableRAM or a pointer to
 ** Xcp_DaqLists.Xcp_DynamicArea.
 ** The ptr parameter is typically the OdtEntry field of an Xcp_OdtType.
 ** The offset parameter represents the number of fields to skip until the required ODT entry field is reached. Note that
 ** this parameter doesn't take into account the type of field.
 ** The field parameter can be XCP_ODT_ENTRY_FIELD_ADDRESS, XCP_ODT_ENTRY_FIELD_LENGTH or XCP_ODT_ENTRY_FIELD_BITOFFSET.
 ** This function returns a void pointer to the desired element in order to allow writing as well. This must be cast to
 ** either an uint32 pointer when the field parameter is XCP_ODT_ENTRY_FIELD_ADDRESS or an uint8 pointer otherwise.
 */
extern FUNC_P2VAR(void, XCP_VAR, XCP_CODE) Xcp_GetOdtEntryData
(
  P2VAR(void, AUTOMATIC, XCP_VAR) ptr,
  uint8 offset,
  uint8 field
);

#define XCP_STOP_SEC_CODE
#include <Xcp_MemMap.h>

[!ENDIF!][!//
/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/
[!IF "$XcpMemoryAccessAreas"!][!//
/** \brief Function pointer type for send Memory Proxy request 
 **/
typedef Std_ReturnType (*Xcp_SendMemoryProxyData_FuncPtr)(
  P2CONST(Xcp_MemoryProxyDataType, AUTOMATIC, RTE_APPL_DATA) data);

/** \brief Array of function pointers for send Memory Proxy request
 **/
extern Xcp_SendMemoryProxyData_FuncPtr Xcp_SendMemoryProxyData[[!"$XcpNumOsApps"!]][[!"$XcpNumOsApps"!]];
[!ENDIF!][!//


[!IF "($XcpDaqStimMemoryAccessAreas = 'true') or ($RAMCalloutProxyEnabled = 'true')"!][!//
/** \brief Function pointer type for response provided by Memory Proxy
 **/
typedef Std_ReturnType (*Xcp_SendMemoryProxyNotification_FuncPtr)(uint16 EventId);
/** \brief Array of function pointers for responses provided by Memory Proxy
 **/
extern Xcp_SendMemoryProxyNotification_FuncPtr Xcp_SendMemoryProxyNotification[[!"$XcpNumOsApps"!]][[!"$XcpNumOsApps"!]];
[!ENDIF!][!//


[!IF "$XcpBswDistributionEnabled"!][!//
[!IF "text:tolower($StoreDaq) = 'true'"!][!//
typedef Std_ReturnType (*Xcp_SendNvMRequest_FuncPtr)(uint8 PartitionId);
extern Xcp_SendNvMRequest_FuncPtr Xcp_SendNvMRequest[[!"$XcpNumOsApps"!]];
[!ENDIF!][!//
[!ENDIF!][!//

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef XCP_INT_CFG_H */
/*==================[end of file]============================================*/
[!ENDINDENT!]
