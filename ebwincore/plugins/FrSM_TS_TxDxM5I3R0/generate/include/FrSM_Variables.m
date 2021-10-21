[!/**
 * \file
 *
 * \brief AUTOSAR FrSM
 *
 * This file contains the implementation of the AUTOSAR
 * module FrSM.
 *
 * \version 5.3.18
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */!][!//

[!INCLUDE "CommonMacros.m"!][!//]

[!VAR "VarFrSMDevErrorDetect"="(FrSMGeneral/FrSMDevErrorDetect = 'true')"!][!/*
*/!][!VAR "VarFrSMVersionInfoApi"="(FrSMGeneral/FrSMVersionInfoApi = 'true')"!][!/*
*/!][!VAR "VarFrSMFrTrcvControlEnable"="(FrSMGeneral/FrSMFrTrcvControlEnable = 'true')"!][!/*
*/!][!VAR "VarFrSMSingleClstOptEnable"="(FrSMGeneral/FrSMSingleClstOptEnable = 'true')"!][!/*
*/!][!VAR "VarFrSMComMIndicationEnable"="(FrSMGeneral/FrSMComMIndicationEnable = 'true')"!][!/*
*/!][!VAR "VarFrSMReportToBswMEnable"="(FrSMGeneral/FrSMReportToBswMEnable = 'true')"!][!/*
*/!][!VAR "VarFrSMSetEcuPassiveEnable"="(FrSMGeneral/FrSMSetEcuPassiveEnable = 'true')"!][!/*
*/!][!VAR "VarFrSMFrNmStartupErrorEnable"="(FrSMGeneral/FrSMFrNmStartupErrorEnable = 'true')"!][!/*
*/!][!VAR "VarFrSMSyncLossIndicationEnable"="node:exists(FrSMGeneral/FrSMSyncLossErrorIndicationName)"!][!/*
*/!][!VAR "VarFrSMSyncLossIndicationHeaderEnable"="node:exists(FrSMGeneral/FrSMSyncLossErrorIndicationHeaderName)"!][!/*
*/!][!VAR "VarFrSMKeySlotOnlyModeEnable"="(FrSMGeneral/FrSMKeySlotOnlyModeEnable = 'true')"!][!/*
*/!][!VAR "VarFrSMMultiCoreSupportEnable"="(FrSMGeneral/FrSMMultiCoreSupportEnable = 'true')"!][!/*
*/!][!VAR "VarNumClst"="num:integer(count(FrSMConfig/*[1]/FrSMCluster/*))"!][!//
