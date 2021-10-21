#ifndef NM_INT_H
#define NM_INT_H

/**
 * \file
 *
 * \brief AUTOSAR Nm
 *
 * This file contains the implementation of the AUTOSAR
 * module Nm.
 *
 * \version 5.12.8
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]============================================*/

#include <Nm.h>                 /* Nm public API incl. module config. */

#if (NM_DEV_ERROR_DETECT == STD_ON)
/* !LINKSTO Nm243,1 */
#include <Det.h>               /* Det API */
#endif

/*==================[macros]================================================*/

/** \brief Coordination cluster index special value for channels which do not
 * belong to a coordination cluster */
#if (defined NM_CC_IDX_INVALID)
#error NM_CC_IDX_INVALID already defined
#endif
#define NM_CC_IDX_INVALID   0xFFU

/** \brief Macro defines for setting Nm-channel-status as
 **        Nm-channel is requested by ComM */
#if (defined NM_CHANNEL_REQUESTED_BY_COMM)
#error NM_CHANNEL_REQUESTED_BY_COMM already defined
#endif
#define NM_CHANNEL_REQUESTED_BY_COMM 0x01U

/** \brief Macro defines for setting Nm-channel-status as
 **        Nm-channel is participating in the coordinated shutdown */
#if (defined NM_CHANNEL_PARTICIPATING_IN_CLUSTER)
#error NM_CHANNEL_PARTICIPATING_IN_CLUSTER already defined
#endif
#define NM_CHANNEL_PARTICIPATING_IN_CLUSTER 0x02U


/** \brief Macro defines for setting Nm-channel-status as
 **        Nm-channel is asleep */
#if (defined NM_CHANNEL_ASLEEP)
#error NM_CHANNEL_ASLEEP already defined
#endif
#define NM_CHANNEL_ASLEEP 0x04U

/** \brief Macro defines for setting Nm-channel-status as channel
 **        has no active remote nodes - all remote nodes are inactive */
#if (defined NM_CHANNEL_REMOTE_NODES_INACTIVE)
#error NM_CHANNEL_REMOTE_NODES_INACTIVE already defined
#endif
#define NM_CHANNEL_REMOTE_NODES_INACTIVE 0x08U

/** \brief Id of instance of Nm module */
#if (defined NM_INSTANCE_ID)
#error NM_INSTANCE_ID already defined
#endif
#define NM_INSTANCE_ID     0U

/* define various macros used for development error reporting, if development
 * error detection is enabled */
#if (NM_DEV_ERROR_DETECT == STD_ON)

/** \brief Macro for reporting an error to Det
 **
 ** \param[in] SID Service ID of the API function
 ** \param[in] ERROR_CODE Error code reported to Det module */
#if (defined NM_DET_REPORT_ERROR)
#error NM_DET_REPORT_ERROR already defined
#endif
#define NM_DET_REPORT_ERROR(SID,ERROR_CODE)                            \
  (void)Det_ReportError(NM_MODULE_ID, NM_INSTANCE_ID, (SID), (ERROR_CODE))

#endif /* if (NM_DEV_ERROR_DETECT == STD_ON) */

/** \brief Answers if channel \p handle is in status \p status
 **
 ** \param[in] handle the channel to test
 ** \param[in] the status to ask for */
#if (defined NM_IS_CHANNEL)
#error NM_IS_CHANNEL is already defined
#endif
#define NM_IS_CHANNEL(handle, status)  \
  ((Nm_ChannelStatus[(Nm_ChanIdxFromNetworkHandle[(handle)])] & (status)) != 0U)

/** \brief Answers if channel \p handle is not in status \p status
 **
 ** \param[in] handle the channel to test
 ** \param[in] status the status to ask for */
#if (defined NM_IS_NOT_CHANNEL)
#error NM_IS_NOT_CHANNEL is already defined
#endif
#define NM_IS_NOT_CHANNEL(handle, status) \
  ((Nm_ChannelStatus[(Nm_ChanIdxFromNetworkHandle[(handle)])] & (status)) == 0U)

/** \brief resets the channel \p handle to the value given by \p status
 **
 ** \param[in] handle the channel to reset
 ** \param[in] status the value to reset with */
#if (defined NM_RESET_CHANNEL)
#error NM_RESET_CHANNEL is already defined
#endif
#define NM_RESET_CHANNEL(handle, status) \
  (Nm_ChannelStatus[(Nm_ChanIdxFromNetworkHandle[(handle)])] = (status))

/** \brief sets the \p status of channel \p handle
 **
 ** \param[in] handle the channel to set
 ** \param[in] status the status to set */
#if (defined NM_SET_CHANNEL)
#error NM_SET_CHANNEL is already defined
#endif
#define NM_SET_CHANNEL(handle, status) \
  (Nm_ChannelStatus[(Nm_ChanIdxFromNetworkHandle[(handle)])] |= (status))

/** \brief unsets the \p status of channel \p handle
 **
 ** \param[in] handle the channel to unset
 ** \param[in] status the status to unset */
#if (defined NM_UNSET_CHANNEL)
#error NM_UNSET_CHANNEL is already defined
#endif
#define NM_UNSET_CHANNEL(handle, status) \
  (Nm_ChannelStatus[(Nm_ChanIdxFromNetworkHandle[(handle)])] &= (~status))

/** \brief Macro definition to access the
 ** correct channel for the NetworkHandle */
#if (defined NM_CHANNEL_CONFIG)
#error NM_CHANNEL_CONFIG is already defined
#endif
#define NM_CHANNEL_CONFIG(NetworkHandle) \
   Nm_ChannelConfig[Nm_ChanIdxFromNetworkHandle[(NetworkHandle)]]

/** \brief Special Value for the undefined channels */
#if (defined NM_INVALID_CHANNEL_ID)
#error NM_INVALID_CHANNEL_ID is already defined
#endif
#define NM_INVALID_CHANNEL_ID      0xFFU

/** \brief Macro definition to check the undefined Network Handle */
#if (defined NM_IS_NETWORK_HANDLE_INVALID)
#error NM_IS_NETWORK_HANDLE_INVALID is already defined
#endif
#define NM_IS_NETWORK_HANDLE_INVALID(NetworkHandle) \
          (((NetworkHandle) > NM_MAX_CHANNELID) || \
           (Nm_ChanIdxFromNetworkHandle[(NetworkHandle)] == NM_INVALID_CHANNEL_ID))

#if (defined NM_IS_NETWORK_HANDLE_VALID)
#error NM_IS_NETWORK_HANDLE_VALID is already defined
#endif
#define NM_IS_NETWORK_HANDLE_VALID(NetworkHandle)  \
          (((NetworkHandle) < NM_MAX_CHANNELID) && \
           (Nm_ChanIdxFromNetworkHandle[(NetworkHandle)] != NM_INVALID_CHANNEL_ID))

/*---------------[Network Management State(NMS) defines]-----------------*/
#if (NM_STATE_CHANGE_IND_ENABLED == STD_ON)

/** \brief Macro to identify call to Nm_StateChangeNotification() for a
 * transition which does not requires Com indication */
#if (defined NM_NMS_RESET)
#error NM_NMS_RESET is already defined
#endif
#define NM_NMS_RESET                0U

/** \brief Macro to indicate Network Management State (NMS)
 * NM in state RepeatMessage (transition from BusSleepMode) */
#if (defined NM_RM_BSM)
#error NM_RM_BSM is already defined
#endif
#define NM_RM_BSM                   1U

/** \brief Macro to indicate Network Management State (NMS)
 * NM in state RepeatMessage (transition from PrepareBusSleepMode) */
#if (defined NM_RM_PBSM)
#error NM_RM_PBSM is already defined
#endif
#define NM_RM_PBSM                  2U

/** \brief Macro to indicate Network Management State (NMS)
 * NM in state NormalOperation (transition from RepeatMessage) */
#if (defined NM_NO_RM)
#error NM_NO_RM is already defined
#endif
#define NM_NO_RM                    4U

/** \brief Macro to indicate Network Management State (NMS)
 * NM in state NormalOperation (transition from ReadySleep) */
#if (defined NM_NO_RS)
#error NM_NO_RS is already defined
#endif
#define NM_NO_RS                    8U

/** \brief Macro to indicate Network Management State (NMS)
 * NM in state RepeatMessage (transition from ReadySleep) */
#if (defined NM_RM_RS)
#error NM_RM_RS is already defined
#endif
#define NM_RM_RS                    16U

/** \brief Macro to indicate Network Management State (NMS)
 * NM in state RepeatMessage (transition from NormalOperation) */
#if (defined NM_RM_NO)
#error NM_RM_NO is already defined
#endif
#define NM_RM_NO                    32U

/** \brief Macro to indicate no ComSignal reference exists
 ** for a particular channel */
#if (defined NM_COM_SIGNAL_INVALID)
#error NM_COM_SIGNAL_INVALID is already defined
#endif
#define NM_COM_SIGNAL_INVALID       0xFFFFU

#endif

#if ( (NM_BUSNM_NUM > 1U)                        ||     \
      (NM_COORDINATOR_SUPPORT_ENABLED == STD_ON) ||     \
      (NM_STATE_REPORT_ENABLED == STD_ON)        ||     \
      (NM_CHANNEL_NUM > 1U)                             \
    )
  #define NM_NODE_DETECTION_ENABLED_VALUE(NetworkHandle) NM_CHANNEL_CONFIG(NetworkHandle).NmNodeDetectionEnabled
  #define NM_NODE_ID_ENABLED_VALUE(NetworkHandle) NM_CHANNEL_CONFIG(NetworkHandle).NmNodeIdEnabled
#else
  #if (NM_NODE_DETECTION_ENABLED == STD_ON)
    #define NM_NODE_DETECTION_ENABLED_VALUE(NetworkHandle) TRUE
  #else
    #define NM_NODE_DETECTION_ENABLED_VALUE(NetworkHandle) FALSE
  #endif
  #if (NM_NODE_ID_ENABLED == STD_ON)
    #define NM_NODE_ID_ENABLED_VALUE(NetworkHandle) TRUE
  #else
    #define NM_NODE_ID_ENABLED_VALUE(NetworkHandle) FALSE
  #endif
#endif

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

#if (NM_COORDINATOR_SUPPORT_ENABLED == STD_ON)

#define NM_START_SEC_CONST_UNSPECIFIED
#include <Nm_MemMap.h>

/** \brief Pointer to array of NM channels of a cluster */
extern CONSTP2CONST(NetworkHandleType, NM_CONST, NM_CONST)
  Nm_CcNmChannels[NM_CC_NUM];

#define NM_STOP_SEC_CONST_UNSPECIFIED
#include <Nm_MemMap.h>

#define NM_START_SEC_CONST_8
#include <Nm_MemMap.h>

/** \brief Number of NM channels in a coordinated cluster */
extern CONST(uint8, NM_CONST)
  Nm_CcNmChannelsNum[NM_CC_NUM];

#define NM_STOP_SEC_CONST_8
#include <Nm_MemMap.h>

#endif

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

#if ((NM_DEV_ERROR_DETECT == STD_ON) || (NM_COORDINATOR_SUPPORT_ENABLED == STD_ON))

#define NM_START_SEC_VAR_INIT_8
#include <Nm_MemMap.h>

/** \brief Intialization information of Nm module */
extern VAR(boolean, NM_VAR) Nm_Initialized;

#define NM_STOP_SEC_VAR_INIT_8
#include <Nm_MemMap.h>

#endif

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
#endif /* ifndef NM_INT_H */
