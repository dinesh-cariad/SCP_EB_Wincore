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
[!AUTOSPACING!]

/*==================[inclusions]=============================================*/

#include <Xcp.h>               /* Module public API */
#if ( (XCP_DEFAULT_XCP_MODULE_STATE == STD_ON) || (XCP_ENABLE_XCP_CONTROL_API == STD_ON) )
#include <Xcp_Int.h>           /* Module internal interface */
#include <Xcp_Cbk.h>           /* callbacks and callouts of Xcp */
#include <TSAutosar.h>         /* EB specific standard types, TS_PARAM_UNUSED*/
#include <Xcp_UserCallouts.h>  /* Xcp callouts */
#include <Xcp_Trace.h>

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

#define XCP_START_SEC_APPL_CODE
#include <Xcp_MemMap.h>

/*------------------[Callouts to compare key]--------------------------------*/

FUNC(Std_ReturnType, XCP_APPL_CODE) Xcp_ApplCompareKey
(
  uint8 ResourceIdentifier,
  P2CONST(uint8, AUTOMATIC, XCP_APPL_DATA) KeyBufferPtr,
  uint8 KeyLength
)
{
  DBG_XCP_APPLCOMPAREKEY_ENTRY(ResourceIdentifier,KeyBufferPtr,KeyLength);
  TS_PARAM_UNUSED(ResourceIdentifier); /* Remove if parameter is used */
  TS_PARAM_UNUSED(KeyBufferPtr); /* Remove if parameter is used */
  TS_PARAM_UNUSED(KeyLength); /* Remove if parameter is used */

  /* insert your code here */
  DBG_XCP_APPLCOMPAREKEY_EXIT(E_OK,ResourceIdentifier,KeyBufferPtr,KeyLength);

  return E_OK;
}

/*------------------[Callouts to generate seed]------------------------------*/

FUNC(Std_ReturnType, XCP_APPL_CODE) Xcp_ApplGetSeed
(
  uint8 ResourceIdentifier,
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) SeedBufferPtr,
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) SeedBufferLengthPtr
)
{
  Std_ReturnType RetValue = E_NOT_OK;
  DBG_XCP_APPLGETSEED_ENTRY(ResourceIdentifier,SeedBufferPtr,SeedBufferLengthPtr);

  
#if (XCP_DEV_ERROR_DETECT == STD_ON)
  if ((SeedBufferPtr == NULL_PTR) || (SeedBufferLengthPtr == NULL_PTR))
  {
    XCP_DET_REPORT_ERROR(XCP_SID_APPL_COMMAND, XCP_E_NULL_POINTER);
  }
  else
#endif
  {
    TS_PARAM_UNUSED(ResourceIdentifier);  /* Remove if parameter is used */
    TS_PARAM_UNUSED(SeedBufferPtr);       /* Remove if parameter is used */
    TS_PARAM_UNUSED(SeedBufferLengthPtr); /* Remove if parameter is used */

    /* insert your code here */
    RetValue = E_OK;
  }

  DBG_XCP_APPLGETSEED_EXIT(RetValue,ResourceIdentifier,SeedBufferPtr,SeedBufferLengthPtr);
  
  return RetValue;
}

[!IF "XcpGeneral/XcpBuildChecksumSupport = 'true'"!]
  [!IF "XcpGeneral/XcpBuildChecksumType = 'XCP_CRC_USER_CALLOUT'"!]
/*------------------[Callout for command BUILD_CHECKSUM]---------------------*/

FUNC(Xcp_ApplReturnType, XCP_APPL_CODE) Xcp_ApplBuildChecksum
(
  P2CONST(void, AUTOMATIC, XCP_APPL_DATA) AddressPtr,
  uint32 BlockSize,
  P2VAR(uint32, AUTOMATIC, XCP_APPL_DATA) ChecksumResult,
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) ChecksumType
)
{
  Xcp_ApplReturnType RetValue;

  DBG_XCP_APPLBUILDCHECKSUM_ENTRY(AddressPtr,BlockSize,ChecksumResult,ChecksumType);

  TS_PARAM_UNUSED(AddressPtr); /* Remove if parameter is used */
  TS_PARAM_UNUSED(BlockSize); /* Remove if parameter is used */
  *ChecksumResult = 0xf0f0f0f0U; /* Remove if function is used */
  *ChecksumType = 0x07U; /* Remove if function is used */


  /* insert your code here */

  RetValue = XCP_APPL_OK;

  DBG_XCP_APPLBUILDCHECKSUM_EXIT(RetValue,AddressPtr,BlockSize,ChecksumResult,ChecksumType);
  return RetValue;
}

  [!ENDIF!][!// "XcpGeneral/XcpBuildChecksumType = 'CRC_USER_CALLOUT'"
[!ENDIF!][!// "XcpGeneral/XcpBuildChecksumSupport = 'true'"
[!IF "(node:exists(XcpGeneral/XcpUserTimestampSupported) = 'true') and (XcpGeneral/XcpUserTimestampSupported = 'true')"!]
/*------------------[User specific timestamp callout function]---------------*/

FUNC(Xcp_TimestampType, XCP_APPL_CODE) Xcp_ApplGetTimestamp(void)
{
  Xcp_TimestampType RetValue;

  DBG_XCP_APPLGETTIMESTAMP_ENTRY();

  /* insert your code here */

  RetValue = 0U;

  DBG_XCP_APPLGETTIMESTAMP_EXIT(RetValue);
  return RetValue;
}

[!ENDIF!][!// "(node:exists(XcpGeneral/XcpUserTimestampSupported) = 'true') and (XcpGeneral/XcpUserTimestampSupported = 'true')"
[!IF "XcpGeneral/XcpCalPagSupported = 'true'"!]
/*------------------[Callout for command Calibration Page Initialization]----*/

FUNC(void, XCP_APPL_CODE) Xcp_ApplCalPagInit
(
  void
)
{
  DBG_XCP_APPLCALPAGINIT_ENTRY();

  /* insert your code here */

  DBG_XCP_APPLCALPAGINIT_EXIT();
}

/*------------------[Callout for command COPY_CAL_PAGE]----------------------*/

FUNC(Xcp_ApplReturnType, XCP_APPL_CODE) Xcp_ApplCopyCalPage
(
  uint8 SrcSegment,
  uint8 SrcPage,
  uint8 DestSegment,
  uint8 DestPage
)
{

  DBG_XCP_APPLCOPYCALPAGE_ENTRY(SrcSegment,SrcPage,DestSegment,DestPage);
  TS_PARAM_UNUSED( SrcSegment );  /* Remove if parameter is used */
  TS_PARAM_UNUSED( SrcPage );     /* Remove if parameter is used */
  TS_PARAM_UNUSED( DestSegment ); /* Remove if parameter is used */
  TS_PARAM_UNUSED( DestPage );    /* Remove if parameter is used */

  /* insert your code here */


  DBG_XCP_APPLCOPYCALPAGE_EXIT(XCP_APPL_OK,SrcSegment,SrcPage,DestSegment,DestPage);
  return XCP_APPL_OK;
}

/*------------------[Callout for command GET_PAG_PROCESSOR_INFO]-------------*/

FUNC(Xcp_ApplReturnType, XCP_APPL_CODE) Xcp_ApplGetPagProcessorInfo
(
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) MaxSegmentPtr,
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) PagPropertiesPtr
)
{

  DBG_XCP_APPLGETPAGPROCESSORINFO_ENTRY(MaxSegmentPtr,PagPropertiesPtr);
  *MaxSegmentPtr    = 0U; /* Remove if function is used */
  *PagPropertiesPtr = 0U; /* Remove if function is used */

  /* insert your code here */


  DBG_XCP_APPLGETPAGPROCESSORINFO_EXIT(XCP_APPL_OK,MaxSegmentPtr,PagPropertiesPtr);
  return XCP_APPL_OK;
}

/*------------------[Callout for command GET_CAL_PAGE]-----------------------*/

FUNC(Xcp_ApplReturnType, XCP_APPL_CODE) Xcp_ApplGetCalPage
(
  uint8 Segment,
  uint8 Mode,
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) PagePtr
)
{

  DBG_XCP_APPLGETCALPAGE_ENTRY(Segment,Mode,PagePtr);
  TS_PARAM_UNUSED( Segment ); /* Remove if parameter is used */
  TS_PARAM_UNUSED( Mode );    /* Remove if parameter is used */
  *PagePtr = 0U; /* Remove if function is used */

  /* insert your code here */


  DBG_XCP_APPLGETCALPAGE_EXIT(XCP_APPL_OK,Segment,Mode,PagePtr);
  return XCP_APPL_OK;
}

/*------------------[Callout for command SET_CAL_PAGE]-----------------------*/

FUNC(Xcp_ApplReturnType, XCP_APPL_CODE) Xcp_ApplSetCalPage
(
  uint8 Segment,
  uint8 Page,
  uint8 Mode
)
{

  DBG_XCP_APPLSETCALPAGE_ENTRY(Segment,Page,Mode);
  TS_PARAM_UNUSED( Segment ); /* Remove if parameter is used */
  TS_PARAM_UNUSED( Page );    /* Remove if parameter is used */
  TS_PARAM_UNUSED( Mode );    /* Remove if parameter is used */

  /* insert your code here */


  DBG_XCP_APPLSETCALPAGE_EXIT(XCP_APPL_OK,Segment,Page,Mode);
  return XCP_APPL_OK;
}

/*------------------[Callout for command GET_SEGMENT_MODE]-------------------*/

FUNC(Xcp_ApplReturnType, XCP_APPL_CODE) Xcp_ApplGetSegmentMode
(
  uint8 Segment,
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) ModePtr
)
{

  DBG_XCP_APPLGETSEGMENTMODE_ENTRY(Segment,ModePtr);
  TS_PARAM_UNUSED( Segment ); /* Remove if parameter is used */

  /* insert your code here */
  *ModePtr = 0U;                 /* Remove if function is used */


  DBG_XCP_APPLGETSEGMENTMODE_EXIT(XCP_APPL_OK,Segment,ModePtr);
  return XCP_APPL_OK;
}

/*------------------[Callout for command SET_SEGMENT_MODE]-------------------*/

FUNC(Xcp_ApplReturnType, XCP_APPL_CODE) Xcp_ApplSetSegmentMode
(
  uint8 Segment,
  uint8 Mode
)
{

  DBG_XCP_APPLSETSEGMENTMODE_ENTRY(Segment,Mode);
  TS_PARAM_UNUSED( Segment ); /* Remove if parameter is used */
  TS_PARAM_UNUSED( Mode );    /* Remove if parameter is used */

  /* insert your code here */


  DBG_XCP_APPLSETSEGMENTMODE_EXIT(XCP_APPL_OK,Segment,Mode);
  return XCP_APPL_OK;
}

/*------------------[Callout for command GET_SEGMENT_INFO]-------------------*/

FUNC(Xcp_ApplReturnType, XCP_APPL_CODE) Xcp_ApplGetSegmentInfo
(
  uint8 Mode,
  uint8 Segment,
  uint8 SegmentInfo,
  uint8 MappingIndex,
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) CommandResponsePtr
)
{

  DBG_XCP_APPLGETSEGMENTINFO_ENTRY(Mode,Segment,SegmentInfo,MappingIndex,CommandResponsePtr);
  TS_PARAM_UNUSED( Mode );         /* Remove if parameter is used */
  TS_PARAM_UNUSED( Segment );      /* Remove if parameter is used */
  TS_PARAM_UNUSED( SegmentInfo );  /* Remove if parameter is used */
  TS_PARAM_UNUSED( MappingIndex ); /* Remove if parameter is used */

  /* insert your code here */
  *CommandResponsePtr = 0U;        /* Remove if function is used */


  DBG_XCP_APPLGETSEGMENTINFO_EXIT(XCP_APPL_OK,Mode,Segment,SegmentInfo,MappingIndex,CommandResponsePtr);
  return XCP_APPL_OK;
}

/*------------------[Callout for command GET_PAGE_INFO]----------------------*/

FUNC(Xcp_ApplReturnType, XCP_APPL_CODE) Xcp_ApplGetPageInfo
(
  uint8 Segment,
  uint8 Page,
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) PagePropertiesPtr,
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) InitSegmentPtr
)
{

  DBG_XCP_APPLGETPAGEINFO_ENTRY(Segment,Page,PagePropertiesPtr,InitSegmentPtr);
  TS_PARAM_UNUSED( Segment );      /* Remove if parameter is used */
  TS_PARAM_UNUSED( Page );         /* Remove if parameter is used */

  *PagePropertiesPtr = 0U;         /* Remove if function is used */
  *InitSegmentPtr = 0U;            /* Remove if function is used */

  /* insert your code here */


  DBG_XCP_APPLGETPAGEINFO_EXIT(XCP_APPL_OK,Segment,Page,PagePropertiesPtr,InitSegmentPtr);
  return XCP_APPL_OK;
}

/*------------------[Callout for command SET_REQUEST - STORE_CAL_REQ Mode]---*/

FUNC(Xcp_ApplReturnType, XCP_APPL_CODE) Xcp_ApplSetReqStoreCalReq
(
  void
)
{
  DBG_XCP_APPLSETREQSTORECALREQ_ENTRY();

  /* insert your code here */


  DBG_XCP_APPLSETREQSTORECALREQ_EXIT(XCP_APPL_OK);
  return XCP_APPL_OK;
}

[!ENDIF!][!// "XcpGeneral/XcpCalPagSupported = 'true'"
[!IF "XcpGeneral/XcpAddressTranslationSupport = 'true'"!]
/*------------------[Callout for address translation]------------------------*/

FUNC_P2VAR(void, XCP_APPL_DATA, XCP_APPL_CODE) Xcp_ApplGetAddress
(
  uint8 AddressExtension,
  P2VAR(void, AUTOMATIC, XCP_APPL_DATA) AddressPtr
)
{
  DBG_XCP_APPLGETADDRESS_ENTRY(AddressExtension,AddressPtr);

  
  TS_PARAM_UNUSED(AddressExtension); /* Remove if parameter is used */
  TS_PARAM_UNUSED(AddressPtr); /* Remove if parameter is used */

  /* insert your code here */

  DBG_XCP_APPLGETADDRESS_EXIT(AddressPtr,AddressExtension,AddressPtr);
  return AddressPtr;
}

[!ENDIF!][!// "XcpGeneral/XcpAddressTranslationSupport = 'true'"
[!IF "node:exists(XcpConfig/*[1]/XcpMemoryAccessArea/XcpMemoryAccessAreasCallout) and XcpConfig/*[1]/XcpMemoryAccessArea/XcpMemoryAccessAreasCallout = 'true'"!]
/*------------------[Callout for requested memory area access]------------------*/

FUNC(boolean, XCP_APPL_CODE) Xcp_ApplIsMemoryAreaAccessible
(
  P2CONST(uint8, AUTOMATIC, XCP_APPL_DATA) MemoryAreaAddress,
  uint32 MemoryAreaLength,
  uint8 MemoryAreaType,
  uint8 MemoryAreaScope
)
{
  boolean RetValue = FALSE;

  DBG_XCP_APPLISMEMORYAREAACCESSIBLE_ENTRY(MemoryAreaAddress,MemoryAreaLength,
                                           MemoryAreaType,MemoryAreaScope);
  
  TS_PARAM_UNUSED(MemoryAreaAddress); /* Remove if parameter is used */
  TS_PARAM_UNUSED(MemoryAreaLength);  /* Remove if parameter is used */
  TS_PARAM_UNUSED(MemoryAreaType);    /* Remove if parameter is used */
  TS_PARAM_UNUSED(MemoryAreaScope);   /* Remove if parameter is used */

  /* insert your code here */

  DBG_XCP_APPLISMEMORYAREAACCESSIBLE_EXIT(RetValue,MemoryAreaAddress,MemoryAreaLength,
                                          MemoryAreaType,MemoryAreaScope);

  return RetValue;
}

[!ENDIF!][!// "node:exists(XcpConfig/*[1]/XcpMemoryAccessArea/XcpMemoryAccessAreasCallout) and XcpConfig/*[1]/XcpMemoryAccessArea/XcpMemoryAccessAreasCallout = 'true'"
[!IF "XcpGeneral/XcpPgmSupported = 'true'"!]

/*------------------[Callout for PROGRAM_START]------------------------------*/

FUNC(Xcp_ApplReturnType, XCP_APPL_CODE) Xcp_ApplProgramStart
(
  P2VAR(uint16, AUTOMATIC, XCP_APPL_DATA) ErrorCodePtr
)
{
  
  DBG_XCP_APPLPROGRAMSTART_ENTRY(ErrorCodePtr);
  
  *ErrorCodePtr = 0U; /* Remove if function is used */

  /* insert your code here */
  DBG_XCP_APPLPROGRAMSTART_EXIT(XCP_APPL_OK);
  
  return XCP_APPL_OK;
}

/*------------------[Callout for PROGRAM_CLEAR]------------------------------*/

FUNC(Xcp_ApplReturnType, XCP_APPL_CODE) Xcp_ApplProgramClear
(
  P2VAR(void, AUTOMATIC, XCP_APPL_DATA) AddressPtr,
  uint32 ClearRange
)
{
  DBG_XCP_APPLPROGRAMCLEAR_ENTRY(AddressPtr, ClearRange);
  TS_PARAM_UNUSED(AddressPtr); /* Remove if parameter is used */
  TS_PARAM_UNUSED(ClearRange); /* Remove if parameter is used */

  /* insert your code here */
  DBG_XCP_APPLPROGRAMCLEAR_EXIT(XCP_APPL_OK, AddressPtr, ClearRange);
  return XCP_APPL_OK;
}

/*------------------[Callout for PROGRAM/PROGRAM_NEXT/PROGRAM_MAX]----------------------------*/

FUNC(Xcp_ApplReturnType, XCP_APPL_CODE) Xcp_ApplProgram
(
  P2VAR(void, AUTOMATIC, XCP_APPL_DATA) AddressPtr,
  P2CONST( uint8, AUTOMATIC, XCP_APPL_DATA ) DataPtr,
  uint16 DataLength
)
{
  DBG_XCP_APPLPROGRAM_ENTRY(AddressPtr, DataPtr, DataLength);
  TS_PARAM_UNUSED(AddressPtr); /* Remove if parameter is used */
  TS_PARAM_UNUSED(DataPtr); /* Remove if parameter is used */
  TS_PARAM_UNUSED(DataLength); /* Remove if parameter is used */

  /* insert your code here */

  DBG_XCP_APPLPROGRAM_EXIT(XCP_APPL_OK, AddressPtr, DataPtr, DataLength)
  return XCP_APPL_OK;
}

/*------------------[Callout for PROGRAM_RESET]------------------------------*/

FUNC(void, XCP_APPL_CODE) Xcp_ApplProgramReset(void)
{

  DBG_XCP_APPLPROGRAMRESET_ENTRY();
  
  /* insert your code here */
  DBG_XCP_APPLPROGRAMRESET_EXIT();
  
}

/*------------------[Callout for GET_PGM_PROCESSOR_INFO]------------------------------*/

FUNC(Xcp_ApplReturnType, XCP_APPL_CODE) Xcp_ApplGetPgmProcessorInfo
(
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) MaxSectorPtr,
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) PgmPropertiesPtr
)
{
  Xcp_ApplReturnType RetValue = XCP_APPL_OK;

  TS_PARAM_UNUSED(MaxSectorPtr);     /* Remove if parameter is used */
  TS_PARAM_UNUSED(PgmPropertiesPtr); /* Remove if parameter is used */

  DBG_XCP_APPLGETPGMPROCESSORINFO_ENTRY(MaxSectorPtr, PgmPropertiesPtr);

  /* insert your code here */

  DBG_XCP_APPLGETPGMPROCESSORINFO_EXIT(MaxSectorPtr, PgmPropertiesPtr, RetValue);

  return RetValue;
}

/*------------------[Callout for GET_SECTOR_INFO]------------------------------*/

FUNC(Xcp_ApplReturnType, XCP_APPL_CODE) Xcp_ApplGetSectorInfo
(
  uint8 Mode,
  uint8 Sector,
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) MtaPtr,
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) CommandResponsePtr
)
{
  Xcp_ApplReturnType RetValue = XCP_APPL_OK;

  TS_PARAM_UNUSED(Mode);               /* Remove if parameter is used */
  TS_PARAM_UNUSED(Sector);             /* Remove if parameter is used */
  TS_PARAM_UNUSED(MtaPtr);             /* Remove if parameter is used */
  TS_PARAM_UNUSED(CommandResponsePtr); /* Remove if parameter is used */

  DBG_XCP_APPLGETSECTORINFO_ENTRY(Mode, Sector, MtaPtr, CommandResponsePtr);

  /* insert your code here */

  DBG_XCP_APPLGETSECTORINFO_EXIT(Mode, Sector, MtaPtr, CommandResponsePtr, RetValue);

  return RetValue;
}

[!ENDIF!][!// "XcpGeneral/XcpPgmSupported = 'true'

[!IF "(XcpGeneral/XcpBlockWriteReadMemoryRAMMechanism = 'true') and (node:exists(XcpGeneral/XcpWriteMemoryRAMCallback)) and not(node:empty(XcpGeneral/XcpWriteMemoryRAMCallback))"!]
/*------------------[Callout for writing data to RAM memory]------------------------------*/
FUNC(Xcp_ApplReturnType, XCP_APPL_CODE) Xcp_ApplWriteDataToRAM
(
  P2VAR( void, AUTOMATIC, XCP_APPL_DATA ) AddressPtr,
  P2CONST( uint8, AUTOMATIC, XCP_APPL_DATA ) DataPtr,
  uint8 DataLength
)
{
    Xcp_ApplReturnType RetValue = XCP_APPL_OK;
    
    DBG_XCP_APPLWRITEDATATORAM_ENTRY(AddressPtr, DataPtr, DataLength);
    
    TS_PARAM_UNUSED(AddressPtr);              /* Remove if parameter is used */
    TS_PARAM_UNUSED(DataPtr);                 /* Remove if parameter is used */
    TS_PARAM_UNUSED(DataLength);              /* Remove if parameter is used */
    
    /* insert your code here */
    
    DBG_XCP_APPLWRITEDATATORAM_EXIT(AddressPtr, DataPtr, DataLength, RetValue);
    return RetValue;
}

[!ENDIF!][!// "(XcpGeneral/XcpBlockWriteReadMemoryRAMMechanism = 'true') and (node:exists(XcpGeneral/XcpReadMemoryRAMCallback)) and not(node:empty(XcpGeneral/XcpReadMemoryRAMCallback))

[!IF "(XcpGeneral/XcpBlockWriteReadMemoryRAMMechanism = 'true') and (node:exists(XcpGeneral/XcpReadMemoryRAMCallback)) and not(node:empty(XcpGeneral/XcpReadMemoryRAMCallback))"!]

/*------------------[Callout for reading from RAM memory]------------------------------*/
FUNC(Xcp_ApplReturnType, XCP_APPL_CODE) Xcp_ApplReadDataFromRAM
(
  P2VAR( void, AUTOMATIC, XCP_APPL_DATA ) AddressPtr,
  P2CONST( uint8, AUTOMATIC, XCP_APPL_DATA ) DataPtr,
  uint8 DataLength
)
{
    Xcp_ApplReturnType RetValue = XCP_APPL_OK;

    DBG_XCP_APPLREADDATAFROMRAM_ENTRY(AddressPtr, DataPtr, DataLength);
    
    TS_PARAM_UNUSED(AddressPtr);               /* Remove if parameter is used */
    TS_PARAM_UNUSED(DataPtr);                  /* Remove if parameter is used */
    TS_PARAM_UNUSED(DataLength);               /* Remove if parameter is used */
    
    /* insert your code here */
    
    DBG_XCP_APPLREADDATAFROMRAM_EXIT(AddressPtr, DataPtr, DataLength, RetValue);
    return RetValue;
}
[!ENDIF!][!// "XcpGeneral/XcpBlockWriteReadMemoryRAMMechanism = 'true' and (node:exists(XcpGeneral/XcpReadMemoryRAMCallback)) and not(node:empty(XcpGeneral/XcpReadMemoryRAMCallback))

[!IF "(node:exists(XcpGeneral/XcpGetIdType1Callout) = 'true') and (XcpGeneral/XcpGetIdType1Callout = 'true')"!]

/*-----------[Callout for retrieving identification information for GET_ID command, type 1]-------------*/
FUNC(Xcp_ApplReturnType, XCP_APPL_CODE) Xcp_ApplGetIdentification
(
  uint8* IdBufferPtr,
  uint8* IdLength
)
{
  Xcp_ApplReturnType RetValue = XCP_APPL_OK;

  DBG_XCP_APPLGETIDENTIFICATION_ENTRY(IdBufferPtr, IdLength);

  TS_PARAM_UNUSED(IdBufferPtr);               /* Remove if parameter is used */
  TS_PARAM_UNUSED(IdLength);                  /* Remove if parameter is used */

  /* insert your code here */

  DBG_XCP_APPLGETIDENTIFICATION_Exit(IdBufferPtr, IdLength, RetValue);
  return RetValue;
}
[!ENDIF!][!// "XcpGeneral/XcpGetIdType1Callout = 'true'"

[!IF "XcpGeneral/XcpSynchReceivedCallout = 'true'"!]
FUNC(void, XCP_APPL_CODE) Xcp_ApplSynchReceived(void)
{
  /* insert your code here */
}
[!ENDIF!]

#define XCP_STOP_SEC_APPL_CODE
#include <Xcp_MemMap.h>

/*==================[internal function definitions]==========================*/

#endif /* ( (XCP_DEFAULT_XCP_MODULE_STATE == STD_ON) || (XCP_ENABLE_XCP_CONTROL_API == STD_ON) ) */
/*==================[end of file]============================================*/
