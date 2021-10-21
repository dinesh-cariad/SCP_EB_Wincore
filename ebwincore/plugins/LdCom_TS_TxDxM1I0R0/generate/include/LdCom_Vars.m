[!/**
 * \file
 *
 * \brief AUTOSAR LdCom
 *
 * This file contains the implementation of the AUTOSAR
 * module LdCom.
 *
 * \version 1.0.26
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */!][!//
[!/*
*** multiple inclusion protection ***
*/!][!IF "not(var:defined('LDCOM_VARS_M'))"!]
[!VAR "LDCOM_VARS_M"="'true'"!][!/*

--------------------------------------------------------------------------
    Set the values of useful variables
--------------------------------------------------------------------------
*/!][!VAR "VarTpTxCount" = "num:i(count(LdComConfig/LdComIPdu/*[./LdComApiType='LDCOM_TP' and ./LdComIPduDirection='LDCOM_SEND']))"!][!/*
*/!][!VAR "VarTpRxCount" = "num:i(count(LdComConfig/LdComIPdu/*[./LdComApiType='LDCOM_TP' and ./LdComIPduDirection='LDCOM_RECEIVE']))"!][!/*
*/!][!VAR "VarIfTxCount" = "num:i(count(LdComConfig/LdComIPdu/*[./LdComApiType='LDCOM_IF' and ./LdComIPduDirection='LDCOM_SEND']))"!][!/*
*/!][!VAR "VarIfRxCount" = "num:i(count(LdComConfig/LdComIPdu/*[./LdComApiType='LDCOM_IF' and ./LdComIPduDirection='LDCOM_RECEIVE']))"!][!/*
*/!][!VAR "VarTrigTxCount" = "num:i(count(LdComConfig/LdComIPdu/*[./LdComApiType='LDCOM_IF' and ./LdComIPduDirection='LDCOM_SEND' and node:exists(./LdComTxTriggerTransmit)]))"!][!/*
*/!][!VAR "VarTxConfCount" = "num:i(count(LdComConfig/LdComIPdu/*[./LdComApiType='LDCOM_IF' and ./LdComIPduDirection='LDCOM_SEND' and node:exists(./LdComTxConfirmation)]))"!][!/*
*** End of file ***
*/!][!ENDIF!][!//
