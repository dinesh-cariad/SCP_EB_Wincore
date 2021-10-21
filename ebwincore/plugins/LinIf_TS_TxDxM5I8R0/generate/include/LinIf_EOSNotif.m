[!/**
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
 */!][!//
[!NOCODE!]
[!IF "not(var:defined('LINIF_EOSNOTIF_M'))"!]
[!VAR "LINIF_EOSNOTIF_M"="'true'"!]
[!VAR "endOfSchedNotifSupported" = "node:value(as:modconf('LinIf')/LinIfGeneral/LinIfScheduleTableEndNotificationSupported)"!]
[!VAR "numEosCallouts" = "count(as:modconf('LinIf')/LinIfGeneral/LinIfScheduleTableEndNotificationCallout/*)"!]

[!/*
****************************************************************************************************
* MACRO to enumerate end-of-schedule notification functions
* Can't enumerate from the references because it might change.
* List contains tuples of referenced container and callout name, separated by a colon.
* Entries separated by a pipe.
****************************************************************************************************
*/!]
[!MACRO "EnumarateEOSCallouts"!][!NOCODE!]
[!VAR "eosCallouts" = "''"!]
[!/* Using FORs because loops would be 3 level nested from xgen and can't dereference anything */!]
[!FOR "n" = "0" TO "$numEosCallouts"!]
  [!SELECT "as:modconf('LinIf')/LinIfGeneral/LinIfScheduleTableEndNotificationCallout/*[position()-1 = $n]"!]
  [!VAR "eosCallouts" ="concat($eosCallouts, as:path(., true()), '|', node:value(./LinIfScheduleTableEndNotificationCalloutName), ';')"!]
  [!ENDSELECT!]
[!ENDFOR!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* MACRO to obtain index of end-of-schedule callout
* SchedTablePath param is an as:path() of a ScheduleTable
* Result resides in eosCalloutIdx. If no reference is set, the value 0 is returned for the
* reserved NULL_PTR.
****************************************************************************************************
*/!]
[!MACRO "GetEOSCalloutIdx", "SchedTablePath"!][!NOCODE!]
[!VAR "eosCalloutIdx" = "num:i(0)"!]
  [!IF "node:refvalid(as:ref($SchedTablePath)/LinIfScheduleTableEndNotificationRef)"!]
    [!IF "not(var:defined('eosCallouts'))"!][!CALL "EnumarateEOSCallouts"!][!ENDIF!]
    [!VAR "refVal" = "node:value(as:ref($SchedTablePath)/LinIfScheduleTableEndNotificationRef)"!]
    [!FOR "j" = "1" TO "$numEosCallouts"!]
      [!SELECT "text:split($eosCallouts, ';')[position() = $j]"!]
        [!IF "text:split(., '|')[1] = $refVal"!]
          [!VAR "eosCalloutIdx" = "num:i($j)"!]
          [!BREAK!]
        [!ENDIF!]
      [!ENDSELECT!]
    [!ENDFOR!]
  [!ENDIF!]
[!ENDNOCODE!][!ENDMACRO!]

[!ENDIF!][!// Include guard
[!ENDNOCODE!][!//
