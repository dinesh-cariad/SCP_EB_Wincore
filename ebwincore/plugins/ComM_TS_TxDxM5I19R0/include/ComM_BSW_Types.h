/**
 * \file
 *
 * \brief AUTOSAR ComM
 *
 * This file contains the implementation of the AUTOSAR
 * module ComM.
 *
 * \version 5.19.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef COMM_BSW_TYPES_H
#define COMM_BSW_TYPES_H

/*==================[inclusions]============================================*/

#include <ComStack_Types.h> /* Autosar standard types */
#include <ComM_Cfg.h>       /* Generated module configuration */

/*==================[macros]================================================*/
#if (defined COMM_NO_INTERNAL_AND_EXTERNAL_REQUESTS)
#error COMM_NO_INTERNAL_AND_EXTERNAL_REQUESTS already defined
#endif
#define COMM_NO_INTERNAL_AND_EXTERNAL_REQUESTS 0U

#if (defined COMM_NO_INTERNAL_REQUESTS)
#error COMM_NO_INTERNAL_REQUESTS already defined
#endif
#define COMM_NO_INTERNAL_REQUESTS 1U

/*------------------[values for ComM_ModeType]------------------------------*/
/* Communication mode values for ComM_ModeType
 *
 * Requirements [BSW00447] and [ComM956] say that:
 *   "Data Types used in Standard Interface and Standard AUTOSAR Interface shall
 *   only be defined in RTE Types Header file only"
 * which is bound to the case "RTE == ON".
 * However, due to 'AUTOSAR 3.2 support' implementation reasons we can't access
 * RTE specific macros here. Therefore sanity checks have been added to
 * ComM_Rte.c, ComM_ASR40_Rte.c, and ComM_ASR32_Rte.c to ensure that these
 * defines fit the defines from RTE. */
/* !LINKSTO ComM.Types_Implicit4,1, ComM828_Refine_7,1 */
#if (defined COMM_NO_COMMUNICATION)
#error COMM_NO_COMMUNICATION already defined
#endif
/** \brief No Communication mode value for \ref ComM_ModeType */
#define COMM_NO_COMMUNICATION     0U

/* !LINKSTO ComM828_Refine_8,1 */
#if (defined COMM_SILENT_COMMUNICATION)
#error COMM_SILENT_COMMUNICATION already defined
#endif
/** \brief Silent Communication mode value for \ref ComM_ModeType */
#define COMM_SILENT_COMMUNICATION 1U

/* !LINKSTO ComM828_Refine_9,1 */
#if (defined COMM_FULL_COMMUNICATION)
#error COMM_FULL_COMMUNICATION already defined
#endif
/** \brief Full Communication mode value for \ref ComM_ModeType */
#define COMM_FULL_COMMUNICATION   2U

/*------------------[values for ComM_StateType]------------------------------*/

#if (defined COMM_NO_COM_NO_PENDING_REQUEST)
#error COMM_NO_COM_NO_PENDING_REQUEST already defined
#endif
/* !LINKSTO ComM881,1 */
/** \brief No_Com_No_Pending_Request value for \ref ComM_StateType */
#define COMM_NO_COM_NO_PENDING_REQUEST     0U

#if (defined COMM_NO_COM_REQUEST_PENDING)
#error COMM_NO_COM_REQUEST_PENDING already defined
#endif
/* !LINKSTO ComM881,1 */
/** \brief No_Com_Pending_Request value for \ref ComM_StateType */
#define COMM_NO_COM_REQUEST_PENDING        1U

#if (defined COMM_FULL_COM_NETWORK_REQUESTED)
#error COMM_FULL_COM_NETWORK_REQUESTED already defined
#endif
/* !LINKSTO ComM880,1 */
/** \brief Full_Com_Network_Requested value for \ref ComM_StateType */
#define COMM_FULL_COM_NETWORK_REQUESTED    2U

#if (defined COMM_FULL_COM_READY_SLEEP)
#error COMM_FULL_COM_READY_SLEEP already defined
#endif
/* !LINKSTO ComM880,1 */
/** \brief Full_Com_Ready_Sleep value for \ref ComM_StateType */
#define COMM_FULL_COM_READY_SLEEP          3U

#if (defined COMM_SILENT_COM)
#error COMM_SILENT_COM already defined
#endif

/** \brief Silent Communication value for \ref ComM_StateType */
#define COMM_SILENT_COM                    4U

/*------------------[values for ComM_InitStatusType]-------------------------------------*/

/** \brief The COM Manager is not initialized or not usable. Default value after reset. */
#if (defined COMM_UNINIT)
#error COMM_UNINIT already defined
#endif
#define COMM_UNINIT 0U

/** \brief COM Manager is initialized and usable */
#if (defined COMM_INIT)
#error COMM_INIT already defined
#endif
#define COMM_INIT 1U

/*------------------[values for ComM_PncModeType]------------------------------*/

/** \brief PNC is requested by a local ComM user */
#if (defined PNC_REQUESTED)
#error PNC_REQUESTED already defined
#endif
#define PNC_REQUESTED 0U
/** \brief PNC is requested by a remote ComM user */
#if (defined PNC_READY_SLEEP)
#error PNC_READY_SLEEP already defined
#endif
#define PNC_READY_SLEEP 1U
/** \brief PNC is active with no deadline monitoring */
#if (defined PNC_PREPARE_SLEEP)
#error PNC_PREPARE_SLEEP already defined
#endif
#define PNC_PREPARE_SLEEP 2U
/** \brief PNC does not communicate */
#if (defined PNC_NO_COMMUNICATION)
#error PNC_NO_COMMUNICATION already defined
#endif
#define PNC_NO_COMMUNICATION 3U
/** \brief PNC is able to communicate */
#if (defined PNC_FULL_COMMUNICATION)
#error PNC_FULL_COMMUNICATION already defined
#endif
#define PNC_FULL_COMMUNICATION 4U

/*==================[type definitions]======================================*/

/** \brief Initialization status of ComM. */
/* !LINKSTO ComM.Types_Implicit1_Refine,1 */
typedef uint8 ComM_InitStatusType;

/* if RTE is included, the following typedefs will be provided by the RTE */
#if ((COMM_MODE_LIMITATION_ENABLED == STD_ON) || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON))

#ifndef RTE_TYPE_ComM_ASR32_InhibitionStatusType
#define RTE_TYPE_ComM_ASR32_InhibitionStatusType
/** \brief Inhibition status of ComM.
 **
 ** Defines whether a mode inhibition is active or not.
 ** - Bit 0 (LSB): Wake Up inhibition active.
 ** - Bit 1 (MSB): Limit to "No Communication" mode. */
typedef uint8 ComM_ASR32_InhibitionStatusType;
#endif /* RTE_TYPE_ComM_ASR32_InhibitionStatusType */

#ifndef RTE_TYPE_ComM_ASR40_InhibitionStatusType
#define RTE_TYPE_ComM_ASR40_InhibitionStatusType
/** \brief Inhibition status of ComM.
 **
 ** Defines whether a mode inhibition is active or not.
 ** - Bit 0 (LSB): Wake Up inhibition active.
 ** - Bit 1 (MSB): Limit to "No Communication" mode. */
/* !LINKSTO ComM.Types_Implicit2,1 */
typedef uint8 ComM_ASR40_InhibitionStatusType;
#endif /* RTE_TYPE_ComM_ASR40_InhibitionStatusType */
#endif /* COMM_MODE_LIMITATION_ENABLED ... */

#ifndef RTE_TYPE_ComM_ASR32_UserHandleType
#define RTE_TYPE_ComM_ASR32_UserHandleType
/** \brief Type for user of the communication manager.
 **
 ** Wraps the ComM_UserHandleType.
 **
 ** Range:
 ** - 0 ... 255 */
typedef uint8 ComM_ASR32_UserHandleType;
#endif /* RTE_TYPE_ComM_ASR32_UserHandleType */

#ifndef RTE_TYPE_ComM_ASR40_UserHandleType
#define RTE_TYPE_ComM_ASR40_UserHandleType
/** \brief Type for user of the communication manager.
 **
 ** Wraps the ComM_UserHandleType.
 **
 ** Range:
 ** - 0 ... 255 */
/* !LINKSTO ComM.Types_Implicit3,1 */
typedef uint8 ComM_ASR40_UserHandleType;
#endif /* RTE_TYPE_ComM_ASR40_UserHandleType */

#ifndef RTE_TYPE_ComM_ASR32_ModeType
#define RTE_TYPE_ComM_ASR32_ModeType
/** \brief Current mode of the Communication Manager (main state of the state
 * machine).
 *
 * If Rte usage is enabled in the ComM, the type ComM_ModeType is defined in
 * \verbatim Rte_Types.h\endverbatim and is made public indirectly by including
 * \verbatim Rte_Types.h\endverbatim in \verbatim ComM.h\endverbatim. */
typedef uint8 ComM_ASR32_ModeType;
#endif /* RTE_TYPE_ComM_ASR32_ModeType */

#ifndef RTE_TYPE_ComM_ASR40_ModeType
#define RTE_TYPE_ComM_ASR40_ModeType
/** \brief Current mode of the Communication Manager (main state of the state
 * machine).
 *
 * If Rte usage is enabled in the ComM, the type ComM_ModeType is defined in
 * \verbatim Rte_Types.h\endverbatim and is made public indirectly by including
 * \verbatim Rte_Types.h\endverbatim in \verbatim ComM.h\endverbatim. */
/* !LINKSTO ComM.Types_Implicit4,1 */
typedef uint8 ComM_ASR40_ModeType;
#endif /* RTE_TYPE_ComM_ASR40_ModeType */

/** \brief State and sub-state of ComM state machine
 **
 ** Range:
 ** 0 : COMM_NO_COM_NO_PENDING_REQUEST
 ** 1 : COMM_NO_COM_REQUEST_PENDING
 ** 2 : COMM_FULL_COM_NETWORK_REQUESTED
 ** 3 : COMM_FULL_COM_READY_SLEEP
 ** 4 : COMM_SILENT_COM
 */
/* !LINKSTO ComM.Types_Implicit5,1 */
typedef uint8 ComM_StateType;

#if ((COMM_MODE_LIMITATION_ENABLED == STD_ON) || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON))
/** \brief Inhibition status of ComM to be stored in NvM. */
typedef struct
{
  /** \brief number of rejected Full ComMode request */
  uint16                    InhibitionCounter;
  /** \brief Current ECU group classification */
  ComM_ASR40_InhibitionStatusType ECUGroupClassification;
#if (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)
  /** \brief Defines whether a WakeUp inhibition is active or not to
   ** Persistently stored.
   **
   ** This array is derived by calculating the number of channels configured
   ** in ComM. Each bit corresponds to a channel.  E.g : If there are more
   ** than 8 channels, size of COMM_NUM_BYTES_NVM will greater than 1 */
  uint8                     ChannelWakeUpInhibition[COMM_NUM_BYTES_NVM];
#endif
} ComM_NvDataType;
#endif

/* !LINKSTO ComM.Types_Implicit6_Refine,1 */
/** \brief Current mode of a PNC */
typedef uint8 ComM_PncModeType;

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef COMM_BSW_TYPES_H */
/*==================[end of file]===========================================*/
