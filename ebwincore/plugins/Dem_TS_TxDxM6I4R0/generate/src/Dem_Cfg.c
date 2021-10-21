/**
 * \file
 *
 * \brief AUTOSAR Dem
 *
 * This file contains the implementation of the AUTOSAR
 * module Dem.
 *
 * \version 6.4.1
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* This file contains all definitions of the configuration of the AUTOSAR
 * module Dem that are target independent. */

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 11.1 (required)
 *     Conversions shall not be performed between a pointer to a function and
 *     any other type.
 *
 *     Reason:
 *     In order to save memory, the function pointers necessary for Read Callbacks are generated
 *     as an array of unions (union type which contains all such possible function pointers),
 *     instead of an array of structures containing all possible pointer types.
 *     The array containing the function pointers is initialized using brace-enclosed initializer
 *     list and C90 standard allow only the initialization of the first union member.
 *     The solution is to use a "generic function pointer" and use that type to define the array,
 *     and use explicit casts for each initialized member.
 *     Casting one function pointer type to the general type at initialization and then back again
 *     to the original type is guaranteed to preserve the correct data.
 */

[!AUTOSPACING!]
[!INCLUDE "../../generate_common/Dem_Include.m"!]
[!//
/*==================[inclusions]============================================*/
/* !LINKSTO dsn.Dem.IncludeStr,1 */

#include <TSAutosar.h>                    /* EB specific standard types */
#include <Std_Types.h>                    /* AUTOSAR standard types */
#include <Dem_Int.h>                      /* API and internal declarations,
                                           * dependent and static part */
[!CALL "CALCULATE_MULTICORE_SETTINGS"!][!//
[!//
[!INDENT "0"!]
  [!IF "$DemBSWDistributionOfEventReports = 'true'"!]
    [!LOOP "(DemGeneral/DemBSWDistribution/DemSatelliteInstanceConfig/*)"!]
      [!VAR "SatelliteCoreId" = "num:i(./DemSatelliteCoreId)"!]
      #include <SchM_Dem_1_Core_[!"$SatelliteCoreId"!].h> /* BSW distribution datatypes and API definitions */
    [!ENDLOOP!]
  [!ENDIF!]
[!ENDINDENT!]
/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

#define DEM_START_SEC_CONST_8
#include <Dem_MemMap.h>

CONST(Dem_ConfigType, DEM_CONST) [!"name(DemConfigSet/*[1])"!] = {0U};

#define DEM_STOP_SEC_CONST_8
#include <Dem_MemMap.h>
[!//
[!INDENT "0"!]

[!IF "(as:modconf('Dem')[1]/DemGeneral/DemExtendedDataCapture = 'DEM_TRIGGER_ON_FDC_THRESHOLD') and
      (as:modconf('Dem')[1]/DemGeneral/DemEventDisplacementStrategy = 'DEM_DISPLACEMENT_FULL') and
      (as:modconf('Dem')[1]/DemGeneral/DemEventDisplacementSupport = 'true')"!]
  /*------------------[Passive event reference configuration]------------------*/
  #define DEM_START_SEC_CALIB_16
  #include <Dem_MemMap.h>

  [!VAR "NumOfLines" = "0"!]
  [!LOOP "DemConfigSet/*/DemAdvDisplacementPassiveTableLine/*"!]
     [!/* !LINKSTO dsn.Dem.Displacement.AdvDisplPassiveEventIds,1 */!][!//
     /** \brief event IDs of passive events, referenced by [!"name(.)"!] */
     CONST(Dem_EventIdType, DEM_CONST)
       [!WS "2"!]Dem_AdvDisplPassiveEventIdsLine[!"num:integer($NumOfLines)"!][DEM_NUM_OF_PASSIVE_EVENT_REFS_LINE[!"num:integer($NumOfLines)"!]] =
       [!VAR "NumOfLines" = "$NumOfLines + 1"!]
     {
       [!LOOP "./DemPassiveEventRefs/*/DemPassiveEventRef"!]
         [!VAR "NameInSegment" = "name(as:ref(.))"!]
         [!WS "2"!][!"as:ref(.)/DemEventId"!]U, /* event ID of passive event: [!"$NameInSegment"!] */
       [!ENDLOOP!]
     };
  [!ENDLOOP!]

  [!IF "num:integer(count(DemConfigSet/*/DemAdvDisplacementPassiveTableLine/*)) > 0"!]
    [!VAR "NumOfLines" = "0"!]
    [!/* !LINKSTO dsn.Dem.Displacement.DemAdvDisplReportedEventIds,1 */!][!//
    /** \brief advanced displacement reported events */
    CONST(Dem_EventIdType, DEM_CONST)
    [!WS "2"!]Dem_AdvDisplPassiveTableLineReportedEventIds[DEM_NUM_OF_PASSIVE_TABLE_LINES] =
    {
    [!LOOP "DemConfigSet/*/DemAdvDisplacementPassiveTableLine/*"!]
      [!VAR "NameInSegment" = "name(as:ref(./DemReportedEventRef))"!]
      [!WS "2"!][!"as:ref(./DemReportedEventRef)/DemEventId"!]U, /* event ID of reported event: [!"$NameInSegment"!] */
      [!VAR "NumOfLines" = "$NumOfLines + 1"!]
    [!ENDLOOP!]
    };
  [!ENDIF!]

  #define DEM_STOP_SEC_CALIB_16
  #include <Dem_MemMap.h>

  [!IF "num:integer(count(DemConfigSet/*/DemAdvDisplacementPassiveTableLine/*)) > 0"!]
    #define DEM_START_SEC_CALIB_UNSPECIFIED
    #include <Dem_MemMap.h>

    [!VAR "NumOfLines" = "0"!]
    [!/* !LINKSTO dsn.Dem.Displacement.DemAdvDisplPassiveEvents,1 */!][!//
    /** \brief advanced displacement passive events */
    CONST(Dem_AdvDisplPassiveTableLinePassiveEventsType, DEM_CONST)
    [!WS "2"!]Dem_AdvDisplPassiveTableLinePassiveEvents[DEM_NUM_OF_PASSIVE_TABLE_LINES] =
    {
    [!LOOP "DemConfigSet/*/DemAdvDisplacementPassiveTableLine/*"!]
      [!VAR "NameInSegment" = "name(as:ref(./DemReportedEventRef))"!]
      [!WS "2"!]/* [!"name(.)"!] */
      [!WS "2"!]{
      [!WS "4"!]Dem_AdvDisplPassiveEventIdsLine[!"num:integer($NumOfLines)"!], /* array of passive event refs */
      [!WS "4"!]DEM_NUM_OF_PASSIVE_EVENT_REFS_LINE[!"num:integer($NumOfLines)"!], /* number of passive event refs */
      [!WS "2"!]},
      [!VAR "NumOfLines" = "$NumOfLines + 1"!]
    [!ENDLOOP!]
    };

    #define DEM_STOP_SEC_CALIB_UNSPECIFIED
    #include <Dem_MemMap.h>
  [!ENDIF!]
[!ENDIF!]

  [!IF "DemGeneral/DemOBDSupport = 'true'"!]
/*------------------[PID class configuration]---------------------------------*/
    [!LOOP "DemConfigSet/*/DemPidClass/*"!]
      [!VAR "NumPidElement" = "count(DemPidDataElement/*/DemPidDataElementClassRef)"!]
      #define DEM_START_SEC_CONST_16
      #include <Dem_MemMap.h>

      /* !LINKSTO dsn.Dem.OBD.DemPidDataElementIdxOf,1 */
      /** \brief Indexes of data elements contained in [!"name(.)"!] */
      STATIC CONST(Dem_OBDFFDataElementIdxType, DEM_CONST)
        [!/* !LINKSTO dsn.Dem.OBD.DemPidDataElementIdxOf.Size,1 */!][!//
        [!WS "2"!]Dem_PidDataElementIdxOf_[!"name(.)"!][[!"num:integer($NumPidElement)"!]] =
      {
        [!LOOP "DemPidDataElement/*/DemPidDataElementClassRef"!]
          [!VAR "NameInSegment" = "name(as:ref(.))"!]
          [!VAR "IdxCtr" = "0"!]
          [!LOOP "as:modconf('Dem')[1]/DemGeneral/DemDataElementClass/*[. = 'DemExternalCSDataElementClass']"!]
            [!IF "$NameInSegment = name(.)"!]
              [!/* !LINKSTO dsn.Dem.OBD.DemPidDataElementIdxOf.Content,1 */!][!//
              [!WS "2"!][!"num:integer($IdxCtr)"!]U, /* [!"$NameInSegment"!] */
              [!BREAK!]
            [!ELSE!][!//
              [!VAR "IdxCtr" = "$IdxCtr + 1"!]
            [!ENDIF!]
          [!ENDLOOP!]
          [!LOOP "as:modconf('Dem')[1]/DemGeneral/DemDataElementClass/*[. = 'DemExternalSRDataElementClass']"!]
            [!IF "$NameInSegment = name(.)"!]
              [!/* !LINKSTO dsn.Dem.OBD.DemPidDataElementIdxOf.Order,1 */!][!//
              [!WS "2"!][!"num:integer($IdxCtr)"!]U, /* [!"$NameInSegment"!] */
              [!BREAK!]
            [!ELSE!][!//
              [!VAR "IdxCtr" = "$IdxCtr + 1"!]
            [!ENDIF!]
          [!ENDLOOP!]
        [!ENDLOOP!]
      };

      /* !LINKSTO dsn.Dem.OBD.Dem_PidDataElementOffsetOf,1 */
      /** \brief Start-bytes of data elements in PIDs [!"name(.)"!] */
      STATIC CONST(Dem_OBDFFDataElementOffsetType, DEM_CONST)
        [!/* !LINKSTO dsn.Dem.OBD.Dem_PidDataElementOffsetOf.Size,1 */!][!//
        [!WS "2"!]Dem_PidDataElementOffsetOf_[!"name(.)"!][[!"num:integer($NumPidElement)"!]] =
      {
        [!VAR "Offset" = "0"!]
        [!LOOP "DemPidDataElement/*/DemPidDataElementClassRef"!]
          [!/* !LINKSTO dsn.Dem.OBD.Dem_PidDataElementOffsetOf.Content,1 */!][!//
          [!WS "2"!][!"num:integer($Offset)"!]U,[!//
          /* [!"name(as:ref(.))"!] */
          [!VAR "Offset" = "$Offset + as:ref(.)/DemDataElementDataSize"!]
        [!ENDLOOP!]
      };

      #define DEM_STOP_SEC_CONST_16
      #include <Dem_MemMap.h>

      #define DEM_START_SEC_CONST_8
      #include <Dem_MemMap.h>

      /* !LINKSTO dsn.Dem.OBD.Dem_PidDataElementSizesOf,1 */
      /** \brief Size of data elements in PIDs [!"name(.)"!] */
      STATIC CONST(uint8, DEM_CONST)
        [!/* !LINKSTO dsn.Dem.OBD.Dem_PidDataElementSizesOf.Size,1 */!][!//
        [!WS "2"!]Dem_PidDataElementSizesOf_[!"name(.)"!][[!"num:integer($NumPidElement)"!]] =
      {
        [!LOOP "DemPidDataElement/*/DemPidDataElementClassRef"!]
          [!VAR "NameInSegment" = "name(as:ref(.))"!]
          [!/* !LINKSTO dsn.Dem.OBD.Dem_PidDataElementSizesOf.Content,1 */!][!//
          [!WS "2"!][!"as:ref(.)/DemDataElementDataSize"!]U, /* [!"$NameInSegment"!] */
        [!ENDLOOP!]
      };

      #define DEM_STOP_SEC_CONST_8
      #include <Dem_MemMap.h>
    [!ENDLOOP!]

    [!IF "count(DemConfigSet/*/DemPidClass/*) > 0"!]
    #define DEM_START_SEC_CALIB_UNSPECIFIED
    #include <Dem_MemMap.h>

    /** \brief OBD freeze frame segments (PIDs) */
     /* !LINKSTO dsn.Dem.OBD.PidClass,1 */
     /* !LINKSTO Dem.ASR431.SWS_Dem_00597,1 */
     /* !LINKSTO Dem733_Conf,1 */
    CONST(Dem_PidClassType, DEM_CONST) Dem_PidClass[DEM_NUM_OF_PIDS_IN_OBDFF] =
    {
      [!LOOP "DemConfigSet/*/DemPidClass/*"!]
        [!WS "2"!]/* [!"name(.)"!] */
        [!WS "2"!]{
          [!VAR "PidSize" = "0"!]
          [!LOOP "DemPidDataElement/*/DemPidDataElementClassRef"!]
            [!VAR "PidSize" = "$PidSize + as:ref(.)/DemDataElementDataSize"!]
          [!ENDLOOP!]
          [!WS "4"!]Dem_PidDataElementOffsetOf_[!"name(.)"!],
          [!WS "4"!]Dem_PidDataElementSizesOf_[!"name(.)"!],
          [!WS "4"!]Dem_PidDataElementIdxOf_[!"name(.)"!],
          [!WS "4"!][!"num:integer($PidSize)"!]U,  /* Size of PID in OBD FF in byte */
          [!WS "4"!][!"num:integer(count(DemPidDataElement/*))"!]U,  /* Number of data elements in PID */
          [!WS "4"!]DEM_OBDFF_PID_[!"name(.)"!],  /* PID */
        [!WS "2"!]},
      [!ENDLOOP!]
    };

    #define DEM_STOP_SEC_CALIB_UNSPECIFIED
    #include <Dem_MemMap.h>
    [!ENDIF!]
  [!ENDIF!]
[!ENDINDENT!]

/*------------------[Freeze frame classes configuration]--------------------*/
[!//
[!INDENT "0"!]
  [!IF "count(DemGeneral/DemDidClass/*) > 0"!]
    #define DEM_START_SEC_CONST_16
    #include <Dem_MemMap.h>
    [!LOOP "DemGeneral/DemDidClass/*"!]
      [!VAR "NumFFElement" = "count(DemDidDataElementClassRef/*)"!]

      /** \brief Indexes of element on freeze frame data segment
       ** [!"name(.)"!]
       **/
      STATIC CONST(Dem_DataElementIdxType, DEM_CONST)
        [!WS "2"!]Dem_DataElementIdx[!"name(.)"!][[!"num:integer($NumFFElement)"!]] =
      {
        [!LOOP "DemDidDataElementClassRef/*"!]
          [!VAR "NameInSegment" = "name(as:ref(.))"!]
          [!VAR "IdxCtr" = "0"!]
          [!LOOP "../../../../DemDataElementClass/*[. = 'DemExternalCSDataElementClass']"!]
            [!IF "$NameInSegment = name(.)"!]
              [!WS "2"!][!"num:integer($IdxCtr)"!]U, /* [!"$NameInSegment"!] */
              [!BREAK!]
            [!ELSE!][!//
              [!VAR "IdxCtr" = "$IdxCtr + 1"!]
            [!ENDIF!]
          [!ENDLOOP!]
          [!/* !LINKSTO dsn.Dem.OBD.DemPidDataElementIdxOf.Order,1 */!][!//
          [!LOOP "../../../../DemDataElementClass/*[. = 'DemExternalSRDataElementClass']"!]
            [!IF "$NameInSegment = name(.)"!]
              [!WS "2"!][!"num:integer($IdxCtr)"!]U, /* [!"$NameInSegment"!] */
              [!BREAK!]
            [!ELSE!][!//
              [!VAR "IdxCtr" = "$IdxCtr + 1"!]
            [!ENDIF!]
          [!ENDLOOP!]
        [!ENDLOOP!]
      };

      /** \brief Start-bytes of element on freeze frame data segment
       ** [!"name(.)"!]
       **/
      STATIC CONST(Dem_DataElementOffsetType, DEM_CONST)
        [!WS "2"!]Dem_DataElementOffset[!"name(.)"!][[!"num:integer($NumFFElement)"!]] =
      {
        [!VAR "Offset" = "0"!]
        [!LOOP "DemDidDataElementClassRef/*"!]
          [!WS "2"!][!"num:integer($Offset)"!]U, [!//
          [!IF "as:ref(.) = 'DemInternalDataElementClass'"!]
            /* [!"as:ref(.)/DemInternalDataElement"!] */
          [!ELSE!][!//
            /* [!"name(as:ref(.))"!] */
            [!VAR "Offset" = "$Offset + as:ref(.)/DemDataElementDataSize"!]
          [!ENDIF!]
        [!ENDLOOP!]
      };
    [!ENDLOOP!]

    #define DEM_STOP_SEC_CONST_16
    #include <Dem_MemMap.h>

    #define DEM_START_SEC_CONST_UNSPECIFIED
    #include <Dem_MemMap.h>

    /** \brief Freeze frame segments (DIDs) */
    CONST(Dem_FFSegmentType, DEM_CONST)
    Dem_FFSegment[DEM_NUM_FFSEGS] =
    {
      [!LOOP "DemGeneral/DemDidClass/*"!]
        [!WS "2"!]/* [!"name(.)"!] */
        [!WS "2"!]{
          [!WS "4"!]Dem_DataElementIdx[!"name(.)"!],
          [!WS "4"!]Dem_DataElementOffset[!"name(.)"!],
          [!WS "4"!]DEM_FFS_DID_[!"name(.)"!],
          [!WS "4"!][!"num:integer(count(DemDidDataElementClassRef/*))"!]U
        [!WS "2"!]},
      [!ENDLOOP!]
    };

    #define DEM_STOP_SEC_CONST_UNSPECIFIED
    #include <Dem_MemMap.h>

    #define DEM_START_SEC_CALIB_16
    #include <Dem_MemMap.h>

    /* !LINKSTO Dem.Dem_FFSegId.Access,1 */
    [!LOOP "DemGeneral/DemFreezeFrameClass/*"!]

      /*---[Freeze frame class [!"name(.)"!]]---*/
      [!VAR "NumFFSegs" = "count(DemDidClassRef/*)"!]

      /** \brief Indexes of segments on freeze frame class
       ** [!"name(.)"!] to ::Dem_FFSegment[]
       **/
      DEM_STATIC CONST(Dem_FFSegIdxType, DEM_CONST)
      Dem_FFSegIdx[!"name(.)"!][[!"num:integer($NumFFSegs)"!]] =
      {
        [!LOOP "DemDidClassRef/*"!]
          [!WS "2"!][!"as:ref(.)/@index"!]U, /* [!"name(as:ref(.))"!] */
        [!ENDLOOP!]
      };
    [!ENDLOOP!]

    #define DEM_STOP_SEC_CALIB_16
    #include <Dem_MemMap.h>

    #define DEM_START_SEC_CONST_UNSPECIFIED
    #include <Dem_MemMap.h>

    /** \brief Array containing all freeze frame classes */
    CONST(Dem_FFClassType, DEM_CONST)
    Dem_FFClass[DEM_NUM_FFCLS] =
    {
      [!INDENT "2"!]
        [!LOOP "DemGeneral/DemFreezeFrameClass/*"!]
          {
            [!VAR "NumFFSegs" = "count(DemDidClassRef/*)"!]
            [!WS "2"!]Dem_FFSegIdx[!"name(.)"!],
            [!WS "2"!][!"num:integer($NumFFSegs)"!]U /* number of segments */
          },
        [!ENDLOOP!]
          {
            [!WS "2"!]NULL_PTR,
            [!WS "2"!]0U
          }
      [!ENDINDENT!]
    };

  #define DEM_STOP_SEC_CONST_UNSPECIFIED
  #include <Dem_MemMap.h>
  [!ENDIF!]
[!ENDINDENT!]

/*------------------[Freeze frame record number classes configuration]------*/
[!//
[!INDENT "0"!]
  [!IF "DemGeneral/DemTypeOfFreezeFrameRecordNumeration = 'DEM_FF_RECNUM_CONFIGURED' and count(DemGeneral/DemFreezeFrameRecNumClass/*) > 0"!]
    #define DEM_START_SEC_CONST_8
    #include <Dem_MemMap.h>
    [!LOOP "DemGeneral/DemFreezeFrameRecNumClass/*"!]
      [!VAR "NumFFRecords" = "count(DemFreezeFrameRecordNumber/*)"!]

      /** \brief Freeze frame record numbers for
       ** [!"name(.)"!]
       **/
      STATIC CONST(Dem_FFIdxType, DEM_CONST)
        [!WS "2"!]Dem_FFRecNumClass[!"name(.)"!][[!"num:integer($NumFFRecords)"!]] =
      {
        [!LOOP "DemFreezeFrameRecordNumber/*"!]
          [!WS "2"!][!"translate(num:inttohex(., 2), 'abcdef', 'ABCDEF')"!]U,
        [!ENDLOOP!]
      };
    [!ENDLOOP!]

    #define DEM_STOP_SEC_CONST_8
    #include <Dem_MemMap.h>

    #define DEM_START_SEC_CONST_UNSPECIFIED
    #include <Dem_MemMap.h>

    /** \brief Array containing all freeze frame record number classes */
    CONST(Dem_FFRecNumerationClassType, DEM_CONST)
      [!WS "2"!]Dem_FFRecNumerationClass[DEM_NUM_FFRECNUMCLS] =
    {
      [!LOOP "DemGeneral/DemFreezeFrameRecNumClass/*"!]
        [!WS "2"!]{
          [!WS "4"!]Dem_FFRecNumClass[!"name(.)"!],
          /* number of freeze frame records */
          [!WS "4"!][!"num:integer(count(DemFreezeFrameRecordNumber/*))"!]U
        [!WS "2"!]},
      [!ENDLOOP!]
        [!WS "2"!]{
          [!WS "4"!]NULL_PTR,
          [!WS "4"!]0U /* number of freeze frame records */
       [!WS "2"!]}
    };

    #define DEM_STOP_SEC_CONST_UNSPECIFIED
    #include <Dem_MemMap.h>
  [!ENDIF!]
[!ENDINDENT!]

/*------------------[Extended data classes configuration]-------------------*/
[!//
[!INDENT "0"!]
  [!IF "count(DemGeneral/DemExtendedDataRecordClass/*) > 0"!]
    #define DEM_START_SEC_CONST_16
    #include <Dem_MemMap.h>
    [!LOOP "DemGeneral/DemExtendedDataRecordClass/*"!]
      [!VAR "NumEDElement" = "count(DemDataElementClassRef/*)"!]

      /** \brief Indexes of element on extended data segment
       ** [!"name(.)"!]
       **/
      STATIC CONST(Dem_DataElementIdxType, DEM_CONST)
        [!WS "2"!]Dem_DataElementIdx[!"name(.)"!][[!"num:integer($NumEDElement)"!]] =
      {
        [!LOOP "DemDataElementClassRef/*"!]
          [!IF "as:ref(.) = 'DemInternalDataElementClass'"!]
            [!/* !LINKSTO SWS_Dem_00472,1 */!][!//
            [!IF "(as:ref(.)/DemInternalDataElement = 'DEM_AGINGCTR_UPCNT') or
                  (as:ref(.)/DemInternalDataElement = 'DEM_AGINGCTR')"!]
              [!WS "2"!]DEM_INT_VAL_IDX_AGECTRUP,
            [!ELSEIF "as:ref(.)/DemInternalDataElement = 'DEM_AGINGCTR_DOWNCNT'"!]
              [!WS "2"!]DEM_INT_VAL_IDX_AGECTRDWN,
            [!ELSEIF "as:ref(.)/DemInternalDataElement = 'DEM_OCCCTR'"!]
              [!WS "2"!]DEM_INT_VAL_IDX_OCCCTR,
            [!ELSEIF "as:ref(.)/DemInternalDataElement = 'DEM_OVFLIND'"!]
              [!WS "2"!]DEM_INT_VAL_IDX_OVFIND,
            [!ELSEIF "as:ref(.)/DemInternalDataElement = 'DEM_SIGNIFICANCE'"!]
              [!WS "2"!]DEM_INT_VAL_IDX_EVSIGNIF,
            [!ELSEIF "as:ref(.)/DemInternalDataElement = 'DEM_SI30'"!]
              [!WS "2"!]DEM_INT_VAL_IDX_SI30,
            [!ELSEIF "as:ref(.)/DemInternalDataElement = 'DEM_CURRENT_FDC'"!]
              [!WS "2"!]DEM_INT_VAL_IDX_CURRENT_FDC,
            [!ELSEIF "as:ref(.)/DemInternalDataElement = 'DEM_FDC12'"!]
              [!WS "2"!]DEM_INT_VAL_IDX_FDC12,
            [!ELSEIF "as:ref(.)/DemInternalDataElement = 'DEM_OCC1'"!]
              [!WS "2"!]DEM_INT_VAL_IDX_OCC1,
            [!ELSEIF "as:ref(.)/DemInternalDataElement = 'DEM_OCC2'"!]
              [!WS "2"!]DEM_INT_VAL_IDX_OCC2,
            [!ELSEIF "as:ref(.)/DemInternalDataElement = 'DEM_OCC3'"!]
              [!WS "2"!]DEM_INT_VAL_IDX_OCC3,
            [!ELSEIF "as:ref(.)/DemInternalDataElement = 'DEM_OCC4'"!]
              [!WS "2"!]DEM_INT_VAL_IDX_OCC4,
            [!ELSEIF "as:ref(.)/DemInternalDataElement = 'DEM_OCC5'"!]
              [!WS "2"!]DEM_INT_VAL_IDX_OCC5,
            [!ELSEIF "as:ref(.)/DemInternalDataElement = 'DEM_OCC6'"!]
              [!WS "2"!]DEM_INT_VAL_IDX_OCC6,
            [!ELSEIF "as:ref(.)/DemInternalDataElement = 'DEM_OCC7'"!]
              [!WS "2"!]DEM_INT_VAL_IDX_OCC7,
            [!/* !LINKSTO Dem.InternalDataElement.Priority.Mapping,1 */!][!//
            [!ELSEIF "as:ref(.)/DemInternalDataElement = 'DEM_PRIORITY'"!]
              [!WS "2"!]DEM_INT_VAL_IDX_PRIO,
            [!/* !LINKSTO Dem.InternalDataElement.EventId.Mapping,1 */!][!//
            [!ELSEIF "as:ref(.)/DemInternalDataElement = 'DEM_EVENT_ID'"!]
              [!WS "2"!]DEM_INT_VAL_IDX_EVENT_ID,
            [!/* !LINKSTO Dem.MultiEventTriggering.InternalDataElement.RootId.Mapping,1 */!][!//
            [!ELSEIF "as:ref(.)/DemInternalDataElement = 'DEM_ROOT_EVENT_ID'"!]
              [!WS "2"!]DEM_INT_VAL_IDX_ROOT_ID,
            [!/* !LINKSTO SWS_Dem_00984,1 */!][!//
            [!ELSEIF "as:ref(.)/DemInternalDataElement = 'DEM_CYCLES_SINCE_FIRST_FAILED'"!]
              [!WS "2"!]DEM_INT_VAL_IDX_CSFF,
            [!/* !LINKSTO SWS_Dem_00775,1 */!][!//
            [!ELSEIF "as:ref(.)/DemInternalDataElement = 'DEM_CYCLES_SINCE_LAST_FAILED'"!]
              [!WS "2"!]DEM_INT_VAL_IDX_CSLF,
            [!ENDIF!]
          [!ENDIF!]
          [!VAR "NameInSegment" = "name(as:ref(.))"!]
          [!VAR "IdxCtr" = "0"!]
          [!LOOP "../../../../DemDataElementClass/*[. = 'DemExternalCSDataElementClass']"!]
            [!IF "$NameInSegment = name(.)"!]
              [!WS "2"!][!"num:integer($IdxCtr)"!]U, /* [!"$NameInSegment"!] */
              [!BREAK!]
            [!ELSE!][!//
              [!VAR "IdxCtr" = "$IdxCtr + 1"!]
            [!ENDIF!]
          [!ENDLOOP!]
          [!LOOP "../../../../DemDataElementClass/*[. = 'DemExternalSRDataElementClass']"!]
            [!IF "$NameInSegment = name(.)"!]
              [!WS "2"!][!"num:integer($IdxCtr)"!]U, /* [!"$NameInSegment"!] */
              [!BREAK!]
            [!ELSE!][!//
              [!VAR "IdxCtr" = "$IdxCtr + 1"!]
            [!ENDIF!]
          [!ENDLOOP!]
        [!ENDLOOP!]
      };

      /** \brief Start-bytes of element on extended data segment
       ** [!"name(.)"!]
       **/
      STATIC CONST(Dem_DataElementOffsetType, DEM_CONST)
        [!WS "2"!]Dem_DataElementOffset[!"name(.)"!][[!"num:integer($NumEDElement)"!]] =
      {
        [!VAR "Offset" = "0"!]
        [!LOOP "DemDataElementClassRef/*"!]
          [!WS "2"!][!"num:integer($Offset)"!]U, [!//
          [!IF "as:ref(.) = 'DemInternalDataElementClass'"!]
            /* [!"as:ref(.)/DemInternalDataElement"!] */
          [!ELSE!][!//
            /* [!"name(as:ref(.))"!] */
            [!VAR "Offset" = "$Offset + as:ref(.)/DemDataElementDataSize"!]
          [!ENDIF!]
        [!ENDLOOP!]
      };
    [!ENDLOOP!]

    #define DEM_STOP_SEC_CONST_16
    #include <Dem_MemMap.h>

    #define DEM_START_SEC_CONST_UNSPECIFIED
    #include <Dem_MemMap.h>

    /** \brief Extended data segments (Records) */
    CONST(Dem_EDSegmentType, DEM_CONST) Dem_EDSegment[DEM_NUM_EDSEGS] =
    {
      [!INDENT "2"!]
      [!LOOP "DemGeneral/DemExtendedDataRecordClass/*"!]
        /* [!"name(.)"!] */
        {
          [!INDENT "4"!]
          Dem_DataElementIdx[!"name(.)"!],
          Dem_DataElementOffset[!"name(.)"!],
          [!"num:integer(count(DemDataElementClassRef/*))"!]U,
          DEM_EDS_NUM_[!"name(.)"!],
          [!IF "not(node:exists(DemExtendedDataRecordUpdate))"!]
            TRUE,
          [!ELSE!]
            [!IF "DemExtendedDataRecordUpdate = 'DEM_UPDATE_RECORD_YES'"!]
              TRUE,
            [!ELSE!]
              FALSE,
            [!ENDIF!]
          [!ENDIF!]
          [!IF "DemExtendedDataRecordAlwaysAvailable = 'true'"!]
            TRUE, /* always available */
          [!ELSE!]
            FALSE, /* always available */
          [!ENDIF!]
          [!ENDINDENT!]
        },
      [!ENDLOOP!]
      [!ENDINDENT!]
    };

    #define DEM_STOP_SEC_CONST_UNSPECIFIED
    #include <Dem_MemMap.h>
    [!LOOP "DemGeneral/DemExtendedDataClass/*"!]

      #define DEM_START_SEC_CONST_8
      #include <Dem_MemMap.h>

      /*---[Extended data class [!"name(.)"!]]---*/
      [!VAR "NumEDSegs" = "count(DemExtendedDataRecordClassRef/*)"!]

      /** \brief Indexes of segments on extended data class
       ** [!"name(.)"!] to ::Dem_EDSegment[]
       **/
      STATIC CONST(Dem_EDSegIdxType, DEM_CONST)
      Dem_EDSegIdx[!"name(.)"!][[!"num:integer($NumEDSegs)"!]] =
      {
        [!LOOP "DemExtendedDataRecordClassRef/*"!]
          [!WS "2"!][!"as:ref(.)/@index"!]U, /* [!"name(as:ref(.))"!] */
        [!ENDLOOP!]
      };

      #define DEM_STOP_SEC_CONST_8
      #include <Dem_MemMap.h>

      #define DEM_START_SEC_CONST_16
      #include <Dem_MemMap.h>

      /** \brief Start-bytes of segments on extended data class
       ** [!"name(.)"!]
       **/
      STATIC CONST(Dem_EDStartByteType, DEM_CONST)
        [!WS "2"!]Dem_EDStartByte[!"name(.)"!][[!"num:integer($NumEDSegs)"!]] =
      {
        [!VAR "StartByte" = "0"!]
        [!LOOP "DemExtendedDataRecordClassRef/*"!]
          [!WS "2"!][!"num:integer($StartByte)"!]U, /* [!"name(as:ref(.))"!] */
          [!LOOP "(as:ref(.)/DemDataElementClassRef/*)"!]
            [!IF "as:ref(.) != 'DemInternalDataElementClass'"!]
              [!VAR "StartByte" = "$StartByte + as:ref(.)/DemDataElementDataSize"!]
            [!ENDIF!]
          [!ENDLOOP!]
        [!ENDLOOP!]
      };

      #define DEM_STOP_SEC_CONST_16
      #include <Dem_MemMap.h>
    [!ENDLOOP!]

    #define DEM_START_SEC_CONST_UNSPECIFIED
    #include <Dem_MemMap.h>

    /** \brief Array containing all extended data classes */
    CONST(Dem_EDClassType, DEM_CONST)
    Dem_EDClass[DEM_NUM_EDCLS] =
    {
      [!INDENT "2"!]
        [!LOOP "DemGeneral/DemExtendedDataClass/*"!]
          {
            [!VAR "NumEDSegs" = "count(DemExtendedDataRecordClassRef/*)"!]
            [!WS "2"!]Dem_EDSegIdx[!"name(.)"!],
            [!WS "2"!]Dem_EDStartByte[!"name(.)"!],
            [!WS "2"!][!"num:integer($NumEDSegs)"!]U /* number of segments */
          },
        [!ENDLOOP!]
          {
            [!WS "2"!]NULL_PTR,
            [!WS "2"!]NULL_PTR,
            [!WS "2"!]0U
          }
      [!ENDINDENT!]
    };

    #define DEM_STOP_SEC_CONST_UNSPECIFIED
    #include <Dem_MemMap.h>
  [!ENDIF!]
[!ENDINDENT!]

/*------------------[Data element classes configuration]--------------------*/

#define DEM_START_SEC_CONST_UNSPECIFIED
#include <Dem_MemMap.h>
[!//
[!IF "count(DemGeneral/DemDataElementClass/*[. = 'DemExternalCSDataElementClass' or . = 'DemExternalSRDataElementClass']) > 0"!]
  [!VAR "SRSupport" = "(DemGeneral/DemRteUsage = 'true') and (count(DemGeneral/DemDataElementClass/*[. = 'DemExternalSRDataElementClass']) > 0)"!]
  [!INDENT "0"!]

    /** \brief Array containing all external data elements */
    CONST(Dem_DataElementType, DEM_CONST)
      Dem_DataElement[DEM_NUM_EXT_DATAELEMENTS] =
    {
      [!WS "2"!]/* elements are sorted for optimization reasons presented in design */
      [!LOOP "DemGeneral/DemDataElementClass/*[. = 'DemExternalCSDataElementClass']"!]
        [!WS "2"!]/* [!"name(.)"!] */
        [!WS "2"!]{
          [!IF "node:exists(./DemDataElementReadFnc)"!]
            [!VAR "CallbackName" = "./DemDataElementReadFnc"!]
          [!ELSE!][!//
            [!VAR "CallbackName" = "concat('Rte_Call_CBReadData_', name(.), '_ReadData')"!]
          [!ENDIF!]
          [!WS "4"!]/* Deviation MISRAC2012-1 */
          [!WS "4"!]{(Dem_GeneralFuncType)&[!"$CallbackName"!]},
          [!WS "4"!][!"./DemDataElementDataSize"!]U,
          [!IF "./DemDataElementInterfaceWithEventParameter = 'false'"!]
            [!WS "4"!]DEM_READ_DATA_CS_PROTOTYPE_BASIC,
          [!ELSE!][!//
            [!WS "4"!]DEM_READ_DATA_CS_PROTOTYPE_WITH_EVENTID,
          [!ENDIF!]
          [!IF "$SRSupport = 'true'"!]
            [!WS "4"!]DEM_OPAQUE  /* default value not used for CS Data Elements */
          [!ENDIF!]
        [!WS "2"!]},
      [!ENDLOOP!]
      [!IF "$SRSupport = 'true'"!]
        [!LOOP "DemGeneral/DemDataElementClass/*[. = 'DemExternalSRDataElementClass']"!]
          [!WS "2"!]/* [!"name(.)"!] */
          [!WS "2"!]{
            [!VAR "ReadFunction" = "concat('Rte_Read_Dem_SRDataServices_', name(.),'_data')"!]
            [!WS "4"!]/* Deviation MISRAC2012-1 */
            [!WS "4"!]{(Dem_GeneralFuncType)&[!"$ReadFunction"!]},
            [!WS "4"!][!"./DemDataElementDataSize"!]U,
            [!WS "4"!]DEM_READ_DATA_SR_DATA_TYPE_[!"./DemDataElementDataType"!],
            [!IF "(node:exists(./DemDataElementEndianness))"!]
              [!WS "4"!]DEM_[!"./DemDataElementEndianness"!]
            [!ELSE!][!//
              [!WS "4"!]DEM_[!"as:modconf('Dem')[1]/DemGeneral/DemDataElementDefaultEndianness"!]
            [!ENDIF!]
          [!WS "2"!]},
        [!ENDLOOP!]
      [!ENDIF!]
    };
  [!ENDINDENT!]
[!ENDIF!]

#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include <Dem_MemMap.h>

/*------------------[DTC groups configuration]------------------------------*/

[!IF "(count(DemGeneral/DemGroupOfDTC/*[name(.) != 'DEM_DTC_GROUP_EMISSION_REL_DTCS'])) > 0"!]
#define DEM_START_SEC_CONST_32
#include <Dem_MemMap.h>
/** \brief Array containing the DTC group values */
CONST(Dem_DTCGroupType, DEM_CONST) Dem_DTCGroups[DEM_NUM_DTC_GROUPS] =
{
  [!/* sort DTC group values in ascending order, to support search-algorithm */!][!//
  [!LOOP "node:order(DemGeneral/DemGroupOfDTC/*[name(.) != 'DEM_DTC_GROUP_EMISSION_REL_DTCS'], './DemGroupDTCs')"!]
    [!WS "2"!]DemConf_DemGroupOfDTC_[!"name(.)"!],
  [!ENDLOOP!]
};

#define DEM_STOP_SEC_CONST_32
#include <Dem_MemMap.h>
[!ENDIF!]

/*------------------[Events Class configuration]----------------------------*/

#define DEM_START_SEC_CALIB_UNSPECIFIED
#include <Dem_MemMap.h>

[!VAR "OBDSupport" = "node:value('DemGeneral/DemOBDSupport')"!]
[!VAR "FreezeFrameRecordCapture" = "node:value('DemGeneral/DemFreezeFrameCapture')"!]
[!VAR "EnableConditionGroupNumber" = "num:integer(count(DemGeneral/DemEnableConditionGroup/*))"!]
[!IF "$EventClassSupport = 'true'"!]
[!CALL "GET_VCC_OCC5_USED"!][!//
[!CALL "CREATE_EVENT_CLASS_LIST"!]
[!CALL "CREATE_PRIORITY_VALUE_LIST"!][!//
/** \brief EventClass configuration description table */
CONST(Dem_EventClassDescType, DEM_CONST) Dem_EventClassDesc[DEM_NUMBER_OF_EVENT_CLASSES] =
{
  [!INDENT "2"!]
  [!VAR "EventClassIdx" = "0"!]
  [!LOOP "text:split($EventClassList, '#')"!]
  [!VAR "EventClassString" = "."!]
   /* Event Class [!"num:i($EventClassIdx)"!] */
  [!VAR "EventClassIdx" = "$EventClassIdx + 1"!]
  {
    [!INDENT "4"!]
    [!IF "$NumMultiEventTriggering > 0"!]
      /* Multi-event triggering index */
      [!VAR "MultiEventTriggeringClassIdx" = "substring-before($EventClassString,'T')"!]
      [!IF "$MultiEventTriggeringClassIdx = '-'"!]
        DEM_MULTIEVENTTRIGGERINGIDX_UNUSED,
      [!ELSE!][!//
        [!"substring-before($EventClassString,'T')"!]U,
      [!ENDIF!]
    [!ENDIF!]
    [!VAR "EventClassString" = "substring-after($EventClassString, 'T')"!]
    [!IF "$IndicatorClassSupport = 'true'"!]
      [!VAR "IndicatorClassIdx" = "substring-before($EventClassString,'I')"!]
      [!IF "$IndicatorClassIdx = '-'"!]
        DEM_INDICATOR_UNUSED,
      [!ELSE!][!//
        [!"$IndicatorClassIdx"!]U,
      [!ENDIF!]
    [!ENDIF!]
    [!VAR "EventClassString" = "substring-after($EventClassString, 'I')"!]
    /* DemAgingCycleCounterThreshold (if 0 DemAgingAllowed == 'false')*/
    [!"substring-before($EventClassString,'A')"!]U,
    [!VAR "EventClassString" = "substring-after($EventClassString, 'A')"!]
    [!IF "$VccOcc5Used = 1"!]
      /* DemOBDAgingCycleCounterThreshold */
      [!"substring-before($EventClassString,'O')"!]U,
    [!ENDIF!]
    [!VAR "EventClassString" = "substring-after($EventClassString, 'O')"!]
    /* DemEventFailureCycleCounterThreshold (0 if disabled')*/
    [!"substring-before($EventClassString,'F')"!]U,
    [!VAR "EventClassString" = "substring-after($EventClassString, 'F')"!]
    /* Event priority (0 = most important) */
    [!CALL "GET_INTERNAL_PRIORITY_IDX", "Prio" = "num:i(substring-before($EventClassString,'P'))"!]
    [!"num:i($PriorityValueIdx)"!]U,
    [!VAR "EventClassString" = "substring-after($EventClassString, 'P')"!]
    [!/* !LINKSTO dsn.Dem.CommonOperationCycle.Dem_EventClassDesc,1 */!][!//
    [!IF "$CommonOperationCycleEnabled = 'true'"!]
      /* Operation cycle*/
      [!"substring-before($EventClassString,'!')"!],
      [!VAR "EventClassString" = "substring-after($EventClassString, '!')"!]
    [!ENDIF!]
    [!IF "$AgingAllowedSeperateFlag = 'true'"!]
      /* DemAgingAllowed */
      [!"substring-before($EventClassString,'?')"!],
      [!VAR "EventClassString" = "substring-after($EventClassString, '?')"!]
    [!ENDIF!]
    [!IF "($OBDSupport = 'true') or ($FreezeFrameRecordCapture = 'DEM_TRIGGER_ON_FDC_THRESHOLD')"!][!//
      /* Bitfield for boolean values
      [!WS!]*   bit 0: DemMinRatio
      [!WS!]*   bit 1: DemFreezeFrameUpdate (always true, for calibration use only)
      [!WS!]*/
      ((uint8)[!"substring-before($EventClassString,'M')"!]U)
      [!/* !LINKSTO Dem.EB.EventClass.FreezeFrameUpdate.Default,1 */!][!//
      | ((uint8)(DEM_FFUPDATEEVENTCLASS_MASK & 1U) << DEM_FFUPDATEEVENTCLASS_OFFSET),
    [!ENDIF!]
    [!VAR "EventClassString" = "substring-after($EventClassString, 'M')"!]
    [!IF "$EnableConditionGroupNumber > 0"!]
    /* enable condition group */
      [!VAR "EnCondGrpIdx" = "substring-before($EventClassString,'&')"!]
      [!IF "$EnCondGrpIdx = '-'"!]
        DEM_ENCONDITIONGROUP_UNUSED
      [!ELSE!][!//
        [!"substring-before($EventClassString,'&')"!]U
      [!ENDIF!]
      [!VAR "EventClassString" = "substring-after($EventClassString, '&')"!]
    [!ENDIF!]
    [!ENDINDENT!]
  },
  [!ENDLOOP!]
  [!ENDINDENT!]
};
[!ENDIF!]

#define DEM_STOP_SEC_CALIB_UNSPECIFIED
#include <Dem_MemMap.h>

/*------------------[Events configuration]----------------------------------*/

#define DEM_START_SEC_CALIB_32
#include <Dem_MemMap.h>

[!CALL "CREATE_FAILURE_COUNTER_CYCLE_LIST"!]
[!CALL "CALC_MULTI_EVENT_CLASSES"!]
[!//
/** \brief Event configuration description table (uses 'bit packing scheme) */
CONST(Dem_EventDescType, DEM_CONST) Dem_EventDesc[DEM_NUMBER_OF_EVENTS] =
{
[!CALL "CREATE_PRIORITY_VALUE_LIST"!][!//
[!IF "DemGeneral/DemEventCombinationSupport = 'DEM_EVCOMB_DISABLED'"!][!//
  [!CALL "DEBOUNCE_COUNTER_CLASS_CALC"!][!//
  [!IF "($EventClassSupport = 'false') and ($NumMultiEventTriggering > 0)"!]
  { 0U, 0U, 0U, 0U },                             /* invalid event id entry */
  [!ELSE!]
  { 0U, 0U, 0U },                                 /* invalid event id entry */
  [!ENDIF!]
[!/* create unique list of all configured
 * failure cycle counter cycles and thresholds */!][!//
[!/* generate event descriptions */!][!//
  [!LOOP "node:order(DemConfigSet/*/DemEventParameter/*, 'DemEventId')"!]
    [!WS "2"!]{ /* event: [!"name(.)"!] */
[!/*  -----[EvConf1]----- */!][!//
[!/*  - [UdsDTC] - */!][!//
      [!CALL "EVENT_DESC_BITFIELD_UDS_DTC_VALUE", "Indent" = "4", "Prefix" = "''"!]
[!/*  - [DTCOrigin] - */!][!//
      [!CALL "EVENT_DESC_BITFIELD_DTC_ORIGIN", "Indent" = "4"!]
[!/*  - [DTCSeverity] - */!][!//
      [!CALL "EVENT_DESC_BITFIELD_DTC_SEVERITY", "Indent" = "4"!]
[!/*  - [EnablePrestorage] - */!][!//
      [!CALL "EVENT_DESC_BITFIELD_ENABLE_PRESTORAGE", "Indent" = "4"!]
[!/*  - [EventKind] - */!][!//
      [!CALL "EVENT_DESC_BITFIELD_EVENT_KIND", "Indent" = "4"!]
[!/*  - [IndicatorUsed] - */!][!//
      [!CALL "EVENT_DESC_BITFIELD_INDICATOR_USED", "Indent" = "4"!]
[!/*  - [ImmediateStorage] - */!][!//
[!/* !LINKSTO Dem.ImmediateStorage.InternalEvents,1 */!][!//
      [!CALL "EVENT_DESC_BITFIELD_IMMEDIATE_STORAGE", "Indent" = "4"!]
      [!WS "4"!],
[!/*  -----[EvConf2]----- */!][!//
[!/*  - [EvSignificance] - */!][!//
[!/* !LINKSTO Dem.DemInternalDataElementRange.EventSignificance,1 */!][!//
      [!CALL "EVENT_DESC_BITFIELD_EVENT_SIGNIFICANCE", "Indent" = "4", "Prefix" = "''"!]
[!/*  - [FFClassIdx] & [FFRecInfo] - */!][!//
      [!CALL "EVENT_DESC_BITFIELD_FREEZE_FRAMES", "Indent" = "4"!]
[!/*  - [EDClassIdx] - */!][!//
      [!CALL "EVENT_DESC_BITFIELD_EXTENDED_DATA_CLASS", "Indent" = "4"!]
[!/*  - [OpCycleIdx] - */!][!//
      [!IF "($EventClassSupport = 'false') or ($CommonOperationCycleEnabled = 'false')"!][!//
[!/* !LINKSTO dsn.Dem.CommonOperationCycle.Dem_EventDesc,1 */!][!//
        [!CALL "EVENT_DESC_BITFIELD_OPERATION_CYCLE", "Indent" = "4"!]
      [!ENDIF!]
[!/*  - [AgingCycleThreshold] - */!][!//
      [!IF "$EventClassSupport = 'false'"!]
        [!CALL "EVENT_DESC_BITFIELD_AGING_CTR_THRESHOLD", "Indent" = "4"!]
      [!ENDIF!]
[!/*  - [DTCGroup] - */!][!//
      [!CALL "EVENT_DESC_BITFIELD_DTC_GROUP", "Indent" = "4"!]
[!/*  - [EventClassIdx] - */!][!//
      [!IF "$EventClassSupport = 'true'"!][!//
        [!CALL "EVENT_DESC_BITFIELD_EVENT_CLASS", "Indent" = "4"!]
      [!ENDIF!]
      [!WS "4"!],
[!/*  -----[EvConf3]----- */!][!//
[!/*  - [EnCondGrpIdx] - */!][!//
      [!VAR "Operation" = "''"!][!//
      [!IF "$CalibrationSupport = 'false'"!]
        [!VAR "Operation" = "'| '"!]
        [!CALL "EVENT_DESC_BITFIELD_ENABLE_CONDITION_GROUP_IDX", "Indent" = "4", "Prefix" = "''"!][!//
      [!ELSE!]
        [!WS "4"!]/* calibration support enabled, enable condition group located in Dem_EventClassDesc */
      [!ENDIF!]
[!/*  - [EventFailureClassIdx] - */!][!//
      [!CALL "EVENT_DESC_BITFIELD_EVENT_FAILUIRE_CLASS_IDX", "Indent" = "4", "Prefix" = "$Operation"!]
[!/*  - [Priority] - */!][!//
      [!IF "$EventClassSupport = 'false'"!]
        [!CALL "EVENT_DESC_BITFIELD_EVENT_PRIORITY", "Indent" = "4"!]
      [!ENDIF!]
[!/*  - [DebounceAlgo] & [DebounceIdx] - */!][!//
      [!CALL "EVENT_DESC_BITFIELD_DEBOUNCE_ALGORITHM", "Indent" = "4"!]
[!/*  - [DTCFuncUnit] - */!][!//
      [!CALL "EVENT_DESC_BITFIELD_FUNCTIONAL_UNIT", "Indent" = "4"!]
[!/*  - [AgingCycleIdx] - */!][!//
      [!CALL "EVENT_DESC_BITFIELD_AGING_CYCLE_ID", "Indent" = "4"!]
      [!IF "($EventClassSupport = 'false') and ($NumMultiEventTriggering > 0)"!]
        [!WS "4"!],
[!/*  -----[EvConf4]----- */!][!//
[!/*  - [MultiEventTriggeringClassIdx] - */!][!//
        [!CALL "EVENT_DESC_BITFIELD_MULTI_EVENT_TRIGGERING_IDX", "Indent" = "4", "Prefix" = "''"!]
      [!ENDIF!]
    [!WS "2"!]},
  [!ENDLOOP!]
[!ELSE!][!/* ---------- DEM_EVCOMB_TYPE1 ---------- */!][!//
  [!CALL "CREATE_DTCCLASS_LIST_FROM_EVENTS"!][!//
  [!IF "($EventClassSupport = 'false') and ($NumMultiEventTriggering > 0)"!]
  { 0U, 0U },                             /* invalid event id entry */
  [!ELSE!]
  { 0U },                                 /* invalid event id entry */
  [!ENDIF!]
  [!LOOP "node:order(DemConfigSet/*/DemEventParameter/*, 'DemEventId')"!][!//
    [!WS "2"!]{ /* event: [!"name(.)"!] */
[!/*  -----[EvConf1]----- */!][!//
[!/*  - [EventKind] - */!][!//
    [!CALL "EVENT_DESC_BITFIELD_EVENT_KIND", "Indent" = "4", "Prefix" = "''"!]
[!/*  - [IndicatorUsed] - */!][!//
    [!CALL "EVENT_DESC_BITFIELD_INDICATOR_USED", "Indent" = "4"!]
[!/*  - [EnablePrestorage] - */!][!//
    [!CALL "EVENT_DESC_BITFIELD_ENABLE_PRESTORAGE", "Indent" = "4"!]
[!/*  - [DebounceAlgo] & [DebounceIdx] - */!][!//
    [!CALL "EVENT_DESC_BITFIELD_DEBOUNCE_ALGORITHM", "Indent" = "4"!]
[!/*  - [EnCondGrpIdx] - */!][!//
    [!CALL "EVENT_DESC_BITFIELD_ENABLE_CONDITION_GROUP_IDX", "Indent" = "4"!]
[!/*  - [EventFailureClassIdx] - */!][!//
    [!CALL "EVENT_DESC_BITFIELD_EVENT_FAILUIRE_CLASS_IDX", "Indent" = "4"!]
[!/*  - [OpCycleIdx] - */!][!//
    [!IF "($EventClassSupport = 'false') or ($CommonOperationCycleEnabled = 'false')"!][!//
[!/* !LINKSTO dsn.Dem.CommonOperationCycle.Dem_EventDesc,1 */!][!//
      [!CALL "EVENT_DESC_BITFIELD_OPERATION_CYCLE", "Indent" = "4"!]
    [!ENDIF!]
[!/*  - [DTCConfIdx] - */!][!//
    [!CALL "EVENT_DESC_BITFIELD_DTC_CONFIG_IDX", "Indent" = "4"!]
      [!IF "($EventClassSupport = 'false') and ($NumMultiEventTriggering > 0)"!]
        [!WS "4"!],
[!/*  -----[EvConf2]----- */!][!//
[!/*  - [MultiEventTriggeringClassIdx] - */!][!//
        [!CALL "EVENT_DESC_BITFIELD_MULTI_EVENT_TRIGGERING_IDX", "Indent" = "4", "Prefix" = "''"!]
      [!ENDIF!]
    [!WS "2"!]},
  [!ENDLOOP!][!//
[!ENDIF!][!//
};

#define DEM_STOP_SEC_CALIB_32
#include <Dem_MemMap.h>

[!IF "not(DemGeneral/DemEventCombinationSupport = 'DEM_EVCOMB_DISABLED')"!][!//
#define DEM_START_SEC_CONST_32
#include <Dem_MemMap.h>

/** \brief DTC configuration description table (uses 'bit packing scheme)
 ** contains DTC related configuration data for
 **   - events without event combination
 **   - the master configuration event in case of event combination
 **   - events without DTC class (referred to by Dem_EventDesc[] entry)
 **/
CONST(Dem_DTCDescType, DEM_CONST) Dem_DTCDesc[DEM_NUMBER_OF_DTCS] =
{
[!/* creation of 'PriorityValueList' (unique list of all configured
   * priority values ordered in ascending order) was done via CREATE_PRIORITY_VALUE_LIST */!][!//
[!//
  [!FOR "DTCCtr" = "0" TO "num:integer(count(text:split($DemDTCClassList, '#')))"!][!//
    [!VAR "DTCClassName" = "text:split($DemDTCClassList, '#')[position() = $DTCCtr]"!][!//
[!/* for performance reasons, looping is not performed in DemEventId order (not needed) */!][!//
    [!LOOP "as:modconf('Dem')[1]/DemConfigSet/*/DemEventParameter/*"!][!//
[!//
      [!IF "node:exists(DemDTCClassRef)"!][!//
[!/* for performance reasons, the name (always unique) is extracted from the SHORT-NAME-path string
   * instead of using the node function "name(as:ref(DemDTCClassRef))" */!][!//
        [!VAR "EventDTCClassName" = "text:split(DemDTCClassRef, '/')[last()]"!][!//
      [!ELSE!][!/* No DTC class or no UDS-DTC configured */!][!//
        [!VAR "EventDTCClassName" = "node:name(.)"!][!//
      [!ENDIF!][!//
      [!IF "$DTCClassName = $EventDTCClassName"!][!//
[!/* output is generated for events without DTC class, events without event combination and for
   * the master configuration event of event combinations */!][!//
        [!IF "not(node:exists(DemDTCClassRef)) or not(node:exists(DemEvtCmbCommonParamMaster)) or (DemEvtCmbCommonParamMaster = 'true')"!][!//
          [!IF "(DemEvtCmbCommonParamMaster = 'true')"!][!//
            [!WS "2"!]/* UDS-DTC of Master Configuration Event: [!"name(.)"!] */
          [!ELSE!][!//
            [!WS "2"!]/* UDS-DTC of Event: [!"name(.)"!] */
          [!ENDIF!][!//
          [!WS "2"!]{
[!/*  - [UdsDTC] - */!][!//
          [!CALL "EVENT_DESC_BITFIELD_UDS_DTC_VALUE", "Indent" = "4", "Prefix" = "''"!]
[!/*  - [DTCOrigin] - */!][!//
          [!CALL "EVENT_DESC_BITFIELD_DTC_ORIGIN", "Indent" = "4"!]
[!/*  - [DTCSeverity] - */!][!//
          [!CALL "EVENT_DESC_BITFIELD_DTC_SEVERITY", "Indent" = "4"!]
[!/*  - [ImmediateStorage] - */!][!//
[!/* !LINKSTO Dem.ImmediateStorage.InternalEvents,1 */!][!//
          [!CALL "EVENT_DESC_BITFIELD_IMMEDIATE_STORAGE", "Indent" = "4"!]
          [!WS "4"!],
[!/*  - [Priority] - */!][!//
          [!CALL "EVENT_DESC_BITFIELD_EVENT_PRIORITY", "Indent" = "4", "Prefix" ="''"!]
[!/*  - [FFClassIdx] & [FFRecInfo] - */!][!//
          [!CALL "EVENT_DESC_BITFIELD_FREEZE_FRAMES", "Indent" = "4"!]
[!/*  - [EDClassIdx] - */!][!//
          [!CALL "EVENT_DESC_BITFIELD_EXTENDED_DATA_CLASS", "Indent" = "4"!]
[!/*  - [AgingCycleIdx] - */!][!//
          [!CALL "EVENT_DESC_BITFIELD_AGING_CYCLE_ID", "Indent" = "4"!]
[!/*  - [AgingCycleThreshold] - */!][!//
          [!CALL "EVENT_DESC_BITFIELD_AGING_CTR_THRESHOLD", "Indent" = "4"!]
[!/*  - [DTCFuncUnit] - */!][!//
          [!CALL "EVENT_DESC_BITFIELD_FUNCTIONAL_UNIT", "Indent" = "4"!]
[!/*  - [DTCGroup] - */!][!//
          [!CALL "EVENT_DESC_BITFIELD_DTC_GROUP", "Indent" = "4"!]
[!/*  - [EvSignificance] - */!][!//
[!/* !LINKSTO Dem.DemInternalDataElementRange.EventSignificance,1 */!][!//
          [!CALL "EVENT_DESC_BITFIELD_EVENT_SIGNIFICANCE", "Indent" = "4"!]
        [!WS "2"!]},
[!/* If output was generated, stop searching for matching EventDTCClassname for the current DTCClassname since
   * only one event is mapped to an UDS-DTC (the Master Configuration Event in case of event combination) */!][!//
          [!BREAK!][!//
        [!ENDIF!][!//
      [!ENDIF!][!//
    [!ENDLOOP!][!//
  [!ENDFOR!][!//
};

#define DEM_STOP_SEC_CONST_32
#include <Dem_MemMap.h>

[!IF "$DemNbOfCmbDTCs > 0"!][!//
#define DEM_START_SEC_CONST_16
#include <Dem_MemMap.h>
CONST(Dem_CmbDTCEvLnkType, DEM_CONST) Dem_CmbDTCEvLnk[DEM_NUM_CMB_DTCS + 1U] =
{
  [!LOOP "text:split($DemCmbDTCEvLnkList, '#')"!][!//
    [!WS "2"!][!"num:integer(.)"!]U,
  [!ENDLOOP!][!//
};

CONST(Dem_EventIdType, DEM_CONST) Dem_CmbDTCEvents[DEM_NUM_CMB_EVENTS] =
{
  [!VAR "DTCClassName" = "''"!]
  [!FOR "EventCtr" = "1" TO "$DemNbOfCmbEvents"!]
    [!VAR "EventId" = "text:split($DemCmbDTCEventsList, '#')[position() = $EventCtr]"!]
    [!IF "not($DTCClassName = name(as:ref(DemConfigSet/*/DemEventParameter/*[DemEventId = $EventId]/DemDTCClassRef)))"!]
      [!VAR "DTCClassName" = "name(as:ref(DemConfigSet/*/DemEventParameter/*[DemEventId = $EventId]/DemDTCClassRef))"!]
      [!WS "2"!]/* Next group of combined events assigned to DemDTCClassRef = [!"$DTCClassName"!] */
    [!ENDIF!]
    [!WS "2"!]DemConf_DemEventParameter_[!"name(DemConfigSet/*/DemEventParameter/*[DemEventId = $EventId])"!],
  [!ENDFOR!]
};
#define DEM_STOP_SEC_CONST_16
#include <Dem_MemMap.h>
[!ENDIF!][!//
[!ENDIF!]

[!IF "DemGeneral/DemEventDisplacementSupport = 'true'"!]
#define DEM_START_SEC_CONST_8
#include <Dem_MemMap.h>

CONST(Dem_PriorityType, DEM_CONST) Dem_ExternalPrioValue[DEM_NUM_EVENT_PRIORITIES] =
{
[!LOOP "text:split($PriorityValueList, '#')"!]
  [!WS "2"!](uint8)[!"."!]U,
[!ENDLOOP!]
};

#define DEM_STOP_SEC_CONST_8
#include <Dem_MemMap.h>

[!ENDIF!]

#define DEM_START_SEC_CONST_8
#include <Dem_MemMap.h>
[!VAR "BitIndex" = "1"!][!// Skip pos 0 which is allocated for invalid event
[!VAR "ByteVal" = "0"!][!//
#if (DEM_STATUS_BIT_STORAGE_TF_PER_EVENT_SUPPORT == STD_ON)
/** \brief Generate the const array Dem_StatusBitStorageTF[],
 ** only if not all configured events do have the same
 ** DemStatusBitStorageTestFailedPerEvent value. */
CONST(uint8, DEM_CONST) Dem_StatusBitStorageTF[DEM_STATUS_BIT_STORAGE_TF_ARRAY_SIZE] =
{
  [!INDENT "2"!]
  [!LOOP "node:order(DemConfigSet/*/DemEventParameter/*, 'DemEventId')"!]
    [!IF "DemStatusBitStorageTestFailedPerEvent = 'true'"!][!//
      [!VAR "ByteVal" = "bit:or($ByteVal,bit:shl(1,$BitIndex))"!]
    [!ENDIF!]
    [!VAR "BitIndex" = "$BitIndex + 1"!]
    [!IF "($BitIndex = 8)"!]
      [!"num:inttohex($ByteVal, 2)"!]U,
      [!VAR "ByteVal" = "0"!]
      [!VAR "BitIndex" = "0"!]
    [!ENDIF!]
  [!ENDLOOP!]
  [!IF "($BitIndex != 0)"!]
    [!"num:inttohex($ByteVal, 2)"!]U
  [!ENDIF!]
  [!ENDINDENT!]
};
#endif

#define DEM_STOP_SEC_CONST_8
#include <Dem_MemMap.h>

[!IF "DemGeneral/DemJ1939Support = 'true'"!]
#define DEM_START_SEC_CONST_UNSPECIFIED
#include <Dem_MemMap.h>

[!IF "DemGeneral/DemEventCombinationSupport = 'DEM_EVCOMB_DISABLED'"!][!//
/** \brief J1939 specific event configuration description table
 * (partially uses 'bit packing scheme) */
CONST(Dem_J1939ConfigType, DEM_CONST) Dem_J1939Config[DEM_NUMBER_OF_EVENTS] =
{
  [!INDENT "2"!][!//
    [!IF "(as:modconf('Dem')[1]/DemGeneral/DemGeneralJ1939/DemJ1939FreezeFrameSupport = 'true')"!][!//
      {0U, DEM_J1939_INVALID_NODE_ID ,DEM_J1939FFCLS_NULL_IDX},                                     /* invalid event id entry */
    [!ELSE!][!/* No DemJ1939FreezeFrameSupport support */!][!//
      {0U, DEM_J1939_INVALID_NODE_ID},                                     /* invalid event id entry */
    [!ENDIF!][!//
    [!LOOP "node:order(DemConfigSet/*/DemEventParameter/*, 'DemEventId')"!][!//
      { /* [!"name(.)"!] */
      [!INDENT "4"!][!//
        [!/*  -----[Dem_J1939DTC]----- */!][!//
        [!IF "node:exists(DemDTCClassRef) and node:exists(as:ref(DemDTCClassRef)/DemJ1939DTCValue)"!][!//
          [!"translate(num:inttohex(as:ref(DemDTCClassRef)/DemJ1939DTCValue, 6), 'abcdef', 'ABCDEF')"!]U, /* J1939-DTC value */
        [!ELSE!][!/* No DTC class or no OBD-DTC configured */!][!//
          (uint32)(DEM_NO_DTC), /* No J1939-DTC Value */
        [!ENDIF!][!//
        [!IF "node:exists(DemDTCClassRef) and node:exists(as:ref(DemDTCClassRef)/DemJ1939DTCValue)"!][!//
          [!/*  -----[Dem_J1939NodeId]----- */!][!//
          [!"node:value(as:ref(as:ref(./DemDTCClassRef)/DemJ1939NodeRef)/J1939NmNodeId)"!]U ,/* Node ID */
        [!ELSE!][!/* No node is referenced */!][!//
          (uint8)(DEM_J1939_INVALID_NODE_ID), /* No node referenced */
        [!ENDIF!][!//
        [!IF "(as:modconf('Dem')[1]/DemGeneral/DemGeneralJ1939/DemJ1939FreezeFrameSupport = 'true')"!][!//
          [!IF "((node:refvalid(./DemJ1939FreezeFrameClassRef)) and (node:exists(as:ref(./DemDTCClassRef)/DemJ1939DTCValue)))"!][!//
          [!/*  -----[DemJ1939FreezeFrameClassRef]----- */!][!//
          [!"as:ref(./DemJ1939FreezeFrameClassRef)/@index"!]U /* J1939 Freeze Frame Class Index - [!"as:ref(./DemJ1939FreezeFrameClassRef)/@name"!] */
          [!ELSE!][!/* No node is referenced */!][!//
            (Dem_J1939FFClassIdxType)(DEM_J1939FFCLS_NULL_IDX) /* No Configured J1939Freeze Frame */
          [!ENDIF!][!//
        [!ENDIF!][!//
      [!ENDINDENT!][!//
      },
      [!//
    [!ENDLOOP!][!//
  [!ENDINDENT!]
};
[!ELSE!][!// "DemGeneral/DemEventCombinationSupport != 'DEM_EVCOMB_DISABLED'
/** \brief J1939 specific event configuration description table
 * (partially uses 'bit packing scheme) */
CONST(Dem_J1939ConfigType, DEM_CONST) Dem_J1939Config[DEM_NUMBER_OF_DTCS] =
{
  [!INDENT "0"!]
    [!FOR "DTCCtr" = "0" TO "num:integer(count(text:split($DemDTCClassList, '#')))"!][!//
      [!VAR "DTCClassName" = "text:split($DemDTCClassList, '#')[position() = $DTCCtr]"!][!//
      [!/* for performance reasons, looping is not performed in DemEventId order (not needed) */!][!//
      [!LOOP "as:modconf('Dem')[1]/DemConfigSet/*/DemEventParameter/*"!][!//
        [!IF "node:exists(DemDTCClassRef)"!][!//
          [!/* for performance reasons, the name (always unique) is extracted from the SHORT-NAME-path string
             * instead of using the node function "name(as:ref(DemDTCClassRef))" */!][!//
          [!VAR "EventDTCClassName" = "text:split(DemDTCClassRef, '/')[last()]"!][!//
        [!ELSE!][!/* No DTC class or no J1939-DTC configured */!][!//
          [!VAR "EventDTCClassName" = "node:name(.)"!][!//
        [!ENDIF!][!//
        [!IF "$DTCClassName = $EventDTCClassName"!][!//
          [!/* output is generated for events without DTC class, events without event combination and for
             * the master configuration event of event combinations */!][!//
          [!IF "not(node:exists(DemDTCClassRef)) or not(node:exists(DemEvtCmbCommonParamMaster)) or (DemEvtCmbCommonParamMaster = 'true')"!][!//
            [!IF "(DemEvtCmbCommonParamMaster = 'true')"!][!//
              [!WS "2"!]/* J1939-DTC of Master Configuration Event: [!"name(.)"!] */
            [!ELSE!][!//
              [!WS "2"!]/* J1939-DTC of Event: [!"name(.)"!] */
            [!ENDIF!][!//
            [!WS "2"!]{
            [!/*  - [J1939 DTC] - */!][!//
            [!IF "node:exists(DemDTCClassRef) and node:exists(as:ref(DemDTCClassRef)/DemJ1939DTCValue)"!][!//
              [!WS "4"!][!"translate(num:inttohex(as:ref(DemDTCClassRef)/DemJ1939DTCValue, 6), 'abcdef', 'ABCDEF')"!]U, /* J1939-DTC value */
            [!ELSE!][!/* No DTC class or no J1939-DTC configured */!][!//
              [!WS "4"!](uint32)(DEM_NO_DTC), /* No J1939-DTC Value */
            [!ENDIF!][!//
            [!IF "node:exists(DemDTCClassRef) and node:exists(as:ref(DemDTCClassRef)/DemJ1939DTCValue)"!][!//
              [!/*  -----[Dem_J1939NodeId]----- */!][!//
              [!WS "4"!][!"node:value(as:ref(as:ref(./DemDTCClassRef)/DemJ1939NodeRef)/J1939NmNodeId)"!]U,/* Node ID */
            [!ELSE!][!/* No node is referenced */!][!//
              [!WS "4"!](uint8)(DEM_J1939_INVALID_NODE_ID),/* No node referenced */
            [!ENDIF!][!//
            [!IF "(as:modconf('Dem')[1]/DemGeneral/DemGeneralJ1939/DemJ1939FreezeFrameSupport = 'true')"!][!//
              [!IF "((node:exists(as:ref(DemDTCClassRef)/DemJ1939DTCValue)) and (node:refvalid(./DemJ1939FreezeFrameClassRef)))"!][!//
                [!/*  -----[DemJ1939FreezeFrameClassRef]----- */!][!//
                [!WS "4"!][!"as:ref(./DemJ1939FreezeFrameClassRef)/@index"!]U /* J1939 Freeze Frame Class Index - [!"as:ref(./DemJ1939FreezeFrameClassRef)/@name"!] */
              [!ELSE!][!/* No node is referenced */!][!//
                [!WS "4"!](Dem_J1939FFClassIdxType)(DEM_J1939FFCLS_NULL_IDX) /* No Configured J1939Freeze Frame Class */
              [!ENDIF!][!//
            [!ENDIF!][!//
            [!WS "2"!]},
            [!/* If output was generated, stop searching for matching EventDTCClassname for the current DTCClassname since
               * only one event is mapped to an UDS-DTC (the Master Configuration Event in case of event combination) */!][!//
            [!BREAK!][!//
          [!ENDIF!][!//
        [!ENDIF!][!//
      [!//
      [!ENDLOOP!][!//
    [!ENDFOR!][!//
  [!ENDINDENT!][!//
};
[!ENDIF!][!//

/*------------------[J1939 Freeze Frame configuration]----------------------------------*/
[!IF "as:modconf('Dem')[1]/DemGeneral/DemGeneralJ1939/DemJ1939FreezeFrameSupport = 'true' "!]
[!//
[!INDENT "0"!]
  [!IF "count(DemGeneral/DemSPNClass/*) > 0"!]

    /** \brief J1939 Freeze frame SPNs */
    CONST(Dem_DataElementIdxType, DEM_CONST)
    Dem_J1939FFSPNs[DEM_NUM_J1939FFSPNS] =
    {
      [!LOOP "DemGeneral/DemSPNClass/*"!]
        [!WS "2"!]/* [!"name(.)"!] */
        [!VAR "NameInSPN" = "name(as:ref(DemSPNDataElementClassRef))"!]
          [!VAR "IdxCtr" = "0"!]
          [!LOOP "../../DemDataElementClass/*[. = 'DemExternalCSDataElementClass']"!]
            [!IF "$NameInSPN = name(.)"!]
            [!WS "2"!][!"num:integer($IdxCtr)"!]U, /* [!"$NameInSPN"!] */
              [!BREAK!]
            [!ELSE!][!//
              [!VAR "IdxCtr" = "$IdxCtr + 1"!]
            [!ENDIF!]
          [!ENDLOOP!]
      [!ENDLOOP!]
    };

    [!LOOP "DemGeneral/DemJ1939FreezeFrameClass/*"!]

      /*---[Freeze frame class [!"name(.)"!]]---*/
      [!VAR "NumJ1939FFSegs" = "count(DemSPNClassRef/*)"!]

      /** \brief Indexes of SPNs on freeze frame class
       ** [!"name(.)"!] to ::Dem_J1939FFSPNs[]
       **/
      STATIC CONST(Dem_J1939FFSPNIdxType, DEM_CONST)
      Dem_J1939FFClassIdx[!"name(.)"!][[!"num:integer($NumJ1939FFSegs)"!]] =
      {
        [!LOOP "DemSPNClassRef/*"!]
          [!WS "2"!][!"as:ref(.)/@index"!]U, /* [!"name(as:ref(.))"!] */
        [!ENDLOOP!]
      };
    [!ENDLOOP!]


    /** \brief Array containing all J1939 freeze frame classes */
    CONST(Dem_J1939FFClassType, DEM_CONST)
    Dem_J1939FFClass[DEM_NUM_J1939FFCLS] =
    {
      [!INDENT "2"!]
        [!LOOP "DemGeneral/DemJ1939FreezeFrameClass/*"!]
          {
            [!VAR "NumJ1939FFSegs" = "count(DemSPNClassRef/*)"!]
            [!WS "2"!]Dem_J1939FFClassIdx[!"name(.)"!],
            [!WS "2"!][!"num:integer($NumJ1939FFSegs)"!]U /* number of SPNs */
          },
        [!ENDLOOP!]
          {
            [!WS "2"!]NULL_PTR,
            [!WS "2"!]0U
          }
      [!ENDINDENT!]
    };

  [!ENDIF!]
[!ENDINDENT!]
[!ENDIF!]
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include <Dem_MemMap.h>
[!ENDIF!][!//

[!IF "DemGeneral/DemOBDSupport = 'true'"!]
#define DEM_START_SEC_CALIB_16
#include <Dem_MemMap.h>

/** \brief OBD specific event configuration description table
 * (partially uses 'bit packing scheme) */
[!IF "DemGeneral/DemEventCombinationSupport = 'DEM_EVCOMB_DISABLED'"!][!//
/* !LINKSTO dsn.Dem_OBD_0018,2 */
/* !LINKSTO dsn.Dem.OBD.config.0001,2 */
CONST(Dem_OBDConfigType, DEM_CONST) Dem_OBDConfig[DEM_NUMBER_OF_EVENTS] =
{
  [!INDENT "2"!]
  { 0U, 0U },                                     /* invalid event id entry */
  [!LOOP "node:order(DemConfigSet/*/DemEventParameter/*, 'DemEventId')"!]
  { /* [!"name(.)"!] */
    [!INDENT "4"!]
    [!/*  -----[Dem_ObdDTC]----- */!][!//
      [!IF "node:exists(DemDTCClassRef) and node:exists(as:ref(DemDTCClassRef)/DemObdDTC)"!]
        [!/* !LINKSTO dsn.Dem.OBD.config.ObdDTC.0002,1 */!][!//
        [!"translate(num:inttohex(as:ref(DemDTCClassRef)/DemObdDTC, 4), 'abcdef', 'ABCDEF')"!]U, /* OBD-DTC value */
      [!ELSE!][!/* No DTC class or no OBD-DTC configured */!]
        [!/* !LINKSTO dsn.Dem.OBD.config.ObdDTC.0003,1 */!][!//
        (uint16)(DEM_NO_DTC), /* No OBD-DTC Value */
      [!ENDIF!]
      [!//
      [!/*  -----[Dem_OBDEvConf1]----- */!][!//
      [!/* !LINKSTO dsn.Dem.OBD.config.OBDEvConf1.MinRatioEvent.0001,2, dsn.Dem.OBD.config.OBDEvConf1.0002,1 */!][!//
      [!IF "node:exists(DemEventClass/DemMinRatioEvent) and DemEventClass/DemMinRatioEvent = 'true'"!]
        1U /* Minimum Ratio Event is set */
      [!ELSE!][!//
        0U /* Minimum Ratio Event is not set */
      [!ENDIF!]
      [!IF "$CalibrationSupport = 'false'"!][!//
        [!/*  -----[Dem_EventOBDReadinessGroupType]----- */!][!//
        [!/* !LINKSTO Dem.ASR422.SWS_Dem_00908,1 */!][!//
        [!/* !LINKSTO dsn.Dem.OBD.config.OBDEvConf1.DemEventOBDReadinessGroup.0001,3 */!][!//
        | ((uint16)(DEM_OBD_READINESS_MASK & [!"DemEventClass/DemEventOBDReadinessGroup"!]) <<
           [!WS "4"!]DEM_OBD_READINESS_OFFSET) /* OBD Readiness Group is set */
        [!/*  -----[DemOBDAgingCycleCounterThreshold]----- */!][!//
        [!/* !LINKSTO dsn.Dem.OBDAging.DemOBDAgingCycleCounterThreshold,2 */!][!//
        [!IF "DemEventClass/DemAgingAllowed = 'true'"!][!//
          | ((uint16)(DEM_OBD_AGING_CYC_CNT_THRESHOLD_MASK & [!"DemEventClass/DemOBDAgingCycleCounterThreshold"!]U) <<
             [!WS "4"!]DEM_OBD_AGING_CYC_CNT_THRESHOLD_OFFSET) /* OBD Aging Cycle Counter Threshold */
        [!ELSE!][!//
          | ((uint16)(DEM_OBD_AGING_CYC_CNT_THRESHOLD_MASK & DEM_AGING_CYC_CNT_DISABLED) <<
             [!WS "4"!]DEM_OBD_AGING_CYC_CNT_THRESHOLD_OFFSET) /* OBD Aging Cycle Counter Threshold Disabled */
        [!ENDIF!]
      [!ENDIF!]
    [!ENDINDENT!]
  },
  [!//
  [!ENDLOOP!]
  [!ENDINDENT!]
};
[!ELSE!][!// "DemGeneral/DemEventCombinationSupport != 'DEM_EVCOMB_DISABLED'
/* !LINKSTO dsn.Dem_OBD_0019,1 */
/* !LINKSTO dsn.Dem.OBD.config.0002,1 */
CONST(Dem_OBDConfigType, DEM_CONST) Dem_OBDConfig[DEM_NUMBER_OF_DTCS] =
{
  [!INDENT "0"!]
  [!FOR "DTCCtr" = "0" TO "num:integer(count(text:split($DemDTCClassList, '#')))"!][!//
    [!VAR "DTCClassName" = "text:split($DemDTCClassList, '#')[position() = $DTCCtr]"!][!//
[!/* for performance reasons, looping is not performed in DemEventId order (not needed) */!][!//
    [!LOOP "as:modconf('Dem')[1]/DemConfigSet/*/DemEventParameter/*"!][!//
      [!IF "node:exists(DemDTCClassRef)"!][!//
[!/* for performance reasons, the name (always unique) is extracted from the SHORT-NAME-path string
   * instead of using the node function "name(as:ref(DemDTCClassRef))" */!][!//
        [!VAR "EventDTCClassName" = "text:split(DemDTCClassRef, '/')[last()]"!][!//
      [!ELSE!][!/* No DTC class or no OBD-DTC configured */!][!//
        [!VAR "EventDTCClassName" = "node:name(.)"!][!//
      [!ENDIF!][!//
      [!IF "$DTCClassName = $EventDTCClassName"!][!//
[!/* output is generated for events without DTC class, events without event combination and for
   * the master configuration event of event combinations */!][!//
        [!IF "not(node:exists(DemDTCClassRef)) or not(node:exists(DemEvtCmbCommonParamMaster)) or (DemEvtCmbCommonParamMaster = 'true')"!][!//
          [!IF "(DemEvtCmbCommonParamMaster = 'true')"!][!//
            [!WS "2"!]/* OBD-DTC of Master Configuration Event: [!"name(.)"!] */
          [!ELSE!][!//
            [!WS "2"!]/* OBD-DTC of Event: [!"name(.)"!] */
          [!ENDIF!][!//
          [!WS "2"!]{
[!/*  - [OBD DTC] - */!][!//
          [!CALL "EVENT_DESC_BITFIELD_OBD_DTC_VALUE", "Indent" = "4", "Prefix" = "''"!]
[!/*  - [Min Event Ratio] - */!][!//
          [!CALL "EVENT_DESC_BITFIELD_OBD_MIN_RATIO_VALUE", "Indent" = "4", "Prefix" = "''"!]
[!/*  - [Readiness Group] - */!][!//
          [!CALL "EVENT_DESC_BITFIELD_OBD_READINESS_VALUE", "Indent" = "4", "Prefix" = "'| '"!]
          [!WS "2"!]},
[!/* If output was generated, stop searching for matching EventDTCClassname for the current DTCClassname since
   * only one event is mapped to an UDS-DTC (the Master Configuration Event in case of event combination) */!][!//
          [!BREAK!][!//
        [!ENDIF!][!//
      [!ENDIF!][!//
    [!//
    [!ENDLOOP!][!//
  [!ENDFOR!][!//
  [!ENDINDENT!][!//
};
[!ENDIF!][!//

#define DEM_STOP_SEC_CALIB_16
#include <Dem_MemMap.h>



[!IF "(node:exists(DemGeneral/DemGeneralOBD/DemOBDEngineType)) and
      (DemGeneral/DemGeneralOBD/DemOBDEngineType = 'DEM_IGNITION_COMPRESSION') and
      (DemGeneral/DemGeneralJ1939/DemJ1939Readiness1Support = 'true')"!][!//
#define DEM_START_SEC_CONST_8
#include <Dem_MemMap.h>
CONST(Dem_EventOBDReadinessGroupType, DEM_CONST)
  Dem_J1939ReadinessGroupCompressionMap[8] =
{
  DEM_OBD_J1939_RDY_HCCAT,
  DEM_OBD_J1939_RDY_NOXCAT,
  DEM_OBD_J1939_RDY_NONE,
  DEM_OBD_J1939_RDY_BOOSTPR,
  DEM_OBD_J1939_RDY_NONE,
  DEM_OBD_J1939_RDY_EGSENS,
  DEM_OBD_J1939_RDY_PMFLT,
  DEM_OBD_J1939_RDY_EGR
};
#define DEM_STOP_SEC_CONST_8
#include <Dem_MemMap.h>
[!ENDIF!]

[!IF "$CalibrationSupport = 'true'"!]
#define DEM_START_SEC_CONST_8
#include <Dem_MemMap.h>
CONST(Dem_EventOBDReadinessGroupType, DEM_CONST)
  Dem_OBDReadinessGroupMap[DEM_NUM_OBD_RDY_GROUPS] =
{
  /* OBD engine type unspecific */
  DEM_OBD_RDY_MISF, /* DataB:0 */
  DEM_OBD_RDY_FLSYS,  /* DataB:1 */
  DEM_OBD_RDY_CMPRCMPT, /* DataB:2 */
  DEM_OBD_RDY_EGR,
  DEM_OBD_RDY_FLSYS_NONCONT,

[!IF "node:exists(DemGeneral/DemGeneralOBD/DemOBDEngineType) and (DemGeneral/DemGeneralOBD/DemOBDEngineType = 'DEM_IGNITION_SPARK')"!][!//
  /* OBD engine type specific: 'spark' */
  DEM_OBD_RDY_CAT, /* DataC:0 */
  DEM_OBD_RDY_HTCAT, /* DataC:1 */
  DEM_OBD_RDY_EVAP, /* DataC:2 */
  DEM_OBD_RDY_SECAIR, /* DataC:3 */
  DEM_OBD_RDY_O2SENS, /* DataC:5 */
  DEM_OBD_RDY_O2SENSHT, /* DataC:6 */
  DEM_OBD_RDY_AC,
[!ELSE!][!//
  /* OBD engine type specific: 'compression' */
  DEM_OBD_RDY_HCCAT, /* DataC:0 */
  DEM_OBD_RDY_NOXCAT, /* DataC:1 */
  DEM_OBD_RDY_NONE, /* unused */
  DEM_OBD_RDY_BOOSTPR, /* DataC:3 */
  DEM_OBD_RDY_EGSENS, /* DataC:5 */
  DEM_OBD_RDY_PMFLT, /* DataC:6 */
  DEM_OBD_RDY_NONE, /* unused */
[!ENDIF!]
};

#define DEM_STOP_SEC_CONST_8
#include <Dem_MemMap.h>

#define DEM_START_SEC_CALIB_16
#include <Dem_MemMap.h>

[!/* !LINKSTO dsn.Dem.OBD.config.OBDEvConf1.DemEventOBDReadinessGroup.0002,1 */!][!//
CONST(Dem_EventIdType, DEM_CONST)
  Dem_OBDReadinessGroup[DEM_NUM_OBD_RDY_GROUPS][DEM_MAX_EVENTS_PER_READINESS_GROUP] =
  {
    [!CALL "GET_EVENTS_IN_READINESSGROUP", "GroupName" = "'DEM_OBD_RDY_MISF'"!]
    [!CALL "GET_EVENTS_IN_READINESSGROUP", "GroupName" = "'DEM_OBD_RDY_FLSYS'"!]
    [!CALL "GET_EVENTS_IN_READINESSGROUP", "GroupName" = "'DEM_OBD_RDY_CMPRCMPT'"!]
    [!CALL "GET_EVENTS_IN_READINESSGROUP", "GroupName" = "'DEM_OBD_RDY_EGR'"!]
    [!CALL "GET_EVENTS_IN_READINESSGROUP", "GroupName" = "'DEM_OBD_RDY_FLSYS_NONCONT'"!]
[!IF "node:exists(DemGeneral/DemGeneralOBD/DemOBDEngineType) and (DemGeneral/DemGeneralOBD/DemOBDEngineType = 'DEM_IGNITION_SPARK')"!][!//
    [!CALL "GET_EVENTS_IN_READINESSGROUP", "GroupName" = "'DEM_OBD_RDY_CAT'"!]
    [!CALL "GET_EVENTS_IN_READINESSGROUP", "GroupName" = "'DEM_OBD_RDY_HTCAT'"!]
    [!CALL "GET_EVENTS_IN_READINESSGROUP", "GroupName" = "'DEM_OBD_RDY_EVAP'"!]
    [!CALL "GET_EVENTS_IN_READINESSGROUP", "GroupName" = "'DEM_OBD_RDY_SECAIR'"!]
    [!CALL "GET_EVENTS_IN_READINESSGROUP", "GroupName" = "'DEM_OBD_RDY_O2SENS'"!]
    [!CALL "GET_EVENTS_IN_READINESSGROUP", "GroupName" = "'DEM_OBD_RDY_O2SENSHT'"!]
    [!CALL "GET_EVENTS_IN_READINESSGROUP", "GroupName" = "'DEM_OBD_RDY_AC'"!]
[!ELSE!][!//
    [!CALL "GET_EVENTS_IN_READINESSGROUP", "GroupName" = "'DEM_OBD_RDY_HCCAT'"!]
    [!CALL "GET_EVENTS_IN_READINESSGROUP", "GroupName" = "'DEM_OBD_RDY_NOXCAT'"!]
    [!CALL "GET_EVENTS_IN_READINESSGROUP", "GroupName" = "'UNUSED'"!]
    [!CALL "GET_EVENTS_IN_READINESSGROUP", "GroupName" = "'DEM_OBD_RDY_BOOSTPR'"!]
    [!CALL "GET_EVENTS_IN_READINESSGROUP", "GroupName" = "'DEM_OBD_RDY_EGSENS'"!]
    [!CALL "GET_EVENTS_IN_READINESSGROUP", "GroupName" = "'DEM_OBD_RDY_PMFLT'"!]
    [!CALL "GET_EVENTS_IN_READINESSGROUP", "GroupName" = "'UNUSED'"!]
[!ENDIF!]
  };

#define DEM_STOP_SEC_CALIB_16
#include <Dem_MemMap.h>
[!ENDIF!][!// "$CalibrationSupport = 'true'"
[!ENDIF!][!// "DemGeneral/DemOBDSupport = 'true'"

[!IF "DemGeneral/DemDevFreezeFrameSupport = 'true'"!]
#define DEM_START_SEC_CALIB_16
#include <Dem_MemMap.h>

/** \brief The array contains Development Freeze Frame classes
 *   for Freeze Frame A and Freeze Frame B and associated
 *   record number */
/* !LINKSTO dsn.Dem.DevFF.EventDescDevFF,1 */
CONST(Dem_EventDescDevFFType, DEM_CONST) Dem_EventDescDevFF[DEM_NUMBER_OF_EVENTS][DEM_NUM_OF_DEVFF_KIND] =
{
  { {0U, 0U}, {0U, 0U} },                             /* invalid event id entry */
  [!LOOP "node:order(DemConfigSet/*/DemEventParameter/*, 'DemEventId')"!]
    [!WS "2"!]{ /* event: [!"name(.)"!] */
      [!WS "4"!]{ /* Freeze Frame A */
        [!IF "(node:exists(DemDevFreezeFrameAClassRef))"!]
          [!WS "6"!]DEM_FFCLS_IDX_[!"name(as:ref(./DemDevFreezeFrameAClassRef))"!],
        [!ELSE!][!//
          [!WS "6"!]DEM_FFCLS_NULL_IDX,
        [!ENDIF!]
        [!IF "(node:exists(DemDevFreezeFrameAClassRef)) or (node:exists(DemDevFreezeFrameARecNum))"!]
          [!WS "6"!][!"translate(num:inttohex(./DemDevFreezeFrameARecNum, 2), 'abcdef', 'ABCDEF')"!]U
        [!ELSE!][!//
          [!WS "6"!]0x00U
        [!ENDIF!]
      [!WS "4"!]},
      [!WS "4"!]{ /* Freeze Frame B */
        [!IF "(node:exists(DemDevFreezeFrameBClassRef))"!]
          [!WS "6"!]DEM_FFCLS_IDX_[!"name(as:ref(./DemDevFreezeFrameBClassRef))"!],
        [!ELSE!][!//
          [!WS "6"!]DEM_FFCLS_NULL_IDX,
        [!ENDIF!]
        [!IF "(node:exists(DemDevFreezeFrameBClassRef))"!]
          [!WS "6"!][!"translate(num:inttohex(./DemDevFreezeFrameBRecNum, 2), 'abcdef', 'ABCDEF')"!]U
        [!ELSE!][!//
          [!WS "6"!]0x00U
        [!ENDIF!]
      [!WS "4"!]}
    [!WS "2"!]},
  [!ENDLOOP!]
};
#define DEM_STOP_SEC_CALIB_16
#include <Dem_MemMap.h>
[!ENDIF!]

/*------------------[Enable conditions configuration]-----------------------*/

[!INDENT "0"!]
  #define DEM_START_SEC_VAR_INIT_8
  #include <Dem_MemMap.h>

  [!IF "count(DemGeneral/DemEnableCondition/*) > 0"!]
    /** \brief Enable condition status */
    VAR(boolean, DEM_VAR) Dem_EnCondStatus[DEM_NUM_ENABLECONDITIONS] =
    {
      [!VAR "MaxEnableConditionId" = "count(DemGeneral/DemEnableCondition/*) - 1"!]
      [!FOR "EnableConditionId" = "0" TO "$MaxEnableConditionId"!]
        [!SELECT "DemGeneral/DemEnableCondition/*[DemEnableConditionId = $EnableConditionId]"!]
          [!IF "DemEnableConditionStatus = 'true'"!]
            [!WS "2"!]TRUE,  [!//
          [!ELSE!]
            [!WS "2"!]FALSE, [!//
          [!ENDIF!]
          /* condition name: [!"name(.)"!] */
        [!ENDSELECT!]
      [!ENDFOR!]
    };
  [!ENDIF!]

  [!IF "count(DemGeneral/DemEnableConditionGroup/*) > 0"!]
    /** \brief Enable condition group status counter array,
    [!WS!]**   holds the status counters per enable condition group, that counts how
    [!WS!]**   many enable conditions, that are linked to the dedicated condition
    [!WS!]**   group are enabled/disabled. (max. 256 conditions per group) */
    VAR(Dem_EnCondGrpStatusCounterType, DEM_VAR)
      [!WS "2"!]Dem_EnCondGrpStatusCounter[DEM_NUM_ENCONDGROUPS + 1U] =
    {
      [!INDENT "2"!]
        [!VAR "GrpCntInitVal" = "0"!]
        [!LOOP "DemGeneral/DemEnableConditionGroup/*"!]
          [!LOOP "DemEnableConditionRef/*"!]
            [!/* calculate initial counter value according initial EnCond. status */!][!//
            [!IF "as:ref(.)/DemEnableConditionStatus = 'false'"!]
              [!VAR "GrpCntInitVal" = "$GrpCntInitVal + 1"!]
            [!ENDIF!]
          [!ENDLOOP!]
          [!"num:i($GrpCntInitVal)"!]U, /* group counter: [!"name(.)"!] */
          [!VAR "GrpCntInitVal" = "0"!]
        [!ENDLOOP!]
        0U  /* Dummy entry for internal processing */
      [!ENDINDENT!]
    };
  [!ENDIF!]
  #define DEM_STOP_SEC_VAR_INIT_8
  #include <Dem_MemMap.h>

  [!IF "count(DemGeneral/DemEnableConditionGroup/*) > 0"!]
    #define DEM_START_SEC_CONST_16
    #include <Dem_MemMap.h>

    /** \brief Enable condition link array,
    [!WS!]** holds the start indexes of Dem_EnCondGrpLink[] */
    CONST(Dem_EnCondLnkType, DEM_CONST) Dem_EnCondToEvIdLink[DEM_NUM_ENABLECONDITIONS + 1U] =
    {
      [!INDENT "2"!]
        [!VAR "EnCondCnt" = "0"!]
        [!/* number of condition groups, referencing enable conditions */!][!//
        [!VAR "NrOfGrpRefEnCond" = "0"!]
        [!VAR "EnCondId" = "0"!]
        [!VAR "EnCondName" = "''"!]
        [!LOOP "node:order(DemGeneral/DemEnableCondition/*, 'DemEnableConditionId')"!]
          [!VAR "EnCondName" = "name(.)"!]
          [!VAR "EnCondId" = "num:i(DemEnableConditionId)"!]
          [!"num:i($NrOfGrpRefEnCond)"!]U, /* start index, [!"$EnCondName"!] */
          [!LOOP "../../DemEnableConditionGroup/*/DemEnableConditionRef/*"!]
            [!IF "as:ref(.)/DemEnableConditionId = $EnCondId"!]
              [!/* incrementing nr of groups, that are referencing this condition */!][!//
              [!VAR "NrOfGrpRefEnCond" = "$NrOfGrpRefEnCond + 1"!]
            [!ENDIF!]
          [!ENDLOOP!]
        [!ENDLOOP!]
        [!"num:i($NrOfGrpRefEnCond)"!]U, /* dummy entry, for internal processing */
      [!ENDINDENT!]
    };

    #define DEM_STOP_SEC_CONST_16
    #include <Dem_MemMap.h>

    #define DEM_START_SEC_CONST_8
    #include <Dem_MemMap.h>

    /** \brief Enable condition group status counter link array,
    [!WS!]** holds the indexes of Dem_EnCondGrpStatusCounter[]. One enable condition
    [!WS!]** can refer to several condition group status counters. */
    CONST(Dem_EnCondGrpType, DEM_CONST)
      [!WS "2"!]Dem_EnCondGrpLink[DEM_NUM_ENCONDGRP_LINKS] =
    {
      [!INDENT "2"!]
        [!VAR "EnCondCnt" = "0"!]
        [!VAR "CurrentGrpIdx" = "0"!]
        [!VAR "EnCondName" = "''"!]
        [!VAR "EnCondId" = "0"!]
        [!VAR "EnCondGrpName" = "''"!]
        [!LOOP "node:order(DemGeneral/DemEnableCondition/*,'DemEnableConditionId')"!]
          [!VAR "EnCondName" = "name(.)"!]
          [!VAR "EnCondId" = "num:i(DemEnableConditionId)"!]
          [!LOOP "../../DemEnableConditionGroup/*/DemEnableConditionRef/*"!]
            [!IF "as:ref(.)/DemEnableConditionId = $EnCondId"!]
              [!/* incrementing nr of groups, that are referencing this condition */!][!//
              [!VAR "CurrentGrpIdx" = "node:pos(../../.)"!]
              [!VAR "EnCondGrpName" = "name(../../.)"!]
              [!"num:i($CurrentGrpIdx)"!]U, /* group: [!"$EnCondGrpName"!], condition: [!"$EnCondName"!] */
            [!ENDIF!]
          [!ENDLOOP!]
        [!ENDLOOP!]
      [!ENDINDENT!]
    };

    #define DEM_STOP_SEC_CONST_8
    #include <Dem_MemMap.h>
  [!ENDIF!]
[!ENDINDENT!]

/*------------------[Multi event triggering configuration]------------------*/

[!INDENT "0"!]
  [!IF "$NumMultiEventTriggering > 0"!]
    #define DEM_START_SEC_CONST_16
    #include <Dem_MemMap.h>

    /** \brief Multi event triggering classes array,
    [!WS!]**   holds data per each multi event triggering class configuration. */
    CONST(Dem_MultiEventTriggeringType, DEM_CONST)
      [!WS "2"!]Dem_MultiEventTriggeringClasses[DEM_MULTI_EVENT_TRIGGERING_CLASS_COUNT] =
    {
      [!LOOP "text:split($MultiEventTriggeringClassList, '#')"!][!//
      [!WS "2"!]{
        [!INDENT "4"!]
          [!VAR "SlaveStartIdx" = "text:split(.)[position() = 1]"!]
          [!VAR "SlaveNextIdx" = "text:split(.)[position() = 2]"!]
          [!VAR "Behavior" = "text:split(.)[position() = 3]"!]
          [!"$SlaveStartIdx"!]U,
          [!"$SlaveNextIdx"!]U,
          [!"$Behavior"!]
        [!ENDINDENT!]
      [!WS "2"!]},
      [!ENDLOOP!][!//
    };

    /** \brief Slave event configurations array,
    [!WS!]**   holds EventId of slave events referenced in
    [!WS!]**   multi event triggering classes */
    CONST(Dem_EventIdType, DEM_CONST)
    [!WS "2"!]Dem_MultiEventTriggeringSlaveEventId[DEM_MULTI_EVENT_TRIGGERING_SLAVE_COUNT] =
    {
    [!LOOP "text:split($MultiEventTriggeringSlaveGroupList, '#')"!][!//
      [!INDENT "2"!]
        [!VAR "SlaveEventIds" = "text:split(., ';')[position() = 2]"!]
        [!LOOP "text:split($SlaveEventIds)"!]
          [!"."!]U,
        [!ENDLOOP!]
      [!ENDINDENT!]
    [!ENDLOOP!][!//
    };

    /** \brief Master events configurations array,
    [!WS!]**   holds sorted EventId of master events referenced in
    [!WS!]**   multi event triggering configurations */
    CONST(Dem_EventIdType, DEM_CONST)
      [!WS "2"!]Dem_MultiEventTriggeringMasterEventId[DEM_MULTI_EVENT_TRIGGERING_COUNT] =
    {
      [!LOOP "text:split($MultiEventTriggeringClassIndexList, '#')"!][!//
        [!INDENT "2"!]
          [!"text:split(.,':')[position() = 1]"!]U,
        [!ENDINDENT!]
      [!ENDLOOP!][!//
    };

    #define DEM_STOP_SEC_CONST_16
    #include <Dem_MemMap.h>
  [!ENDIF!]
[!ENDINDENT!]

/*------------------[Warning indicators configuration]----------------------*/

[!INDENT "0"!]
[!VAR "NumIndicator" = "count(DemGeneral/DemIndicator/*)"!]
  [!IF "$IndicatorClassSupport = 'true'"!]
    [!CALL "CALC_INDICATOR_CLASSES"!]
    [!CALL "GET_NUMBER_OF_INDICATOR_CLASSES"!]
    [!IF "$NumberOfIndicatorClasses != 0"!]
      /* Indicator class index:
        [!LOOP "node:order(DemConfigSet/*/DemEventParameter/*, 'DemEventId')"!]
          [!CALL "GET_INDICATOR_CLASS_INDEX", "EVENT_ID" = "DemEventId"!]
          [!WS "1"!]* [!"name(.)"!]: [!"$IndicatorClassIndex"!]
        [!ENDLOOP!]
       [!WS "1"!]*/
      #if (DEM_NUM_INDICATOR_LINKS > 0U)
      #define DEM_START_SEC_CALIB_8
      #include <Dem_MemMap.h>

      CONST(Dem_IndicatorClassType, DEM_CONST) Dem_IndicatorClasses[DEM_NUM_INDICATOR_LINKS] =
      {
        [!VAR "ClassIndex" = "0"!]
        [!LOOP "text:split($IndicatorClassList, '#')"!]
          [!WS "2"!]/* Class [!"num:i($ClassIndex)"!] */
          [!VAR "ClassIndex" = "$ClassIndex + 1"!]
          [!LOOP "text:split(., '+')"!]
            [!VAR "IndicatorId" = "text:split(., ' ')[position() = 1]"!]
            [!VAR "IndicatorBehaviour" = "text:split(., ' ')[position() = 2]"!]
            [!VAR "FailureThreshold" = "text:split(., ' ')[position() = 3]"!]
            [!VAR "HealingThreshold" = "text:split(., ' ')[position() = 4]"!]
            [!VAR "IsIndFailureCycleSrcEvent" = "text:split(., ' ')[position() = 5]"!]
            [!IF "$CommonOperationCycleEnabled = 'false'"!]
              [!VAR "FailureCycle" = "text:split(., ' ')[position() = 6]"!]
              [!VAR "HealingCycle" = "text:split(., ' ')[position() = 7]"!]
            [!ENDIF!]
            [!WS "2"!]{
              [!INDENT "4"!]
                TRUE, /* Enabled */
                DemConf_DemIndicator_[!"$IndicatorId"!], /* Indicator Id */
                [!"$IndicatorBehaviour"!],
                [!"$HealingThreshold"!]U, /* Indicator Healing Cycle Counter Threshold */
                [!"$FailureThreshold"!]U, /* Indicator Failure Cycle Counter Threshold */
                [!IF "$CommonOperationCycleEnabled = 'false'"!]
                  [!"$HealingCycle"!], /* Indicator Healing Cycle */
                  [!"$FailureCycle"!], /* Indicator Failure Cycle */
                [!ENDIF!]
                /* Flag indicates, whether DemIndicatorFailureCycleSource == DEM_FAILURE_CYCLE_EVENT */
                [!"$IsIndFailureCycleSrcEvent"!],
              [!ENDINDENT!]
            [!WS "2"!]},
          [!ENDLOOP!]
        [!ENDLOOP!]

        [!FOR "IndicatorClass" = "$NumberOfIndicatorClasses" TO "DemGeneral/DemMaxNumberIndicatorClasses - 1"!]
          [!WS "2"!]{
            [!INDENT "4"!]
              FALSE, /* Enabled */
              DemConf_DemIndicator_[!"name(as:modconf('Dem')[1]/DemGeneral/DemIndicator/*[1])"!], /* Indicator id */
              DEM_INDICATOR_CONTINUOUS,
              0U, /* Indicator Healing Cycle Counter Threshold */
              0U, /* Indicator Failure Cycle Counter Threshold */
              [!IF "$CommonOperationCycleEnabled = 'false'"!]
                [!"$HealingCycle"!], /* Indicator Healing Cycle */
                [!"$FailureCycle"!], /* Indicator Failure Cycle */
              [!ENDIF!]
              /* Flag indicates, whether DemIndicatorFailureCycleSource == DEM_FAILURE_CYCLE_EVENT */
              TRUE,
            [!ENDINDENT!]
          [!WS "2"!]},
        [!ENDFOR!]
      };
      #define DEM_STOP_SEC_CALIB_8
      #include <Dem_MemMap.h>
      #endif

      #if (DEM_NUM_INDICATOR_GROUPS > 0U)
      #define DEM_START_SEC_CONST_UNSPECIFIED
      #include <Dem_MemMap.h>
      CONST(Dem_IndicatorIdxType, DEM_CONST) Dem_IndicatorGroups[DEM_NUM_INDICATOR_GROUPS] =
      {
        [!WS "2"!]0U,
        [!VAR "IndicatorClassIndex" = "0"!]
        [!LOOP "text:split($IndicatorClassList, '#')"!]
          [!VAR "IndicatorClassIndex" = "$IndicatorClassIndex + count(text:split(., '+'))"!]
          [!WS "2"!][!"num:i($IndicatorClassIndex)"!]U,
        [!ENDLOOP!]
        /* !LINKSTO VCC_DEM_006_Req115v1,1 */
        [!FOR "IndicatorClass" = "$ClassIndex" TO "DemGeneral/DemMaxNumberIndicatorClasses - 1"!]
          [!VAR "IndicatorClassIndex" = "$IndicatorClassIndex + 1"!]
          [!WS "2"!][!"num:i($IndicatorClassIndex)"!]U,
        [!ENDFOR!]
      };
      #define DEM_STOP_SEC_CONST_UNSPECIFIED
      #include <Dem_MemMap.h>

      [!IF "($EventClassSupport = 'false')"!]
      #define DEM_START_SEC_CALIB_UNSPECIFIED
      #include <Dem_MemMap.h>
      CONST(Dem_IndicatorGrpIdxType, DEM_CONST) Dem_EventIndicatorClassIdx[DEM_NUMBER_OF_EVENTS] =
      {
        [!WS "2"!]DEM_INDICATOR_UNUSED, /* invalid event id entry */
        [!LOOP "node:order(DemConfigSet/*/DemEventParameter/*, 'DemEventId')"!]
          [!CALL "GET_INDICATOR_CLASS_INDEX", "EVENT_ID" = "DemEventId"!]
          [!IF "$IndicatorClassIndex = '-'"!]
            [!WS "2"!]DEM_INDICATOR_UNUSED, /* [!"name(.)"!] */
          [!ELSE!]
            [!WS "2"!][!"$IndicatorClassIndex"!]U, /* [!"name(.)"!] */
          [!ENDIF!]
        [!ENDLOOP!]
      };
      #define DEM_STOP_SEC_CALIB_UNSPECIFIED
      #include <Dem_MemMap.h>
      [!ENDIF!]
      #endif

    [!ENDIF!]
  [!ELSE!][!//
    #define DEM_START_SEC_CONST_UNSPECIFIED
    #include <Dem_MemMap.h>

    [!IF "$NumIndicator > 0"!]
      /** \brief Warning indicator configuration description table */
      CONST(Dem_IndicatorDescType, DEM_CONST)
        [!WS "2"!]Dem_IndicatorDesc[DEM_NUMBER_OF_INDICATORS] =
      {
        [!VAR "IndicatorLnkIdx" = "0"!]
        [!VAR "MaxIndicatorId" = "$NumIndicator - 1"!]
        [!FOR "IndicatorId" = "0" TO "$MaxIndicatorId"!]
          [!VAR "IndicatorLnkNum" = "count(DemConfigSet/*/DemEventParameter/*/DemEventClass/DemIndicatorAttribute/*[as:ref(DemIndicatorRef)/DemIndicatorID = $IndicatorId])"!]
          [!INDENT "2"!]
            { /* warning indicator: [!"name(DemGeneral/DemIndicator/*[DemIndicatorID = $IndicatorId])"!] (= ID [!"num:i($IndicatorId)"!]) */
              [!WS "2"!]/* number: */ [!"num:i($IndicatorLnkNum)"!]U, /* index: */ [!"num:i($IndicatorLnkIdx)"!]U
            }[!//
            ,
          [!ENDINDENT!]
          [!VAR "IndicatorLnkIdx" = "$IndicatorLnkIdx + $IndicatorLnkNum"!]
        [!ENDFOR!]
      };
    [!ENDIF!]

    [!/* Generate healing and failure lists containing all unique combinations of cycles and thresholds */!][!//
    [!IF "$NumIndicator > 0"!]
      [!VAR "HealList" = "'#'"!][!/* List of all unique healing cycle and threshold pairs */!][!//
      [!VAR "FailList" = "'#'"!][!/* List of all unique failure cycle and threshold pairs */!][!//
      [!LOOP "DemConfigSet/*/DemEventParameter/*"!]
        [!LOOP "./DemEventClass/DemIndicatorAttribute/*"!]
          [!IF "$CommonOperationCycleEnabled = 'false'"!]
            [!VAR "HealingCycle" = "node:name(as:ref(./DemIndicatorHealingCycleRef))"!]
          [!ELSE!]
            [!VAR "HealingCycle" = "node:name(as:ref(../../DemOperationCycleRef))"!]
          [!ENDIF!]
          [!IF "not(contains($HealList, concat('#', $HealingCycle,'*',node:value(./DemIndicatorHealingCycleCounterThreshold), '#')))"!]
            [!VAR "HealList" = "concat($HealList, $HealingCycle,'*',node:value(./DemIndicatorHealingCycleCounterThreshold), '#')"!]
          [!ENDIF!]
          [!IF "./DemIndicatorFailureCycleSource = 'DEM_FAILURE_CYCLE_INDICATOR'"!]
            [!IF "$CommonOperationCycleEnabled = 'false'"!]
              [!VAR "FailCycle" = "node:name(as:ref(./DemIndicatorFailureCycleRef))"!]
            [!ENDIF!]
            [!VAR "FailThrsld" = "node:value(./DemIndicatorFailureCycleCounterThreshold)"!]
          [!ELSE!]
            [!IF "$CommonOperationCycleEnabled = 'false'"!]
              [!/* check if event is combined */!][!//
              [!IF "node:exists(../../../DemEvtCmbCommonParamMaster)"!][!//
                [!VAR "FailCycle" = "node:name(as:ref(../../../../*[(DemDTCClassRef=node:current()/../../../DemDTCClassRef) and (DemEvtCmbCommonParamMaster = 'true')]/DemEventClass/DemEventFailureCycleRef))"!][!//
              [!ELSE!][!//
                [!VAR "FailCycle" = "node:name(as:ref(../../DemEventFailureCycleRef))"!]
              [!ENDIF!][!//
            [!ENDIF!]
            [!/* check if event is combined */!][!//
            [!IF "node:exists(../../../DemEvtCmbCommonParamMaster)"!][!//
              [!VAR "FailThrsld" = "node:value(../../../../*[(DemDTCClassRef=node:current()/../../../DemDTCClassRef) and (DemEvtCmbCommonParamMaster = 'true')]/DemEventClass/DemEventFailureCycleCounterThreshold)"!][!//
            [!ELSE!][!//
              [!VAR "FailThrsld" = "node:value(../../DemEventFailureCycleCounterThreshold)"!]
            [!ENDIF!][!//
          [!ENDIF!]
          [!IF "$CommonOperationCycleEnabled = 'true'"!]
            [!VAR "FailCycle" = "node:name(as:ref(../../DemOperationCycleRef))"!]
          [!ENDIF!]
          [!IF "not(contains($FailList, concat('#', $FailCycle,'*', $FailThrsld, '#')))"!]
            [!VAR "FailList" = "concat($FailList, $FailCycle,'*',$FailThrsld, '#')"!]
          [!ENDIF!]
        [!ENDLOOP!]
      [!ENDLOOP!]
    [!ENDIF!]

    [!IF "$NumIndicator > 0"!]
      #if (DEM_NUM_INDICATOR_LINKS != 0)
      /** \brief Warning indicator-link configuration link table */
      CONST(Dem_IndicatorLinkType, DEM_CONST)
        [!WS "2"!]Dem_IndicatorLink[DEM_NUM_INDICATOR_LINKS] =
      {
        [!INDENT "2"!]
          [!VAR "MaxIndicatorId" = "$NumIndicator - 1"!]
          [!FOR "IndicatorId" = "0" TO "$MaxIndicatorId"!]
            [!IF "count(DemConfigSet/*/DemEventParameter/*/DemEventClass/DemIndicatorAttribute/*[as:ref(DemIndicatorRef)/DemIndicatorID = $IndicatorId]) = 0"!]
              [!WS "2"!]/* no links */
            [!ENDIF!]
            [!LOOP "node:order(DemConfigSet/*/DemEventParameter/*, 'DemEventId')"!]
              [!LOOP "./DemEventClass/DemIndicatorAttribute/*[as:ref(./DemIndicatorRef)/DemIndicatorID = $IndicatorId != 0]"!]
                {
                  [!INDENT "4"!]
                  DemConf_DemEventParameter_[!"name(../../../.)"!],
                  [!VAR "IndicatorBehaviour" = "../../../DemEventClass/DemIndicatorAttribute/*[as:ref(DemIndicatorRef)/DemIndicatorID = $IndicatorId]/DemIndicatorBehaviour"!]
                  [!IF "$IndicatorBehaviour = 'DEM_INDICATOR_CONTINUOUS'"!]
                    DEM_INDICATOR_CONTINUOUS,
                  [!ELSEIF "$IndicatorBehaviour = 'DEM_INDICATOR_BLINKING'"!][!//
                    DEM_INDICATOR_BLINKING,
                  [!ELSEIF "$IndicatorBehaviour = 'DEM_INDICATOR_BLINK_CONT'"!][!//
                    DEM_INDICATOR_BLINK_CONT,
                  [!ELSEIF "$IndicatorBehaviour = 'DEM_INDICATOR_SLOW_FLASH'"!][!//
                    DEM_INDICATOR_SLOW_FLASH,
                  [!ELSE!][!//
                    DEM_INDICATOR_FAST_FLASH,
                  [!ENDIF!]
                  [!IF "$CommonOperationCycleEnabled = 'false'"!]
                    [!VAR "HealingCycle" = "node:name(as:ref(./DemIndicatorHealingCycleRef))"!]
                  [!ELSE!]
                    [!VAR "HealingCycle" = "node:name(as:ref(../../DemOperationCycleRef))"!]
                  [!ENDIF!]
                  [!VAR "HealingThreshold" = "node:value(./DemIndicatorHealingCycleCounterThreshold)"!]
                  [!VAR "HealCtr" = "0"!]
                  [!LOOP "text:split($HealList, '#')"!]
                     [!IF "text:contains(., concat($HealingCycle,'*', $HealingThreshold))"!]
                       [!"num:integer($HealCtr)"!]U, /* Index of Dem_HealingCycleCounterInfo[] */
                       [!BREAK!]
                     [!ENDIF!]
                     [!VAR "HealCtr" = "$HealCtr + 1"!]
                  [!ENDLOOP!]
                  [!IF "./DemIndicatorFailureCycleSource = 'DEM_FAILURE_CYCLE_INDICATOR'"!]
                    [!IF "$CommonOperationCycleEnabled = 'false'"!]
                      [!VAR "FailureCycle" = "node:name(as:ref(./DemIndicatorFailureCycleRef))"!]
                    [!ENDIF!]
                    [!VAR "FailureThreshold" = "node:value(./DemIndicatorFailureCycleCounterThreshold)"!]
                  [!ELSE!]
                    [!IF "$CommonOperationCycleEnabled = 'false'"!]
                      [!/* check if event is combined */!][!//
                      [!IF "node:exists(../../../DemEvtCmbCommonParamMaster)"!][!//
                        [!VAR "FailureCycle" = "node:name(as:ref(../../../../*[(DemDTCClassRef=node:current()/../../../DemDTCClassRef) and (DemEvtCmbCommonParamMaster = 'true')]/DemEventClass/DemEventFailureCycleRef))"!][!//
                      [!ELSE!][!//
                        [!VAR "FailureCycle" = "node:name(as:ref(../../DemEventFailureCycleRef))"!]
                      [!ENDIF!][!//
                    [!ENDIF!]
                    [!/* check if event is combined */!][!//
                    [!IF "node:exists(../../../DemEvtCmbCommonParamMaster)"!][!//
                      [!VAR "FailureThreshold" = "node:value(../../../../*[(DemDTCClassRef=node:current()/../../../DemDTCClassRef) and (DemEvtCmbCommonParamMaster = 'true')]/DemEventClass/DemEventFailureCycleCounterThreshold)"!][!//
                    [!ELSE!][!//
                      [!VAR "FailureThreshold" = "node:value(../../DemEventFailureCycleCounterThreshold)"!]
                    [!ENDIF!][!//
                  [!ENDIF!]
                  [!IF "$CommonOperationCycleEnabled = 'true'"!]
                    [!VAR "FailureCycle" = "node:name(as:ref(../../DemOperationCycleRef))"!]
                  [!ENDIF!]
                  [!VAR "FailCtr" = "0"!]
                  [!LOOP "text:split($FailList, '#')"!]
                     [!IF "text:contains(., concat($FailureCycle,'*', $FailureThreshold))"!]
                       [!"num:integer($FailCtr)"!]U, /* Index of Dem_FailureCycleCounterInfo[] */
                       [!BREAK!]
                     [!ENDIF!]
                     [!VAR "FailCtr" = "$FailCtr + 1"!]
                  [!ENDLOOP!]
                  [!ENDINDENT!]
                }[!//
              ,
              [!ENDLOOP!]
            [!ENDLOOP!]
          [!ENDFOR!]
        [!ENDINDENT!]
      };
      #endif
    [!ENDIF!]

    #define DEM_STOP_SEC_CONST_UNSPECIFIED
    #include <Dem_MemMap.h>

    #define DEM_START_SEC_CONST_8
    #include <Dem_MemMap.h>

    [!IF "$NumIndicator > 0"!]
      #if (DEM_NUM_HEALINGCYC_COUNTER_INFO_ELEMENTS != 0U)
      /** \brief Warning indicator configuration healing cycle-counter information table */
      CONST(Dem_CounterInfoType, DEM_CONST)
        [!WS "2"!]Dem_HealingCycleCounterInfo[DEM_NUM_HEALINGCYC_COUNTER_INFO_ELEMENTS] =
      {
        [!/* list of all unique healing cycle and threshold pair */!][!//
        [!LOOP "text:split($HealList, '#')"!]
          [!VAR "HealingCycle" = "text:split(., '*')[position()=1]"!]
          [!VAR "HealingThreshold" = "text:split(., '*')[position()=2]"!]
          [!INDENT "2"!]
          {
            [!WS "2"!][!"$HealingCycle"!],
            [!WS "2"!][!"$HealingThreshold"!]U /* Healing threshold */
          },
          [!ENDINDENT!]
        [!ENDLOOP!]
      };
      #endif
    [!ENDIF!]

    [!IF "$NumIndicator > 0"!]
      #if (DEM_NUM_FAILURECYC_COUNTER_INFO_ELEMENTS != 0U)
      /** \brief Warning indicator configuration failure cycle-counter information table */
      CONST(Dem_CounterInfoType, DEM_CONST)
        [!WS "2"!]Dem_FailureCycleCounterInfo[DEM_NUM_FAILURECYC_COUNTER_INFO_ELEMENTS] =
      {
        [!/* list of all unique failure cycle and threshold pair */!][!//
        [!LOOP "text:split($FailList, '#')"!]
          [!VAR "FailCycle" = "text:split(., '*')[position()=1]"!]
          [!VAR "FailThreshold" = "text:split(., '*')[position()=2]"!]
          [!INDENT "2"!]
          {
            [!WS "2"!][!"$FailCycle"!],
            [!WS "2"!][!"$FailThreshold"!]U /* Failure threshold */
          },
          [!ENDINDENT!]
        [!ENDLOOP!]
      };
      #endif
    [!ENDIF!]

    #define DEM_STOP_SEC_CONST_8
    #include <Dem_MemMap.h>
  [!ENDIF!]

[!ENDINDENT!]

/*------------------[Debouncing configuration]------------------------------*/

[!INDENT "0"!]
  /** \brief Counter based debouncing configuration description table */
  [!IF "(count(DemConfigSet/*/DemEventParameter/*[DemEventClass/DemDebounceAlgorithmClass = 'DemDebounceCounterBased']) > 0) or (node:exists(DemGeneral/DemMaxNumberDebCounterClasses) and (DemGeneral/DemMaxNumberDebCounterClasses > 0))"!]
  #define DEM_START_SEC_CALIB_16
  #include <Dem_MemMap.h>

    [!VAR "UnconfirmedThresholdEnabled" = "'false'"!]
    [!IF "(DemGeneral/DemFreezeFrameCapture = 'DEM_TRIGGER_ON_FDC_THRESHOLD') or (DemGeneral/DemExtendedDataCapture = 'DEM_TRIGGER_ON_FDC_THRESHOLD')"!]
      [!VAR "UnconfirmedThresholdEnabled" = "'true'"!]
    [!ENDIF!]
    [!CALL "DEBOUNCE_COUNTER_CLASS_CALC"!]
    CONST(Dem_DebounceCounterClassType, DEM_CONST)
      [!WS "2"!]Dem_DebounceCounterClass[DEM_NUM_DEBOUNCE_COUNTER_CLASSES] =
    {
      [!INDENT "2"!]
      [!VAR "DebounceClassIdx" = "0"!]
      [!LOOP "text:split($DebounceClasses, '#')"!]
        [!VAR "DebounceCounterDecrementStepSize" = "text:split(., ':')[position() = 1]"!]
        [!VAR "DebounceCounterIncrementStepSize" = "text:split(., ':')[position() = 2]"!]
        [!VAR "DebounceCounterPassedThreshold" = "text:split(., ':')[position() = 3]"!]
        [!VAR "DebounceCounterFailedThreshold" = "text:split(., ':')[position() = 4]"!]
        [!VAR "DebounceCounterUnconfirmedThreshold" = "text:split(., ':')[position() = 5]"!]
        [!VAR "DebounceCounterJumpDownValue" = "text:split(., ':')[position() = 6]"!]
        [!VAR "DebounceCounterJumpUpValue" = "text:split(., ':')[position() = 7]"!]
        [!VAR "DebounceCounterBehavior" = "text:split(., ':')[position() = 8]"!]
        [!VAR "DebounceCounterJumpDown" = "text:split(., ':')[position() = 9]"!]
        [!VAR "DebounceCounterJumpUp" = "text:split(., ':')[position() = 10]"!]
        [!VAR "DebounceCounterStorage" = "text:split(., ':')[position() = 11]"!]
        { /* [!"num:i($DebounceClassIdx)"!] */
          [!INDENT "4"!]
          [!"$DebounceCounterDecrementStepSize"!]U, /* DemDebounceCounterDecrementStepSize */
          [!"$DebounceCounterIncrementStepSize"!]U, /* DemDebounceCounterIncrementStepSize */
          [!"$DebounceCounterPassedThreshold"!], /* DemDebounceCounterPassedThreshold */
          [!"$DebounceCounterFailedThreshold"!], /* DemDebounceCounterFailedThreshold */
          [!IF "$UnconfirmedThresholdEnabled = 'true'"!]
            [!"$DebounceCounterUnconfirmedThreshold"!], /* DemDebounceCounterUnconfirmedThreshold */
          [!ENDIF!]
          [!"$DebounceCounterJumpDownValue"!], /* DemDebounceCounterJumpDownValue */
          [!"$DebounceCounterJumpUpValue"!], /* DemDebounceCounterJumpUpValue */
          [!IF "$DebounceCounterBehavior = 'DEM_DEBOUNCE_FREEZE'"!]
            DEM_DEBOUNCE_FREEZE, /* DemDebounceBehavior */
          [!ELSE!]
            DEM_DEBOUNCE_RESET, /* DemDebounceBehavior */
          [!ENDIF!]
          [!IF "$DebounceCounterJumpDown = 'true'"!]
            TRUE, /* DemDebounceCounterJumpDown */
          [!ELSE!]
            FALSE, /* DemDebounceCounterJumpDown */
          [!ENDIF!]
          [!IF "$DebounceCounterJumpUp = 'true'"!]
            TRUE, /* DemDebounceCounterJumpUp */
          [!ELSE!]
            FALSE, /* DemDebounceCounterJumpUp */
          [!ENDIF!]
          [!ENDINDENT!]
        },
        [!VAR "DebounceClassIdx" = "$DebounceClassIdx + 1"!]
      [!ENDLOOP!]
      [!VAR "DebounceCtr" = "num:integer(count(text:split($DebounceClasses, '#')))"!]
      [!IF "node:exists(DemGeneral/DemMaxNumberDebCounterClasses)"!]
        [!VAR "MaxNumberDebCounter" = "num:integer(DemGeneral/DemMaxNumberDebCounterClasses)"!]
        [!IF "$DebounceCtr < $MaxNumberDebCounter"!]
          /* Unused debounce classes */
          [!FOR "DebounceCtrIdx" = "$DebounceCtr" TO "$MaxNumberDebCounter - 1"!]
            {
              [!INDENT "4"!]
              1U, /* DemDebounceCounterDecrementStepSize */
              1U, /* DemDebounceCounterIncrementStepSize */
              -128, /* DemDebounceCounterPassedThreshold */
              127, /* DemDebounceCounterFailedThreshold */
              [!IF "$UnconfirmedThresholdEnabled = 'true'"!]
                127, /* DemDebounceCounterUnconfirmedThreshold */
              [!ENDIF!]
              0, /* DemDebounceCounterJumpDownValue */
              0, /* DemDebounceCounterJumpUpValue */
              DEM_DEBOUNCE_FREEZE, /* DemDebounceBehavior */
              FALSE, /* DemDebounceCounterJumpDown */
              FALSE, /* DemDebounceCounterJumpUp */
              [!ENDINDENT!]
            },
          [!ENDFOR!]
        [!ENDIF!]
      [!ENDIF!]
      [!ENDINDENT!]
    };

    /** \brief Counter based debouncing class index table */
    CONST(Dem_DebounceCounterClassIdxType, DEM_CONST)
      [!WS "2"!]Dem_DebounceCounterClassIdx[DEM_NUM_DEBOUNCE_COUNTER] =
    {
      [!CALL "DEBOUNCE_COUNTER_CLASS_CALC"!]
      [!LOOP "node:order(DemConfigSet/*/DemEventParameter/*[DemEventClass/DemDebounceAlgorithmClass = 'DemDebounceCounterBased'], 'DemEventId')"!]
        [!VAR "DebounceStorage" = "node:value(./DemEventClass/DemDebounceAlgorithmClass/DemDebounceCounterStorage)"!]
        [!CALL "DEBOUNCE_COUNTER_GET_CLASS_INDEX", "EventId" = "DemEventId"!]
        [!IF "$DebounceStorage = 'true'"!]
          [!WS "2"!][!"$DebounceCounterClassIndex"!] | DEM_DEBOUNCE_COUNTER_NVSTORAGE_MASK, /* [!"DemEventId"!] */
        [!ELSE!][!//
          [!WS "2"!][!"$DebounceCounterClassIndex"!], /* [!"DemEventId"!] */
        [!ENDIF!]
      [!ENDLOOP!]
    };

  #define DEM_STOP_SEC_CALIB_16
  #include <Dem_MemMap.h>
  [!ELSE!]
    /* none */
  [!ENDIF!]

  /** \brief Time based debouncing configuration description table */
   /* !LINKSTO Dem.Calibration.DemTimeBasedDebounceClassConcept,1 */
  [!IF "(count(DemConfigSet/*/DemEventParameter/*[DemEventClass/DemDebounceAlgorithmClass = 'DemDebounceTimeBase']) > 0) or (node:exists(DemGeneral/DemMaxNumberDebTimeClasses) and (DemGeneral/DemMaxNumberDebTimeClasses > 0))"!][!//
  #define DEM_START_SEC_CALIB_32
  #include <Dem_MemMap.h>

    CONST(Dem_DebounceTimeClassType, DEM_CONST)
      [!WS "2"!]Dem_DebounceTimeClass[DEM_NUM_DEBOUNCE_TIME_CLASSES] =
    {
      [!CALL "DEBOUNCE_TIME_CLASS_CALC"!][!//
      [!INDENT "2"!]
      [!VAR "TimeDebounceClassIdx" = "0"!]
      [!LOOP "text:split($TimeDebounceClasses, '#')"!]
      [!VAR "DemDebounceTimeFailedThreshold" = "text:split(., ':')[position() = 1]"!]
      [!VAR "DemDebounceTimePassedThreshold" = "text:split(., ':')[position() = 2]"!]
      [!VAR "DebounceTimeBehavior" = "text:split(., ':')[position() = 3]"!]
      { /* [!"num:i($TimeDebounceClassIdx)"!] */
      [!/* TimeFailedThreshold and TimePassedThreshold are stored in main
              * function "ticks", not in ms! */!][!//
        [!INDENT "4"!]
        [!"num:i(ceiling(($DemDebounceTimeFailedThreshold * 1000) div ($DemTaskTime* 1000)))"!]U, /* DemDebounceTimeFailedThreshold */
        [!"num:i(ceiling(($DemDebounceTimePassedThreshold * 1000) div ($DemTaskTime * 1000)))"!]U, /* DemDebounceTimePassedThreshold */
        [!IF "$DebounceTimeBehavior = 'DEM_DEBOUNCE_FREEZE'"!]
          DEM_DEBOUNCE_FREEZE, /* DemDebounceBehavior */
        [!ELSEIF "$DebounceTimeBehavior = 'DEM_DEBOUNCE_RESET'"!]
          DEM_DEBOUNCE_RESET, /* DemDebounceBehavior */
        [!ELSE!][!//
          DEM_DEBOUNCE_CONTINUE, /* DemDebounceBehavior */
        [!ENDIF!]
        [!ENDINDENT!]
      },
        [!VAR "TimeDebounceClassIdx" = "$TimeDebounceClassIdx + 1"!]
      [!ENDLOOP!]
      [!VAR "TimeDebounceCtr" = "num:integer(count(text:split($TimeDebounceClasses, '#')))"!]
      [!IF "node:exists(DemGeneral/DemMaxNumberDebTimeClasses)"!]
        [!VAR "MaxNumberTimeDebCounter" = "num:integer(DemGeneral/DemMaxNumberDebTimeClasses)"!]
        [!IF "$TimeDebounceCtr < $MaxNumberTimeDebCounter"!]
          /* Unused debounce classes */
          [!FOR "DebounceTimeIdx" = "$TimeDebounceCtr" TO "$MaxNumberTimeDebCounter - 1"!]
            {
               [!INDENT "4"!]
                /* [!"num:i($DebounceTimeIdx)"!] */
               1U, /* DemDebounceTimeFailedThreshold */
               1U, /* DemDebounceTimePassedThreshold */
               DEM_DEBOUNCE_CONTINUE, /* DemDebounceBehavior */
               [!ENDINDENT!]
             },
          [!ENDFOR!]
        [!ENDIF!]
      [!ENDIF!]
      [!ENDINDENT!]
       };

    /** \brief Time based debouncing class index table */
    CONST(Dem_DebounceTimeClassIdxType, DEM_CONST)
      [!WS "2"!]Dem_DebounceTimeClassIdx[DEM_NUM_DEBOUNCE_TIME] =
    {
      [!CALL "DEBOUNCE_TIME_CLASS_CALC"!][!//
      [!LOOP "node:order(DemConfigSet/*/DemEventParameter/*[DemEventClass/DemDebounceAlgorithmClass = 'DemDebounceTimeBase'], 'DemEventId')"!][!//
      [!CALL "DEBOUNCE_TIME_GET_CLASS_INDEX", "EventId" = "DemEventId"!][!//
        [!WS "2"!][!"$DebounceTimeClassIndex"!], /* EventName: [!"name(.)"!] */
      [!ENDLOOP!][!//
    };

     #define DEM_STOP_SEC_CALIB_32
     #include <Dem_MemMap.h>
     [!ELSE!]
       /* none */
     [!ENDIF!]

  #define DEM_START_SEC_CONST_16
  #include <Dem_MemMap.h>

  /** \brief Time based debouncing configuration description table */
  [!IF "count(DemConfigSet/*/DemEventParameter/*[DemEventClass/DemDebounceAlgorithmClass = 'DemDebounceTimeBase']) > 0"!]
    CONST(Dem_EventIdType, DEM_CONST)
      [!WS "2"!]Dem_DebounceTimeCfg[DEM_NUM_DEBOUNCE_TIME] =
    {
      [!LOOP "node:order(DemConfigSet/*/DemEventParameter/*[DemEventClass/DemDebounceAlgorithmClass = 'DemDebounceTimeBase'], 'DemEventId')"!]
        [!SELECT "DemEventClass"!]
          [!WS "2"!] /* event: [!"name(..)"!] */
           [!WS "2"!] DemConf_DemEventParameter_[!"name(..)"!],
        [!ENDSELECT!]
      [!ENDLOOP!]
    };
  [!ELSE!]
    /* none */
  [!ENDIF!]

  /** \brief Frequency based debouncing configuration description table */
  [!IF "count(DemConfigSet/*/DemEventParameter/*[DemEventClass/DemDebounceAlgorithmClass = 'DemDebounceFrequencyBased']) > 0"!]
    CONST(Dem_DebounceFrequencyCfgType, DEM_CONST)
      [!WS "2"!]Dem_DebounceFrequencyCfg[DEM_NUM_DEBOUNCE_FREQUENCY] =
    {
      [!LOOP "node:order(DemConfigSet/*[1]/DemEventParameter/*[DemEventClass/DemDebounceAlgorithmClass = 'DemDebounceFrequencyBased'], 'DemEventId')"!]
        [!SELECT "DemEventClass"!]
          [!WS "2"!]{ /* event: [!"name(..)"!] */
            [!/* DurationOfTimeWindow is stored in main function "ticks", not in ms! */!][!//
            [!WS "4"!][!//
            [!"num:i(ceiling((DemDebounceAlgorithmClass/DemDurationOfTimeWindow * 1000) div (as:modconf('Dem')[1]/DemGeneral/DemTaskTime * 1000)))"!]U,
            [!WS "4"!][!//
            [!"num:i(DemDebounceAlgorithmClass/DemThresholdForEventTestedFailed - 1)"!]U,
            [!WS "4"!][!//
            [!"num:i(DemDebounceAlgorithmClass/DemThresholdForEventTestedPassed - 1)"!]U,
            [!WS "4"!][!//
            DemConf_DemEventParameter_[!"name(..)"!],
            [!WS "4"!][!//
            [!IF "DemDebounceAlgorithmClass/DemDebounceBehavior = 'DEM_DEBOUNCE_FREEZE'"!]
              DEM_DEBOUNCE_FREEZE, /* DemDebounceBehavior */
            [!ELSEIF "DemDebounceAlgorithmClass/DemDebounceBehavior = 'DEM_DEBOUNCE_RESET'"!]
              DEM_DEBOUNCE_RESET, /* DemDebounceBehavior */
            [!ELSE!][!//
              DEM_DEBOUNCE_CONTINUE, /* DemDebounceBehavior */
            [!ENDIF!]
          [!WS "2"!]},
        [!ENDSELECT!]
      [!ENDLOOP!]
    };
  [!ELSE!]
    /* none */
  [!ENDIF!]

  #define DEM_STOP_SEC_CONST_16
  #include <Dem_MemMap.h>
[!ENDINDENT!]

/*------------------[Fault confirmation configuration]----------------------*/

[!INDENT "0"!]
  [!IF "count(DemConfigSet/*/DemEventParameter/*[node:exists(DemEventClass/DemEventFailureCycleRef)]) > 0"!]
    [!IF "($CommonOperationCycleEnabled = 'false') or ($EventClassSupport = 'false')"!]
      #define DEM_START_SEC_CONST_8
      #include <Dem_MemMap.h>

      /** \brief Failure cycle and failure counter threshold configuration */
      CONST(Dem_EventFailureCycleCfgType, DEM_CONST)
        [!WS "2"!]Dem_EventFailureCycleCfg[DEM_NUM_FAILURECYCLES] =
      {
        [!LOOP "text:split($FailureCycleCfgList, '#')"!]
          [!VAR "EventFailureCycle" = "text:split(., '*')[position() = 1]"!]
          [!VAR "EventFailureThreshold" = "text:split(., '*')[position() = 2]"!]
          [!WS "2"!]{
            [!IF "$CommonOperationCycleEnabled = 'false'"!]
              [!WS "4"!][!"$EventFailureCycle"!],
            [!ENDIF!]
            [!IF "$EventClassSupport = 'false'"!]
              [!WS "4"!][!"num:i($EventFailureThreshold)"!]U
            [!ENDIF!]
          [!WS "2"!]},
        [!ENDLOOP!]
      };

      #define DEM_STOP_SEC_CONST_8
      #include <Dem_MemMap.h>
    [!ELSE!]
      /* stored in event class */
    [!ENDIF!]
  [!ELSE!]
    /* none */
  [!ENDIF!]
[!ENDINDENT!]

[!INDENT "0"!]
  [!VAR "NumberEventsWithFaultConfirmation" = "count((DemConfigSet/*/DemEventParameter/*[node:exists(DemEventClass/DemEventFailureCycleCounterThreshold)]))"!]
  [!IF "(DemGeneral/DemResetConfirmedBitOnOverflow = 'false') and ($NumberEventsWithFaultConfirmation > 0)"!]
    #define DEM_START_SEC_CONST_16
    #include <Dem_MemMap.h>

    /** \brief Indexes for status counter for events array in ::Dem_NvData */
    CONST(Dem_StatusCounterIndexType, DEM_CONST)
      [!WS "2"!]Dem_StatusCounterIndex[DEM_STATUS_COUNTER_INDEX_SIZE] =
    {
      [!/* ConstGroupSize must be equal to DEM_STATUS_COUNTER_GROUP_SIZE */!][!//
      [!VAR "ConstGroupSize" = "4"!]
      [!VAR "StatusCounterIndex" = "0"!]
      [!WS "2"!]0U, /* base index of first event group always points to first data element */
      [!LOOP "node:order(DemConfigSet/*/DemEventParameter/*, 'DemEventId')"!]
        [!IF "(node:value(DemEventId) mod $ConstGroupSize) = 0"!]
          [!/* print base index of group calculated over all previous groups */!][!//
          [!WS "2"!][!"num:i($StatusCounterIndex)"!]U, /* base index of event group [[!"num:i(DemEventId)"!]..[!"num:i(DemEventId + ($ConstGroupSize - 1))"!]] */
        [!ENDIF!]
        [!IF "not(node:exists(DemEvtCmbCommonParamMaster)) and node:exists(DemEventClass/DemEventFailureCycleCounterThreshold)"!]
          [!VAR "StatusCounterIndex" = "$StatusCounterIndex + 1"!]
        [!ENDIF!]
      [!ENDLOOP!]
    };

    #define DEM_STOP_SEC_CONST_16
    #include <Dem_MemMap.h>
  [!ENDIF!]
[!ENDINDENT!]

/*------------------[ Event Availability ]------------------------*/

#define DEM_START_SEC_CALIB_8
#include <Dem_MemMap.h>

[!INDENT "0"!]
  [!IF "($CalibrationSupport = 'true') or ($AvailabilitySupport = 'true')"!]
    [!VAR "BitIndex" = "1"!][!//
    [!VAR "ByteVal" = "0"!][!//
      /** \brief Calibratable bit-field array that holds availability status (enabled/disabled)
      [!WS!]**        of all events
      [!WS!]** Note: If an event is available, the corresponding bit is set to one. */
      CONST(uint8, DEM_CONST) Dem_EventEnabled[DEM_EVENT_ENABLED_ARRAY_SIZE] =
    {
      [!INDENT "2"!]
        [!LOOP "node:order(DemConfigSet/*/DemEventParameter/*, 'DemEventId')"!]
          [!IF "DemEventClass/DemEventAvailable = 'true'"!]
            [!VAR "ByteVal" = "bit:or($ByteVal,bit:shl(1,$BitIndex))"!]
          [!ENDIF!]
          [!VAR "BitIndex" = "$BitIndex + 1"!]
          [!IF "($BitIndex mod 8) = 0"!]
            [!"num:inttohex($ByteVal, 2)"!]U,
            [!VAR "ByteVal" = "0"!]
            [!VAR "BitIndex" = "0"!]
          [!ENDIF!]
        [!ENDLOOP!]
        [!IF "($BitIndex mod 8) != 0"!]
          [!"num:inttohex($ByteVal, 2)"!]U
        [!ENDIF!]
      [!ENDINDENT!]
    };
  [!ENDIF!]
[!ENDINDENT!]

#define DEM_STOP_SEC_CALIB_8
#include <Dem_MemMap.h>

/*------------------[RTE / C-callback notifications]------------------------*/

#define DEM_START_SEC_CONST_8
#include <Dem_MemMap.h>

[!INDENT "0"!]
  /** \brief Event callback property table */
  CONST(Dem_CbPropertyType, DEM_CONST) Dem_CbProperty[DEM_NUMBER_OF_EVENTS] =
  {
    [!WS "2"!]0U,
    [!WS "2"!]/* invalid event id entry */
    [!LOOP "node:order(DemConfigSet/*/DemEventParameter/*, 'DemEventId')"!]
      [!WS "2"!]/* event: [!"name(.)"!] */
      [!IF "node:exists(DemCallbackInitMForE)"!]
        DEM_GEN_CB_INIT_MONITOR |
        [!//
      [!ELSE!]
       [!WS "2"!]/* no init monitor callback */
     [!//
      [!ENDIF!]
      [!IF "count(DemCallbackEventStatusChanged/*) > 0"!]
        [!IF "count(DemCallbackEventStatusChanged/*[not(node:exists(DemCallbackEventStatusChangedFnc))]) > 0"!]
          DEM_RTE_CB_TRIG_ON_EVST | [!//
        [!ENDIF!]
        [!IF "node:exists(DemCallbackEventStatusChanged/*/DemCallbackEventStatusChangedFnc)"!]
          DEM_C_CB_TRIG_ON_EVST | [!//
        [!ENDIF!]
      [!ELSE!]
         [!WS "2"!]/* no trigger on event status callback(s) */
        [!//
      [!ENDIF!]
      [!IF "node:exists(DemCallbackEventDataChanged)"!]
        [!IF "not(node:exists(DemCallbackEventDataChanged/DemCallbackEventDataChangedFnc))"!]
          DEM_RTE_CB_TRIG_ON_EVDAT | [!//
        [!ELSE!]
          DEM_C_CB_TRIG_ON_EVDAT | [!//
        [!ENDIF!]
      [!ELSE!]
         [!WS "2"!]/* no trigger on event data changed callback */
        [!//
      [!ENDIF!]
      [!IF "(not(node:exists(DemEvtCmbCommonParamMaster)) or (DemEvtCmbCommonParamMaster = 'true')) and node:exists(DemCallbackClearEventAllowed)"!]
        [!IF "not(node:exists(DemCallbackClearEventAllowed/DemCallbackClearEventAllowedFnc))"!]
          DEM_RTE_CB_CLEAR_EVENT_ALLOWED | [!//
        [!ELSE!]
          DEM_C_CB_CLEAR_EVENT_ALLOWED | [!//
        [!ENDIF!]
      [!ELSE!]
         [!WS "2"!]/* no clearEventAllowed callback */
        [!//
      [!ENDIF!]
      [!IF "DemEventClass/DemDebounceAlgorithmClass = 'DemDebounceMonitorInternal'"!]
        [!IF "node:exists(DemEventClass/DemDebounceAlgorithmClass/DemCallbackGetFDC)"!]
          DEM_GEN_CB_FAULT_DETECTION_CTR | [!//
        [!ELSE!]
          [!WS "2"!]/* no fault detection counter callback */
          [!//
        [!ENDIF!]
      [!ELSE!]
         [!WS "2"!]/* DEM-internal fault detection counter */
        [!//
      [!ENDIF!]
       [!WS "2"!]DEM_ZERO_END,
    [!ENDLOOP!]
  };
  [!//
  /*---------------[Operation cycle automatic end enabled masks]-------------*/
  [!INDENT "0"!]
   [!/* vars for calculation of AutomaticEndEnabled bitmask */!]
    [!VAR "isAutoEndEnabled" = "0"!]
    [!VAR "maskCnt" = "1"!]
    [!VAR "autEndEnabledMask" = "0"!]
    [!VAR "autEndEnabledOpCycFrom" = "0"!]
    [!VAR "opCycCount" = "0"!]
    [!IF "count(DemGeneral/DemOperationCycle/*[DemOperationCycleAutomaticEnd = 'true']) > 0"!]
      [!VAR "isAutoEndEnabled" = "1"!]
    [!ENDIF!]
    [!IF "$isAutoEndEnabled = 1"!]
      /* Array contains masks for automatic ending of operation cycles. */
      /* One mask groups 8 operation cycles to keep the numerical value of */
      /* the masks small. Every mask is a bitfield, containing the auto end */
      /* enabled information of 8 consecutive operation cycles. */
      CONST(uint8, DEM_CONST)
      [!WS "2"!]Dem_OpCycAutoEndEnabledMasks[DEM_OPCYC_NUM_OF_AUTO_END_ENABLED_MASKS] =
      {
        [!LOOP "DemGeneral/DemOperationCycle/*"!]
          [!VAR "opCycCount" = "$opCycCount + 1"!]
          [!IF "DemOperationCycleAutomaticEnd = 'true'"!]
            [!VAR "autEndEnabledMask" = "$autEndEnabledMask + $maskCnt"!]
          [!ENDIF!]
          [!VAR "maskCnt" = "$maskCnt * 2"!]
          [!IF "(@index + 1) mod 8 = 0 or node:islast()"!]
            [!WS "2"!]/* Automatic end enabled mask for operation cycles [!"num:integer($autEndEnabledOpCycFrom)"!] .. [!"num:integer($opCycCount - 1)"!] */
            [!WS "2"!][!"num:inttohex($autEndEnabledMask)"!]U,
            [!VAR "autEndEnabledMask" = "0"!]
            [!VAR "maskCnt" = "1"!]
            [!VAR "autEndEnabledOpCycFrom" = "$opCycCount"!]
          [!ENDIF!]
        [!ENDLOOP!]
      };
    [!ENDIF!]
  [!ENDINDENT!]
  [!//
  #define DEM_STOP_SEC_CONST_8
  #include <Dem_MemMap.h>

  /*---[INIT_MONITOR]---*/

  #if (DEM_CB_TABLE_INIT_MONITOR_SIZE != 0U)
  #define DEM_START_SEC_CONST_UNSPECIFIED
  #include <Dem_MemMap.h>

  /** \brief Generic callback function pointer table for InitMonitor */
  CONST(Dem_CbFuncPtrInitMonitorType, DEM_CONST)
    [!WS "2"!]Dem_CbFuncPtrInitMonitor[DEM_CB_TABLE_INIT_MONITOR_SIZE] =
  {
    [!LOOP "DemConfigSet/*/DemEventParameter/*/DemCallbackInitMForE"!]
      [!IF "node:exists(DemCallbackInitMForEFnc)"!]
        [!VAR "CallbackName" = "DemCallbackInitMForEFnc"!]
        [!WS "2"!]/* event: [!"name(../.)"!] */
      [!ELSE!]
        [!VAR "CallbackName" = "concat('Rte_Call_CBInitEvt_', name(../.), '_InitMonitorForEvent')"!]
      [!ENDIF!]
      [!WS "2"!]&[!"$CallbackName"!],
    [!ENDLOOP!]
  };

  #define DEM_STOP_SEC_CONST_UNSPECIFIED
  #include <Dem_MemMap.h>
  #endif

  #if (DEM_LOOKUP_TABLE_INIT_MONITOR_SIZE != 0U)
  #define DEM_START_SEC_CONST_16
  #include <Dem_MemMap.h>

  /** \brief Generic lookup table for InitMonitor */
  CONST(Dem_EventIdType, DEM_CONST)
    [!WS "2"!]Dem_CbLookupTableInitMonitor[DEM_LOOKUP_TABLE_INIT_MONITOR_SIZE] =
  {
    [!LOOP "DemConfigSet/*/DemEventParameter/*/DemCallbackInitMForE"!]
      [!WS "2"!]DemConf_DemEventParameter_[!"name(../.)"!], [!//
      [!IF "node:exists(DemCallbackInitMForEFnc)"!]
        /* -> [!"DemCallbackInitMForEFnc"!]() */
      [!ELSE!]
        /* -> Rte_Call */
      [!ENDIF!]
    [!ENDLOOP!]
  };

  #define DEM_STOP_SEC_CONST_16
  #include <Dem_MemMap.h>
  #endif

  /*---[TRIGGER_ON_EVENT_STATUS]---*/

  #if (DEM_C_CALLBACK_TABLE_TRIG_ON_EVST_SIZE != 0U)
  #define DEM_START_SEC_CONST_UNSPECIFIED
  #include <Dem_MemMap.h>

  /** \brief C-Callback function pointer table for TriggerOnEventStatus */
  CONST(Dem_CbFuncPtrTrigOnEvStType, DEM_CONST)
    [!WS "2"!]Dem_CbFuncPtrTrigOnEvSt[DEM_C_CALLBACK_TABLE_TRIG_ON_EVST_SIZE] =
  {
    [!VAR "CbNameList" = "'#'"!]  [!/* list of all unique callbacks */!][!//
    [!VAR "CCbCtr" = "0"!]        [!/* number of unique callbacks */!][!//
    [!LOOP "DemConfigSet/*/DemEventParameter/*/DemCallbackEventStatusChanged/*[node:exists(DemCallbackEventStatusChangedFnc)]"!]
      [!IF "not(contains($CbNameList, concat('#', DemCallbackEventStatusChangedFnc, '#')))"!]
        [!VAR "CallbackName" = "DemCallbackEventStatusChangedFnc"!]
        [!WS "2"!]&[!"$CallbackName"!],
        [!VAR "CbNameList" = "concat($CbNameList, $CallbackName, '#')"!]
        [!VAR "CCbCtr" = "$CCbCtr + 1"!]
      [!ENDIF!]
    [!ENDLOOP!]
  };

  #define DEM_STOP_SEC_CONST_UNSPECIFIED
  #include <Dem_MemMap.h>
  #endif

  #if (DEM_C_LOOKUP_TABLE_TRIG_ON_EVST_SIZE != 0U)
  #define DEM_START_SEC_CONST_16
  #include <Dem_MemMap.h>

  /** \brief C-Lookup table for TriggerOnEventStatus */
  CONST(Dem_CbLookupTableType, DEM_CONST)
    [!WS "2"!]Dem_CbLookupTableTrigOnEvSt[DEM_C_LOOKUP_TABLE_TRIG_ON_EVST_SIZE] =
  {
    [!LOOP "DemConfigSet/*/DemEventParameter/*/DemCallbackEventStatusChanged/*[node:exists(DemCallbackEventStatusChangedFnc)]"!]
      [!VAR "CallbackName" = "DemCallbackEventStatusChangedFnc"!]
      [!VAR "SearchStr" = "substring-after($CbNameList, '#')"!]
      [!/* finding the index of the callback in array Dem_CbFuncPtrTrigOnEvSt[] */!][!//
      [!FOR "FuncPtrTableIdx" = "0" TO "$CCbCtr"!]
        [!VAR "FuncNameStr" = "substring-before($SearchStr, '#')"!]
        [!VAR "SearchStr" = "substring-after($SearchStr, '#')"!][!//
        [!IF "$FuncNameStr = $CallbackName"!]
          [!/* current event-name and index to the array Dem_CbFuncPtrTrigOnEvSt[] */!][!//
          [!WS "2"!]{ DemConf_DemEventParameter_[!"name(../../.)"!], [!"num:integer($FuncPtrTableIdx)"!]U }, [!//
          /* -> [!"$CallbackName"!]() */
          [!BREAK!]
        [!ENDIF!]
      [!ENDFOR!]
    [!ENDLOOP!]
  };

  #define DEM_STOP_SEC_CONST_16
  #include <Dem_MemMap.h>
  #endif

  #if (DEM_RTE_CALLBACK_TABLE_TRIG_ON_EVST_SIZE != 0U)
  #define DEM_START_SEC_CONST_UNSPECIFIED
  #include <Dem_MemMap.h>

  /** \brief RTE-Callback function pointer table
   ** for TriggerOnEventStatus
   **/
  CONST(Dem_RteFuncPtrTrigOnEvStType, DEM_CONST)
    [!WS "2"!]Dem_RteFuncPtrTrigOnEvSt[DEM_RTE_CALLBACK_TABLE_TRIG_ON_EVST_SIZE] =
  {
    [!LOOP "DemConfigSet/*/DemEventParameter/*"!]
      [!VAR "RunningNr" = "0"!]
      [!LOOP "DemCallbackEventStatusChanged/*[not(node:exists(DemCallbackEventStatusChangedFnc))]"!]
        [!VAR "RunningNr" = "num:integer($RunningNr + 1)"!]
        [!VAR "CallbackName" = "concat('Rte_Call_CBStatusEvt_', name(../../.), '_', $RunningNr, '_EventStatusChanged')"!]
        [!WS "2"!]&[!"$CallbackName"!],
      [!ENDLOOP!]
    [!ENDLOOP!]
  };

  #define DEM_STOP_SEC_CONST_UNSPECIFIED
  #include <Dem_MemMap.h>
  #endif

  #if (DEM_RTE_LOOKUP_TABLE_TRIG_ON_EVST_SIZE != 0U)
  #define DEM_START_SEC_CONST_16
  #include <Dem_MemMap.h>

  /** \brief RTE-Lookup table for TriggerOnEventStatus */
  CONST(Dem_EventIdType, DEM_CONST)
    [!WS "2"!]Dem_RteLookupTableTrigOnEvSt[DEM_RTE_LOOKUP_TABLE_TRIG_ON_EVST_SIZE] =
  {
    [!LOOP "DemConfigSet/*/DemEventParameter/*/DemCallbackEventStatusChanged/*[not(node:exists(DemCallbackEventStatusChangedFnc))]"!]
      [!WS "2"!]DemConf_DemEventParameter_[!"name(../../.)"!],
    [!ENDLOOP!]
  };

  #define DEM_STOP_SEC_CONST_16
  #include <Dem_MemMap.h>
  #endif

  /*---[TRIGGER_ON_DTC_STATUS]---*/

  #define DEM_START_SEC_CONST_UNSPECIFIED
  #include <Dem_MemMap.h>

  #if (DEM_CB_TABLE_TRIG_ON_DTCST_SIZE != 0U)
  /** \brief Generic callback function pointer table for TriggerOnDTCStatus */
  CONST(Dem_CbFuncPtrTrigOnDTCStType, DEM_CONST)
    [!WS "2"!]Dem_CbFuncPtrTrigOnDTCSt[DEM_CB_TABLE_TRIG_ON_DTCST_SIZE] =
  {
    [!VAR "RunningNr" = "0"!]
    [!LOOP "DemGeneral/DemCallbackDTCStatusChanged/*"!]
      [!IF "node:exists(DemCallbackDTCStatusChangedFnc)"!]
        [!VAR "CallbackName" = "DemCallbackDTCStatusChangedFnc"!]
      [!ELSE!]
        [!VAR "RunningNr" = "num:integer($RunningNr + 1)"!]
        [!VAR "CallbackName" = "concat('Rte_Call_CBStatusDTC_', $RunningNr, '_DTCStatusChanged')"!]
      [!ENDIF!]
      [!WS "2"!]&[!"$CallbackName"!],
    [!ENDLOOP!]
  };
  #endif

  /*---[TRIGGER_ON_EVENT_DATA_CHANGED]---*/

  #if (DEM_C_CB_TABLE_TRIG_ON_EVDAT_SIZE != 0U)
  /** \brief C-Callback function pointer table for TriggerOnEventDataChanged */
  CONST(Dem_CbFuncPtrTrigOnEvDatType, DEM_CONST)
    [!WS "2"!]Dem_CbFuncPtrTrigOnEvDat[DEM_C_CB_TABLE_TRIG_ON_EVDAT_SIZE] =
  {
    [!VAR "CbNameList" = "'#'"!]  [!/* list of all unique callbacks */!][!//
    [!VAR "CCbCtr" = "0"!]        [!/* number of unique callbacks */!][!//
    [!LOOP "DemConfigSet/*/DemEventParameter/*/DemCallbackEventDataChanged[node:exists(DemCallbackEventDataChangedFnc)]"!]
      [!IF "not(contains($CbNameList, concat('#', DemCallbackEventDataChangedFnc, '#')))"!]
        [!VAR "CallbackName" = "DemCallbackEventDataChangedFnc"!]
        [!WS "2"!]&[!"$CallbackName"!],
        [!VAR "CbNameList" = "concat($CbNameList, $CallbackName, '#')"!]
        [!VAR "CCbCtr" = "$CCbCtr + 1"!]
      [!ENDIF!]
    [!ENDLOOP!]
  };
  #endif

  #if (DEM_RTE_CB_TABLE_TRIG_ON_EVDAT_SIZE != 0U)
  /** \brief RTE-Callback function pointer table for TriggerOnEventDataChanged */
  CONST(Dem_RteFuncPtrTrigOnEvDatType, DEM_CONST)
    [!WS "2"!]Dem_RteFuncPtrTrigOnEvDat[DEM_RTE_CB_TABLE_TRIG_ON_EVDAT_SIZE] =
  {
    [!LOOP "DemConfigSet/*/DemEventParameter/*/DemCallbackEventDataChanged[not(node:exists(DemCallbackEventDataChangedFnc))]"!]
      [!VAR "CallbackName" = "concat('Rte_Call_CBDataEvt_', name(../.), '_EventDataChanged')"!]
      [!WS "2"!]&[!"$CallbackName"!],
    [!ENDLOOP!]
  };
  #endif

  #define DEM_STOP_SEC_CONST_UNSPECIFIED
  #include <Dem_MemMap.h>

  #define DEM_START_SEC_CONST_16
  #include <Dem_MemMap.h>

  #if (DEM_C_LOOKUP_TABLE_TRIG_ON_EVDAT_SIZE != 0U)
  /** \brief C-Lookup table for TriggerOnEventDataChanged */
  CONST(Dem_CbLookupTableType, DEM_CONST)
    [!WS "2"!]Dem_CbLookupTableTrigOnEvDat[DEM_C_LOOKUP_TABLE_TRIG_ON_EVDAT_SIZE] =
  {
    [!LOOP "DemConfigSet/*/DemEventParameter/*/DemCallbackEventDataChanged[node:exists(DemCallbackEventDataChangedFnc)]"!]
      [!VAR "CallbackName" = "DemCallbackEventDataChangedFnc"!]
      [!VAR "SearchStr" = "substring-after($CbNameList, '#')"!]
      [!/* finding the index of the callback in array Dem_CbFuncPtrTrigOnEvDat[] */!][!//
      [!FOR "FuncPtrTableIdx" = "0" TO "$CCbCtr"!]
        [!VAR "FuncNameStr" = "substring-before($SearchStr, '#')"!]
        [!VAR "SearchStr" = "substring-after($SearchStr, '#')"!]
        [!IF "$FuncNameStr = $CallbackName"!]
          [!/* current event-name and index to the array Dem_CbFuncPtrTrigOnEvDat[] */!][!//
          [!WS "2"!]{ DemConf_DemEventParameter_[!"name(../.)"!], [!"num:integer($FuncPtrTableIdx)"!]U }, [!//
          /* -> [!"$CallbackName"!]() */
          [!BREAK!]
        [!ENDIF!]
      [!ENDFOR!]
    [!ENDLOOP!]
  };
  #endif

  #if (DEM_RTE_LOOKUP_TABLE_TRIG_ON_EVDAT_SIZE != 0U)
  /** \brief RTE-Lookup table for TriggerOnEventDataChanged */
  CONST(Dem_EventIdType, DEM_CONST)
    [!WS "2"!]Dem_RteLookupTableTrigOnEvDat[DEM_RTE_LOOKUP_TABLE_TRIG_ON_EVDAT_SIZE] =
  {
    [!LOOP "DemConfigSet/*/DemEventParameter/*/DemCallbackEventDataChanged[not(node:exists(DemCallbackEventDataChangedFnc))]"!]
      [!WS "2"!]DemConf_DemEventParameter_[!"name(../.)"!],
    [!ENDLOOP!]
  };
  #endif

  #define DEM_STOP_SEC_CONST_16
  #include <Dem_MemMap.h>

  /*---[CLEAR_EVENT_ALLOWED]---*/

  #define DEM_START_SEC_CONST_UNSPECIFIED
  #include <Dem_MemMap.h>

  #if (DEM_C_CB_TABLE_CLEAR_EVENT_ALLOWED_SIZE != 0U)
  /** \brief C-Callback function pointer table for ClearEventAllowed */
  CONST(Dem_CbFuncPtrClearEventAllowedType, DEM_CONST)
    [!WS "2"!]Dem_CbFuncPtrClearEventAllowed[DEM_C_CB_TABLE_CLEAR_EVENT_ALLOWED_SIZE] =
  {
    [!VAR "CbNameList" = "'#'"!]  [!/* list of all unique callbacks */!][!//
    [!VAR "CCbCtr" = "0"!]        [!/* number of unique callbacks */!][!//
    [!LOOP "DemConfigSet/*/DemEventParameter/*[not(node:exists(DemEvtCmbCommonParamMaster)) or (DemEvtCmbCommonParamMaster = 'true')]/DemCallbackClearEventAllowed[node:exists(DemCallbackClearEventAllowedFnc)]"!]
      [!IF "not(contains($CbNameList, concat('#', DemCallbackClearEventAllowedFnc, '#')))"!]
        [!VAR "CallbackName" = "DemCallbackClearEventAllowedFnc"!]
        [!WS "2"!]&[!"$CallbackName"!],
        [!VAR "CbNameList" = "concat($CbNameList, $CallbackName, '#')"!]
        [!VAR "CCbCtr" = "$CCbCtr + 1"!]
      [!ENDIF!]
    [!ENDLOOP!]
  };
  #endif

  #if (DEM_C_LOOKUP_TABLE_CLEAR_EVENT_ALLOWED_SIZE != 0U)
  /** \brief C-Lookup table for ClearEventAllowed
  [!WS!]** also Clear Event Behavior info is packed in EventId */
  CONST(Dem_CbLookupTableType, DEM_CONST)
    [!WS "2"!]Dem_CbLookupTableClearEventAllowed
      [!WS "4"!][DEM_C_LOOKUP_TABLE_CLEAR_EVENT_ALLOWED_SIZE] =
  {
    [!LOOP "DemConfigSet/*/DemEventParameter/*[not(node:exists(DemEvtCmbCommonParamMaster)) or (DemEvtCmbCommonParamMaster = 'true')]/DemCallbackClearEventAllowed[node:exists(DemCallbackClearEventAllowedFnc)]"!]
      [!VAR "CallbackName" = "DemCallbackClearEventAllowedFnc"!]
      [!VAR "ClEvBehavior" = "DemClearEventAllowedBehavior"!]
      [!VAR "SearchStr" = "substring-after($CbNameList, '#')"!][!//
      [!/* finding the index of the callback in array Dem_CbFuncPtrClearEventAllowed[] */!][!//
      [!FOR "FuncPtrTableIdx" = "0" TO "$CCbCtr"!]
        [!VAR "FuncNameStr" = "substring-before($SearchStr, '#')"!]
        [!VAR "SearchStr" = "substring-after($SearchStr, '#')"!]
        [!IF "$FuncNameStr = $CallbackName"!][!//
          [!/* current event-name and index to the array Dem_CbFuncPtrClearEventAllowed[] */!][!//
          [!WS "2"!]{ [!"$ClEvBehavior"!] | DemConf_DemEventParameter_[!"name(../.)"!], [!"num:integer($FuncPtrTableIdx)"!]U }, [!//
          /* -> [!"$CallbackName"!]() */
          [!BREAK!]
        [!ENDIF!]
      [!ENDFOR!]
    [!ENDLOOP!]
  };
  #endif

  #define DEM_STOP_SEC_CONST_UNSPECIFIED
  #include <Dem_MemMap.h>

  #if (DEM_RTE_LOOKUP_TABLE_CLEAR_EVENT_ALLOWED_SIZE != 0U)
  #define DEM_START_SEC_CONST_16
  #include <Dem_MemMap.h>

  /** \brief RTE-Lookup table for ClearEventAllowed
  [!WS!]** also Clear Event Behavior info is packed in EventId */
  CONST(Dem_EventIdType, DEM_CONST)
    [!WS "2"!]Dem_RteLookupTableClearEventAllowed
      [!WS "4"!][DEM_RTE_LOOKUP_TABLE_CLEAR_EVENT_ALLOWED_SIZE] =
  {
    [!LOOP "DemConfigSet/*/DemEventParameter/*[not(node:exists(DemEvtCmbCommonParamMaster)) or (DemEvtCmbCommonParamMaster = 'true')]/DemCallbackClearEventAllowed[not(node:exists(DemCallbackClearEventAllowedFnc))]"!]
      [!VAR "ClEvBehavior" = "DemClearEventAllowedBehavior"!]
      [!WS "2"!][!"$ClEvBehavior"!] | DemConf_DemEventParameter_[!"name(../.)"!],
    [!ENDLOOP!]
  };

  #define DEM_STOP_SEC_CONST_16
  #include <Dem_MemMap.h>
  #endif

  #define DEM_START_SEC_CONST_UNSPECIFIED
  #include <Dem_MemMap.h>

  #if (DEM_RTE_CB_TABLE_CLEAR_EVENT_ALLOWED_SIZE != 0U)
  /** \brief RTE-Callback function pointer table for ClearEventAllowed */
  CONST(Dem_CbFuncPtrClearEventAllowedType, DEM_CONST)
    [!WS "2"!]Dem_RteFuncPtrClearEventAllowed[DEM_RTE_CB_TABLE_CLEAR_EVENT_ALLOWED_SIZE] =
  {
    [!LOOP "DemConfigSet/*/DemEventParameter/*[not(node:exists(DemEvtCmbCommonParamMaster)) or (DemEvtCmbCommonParamMaster = 'true')]/DemCallbackClearEventAllowed[not(node:exists(DemCallbackClearEventAllowedFnc))]"!]
      [!VAR "CallbackName" = "concat('Rte_Call_CBClrEvt_', name(../.), '_ClearEventAllowed')"!]
      [!WS "2"!]&[!"$CallbackName"!],
    [!ENDLOOP!]
  };
  #endif

  /*---[GET_FDC]---*/

  #if (DEM_CB_TABLE_GET_FDC_SIZE != 0U)
  /** \brief Generic callback function pointer table for
   ** GetFaultDetectionCounter
   **/
  CONST(Dem_CbFuncPtrGetFDCType, DEM_CONST)
    [!WS "2"!]Dem_CbFuncPtrGetFDC[DEM_CB_TABLE_GET_FDC_SIZE] =
  {
    [!LOOP "DemConfigSet/*/DemEventParameter/*/DemEventClass/DemDebounceAlgorithmClass[. = 'DemDebounceMonitorInternal']/DemCallbackGetFDC"!]
      [!IF "node:exists(DemCallbackGetFDCFnc)"!]
        [!VAR "CallbackName" = "DemCallbackGetFDCFnc"!]
        [!WS "2"!]/* event: [!"name(../../../.)"!] */
      [!ELSE!]
        [!VAR "CallbackName" = "concat('Rte_Call_CBFaultDetectCtr_', name(../../../.), '_GetFaultDetectionCounter')"!]
      [!ENDIF!]
      [!WS "2"!]&[!"$CallbackName"!],
    [!ENDLOOP!]
  };
  #endif

  #define DEM_STOP_SEC_CONST_UNSPECIFIED
  #include <Dem_MemMap.h>
  #if (DEM_LOOKUP_TABLE_GET_FDC_SIZE != 0U)

  #define DEM_START_SEC_CONST_16
  #include <Dem_MemMap.h>

  /** \brief Generic lookup table for GetFaultDetectionCounter */
  CONST(Dem_EventIdType, DEM_CONST)
    [!WS "2"!]Dem_CbLookupTableGetFDC[DEM_LOOKUP_TABLE_GET_FDC_SIZE] =
  {
    [!LOOP "DemConfigSet/*/DemEventParameter/*/DemEventClass/DemDebounceAlgorithmClass[. = 'DemDebounceMonitorInternal']/DemCallbackGetFDC"!]
      [!WS "2"!]DemConf_DemEventParameter_[!"name(../../../.)"!], [!//
      [!IF "node:exists(DemCallbackGetFDCFnc)"!]
        /* -> [!"DemCallbackGetFDCFnc"!]() */
      [!ELSE!]
        /* -> Rte_Call */
      [!ENDIF!]
    [!ENDLOOP!]
  };

  #define DEM_STOP_SEC_CONST_16
  #include <Dem_MemMap.h>
  #endif
[!ENDINDENT!]

/*------------------[OBD Readiness Groups force complete]-------------------*/

/* !LINKSTO dsn.Dem.OBDPidSupport.Dem_ReadinessForceStatus_01,1 */
[!VAR "DemOBDForceReady" = "num:hextoint('0xFF')"!]
[!IF "node:exists(DemGeneral/DemGeneralOBD/DemOBDEngineType) and (DemGeneral/DemGeneralOBD/DemOBDEngineType = 'DEM_IGNITION_SPARK')"!]
[!VAR "DemOBDForceReady" = "bit:xor($DemOBDForceReady,num:hextoint('0x08'))"!]
[!ENDIF!]
[!IF "node:exists(DemGeneral/DemOBDForceMisfireMonitoringReady) and (DemGeneral/DemOBDForceMisfireMonitoringReady = 'true')"!]
[!VAR "DemOBDForceReady" = "bit:xor($DemOBDForceReady,num:hextoint('0x10'))"!]
[!ENDIF!]
[!IF "node:exists(DemGeneral/DemOBDForceFuelSysMonitoringReady) and (DemGeneral/DemOBDForceFuelSysMonitoringReady = 'true')"!]
[!VAR "DemOBDForceReady" = "bit:xor($DemOBDForceReady,num:hextoint('0x20'))"!]
[!ENDIF!]
[!IF "node:exists(DemGeneral/DemOBDForceCCMonitoringReady) and (DemGeneral/DemOBDForceCCMonitoringReady = 'true')"!]
[!VAR "DemOBDForceReady" = "bit:xor($DemOBDForceReady,num:hextoint('0x40'))"!]
[!ENDIF!]
#define DEM_START_SEC_CALIB_8
#include <Dem_MemMap.h>

#if (DEM_OBD_Support == STD_ON)
CONST(uint8, DEM_CONST) Dem_ObdReadinessForceCompleteMask = [!"translate(num:inttohex($DemOBDForceReady, 2), 'abcdef', 'ABCDEF')"!]U;
#endif

/*------------------[OBD Readiness Groups PID41 ByteB Pre-set value]---------*/
#if (DEM_OBD_Support == STD_ON)
CONST(uint8, DEM_CONST) Dem_ObdReadinessPID41ByteB = 0x0CU;

[!IF "node:exists(DemGeneral/DemGeneralOBD/DemOBDCompliancy)"!]
CONST(uint8, DEM_CONST) Dem_ObdCompliancyPID1CByte = [!"num:integer(DemGeneral/DemGeneralOBD/DemOBDCompliancy)"!];
[!ENDIF!]
#endif

#define DEM_STOP_SEC_CALIB_8
#include <Dem_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/
[!//
[!INDENT "0"!]
  [!IF "$DemBSWDistributionOfEventReports = 'true'"!]
    [!LOOP "(DemGeneral/DemBSWDistribution/DemSatelliteInstanceConfig/*)"!]
      [!VAR "SatelliteCoreId" = "num:i(DemSatelliteCoreId)"!]

      #define DEM_1_Core_[!"$SatelliteCoreId"!]_START_SEC_VAR_CLEARED_8
      #include <Dem_1_Core_[!"$SatelliteCoreId"!]_MemMap.h>

      /** \brief Index of the next entry in the FIFO queue belonging to the satellite instance [!"$SatelliteCoreId"!] to write into */
      VAR(uint8, DEM_VAR) Dem_Core[!"$SatelliteCoreId"!]_LocalBswEventQueueWriteIdx;
      /** \brief Index of the next entry in the FIFO queue belonging to the satellite instance [!"$SatelliteCoreId"!] to read from */
      VAR(uint8, DEM_VAR) Dem_Core[!"$SatelliteCoreId"!]_LocalBswEventQueueReadIdx;

      #define DEM_1_Core_[!"$SatelliteCoreId"!]_STOP_SEC_VAR_CLEARED_8
      #include <Dem_1_Core_[!"$SatelliteCoreId"!]_MemMap.h>
    [!ENDLOOP!]

    #define DEM_START_SEC_CONST_UNSPECIFIED
    #include <Dem_MemMap.h>

    /* !LINKSTO dsn.Dem.BSWDistribution.SatelliteContext,1 */
    /** \brief Function pointer array for easy access to SchM functions */
    CONST(Dem_SatelliteFncPtrType, DEM_CONST) Dem_SatelliteFncPtr[DEM_NUMBER_OF_CORES] =
    {
      [!FOR "SatelliteCoreId" = "0" TO "$NumberOfSatteliteCores"!]
        [!IF "$SatelliteCoreId != $MasterCoreId"!]
          [!WS "2"!]/* core Id [!"($SatelliteCoreId)"!] */
          [!WS "2"!]{
            [!WS "4"!]SchM_Enter_Dem_1_Core_[!"$SatelliteCoreId"!]_SCHM_DEM_EXCLUSIVE_AREA_SatelliteCore[!"$SatelliteCoreId"!],
            [!WS "4"!]SchM_Exit_Dem_1_Core_[!"$SatelliteCoreId"!]_SCHM_DEM_EXCLUSIVE_AREA_SatelliteCore[!"$SatelliteCoreId"!],
            [!WS "4"!]SchM_Send_Dem_1_Core_[!"$SatelliteCoreId"!]_ReportErrorStatusProvide
          [!WS "2"!]},
        [!ELSE!][!//
          [!WS "2"!]/* master core [!"$MasterCoreId"!] member --> must never be accessed */
          [!WS "2"!]{
            [!WS "4"!]NULL_PTR,
            [!WS "4"!]NULL_PTR,
            [!WS "4"!]NULL_PTR
          [!WS "2"!]},
        [!ENDIF!]
      [!ENDFOR!]
    };

    #define DEM_STOP_SEC_CONST_UNSPECIFIED
    #include <Dem_MemMap.h>
  [!ENDIF!]
[!ENDINDENT!]

/*==================[internal data]=========================================*/
[!//
[!INDENT "0"!]
  [!IF "$DemBSWDistributionOfEventReports = 'true'"!]
    [!LOOP "(DemGeneral/DemBSWDistribution/DemSatelliteInstanceConfig/*)"!]
      [!VAR "SatelliteCoreId" = "num:i(DemSatelliteCoreId)"!]

      #define DEM_1_Core_[!"$SatelliteCoreId"!]_START_SEC_VAR_CLEARED_16
      #include <Dem_1_Core_[!"$SatelliteCoreId"!]_MemMap.h>
      /** \brief Local queue on satellite instance of Core [!"$SatelliteCoreId"!] used for the multi-core functionality,
      [!WS!]** to store BSW event reports when the Dem is not yet fully initialized, i.e. pre-initialized */
      STATIC VAR(Dem_LocalBswEventQueueType, DEM_VAR)
      [!WS "2"!]Dem_Core[!"$SatelliteCoreId"!]_LocalBswEventQueue[DEM_SATELLITE_[!"$SatelliteCoreId"!]_LOCAL_BSW_EVENT_QUEUE_SIZE];
      #define DEM_1_Core_[!"$SatelliteCoreId"!]_STOP_SEC_VAR_CLEARED_16
      #include <Dem_1_Core_[!"$SatelliteCoreId"!]_MemMap.h>
    [!ENDLOOP!]
  [!ENDIF!]
[!ENDINDENT!]

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/
[!//
[!IF "$DemBSWDistributionOfEventReports = 'true'"!]

#define DEM_START_SEC_CODE
#include <Dem_MemMap.h>

FUNC(boolean, DEM_CODE) Dem_QueueBswEventReportOnSatellite(
  CoreIdType          CoreId,
  Dem_EventIdType     EventId,
  Dem_EventStatusType EventStatus)
{
  Dem_LocalBswEventQueueType QueueEntry;
  boolean QueueOperationStatus = FALSE;

  QueueEntry.EventId = EventId;
  QueueEntry.EventStatus = EventStatus;

  switch (CoreId)
  {
[!LOOP "(DemGeneral/DemBSWDistribution/DemSatelliteInstanceConfig/*)"!]
  [!VAR "SatelliteCoreId" = "num:i(DemSatelliteCoreId)"!]
    case DEM_SATELLITE_[!"$SatelliteCoreId"!]_CORE_ID:
    {
      /* queue overflow handling */
      if (Dem_Core[!"$SatelliteCoreId"!]_LocalBswEventQueueWriteIdx <
          DEM_SATELLITE_[!"$SatelliteCoreId"!]_LOCAL_BSW_EVENT_QUEUE_SIZE)
      {
        /* write request into the queue */
        Dem_Core[!"$SatelliteCoreId"!]_LocalBswEventQueue[
          Dem_Core[!"$SatelliteCoreId"!]_LocalBswEventQueueWriteIdx] = QueueEntry;
        ++Dem_Core[!"$SatelliteCoreId"!]_LocalBswEventQueueWriteIdx;

        QueueOperationStatus = TRUE;
      }
      break;
    }
[!ENDLOOP!]
    /* CHECK: NOPARSE */
    default:
    {
      /* never reached, since it must not be called from the master instance core */
      DEM_UNREACHABLE_CODE_ASSERT(DEM_INTERNAL_API_ID);
      break;
    }
    /* CHECK: PARSE */
  }

  return QueueOperationStatus;
}

FUNC(boolean, DEM_CODE) Dem_IsSatelliteQueueEmpty(CoreIdType CoreId)
{
  boolean Result = FALSE;

  switch (CoreId)
  {
[!LOOP "(DemGeneral/DemBSWDistribution/DemSatelliteInstanceConfig/*)"!]
  [!VAR "SatelliteCoreId" = "num:i(DemSatelliteCoreId)"!]
    case DEM_SATELLITE_[!"$SatelliteCoreId"!]_CORE_ID:
    {
      if (Dem_Core[!"$SatelliteCoreId"!]_LocalBswEventQueueWriteIdx ==
          Dem_Core[!"$SatelliteCoreId"!]_LocalBswEventQueueReadIdx)
      {
        Result = TRUE;
      }
      break;
    }
[!ENDLOOP!]
    /* CHECK: NOPARSE */
    default:
    {
      /* never reached, since it must not be called from the master instance core */
      DEM_UNREACHABLE_CODE_ASSERT(DEM_INTERNAL_API_ID);
      break;
    }
    /* CHECK: PARSE */
  }

  return Result;
}

#define DEM_STOP_SEC_CODE
#include <Dem_MemMap.h>
[!//
[!VAR "DevErrorDetect" = "'false'"!]
[!IF "DemGeneral/DemDevErrorDetect = 'true'"!]
  [!VAR "DevErrorDetect" = "'true'"!]
[!ENDIF!]
[!//
[!LOOP "(DemGeneral/DemBSWDistribution/DemSatelliteInstanceConfig/*)"!]
  [!VAR "SatelliteCoreId" = "num:i(DemSatelliteCoreId)"!]

#define DEM_1_Core_[!"$SatelliteCoreId"!]_START_SEC_CODE
#include <Dem_1_Core_[!"$SatelliteCoreId"!]_MemMap.h>
FUNC(void, DEM_1_Core_[!"$SatelliteCoreId"!]_CODE)
  Dem_1_Core_[!"$SatelliteCoreId"!]_SendQueuedBswEventReports(void)
{
  const CoreIdType CoreId = GetCoreID();

[!IF "$DevErrorDetect = 'true'"!]
  if (CoreId != [!"$SatelliteCoreId"!]U)
  {
    /* !LINKSTO Dem.BSWDistribution.BSWReportBufferSendingOnSatellites.WrongContext,1 */
    DEM_REPORT_ERROR(DEM_INTERNAL_API_ID, DEM_E_WRONG_CONTEXT);
  }
  else
[!ENDIF!]
  {
    uint8 QueueIdx;

    if (Dem_Core[!"$SatelliteCoreId"!]_LocalBswEventQueueWriteIdx > 0U)
    {
      for (QueueIdx = 0U;
           QueueIdx < Dem_Core[!"$SatelliteCoreId"!]_LocalBswEventQueueWriteIdx;
           QueueIdx++)
      {
        Dem_EventReportInfoType LocalBswEventInfo;

        LocalBswEventInfo.EventId = Dem_Core[!"$SatelliteCoreId"!]_LocalBswEventQueue[QueueIdx].EventId;
        LocalBswEventInfo.EventStatus = Dem_Core[!"$SatelliteCoreId"!]_LocalBswEventQueue[QueueIdx].EventStatus;
        LocalBswEventInfo.QueuedInPreinitPhase = TRUE;

        /* send the information to the master one by one
         * cast away the return value since it cannot be handled */
        (void)Dem_SatelliteFncPtr[CoreId].SchMSendReportErrorStatusToMaster(
          &LocalBswEventInfo);

        /* update read index of FIFO queue:
         * it's essential to update the ReadIdx after sending the information via Rte
         * to protect the reporting order in case of interruption by Dem_ReportErrorStatus() */
        ++Dem_Core[!"$SatelliteCoreId"!]_LocalBswEventQueueReadIdx;
      }
    }
  }
}
#define DEM_1_Core_[!"$SatelliteCoreId"!]_STOP_SEC_CODE
#include <Dem_1_Core_[!"$SatelliteCoreId"!]_MemMap.h>
[!ENDLOOP!]
[!ENDIF!]

/*==================[end of file]===========================================*/
