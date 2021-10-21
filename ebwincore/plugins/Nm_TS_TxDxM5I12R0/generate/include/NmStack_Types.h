#ifndef NM_STACK_TYPES_H
#define NM_STACK_TYPES_H

/**
 * \file
 *
 * \brief AUTOSAR Nm
 *
 * This file contains the implementation of the AUTOSAR
 * module Nm.
 *
 * \version 5.12.8
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*===============================[includes]=================================*/

#include <ComStack_Types.h>
/* !LINKSTO Nm124_Std_Types,1 */
#include <SchM_Nm.h>

/*===========================[macro definitions]============================*/

/*============================[type definitions]============================*/

/** \brief Bus-Sleep Mode */
#if (defined NM_MODE_BUS_SLEEP)
#error NM_MODE_BUS_SLEEP already defined
#endif
#define NM_MODE_BUS_SLEEP          0U

/** \brief Prepare-Bus Sleep Mode */
#if (defined NM_MODE_PREPARE_BUS_SLEEP)
#error NM_MODE_PREPARE_BUS_SLEEP already defined
#endif
#define NM_MODE_PREPARE_BUS_SLEEP  1U

/** \brief Synchronize Mode */
#if (defined NM_MODE_SYNCHRONIZE)
#error NM_MODE_SYNCHRONIZE already defined
#endif
#define NM_MODE_SYNCHRONIZE        2U

/** \brief Network Mode */
#if (defined NM_MODE_NETWORK)
#error NM_MODE_NETWORK already defined
#endif
#define NM_MODE_NETWORK            3U

/** \brief Operational modes of the network management */
#ifndef RTE_TYPE_Nm_ModeType
#define RTE_TYPE_Nm_ModeType
typedef uint8 Nm_ModeType;
#endif


#if (defined NM_STATE_UNINIT)
#error NM_STATE_UNINIT already defined
#endif
#define NM_STATE_UNINIT            0U

#if (defined NM_STATE_BUS_SLEEP)
#error NM_STATE_BUS_SLEEP already defined
#endif
#define NM_STATE_BUS_SLEEP         1U

#if (defined NM_STATE_PREPARE_BUS_SLEEP)
#error NM_STATE_PREPARE_BUS_SLEEP already defined
#endif
#define NM_STATE_PREPARE_BUS_SLEEP 2U

#if (defined NM_STATE_READY_SLEEP)
#error NM_STATE_READY_SLEEP already defined
#endif
#define NM_STATE_READY_SLEEP       3U

#if (defined NM_STATE_NORMAL_OPERATION)
#error NM_STATE_NORMAL_OPERATION already defined
#endif
#define NM_STATE_NORMAL_OPERATION  4U

#if (defined NM_STATE_REPEAT_MESSAGE)
#error NM_STATE_REPEAT_MESSAGE already defined
#endif
#define NM_STATE_REPEAT_MESSAGE    5U

#if (defined NM_STATE_SYNCHRONIZE)
#error NM_STATE_SYNCHRONIZE already defined
#endif
#define NM_STATE_SYNCHRONIZE       6U

#if (defined NM_STATE_OFFLINE)
#error NM_STATE_OFFLINE already defined
#endif
#define NM_STATE_OFFLINE           7U

/** \brief States of the network management state machine */
#ifndef RTE_TYPE_Nm_StateType
#define RTE_TYPE_Nm_StateType
typedef uint8 Nm_StateType;
#endif


/** \brief CAN NM type */
#if (defined NM_BUSNM_CANNM)
#error NM_BUSNM_CANNM already defined
#endif
#define NM_BUSNM_CANNM             0U

/** \brief FR NM type */
#if (defined NM_BUSNM_FRNM)
#error NM_BUSNM_FRNM already defined
#endif
#define NM_BUSNM_FRNM              1U

/** \brief LIN NM type */
#if (defined NM_BUSNM_LINNM)
#error NM_BUSNM_LINNM already defined
#endif
#define NM_BUSNM_LINNM             2U

/** \brief UDP NM type */
#if (defined NM_BUSNM_UDPNM)
#error NM_BUSNM_UDPNM already defined
#endif
#define NM_BUSNM_UDPNM             3U

/** \brief Generic NM type */
#if (defined NM_BUSNM_GENERICNM)
#error NM_BUSNM_GENERICNM already defined
#endif
#define NM_BUSNM_GENERICNM         4U

/** \brief NM type undefined */
#if (defined NM_BUSNM_UNDEF)
#error NM_BUSNM_UNDEF already defined
#endif
#define NM_BUSNM_UNDEF             0xFFU

/** \brief BusNm types */
#ifndef RTE_TYPE_Nm_BusNmType
#define RTE_TYPE_Nm_BusNmType
typedef uint8 Nm_BusNmType;
#endif


/*=====================[external function declarations]=====================*/

/*=====================[internal function declarations]=====================*/

/*=====================[external constants declarations]====================*/

/*=====================[internal constants declarations]====================*/

/*=======================[external data declarations]=======================*/

#endif /* ifndef NM_STACK_TYPES_H */
