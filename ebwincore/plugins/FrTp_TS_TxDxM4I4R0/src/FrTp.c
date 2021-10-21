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

#include <PduR_FrTp.h>  /* PduR API called by FrTp */
#include <FrTp_Lcfg.h>
#include <FrTp_guards.h> /* After foreign/API/Cfg, before internal/private */
#include <FrTp_Trace.h>
#include <FrTp_Priv.h>  /* data types */


/* !LINKSTO FrTp.EB.PublishedInformation,1 */
/*------------------[AUTOSAR vendor identification check]-------------------*/

#ifndef FRTP_VENDOR_ID /* configuration check */
#error FRTP_VENDOR_ID must be defined
#endif

#if (FRTP_VENDOR_ID != 1U) /* vendor check */
#error FRTP_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#ifndef FRTP_AR_RELEASE_MAJOR_VERSION /* configuration check */
#error FRTP_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (FRTP_AR_RELEASE_MAJOR_VERSION != 4U)
#error FRTP_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef FRTP_AR_RELEASE_MINOR_VERSION /* configuration check */
#error FRTP_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (FRTP_AR_RELEASE_MINOR_VERSION != 0U )
#error FRTP_AR_RELEASE_MINOR_VERSION wrong (!= 0U)
#endif

#ifndef FRTP_AR_RELEASE_REVISION_VERSION /* configuration check */
#error FRTP_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (FRTP_AR_RELEASE_REVISION_VERSION != 3U )
#error FRTP_AR_RELEASE_REVISION_VERSION wrong (!= 3U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#ifndef FRTP_SW_MAJOR_VERSION /* configuration check */
#error FRTP_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (FRTP_SW_MAJOR_VERSION != 4U)
#error FRTP_SW_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef FRTP_SW_MINOR_VERSION /* configuration check */
#error FRTP_SW_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (FRTP_SW_MINOR_VERSION < 4U)
#error FRTP_SW_MINOR_VERSION wrong (< 4U)
#endif

#ifndef FRTP_SW_PATCH_VERSION /* configuration check */
#error FRTP_SW_PATCH_VERSION must be defined
#endif

/* patch version check */
#if (FRTP_SW_PATCH_VERSION < 26U)
#error FRTP_SW_PATCH_VERSION wrong (< 26U)
#endif

/*======================[Local Macros]=======================================*/

/*=====================[Variable Definitions]================================*/

/*
 * Start variable section declaration
 */
#define FRTP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <FrTp_MemMap.h>

/* channel run-time data */ /* !LINKSTO FRTP210, 1 */
VAR(FrTp_ChannelType,FRTP_VAR) FrTp_Channel[FRTP_CHAN_NUM];

VAR(FrTp_RuntimeTxPduType,FRTP_VAR) FrTp_RuntimeTxPdu[FRTP_TXPDU_NUM];

#if (FRTP_LIMIT_NUMBER_CONNECTIONS == STD_ON)
VAR(FrTp_ConnectionBufferType, FRTP_VAR) FrTp_ConnectionBuffer;
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
VAR(uint8, FRTP_VAR) FrTp_ActiveConnectionsCnt[FRTP_NUMBER_CONNECTION_LIMITS];
#else
/* Dummy variable in case there are no limits configured */
P2VAR(uint8, FRTP_VAR, FRTP_APPL_DATA) FrTp_ActiveConnectionsCnt;
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

/* pointer to post-build configuration */ /* !LINKSTO FRTP210, 1 */
P2CONST(FrTp_ConfigType,FRTP_VAR,FRTP_APPL_CONST) FrTp_RootPtr = NULL_PTR;

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
VAR(uint8,FRTP_VAR) FrTp_InitStatus = FRTP_UNINIT;

/*
 * Stop variable section declaration
 */
#define FRTP_STOP_SEC_VAR_INIT_8
#include <FrTp_MemMap.h>

/*=====================[Local Function Prototypes]===========================*/

/*
 * Start code section declaration
 */
#define FRTP_START_SEC_CODE
#include <FrTp_MemMap.h>

STATIC FUNC(void,FRTP_CODE) FrTp_HandleTimers(P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel);
STATIC FUNC(void,FRTP_CODE) FrTp_HandleTxTimer1(P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel);
STATIC FUNC(void,FRTP_CODE) FrTp_HandleTxTimer2(P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel);

#if (FRTP_FULLDUPLEX_ENABLE == STD_ON)
STATIC FUNC(void,FRTP_CODE) FrTp_HandleRxTimer1(P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel);
STATIC FUNC(void,FRTP_CODE) FrTp_HandleRxTimer2(P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel);
#endif /* FRTP_FULLDUPLEX_ENABLE */

STATIC FUNC(void,FRTP_CODE) FrTp_HandleRxTimer3(P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel);

STATIC FUNC_P2VAR(FrTp_ChannelType, AUTOMATIC, FRTP_CODE)
    FrTp_FindTxChannel(VAR(PduIdType,AUTOMATIC) TxPduId);

/**
 * \brief Resets all state variables for the given channel.
 */
STATIC FUNC(void,FRTP_CODE) FrTp_ResetChannel
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel
);

/**
 * \brief Attempts to save a connection in the connection buffer
 */
#if (FRTP_LIMIT_NUMBER_CONNECTIONS == STD_ON)
FUNC(Std_ReturnType, FRTP_CODE) FrTp_BufferConnection
(
    VAR(PduIdType, AUTOMATIC) FrTpTxPduId,
    P2CONST(FrTp_ConnectionType, AUTOMATIC, FRTP_APPL_CONST) pConnection,
    P2CONST(PduInfoType, AUTOMATIC, FRTP_APPL_DATA) PduInfoPtr
);
#endif /* FRTP_LIMIT_NUMBER_CONNECTIONS */

/*
 * Stop code section declaration
 */
#define FRTP_STOP_SEC_CODE
#include <FrTp_MemMap.h>

/*
 * Start code section declaration
 */
#define FRTP_START_SEC_CODE
#include <FrTp_MemMap.h>

/*=====================[Local Function Definitions]==========================*/

STATIC FUNC(void,FRTP_CODE) FrTp_HandleTimers
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel
)
{

  FrTp_HandleTxTimer1(pChannel);
  FrTp_HandleTxTimer2(pChannel);
#if (FRTP_FULLDUPLEX_ENABLE == STD_ON)
  FrTp_HandleRxTimer1(pChannel);
  FrTp_HandleRxTimer2(pChannel);
#endif /* FRTP_FULLDUPLEX_ENABLE */
  FrTp_HandleRxTimer3(pChannel);

}


STATIC FUNC(void,FRTP_CODE) FrTp_HandleTxTimer1
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel
)
{
    boolean TimerExpiredFlag = FALSE;


    /* enter exclusive area */
    SchM_Enter_FrTp_SCHM_FRTP_EXCLUSIVE_AREA_0();

    /* handle channel timer AS */
    switch(pChannel->Tx_Timer1)
    {
        case 0U: break; /* timer deactivated */
        case 1U: /* timer expired */
        {
            FrTp_StopTimer(pChannel->Tx_Timer1);
            TimerExpiredFlag = TRUE;
            break;
        }
        default:
        {
            pChannel->Tx_Timer1--;
            break; /* timer still running */
        }
    }

    /* exit exclusive area */
    SchM_Exit_FrTp_SCHM_FRTP_EXCLUSIVE_AREA_0();

    if( TimerExpiredFlag == TRUE )
    {
        /* in half duplex case, demultiplex timer usage */
#if (FRTP_FULLDUPLEX_ENABLE == STD_OFF)
        if(FrTp_Chnl_IsRxState(pChannel->Tx_State))
        {
            (void)FrTp_FrIfCancelTransmit(pChannel);
            FrTp_RxSm_IndicateAndAbort(pChannel,NTFRSLT_E_TIMEOUT_A);
        }
        else
#endif /* FRTP_FULLDUPLEX_ENABLE */
        {
            (void)FrTp_FrIfCancelTransmit(pChannel);
            FrTp_TxSm_IndicateAndAbort(pChannel,NTFRSLT_E_TIMEOUT_A);
        }
    }

}


STATIC FUNC(void,FRTP_CODE) FrTp_HandleTxTimer2
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel
)
{
    boolean TimerExpiredFlag = FALSE;


    /* enter exclusive area */
    SchM_Enter_FrTp_SCHM_FRTP_EXCLUSIVE_AREA_0();

    /* handle channel timer BS */
    switch(pChannel->Tx_Timer2)
    {
        case 0U: break; /* timer deactivated */
        case 1U: /* timer expired */
        {
            FrTp_StopTimer(pChannel->Tx_Timer2);
            TimerExpiredFlag = TRUE;
            break;
        }
        default:
        {
            pChannel->Tx_Timer2--;
            break; /* timer still running */
        }
    }

    /* exit exclusive area */
    SchM_Exit_FrTp_SCHM_FRTP_EXCLUSIVE_AREA_0();

    if( TimerExpiredFlag == TRUE )
    {
/* in half duplex case, demultiplex timer usage */
#if (FRTP_FULLDUPLEX_ENABLE == STD_OFF)
        if(FrTp_Chnl_IsRxState(pChannel->Tx_State))
        {
            FrTp_RxSm_IndicateAndAbort(pChannel,NTFRSLT_E_TIMEOUT_CR);
        }
        else
#endif /* FRTP_FULLDUPLEX_ENABLE */
        {
            FrTp_TxSm_IndicateAndAbort(pChannel,NTFRSLT_E_TIMEOUT_BS);
        }
    }

}


#if (FRTP_FULLDUPLEX_ENABLE == STD_ON)
STATIC FUNC(void,FRTP_CODE) FrTp_HandleRxTimer1
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel
)
{
    boolean TimerExpiredFlag = FALSE;


    /* full duplex implementation */

    /* enter exclusive area */
    SchM_Enter_FrTp_SCHM_FRTP_EXCLUSIVE_AREA_0();

    /* handle channel timer AR */
    switch(pChannel->Rx_Timer1)
    {
        case 0U: break; /* timer deactivated */
        case 1U:
        {
            FrTp_StopTimer(pChannel->Rx_Timer1);
            TimerExpiredFlag = TRUE;
            break; /* timer expired */
        }
        default:
        {
            pChannel->Rx_Timer1--;
            break; /* timer still running */
        }
    }

    /* exit exclusive area */
    SchM_Exit_FrTp_SCHM_FRTP_EXCLUSIVE_AREA_0();

    if( TimerExpiredFlag == TRUE )
    {
        (void)FrTp_FrIfCancelTransmit(pChannel);
        FrTp_RxSm_IndicateAndAbort(pChannel,NTFRSLT_E_TIMEOUT_A);
    }

}


STATIC FUNC(void,FRTP_CODE) FrTp_HandleRxTimer2
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel
)
{
    boolean TimerExpiredFlag = FALSE;


    /* enter exclusive area */
    SchM_Enter_FrTp_SCHM_FRTP_EXCLUSIVE_AREA_0();

    /* handle channel timer CR */
    switch(pChannel->Rx_Timer2)
    {
        case 0U: break; /* timer deactivated */
        case 1U:
        {
            FrTp_StopTimer(pChannel->Rx_Timer2);
            TimerExpiredFlag = TRUE;
            break;
        }
        default:
        {
            pChannel->Rx_Timer2--;
            break; /* timer still running */
        }
    }

    /* exit exclusive area */
    SchM_Exit_FrTp_SCHM_FRTP_EXCLUSIVE_AREA_0();

    if( TimerExpiredFlag == TRUE )
    {
        FrTp_RxSm_IndicateAndAbort(pChannel,NTFRSLT_E_TIMEOUT_CR);
    }

}
#endif /* FRTP_FULLDUPLEX_ENABLE */


STATIC FUNC(void,FRTP_CODE) FrTp_HandleRxTimer3
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel
)
{
    /* enter exclusive area */

    SchM_Enter_FrTp_SCHM_FRTP_EXCLUSIVE_AREA_0();

    /* handle channel timer BR */
    switch(pChannel->Rx_Timer3)
    {
        case 0U: break; /* timer deactivated */
        case 1U:
        {
            FrTp_StopTimer(pChannel->Rx_Timer3);
            break;
        }
        default:
        {
            pChannel->Rx_Timer3--;
            break; /* timer still running */
        }
    }

    /* exit exclusive area */
    SchM_Exit_FrTp_SCHM_FRTP_EXCLUSIVE_AREA_0();

}


STATIC FUNC_P2VAR(FrTp_ChannelType, AUTOMATIC, FRTP_CODE)
    FrTp_FindTxChannel(VAR(PduIdType,AUTOMATIC) TxPduId)
{
    /* this function is called from within an exclusive area */

    /* channel iterator variable */
    uint8_least iChannel = 0U;

    P2VAR(FrTp_ChannelType, AUTOMATIC, FRTP_VAR) pChannelCandidate = NULL_PTR;

    /* iterate over all channels */
    do
    {
        /* get pointer to right channel structure */
        CONSTP2VAR(FrTp_ChannelType, AUTOMATIC, FRTP_VAR) pChannel = &FrTp_Channel[iChannel];

        /* get connection associated with this channel */
        CONST(uint8,AUTOMATIC) ConnectionIdx = pChannel->ConnectionIdx;

        /* check if already a channel is assigned for this TxPduId */
        if(ConnectionIdx == (uint8)TxPduId)
        {
            /* yes it is - now check if tx is still idle */
            if(pChannel->Tx_State != FRTP_CHNL_TX_IDLE)
            {
                /* abort further transmission procedure */
                pChannelCandidate = NULL_PTR;
            }
            else
            {
                /* save candidate index */
                pChannelCandidate = pChannel;

                /* abort search, since we must use this channel */
            }
            break;
        }
        /* if this is an unused channel, save as candidate for usage */
        else if((pChannelCandidate == NULL_PTR) && (ConnectionIdx == FRTP_CONNECTION_INVALID))
        {
            /* save free channel as candidate for allocation */
            pChannelCandidate = pChannel;
        }
        else
        {
            /* continue channel search */
        }
        iChannel++;
    } while(iChannel < FRTP_CHAN_NUM);

    if(pChannelCandidate != NULL_PTR)
    {
        /* allocate this channel structure */
        pChannelCandidate->ConnectionIdx = (uint8)TxPduId;
    }

    /* return index of found/allocated channel */

    return pChannelCandidate;
}

/**
 * \brief Resets all state variables for the given channel.
 */
STATIC FUNC(void,FRTP_CODE) FrTp_ResetChannel
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel
)
{
    /* enter exclusive area */

    SchM_Enter_FrTp_SCHM_FRTP_EXCLUSIVE_AREA_0();

    FrTp_StopTimer((pChannel)->Tx_Timer1);
    FrTp_StopTimer((pChannel)->Tx_Timer2);
    (pChannel)->Tx_State = FRTP_CHNL_TX_IDLE;
#if (FRTP_FULLDUPLEX_ENABLE == STD_ON)
    FrTp_StopTimer((pChannel)->Rx_Timer1);
    FrTp_StopTimer((pChannel)->Rx_Timer2);
    (pChannel)->Rx_State = FRTP_CHNL_RX_IDLE;
#endif /* FRTP_FULLDUPLEX_ENABLE */
    (pChannel)->ConnectionIdx = FRTP_CONNECTION_INVALID;

    /* exit exclusive area */
    SchM_Exit_FrTp_SCHM_FRTP_EXCLUSIVE_AREA_0();

}

/*=====================[Function Definitions]================================*/

FUNC( Std_ReturnType, FRTP_CODE) FrTp_IsValidConfig
(
        P2CONST(void,AUTOMATIC,FRTP_APPL_CONST) voidConfigPtr
)
{
    /* Assume an invalid configuration */
    Std_ReturnType RetVal = E_NOT_OK;
    P2CONST(FrTp_ConfigType,AUTOMATIC,FRTP_APPL_CONST) ConfigPtr = voidConfigPtr;

    DBG_FRTP_ISVALIDCONFIG_ENTRY(voidConfigPtr);

    if (ConfigPtr != NULL_PTR)
    {
        /* Check if the configuration fits to the platform */
        if (TS_PlatformSigIsValid(ConfigPtr->PlatformSignature))
        {
            /* Validate the post build configuration against the compile time configuration */
            if (FRTP_CFG_SIGNATURE == ConfigPtr->CfgSignature)
            {
                /* Validate the post build configuration against the link time configuration */
                if (FrTp_LcfgSignature == ConfigPtr->LcfgSignature)
                {
                    /* Validate the Published information in post build configuration against the
                       Published information in compile time configuration*/
                    if(FRTP_PUBLIC_INFO_SIGNATURE == ConfigPtr->PublicInfoSignature)
                    {
                        /* Indicate that the configuration is valid */
                        RetVal = E_OK;
                    }
                }
            }
        }
    }

    DBG_FRTP_ISVALIDCONFIG_EXIT(RetVal,voidConfigPtr);
    return RetVal;
}

FUNC(void,FRTP_CODE) FrTp_Init
(
    P2CONST(FrTp_ConfigType,AUTOMATIC,FRTP_APPL_CONST) PBCfgPtr
)
{
    P2CONST(FrTp_ConfigType,AUTOMATIC,FRTP_APPL_CONST) LocalConfigPtr = PBCfgPtr;

    DBG_FRTP_INIT_ENTRY(PBCfgPtr);

    /* Save the configuration pointer */
#if (FRTP_PBCFGM_SUPPORT_ENABLED == STD_ON)
    /* If the initialization function is called with a null pointer get the configuration from the
     * post build configuration manager */
    if (LocalConfigPtr == NULL_PTR)
    {
        PbcfgM_ModuleConfigPtrType ModuleConfig = NULL_PTR;
        if (E_OK == PbcfgM_GetConfig(
              FRTP_MODULE_ID,
              FRTP_DET_GENERIC_INSTANCE_ID,
              &ModuleConfig))
        {
            LocalConfigPtr = (P2CONST(FrTp_ConfigType, AUTOMATIC, FRTP_APPL_CONST)) ModuleConfig;
        }
    }
#endif /* FRTP_PBCFGM_SUPPORT_ENABLED == STD_OFF */

/* check if development error detection is enabled */
#if (FRTP_DEV_ERROR_DETECT == STD_ON)

    /* check whether parameter FrTp_RootPtr is a NULL_PTR */
    if(LocalConfigPtr == NULL_PTR)
    {
        /* found a NULL_PTR, report to DET */
        (void)Det_ReportError(FRTP_MODULE_ID,
                              FRTP_DET_GENERIC_INSTANCE_ID,
                              FRTP_INIT_SERVICE_ID,
                              FRTP_E_NULL_PTR
                             );
    }
    /* check that configuration pointer is valid */
    else if ( E_OK != FrTp_IsValidConfig(LocalConfigPtr))
    {
        (void)Det_ReportError(FRTP_MODULE_ID,
                              FRTP_DET_GENERIC_INSTANCE_ID,
                              FRTP_INIT_SERVICE_ID,
                              FRTP_E_INVALID_PARAMETER
                             );
    }
    else
#endif /* FRTP_DEV_ERROR_DETECT */

    /* perform functionality */
    {
        uint8_least iChannel = 0U; /* channel iterator variable */
        uint8_least iTxPdu;   /* TxPdu iterator variable */

        TS_AtomicAssign8(FrTp_InitStatus, FRTP_UNINIT);
        FrTp_RootPtr = LocalConfigPtr;

        /* loop over channels */
        do
        {
            /* get pointer to right channel structure */
            CONSTP2VAR(FrTp_ChannelType, AUTOMATIC, FRTP_VAR) pChannel =
                    &FrTp_Channel[iChannel];

            /* reset channel data structure */
            FrTp_ResetChannel(pChannel);
            iChannel++;
        } while(iChannel < FRTP_CHAN_NUM);

        /* loop over Tx PDUs */
        for(iTxPdu = 0U; iTxPdu < FrTp_RootPtr->nNumTxPdus; iTxPdu++)
        {
            /* set all TxPdus to unused */
            FrTp_RuntimeTxPdu[iTxPdu].pChannel = NULL_PTR;
        }

#if (FRTP_LIMIT_NUMBER_CONNECTIONS == STD_ON)

#if (FRTP_NUMBER_CONNECTION_LIMITS > 0)
        {
            /* Reset active connection counter */
            FrTp_ConnectionLimitConfigIdxType iLimit; /* Connection limit iterator variable */
            for(iLimit = 0U; iLimit < FRTP_NUMBER_CONNECTION_LIMITS; iLimit++)
            {
                FrTp_ActiveConnectionsCnt[iLimit] = 0U;
            }
        }
#endif /* (FRTP_NUMBER_CONNECTION_LIMITS > 0) */

#if (FRTP_SUM_BUFFER_SIZE > 0)
        {
            /* Reset connection buffers */
            FrTp_ConnectionLimitConfigIdxType iBuffer; /* Buffer info iterator variable */
            for(iBuffer = 0U; iBuffer < FRTP_NUMBER_CONNECTION_BUFFERS; iBuffer++)
            {
                CONSTP2CONST(FrTp_ConnectionLimitConfigType, AUTOMATIC, FRTP_APPL_CONST)
                    pConnectionLimitConfig =
                    FRTP_CFG_GET_PTR_TO_CONNECTION_LIMIT_CONFIG(iBuffer);
                FrTp_ConnectionBuffer.BufferInfos[iBuffer].IdxHead = 0U;
                FrTp_ConnectionBuffer.BufferInfos[iBuffer].IdxTail = 0U;
                FrTp_ConnectionBuffer.BufferInfos[iBuffer].BufferSize =
                    pConnectionLimitConfig->BufferSize;
                FrTp_ConnectionBuffer.BufferInfos[iBuffer].BufferPtr =
                    &FrTp_ConnectionBuffer.Connections[pConnectionLimitConfig->BufferOffset];
            }
        }
#endif /* (FRTP_SUM_BUFFER_SIZE > 0) */

#endif /* FRTP_LIMIT_NUMBER_CONNECTIONS */

        /* mark module as initialized */
        TS_AtomicAssign8(FrTp_InitStatus, FRTP_INIT);

    }

    DBG_FRTP_INIT_EXIT(PBCfgPtr);
}


FUNC(Std_ReturnType,FRTP_CODE) FrTp_Transmit
(
    VAR(PduIdType,AUTOMATIC) FrTpTxPduId,
    P2CONST(PduInfoType,AUTOMATIC,FRTP_APPL_DATA) PduInfoPtr
)
{
    /* variable holding function status code */
    Std_ReturnType RetCode = E_NOT_OK;

    DBG_FRTP_TRANSMIT_ENTRY(FrTpTxPduId,PduInfoPtr);

    /* check for successfully initialized module */
    if (FrTp_InitStatus == FRTP_UNINIT)
    {
/* check if development error detection is enabled */
#if (FRTP_DEV_ERROR_DETECT == STD_ON)
        /* Report to DET that module was not initialized */
        (void)Det_ReportError(FRTP_MODULE_ID,
                              FRTP_DET_GENERIC_INSTANCE_ID,
                              FRTP_TRANSMIT_SERVICE_ID,
                              FRTP_E_UNINIT
                             );
#endif /* FRTP_DEV_ERROR_DETECT */
    }
    /* check whether that LPduIdx is supported */
    else if(FrTpTxPduId >= FrTp_RootPtr->nNumTxConnections)
    {
/* check if development error detection is enabled */
#if (FRTP_DEV_ERROR_DETECT == STD_ON)
        /* passed an invalid FrTpTxPduId, report to DET */
        (void)Det_ReportError(FRTP_MODULE_ID,
                              FRTP_DET_GENERIC_INSTANCE_ID,
                              FRTP_TRANSMIT_SERVICE_ID,
                              FRTP_E_INVALID_PDU_SDU_ID
                             );
#endif /* FRTP_DEV_ERROR_DETECT */
    }
    /* check whether parameter is a NULL_PTR */
    else if(PduInfoPtr == NULL_PTR)
    {
/* check if development error detection is enabled */
#if (FRTP_DEV_ERROR_DETECT == STD_ON)
        /* found a NULL_PTR, report to DET */
        (void)Det_ReportError(FRTP_MODULE_ID,
                              FRTP_DET_GENERIC_INSTANCE_ID,
                              FRTP_TRANSMIT_SERVICE_ID,
                              FRTP_E_NULL_PTR
                             );
#endif /* FRTP_DEV_ERROR_DETECT */
    }
    else
    {
#if (FRTP_UNKNOWN_MSG_LENGTH == STD_OFF)
        /* Check for unknown SDU length request */
        if(PduInfoPtr->SduLength == 0U)
        {
/* check if development error detection is enabled */
#if (FRTP_DEV_ERROR_DETECT == STD_ON)

            /* make a DET call */
            (void)Det_ReportError(FRTP_MODULE_ID,
                                  FRTP_DET_GENERIC_INSTANCE_ID,
                                  FRTP_TRANSMIT_SERVICE_ID,
                                  FRTP_E_UMSG_LENGTH_ERROR
                                 );

#endif /* FRTP_DEV_ERROR_DETECT */
        }
        else
#endif /* FRTP_UNKNOWN_MSG_LENGTH == STD_OFF */

#if (TS_SIZE_PduLengthType == 4U)
        /* Check for SDU length out of range (e.g in case of PduLengthType UINT32) */
        if (PduInfoPtr->SduLength > 65535U)
        {
/* check if development error detection is enabled */
#if (FRTP_DEV_ERROR_DETECT == STD_ON)
            /* make a DET call */
            (void)Det_ReportError(
                FRTP_MODULE_ID,
                FRTP_DET_GENERIC_INSTANCE_ID,
                FRTP_TRANSMIT_SERVICE_ID,
                FRTP_E_INVALID_PARAMETER
            );
#endif /* FRTP_DEV_ERROR_DETECT */
        }
        else
#endif /*TS_SIZE_PduLengthType */

        {
            /* Get pointer to connection */
            CONSTP2CONST(FrTp_ConnectionType, AUTOMATIC, FRTP_APPL_CONST) pConnection =
                FRTP_CFG_GET_PTR_TO_CONNECTION(FrTpTxPduId);

            /* Check for functional addressing with segmentation */
            if(((pConnection->TpOptions&FRTP_CONNECTION_ISBROADCAST) != 0U) &&
               (pConnection->TxPduMaxLength < (PduInfoPtr->SduLength + FrTp_Size_Header_STF))
              )
            {
#if (FRTP_DEV_ERROR_DETECT == STD_ON)
                /* Found a invalid Tx length, report to DET */
                (void)Det_ReportError(FRTP_MODULE_ID,
                                      FRTP_DET_GENERIC_INSTANCE_ID,
                                      FRTP_TRANSMIT_SERVICE_ID,
                                      FRTP_E_SEG_ERROR
                                     );
#endif /* FRTP_DEV_ERROR_DETECT */
            }
            else
            {
#if (FRTP_LIMIT_NUMBER_CONNECTIONS == STD_ON)

                uint8 connectionLimit;
                boolean bufferConnection = FALSE;
                uint8_least iChannel;

                SchM_Enter_FrTp_SCHM_FRTP_EXCLUSIVE_AREA_0();

                /* Get the connection limit corresponding to the connection's remote address */
                if(pConnection->ConnectionLimitConfigIdx != FRTP_NUMBER_CONNECTION_LIMITS)
                {
                    connectionLimit =
                        FRTP_CFG_GET_PTR_TO_CONNECTION_LIMIT_CONFIG(pConnection->ConnectionLimitConfigIdx)->Limit;
                    if(FrTp_ActiveConnectionsCnt[pConnection->ConnectionLimitConfigIdx] >= connectionLimit)
                    {
                        bufferConnection = TRUE;
                    }
                }

                if(!bufferConnection)
                {
                    bufferConnection = TRUE;
                    /* Search for an unused channel */
                    for(iChannel = 0U; iChannel < FRTP_CHAN_NUM; iChannel++)
                    {
                        /* Get pointer to channel structure */
                        CONSTP2VAR(FrTp_ChannelType, AUTOMATIC, FRTP_VAR) pChannel =
                            &FrTp_Channel[iChannel];

                        /* Check if channel is unused */
                        if((pChannel->ConnectionIdx == FRTP_CONNECTION_INVALID) ||
                            ((pChannel->ConnectionIdx == FrTpTxPduId) &&
                                (pChannel->Tx_State == FRTP_CHNL_TX_IDLE)))
                        {
                            bufferConnection = FALSE;
                            break;
                        }
                    }
                }

                if(bufferConnection)
                {
                    RetCode = FrTp_BufferConnection(FrTpTxPduId, pConnection, PduInfoPtr);
                }

                SchM_Exit_FrTp_SCHM_FRTP_EXCLUSIVE_AREA_0();

                if(bufferConnection)
                {
#if (FRTP_DEV_ERROR_DETECT == STD_ON)
                    if(RetCode == E_NOT_OK)
                    {
                        /* make a DET call */
                        (void)Det_ReportError(
                            FRTP_MODULE_ID,
                            FRTP_DET_GENERIC_INSTANCE_ID,
                            FRTP_TRANSMIT_SERVICE_ID,
                            FRTP_E_NO_CHANNEL
                        );
                    }
#endif /* FRTP_DEV_ERROR_DETECT */
                }
                else
                {
#endif /* FRTP_LIMIT_NUMBER_CONNECTIONS */

                    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel;

                    /* enter exclusive area */
                    SchM_Enter_FrTp_SCHM_FRTP_EXCLUSIVE_AREA_0();

                    /* get pointer to right channel structure */
                    pChannel = FrTp_FindTxChannel(FrTpTxPduId);

#if (FRTP_LIMIT_NUMBER_CONNECTIONS == STD_ON)
                    /* Increment number of active connections */
                    if(
                        (pChannel != NULL_PTR) &&
                        (pConnection->ConnectionLimitConfigIdx != FRTP_NUMBER_CONNECTION_LIMITS)
                    )
                    {
                        FrTp_ActiveConnectionsCnt[pConnection->ConnectionLimitConfigIdx]++;
                    }
#endif /* FRTP_LIMIT_NUMBER_CONNECTIONS */

                    /* exit exclusive area */
                    SchM_Exit_FrTp_SCHM_FRTP_EXCLUSIVE_AREA_0();

                    /* no free channel resource available */
                    if(pChannel == NULL_PTR)
                    {
/* check if development error detection is enabled */
#if (FRTP_DEV_ERROR_DETECT == STD_ON)
                        /* make a DET call */
                        (void)Det_ReportError(FRTP_MODULE_ID,
                                              FRTP_DET_GENERIC_INSTANCE_ID,
                                              FRTP_TRANSMIT_SERVICE_ID,
                                              FRTP_E_NO_CHANNEL
                                             );

#endif /* FRTP_DEV_ERROR_DETECT */
                    }
                    else
                    {
                        /* Trigger Tx-state machine to start transmission procedure */
                        FrTp_TxSm_RequestTransmission(pChannel,(uint16)PduInfoPtr->SduLength);
                        RetCode = E_OK;
                    }
#if (FRTP_LIMIT_NUMBER_CONNECTIONS == STD_ON)
                }
#endif /* FRTP_LIMIT_NUMBER_CONNECTIONS */
            }
        }
    }

    DBG_FRTP_TRANSMIT_EXIT(RetCode,FrTpTxPduId,PduInfoPtr);

    /* Return status value */
    return RetCode;
}


#if (FRTP_HAVE_TC == STD_ON)
FUNC(Std_ReturnType,FRTP_CODE) FrTp_CancelTransmit
(
    VAR(PduIdType,AUTOMATIC) FrTpTxPduId
)
{
    /* variable holding function status code */
    Std_ReturnType RetCode = E_NOT_OK;

    DBG_FRTP_CANCELTRANSMIT_ENTRY(FrTpTxPduId);

    TS_PARAM_UNUSED(FrTpTxPduId);

/* check if development error detection is enabled */
#if (FRTP_DEV_ERROR_DETECT == STD_ON)

    /* check for successfully initialized module */
    /* Report to DET and return Error in case module was not initialized before */
    if (FrTp_InitStatus == FRTP_UNINIT)
    {
        (void)Det_ReportError(FRTP_MODULE_ID,
                              FRTP_DET_GENERIC_INSTANCE_ID,
                              FRTP_CANCELTRANSMIT_SERVICE_ID,
                              FRTP_E_UNINIT
                             );
        RetCode = E_NOT_OK;
    }
    else
#endif /* FRTP_DEV_ERROR_DETECT */
    {
        /* FrTp_CancelTransmit functionality is not supported right now */
        /* Transmission is not cancelled. */
        RetCode = E_NOT_OK;
    }

    DBG_FRTP_CANCELTRANSMIT_EXIT(RetCode,FrTpTxPduId);
    return RetCode;
}
#endif /* FRTP_HAVE_TC */


FUNC(Std_ReturnType,FRTP_CODE) FrTp_CancelReceive
(
    VAR(PduIdType,AUTOMATIC) FrTpRxSduId
)
{
    /* variable holding function status code */
    Std_ReturnType RetCode = E_NOT_OK;

    DBG_FRTP_CANCELRECEIVE_ENTRY(FrTpRxSduId);

    TS_PARAM_UNUSED(FrTpRxSduId);

/* check if development error detection is enabled */
#if (FRTP_DEV_ERROR_DETECT == STD_ON)

    /* check for successfully initialized module */
    /* Report to DET and return Error in case module was not initialized before */
    if (FrTp_InitStatus == FRTP_UNINIT)
    {
        (void)Det_ReportError(FRTP_MODULE_ID,
                              FRTP_DET_GENERIC_INSTANCE_ID,
                              FRTP_CANCELRECEIVE_SERVICE_ID,
                              FRTP_E_UNINIT
                             );
        RetCode = E_NOT_OK;
    }
    else
#endif /* FRTP_DEV_ERROR_DETECT */
    {
        /* FrTp_CancelReceive functionality is not supported right now. */
        /* Reception is not cancelled. */
        RetCode = E_NOT_OK;
    }

    DBG_FRTP_CANCELRECEIVE_EXIT(RetCode,FrTpRxSduId);
    return RetCode;
}


FUNC(Std_ReturnType,FRTP_CODE) FrTp_ChangeParameter
(
    VAR(PduIdType,AUTOMATIC) FrTpTxPduId,
    VAR(TPParameterType,AUTOMATIC) parameter,
    VAR(FrTp_ParameterValueType,AUTOMATIC) FrTpParameterValue
)
{
    Std_ReturnType RetCode = E_NOT_OK;

    DBG_FRTP_CHANGEPARAMETER_ENTRY(FrTpTxPduId,parameter,FrTpParameterValue);
    TS_PARAM_UNUSED(FrTpTxPduId);
    TS_PARAM_UNUSED(parameter);
    TS_PARAM_UNUSED(FrTpParameterValue);

/* check if development error detection is enabled */
#if (FRTP_DEV_ERROR_DETECT == STD_ON)

    /* check for successfully initialized module */
    /* Report to DET and return Error in case module was not initialized before */
    if (FrTp_InitStatus == FRTP_UNINIT)
    {
        (void)Det_ReportError(FRTP_MODULE_ID,
                              FRTP_DET_GENERIC_INSTANCE_ID,
                              FRTP_CHANGEPARAMETER_SERVICE_ID,
                              FRTP_E_UNINIT
                             );
    }
#endif /* FRTP_DEV_ERROR_DETECT */

    /* FrTp_ChangeParameter functionality is not supported right now. */

    DBG_FRTP_CHANGEPARAMETER_EXIT(RetCode,FrTpTxPduId,parameter,FrTpParameterValue);
    return RetCode;
}


FUNC(void,FRTP_CODE) FrTp_MainFunction(void)
{
    /* check for successfully initialized module */

    DBG_FRTP_MAINFUNCTION_ENTRY();
    if (FrTp_InitStatus == FRTP_INIT)
    {
        uint8_least iChannel = 0U;                           /* iterator variable for channels */
        P2VAR(FrTp_ChannelType, AUTOMATIC, FRTP_VAR) ActiveChannelTable[FRTP_CHAN_NUM];
        uint8_least ActiveChannelCount = 0U;

        /* loop over each channel and */
        /* a) forward timers */
        /* b) call the receive statemachine transmission routine */
        /* Flow control transmission (from receiver) are preferred over data transmissions   */
        /* we do this just once, since receiver send only one FC frame per cycle per channel */
        do
        {
            /* get pointer to right channel structure */
            CONSTP2VAR(FrTp_ChannelType, AUTOMATIC, FRTP_VAR) pChannel = &FrTp_Channel[iChannel];

            FrTp_HandleTimers(pChannel);
            FrTp_RxSm_TransmitSyncPoint(pChannel);
            iChannel++;
        } while(iChannel < FRTP_CHAN_NUM);

        /* Now iterate over channels as long as there are transmit requests for this cycle
        available. Transmit requests are active as long as data is available AND an TxPdus
        are still available for this channel. Indicate that this is the first transmission
        opportunity in this cycle. */
        iChannel = 0;
        do
        {
            /* get pointer to right channel structure */
            CONSTP2VAR(FrTp_ChannelType, AUTOMATIC, FRTP_VAR) pChannel = &FrTp_Channel[iChannel];

            /* store whether another transmission trigger would be needed by this channel */
            const boolean FurtherTransmitRequest = FrTp_TxSm_TransmitSyncPoint(pChannel,TRUE);

            /* track channels that need further transmission triggers */
            if(FurtherTransmitRequest)
            {
                ActiveChannelTable[ActiveChannelCount] = pChannel;
                ActiveChannelCount++;
            }
            iChannel++;
        } while(iChannel < FRTP_CHAN_NUM);

        /* Now iterate over channels as long as there are transmit requests
           for this cycle available. Transmit requests are active as long as
           data is available AND an TxPdus are still available for this channel.
           Indicate that this is not the first transmission opportunity in this cycle. */
        while(ActiveChannelCount != 0U)
        {
            /* store number of currently recorded channels active for transmission */
            const uint8_least ActiveChannelCountOld = ActiveChannelCount;
            uint8_least iActiveChannel;

            /* assume no more transmissions are active */
            ActiveChannelCount = 0U;

            /* iterate over all channels */
            for(iActiveChannel = 0U; iActiveChannel < ActiveChannelCountOld; iActiveChannel++)
            {
                /* store index of active Tx channel */
                CONSTP2VAR(FrTp_ChannelType, AUTOMATIC, FRTP_VAR) pChannel =
                        ActiveChannelTable[iActiveChannel];

                /* store whether another transmission trigger would be needed by this channel */
                const boolean FurtherTransmitRequest =
                    FrTp_TxSm_TransmitSyncPoint(pChannel,FALSE);

                /* track channels that need further transmission triggers */
                if(FurtherTransmitRequest)
                {
                    /* store index of still active tx channel */
                    ActiveChannelTable[ActiveChannelCount] = pChannel;
                    ActiveChannelCount++;
                }
            }
        }
    }

    DBG_FRTP_MAINFUNCTION_EXIT();
}


FUNC(uint8,FRTP_CODE) FrTp_GetFreeTxPdu
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    VAR(PduLengthType,AUTOMATIC) minLength
)
{
    uint8 iTxPdu; /* Pdu iterator */
    uint8 CandidateIdx = FRTP_TXPDU_INVALID;
    CONSTP2CONST(FrTp_ConnectionType,AUTOMATIC,FRTP_APPL_CONST) pConnection =
            FRTP_CFG_GET_PTR_TO_CONNECTION(pChannel->ConnectionIdx);


    if(((pConnection->TpOptions & FRTP_CONNECTION_HASBCLIMIT) != 0U) &&
       (((pChannel->Rx_State & FRTP_CHNL_STATE_DIRECTION_RX) != 0U) ||
        ((pChannel->Tx_CtrDataAck == 0U) && (pChannel->Tx_State == FRTP_CHNL_TX_DATA))
       )
      )
    {
      /* FrTpBandwidthLimitation is true AND
          this is a flow control frame OR the start frame of a new transmission
          -> in both cases the frame shall be sent using the first Pdu of the referenced PduPool*/
      if(((pChannel->Rx_State & FRTP_CHNL_STATE_DIRECTION_RX) != 0U) &&
         (FrTp_RuntimeTxPdu[pConnection->TxPduStartIdx].pChannel == NULL_PTR)
        )
      {
        /* it is a flow control frame AND the first Pdu of the corresponding PduPool is available
            -> get it */
        CandidateIdx = pConnection->TxPduStartIdx;
      }
      else if((pChannel->Tx_CtrDataAck == 0U) && (pChannel->Tx_State == FRTP_CHNL_TX_DATA))
      {
        /* it is the start frame of a new transmission
            -> get the first Pdu which is large enough to hold the frame */
        iTxPdu = pConnection->TxPduStartIdx;
        FRTP_PRECONDITION_ASSERT(iTxPdu <= pConnection->TxPduEndIdx, FRTP_MAINFUNCTION_SERVICE_ID);
        /* CHECK: NOPARSE */
        while(iTxPdu <= pConnection->TxPduEndIdx) /* this loop is exited normally only if there
                                                    are no PDUs of sufficient size configured
                                                    (which is prevented by an xdm check). */
        /* CHECK: PARSE */
        {
          /* get TxPdu information */
          CONSTP2CONST(FrTp_TxPduConfigType, AUTOMATIC, FRTP_APPL_CONST) pTxPdu =
                  FRTP_CFG_GET_PTR_TO_TXPDU(iTxPdu);
          if(pTxPdu->PduLength >= minLength)
          {
            if(FrTp_RuntimeTxPdu[iTxPdu].pChannel == NULL_PTR)
            {
               CandidateIdx = iTxPdu;
            }
            break;
          }
          iTxPdu++;
        }
        FRTP_PRECONDITION_ASSERT(iTxPdu <= pConnection->TxPduEndIdx, FRTP_MAINFUNCTION_SERVICE_ID);
      }
      else
      {
        /* trying to send CTS: bandwidth limitation is enabled, but first Pdu is already in use
            -> nothing to do */
      }
    }
    else
    {
        /*  FrTpBandwidthLimitation is false OR this is a consecutive or last frame
            -> in both cases the frame does not need to use the first Pdu of the referenced PduPool */

        /* try to find a free tx Pdu (in ascending order!!!) */
        for( iTxPdu = pConnection->TxPduStartIdx;
             iTxPdu <= pConnection->TxPduEndIdx;
             iTxPdu++
           )
        {
            /* is there an unused TxPdu ?? */
            if(FrTp_RuntimeTxPdu[iTxPdu].pChannel == NULL_PTR)
            {
                /* get TxPdu information */
                CONSTP2CONST(FrTp_TxPduConfigType, AUTOMATIC, FRTP_APPL_CONST) pTxPdu =
                        FRTP_CFG_GET_PTR_TO_TXPDU(iTxPdu);

                /* get the length of the Pdu into a local variable */
                const uint32 PduLength = pTxPdu->PduLength;

                /* save the first found candidate PDU */
                if (PduLength >= minLength)
                {
                    CandidateIdx = iTxPdu;
                    break;
                }
            }
        }
    }

    /* return first free TxPdu in this pool */
    return CandidateIdx;
}

#if (FRTP_LIMIT_NUMBER_CONNECTIONS == STD_ON)
FUNC(Std_ReturnType, FRTP_CODE) FrTp_BufferConnection
(
    VAR(PduIdType, AUTOMATIC) FrTpTxPduId,
    P2CONST(FrTp_ConnectionType, AUTOMATIC, FRTP_APPL_CONST) pConnection,
    P2CONST(PduInfoType,AUTOMATIC,FRTP_APPL_DATA) PduInfoPtr
)
{
    /* Return value */
    Std_ReturnType RetCode = E_NOT_OK;

    P2VAR(FrTp_BufferInfoType, AUTOMATIC, FRTP_VAR) pBufferInfo =
        &FrTp_ConnectionBuffer.BufferInfos[pConnection->ConnectionLimitConfigIdx];

    /* Check if buffer exists and is not full */
    if
    (
        (pBufferInfo->BufferSize != 0) &&
        (((pBufferInfo->IdxTail + 1U) % pBufferInfo->BufferSize) != pBufferInfo->IdxHead)
    )
    {
        /* Search for TxSduId in buffered connections */
        boolean found = FALSE;
        PduIdType iBuffer;
        for(
            iBuffer = pBufferInfo->IdxHead;
            iBuffer != pBufferInfo->IdxTail;
            iBuffer = (iBuffer + 1) % pBufferInfo->BufferSize
            )
        {
            if(pBufferInfo->BufferPtr[iBuffer].TxSduId == FrTpTxPduId)
            {
              found = TRUE;
              break;
            }
        }

        /* Search for TxSduId in active connections, i.e. channels */
        if(!found)
        {
            uint8_least iChannel;
            for(iChannel = 0U; iChannel < FRTP_CHAN_NUM; iChannel++)
            {
                /* Get pointer to channel structure */
                CONSTP2VAR(FrTp_ChannelType, AUTOMATIC, FRTP_VAR) pChannel =
                    &FrTp_Channel[iChannel];

                /* Check if non-idle channel is assigned for this TxPduId */
                if((pChannel->ConnectionIdx == FrTpTxPduId) &&
                    (pChannel->Tx_State != FRTP_CHNL_TX_IDLE))
                {
                    found = TRUE;
                    break;
                }
            }
        }
        if(!found)
        {
            /* Save Tx in buffer */
            pBufferInfo->BufferPtr[pBufferInfo->IdxTail].TxSduId = FrTpTxPduId;
            pBufferInfo->BufferPtr[pBufferInfo->IdxTail].SduLength = PduInfoPtr->SduLength;

            /* Advance tail index by one */
            pBufferInfo->IdxTail = (pBufferInfo->IdxTail + 1U) % pBufferInfo->BufferSize;

            RetCode = E_OK;
        }
    }
    return RetCode;
}
#endif /* FRTP_LIMIT_NUMBER_CONNECTIONS */

/*
 * Stop code section declaration
 */
#define FRTP_STOP_SEC_CODE
#include <FrTp_MemMap.h>
