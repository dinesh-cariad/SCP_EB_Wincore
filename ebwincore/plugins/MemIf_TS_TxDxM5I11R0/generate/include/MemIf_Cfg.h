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
#ifndef MEMIF_CFG_H
#define MEMIF_CFG_H
[!VAR "NumDevices"="num:i(MemIfGeneral/MemIfNumberOfDevices)"!][!//
[!//
[!/*
 Check if the Ea and Fee is able to allow multiple instanciations
 if there are multiple configurations for these modules.
*/!][!//
[!MACRO "HEADER_NAME", "ModName" = "'Ea'"!][!//
[!VAR "Infix"!][!//
[!IF "node:exists(CommonPublishedInformation/VendorApiInfix)"!][!//
[!"CommonPublishedInformation/VendorApiInfix"!][!//
[!ENDIF!][!//
[!ENDVAR!][!//
[!//
[!"$ModName"!][!//
[!IF "$Infix != ''"!][!//
_[!"CommonPublishedInformation/VendorId"!]_[!"$Infix"!][!//
[!ENDIF!][!//
.h[!//
[!ENDMACRO!][!//
[!//
[!MACRO "EXTEND_NAME", "ModName" = "'Ea'"!][!//
[!IF "node:exists(CommonPublishedInformation/VendorApiInfix)"!][!//
[!VAR "Name" = "concat($ModName,'_',CommonPublishedInformation/VendorId,'_',CommonPublishedInformation/VendorApiInfix)"!][!//
[!ELSE!][!//
[!VAR "Name" = "$ModName"!][!//
[!ENDIF!][!//
[!ENDMACRO!][!//
[!//
[!/* *** first build prefix-list *** */!][!//
[!VAR "Prefixes"="''"!][!//
[!VAR "OldDeviceIndex" ="-1"!][!//
[!VAR "NumberOfDevices"="0"!][!//
[!VAR "DEVICE_IDs" = "''"!][!//
[!LOOP "as:modconf('Eep')"!][!//
[!VAR "NumberOfDevices"="$NumberOfDevices+1"!][!//
[!IF "contains($DEVICE_IDs, concat(EepGeneral/EepDriverIndex,'_') )"!][!//
[!ERROR "#error There may not be two memory drivers with the same device ID. Please check EepGeneral/EepDriverIndex"!][!//
[!BREAK!][!//	
[!ENDIF!][!//
[!VAR "DEVICE_IDs" = "concat($DEVICE_IDs,EepGeneral/EepDriverIndex,'_')"!][!// 	
[!ENDLOOP!][!//
[!LOOP "as:modconf('Fls')"!][!//
[!VAR "NumberOfDevices"="$NumberOfDevices+1"!][!//
[!IF "contains($DEVICE_IDs, concat(FlsGeneral/FlsDriverIndex,'_') )"!][!//
[!ERROR "#error There may not be two memory drivers with the same device ID. Please check FlsGeneral/FlsDriverIndex"!][!//
[!BREAK!][!//
[!ENDIF!][!//
[!VAR "DEVICE_IDs" = "concat($DEVICE_IDs,FlsGeneral/FlsDriverIndex,'_')"!][!// 	
[!ENDLOOP!][!//
[!/*
 The relation
 MemIfGeneral/MemIfNumberOfDevices
   = count(as:modconf('Ea')) + count(as:modconf('Fee'))
 is also enforced by the xdm file. 
*/!][!//
  [!IF "ceiling($NumDevices) != (count(as:modconf('Ea')) + count(as:modconf('Fee')))"!][!//
  [!ERROR "Number of devices must match the configured Ea and Fee devices"!][!//
  [!ENDIF!][!//
[!IF "$NumberOfDevices > 1"!][!//
[!VAR "Driver"="''"!][!//
[!VAR "ListDrivers"="''"!][!//
[!/* *** If more than 1 device is present perform validity checks *** */!][!//
/* !LINKSTO EB_MEMIF_018_3,1  */
/* !LINKSTO EB_MEMIF_018_4,1  */
[!LOOP "as:modconf('Ea')"!][!//
[!/* *** All the blocks of Ea must link the same driver *** */!][!//
[!VAR "Driver" = "EaBlockConfiguration/*/EaDeviceIndex"!][!//
[!IF "contains($ListDrivers,$Driver)"!][!//
#error MEMIF cannot map two Hardware abstraction modules to the same driver! [!"$Driver"!] referenced twice!
[!BREAK!][!//  
[!ENDIF!][!// 	   
[!VAR "ListDrivers" = "concat($ListDrivers,'_',$Driver)"!][!//    
[!LOOP "EaBlockConfiguration/*"!][!//	     
[!IF "''!=( ./EaDeviceIndex )"!][!// 		 
[!IF "$Driver != ./EaDeviceIndex "!][!//		
#error A hardware abstraction module may not have blocks in two driver modules References unequal [!"$Driver"!] != [!"(./EaDeviceIndex)"!]
[!BREAK!][!//  
[!ENDIF!][!// 
[!ELSE!][!//
#error field EaDeviceIndex must hold Reference to the device this block is stored in.
[!ENDIF!][!//
[!ENDLOOP!][!//  
[!ENDLOOP!][!//
[!LOOP "as:modconf('Fee')"!][!//
[!/* *** All the blocks of Ea must link the same driver *** */!][!//
[!VAR "Driver" = "FeeBlockConfiguration/*/FeeDeviceIndex"!][!//
[!IF "contains($ListDrivers,$Driver)"!][!//
#error MEMIF cannot map two Hardware abstraction modules to the same driver! [!"$Driver"!] referenced twice!
[!BREAK!][!//  
[!ENDIF!][!//
[!VAR "ListDrivers" = "concat($ListDrivers,'_',$Driver)"!][!//    
[!LOOP "FeeBlockConfiguration/*"!][!//	     
[!IF "''!=( ./FeeDeviceIndex )"!][!//
[!IF "$Driver != ./FeeDeviceIndex "!][!//
#error A hardware abstraction module may not have blocks in two driver modules References unequal [!"$Driver"!] != [!"(./FeeDeviceIndex)"!]
[!BREAK!][!//
[!ENDIF!][!// 
[!ELSE!][!//
#error field FeeDeviceIndex must hold Reference to the device this block is stored in.
[!ENDIF!][!//
[!ENDLOOP!][!//  
[!ENDLOOP!][!//
[!ENDIF!][!//
[!FOR "k" = "1" TO "$NumberOfDevices"!][!//
[!VAR "NewDeviceIndex" = "256"!][!//
[!VAR "Name"="''"!][!//
[!VAR "Infix"= "''"!][!//
[!LOOP "as:modconf('Eep')"!][!//
[!IF "($NewDeviceIndex > EepGeneral/EepDriverIndex) and ( $OldDeviceIndex < EepGeneral/EepDriverIndex )"!][!//
[!VAR "NewDeviceIndex" = "EepGeneral/EepDriverIndex"!][!//
[!VAR "Name"="''"!][!//
[!LOOP "as:modconf('Ea')"!][!//
[!IF "$NewDeviceIndex = node:value(node:ref((.)/EaBlockConfiguration/*/EaDeviceIndex)/EepDriverIndex)"!][!//
[!CALL "EXTEND_NAME", "ModName" = "'Ea'"!][!//
[!ENDIF!][!//
[!ENDLOOP!][!//
[!ENDIF!][!//
[!ENDLOOP!][!//
[!LOOP "as:modconf('Fls')"!][!//
[!IF "($NewDeviceIndex > FlsGeneral/FlsDriverIndex) and ( $OldDeviceIndex < FlsGeneral/FlsDriverIndex )"!][!//
[!VAR "NewDeviceIndex" = "FlsGeneral/FlsDriverIndex"!][!//
[!VAR "Name"="''"!][!//
[!LOOP "as:modconf('Fee')"!][!//
[!IF "$NewDeviceIndex = node:value(node:ref((.)/FeeBlockConfiguration/*/FeeDeviceIndex)/FlsDriverIndex)"!][!//
[!CALL "EXTEND_NAME", "ModName" = "'Fee'"!][!//
[!ENDIF!][!//
[!ENDLOOP!][!//
[!ENDIF!][!//
[!ENDLOOP!][!//
[!/* *** Fill missing indexes with null pointer so NVM and driver indexes match *** */!][!//
[!FOR "z" = "$OldDeviceIndex+2" TO "$NewDeviceIndex"!][!//
[!VAR "Prefixes" = "concat($Prefixes,';&',"",'_')"!][!//
[!ENDFOR!][!//
[!VAR "Prefixes" = "concat($Prefixes,';&',$Name,'_')"!][!//
[!VAR "OldDeviceIndex" = "$NewDeviceIndex"!][!//
[!ENDFOR!][!//
[!VAR "Prefixes" = "substring-after($Prefixes, ';')"!][!//
/*==================[includes]==============================================*/

/* !LINKSTO MemIf037,1 */
#include <Std_Types.h>                            /* AUTOSAR standard types     */
#include <TSAutosar.h>                            /* EB specific standard types */

/* include lower layer headers */
[!LOOP "as:modconf('Ea')"!][!//
#include <[!CALL "HEADER_NAME", "ModName" = "'Ea'"!]>
[!ENDLOOP!][!//
[!//
[!LOOP "as:modconf('Fee')"!][!//
#include <[!CALL "HEADER_NAME", "ModName" = "'Fee'"!]>
[!ENDLOOP!][!//

/*==================[macros]================================================*/
#if (defined MEMIF_DEV_ERROR_DETECT)
#error MEMIF_DEV_ERROR_DETECT is already defined
#endif /* if (defined MEMIF_DEV_ERROR_DETECT) */
/** \brief En-/disable development error tracer checks */
#define MEMIF_DEV_ERROR_DETECT   [!//
[!IF "MemIfGeneral/MemIfDevErrorDetect = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


#if (defined MEMIF_MAXIMUM_NUMBER_OF_DEVICES)
#error MEMIF_MAXIMUM_NUMBER_OF_DEVICES is already defined
#endif /* if (defined MEMIF_MAXIMUM_NUMBER_OF_DEVICES) */
/** \brief Maximum number of the underlying memory abstraction modules
 **
 ** Largest Driver Index or 1 if only one abstraction module is present */
[!IF "$NumDevices > 1"!][!//
 #define MEMIF_MAXIMUM_NUMBER_OF_DEVICES  [!"num:dectoint($OldDeviceIndex+1)"!]U
[!ELSE!][!//
 #define MEMIF_MAXIMUM_NUMBER_OF_DEVICES 1U
[!ENDIF!][!//
#if (defined MEMIF_VERSION_INFO_API)
#error MEMIF_VERSION_INFO_API is already defined
#endif /* if (defined MEMIF_VERSION_INFO_API) */
/** \brief En-/disable API version information */
#define MEMIF_VERSION_INFO_API   [!//
[!IF "MemIfGeneral/MemIfVersionInfoApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/* !LINKSTO MemIf019,1 */
[!IF "$NumDevices = 1"!][!//
[!/* *** determine device prefix *** */!][!//
[!LOOP "as:modconf('Ea')"!][!//
[!VAR "Prefix"!]Ea_[!ENDVAR!][!//
[!ENDLOOP!][!//
[!LOOP "as:modconf('Fee')"!][!//
[!VAR "Prefix"!]Fee_[!ENDVAR!][!//
[!ENDLOOP!][!//
[!//

/*
 * Since only one underlaying device is configured, the partmeter DeviceIndex is not used therefore
 * it is cast to void.
 * Direct cast to void is done in MemIf to avoid compilation errors in case of static code checking
 * enabled (EB_STATIC_CHECK is defined).
 */

/* Macro mappings of driver API calls */

#if (defined MemIf_Read)
#error MemIf_Read is already defined
#endif /* if (defined MemIf_Read) */
#define MemIf_Read(DeviceIndex, BlockNumber, BlockOffset, DataBufferPtr, Length) \
  (((void) (DeviceIndex)),[!"$Prefix"!]Read(BlockNumber, BlockOffset, DataBufferPtr, Length))

#if (defined MemIf_Write)
#error MemIf_Write is already defined
#endif /* if (defined MemIf_Write) */
#define MemIf_Write(DeviceIndex,  BlockNumber, DataBufferPtr) \
  (((void) (DeviceIndex)),[!"$Prefix"!]Write(BlockNumber, DataBufferPtr))

#if (defined MemIf_Cancel)
#error MemIf_Cancel is already defined
#endif /* if (defined MemIf_Cancel) */
#define MemIf_Cancel(DeviceIndex) \
  (((void) (DeviceIndex)),[!"$Prefix"!]Cancel())

#if (defined MemIf_GetStatus)
#error MemIf_GetStatus is already defined
#endif /* if (defined MemIf_GetStatus) */
#define MemIf_GetStatus(DeviceIndex) \
  (((void) (DeviceIndex)),[!"$Prefix"!]GetStatus())

#if (defined MemIf_GetJobResult)
#error MemIf_GetJobResult is already defined
#endif /* if (defined MemIf_GetJobResult) */
#define MemIf_GetJobResult(DeviceIndex) \
  (((void) (DeviceIndex)),[!"$Prefix"!]GetJobResult())

#if (defined MemIf_InvalidateBlock)
#error MemIf_InvalidateBlock is already defined
#endif /* if (defined MemIf_InvalidateBlock) */
#define MemIf_InvalidateBlock(DeviceIndex, BlockNumber) \
  (((void) (DeviceIndex)),[!"$Prefix"!]InvalidateBlock(BlockNumber))

#if (defined MemIf_EraseImmediateBlock)
#error MemIf_EraseImmediateBlock is already defined
#endif /* if (defined MemIf_EraseImmediateBlock) */
#define MemIf_EraseImmediateBlock(DeviceIndex, BlockNumber) \
  (((void) (DeviceIndex)),[!"$Prefix"!]EraseImmediateBlock(BlockNumber))

#if (defined MemIf_SetMode)
#error MemIf_SetMode is already defined
#endif /* if (defined MemIf_SetMode) */
#define MemIf_SetMode(Mode) \
  [!"$Prefix"!]SetMode(Mode)
[!//
[!ENDIF!][!/* NumDevices */!][!//

/* !LINKSTO MemIf020,1 */
[!IF "$NumDevices > 1"!][!//

#if (defined MEMIF_SETMODE_FCTPTR)
#error MEMIF_SETMODE_FCTPTR is already defined
#endif /* if (defined MEMIF_SETMODE_FCTPTR) */
/* function pointers for the function pointer arrays */
#define MEMIF_SETMODE_FCTPTR \
{\
[!LOOP "text:split($Prefixes, ';')"!][!//
 [!IF "(.) != '&_'"!][!//
[!"."!]SetMode,\
 [!ELSE!][!//
NULL_PTR,\
[!ENDIF!][!ENDLOOP!][!//
}

#if (defined MEMIF_READ_FCTPTR)
#error MEMIF_READ_FCTPTR is already defined
#endif /* if (defined MEMIF_READ_FCTPTR) */
#define MEMIF_READ_FCTPTR \
{\
[!LOOP "text:split($Prefixes, ';')"!][!//
 [!IF "(.) != '&_'"!][!//
[!"."!]Read,\
 [!ELSE!][!//
NULL_PTR,\
[!ENDIF!][!ENDLOOP!][!//
}

#if (defined MEMIF_WRITE_FCTPTR)
#error MEMIF_WRITE_FCTPTR is already defined
#endif /* if (defined MEMIF_WRITE_FCTPTR) */
#define MEMIF_WRITE_FCTPTR \
{\
[!LOOP "text:split($Prefixes, ';')"!][!//
 [!IF "(.) != '&_'"!][!//
[!"."!]Write,\
 [!ELSE!][!//
NULL_PTR,\
[!ENDIF!][!ENDLOOP!][!//
}

#if (defined MEMIF_CANCEL_FCTPTR)
#error MEMIF_CANCEL_FCTPTR is already defined
#endif /* if (defined MEMIF_CANCEL_FCTPTR) */
#define MEMIF_CANCEL_FCTPTR \
{\
[!LOOP "text:split($Prefixes, ';')"!][!//
 [!IF "(.) != '&_'"!][!//
[!"."!]Cancel,\
 [!ELSE!][!//
NULL_PTR,\
[!ENDIF!][!ENDLOOP!][!//
}

#if (defined MEMIF_GETSTATUS_FCTPTR)
#error MEMIF_GETSTATUS_FCTPTR is already defined
#endif /* if (defined MEMIF_GETSTATUS_FCTPTR) */
#define MEMIF_GETSTATUS_FCTPTR \
{\
[!LOOP "text:split($Prefixes, ';')"!][!//
 [!IF "(.) != '&_'"!][!//
[!"."!]GetStatus,\
 [!ELSE!][!//
NULL_PTR,\
[!ENDIF!][!ENDLOOP!][!//
}

#if (defined MEMIF_GETJOBRESULT_FCTPTR)
#error MEMIF_GETJOBRESULT_FCTPTR is already defined
#endif /* if (defined MEMIF_GETJOBRESULT_FCTPTR) */
#define MEMIF_GETJOBRESULT_FCTPTR \
{\
[!LOOP "text:split($Prefixes, ';')"!][!//
 [!IF "(.) != '&_'"!][!//
[!"."!]GetJobResult,\
 [!ELSE!][!//
NULL_PTR,\
[!ENDIF!][!ENDLOOP!][!//
}

#if (defined MEMIF_INVALIDATEBLOCK_FCTPTR)
#error MEMIF_INVALIDATEBLOCK_FCTPTR is already defined
#endif /* if (defined MEMIF_INVALIDATEBLOCK_FCTPTR) */
#define MEMIF_INVALIDATEBLOCK_FCTPTR \
{\
[!LOOP "text:split($Prefixes, ';')"!][!//
 [!IF "(.) != '&_'"!][!//
[!"."!]InvalidateBlock,\
 [!ELSE!][!//
NULL_PTR,\
[!ENDIF!][!ENDLOOP!][!//
}

#if (defined MEMIF_ERASEIMMEDIATEBLOCK_FCTPTR)
#error MEMIF_ERASEIMMEDIATEBLOCK_FCTPTR is already defined
#endif /* if (defined MEMIF_ERASEIMMEDIATEBLOCK_FCTPTR) */
#define MEMIF_ERASEIMMEDIATEBLOCK_FCTPTR \
{\
[!LOOP "text:split($Prefixes, ';')"!][!//
 [!IF "(.) != '&_'"!][!//
[!"."!]EraseImmediateBlock,\
 [!ELSE!][!//
NULL_PTR,\
[!ENDIF!][!ENDLOOP!][!//
}

[!ENDIF!][!//

/*==================[type definitions]======================================*/

/*==================[external data]=========================================*/

#endif /* ifndef MEMIF_CFG_H */
/*==================[end of file]===========================================*/
