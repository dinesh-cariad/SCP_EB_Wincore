/**
 * \file
 *
 * \brief AUTOSAR Com
 *
 * This file contains the implementation of the AUTOSAR
 * module Com.
 *
 * \version 6.3.50
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 11.4 (advisory)
 * A conversion should not be performed between a pointer to object and an integer type.
 *
 * Reason:
 * Actually this is not a conversion between a pointer to object and a integral type, since
 * 0 is the null pointer constant.
 *
 * MISRAC2012-2) Deviated Rule: 1.3 (required)
 * There shall be no occurrence of undefined or critical unspecified behaviour.
 *
 * Reason:
 * MISRA-C:2012 Rule 1.3 gives the following rationale for
 * forbidding the use of this macro: "Use of this macro can lead
 * to undefined behaviour when the types of the operands are
 * incompatible or when bitfields are used."
 * This rationale is an ambiguous and incomplete deduction of a
 * statement in section G.2 "Undefined behavior" of the ISO 9899:1990
 * standard: "The behavior is undefined in the following
 * circumstances: The member designator parameter of an offsetof
 * macro is an invalid right operand of the .operator for the type
 * parameter, or designates a bit-field."
 * Since the only uses of offsetof in the context of this header
 * file is in line with the restrictions imposed by ISO 9899:1990, the
 * behavior is not undefined here.
 * The offsetof macro is needed to compute a structure member's
 * offset from the beginning of a structure in order to ensure that
 * the structure member is the first member of the structure.
 *
 * MISRAC2012-3) Deviated Rule: 11.9 (required)
 * The macro NULL shall be the only permitted form of integer null pointer constant.
 *
 * Reason:
 * offsetof casts a void pointer to a pointer to struct and then obtaining the
 * address of member within the struct. It returns the address offset to the member
 * as size_t type which is not a dereferenced pointer value. This means that the comparison
 * does not require a null pointer constant. Instead a zero with an explicit cast to size_t is valid.
 *
 */

/*==================[inclusions]=============================================*/

#include <Com_Trace.h>
#include <TSAutosar.h>          /* EB specific standard types */
#include <ComStack_Types.h>     /* AUTOSAR standard types */
#include <Com_Api_Static.h>     /* module public static API */
#include <Com_Priv.h>           /* public intra module API */
#include <TSPBConfig_Signature.h> /* TS_PlatformSigIsValid */
#include <Com_DefProg_Cfg.h>

#include <Com_Core_Read.h>
#include <Com_Core_Write.h>

#if (COM_PRECONDITION_ASSERT_ENABLED == STD_ON)
#include <Det.h>                /* Det API */
#include <stddef.h>             /* offsetof() */
#endif

/*==================[macros]=================================================*/

#if (defined COM_INVALID_SIGNAL_ID) /* To prevent double declaration */
#error COM_INVALID_SIGNAL_ID already defined
#endif /* #if (defined COM_INVALID_SIGNAL_ID) */

/** \brief Define COM_INVALID_SIGNAL_ID
 * defines an invalid reference to a signal or group signal */
#define COM_INVALID_SIGNAL_ID COM_SIGNAL_ARRAY_SIZE_MAX

/*==================[type definitions]=======================================*/

#if (COM_PRECONDITION_ASSERT_ENABLED == STD_ON)
   /* CHECK: NOPARSE */

   /* required for retval handling; in several functions the return value is accumulated
    * with bitwise OR */
   DET_STATIC_ASSERT(E_OK == 0U)
   DET_STATIC_ASSERT(E_OK < E_NOT_OK)

   /* required for cast from pointer to ComSignalBaseType to pointer to ComRxSignalType
    * in Com_EB_Read_SignalExt() */
    /* Deviation MISRAC2012-1, MISRAC2012-2, MISRAC2012-3 */
   DET_STATIC_ASSERT((size_t)0U == offsetof(ComRxSignalType, ComSignalBase))

   /* required for internal PduLengthType uint32 handling */
   DET_STATIC_ASSERT( (sizeof(PduLengthType)) >= (sizeof(ComIPduSizeType)) )
   /* CHECK: PARSE */
#endif

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

#if (COM_PRECONDITION_ASSERT_ENABLED == STD_ON)

#define COM_START_SEC_CODE
#include <Com_MemMap.h>

/** \brief Com_EB_CheckAllRxSignals - performs sanity checks all for RxSignals
 ** \param[in] CfgPtr - pointer to a post build configuration
 ** \return Function execution success status
 ** \retval E_OK if no error was found
 **         E_NOT_OK if an error was found
 */
_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckAllRxSignals
(
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
);


/** \brief Com_EB_CheckRxSignal - performs sanity checks for a RxSignal
 ** \param[in] SignalPtr - pointer to configuration of a signal
 ** \param[in] CfgPtr - pointer to a post build configuration
 ** \return Function execution success status
 ** \retval E_OK if no error was found
 **         E_NOT_OK if an error was found
 */
_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckRxSignal
(
   P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) SignalPtr,
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
);


/** \brief Com_EB_CheckRxSignalExt - performs sanity checks for RxSignals
 * regarding extended signal configuration
 ** \param[in] SignalPtr - pointer to configuration of a signal
 ** \param[in] CfgPtr - pointer to a post build configuration
 ** \param[in] BufferRef - reference to I-Pdu buffer
 ** \return Function execution success status
 ** \retval E_OK if no error was found
 **         E_NOT_OK if an error was found
 */
_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckRxSignalExt
(
   P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) SignalPtr,
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr,
   ComRAMIdxType BufferRef
);


/** \brief Com_EB_CheckRxSignalG - performs sanity checks for RxSignals
 * regarding the signal group reference
 ** \param[in] SignalGPtr - pointer to configuration of a signal group
 ** \param[in] CfgPtr - pointer to a post build configuration
 ** \return Function execution success status
 ** \retval E_OK if no error was found
 **         E_NOT_OK if an error was found
 */
_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckRxSignalG
(
   P2CONST(ComRxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr,
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr,
   ComIPduRefType PduId
);


/** \brief Com_EB_CheckAllRxSignals - performs sanity checks all for TxSignals
 ** \param[in] CfgPtr - pointer to a post build configuration
 ** \return Function execution success status
 ** \retval E_OK if no error was found
 **         E_NOT_OK if an error was found
 */
_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckAllTxSignals
(
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
);

/** \brief Com_EB_CheckAllRxSignalGs - performs sanity checks all for RxSignalsGroups
 ** \param[in] CfgPtr - pointer to a post build configuration
 ** \return Function execution success status
 ** \retval E_OK if no error was found
 **         E_NOT_OK if an error was found
 */
_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckAllRxSignalGs
(
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
);

/** \brief Com_EB_CheckAllTxSignalGs - performs sanity checks all for TxSignalsGroups
 ** \param[in] CfgPtr - pointer to a post build configuration
 ** \return Function execution success status
 ** \retval E_OK if no error was found
 **         E_NOT_OK if an error was found
 */
_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckAllTxSignalGs
(
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
);

/** \brief Com_EB_CheckTxSignal - performs sanity checks for a TxSignal
 ** \param[in] SignalPtr - pointer to configuration of a signal
 ** \param[in] CfgPtr - pointer to a post build configuration
 ** \return Function execution success status
 ** \retval E_OK if no error was found
 **         E_NOT_OK if an error was found
 */
_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckTxSignal
(
   P2CONST(ComTxSignalType, AUTOMATIC, COM_APPL_CONST) SignalPtr,
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
);


/** \brief Com_EB_CheckTxSignalG - performs sanity checks for TxSignalGroup
 * regarding the signal group reference and the relation with its group signals
 ** \param[in] CfgPtr - pointer to a post build configuration
 ** \param[in] signalGId - Id of a signal group
 ** \param[in] PduId - Pdu Id
 ** \return Function execution success status
 ** \retval E_OK if no error was found
 **         E_NOT_OK if an error was found
 */
_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckTxSignalG
(
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr,
   ComSignalGroupRefType signalGId,
   ComIPduRefType PduId
);

#if (COM_TMS_ENABLE == STD_ON)
/** \brief Com_EB_CheckGroupSigTMS - performs sanity checks for Com_EB_CheckTxSignalG
 * regarding the signal group reference and the TMS flag of the groups signals
 * \param[in] CfgPtr - pointer to a post build configuration
 * \param[in] SignalGPtr - pointer to configuration of a signal group
 * \param[in] signalGId - Id of a signal group
 * \param[in] firstListEntry - offset value of first filter list entry signal
 * \param[in] listEntry - signal list Id from the TMS flag
 ** \return Function execution success status
 * \retval E_OK if no error was found
 *         E_NOT_OK if an error was found
 */
_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckGroupSigTMS
(
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr,
   P2CONST(ComTxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr,
   ComSignalGroupRefType signalGId,
   Com_SignalIdType firstListEntry,
   Com_SignalIdType listEntry
);
#endif /* (COM_TMS_ENABLE == STD_ON) */

/** \brief Com_EB_CheckAllRxPDUs - performs sanity checks for all for RxPDUs
 ** \param[in] CfgPtr - pointer to a post build configuration
 ** \return Function execution success status
 ** \retval E_OK if no error was found
 **         E_NOT_OK if an error was found
 */
_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckAllRxPDUs
(
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
);

/** \brief Com_EB_CheckAllTxPDUs - performs sanity checks for all for txPDUs
 ** \param[in] CfgPtr - pointer to a post build configuration
 ** \return Function execution success status
 ** \retval E_OK if no error was found
 **         E_NOT_OK if an error was found
 */
_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckAllTxPDUs
(
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
);

/** \brief Com_EB_CheckTxPdu - performs sanity checks for an TxIPdu
 ** \param[in] IPduPtr - pointer to configuration of an IPdu
 ** \param[in] CfgPtr - pointer to a post build configuration
 ** \param[in] PduId - Pdu Id
 ** \return Function execution success status
 ** \retval E_OK if no error was found
 **         E_NOT_OK if an error was found
 */
_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckTxPdu
(
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr,
   ComIPduRefType PduId
);

/** \brief Com_EB_CheckRxPdu - performs sanity checks for a RxIPdu
 ** \param[in] IPduPtr - pointer to configuration of an IPdu
 ** \param[in] CfgPtr - pointer to a post build configuration
 ** \param[in] PduId - Id of ComRxIPdu
 ** \return Function execution success status
 ** \retval E_OK if no error was found
 **         E_NOT_OK if an error was found
 */
_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckRxPdu
(
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr,
   ComIPduRefType PduId
);

/** \brief Com_EB_CheckRxPdu_RxSigs_W_F_A_A_Noti - performs sanity checks for an RxIPdu
 ** \param[in] IPduPtr - pointer to configuration of an IPdu
 ** \param[in] CfgPtr - pointer to a post build configuration
 ** \return Function execution success status
 ** \retval E_OK if no error was found
 **         E_NOT_OK if an error was found
 */
_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckRxPdu_RxSigs_W_F_A_A_Noti
(
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
);

/** \brief Com_EB_CheckRxPdu_RxSignalsDMUBOnly - performs sanity checks for an RxIPdu
 ** \param[in] IPduPtr - pointer to configuration of an IPdu
 ** \param[in] CfgPtr - pointer to a post build configuration
 ** \return Function execution success status
 ** \retval E_OK if no error was found
 **         E_NOT_OK if an error was found
 */
_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckRxPdu_RxSignalsDMUBOnly
(
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
);

/** \brief Com_EB_CheckRxPdu_Callouts - performs sanity checks for an RxIPdu
 ** \param[in] IPduPtr - pointer to configuration of an IPdu
 ** \return Function execution success status
 ** \retval E_OK if no error was found
 **         E_NOT_OK if an error was found
 */
_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckRxPdu_Callouts
(
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
);

/** \brief Com_EB_CheckRxPdu_RxSigs - performs sanity checks for an RxIPdu
 ** \param[in] IPduPtr - pointer to configuration of an IPdu
 ** \param[in] CfgPtr - pointer to a post build configuration
 ** \return Function execution success status
 ** \retval E_OK if no error was found
 **         E_NOT_OK if an error was found
 */
_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckRxPdu_RxSigs
(
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
);

/** \brief Com_EB_CheckRxPdu_RxSigGroups - performs sanity checks for an RxIPdu
 ** \param[in] IPduPtr - pointer to configuration of an IPdu
 ** \param[in] CfgPtr - pointer to a post build configuration
 ** \param[in] PduId - Id of ComRxIPdu
 ** \return Function execution success status
 ** \retval E_OK if no error was found
 **         E_NOT_OK if an error was found
 */
_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckRxPdu_RxSigGroups
(
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr,
   ComIPduRefType PduId
);

/** \brief Com_EB_CheckTxPdu_TMS - performs sanity checks for an TxIPdu
 ** \param[in] IPduPtr - pointer to configuration of an IPdu
 ** \return Function execution success status
 ** \retval E_OK if no error was found
 **         E_NOT_OK if an error was found
 */
_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckTxPdu_TMS
(
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
);

/** \brief Com_EB_CheckTxPdu_Callouts - performs sanity checks for an TxIPdu
 ** \param[in] IPduPtr - pointer to configuration of an IPdu
 ** \param[in] CfgPtr - pointer to a post build configuration
 ** \return Function execution success status
 ** \retval E_OK if no error was found
 **         E_NOT_OK if an error was found
 */
_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckTxPdu_Callouts
(
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
);

/** \brief Com_EB_CheckTxPdu_TxSigs - performs sanity checks for an TxIPdu
 ** \param[in] IPduPtr - pointer to configuration of an IPdu
 ** \param[in] CfgPtr - pointer to a post build configuration
 ** \return Function execution success status
 ** \retval E_OK if no error was found
 **         E_NOT_OK if an error was found
 */
_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckTxPdu_TxSigs
(
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
);

/** \brief Com_EB_CheckTxPdu_TxSigGroups - performs sanity checks for an TxIPdu
 ** \param[in] IPduPtr - pointer to configuration of an IPdu
 ** \param[in] CfgPtr - pointer to a post build configuration
 ** \param[in] PduId - Pdu Id
 ** \return Function execution success status
 ** \retval E_OK if no error was found
 **         E_NOT_OK if an error was found
 */
_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckTxPdu_TxSigGroups
(
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr,
   ComIPduRefType PduId
);

#if (COM_SIGNAL_GW_ENABLE == STD_ON)
/** \brief Com_EB_CheckAllRouteSignals - performs sanity checks all for routed signals
 ** \param[in] CfgPtr - pointer to a post build configuration
 ** \return Function execution success status
 ** \retval E_OK if no error was found
 **         E_NOT_OK if an error was found
 */
_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckAllRouteSignals
(
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
);

/** \brief Com_EB_CheckAllRouteSignals_GWSourceRefs - performs sanity checks for a routed RxIPdu
 ** \param[in] IPduPtr - pointer to configuration of an IPdu
 ** \param[in] CfgPtr - pointer to a post build configuration
 ** \return Function execution success status
 ** \retval E_OK if no error was found
 **         E_NOT_OK if an error was found
 */
_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckAllRouteSignals_GWSourceRefs
(
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
);

/** \brief Com_EB_CheckRouteSignal_GWSourceRefs - performs sanity checks for a routed RxIPdu
 ** \param[in] GWSourcePtr - pointer to configuration of a GWSource
 ** \param[in] CfgPtr - pointer to a post build configuration
 ** \return Function execution success status
 ** \retval E_OK if no error was found
 **         E_NOT_OK if an error was found
 */
_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckRouteSignal_GWSourceRefs
(
  P2CONST(ComGWSourceType, AUTOMATIC, COM_APPL_CONST) GWSourcePtr,
  P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
);

/** \brief Com_EB_CheckRouteSignalGroup_GWSourceRefs - performs sanity checks for a routed RxIPdu
 ** \param[in] GWSourcePtr - pointer to configuration of a GWSource
 ** \param[in] CfgPtr - pointer to a post build configuration
 ** \return Function execution success status
 ** \retval E_OK if no error was found
 **         E_NOT_OK if an error was found
 */
_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckRouteSignalGroup_GWSourceRefs
(
  P2CONST(ComGWSourceType, AUTOMATIC, COM_APPL_CONST) GWSourcePtr,
  P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
);

/** \brief Com_EB_CheckAllRouteSignals_GWDestinationSigRef - sanity checks destination for
 **                                                           a sig GWSource
 ** \param[in] GWSourcePtr - pointer to configuration of a GWSource
 ** \param[in] CfgPtr - pointer to a post build configuration
 ** \return Function execution success status
 ** \retval E_OK if no error was found
 **         E_NOT_OK if an error was found
 */
_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckAllRouteSignals_GWDestinationSigRef
(
   P2CONST(ComGWSourceType, AUTOMATIC, COM_APPL_CONST) GWSourcePtr,
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
);

/** \brief Com_EB_Check_DestSignalRef - sanity checks destination for a signal
 ** \param[in] GWDestinationPtr - pointer to configuration of a GWDestination
 ** \param[in] CfgPtr - pointer to a post build configuration
 ** \return Function execution success status
 ** \retval E_OK if no error was found
 **         E_NOT_OK if an error was found
 */
_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_Check_DestSignalRef
(
  P2CONST(ComGWDestinationType, AUTOMATIC, COM_APPL_CONST) GWDestinationPtr,
  P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
);

/** \brief Com_EB_CheckRouteGroupSignal_GWSourceRefs - performs sanity checks for a routed RxIPdu
 ** \param[in] GWSourcePtr - pointer to configuration of a GWSource
 ** \param[in] CfgPtr - pointer to a post build configuration
 ** \return Function execution success status
 ** \retval E_OK if no error was found
 **         E_NOT_OK if an error was found
 */
_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckRouteGroupSignal_GWSourceRefs
(
  P2CONST(ComGWSourceType, AUTOMATIC, COM_APPL_CONST) GWSourcePtr,
  P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
);

/** \brief Com_EB_CheckAllRouteSignals_GWDestinationSigGRef - sanity checks destinations for
 **                                                            a sig group GWSource
 ** \param[in] GWSourcePtr - pointer to configuration of a GWSource
 ** \param[in] CfgPtr - pointer to a post build configuration
 ** \return Function execution success status
 ** \retval E_OK if no error was found
 **         E_NOT_OK if an error was found
 */
_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckAllRouteSignals_GWDestinationSigGRef
(
   P2CONST(ComGWSourceType, AUTOMATIC, COM_APPL_CONST) GWSourcePtr,
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
);

/** \brief Com_EB_Check_DestSignalGRef - sanity checks destination for a sig group
 ** \param[in] GWDestinationPtr - pointer to configuration of a GWDestination
 ** \param[in] CfgPtr - pointer to a post build configuration
 ** \return Function execution success status
 ** \retval E_OK if no error was found
 **         E_NOT_OK if an error was found
 */
_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_Check_DestSignalGRef
(
  P2CONST(ComGWDestinationType, AUTOMATIC, COM_APPL_CONST) GWDestinationPtr,
  P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
);

#endif

#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>

#endif /* (COM_PRECONDITION_ASSERT_ENABLED == STD_ON) */

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

#if (COM_ISVALIDCONFIG_MMAP_ENABLED == STD_ON)
#define COM_START_SEC_CODE_CC_BLOCK
#include <Com_MemMap.h>
#else
#define COM_START_SEC_CODE
#include <Com_MemMap.h>
#endif

FUNC(Std_ReturnType, COM_CODE) Com_IsValidConfig
(
   P2CONST(void, AUTOMATIC,COM_APPL_CONST) ConfigPtr
)
{
   Std_ReturnType retval = E_NOT_OK;

   DBG_COM_ISVALIDCONFIG_ENTRY(ConfigPtr);

   /* check for NULL_PTR */
   if (NULL_PTR != ConfigPtr)
   {
      CONSTP2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) TmpCfgPtr =
         (P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST)) ConfigPtr;

      /* check platform signature */
      if (TRUE == TS_PlatformSigIsValid(TmpCfgPtr->ComPlatformSignature))
      {
         /* check pre-compile time signature */
         if (COM_CFG_SIGNATURE == TmpCfgPtr->ComCfgSignature)
         {
            /* check link-time signature */
            if (Com_LcfgSignature == TmpCfgPtr->ComLcfgSignature)
            {
               /* check published information signature */
               if (COM_PUBLIC_INFO_SIGNATURE == TmpCfgPtr->ComPublicInfoSignature)
               {
                  /* Indicate that the configuration is valid */
                  retval = E_OK;

#if (COM_PRECONDITION_ASSERT_ENABLED == STD_ON)
    /* CHECK: NOPARSE */
                  retval |= Com_EB_CheckAllRxSignals(TmpCfgPtr);

                  retval |= Com_EB_CheckAllTxSignals(TmpCfgPtr);

                  retval |= Com_EB_CheckAllRxSignalGs(TmpCfgPtr);

                  retval |= Com_EB_CheckAllTxSignalGs(TmpCfgPtr);

                  retval |= Com_EB_CheckAllRxPDUs(TmpCfgPtr);

                  retval |= Com_EB_CheckAllTxPDUs(TmpCfgPtr);

# if (COM_SIGNAL_GW_ENABLE == STD_ON)
                  retval |= Com_EB_CheckAllRouteSignals(TmpCfgPtr);
# endif
    /* CHECK: PARSE */
#endif /* (COM_PRECONDITION_ASSERT_ENABLED == STD_ON) */

               }
            }
         }
      }
   }


   DBG_COM_ISVALIDCONFIG_EXIT(retval,ConfigPtr);
   return retval;
}                               /* Com_IsValidConfig */

#if (COM_ISVALIDCONFIG_MMAP_ENABLED == STD_ON)
#define COM_STOP_SEC_CODE_CC_BLOCK
#include <Com_MemMap.h>
#else
#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>
#endif

/*==================[internal function definitions]==========================*/

#if (COM_PRECONDITION_ASSERT_ENABLED == STD_ON)
   /* CHECK: NOPARSE */
#define COM_START_SEC_CODE
#include <Com_MemMap.h>

_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckAllRxSignals
(
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
)
{
   Std_ReturnType retval = E_OK;

   uint32 ii;

   for (ii = 0U; ii < CfgPtr->ComRxSignalArraySize; ii++)
   {
      CONSTP2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) SignalPtr =
               COM_GET_CONFIG_ELEMENT_TEMP_CHECK(CfgPtr, ComRxSignalType, CfgPtr->ComRxSignalRef,
                     ii);

      if (SignalPtr == NULL_PTR)
      {
         retval = E_NOT_OK;
         COM_PRECONDITION_ASSERT_NO_EVAL(
               (SignalPtr != NULL_PTR),
               COM_INTERNAL_API_ID);
      } else if (SignalPtr->ComConfigCheckData != COM_CONFIG_CHECK_RX_SIGNAL)
      {
         retval = E_NOT_OK;
         COM_PRECONDITION_ASSERT_NO_EVAL(
               (SignalPtr->ComConfigCheckData == COM_CONFIG_CHECK_RX_SIGNAL),
               COM_INTERNAL_API_ID);
      } else if (Com_EB_CheckRxSignal(SignalPtr, CfgPtr) != E_OK)
      {
         retval = E_NOT_OK;
         COM_PRECONDITION_ASSERT_NO_EVAL(
               (Com_EB_CheckRxSignal(SignalPtr, CfgPtr) == E_OK),
               COM_INTERNAL_API_ID);
      }
      else
      {
         /* empty else */
      }

   }

   return retval;
}                               /* Com_EB_CheckRxSignal */


_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckAllRxSignalGs
(
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
)
{
   Std_ReturnType retval = E_OK;

   uint32 ii;

   for (ii = 0U; ii < CfgPtr->ComRxSignalGroupArraySize; ii++)
   {
      CONSTP2CONST(ComRxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr =
               COM_GET_CONFIG_ELEMENT_TEMP_CHECK(CfgPtr, ComRxSignalGroupType,
                        CfgPtr->ComRxSignalGroupRef, ii);

      if (SignalGPtr == NULL_PTR)
      {
         retval = E_NOT_OK;
         COM_PRECONDITION_ASSERT_NO_EVAL(
               (SignalGPtr != NULL_PTR),
               COM_INTERNAL_API_ID);
      } else if (SignalGPtr->ComConfigCheckData != COM_CONFIG_CHECK_RX_SIG_GROUP)
      {
         retval = E_NOT_OK;
         COM_PRECONDITION_ASSERT_NO_EVAL(
               (SignalGPtr->ComConfigCheckData == COM_CONFIG_CHECK_RX_SIG_GROUP),
               COM_INTERNAL_API_ID);
      }
      else
      {
         /* empty else */
      }

   }

   return retval;
}                               /* Com_EB_CheckAllRxSignalGs */

_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckAllTxSignalGs
(
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
)
{
   Std_ReturnType retval = E_OK;

   uint32 ii;

   for (ii = 0U; ii < CfgPtr->ComTxSignalGroupArraySize; ii++)
   {
      CONSTP2CONST(ComTxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr =
               COM_GET_CONFIG_ELEMENT_TEMP_CHECK(CfgPtr, ComTxSignalGroupType,
                        CfgPtr->ComTxSignalGroupRef, ii);

      if (SignalGPtr == NULL_PTR)
      {
         retval = E_NOT_OK;
         COM_PRECONDITION_ASSERT_NO_EVAL(
               (SignalGPtr != NULL_PTR),
               COM_INTERNAL_API_ID);
      } else if (SignalGPtr->ComConfigCheckData != COM_CONFIG_CHECK_TX_SIG_GROUP)
      {
         retval = E_NOT_OK;
         COM_PRECONDITION_ASSERT_NO_EVAL(
               (SignalGPtr->ComConfigCheckData == COM_CONFIG_CHECK_TX_SIG_GROUP),
               COM_INTERNAL_API_ID);
      }
      else
      {
         /* empty else */
      }

   }

   return retval;
}                               /* Com_EB_CheckAllTxSignalGs */



_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckRxSignal
(
   P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) SignalPtr,
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
)
{

   Std_ReturnType retval = E_NOT_OK;
   uint8 ipdu_check = E_NOT_OK;

   ComRAMIdxType PduBufferRef = COM_RAM_SIZE_MAX;
   ComIPduRefType PduId;
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr = NULL_PTR;

#if ( (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) )
   if(TRUE == COM_IS_RX_DUMMY_SIGNAL(SignalPtr->ComSignalBase))
   {
     /* no Rx pdus exists only dummy Rx signals */
     ipdu_check = E_OK;
   }
   else
#endif /* ( (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) ) */
   {
     PduId = SignalPtr->ComIPduRef;
     IPduPtr = COM_GET_CONFIG_ELEMENT_TEMP_CHECK(CfgPtr, ComRxIPduType, CfgPtr->ComRxIPduRef, PduId);

     if (NULL_PTR != IPduPtr)
     {
       if (IPduPtr->ComConfigCheckData == COM_CONFIG_CHECK_RX_IPDU)
       {
         ipdu_check = E_OK;
         PduBufferRef = IPduPtr->ComIPduValueRef;
       }
     }
   }

   if (ipdu_check == E_OK)
   {

      uint8 sigG_check = E_NOT_OK;
      uint8 sigE_check = E_NOT_OK;

      ComSignalGroupRefType const SignalGId = SignalPtr->ComSignalBase.ComSignalGroupRef;

#if ( (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) )
      if(TRUE == COM_IS_RX_DUMMY_SIGNAL(SignalPtr->ComSignalBase) )
      {
        /* signal is a dummy signal / dummy group signal */
        sigG_check = E_OK;

        if( (SignalGId == COM_SIGNAL_INVALID) &&
            (SignalPtr->ComSignalExtRef == COM_SIGNAL_INVALID) &&
            (SignalPtr->ComIPduRef == 0U) &&
            (SignalPtr->ComSignalBase.ComBufferRef == 0U))
        {
          sigE_check = E_OK;
        }

      }
      else
#endif /* ( (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) ) */
      {
        if (SignalGId == COM_SIGNAL_INVALID)
        {
          /* signal is a normal signal */
          sigG_check = E_OK;
          sigE_check = Com_EB_CheckRxSignalExt(SignalPtr, CfgPtr, PduBufferRef);
        }
        else
        {
          /* signal is a group signal */

          if (SignalPtr->ComSignalExtRef == COM_SIGNAL_INVALID)
          {
            sigE_check = E_OK;
          }
          if (CfgPtr->ComRxSignalGroupArraySize > SignalGId)
          {
            CONSTP2CONST(ComRxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr =
                COM_GET_CONFIG_ELEMENT_TEMP_CHECK(CfgPtr, ComRxSignalGroupType,
                                                  CfgPtr->ComRxSignalGroupRef,
                                                  SignalPtr->ComSignalBase.ComSignalGroupRef);

            if (SignalPtr->ComSignalBase.ComBufferRef == SignalGPtr->ComSignalGroupValueRef)
            {
              sigG_check = E_OK;
            }
          }

        }
      }

      retval = sigE_check | sigG_check | ipdu_check;
   }

   return retval;
}                               /* Com_EB_CheckRxSignal */

_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckRxSignalExt
(
   P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) SignalPtr,
      P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr,
   ComRAMIdxType BufferRef
)
{

   Std_ReturnType retval = E_NOT_OK;

   CONSTP2CONST(ComRxSignalExtType, AUTOMATIC, COM_APPL_CONST) SignalExtPtr =
            COM_GET_CONFIG_ELEMENT_TEMP_CHECK(CfgPtr, ComRxSignalExtType,
                     CfgPtr->ComRxSignalExtRef, SignalPtr->ComSignalExtRef);

   if (SignalExtPtr != NULL_PTR)
   {

      if (SignalExtPtr->ComConfigCheckData == COM_CONFIG_CHECK_RX_SIG_EXT)
      {

         uint8 buffer_check = E_NOT_OK;
         uint8 fptr_check = E_OK;
         uint8 filterRef_check = E_OK;

         if (SignalPtr->ComSignalBase.ComBufferRef == BufferRef)
         {
            buffer_check = E_OK;
         }

#if (COM_FILTER_RECEIVER_ENABLE == STD_ON)
         /* if ComFilterType is TRIVIAL then ComFilterSignalValueRef shall be invalid */
         if ( FALSE == (COM_F_NON_TRIVIAL(SignalPtr->ComSignalBase.ComFilterConf.ComFilterType)) )
         {
           /* no signal buffer shall be allocated / referenced */
           if (SignalPtr->ComSignalBase.ComFilterConf.ComFilterSignalValueRef !=
                    COM_SIGNAL_BUFFER_REF_INVALID)
           {
             filterRef_check= E_NOT_OK;
              COM_PRECONDITION_ASSERT_NO_EVAL(
                       (SignalPtr->ComSignalBase.ComFilterConf.ComFilterSignalValueRef ==
                                COM_SIGNAL_BUFFER_REF_INVALID),
                                COM_INTERNAL_API_ID);
           }
         }
         else /* ComFilterType is NON_TRIVIAL */
         {
           /* if ComFilterType is NON_TRIVIAL and SignalType is COM_UINT8_N, SINT64, UINT64 or UINT8_DYN then
            * ComFilterSignalValueRef shall be invalid */
           /* HINT: no filter support for UINT8_N and UINT8_DYN (SINT64/UINT64 supports only filter
            * algorithm which does not require a ComSignalBase) */
           if( (COM_GET_RX_SIGNAL_SIGNAL_TYPE(&SignalPtr->ComSignalBase) == COM_UINT8_N) ||
               (COM_GET_RX_SIGNAL_SIGNAL_TYPE(&SignalPtr->ComSignalBase) == COM_UINT8_DYN) ||
               (COM_GET_RX_SIGNAL_SIGNAL_TYPE(&SignalPtr->ComSignalBase) == COM_SINT64) ||
               (COM_GET_RX_SIGNAL_SIGNAL_TYPE(&SignalPtr->ComSignalBase) == COM_UINT64) )
           {
             /* no signal buffer shall be allocated / referenced */
             if (SignalPtr->ComSignalBase.ComFilterConf.ComFilterSignalValueRef !=
                      COM_SIGNAL_BUFFER_REF_INVALID)
             {
               filterRef_check= E_NOT_OK;
                COM_PRECONDITION_ASSERT_NO_EVAL(
                         (SignalPtr->ComSignalBase.ComFilterConf.ComFilterSignalValueRef ==
                                  COM_SIGNAL_BUFFER_REF_INVALID),
                                  COM_INTERNAL_API_ID);
             }
           }
           /* if ComFilterType is NON_TRIVIAL and SignalType is not COM_UINT8_N (SINT64/UINT64) then
            * ComFilterSignalValueRef shall be valid */
           else
           {
             /* signal buffer shall be allocated / referenced */
             if (SignalPtr->ComSignalBase.ComFilterConf.ComFilterSignalValueRef ==
                      COM_SIGNAL_BUFFER_REF_INVALID)
             {
               filterRef_check= E_NOT_OK;
                COM_PRECONDITION_ASSERT_NO_EVAL(
                         (SignalPtr->ComSignalBase.ComFilterConf.ComFilterSignalValueRef ==
                                  COM_SIGNAL_BUFFER_REF_INVALID),
                                  COM_INTERNAL_API_ID);
             }
           }
         }
#endif /* #if (COM_FILTER_RECEIVER_ENABLE == STD_ON) */

         if (SignalExtPtr->ComNotification != COM_CBK_RX_ACK_FUNC_PTR_NULL)
         {
            /* function pointer for ComNotification callbacks */
            P2VAR(Com_CbkRxAck_Type, AUTOMATIC, COM_APPL_CODE) fptr;

            fptr = Com_CbkRxAck_Array[(SignalExtPtr->ComNotification)];
            if (fptr == NULL_PTR)
            {
               fptr_check = E_NOT_OK;
               COM_PRECONDITION_ASSERT_NO_EVAL((fptr != NULL_PTR), COM_INTERNAL_API_ID);
            }
         }


         retval =  buffer_check | fptr_check | filterRef_check;

      }

   }

   return retval;
}                               /* Com_EB_CheckRxSignalExt */

_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckRxSignalG
(
   P2CONST(ComRxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr,
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr,
   ComIPduRefType PduId
)
{

   Std_ReturnType retval = E_NOT_OK;

   if (SignalGPtr != NULL_PTR)
   {

      if (SignalGPtr->ComConfigCheckData == COM_CONFIG_CHECK_RX_SIG_GROUP)
      {
         uint8 pduId_check = E_NOT_OK;

         if (SignalGPtr->ComIPduRef == PduId)
         {
            pduId_check = E_OK;
         }

#if (COM_NOTIFY_RECEIVERS_ENABLE == STD_ON)
#if (COM_CBK_RX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE)
         if (pduId_check == E_OK)
         {
            CONSTP2CONST(ComRxSignalExtType, AUTOMATIC, COM_APPL_CONST) SignalExtPtr =
                  COM_GET_CONFIG_ELEMENT_TEMP_CHECK(CfgPtr,ComRxSignalExtType,
                        CfgPtr->ComRxSignalExtRef, SignalGPtr->ComSignalExtRef);

            if (SignalExtPtr->ComNotification != COM_CBK_RX_ACK_FUNC_PTR_NULL)
            {
               CONSTP2VAR(Com_CbkRxAck_Type, AUTOMATIC, COM_APPL_CODE) fptr =
                   Com_CbkRxAck_Array[(SignalExtPtr->ComNotification)];
               if (fptr == NULL_PTR)
               {
                  pduId_check = E_NOT_OK;
                  COM_PRECONDITION_ASSERT_NO_EVAL((fptr != NULL_PTR), COM_INTERNAL_API_ID);
               }
            }
         }
#endif
#endif /* (COM_NOTIFY_RECEIVERS_ENABLE == STD_ON) */

         retval = pduId_check;

      }
   }

   return retval;
}                               /* Com_EB_CheckRxSignalG */

_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckAllTxSignals
(
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
)
{
   Std_ReturnType retval = E_OK;

   uint32 ii;

   for (ii = 0U; ii < CfgPtr->ComTxSignalArraySize; ii++)
   {
      CONSTP2CONST(ComTxSignalType, AUTOMATIC, COM_APPL_CONST) SignalPtr =
               COM_GET_CONFIG_ELEMENT_TEMP_CHECK(CfgPtr, ComTxSignalType, CfgPtr->ComTxSignalRef,
                     ii);

      if (SignalPtr == NULL_PTR)
      {
         retval = E_NOT_OK;
         COM_PRECONDITION_ASSERT_NO_EVAL(
               (SignalPtr != NULL_PTR),
               COM_INTERNAL_API_ID);
      } else if (SignalPtr->ComConfigCheckData != COM_CONFIG_CHECK_TX_SIGNAL)
      {
         retval = E_NOT_OK;
         COM_PRECONDITION_ASSERT_NO_EVAL(
               (SignalPtr->ComConfigCheckData == COM_CONFIG_CHECK_TX_SIGNAL),
               COM_INTERNAL_API_ID);
      } else if (Com_EB_CheckTxSignal(SignalPtr, CfgPtr) != E_OK)
      {
         retval = E_NOT_OK;
         COM_PRECONDITION_ASSERT_NO_EVAL(
               (Com_EB_CheckTxSignal(SignalPtr, CfgPtr) == E_OK),
               COM_INTERNAL_API_ID);
      }
      else
      {
         /* empty else */
      }
   }

   return retval;
}                               /* Com_EB_CheckAllTxSignals */

_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckTxSignal
(
   P2CONST(ComTxSignalType, AUTOMATIC, COM_APPL_CONST) SignalPtr,
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
)
{
   Std_ReturnType retval = E_NOT_OK;
   uint8 ipdu_check = E_NOT_OK;
   ComRAMIdxType PduBufferRef = COM_RAM_SIZE_MAX;

   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr = NULL_PTR;
   ComIPduRefType PduId = SignalPtr->ComIPduRef;


#if ( (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) )
   if(TRUE == COM_IS_TX_DUMMY_SIGNAL(SignalPtr))
   {
     /* no Tx pdus exists only dummy Tx signals */
     ipdu_check = E_OK;
   }
   else
#endif /* (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) */
   {
     IPduPtr = COM_GET_CONFIG_ELEMENT_TEMP_CHECK(CfgPtr, ComTxIPduType, CfgPtr->ComTxIPduRef, PduId);

     if (NULL_PTR != IPduPtr)
     {
       if (IPduPtr->ComConfigCheckData == COM_CONFIG_CHECK_TX_IPDU)
       {
         ipdu_check = E_OK;
         PduBufferRef = IPduPtr->ComIPduValueRef;
       }
     }

   }

   if (ipdu_check == E_OK)
   {
      uint8 sigG_check = E_NOT_OK;
      uint8 sigE_check = E_NOT_OK;

      {
         ComSignalGroupRefType const signalGId = SignalPtr->ComSignalBase.ComSignalGroupRef;

         if (signalGId == COM_SIGNAL_INVALID)
         {
           sigG_check = E_OK;

#if ( (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) )
           if (TRUE == COM_IS_TX_DUMMY_SIGNAL(SignalPtr))
           {
             /* signal is a dummy signal / dummy group signal */

             if( (SignalPtr->ComSignalBase.ComBufferRef == 0U) &&
                 (SignalPtr->ComIPduRef == 0U) )
             {
               if( COM_UINT8_DYN == COM_GET_TX_SIGNAL_SIGNAL_TYPE(SignalPtr) && (SignalPtr->ComSignalBase.ComBitSize == 0U) )
               {
                 /* dyn dummy signal / dyn dummy group signal is valid */
                 sigE_check = E_OK;
               }
               else if( COM_UINT8_DYN != COM_GET_TX_SIGNAL_SIGNAL_TYPE(SignalPtr) && (SignalPtr->ComSignalBase.ComBitSize == COM_SIGNAL_INVALID) )
               {
                 /* dummy signal / dummy group signal is valid */
                 sigE_check = E_OK;
               }
               else
               {
                 /* empty else */
               }
             }
           }
           else
#endif /* (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) */
           {
             if ( (SignalPtr->ComSignalBase.ComBufferRef == PduBufferRef) )
             {
                /* signal is a normal signal */
                sigE_check = E_OK;
             }
           }

         }
         else
         {
            /* signal is a group signal */

            CONSTP2CONST(ComTxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr =
                     COM_GET_CONFIG_ELEMENT_TEMP_CHECK(CfgPtr, ComTxSignalGroupType,
                              CfgPtr->ComTxSignalGroupRef, signalGId);

            sigE_check = E_OK;
            sigG_check = Com_EB_CheckTxSignalG(CfgPtr, signalGId, PduId);

            if (sigG_check == E_OK)
            {
               if (SignalPtr->ComSignalBase.ComBufferRef != SignalGPtr->ComSignalGroupValueRef)
               {
                  sigG_check = E_NOT_OK;
                  COM_PRECONDITION_ASSERT_NO_EVAL(SignalPtr->ComSignalBase.ComBufferRef ==
                           SignalGPtr->ComSignalGroupValueRef, COM_INTERNAL_API_ID);
               }
            }
         }
      }

#if (COM_TMS_ENABLE == STD_ON)
#if ((COM_TXF_MASKNEWDIFFERSMASKOLD_EN == STD_ON) || \
         (COM_FILTER_ONE_EVERY_N_AVAILABLE == STD_ON))
      if(sigE_check == E_OK)
      {
         /* check if the signal has the filter COM_F_MASK_NEW_DIFFERS_MASK_OLD */
         if (SignalPtr->ComSignalBase.ComFilterConf.ComFilterType ==
                  COM_F_MASK_NEW_DIFFERS_MASK_OLD)
         {
            if (SignalPtr->ComSignalBase.ComFilterConf.ComFilterSignalValueRef ==
                     COM_SIGNAL_BUFFER_REF_INVALID)
            {
               sigE_check= E_NOT_OK;
               COM_PRECONDITION_ASSERT_NO_EVAL(
                        (SignalPtr->ComSignalBase.ComFilterConf.ComFilterSignalValueRef !=
                                 COM_SIGNAL_BUFFER_REF_INVALID),
                                 COM_INTERNAL_API_ID);
            }
         }
         else
#endif
         /* ComFilterSignalValueRef set COM_SIGNAL_BUFFER_REF_INVALID in case of TXSignals / TxGroupSignals
          * where the filter is different COM_F_MASK_NEW_DIFFERS_MASK_OLD */
         {
            if (SignalPtr->ComSignalBase.ComFilterConf.ComFilterSignalValueRef !=
                     COM_SIGNAL_BUFFER_REF_INVALID)
            {
               sigE_check= E_NOT_OK;
               COM_PRECONDITION_ASSERT_NO_EVAL(
                        (SignalPtr->ComSignalBase.ComFilterConf.ComFilterSignalValueRef ==
                                 COM_SIGNAL_BUFFER_REF_INVALID),
                                 COM_INTERNAL_API_ID);
            }
         }
      }
#endif

      retval = sigE_check | sigG_check | ipdu_check;
   }

   return retval;
}

_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckTxSignalG
(
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr,
   ComSignalGroupRefType signalGId,
   ComIPduRefType PduId
)
{

   CONSTP2CONST(ComTxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr =
            COM_GET_CONFIG_ELEMENT_TEMP_CHECK(CfgPtr, ComTxSignalGroupType,
                     CfgPtr->ComTxSignalGroupRef, signalGId);

   Std_ReturnType retval = E_NOT_OK;
   uint8 sigG_check = E_NOT_OK;
   uint8 TMSCfg_check = E_NOT_OK;

   if (SignalGPtr != NULL_PTR)
   {
      if (SignalGPtr->ComConfigCheckData == COM_CONFIG_CHECK_TX_SIG_GROUP)
      {
         if (SignalGPtr->ComIPduRef == PduId)
         {
            sigG_check = E_OK;
         }
      }
   }

   if (sigG_check == E_OK)
   {

#if (COM_TMS_ENABLE == STD_ON)
      if (SignalGPtr->ComGroupSignalWithFilterRefNum == 0U)
      {
         /* no signals with filters are configured */
         TMSCfg_check = E_OK;
      }
      else
      {

         /* signal has filter */

         Com_SignalIdType listEntry; /* iteration variable */

         Com_SignalIdType const firstListEntry = SignalGPtr->ComGroupSignalRefFirst;

         /* loop over all group signals with filter */
         for(listEntry = firstListEntry;
                  listEntry < (SignalGPtr->ComGroupSignalWithFilterRefNum + firstListEntry);
                  listEntry++)
         {
            TMSCfg_check = E_OK;
            TMSCfg_check |= Com_EB_CheckGroupSigTMS(
                     CfgPtr,
                     SignalGPtr,
                     signalGId,
                     firstListEntry,
                     listEntry);

         }
      }
#else
      TMSCfg_check = E_OK;
#endif /* (COM_TMS_ENABLE == STD_ON) */
   }

   retval = sigG_check | TMSCfg_check;

   return retval;
}                               /* Com_EB_CheckTxSignalG */

#if (COM_TMS_ENABLE == STD_ON)
/** \brief Com_EB_CheckGroupSigTMS - performs sanity checks for Com_EB_CheckTxSignalG
 * regarding the signal group reference and the TMS flag of the groups signals
 * \param[in] CfgPtr - pointer to a post build configuration
 * \param[in] SignalGPtr - pointer to configuration of a signal group
 * \param[in] signalGId - Id of a signal group
 * \param[in] firstListEntry - offset value of first filter list entry signal
 * \param[in] listEntry - signal list Id from the TMS flag
 * \return Function execution success status
 * \retval E_OK if no error was found
 *         E_NOT_OK if an error was found
 */
_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckGroupSigTMS
(
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr,
   P2CONST(ComTxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr,
   ComSignalGroupRefType signalGId,
   Com_SignalIdType firstListEntry,
   Com_SignalIdType listEntry
)
{

   Std_ReturnType TMSCfg_check = E_NOT_OK;

   /* Get signal id and pointer to config of the signal in the signal list of the IPdu */
   ComSignalRefType const GSignalId =
            (ComSignalRefType) * (COM_GET_CONFIG_ELEMENT_TEMP_CHECK(CfgPtr,ComSignalRefType,
                     (CfgPtr->ComTxSignalGroupGroupSignalListRef), listEntry));

   CONSTP2CONST(ComTxSignalType, AUTOMATIC, COM_APPL_CONST) GSignalPtr =
            COM_GET_CONFIG_ELEMENT_TEMP_CHECK(CfgPtr,ComTxSignalType,
                     CfgPtr->ComTxSignalRef, GSignalId);

   /* the groups signal shall belong to the signal group */
   if (GSignalPtr->ComSignalBase.ComSignalGroupRef == signalGId)
   {
      /* the TMS flag of the  group signal shall be the position of the signal in the
       * signal with filter list */
      if ((GSignalPtr->ComPositionOfTMSFlag + firstListEntry) == listEntry)
      {
         if ((GSignalPtr->ComSignalBase.ComFilterConf.ComFilterType != COM_F_ALWAYS)
                  && (GSignalPtr->ComSignalBase.ComFilterConf.ComFilterType != COM_F_NEVER))
         {

            /* get initial TMS flags of the groups signals */
            CONSTP2CONST(uint8, AUTOMATIC, COM_APPL_CONST) initFlagPtr =
                     COM_GET_CONFIG_ELEMENT_TEMP_CHECK(CfgPtr, uint8,
                              SignalGPtr->ComSignalGTMSFlagsInitValueRef, 0U);

            /* an initial TMS config shall be available  */
            if (initFlagPtr != NULL_PTR)
            {
               TMSCfg_check = E_OK;
            }
         }
      }

   }

   return TMSCfg_check;
}

#endif /* (COM_TMS_ENABLE == STD_ON) */

_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckAllRxPDUs
(
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
)
{
   Std_ReturnType RetVal = E_OK;
   ComIPduRefType RxPduId;
   for (RxPduId = 0; RxPduId < CfgPtr->ComRxIPduArraySize; RxPduId++)
   {
      CONSTP2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr =
      COM_GET_CONFIG_ELEMENT_TEMP_CHECK(CfgPtr,ComRxIPduType, CfgPtr->ComRxIPduRef, RxPduId);
      if (NULL_PTR == IPduPtr)
      {
         RetVal = E_NOT_OK;
         COM_PRECONDITION_ASSERT_NO_EVAL(
                  (IPduPtr != NULL_PTR),
                  COM_INTERNAL_API_ID);
      }
      else if (IPduPtr->ComConfigCheckData != COM_CONFIG_CHECK_RX_IPDU)
      {
         RetVal = E_NOT_OK;
         COM_PRECONDITION_ASSERT_NO_EVAL(
                  (IPduPtr->ComConfigCheckData == COM_CONFIG_CHECK_RX_IPDU),
                  COM_INTERNAL_API_ID);
      }
      else if (Com_EB_CheckRxPdu(IPduPtr, CfgPtr, RxPduId) != E_OK)
      {
         RetVal = E_NOT_OK;
         COM_PRECONDITION_ASSERT_NO_EVAL(
               (Com_EB_CheckRxPdu(IPduPtr, CfgPtr, RxPduId) == E_OK),
               COM_INTERNAL_API_ID);
      }
      else
      {
         /* empty else */
      }
   }

   return RetVal;
}


_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckAllTxPDUs
(
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
)
{
   Std_ReturnType RetVal = E_OK;
   ComIPduRefType TxPduId;
   for (TxPduId = 0; TxPduId < CfgPtr->ComTxIPduArraySize; TxPduId++)
   {
      CONSTP2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr =
      COM_GET_CONFIG_ELEMENT_TEMP_CHECK(CfgPtr,ComTxIPduType, CfgPtr->ComTxIPduRef, TxPduId);

      if (NULL_PTR == IPduPtr)
      {
         RetVal = E_NOT_OK;
         COM_PRECONDITION_ASSERT_NO_EVAL(
                  (IPduPtr != NULL_PTR),
                  COM_INTERNAL_API_ID);
      }
      else if (IPduPtr->ComConfigCheckData != COM_CONFIG_CHECK_TX_IPDU)
      {
         RetVal = E_NOT_OK;
         COM_PRECONDITION_ASSERT_NO_EVAL(
                  (IPduPtr->ComConfigCheckData == COM_CONFIG_CHECK_TX_IPDU),
                  COM_INTERNAL_API_ID);
      }
      else if (Com_EB_CheckTxPdu(IPduPtr, CfgPtr, TxPduId) != E_OK)
      {
         RetVal = E_NOT_OK;
         COM_PRECONDITION_ASSERT_NO_EVAL(
               (Com_EB_CheckTxPdu(IPduPtr, CfgPtr, TxPduId) == E_OK),
               COM_INTERNAL_API_ID);
      }
      else
      {
         /* empty else */
      }
   }

   return RetVal;
}

_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckRxPdu
(
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr,
   ComIPduRefType PduId
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  Std_ReturnType IpduCheck = E_NOT_OK;
  Std_ReturnType SigCheck = E_NOT_OK;

  if (NULL_PTR != IPduPtr)
  {

    if (IPduPtr->ComConfigCheckData == COM_CONFIG_CHECK_RX_IPDU)
    {

#if (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON)
      if( TRUE == COM_GET_COM_RX_IPDU_DUMMY(IPduPtr) )
      {
        /* dummy pdu does not references signals */
        SigCheck = E_OK;

#if (COM_RX_TPAPI_ENABLE == STD_ON)
        if (IPduPtr->ComRxIpduTPControlInfoRef == COM_IPDU_ARRAY_SIZE_MAX)
        {
          /* dummy Pdu shall not be Tp Pdu */
          IpduCheck = E_OK;
        }
#endif /* (COM_RX_TPAPI_ENABLE == STD_ON) */

      }
      else
#endif /* (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) */
      {

        IpduCheck = Com_EB_CheckRxPdu_RxSigs_W_F_A_A_Noti(IPduPtr, CfgPtr);

        IpduCheck |= Com_EB_CheckRxPdu_RxSignalsDMUBOnly(IPduPtr, CfgPtr);

        IpduCheck |= Com_EB_CheckRxPdu_Callouts(IPduPtr);

        if (IpduCheck == E_OK)
        {

          SigCheck = Com_EB_CheckRxPdu_RxSigs(IPduPtr, CfgPtr);

          SigCheck |= Com_EB_CheckRxPdu_RxSigGroups(IPduPtr, CfgPtr, PduId);

        }

      }

    }

  }

  RetVal = IpduCheck | SigCheck;

  return RetVal;
}

_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckTxPdu
(
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr,
   ComIPduRefType PduId
)
{
   Std_ReturnType RetVal = E_NOT_OK;
   Std_ReturnType IpduCheck = E_NOT_OK;
   Std_ReturnType SigCheck = E_NOT_OK;

   if (NULL_PTR != IPduPtr)
   {

     if (IPduPtr->ComConfigCheckData == COM_CONFIG_CHECK_TX_IPDU)
     {

#if (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON)
       if( TRUE == COM_GET_COM_TX_IPDU_DUMMY(IPduPtr) )
       {
         /* dummy pdu does not references signals */
         SigCheck = E_OK;

#if (COM_TX_TPAPI_ENABLE == STD_ON)
         if (IPduPtr->ComTxIpduTPControlInfoRef == COM_IPDU_ARRAY_SIZE_MAX)
         {
           IpduCheck = E_OK;
         }
#endif /* (COM_RX_TPAPI_ENABLE == STD_ON) */

       }
       else
#endif /* (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) */
       {

         IpduCheck = Com_EB_CheckTxPdu_TMS(IPduPtr);

         IpduCheck |= Com_EB_CheckTxPdu_Callouts(IPduPtr, CfgPtr);

         if (IpduCheck == E_OK)
         {

           SigCheck = Com_EB_CheckTxPdu_TxSigs(IPduPtr, CfgPtr);

           SigCheck |= Com_EB_CheckTxPdu_TxSigGroups(IPduPtr, CfgPtr, PduId);

         }

       }

     }

   }

   RetVal = IpduCheck | SigCheck;

   return RetVal;
}

_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckRxPdu_RxSigs_W_F_A_A_Noti
(
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
)
{
   Std_ReturnType RetVal = E_OK;

#if ((COM_CBK_RX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE) && \
     (COM_FILTER_RECEIVER_ENABLE == STD_ON))
   ComSignalRefType i;              /* loop variable */
   ComSignalRefType loopStart;      /* variable for start value of loop */
   ComSignalRefType loopStop;       /* variable for stop value of loop */
   loopStart = IPduPtr->ComIPduSignalWithBufferRefNum;
   loopStop = IPduPtr->ComIPduSignalW_F_A_NotiRefNum;

   /* Handle all which have no invalidation and COM_F_ALWAYS */
   for (i = loopStart; i < loopStop; i++)
   {
      /* Get signal id and pointer to config of the signal in the signal list of the IPdu */
      const ComSignalRefType signalId =
         (ComSignalRefType) *
         (COM_GET_CONFIG_ELEMENT_TEMP_CHECK(CfgPtr,
           ComSignalRefType,
           (CfgPtr->ComRxIPduSignalListRef), (i + IPduPtr->ComIPduSignalRefFirst)));

      if (signalId == COM_INVALID_SIGNAL_ID)
      {
         RetVal = E_NOT_OK;
         COM_PRECONDITION_ASSERT_NO_EVAL(signalId != COM_INVALID_SIGNAL_ID, COM_INTERNAL_API_ID);
      }
      else
      {
         CONSTP2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) SignalPtr =
            COM_GET_CONFIG_ELEMENT_TEMP_CHECK(CfgPtr,ComRxSignalType, CfgPtr->ComRxSignalRef,
                  signalId);
         if (SignalPtr->ComSignalBase.ComFilterConf.ComFilterType != COM_F_ALWAYS)
         {
            RetVal = E_NOT_OK;
            /* The signal has a filter configured which is not COM_F_ALWAYS */
            COM_PRECONDITION_ASSERT_NO_EVAL(SignalPtr->ComSignalBase.ComFilterConf.ComFilterType ==
                  COM_F_ALWAYS, COM_INTERNAL_API_ID);
         }
      }
   }
#else
   TS_PARAM_UNUSED(IPduPtr);
   TS_PARAM_UNUSED(CfgPtr);
#endif /* (COM_FILTER_RECEIVER_ENABLE == STD_ON) && \
 (COM_CBK_RX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE) */

   return RetVal;
}

_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckRxPdu_RxSignalsDMUBOnly
(
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
)
{
   Std_ReturnType RetVal = E_OK;

#if (COM_SIGNALS_WITH_UB_RXDM_GW_ONLY == STD_ON)
   ComSignalRefType i;              /* loop variable */
   ComSignalRefType loopStart = IPduPtr->ComIPduSignalRefFirst;
   ComSignalRefType loopStop = IPduPtr->ComIPduSignalRefFirst;
#if (COM_SIGNAL_GW_ENABLE == STD_ON)
   loopStop = loopStop + IPduPtr->ComIPduSignalRefNum;
#else
   loopStop = loopStop + IPduPtr->ComIPduSignalWithDMOnly;
#endif /* (COM_SIGNAL_GW_ENABLE == STD_ON) */
#if (COM_CBK_RX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE)
   loopStart = loopStart + IPduPtr->ComIPduSignalW_F_A_NotiRefNum;

#else
   /* no notifications can be configured, therefore the loop starts either
    * with ComIPduSignalWithBufferRefNum if available or with 0 */
#if (COM_FILTER_RECEIVER_ENABLE == STD_ON)
   loopStart = loopStart + IPduPtr->ComIPduSignalWithBufferRefNum;
#else
   /* do not change loopStart */
#endif /* (COM_FILTER_RECEIVER_ENABLE == STD_ON) */
#endif /* (COM_CBK_RX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE) */
   /* Check all signals with update bit */
   for (i = loopStart; i < loopStop; i++)
   {
      /* Get signal id and pointer to config of the signal in the signal list of the IPdu */
      const ComSignalRefType signalId =
         (ComSignalRefType) *
         (COM_GET_CONFIG_ELEMENT_TEMP_CHECK(CfgPtr,
           ComSignalRefType, CfgPtr->ComRxIPduSignalListRef, i));

      if (signalId == COM_INVALID_SIGNAL_ID)
      {
         RetVal = E_NOT_OK;
         COM_PRECONDITION_ASSERT_NO_EVAL(signalId != COM_INVALID_SIGNAL_ID, COM_INTERNAL_API_ID);
      }
      else
      {
         CONSTP2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) SignalPtr =
            COM_GET_CONFIG_ELEMENT_TEMP_CHECK(CfgPtr,ComRxSignalType, CfgPtr->ComRxSignalRef,
                  signalId);

#if (COM_FILTER_RECEIVER_ENABLE == STD_ON)
         {
             boolean fail_condition;
             fail_condition =
                   (SignalPtr->ComSignalBase.ComFilterConf.ComFilterType != COM_F_ALWAYS)
                   && (SignalPtr->ComSignalBase.ComFilterConf.ComFilterType != COM_F_NEVER);

             if (fail_condition)
             {
                RetVal = E_NOT_OK;
                /* The signal has a filter configured which is not COM_F_ALWAYS or COM_F_NEVER*/
                COM_PRECONDITION_ASSERT_NO_EVAL((!fail_condition), COM_INTERNAL_API_ID);
             }
         }
#else
   /* No filter available, therefore the signal is treated as a
    * signal with the filter COM_F_ALWAYS */
#endif /* (COM_FILTER_RECEIVER_ENABLE == STD_ON) */

#if (COM_CBK_RX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE)
         {

            CONSTP2CONST(ComRxSignalExtType, AUTOMATIC, COM_APPL_CONST) SignalExtPtr =
                     COM_GET_CONFIG_ELEMENT_TEMP_CHECK(CfgPtr,ComRxSignalExtType,
                              CfgPtr->ComRxSignalExtRef, SignalPtr->ComSignalExtRef);

             boolean fail_condition;
             fail_condition =
#if (COM_SIGNAL_GW_ENABLE == STD_ON)
                  (i < (ComSignalRefType)(IPduPtr->ComIPduSignalRefFirst +
                       IPduPtr->ComIPduSignalWithDMOnly)) &&
#endif /* (COM_SIGNAL_GW_ENABLE == STD_ON) */
                  (SignalExtPtr->ComNotification != COM_CBK_RX_ACK_FUNC_PTR_NULL);
            if (fail_condition)
            {
               RetVal = E_NOT_OK;
               COM_PRECONDITION_ASSERT_NO_EVAL((!fail_condition), COM_INTERNAL_API_ID);
            }
         }
      }
#endif /* (COM_CBK_RX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE) */
   }
#else
   TS_PARAM_UNUSED(IPduPtr);
   TS_PARAM_UNUSED(CfgPtr);
#endif /* (COM_SIGNALS_WITH_UB_RXDM_GW_ONLY == STD_ON) */
   return RetVal;
}

_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckRxPdu_Callouts
(
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
)
{
   Std_ReturnType RetVal;

#if (COM_CALL_OUT_FUNC_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE)
   /* Get the function pointer of the callout function */
   if (IPduPtr->ComIPduCallOutFuncPtrRef == COM_CALL_OUT_FUNC_PTR_NULL)
   {
     RetVal = E_OK;
   }
   else
   {
      CONSTP2VAR(Com_RxCalloutType, AUTOMATIC, COM_APPL_CODE) fptr =
            Com_RxCallouts[(IPduPtr->ComIPduCallOutFuncPtrRef)];
      if (fptr == NULL_PTR)
      {
         RetVal = E_NOT_OK;
         COM_PRECONDITION_ASSERT_NO_EVAL((fptr != NULL_PTR), COMServiceId_RxIndication);
      }
      else
      {
         RetVal = E_OK;
      }
   }
#else
   TS_PARAM_UNUSED(IPduPtr);
   RetVal = E_OK;
#endif

   return RetVal;
}

_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckRxPdu_RxSigs
(
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
)
{
   Std_ReturnType RetVal = E_OK;

#if (COM_FILTER_RECEIVER_ENABLE == STD_ON)
   ComSignalRefType ii;

   for (ii = 0; ii < IPduPtr->ComIPduSignalWithBufferRefNum; ii++)
   {
      /* Get signal id and pointer to config of the signal in the signal list of the IPdu */
      /* Id of signal which belongs to Pdu */
      const ComSignalRefType signalId =
               (ComSignalRefType) * (COM_GET_CONFIG_ELEMENT_TEMP_CHECK(CfgPtr,ComSignalRefType,
                        (CfgPtr->ComRxIPduSignalListRef),
                        (ii + IPduPtr->ComIPduSignalRefFirst)));

      if (signalId == COM_INVALID_SIGNAL_ID)
      {
         RetVal = E_NOT_OK;
         COM_PRECONDITION_ASSERT_NO_EVAL(signalId != COM_INVALID_SIGNAL_ID, COM_INTERNAL_API_ID);
      }
      else
      {
         /* pointer to the configuration of the signal */
         P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) SignalPtr =
                  COM_GET_CONFIG_ELEMENT_TEMP_CHECK(CfgPtr,ComRxSignalType,
                        CfgPtr->ComRxSignalRef, signalId);

         if (SignalPtr == NULL_PTR)
         {
            RetVal = E_NOT_OK;
            COM_PRECONDITION_ASSERT_NO_EVAL(
                  (SignalPtr != NULL_PTR),
                  COM_INTERNAL_API_ID);
         } else if (SignalPtr->ComConfigCheckData != COM_CONFIG_CHECK_RX_SIGNAL)
         {
            RetVal = E_NOT_OK;
            COM_PRECONDITION_ASSERT_NO_EVAL(
                  (SignalPtr->ComConfigCheckData == COM_CONFIG_CHECK_RX_SIGNAL),
                  COM_INTERNAL_API_ID);
         } else if (Com_EB_CheckRxSignal(SignalPtr, CfgPtr) != E_OK)
         {
            RetVal = E_NOT_OK;
            COM_PRECONDITION_ASSERT_NO_EVAL(
                  (Com_EB_CheckRxSignal(SignalPtr, CfgPtr) == E_OK),
                  COM_INTERNAL_API_ID);
         }
         else
         {
            /* empty else */
         }
      }

   }
#else
   TS_PARAM_UNUSED(IPduPtr);
   TS_PARAM_UNUSED(CfgPtr);
#endif

   return RetVal;
}

_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckRxPdu_RxSigGroups
(
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr,
   ComIPduRefType PduId
)
{
   Std_ReturnType RetVal;
   ComSignalGroupRefType ii;              /* loop variable */

   if(IPduPtr->ComIPduSignalGroupRefNum > 0U)
   {

      RetVal = E_OK;

      /* Handle signals all signal groups within the I-Pdu */
      for (ii = 0U; ii < IPduPtr->ComIPduSignalGroupRefNum; ii++)
      {
         /* Get signal id and pointer to config of the signal in the signal list of the IPdu */
         const ComSignalGroupRefType signalGId =
               (ComSignalGroupRefType) *
               (COM_GET_CONFIG_ELEMENT_TEMP_CHECK(CfgPtr,
                     ComSignalGroupRefType,
                     (CfgPtr->ComRxIPduSignalGroupListRef),
                     (ii + IPduPtr->ComIPduSignalGroupRefFirst)));

         CONSTP2CONST(ComRxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr =
               COM_GET_CONFIG_ELEMENT_TEMP_CHECK(CfgPtr,ComRxSignalGroupType,
                     CfgPtr->ComRxSignalGroupRef, signalGId);

         if(E_NOT_OK == Com_EB_CheckRxSignalG(SignalGPtr, CfgPtr, PduId))
         {
            RetVal = E_NOT_OK;
            break;
         }
      }

   }
   else
   {
      RetVal = E_OK;
   }

   return RetVal;
}

_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckTxPdu_TMS
(
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
)
{
   Std_ReturnType RetVal;
   boolean fail_condition;

   TS_PARAM_UNUSED(IPduPtr);

#if (COM_TM_AVAILABLE == STD_ON)

   /* If no reference is available, the transmission mode has to be
    * COM_TX_MODE_MODE_NONE or COM_TX_MODE_MODE_MIXED_DIRECT */

   fail_condition =
         (!((COM_GET_COM_TX_MODE_TRUE(IPduPtr->ComTxModeModes) == COM_TX_MODE_MODE_NONE)
               ||
               (COM_GET_COM_TX_MODE_TRUE(IPduPtr->ComTxModeModes) == COM_TX_MODE_MODE_DIRECT))
         )
         &&
         (COM_GET_COM_TX_MODE_REF_TRUE(IPduPtr) == COM_TX_MODE_INVALID);
   if (fail_condition == TRUE)
   {
      COM_PRECONDITION_ASSERT_NO_EVAL(!(fail_condition), COMServiceId_Init);
      RetVal = E_NOT_OK;
   }
   else
   {
      RetVal = E_OK;
   }

#if (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL)

   /* COM054: Note: Update-bits are not allowed if Direct/N-Times Transmission
    * Mode with n>1 is used */
   fail_condition =
#if (COM_UPDATE_BIT_TX_CONFIG == COM_UPDATE_BIT_INDIVIDUAL)
         (COM_GET_COM_TX_IPDU_CLEAR_UPDATE_BIT(IPduPtr) != COM_CL_UB_NEVER)
         &&
#endif /* (COM_UPDATE_BIT_TX_CONFIG == COM_UPDATE_BIT_INDIVIDUAL) */
         ( (COM_GET_COM_TX_MODE_TRUE(IPduPtr->ComTxModeModes) == COM_TX_MODE_MODE_N_TIMES)
               ||
               (COM_GET_COM_TX_MODE_FALSE(IPduPtr->ComTxModeModes) == COM_TX_MODE_MODE_N_TIMES)
         );
   if ((fail_condition == TRUE) && (RetVal == E_OK))
   {
      COM_PRECONDITION_ASSERT_NO_EVAL((!fail_condition), COMServiceId_Init);
      RetVal = E_NOT_OK;
   }

#endif /* (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) */

#if (COM_TMS_ENABLE == STD_ON)

   /* check if Transmission Mode FALSE is configured right */
   fail_condition =
         (!((COM_GET_COM_TX_MODE_FALSE(IPduPtr->ComTxModeModes) == COM_TX_MODE_MODE_NONE)
               ||
               (COM_GET_COM_TX_MODE_FALSE(IPduPtr->ComTxModeModes) == COM_TX_MODE_MODE_DIRECT))
         )
         &&
         (COM_GET_COM_TX_MODE_REF_FALSE(IPduPtr) == COM_TX_MODE_INVALID);
   if ((fail_condition == TRUE) && (RetVal == E_OK))
   {
      COM_PRECONDITION_ASSERT_NO_EVAL((!fail_condition), COMServiceId_Init);
      RetVal = E_NOT_OK;
   }

#endif /* (COM_TMS_ENABLE == STD_ON) */

#else
   RetVal = E_OK;
#endif /* (COM_TM_AVAILABLE == STD_ON) */

   return RetVal;
}

/** \brief Com_EB_CheckTxPdu_Callouts - performs sanity checks for an TxIPdu
 ** \param[in] IPduPtr - pointer to configuration of an IPdu
 ** \param[in] CfgPtr - pointer to a post build configuration
 ** \return Function execution success status
 ** \retval E_OK if no error was found
 **         E_NOT_OK if an error was found
 */
_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckTxPdu_Callouts
(
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
)
{
   Std_ReturnType RetVal = E_OK;

   TS_PARAM_UNUSED(IPduPtr);
   TS_PARAM_UNUSED(CfgPtr);

#if (COM_CALL_OUT_FUNC_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE)
   if (IPduPtr->ComIPduCallOutFuncPtrRef != COM_CALL_OUT_FUNC_PTR_NULL)
   {
      CONSTP2VAR(Com_TxCalloutType, AUTOMATIC, COM_APPL_CODE) fptr =
            Com_TxCallouts[(IPduPtr->ComIPduCallOutFuncPtrRef)];
      /* check it the function pointer is NULL_PTR */
      if (fptr == NULL_PTR)
      {
         RetVal = E_NOT_OK;
         COM_PRECONDITION_ASSERT_NO_EVAL((fptr != NULL_PTR), COM_INTERNAL_API_ID);
      }
   }
#endif

#if (COM_CALL_OUT_FUNC_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE)
#if (COM_TRIGGERTX_CALLOUT_ENABLE == STD_ON)
   if (IPduPtr->ComIPduTrigTxCallOutFuncPtrRef != COM_CALL_OUT_FUNC_PTR_NULL)
   {
      CONSTP2VAR(Com_TxCalloutType, AUTOMATIC, COM_APPL_CODE) fptr =
               Com_TxCallouts[(IPduPtr->ComIPduTrigTxCallOutFuncPtrRef)];
      /* Check if fptr is valid */
      if (fptr == NULL_PTR)
      {
         RetVal = E_NOT_OK;
         COM_PRECONDITION_ASSERT_NO_EVAL((fptr != NULL_PTR), COMServiceId_TriggerTransmit);
      }
   }
#endif /* (COM_TRIGGERTX_CALLOUT_ENABLE == STD_ON) */
#endif /* (COM_CALL_OUT_FUNC_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE) */

#if (COM_CBK_TX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE)
   {
      Com_CbkTxAckIdxType i;
      for (i = IPduPtr->ComNotificationFirst;
           i < (IPduPtr->ComNotificationFirst + IPduPtr->ComNotificationNum); i++)
      {
         const Com_CbkTxAckIdxType TxAckIdx =
               ((Com_CbkTxAckIdxType) *COM_GET_CONFIG_ELEMENT_TEMP_CHECK(CfgPtr,Com_CbkTxAckIdxType,
               (CfgPtr->ComTxNotificationRef), (i)));
         CONSTP2VAR(Com_CbkTxAck_Type, AUTOMATIC, COM_APPL_CODE) fptr =
               Com_CbkTxAck_Array[TxAckIdx];
         /* check it the function pointer is NULL_PTR */
         if (fptr == NULL_PTR)
         {
            RetVal = E_NOT_OK;
            COM_PRECONDITION_ASSERT_NO_EVAL((fptr != NULL_PTR), COM_INTERNAL_API_ID);
         }
      }
   }
#endif

   return RetVal;
}

_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckTxPdu_TxSigs
(
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
)
{
   Std_ReturnType RetVal = E_OK;
   ComSignalRefType ii;

   for (ii = IPduPtr->ComIPduSignalRefFirst; ii < (IPduPtr->ComIPduSignalRefNum +
         IPduPtr->ComIPduSignalRefFirst); ii++)
   {
      /* Get signal id and pointer to config of the signal in the signal list of the IPdu */
      /* Id of signal which belongs to Pdu */
      const ComSignalRefType signalId =
         (ComSignalRefType) * (COM_GET_CONFIG_ELEMENT_TEMP_CHECK(CfgPtr,ComSignalRefType,
            (CfgPtr->ComTxIPduSignalListRef), ii));

      if (signalId == COM_INVALID_SIGNAL_ID)
      {
         RetVal = E_NOT_OK;
         COM_PRECONDITION_ASSERT_NO_EVAL(signalId != COM_INVALID_SIGNAL_ID, COM_INTERNAL_API_ID);
      }
      else
      {
         /* pointer to the configuration of the signal */
         P2CONST(ComTxSignalType, AUTOMATIC, COM_APPL_CONST) SignalPtr =
           COM_GET_CONFIG_ELEMENT_TEMP_CHECK(CfgPtr,ComTxSignalType, CfgPtr->ComTxSignalRef, signalId);

         if (SignalPtr == NULL_PTR)
         {
            RetVal = E_NOT_OK;
            COM_PRECONDITION_ASSERT_NO_EVAL(
                  (SignalPtr != NULL_PTR),
                  COM_INTERNAL_API_ID);
         } else if (SignalPtr->ComConfigCheckData != COM_CONFIG_CHECK_TX_SIGNAL)
         {
            RetVal = E_NOT_OK;
            COM_PRECONDITION_ASSERT_NO_EVAL(
                  (SignalPtr->ComConfigCheckData == COM_CONFIG_CHECK_TX_SIGNAL),
                  COM_INTERNAL_API_ID);
         } else if (Com_EB_CheckTxSignal(SignalPtr, CfgPtr) != E_OK)
         {
            RetVal = E_NOT_OK;
            COM_PRECONDITION_ASSERT_NO_EVAL(
                  (Com_EB_CheckTxSignal(SignalPtr, CfgPtr) == E_OK),
                  COM_INTERNAL_API_ID);
         }
         else
         {
            /* empty else */
         }
      }
   }

   return RetVal;
}

_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckTxPdu_TxSigGroups
(
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr,
   ComIPduRefType PduId
)
{
   Std_ReturnType RetVal;
   ComSignalRefType ii;

   if(IPduPtr->ComIPduSignalGroupRefFirst < (IPduPtr->ComIPduSignalGroupRefFirst + IPduPtr->ComIPduSignalGroupRefNum))
   {

      RetVal = E_OK;

      for (ii = IPduPtr->ComIPduSignalGroupRefFirst; ii < (IPduPtr->ComIPduSignalGroupRefNum +
            IPduPtr->ComIPduSignalGroupRefFirst); ii++)
      {
         ComSignalGroupRefType const signalGId =
               (ComSignalGroupRefType) *
               (COM_GET_CONFIG_ELEMENT_TEMP_CHECK(CfgPtr, ComSignalGroupRefType,
                     CfgPtr->ComTxIPduSignalGroupListRef, ii));

         if(E_NOT_OK == Com_EB_CheckTxSignalG(CfgPtr, signalGId, PduId))
         {
            RetVal = E_NOT_OK;
            break;
         }
      }

   }
   else
   {
      RetVal = E_OK;
   }

   return RetVal;
}

#if (COM_SIGNAL_GW_ENABLE == STD_ON)
_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckAllRouteSignals
(
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
)
{
   /* To ensure that combining of retvals in calling function works, if not entering loop */
   uint8 RetVal = E_OK;

   uint16 sigGWRxPduRef; /* iteration variable */
   for (sigGWRxPduRef = 0U;
         sigGWRxPduRef < CfgPtr->ComSigGWRxIPduArraySize;
         sigGWRxPduRef++)
   {
      /* get Id of IPdu */
      const ComIPduRefType IPduId = (ComIPduRefType) * (COM_GET_CONFIG_ELEMENT_TEMP_CHECK(CfgPtr,
             ComIPduRefType,(CfgPtr->ComSigGWRxIPduListRef), sigGWRxPduRef));

      /* get RxPduId configuration reference */
      CONSTP2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr =
            COM_GET_CONFIG_ELEMENT_TEMP_CHECK(CfgPtr,ComRxIPduType, (CfgPtr->ComRxIPduRef), IPduId);

      if (NULL_PTR == IPduPtr)
      {
         RetVal = E_NOT_OK;
         COM_PRECONDITION_ASSERT_NO_EVAL(
               (IPduPtr != NULL_PTR),
               COM_INTERNAL_API_ID);
      }
      else if (IPduPtr->ComConfigCheckData != COM_CONFIG_CHECK_RX_IPDU)
      {
         RetVal = E_NOT_OK;
         COM_PRECONDITION_ASSERT_NO_EVAL(
               (IPduPtr->ComConfigCheckData == COM_CONFIG_CHECK_RX_IPDU),
               COM_INTERNAL_API_ID);
      }
      else
      {
         RetVal |= Com_EB_CheckAllRouteSignals_GWSourceRefs(IPduPtr, CfgPtr);
      }
   }

   return RetVal;
}

_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckAllRouteSignals_GWSourceRefs
(
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
)
{
   /* To ensure that combining of retvals in calling function works, if not entering loop */
   Std_ReturnType RetVal = E_OK;
   ComGWSourceRefType sigGWSourceRef; /* iteration variable */

   for (sigGWSourceRef = IPduPtr->ComIPduGWSourceRefFirst;
         sigGWSourceRef < (IPduPtr->ComIPduGWSourceRefFirst + IPduPtr->ComIPduGWSourceRefNum);
         sigGWSourceRef++)
   {
      /* get routing information */
      CONSTP2CONST(ComGWSourceType, AUTOMATIC, COM_APPL_CONST) GWSourcePtr =
            COM_GET_CONFIG_ELEMENT_TEMP_CHECK(CfgPtr,ComGWSourceType,
            (CfgPtr->ComGWSourceRef), sigGWSourceRef);

      if (NULL_PTR == GWSourcePtr)
      {
         RetVal = E_NOT_OK;
         COM_PRECONDITION_ASSERT_NO_EVAL(
               (NULL_PTR != GWSourcePtr),
               COM_INTERNAL_API_ID);
      }
      else if (GWSourcePtr->ComConfigCheckData != COM_CONFIG_CHECK_GW_SOURCE)
      {
         RetVal = E_NOT_OK;
         COM_PRECONDITION_ASSERT_NO_EVAL(
               (GWSourcePtr->ComConfigCheckData == COM_CONFIG_CHECK_GW_SOURCE),
               COM_INTERNAL_API_ID);
      }
      else
      {
         switch (GWSourcePtr->ComGWSourceSelect)
         {
            case COM_GW_SIGNAL:
            {
               /* the gated object is a signal, handle it */
               RetVal |= Com_EB_CheckRouteSignal_GWSourceRefs(GWSourcePtr, CfgPtr);
               break;
            } /* case COM_GW_SIGNAL */

            case COM_GW_SIGNAL_GROUP:
            {
               /* the gated object is a signal group, handle it */
               RetVal |= Com_EB_CheckRouteSignalGroup_GWSourceRefs(GWSourcePtr, CfgPtr);
               break;
            }                   /* case COM_GW_SIGNAL_GROUP */

            case COM_GW_GROUP_SIGNAL:
            {
               /* the gated object is a signal group, handle it */
               RetVal |= Com_EB_CheckRouteGroupSignal_GWSourceRefs(GWSourcePtr, CfgPtr);
               break;
            }                   /* case COM_GW_GROUP_SIGNAL */

            default:
               COM_UNREACHABLE_CODE_ASSERT(COMServiceId_MainFunctionRouteSignals);
               break;
         }          /* switch (GWSourcePtr->ComGWSourceSelect) */

      }

   }

   return RetVal;
}

_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckRouteSignal_GWSourceRefs
(
  P2CONST(ComGWSourceType, AUTOMATIC, COM_APPL_CONST) GWSourcePtr,
  P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
)
{
  Std_ReturnType RetVal;

  CONSTP2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) SrcSignalPtr =
  COM_GET_CONFIG_ELEMENT_TEMP_CHECK_GWTYPE(CfgPtr,ComRxSignalType,
        GWSourcePtr->ComGWSourceRef, 0);

  if (NULL_PTR == SrcSignalPtr)
  {
     RetVal = E_NOT_OK;
     COM_PRECONDITION_ASSERT_NO_EVAL(
     (NULL_PTR != SrcSignalPtr),
     COM_INTERNAL_API_ID);
  }
  else if (SrcSignalPtr->ComConfigCheckData != COM_CONFIG_CHECK_RX_SIGNAL)
  {
     RetVal = E_NOT_OK;
     COM_PRECONDITION_ASSERT_NO_EVAL(
     (SrcSignalPtr->ComConfigCheckData == COM_CONFIG_CHECK_RX_SIGNAL),
     COM_INTERNAL_API_ID);
  }
  else
  {
     RetVal = Com_EB_CheckAllRouteSignals_GWDestinationSigRef(GWSourcePtr, CfgPtr);
  }

  return RetVal;
}

_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckAllRouteSignals_GWDestinationSigRef
(
   P2CONST(ComGWSourceType, AUTOMATIC, COM_APPL_CONST) GWSourcePtr,
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
)
{
   /* To ensure that combining of retvals in calling function works, if not entering loop */
   Std_ReturnType RetVal = E_OK;
   ComGWDestinationRefType i; /* iteration variable for destination object */

   /* loop over all destination refs */
   for (i = GWSourcePtr->ComGWDestinationFirstRef;
            (i < (GWSourcePtr->ComGWDestinationFirstRef + GWSourcePtr->ComGWDestinationNumRef));
            i++)
   {
      CONSTP2CONST(ComGWDestinationType, AUTOMATIC, COM_APPL_CONST) GWDestinationPtr =
               COM_GET_CONFIG_ELEMENT_TEMP_CHECK(CfgPtr,ComGWDestinationType,
                        (CfgPtr->ComGWDestinationRef), i);

      if (NULL_PTR == GWDestinationPtr)
      {
         RetVal = E_NOT_OK;
         COM_PRECONDITION_ASSERT_NO_EVAL(
                  (NULL_PTR != GWDestinationPtr),
                  COM_INTERNAL_API_ID);
      }
      else if (GWDestinationPtr->ComConfigCheckData != COM_CONFIG_CHECK_GW_DESTINATION)
      {
         RetVal = E_NOT_OK;
         COM_PRECONDITION_ASSERT_NO_EVAL(
                  (GWDestinationPtr->ComConfigCheckData == COM_CONFIG_CHECK_GW_DESTINATION),
                  COM_INTERNAL_API_ID);
      }
      else if ((GWDestinationPtr->ComGWDestinationSelect != COM_GW_SIGNAL) &&
               (GWDestinationPtr->ComGWDestinationSelect != COM_GW_GROUP_SIGNAL))
      {
         RetVal = E_NOT_OK;
         COM_PRECONDITION_ASSERT_NO_EVAL(
                  ((GWDestinationPtr->ComGWDestinationSelect == COM_GW_SIGNAL) ||
                           (GWDestinationPtr->ComGWDestinationSelect == COM_GW_GROUP_SIGNAL)),
                  COM_INTERNAL_API_ID);
      }
      else
      {
         RetVal |= Com_EB_Check_DestSignalRef(GWDestinationPtr,CfgPtr);
      }
   }

   return RetVal;
}

_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_Check_DestSignalRef
(
  P2CONST(ComGWDestinationType, AUTOMATIC, COM_APPL_CONST) GWDestinationPtr,
  P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  CONSTP2CONST(ComTxSignalType, AUTOMATIC, COM_APPL_CONST) DestSignalPtr =
        COM_GET_CONFIG_ELEMENT_TEMP_CHECK_GWTYPE(CfgPtr,ComTxSignalType,
              GWDestinationPtr->ComGWDestinationRef, 0U);

  /* Unused in case of TS_RELOCATABLE_CFG_ENABLE == STD_OFF */
  TS_PARAM_UNUSED(CfgPtr);

  if (NULL_PTR == DestSignalPtr)
  {
     COM_PRECONDITION_ASSERT_NO_EVAL(
     (NULL_PTR != DestSignalPtr),
     COM_INTERNAL_API_ID);
  }
  else if (DestSignalPtr->ComConfigCheckData != COM_CONFIG_CHECK_TX_SIGNAL)
  {
     COM_PRECONDITION_ASSERT_NO_EVAL(
     (DestSignalPtr->ComConfigCheckData == COM_CONFIG_CHECK_TX_SIGNAL),
     COM_INTERNAL_API_ID);
  }
  else
  {
    RetVal = E_OK;
  }
  return RetVal;
}

_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckRouteSignalGroup_GWSourceRefs
(
  P2CONST(ComGWSourceType, AUTOMATIC, COM_APPL_CONST) GWSourcePtr,
  P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
)
{
  Std_ReturnType RetVal;

  CONSTP2CONST(ComRxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SrcSignalGPtr =
        COM_GET_CONFIG_ELEMENT_TEMP_CHECK_GWTYPE(CfgPtr, ComRxSignalGroupType,
              GWSourcePtr->ComGWSourceRef, 0U);

  if (NULL_PTR == SrcSignalGPtr)
  {
     RetVal = E_NOT_OK;
     COM_PRECONDITION_ASSERT_NO_EVAL(
           (NULL_PTR != SrcSignalGPtr),
           COM_INTERNAL_API_ID);
  }
  else if (SrcSignalGPtr->ComConfigCheckData != COM_CONFIG_CHECK_RX_SIG_GROUP)
  {
     RetVal = E_NOT_OK;
     COM_PRECONDITION_ASSERT_NO_EVAL(
           (SrcSignalGPtr->ComConfigCheckData == COM_CONFIG_CHECK_RX_SIG_GROUP),
           COM_INTERNAL_API_ID);
  }
  else
  {
     RetVal = Com_EB_CheckAllRouteSignals_GWDestinationSigGRef(GWSourcePtr, CfgPtr);
  }

  return RetVal;
}

_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckRouteGroupSignal_GWSourceRefs
(
  P2CONST(ComGWSourceType, AUTOMATIC, COM_APPL_CONST) GWSourcePtr,
  P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
)
{
  Std_ReturnType RetVal;

  RetVal = Com_EB_CheckRouteSignal_GWSourceRefs(GWSourcePtr, CfgPtr);

  return RetVal;
}

_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_CheckAllRouteSignals_GWDestinationSigGRef
(
   P2CONST(ComGWSourceType, AUTOMATIC, COM_APPL_CONST) GWSourcePtr,
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
)
{
  /* To ensure that combining of retvals in calling function works, if not entering loop */
   Std_ReturnType RetVal = E_OK;
   ComGWDestinationRefType i; /* iteration variable for destination object */

   /* loop over all destination refs */
   for (i = GWSourcePtr->ComGWDestinationFirstRef;
         (i < (GWSourcePtr->ComGWDestinationFirstRef + GWSourcePtr->ComGWDestinationNumRef));
         i++)
   {
      CONSTP2CONST(ComGWDestinationType, AUTOMATIC, COM_APPL_CONST) GWDestinationPtr =
            COM_GET_CONFIG_ELEMENT_TEMP_CHECK(CfgPtr,ComGWDestinationType,
                              (CfgPtr->ComGWDestinationRef), i);

      if (NULL_PTR == GWDestinationPtr)
      {
         RetVal = E_NOT_OK;
         COM_PRECONDITION_ASSERT_NO_EVAL(
               (NULL_PTR != GWDestinationPtr),
               COM_INTERNAL_API_ID);
      }
      else if (GWDestinationPtr->ComConfigCheckData != COM_CONFIG_CHECK_GW_DESTINATION)
      {
         RetVal = E_NOT_OK;
         COM_PRECONDITION_ASSERT_NO_EVAL(
               (GWDestinationPtr->ComConfigCheckData == COM_CONFIG_CHECK_GW_DESTINATION),
               COM_INTERNAL_API_ID);
      }
      else if (GWDestinationPtr->ComGWDestinationSelect != COM_GW_SIGNAL_GROUP)
      {
         RetVal = E_NOT_OK;
         COM_PRECONDITION_ASSERT_NO_EVAL(
               (GWDestinationPtr->ComGWDestinationSelect == COM_GW_SIGNAL_GROUP),
               COM_INTERNAL_API_ID);
      }
      else
      {
        RetVal |= Com_EB_Check_DestSignalGRef(GWDestinationPtr,CfgPtr);
      }
   }

   return RetVal;
}

_COM_STATIC_ FUNC(Std_ReturnType, COM_CODE) Com_EB_Check_DestSignalGRef
(
  P2CONST(ComGWDestinationType, AUTOMATIC, COM_APPL_CONST) GWDestinationPtr,
  P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) CfgPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  CONSTP2CONST(ComTxSignalGroupType, AUTOMATIC, COM_APPL_CONST) DestSignalGPtr =
        COM_GET_CONFIG_ELEMENT_TEMP_CHECK_GWTYPE(CfgPtr,ComTxSignalGroupType,
              GWDestinationPtr->ComGWDestinationRef, 0U);

  /* Unused in case of TS_RELOCATABLE_CFG_ENABLE == STD_OFF */
  TS_PARAM_UNUSED(CfgPtr);

  if (NULL_PTR == DestSignalGPtr)
  {
     COM_PRECONDITION_ASSERT_NO_EVAL(
           (NULL_PTR != DestSignalGPtr),
           COM_INTERNAL_API_ID);
  }
  else if (DestSignalGPtr->ComConfigCheckData != COM_CONFIG_CHECK_TX_SIG_GROUP)
  {
     COM_PRECONDITION_ASSERT_NO_EVAL(
           (DestSignalGPtr->ComConfigCheckData == COM_CONFIG_CHECK_TX_SIG_GROUP),
           COM_INTERNAL_API_ID);
  }
  else
  {
    RetVal = E_OK;
  }
  return RetVal;
}

#endif

#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>
    /* CHECK: PARSE */
#endif /* (COM_PRECONDITION_ASSERT_ENABLED == STD_ON) */

/*==================[end of file]============================================*/

