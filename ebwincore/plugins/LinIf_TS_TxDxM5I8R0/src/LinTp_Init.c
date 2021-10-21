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

#include <LinIf_Cfg.h>        /* Generated configuration */
#include <TSAutosar.h>        /* EB specific standard types */

#if (LINIF_TP_SUPPORTED == STD_ON)

#define LINIF_NO_PBCFG_REQUIRED
#include <LinIf.h>            /* Module library API */
#include <LinIf_Int.h>        /* LinIf internal data */
#include <LinTp_Int.h>        /* LinTp internal data */

#if ((LINIF_DEV_ERROR_DETECT == STD_ON) || (LINIF_RUNTIME_ERROR_REPORTING == STD_ON))
#include <Det.h>              /* Default Error Tracer */
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) || (LINIF_RUNTIME_ERROR_REPORTING == STD_ON) */

#include <LinTp_Lcfg.h>


#define LINIF_START_SEC_VAR_INIT_8
#include <LinIf_MemMap.h>

/** \brief Global state of the LIN Transport Layer */
VAR(uint8, LINIF_VAR) LinTp_InitState = LINTP_UNINIT;

#define LINIF_STOP_SEC_VAR_INIT_8
#include <LinIf_MemMap.h>

#define LINIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <LinIf_MemMap.h>

/** \brief This pointer holds the root of the configuration for the TP. */
P2CONST(LinTp_ConfigType, LINIF_VAR, LINIF_VAR) LinTp_ConfigPtr;

#define LINIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <LinIf_MemMap.h>


/*==================[external function definitions]==========================*/

/* AUTOSAR Memory Mapping - start section for code */
#define LINIF_START_SEC_CODE
#include <LinIf_MemMap.h>

FUNC( Std_ReturnType, LINIF_CODE) LinTp_IsValidConfig
(
        P2CONST(void,AUTOMATIC,LINIF_APPL_CONST) voidConfigPtr
)
{
    /* Assume an invalid configuration */
    Std_ReturnType RetVal = E_NOT_OK;
    P2CONST(LinTp_ConfigType,AUTOMATIC,LINIF_APPL_CONST) ConfigPtr = voidConfigPtr;

    DBG_LINTP_ISVALIDCONFIG_ENTRY(voidConfigPtr);

    if (ConfigPtr != NULL_PTR)
    {
        /* Check if the configuration fits to the platform */
        if (TS_PlatformSigIsValid(ConfigPtr->PlatformSignature) == TRUE)
        {
            /* Validate the post build configuration against the compile time configuration */
            if (LINTP_CFG_SIGNATURE == ConfigPtr->CfgSignature)
            {
                /* Validate the post build configuration against the link time configuration */
                if (LinTp_LcfgSignature == ConfigPtr->LcfgSignature)
                {
                    /* Validate the post build configuration against the compile time published
                       information configuration */
                    if(LINTP_PUBLIC_INFO_SIGNATURE == ConfigPtr->PublicInfoSignature)
                    {
                        /* Indicate that the configuration is valid */
                        RetVal = E_OK;
                    }
                }
            }
        }
    }

    DBG_LINTP_ISVALIDCONFIG_EXIT(RetVal,voidConfigPtr);

    return RetVal;
}

/** \brief Initialize TP
 **
 ** This function initializes the LIN Transport Layer
 **
 ** \param[in] ConfigPtr Not used.
 */
FUNC(void, LINIF_CODE) LinTp_Init
(
    P2CONST(LinTp_ConfigType, AUTOMATIC, LINIF_APPL_CONST) ConfigPtr
)
{
    DBG_LINTP_INIT_ENTRY(ConfigPtr);

    LinTp_ConfigPtr = ConfigPtr;

    /* Save the configuration pointer */
#if (LINTP_PBCFGM_SUPPORT_ENABLED == STD_ON)
    /* If the initialization function is called with an null pointer get the configuration from the
     * post build configuration manager */
    if (LinTp_ConfigPtr == NULL_PTR)
    {
        PbcfgM_ModuleConfigPtrType ModuleConfig = NULL_PTR;
        if (E_OK == PbcfgM_GetConfig(
              LINTP_MODULE_ID,
              LINIF_INSTANCE_ID,
              &ModuleConfig))
        {
            LinTp_ConfigPtr = (P2CONST(LinTp_ConfigType, AUTOMATIC, LINIF_APPL_CONST)) ModuleConfig;
        }
    }
#endif /* LINTP_PBCFGM_SUPPORT_ENABLED == STD_OFF */

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    if (LINIF_UNINIT_ACCESS)
    {
        (void)Det_ReportError(LINIF_MODULE_ID,
                              LINIF_INSTANCE_ID,
                              LINTP_API_INIT,
                              LINIF_E_UNINIT
                             );
    }
    else if(LinTp_ConfigPtr == NULL_PTR)
    {
        (void)Det_ReportError(LINIF_MODULE_ID,
                              LINIF_INSTANCE_ID,
                              LINTP_API_INIT,
                              LINIF_E_PARAMETER_POINTER
                             );
    }
    /* check that configuration pointer is valid */
    else if ( E_OK != LinTp_IsValidConfig(LinTp_ConfigPtr))
    {
        (void)Det_ReportError(LINIF_MODULE_ID,
                              LINIF_INSTANCE_ID,
                              LINTP_API_INIT,
                              LINIF_E_PARAMETER
                             );
    }
    else
#endif
    {

      NetworkHandleType Channel;

      /* set channels & TP to INIT */
      for ( Channel = 0U ; Channel < (LinIf_ConfigPtr->NumChannelsMaster + LinIf_ConfigPtr->NumSlaveChannels) ; Channel++ )
      {
          LinTp_ChannelInfo[Channel].ChannelFlags = LINTP_FLAGS_CLEARED;

          DBG_LINTP_CHANNELINFO_PHYSCHANNELINFO_SID_GRP(Channel,
                                                        (LinTp_ChannelInfo[Channel].PhysChannelInfo.SID),
                                                        (LINTP_INVALID_SID)
                                                       );

          LinTp_ChannelInfo[Channel].PhysChannelInfo.SID = LINTP_INVALID_SID;

          DBG_LINTP_CHANNELINFO_PHYSCHANNELINFO_NAD_GRP(Channel,
                                                        (LinTp_ChannelInfo[Channel].PhysChannelInfo.NAD),
                                                        (LINTP_INVALID_NAD)
                                                       );

          LinTp_ChannelInfo[Channel].PhysChannelInfo.NAD = LINTP_INVALID_NAD;
          LinTp_ChannelInfo[Channel].PhysChannelInfo.Timer = 0U;
          LinTp_ChannelInfo[Channel].PhysChannelInfo.P2Timer = 0U;
          LinTp_ChannelInfo[Channel].PhysChannelInfo.PduRResult = LINTP_INVALID_PDUR_RESULT;

          DBG_LINTP_CHANNELINFO_PHYSCHANNELINFO_STATE_GRP(Channel,
                                                          (LinTp_ChannelInfo[Channel].PhysChannelInfo.State),
                                                          (LINTP_PHYS_STATE_IDLE)
                                                         );

          LinTp_ChannelInfo[Channel].PhysChannelInfo.State = LINTP_PHYS_STATE_IDLE;
#if (LINIF_MASTER_SUPPORTED == STD_ON)
          LinTp_ChannelInfo[Channel].FuncChannelInfo.Timer = 0U;

          DBG_LINTP_CHANNELINFO_FUNCCHANNELINFO_STATE_GRP(Channel,
                                                          (LinTp_ChannelInfo[Channel].FuncChannelInfo.State),
                                                          (LINTP_FUNC_STATE_IDLE)
                                                         );

          LinTp_ChannelInfo[Channel].FuncChannelInfo.State = LINTP_FUNC_STATE_IDLE;
          LinTp_ChannelInfo[Channel].FuncChannelInfo.PduRResult = LINTP_INVALID_PDUR_RESULT;
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */
      }


      DBG_LINTP_INITSTATE((LinTp_InitState),(LINTP_INIT));

      LinTp_InitState = LINTP_INIT;
    }

    DBG_LINTP_INIT_EXIT(ConfigPtr);
}

/* AUTOSAR Memory Mapping - end section for code */
#define LINIF_STOP_SEC_CODE
#include <LinIf_MemMap.h>

#else /* (LINIF_TP_SUPPORTED == STD_ON) */

/* prevent empty translation unit in case LinIf_Merged.c is not used */
#if (TS_MERGED_COMPILE == STD_OFF)

#include <TSCompiler.h>
TS_PREVENTEMPTYTRANSLATIONUNIT

#endif /* TS_MERGED_COMPILE */

#endif /* (LINIF_TP_SUPPORTED == STD_ON) */

/*==================[end of file]============================================*/
