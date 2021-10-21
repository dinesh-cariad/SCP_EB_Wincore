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
/* 
 * MISRA-C:2012 Deviation List
 *
 */
#ifndef XCP_USERCALLOUTS_H
#define XCP_USERCALLOUTS_H

[!AUTOSPACING!]

/*==================[includes]===============================================*/

#include <Xcp.h>

/*==================[macros]=================================================*/

#if (defined XCP_APPL_OK)
#error XCP_APPL_OK already defined
#endif
/** \brief return value of XCP application callout for successful execution. */
#define XCP_APPL_OK                     0x01U

/* !LINKSTO Xcp.UserCallouts.RAMCallback.Busy,1 */
#if (defined XCP_APPL_BUSY)
#error XCP_APPL_BUSY already defined
#endif
/** \brief return value of XCP application callout for signaling the Xcp slave
 ** that the requested data is not yet available */
#define XCP_APPL_BUSY                     0x02U

#if (defined XCP_APPL_ERR_CMD_UNKNOWN)
#error XCP_APPL_ERR_CMD_UNKNOWN already defined
#endif
/** \brief return value of XCP application callout with XCP ERR_CMD_UNKNOWN error code. */
#define XCP_APPL_ERR_CMD_UNKNOWN        0x20U

#if (defined XCP_APPL_ERR_OUT_OF_RANGE)
#error XCP_APPL_ERR_OUT_OF_RANGE already defined
#endif
/** \brief return value of XCP application callout with XCP ERR_OUT_OF_RANGE error code. */
#define XCP_APPL_ERR_OUT_OF_RANGE       0x22U

#if (defined XCP_APPL_ERR_WRITE_PROTECTED)
#error XCP_APPL_ERR_WRITE_PROTECTED already defined
#endif
/** \brief return value of XCP application callout with XCP ERR_WRITE_PROTECTED error code. */
#define XCP_APPL_ERR_WRITE_PROTECTED    0x23U

#if (defined XCP_APPL_ERR_ACCESS_DENIED)
#error XCP_APPL_ERR_ACCESS_DENIED already defined
#endif
/** \brief return value of XCP application callout with XCP ERR_ACCESS_DENIED error code. */
#define XCP_APPL_ERR_ACCESS_DENIED      0x24U

#if (defined XCP_APPL_ERR_PAGE_NOT_VALID)
#error XCP_APPL_ERR_PAGE_NOT_VALID already defined
#endif
/** \brief return value of XCP application callout with XCP ERR_PAGE_NOT_VALID error code. */
#define XCP_APPL_ERR_PAGE_NOT_VALID     0x26U

#if (defined XCP_APPL_ERR_MODE_NOT_VALID)
#error XCP_APPL_ERR_MODE_NOT_VALID already defined
#endif
/** \brief return value of XCP application callout with XCP ERR_MODE_NOT_VALID error code. */
#define XCP_APPL_ERR_MODE_NOT_VALID     0x27U

#if (defined XCP_APPL_ERR_SEGMENT_NOT_VALID)
#error XCP_APPL_ERR_SEGMENT_NOT_VALID already defined
#endif
/** \brief return value of XCP application callout XCP ERR_SEGMENT_NOT_VALID error code. */
#define XCP_APPL_ERR_SEGMENT_NOT_VALID  0x28U

#if (defined XCP_APPL_ERR_MEMORY_OVERFLOW)
#error XCP_APPL_ERR_MEMORY_OVERFLOW already defined
#endif
/** \brief return value of XCP application callout with XCP ERR_MEMORY_OVERFLOW error code. */
#define XCP_APPL_ERR_MEMORY_OVERFLOW    0x30U

#if (defined XCP_APPL_ERR_GENERIC)
#error XCP_APPL_ERR_GENERIC already defined
#endif
/** \brief return value of XCP application callout with XCP ERR_GENERIC error code. */
#define XCP_APPL_ERR_GENERIC            0x31U

/** \brief Macro for address translation
 **
 ** Translates the given XCP address and address extension into the physical
 ** address to access.
 **
 ** \param[in] AddressExtension 8 bit XCP address extension
 ** \param[in] AddressPtr Pointer to the XCP address
 **
 ** \return Pointer to the physical address to access
 */
#if (defined XCP_APPL_GET_ADDRESS)
#error XCP_APPL_GET_ADDRESS already defined
#endif

[!IF "XcpGeneral/XcpAddressTranslationSupport = 'true'"!]
#define XCP_APPL_GET_ADDRESS( AddressExtension, AddressPtr ) \
  Xcp_ApplGetAddress( (AddressExtension), (AddressPtr) )
[!ELSE!]
#define XCP_APPL_GET_ADDRESS( AddressExtension, AddressPtr ) \
  (AddressPtr)
[!ENDIF!][!// "XcpGeneral/XcpAddressTranslationSupport = 'true'"

/*==================[type definitions]=======================================*/

/** \brief XCP application callout return type
 **
 ** This type provides the different return values to be used by the application
 ** callouts.
 */
typedef uint8 Xcp_ApplReturnType;


/*==================[external function declarations]=========================*/

[!IF "XcpGeneral/XcpBlockWriteReadMemoryRAMMechanism='true'"!]
#define XCP_START_SEC_CODE
#include <Xcp_MemMap.h>

/** \brief Function prototype providing the callback for informing Xcp that User API finished writing the data
 ** to RAM
 **
 ** This callout function shall provide the user specific writing data to RAM, if that feature is
 ** enabled (i.e. the configuration parameter XcpBlockWriteReadMemoryRAMMechanism is set to true).
 **
 ** \param[in] result  The result of the operation.
 ** \param[in] AddressPtr The address to which the data shall be written to RAM.
 ** \param[in] DataPtr Pointer to a data/buffer to be written.
 ** \param[in] DataLength Number of bytes to be written.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 ** */
extern FUNC(void, XCP_CODE) Xcp_ApplWriteDataToRAMFinished
(
  uint8 result,
  P2VAR( void, AUTOMATIC, XCP_VAR ) AddressPtr,
  P2CONST( uint8, AUTOMATIC, XCP_CONST ) DataPtr,
  uint8 DataLength
);

/** \brief Function prototype providing the callback for informing Xcp that User API finished reading the data
 ** from RAM
 **
 ** This callout function shall provide the user specific writing data to RAM, if that feature is
 ** enabled (i.e. the configuration parameter XcpBlockWriteReadMemoryRAMMechanism is set to true).
 **
 ** \param[in] result  The result of the operation.
 ** \param[in] AddressPtr The address to which the data shall be written to RAM.
 ** \param[in] DataPtr Pointer to a data/buffer to be written.
 ** \param[in] DataLength Number of bytes to be written.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 ** */
extern FUNC(void, XCP_CODE) Xcp_ApplReadDataFromRAMFinished
(
  uint8 result,
  P2VAR( void, AUTOMATIC, XCP_VAR ) AddressPtr,
  P2CONST( uint8, AUTOMATIC, XCP_CONST ) DataPtr,
  uint8 DataLength
);
#define XCP_STOP_SEC_CODE
#include <Xcp_MemMap.h>
[!ENDIF!]

#define XCP_START_SEC_APPL_CODE
#include <Xcp_MemMap.h>

[!IF "XcpGeneral/XcpSynchReceivedCallout = 'true'"!]
/* !LINKSTO Xcp.Xcp_ApplSynchReceived.Declaration, 1 */
/** \brief This callout function informs the application that a SYNCH command has been received by the Xcp.
 **
 ** One possible usage is to reset the internal application logic when parameter XcpBlockWriteReadMemoryRAMMechanism
 ** is set to TRUE and the access to RAM is implemented via functions configured in XcpWriteMemoryRAMCallback and 
 ** XcpReadMemoryRAMCallback.
 **/
extern FUNC(void, XCP_APPL_CODE) Xcp_ApplSynchReceived(void);
[!ENDIF!]

[!IF "(node:exists(XcpGeneral/XcpBlockWriteReadMemoryRAMMechanism) = 'true')"!]
[!IF "(node:exists(XcpGeneral/XcpWriteMemoryRAMCallback) and not(node:empty(XcpGeneral/XcpWriteMemoryRAMCallback)))"!]
/** \brief Function prototype providing the feature write data to RAM memory
 **
 ** This callout function shall provide the user specific writing data to RAM, if that feature is
 ** enabled (i.e. the configuration parameter XcpBlockWriteReadMemoryRAMMechanism is set to true).
 **
 ** Implementation of this function shall be provided by the user.
 ** This callout is not protected for data access concurrency issues. Therefore, the user shall
 ** implement the necessary countermeasures to protect copying data access against possible
 ** concurrency issues, if applicable.
 ** The file Xcp_Callouts.c from the templates folder can be used as a template.
 **
 ** \param[in] AddressPtr The address to which the data shall be written to RAM.
 ** \param[in] DataPtr Pointer to a data/buffer to be written.
 ** \param[in] DataLength Number of bytes to be written.
 **
 ** \return Function result
 ** \retval XCP_APPL_OK Function successful
 ** \retval XCP_APPL_ERR_OUT_OF_RANGE Parameter \a Segment is invalid.
 ** \retval XCP_ERR_CMD_UNKNOWN Unknown command or not implemented optional command
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 ** */
extern FUNC(Xcp_ApplReturnType, XCP_APPL_CODE) Xcp_ApplWriteDataToRAM
(
  P2VAR( void, AUTOMATIC, XCP_APPL_DATA ) AddressPtr,
  P2CONST( uint8, AUTOMATIC, XCP_APPL_DATA ) DataPtr,
  uint8 DataLength
);
[!ENDIF!][!// "(node:exists(XcpGeneral/XcpWriteMemoryRAMCallback) and not(node:empty(XcpGeneral/XcpWriteMemoryRAMCallback)))
[!IF "(node:exists(XcpGeneral/XcpReadMemoryRAMCallback) and not(node:empty(XcpGeneral/XcpReadMemoryRAMCallback)))"!]
/** \brief Function prototype providing the feature read data from RAM memory
 **
 ** This callout function shall provide the user specific reading data from RAM, if that feature is
 ** enabled (i.e. the configuration parameter XcpBlockWriteReadMemoryRAMMechanism is set to true).
 **
 ** Implementation of this function shall be provided by the user.
 ** This callout is not protected for data access concurrency issues. Therefore, the user shall
 ** implement the necessary countermeasures to protect copying data access against possible
 ** concurrency issues, if applicable.
 ** The file Xcp_Callouts.c from the templates folder can be used as a template.
 **
 ** \param[in] AddressPtr The address from where data shall be read from RAM.
 ** \param[in] DataPtr Pointer to a data/buffer to which the data shall be read.
 ** \param[in] DataLength Number of bytes to be read.
 **
 ** \return Function result
 ** \retval XCP_APPL_OK Function successful
 ** \retval XCP_APPL_ERR_OUT_OF_RANGE Parameter \a Segment is invalid.
 ** \retval XCP_APPL_ERR_ACCESS_DENIED The memory location is not accessible.
 ** \retval XCP_ERR_CMD_UNKNOWN Unknown command or not implemented optional command.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 ** */
extern FUNC(Xcp_ApplReturnType, XCP_APPL_CODE) Xcp_ApplReadDataFromRAM
(
  P2VAR( void, AUTOMATIC, XCP_APPL_DATA ) AddressPtr,
  P2CONST( uint8, AUTOMATIC, XCP_APPL_DATA ) DataPtr,
  uint8 DataLength
);
[!ENDIF!][!// "(node:exists(XcpGeneral/XcpReadMemoryRAMCallback) and not(node:empty(XcpGeneral/XcpReadMemoryRAMCallback)))
[!ENDIF!][!// "(node:exists(XcpGeneral/XcpBlockWriteReadMemoryRAMMechanism) = 'true')

[!IF "XcpGeneral/XcpBuildChecksumSupport = 'true' and XcpGeneral/XcpBuildChecksumType = 'XCP_CRC_USER_CALLOUT'"!]
/** \brief Callout function for command BUILD_CHECKSUM
 **
 ** This callout function shall be used to calculate the checksum for a given memory block.
 **
 ** Note: if the function returns a different value than those listed in the return value
 ** section, the Xcp module will assume that the command was not executed and send the error
 ** ERR_CMD_UNKNOWN as response to the master.
 **
 ** Implementation of this function shall be provided by the user.
 ** The file Xcp_Callouts.c from the templates folder can be used as a template.
 **
 ** \param[in] AddressPtr Starting address for calculating the checksum
 ** \param[in] BlockSize Block size in AG
 ** \param[out] ChecksumResult Will return the calculated checksum if ok OR the maximum allowed
 **             block size in case of a ERR_OUT_OF_RANGE error for the BlockSize parameter
 ** \param[out] ChecksumType Checksum type
 **
 ** \return Function result.
 ** \retval XCP_APPL_OK Function successful.
 ** \retval XCP_APPL_ERR_OUT_OF_RANGE Parameter \a BlockSize is invalid (too large).
 ** \retval XCP_APPL_ERR_ACCESS_DENIED Memory access not allowed.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Xcp_ApplReturnType, XCP_APPL_CODE) Xcp_ApplBuildChecksum
(
  P2CONST( void, AUTOMATIC, XCP_APPL_DATA ) AddressPtr,
  uint32 BlockSize,
  P2VAR( uint32, AUTOMATIC, XCP_APPL_DATA ) ChecksumResult,
  P2VAR( uint8, AUTOMATIC, XCP_APPL_DATA ) ChecksumType
);
[!ENDIF!][!// "XcpGeneral/XcpBuildChecksumSupport = 'true' and XcpGeneral/XcpBuildChecksumType = 'XCP_CRC_USER_CALLOUT'"

[!IF "(node:exists(XcpGeneral/XcpUserTimestampSupported) = 'true') and (XcpGeneral/XcpUserTimestampSupported = 'true')"!]
/** \brief Function prototype providing the timestamp
 **
 ** This callout function shall provide the user specific timestamp to the Xcp, if that feature is
 ** enabled (i.e. the configuration parameter XcpUserTimestampSupported is set to true).
 **
 ** Implementation of this function shall be provided by the user.
 ** The file Xcp_Callouts.c from the templates folder can be used as a template.
 **
 ** \return The Timestamp value
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 ** */
extern FUNC(Xcp_TimestampType, XCP_APPL_CODE) Xcp_ApplGetTimestamp
(
  void
);
[!ENDIF!][!// "(node:exists(XcpGeneral/XcpUserTimestampSupported) = 'true') and (XcpGeneral/XcpUserTimestampSupported = 'true')"

[!IF "XcpGeneral/XcpCalPagSupported = 'true'"!]
/** \brief Callout function for Calibration Page and Segment Initialization
 **
 ** This callout function shall do the initialization of all Segments and Pages.
 **
 ** Implementation of this function shall be provided by the user.
 ** The file Xcp_Callouts.c from the templates folder can be used as a template.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, XCP_APPL_CODE) Xcp_ApplCalPagInit
(
  void
);

/** \brief Callout function for command COPY_CAL_PAGE
 **
 ** This callout function shall copy the given source page to the given target page.
 **
 ** Note: if the function returns a different value than those listed in the return value
 ** section, the Xcp module will assume that the command was not executed and send the error
 ** ERR_CMD_UNKNOWN as response to the master.
 **
 ** Implementation of this callout function shall be provided by the user.
 ** The file Xcp_Callouts.c from the templates folder can be used as a template.
 **
 ** \param[in] SrcSegment Logical data segment number source
 ** \param[in] SrcPage Logical data page number source
 ** \param[in] DestSegment Logical data segment number destination
 ** \param[in] DestPage Logical data page number destination
 **
 ** \return Function result.
 ** \retval XCP_APPL_OK Function successful.
 ** \retval XCP_APPL_ERR_WRITE_PROTECTED Destination is write protected because it is a Flash segment.
 ** \retval XCP_APPL_ERR_PAGE_NOT_VALID Parameter \a SrcPage or \a DestPage is invalid.
 ** \retval XCP_APPL_ERR_SEGMENT_NOT_VALID Parameter \a SrcSegment or \a DestSegment is invalid.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Xcp_ApplReturnType, XCP_APPL_CODE) Xcp_ApplCopyCalPage
(
  uint8 SrcSegment,
  uint8 SrcPage,
  uint8 DestSegment,
  uint8 DestPage
);

/** \brief Callout function for command GET_PAG_PROCESSOR_INFO
 **
 ** This callout function provides the paging processor basic information:
 ** total number of available segments and if freezing mode is supported.
 **
 ** Note: if the function returns a different value than those listed in the return value
 ** section, the Xcp module will assume that the command was not executed and send the error
 ** ERR_CMD_UNKNOWN as response to the master.
 **
 ** Implementation of this function shall be provided by the user.
 ** The file Xcp_Callouts.c from the templates folder can be used as a template.
 **
 ** \param[out] MaxSegmentPtr Total number of available segments to be returned to the master
 ** \param[out] PagPropertiesPtr General properties for paging (freeze mode) to be returned to
 **             the master.
 **
 ** \return Function result.
 ** \retval XCP_APPL_OK Function successful.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Xcp_ApplReturnType, XCP_APPL_CODE) Xcp_ApplGetPagProcessorInfo
(
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) MaxSegmentPtr,
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) PagPropertiesPtr
);

/** \brief Callout function for command GET_CAL_PAGE
 **
 ** This callout function shall get the logical data page number belonging to the given logical
 ** segment (\a Segment) and to the \a Mode parameter, and provide it in the
 ** parameter \a PagePtr.
 **
 ** Note: if the function returns a different value than those listed in the return value
 ** section, the Xcp module will assume that the command was not executed and send the error
 ** ERR_CMD_UNKNOWN as response to the master.
 **
 ** Implementation of this function shall be provided by the user.
 ** The file Xcp_Callouts.c from the templates folder can be used as a template.
 **
 ** \param[in] Segment Logical data segment number
 ** \param[in] Mode Access mode
 ** \param[out] PagePtr Logical data page number to be returned to the master.
 **
 ** \return Function result.
 ** \retval XCP_APPL_OK Function successful.
 ** \retval XCP_APPL_ERR_MODE_NOT_VALID Parameter \a Mode is invalid.
 ** \retval XCP_APPL_ERR_SEGMENT_NOT_VALID Parameter \a Segment is invalid.
 ** \retval XCP_APPL_ERR_PAGE_NOT_VALID Parameter \a PagePtr is invalid.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Xcp_ApplReturnType, XCP_APPL_CODE) Xcp_ApplGetCalPage
(
  uint8 Segment,
  uint8 Mode,
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) PagePtr
);

/** \brief Callout function for command SET_CAL_PAGE
 **
 ** This callout function shall set the \a Page for the \a Segment as the active page for the
 ** indicated \a Mode access of that segment.
 **
 ** Note: if the function returns a different value than those listed in the return value
 ** section, the Xcp module will assume that the command was not executed and send the error
 ** ERR_CMD_UNKNOWN as response to the master.
 **
 ** Implementation of this function shall be provided by the user.
 ** The file Xcp_Callouts.c from the templates folder can be used as a template.
 **
 ** \param[in] Segment Logical data segment number
 ** \param[in] Page Logical data page number
 ** \param[in] Mode Mode
 **
 ** \return Function result.
 ** \retval XCP_APPL_OK Function successful.
 ** \retval XCP_APPL_ERR_PAGE_NOT_VALID Parameter \a Page is invalid.
 ** \retval XCP_APPL_ERR_MODE_NOT_VALID Parameter \a Mode is invalid.
 ** \retval XCP_APPL_ERR_SEGMENT_NOT_VALID Parameter \a Segment is invalid.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Xcp_ApplReturnType, XCP_APPL_CODE) Xcp_ApplSetCalPage
(
  uint8 Segment,
  uint8 Page,
  uint8 Mode
);

/** \brief Callout function for command GET_SEGMENT_MODE
 **
 ** This callout function shall provide information regarding that the \a Segment was or was not
 ** set for Freeze mode.
 **
 ** Note: if the function returns a different value than those listed in the return value
 ** section, the Xcp module will assume that the command was not executed and send the error
 ** ERR_CMD_UNKNOWN as response to the master.
 **
 ** Implementation of this function shall be provided by the user.
 ** The file Xcp_Callouts.c from the templates folder can be used as a template.
 **
 ** \param[in] Segment Logical data segment number
 ** \param[out] ModePtr (FREEZE Mode Disable/Enable) to be returned to the master.
 **             The Mode parameter ( \a ModePtr ) is a bit mask:
 **                      - Bit 7 - 1: Don't care;
 **                      - Bit 0 - FREEZE
 ** \return Function result.
 ** \retval XCP_APPL_OK Function successful.
 ** \retval XCP_APPL_ERR_OUT_OF_RANGE The length of the \a Segment parameter is invalid.
 ** \retval XCP_APPL_ERR_SEGMENT_NOT_VALID Parameter \a Segment is invalid.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Xcp_ApplReturnType, XCP_APPL_CODE) Xcp_ApplGetSegmentMode
(
  uint8 Segment,
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) ModePtr
);

/** \brief Callout function for command SET_SEGMENT_MODE
 **
 ** This callout function shall set the freeze mode flag of the segment \a Segment.
 **
 ** Note: if the function returns a different value than those listed in the return value
 ** section, the Xcp module will assume that the command was not executed and send the error
 ** ERR_CMD_UNKNOWN as response to the master.
 **
 ** Implementation of this function shall be provided by the user.
 ** The file Xcp_Callouts.c from the templates folder can be used as a template.
 **
 ** \param[in] Segment Logical data segment number
 ** \param[in] Mode (FREEZE Mode Disable/Enable)
 ** The \a Mode parameter is a bit mask:
 **             - Bit 7 - 1: Don't care;
 **             - Bit 0 - FREEZE:
 **                             - 0 = Disable FREEZE Mode;
 **                             - 1 = Enable FREEZE Mode.
 **
 ** \return Function result.
 ** \retval XCP_APPL_OK Function successful.
 ** \retval XCP_APPL_ERR_OUT_OF_RANGE Parameter \a Segment is invalid.
 ** \retval XCP_APPL_ERR_SEGMENT_NOT_VALID Parameter \a Segment is invalid (any of these two values
                                           shall be returned XCP_APPL_ERR_SEGMENT_NOT_VALID or
                                           XCP_APPL_ERR_OUT_OF_RANGE. The preferred value is
                                           XCP_APPL_ERR_OUT_OF_RANGE because this is specified
                                           by ASAM).
 ** \retval XCP_APPL_ERR_MODE_NOT_VALID Parameter Mode is invalid.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Xcp_ApplReturnType, XCP_APPL_CODE) Xcp_ApplSetSegmentMode
(
  uint8 Segment,
  uint8 Mode
);

/** \brief Callout function for command GET_SEGMENT_INFO
 **
 ** This callout function shall provide the segment information that was requested.
 ** That information shall be written to the address, to which \a CommandResponsePtr points to.
 **
 ** Note: if the function returns a different value than those listed in the return value
 ** section, the Xcp module will assume that the command was not executed and send the error
 ** ERR_CMD_UNKNOWN as response to the master.
 **
 ** Implementation of this function shall be provided by the user.
 ** The file Xcp_Callouts.c from the templates folder can be used as a template.
 **
 ** \param[in] Mode Type of information requested (basic, standard or address mapping info)
 ** \param[in] Segment Logical data segment number
 ** \param[in] SegmentInfo Address range information depending on the Mode input parameter
 ** \param[in] MappingIndex Identifier for address mapping range that MAPPING_INFO belongs to
 ** \param[out] CommandResponsePtr Command response buffer address.
 **    \a CommandResponsePtr parameter is the address of a 5-byte buffer where the callout function
 ** stores its response data / parameters depending on \a Mode, \a SegmentInfo and \a MappingIndex
 ** input parameters.
 **    - Depending on the Mode input parameter, this callout function shall provide the following
 ** data in the command response buffer:
 ** \li Mode 0:
 **     - Bytes 0-3: Basic Info (DWord) - Segment Address / Segment Length;
 **     - Byte 4: Reserved.
 ** \li Mode 1:
 **      - Byte 0: Number of Pages;
 **      - Byte 1: Address Extension for this segment;
 **      - Byte 2: Number of mapped address ranges for this segment;
 **      - Byte 3: Compression Method;
 **      - Byte 4: Encryption Method.
 ** \li Mode 2:
 **      - Bytes 0-3: Mapping Info (DWord) - Source Address / Destination Address / Length;
 **      - Byte 4: Reserved.
 **
 ** \return Function result.
 ** \retval XCP_APPL_OK Function successful.
 ** \retval XCP_APPL_ERR_MODE_NOT_VALID Parameter \a Mode is invalid.
 ** \retval XCP_APPL_ERR_SEGMENT_NOT_VALID Parameter \a Segment is invalid.
 ** \retval XCP_APPL_ERR_OUT_OF_RANGE Parameter \a Segment is invalid or parameter \a SegmentInfo
                                      is invalid for that mode (any of these two values shall be
                                      returned XCP_APPL_ERR_SEGMENT_NOT_VALID or
                                      XCP_APPL_ERR_OUT_OF_RANGE. The preferred value is
                                      XCP_APPL_ERR_OUT_OF_RANGE because this is specified by ASAM).
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Xcp_ApplReturnType, XCP_APPL_CODE) Xcp_ApplGetSegmentInfo
(
  uint8 Mode,
  uint8 Segment,
  uint8 SegmentInfo,
  uint8 MappingIndex,
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) CommandResponsePtr
);

/** \brief Callout function for command GET_PAGE_INFO
 **
 ** This callout function shall provide the properties and the INIT_SEGMENT of the
 ** \a Page and its associated \a Segment.
 ** This information shall be written to the following addresses:
 **        - \a PagePropertiesPtr and
 **        - \a InitSegmentPtr parameter.
 **
 ** Note: if the function returns a different value than those listed in the return value
 ** section, the Xcp module will assume that the command was not executed and send the error
 ** ERR_CMD_UNKNOWN as response to the master.
 **
 ** Implementation of this function shall be provided by the user.
 ** The file Xcp_Callouts.c from the templates folder can be used as a template.
 **
 ** \param[in] Segment Logical data segment number
 ** \param[in] Page Logical data page number
 ** \param[out] PagePropertiesPtr Page properties to be returned to the master
 ** \param[out] InitSegmentPtr Segment that initializes this Page
 **
 ** \return Function result.
 ** \retval XCP_APPL_OK Function successful.
 ** \retval XCP_APPL_ERR_SEGMENT_NOT_VALID Parameter \a Segment is invalid.
 ** \retval XCP_APPL_ERR_PAGE_NOT_VALID Parameter \a Page is invalid.
 ** \retval XCP_APPL_ERR_OUT_OF_RANGE Parameter \a Segment is invalid.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Xcp_ApplReturnType, XCP_APPL_CODE) Xcp_ApplGetPageInfo
(
  uint8 Segment,
  uint8 Page,
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) PagePropertiesPtr,
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) InitSegmentPtr
);

/** \brief Callout function for command SET_REQUEST - STORE_CAL_REQ mode
 **
 ** This callout function is a user specific function for the command SET_REQUEST if the flag
 ** STORE_CAL_REQ is set. It shall trigger the job of that command, i.e. the storage of
 ** the calibration data into the non-volatile memory and it should be done asynchronously not to
 ** block the Xcp.
 **
 ** Notes:
 **  - when the job is finished, i.e. the data are stored in the non-volatile memory,
 **    the callback function Xcp_SetReqStoreCalReqCbk() shall be called (e.g. by this
 **    callout function if it is synchronous or by some other function if it is asynchronous);
 **  - if the function returns a different value than those listed in the return value
 **    section, the Xcp module will assume that the command was not executed and send the
 **    error ERR_CMD_UNKNOWN as response to the master.
 **
 ** Implementation of this function shall be provided by the user.
 ** The file Xcp_Callouts.c from the templates folder can be used as a template.
 **
 ** \return Function result.
 ** \retval XCP_APPL_OK Function successful.
 ** \retval XCP_APPL_ERR_OUT_OF_RANGE Mode is not supported.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Xcp_ApplReturnType, XCP_APPL_CODE) Xcp_ApplSetReqStoreCalReq
(
  void
);

[!ENDIF!][!// "XcpGeneral/XcpCalPagSupported = 'true'"

[!IF "XcpGeneral/XcpAddressTranslationSupport = 'true'"!]
/** \brief Callout function for address translation
 **
 ** This callout function shall translate the given XCP address and the address extension
 ** into the physical address to access.
 **
 ** Implementation of this function shall be provided by the user.
 ** The file Xcp_Callouts.c from the templates folder can be used as a template.
 **
 ** \param[in] AddressExtension 8 bit XCP address extension. It is always called with value 0 as the
 **            address extension feature is not supported.
 ** \param[in] AddressPtr Pointer to the XCP address
 **
 ** \return Pointer to the physical address to access.
 **
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC_P2VAR(void, XCP_APPL_DATA, XCP_APPL_CODE) Xcp_ApplGetAddress
(
  uint8 AddressExtension,
  P2VAR(void, AUTOMATIC, XCP_APPL_DATA) AddressPtr
);
[!ENDIF!][!// "XcpGeneral/XcpAddressTranslationSupport = 'true'"

[!IF "node:exists(XcpConfig/*[1]/XcpMemoryAccessArea/XcpMemoryAccessAreasCallout) and XcpConfig/*[1]/XcpMemoryAccessArea/XcpMemoryAccessAreasCallout = 'true'"!]
/** \brief Callout function to check if access to the requested memory area is allowed
 **
 ** Implementation of this function shall be provided by the user.
 ** The file Xcp_Callouts.c from the templates folder can be used as a template.
 **
 ** \param[in] MemoryAreaAddress Start address of the memory area that needs to be validated
 ** \param[in] MemoryAreaLength Length of the memory area that needs to be validated
 ** \param[in] MemoryAreaType Type (read/write) of the memory area that needs to be validated
 ** \param[in] MemoryAreaScope Scope (commands/DAQ) of the memory area that needs to be validated
 **
 ** \return 
 ** \retval TRUE Requested memory area access is allowed
 ** \retval FALSE Requested memory area access is denied
 **
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(boolean, XCP_APPL_CODE) Xcp_ApplIsMemoryAreaAccessible
(
  P2CONST(uint8, AUTOMATIC, XCP_APPL_DATA) MemoryAreaAddress,
  uint32 MemoryAreaLength,
  uint8 MemoryAreaType,
  uint8 MemoryAreaScope
);
[!ENDIF!][!// "node:exists(XcpConfig/*[1]/XcpMemoryAccessArea/XcpMemoryAccessAreasCallout) and XcpConfig/*[1]/XcpMemoryAccessArea/XcpMemoryAccessAreasCallout = 'true'"

/** \brief Compare Key from the Master.
 **
 ** This function will be called by the XCP Slave when the key from master
 ** needs to be compared.
 ** Implementation of this function shall be provided by the user.
 ** The file Xcp_Callouts.c from the templates folder can be used as a reference.
 ** The function shall implement the algorithm being used by the master to generate the Key.
 **
 ** \param[in]  ResourceIdentifier  A Resource to be unlocked.
 ** \param[in]  KeyBufferPtr        Pointer to the key to be verified.
 ** \param[in]  KeyLength           Length of the key RAM.
 **
 ** \return Function result
 ** \retval E_OK Function successful
 ** \retval E_NOT_OK Function not successful
 **
 ** \Reentrancy{Reentrant for different ResourceIdentifier. Non-reentrant for the same ResourceIdentifier}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, XCP_APPL_CODE) Xcp_ApplCompareKey
(
  uint8 ResourceIdentifier,
  P2CONST(uint8, AUTOMATIC, XCP_APPL_DATA) KeyBufferPtr,
  uint8 KeyLength
);

/** \brief Generate Seed for a resource.
 **
 ** This function is called by the XCP slave to generate seed for a requested resource.
 ** Implementation of this function shall be provided by the user.
 ** The file Xcp_Callouts.c from the templates folder can be used as a reference.
 ** The function shall implement the algorithm being used by the master to generate the Seed.
 ** When requesting for a resource the user shall make sure that the resource is supported 
 ** and allowed.
 ** Any attempt to unlock a not supporter resource shall return E_NOT_OK.
 **
 ** \param[in]     ResourceIdentifier    A Resource to be unlocked.
 ** \param[in,out]  SeedBufferPtr        Pointer to the RAM area where the seed needs to be stored.
 ** \param[in,out]  SeedBufferLengthPtr  Pointer to the RAM area where the seed length needs to be stored.
 **
 ** \return Function result
 ** \retval E_OK Function successful
 ** \retval E_NOT_OK Function not successful
 **
 ** \Reentrancy{Reentrant for different ResourceIdentifier. Non-reentrant for the same ResourceIdentifier}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, XCP_APPL_CODE) Xcp_ApplGetSeed
(
  uint8 ResourceIdentifier,
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) SeedBufferPtr,
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) SeedBufferLengthPtr
);

[!IF "XcpGeneral/XcpPgmSupported = 'true'"!]
/** \brief Callout function for the command PROGRAM_START
 **
 ** This function is called by the Xcp whenever the command PROGRAM_START is
 ** executed. Use this callout to setup the programming (memory programming may have implementation
 ** specific preconditions (slave device in a secure physical state, additional code
 ** downloaded, ...).
 **
 ** \param[out] ErrorCodePtr Pointer to write the slave specific error code to. Value is used only
 ** to fill the error message ERR_GENERIC. If XCP_APPL_ERR_GENERIC is not thrown by the callout
 ** function this parameter is ignored.
 **
 ** \return Function result
 ** \retval XCP_APPL_OK Function successful
 ** \retval XCP_APPL_ERR_GENERIC The slave device is not in a state which permits programming
 */
extern FUNC(Xcp_ApplReturnType, XCP_APPL_CODE) Xcp_ApplProgramStart
(
  P2VAR( uint16, AUTOMATIC, XCP_APPL_DATA ) ErrorCodePtr
);

/** \brief Callout function for the command PROGRAM_CLEAR
 **
 ** This function is called by the Xcp whenever the command PROGRAM_CLEAR is
 ** executed. Use this function to clear a part of non-volatile memory prior to reprogramming.
 **
 ** \param[in] AddressPtr Pointer to the address of the flash block to be
 **                       cleared
 ** \param[in] ClearRange Length, in bytes, of the block to be cleared
 **
 ** \return Function result
 ** \retval XCP_APPL_OK Function successful
 ** \retval XCP_APPL_ERR_OUT_OF_RANGE Parameter is invalid
 ** \retval XCP_APPL_ERR_ACCESS_DENIED Memory access not allowed
 */
extern FUNC(Xcp_ApplReturnType, XCP_APPL_CODE) Xcp_ApplProgramClear
(
  P2VAR( void, AUTOMATIC, XCP_APPL_DATA ) AddressPtr,
  uint32 ClearRange
);

/** \brief Callout function for the command PROGRAM
 **
 ** This function is called by the Xcp in the following cases, depending on the communication mode:
 ** - normal mode: with each execution of PROGRAM or PROGRAM_MAX commands
 ** - block mode: at the end of an entire block mode sequence (i.e. the last PROGRAM_NEXT) and each
 **   time a PROGRAM with number of bytes to be programmed = 0 is called.
 **
 ** This function should do the actual programming.
 **
 ** When this callout is called with DataLength = 0, it means the end of the programming for the
 ** current segment.
 **
 ** \param[in] AddressPtr Pointer to the address of the flash block to be
 **                       programmed.
 ** \param[in] DataPtr Pointer to a data buffer to be programmed.
 ** \param[in] DataLength Length of the buffer to be programmed (in bytes).
 **
 ** \return Function result
 ** \retval XCP_APPL_OK Function successful
 ** \retval XCP_APPL_ERR_OUT_OF_RANGE Parameter is invalid
 ** \retval XCP_APPL_ERR_ACCESS_DENIED Memory access not allowed
 ** \retval XCP_APPL_ERR_MEMORY_OVERFLOW Memory overflow during execution
 */
extern FUNC(Xcp_ApplReturnType, XCP_APPL_CODE) Xcp_ApplProgram
(
  P2VAR( void, AUTOMATIC, XCP_APPL_DATA ) AddressPtr,
  P2CONST( uint8, AUTOMATIC, XCP_APPL_DATA ) DataPtr,
  uint16 DataLength
);

/** \brief Callout function for the command PROGRAM_RESET
 **
 ** This function is called by the Xcp whenever the command PROGRAM_RESET is executed.
 **
 */
extern FUNC(void, XCP_APPL_CODE) Xcp_ApplProgramReset( void );

/** \brief Callout function for command GET_PGM_PROCESSOR_INFO
 **
 ** This callout function provides the programming processor basic information:
 ** \li total number of available sectors
 ** \li and programming general properties:
 ** - the available mode[s];
 ** - data compression properties;
 ** - data encryption properties;
 ** - and message sequence processing properties.
 **
 ** Note: if the function returns a different value than those listed in the return value
 ** section, the Xcp module will assume that the command was not executed and send the error
 ** ERR_CMD_UNKNOWN as response to the master.
 **
 ** Implementation of this function shall be provided by the user.
 ** The file Xcp_Callouts.c from the templates folder can be used as a template.
 **
 ** \param[out] MaxSectorPtr Total number of available sectors to be returned to the master
 ** \param[out] PgmPropertiesPtr General properties for programming to be returned to
 **             the master. The programming mode bits will be masked to only contain those
 **             modes which are supported by the Xcp (only ABSOLUTE_MODE is supported by the Xcp).
 **
 ** \return Function result.
 ** \retval XCP_APPL_OK Function successful.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Xcp_ApplReturnType, XCP_APPL_CODE) Xcp_ApplGetPgmProcessorInfo
(
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) MaxSectorPtr,
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) PgmPropertiesPtr
);

/** \brief Callout function for command GET_SECTOR_INFO
 **
 ** This callout function shall provide the sector information that was requested.
 ** That information shall be written to the address, to which \a CommandResponsePtr points to.
 **
 ** Note: if the function returns a different value than those listed in the return value
 ** section, the Xcp module will assume that the command was not executed and send the error
 ** ERR_CMD_UNKNOWN as response to the master.
 **
 ** Implementation of this function shall be provided by the user.
 ** The file Xcp_Callouts.c from the templates folder can be used as a template.
 **
 ** Implementation hints: The following ASAM requirement has to fulfilled by this callout:
 **   "The clear and programming sequence number of a flash sector shall be unique and continuous."
 **
 ** \param[in] Mode Type of information requested for the given Sector number (start address,
 **            length [BYTE] or name length).
 ** \param[in] Sector The Sector number for which information is requested.
 ** \param[out] MtaPtr Optional parameter used to get the MTA of the SECTOR name.
 **    \a MtaPtr parameter is the address of a 4-byte buffer containing the location address from
 **     which the Master may upload the SECTOR name as ASCII text, using one or more UPLOAD
 **     commands.
 **     Note: This parameter is mandatory in case \a Mode equals 2.
 ** \param[out] CommandResponsePtr Command response buffer address:
 **     - a 7-byte buffer, in case one of the following sector information is requested: the
 **       start address or length [BYTE], or
 **     - a 1-byte buffer, in case the name length sector information is requested;
 **
 **    where, the callout function stores its response data depending on \a Mode input parameters.
 **    - Depending on the Mode input parameter, this callout function shall provide the following
 ** data in the command response buffer:
 ** \li Mode 0:
 **     - Byte 0: Clear Sequence Number;
 **     - Byte 1: Program Sequence Number;
 **     - Byte 2: Programming method.
 **     - Bytes 3-6: Start address for this Sector (DWORD).
 ** \li Mode 1:
 **     - Byte 0: Clear Sequence Number;
 **     - Byte 1: Program Sequence Number;
 **     - Byte 2: Programming method.
 **     - Bytes 3-6: Length of this Sector [Byte] (DWORD).
 ** \li Mode 2:
 **     - Byte 0: SECTOR_NAME_LENGTH in bytes or 0 if not available;
 **
 ** \return Function result.
 ** \retval XCP_APPL_OK Function successful.
 ** \retval XCP_APPL_ERR_MODE_NOT_VALID Parameter \a Mode is invalid.
 ** \retval XCP_APPL_ERR_SEGMENT_NOT_VALID Parameter \a Sector is invalid.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Xcp_ApplReturnType, XCP_APPL_CODE) Xcp_ApplGetSectorInfo
(
  uint8 Mode,
  uint8 Sector,
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) MtaPtr,
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) CommandResponsePtr
);

[!ENDIF!][!// "XcpGeneral/XcpPgmSupported = 'true'

[!IF "(node:exists(XcpGeneral/XcpGetIdType1Callout) = 'true') and (XcpGeneral/XcpGetIdType1Callout = 'true')"!]

/** \brief Callout function for command GET_ID, type 1
 **
 ** This callout function shall retrieve the identification information for the command
 ** GET_ID type 1.
 **
 ** Note: if the function returns a different value than XCP_APPL_OK,
 ** the Xcp module will return a positive response with length
 ** field set to 0.
 **
 ** Implementation of this function shall be provided by the user.
 **
 ** \param[in] IdBufferPtr - pointer where identification information shall be written.
 ** \param[out] IdLength - length of the provided identification information
 **
 ** \return Function result.
 ** \retval XCP_APPL_OK Operation successful and values for the parameters were properly updated.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Xcp_ApplReturnType, XCP_APPL_CODE) Xcp_ApplGetIdentification
(
  uint8* IdBufferPtr,
  uint8* IdLength
);

[!ENDIF!][!// "XcpGeneral/XcpGetIdType1Callout = 'true'"

#define XCP_STOP_SEC_APPL_CODE
#include <Xcp_MemMap.h>

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef XCP_USERCALLOUTS_H */
/*==================[end of file]============================================*/
