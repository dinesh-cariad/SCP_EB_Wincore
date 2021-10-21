/**
 * \file
 *
 * \brief AUTOSAR Dcm
 *
 * This file contains the implementation of the AUTOSAR
 * module Dcm.
 *
 * \version 5.0.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef DCM_COMMUNICATIONCONTROL_CFG_H
#define DCM_COMMUNICATIONCONTROL_CFG_H

/* !LINKSTO Dcm.Dsn.File.CommunicationControl.ConfigH,1 */

/*==================[includes]===================================================================*/

[!AUTOSPACING!]
#include <Std_Types.h>
[!INCLUDE "../include/Dcm_common.m"!]
[!IF "$DCM_MODEDECLARATION_SUPPORT = 'STD_ON'"!]
#include <Dcm_Int.h>                                   /* Definition of Dcm_ModeRuleFunctionType */
[!ELSE!]
#include <ComM_Dcm.h>                                         /* Definition of NetworkHandleType */
[!ENDIF!]

/*==================[macros]=====================================================================*/

/** \brief Support for resetting of channel state on failure of the
 **        DcmDspComControlCommunicationReEnableModeRuleRef mode rule check */
#if (defined DCM_DSP_COMMUNICATIONCONTROL_REENABLE_MODE_RULE_SUPPORT)
  #error "DCM_DSP_COMMUNICATIONCONTROL_REENABLE_MODE_RULE_SUPPORT is already defined"
#endif
#define DCM_DSP_COMMUNICATIONCONTROL_REENABLE_MODE_RULE_SUPPORT            [!//
[!IF "$DCM_MODEDECLARATION_SUPPORT = 'STD_ON' and (node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspComControl/DcmDspComControlSetting/DcmDspComControlCommunicationReEnableModeRuleRef))"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!//

[!IF "$DCM_DSP_USE_SERVICE_0X28 = 'STD_ON'"!]

#if (defined DCM_0X28_0X00_SSVC_ENABLED ) /* To prevent double declaration */
#error DCM_0X28_0X00_SSVC_ENABLED  already defined
#endif /* if (defined DCM_0X28_0X00_SSVC_ENABLED ) */

/** \brief Subservice 0x00 for 0x28 enabled or not */
#define DCM_0X28_0X00_SSVC_ENABLED                                         [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='40']/*/*[DcmDsdSubServiceId='0' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_0X28_0X01_SSVC_ENABLED ) /* To prevent double declaration */
#error DCM_0X28_0X01_SSVC_ENABLED  already defined
#endif /* if (defined DCM_0X28_0X01_SSVC_ENABLED ) */

/** \brief Subservice 0x01 for 0x28 enabled or not */
#define DCM_0X28_0X01_SSVC_ENABLED                                         [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='40']/*/*[DcmDsdSubServiceId='1' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_0X28_0X02_SSVC_ENABLED ) /* To prevent double declaration */
#error DCM_0X28_0X02_SSVC_ENABLED  already defined
#endif /* if (defined DCM_0X28_0X02_SSVC_ENABLED ) */

/** \brief Subservice 0x02 for 0x28 enabled or not */
#define DCM_0X28_0X02_SSVC_ENABLED                                         [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='40']/*/*[DcmDsdSubServiceId='2' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_0X28_0X03_SSVC_ENABLED ) /* To prevent double declaration */
#error DCM_0X28_0X03_SSVC_ENABLED  already defined
#endif /* if (defined DCM_0X28_0X03_SSVC_ENABLED ) */

/** \brief Subservice 0x03 for 0x28 enabled or not */
#define DCM_0X28_0X03_SSVC_ENABLED                                         [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='40']/*/*[DcmDsdSubServiceId='3' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_0X28_0X04_SSVC_ENABLED ) /* To prevent double declaration */
#error DCM_0X28_0X04_SSVC_ENABLED  already defined
#endif /* if (defined DCM_0X28_0X04_SSVC_ENABLED ) */

/** \brief Subservice 0x04 for 0x28 enabled or not */
#define DCM_0X28_0X04_SSVC_ENABLED                                         [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='40']/*/*[DcmDsdSubServiceId='4' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_0X28_0X05_SSVC_ENABLED ) /* To prevent double declaration */
#error DCM_0X28_0X05_SSVC_ENABLED  already defined
#endif /* if (defined DCM_0X28_0X05_SSVC_ENABLED ) */

/** \brief Subservice 0x05 for 0x28 enabled or not */
#define DCM_0X28_0X05_SSVC_ENABLED                                         [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='40']/*/*[DcmDsdSubServiceId='5' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!][!//
[!//
[!IF "$DCM_MODEDECLARATION_SUPPORT = 'STD_ON'"!]
[!//
#if (defined DCM_NUM_COMMUNICATIONCONTROL_ALL_CHANNEL) /* To prevent double declaration */
#error DCM_NUM_COMMUNICATIONCONTROL_ALL_CHANNEL already defined
#endif /* if (defined DCM_NUM_COMMUNICATIONCONTROL_ALL_CHANNEL) */

/** \brief Number of Communication Control All channels */
#define DCM_NUM_COMMUNICATIONCONTROL_ALL_CHANNEL                           [!//
[!"num:integer( count( DcmConfigSet/*[1]/DcmDsp/DcmDspComControl/DcmDspComControlAllChannel/*))"!]U

#if (defined DCM_NUM_COMMUNICATIONCONTROL_SPECIFIC_CHANNEL) /* To prevent double declaration */
#error DCM_NUM_COMMUNICATIONCONTROL_SPECIFIC_CHANNEL already defined
#endif /* if (defined DCM_NUM_COMMUNICATIONCONTROL_SPECIFIC_CHANNEL) */

/** \brief Number of Communication Control Specific channels */
#define DCM_NUM_COMMUNICATIONCONTROL_SPECIFIC_CHANNEL                      [!//
[!"num:integer( count( DcmConfigSet/*[1]/DcmDsp/DcmDspComControl/DcmDspComControlSpecificChannel/*))"!]U

#if (defined DCM_NUM_COMMUNICATIONCONTROL_SUBNODE_CHANNEL) /* To prevent double declaration */
#error DCM_NUM_COMMUNICATIONCONTROL_SUBNODE_CHANNEL already defined
#endif /* if (defined DCM_NUM_COMMUNICATIONCONTROL_SUBNODE_CHANNEL) */

/** \brief Number of Communication Control SubNode channels */
#define DCM_NUM_COMMUNICATIONCONTROL_SUBNODE_CHANNEL                       [!//
[!"num:integer(count(DcmConfigSet/*[1]/DcmDsp/DcmDspComControl/DcmDspComControlSubNode/*))"!]U
[!//
[!ENDIF!][!//
[!//
[!VAR "ChannelList"="'#'"!]
[!VAR "LookUpTabSize" = "0"!]
  [!LOOP "DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*/DcmDslProtocolRx/*"!]
    [!IF "not( contains( $ChannelList, concat( '#', ./DcmDslProtocolRxChannelId,'#')))"!]
      [!VAR "LookUpTabSize" = "$LookUpTabSize + 1"!]
      [!VAR "ChannelList" = "concat( $ChannelList,'#',./DcmDslProtocolRxChannelId,'#')"!]
    [!ENDIF!]
  [!ENDLOOP!]
[!IF "$DCM_MODEDECLARATION_SUPPORT = 'STD_ON'"!]
  [!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspComControl/DcmDspComControlAllChannel/*"!]
    [!IF "not(contains($ChannelList, concat('#', as:ref(DcmDspAllComMChannelRef)/ComMChannelId, '#')))"!]
      [!VAR "LookUpTabSize" = "$LookUpTabSize + 1"!]
      [!VAR "ChannelList" = "concat($ChannelList, as:ref(DcmDspAllComMChannelRef)/ComMChannelId, '#')"!]
    [!ENDIF!]
  [!ENDLOOP!]
  [!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspComControl/DcmDspComControlSpecificChannel/*"!][!//
    [!IF "not(contains($ChannelList, concat('#', as:ref(DcmDspSpecificComMChannelRef)/ComMChannelId, '#')))"!]
      [!VAR "LookUpTabSize" = "$LookUpTabSize + 1"!]
      [!VAR "ChannelList" = "concat($ChannelList, as:ref(DcmDspSpecificComMChannelRef)/ComMChannelId, '#')"!]
    [!ENDIF!]
  [!ENDLOOP!]
  [!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspComControl/DcmDspComControlSubNode/*"!][!//
    [!IF "not(contains($ChannelList, concat('#', as:ref(DcmDspComControlSubNodeComMChannelRef)/ComMChannelId, '#')))"!]
      [!VAR "LookUpTabSize" = "$LookUpTabSize + 1"!]
      [!VAR "ChannelList" = "concat($ChannelList, as:ref(DcmDspComControlSubNodeComMChannelRef)/ComMChannelId, '#')"!]
    [!ENDIF!]
  [!ENDLOOP!]
[!ENDIF!]

#if (defined DCM_COMMUNICATIONCONTROL_LOOKUP_SIZE) /* To prevent double declaration */
#error DCM_COMMUNICATIONCONTROL_LOOKUP_SIZE already defined
#endif /* if (defined DCM_COMMUNICATIONCONTROL_LOOKUP_SIZE) */

/** \brief Size of array Dcm_CommunicationControlLookUpTable[] and
 **        Dcm_CommunicationControlLookUpTableComMode[].
 **
 ** If mode declaration is enabled than the DCM_COMMUNICATIONCONTROL_LOOKUP_SIZE will be equal with
 ** the number of channels referenced from RxConnections + number of channels from DcmDspComControl.
 ** If mode declaration is disable then the DCM_COMMUNICATIONCONTROL_LOOKUP_SIZE will be equal with
 ** the number of channels referenced from RxConnections.
 */
#define DCM_COMMUNICATIONCONTROL_LOOKUP_SIZE                               [!"num:integer( $LookUpTabSize)"!]U

/*==================[type definitions]===========================================================*/

[!IF "$DCM_MODEDECLARATION_SUPPORT = 'STD_ON'"!]
/** \brief Type of SchM Switch function for DcmCommunicationControl */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_SchMSwitchCommunicationControlFunctionType)
                                                                                       (uint8 mode);

/** \brief ComMChannel Id type for DcmDspComControlAllChannel */
typedef uint8 Dcm_CommunicationControlAllChannelType;

typedef struct
{
  /** \brief Network Id */
  uint8 ComMChannelId;

  /** \brief Subnet Number which controls the specific ComMChannel */
  uint8 SubnetNumber;
}
Dcm_CommunicationControlSpecificChannelType;

typedef struct
{
  /** \brief pointer to SchM Switch for DcmCommunicationControl */
  Dcm_SchMSwitchCommunicationControlFunctionType SchMSwitchCommunicationControl;

  /** \brief Network Id */
  uint8 ComMChannelId;
}
Dcm_CommunicationControlLookupType;

typedef struct
{
  /** \brief SubNode Id */
  uint16 SubNodeId;

  /** \brief Network Id */
  uint8 ComMChannelId;
}
Dcm_CommunicationControlSubNodeChannelType;
[!ENDIF!][!// DCM_MODEDECLARATION_SUPPORT = 'STD_ON'

/*==================[external function declarations]=============================================*/

/*==================[internal function declarations]=============================================*/

/*==================[internal constants]=========================================================*/

/*==================[internal data]==============================================================*/

/*==================[external constants]=========================================================*/

[!IF "$DCM_MODEDECLARATION_SUPPORT = 'STD_ON'"!]
#if(DCM_NUM_COMMUNICATIONCONTROL_ALL_CHANNEL > 0U)

#define DCM_START_SEC_CONST_8
#include <Dcm_MemMap.h>

/** \brief Global Array of configuration information for Communication Control All Channels.
 **        This array is used in case of a CommunicationControl request with the subnet parameter
 **        of the request equal to 0x00, for all channels the ID is taken from this array to can be
 **        found in the Dcm_CommunicationControlLookUpTable[] array.
 */
extern CONST(Dcm_CommunicationControlAllChannelType, DCM_CONST)
  Dcm_CommunicationControlAllChannel[DCM_NUM_COMMUNICATIONCONTROL_ALL_CHANNEL];

#define DCM_STOP_SEC_CONST_8
#include <Dcm_MemMap.h>
#endif

#define DCM_START_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

#if(DCM_NUM_COMMUNICATIONCONTROL_SPECIFIC_CHANNEL > 0U)
/** \brief Global array of configuration information for Communication Control Specific Channels.
 **        This array is used in case of a CommunicationControl request with subnet parameter
 **        between 0x01 and 0x0E, the requested channel ID is taken from this array to can be found
 **        in the Dcm_CommunicationControlLookUpTable[] array.
 */
extern CONST(Dcm_CommunicationControlSpecificChannelType, DCM_CONST)
  Dcm_CommunicationControlSpecificChannel[DCM_NUM_COMMUNICATIONCONTROL_SPECIFIC_CHANNEL];
#endif

#if (DCM_NUM_COMMUNICATIONCONTROL_SUBNODE_CHANNEL > 0U)
/** \brief Global array of configuration information for Communication Control SubNode Channels.
 **        This array is used in case of a CommunicationControl request with sub-function
 **        enableRxAndDisableTxWithEnhancedAddressInformation(0x04) or
 **        enableRxAndTxWithEnhancedAddressInformation(0x05), the channel ID is taken from this
 **        array to can be found in the Dcm_CommunicationControlLookUpTable[] array according with
 **        requested SubNodeId.
 */
extern CONST(Dcm_CommunicationControlSubNodeChannelType, DCM_CONST)
                         Dcm_CommunicationControlSubNodeChannel[DCM_NUM_COMMUNICATIONCONTROL_SUBNODE_CHANNEL];
#endif

#if(DCM_COMMUNICATIONCONTROL_LOOKUP_SIZE > 0U)
/** \brief Global array holding the ComM channels ID referenced from RxConnections and from
 **        DcmDspComControl(SpecificChannels/AllChannels/SubNodeChannels).
 **        This array is used to search the requested channel. Always when the communication
 **        mode is changed(0x28 request or communication control mode rule fail or session change)
 **        the channel is searched in this array and the same index on which it is found is changed
 **        with the request communication mode in the Dcm_CommunicationControl_CommunicationMode[]
 **        array.
 */
extern CONST(Dcm_CommunicationControlLookupType, DCM_CONST)
                          Dcm_CommunicationControlLookUpTable[DCM_COMMUNICATIONCONTROL_LOOKUP_SIZE];
#endif

[!IF "node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspComControl/DcmDspComControlSetting/DcmDspComControlCommunicationReEnableModeRuleRef)"!][!//
/** \brief Global constant holding the pointer to the mode rule function that controls
 **        resetting of communication mode.
 **        This pointer is used to determine if a communication mode reset is needed. This
 **        pointer will contain a mode rule function. This mode rule function is called in each
 **        main function cycle and if return true than the communication mode will be reset.
 */
extern CONST(Dcm_ModeRuleFunctionType, DCM_CONST) Dcm_Dsp_CommunicationControl_ModeRuleFunction;
[!ENDIF!][!//

#define DCM_STOP_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

[!//
[!ELSE!][!//
#if(DCM_COMMUNICATIONCONTROL_LOOKUP_SIZE > 0U)

#define DCM_START_SEC_CONST_8
#include <Dcm_MemMap.h>

/** \brief Global array holding the ComM channels ID referenced from RxConnections.
 **        This array is used to search the requested channel. Always when the communication
 **        mode is changed(0x28 request or session change) the channel is searched in this array and
 **        the same index on which it is found is changed with the request communication mode in the
 **        Dcm_CommunicationControl_CommunicationMode[] array.
 */
extern CONST(NetworkHandleType, DCM_CONST)
                          Dcm_CommunicationControlLookUpTable[DCM_COMMUNICATIONCONTROL_LOOKUP_SIZE];

#define DCM_STOP_SEC_CONST_8
#include <Dcm_MemMap.h>

#endif
[!ENDIF!][!//

/*==================[external data]==============================================================*/

/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/
[!ENDIF!][!// DCM_DSP_USE_SERVICE_0X28

#endif /* ifndef DCM_COMMUNICATIONCONTROL_CFG_H */
/*==================[end of file]================================================================*/
