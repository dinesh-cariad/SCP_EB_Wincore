#ifndef LIN_GENERALTYPES_CFG_H
#define LIN_GENERALTYPES_CFG_H
[!AUTOSPACING!][!//
/**
 * \file
 *
 * \brief AUTOSAR Base
 *
 * This file contains the implementation of the AUTOSAR
 * module Base.
 *
 * \version 5.0.31
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]=============================================*/

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/* !LINKSTO Base.ASR403.LIN230,1 */
/** \brief This type is used to specify whether the frame processor is
 * required to transmit the response part of the LIN frame. */
[!IF "node:exists(as:modconf('LinIf')[1]/LinIfGeneral/LinIfLinDriverAPI) and
      (as:modconf('LinIf')[1]/LinIfGeneral/LinIfLinDriverAPI = 'REV44')"!][!//
typedef enum
{
  LIN_FRAMERESPONSE_TX,    /**< Response is generated from this (master) node */
  LIN_FRAMERESPONSE_RX,    /**< Response is generated from a remote slave node */
  LIN_FRAMERESPONSE_IGNORE /**< Response is generated from one slave to another
                            * slave, for the master the response will be anonymous,
                            * it does not have to receive the response. */
} Lin_FrameResponseType;
[!ELSE!][!//
typedef enum
{
  LIN_MASTER_RESPONSE, /**< Response is generated from this (master) node */
  LIN_SLAVE_RESPONSE,  /**< Response is generated from a remote slave node */
  LIN_SLAVE_TO_SLAVE   /**< Response is generated from one slave to another
                        * slave, for the master the response will be anonymous,
                        * it does not have to receive the response. */
} Lin_FrameResponseType;
[!ENDIF!]

#endif /* ifndef LIN_GENERALTYPES_CFG_H */
/*==================[end of file]============================================*/
