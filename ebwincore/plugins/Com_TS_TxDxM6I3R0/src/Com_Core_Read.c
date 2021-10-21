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
 * MISRAC2012-1) Deviated Rule: 10.8 (required)
 * The value of a composite expression shall not be
 * cast to a different essential type category
 * or a wider essential type.
 *
 * Reason:
 * The msb extraction is implemented through an
 * explicit type cast to a lower essential type
 * category. The truncation of the value is intended.
 *
 * MISRAC2012-2) Deviated Rule: 11.5 (advisory)
 * A conversion should not be performed from pointer to
 * void into pointer to object.
 *
 * Reason:
 * The memory routines are optimized for dealing with aligned
 * memory sections.
 *
 */

/*==================[inclusions]============================================*/

#include <TSAutosar.h>
#include <Com_Core_Read.h>
#include <Com_Cfg.h>
#include <TSMem.h>
#include <Com_Core_Read_hlp.h>
#include <Com_Rules.h>

#if (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON)
#include <Com_Core_Write.h>
#if (COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL)
#include <Com_Core_RxUpdateBit.h>
#endif
#endif /* (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON) */

/*==================[macros]================================================*/

#if (defined COM_EB_READ_SET_TABLE)        /* To prevent double declaration */
#error COM_EB_READ_SET_TABLE already defined
#endif /* #if (defined COM_EB_READ_SET_TABLE) */

/** \brief COM_EB_READ_SET_TABLE
 * Initializes a member of the function pointer array for read functions
 * \param[in] read - function for reading a signal
 * \param[in] isRx - function for checking if a signal is received
 * \param[in] copy - function for copying a signal from an external buffer to Rx-Pdu buffer
 */
#if (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON)
#define COM_EB_READ_SET_TABLE(read, isRx, copy) {(read), (isRx), (copy)}
#elif ((COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_OFF) && (COM_HANDLESMALLERRXPDUS_NOTIFY == STD_ON))
#define COM_EB_READ_SET_TABLE(read, isRx, copy) {(read), (isRx)}
#elif ((COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_OFF) && (COM_HANDLESMALLERRXPDUS_NOTIFY == STD_OFF))
#define COM_EB_READ_SET_TABLE(read, isRx, copy) {(read)}
#else
#error Unknown combination
#endif


#if (defined Com_EBWriteIdx_08_SX_DL_DUB_DOC_E1)        /* To prevent double declaration */
#error Com_EBWriteIdx_08_SX_DL_DUB_DOC_E1 already defined
#endif /* #if (defined Com_EBWriteIdx_08_SX_DL_DUB_DOC_E1) */

/** \brief definition Com_EBWriteIdx_08_SX_DL_DUB_DOC_E1
 * Index in write function table for setting value of 8 bit little endian signals
 */
#define Com_EBWriteIdx_08_SX_DL_DUB_DOC_E1 0U


#if (defined Com_EBWriteIdx_16_SX_DL_DUB_DOC_E1)        /* To prevent double declaration */
#error Com_EBWriteIdx_16_SX_DL_DUB_DOC_E1 already defined
#endif /* #if (defined Com_EBWriteIdx_16_SX_DL_DUB_DOC_E1) */

/** \brief definition Com_EBWriteIdx_16_SX_DL_DUB_DOC_E1
 * Index in write function table for setting value of 16 bit little endian signals
 */
#define Com_EBWriteIdx_16_SX_DL_DUB_DOC_E1 1U


#if (defined Com_EBWriteIdx_32_SX_DL_DUB_DOC_E1)        /* To prevent double declaration */
#error Com_EBWriteIdx_32_SX_DL_DUB_DOC_E1 already defined
#endif /* #if (defined Com_EBWriteIdx_32_SX_DL_DUB_DOC_E1) */

/** \brief definition Com_EBWriteIdx_32_SX_DL_DUB_DOC_E1
 * Index in write function table for setting value of 32 bit little endian signals
 */
#define Com_EBWriteIdx_32_SX_DL_DUB_DOC_E1 2U


#if (defined Com_EBWriteIdx_64_SX_DL_DUB_DOC_E1)        /* To prevent double declaration */
#error Com_EBWriteIdx_64_SX_DL_DUB_DOC_E1 already defined
#endif /* #if (defined Com_EBWriteIdx_64_SX_DL_DUB_DOC_E1) */

/** \brief definition Com_EBWriteIdx_64_SX_DL_DUB_DOC_E1
 * Index in write function table for setting value of 64 bit little endian signals
 */
#define Com_EBWriteIdx_64_SX_DL_DUB_DOC_E1 131U


#if (defined Com_EBWriteIdx_08_SX_DL_DUB_DOC_E2)        /* To prevent double declaration */
#error Com_EBWriteIdx_08_SX_DL_DUB_DOC_E2 already defined
#endif /* #if (defined Com_EBWriteIdx_08_SX_DL_DUB_DOC_E2) */

/** \brief definition Com_EBWriteIdx_08_SX_DL_DUB_DOC_E2
 * Index in write function table for setting value of 8 bit big endian signals
 */
#define Com_EBWriteIdx_08_SX_DL_DUB_DOC_E2 8U


#if (defined Com_EBWriteIdx_16_SX_DL_DUB_DOC_E2)        /* To prevent double declaration */
#error Com_EBWriteIdx_16_SX_DL_DUB_DOC_E2 already defined
#endif /* #if (defined Com_EBWriteIdx_16_SX_DL_DUB_DOC_E2) */

/** \brief definition Com_EBWriteIdx_16_SX_DL_DUB_DOC_E2
 * Index in write function table for setting value of 16 bit big endian signals
 */
#define Com_EBWriteIdx_16_SX_DL_DUB_DOC_E2 9U


#if (defined Com_EBWriteIdx_32_SX_DL_DUB_DOC_E2)        /* To prevent double declaration */
#error Com_EBWriteIdx_32_SX_DL_DUB_DOC_E2 already defined
#endif /* #if (defined Com_EBWriteIdx_32_SX_DL_DUB_DOC_E2) */

/** \brief definition Com_EBWriteIdx_32_SX_DL_DUB_DOC_E2
 * Index in write function table for setting value of 32 bit big endian signals
 */
#define Com_EBWriteIdx_32_SX_DL_DUB_DOC_E2 10U


#if (defined Com_EBWriteIdx_64_SX_DL_DUB_DOC_E2)        /* To prevent double declaration */
#error Com_EBWriteIdx_64_SX_DL_DUB_DOC_E2 already defined
#endif /* #if (defined Com_EBWriteIdx_64_SX_DL_DUB_DOC_E2) */

/** \brief definition Com_EBWriteIdx_32_SX_DL_DUB_DOC_64
 * Index in write function table for setting value of 64 bit big endian signals
 */
#define Com_EBWriteIdx_64_SX_DL_DUB_DOC_E2 139U


#if (defined COM_EB_CREATE_TX_SIG_CONF_FROM_RX_SET_TX_BIT_POS_E1)        /* To prevent double declaration */
#error COM_EB_CREATE_TX_SIG_CONF_FROM_RX_SET_TX_BIT_POS_E1 already defined
#endif /* #if (defined COM_EB_CREATE_TX_SIG_CONF_FROM_RX_SET_TX_BIT_POS_E1) */

/** \brief COM_EB_CREATE_TX_SIG_CONF_FROM_RX_SET_TX_BIT_POS_E1
 * calculates Tx bit Pos (ComSignalPositionInBit)
 * for little endian signal from ComSignalBase (which is Rx configuration)
 * \param[in/out] hlpSignal - variable for a Tx signals where ComSignalBase is configured and
 *                            ComSignalPositionInBit shall be updated
 */
#define COM_EB_CREATE_TX_SIG_CONF_FROM_RX_SET_TX_BIT_POS_E1(hlpSignal)                                             \
do{                                                                                                                \
    (hlpSignal).ComSignalPositionInBit = (ComSignalDescBitType)((hlpSignal).ComSignalBase.ComSignalPositionInBit   \
                                         + 1U - (hlpSignal).ComSignalBase.ComBitSize);                             \
}while(0)


#if (defined COM_EB_CREATE_TX_SIG_CONF_FROM_RX_SET_TX_BIT_POS_E2)        /* To prevent double declaration */
#error COM_EB_CREATE_TX_SIG_CONF_FROM_RX_SET_TX_BIT_POS_E2 already defined
#endif /* #if (defined COM_EB_CREATE_TX_SIG_CONF_FROM_RX_SET_TX_BIT_POS_E2) */

/** \brief COM_EB_CREATE_TX_SIG_CONF_FROM_RX_SET_TX_BIT_POS_E2
 * calculates Tx bit Pos (ComSignalPositionInBit)
 * for little big signal from ComSignalBase (which is Rx configuration)
 * \param[in/out] hlpSignal - variable for a Tx signals where ComSignalBase is configured and
 *                            ComSignalPositionInBit shall be updated
 */
#define COM_EB_CREATE_TX_SIG_CONF_FROM_RX_SET_TX_BIT_POS_E2(hlpSignal)                             \
do{                                                                                                \
    (hlpSignal).ComSignalPositionInBit =  (ComSignalDescBitType) Com_EB_ConvertMSBPos2LSBPos(      \
                                          (hlpSignal).ComSignalBase.ComBitSize,                    \
                                          (hlpSignal).ComSignalBase.ComSignalPositionInBit);       \
}while(0)


#if (defined COM_EB_CREATE_TX_SIG_CONF_FROM_RX_SIG_CHECK_E1)        /* To prevent double declaration */
#error COM_EB_CREATE_TX_SIG_CONF_FROM_RX_SIG_CHECK_E1 already defined
#endif /* #if (defined COM_EB_CREATE_TX_SIG_CONF_FROM_RX_SIG_CHECK_E1) */

/** \brief COM_EB_CREATE_TX_SIG_CONF_FROM_RX_SIG_CHECK_E1
 * Creates a Tx signal configuration from an Rx signal for little endian signal.
 * Checks if the signal is received
 * \param[in/out] hlpSignal - variable where the Tx signal configuration shall be stored
 * \param[in] rxSignalPtr - pointer to Rx signal configuration which shall be used
 *                          to create the Tx signal
 * \param[in] pduLength - length of the external buffer
 * \param[in] bufOf - uses parameter
 * \param[in/out] isRx - variable where TRUE is stored when the signal is received, else FALSE
 */
#define COM_EB_CREATE_TX_SIG_CONF_FROM_RX_SIG_CHECK_E1(hlpSignal, rxSignalPtr, pduLength, bufOf, isRx) \
do{                                                                                                    \
  isRx = Com_EB_ReadReceived_E1((rxSignalPtr), (pduLength));                                           \
  (hlpSignal).ComSignalBase = (rxSignalPtr)->ComSignalBase;                                            \
  COM_EB_CREATE_TX_SIG_CONF_FROM_RX_SET_TX_BIT_POS_E1((hlpSignal));                                    \
  TS_PARAM_UNUSED((bufOf));                                                                            \
}while(0)


#if (defined COM_EB_CREATE_TX_SIG_CONF_FROM_RX_SIG_CHECK_E2)        /* To prevent double declaration */
#error COM_EB_CREATE_TX_SIG_CONF_FROM_RX_SIG_CHECK_E2 already defined
#endif /* #if (defined COM_EB_CREATE_TX_SIG_CONF_FROM_RX_SIG_CHECK_E2) */

/** \brief COM_EB_CREATE_TX_SIG_CONF_FROM_RX_SIG_CHECK_E2
 * Creates a Tx signal configuration from an Rx signal for big endian signal.
 * Checks if the signal is received.
 * \param[in/out] hlpSignal - variable where the Tx signal configuration shall be stored
 * \param[in] rxSignalPtr - pointer to Rx signal configuration which shall be used
 *                          to create the Tx signal
 * \param[in] pduLength - length of the external buffer
 * \param[in] bufOf - uses parameter
 * \param[in/out] isRx - variable where TRUE is stored when the signal is received, else FALSE
 */
#define COM_EB_CREATE_TX_SIG_CONF_FROM_RX_SIG_CHECK_E2(hlpSignal, rxSignalPtr, pduLength, bufOf, isRx) \
do{                                                                                                    \
  isRx = Com_EB_ReadReceived_E2((rxSignalPtr), (pduLength));                                           \
  (hlpSignal).ComSignalBase = (rxSignalPtr)->ComSignalBase;                                            \
  COM_EB_CREATE_TX_SIG_CONF_FROM_RX_SET_TX_BIT_POS_E2((hlpSignal));                                    \
  TS_PARAM_UNUSED((bufOf));                                                                            \
}while(0)


#if (defined COM_EB_CREATE_TX_SIG_CONF_FROM_RX_G_SIG)        /* To prevent double declaration */
#error COM_EB_CREATE_TX_SIG_CONF_FROM_RX_G_SIG already defined
#endif /* #if (defined COM_EB_CREATE_TX_SIG_CONF_FROM_RX_G_SIG) */

/** \brief COM_EB_CREATE_TX_SIG_CONF_FROM_RX_G_SIG
 * Creates a Tx signal ComSignalBase configuration from an Rx group signal.
 * \param[in/out] hlpSignal - variable where the Tx signal configuration shall be stored
 * \param[in] rxSignalPtr - pointer to Rx group signal configuration which shall be used
 *                          to create the Tx signal
 * \param[in] ipduPtr - pointer to Rx-Pdu configuration to which the group signal belongs
 * \param[in] bufOf - offset in bytes of the shadow buffer relative to pdu buffer (value which is
 *                    needed for correction of signal positions)
 */
#define COM_EB_CREATE_TX_SIG_CONF_FROM_RX_G_SIG(hlpSignal, rxSignalPtr, ipduPtr, bufOf)            \
  do{                                                                                              \
    /* copy signal base from Rx signal */                                                          \
    (hlpSignal).ComSignalBase = (rxSignalPtr)->ComSignalBase;                                      \
    (hlpSignal).ComSignalBase.ComBufferRef = (ipduPtr)->ComIPduValueRef;                           \
    (hlpSignal).ComSignalBase.ComSignalPositionInBit =                                             \
        (ComSignalDescBitType)((hlpSignal).ComSignalBase.ComSignalPositionInBit + ((bufOf) * 8U)); \
  }while(0)


#if (defined COM_EB_CREATE_TX_SIG_CONF_FROM_RX_G_SIG_E1)        /* To prevent double declaration */
#error COM_EB_CREATE_TX_SIG_CONF_FROM_RX_G_SIG_E1 already defined
#endif /* #if (defined COM_EB_CREATE_TX_SIG_CONF_FROM_RX_G_SIG_E1) */

/** \brief COM_EB_CREATE_TX_SIG_CONF_FROM_RX_G_SIG_E1
 * Creates a Tx signal configuration from a little endian Rx group signal
 * \param[in/out] hlpSignal - variable where the Tx signal configuration shall be stored
 * \param[in] rxSignalPtr - pointer to Rx group signal configuration which shall be used
 *                          to create the Tx signal
 * \param[in] ipduPtr - pointer to Rx-Pdu configuration to which the group signal belongs
 * \param[in] bufOf - offset in bytes of the shadow buffer relative to pdu buffer (value which is
 *                    needed for correction of signal positions)
 */
#define COM_EB_CREATE_TX_SIG_CONF_FROM_RX_G_SIG_E1(hlpSignal, rxSignalPtr, ipduPtr, bufOf)         \
do{                                                                                                \
    /* Set a destination signal */                                                                 \
    COM_EB_CREATE_TX_SIG_CONF_FROM_RX_G_SIG((hlpSignal), (rxSignalPtr), (ipduPtr), (bufOf));       \
    COM_EB_CREATE_TX_SIG_CONF_FROM_RX_SET_TX_BIT_POS_E1(hlpSignal);                                \
}while(0)


#if (defined COM_EB_CREATE_TX_SIG_CONF_FROM_RX_G_SIG_E2)        /* To prevent double declaration */
#error COM_EB_CREATE_TX_SIG_CONF_FROM_RX_G_SIG_E2 already defined
#endif /* #if (defined COM_EB_CREATE_TX_SIG_CONF_FROM_RX_G_SIG_E2) */

/** \brief COM_EB_CREATE_TX_SIG_CONF_FROM_RX_G_SIG_E2
 * Creates a Tx signal configuration from a big endian Rx group signal
 * \param[in/out] hlpSignal - variable where the Tx signal configuration shall be stored
 * \param[in] rxSignalPtr - pointer to Rx group signal configuration which shall be used
 *                          to create the Tx signal
 * \param[in] ipduPtr - pointer to Rx-Pdu configuration to which the group signal belongs
 * \param[in] bufOf - offset in bytes of the shadow buffer relative to pdu buffer (value which is
 *                    needed for correction of signal positions)
 */
#define COM_EB_CREATE_TX_SIG_CONF_FROM_RX_G_SIG_E2(hlpSignal, rxSignalPtr, ipduPtr, bufOf)         \
do{                                                                                                \
    /* Set a destination signal */                                                                 \
    COM_EB_CREATE_TX_SIG_CONF_FROM_RX_G_SIG((hlpSignal), (rxSignalPtr), (ipduPtr), (bufOf));       \
    COM_EB_CREATE_TX_SIG_CONF_FROM_RX_SET_TX_BIT_POS_E2(hlpSignal);                                \
}while(0)


#if (defined COM_EB_INITDATA)        /* To prevent double declaration */
#error COM_EB_INITDATA already defined
#endif /* #if (defined COM_EB_INITDATA) */

#if (defined COM_EB_INITDATAPTR)        /* To prevent double declaration */
#error COM_EB_INITDATAPTR already defined
#endif /* #if (defined COM_EB_INITDATAPTR) */

#if (COM_CHECK_VALUE_SIZE == STD_ON)
 /** \brief COM_EB_INITDATA
  * Allocates local variable for int data needed for range check which cannot fail in the copy use-case
  * Rational: the bit size of read and write signal has always the same size.
  */
#define COM_EB_INITDATA Com_EB_WriteIntDataType IntData;
 /** \brief COM_EB_INITDATAPTR
  * pointer to IntData for write functions
  */
#define COM_EB_INITDATAPTR &IntData
#else
#define COM_EB_INITDATA
#define COM_EB_INITDATAPTR NULL_PTR
#endif


#if (defined COM_EB_SET_VALUE_IN_RX_BUFFER)        /* To prevent double declaration */
#error COM_EB_SET_VALUE_IN_RX_BUFFER already defined
#endif /* #if (defined COM_EB_SET_VALUE_IN_RX_BUFFER) */

/** \brief COM_EB_SET_VALUE_IN_RX_BUFFER
 * Copy value of signal from external buffer to Pdu buffer
 * \param[in] hlpSignal - variable where the Tx signal configuration derived from Rx signal
 * \param[in] srcValPtr - pointer to external buffer
 * \param[in] writeIdx - ide in write function array which shall be used for writing the signal
 * \param[in] tmpValPtr - pointer to variable where value of signal can be stored
 */
#define COM_EB_SET_VALUE_IN_RX_BUFFER(hlpSignal, srcValPtr, writeIdx, tmpValPtr)                   \
do{                                                                                                \
  /* read value from input buffer */                                                               \
    Com_EB_Read_Core_NoLock_NoSign((tmpValPtr), (srcValPtr), &(hlpSignal).ComSignalBase, NULL_PTR);\
  {                                                                                                \
    /* get Rx I-PDIU buffer */                                                                     \
    CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) DestDataPtr =                                     \
             &Com_gDataMemPtr[(hlpSignal).ComSignalBase.ComBufferRef];                             \
    COM_EB_INITDATA                                                                                \
    /* Write the data to Rx buffer */                                                              \
    COM_WRITEFUNCS[(writeIdx)]((tmpValPtr), DestDataPtr, &(hlpSignal), COM_EB_INITDATAPTR);        \
  }                                                                                                \
}while(0)


#if (defined COM_EB_SET_VALUE_IN_RX_BUFFER_08)        /* To prevent double declaration */
#error COM_EB_SET_VALUE_IN_RX_BUFFER_08 already defined
#endif /* #if (defined COM_EB_SET_VALUE_IN_RX_BUFFER_08) */

/** \brief COM_EB_SET_VALUE_IN_RX_BUFFER_08
 * Copy value of a 8 bit signal from external buffer to Pdu buffer
 * \param[in] hlpSignal - variable where the Tx signal configuration derived from Rx signal
 * \param[in] srcValPtr - pointer to external buffer
 * \param[in] writeIdx - index in write function array which shall be used for writing the signal
 */
#define COM_EB_SET_VALUE_IN_RX_BUFFER_08(hlpSignal, srcValPtr, writeIdx)                          \
do{                                                                                               \
  uint8 valuint8;                                                                                 \
  COM_EB_SET_VALUE_IN_RX_BUFFER((hlpSignal), (srcValPtr), (writeIdx), &valuint8);                 \
}while(0)


#if (defined COM_EB_SET_VALUE_IN_RX_BUFFER_16)        /* To prevent double declaration */
#error COM_EB_SET_VALUE_IN_RX_BUFFER_16 already defined
#endif /* #if (defined COM_EB_SET_VALUE_IN_RX_BUFFER_16) */

/** \brief COM_EB_SET_VALUE_IN_RX_BUFFER_16
 * Copy value of a 16 bit signal from external buffer to Pdu buffer
 * \param[in] hlpSignal - variable where the Tx signal configuration derived from Rx signal
 * \param[in] srcValPtr - pointer to external buffer
 * \param[in] writeIdx - index in write function array which shall be used for writing the signal
 */
#define COM_EB_SET_VALUE_IN_RX_BUFFER_16(hlpSignal, srcValPtr, writeIdx)                           \
do{                                                                                                \
  uint16 valuint16;                                                                                \
  COM_EB_SET_VALUE_IN_RX_BUFFER((hlpSignal), (srcValPtr), (writeIdx), &valuint16);                 \
}while(0)


#if (defined COM_EB_SET_VALUE_IN_RX_BUFFER_32)        /* To prevent double declaration */
#error COM_EB_SET_VALUE_IN_RX_BUFFER_32 already defined
#endif /* #if (defined COM_EB_SET_VALUE_IN_RX_BUFFER_32) */

/** \brief COM_EB_SET_VALUE_IN_RX_BUFFER_32
 * Copy value of a 32 bit signal from external buffer to Pdu buffer
 * \param[in] hlpSignal - variable where the Tx signal configuration derived from Rx signal
 * \param[in] srcValPtr - pointer to external buffer
 * \param[in] writeIdx - index in write function array which shall be used for writing the signal
 */
#define COM_EB_SET_VALUE_IN_RX_BUFFER_32(hlpSignal, srcValPtr, writeIdx)                           \
do{                                                                                                \
  uint32 valuint32;                                                                                \
  COM_EB_SET_VALUE_IN_RX_BUFFER((hlpSignal), (srcValPtr), (writeIdx), &valuint32);                 \
}while(0)


#if (defined COM_EB_SET_VALUE_IN_RX_BUFFER_64)        /* To prevent double declaration */
#error COM_EB_SET_VALUE_IN_RX_BUFFER_64 already defined
#endif /* #if (defined COM_EB_SET_VALUE_IN_RX_BUFFER_64) */

/** \brief COM_EB_SET_VALUE_IN_RX_BUFFER_64
 * Copy value of a 64 bit signal from external buffer to Pdu buffer
 * \param[in] hlpSignal - variable where the Tx signal configuration derived from Rx signal
 * \param[in] srcValPtr - pointer to external buffer
 * \param[in] writeIdx - index in write function array which shall be used for writing the signal
 */
#define COM_EB_SET_VALUE_IN_RX_BUFFER_64(hlpSignal, srcValPtr, writeIdx)                           \
do{                                                                                                \
  ComEmu64Type valuint64;                                                                          \
  COM_EB_SET_VALUE_IN_RX_BUFFER((hlpSignal), (srcValPtr), (writeIdx), &valuint64);                 \
}while(0)


#if (defined COM_EB_HANDLE_UB)        /* To prevent double declaration */
#error COM_EB_HANDLE_UB already defined
#endif /* #if (defined COM_EB_HANDLE_UB) */

/** \brief COM_EB_HANDLE_UB
 * Sets the update bit of the signal
 * \param[in] rxSignalPtr - pointer to Rx signal configuration
 * \param[in] ipduPtr - pointer to Rx-Pdu configuration to which the group signal belongs
 * \param[in] pduInfoPtr - pointer to buffer and length of the external buffer
 * \param[int] isRx - variable which indicates of the value of the signal was revived
 */
#if (COM_UPDATE_BIT_RX_CONFIG == COM_UPDATE_BIT_INDIVIDUAL)
#define COM_EB_HANDLE_UB(rxSignalPtr, ipduPtr, pduInfoPtr, isRx)                                   \
do{                                                                                                \
    CONSTP2CONST(ComRxSignalExtType, AUTOMATIC, COM_APPL_CONST) SignalExtPtr =                     \
        COM_GET_CONFIG_ELEMENT(ComRxSignalExtType,                                                 \
                               Com_gConfigPtr->ComRxSignalExtRef, (rxSignalPtr)->ComSignalExtRef); \
  if (COM_GET_RX_SIGNAL_EXT_UPDATE_BIT(SignalExtPtr) == COM_UPDATE_BIT_PRESENT)                    \
  {  /* Update bit is configured */                                                                \
    Com_EB_HandleShortPdu_HandleUB(                                                                \
        (ipduPtr),                                                                                 \
        (pduInfoPtr)->SduDataPtr,                                                                  \
        (pduInfoPtr)->SduLength,                                                                   \
        SignalExtPtr->ComUpdateBitPositionInBit,                                                   \
        (isRx));                                                                                   \
  }                                                                                                \
}while(0)
#elif (COM_UPDATE_BIT_RX_CONFIG == COM_UPDATE_BIT_PRESENT_FOR_ALL)
#define COM_EB_HANDLE_UB(rxSignalPtr, ipduPtr, pduInfoPtr, isRx)                                   \
do{                                                                                                \
    CONSTP2CONST(ComRxSignalExtType, AUTOMATIC, COM_APPL_CONST) SignalExtPtr =                     \
        COM_GET_CONFIG_ELEMENT(ComRxSignalExtType,                                                 \
                               Com_gConfigPtr->ComRxSignalExtRef, (rxSignalPtr)->ComSignalExtRef); \
    Com_EB_HandleShortPdu_HandleUB(                                                                \
        (ipduPtr),                                                                                 \
        (pduInfoPtr)->SduDataPtr,                                                                  \
        (pduInfoPtr)->SduLength,                                                                   \
        SignalExtPtr->ComUpdateBitPositionInBit,                                                   \
        (isRx));                                                                                   \
}while(0)
#elif (COM_UPDATE_BIT_RX_CONFIG == COM_UPDATE_BIT_ABSENT_FOR_ALL)
#define COM_EB_HANDLE_UB(rxSignalPtr, ipduPtr, pduInfoPtr, isRx) TS_PARAM_UNUSED((ipduPtr))
#else
#error uknown value of COM_UPDATE_BIT_RX_CONFIG
#endif

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define COM_START_SEC_CODE
#include <Com_MemMap.h>

#if (COM_RX_LITTLEENDIAN_ENABLE == STD_ON)
static FUNC(void, COM_CODE) Com_EB_Read_08_DL_E1
(
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
);
#else
#define Com_EB_Read_08_DL_E1 Com_EB_Read_DET
#endif

#if (COM_RX_LITTLEENDIAN_ENABLE == STD_ON)
static FUNC(void, COM_CODE) Com_EB_Read_16_DL_E1
(
      P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
      P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
      P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
      P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
);
#else
#define Com_EB_Read_16_DL_E1 Com_EB_Read_DET
#endif

#if (COM_RX_LITTLEENDIAN_ENABLE == STD_ON)
static FUNC(void, COM_CODE) Com_EB_Read_32_DL_E1
(
      P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
      P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
      P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
      P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
);
#else
#define Com_EB_Read_32_DL_E1 Com_EB_Read_DET
#endif

#if ( (COM_RX_LITTLEENDIAN_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Read_64_DL_E1
(
      P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
      P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
      P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
      P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
);
#else
#define Com_EB_Read_64_DL_E1 Com_EB_Read_DET
#endif

static FUNC(void, COM_CODE) Com_EB_Read_AY_DL_E1
(
      P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
      P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
      P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
      P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
);

#if (COM_RX_BIGENDIAN_ENABLE == STD_ON)
static FUNC(void, COM_CODE) Com_EB_Read_08_DL_E2
(
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
);
#else
#define Com_EB_Read_08_DL_E2 Com_EB_Read_DET
#endif

#if (COM_RX_BIGENDIAN_ENABLE == STD_ON)
static FUNC(void, COM_CODE) Com_EB_Read_16_DL_E2
(
      P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
      P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
      P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
      P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
);
#else
#define Com_EB_Read_16_DL_E2 Com_EB_Read_DET
#endif

#if (COM_RX_BIGENDIAN_ENABLE == STD_ON)
static FUNC(void, COM_CODE) Com_EB_Read_32_DL_E2
(
      P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
      P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
      P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
      P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
);
#else
#define Com_EB_Read_32_DL_E2 Com_EB_Read_DET
#endif

#if ( (COM_RX_BIGENDIAN_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Read_64_DL_E2
(
      P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
      P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
      P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
      P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
);
#else
#define Com_EB_Read_64_DL_E2 Com_EB_Read_DET
#endif

static FUNC(void, COM_CODE) Com_EB_Read_AY_DL_E2
(
      P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
      P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
      P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
      P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
);

#if (COM_RX_LITTLEENDIAN_ENABLE == STD_ON)
static FUNC(void, COM_CODE) Com_EB_Read_08_EL_E1
(
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
);
#else
#define Com_EB_Read_08_EL_E1 Com_EB_Read_DET
#endif

#if (COM_RX_LITTLEENDIAN_ENABLE == STD_ON)
static FUNC(void, COM_CODE) Com_EB_Read_16_EL_E1
(
      P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
      P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
      P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
      P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
);
#else
#define Com_EB_Read_16_EL_E1 Com_EB_Read_DET
#endif

#if (COM_RX_LITTLEENDIAN_ENABLE == STD_ON)
static FUNC(void, COM_CODE) Com_EB_Read_32_EL_E1
(
      P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
      P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
      P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
      P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
);
#else
#define Com_EB_Read_32_EL_E1 Com_EB_Read_DET
#endif

#if ( (COM_RX_LITTLEENDIAN_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Read_64_EL_E1
(
      P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
      P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
      P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
      P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
);
#else
#define Com_EB_Read_64_EL_E1 Com_EB_Read_DET
#endif

static FUNC(void, COM_CODE) Com_EB_Read_AY_EL_E1
(
      P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
      P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
      P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
      P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
);

#if (COM_RX_BIGENDIAN_ENABLE == STD_ON)
static FUNC(void, COM_CODE) Com_EB_Read_08_EL_E2
(
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
);
#else
#define Com_EB_Read_08_EL_E2 Com_EB_Read_DET
#endif

#if (COM_RX_BIGENDIAN_ENABLE == STD_ON)
static FUNC(void, COM_CODE) Com_EB_Read_16_EL_E2
(
      P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
      P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
      P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
      P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
);
#else
#define Com_EB_Read_16_EL_E2 Com_EB_Read_DET
#endif

#if (COM_RX_BIGENDIAN_ENABLE == STD_ON)
static FUNC(void, COM_CODE) Com_EB_Read_32_EL_E2
(
      P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
      P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
      P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
      P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
);
#else
#define Com_EB_Read_32_EL_E2 Com_EB_Read_DET
#endif

#if ( (COM_RX_BIGENDIAN_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Read_64_EL_E2
(
      P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
      P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
      P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
      P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
);
#else
#define Com_EB_Read_64_EL_E2 Com_EB_Read_DET
#endif

static FUNC(void, COM_CODE) Com_EB_Read_AY_EL_E2
(
      P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
      P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
      P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
      P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
);

static FUNC(void, COM_CODE) Com_EB_Read_AY_DL_E3
(
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
);

static FUNC(void, COM_CODE) Com_EB_Read_AY_EL_E3
(
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
);

static FUNC(void, COM_CODE) Com_EB_Read_08_DL_SignExt
(
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
);

static FUNC(void, COM_CODE) Com_EB_Read_08_EL_SignExt
(
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
);

static FUNC(void, COM_CODE) Com_EB_Read_16_DL_SignExt
(
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
);

static FUNC(void, COM_CODE) Com_EB_Read_16_EL_SignExt
(
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
);

static FUNC(void, COM_CODE) Com_EB_Read_32_DL_SignExt
(
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
);

static FUNC(void, COM_CODE) Com_EB_Read_32_EL_SignExt
(
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
);

static FUNC(void, COM_CODE) Com_EB_Read_BO_DL
(
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
);

static FUNC(void, COM_CODE) Com_EB_Read_A1_DL
(
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
);

static FUNC(void, COM_CODE) Com_EB_Read_BO_EL
(
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
);

static FUNC(void, COM_CODE) Com_EB_Read_A1_EL
(
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
);

static FUNC(void, COM_CODE) Com_EB_Read_DET
(
      P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
      P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
      P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
      P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
);

#if ((COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON) || (COM_HANDLESMALLERRXPDUS_NOTIFY == STD_ON))

static FUNC(uint8, COM_CODE) Com_EB_ReadReceived_DET
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    uint32 Pdulength
);

#if (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON)
static FUNC(void, COM_CODE) Com_EB_ReadCopy_DET
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
);
#endif

#if (COM_RX_LITTLEENDIAN_ENABLE == STD_ON)

static FUNC(uint8, COM_CODE) Com_EB_ReadReceived_E1
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    uint32 Pdulength
);

#else
#define Com_EB_ReadReceived_E1 Com_EB_ReadReceived_DET
#endif


#if (COM_RX_BIGENDIAN_ENABLE == STD_ON)

static FUNC(uint32, COM_CODE) Com_EB_ConvertMSBPos2LSBPos
(
    uint32 ComBitSize,
    uint32 ComSignalPositionInBit
);

static FUNC(uint8, COM_CODE) Com_EB_ReadReceived_E2
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    uint32 Pdulength
);

#else
#define Com_EB_ReadReceived_E2 Com_EB_ReadReceived_DET
#endif


static FUNC(uint8, COM_CODE) Com_EB_ReadReceived_AY
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    uint32 Pdulength
);

static FUNC(uint8, COM_CODE) Com_EB_ReadReceived_DYN
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    uint32 Pdulength
);

#if ((COM_RX_LITTLEENDIAN_ENABLE == STD_ON) && (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON))
static FUNC(void, COM_CODE) Com_EB_ReadCopySig_08_E1
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
);
#else
#define Com_EB_ReadCopySig_08_E1 Com_EB_ReadCopy_DET
#endif

#if ((COM_RX_LITTLEENDIAN_ENABLE == STD_ON) && (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON))
static FUNC(void, COM_CODE) Com_EB_ReadCopySig_16_E1
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
);
#else
#define Com_EB_ReadCopySig_16_E1 Com_EB_ReadCopy_DET
#endif

#if ((COM_RX_LITTLEENDIAN_ENABLE == STD_ON) && (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON))
static FUNC(void, COM_CODE) Com_EB_ReadCopySig_32_E1
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
);
#else
#define Com_EB_ReadCopySig_32_E1 Com_EB_ReadCopy_DET
#endif

#if ((COM_RX_LITTLEENDIAN_ENABLE == STD_ON) && (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON))
static FUNC(void, COM_CODE) Com_EB_ReadCopySig_64_E1
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
);
#else
#define Com_EB_ReadCopySig_64_E1 Com_EB_ReadCopy_DET
#endif

#if (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON)
static FUNC(void, COM_CODE) Com_EB_ReadCopySig_AY_EX
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
);
#endif

#if (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON)
static FUNC(void, COM_CODE) Com_EB_ReadCopySig_AY_DYN
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
);
#endif

#if ((COM_RX_BIGENDIAN_ENABLE == STD_ON) && (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON))
static FUNC(void, COM_CODE) Com_EB_ReadCopySig_08_E2
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
);
#else
#define Com_EB_ReadCopySig_08_E2 Com_EB_ReadCopy_DET
#endif

#if ((COM_RX_BIGENDIAN_ENABLE == STD_ON) && (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON))
static FUNC(void, COM_CODE) Com_EB_ReadCopySig_16_E2
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
);
#else
#define Com_EB_ReadCopySig_16_E2 Com_EB_ReadCopy_DET
#endif

#if ((COM_RX_BIGENDIAN_ENABLE == STD_ON) && (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON))
static FUNC(void, COM_CODE) Com_EB_ReadCopySig_32_E2
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
);
#else
#define Com_EB_ReadCopySig_32_E2 Com_EB_ReadCopy_DET
#endif

#if ((COM_RX_BIGENDIAN_ENABLE == STD_ON) && (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON))
static FUNC(void, COM_CODE) Com_EB_ReadCopySig_64_E2
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
);
#else
#define Com_EB_ReadCopySig_64_E2 Com_EB_ReadCopy_DET
#endif

#if (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON)
static FUNC(void, COM_CODE) Com_EB_ReadCopy_SetBoolVal
(
    P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) SduDataPtr,
    P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
    ComSignalDescBitType ComSignalPositionInBit
);
static FUNC(void, COM_CODE) Com_EB_ReadCopySig_BO
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
);
#endif

#if (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON)
static FUNC(void, COM_CODE) Com_EB_ReadCopySig_A1
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
);
#endif

#if ((COM_RX_LITTLEENDIAN_ENABLE == STD_ON) && (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON))
static FUNC(void, COM_CODE) Com_EB_ReadCopyGSig_08_E1
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
);
#else
#define Com_EB_ReadCopyGSig_08_E1 Com_EB_ReadCopy_DET
#endif

#if ((COM_RX_LITTLEENDIAN_ENABLE == STD_ON) && (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON))
static FUNC(void, COM_CODE) Com_EB_ReadCopyGSig_16_E1
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
);
#else
#define Com_EB_ReadCopyGSig_16_E1 Com_EB_ReadCopy_DET
#endif

#if ((COM_RX_LITTLEENDIAN_ENABLE == STD_ON) && (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON))
static FUNC(void, COM_CODE) Com_EB_ReadCopyGSig_32_E1
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
);
#else
#define Com_EB_ReadCopyGSig_32_E1 Com_EB_ReadCopy_DET
#endif

#if ((COM_RX_LITTLEENDIAN_ENABLE == STD_ON) && (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON))
static FUNC(void, COM_CODE) Com_EB_ReadCopyGSig_64_E1
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
);
#else
#define Com_EB_ReadCopyGSig_64_E1 Com_EB_ReadCopy_DET
#endif

#if (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON)
static FUNC(void, COM_CODE) Com_EB_ReadCopyGSig_AY_EX
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
);
#endif

#if ((COM_RX_BIGENDIAN_ENABLE == STD_ON) && (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON))
static FUNC(void, COM_CODE) Com_EB_ReadCopyGSig_08_E2
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
);
#else
#define Com_EB_ReadCopyGSig_08_E2 Com_EB_ReadCopy_DET
#endif

#if ((COM_RX_BIGENDIAN_ENABLE == STD_ON) && (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON))
static FUNC(void, COM_CODE) Com_EB_ReadCopyGSig_16_E2
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
);
#else
#define Com_EB_ReadCopyGSig_16_E2 Com_EB_ReadCopy_DET
#endif

#if ((COM_RX_BIGENDIAN_ENABLE == STD_ON) && (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON))
static FUNC(void, COM_CODE) Com_EB_ReadCopyGSig_32_E2
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
);
#else
#define Com_EB_ReadCopyGSig_32_E2 Com_EB_ReadCopy_DET
#endif

#if ((COM_RX_BIGENDIAN_ENABLE == STD_ON) && (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON))
static FUNC(void, COM_CODE) Com_EB_ReadCopyGSig_64_E2
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
);
#else
#define Com_EB_ReadCopyGSig_64_E2 Com_EB_ReadCopy_DET
#endif

#if (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON)
static FUNC(void, COM_CODE) Com_EB_ReadCopyGSig_BO
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
);
#endif

#if (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON)
static FUNC(void, COM_CODE) Com_EB_ReadCopyGSig_A1
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
);
#endif

#endif /* ((COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON) || (COM_HANDLESMALLERRXPDUS_NOTIFY == STD_ON)) */


#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>

/*==================[external constants]====================================*/

#define COM_START_SEC_CONST_UNSPECIFIED
#include <Com_MemMap.h>

CONST(Com_ReadExtFuncTableType, AUTOMATIC) COM_READFUNCS[COM_CORE_READ_ARRAY_SIZE] =
{

  /*  00 */           COM_EB_READ_SET_TABLE( Com_EB_Read_08_DL_E1,        Com_EB_ReadReceived_E1,  Com_EB_ReadCopySig_08_E1),
  /*  01 */           COM_EB_READ_SET_TABLE( Com_EB_Read_16_DL_E1,        Com_EB_ReadReceived_E1,  Com_EB_ReadCopySig_16_E1),
  /*  02 */           COM_EB_READ_SET_TABLE( Com_EB_Read_32_DL_E1,        Com_EB_ReadReceived_E1,  Com_EB_ReadCopySig_32_E1),
  /*  03 */           COM_EB_READ_SET_TABLE( Com_EB_Read_AY_DL_E1,        Com_EB_ReadReceived_AY,  Com_EB_ReadCopySig_AY_EX),
  /*  04 */           COM_EB_READ_SET_TABLE( Com_EB_Read_08_DL_E2,        Com_EB_ReadReceived_E2,  Com_EB_ReadCopySig_08_E2),
  /*  05 */           COM_EB_READ_SET_TABLE( Com_EB_Read_16_DL_E2,        Com_EB_ReadReceived_E2,  Com_EB_ReadCopySig_16_E2),
  /*  06 */           COM_EB_READ_SET_TABLE( Com_EB_Read_32_DL_E2,        Com_EB_ReadReceived_E2,  Com_EB_ReadCopySig_32_E2),
  /*  07 */           COM_EB_READ_SET_TABLE( Com_EB_Read_AY_DL_E2,        Com_EB_ReadReceived_AY,  Com_EB_ReadCopySig_AY_EX),
  /*  08 */           COM_EB_READ_SET_TABLE( Com_EB_Read_08_EL_E1,        Com_EB_ReadReceived_E1,  Com_EB_ReadCopySig_08_E1),
  /*  09 */           COM_EB_READ_SET_TABLE( Com_EB_Read_16_EL_E1,        Com_EB_ReadReceived_E1,  Com_EB_ReadCopySig_16_E1),
  /*  10 */           COM_EB_READ_SET_TABLE( Com_EB_Read_32_EL_E1,        Com_EB_ReadReceived_E1,  Com_EB_ReadCopySig_32_E1),
  /*  11 */           COM_EB_READ_SET_TABLE( Com_EB_Read_AY_EL_E1,        Com_EB_ReadReceived_AY,  Com_EB_ReadCopySig_AY_EX),
  /*  12 */           COM_EB_READ_SET_TABLE( Com_EB_Read_08_EL_E2,        Com_EB_ReadReceived_E2,  Com_EB_ReadCopySig_08_E2),
  /*  13 */           COM_EB_READ_SET_TABLE( Com_EB_Read_16_EL_E2,        Com_EB_ReadReceived_E2,  Com_EB_ReadCopySig_16_E2),
  /*  14 */           COM_EB_READ_SET_TABLE( Com_EB_Read_32_EL_E2,        Com_EB_ReadReceived_E2,  Com_EB_ReadCopySig_32_E2),
  /*  15 */           COM_EB_READ_SET_TABLE( Com_EB_Read_AY_EL_E2,        Com_EB_ReadReceived_AY,  Com_EB_ReadCopySig_AY_EX),
  /*  16 */           COM_EB_READ_SET_TABLE( Com_EB_Read_08_DL_SignExt,  Com_EB_ReadReceived_E1,  Com_EB_ReadCopySig_08_E1),
  /*  17 */           COM_EB_READ_SET_TABLE( Com_EB_Read_16_DL_SignExt,  Com_EB_ReadReceived_E1,  Com_EB_ReadCopySig_16_E1),
  /*  18 */           COM_EB_READ_SET_TABLE( Com_EB_Read_32_DL_SignExt,  Com_EB_ReadReceived_E1,  Com_EB_ReadCopySig_32_E1),
  /*  19 */           COM_EB_READ_SET_TABLE( Com_EB_Read_AY_DL_E3,        Com_EB_ReadReceived_AY,  Com_EB_ReadCopySig_AY_EX),
  /*  20 */           COM_EB_READ_SET_TABLE( Com_EB_Read_08_DL_SignExt,  Com_EB_ReadReceived_E2,  Com_EB_ReadCopySig_08_E2),
  /*  21 */           COM_EB_READ_SET_TABLE( Com_EB_Read_16_DL_SignExt,  Com_EB_ReadReceived_E2,  Com_EB_ReadCopySig_16_E2),
  /*  22 */           COM_EB_READ_SET_TABLE( Com_EB_Read_32_DL_SignExt,  Com_EB_ReadReceived_E2,  Com_EB_ReadCopySig_32_E2),
  /*  23 */           COM_EB_READ_SET_TABLE(     Com_EB_Read_DET,       Com_EB_ReadReceived_DYN, Com_EB_ReadCopySig_AY_DYN),
  /*  24 */           COM_EB_READ_SET_TABLE( Com_EB_Read_08_EL_SignExt,  Com_EB_ReadReceived_E1,  Com_EB_ReadCopySig_08_E1),
  /*  25 */           COM_EB_READ_SET_TABLE( Com_EB_Read_16_EL_SignExt,  Com_EB_ReadReceived_E1,  Com_EB_ReadCopySig_16_E1),
  /*  26 */           COM_EB_READ_SET_TABLE( Com_EB_Read_32_EL_SignExt,  Com_EB_ReadReceived_E1,  Com_EB_ReadCopySig_32_E1),
  /*  27 */           COM_EB_READ_SET_TABLE( Com_EB_Read_AY_EL_E3,        Com_EB_ReadReceived_AY,  Com_EB_ReadCopySig_AY_EX),
  /*  28 */           COM_EB_READ_SET_TABLE( Com_EB_Read_08_EL_SignExt,  Com_EB_ReadReceived_E2,  Com_EB_ReadCopySig_08_E2),
  /*  29 */           COM_EB_READ_SET_TABLE( Com_EB_Read_16_EL_SignExt,  Com_EB_ReadReceived_E2,  Com_EB_ReadCopySig_16_E2),
  /*  30 */           COM_EB_READ_SET_TABLE( Com_EB_Read_32_EL_SignExt,  Com_EB_ReadReceived_E2,  Com_EB_ReadCopySig_32_E2),
  /*  31 */           COM_EB_READ_SET_TABLE(     Com_EB_Read_DET,       Com_EB_ReadReceived_DET,      Com_EB_ReadCopy_DET),

  /*  32 */           COM_EB_READ_SET_TABLE( Com_EB_Read_BO_DL,          Com_EB_ReadReceived_E1,    Com_EB_ReadCopySig_BO),
  /*  33 */           COM_EB_READ_SET_TABLE( Com_EB_Read_A1_DL,          Com_EB_ReadReceived_AY,    Com_EB_ReadCopySig_A1),
  /*  34 */           COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,      Com_EB_ReadCopy_DET),
  /*  35 */           COM_EB_READ_SET_TABLE( Com_EB_Read_64_DL_E1,        Com_EB_ReadReceived_E1,  Com_EB_ReadCopySig_64_E1),
  /*  36 */           COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,      Com_EB_ReadCopy_DET),
  /*  37 */           COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,      Com_EB_ReadCopy_DET),
  /*  38 */           COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,      Com_EB_ReadCopy_DET),
  /*  39 */           COM_EB_READ_SET_TABLE( Com_EB_Read_64_DL_E2,        Com_EB_ReadReceived_E2,  Com_EB_ReadCopySig_64_E2),
  /*  40 */           COM_EB_READ_SET_TABLE( Com_EB_Read_BO_EL,          Com_EB_ReadReceived_E1,    Com_EB_ReadCopySig_BO),
  /*  41 */           COM_EB_READ_SET_TABLE( Com_EB_Read_A1_EL,          Com_EB_ReadReceived_AY,    Com_EB_ReadCopySig_A1),
  /*  42 */           COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,      Com_EB_ReadCopy_DET),
  /*  43 */           COM_EB_READ_SET_TABLE( Com_EB_Read_64_EL_E1,        Com_EB_ReadReceived_E1,  Com_EB_ReadCopySig_64_E1),
  /*  44 */           COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,      Com_EB_ReadCopy_DET),
  /*  45 */           COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,      Com_EB_ReadCopy_DET),
  /*  46 */           COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,      Com_EB_ReadCopy_DET),
  /*  47 */           COM_EB_READ_SET_TABLE( Com_EB_Read_64_EL_E2,        Com_EB_ReadReceived_E2,  Com_EB_ReadCopySig_64_E2),
  /*  48 */           COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,      Com_EB_ReadCopy_DET),
  /*  49 */           COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,      Com_EB_ReadCopy_DET),
  /*  50 */           COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,      Com_EB_ReadCopy_DET),
  /*  51 */           COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,      Com_EB_ReadCopy_DET),
  /*  52 */           COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,      Com_EB_ReadCopy_DET),
  /*  53 */           COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,      Com_EB_ReadCopy_DET),
  /*  54 */           COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,      Com_EB_ReadCopy_DET),
  /*  55 */           COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,      Com_EB_ReadCopy_DET),
  /*  56 */           COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,      Com_EB_ReadCopy_DET),
  /*  57 */           COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,      Com_EB_ReadCopy_DET),
  /*  58 */           COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,      Com_EB_ReadCopy_DET),
  /*  59 */           COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,      Com_EB_ReadCopy_DET),
  /*  60 */           COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,      Com_EB_ReadCopy_DET),
  /*  61 */           COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,      Com_EB_ReadCopy_DET),
  /*  62 */           COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,      Com_EB_ReadCopy_DET),
  /*  63 */           COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,      Com_EB_ReadCopy_DET),

  /*  64 */ /*  00 */ COM_EB_READ_SET_TABLE( Com_EB_Read_SignalExt,     Com_EB_ReadReceived_DET,   Com_EB_ReadCopyGSig_08_E1),
  /*  65 */ /*  01 */ COM_EB_READ_SET_TABLE( Com_EB_Read_SignalExt,     Com_EB_ReadReceived_DET,   Com_EB_ReadCopyGSig_16_E1),
  /*  66 */ /*  02 */ COM_EB_READ_SET_TABLE( Com_EB_Read_SignalExt,     Com_EB_ReadReceived_DET,   Com_EB_ReadCopyGSig_32_E1),
  /*  67 */ /*  03 */ COM_EB_READ_SET_TABLE( Com_EB_Read_SignalExt,     Com_EB_ReadReceived_DET,   Com_EB_ReadCopyGSig_AY_EX),
  /*  68 */ /*  04 */ COM_EB_READ_SET_TABLE( Com_EB_Read_SignalExt,     Com_EB_ReadReceived_DET,   Com_EB_ReadCopyGSig_08_E2),
  /*  69 */ /*  05 */ COM_EB_READ_SET_TABLE( Com_EB_Read_SignalExt,     Com_EB_ReadReceived_DET,   Com_EB_ReadCopyGSig_16_E2),
  /*  70 */ /*  06 */ COM_EB_READ_SET_TABLE( Com_EB_Read_SignalExt,     Com_EB_ReadReceived_DET,   Com_EB_ReadCopyGSig_32_E2),
  /*  71 */ /*  07 */ COM_EB_READ_SET_TABLE( Com_EB_Read_SignalExt,     Com_EB_ReadReceived_DET,   Com_EB_ReadCopyGSig_AY_EX),
  /*  72 */ /*  08 */ COM_EB_READ_SET_TABLE( Com_EB_Read_SignalExt,     Com_EB_ReadReceived_DET,   Com_EB_ReadCopyGSig_08_E1),
  /*  73 */ /*  09 */ COM_EB_READ_SET_TABLE( Com_EB_Read_SignalExt,     Com_EB_ReadReceived_DET,   Com_EB_ReadCopyGSig_16_E1),
  /*  74 */ /*  10 */ COM_EB_READ_SET_TABLE( Com_EB_Read_SignalExt,     Com_EB_ReadReceived_DET,   Com_EB_ReadCopyGSig_32_E1),
  /*  75 */ /*  11 */ COM_EB_READ_SET_TABLE( Com_EB_Read_SignalExt,     Com_EB_ReadReceived_DET,   Com_EB_ReadCopyGSig_AY_EX),
  /*  76 */ /*  12 */ COM_EB_READ_SET_TABLE( Com_EB_Read_SignalExt,     Com_EB_ReadReceived_DET,   Com_EB_ReadCopyGSig_08_E2),
  /*  77 */ /*  13 */ COM_EB_READ_SET_TABLE( Com_EB_Read_SignalExt,     Com_EB_ReadReceived_DET,   Com_EB_ReadCopyGSig_16_E2),
  /*  78 */ /*  14 */ COM_EB_READ_SET_TABLE( Com_EB_Read_SignalExt,     Com_EB_ReadReceived_DET,   Com_EB_ReadCopyGSig_32_E2),
  /*  79 */ /*  15 */ COM_EB_READ_SET_TABLE( Com_EB_Read_SignalExt,     Com_EB_ReadReceived_DET,   Com_EB_ReadCopyGSig_AY_EX),
  /*  80 */ /*  16 */ COM_EB_READ_SET_TABLE( Com_EB_Read_SignalExt,     Com_EB_ReadReceived_DET,   Com_EB_ReadCopyGSig_08_E1),
  /*  81 */ /*  17 */ COM_EB_READ_SET_TABLE( Com_EB_Read_SignalExt,     Com_EB_ReadReceived_DET,   Com_EB_ReadCopyGSig_16_E1),
  /*  82 */ /*  18 */ COM_EB_READ_SET_TABLE( Com_EB_Read_SignalExt,     Com_EB_ReadReceived_DET,   Com_EB_ReadCopyGSig_32_E1),
  /*  83 */ /*  19 */ COM_EB_READ_SET_TABLE( Com_EB_Read_SignalExt,     Com_EB_ReadReceived_DET,   Com_EB_ReadCopyGSig_AY_EX),
  /*  84 */ /*  20 */ COM_EB_READ_SET_TABLE( Com_EB_Read_SignalExt,     Com_EB_ReadReceived_DET,   Com_EB_ReadCopyGSig_08_E2),
  /*  85 */ /*  21 */ COM_EB_READ_SET_TABLE( Com_EB_Read_SignalExt,     Com_EB_ReadReceived_DET,   Com_EB_ReadCopyGSig_16_E2),
  /*  86 */ /*  22 */ COM_EB_READ_SET_TABLE( Com_EB_Read_SignalExt,     Com_EB_ReadReceived_DET,   Com_EB_ReadCopyGSig_32_E2),
  /*  87 */ /*  23 */ COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,       Com_EB_ReadCopy_DET),
  /*  88 */ /*  24 */ COM_EB_READ_SET_TABLE( Com_EB_Read_SignalExt,     Com_EB_ReadReceived_DET,   Com_EB_ReadCopyGSig_08_E1),
  /*  89 */ /*  25 */ COM_EB_READ_SET_TABLE( Com_EB_Read_SignalExt,     Com_EB_ReadReceived_DET,   Com_EB_ReadCopyGSig_16_E1),
  /*  90 */ /*  26 */ COM_EB_READ_SET_TABLE( Com_EB_Read_SignalExt,     Com_EB_ReadReceived_DET,   Com_EB_ReadCopyGSig_32_E1),
  /*  91 */ /*  27 */ COM_EB_READ_SET_TABLE( Com_EB_Read_SignalExt,     Com_EB_ReadReceived_DET,   Com_EB_ReadCopyGSig_AY_EX),
  /*  92 */ /*  28 */ COM_EB_READ_SET_TABLE( Com_EB_Read_SignalExt,     Com_EB_ReadReceived_DET,   Com_EB_ReadCopyGSig_08_E2),
  /*  93 */ /*  29 */ COM_EB_READ_SET_TABLE( Com_EB_Read_SignalExt,     Com_EB_ReadReceived_DET,   Com_EB_ReadCopyGSig_16_E2),
  /*  94 */ /*  30 */ COM_EB_READ_SET_TABLE( Com_EB_Read_SignalExt,     Com_EB_ReadReceived_DET,   Com_EB_ReadCopyGSig_32_E2),
  /*  95 */ /*  31 */ COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,       Com_EB_ReadCopy_DET),

  /*  96 */ /*  32 */ COM_EB_READ_SET_TABLE( Com_EB_Read_SignalExt,     Com_EB_ReadReceived_DET,     Com_EB_ReadCopyGSig_BO),
  /*  97 */ /*  33 */ COM_EB_READ_SET_TABLE( Com_EB_Read_SignalExt,     Com_EB_ReadReceived_DET,     Com_EB_ReadCopyGSig_A1),
  /*  98 */ /*  34 */ COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,       Com_EB_ReadCopy_DET),
  /*  99 */ /*  35 */ COM_EB_READ_SET_TABLE( Com_EB_Read_SignalExt,     Com_EB_ReadReceived_DET,   Com_EB_ReadCopyGSig_64_E1),
  /* 100 */ /*  36 */ COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,       Com_EB_ReadCopy_DET),
  /* 101 */ /*  37 */ COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,       Com_EB_ReadCopy_DET),
  /* 102 */ /*  38 */ COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,       Com_EB_ReadCopy_DET),
  /* 103 */ /*  39 */ COM_EB_READ_SET_TABLE( Com_EB_Read_SignalExt,     Com_EB_ReadReceived_DET,   Com_EB_ReadCopyGSig_64_E2),
  /* 104 */ /*  40 */ COM_EB_READ_SET_TABLE( Com_EB_Read_SignalExt,     Com_EB_ReadReceived_DET,     Com_EB_ReadCopyGSig_BO),
  /* 105 */ /*  41 */ COM_EB_READ_SET_TABLE( Com_EB_Read_SignalExt,     Com_EB_ReadReceived_DET,     Com_EB_ReadCopyGSig_A1),
  /* 106 */ /*  42 */ COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,       Com_EB_ReadCopy_DET),
  /* 107 */ /*  43 */ COM_EB_READ_SET_TABLE( Com_EB_Read_SignalExt,     Com_EB_ReadReceived_DET,   Com_EB_ReadCopyGSig_64_E1),
  /* 108 */ /*  44 */ COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,       Com_EB_ReadCopy_DET),
  /* 109 */ /*  45 */ COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,       Com_EB_ReadCopy_DET),
  /* 110 */ /*  46 */ COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,       Com_EB_ReadCopy_DET),
  /* 111 */ /*  47 */ COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,   Com_EB_ReadCopyGSig_64_E2),
  /* 112 */ /*  48 */ COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,       Com_EB_ReadCopy_DET),
  /* 113 */ /*  49 */ COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,       Com_EB_ReadCopy_DET),
  /* 114 */ /*  50 */ COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,       Com_EB_ReadCopy_DET),
  /* 115 */ /*  51 */ COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,       Com_EB_ReadCopy_DET),
  /* 116 */ /*  52 */ COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,       Com_EB_ReadCopy_DET),
  /* 117 */ /*  53 */ COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,       Com_EB_ReadCopy_DET),
  /* 118 */ /*  54 */ COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,       Com_EB_ReadCopy_DET),
  /* 119 */ /*  55 */ COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,       Com_EB_ReadCopy_DET),
  /* 120 */ /*  56 */ COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,       Com_EB_ReadCopy_DET),
  /* 121 */ /*  57 */ COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,       Com_EB_ReadCopy_DET),
  /* 122 */ /*  58 */ COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,       Com_EB_ReadCopy_DET),
  /* 123 */ /*  59 */ COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,       Com_EB_ReadCopy_DET),
  /* 124 */ /*  60 */ COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,       Com_EB_ReadCopy_DET),
  /* 125 */ /*  61 */ COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,       Com_EB_ReadCopy_DET),
  /* 126 */ /*  62 */ COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,       Com_EB_ReadCopy_DET),
  /* 127 */ /*  63 */ COM_EB_READ_SET_TABLE( Com_EB_Read_DET,           Com_EB_ReadReceived_DET,       Com_EB_ReadCopy_DET),

};


#define COM_STOP_SEC_CONST_UNSPECIFIED
#include <Com_MemMap.h>


/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]==========================*/

#define COM_START_SEC_CODE
#include <Com_MemMap.h>

#if (COM_RX_LITTLEENDIAN_ENABLE == STD_ON)
static FUNC(void, COM_CODE) Com_EB_Read_08_DL_E1
(
      P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
      P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
      P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
      P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
)
{
   /* Deviation MISRAC2012-2 */
   P2VAR (uint8, AUTOMATIC, COM_APPL_DATA) retPtr =
         (P2VAR (uint8, AUTOMATIC, COM_APPL_DATA))SignalDataPtr;

   uint8 value;

   ComSignalDescBitType row = (Com_EB_ReadPropPtr->ComSignalPositionInBit / 8U); /* Starting position of the signals in bits divided by 8 => starting position in byte */
   uint8 col = (uint8)Com_EB_ReadPropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;

   uint16 tmpval;
   const uint8 space = col + 1U;
   uint8 bitsread = 0U;


   if (col == 7U)
   {
       tmpval = dataPtr[pos];
       /* read to much, shift back */
       value = (uint8)(tmpval >> (8U - Com_EB_ReadPropPtr->ComBitSize));
   }
   else
   {
       tmpval = dataPtr[pos];
       /* check number of bytes which shall be read */
       if (space < Com_EB_ReadPropPtr->ComBitSize)
       {
          /* another byte has to be read */
          bitsread = bitsread + 8U;
          tmpval = tmpval << 8U;
          pos = pos - 1;
          tmpval = (uint16)(tmpval | (uint8)(dataPtr[pos]));
       }

       /* read to much, shift back */
       value = (uint8)(tmpval >> ((col + 1U + bitsread) - (uint8)Com_EB_ReadPropPtr->ComBitSize));
   }

   /* mask the unused bits */
   {
      uint8 const mask = (uint8)(0xFFU >> (8U - Com_EB_ReadPropPtr->ComBitSize));
      value = (uint8)(value & mask);
   }

   *retPtr = value;

   TS_PARAM_UNUSED(retvalPtr);
   return;
} /* Com_EB_Read_08_FUNC(COM_EB_LOCK,COM_EB_ENDIAN) */
#endif

#if (COM_RX_LITTLEENDIAN_ENABLE == STD_ON)
static FUNC(void, COM_CODE) Com_EB_Read_16_DL_E1
(
      P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
      P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
      P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
      P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr

)
{
   /* Deviation MISRAC2012-2 */
   P2VAR (uint16, AUTOMATIC, COM_APPL_DATA) retPtr =
         (P2VAR (uint16, AUTOMATIC, COM_APPL_DATA))SignalDataPtr;

   uint16 value;

   uint8 bitsread = 0U;

   sint8 ssize = (sint8)Com_EB_ReadPropPtr->ComBitSize;

   uint32 tmpval;

   ComSignalDescBitType row = (Com_EB_ReadPropPtr->ComSignalPositionInBit / 8U); /* Starting position of the signals in bits divided by 8 => starting position in byte */
   uint8 col = (uint8)Com_EB_ReadPropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;


   if (col == 7U)
   {
       bitsread = 8U;
       /* read the byte with the MSB */
       tmpval = (uint8)(dataPtr[pos]);

       /* range of col if 0-7, cast to signed char is still in range */
       ssize = ssize - 8;

       while (ssize > 0)
       {
          bitsread = bitsread + 8U;
          ssize = ssize - 8;
          tmpval = (uint32)tmpval << 8U;
          pos = pos - 1;
          tmpval = (uint32)(tmpval | (uint8)(dataPtr[pos]));
       }

       /* read to much, shift back */
       value = (uint16)(tmpval >> (bitsread - Com_EB_ReadPropPtr->ComBitSize));
   }
   else
   {
       /* read the byte with the MSB */
       tmpval = (uint8)(dataPtr[pos]);

       /* range of col if 0-7, cast to signed char is still in range */
       ssize = ssize - 1 - (sint8) col;

       while (ssize > 0)
       {
          bitsread = bitsread + 8U;
          ssize = ssize - 8;
          tmpval = (uint32)tmpval << 8U;
          pos = pos - 1;
          tmpval = (uint32)(tmpval | (uint8)(dataPtr[pos]));
       }

       /* read to much, shift back */
       value = (uint16)(tmpval >> ((col + 1U + bitsread) - (uint8)Com_EB_ReadPropPtr->ComBitSize));
   }

   /* mask the unused bits */
   {
      uint16 const mask = (uint16)(0xFFFFU >> (16U - Com_EB_ReadPropPtr->ComBitSize));
      value = (uint16)(value & mask);
   }

   *retPtr = value;

   TS_PARAM_UNUSED(retvalPtr);
   return;
} /* Com_EB_Read_16_FUNC(COM_EB_LOCK,COM_EB_ENDIAN) */
#endif

#if (COM_RX_LITTLEENDIAN_ENABLE == STD_ON)
static FUNC(void, COM_CODE) Com_EB_Read_32_DL_E1
(
      P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
      P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
      P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
      P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
)
{
  /* Deviation MISRAC2012-2 */
   P2VAR (uint32, AUTOMATIC, COM_APPL_DATA) retPtr =
         (P2VAR (uint32, AUTOMATIC, COM_APPL_DATA))SignalDataPtr;

   uint32 value = 0;

   ComSignalDescBitType row = (Com_EB_ReadPropPtr->ComSignalPositionInBit / 8U); /* Starting position of the signals in bits divided by 8 => starting position in byte */
   uint8 col = (uint8)Com_EB_ReadPropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;

   ComSignalBitSizeType size = Com_EB_ReadPropPtr->ComBitSize;

      if (col != 7U)
      {
         /* data does not start on byte boundary (MSB) */
         uint8 const space = col + 1;
         uint8 const mask = ((uint8) 0xFFU) >> (7U - col);
         uint8 const tmp_val = dataPtr[pos] & mask;
         value = (uint32) tmp_val;

         if (space >= size)
         {
            /* If we've already read as many or more bits than intended */
            value >>= (space - size);
            size = 0U;
         }
         else
         {
            /* prepare to continue reading */
            pos = pos - 1;
            size -= space;
         }
      }


      while (size > 7U)
      {
         /* read whole bytes */
         value <<= 8U;
         value += dataPtr[pos];
         size -= 8U;
         pos = pos - 1;
      }


      if (size > 0U)
      {
         /* read last bits */
         value <<= size;
         value += (uint32) dataPtr[pos] >> (8U - size);
      }

   *retPtr = value;

   TS_PARAM_UNUSED(retvalPtr);
   return;
} /* Com_EB_Read_32_FUNC(COM_EB_LOCK,COM_EB_ENDIAN) */
#endif

#if ( (COM_RX_LITTLEENDIAN_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Read_64_DL_E1
(
      P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
      P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
      P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
      P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr

)
{
  P2VAR(ComEmu64Type, AUTOMATIC, AUTOMATIC) SignalDataPtr_loc =
      (P2VAR(ComEmu64Type, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

  if(Com_EB_ReadPropPtr->ComBitSize > 32U)
  {
    ComSignalBaseType Com_EB_ReadProp_loc = *Com_EB_ReadPropPtr;

    /* set ComBitSize for higher bytes */
    Com_EB_ReadProp_loc.ComBitSize = Com_EB_ReadPropPtr->ComBitSize - 32U;

    Com_EB_Read_32_DL_E1(
          &SignalDataPtr_loc->HiWord,
          dataPtr,
          &Com_EB_ReadProp_loc,
          retvalPtr);

    /* adapt Com_EB_ReadPropPtr_loc for low 32 bits */
    Com_EB_ReadProp_loc.ComSignalPositionInBit =
        Com_EB_ReadPropPtr->ComSignalPositionInBit - (Com_EB_ReadPropPtr->ComBitSize - 32U);
    Com_EB_ReadProp_loc.ComBitSize = 32U;

    Com_EB_Read_32_DL_E1(
          &SignalDataPtr_loc->LoWord,
          dataPtr,
          &Com_EB_ReadProp_loc,
          retvalPtr);
  }
  else
  {

    Com_EB_Read_32_DL_E1(
          &SignalDataPtr_loc->LoWord,
          dataPtr,
          Com_EB_ReadPropPtr,
          retvalPtr);
  }

  TS_PARAM_UNUSED(retvalPtr);
  return;
} /* Com_EB_Read_64_FUNC(COM_EB_LOCK,COM_EB_ENDIAN) */
#endif

static FUNC(void, COM_CODE) Com_EB_Read_AY_DL_E1
(
      P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
      P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
      P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
      P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
)
{
   uint16 ii = 0U;
   /* Deviation MISRAC2012-2 */
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) U8Ptr =
         (P2VAR(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   do
   {
      uint16 pos = COM_EB_CONVERT_ENDIAN_RX_POS_NET_E1(Com_EB_ReadPropPtr->ComSignalPositionInBit, Com_EB_ReadPropPtr->ComBitSize, ii);
      U8Ptr[ii] = dataPtr[pos];
      ii++;
   }while (ii < (Com_EB_ReadPropPtr->ComBitSize));

   TS_PARAM_UNUSED(retvalPtr);
   return;
} /* Com_EB_Read_AY_FUNC(COM_EB_LOCK,COM_EB_ENDIAN) */

#if (COM_RX_BIGENDIAN_ENABLE == STD_ON)
static FUNC(void, COM_CODE) Com_EB_Read_08_DL_E2
(
      P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
      P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
      P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
      P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
)
{
   /* Deviation MISRAC2012-2 */
   P2VAR (uint8, AUTOMATIC, COM_APPL_DATA) retPtr =
         (P2VAR (uint8, AUTOMATIC, COM_APPL_DATA))SignalDataPtr;

   uint8 value;

   ComSignalDescBitType row = (Com_EB_ReadPropPtr->ComSignalPositionInBit / 8U); /* Starting position of the signals in bits divided by 8 => starting position in byte */
   uint8 col = (uint8)Com_EB_ReadPropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;

   uint16 tmpval;
   const uint8 space = col + 1U;
   uint8 bitsread = 0U;

   if (col == 7U)
   {
       tmpval = dataPtr[pos];
       /* read to much, shift back */
       value = (uint8)(tmpval >> (8U - Com_EB_ReadPropPtr->ComBitSize));
   }
   else
   {
       tmpval = dataPtr[pos];
       /* check number of bytes which shall be read */
       if (space < Com_EB_ReadPropPtr->ComBitSize)
       {
          /* another byte has to be read */
          bitsread = bitsread + 8U;
          tmpval = tmpval << 8U;
          pos = pos + 1;
          tmpval = (uint16)(tmpval | (uint8)(dataPtr[pos]));
       }

       /* read to much, shift back */
       value = (uint8)(tmpval >> ((col + 1U + bitsread) - (uint8)Com_EB_ReadPropPtr->ComBitSize));
   }

   /* mask the unused bits */
   {
      uint8 const mask = (uint8)(0xFFU >> (8U - Com_EB_ReadPropPtr->ComBitSize));
      value = (uint8)(value & mask);
   }

   *retPtr = value;

   TS_PARAM_UNUSED(retvalPtr);
   return;
} /* Com_EB_Read_08_FUNC(COM_EB_LOCK,COM_EB_ENDIAN) */
#endif

#if (COM_RX_BIGENDIAN_ENABLE == STD_ON)
static FUNC(void, COM_CODE) Com_EB_Read_16_DL_E2
(
      P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
      P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
      P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
      P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr

)
{
   /* Deviation MISRAC2012-2 */
   P2VAR (uint16, AUTOMATIC, COM_APPL_DATA) retPtr =
         (P2VAR (uint16, AUTOMATIC, COM_APPL_DATA))SignalDataPtr;

   uint16 value;

   uint8 bitsread = 0U;

   sint8 ssize = (sint8)Com_EB_ReadPropPtr->ComBitSize;

   uint32 tmpval;

   ComSignalDescBitType row = (Com_EB_ReadPropPtr->ComSignalPositionInBit / 8U); /* Starting position of the signals in bits divided by 8 => starting position in byte */
   uint8 col = (uint8)Com_EB_ReadPropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;

   if (col == 7U)
   {
       bitsread = 8U;
       /* read the byte with the MSB */
       tmpval = (uint8)(dataPtr[pos]);

       /* range of col if 0-7, cast to signed char is still in range */
       ssize = ssize - 8;

       while (ssize > 0)
       {
          bitsread = bitsread + 8U;
          ssize = ssize - 8;
          tmpval = (uint32)tmpval << 8U;
          pos = pos + 1;
          tmpval = (uint32)(tmpval | (uint8)(dataPtr[pos]));
       }

       /* read to much, shift back */
       value = (uint16)(tmpval >> (bitsread - Com_EB_ReadPropPtr->ComBitSize));
   }
   else
   {
       /* read the byte with the MSB */
       tmpval = (uint8)(dataPtr[pos]);

       /* range of col if 0-7, cast to signed char is still in range */
       ssize = ssize - 1 - (sint8) col;

       while (ssize > 0)
       {
          bitsread = bitsread + 8U;
          ssize = ssize - 8;
          tmpval = (uint32)tmpval << 8U;
          pos = pos + 1;
          tmpval = (uint32)(tmpval | (uint8)(dataPtr[pos]));
       }

       /* read to much, shift back */
       value = (uint16)(tmpval >> ((col + 1U + bitsread) - (uint8)Com_EB_ReadPropPtr->ComBitSize));
   }

   /* mask the unused bits */
   {
      uint16 const mask = (uint16)(0xFFFFU >> (16U - Com_EB_ReadPropPtr->ComBitSize));
      value = (uint16)(value & mask);
   }

   *retPtr = value;

   TS_PARAM_UNUSED(retvalPtr);
   return;
} /* Com_EB_Read_16_FUNC(COM_EB_LOCK,COM_EB_ENDIAN) */
#endif

#if (COM_RX_BIGENDIAN_ENABLE == STD_ON)
static FUNC(void, COM_CODE) Com_EB_Read_32_DL_E2
(
      P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
      P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
      P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
      P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr

)
{
   /* Deviation MISRAC2012-2 */
   P2VAR (uint32, AUTOMATIC, COM_APPL_DATA) retPtr =
         (P2VAR (uint32, AUTOMATIC, COM_APPL_DATA))SignalDataPtr;

   uint32 value = 0;

   ComSignalDescBitType row = (Com_EB_ReadPropPtr->ComSignalPositionInBit / 8U); /* Starting position of the signals in bits divided by 8 => starting position in byte */
   uint8 col = (uint8)Com_EB_ReadPropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;

   ComSignalBitSizeType size = Com_EB_ReadPropPtr->ComBitSize;

      if (col != 7U)
      {
         /* data does not start on byte boundary (MSB) */
         uint8 const space = col + 1;
         uint8 const mask = ((uint8) 0xFFU) >> (7U - col);
         uint8 const tmp_val = dataPtr[pos] & mask;
         value = (uint32) tmp_val;

         if (space >= size)
         {
            /* If we've already read as many or more bits than intended */
            value >>= (space - size);
            size = 0U;
         }
         else
         {
            /* prepare to continue reading */
            pos = pos + 1;
            size -= space;
         }
      }

      while (size > 7U)
      {
         /* read whole bytes */
         value <<= 8U;
         value += dataPtr[pos];
         size -= 8U;
         pos = pos + 1;
      }

      if (size > 0U)
      {
         /* read last bits */
         value <<= size;
         value += (uint32) dataPtr[pos] >> (8U - size);
      }

   *retPtr = value;

   TS_PARAM_UNUSED(retvalPtr);
   return;
} /* Com_EB_Read_32_FUNC(COM_EB_LOCK,COM_EB_ENDIAN) */
#endif

#if ( (COM_RX_BIGENDIAN_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Read_64_DL_E2
(
      P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
      P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
      P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
      P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr

)
{
  P2VAR(ComEmu64Type, AUTOMATIC, AUTOMATIC) SignalDataPtr_loc =
      (P2VAR(ComEmu64Type, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

  if(Com_EB_ReadPropPtr->ComBitSize > 32U)
  {
    ComSignalBaseType Com_EB_ReadProp_loc = *Com_EB_ReadPropPtr;

    /* set ComBitSize for higher bytes */
    Com_EB_ReadProp_loc.ComBitSize = Com_EB_ReadPropPtr->ComBitSize - 32U;

    Com_EB_Read_32_DL_E2(
          &SignalDataPtr_loc->HiWord,
          dataPtr,
          &Com_EB_ReadProp_loc,
          retvalPtr);

    /* adapt Com_EB_ReadPropPtr_loc for low 32 bits */
    Com_EB_ReadProp_loc.ComSignalPositionInBit =
        Com_EB_ReadPropPtr->ComSignalPositionInBit + (Com_EB_ReadPropPtr->ComBitSize - 32U);
    Com_EB_ReadProp_loc.ComBitSize = 32U;

    Com_EB_Read_32_DL_E2(
          &SignalDataPtr_loc->LoWord,
          dataPtr,
          &Com_EB_ReadProp_loc,
          retvalPtr);
  }
  else
  {
    Com_EB_Read_32_DL_E2(
          &SignalDataPtr_loc->LoWord,
          dataPtr,
          Com_EB_ReadPropPtr,
          retvalPtr);
  }

  TS_PARAM_UNUSED(retvalPtr);
  return;
} /* Com_EB_Read_64_FUNC(COM_EB_LOCK,COM_EB_ENDIAN) */
#endif

static FUNC(void, COM_CODE) Com_EB_Read_AY_DL_E2
(
      P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
      P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
      P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
      P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
)
{
   uint16 ii = 0U;
   /* Deviation MISRAC2012-2 */
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) U8Ptr =
         (P2VAR(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   do
   {
      uint16 pos = COM_EB_CONVERT_ENDIAN_RX_POS_NET_E2(Com_EB_ReadPropPtr->ComSignalPositionInBit, Com_EB_ReadPropPtr->ComBitSize, ii);
      U8Ptr[ii] = dataPtr[pos];
      ii++;
   }while (ii < (Com_EB_ReadPropPtr->ComBitSize));

   TS_PARAM_UNUSED(retvalPtr);
   return;
} /* Com_EB_Read_AY_FUNC(COM_EB_LOCK,COM_EB_ENDIAN) */

#if (COM_RX_LITTLEENDIAN_ENABLE == STD_ON)
static FUNC(void, COM_CODE) Com_EB_Read_08_EL_E1
(
      P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
      P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
      P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
      P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
)
{
   /* Deviation MISRAC2012-2 */
   P2VAR (uint8, AUTOMATIC, COM_APPL_DATA) retPtr =
         (P2VAR (uint8, AUTOMATIC, COM_APPL_DATA))SignalDataPtr;

   uint8 value;

   ComSignalDescBitType row = (Com_EB_ReadPropPtr->ComSignalPositionInBit / 8U); /* Starting position of the signals in bits divided by 8 => starting position in byte */
   uint8 col = (uint8)Com_EB_ReadPropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;

   uint16 tmpval;
   const uint8 space = col + 1U;
   uint8 bitsread = 0U;

   if (col == 7U)
   {
       TS_AtomicAssign8(tmpval, dataPtr[pos]);
       /* read to much, shift back */
       value = (uint8)(tmpval >> (8U - Com_EB_ReadPropPtr->ComBitSize));
   }
   else
   {
       /* check number of bytes which shall be read */
       if (space < Com_EB_ReadPropPtr->ComBitSize)
       {
          Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

          tmpval = dataPtr[pos];
          /* another byte has to be read */
          bitsread = bitsread + 8U;
          tmpval = tmpval << 8U;
          pos = pos - 1;
          tmpval = (uint16)(tmpval | (uint8)(dataPtr[pos]));

          Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();
       }
       else
       {
          TS_AtomicAssign8(tmpval, dataPtr[pos]);
       }

       /* read to much, shift back */
       value = (uint8)(tmpval >> ((col + 1U + bitsread) - (uint8)Com_EB_ReadPropPtr->ComBitSize));
   }


   /* mask the unused bits */
   {
      uint8 const mask = (uint8)(0xFFU >> (8U - Com_EB_ReadPropPtr->ComBitSize));
      value = (uint8)(value & mask);
   }

   *retPtr = value;

   TS_PARAM_UNUSED(retvalPtr);
   return;
} /* Com_EB_Read_08_FUNC(COM_EB_LOCK,COM_EB_ENDIAN) */
#endif

#if (COM_RX_LITTLEENDIAN_ENABLE == STD_ON)
static FUNC(void, COM_CODE) Com_EB_Read_16_EL_E1
(
      P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
      P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
      P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
      P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
)
{
   /* Deviation MISRAC2012-2 */
   P2VAR (uint16, AUTOMATIC, COM_APPL_DATA) retPtr =
         (P2VAR (uint16, AUTOMATIC, COM_APPL_DATA))SignalDataPtr;

   uint16 value;

   uint8 bitsread = 0U;

   sint8 ssize = (sint8)Com_EB_ReadPropPtr->ComBitSize;

   uint32 tmpval;

   ComSignalDescBitType row = (Com_EB_ReadPropPtr->ComSignalPositionInBit / 8U); /* Starting position of the signals in bits divided by 8 => starting position in byte */
   uint8 col = (uint8)Com_EB_ReadPropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();
   if (col == 7U)
   {
       bitsread = 8U;
       /* read the byte with the MSB */
       tmpval = (uint8)(dataPtr[pos]);

       /* range of col if 0-7, cast to signed char is still in range */
       ssize = ssize - 8;

       while (ssize > 0)
       {
          bitsread = bitsread + 8U;
          ssize = ssize - 8;
          tmpval = (uint32)tmpval << 8U;
          pos = pos - 1;
          tmpval = (uint32)(tmpval | (uint8)(dataPtr[pos]));
       }

       /* read to much, shift back */
       value = (uint16)(tmpval >> (bitsread - Com_EB_ReadPropPtr->ComBitSize));
   }
   else
   {
       /* read the byte with the MSB */
       tmpval = (uint8)(dataPtr[pos]);

       /* range of col if 0-7, cast to signed char is still in range */
       ssize = ssize - 1 - (sint8) col;

       while (ssize > 0)
       {
          bitsread = bitsread + 8U;
          ssize = ssize - 8;
          tmpval = (uint32)tmpval << 8U;
          pos = pos - 1;
          tmpval = (uint32)(tmpval | (uint8)(dataPtr[pos]));
       }

       /* read to much, shift back */
       value = (uint16)(tmpval >> ((col + 1U + bitsread) - (uint8)Com_EB_ReadPropPtr->ComBitSize));
   }

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   /* mask the unused bits */
   {
      uint16 const mask = (uint16)(0xFFFFU >> (16U - Com_EB_ReadPropPtr->ComBitSize));
      value = (uint16)(value & mask);
   }

   *retPtr = value;

   TS_PARAM_UNUSED(retvalPtr);
   return;
} /* Com_EB_Read_16_FUNC(COM_EB_LOCK,COM_EB_ENDIAN) */
#endif

#if (COM_RX_LITTLEENDIAN_ENABLE == STD_ON)
static FUNC(void, COM_CODE) Com_EB_Read_32_EL_E1
(
      P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
      P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
      P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
      P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
)
{
   /* Deviation MISRAC2012-2 */
   P2VAR (uint32, AUTOMATIC, COM_APPL_DATA) retPtr =
         (P2VAR (uint32, AUTOMATIC, COM_APPL_DATA))SignalDataPtr;

   uint32 value = 0;

   ComSignalDescBitType row = (Com_EB_ReadPropPtr->ComSignalPositionInBit / 8U); /* Starting position of the signals in bits divided by 8 => starting position in byte */
   uint8 col = (uint8)Com_EB_ReadPropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;

   ComSignalBitSizeType size = Com_EB_ReadPropPtr->ComBitSize;

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

      if (col != 7U)
      {
         /* data does not start on byte boundary (MSB) */
         uint8 const space = col + 1;
         uint8 const mask = ((uint8) 0xFFU) >> (7U - col);
         uint8 const tmp_val = dataPtr[pos] & mask;
         value = (uint32) tmp_val;

         if (space >= size)
         {
            /* If we've already read as many or more bits than intended */
            value >>= (space - size);
            size = 0U;
         }
         else
         {
            /* prepare to continue reading */
            pos = pos - 1;
            size -= space;
         }
      }

      while (size > 7U)
      {
         /* read whole bytes */
         value <<= 8U;
         value += dataPtr[pos];
         size -= 8U;
         pos = pos - 1;
      }

      if (size > 0U)
      {
         /* read last bits */
         value <<= size;
         value += (uint32) dataPtr[pos] >> (8U - size);
      }

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   *retPtr = value;

   TS_PARAM_UNUSED(retvalPtr);
   return;
} /* Com_EB_Read_32_FUNC(COM_EB_LOCK,COM_EB_ENDIAN) */
#endif

#if ( (COM_RX_LITTLEENDIAN_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Read_64_EL_E1
(
      P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
      P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
      P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
      P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
)
{
  P2VAR(ComEmu64Type, AUTOMATIC, AUTOMATIC) SignalDataPtr_loc =
      (P2VAR(ComEmu64Type, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

  if(Com_EB_ReadPropPtr->ComBitSize > 32U)
  {
    ComSignalBaseType Com_EB_ReadProp_loc = *Com_EB_ReadPropPtr;

    /* set ComBitSize for higher bytes */
    Com_EB_ReadProp_loc.ComBitSize = Com_EB_ReadPropPtr->ComBitSize - 32U;

    Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

    Com_EB_Read_32_DL_E1(
          &SignalDataPtr_loc->HiWord,
          dataPtr,
          &Com_EB_ReadProp_loc,
          retvalPtr);

    /* adapt Com_EB_ReadPropPtr_loc for low 32 bits */
    Com_EB_ReadProp_loc.ComSignalPositionInBit =
        Com_EB_ReadPropPtr->ComSignalPositionInBit - (Com_EB_ReadPropPtr->ComBitSize - 32U);
    Com_EB_ReadProp_loc.ComBitSize = 32U;

    Com_EB_Read_32_DL_E1(
          &SignalDataPtr_loc->LoWord,
          dataPtr,
          &Com_EB_ReadProp_loc,
          retvalPtr);

    Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();
  }
  else
  {
    Com_EB_Read_32_EL_E1(
          &SignalDataPtr_loc->LoWord,
          dataPtr,
          Com_EB_ReadPropPtr,
          retvalPtr);
  }

  TS_PARAM_UNUSED(retvalPtr);
  return;
} /* Com_EB_Read_64_FUNC(COM_EB_LOCK,COM_EB_ENDIAN) */
#endif

static FUNC(void, COM_CODE) Com_EB_Read_AY_EL_E1
(
      P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
      P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
      P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
      P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
)
{
   uint16 ii = 0U;
   /* Deviation MISRAC2012-2 */
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) U8Ptr =
         (P2VAR(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

   do
   {
      uint16 pos = COM_EB_CONVERT_ENDIAN_RX_POS_NET_E1(Com_EB_ReadPropPtr->ComSignalPositionInBit, Com_EB_ReadPropPtr->ComBitSize, ii);
      U8Ptr[ii] = dataPtr[pos];
      ii++;
   }while (ii < (Com_EB_ReadPropPtr->ComBitSize));

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   TS_PARAM_UNUSED(retvalPtr);
   return;
} /* Com_EB_Read_AY_FUNC(COM_EB_LOCK,COM_EB_ENDIAN) */

#if (COM_RX_BIGENDIAN_ENABLE == STD_ON)
static FUNC(void, COM_CODE) Com_EB_Read_08_EL_E2
(
      P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
      P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
      P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
      P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
)
{
   /* Deviation MISRAC2012-2 */
   P2VAR (uint8, AUTOMATIC, COM_APPL_DATA) retPtr =
         (P2VAR (uint8, AUTOMATIC, COM_APPL_DATA))SignalDataPtr;

   uint8 value;

   ComSignalDescBitType row = (Com_EB_ReadPropPtr->ComSignalPositionInBit / 8U); /* Starting position of the signals in bits divided by 8 => starting position in byte */
   uint8 col = (uint8)Com_EB_ReadPropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;

   uint16 tmpval;
   const uint8 space = col + 1U;
   uint8 bitsread = 0U;

   if (col == 7U)
   {
       TS_AtomicAssign8(tmpval, dataPtr[pos]);
       /* read to much, shift back */
       value = (uint8)(tmpval >> (8U - Com_EB_ReadPropPtr->ComBitSize));
   }
   else
   {
       /* check number of bytes which shall be read */
       if (space < Com_EB_ReadPropPtr->ComBitSize)
       {
          Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

          tmpval = dataPtr[pos];
          /* another byte has to be read */
          bitsread = bitsread + 8U;
          tmpval = tmpval << 8U;
          pos = pos + 1;
          tmpval = (uint16)(tmpval | (uint8)(dataPtr[pos]));

          Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();
       }
       else
       {
          TS_AtomicAssign8(tmpval, dataPtr[pos]);
       }

       /* read to much, shift back */
       value = (uint8)(tmpval >> ((col + 1U + bitsread) - (uint8)Com_EB_ReadPropPtr->ComBitSize));
   }


   /* mask the unused bits */
   {
      uint8 const mask = (uint8)(0xFFU >> (8U - Com_EB_ReadPropPtr->ComBitSize));
      value = (uint8)(value & mask);
   }

   *retPtr = value;

   TS_PARAM_UNUSED(retvalPtr);
   return;
} /* Com_EB_Read_08_FUNC(COM_EB_LOCK,COM_EB_ENDIAN) */
#endif

#if (COM_RX_BIGENDIAN_ENABLE == STD_ON)
static FUNC(void, COM_CODE) Com_EB_Read_16_EL_E2
(
      P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
      P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
      P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
      P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
)
{
   /* Deviation MISRAC2012-2 */
   P2VAR (uint16, AUTOMATIC, COM_APPL_DATA) retPtr =
         (P2VAR (uint16, AUTOMATIC, COM_APPL_DATA))SignalDataPtr;

   uint16 value;

   uint8 bitsread = 0U;

   sint8 ssize = (sint8)Com_EB_ReadPropPtr->ComBitSize;

   uint32 tmpval;

   ComSignalDescBitType row = (Com_EB_ReadPropPtr->ComSignalPositionInBit / 8U); /* Starting position of the signals in bits divided by 8 => starting position in byte */
   uint8 col = (uint8)Com_EB_ReadPropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

   if (col == 7U)
   {
       bitsread = 8U;
       /* read the byte with the MSB */
       tmpval = (uint8)(dataPtr[pos]);

       /* range of col if 0-7, cast to signed char is still in range */
       ssize = ssize - 8;

       while (ssize > 0)
       {
          bitsread = bitsread + 8U;
          ssize = ssize - 8;
          tmpval = (uint32)tmpval << 8U;
          pos = pos + 1;
          tmpval = (uint32)(tmpval | (uint8)(dataPtr[pos]));
       }

       /* read to much, shift back */
       value = (uint16)(tmpval >> (bitsread - Com_EB_ReadPropPtr->ComBitSize));
   }
   else
   {
       /* read the byte with the MSB */
       tmpval = (uint8)(dataPtr[pos]);

       /* range of col if 0-7, cast to signed char is still in range */
       ssize = ssize - 1 - (sint8) col;

       while (ssize > 0)
       {
          bitsread = bitsread + 8U;
          ssize = ssize - 8;
          tmpval = (uint32)tmpval << 8U;
          pos = pos + 1;
          tmpval = (uint32)(tmpval | (uint8)(dataPtr[pos]));
       }

       /* read to much, shift back */
       value = (uint16)(tmpval >> ((col + 1U + bitsread) - (uint8)Com_EB_ReadPropPtr->ComBitSize));
   }

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   /* mask the unused bits */
   {
      uint16 const mask = (uint16)(0xFFFFU >> (16U - Com_EB_ReadPropPtr->ComBitSize));
      value = (uint16)(value & mask);
   }

   *retPtr = value;

   TS_PARAM_UNUSED(retvalPtr);
   return;
} /* Com_EB_Read_16_FUNC(COM_EB_LOCK,COM_EB_ENDIAN) */
#endif

#if (COM_RX_BIGENDIAN_ENABLE == STD_ON)
static FUNC(void, COM_CODE) Com_EB_Read_32_EL_E2
(
      P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
      P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
      P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
      P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
)
{
   /* Deviation MISRAC2012-2 */
   P2VAR (uint32, AUTOMATIC, COM_APPL_DATA) retPtr =
         (P2VAR (uint32, AUTOMATIC, COM_APPL_DATA))SignalDataPtr;

   uint32 value = 0;

   ComSignalDescBitType row = (Com_EB_ReadPropPtr->ComSignalPositionInBit / 8U); /* Starting position of the signals in bits divided by 8 => starting position in byte */
   uint8 col = (uint8)Com_EB_ReadPropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;

   ComSignalBitSizeType size = Com_EB_ReadPropPtr->ComBitSize;

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

      if (col != 7U)
      {
         /* data does not start on byte boundary (MSB) */
         uint8 const space = col + 1;
         uint8 const mask = ((uint8) 0xFFU) >> (7U - col);
         uint8 const tmp_val = dataPtr[pos] & mask;
         value = (uint32) tmp_val;

         if (space >= size)
         {
            /* If we've already read as many or more bits than intended */
            value >>= (space - size);
            size = 0U;
         }
         else
         {
            /* prepare to continue reading */
            pos = pos + 1;
            size -= space;
         }
      }

      while (size > 7U)
      {
         /* read whole bytes */
         value <<= 8U;
         value += dataPtr[pos];
         size -= 8U;
         pos = pos + 1;
      }

      if (size > 0U)
      {
         /* read last bits */
         value <<= size;
         value += (uint32) dataPtr[pos] >> (8U - size);
      }

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   *retPtr = value;

   TS_PARAM_UNUSED(retvalPtr);
   return;
} /* Com_EB_Read_32_FUNC(COM_EB_LOCK,COM_EB_ENDIAN) */
#endif

#if ( (COM_RX_BIGENDIAN_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Read_64_EL_E2
(
      P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
      P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
      P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
      P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
)
{
  P2VAR(ComEmu64Type, AUTOMATIC, AUTOMATIC) SignalDataPtr_loc =
      (P2VAR(ComEmu64Type, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

  if(Com_EB_ReadPropPtr->ComBitSize > 32U)
  {
    ComSignalBaseType Com_EB_ReadProp_loc = *Com_EB_ReadPropPtr;

    /* set ComBitSize for higher bytes */
    Com_EB_ReadProp_loc.ComBitSize = Com_EB_ReadPropPtr->ComBitSize - 32U;

    Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

    Com_EB_Read_32_DL_E2(
          &SignalDataPtr_loc->HiWord,
          dataPtr,
          &Com_EB_ReadProp_loc,
          retvalPtr);

    /* adapt Com_EB_ReadPropPtr_loc for low 32 bits */
    Com_EB_ReadProp_loc.ComSignalPositionInBit =
        Com_EB_ReadPropPtr->ComSignalPositionInBit + (Com_EB_ReadPropPtr->ComBitSize - 32U);
    Com_EB_ReadProp_loc.ComBitSize = 32U;

    Com_EB_Read_32_DL_E2(
          &SignalDataPtr_loc->LoWord,
          dataPtr,
          &Com_EB_ReadProp_loc,
          retvalPtr);

    Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();
  }
  else
  {
    Com_EB_Read_32_EL_E2(
          &SignalDataPtr_loc->LoWord,
          dataPtr,
          Com_EB_ReadPropPtr,
          retvalPtr);
  }

  TS_PARAM_UNUSED(retvalPtr);
  return;
} /* Com_EB_Read_64_FUNC(COM_EB_LOCK,COM_EB_ENDIAN) */
#endif

static FUNC(void, COM_CODE) Com_EB_Read_AY_EL_E2
(
      P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
      P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
      P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
      P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
)
{
   uint16 ii = 0U;
   /* Deviation MISRAC2012-2 */
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) U8Ptr =
         (P2VAR(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

   do
   {
      uint16 pos = COM_EB_CONVERT_ENDIAN_RX_POS_NET_E2(Com_EB_ReadPropPtr->ComSignalPositionInBit, Com_EB_ReadPropPtr->ComBitSize, ii);
      U8Ptr[ii] = dataPtr[pos];
      ii++;
   }while (ii < (Com_EB_ReadPropPtr->ComBitSize));

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   TS_PARAM_UNUSED(retvalPtr);
   return;
} /* Com_EB_Read_AY_FUNC(COM_EB_LOCK,COM_EB_ENDIAN) */

static FUNC(void, COM_CODE) Com_EB_Read_AY_DL_E3
(
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
)
{

   ComSignalDescBitType startbyte =
         Com_EB_ReadPropPtr->ComSignalPositionInBit;

   CONSTP2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) valPtr =
         &dataPtr[startbyte];

   /* copy the data from the Pdu */
   TS_MemCpy(SignalDataPtr,
      valPtr,
      Com_EB_ReadPropPtr->ComBitSize);

   TS_PARAM_UNUSED(retvalPtr);
   return;
}

static FUNC(void, COM_CODE) Com_EB_Read_AY_EL_E3
(
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
)
{
   ComSignalDescBitType startbyte =
         Com_EB_ReadPropPtr->ComSignalPositionInBit;

   CONSTP2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) valPtr =
         &dataPtr[startbyte];

   /* Enter critical section */
   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

   /* copy the data from the Pdu */
   TS_MemCpy(SignalDataPtr,
      valPtr,
      Com_EB_ReadPropPtr->ComBitSize);

   /* Leave critical section */
   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   TS_PARAM_UNUSED(retvalPtr);
   return;
}

static FUNC(void, COM_CODE) Com_EB_Read_08_DL_SignExt
(
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
)
{
   /* Deviation MISRAC2012-2 */
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) SignalDataPtrU8 =
         (P2VAR(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   Com_EB_Read_Core_NoLock_NoSign(SignalDataPtr, dataPtr, Com_EB_ReadPropPtr, retvalPtr);

   {
      /* Deviation MISRAC2012-1 */
      boolean const MSB = (boolean)((*SignalDataPtrU8) >> (Com_EB_ReadPropPtr->ComBitSize - 1U));
      if (MSB != 0U)
      {
         uint8 const sigExtMask  = (uint8)(0xFFU << (Com_EB_ReadPropPtr->ComBitSize - 1U));
         (*SignalDataPtrU8) = (uint8)((*SignalDataPtrU8) | sigExtMask );
      }
   }

   TS_PARAM_UNUSED(retvalPtr);
   return;
}

static FUNC(void, COM_CODE) Com_EB_Read_08_EL_SignExt
(
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
)
{
   /* Deviation MISRAC2012-2 */
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) SignalDataPtrU8 =
         (P2VAR(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   Com_EB_Read_Core_NoSign(SignalDataPtr, dataPtr, Com_EB_ReadPropPtr, retvalPtr);

   {
      /* Deviation MISRAC2012-1 */
      boolean const MSB = (boolean)((*SignalDataPtrU8) >> (Com_EB_ReadPropPtr->ComBitSize - 1U));
      if (MSB != 0U)
      {
         uint8 const sigExtMask  = (uint8)(0xFFU << (Com_EB_ReadPropPtr->ComBitSize - 1U));
         (*SignalDataPtrU8) = (uint8)((*SignalDataPtrU8) | sigExtMask );
      }
   }

   TS_PARAM_UNUSED(retvalPtr);
   return;
}

static FUNC(void, COM_CODE) Com_EB_Read_16_DL_SignExt
(
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
)
{
   /* Deviation MISRAC2012-2 */
   P2VAR(uint16, AUTOMATIC, COM_APPL_DATA) SignalDataPtrU16 =
         (P2VAR(uint16, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   Com_EB_Read_Core_NoLock_NoSign(SignalDataPtr, dataPtr, Com_EB_ReadPropPtr, retvalPtr);

   {
      /* Deviation MISRAC2012-1 */
      boolean const MSB = (boolean)((*SignalDataPtrU16) >> (Com_EB_ReadPropPtr->ComBitSize - 1U));
      if (MSB != 0U)
      {
         uint16 const sigExtMask  = (uint16)(0xFFFFU << (Com_EB_ReadPropPtr->ComBitSize - 1U));
         (*SignalDataPtrU16) = (uint16)((*SignalDataPtrU16) | sigExtMask );
      }
   }

   TS_PARAM_UNUSED(retvalPtr);
   return;
}

static FUNC(void, COM_CODE) Com_EB_Read_16_EL_SignExt
(
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
)
{
   /* Deviation MISRAC2012-2 */
   P2VAR(uint16, AUTOMATIC, COM_APPL_DATA) SignalDataPtrU16 =
         (P2VAR(uint16, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   Com_EB_Read_Core_NoSign(SignalDataPtr, dataPtr, Com_EB_ReadPropPtr, retvalPtr);

   {
      /* Deviation MISRAC2012-1 */
      boolean const MSB = (boolean)((*SignalDataPtrU16) >> (Com_EB_ReadPropPtr->ComBitSize - 1U));
      if (MSB != 0U)
      {
         uint16 const sigExtMask  = (uint16)(0xFFFFU << (Com_EB_ReadPropPtr->ComBitSize - 1U));
         (*SignalDataPtrU16) = (uint16)((*SignalDataPtrU16) | sigExtMask );
      }
   }

   TS_PARAM_UNUSED(retvalPtr);
   return;
}

static FUNC(void, COM_CODE) Com_EB_Read_32_DL_SignExt
(
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
)
{
   /* Deviation MISRAC2012-2 */
   P2VAR(uint32, AUTOMATIC, COM_APPL_DATA) SignalDataPtrU32 =
         (P2VAR(uint32, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   Com_EB_Read_Core_NoLock_NoSign(SignalDataPtr, dataPtr, Com_EB_ReadPropPtr, retvalPtr);

   {
      /* Deviation MISRAC2012-1 */
      boolean const MSB = (boolean)((*SignalDataPtrU32) >> (Com_EB_ReadPropPtr->ComBitSize - 1U));
      if (MSB != 0U)
      {
         uint32 const sigExtMask  = (uint32)(0xFFFFFFFFUL << (Com_EB_ReadPropPtr->ComBitSize - 1U));
         (*SignalDataPtrU32) = (uint32)((*SignalDataPtrU32) | sigExtMask );
      }
   }

   TS_PARAM_UNUSED(retvalPtr);
   return;
}

static FUNC(void, COM_CODE) Com_EB_Read_32_EL_SignExt
(
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
)
{
   /* Deviation MISRAC2012-2 */
   P2VAR(uint32, AUTOMATIC, COM_APPL_DATA) SignalDataPtrU32 =
         (P2VAR(uint32, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   Com_EB_Read_Core_NoSign(SignalDataPtr, dataPtr, Com_EB_ReadPropPtr, retvalPtr);

   {
      /* Deviation MISRAC2012-1 */
      boolean const MSB = (boolean)((*SignalDataPtrU32) >> (Com_EB_ReadPropPtr->ComBitSize - 1U));
      if (MSB != 0U)
      {
         uint32 const sigExtMask  = (uint32)(0xFFFFFFFFUL << (Com_EB_ReadPropPtr->ComBitSize - 1U));
         (*SignalDataPtrU32) = (uint32)((*SignalDataPtrU32) | sigExtMask );
      }
   }

   TS_PARAM_UNUSED(retvalPtr);
   return;
}

static FUNC(void, COM_CODE) Com_EB_Read_BO_DL
(
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
)
{
   /* Deviation MISRAC2012-2 */
   P2VAR(boolean, AUTOMATIC, COM_APPL_DATA) SignalDataPtrBool =
         (P2VAR(boolean, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   uint16 const pos = Com_EB_ReadPropPtr->ComSignalPositionInBit / 8U;
   uint8 const bit = (uint8) Com_EB_ReadPropPtr->ComSignalPositionInBit % 8U;

   *SignalDataPtrBool = Com_IsBitSet_8(&dataPtr[pos], bit);

   TS_PARAM_UNUSED(retvalPtr);
   return;
}

static FUNC(void, COM_CODE) Com_EB_Read_A1_DL
(
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
)
{
   /* Deviation MISRAC2012-2 */
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) SignalDataPtrU8 =
         (P2VAR(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   uint16 const pos = Com_EB_ReadPropPtr->ComSignalPositionInBit;

   ((*SignalDataPtrU8) = (dataPtr[pos]));

   TS_PARAM_UNUSED(retvalPtr);
   return;
}

static FUNC(void, COM_CODE) Com_EB_Read_BO_EL
(
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
)
{
   /* Deviation MISRAC2012-2 */
   P2VAR(boolean, AUTOMATIC, COM_APPL_DATA) SignalDataPtrBool =
         (P2VAR(boolean, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   uint16 const pos = Com_EB_ReadPropPtr->ComSignalPositionInBit / 8U;
   uint8 const bit = (uint8) Com_EB_ReadPropPtr->ComSignalPositionInBit % 8U;

   *SignalDataPtrBool = Com_IsBitSet_8(&dataPtr[pos],bit);

   TS_PARAM_UNUSED(retvalPtr);
   return;
}

static FUNC(void, COM_CODE) Com_EB_Read_A1_EL
(
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
)
{
   /* Deviation MISRAC2012-2 */
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) SignalDataPtrU8 =
         (P2VAR(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   uint16 const pos = Com_EB_ReadPropPtr->ComSignalPositionInBit;

   TS_AtomicAssign8(*SignalDataPtrU8, dataPtr[pos]);

   TS_PARAM_UNUSED(retvalPtr);
   return;
}

static FUNC(void, COM_CODE) Com_EB_Read_DET
(
      P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
      P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
      P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
      P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
)
{
#if (COM_DEV_ERROR_DETECT == STD_ON)

     COM_DET_REPORTERROR(COM_E_PARAM_POINTER, COM_INTERNAL_API_ID);

#endif /* COM_DEV_ERROR_DETECT */

#if (COM_PRECONDITION_ASSERT_ENABLED == STD_ON)
   if(retvalPtr != NULL_PTR)
#endif /* (COM_PRECONDITION_ASSERT_ENABLED == STD_ON) */
   {
     *retvalPtr = COM_SERVICE_NOT_AVAILABLE;
   }

   TS_PARAM_UNUSED(SignalDataPtr);
   TS_PARAM_UNUSED(dataPtr);
   TS_PARAM_UNUSED(Com_EB_ReadPropPtr);
   return;
}


#if ((COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON) || (COM_HANDLESMALLERRXPDUS_NOTIFY == STD_ON))


#if (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON)
static FUNC(void, COM_CODE) Com_EB_ReadCopy_DET
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
)
{

#if (COM_DEV_ERROR_DETECT == STD_ON)
     COM_DET_REPORTERROR(COM_E_PARAM_POINTER, COM_INTERNAL_API_ID);
#endif /* COM_DEV_ERROR_DETECT */

     TS_PARAM_UNUSED(RxSignalPtr);
     TS_PARAM_UNUSED(IPduPtr);
     TS_PARAM_UNUSED(PduInfoPtr);
     TS_PARAM_UNUSED(BufferOffset);
     return;
}
#endif

#if (COM_RX_LITTLEENDIAN_ENABLE == STD_ON)

static FUNC(uint8, COM_CODE) Com_EB_ReadReceived_E1
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    uint32 Pdulength
)
{
  uint32 const firstBitOutside = Pdulength * 8U;

  /* check if signal is fully received */
  uint8 SignalIsReceived =
      (firstBitOutside > RxSignalPtr->ComSignalBase.ComSignalPositionInBit);

  return SignalIsReceived;
}
#endif


#if (COM_RX_BIGENDIAN_ENABLE == STD_ON)

static FUNC(uint32, COM_CODE) Com_EB_ConvertMSBPos2LSBPos
(
    uint32 ComBitSize,
    uint32 ComSignalPositionInBit
)
{
  /* get bit position of LSB */
  const uint32 shiftBits = (ComBitSize / 8U) * 8U;
  const uint32 msbTemp = ComSignalPositionInBit + shiftBits;
  const uint32 helpTemp = (msbTemp - ComBitSize) + 1U;
  const uint32 vaTemp = msbTemp / 8U;
  const uint32 vbTemp = helpTemp / 8U;
  const uint32 dTemp = vaTemp - vbTemp;
  const uint32 LSB = (helpTemp + (dTemp * 16)) - shiftBits;

  return LSB;
}

static FUNC(uint8, COM_CODE) Com_EB_ReadReceived_E2
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    uint32 Pdulength
)
{
  uint32 const firstBitOutside = Pdulength * 8U;

  uint32 const LSB = Com_EB_ConvertMSBPos2LSBPos(
      RxSignalPtr->ComSignalBase.ComBitSize,
      RxSignalPtr->ComSignalBase.ComSignalPositionInBit);

  /* check if signal is fully received */
  uint8 SignalIsReceived = (firstBitOutside > LSB);

  return SignalIsReceived;
}
#endif

static FUNC(uint8, COM_CODE) Com_EB_ReadReceived_AY
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    uint32 Pdulength
)
{
  uint8 SignalIsReceived = (Pdulength >=
      ((uint32)RxSignalPtr->ComSignalBase.ComSignalPositionInBit + RxSignalPtr->ComSignalBase.ComBitSize));

  return SignalIsReceived;
}

static FUNC(uint8, COM_CODE) Com_EB_ReadReceived_DYN
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    uint32 Pdulength
)
{
  uint8 SignalIsReceived = (Pdulength >= RxSignalPtr->ComSignalBase.ComSignalPositionInBit);

  return SignalIsReceived;
}

#if ((COM_RX_LITTLEENDIAN_ENABLE == STD_ON) && (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON))
static FUNC(void, COM_CODE) Com_EB_ReadCopySig_08_E1
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
)
{
  uint8 isRx;

  /* Create a destination signal */
  ComTxSignalType HlpSignal;

  /* Set signal parameter */
  COM_EB_CREATE_TX_SIG_CONF_FROM_RX_SIG_CHECK_E1(HlpSignal, RxSignalPtr, PduInfoPtr->SduLength, BufferOffset, isRx);

  if(isRx == TRUE)
  {
    /* Signal is received, copy value */
    COM_EB_SET_VALUE_IN_RX_BUFFER_08(HlpSignal, PduInfoPtr->SduDataPtr, Com_EBWriteIdx_08_SX_DL_DUB_DOC_E1);
  }

  COM_EB_HANDLE_UB(RxSignalPtr,IPduPtr,PduInfoPtr,isRx);
  return;
}
#endif

#if ((COM_RX_LITTLEENDIAN_ENABLE == STD_ON) && (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON))
static FUNC(void, COM_CODE) Com_EB_ReadCopySig_16_E1
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
)
{
  uint8 isRx;

  /* Create a destination signal */
  ComTxSignalType HlpSignal;

  /* Set signal parameter */
  COM_EB_CREATE_TX_SIG_CONF_FROM_RX_SIG_CHECK_E1(HlpSignal, RxSignalPtr, PduInfoPtr->SduLength, BufferOffset, isRx);

  if(isRx == TRUE)
  {
    /* Signal is received, copy value */
    COM_EB_SET_VALUE_IN_RX_BUFFER_16(HlpSignal, PduInfoPtr->SduDataPtr, Com_EBWriteIdx_16_SX_DL_DUB_DOC_E1);
  }

  COM_EB_HANDLE_UB(RxSignalPtr,IPduPtr,PduInfoPtr,isRx);
  return;
}
#endif

#if ((COM_RX_LITTLEENDIAN_ENABLE == STD_ON) && (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON))
static FUNC(void, COM_CODE) Com_EB_ReadCopySig_32_E1
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
)
{
  uint8 isRx;

  /* Create a destination signal */
  ComTxSignalType HlpSignal;

  /* Set signal parameter */
  COM_EB_CREATE_TX_SIG_CONF_FROM_RX_SIG_CHECK_E1(HlpSignal, RxSignalPtr, PduInfoPtr->SduLength, BufferOffset, isRx);

  if(isRx == TRUE)
  {
    /* Signal is received, copy value */
    COM_EB_SET_VALUE_IN_RX_BUFFER_32(HlpSignal, PduInfoPtr->SduDataPtr, Com_EBWriteIdx_32_SX_DL_DUB_DOC_E1);
  }

  COM_EB_HANDLE_UB(RxSignalPtr,IPduPtr,PduInfoPtr,isRx);
  return;
}
#endif

#if ((COM_RX_LITTLEENDIAN_ENABLE == STD_ON) && (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON))
static FUNC(void, COM_CODE) Com_EB_ReadCopySig_64_E1
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
)
{
  uint8 isRx;

  /* Create a destination signal */
  ComTxSignalType HlpSignal;

  /* Set signal parameter */
  COM_EB_CREATE_TX_SIG_CONF_FROM_RX_SIG_CHECK_E1(HlpSignal, RxSignalPtr, PduInfoPtr->SduLength, BufferOffset, isRx);

  if(isRx == TRUE)
  {
    /* Signal is received, copy value */
    COM_EB_SET_VALUE_IN_RX_BUFFER_64(HlpSignal, PduInfoPtr->SduDataPtr, Com_EBWriteIdx_64_SX_DL_DUB_DOC_E1);
  }

  COM_EB_HANDLE_UB(RxSignalPtr,IPduPtr,PduInfoPtr,isRx);
  return;
}
#endif

#if (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON)
static FUNC(void, COM_CODE) Com_EB_ReadCopySig_AY_EX
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
)
{
  uint8 isRx;

  isRx = Com_EB_ReadReceived_AY(RxSignalPtr, PduInfoPtr->SduLength);

  if(isRx == TRUE)
  {
    /* Signal is received, copy value */
    ComSignalDescBitType const startbyte = RxSignalPtr->ComSignalBase.ComSignalPositionInBit;

    CONSTP2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) valPtr = &PduInfoPtr->SduDataPtr[startbyte];

    CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) pdudataptr =
        &Com_gDataMemPtr[IPduPtr->ComIPduValueRef + startbyte];

    /* copy the data to the Pdu */
    TS_MemCpy(pdudataptr, valPtr, RxSignalPtr->ComSignalBase.ComBitSize);
  }

  COM_EB_HANDLE_UB(RxSignalPtr,IPduPtr,PduInfoPtr,isRx);

  TS_PARAM_UNUSED(BufferOffset);
  return;
}
#endif

#if (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON)
static FUNC(void, COM_CODE) Com_EB_ReadCopySig_AY_DYN
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
)
{

  sint32 delta = PduInfoPtr->SduLength - RxSignalPtr->ComSignalBase.ComSignalPositionInBit;

  if(delta > 0)
  {
    /* Signal is received, copy value */
    ComSignalDescBitType const startbyte = RxSignalPtr->ComSignalBase.ComSignalPositionInBit;

    CONSTP2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) valPtr = &PduInfoPtr->SduDataPtr[startbyte];

    CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) pdudataptr =
        &Com_gDataMemPtr[IPduPtr->ComIPduValueRef + startbyte];

    /* copy the data to the Pdu */
    TS_MemCpy(pdudataptr, valPtr, ((uint32)delta));
  }

#if (COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL)
  {
    uint8 isRx = 0U;
    if(delta >= 0)
    {
      isRx = 1U;
    }

    COM_EB_HANDLE_UB(RxSignalPtr,IPduPtr,PduInfoPtr,isRx);
  }
#endif

  TS_PARAM_UNUSED(BufferOffset);
  return;
}
#endif

#if ((COM_RX_BIGENDIAN_ENABLE == STD_ON) && (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON))
static FUNC(void, COM_CODE) Com_EB_ReadCopySig_08_E2
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
)
{
  uint8 isRx;

  /* Create a destination signal */
  ComTxSignalType HlpSignal;

  /* Set signal parameter */
  COM_EB_CREATE_TX_SIG_CONF_FROM_RX_SIG_CHECK_E2(HlpSignal, RxSignalPtr, PduInfoPtr->SduLength, BufferOffset, isRx);

  if(isRx == TRUE)
  {
    /* Signal is received, copy value */
    COM_EB_SET_VALUE_IN_RX_BUFFER_08(HlpSignal, PduInfoPtr->SduDataPtr, Com_EBWriteIdx_08_SX_DL_DUB_DOC_E2);
  }

  COM_EB_HANDLE_UB(RxSignalPtr,IPduPtr,PduInfoPtr,isRx);
  return;
}
#endif

#if ((COM_RX_BIGENDIAN_ENABLE == STD_ON) && (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON))
static FUNC(void, COM_CODE) Com_EB_ReadCopySig_16_E2
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
)
{
  uint8 isRx;

  /* Create a destination signal */
  ComTxSignalType HlpSignal;

  /* Set signal parameter */
  COM_EB_CREATE_TX_SIG_CONF_FROM_RX_SIG_CHECK_E2(HlpSignal, RxSignalPtr, PduInfoPtr->SduLength, BufferOffset, isRx);

  if(isRx == TRUE)
  {
    /* Signal is received, copy value */
    COM_EB_SET_VALUE_IN_RX_BUFFER_16(HlpSignal, PduInfoPtr->SduDataPtr, Com_EBWriteIdx_16_SX_DL_DUB_DOC_E2);
  }

  COM_EB_HANDLE_UB(RxSignalPtr,IPduPtr,PduInfoPtr,isRx);
  return;
}
#endif

#if ((COM_RX_BIGENDIAN_ENABLE == STD_ON) && (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON))
static FUNC(void, COM_CODE) Com_EB_ReadCopySig_32_E2
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
)
{
  uint8 isRx;

  /* Create a destination signal */
  ComTxSignalType HlpSignal;

  /* Set signal parameter */
  COM_EB_CREATE_TX_SIG_CONF_FROM_RX_SIG_CHECK_E2(HlpSignal, RxSignalPtr, PduInfoPtr->SduLength, BufferOffset, isRx);

  if(isRx == TRUE)
  {
    /* Signal is received, copy value */
    COM_EB_SET_VALUE_IN_RX_BUFFER_32(HlpSignal, PduInfoPtr->SduDataPtr, Com_EBWriteIdx_32_SX_DL_DUB_DOC_E2);
  }

  COM_EB_HANDLE_UB(RxSignalPtr,IPduPtr,PduInfoPtr,isRx);
  return;
}
#endif


#if ((COM_RX_BIGENDIAN_ENABLE == STD_ON) && (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON))
static FUNC(void, COM_CODE) Com_EB_ReadCopySig_64_E2
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
)
{
  uint8 isRx;

  /* Create a destination signal */
  ComTxSignalType HlpSignal;

  /* Set signal parameter */
  COM_EB_CREATE_TX_SIG_CONF_FROM_RX_SIG_CHECK_E2(HlpSignal, RxSignalPtr, PduInfoPtr->SduLength, BufferOffset, isRx);

  if(isRx == TRUE)
  {
    /* Signal is received, copy value */
    COM_EB_SET_VALUE_IN_RX_BUFFER_64(HlpSignal, PduInfoPtr->SduDataPtr, Com_EBWriteIdx_64_SX_DL_DUB_DOC_E2);
  }

  COM_EB_HANDLE_UB(RxSignalPtr,IPduPtr,PduInfoPtr,isRx);
  return;
}
#endif

#if (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON)
static FUNC(void, COM_CODE) Com_EB_ReadCopy_SetBoolVal
(
    P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) SduDataPtr,
    P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
    ComSignalDescBitType ComSignalPositionInBit
)
{
  /* Signal is received, copy value */
  ComSignalDescBitType const startbyte = (ComSignalDescBitType)((uint32)ComSignalPositionInBit / 8U);

  uint8 const bit = (uint8) ((uint32)ComSignalPositionInBit % 8U);

  CONSTP2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) valPtr = &SduDataPtr[startbyte];

  CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) pdudataptr =
      &dataPtr[startbyte];

  uint8 const Val = Com_IsBitSet_8(valPtr, bit);

  if (Val == 0U)
  {
     /* clear the bit */
    Com_ClearBitUInt8(pdudataptr, bit);
  }
  else
  {
     /* set the bit */
    Com_SetBitUInt8(pdudataptr, bit);
  }
}

static FUNC(void, COM_CODE) Com_EB_ReadCopySig_BO
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
)
{
  uint8 isRx;

  isRx = Com_EB_ReadReceived_E1(RxSignalPtr, PduInfoPtr->SduLength);

  if(isRx == TRUE)
  {
    /* Signal is received, copy value */
    Com_EB_ReadCopy_SetBoolVal(
        PduInfoPtr->SduDataPtr,
        &Com_gDataMemPtr[IPduPtr->ComIPduValueRef],
        RxSignalPtr->ComSignalBase.ComSignalPositionInBit);
  }

  COM_EB_HANDLE_UB(RxSignalPtr,IPduPtr,PduInfoPtr,isRx);

  TS_PARAM_UNUSED(BufferOffset);
  return;
}
#endif


#if (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON)
static FUNC(void, COM_CODE) Com_EB_ReadCopySig_A1
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
)
{
  uint8 isRx;

  isRx = Com_EB_ReadReceived_AY(RxSignalPtr, PduInfoPtr->SduLength);

  if(isRx == TRUE)
  {
    /* Signal is received, copy value */
    ComSignalDescBitType const startbyte =
        RxSignalPtr->ComSignalBase.ComSignalPositionInBit;

    CONSTP2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) valPtr =
          &PduInfoPtr->SduDataPtr[startbyte];

    CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) pdudataptr =
        &Com_gDataMemPtr[IPduPtr->ComIPduValueRef + startbyte];

    /* copy the data to the Pdu */
    pdudataptr[0] = valPtr[0];

  }

  COM_EB_HANDLE_UB(RxSignalPtr,IPduPtr,PduInfoPtr,isRx);

  TS_PARAM_UNUSED(BufferOffset);
  return;
}
#endif


static FUNC(uint8, COM_CODE) Com_EB_ReadReceived_DET
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    uint32 Pdulength
)
{
#if (COM_DEV_ERROR_DETECT == STD_ON)
     COM_DET_REPORTERROR(COM_E_PARAM_POINTER, COM_INTERNAL_API_ID);
#endif /* COM_DEV_ERROR_DETECT */

   TS_PARAM_UNUSED(RxSignalPtr);
   TS_PARAM_UNUSED(Pdulength);
   return 0U;
}

#if ((COM_RX_LITTLEENDIAN_ENABLE == STD_ON) && (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON))
static FUNC(void, COM_CODE) Com_EB_ReadCopyGSig_08_E1
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
)
{
  /* Create a destination signal */
  ComTxSignalType HlpSignal;

  /* Set signal parameter */
  COM_EB_CREATE_TX_SIG_CONF_FROM_RX_G_SIG_E1(HlpSignal, RxSignalPtr, IPduPtr, BufferOffset);

  /* Copy value */
  COM_EB_SET_VALUE_IN_RX_BUFFER_08(HlpSignal, PduInfoPtr->SduDataPtr, Com_EBWriteIdx_08_SX_DL_DUB_DOC_E1);
  return;
}
#endif

#if ((COM_RX_LITTLEENDIAN_ENABLE == STD_ON) && (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON))
static FUNC(void, COM_CODE) Com_EB_ReadCopyGSig_16_E1
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
)
{
  /* Create a destination signal */
  ComTxSignalType HlpSignal;

  /* Set signal parameter */
  COM_EB_CREATE_TX_SIG_CONF_FROM_RX_G_SIG_E1(HlpSignal, RxSignalPtr, IPduPtr, BufferOffset);

  /* Copy value */
  COM_EB_SET_VALUE_IN_RX_BUFFER_16(HlpSignal, PduInfoPtr->SduDataPtr, Com_EBWriteIdx_16_SX_DL_DUB_DOC_E1);
  return;
}
#endif

#if ((COM_RX_LITTLEENDIAN_ENABLE == STD_ON) && (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON))
static FUNC(void, COM_CODE) Com_EB_ReadCopyGSig_32_E1
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
)
{
  /* Create a destination signal */
  ComTxSignalType HlpSignal;

  /* Set signal parameter */
  COM_EB_CREATE_TX_SIG_CONF_FROM_RX_G_SIG_E1(HlpSignal, RxSignalPtr, IPduPtr, BufferOffset);

  /* Copy value */
  COM_EB_SET_VALUE_IN_RX_BUFFER_32(HlpSignal, PduInfoPtr->SduDataPtr, Com_EBWriteIdx_32_SX_DL_DUB_DOC_E1);
  return;
}
#endif

#if ((COM_RX_LITTLEENDIAN_ENABLE == STD_ON) && (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON))
static FUNC(void, COM_CODE) Com_EB_ReadCopyGSig_64_E1
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
)
{
  /* Create a destination signal */
  ComTxSignalType HlpSignal;

  /* Set signal parameter */
  COM_EB_CREATE_TX_SIG_CONF_FROM_RX_G_SIG_E1(HlpSignal, RxSignalPtr, IPduPtr, BufferOffset);

  /* Copy value */
  COM_EB_SET_VALUE_IN_RX_BUFFER_64(HlpSignal, PduInfoPtr->SduDataPtr, Com_EBWriteIdx_64_SX_DL_DUB_DOC_E1);
  return;
}
#endif

#if (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON)
static FUNC(void, COM_CODE) Com_EB_ReadCopyGSig_AY_EX
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
)
{
  /* Signal is received, copy value */
  ComSignalDescBitType const startbyte = RxSignalPtr->ComSignalBase.ComSignalPositionInBit;

  CONSTP2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) valPtr =
      &PduInfoPtr->SduDataPtr[startbyte + BufferOffset];

  CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) pdudataptr =
      &Com_gDataMemPtr[IPduPtr->ComIPduValueRef + startbyte + BufferOffset];

  /* copy the data to the Pdu */
  TS_MemCpy(pdudataptr, valPtr, RxSignalPtr->ComSignalBase.ComBitSize);

  return;
}
#endif

#if ((COM_RX_BIGENDIAN_ENABLE == STD_ON) && (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON))
static FUNC(void, COM_CODE) Com_EB_ReadCopyGSig_08_E2
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
)
{
  /* Create a destination signal */
  ComTxSignalType HlpSignal;

  /* Set signal parameter */
  COM_EB_CREATE_TX_SIG_CONF_FROM_RX_G_SIG_E2(HlpSignal, RxSignalPtr, IPduPtr, BufferOffset);

  /* Copy value */
  COM_EB_SET_VALUE_IN_RX_BUFFER_08(HlpSignal, PduInfoPtr->SduDataPtr, Com_EBWriteIdx_08_SX_DL_DUB_DOC_E2);
  return;
}
#endif

#if ((COM_RX_BIGENDIAN_ENABLE == STD_ON) && (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON))
static FUNC(void, COM_CODE) Com_EB_ReadCopyGSig_16_E2
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
)
{
  /* Create a destination signal */
  ComTxSignalType HlpSignal;

  /* Set signal parameter */
  COM_EB_CREATE_TX_SIG_CONF_FROM_RX_G_SIG_E2(HlpSignal, RxSignalPtr, IPduPtr, BufferOffset);

  /* Copy value */
  COM_EB_SET_VALUE_IN_RX_BUFFER_16(HlpSignal, PduInfoPtr->SduDataPtr, Com_EBWriteIdx_16_SX_DL_DUB_DOC_E2);
  return;
}
#endif

#if ((COM_RX_BIGENDIAN_ENABLE == STD_ON) && (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON))
static FUNC(void, COM_CODE) Com_EB_ReadCopyGSig_32_E2
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
)
{
  /* Create a destination signal */
  ComTxSignalType HlpSignal;

  /* Set signal parameter */
  COM_EB_CREATE_TX_SIG_CONF_FROM_RX_G_SIG_E2(HlpSignal, RxSignalPtr, IPduPtr, BufferOffset);

  /* Copy value */
  COM_EB_SET_VALUE_IN_RX_BUFFER_32(HlpSignal, PduInfoPtr->SduDataPtr, Com_EBWriteIdx_32_SX_DL_DUB_DOC_E2);
  return;
}
#endif

#if ((COM_RX_BIGENDIAN_ENABLE == STD_ON) && (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON))
static FUNC(void, COM_CODE) Com_EB_ReadCopyGSig_64_E2
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
)
{
  /* Create a destination signal */
  ComTxSignalType HlpSignal;

  /* Set signal parameter */
  COM_EB_CREATE_TX_SIG_CONF_FROM_RX_G_SIG_E2(HlpSignal, RxSignalPtr, IPduPtr, BufferOffset);

  /* Copy value */
  COM_EB_SET_VALUE_IN_RX_BUFFER_64(HlpSignal, PduInfoPtr->SduDataPtr, Com_EBWriteIdx_64_SX_DL_DUB_DOC_E2);
  return;
}
#endif

#if (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON)
static FUNC(void, COM_CODE) Com_EB_ReadCopyGSig_BO
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
)
{

  Com_EB_ReadCopy_SetBoolVal(
      &PduInfoPtr->SduDataPtr[BufferOffset],
      &Com_gDataMemPtr[IPduPtr->ComIPduValueRef + BufferOffset],
      RxSignalPtr->ComSignalBase.ComSignalPositionInBit);
  return;
}
#endif

#if (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON)
static FUNC(void, COM_CODE) Com_EB_ReadCopyGSig_A1
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
    uint32 BufferOffset
)
{
  ComSignalDescBitType const startbyte = RxSignalPtr->ComSignalBase.ComSignalPositionInBit;

  CONSTP2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) valPtr =
      &PduInfoPtr->SduDataPtr[startbyte + BufferOffset];

  CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) pdudataptr =
      &Com_gDataMemPtr[IPduPtr->ComIPduValueRef + startbyte + BufferOffset];

  /* copy the data to the Pdu */
  pdudataptr[0] = valPtr[0];

  return;
}
#endif

#endif /* ((COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON) || (COM_HANDLESMALLERRXPDUS_NOTIFY == STD_ON)) */

#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>

/*==================[end of file]============================================*/
