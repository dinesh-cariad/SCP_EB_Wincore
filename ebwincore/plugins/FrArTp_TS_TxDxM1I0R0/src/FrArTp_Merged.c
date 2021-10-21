/**
 * \file
 *
 * \brief AUTOSAR FrArTp
 *
 * This file contains the implementation of the AUTOSAR
 * module FrArTp.
 *
 * \version 1.0.8
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* set macro that enables merged compilation macros */
#define TS_MERGED_COMPILE STD_ON

#include <FrArTp.c>
#include <FrArTp_Decode.c>
#include <FrArTp_Encode.c>
#include <FrArTp_CfgAccess.c>
#include <FrArTp_SM.c>
#include <FrArTp_SM_Rx.c>
#include <FrArTp_SM_Tx.c>
#include <FrArTp_SduMng.c>
