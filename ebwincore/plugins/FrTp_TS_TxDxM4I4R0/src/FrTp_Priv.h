/**
 * \file
 *
 * \brief AUTOSAR FrTp
 *
 * This file contains the implementation of the AUTOSAR
 * module FrTp.
 *
 * \version 4.4.26
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*  MISRA-C:2012 Deviation List
 *
 *  MISRAC2012-1) Deviated Rule: 20.10 (advisory)
 *  The # and ## preprocessor operators should not be used.
 *
 *   Reason:
 *   Unspecified evaluation order is not an issue here
 *
 */

#ifndef FRTP_PRIV_H
#define FRTP_PRIV_H

/*=====================[Include Section]=====================================*/

#include <SchM_FrTp.h>            /* scheduling manager */
#include <PduR_FrTp.h>            /* PduR API called by FrTp */
#include <FrTp_Api.h>             /* FrTp module interface header */ /* !LINKSTO FRTP1135_1,1 */

#if (FRTP_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>                  /* get development error tracer interface */
#endif /* (FRTP_DEV_ERROR_DETECT == STD_ON) */

#ifdef TS_RELOCATABLE_CFG_ENABLE
#error TS_RELOCATABLE_CFG_ENABLE is already defined
#endif
#define TS_RELOCATABLE_CFG_ENABLE FRTP_RELOCATABLE_CFG_ENABLE

#include <TSPBConfig.h>           /* relocatable post-build macros */
#include <TSPBConfig_Access.h>
#include <FrTp_guards.h> /* After foreign/API/Cfg, before internal/private */

/*=====================[defensive programming]===============================*/
#if (FRTP_DEV_ERROR_DETECT == STD_ON)

#if (defined FRTP_INSTANCE_ID)
#error FRTP_INSTANCE_ID is already defined
#endif
#define FRTP_INSTANCE_ID     0U

#endif /* FRTP_DEV_ERROR_DETECT */






/*------------------------[Defensive programming]----------------------------*/

#if (defined FRTP_PRECONDITION_ASSERT)
#error FRTP_PRECONDITION_ASSERT is already defined
#endif

#if (defined FRTP_PRECONDITION_ASSERT_NO_EVAL)
#error FRTP_PRECONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (FRTP_PRECONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define FRTP_PRECONDITION_ASSERT(Condition, ApiId) \
  DET_PRECONDITION_ASSERT((Condition), FRTP_MODULE_ID, FRTP_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define FRTP_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_PRECONDITION_ASSERT_NO_EVAL((Condition), FRTP_MODULE_ID, FRTP_INSTANCE_ID, (ApiId))
#else
#define FRTP_PRECONDITION_ASSERT(Condition, ApiId)
#define FRTP_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined FRTP_POSTCONDITION_ASSERT)
#error FRTP_POSTCONDITION_ASSERT is already defined
#endif

#if (defined FRTP_POSTCONDITION_ASSERT_NO_EVAL)
#error FRTP_POSTCONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (FRTP_POSTCONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define FRTP_POSTCONDITION_ASSERT(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT((Condition), FRTP_MODULE_ID, FRTP_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define FRTP_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT_NO_EVAL((Condition), FRTP_MODULE_ID, FRTP_INSTANCE_ID, (ApiId))
#else
#define FRTP_POSTCONDITION_ASSERT(Condition, ApiId)
#define FRTP_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined FRTP_INVARIANT_ASSERT)
#error FRTP_INVARIANT_ASSERT is already defined
#endif

#if (defined FRTP_INVARIANT_ASSERT_NO_EVAL)
#error FRTP_INVARIANT_ASSERT_NO_EVAL is already defined
#endif

#if (FRTP_INVARIANT_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define FRTP_INVARIANT_ASSERT(Condition, ApiId) \
  DET_INVARIANT_ASSERT((Condition), FRTP_MODULE_ID, FRTP_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define FRTP_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_INVARIANT_ASSERT_NO_EVAL((Condition), FRTP_MODULE_ID, FRTP_INSTANCE_ID, (ApiId))
#else
#define FRTP_INVARIANT_ASSERT(Condition, ApiId)
#define FRTP_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined FRTP_STATIC_ASSERT)
# error FRTP_STATIC_ASSERT is already defined
#endif
#if (FRTP_STATIC_ASSERT_ENABLED == STD_ON)
/** \brief Report an static assertion violation to Det
 **
 ** \param[in] Condition Condition which is violated */
# define FRTP_STATIC_ASSERT(expr) DET_STATIC_ASSERT(expr)
#else
# define FRTP_STATIC_ASSERT(expr)
#endif

#if (defined FRTP_UNREACHABLE_CODE_ASSERT)
#error FRTP_UNREACHABLE_CODE_ASSERT is already defined
#endif
#if (FRTP_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
/** \brief Report an unreachable code assertion violation to Det
 **

 ** \param[in] ApiId Service ID of the API function */
#define FRTP_UNREACHABLE_CODE_ASSERT(ApiId) \
  DET_UNREACHABLE_CODE_ASSERT(FRTP_MODULE_ID, FRTP_INSTANCE_ID, (ApiId))
#else
#define FRTP_UNREACHABLE_CODE_ASSERT(ApiId)
#endif

#if (defined FRTP_INTERNAL_API_ID)
#error FRTP_INTERNAL_API_ID is already defined
#endif
/** \brief API ID of module internal functions to be used in assertions */
#define FRTP_INTERNAL_API_ID DET_INTERNAL_API_ID



/*=====================[Global Macros]=======================================*/

#define FRTP_UNINIT 0U                          /* status value for uninitialized module */
#define FRTP_INIT   1U                          /* status value for initialized module */

/*
 * Instance ID for DET reporting
 */
#define FRTP_DET_GENERIC_INSTANCE_ID    0x0U   /* instance ID reported to DET and SchM */


/*
 * Invalid item literals
 */
#define FRTP_CONNECTION_INVALID         0xFFU  /* value for invalid connection index */
#define FRTP_TXPDU_INVALID              0xFFU  /* value for invalid TxPdu index */


#define FRTP_CONNECTION_HASACK          0x01U  /* bitmask for 'has acknowledge' config switch */
#define FRTP_CONNECTION_ISBROADCAST     0x02U  /* bitmask for '1-n' communication config switch */
#define FRTP_CONNECTION_HASBCLIMIT      0x04U  /* bitmask for 'bandwidthlimitation' config switch */

/*=====================[State machinemacros]=================================*/

/* State machine helper macros */
#define FRTP_CHNL_STATE_DIRECTION_MASK  0x80U   /* mask for direction indication */
#define FRTP_CHNL_STATE_DIRECTION_TX    0x00U   /* direction bit tx value */
#define FRTP_CHNL_STATE_DIRECTION_RX    0x80U   /* direction bit tx value */

#define FRTP_CHNL_STATE_MASK            0x8FU   /* mask for state values */
#define FRTP_CHNL_SUBSTATE_MASK         0x30U   /* mask for sub-state values */
#define FRTP_CHNL_RX_WAITTX_MASK        0x40U   /* mask for wait Tx-state */

/* macro extracts Tx-State from state variable */
#define FrTp_Chnl_GetTxState(state) \
            (FrTp_Chnl_IsTxState(state) ? \
                ((state)&FRTP_CHNL_STATE_MASK) : \
                FRTP_CHNL_TX_IDLE)

/* macro extracts Rx-State from state variable */
#define FrTp_Chnl_GetRxState(state) \
            (FrTp_Chnl_IsRxState(state) ? \
                ((state)&FRTP_CHNL_STATE_MASK) : \
                FRTP_CHNL_RX_IDLE)

#define FrTp_Chnl_GetRxSubState(state) \
            (FrTp_Chnl_IsRxState(state) ? \
                ((state)&(FRTP_CHNL_STATE_MASK|FRTP_CHNL_SUBSTATE_MASK)) : \
                FRTP_CHNL_RX_IDLE)

#define FrTp_Chnl_IsTxPending(state) ((boolean)(((state)&FRTP_CHNL_RX_WAITTX_MASK)!=0U))

/* macro returns whether the state variable is in Rx state */
#define FrTp_Chnl_IsRxState(state) (((state)&FRTP_CHNL_STATE_DIRECTION_MASK)!=0U)

/* macro returns whether the state variable is in Tx state */
#define FrTp_Chnl_IsTxState(state) (((state)&FRTP_CHNL_STATE_DIRECTION_MASK)==0U)

/*
 * FrTp transmit state machine states
 */
#define FRTP_CHNL_TX_IDLE           0x00U   /* idle state of TX state machine */
#define FRTP_CHNL_TX_DATA           0x01U   /* transmission of data state */
#define FRTP_CHNL_TX_WAIT_TX        0x02U   /* waiting for transmission completed state */
#define FRTP_CHNL_TX_WAIT_FC        0x03U   /* waiting for reception of flow control */
#define FRTP_CHNL_TX_WAIT_ACK       0x04U   /* waiting for reception of acknowledge */
#define FRTP_CHNL_TX_ABORT_PENDING  0x7fU   /* waiting for pending TxConfirmations before aborting */

/*
 * FrTp receive state machine states
 */
 /* idle state of RX state machine */
#define FRTP_CHNL_RX_IDLE               FRTP_CHNL_TX_IDLE
/* receiving data */
#define FRTP_CHNL_RX_DATA              (FRTP_CHNL_STATE_DIRECTION_RX|0x01U)
/* transmit an FC ABORT (no indication) */
#define FRTP_CHNL_RX_ABORT             (FRTP_CHNL_STATE_DIRECTION_RX|0x02U)
/* transmit an FC ABORT (indicate: wrong SN) */
#define FRTP_CHNL_RX_ABORT_WRONG_SN    (FRTP_CHNL_STATE_DIRECTION_RX|0x12U)
/* transmit an FC ABORT (indicate: ML mismatch)*/
#define FRTP_CHNL_RX_ABORT_ML_MISMATCH (FRTP_CHNL_STATE_DIRECTION_RX|0x22U)
/* transmit an FC ABORT (indicate: NOT OK)*/
#define FRTP_CHNL_RX_ABORT_NO_BUFFER   (FRTP_CHNL_STATE_DIRECTION_RX|0x32U)
/* transmit an FC ACK */
#define FRTP_CHNL_RX_ACK               (FRTP_CHNL_STATE_DIRECTION_RX|0x03U)
/* transmit an FC CTS */
#define FRTP_CHNL_RX_CTS               (FRTP_CHNL_STATE_DIRECTION_RX|0x04U)
/* transmit an FC RETRY */
#define FRTP_CHNL_RX_RETRY             (FRTP_CHNL_STATE_DIRECTION_RX|0x05U)
/* Prepare an FC RETRY */
#define FRTP_CHNL_RX_STF_RETRY         (FRTP_CHNL_STATE_DIRECTION_RX|0x08U)
/* transmit an FC OVFL */
#define FRTP_CHNL_RX_OVFL              (FRTP_CHNL_STATE_DIRECTION_RX|0x06U)
/* transmit an FC WAIT */
#define FRTP_CHNL_RX_WAIT              (FRTP_CHNL_STATE_DIRECTION_RX|0x07U)


/*
 * usage types of tx Pdus
 */
#define FRTP_TXPDUTYPE_DATA_SF      0x00U   /* this Pdu contains a Start Frame (SF) */
#define FRTP_TXPDUTYPE_DATA_CF      0x01U   /* this Pdu contains a Consecutive Frame (CF) */
#define FRTP_TXPDUTYPE_DATA_CFEOB   0x02U   /* this Pdu contains an End-Of-Block CF (CFEOB) */
#define FRTP_TXPDUTYPE_DATA_LF      0x03U   /* this Pdu contains a Last Frame (LF) */
#define FRTP_TXPDUTYPE_FC           0x04U   /* this Pdu contains a Flow Control (FC) */

/*=====================[half duplex helper macros]===========================*/

/* in case of half-duplex, we re-use the Tx related variables also for Rx state machine */
#if (FRTP_FULLDUPLEX_ENABLE == STD_OFF)

/* renaming of generic state machine variables */
#ifdef Rx_State
  #error Rx_State already defined
#endif
#define Rx_State            Tx_State

#ifdef Rx_Timer1
  #error Rx_Timer1 already defined
#endif
#define Rx_Timer1           Tx_Timer1

#ifdef Rx_Timer2
  #error Rx_Timer2 already defined
#endif
#define Rx_Timer2           Tx_Timer2

#ifdef Rx_PCI
  #error Rx_PCI already defined
#endif
#define Rx_PCI              Tx_PCI

#ifdef Rx_ML
  #error Rx_ML already defined
#endif
#define Rx_ML               Tx_ML

#ifdef Rx_CtrDataAck
  #error Rx_CtrDataAck already defined
#endif
#define Rx_CtrDataAck       Tx_CtrDataAck

#ifdef Rx_BfS
  #error Rx_BfS already defined
#endif
#define Rx_BfS              Tx_BfS

#ifdef Rx_CtrBlockReceived
  #error Rx_CtrBlockReceived already defined
#endif
#define Rx_CtrBlockReceived Tx_CtrBlockTransmit

#ifdef Rx_CtrWait
  #error Rx_CtrWait already defined
#endif
#define Rx_CtrWait          Tx_CtrBlockTriggerTransmit

#ifdef Rx_CtrRetry
  #error Rx_CtrRetry already defined
#endif
#define Rx_CtrRetry         Tx_CtrTxPdusPending

#endif /* FRTP_FULLDUPLEX_ENABLE */


/*=====================[Tx message construction macros]======================*/


/* PCI type value definition */
/* startframes */
#define FRTP_PCITYPE_STFU       0x40U /* start frame, unacknowledged */
#define FRTP_PCITYPE_STFA       0x41U /* start frame, acknowledged */

/* consecutive frames */
#define FRTP_PCITYPE_CF1        0x50U /* consecutive frame: 1 */
#define FRTP_PCITYPE_CF2        0x60U /* consecutive frame: 2 */
#define FRTP_PCITYPE_EOB        0x70U /* consecutive frame: End of block */

/* flow control frames */
#define FRTP_PCITYPE_FC         0x80U /* Flow Control: general */
#define FRTP_PCITYPE_FC_CTS     0x83U /* Flow Control: Continue To Send */
#define FRTP_PCITYPE_FC_ACK_RET 0x84U /* Flow Control: Retry */
#define FRTP_PCITYPE_FC_WT      0x85U /* Flow Control: Wait */
#define FRTP_PCITYPE_FC_ABT     0x86U /* Flow Control: Abort */
#define FRTP_PCITYPE_FC_OVFLW   0x87U /* Flow Control: Overflow */

/* last frame */
#define FRTP_PCITYPE_LF         0x90U /* last frame */

/* ACK_RET ACK field decoding values */
#define FRTP_PCITYPE_ACK_RET_ACK   0x00U /* ACK-field: acknowlesge */
#define FRTP_PCITYPE_ACK_RET_RETRY 0x01U /* ACK-field: retry */

#define FRTP_PCITYPE_SN_MASK  0x0FU
#define FRTP_PCITYPE_CF_MASK  0xF0U
#define FRTP_PCITYPE_CFI_MASK  0x10U

/* helper macro calculating the maximum FPL value for a specific frame type */
#define FrTp_Max_FPL_STF (254U - FrTp_Size_Header_STF)

/* helper macros for the maximum FPL value for a specific frame type */
/* values are hard coded since sizeof(struct) might cause problems on specific platforms */
#define FrTp_Size_Header_FC_ACK_RET ((PduLengthType)8U)
#define FrTp_Size_Header_FC_CTS     ((PduLengthType)8U)
#define FrTp_Size_Header_FC         ((PduLengthType)5U)
#define FrTp_Size_Header_STF        ((PduLengthType)8U)
#define FrTp_Size_Header_CF         ((PduLengthType)6U)
#define FrTp_Size_Header_LF         ((PduLengthType)8U)
#define FrTp_Size_Header_STF_LF     ((PduLengthType)8U)
#define FrTp_Size_Header_PCI        ((PduLengthType)5U)

/* FrTp Header Field Offsets */
#define FrTp_Header_Offset_AI_TA_High       0U   /* target adress high byte */
#define FrTp_Header_Offset_AI_TA_Low        1U   /* target address low byte */
#define FrTp_Header_Offset_AI_SA_High       2U   /* source address high byte */
#define FrTp_Header_Offset_AI_SA_Low        3U   /* source address low byte */
#define FrTp_Header_Offset_PCI_Type         4U   /* PCI field */
#define FrTp_Header_Offset_FPL              5U   /* frame payload length */
#define FrTp_Header_Offset_ML_High          6U   /* message length high byte */
#define FrTp_Header_Offset_ML_Low           7U   /* message length low byte */
#define FrTp_Header_Offset_BC               5U   /* bandwith control */
#define FrTp_Header_Offset_BfS_High         6U   /* Buffersize high byte */
#define FrTp_Header_Offset_BfS_Low          7U   /* Buffersize low byte */
#define FrTp_Header_Offset_ACK              5U   /* acknowledge type */
#define FrTp_Header_Offset_BP_High          6U   /* Byte position high byte */
#define FrTp_Header_Offset_BP_Low           7U   /* Byte position low byte */

/* set address information field into a buffer */
#define FrTp_SetAI(pBuffer,targetAddress,sourceAddress)                                           \
        do {                                                                                      \
            const uint16 ta = (uint16)(targetAddress);                                            \
            const uint16 sa = (uint16)(sourceAddress);                                            \
            CONSTP2VAR(uint8,AUTOMATIC,FRTP_APPL_DATA) pBuf =                                     \
                (P2VAR(uint8, AUTOMATIC, FRTP_APPL_DATA))(pBuffer);                              \
            FrTp_Uint16ToField(pBuf[FrTp_Header_Offset_AI_TA_High],                               \
                               pBuf[FrTp_Header_Offset_AI_TA_Low],ta);                            \
            FrTp_Uint16ToField(pBuf[FrTp_Header_Offset_AI_SA_High],                               \
                               pBuf[FrTp_Header_Offset_AI_SA_Low],sa);                            \
        }while(0)

/* macro constructs a STF header information */
#define FrTp_SetPCI_SF(pBuffer,hasAck,payloadLength,messageLength) \
            FrTp_SetPCI_SF_Hlp(pBuffer,hasAck,payloadLength,messageLength)

/* helper macro constructs a STF header information */
#define FrTp_SetPCI_SF_Hlp(pBuffer,hasAck,payloadLength,messageLength)                            \
        do {                                                                                      \
            const uint16 ml = (uint16)(messageLength);                                            \
            const uint8 pl = (uint8)(payloadLength);                                              \
            CONSTP2VAR(uint8,AUTOMATIC,FRTP_APPL_DATA) pBuf =                                     \
                 (P2VAR(uint8, AUTOMATIC, FRTP_APPL_DATA))(pBuffer);                              \
            pBuf[FrTp_Header_Offset_PCI_Type] =                                                   \
                ((hasAck) ? FRTP_PCITYPE_STFA : FRTP_PCITYPE_STFU);                               \
            pBuf[FrTp_Header_Offset_FPL] =  pl;                                                   \
            FrTp_Uint16ToField(pBuf[FrTp_Header_Offset_ML_High],                                  \
                               pBuf[FrTp_Header_Offset_ML_Low],ml);                               \
        }while(0)


/* constructs a consecutive frame PCI information */
#define FrTp_SetPCI_CF(pBuffer,payloadLength,protocolControlInformation)                          \
        do {                                                                                      \
            const uint8 pl = (uint8)(payloadLength);                                              \
            const uint8 pci = (uint8)(protocolControlInformation);                                \
            CONSTP2VAR(uint8,AUTOMATIC,FRTP_APPL_DATA) pBuf =                                     \
                 (P2VAR(uint8, AUTOMATIC, FRTP_APPL_DATA))(pBuffer);                              \
            pBuf[FrTp_Header_Offset_PCI_Type] =  pci;                                             \
            pBuf[FrTp_Header_Offset_FPL] =  pl;                                                   \
        }while(0)


/* constructs a last frame PCI information */
#define FrTp_SetPCI_LF(pBuffer,payloadLength,messageLength)                                       \
        do {                                                                                      \
            const uint16 ml = (uint16)(messageLength);                                            \
            const uint8 pl = (uint8)(payloadLength);                                              \
            CONSTP2VAR(uint8,AUTOMATIC,FRTP_APPL_DATA) pBuf =                                     \
                 (P2VAR(uint8, AUTOMATIC, FRTP_APPL_DATA))(pBuffer);                              \
            pBuf[FrTp_Header_Offset_PCI_Type] = (FRTP_PCITYPE_LF);                                \
            pBuf[FrTp_Header_Offset_FPL] =  pl;                                                   \
            FrTp_Uint16ToField(pBuf[FrTp_Header_Offset_ML_High],                                  \
                               pBuf[FrTp_Header_Offset_ML_Low],ml);                               \
        }while(0)


/* constructs a CTS frame */
#define FrTp_SetPCI_CTS(pBuffer,bandwith,buffersize)                                              \
        do {                                                                                      \
            const uint16 bfs = (uint16)(buffersize);                                              \
            const uint8 bwc = (uint8)(bandwith);                                                  \
            CONSTP2VAR(uint8,AUTOMATIC,FRTP_APPL_DATA) pBuf =                                     \
                 (P2VAR(uint8, AUTOMATIC, FRTP_APPL_DATA))(pBuffer);                              \
            pBuf[FrTp_Header_Offset_PCI_Type] = (FRTP_PCITYPE_FC_CTS);                            \
            pBuf[FrTp_Header_Offset_BC] = bwc;                                                    \
            FrTp_Uint16ToField(pBuf[FrTp_Header_Offset_BfS_High],                                 \
                               pBuf[FrTp_Header_Offset_BfS_Low],bfs);                             \
        }while(0)

/* constructs an acknowledge frame */
#define FrTp_SetPCI_ACK(pBuffer)                                                                  \
        do {                                                                                      \
            CONSTP2VAR(uint8,AUTOMATIC,FRTP_APPL_DATA) pBuf =                                     \
                 (P2VAR(uint8, AUTOMATIC, FRTP_APPL_DATA))(pBuffer);                              \
            pBuf[FrTp_Header_Offset_PCI_Type] = (FRTP_PCITYPE_FC_ACK_RET);                        \
            pBuf[FrTp_Header_Offset_ACK] = 0x00U;                                                 \
            pBuf[FrTp_Header_Offset_BP_High] = 0x00U;                                             \
            pBuf[FrTp_Header_Offset_BP_Low] = 0x00U;                                              \
        }while(0)

/* constructs an retry frame */
#define FrTp_SetPCI_RET(pBuffer,bufferposition)                                                   \
        do {                                                                                      \
            const uint16 bp = (uint16)(bufferposition);                                           \
            CONSTP2VAR(uint8,AUTOMATIC,FRTP_APPL_DATA) pBuf =                                     \
                 (P2VAR(uint8, AUTOMATIC, FRTP_APPL_DATA))(pBuffer);                              \
            pBuf[FrTp_Header_Offset_PCI_Type] = (FRTP_PCITYPE_FC_ACK_RET);                        \
            pBuf[FrTp_Header_Offset_ACK] = FRTP_PCITYPE_ACK_RET_RETRY;                            \
            FrTp_Uint16ToField(pBuf[FrTp_Header_Offset_BP_High],                                  \
                               pBuf[FrTp_Header_Offset_BP_Low],bp);                               \
        }while(0)

/* construct an abort frame */
#define FrTp_SetPCI_ABT(pBuffer) \
            ((P2VAR(uint8, AUTOMATIC, FRTP_APPL_DATA))(pBuffer))[FrTp_Header_Offset_PCI_Type] = \
                                                                              (FRTP_PCITYPE_FC_ABT)

/* construct an overflow frame */
#define FrTp_SetPCI_OVFLW(pBuffer) \
            ((P2VAR(uint8, AUTOMATIC, FRTP_APPL_DATA))(pBuffer))[FrTp_Header_Offset_PCI_Type] = \
                                                                             (FRTP_PCITYPE_FC_OVFLW)

/* construct a wait frame */
#define FrTp_SetPCI_WAIT(pBuffer) \
            ((P2VAR(uint8, AUTOMATIC, FRTP_APPL_DATA))(pBuffer))[FrTp_Header_Offset_PCI_Type] = \
                                                                                (FRTP_PCITYPE_FC_WT)




/* extracts the target address out of a frame */
#define FrTp_GetAI_Target(pBuffer)  \
                FrTp_FieldToUint16(((P2CONST(uint8, AUTOMATIC, FRTP_APPL_DATA))(pBuffer))          \
                                                                   [FrTp_Header_Offset_AI_TA_High],\
                                   ((P2CONST(uint8, AUTOMATIC, FRTP_APPL_DATA))(pBuffer))          \
                                                                     [FrTp_Header_Offset_AI_TA_Low])

/* extracts the source address out of a frame */
#define FrTp_GetAI_Source(pBuffer)  \
            FrTp_FieldToUint16(((P2CONST(uint8, AUTOMATIC, FRTP_APPL_DATA))(pBuffer))              \
                                                                   [FrTp_Header_Offset_AI_SA_High],\
                               ((P2CONST(uint8, AUTOMATIC, FRTP_APPL_DATA))(pBuffer))              \
                                                                     [FrTp_Header_Offset_AI_SA_Low])

/* extracts the PCI out of a frame */
#define FrTp_GetPCI(pBuffer) \
    (((P2CONST(uint8, AUTOMATIC, FRTP_APPL_DATA))(pBuffer))[FrTp_Header_Offset_PCI_Type])

/* extracts the message length out of a frame */
#define FrTp_GetML(pBuffer)  \
            FrTp_FieldToUint16(((P2CONST(uint8, AUTOMATIC, FRTP_APPL_DATA))(pBuffer))              \
                                                                      [FrTp_Header_Offset_ML_High],\
                               ((P2CONST(uint8, AUTOMATIC, FRTP_APPL_DATA))(pBuffer))              \
                                                                        [FrTp_Header_Offset_ML_Low])

/* extracts the FPL out of a frame */
#define FrTp_GetFPL(pBuffer) (((P2CONST(uint8, AUTOMATIC, FRTP_APPL_DATA))(pBuffer))               \
                                                                           [FrTp_Header_Offset_FPL])

/* extracts the payload start address out of a frame */
#define FrTp_GetPayload_STF(pBuffer)    (&(pBuffer)[FrTp_Size_Header_STF])
#define FrTp_GetPayload_CF(pBuffer)     (&(pBuffer)[FrTp_Size_Header_CF])
#define FrTp_GetPayload_LF(pBuffer)     (&(pBuffer)[FrTp_Size_Header_LF])

/* extracts the acknowledge type information out of a frame */
#define FrTp_GetPCIAckType(pBuffer) \
    (((P2CONST(uint8, AUTOMATIC, FRTP_APPL_DATA))(pBuffer))[FrTp_Header_Offset_ACK])

/* extracts the byte position information out of a frame */
#define FrTp_GetAckRet_BP(pBuffer)  \
            FrTp_FieldToUint16(((P2CONST(uint8, AUTOMATIC, FRTP_APPL_DATA))(pBuffer))              \
                                                                     [FrTp_Header_Offset_BP_High], \
                               ((P2CONST(uint8, AUTOMATIC, FRTP_APPL_DATA))(pBuffer))              \
                                                                        [FrTp_Header_Offset_BP_Low])

/* extracts the buffersize information out of a frame */
#define FrTp_GetCTS_BfS(pBuffer)  \
            FrTp_FieldToUint16(((P2CONST(uint8, AUTOMATIC, FRTP_APPL_DATA))(pBuffer))              \
                                                                    [FrTp_Header_Offset_BfS_High], \
                               ((P2CONST(uint8, AUTOMATIC, FRTP_APPL_DATA))(pBuffer))              \
                                                                       [FrTp_Header_Offset_BfS_Low])

/* extracts the bandwidth control information out of a frame */
#define FrTp_GetCTS_BC(pBuffer) (((P2CONST(uint8, AUTOMATIC, FRTP_APPL_DATA))(pBuffer))            \
                                                                            [FrTp_Header_Offset_BC])
/*=====================[Function like helper macros]=========================*/

/* set uint16 values to frame header */
#define FrTp_Uint16ToField(fieldname_hi, fieldname_lo,value)  \
        do {                                                  \
            (fieldname_hi) = (uint8)((value)>>8U);            \
            (fieldname_lo) = (uint8)((value)>>0U);            \
        } while(0)

/* set uint16 values to frame header */
#define FrTp_FieldToUint16(fieldname_hi, fieldname_lo) \
            ((uint16)((((uint32)(fieldname_hi))<<8U)|((uint32)(fieldname_lo))))


/* helper macro limits a value to the ranges of a type - uint16 implementation */
#define FrTp_Limit_uint8(value) (((value) > 0xFFU) ? 0xFFU : ((uint8)(value)))

/* helper macro limits a value to the ranges of a type - uint16 implementation */
#define FrTp_Limit_uint16(value) (((value) > 0xFFFFU) ? 0xFFFFU : ((uint16)(value)))

/* helper macro limits a value to the ranges of a type - uint16 implementation */
#define FrTp_Limit_uint32(value) (((value) > 0xFFFFFFFFUL) ? 0xFFFFFFFFUL : ((uint32)(value)))

/* Set timer to a start value */
#define FrTp_StartTimer(timer, time) TS_AtomicAssign16((timer), (time))

/* Stop timer */
#define FrTp_StopTimer(timer) TS_AtomicAssign16((timer), (0U))

#ifdef FRTP_PRECOMPILE_TIME_PBCFG_ACCESS

#define FRTP_PBCFG_ACCESS(type, element)\
(FrTp_ConfigLayout->a ## element)

#else

/* Deviation MISRAC2012-1 */
#define FRTP_PBCFG_ACCESS(type, element)\
(TS_UNCHECKEDGETCFG(FrTp_RootPtr, type, FRTP, (FrTp_RootPtr->p ## element)))

#endif /* FRTP_PRECOMPILE_TIME_PBCFG_ACCESS */

#define FRTP_CFG_GET_PTR_TO_CONNECTION(Index) \
(&FRTP_PBCFG_ACCESS(FrTp_ConnectionType, Connections)[(Index)])

#define FRTP_CFG_GET_PTR_TO_TXPDU(Index) \
(&FRTP_PBCFG_ACCESS(FrTp_TxPduConfigType, TxPdus)[(Index)])

#define FRTP_CFG_GET_TXCONFLOOKUPTABLE(Index) \
(FRTP_PBCFG_ACCESS(uint8, TxConfLookupTable)[(Index)])

#if (FRTP_LIMIT_NUMBER_CONNECTIONS == STD_ON)
#define FRTP_CFG_GET_PTR_TO_CONNECTION_LIMIT_CONFIG(Index) \
(&FRTP_PBCFG_ACCESS(FrTp_ConnectionLimitConfigType, ConnectionLimitConfigs)[(Index)])
#endif /* FRTP_LIMIT_NUMBER_CONNECTIONS */

/*=====================[Global Data Types]===================================*/

/*
 * channel data type (RAM)
 */
typedef struct
{
    /* TX related members */
    volatile VAR(uint16,TYPEDEF) Tx_Timer1;              /**< Tx Timer 1 counter variable (Timer AS) */
    volatile VAR(uint16,TYPEDEF) Tx_Timer2;              /**< Tx Timer 2 counter variable (Timer BS) */
    volatile VAR(uint16,TYPEDEF) Tx_ML;                  /**< Tx message length */
    volatile VAR(uint16,TYPEDEF) Tx_CtrDataAck;          /**< Tx bytes already acknowledged by receiver */
    volatile VAR(uint16,TYPEDEF) Tx_BfS;                 /**< Tx current block size */
    volatile VAR(uint16,TYPEDEF) Tx_BfSPduR;             /**< Tx PduR buffer size */
    volatile VAR(uint16,TYPEDEF) Tx_CtrBlockTransmit;    /**< Tx number of bytes transmitted for current block */
    volatile VAR(uint16,TYPEDEF) Tx_CtrBlockTriggerTransmit;
    volatile VAR(uint16,TYPEDEF) Tx_Retry;               /**< >0 means that channel is in retry state (next frame is a retry) and
                                                      value is byte position received in RETRY frame*/
    volatile VAR(uint16,TYPEDEF) Tx_CtrBufReq;           /**< Counts number of retries to PduR_FrTpCopyTxData
                                                     in case the buffer is temporarily unavailable*/

    /* RX related members */
    volatile VAR(uint16,TYPEDEF) Rx_Timer3;               /**< Rx Timer 1 counter variable (Timer BR) */

    volatile VAR(uint16,TYPEDEF)  Tx_BCMinCycleDistance;  /**< Tx minimum cycle distance
                                                     between 2 burst transmissions */

    volatile VAR(uint16,TYPEDEF)  Tx_BCCtrCycleDistance;  /**< number of cycles for which this channel
                                                     must not transmit */
#if (FRTP_FULLDUPLEX_ENABLE == STD_ON)

    volatile VAR(uint16,TYPEDEF) Rx_Timer1;               /**< Rx Timer 1 counter variable (Timer AR) */
    volatile VAR(uint16,TYPEDEF) Rx_Timer2;               /**< Rx Timer 1 counter variable (Timer CR) */
    volatile VAR(uint16,TYPEDEF) Rx_ML;                   /**< Rx message length */
    volatile VAR(uint16,TYPEDEF) Rx_CtrDataAck;           /**< Rx number of bytes received
                                                      and to be acknowledged */
    volatile VAR(uint16,TYPEDEF) Rx_BfS;                  /**< Rx current block size */
    volatile VAR(uint16,TYPEDEF) Rx_CtrBlockReceived;     /**< Rx Number of bytes received within the
                                                      current block */
    volatile VAR(uint16,TYPEDEF) Rx_CtrWait;              /**< Rx number of wait FC messages
                                                      already transmitted */
    volatile VAR(uint8,TYPEDEF)  Rx_PCI;                  /**< Rx consecutive frame PCI (CF1 vs. CF2, SN)*/
    volatile VAR(uint8,TYPEDEF)  Rx_CtrRetry;             /**< Rx number of retry messages already
                                                      transmitted for this block */
    volatile VAR(uint8,TYPEDEF)  Rx_State;                /**< Rx State machine state variable */

#endif /* FRTP_FULLDUPLEX_ENABLE */

    /* TX related members */
    volatile VAR(uint8,TYPEDEF)  Tx_State;               /**< Tx State machine state variable */
    volatile VAR(uint8,TYPEDEF)  Tx_PCI;                 /**< Tx consecutive frame PCI (CF1 vs. CF2, SN)*/
    volatile VAR(uint8,TYPEDEF)  Tx_CtrTxPdusPending;    /**< Tx number of Pdus pending for transmission */

    volatile VAR(uint8,TYPEDEF)  Tx_FrIfRetry;            /**< Number of Tx retries to FrIf_Transmit in case
                                                      it returns E_NOT_OK for SF, CF and LF */

    volatile VAR(uint8,TYPEDEF)  TxFc_FrIfRetry;          /**< Number of Tx retries to FrIf_Transmit in case
                                                      it returns E_NOT_OK for FC frames. */

    volatile VAR(uint8,TYPEDEF)  Tx_BCMaxPduPerCycle;    /**< maximum number of Pdus to
                                                     transmit per cycle */
    volatile VAR(uint8,TYPEDEF)  Tx_BCCtrPdu;            /**< number of TxPdus which are
                                                     transmitted for this cycle */

    volatile VAR(uint8,TYPEDEF)  RxHasAck;                /**< This message expects an acknowledge */

    /* common (TX/RX)) members */
    volatile VAR(uint8,TYPEDEF) ConnectionIdx;           /**< index of connection associated
                                                     with this channel */

#if (FRTP_COPY_STF_LOCALBUFFER == STD_ON)
    volatile VAR(uint8,TYPEDEF) STFCopied;                /**< Total bytes of STF frame payload copied */
    VAR(uint8,TYPEDEF) LocalSTFBuffer[FRTP_MAXSIZE_LOCALBUFFER];
                                                /**< Buffer to hold the STF frame payload */
#endif

} FrTp_ChannelType;

/* TxPdu administration data (RAM) */
typedef struct
{
    P2VAR(FrTp_ChannelType,TYPEDEF,FRTP_VAR) pChannel;  /* TxPdu is allocated to this channel */
    VAR(uint8,TYPEDEF) PayloadLength;                   /* Payload length (FPL) used
                                                           for transmission */
    VAR(uint8,TYPEDEF) PduType;                         /* usage of this Pdu
                                                           (e.g. STF, FC, CF, ...) */
}FrTp_RuntimeTxPduType;

#if (FRTP_LIMIT_NUMBER_CONNECTIONS == STD_ON)

/* Connection limit index type */
typedef FRTP_CONNECTION_LIMIT_CONFIG_IDX_TYPE FrTp_ConnectionLimitConfigIdxType;

/* A buffered connection, i.e. one element of the buffer */
typedef struct
{
    VAR(PduIdType, FRTP_VAR) TxSduId;
    VAR(PduLengthType, FRTP_VAR) SduLength;
} FrTp_BufferedConnectionType;

/* Information about one remote address' buffer, i.e. size and head and tail index */
typedef struct
{
    P2VAR(FrTp_BufferedConnectionType, AUTOMATIC, FRTP_VAR) BufferPtr;
    VAR(uint8, FRTP_VAR) IdxHead;
    VAR(uint8, FRTP_VAR) IdxTail;
    VAR(uint8, FRTP_VAR) BufferSize;
} FrTp_BufferInfoType;

/* Connection buffer type (RAM) */
typedef struct
{
#if (FRTP_SUM_BUFFER_SIZE > 0)
    VAR(FrTp_BufferedConnectionType, FRTP_VAR) Connections[FRTP_SUM_BUFFER_SIZE];
#else
    /* Dummy variable in case there are no buffers configured */
    P2VAR(FrTp_BufferedConnectionType, AUTOMATIC, FRTP_VAR) Connections;
#endif /* (FRTP_SUM_BUFFER_SIZE > 0) */

    VAR(FrTp_BufferInfoType, FRTP_VAR) BufferInfos[FRTP_NUMBER_CONNECTION_BUFFERS];
} FrTp_ConnectionBufferType;

#endif /* FRTP_LIMIT_NUMBER_CONNECTIONS */

/*=====================[Global Data Prototypes]==============================*/

/*
 * Start variable section declaration
 */
#define FRTP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <FrTp_MemMap.h>

/* channel runtime data variable declaration */
extern VAR(FrTp_ChannelType,FRTP_VAR) FrTp_Channel[FRTP_CHAN_NUM];

/* TxPdu runtime data variable declaration */
extern VAR(FrTp_RuntimeTxPduType,FRTP_VAR) FrTp_RuntimeTxPdu[FRTP_TXPDU_NUM];

#if (FRTP_LIMIT_NUMBER_CONNECTIONS == STD_ON)

/* Connection buffer declaration */
extern VAR(FrTp_ConnectionBufferType, FRTP_VAR) FrTp_ConnectionBuffer;

#endif /* FRTP_LIMIT_NUMBER_CONNECTIONS */

/*
 * Stop variable section declaration
 */
#define FRTP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <FrTp_MemMap.h>

/*
 * Start variable section declaration
 */
#define FRTP_START_SEC_VAR_CLEARED_8
#include <FrTp_MemMap.h>

#if (FRTP_LIMIT_NUMBER_CONNECTIONS == STD_ON)
#if (FRTP_NUMBER_CONNECTION_LIMITS > 0)
extern VAR(uint8, FRTP_VAR) FrTp_ActiveConnectionsCnt[FRTP_NUMBER_CONNECTION_LIMITS];
#else
/* Dummy variable in case there are no limits configured */
extern P2VAR(uint8, FRTP_VAR, FRTP_APPL_DATA) FrTp_ActiveConnectionsCnt;
#endif /* FRTP_NUMBER_CONNECTION_LIMITS */
#endif /* FRTP_LIMIT_NUMBER_CONNECTIONS */

/*
 * Stop variable section declaration
 */
#define FRTP_STOP_SEC_VAR_CLEARED_8
#include <FrTp_MemMap.h>


/*
 * Start variable section declaration
 */
#define FRTP_START_SEC_VAR_FAST_INIT_UNSPECIFIED
#include <FrTp_MemMap.h>

extern P2CONST(FrTp_ConfigType,FRTP_VAR,FRTP_APPL_DATA) FrTp_RootPtr;
/*
 * Stop variable section declaration
 */
#define FRTP_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
#include <FrTp_MemMap.h>

/*
 * Start variable section declaration
 */
#define FRTP_START_SEC_VAR_INIT_8
#include <FrTp_MemMap.h>

/* module initialization status */ /* !LINKSTO FRTP210, 1 */
extern VAR(uint8,FRTP_VAR) FrTp_InitStatus;

/*
 * Stop variable section declaration
 */
#define FRTP_STOP_SEC_VAR_INIT_8
#include <FrTp_MemMap.h>

/*=====================[Function prototypes]=================================*/

/*
 * Start code section declaration
 */
#define FRTP_START_SEC_CODE
#include <FrTp_MemMap.h>

/**
 * \brief Returns the first free TxPdu that has a minimum length of MinLength
 *
 */
extern FUNC(uint8,FRTP_CODE) FrTp_GetFreeTxPdu
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    VAR(PduLengthType,AUTOMATIC) minLength
);

/**
 * \brief Indicates a transmission request to the state machine.
 */
extern FUNC(void, FRTP_CODE) FrTp_TxSm_RequestTransmission
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    VAR(uint16,AUTOMATIC) msgLength
);


/**
 * \brief Indicates that the Statemachine is allowed to transmit.
 */
extern FUNC(boolean,FRTP_CODE) FrTp_TxSm_TransmitSyncPoint
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    VAR(uint8,AUTOMATIC) NewCycle
);

/**
 * \brief fetch transmission data from Tx-statemachine.
 */
extern FUNC(boolean,FRTP_CODE) FrTp_TxSm_TriggerTransmit
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    VAR(uint8,AUTOMATIC) TxPduIdx,
    P2VAR(PduInfoType,AUTOMATIC,FRTP_APPL_DATA) pPduInfo
);

/**
 * \brief Indicate a transmission confirmation to the Tx-statemachine.
 */
extern FUNC(void,FRTP_CODE) FrTp_TxSm_TransmitConfirmation
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    VAR(uint8,AUTOMATIC) TxPduIdx
);

/**
 * \brief Indicate a FC CTS reception to the Tx-statemachine.
 */
extern FUNC(void,FRTP_CODE) FrTp_TxSm_CTS
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    VAR(uint16,AUTOMATIC) bufferSize,
    VAR(uint8,AUTOMATIC) bandwidthControl
);

/**
 * \brief Indicate a FC ACK reception to the Tx-statemachine.
 */
extern FUNC(void,FRTP_CODE) FrTp_TxSm_ACK
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel
);

/**
 * \brief Indicate a FC RETRY reception to the Tx-statemachine.
 */
extern FUNC(void,FRTP_CODE) FrTp_TxSm_RETRY
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    VAR(uint16,AUTOMATIC) bufferPointer
);

/**
 * \brief Indicate result to upper layer and abort communication immediately.
 */
FUNC(void,FRTP_CODE) FrTp_TxSm_IndicateAndAbort
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    VAR(NotifResultType,AUTOMATIC) result
);

/**
 * \brief Indicate a FC WAIT reception to the Tx-statemachine.
 */
extern FUNC(void,FRTP_CODE) FrTp_TxSm_WT
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel
);

/**
 * \brief Indicate a FC OVFLW reception to the Tx-statemachine.
 */
extern FUNC(void,FRTP_CODE) FrTp_TxSm_OVFLW
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel
);

/**
 * \brief Cancel outstanding transmission on given channel.
 */
extern FUNC(Std_ReturnType, FRTP_CODE) FrTp_FrIfCancelTransmit
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel
);

/**
 * \brief fetch transmission data from Rx-statemachine.
 */
extern FUNC(boolean,FRTP_CODE) FrTp_RxSm_TriggerTransmit
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    P2VAR(PduInfoType,AUTOMATIC,FRTP_APPL_DATA) pPduInfo
);

/**
 * \brief Indicate a transmission confirmation to the Tx-statemachine.
 */
extern FUNC(void,FRTP_CODE) FrTp_RxSm_TransmitConfirmation
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    VAR(uint8,AUTOMATIC) TxPduIdx
);

/**
 * \brief Indicates that the Statemachine is allowed to transmit.
 */
extern FUNC(void,FRTP_CODE) FrTp_RxSm_TransmitSyncPoint
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel
);

/**
 * \brief Indicate result to upper layer and abort communication immediately.
 */
FUNC(void,FRTP_CODE) FrTp_RxSm_IndicateAndAbort
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    VAR(NotifResultType,AUTOMATIC) result
);

/**
 * \brief Indicate received data to the Rx-statemachine.
 */
extern FUNC(void,FRTP_CODE) FrTp_RxSm_RxData
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    VAR(uint8,AUTOMATIC) PCI,
    VAR(uint8,AUTOMATIC) SN,
    P2CONST(PduInfoType,AUTOMATIC,FRTP_APPL_DATA) pPduInfo
);


#if (FRTP_LIMIT_NUMBER_CONNECTIONS == STD_ON)
/**
 * \brief Starts a connection from the connection buffer
 */
extern FUNC(Std_ReturnType, FRTP_CODE) FrTp_StartBufferedConnection
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel
);
#endif /* FRTP_LIMIT_NUMBER_CONNECTIONS */

/*
 * Stop code section declaration
 */
#define FRTP_STOP_SEC_CODE
#include <FrTp_MemMap.h>

#undef TS_RELOCATABLE_CFG_ENABLE

#endif /* FRTP_PRIV_H */
