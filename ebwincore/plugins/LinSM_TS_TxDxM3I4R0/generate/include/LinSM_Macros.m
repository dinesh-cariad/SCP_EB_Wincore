[!/**
 * \file
 *
 * \brief AUTOSAR LinSM
 *
 * This file contains the implementation of the AUTOSAR
 * module LinSM.
 *
 * \version 3.4.19
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */!][!//
[!NOCODE!]
[!IF "not(var:defined('LINSM_MACROS_M'))"!]
[!VAR "LINSM_MACROS_M"="'true'"!]

[!// ****************************************************
[!// error checking against LinIf post build paramters
[!// ****************************************************
[!LOOP "as:modconf('LinSM')[1]/LinSMConfigSet/*[1]/LinSMChannel/*"!]
  [!SELECT "LinSMComMNetworkHandleRef"!]
    [!IF "node:refvalid(.) and (count(as:modconf('LinIf')[1]/LinIfGlobalConfig/*[1]/LinIfChannel/*/LinIfComMNetworkHandleRef[. = node:current()]) != 1)"!]
      [!ERROR "LinSMConfigSet/LinSMChannel/LinSMComMNetworkHandleRef: A 1:1 relation between LinIf channels and LinSM channels is required. The ComMChannel referenced here is not referenced by exactly one LinIf channel."!]
    [!ENDIF!]
  [!ENDSELECT!]

  [!IF "LinSMNodeType = 'MASTER'"!]
  [!LOOP "LinSMSchedule/*"!]
    [!SELECT "LinSMScheduleIndexRef"!]
      [!IF "node:refvalid(.) and (not (node:exists(as:ref(.)/LinIfScheduleTableIndex)) or (as:ref(.)/LinIfScheduleTableIndex = ''))"!]
        [!ERROR "LinSMConfigSet/LinSMChannel/LinSMSchedule/LinSMScheduleIndexRef: The LinIfScheduleTable referenced needs to provide a valid LinIfScheduleTableIndex."!]
      [!ENDIF!]

      [!IF "text:uniq(../../*/LinSMScheduleIndexRef, .) != 'true'"!]
        [!ERROR "LinSMConfigSet/LinSMChannel/LinSMSchedule/LinSMScheduleIndexRef: A 1:1 relation between LinIf and LinSM schedule tables is required per channel. This schedule is referenced more than once."!]
      [!ENDIF!]

      [!// LinSM_ScheduleToIndex() supports only indices smaller than 255
      [!IF "node:refvalid(.) and (as:ref(.)/LinIfScheduleTableIndex >= 255)"!]
        [!ERROR "LinSMConfigSet/LinSMChannel/LinSMSchedule/LinSMScheduleIndexRef: The LinIfScheduleTable referenced must have a LinIfScheduleTableIndex smaller than 255."!]
      [!ENDIF!]

      [!// consistency: the schedule referenced should match the referenced channel
      [!IF "node:refvalid(.) and node:refvalid(../../../LinSMComMNetworkHandleRef) and (count(as:modconf('LinIf')[1]/LinIfGlobalConfig/*[1]/LinIfChannel/*[LinIfComMNetworkHandleRef = node:current()/../../../LinSMComMNetworkHandleRef]) = 1) and (as:path(as:ref(.)/../..) != as:path(as:modconf('LinIf')[1]/LinIfGlobalConfig/*[1]/LinIfChannel/*[LinIfComMNetworkHandleRef = node:current()/../../../LinSMComMNetworkHandleRef]))"!]
        [!ERROR "LinSMConfigSet/LinSMChannel/LinSMSchedule/LinSMScheduleIndexRef: The LinIfScheduleTable referenced does not belong to the LinIfChannel associated through the LinSMComMNetworkHandleRef reference."!]
      [!ENDIF!]
    [!ENDSELECT!]
  [!ENDLOOP!]
  [!ENDIF!]
[!ENDLOOP!]

[!/* overall number of channels to handle */!]
[!VAR "numChannels"="num:i(count(LinSMConfigSet/*[1]/LinSMChannel/*))"!]

[!/* level of sleep support used: 'all' channels, 'none', or 'mixed' */!]
[!IF "count(LinSMConfigSet/*[1]/LinSMChannel/*[LinSMSleepSupport = 'true']) = 0"!]
  [!VAR "sleepSupportType"="'none'"!]
[!ELSEIF "count(LinSMConfigSet/*[1]/LinSMChannel/*[LinSMSleepSupport = 'true']) = $numChannels"!]
  [!VAR "sleepSupportType"="'all'"!]
[!ELSE!]
  [!VAR "sleepSupportType"="'mixed'"!]
[!ENDIF!]

[!/* level of transceiver support used: 'allPassive', 'passiveNone', 'mixed' or none */!]
[!VAR "numChTrcv" = "num:i(count(LinSMConfigSet/*[1]/LinSMChannel/*[LinSMTransceiverPassiveMode]))"!]
[!VAR "numChTrcvP" = "num:i(count(LinSMConfigSet/*[1]/LinSMChannel/*[LinSMTransceiverPassiveMode = 'true']))"!]
[!IF "$numChTrcv = 0"!]
  [!VAR "trcvSupportType" = "'none'"!]
[!ELSEIF "$numChTrcv = $numChTrcvP"!]
  [!VAR "trcvSupportType" = "'allPassive'"!]
[!ELSEIF "$numChTrcvP = 0"!]
  [!VAR "trcvSupportType" = "'passiveNone'"!]
[!ELSE!]
  [!VAR "trcvSupportType" = "'mixed'"!]
[!ENDIF!]

[!/* determine if muticore support is enabled */!]
[!VAR "multiCoreSupport"="as:modconf('LinSM')[1]/LinSMGeneral/LinSMMultiCoreSupport"!]

[!/* determine the number of slave nodes support API and behavior */!]
[!VAR "numSlaveNodes"="num:i(count(LinSMConfigSet/*[1]/LinSMChannel/*[LinSMNodeType = 'SLAVE']))"!]


[!/* ----------[LinIf channel / ComM network handle mapping]-----------*/!]
[!/* NOTE: Using ComM IDs seems to be have been a design decision, originating
           somewhere / somwhen around ASCLINSM-200.
           Due to ASCLINSM-359, the APIs of LinIf will be called with the
           associated LinIfChannelIds.
*/!]

[!/* Used by the MACRO "LinSM_GetID" */!]
[!VAR "LinSM_HandleIDMap" = "''"!]
[!VAR "LinSMAssocLinIfHandles" = "''"!]

[!/* It's simpler establishing these now than splitting and joining later on */!]
[!VAR "LinSMIDComMMin" = "num:min(node:refs(as:modconf('LinSM')[1]/LinSMConfigSet/*[1]/LinSMChannel/*/LinSMComMNetworkHandleRef)/ComMChannelId)"!]
[!/* node:isconsecutive() doesn't care about the order but it's ok since the channels are ordered by ComM ID */!]
[!VAR "LinSMComMIDsConsecutive" = "node:isconsecutive(node:refs(as:modconf('LinSM')[1]/LinSMConfigSet/*[1]/LinSMChannel/*/LinSMComMNetworkHandleRef)/ComMChannelId, $LinSMIDComMMin)"!]

[!VAR "LinSMIDLinIfMin" = "'255'"!]
[!LOOP "as:modconf('LinSM')[1]/LinSMConfigSet/*[1]/LinSMChannel/*"!]
[!IF "LinSMNodeType = 'MASTER'"!]
[!VAR "LinSMIDLinIfMinTmp" = "num:min(node:refs(LinSMSchedule/*/LinSMScheduleIndexRef)/../../../*/LinIfChannelId)"!]
[!ELSE!]
[!VAR "assocComMChanID" = "LinSMComMNetworkHandleRef"!]
[!VAR "LinSMIDLinIfMinTmp" = "num:i(as:modconf('LinIf')[1]/LinIfGlobalConfig/*[1]/LinIfChannel/*/LinIfChannelId[../LinIfComMNetworkHandleRef = $assocComMChanID])"!]
[!ENDIF!]
[!IF "$LinSMIDLinIfMin > $LinSMIDLinIfMinTmp"!]
[!VAR "LinSMIDLinIfMin" = "$LinSMIDLinIfMinTmp"!]
[!ENDIF!]
[!ENDLOOP!]

[!/* For LinIf it has to be established by hand */!]
[!VAR "LinSMLinIfIDsConsecutive" = "true()"!]
[!LOOP "node:order(as:modconf('LinSM')[1]/LinSMConfigSet/*[1]/LinSMChannel/*,'as:ref(LinSMComMNetworkHandleRef)/ComMChannelId')"!]
[!IF "LinSMNodeType = 'MASTER'"!]
[!VAR "LinSMAssocLinIfHandles" = "concat($LinSMAssocLinIfHandles, num:i(as:ref(LinSMSchedule/*/LinSMScheduleIndexRef[1])/../../LinIfChannelId), ',')"!]
[!ELSE!]
[!VAR "assocComMChanID" = "LinSMComMNetworkHandleRef"!]
[!VAR "LinSMAssocLinIfHandles" = "concat($LinSMAssocLinIfHandles, num:i(as:modconf('LinIf')[1]/LinIfGlobalConfig/*[1]/LinIfChannel/*/LinIfChannelId[../LinIfComMNetworkHandleRef = $assocComMChanID]), ',')"!]
[!ENDIF!]
[!ENDLOOP!]
[!VAR "LinSMLoopVar" = "num:i(text:split($LinSMAssocLinIfHandles, ',')[1])"!]
[!LOOP "text:split($LinSMAssocLinIfHandles, ',')[position() > 1]"!]
  [!IF ". != ($LinSMLoopVar + 1)"!]
    [!VAR "LinSMLinIfIDsConsecutive" = "false()"!]
    [!BREAK!]
  [!ENDIF!]
  [!VAR "LinSMLoopVar" = "num:i(.)"!]
[!ENDLOOP!]
[!/* Establish whether the IDs can be mapped directly, with an offset or only by a LUT. (Values are ("DIRECT", "OFFSET", "LUT")) */!]
[!VAR "LinSMIDMapComMType" = "node:when($LinSMComMIDsConsecutive = 'true', node:when($LinSMIDComMMin = 0, 'DIRECT', 'OFFSET'), 'LUT')"!]
[!VAR "LinSMIDMapLinIfType" = "node:when($LinSMLinIfIDsConsecutive = 'true', node:when($LinSMIDLinIfMin = 0, 'DIRECT', 'OFFSET'), 'LUT')"!]

[!LOOP "node:order(as:modconf('LinSM')[1]/LinSMConfigSet/*[1]/LinSMChannel/*,'as:ref(LinSMComMNetworkHandleRef)/ComMChannelId')"!]
[!VAR "chanPos" = "node:pos(.)"!]
[!/* Waste time looking them up only if needed */!]
[!IF "LinSMNodeType = 'MASTER'"!]
[!VAR "assocLinIfChanID" = "node:when($LinSMIDMapLinIfType = 'LUT', num:i(as:ref(LinSMSchedule/*/LinSMScheduleIndexRef[1])/../../LinIfChannelId), $chanPos)"!]
[!ELSE!]
[!VAR "assocComMChanID" = "LinSMComMNetworkHandleRef"!]
[!VAR "assocLinIfChanID" = "node:when($LinSMIDMapLinIfType = 'LUT', num:i(as:modconf('LinIf')[1]/LinIfGlobalConfig/*[1]/LinIfChannel/*/LinIfChannelId[../LinIfComMNetworkHandleRef = $assocComMChanID]), $chanPos)"!]
[!ENDIF!]
[!VAR "assocComMChanID" = "node:when($LinSMIDMapComMType = 'LUT', num:i(as:ref(LinSMComMNetworkHandleRef)/ComMChannelId), $chanPos)"!]
[!VAR "LinSM_HandleIDMap" = "concat($LinSM_HandleIDMap, as:path(.), '|', $chanPos, '|', $assocLinIfChanID, '|', $assocComMChanID, ';')"!]
[!ENDLOOP!]

[!/* Returns the handle ID for the given module.
   * The "Dest" parameter is expected to be either "LinIf" or "ComM".
   * The "Path" parameter is the AS path of the node (LinSMChannel).
   * The returned value resides in the variable "LinSM_HandleID".
   * The entries have as a separator a semicolon (";"), while the values of an entry a pipe ("|").
   * Entry format: as:path|node position of SM channel|LinIf ID|ComM ID
   */!]
[/* No filtering since the macro will be called only if a LUT is used */]
[!MACRO "LinSM_GetID", "Path", "Dest"!][!NOCODE!]
[!VAR "LinSM_HandleID" = "num:i(text:split(text:grep(text:split($LinSM_HandleIDMap, ';'), concat('^', $Path, '.+'))[1], '|')[node:when($Dest = 'LinIf', 3 , 4)])"!]
[!ENDNOCODE!][!ENDMACRO!][!// LinSM_GetID

[!ENDIF!][!// IF "not(var:defined('LINSM_MACROS_M'))"
[!ENDNOCODE!][!//
