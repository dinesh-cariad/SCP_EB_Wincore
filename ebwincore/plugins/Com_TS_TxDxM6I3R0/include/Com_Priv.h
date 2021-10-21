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


#ifndef COM_PRIV_H
#define COM_PRIV_H

/*==================[inclusions]============================================*/

#include <TSAutosar.h>          /* EB specific standard types */
#include <Com_Api.h>                /* get module interface */
#include <Com_Lcfg_Static.h>    /* declaration of the callback arrays */
#include <Com_Version.h>        /* declaration of COM_MODULE_ID */
#define TS_RELOCATABLE_CFG_ENABLE COM_RELOCATABLE_CFG_ENABLE
#include <TSPBConfig_Access.h>  /* Post Build Config Helpers */
#undef TS_RELOCATABLE_CFG_ENABLE

/*==================[version check]=========================================*/

/*==================[macros]================================================*/

#if (COM_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>                /* get development error tracer interface */

#if (defined COM_DET_REPORTERROR)        /* To prevent double declaration */
#error COM_DET_REPORTERROR is already defined
#endif /* (defined COM_DET_REPORTERROR) */

/** \brief Define COM_DET_REPORTERROR
 * macro for simplified DET usage - map to Det - API function call
 */
#define COM_DET_REPORTERROR(nErrorID, nFuncID) \
    COM_DET_REPORTERROR_HLP(nErrorID, nFuncID)

#if (defined COM_DET_REPORTERROR_HLP)        /* To prevent double declaration */
#error COM_DET_REPORTERROR_HLP is already defined
#endif /* (defined COM_DET_REPORTERROR_HLP) */

#define COM_DET_REPORTERROR_HLP(nErrorID, nFuncID)      \
    (void)Det_ReportError(                              \
        ((uint16) COM_MODULE_ID),                       \
        ((uint8) 0U),                                   \
        ((uint8) (nFuncID)),                            \
        ((uint8) (nErrorID))                            \
        )

#endif /* (COM_DEV_ERROR_DETECT == STD_ON) */


/** \brief Define COM_GET_CONFIG_ADDR
 * macro for getting a reference to a config element
 */

#if (defined COM_GET_CONFIG_ADDR_NOCHK)        /* To prevent double declaration */
#error COM_GET_CONFIG_ADDR_NOCHK is already defined
#endif /* (defined COM_GET_CONFIG_ADDR_NOCHK) */

/* there are MISRA deviations in TS_UNCHECKEDGETCFG */
#define COM_GET_CONFIG_ADDR_NOCHK(type, offset) \
TS_UNCHECKEDGETCFG(Com_gConfigPtr, type, COM, offset)

#if (defined COM_GET_CONFIG_ADDR_CHK)        /* To prevent double declaration */
#error COM_GET_CONFIG_ADDR_CHK is already defined
#endif /* (defined COM_GET_CONFIG_ADDR_CHK) */

/* there are MISRA deviations in TS_CHECKEDGETCFG */
#define COM_GET_CONFIG_ADDR_CHK(type, offset) \
TS_CHECKEDGETCFG(Com_gConfigPtr, type, COM, offset)

#if (defined COM_GET_CONFIG_ADDR_TEMP_CHK)        /* To prevent double declaration */
#error COM_GET_CONFIG_ADDR_TEMP_CHK is already defined
#endif /* (defined COM_GET_CONFIG_ADDR_TEMP_CHK) */

#define COM_GET_CONFIG_ADDR_TEMP_CHK(tempCfgPtr, type, offset) \
TS_CHECKEDGETCFG(tempCfgPtr, type, COM, offset)


#if (COM_CONST_CFG_ADDRESS_ENABLE == STD_ON)

#if (defined Com_gConfigPtr)        /* To prevent double declaration */
#error Com_gConfigPtr is already defined
#endif /* (defined Com_gConfigPtr) */

/** \brief Define Com_gConfigPtr
 * root config pointer is replaced by absolute address that is given via
 * pre-compile-time switch COM_CONST_CONFIG_ADDRESS
 */
#define Com_gConfigPtr \
    ((P2CONST(Com_ConfigType, COM_VAR_CLEARED, COM_APPL_CONST)) \
    COM_CONST_CFG_ADDRESS)
#endif /* COM_CONST_CFG_ADDRESS_ENABLE */


#if (defined COM_GET_MEMPTR)        /* To prevent double declaration */
#error COM_GET_MEMPTR is already defined
#endif /* (defined COM_GET_MEMPTR) */

/** \brief COM_GET_MEMPTR
 * Provides a pointer of type \p type to the Com memory
 * \param[in] type defines the type of the provided pointer
 * \param[in] offset in Com memory in bytes
 */
#define COM_GET_MEMPTR(type, offset)                                           \
    ( (P2VAR(type, AUTOMATIC, COM_VAR_CLEARED))                                 \
      ( (P2VAR(void, AUTOMATIC, COM_VAR_CLEARED)) &Com_gDataMemPtr[(offset)] )  \
    )

#if (defined COM_GET_Com_MainFunctionRxIdx)        /* To prevent double declaration */
#error COM_GET_Com_MainFunctionRxIdx is already defined
#endif /* (defined COM_GET_Com_MainFunctionRxIdx) */

#if (COM_TIMEBASE == STD_ON)
#define COM_GET_Com_MainFunctionRxIdx(IPduPtr)
#else
#define COM_GET_Com_MainFunctionRxIdx(IPduPtr) (IPduPtr)->ComMainFunctionsRx_Idx
#endif

#if (defined COM_GET_Com_MainFunctionTxIdx)        /* To prevent double declaration */
#error COM_GET_Com_MainFunctionTxIdx is already defined
#endif /* (defined COM_GET_Com_MainFunctionTxIdx) */

#if (COM_TIMEBASE == STD_ON)
#define COM_GET_Com_MainFunctionTxIdx(IPduPtr)
#else
#define COM_GET_Com_MainFunctionTxIdx(IPduPtr) (IPduPtr)->ComMainFunctionsTx_Idx
#endif

#if (defined COM_GET_Com_GlobalRxTime)        /* To prevent double declaration */
#error COM_GET_Com_GlobalRxTime(idx) is already defined
#endif /* (defined COM_GET_Com_GlobalRxTime(idx)) */

#if (COM_TIMEBASE == STD_ON)
#define COM_GET_Com_GlobalRxTime(idx) Com_GlobalTime
#else
#define COM_GET_Com_GlobalRxTime(idx) Com_GlobalTimes_Rx[(idx)]
#endif

#if (defined COM_GET_Com_GlobalTxTime)        /* To prevent double declaration */
#error COM_GET_Com_GlobalTxTime is already defined
#endif /* (defined COM_GET_Com_GlobalTxTime) */

#if (COM_TIMEBASE == STD_ON)
#define COM_GET_Com_GlobalTxTime(idx) Com_GlobalTime
#else
#define COM_GET_Com_GlobalTxTime(idx) Com_GlobalTimes_Tx[(idx)]
#endif

#if (defined COM_TXPDU_STARTED) /* To prevent double declaration */
#error COM_TXPDU_STARTED already defined
#endif /* if (defined COM_TXPDU_STARTED) */

/** \brief Define COM_TXPDU_STARTED */
#define COM_TXPDU_STARTED 0U

#if (defined COM_TXPDU_MDTQUEUED)       /* To prevent double declaration */
#error COM_TXPDU_MDTQUEUED already defined
#endif /* if (defined COM_TXPDU_MDTQUEUED) */

/** \brief Define COM_TXPDU_MDTQUEUED */
#define COM_TXPDU_MDTQUEUED 1U

#if (defined COM_TXPDU_PERIODICSENDQUEUED)      /* To prevent double declaration */
#error COM_TXPDU_PERIODICSENDQUEUED already defined
#endif /* if (defined COM_TXPDU_PERIODICSENDQUEUED) */

/** \brief Define COM_TXPDU_PERIODICSENDQUEUED */
#define COM_TXPDU_PERIODICSENDQUEUED 2U

#if (defined COM_TXPDU_NTIMESSENDQUEUED)        /* To prevent double declaration */
#error COM_TXPDU_NTIMESSENDQUEUED already defined
#endif /* if (defined COM_TXPDU_NTIMESSENDQUEUED) */

/** \brief Define COM_TXPDU_NTIMESSENDQUEUED */
#define COM_TXPDU_NTIMESSENDQUEUED 3U

#if (defined COM_TXPDU_SENDIMMEDIATELY) /* To prevent double declaration */
#error COM_TXPDU_SENDIMMEDIATELY already defined
#endif /* if (defined COM_TXPDU_SENDIMMEDIATELY) */

/** \brief Define COM_TXPDU_SENDIMMEDIATELY */
#define COM_TXPDU_SENDIMMEDIATELY 4U

#if (defined COM_TXPDU_TXDMQUEUED)      /* To prevent double declaration */
#error COM_TXPDU_TXDMQUEUED already defined
#endif /* if (defined COM_TXPDU_TXDMQUEUED) */

/** \brief Define COM_TXPDU_TXDMQUEUED */
#define COM_TXPDU_TXDMQUEUED 5U

#if (COM_TM_AVAILABLE == STD_ON)
#if (defined COM_TXPDU_TMSSTATE)        /* To prevent double declaration */
#error COM_TXPDU_TMSSTATE already defined
#endif /* if (defined COM_TXPDU_TMSSTATE) */

/** \brief Define COM_TXPDU_TMSSTATE */
#define COM_TXPDU_TMSSTATE 6U

#if (defined COM_TXPDU_TMSCHANGED)  /* To prevent double declaration */
#error COM_TXPDU_TMSCHANGED already defined
#endif /* if (defined COM_TXPDU_TMSCHANGED) */

/** \brief Define COM_TXPDU_TMSCHANGED */
#define COM_TXPDU_TMSCHANGED 7U
#endif /* (COM_TM_AVAILABLE == STD_ON) */


#if (defined COM_RXPDU_STARTED) /* To prevent double declaration */
#error COM_RXPDU_STARTED already defined
#endif /* if (defined COM_RXPDU_STARTED) */

/** \brief Define COM_RXPDU_STARTED */
#define COM_RXPDU_STARTED 0U

#if (defined COM_RXPDU_RXDMRUNNING)     /* To prevent double declaration */
#error COM_RXPDU_RXDMRUNNING already defined
#endif /* if (defined COM_RXPDU_RXDMRUNNING) */

/** \brief Define COM_RXPDU_RXDMRUNNING */
#define COM_RXPDU_RXDMRUNNING 1U

#if (defined COM_RXPDU_RXDMTIMEOUT)     /* To prevent double declaration */
#error COM_RXPDU_RXDMTIMEOUT already defined
#endif /* if (defined COM_RXPDU_RXDMTIMEOUT) */

/** \brief Define COM_RXPDU_RXDMTIMEOUT */
#define COM_RXPDU_RXDMTIMEOUT 2U

#if (defined COM_RXPDU_RXDEFNOTIFY)     /* To prevent double declaration */
#error COM_RXPDU_RXDEFNOTIFY already defined
#endif /* if (defined COM_RXPDU_RXDEFNOTIFY) */

/** \brief Define COM_RXPDU_RXDEFNOTIFY */
#define COM_RXPDU_RXDEFNOTIFY 3U

#if (COM_RX_FIRST_TIMEOUT_FACTOR_SIZE != COM_SIZE_0_BIT)

#if (defined COM_RXPDU_RXDMSWITCHON)     /* To prevent double declaration */
#error COM_RXPDU_RXDMSWITCHON already defined
#endif /* if (defined COM_RXPDU_RXDMSWITCHON) */

/** \brief Define COM_RXPDU_RXDMSWITCHON */
/* indicates that RxDM is switch on */
#define COM_RXPDU_RXDMSWITCHON 4U

#endif /* (COM_RX_FIRST_TIMEOUT_FACTOR_SIZE != COM_SIZE_0_BIT) */

#if (defined COM_CONST_NULL_PTR)        /* To prevent double declaration */
#error COM_CONST_NULL_PTR is already defined
#endif /* (defined COM_CONST_NULL_PTR) */

/** \brief Define COM_CONST_NULL_PTR */
#define COM_CONST_NULL_PTR ((P2CONST(void, AUTOMATIC, COM_APPL_CONST)) 0)

#if (defined COM_INIT_Com_EB_WriteIntDataType_TMSChanged)        /* To prevent double declaration */
#error COM_INIT_Com_EB_WriteIntDataType_TMSChanged is already defined
#endif /* (defined COM_INIT_Com_EB_WriteIntDataType_TMSChanged) */

#if (COM_TMS_ENABLE == STD_ON)
#define COM_INIT_Com_EB_WriteIntDataType_TMSChanged(IntData, TMSChanged_param) \
   do{                                                            \
        (IntData).TMSChanged = (TMSChanged_param);                \
   }while(0);
#else
#define COM_INIT_Com_EB_WriteIntDataType_TMSChanged(IntData, TMSChanged_param)
#endif

#if (defined COM_INIT_Com_EB_WriteIntDataType_RangeCheckPassed)        /* To prevent double declaration */
#error COM_INIT_Com_EB_WriteIntDataType_RangeCheckPassed is already defined
#endif /* (defined COM_INIT_Com_EB_WriteIntDataType_RangeCheckPassed) */

#if (COM_CHECK_VALUE_SIZE == STD_ON)
#define COM_INIT_Com_EB_WriteIntDataType_RangeCheckPassed(IntData, RangeCheckPassed_param) \
   do{                                                                  \
        (IntData).RangeCheckPassed = (RangeCheckPassed_param);          \
   }while(0);
#else
#define COM_INIT_Com_EB_WriteIntDataType_RangeCheckPassed(IntData, RangeCheckPassed_param)
#endif

#if (defined COM_INIT_Com_EB_WriteIntDataType)        /* To prevent double declaration */
#error COM_INIT_Com_EB_WriteIntDataType is already defined
#endif /* (defined COM_INIT_Com_EB_WriteIntDataType) */

#define COM_INIT_Com_EB_WriteIntDataType(IntData, arraySize_param, trigger_param, SendSigRetval_param, \
                                         SendInMainFunctionTx_param, TMSChanged_param, RangeCheckPassed_param) \
   do{                                                                                             \
        (IntData).arraySize = (arraySize_param);                                                   \
        (IntData).trigger = (trigger_param);                                                       \
        (IntData).SendSigRetval = (SendSigRetval_param);                                           \
        (IntData).SendInMainFunctionTx = (SendInMainFunctionTx_param);                             \
        COM_INIT_Com_EB_WriteIntDataType_TMSChanged((IntData), (TMSChanged_param))                 \
        COM_INIT_Com_EB_WriteIntDataType_RangeCheckPassed((IntData), (RangeCheckPassed_param))     \
   }while(0)

/*==================[type definitions]======================================*/

typedef struct
{
   /* size of a UINT8_N/_DYN signal */
   ComSignalBitSizeType arraySize;

   /* defines if Pdu shall be triggered */
   uint8 trigger;

   /* retval for SendSignal() */
   uint8 SendSigRetval;

   /* defines if the signal shall be sent immediately or within the next Com_MainFunctionTx() */
   boolean SendInMainFunctionTx;

#if (COM_TMS_ENABLE == STD_ON)
   /* defines the TM has changed */
   boolean TMSChanged;
#endif

#if (COM_CHECK_VALUE_SIZE == STD_ON)
   uint8 RangeCheckPassed;
#endif /* (COM_CHECK_VALUE_SIZE == STD_ON) */

} Com_EB_WriteIntDataType;


typedef struct
{
   /** \brief pointer to the signal group array  */
   P2VAR(uint8, AUTOMATIC, AUTOSAR_COMSTACKDATA) SGArrayPtr;
   /** \brief pointer where the number of bytes copied shall be stored  */
   P2VAR(uint16, AUTOMATIC, AUTOSAR_COMSTACKDATA) outSGArrayLengthPtr;
   /** \brief length signal group array buffer */
   uint16 inSGArrayLength;
} Com_RxSGArrayInfoType;

typedef FUNC(void, COM_CODE) (Com_RxSignalGroupFPtrType)
(
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr,
   P2VAR(Com_RxSGArrayInfoType, AUTOMATIC, COM_APPL_DATA) Com_RxSGArrayInfoPtr,
   P2CONST(ComRxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr,
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   ComIPduRefType PduId
);

typedef struct Com_Emu_64 ComEmu64Type;

struct Com_Emu_64
{
#if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
  uint32 HiWord;
  uint32 LoWord;
#else
  uint32 LoWord;
  uint32 HiWord;
#endif
};

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/


#define COM_START_SEC_VAR_INIT_8BIT
#include <Com_MemMap.h>

/** \brief Com_InitStatus
 * global variable for the actual initialization status of the Com */
extern VAR(Com_EB_StatusType, COM_VAR) Com_InitStatus;

#define COM_STOP_SEC_VAR_INIT_8BIT
#include <Com_MemMap.h>

#if (COM_GLOBAL_TIME == COM_SIZE_8_BIT)

#define COM_START_SEC_VAR_CLEARED_8BIT
#include <Com_MemMap.h>

/** \brief Com_GlobalTime (8 bit)
 * time in COM specific ticks */
#if (COM_TIMEBASE == STD_ON)
extern VAR(ComGlobalTimeType, COM_VAR_CLEARED) Com_GlobalTime;
#else
extern VAR(ComGlobalTimeType_Tx, COM_VAR_CLEARED) Com_GlobalTimes_Tx;
extern VAR(ComGlobalTimeType_Rx, COM_VAR_CLEARED) Com_GlobalTimes_Rx;
#endif /* (COM_TIMEBASE == STD_ON) */

#define COM_STOP_SEC_VAR_CLEARED_8BIT
#include <Com_MemMap.h>

#endif /* (COM_GLOBAL_TIME == COM_SIZE_8_BIT) */


#if (COM_GLOBAL_TIME == COM_SIZE_16_BIT)

#define COM_START_SEC_VAR_CLEARED_16BIT
#include <Com_MemMap.h>

/** \brief Com_GlobalTime (16 bit)
 * time in COM specific ticks */
#if (COM_TIMEBASE == STD_ON)
extern VAR(ComGlobalTimeType, COM_VAR_CLEARED) Com_GlobalTime;
#else
extern VAR(ComGlobalTimeType_Tx, COM_VAR_CLEARED) Com_GlobalTimes_Tx;
extern VAR(ComGlobalTimeType_Rx, COM_VAR_CLEARED) Com_GlobalTimes_Rx;
#endif /* (COM_TIMEBASE == STD_ON) */

#define COM_STOP_SEC_VAR_CLEARED_16BIT
#include <Com_MemMap.h>

#endif /* (COM_GLOBAL_TIME == COM_SIZE_16_BIT) */


#if (COM_GLOBAL_TIME == COM_SIZE_32_BIT)

#define COM_START_SEC_VAR_CLEARED_32BIT
#include <Com_MemMap.h>

/** \brief Com_GlobalTime (32 bit)
 * time in COM specific ticks */
#if (COM_TIMEBASE == STD_ON)
extern VAR(ComGlobalTimeType, COM_VAR_CLEARED) Com_GlobalTime;
#else
extern VAR(ComGlobalTimeType_Tx, COM_VAR_CLEARED) Com_GlobalTimes_Tx;
extern VAR(ComGlobalTimeType_Rx, COM_VAR_CLEARED) Com_GlobalTimes_Rx;
#endif /* (COM_TIMEBASE == STD_ON) */

#define COM_STOP_SEC_VAR_CLEARED_32BIT
#include <Com_MemMap.h>

#endif /* (COM_GLOBAL_TIME == COM_SIZE_32_BIT) */


/*
 * if not a constant configuration address is used - define pointer variable
 */
#if (COM_CONST_CFG_ADDRESS_ENABLE == STD_OFF)


#define COM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Com_MemMap.h>

/* CHECK: RULE 506 OFF */
/* Check_C disabled. Com_gConfigPtr is not renamed because of readability. */
/** \brief Com_gConfigPtr
 * global variable for the pointer to the config of Com
 */
extern P2CONST(Com_ConfigType, COM_VAR_CLEARED, COM_APPL_CONST) Com_gConfigPtr;
/* CHECK: RULE 506 ON */


#define COM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Com_MemMap.h>

#endif /* COM_CONST_CFG_ADDRESS_ENABLE == STD_OFF */


/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/


#define COM_START_SEC_CODE
#include <Com_MemMap.h>


#if ((COM_FILTER_RECEIVER_ENABLE == STD_ON) || \
      ((COM_TMS_ENABLE == STD_ON) && (COM_SIGNAL_GW_ENABLE == STD_ON)))

TS_MOD_PRIV_DECL FUNC(boolean, COM_CODE) Com_EB_CalculateFilter
(
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComFilterConfType, AUTOMATIC, AUTOMATIC) ComFilterPropPtr
);

#endif /* ((COM_FILTER_RECEIVER_ENABLE == STD_ON) || \
      ((COM_TMS_ENABLE == STD_ON) && (COM_SIGNAL_GW_ENABLE == STD_ON))) */


#if ((COM_RX_SIGNAL_BUFFER_AVAILABLE == STD_ON) || \
      (COM_TX_SIGNAL_BUFFER_AVAILABLE == STD_ON))

/** \brief Com_EB_SetBuffer - sets the signal buffer with the value of the signal
 * in the I-Pdu
 * Preconditions:
 * - prior to this call, the COM module shall be initialized
 * \param[in] Com_EB_ReadPropPtr - properties of the signal
 */
TS_MOD_PRIV_DECL FUNC(void, COM_CODE) Com_EB_SetBuffer
(
   P2CONST(ComSignalBaseType, AUTOMATIC, COM_APPL_CONST) Com_EB_ReadPropPtr
);

#endif /* ((COM_RX_SIGNAL_BUFFER_AVAILABLE == STD_ON) || \
      (COM_TX_SIGNAL_BUFFER_AVAILABLE == STD_ON)) */


#if (COM_FILTER_ONE_EVERY_N_AVAILABLE == STD_ON)
/** \brief Com_ResetFilerOneEveryN - resets the occurrence value of a
 * OneEveryN filter
 * Preconditions:
 * - prior to this call, the COM module shall be initialized
 * - ComFilterRef has to address a ComOneEveryN configuration
 * \param[in] ComFilterRef - Filter which shall be reseted
 */

TS_MOD_PRIV_DECL FUNC(void, COM_CODE) Com_ResetFilerOneEveryN
(
   ComFilterRefType ComFilterRef
);
#endif /* (COM_FILTER_ONE_EVERY_N_AVAILABLE == STD_ON) */


extern FUNC(void, COM_CODE) Com_EB_Read_SignalExt
(
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
);

extern FUNC(void, COM_CODE) Com_EB_Write_SignalExt
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);

TS_MOD_PRIV_DECL FUNC(void, COM_CODE) Com_SetupRxSignalGroup
(
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr,
   P2VAR(Com_RxSGArrayInfoType, AUTOMATIC, COM_APPL_DATA) RxSGArrayInfoPtr,
   P2CONST(ComRxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr,
   P2VAR(Com_RxSignalGroupFPtrType, AUTOMATIC, COM_CODE) Com_RxSignalGroupFPtr
);


/** \brief Com_CopyRxSignalGroupArray_Shadow - copies the signal group array to
 * the I-Pdu buffer from a shadow buffer
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * \param[out] retvalPtr - reference where the return vale shall be stored
 *    E_OK - service has been accepted
 *    COM_SERVICE_NOT_AVAILABLE - corresponding I-PDU group was stopped
 *    (or service failed due to development error)
 * \param[out] SGArrayPtr - not used
 * \param[in] SGArrayLengthPtr - not used
 * \param[in] SignalGPtr - reference to the configuration of the signal group
 * \param[in] IPduPtr - reference to the configuration of the I-Pdu
 * to which the signal group belongs to
 * \param[in] PduId - Id of the I-Pdu
 * to which the signal group belongs to
 */
TS_MOD_PRIV_DECL FUNC(void, COM_CODE) Com_CopyRxSignalGroupArray_Shadow
(
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr,
   P2VAR(Com_RxSGArrayInfoType, AUTOMATIC, COM_APPL_DATA) RxSGArrayInfoType,
   P2CONST(ComRxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr,
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   ComIPduRefType PduId
);

#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>

#endif /* ifndef COM_PRIV_H */
/*==================[end of file]===========================================*/
