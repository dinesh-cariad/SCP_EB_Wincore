/**
 * \file
 *
 * \brief AUTOSAR FrIf
 *
 * This file contains the implementation of the AUTOSAR
 * module FrIf.
 *
 * \version 5.3.25
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*======================[Include Section]=====================================*/

#include <TSAtomic_Assign.h>
#include <TSPBConfig_Signature.h>
#include <FrIf_guards.h> /* Include define guards */
#include <FrIf_Trace.h>
#include <FrIf_Priv.h>

/*======================[Local Macros]========================================*/

/*======================[Global Data]=========================================*/

/* if not a constant configuration address is used - define pointer variable */
#if (FRIF_CONST_CFG_ADDRESS_ENABLE == STD_OFF)

/* start data section declaration */
#define FRIF_START_SEC_VAR_FAST_INIT_UNSPECIFIED
#include <FrIf_MemMap.h>  /* !LINKSTO FrIf.ASR40.FrIf05088,1 */

P2CONST(FrIf_ConfigType,FRIF_VAR_FAST,FRIF_APPL_CONST) FrIf_RootConfigPtr = NULL_PTR;

/* stop data section declaration */
#define FRIF_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
#include <FrIf_MemMap.h>  /* !LINKSTO FrIf.ASR40.FrIf05088,1 */

#endif /* FRIF_CONST_CFG_ADDRESS_ENABLE */

/* start data section declaration */
#define FRIF_START_SEC_VAR_INIT_8
#include <FrIf_MemMap.h> /* !LINKSTO FrIf.ASR40.FrIf05088,1 */

/* variable definition used for module initialization status checking */
VAR(uint8,FRIF_VAR) FrIf_ModuleInitialized = FRIF_UNINIT;

/* stop data section declaration */
#define FRIF_STOP_SEC_VAR_INIT_8
#include <FrIf_MemMap.h> /* !LINKSTO FrIf.ASR40.FrIf05088,1 */

/*======================[Global Functions]====================================*/

/* start code section declaration */
#define FRIF_START_SEC_CODE
#include <FrIf_MemMap.h>  /* !LINKSTO FrIf.ASR40.FrIf05088,1 */

FUNC( Std_ReturnType, FRIF_CODE) FrIf_IsValidConfig
(
        P2CONST(void,AUTOMATIC,FRIF_APPL_CONST) voidConfigPtr
)
{

    /* Assume an invalid configuration */
    Std_ReturnType RetValue = E_NOT_OK;
    P2CONST(FrIf_ConfigType,AUTOMATIC,FRIF_APPL_CONST) ConfigPtr = voidConfigPtr;

    DBG_FRIF_ISVALIDCONFIG_ENTRY(voidConfigPtr);

    if (ConfigPtr != NULL_PTR)
    {
        /* Check if the configuration fits to the platform */
        if (TS_PlatformSigIsValid(ConfigPtr->PlatformSignature) == TRUE)
        {
            /* Validate the post build configuration against the compile time configuration */
            if (FRIF_PUBLIC_INFO_SIGNATURE == ConfigPtr->PublicInfoSignature)
            {
                /* Validate the post build configuration against the compile time configuration */
                if (FRIF_CFG_SIGNATURE == ConfigPtr->CfgSignature)
                {
                    /* Validate the post build configuration against the link time configuration */
                    if (FrIf_LcfgSignature == ConfigPtr->LcfgSignature)
                    {
                        /* Indicate that the configuration is valid */
                        RetValue = E_OK;
                    }
                }
            }
        }
    }

    /* return result */

    DBG_FRIF_ISVALIDCONFIG_EXIT(RetValue,voidConfigPtr);
    return RetValue;
}

/**
 * \brief   Initializes the FrIf according to the post-build-time configuration
 *          passed as argument.
 *
 * \param   FrIf_ConfigIdx (in)     Configuration index (currently not used - shall be 0)
 * \param   FrIf_ConfigPtr (in)     Address of module post-build-time configuration
 *
 */

FUNC(void,FRIF_CODE) FrIf_Init
    (
        P2CONST(FrIf_ConfigType,AUTOMATIC,FRIF_APPL_CONST) FrIf_ConfigPtr
    )
{
    /* local configuration pointer */
    P2CONST(FrIf_ConfigType, AUTOMATIC, FRIF_APPL_CONST) TmpCfgPtr =

/* check for constant configuration address */
#if (FRIF_CONST_CFG_ADDRESS_ENABLE == STD_OFF)    /* don't use constant configuration address */

        /* no constant config addess - take argument pointer */
        FrIf_ConfigPtr;

        DBG_FRIF_INIT_ENTRY(FrIf_ConfigPtr);
#if (FRIF_PBCFGM_SUPPORT_ENABLED == STD_ON)
        /* If the initialization function is called with an null pointer get the configuration from the
         * post build configuration manager */
        if (TmpCfgPtr == NULL_PTR)
        {
            PbcfgM_ModuleConfigPtrType ModuleConfig = NULL_PTR;
            if (E_OK == PbcfgM_GetConfig(
                  FRIF_MODULE_ID,
                  0U,
                  &ModuleConfig))
            {
                TmpCfgPtr = (P2CONST(FrIf_ConfigType, AUTOMATIC, FRIF_APPL_CONST)) ModuleConfig;
            }
        }
#endif /* FRIF_PBCFGM_SUPPORT_ENABLED == STD_OFF */

#else   /* FRIF_CONST_CFG_ADDRESS_ENABLE */

        /* constant configuration address - assign variable from address for local operation */
        FrIf_RootConfigPtr;

        DBG_FRIF_INIT_ENTRY(FrIf_ConfigPtr);

        TS_PARAM_UNUSED(FrIf_ConfigPtr);

#endif /* FRIF_CONST_CFG_ADDRESS_ENABLE */

/* check if development error detection is enabled */
#if (FRIF_DEV_ERROR_DETECT == STD_ON)

    /* check that configuration pointer is valid */
    if(NULL_PTR == TmpCfgPtr)
    {
        /* Report to DET */
        FrIf_Det_ReportError(FRIF_INIT_SERVICE_ID, FRIF_E_INV_POINTER);
    }

    else if ( E_OK != FrIf_IsValidConfig(TmpCfgPtr))
    {
        /* Report to DET */
        FrIf_Det_ReportError(FRIF_INIT_SERVICE_ID, FRIF_E_INV_POINTER);
    }
    else

#endif /* FRIF_DEV_ERROR_DETECT */

    /* initialize controller related data */
    {
/* create iterator code only if single controller optimization is disabled */
#if (FRIF_SINGLE_CLST_OPT_ENABLE == STD_OFF)

        /* Get number of controllers out of the configuration */
        const uint8 FrIf_NumClst =
                TmpCfgPtr->FrIf_ClstNum;

        /* create iterator variable */
        uint8 FrIf_ClstIdx;


        /* iterate over controllers ... */
        for(FrIf_ClstIdx = 0U; FrIf_ClstIdx < FrIf_NumClst; FrIf_ClstIdx++)
#endif /* FRIF_SINGLE_CLST_OPT_ENABLE */
        {
            /* Initialize FrIf state machine */
            TS_AtomicAssign8(FrIf_ClstRuntimeData[FRIF_CLSTIDX].FrIf_SyncState,FRIF_SYNCSTATE_ASYNC);
            TS_AtomicAssign8(FrIf_ClstRuntimeData[FRIF_CLSTIDX].FrIf_State,(uint8)FRIF_STATE_OFFLINE);
            DBG_FRIF_STATE_GRP(FRIF_CLSTIDX, (FrIf_ClstRuntimeData[FRIF_CLSTIDX].FrIf_State), (FRIF_STATE_OFFLINE));
            /* reset timeout counter */
            FrIf_ClstRuntimeData[FRIF_CLSTIDX].JobListTimeoutCounter = 0U;
#if (FRIF_BUSMIRRORINGSUPPORT_ENABLE == STD_ON)
            /* reset BusmirroringEnabled indicator */
            FrIf_ClstRuntimeData[FRIF_CLSTIDX].BusmirroringEnabled = FALSE;
#endif /* FRIF_BUSMIRRORINGSUPPORT_ENABLE */

        }

#if (FRIF_CONST_CFG_ADDRESS_ENABLE == STD_OFF)

      /* save configuration pointer */
      FrIf_RootConfigPtr = TmpCfgPtr;

#endif  /* FRIF_USE_CONST_CONFIG_ADDRESS */

      /* module was successfully initialized - set marker */

      DBG_FRIF_MODULEINITIALIZED((FrIf_ModuleInitialized),(FRIF_INIT));
      FrIf_ModuleInitialized = FRIF_INIT;
    }

    DBG_FRIF_INIT_EXIT(FrIf_ConfigPtr);
}

/* stop code section declaration */
#define FRIF_STOP_SEC_CODE
#include <FrIf_MemMap.h>  /* !LINKSTO FrIf.ASR40.FrIf05088,1 */

