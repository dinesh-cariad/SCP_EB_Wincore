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
#ifndef COMM_INT_H
#define COMM_INT_H

/*==================[inclusions]============================================*/

#include <ComM_DefProg_Cfg.h>
#include <ComM_BSW_Types.h>
#include <ComM_HsmComM.h>
#include <ComM_Types.h>
#include <ComM_Types_Int.h> /* PostBuild type definitions */
#ifdef COMM_PRECOMPILE_TIME_PBCFG_ACCESS
#include <ComM_PBcfg.h>
#endif
#if (defined TS_RELOCATABLE_CFG_ENABLE)
#error TS_RELOCATABLE_CFG_ENABLE already defined
#endif
#define TS_RELOCATABLE_CFG_ENABLE COMM_RELOCATABLE_CFG_ENABLE

#if (defined TS_PB_CFG_PTR_CLASS)
#error TS_PB_CFG_PTR_CLASS already defined
#endif
#define TS_PB_CFG_PTR_CLASS COMM_APPL_CONST

#if ((COMM_MULTICORE_ENABLED == STD_ON) && (COMM_NM_ACCESS_NEEDED == STD_ON))
#include <NmStack_Types.h>
#endif

#include <TSPBConfig.h>

/*==================[macros]================================================*/

/** \brief In case of Polyspace test COMM_PRECOMPILE_TIME_PBCFG_ACCESS is defined.
 **        Polyspace has issues with expanding postbuild macros.
 **        Macro has to be expanded manually
 */
#if (defined COMM_MANUAL_EXPAND_MACRO)
#error COMM_MANUAL_EXPAND_MACRO already defined
#endif
#ifdef COMM_PRECOMPILE_TIME_PBCFG_ACCESS
#if (COMM_RELOCATABLE_CFG_ENABLE == STD_ON)
#define COMM_MANUAL_EXPAND_MACRO STD_ON
#else /* COMM_RELOCATABLE_CFG_ENABLE */
#define COMM_MANUAL_EXPAND_MACRO STD_OFF
#endif /* COMM_RELOCATABLE_CFG_ENABLE */
#else /* COMM_PRECOMPILE_TIME_PBCFG_ACCESS */
#define COMM_MANUAL_EXPAND_MACRO STD_OFF
#endif /* COMM_PRECOMPILE_TIME_PBCFG_ACCESS */

/** \brief Represent the mask on the last 3 bits.
 ** This macro is used for getting the bit index from a uint8
 */
#if (defined MASK_3_BITS)
#error MASK_3_BITS already defined
#endif
#define MASK_3_BITS  0x07U
/** \brief Represent the 3 LSB masked by MASK_3_BITS
 ** This macro is used for getting the byte index from an array of uint8
 ** The 5 MSB represents the byte array index
 */
#if (defined SHIFT_3_BITS)
#error SHIFT_3_BITS already defined
#endif
#define SHIFT_3_BITS 0x03U

/** \brief Returns a pointer to a dynamic post build element.
 **
 ** This macro will function for both post-build selectable (by reference) and
 ** loadable (by offset) variants.
 **
 ** \param type The data type of the post-build member.
 ** \param element The symbol of post-build member to access.
 ** \retval Pointer the post-build member within the ConfigLayoutType
 ** structure. */
#if (defined COMM_PBCFG_ACCESS)
#error COMM_PBCFG_ACCESS already defined
#endif
#define COMM_PBCFG_ACCESS(type, element) \
    (TS_UNCHECKEDGETCFG(ComM_ConfigPtr, type, COMM, (element)))

/**
 ** \brief Returns the number of users assigned to a channel.
 **
 ** This macro will function for both post-build selectable (by reference) and
 ** loadable (by offset) variants.
 **
 ** \param channel the ID of the channel
 ** \retval the number of mapped users
 **/
#if (defined COMM_NUM_USERS_OF_CHANNEL)
#error COMM_NUM_USERS_OF_CHANNEL already defined
#endif
#define COMM_NUM_USERS_OF_CHANNEL(channel) (uint8) \
  ( (ComM_ConfigPtr->NumUsersOfAllChannel)[(channel)+1U] - \
    (ComM_ConfigPtr->NumUsersOfAllChannel)[(channel)] )

/**
 ** \brief Returns the user ID of a user assigned to a channel.
 **
 ** This macro will function for both post-build selectable (by reference) and
 ** loadable (by offset) variants.
 **
 ** \param channel the ID of the channel
 ** \param user index in the array of mapped users
 ** \retval the ID of the user
 **/
#if (defined COMM_USER_OF_CHANNEL)
#error COMM_USER_OF_CHANNEL already defined
#endif
#if (COMM_MANUAL_EXPAND_MACRO == STD_ON)
#define COMM_USER_OF_CHANNEL(channel,user) (uint8)\
  ((uint8 const * AUTOMATIC COMM_APPL_CONST)(void const * AUTOMATIC COMM_APPL_CONST) \
  &((uint8 const * AUTOMATIC COMM_APPL_CONST)(void const * AUTOMATIC COMM_APPL_CONST) \
  &ComM_Configuration)[ComM_Configuration.RootCfg.pUsersOfAllChannel]) \
  [ComM_Configuration.RootCfg.NumUsersOfAllChannel[(channel)] + (user)]
#else
#define COMM_USER_OF_CHANNEL(channel,user) (uint8)\
   ( COMM_PBCFG_ACCESS(uint8,ComM_ConfigPtr->pUsersOfAllChannel)[ \
       (ComM_ConfigPtr->NumUsersOfAllChannel)[(channel)] + (user) ])
#endif

/**
 ** \brief Returns the number of channels assigned to a user.
 **
 ** This macro will function for both post-build selectable (by reference) and
 ** loadable (by offset) variants.
 **
 ** \param user the ID of the User
 ** \retval the number of mapped channels
 **/
#if (defined COMM_NUM_CHANNELS_OF_USER)
#error COMM_NUM_CHANNELS_OF_USER already defined
#endif
#define COMM_NUM_CHANNELS_OF_USER(user) (uint8) \
  ( (ComM_ConfigPtr->NumChannelsOfAllUser)[(user)+1U] - \
    (ComM_ConfigPtr->NumChannelsOfAllUser)[(user)] )

/**
 ** \brief Returns the number of channels directly assigned to a user(not via a pnc).
 **
 ** This macro will function for both post-build selectable (by reference) and
 ** loadable (by offset) variants.
 **
 ** \param user the ID of the User
 ** \retval the number of mapped channels
 **/
#if (defined COMM_DIRECT_NUM_CHANNELS_OF_USER)
#error COMM_DIRECT_NUM_CHANNELS_OF_USER already defined
#endif
#define COMM_DIRECT_NUM_CHANNELS_OF_USER(user) (uint8) \
  ( (ComM_ConfigPtr->DirectNumChannelsOfAllUser)[(user)+1U] - \
    (ComM_ConfigPtr->DirectNumChannelsOfAllUser)[(user)] )

/**
 ** \brief Returns the channel ID of a channel assigned to a user.
 **
 ** This macro will function for both post-build selectable (by reference) and
 ** loadable (by offset) variants.
 **
 ** \param user the ID of the user
 ** \param channel index in the array of mapped channels
 ** \retval the ID of the channel
 **/
#if (defined COMM_CHANNEL_OF_USER)
#error COMM_CHANNEL_OF_USER already defined
#endif
#if (COMM_MANUAL_EXPAND_MACRO == STD_ON)
#define COMM_CHANNEL_OF_USER(user,channel) (uint8) \
  ((uint8 const * AUTOMATIC COMM_APPL_CONST)(void const * AUTOMATIC COMM_APPL_CONST) \
  &((uint8 const * AUTOMATIC COMM_APPL_CONST)(void const * AUTOMATIC COMM_APPL_CONST) \
  &ComM_Configuration)[ComM_Configuration.RootCfg.pChannelsOfAllUser]) \
  [ComM_Configuration.RootCfg.NumChannelsOfAllUser[(user)] + (channel)]
#else
#define COMM_CHANNEL_OF_USER(user,channel) (uint8) \
   ( COMM_PBCFG_ACCESS(uint8,ComM_ConfigPtr->pChannelsOfAllUser)[ \
       (ComM_ConfigPtr->NumChannelsOfAllUser)[(user)] + (channel) ])
#endif

#if ( COMM_PNC_SUPPORT == STD_ON )

/**
 ** \brief Evalutes TRUE if PNC support is enabled (FALSE if not).
 **/
#if (defined COMM_PNC_ENABLED)
#error COMM_PNC_ENABLED already defined
#endif
#define COMM_PNC_ENABLED ( (ComM_ConfigPtr->ComMPncEnabled) )


/**
 ** \brief Returns the number of channels associated with a PNC.
 **
 ** This macro will function for both post-build selectable (by reference) and
 ** loadable (by offset) variants.
 **
 ** \param pnc the index (not the ID) of the PNC
 ** \retval the number of mapped channels
 **/
#if (defined COMM_NUM_CHANNELS_OF_PNC)
#error COMM_NUM_CHANNELS_OF_PNC already defined
#endif
#define COMM_NUM_CHANNELS_OF_PNC(pnc) (uint8) \
  ( (ComM_ConfigPtr->NumChannelsOfAllPnc)[(pnc)+1U] - \
    (ComM_ConfigPtr->NumChannelsOfAllPnc)[(pnc)] )

/**
 ** \brief Returns a channel ID assigned to a PNC.
 **
 ** This macro will function for both post-build selectable (by reference) and
 ** loadable (by offset) variants.
 **
 ** \param pnc the index (not the ID) of the PNC
 ** \param channel index in the array of mapped channels
 ** \retval the ID of the channel
 **/
#if (defined COMM_CHANNEL_OF_PNC)
#error COMM_CHANNEL_OF_PNC already defined
#endif
#if (COMM_MANUAL_EXPAND_MACRO == STD_ON)
#define COMM_CHANNEL_OF_PNC(pnc,channel) (uint8)\
  ((uint8 const * AUTOMATIC COMM_APPL_CONST)(void const * AUTOMATIC COMM_APPL_CONST) \
  &((uint8 const * AUTOMATIC COMM_APPL_CONST)(void const * AUTOMATIC COMM_APPL_CONST) \
  &ComM_Configuration)[ComM_Configuration.RootCfg.pChannelsOfAllPnc]) \
  [ComM_Configuration.RootCfg.NumChannelsOfAllPnc[(pnc)] + (channel)]
#else
#define COMM_CHANNEL_OF_PNC(pnc,channel) (uint8)\
   ( COMM_PBCFG_ACCESS(uint8,ComM_ConfigPtr->pChannelsOfAllPnc)[ \
       (ComM_ConfigPtr->NumChannelsOfAllPnc)[(pnc)] + (channel) ])
#endif

/**
 ** \brief Returns the number of PNCs associated with a channel.
 **
 ** This macro will function for both post-build selectable (by reference) and
 ** loadable (by offset) variants.
 **
 ** \param channel the id of the channel
 ** \retval the number of mapped PNCs
 **/
#if (defined COMM_NUM_PNCS_OF_CHANNEL)
#error COMM_NUM_PNCS_OF_CHANNEL already defined
#endif
#define COMM_NUM_PNCS_OF_CHANNEL(channel) (uint8) \
  ( (ComM_ConfigPtr->NumPncOfAllCh)[(channel)+1U] - \
    (ComM_ConfigPtr->NumPncOfAllCh)[(channel)] )

/**
 ** \brief Returns a PNC ID assigned to a channel.
 **
 ** This macro will function for both post-build selectable (by reference) and
 ** loadable (by offset) variants.
 **
 ** \param channel the ID of the channel
 ** \param pnc index in the array of mapped PNCs
 ** \retval the ID of the PNC
 **/
#if (defined COMM_PNC_OF_CHANNEL)
#error COMM_PNC_OF_CHANNEL already defined
#endif
#if (COMM_MANUAL_EXPAND_MACRO == STD_ON)
#define COMM_PNC_OF_CHANNEL(channel,pnc) (uint8)\
  ((uint8 const * AUTOMATIC COMM_APPL_CONST)(void const * AUTOMATIC COMM_APPL_CONST) \
  &((uint8 const * AUTOMATIC COMM_APPL_CONST)(void const * AUTOMATIC COMM_APPL_CONST) \
  &ComM_Configuration)[ComM_Configuration.RootCfg.pPNCsOfAllChannel]) \
  [ComM_Configuration.RootCfg.NumPncOfAllCh[(channel)] + (pnc)]
#else
#define COMM_PNC_OF_CHANNEL(channel,pnc) (uint8)\
   ( COMM_PBCFG_ACCESS(uint8,ComM_ConfigPtr->pPNCsOfAllChannel)[ \
       (ComM_ConfigPtr->NumPncOfAllCh)[(channel)] + (pnc) ])
#endif

/**
 ** \brief Returns the number of PNCs associated with a user.
 **
 ** This macro will function for both post-build selectable (by reference) and
 ** loadable (by offset) variants.
 **
 ** \param user the id of the user
 ** \retval the number of mapped PNCs
 **/
#if (defined COMM_NUM_PNCS_OF_USER)
#error COMM_NUM_PNCS_OF_USER already defined
#endif
#define COMM_NUM_PNCS_OF_USER(user) (uint8) \
  ( (ComM_ConfigPtr->NumPncOfAllUser)[(user)+1U] - \
    (ComM_ConfigPtr->NumPncOfAllUser)[(user)] )

/**
 ** \brief Returns a PNC index (NOT THE ID) assigned to a user.
 **
 ** This macro will function for both post-build selectable (by reference) and
 ** loadable (by offset) variants.
 **
 ** \param user the ID of the user
 ** \param pnc index in the array of mapped PNCs
 ** \retval the index of the PNC
 **/
#if (defined COMM_PNC_OF_USER)
#error COMM_PNC_OF_USER already defined
#endif
#if (COMM_MANUAL_EXPAND_MACRO == STD_ON)
#define COMM_PNC_OF_USER(user,pnc) (uint8)\
  ((uint8 const * AUTOMATIC COMM_APPL_CONST)(void const * AUTOMATIC COMM_APPL_CONST) \
  &((uint8 const * AUTOMATIC COMM_APPL_CONST)(void const * AUTOMATIC COMM_APPL_CONST) \
  &ComM_Configuration)[ComM_Configuration.RootCfg.pPncOfAllUser]) \
  [ComM_Configuration.RootCfg.NumPncOfAllUser[(user)] + (pnc)]
#else
#define COMM_PNC_OF_USER(user,pnc) (uint8)\
   ( COMM_PBCFG_ACCESS(uint8,ComM_ConfigPtr->pPncOfAllUser)[ \
       (ComM_ConfigPtr->NumPncOfAllUser)[(user)] + (pnc) ])
#endif

/**
 ** \brief Returns the number of users associated with a PNC.
 **
 ** This macro will function for both post-build selectable (by reference) and
 ** loadable (by offset) variants.
 **
 ** \param pnc the index of the PNC
 ** \retval the number of mapped PNCs
 **/
#if (defined COMM_NUM_USERS_OF_PNC)
#error COMM_NUM_USERS_OF_PNC already defined
#endif
#define COMM_NUM_USERS_OF_PNC(pnc) (uint8) \
  ( (ComM_ConfigPtr->NumUsersOfAllPnc)[(pnc)+1U] - \
    (ComM_ConfigPtr->NumUsersOfAllPnc)[(pnc)] )

/**
 ** \brief Returns a user id assigned to a PNC.
 **
 ** This macro will function for both post-build selectable (by reference) and
 ** loadable (by offset) variants.
 **
 ** \param pnc the index of the PNC
 ** \param user index in the array of mapped users
 ** \retval the ID of the user
 **/
#if (defined COMM_USER_OF_PNC)
#error COMM_USER_OF_PNC already defined
#endif
#if (COMM_MANUAL_EXPAND_MACRO == STD_ON)
#define COMM_USER_OF_PNC(pnc,user) (uint8)\
  ((uint8 const * AUTOMATIC COMM_APPL_CONST)(void const * AUTOMATIC COMM_APPL_CONST) \
  &((uint8 const * AUTOMATIC COMM_APPL_CONST)(void const * AUTOMATIC COMM_APPL_CONST) \
  &ComM_Configuration)[ComM_Configuration.RootCfg.pUsersOfAllPnc]) \
  [ComM_Configuration.RootCfg.NumUsersOfAllPnc[(pnc)] + (user)]
#else
#define COMM_USER_OF_PNC(pnc,user) (uint8)\
   ( COMM_PBCFG_ACCESS(uint8,ComM_ConfigPtr->pUsersOfAllPnc)[ \
       (ComM_ConfigPtr->NumUsersOfAllPnc)[(pnc)] + (user) ])
#endif
#endif /* ( COMM_PNC_SUPPORT == STD_ON ) */

/**
 ** \brief Returns between two numbers
 **
 ** \param a
 ** \param b
 ** \retval max(a,b)
 **/
#if (defined COMM_MAX)
#error COMM_MAX already defined
#endif
#define COMM_MAX(A,B) (((A) > (B)) ? (A):(B))

/**
 ** \brief Check if events are present in the channel HSM queue
 **
 ** This macro can be used to scheduale channel HSM execution
 ** only if events are pending.
 **
 ** \param hsmInstance HSM instance number
 ** \retval
 **/
#if (defined COMM_CH_HSM_EVENT_PENDING)
#error COMM_CH_HSM_EVENT_PENDING already defined
#endif
#define COMM_CH_HSM_EVENT_PENDING(hsmInstance) \
    (ComM_HsmScComM.instances[(hsmInstance)].evQueueFill != 0U)

/**
 ** \brief Check if events are present in the PNC HSM queue
 **
 ** This macro can be used to scheduale PNC HSM execution
 ** only if events are pending.
 **
 ** \param hsmInstance HSM instance number
 ** \retval
 **/
#if (defined COMM_PNC_HSM_EVENT_PENDING)
#error COMM_PNC_HSM_EVENT_PENDING already defined
#endif
#define COMM_PNC_HSM_EVENT_PENDING(hsmInstance) \
    (ComM_HsmScComMPnc.instances[(hsmInstance)].evQueueFill != 0U)



/*==================[type definitions]======================================*/

/**
 ** \brief Type to allow declaration of a do-action function pointer.
 **
 ** Replace HSM do-action. This type allow declaration of a function
 ** pointer which can be used to register a function cyclic execution
 ** in a state_entry and stop this cyclic execution by set to NULL on
 ** state_exit.
 **
 ** \param instIdx HSM instance number
 ** \retval void
 **/
typedef void (*ComM_DoActionType)(uint8 instIdx);



#if(COMM_VLAN_SUPPORT)
/**
 ** \brief Represent link between managed channel and managing channel.
 **
 ** \param managedCh    - channel which reference a managing channel
 ** \param managingCh   - channel referenced by a managed channel
 **/
typedef struct
{
    uint8  managedCh;
    uint8  managingCh;
}ComM_VlanTableStructType;

#endif

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define COMM_START_SEC_CODE
#include <ComM_MemMap.h>

/** \brief Proxy function for SchM_Enter_ComM_SCHM_COMM_EXCLUSIVE_AREA_0
 **
 ** This function was introduced to avoid RTE includes
 ** within compilation unit ComM.c
 **
 ** \return No return value. */
FUNC(void, COMM_CODE) ComM_Enter_SCHM_COMM_EXCLUSIVE_AREA_0 (
  void);

/** \brief Proxy function for SchM_Exit_ComM_SCHM_COMM_EXCLUSIVE_AREA_0
 **
 ** This function was introduced to avoid RTE includes
 ** within compilation unit ComM.c
 **
 ** \return No return value. */
FUNC(void, COMM_CODE) ComM_Exit_SCHM_COMM_EXCLUSIVE_AREA_0 (
  void);

/** \brief Evaluate ComM Communication Channels
 ** This function checks the ComM channel mode and emits specific event
 ** related to ComMode.
 ** If the ComM_CurrentChannelRequest RTE interface is enabled, it also
 ** signals the current state of this interface to the RTE.
 ** \param[in] Channel Network channel on which the mode transition
 ** has occurred. */
FUNC(void, COMM_CODE) ComM_EvaluateChannel
(
  uint8 Channel
);

/** \brief Wrapper function to access the ComM_UserRequestedComMode array
 *
 * Accessor function to retrieve the requested communication mode of a
 * given user.
 *
 * \param[in] User A user Id
 * \return The requested communication mode of a given user
 */
FUNC(ComM_ASR40_ModeType, COMM_CODE) ComM_EB_GetUserRequestedComMode
(
    uint8 User
);

/** \brief Wrapper function to access the ComM_RequestedComMode, ComM_RequestedComModeWithoutPnc array
 *
 * Accessor function to set the current requested communication mode.
 *
 * \param[in] Channel Network channel of interest
 * \param[in] ComMode The requested communication mode
 * \param[in] ComModeWithoutPncRequest The requested communication mode (user, managed, managing)
 */
#if (COMM_PNC_GW_ENABLED == STD_ON)
FUNC(void, COMM_CODE) ComM_EB_SetRequestedComMode
(
    uint8 Channel,
    ComM_ASR40_ModeType ComMode,
    ComM_ASR40_ModeType ComModeWithoutPncRequest
);
#else /* COMM_PNC_GW_ENABLED == STD_ON */
FUNC(void, COMM_CODE) ComM_EB_SetRequestedComMode
(
    uint8 Channel,
    ComM_ASR40_ModeType ComMode
);
#endif /* COMM_PNC_GW_ENABLED == STD_ON */

#if (COMM_INCLUDE_RTE == STD_ON)
#if (COMM_DEFAULT_ASR_SERVICE_API != COMM_SERVICE_API_NONE)

/** \brief Signal mode of a channel to a specific user via RTE
 *
 * The function receives a user and translates its channel mode into
 * the values expected by the RTE. The reported values depend
 * on the AUTOSAR service API selected as default.
 *
 * \param[in] User  User of channel whose mode must be reported via RTE */
FUNC(void, COMM_CODE) ComM_RteNotify(
    uint8               User,
    ComM_ASR40_ModeType CurComMMode
);

#endif /* (COMM_DEFAULT_ASR_SERVICE_API != COMM_SERVICE_API_NONE) */

#if (COMM_ENABLE_ASR32_SERVICE_API == STD_ON)

/** \brief Signal mode of a channel to a specific user via RTE
 *
 * The function receives a user and translates its channel mode into
 * the ASR32 values expected by the RTE.
 *
 * \param[in] User  User of channel whose mode must be reported via RTE */
FUNC(void, COMM_CODE) ComM_ASR32_RteNotify(
    uint8               User,
    ComM_ASR32_ModeType CurComMMode
);

#endif /* (COMM_ENABLE_ASR32_SERVICE_API == STD_ON) */

#if (COMM_ENABLE_ASR40_SERVICE_API == STD_ON)

/** \brief Signal mode of a channel to a specific user via RTE
 *
 * The function receives a user and translates its channel mode into
 * the ASR40 values expected by the RTE.
 *
 * \param[in] User  User of channel whose mode must be reported via RTE */
FUNC(void, COMM_CODE) ComM_ASR40_RteNotify(
    uint8               User,
    ComM_ASR40_ModeType CurComMMode
);

#endif /* (COMM_ENABLE_ASR40_SERVICE_API == STD_ON) */
#endif /* (COMM_INCLUDE_RTE == STD_ON) */

#if (COMM_PNC_SUPPORT == STD_ON)
/** \brief Wrapper function to access the ComM_PNCRequestedComMode array
 *
 * Accessor function to retrieve the requested communication mode of a
 * given PNC.
 *
 * \param[in] Channel
 * \param[in] PncPos
 * \return The requested communication mode of the given PNC
 */
FUNC(ComM_ASR40_ModeType, COMM_CODE) ComM_EB_GetPncRequestedComMode
(
  uint8 Channel,
  uint8 PncPos
);

#if (COMM_0_PNC_VECTOR_AVOIDANCE == STD_ON)
/** \brief Wrapper function to access the ComM_PNCRequestedComMode0PncVectorAvoidance array
 *
 * Accessor function to retrieve the requested communication mode of a
 * given PNC.
 *
 * \param[in] Channel
 * \param[in] PncPos
 * \return The requested communication mode of the given PNC
 */
FUNC(ComM_ASR40_ModeType, COMM_CODE) ComM_EB_GetPncRequestedComMode0PncVectorAvoidance
(
  uint8 Channel,
  uint8 PncPos
);
#endif
#endif /* (COMM_PNC_SUPPORT == STD_ON) */

/** \brief Send RTE mode notification if not allready sent.
 *
 * \param[in] instIdx HSM instance index.
 */
FUNC(void, COMM_CODE) ComM_RteNotificationDoAction(uint8 instIdx);


#define COMM_STOP_SEC_CODE
#include <ComM_MemMap.h>

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

#if (COMM_INCLUDE_RTE == STD_ON)
#define COMM_START_SEC_VAR_CLEARED_8
#include <ComM_MemMap.h>


/** \brief Status flag indicating if the Rte could be notified successfully. */
extern VAR(boolean, COMM_VAR) ComM_RteNotificationSuccessful[USER_ARRAY_SIZE];


#define COMM_STOP_SEC_VAR_CLEARED_8
#include <ComM_MemMap.h>
#endif /* #if (COMM_INCLUDE_RTE == STD_ON) */

#define COMM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <ComM_MemMap.h>

/** \brief Function pointer to routine schedualed from channel main function */
extern VAR(ComM_DoActionType, COMM_VAR) ComM_RteNotificationDoActFkp;

#define COMM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <ComM_MemMap.h>

/*==================[internal data]=========================================*/

#define COMM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <ComM_MemMap.h>

extern P2CONST(ComM_ConfigType,AUTOMATIC,COMM_APPL_CONST) ComM_ConfigPtr;

#if ((COMM_MULTICORE_ENABLED == STD_ON) && (COMM_NM_ACCESS_NEEDED == STD_ON))
extern VAR(Nm_ModeType,COMM_VAR) ComM_LastNmMode[COMM_NUM_CHANNELS];
#endif

#define COMM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <ComM_MemMap.h>

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#undef TS_RELOCATABLE_CFG_ENABLE
#undef TS_PB_CFG_PTR_CLASS

#endif /* ifndef COMM_INT_H */
/*==================[end of file]===========================================*/
