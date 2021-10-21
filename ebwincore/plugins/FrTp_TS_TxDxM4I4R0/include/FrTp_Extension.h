/**
 * \file
 *
 * \brief AUTOSAR FrTp
 *
 * This file contains the implementation of the AUTOSAR
 * module FrTp.
 *
 * \version 4.4.26
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef FRTP_EXTENSION_H
#define FRTP_EXTENSION_H

/*=====================[Include Section]=====================================*/

#include <ComStack_Types.h> /* basic commstack types from AUTOSAR */
#include <FrTp_Cfg.h>       /* static configuration of FrTp */

/*=====================[Global Macros]=======================================*/

/*
 * EB extensions of Det error values for extended error checking
 */

/*=====================[Global Macro Checks]=================================*/

/*
 * Consistency checking of the used static configuration parameter
 */
#ifdef FRTP_RELOCATABLE_CFG_ENABLE
#if   (FRTP_RELOCATABLE_CFG_ENABLE != STD_ON) \
    &&(FRTP_RELOCATABLE_CFG_ENABLE != STD_OFF)
#error "FrTp config error: Wrong value for FRTP_RELOCATABLE_CFG_ENABLE"
#endif
#else
#error "FrTp config error: Missing parameter FRTP_RELOCATABLE_CFG_ENABLE"
#endif

#ifdef FRTP_FULLDUPLEX_ENABLE
#if   (FRTP_FULLDUPLEX_ENABLE != STD_ON) \
    &&(FRTP_FULLDUPLEX_ENABLE != STD_OFF)
#error "FrTp config error: Wrong value for FRTP_FULLDUPLEX_ENABLE"
#endif
#else
#error "FrTp config error: Missing parameter FRTP_FULLDUPLEX_ENABLE"
#endif

#ifdef FRTP_CON_NUM
#if   (FRTP_CON_NUM >  254U) \
    ||(FRTP_CON_NUM <  1U)
#error "FrTp: FRTP_CON_NUM not in range"
#endif
#else
#error "FrTp config error: Missing parameter FRTP_CON_NUM"
#endif

#ifdef FRTP_TXPDU_NUM
#if   (FRTP_TXPDU_NUM >  254U)
#error "FrTp: FRTP_TXPDU_NUM out of range (0 to 254)"
#endif
#else
#error "FrTp config error: Missing parameter FRTP_TXPDU_NUM"
#endif

#endif /* FRTP_EXTENSION_H */

