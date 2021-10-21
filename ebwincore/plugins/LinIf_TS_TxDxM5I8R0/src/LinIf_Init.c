/**
 * \file
 *
 * \brief AUTOSAR LinIf
 *
 * This file contains the implementation of the AUTOSAR
 * module LinIf.
 *
 * \version 5.8.27
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]=============================================*/

#include <LinIf_Trace.h>

#define LINIF_NO_PBCFG_REQUIRED
#include <LinIf.h>            /* Module library API */
#include <LinIf_Int.h>        /* LinIf internal data */

#include <LinIf_Cfg.h>        /* Configurable parameters */
#include <LinIf_Lcfg.h>       /* Link time configurable parameters */

#if ((LINIF_DEV_ERROR_DETECT == STD_ON) || (LINIF_RUNTIME_ERROR_REPORTING == STD_ON))
#include <Det.h>              /* Default Error Tracer */
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) || (LINIF_RUNTIME_ERROR_REPORTING == STD_ON) */

#define LINIF_START_SEC_VAR_INIT_8
#include <LinIf_MemMap.h>

/** \brief Global state of the LIN Interface */
VAR(uint8, LINIF_VAR) LinIf_InitState = LINIF_UNINIT;

#define LINIF_STOP_SEC_VAR_INIT_8
#include <LinIf_MemMap.h>

#define LINIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <LinIf_MemMap.h>

/** \brief This pointer holds the root of the Configuration for the linIf. */
P2CONST(LinIf_ConfigType, LINIF_VAR, LINIF_APPL_CONST) LinIf_ConfigPtr;

#define LINIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <LinIf_MemMap.h>

/*==================[external function definitions]==========================*/

/* AUTOSAR Memory Mapping - start section for code */
#define LINIF_START_SEC_CODE
#include <LinIf_MemMap.h>

FUNC( Std_ReturnType, LINIF_CODE) LinIf_IsValidConfig
(
        P2CONST(void,AUTOMATIC,LINIF_APPL_CONST) voidConfigPtr
)
{
    /* Assume an invalid configuration */
    Std_ReturnType RetVal = E_NOT_OK;
    P2CONST(LinIf_ConfigType, AUTOMATIC, LINIF_APPL_CONST) ConfigPtr = voidConfigPtr;

    DBG_LINIF_ISVALIDCONFIG_ENTRY(voidConfigPtr);

    if (ConfigPtr != NULL_PTR)
    {
        /* Check if the configuration fits to the platform */
        if (TS_PlatformSigIsValid(ConfigPtr->PlatformSignature) == TRUE)
        {
            /* Validate the post build configuration against the compile time configuration */
            if (LINIF_CFG_SIGNATURE == ConfigPtr->CfgSignature)
            {
                /* Validate the post build configuration against the link time configuration */
                if (LinIf_LcfgSignature == ConfigPtr->LcfgSignature)
                {
                    /* Validate the post build configuration against the compile time published
                       information configuration */
                    if(LINIF_PUBLIC_INFO_SIGNATURE == ConfigPtr->PublicInfoSignature)
                    {
                        /* Indicate that the configuration is valid */
                        RetVal = E_OK;
                    }
                }
            }
        }
    }

    DBG_LINIF_ISVALIDCONFIG_EXIT(RetVal,voidConfigPtr);

    return RetVal;
}

/** \brief Initialize module
 **
 ** This function initializes the LIN Interface
 **
 ** \param[in] ConfigPtr Not used.
 **
 */
FUNC(void, LINIF_CODE) LinIf_Init
(
    P2CONST(LinIf_ConfigType, AUTOMATIC, LINIF_APPL_CONST) ConfigPtr
)
{

    P2CONST(LinIf_ConfigType,AUTOMATIC,LINIF_APPL_CONST) LocalConfigPtr = ConfigPtr;

    DBG_LINIF_INIT_ENTRY(ConfigPtr);

    /* Save the configuration pointer */
#if (LINIF_PBCFGM_SUPPORT_ENABLED == STD_ON)
    /* If the initialization function is called with an null pointer get the configuration from the
     * post build configuration manager */
    if (LocalConfigPtr == NULL_PTR)
    {
        PbcfgM_ModuleConfigPtrType ModuleConfig = NULL_PTR;
        if (E_OK == PbcfgM_GetConfig(
              LINIF_MODULE_ID,
              LINIF_INSTANCE_ID,
              &ModuleConfig))
        {
            LocalConfigPtr = (P2CONST(LinIf_ConfigType, AUTOMATIC, LINIF_APPL_CONST)) ModuleConfig;
        }
    }
#endif /* LINIF_PBCFGM_SUPPORT_ENABLED == STD_OFF */


#if (LINIF_DEV_ERROR_DETECT == STD_ON)
   /* check whether parameter LocalConfigPtr is a NULL_PTR */
    if(LocalConfigPtr == NULL_PTR)
    {
        (void)Det_ReportError(LINIF_MODULE_ID,
                              LINIF_INSTANCE_ID,
                              LINIF_API_INIT,
                              LINIF_E_PARAMETER_POINTER
                             );
    }

    else if ( E_OK != LinIf_IsValidConfig(LocalConfigPtr))
    {
        (void)Det_ReportError(LINIF_MODULE_ID,
                              LINIF_INSTANCE_ID,
                              LINIF_API_INIT,
                              LINIF_E_PARAMETER
                             );
    }
    else
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) */
    {
        uint8 Channel;

        /* Setup of the configurations pointers */
        LinIf_ConfigPtr = LocalConfigPtr;

        /* initialize common data */
        for (Channel = 0U ; Channel < (LinIf_ConfigPtr->NumChannelsMaster + LinIf_ConfigPtr->NumSlaveChannels) ; Channel++ )
        {
            CONSTP2VAR(LinIf_ChannelInfoType, AUTOMATIC, LINIF_VAR) pChannel = &LinIf_ChannelInfo[Channel];
            const uint8 StartupState = LINIF_CFG_GET_CHANNEL[Channel].CfgFlags & LINIF_SUSTATE_MASK;

            pChannel->SleepHandling = LINIF_NO_SLEEP_REQUEST;
            pChannel->LastDl = 1U;
            pChannel->LastHeaderType = LINIF_FRM_EMPTY;

            DBG_LINIF_CFSM_GRP(Channel,
                               (pChannel->cFSM),
                               StartupState
                              );

            pChannel->cFSM = StartupState;


#if ((defined LINIF_USE_LINAPI_REV2) || (defined LINIF_USE_LINAPI_REV3) || (defined LINIF_USE_LINAPI_42) || (defined LINIF_USE_LINAPI_REV44))

            /* since 4.2 MCAL drivers always start in OPERATIONAL mode, we need to
               make them go to sleep if LinIf channel also starts in sleep. Otherwise
               there will be an inconsistency between these modules, and the Lin
               communication will not start */
            /* !LINKSTO LinIf.EB.LinDriverToSleep,1 */
            if(StartupState == LINIF_CHANNEL_SLEEP)
            {

#if (LINIF_DEV_ERROR_DETECT == STD_ON)

                /* !LINKSTO LinIf.EB.LinDriverToSleep.E_NOT_OK,1 */
                if(E_NOT_OK == LinIf_LinDriverConfig[LINIF_GET_LIN_DRIVER_INDEX(Channel)].goToSleepInt(LINIF_GET_LIN_CHANNEL(Channel)))
                {
                    /* throw a Det in case we did not successfully make the driver channel go to sleep */
                    (void)Det_ReportError(LINIF_MODULE_ID,
                                          LINIF_INSTANCE_ID,
                                          LINIF_API_INIT,
                                          LINIF_DRIVER_CHANNEL_NOT_IN_SLEEP
                                         );
                }

#else

                (void)LinIf_LinDriverConfig[LINIF_GET_LIN_DRIVER_INDEX(Channel)].goToSleepInt(LINIF_GET_LIN_CHANNEL(Channel));

#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) */

            }

#endif /* (defined LINIF_USE_LINAPI_REV2) || (defined LINIF_USE_LINAPI_REV3) || (defined LINIF_USE_LINAPI_42) || (defined LINIF_USE_LINAPI_REV44) */

#if (LINIF_BUS_MIRRORING_SUPPORT_ENABLE == STD_ON)
            {
              uint8 i;
              for (i = 0U; i < 8U; i++)
              {
                pChannel->FrameInfo.FrameData[i] = 0U;
              }
              pChannel->FrameInfo.FrameLength = 0U;
              pChannel->FrameInfo.Pid = 0U;
            }
#endif /* LINIF_BUS_MIRRORING_SUPPORT_ENABLE == STD_ON */
        }

#if (LINIF_MASTER_SUPPORTED == STD_ON)
        /* now initialize master channels */
        for (Channel = 0U ; Channel < LinIf_ConfigPtr->NumChannelsMaster ; Channel++ )
        {
            CONSTP2VAR(LinIf_MasterChannelInfoType, AUTOMATIC, LINIF_VAR) pChannelMaster = &LinIf_MasterChannelInfo[Channel];

            const uint16 NullSchedule = LINIF_CFG_GET_CHANNEL_MASTER[Channel].SchTableInd;

            /* first table assigned to a channel is NULL_SCHEDULE */
            pChannelMaster->SchContTable = NullSchedule;

            DBG_LINIF_SCHCURRENTTABLE_GRP(Channel,
                                          (pChannelMaster->SchCurrentTable),
                                          (pChannelMaster->SchContTable)
                                         );

            pChannelMaster->SchCurrentTable = NullSchedule;
            pChannelMaster->SchROTable = LINIF_NULL_SCHEDULE;
            pChannelMaster->SchEntry = LINIF_CFG_GET_SCHTABLE[pChannelMaster->SchCurrentTable].EntryStartInd;

            /* initialize scheduling */
            pChannelMaster->ScheduleDelay = 1U;
            pChannelMaster->StatusDelay = 0U;
            pChannelMaster->ForceReschedule = TRUE;
            pChannelMaster->ResumeSch = FALSE;
#if (LINIF_EOS_NOTIF_CALLOUT_SUPPORTED == STD_ON)
            pChannelMaster->CallEOSNotif = FALSE;
#endif /* LINIF_EOS_NOTIF_CALLOUT_SUPPORTED == STD_ON */
            pChannelMaster->RestartCurrentTable = FALSE;
            pChannelMaster->SchCollRef = LINIF_INVALID_SCHEDULE_TABLE;
        }
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */

#if (LINIF_SLAVE_SUPPORTED == STD_ON)
        /* now initialize slave channels */
        for (Channel = LinIf_ConfigPtr->NumChannelsMaster ; Channel < (LinIf_ConfigPtr->NumChannelsMaster + LinIf_ConfigPtr->NumSlaveChannels) ; Channel++ )
        {
            CONSTP2CONST(LinIfSlaveChannelType, AUTOMATIC, LINIF_VAR) pIfChConfSlave =
                LINIF_CFG_GET_CHANNEL_SLAVE(Channel);
            CONSTP2VAR(LinIf_SlaveChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannelSlave =
                LINIF_GET_CHANNEL_SLAVE(Channel);

            /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00752,1 */
            pIfChannelSlave->TimerBusIdle = (LinIf_ChannelInfo[Channel].cFSM == LINIF_CHANNEL_SLEEP) ? 0U : pIfChConfSlave->BusIdleTimeout;
            /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00767.Configured,1 */
            pIfChannelSlave->ConfNad = pIfChConfSlave->NCConfNad;
            pIfChannelSlave->TimerNas = 0U;
            pIfChannelSlave->RSID = 0U;
            pIfChannelSlave->Flags = 0U;

            {
                P2CONST(LinIf_SlaveFrameTypesType, AUTOMATIC, LINIF_VAR) pSlFrameType;
                uint16 i;
                /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00768,1 */
                for(i = pIfChConfSlave->FrameStartInd; i < (pIfChConfSlave->FrameStartInd + pIfChConfSlave->FrameNumAssoc); i++)
                {
                    pSlFrameType = &LINIF_CFG_GET_SLAVEFRTYPE[i];
                    switch(pSlFrameType->LinIfFrameType)
                    {
                        case LINIF_FRM_RX:
                          LinIf_PidTable[i] = LINIF_CFG_GET_RXFRAME[pSlFrameType->LinIfFrameIdx].PID;
                            break;
                        case LINIF_FRM_TX:
                          LinIf_PidTable[i] = LINIF_CFG_GET_TXFRAME[pSlFrameType->LinIfFrameIdx].PID;
                            break;
                        case LINIF_FRM_EVENT_TRIGGERED:
                          LinIf_PidTable[i] = LINIF_CFG_GET_EVTTRIGGERED[pSlFrameType->LinIfFrameIdx].PID;
                            break;
                        /* CHECK: NOPARSE */
                        default:
                        /*should not happen*/
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
                        LINIF_UNREACHABLE_CODE_ASSERT(LINIF_API_INIT);
#endif /* LINIF_DEV_ERROR_DETECT == STD_ON */
                        break;
                        /* CHECK: PARSE */
                    }
                }
            }
        }
#endif /* LINIF_SLAVE_SUPPORTED == STD_ON */

#if (LINIF_BUS_MIRRORING_SUPPORT_ENABLE == STD_ON)
        LinIf_BusMiroringChannelEnabled = 0U;
#endif /* LINIF_BUS_MIRRORING_SUPPORT_ENABLE == STD_ON */
        /* set global state to INIT */

        DBG_LINIF_INITSTATE((LinIf_InitState),(LINIF_INIT));

        LinIf_InitState = LINIF_INIT;
    }

    DBG_LINIF_INIT_EXIT(ConfigPtr);
}

/* AUTOSAR Memory Mapping - end section for code */
#define LINIF_STOP_SEC_CODE
#include <LinIf_MemMap.h>

/*==================[end of file]============================================*/
