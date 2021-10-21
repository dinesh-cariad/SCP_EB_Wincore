/**
 * \file
 *
 * \brief AUTOSAR CanTp
 *
 * This file contains the implementation of the AUTOSAR
 * module CanTp.
 *
 * \version 6.8.42
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* tasking Deviation List
 *
 * TASKING-1) Deviated Rule: W549
 * W549: condition is always true
 *
 * Reason: False-positive from the Tasking v5.0r2 compiler (TCVX-41885)
 */

/*==================[inclusions]=============================================*/

#include <CanTp_Trace.h>
#include <TSAutosar.h>         /* EB specific standard types */
#include <ComStack_Types.h>    /* typedefs for AUTOSAR com stack */
#include <CanTp_Api.h>         /* Module public API */
#include <CanTp_Internal.h>    /* internal macros and variables */
#include <CanTp_InternalCfg.h> /* CanTp internal configuration */
#include <CanTp_Lcfg.h>         /* LinkTime configuration */
#include <TSPBConfig_Signature.h>           /* relocatable post-build macros */
#if(CANTP_USE_GPT == STD_ON)
#include <Gpt.h>               /* Gpt module types and API */
#endif

#if(CANTP_JUMPTABLE_SUPPORT == STD_ON)
#include <CanTp_EntryTable.h>  /* CanTp entry jumptable */
#endif /* CANTP_JUMPTABLE_SUPPORT == STD_ON */

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

#if(CANTP_JUMPTABLE_MODE == CANTP_JUMPTABLE_SERVER)
/** \brief Type for pointer to the entry table */
typedef P2CONST(CanTp_EntryTableType, TYPEDEF, CANTP_ENTRY_JUMP_TABLE)
  CanTp_EntryTablePtrType;
#elif(CANTP_JUMPTABLE_MODE == CANTP_JUMPTABLE_CLIENT)
/** \brief Type for pointer to initialization variables */
typedef P2VAR(CanTp_InitVariableType, TYPEDEF, CANTP_VAR)
  CanTp_InitVariablePtrType;
#endif /* CANTP_JUMPTABLE_MODE == CANTP_JUMPTABLE_SERVER */

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/
#define CANTP_START_SEC_VAR_INIT_UNSPECIFIED
#include <CanTp_MemMap.h>

#if(CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT)
/** \brief CanTp global configuration */
P2CONST(CanTp_ConfigType, CANTP_VAR, CANTP_APPL_CONST) CanTp_CfgPtr = NULL_PTR;
#endif /* CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT */

#define CANTP_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <CanTp_MemMap.h>
/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

#if CANTP_ISVALIDCONFIG_MMAP_ENABLED == STD_ON
#define CANTP_START_SEC_CODE_CC_BLOCK
#else
#define CANTP_START_SEC_CODE
#endif
#include <CanTp_MemMap.h>

/** \brief CanTp_IsValidConfig()
 **
 ** This API-Service Checks if the post build configuration fits to the
 ** link time configuration part.
 */
FUNC(Std_ReturnType, CANTP_CODE) CanTp_IsValidConfig
(
  P2CONST(void,AUTOMATIC,CANTP_APPL_CONST) voidConfigPtr
)
{
  /* Assume an invalid configuration */
  Std_ReturnType RetVal = E_NOT_OK;
  P2CONST(CanTp_ConfigType,AUTOMATIC,CANTP_APPL_CONST) ConfigPtr = voidConfigPtr;

  DBG_CANTP_ISVALIDCONFIG_ENTRY(voidConfigPtr);

  if(ConfigPtr != NULL_PTR)
  {
   /* Check if the configuration fits to the platform */
   if(TS_PlatformSigIsValid(ConfigPtr->PlatformSignature) == (boolean)TRUE)
   {
     /* Validate the post build configuration against the compile time configuration */
     if(CANTP_CFG_SIGNATURE == ConfigPtr->CfgSignature)
     {
       /* Validate the post build configuration against the link time configuration */
       if(CanTp_LcfgSignature == ConfigPtr->LcfgSignature)
       {
         /* Validate the post build configuration against the compile time published information
            configuration */
         if(CANTP_PUBLIC_INFO_SIGNATURE == ConfigPtr->PublicInfoSignature)
         {
           /* Indicate that the configuration is valid */
           RetVal = E_OK;
         }
       }
     }
   }
  }

  DBG_CANTP_ISVALIDCONFIG_EXIT(RetVal, voidConfigPtr);
  return RetVal;
}

#if CANTP_ISVALIDCONFIG_MMAP_ENABLED == STD_ON
#define CANTP_STOP_SEC_CODE_CC_BLOCK
#include <CanTp_MemMap.h>

#define CANTP_START_SEC_CODE
#include <CanTp_MemMap.h>
#endif

/** \brief CanTp_Init()
 **
 ** This API-Service is used to initialize or reinitialize the CanTp
 */
FUNC(void, CANTP_CODE) CanTp_Init
(
  P2CONST(CanTp_ConfigType, AUTOMATIC, CANTP_APPL_CONST) CfgPtr
)
{
  P2CONST(CanTp_ConfigType,AUTOMATIC,CANTP_APPL_CONST) LocalConfigPtr = CfgPtr;

  DBG_CANTP_INIT_ENTRY(CfgPtr);

  /* Save the configuration pointer */
#if(CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_SERVER)
#if(CANTP_PBCFGM_SUPPORT_ENABLED == STD_ON)
  /* If the initialization function is called with an null pointer get the configuration from the
   * post build configuration manager */
  if(LocalConfigPtr == NULL_PTR)
  {
    PbcfgM_ModuleConfigPtrType ModuleConfig = NULL_PTR;
    if(E_OK == PbcfgM_GetConfig(
                                 CANTP_MODULE_ID,
                                 CANTP_INSTANCE_ID,
                                 &ModuleConfig
                                )
      )
    {
      LocalConfigPtr = (P2CONST(CanTp_ConfigType, AUTOMATIC, CANTP_APPL_CONST)) ModuleConfig;
    }
  }
#endif /* CANTP_PBCFGM_SUPPORT_ENABLED == STD_OFF */
#endif /* CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_SERVER */

#if(CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT)

/* check if development error detection is enabled */
#if(CANTP_DEV_ERROR_DETECT == STD_ON)

  /* check whether parameter LocalConfigPtr is a NULL_PTR */
  if(LocalConfigPtr == NULL_PTR)
  {
    /* found a NULL_PTR, report to DET */
    CANTP_DET_REPORT_ERROR(
                            CANTP_MODULE_ID,
                            CANTP_INSTANCE_ID,
                            CANTP_API_INIT,
                            CANTP_E_PARAM_POINTER
                          );
  }
  /* check that configuration pointer is valid */
  /* !LINKSTO CanTp.EB.Init.ParamConfig, 1 */
  else if(E_OK != CanTp_IsValidConfig(LocalConfigPtr))
  {
    CANTP_DET_REPORT_ERROR(
                            CANTP_MODULE_ID,
                            CANTP_INSTANCE_ID,
                            CANTP_API_INIT,
                            CANTP_E_PARAM_CONFIG
                          );
  }
  else
#endif /* CANTP_DEV_ERROR_DETECT */

  /* perform functionality */
  {
    P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr;
    CanTp_ChType Channel; /* counter for channels */
    boolean ChannelInitFlag = FALSE;
#if((CANTP_DYNAMIC_NSA_ENABLED == STD_ON) || (CANTP_CHANGE_PARAMETER_REQ_API == STD_ON) || (CANTP_CHANGE_TX_PARAMETER_REQ_API == STD_ON))
    CanTp_RxNPduLookupEntryType Index; /* index for copying N_SA values and for copying STmin (Rx,Tx) and BS (Rx) configuration into RAM */
#endif
    boolean StopChannel = FALSE;

#if(CANTP_JUMPTABLE_MODE == CANTP_JUMPTABLE_SERVER)
    /* The following pointer is initialized to support the linker to not
     optimize the entry jump table away. It is not used for anything else.
    */
    volatile CanTp_EntryTablePtrType EntryTablePtr = &CanTp_EntryTable;
    TS_PARAM_UNUSED(EntryTablePtr);
#endif /* CANTP_JUMPTABLE_MODE == CANTP_JUMPTABLE_SERVER */

    CANTP_ENTER_CRITICAL_SECTION();
    /* making post-build configuration visible for whole module */
    /* !LINKSTO CanTp.ASR40.CANTP169, 1 */
    CanTp_CfgPtr = LocalConfigPtr;

    /* if CanTp is already activated */
    if(CanTp_InitVars.State == CANTP_ON)
    {
      StopChannel = TRUE;
      DBG_CANTP_INITVARS_STATE(CanTp_InitVars.State, CANTP_OFF);
      /* Deactivate CanTp to protect the initialization process from external influences. */
      CanTp_InitVars.State = CANTP_OFF;
    }
    else
    {
      /* CanTp is not activated */

#if(CANTP_DYNAMIC_NSA_ENABLED == STD_ON)
      /* copy N_SA - values from configuration */
      for(Index = 0U; Index < CanTp_CfgPtr->RxPduTableEntries; Index++)
      {
        /* Init runtime values for N_SA as default */
        CanTp_NSaValues[Index] = CANTP_CFG_GET_RXNPDU_TABLE(Index).N_SA_AE;
      }
#endif

      /* Initializes Counter for Non Idle channels */
      CanTp_NonIdleChannelCounter = 0U;

      /* Setting ChannelInitFlag to initialize each connection channel */
      ChannelInitFlag = TRUE;
    }

#if(CANTP_CHANGE_PARAMETER_REQ_API == STD_ON)
    for(Index = 0U; Index < CanTp_CfgPtr->NumberRxNSdus; Index++)
    {
      /* copy STmin configuration into RAM */
      CanTp_RxNSduStMinValues[Index] = CANTP_CFG_GET_RXNSDU_CONFIG(Index).STmin;
      /* copy BS configuration into RAM */
      CanTp_RxNSduBSValues[Index] = CANTP_CFG_GET_RXNSDU_CONFIG(Index).BS;
    }
#endif /* CANTP_CHANGE_PARAMETER_REQ_API == STD_ON */

#if(CANTP_CHANGE_TX_PARAMETER_REQ_API == STD_ON)
    for(Index = 0U; Index < CanTp_CfgPtr->NumberTxNSdus; Index++)
    {
      /* copy STmin configuration into RAM */
      CanTp_TxNSduStMinValues[Index] = 0xFFU;
    }
#endif /* CANTP_CHANGE_TX_PARAMETER_REQ_API == STD_ON */

    CANTP_LEAVE_CRITICAL_SECTION();

    if(StopChannel == (boolean)TRUE)
    {
      /* stop each channel */
      for(Channel = 0U; Channel < MAX_NUMBER_OF_CHANNELS; Channel++)
      {
        ChannelPtr = &CanTp_Channel[Channel];
        CanTp_StopChannel(ChannelPtr, CANTP_ULIND_STATUS, NTFRSLT_E_NOT_OK);
      }
    }

    if(ChannelInitFlag == (boolean)TRUE)
    {
    /* initialize ( = stop) each connection channel */
      for(Channel = 0U; Channel < MAX_NUMBER_OF_CHANNELS; Channel++)
      {
        ChannelPtr = &CanTp_Channel[Channel];

        /* set channel to IDLE */
        CanTp_InitChannelPtr(ChannelPtr);

#if(CANTP_STALLHANDLING_SUPPORT == STD_ON)
        /* Init channel flags which shall not be reset by CanTp_StopChannel() */
        CANTP_CLEAR_FLAG(ChannelPtr->PostFlag,CANTP_POST_RX_IND_MASK);
#endif
        ChannelPtr->ChannelLockFlag = 0U;
      }

#if(CANTP_USE_GPT == STD_ON)
      /* initialize each Gpt channel */
      /* Deviation TASKING-1 */
      for(Channel = 0U; Channel < MAX_NUMBER_OF_TX_CHANNELS; Channel++)
      {
        if(CANTP_CFG_GET_MFCTR_CONFIG(Channel).CallbackType == CANTP_GPT_CALLBACK)
        {
          CANTP_GPT_ENABLE_NOTIFICATION((Gpt_ChannelType) CANTP_CFG_GET_GPT_CHANNEL(Channel));
        }
      }
#endif

    }

    DBG_CANTP_INITVARS_STATE(CanTp_InitVars.State, CANTP_ON);
    /* set CanTp state to on */
    CanTp_InitVars.State = CANTP_ON;
  }
#else /* => CANTP_JUMPTABLE_MODE == CANTP_JUMPTABLE_CLIENT */
  {
    /* The following pointer is initialized to support the linker to not
       optimize the initialization variables away. It is not used for anything
       else. */
    volatile CanTp_InitVariablePtrType InitVarPtr = &CanTp_InitVars;
    TS_PARAM_UNUSED(InitVarPtr);

    (CanTp_EntryTablePtr->Init)(LocalConfigPtr);

  }
#endif /* CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT */

  DBG_CANTP_INIT_EXIT(CfgPtr);
}



/*------------------[channel initialisation]---------------------------------*/

/** \brief macro CANTP_INIT_CHANNEL_PTR
 **
 ** (Re-)Initializes the CanTp_Channel structure pointed to by the given
 ** ChannelPtr. Used in CanTp_Init() and CanTp_StopChannel().
 ** Note: does not reset the ChannelLockFlag.
 */
#if(CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT)

FUNC(void, CANTP_CODE) CanTp_InitChannelPtr
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr
)
{
  DBG_CANTP_CHANNEL_STATE_GRP(
                              DBG_CANTP_GET_CHANNEL_INDEX(ChannelPtr),
                              ChannelPtr->State,
                              CANTP_CH_IDLE
                             );
  TS_AtomicAssign8(ChannelPtr->State, CANTP_CH_IDLE);

  ChannelPtr->NSduId            = 0U;
  ChannelPtr->TotalLength       = 0U;
  ChannelPtr->ActualBufferSize  = 0U;
  ChannelPtr->ProcessedLength   = 0U;
  ChannelPtr->BlockSize         = 0U;
  ChannelPtr->ProcessedBlockFrames   = 0U;
  ChannelPtr->SequenceNumber    = 1U; /* since FF is 0 */
  ChannelPtr->TempLength        = 0U;
  ChannelPtr->WftCounter        = 0U;
  ChannelPtr->Ticks             = 0U;
  ChannelPtr->PostTicks         = 0U;
  CANTP_CLEAR_FLAG(ChannelPtr->PostFlag,CANTP_POST_TX_CONF_MASK);
  CANTP_CLEAR_FLAG(ChannelPtr->PostFlag,CANTP_POST_STMIN_MASK);
  ChannelPtr->StMin             = 0xFFU;
#if(CANTP_CHANGE_TX_PARAMETER_REQ_API == STD_ON)
  ChannelPtr->LastStMinFC       = 0xFFU;
#endif /* (CANTP_CHANGE_TX_PARAMETER_REQ_API == STD_ON) */
  ChannelPtr->MfCounterTicks    = 0U;
  ChannelPtr->CanIfDLC          = 0U;

  ChannelPtr->ConnectionFlags = 0U;

#if(CANTP_CANFD_ENABLED == STD_ON)
  /* initialize receivedSduLength */
  ChannelPtr->ReceivedSduLength = 0U;
#endif /* (CANTP_CANFD_ENABLED == STD_ON) */

  if(0U != CanTp_NonIdleChannelCounter)
  {
    CANTP_DEC_NON_IDLE_CHANNELS();
  }

}

#endif /* CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT */

#define CANTP_STOP_SEC_CODE
#include <CanTp_MemMap.h>

/*==================[internal function definitions]==========================*/

/*==================[end of file]============================================*/
