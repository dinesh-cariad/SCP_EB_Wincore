/**
 * \file
 *
 * \brief AUTOSAR FiM
 *
 * This file contains the implementation of the AUTOSAR
 * module FiM.
 *
 * \version 2.5.3
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef FIM_MESSAGE_H
#define FIM_MESSAGE_H

/* This file contains the generated FiM module configuration. */

[!AUTOSPACING!]
[!//
[!IF "node:exists(FiMGeneral/FiMExtendedLevel) and (FiMGeneral/FiMExtendedSupport = 'true')"!]
  [!VAR "ExtendedLevel" = "FiMGeneral/FiMExtendedLevel"!]
[!ELSE!][!//
  [!VAR "ExtendedLevel" = "'FIM_NO_EXTENSION'"!]
[!ENDIF!]
[!/* only generate this file if extented FiM is enabled */!][!//
[!SKIPFILE "$ExtendedLevel = 'FIM_NO_EXTENSION'"!][!//
/*==================[includes]===============================================*/

/*==================[macros]=================================================*/

/*-----------------[FiMMessage short names]----------------------------------*/
/* !LINKSTO VCC-FIM-003:Req9v1,1 */
[!INDENT "0"!]
  [!IF "$ExtendedLevel != 'FIM_NO_EXTENSION'"!]
    [!LOOP "node:order(FiMConfigSet/*/FiMMessage/*, 'FiMMessageId')"!]

      #if (defined [!"name(.)"!])
      #error [!"name(.)"!] already defined
      #endif
      /** \brief Export symbolic name value */
      #define [!"name(.)"!]         [!"num:i(FiMMessageId)"!]
    [!ENDLOOP!]
  [!ENDIF!]
[!ENDINDENT!]

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef FIM_MESSAGE_H */
/*==================[end of file]============================================*/
