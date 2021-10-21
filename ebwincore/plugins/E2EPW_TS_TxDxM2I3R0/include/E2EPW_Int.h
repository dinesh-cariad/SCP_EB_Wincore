/**
 * \file
 *
 * \brief AUTOSAR E2EPW
 *
 * This file contains the implementation of the AUTOSAR
 * module E2EPW.
 *
 * \version 2.3.20
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 20.10 (advisory)
 * The # and ## preprocessor operators should not be used.
 *
 * Reason:
 * ## operator is used for code generation via pre-processor.
 *
 */

#if (!defined E2EPW_INT_H)
#define E2EPW_INT_H

/*==================[inclusions]=============================================*/

#include <stddef.h>                 /* offsetof() */
#include <Std_Types.h>              /* AUTOSAR Std types */
#include <E2EPW.h>                  /* E2EPW module */
#include <E2EPW_Int_Cfg.h>          /* E2EPW PrecCompile parameters */
#include <E2EPW_Int_Stc.h>          /* E2EPW static internals */

/*==================[macros]=================================================*/

#if (!defined E2EPW_ENABLE_TX_SIGNAL_GROUP_ARRAY_ACCESS)
#error E2EPW_ENABLE_TX_SIGNAL_GROUP_ARRAY_ACCESS not defined
#endif

#if (defined E2EPW_EB_PARAM_UNUSED)
#error E2EPW_EB_PARAM_UNUSED already defined
#endif
#if (defined EB_STATIC_CHECK)
/** \brief This macro can be used to avoid compiler warnings
*/
#define E2EPW_EB_PARAM_UNUSED(x)
#else
/** \brief This macro can be used to avoid compiler warnings */
#define E2EPW_EB_PARAM_UNUSED(x) ((void) (x))
#endif

/* Deviation MISRAC2012-1 <START> */

/*------------------[Module internal static macros]-------------------------*/

#if (defined E2EPW_DEFVAR1) /* To prevent double declaration */
#error E2EPW_DEFVAR1 already defined
#endif /* if (defined E2EPW_DEFVAR1) */

#if (defined E2EPW_DEFVAR2) /* To prevent double declaration */
#error E2EPW_DEFVAR2 already defined
#endif /* if (defined E2EPW_DEFVAR2) */

#if (defined E2EPW_DEFVAR_CONST1) /* To prevent double declaration */
#error E2EPW_DEFVAR_CONST1 already defined
#endif /* if (defined E2EPW_DEFVAR_CONST1) */

#if (defined E2EPW_DEFVAR_CONST2) /* To prevent double declaration */
#error E2EPW_DEFVAR_CONST2 already defined
#endif /* if (defined E2EPW_DEFVAR_CONST2) */

#if (defined E2EPW_DEFVAR_CONST3) /* To prevent double declaration */
#error E2EPW_DEFVAR_CONST3 already defined
#endif /* if (defined E2EPW_DEFVAR_CONST3) */

#if (defined E2EPW_DEFVAR_CONST4) /* To prevent double declaration */
#error E2EPW_DEFVAR_CONST4 already defined
#endif /* if (defined E2EPW_DEFVAR_CONST4) */

#if (defined E2EPW_DEFFUNC1) /* To prevent double declaration */
#error E2EPW_DEFFUNC1 already defined
#endif /* if (defined E2EPW_DEFFUNC1) */

#if (defined E2EPW_DEFFUNC2) /* To prevent double declaration */
#error E2EPW_DEFFUNC2 already defined
#endif /* if (defined E2EPW_DEFFUNC2) */

#if (defined E2EPW_DEFFUNC3) /* To prevent double declaration */
#error E2EPW_DEFFUNC3 already defined
#endif /* if (defined E2EPW_DEFFUNC3) */

#if (defined E2EPW_DEFFUNC4) /* To prevent double declaration */
#error E2EPW_DEFFUNC4 already defined
#endif /* if (defined E2EPW_DEFFUNC4) */

#if (defined E2EPW_DEFFUNC5) /* To prevent double declaration */
#error E2EPW_DEFFUNC5 already defined
#endif /* if (defined E2EPW_DEFFUNC5) */

#if (defined E2EPW_DEFFUNC6) /* To prevent double declaration */
#error E2EPW_DEFFUNC6 already defined
#endif /* if (defined E2EPW_DEFFUNC6) */

/** \brief Define E2EPW_DEFVAR1
 * Macro for defining global variables with internal linkage,
 * used in the corresponding E2EPW redundant channel 1 function defined via E2EPW_DEFFUNC()
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] wrtype_rw Concatenation of wrapper type (single or redundant channel) and read/write
 *            oper. Shall be one of E2EPW_SC_RD, E2EPW_SC_WR, E2EPW_RC_RD, E2EPW_RC_WR
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *                    and Ecu communication type (Inter or intra Ecu communication)
 *                    Can be on of E2EPW_CX_IR, E2EPW_CX_IA
 */
#define E2EPW_DEFVAR1(port_dataelement,applDataType,e2eprotsn,wrtype_rw,detype_comtype) \
   E2EPW_DEFVAR1_ ## wrtype_rw(port_dataelement,applDataType,e2eprotsn,E2EPW_CFG_PF_ ## e2eprotsn)

/** \brief Define E2EPW_DEFVAR2
 * Macro for defining global variables with internal linkage,
 * used in the corresponding E2EPW redundant channel 2 function defined via E2EPW_DEFFUNC()
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] wrtype_rw Concatenation of wrapper type (single or redundant channel) and read/write
 *            oper. Shall be one of E2EPW_SC_RD, E2EPW_SC_WR, E2EPW_RC_RD, E2EPW_RC_WR
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *                    and Ecu communication type (Inter or intra Ecu communication)
 *                    Can be on of E2EPW_CX_IR, E2EPW_CX_IA
 */
#define E2EPW_DEFVAR2(port_dataelement,applDataType,e2eprotsn,wrtype_rw,detype_comtype) \
   E2EPW_DEFVAR2_ ## wrtype_rw(port_dataelement,applDataType,e2eprotsn,E2EPW_CFG_PF_ ## e2eprotsn)

/** \brief Define E2EPW_DEFVAR_CONST1
 * Macro for defining global const variables with internal linkage,
 * used in the corresponding E2EPW function defined via E2EPW_DEFFUNC()
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] wrtype_rw Concatenation of wrapper type (single or redundant channel) and read/write
 *            oper. Shall be one of E2EPW_SC_RD, E2EPW_SC_WR, E2EPW_RC_RD, E2EPW_RC_WR
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *                    and Ecu communication type (Inter or intra Ecu communication)
 *                    Can be on of E2EPW_CX_IR, E2EPW_CX_IA
 */
#define E2EPW_DEFVAR_CONST1(port_dataelement,applDataType,e2eprotsn,wrtype_rw,detype_comtype)\
   E2EPW_DEFVAR_CONST1_ ## wrtype_rw(port_dataelement,applDataType,e2eprotsn,                \
                                     E2EPW_CFG_PF_ ## e2eprotsn,detype_comtype)

/** \brief Define E2EPW_DEFVAR_CONST2
 * Macro for defining global const variables with internal linkage,
 * used in the corresponding E2EPW function defined via E2EPW_DEFFUNC()
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] wrtype_rw Concatenation of wrapper type (single or redundant channel) and read/write
 *            oper. Shall be one of E2EPW_SC_RD, E2EPW_SC_WR, E2EPW_RC_RD, E2EPW_RC_WR
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *                    and Ecu communication type (Inter or intra Ecu communication)
 *                    Can be on of E2EPW_CX_IR, E2EPW_CX_IA
 */
#define E2EPW_DEFVAR_CONST2(port_dataelement,applDataType,e2eprotsn,wrtype_rw,detype_comtype)\
   E2EPW_DEFVAR_CONST2_ ## wrtype_rw(port_dataelement,applDataType,e2eprotsn,                \
                                     E2EPW_CFG_PF_ ## e2eprotsn,detype_comtype)

/** \brief Define E2EPW_DEFVAR_CONST3
 * Macro for defining global const variables with internal linkage,
 * used in the corresponding E2EPW function defined via E2EPW_DEFFUNC()
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] wrtype_rw Concatenation of wrapper type (single or redundant channel) and read/write
 *            oper. Shall be one of E2EPW_SC_RD, E2EPW_SC_WR, E2EPW_RC_RD, E2EPW_RC_WR
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *                    and Ecu communication type (Inter or intra Ecu communication)
 *                    Can be on of E2EPW_CX_IR, E2EPW_CX_IA
 */
#define E2EPW_DEFVAR_CONST3(port_dataelement,applDataType,e2eprotsn,wrtype_rw,detype_comtype)\
   E2EPW_DEFVAR_CONST3_ ## wrtype_rw(port_dataelement,applDataType,e2eprotsn,detype_comtype, \
                                     E2EPW_CFG_PF_ ## e2eprotsn)

/** \brief Define E2EPW_DEFVAR_CONST4
 * Macro for defining global const variables with internal linkage,
 * used in the corresponding E2EPW function defined via E2EPW_DEFFUNC()
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] wrtype_rw Concatenation of wrapper type (single or redundant channel) and read/write
 *            oper. Shall be one of E2EPW_SC_RD, E2EPW_SC_WR, E2EPW_RC_RD, E2EPW_RC_WR
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *                    and Ecu communication type (Inter or intra Ecu communication)
 *                    Can be on of E2EPW_CX_IR, E2EPW_CX_IA
 */
#define E2EPW_DEFVAR_CONST4(port_dataelement,applDataType,e2eprotsn,wrtype_rw,detype_comtype)\
   E2EPW_DEFVAR_CONST4_ ## wrtype_rw(port_dataelement,applDataType,e2eprotsn,detype_comtype, \
                                     E2EPW_CFG_PF_ ## e2eprotsn)

/** \brief Define E2EPW_DEFFUNC1
 * Macro generates the wrapper function based on the information given by the argument.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] wrtype_rw Concatenation of wrapper type (single or redundant channel) and read/write
 *            oper. Shall be one of E2EPW_SC_RD, E2EPW_SC_WR, E2EPW_RC_RD, E2EPW_RC_WR
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Can be on of E2EPW_CX_IR, E2EPW_CX_IA
 */
#define E2EPW_DEFFUNC1(port_dataelement,applDataType,e2eprotsn,wrtype_rw,detype_comtype) \
   E2EPW_DEFFUNC1_ ## wrtype_rw(port_dataelement,applDataType,e2eprotsn,                 \
                   E2EPW_CFG_PF_ ## e2eprotsn,detype_comtype)

/** \brief Define E2EPW_DEFFUNC2
 * Macro generates the wrapper function based on the information given by the argument.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] wrtype_rw Concatenation of wrapper type (single or redundant channel) and read/write
 *            oper. Shall be one of E2EPW_SC_RD, E2EPW_SC_WR, E2EPW_RC_RD, E2EPW_RC_WR
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Can be on of E2EPW_CX_IR, E2EPW_CX_IA
 */
#define E2EPW_DEFFUNC2(port_dataelement,applDataType,e2eprotsn,wrtype_rw,detype_comtype) \
   E2EPW_DEFFUNC2_ ## wrtype_rw(port_dataelement,applDataType,e2eprotsn,                 \
                   E2EPW_CFG_PF_ ## e2eprotsn,detype_comtype)

/** \brief Define E2EPW_DEFFUNC3
 * Macro generates the wrapper function based on the information given by the argument.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] wrtype_rw Concatenation of wrapper type (single or redundant channel) and read/write
 *            oper. Shall be one of E2EPW_SC_RD, E2EPW_SC_WR, E2EPW_RC_RD, E2EPW_RC_WR
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Can be on of E2EPW_CX_IR, E2EPW_CX_IA
 */
#define E2EPW_DEFFUNC3(port_dataelement,applDataType,e2eprotsn,wrtype_rw,detype_comtype) \
   E2EPW_DEFFUNC3_ ## wrtype_rw(port_dataelement,applDataType,e2eprotsn,                 \
                   E2EPW_CFG_PF_ ## e2eprotsn,detype_comtype)

/** \brief Define E2EPW_DEFFUNC4
 * Macro generates the wrapper function based on the information given by the argument.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] wrtype_rw Concatenation of wrapper type (single or redundant channel) and read/write
 *            oper. Shall be one of E2EPW_SC_RD, E2EPW_SC_WR, E2EPW_RC_RD, E2EPW_RC_WR
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Can be on of E2EPW_CX_IR, E2EPW_CX_IA
 */
#define E2EPW_DEFFUNC4(port_dataelement,applDataType,e2eprotsn,wrtype_rw,detype_comtype) \
   E2EPW_DEFFUNC4_ ## wrtype_rw(port_dataelement,applDataType,e2eprotsn,                 \
                   E2EPW_CFG_PF_ ## e2eprotsn,detype_comtype)

/** \brief Define E2EPW_DEFFUNC5
 * Macro generates the initialization function for single channel wrapper or redundant
 * channel wrapper 1 based on the information given by the arguments.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] wrtype_rw Concatenation of wrapper type (single or redundant channel) and read/write
 *            oper. Shall be one of E2EPW_SC_RD, E2EPW_SC_WR, E2EPW_RC_RD, E2EPW_RC_WR
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Can be on of E2EPW_CX_IR, E2EPW_CX_IA
 */
#define E2EPW_DEFFUNC5(port_dataelement,applDataType,e2eprotsn,wrtype_rw,detype_comtype) \
   E2EPW_DEFFUNC5_ ## wrtype_rw(port_dataelement,applDataType,e2eprotsn,                 \
                   E2EPW_CFG_PF_ ## e2eprotsn,detype_comtype)

/** \brief Define E2EPW_DEFFUNC6
 * Macro generates the initialization function for redundant wrapper channel 2
 * based on the information given by the arguments.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] wrtype_rw Concatenation of wrapper type (single or redundant channel) and read/write
 *            oper. Shall be one of E2EPW_SC_RD, E2EPW_SC_WR, E2EPW_RC_RD, E2EPW_RC_WR
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Can be on of E2EPW_CX_IR, E2EPW_CX_IA
 */
#define E2EPW_DEFFUNC6(port_dataelement,applDataType,e2eprotsn,wrtype_rw,detype_comtype) \
   E2EPW_DEFFUNC6_ ## wrtype_rw(port_dataelement,applDataType,e2eprotsn,                 \
                   E2EPW_CFG_PF_ ## e2eprotsn,detype_comtype)

/* ======================== E2EPW profile specific macros code generation =============== */

#if (defined E2EPW_PROFILE_DAI1) /* Ensure that string can be used for macro concatenation */
#error E2EPW_PROFILE_DAI1 already defined
#endif /* if (defined E2EPW_PROFILE_DAI1) */

/** \brief Infix DAI1 shall be used for E2E state variable definitions
 *  (i.e. E2E_P ## DAI1 ## SenderStateType, E2E_P ## DAI1 ## ReceiverStateType) */
#define E2EPW_PROFILE_DAI1 DAI1

#if (defined E2EPW_PROFILE_DAI2) /* Ensure that string can be used for macro concatenation */
#error E2EPW_PROFILE_DAI2 already defined
#endif /* if (defined E2EPW_PROFILE_DAI2) */

/** \brief Infix DAI2 shall be used for E2E state variable definitions
 *  (i.e. E2E_P ## DAI2 ## SenderStateType, E2E_P ## DAI2 ## ReceiverStateType) */
#define E2EPW_PROFILE_DAI2 DAI2

#if (defined E2EPW_PROFILE_01) /* Ensure that string can be used for macro concatenation */
#error E2EPW_PROFILE_01 already defined
#endif /* if (defined E2EPW_PROFILE_01) */

/** \brief Infix 01 shall be used for E2E state variable definitions
 *  (i.e. E2E_P ## 01 ## SenderStateType, E2E_P ## 01 ## ReceiverStateType) */
#define E2EPW_PROFILE_01 01

#if (defined E2EPW_PROFILE_02) /* Ensure that string can be used for macro concatenation */
#error E2EPW_PROFILE_02 already defined
#endif /* if (defined E2EPW_PROFILE_02) */

/** \brief Infix 02 shall be used for E2E state variable definitions
 *  (i.e. E2E_P ## 02 ## SenderStateType, E2E_P ## 02 ## ReceiverStateType) */
#define E2EPW_PROFILE_02 02

#if (defined E2EPW_PROFILE_VCC) /* Ensure that string can be used for macro concatenation */
#error E2EPW_PROFILE_VCC already defined
#endif /* if (defined E2EPW_PROFILE_VCC) */

/** \brief Infix 01 shall be used for E2E state variable definitions
 *  (i.e. E2E_P ## 01 ## SenderStateType, E2E_P ## 01 ## ReceiverStateType) */
#define E2EPW_PROFILE_VCC 01

#if (defined E2EPW_GET_CFG_P02) /* To prevent double declaration */
#error E2EPW_GET_CFG_P02 already defined
#endif /* if (defined E2EPW_GET_CFG_P02) */

/** \brief Get configuration data for E2E profile 02
 * This macros expands to the configuration data required to initialize the configuration data
 * of the struct type E2E_P02ConfigType.
 * Note: Member offset shall be set to 0 for P02 (see Limitations in 4.2.1 SWS
 *
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Can be on of E2EPW_CX_IR, E2EPW_CX_IA
 * \param[in] applDataType Data type name of the application data element
 */
#define E2EPW_GET_CFG_P02(e2eprotsn,detype_comtype,applDataType)                       \
   (uint16)detype_comtype ## _ISCOMPLEXANDINTRA((((uint16)sizeof(applDataType))*8U),   \
                                                E2EPW_CFG_DL_ ## e2eprotsn),           \
   E2EPW_CFG_DI_ ## e2eprotsn,                                                         \
   E2EPW_CFG_CI_ ## e2eprotsn,                                                         \
   E2EPW_CFG_LD_ ## e2eprotsn,                                                         \
   E2EPW_CFG_SC_ ## e2eprotsn,                                                         \
   0U

#if (defined E2EPW_GET_CFG_P01) /* To prevent double declaration */
#error E2EPW_GET_CFG_P01 already defined
#endif /* if (defined E2EPW_GET_CFG_P01) */

/** \brief Get configuration data for E2E profile 01
 * This macros expands to the configuration data required to initialize the configuration data
 * of the struct type E2E_P01ConfigType.
 *
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Can be on of E2EPW_CX_IR, E2EPW_CX_IA
 * \param[in] applDataType Data type name of the application data element
 */
#define E2EPW_GET_CFG_P01(e2eprotsn,detype_comtype,applDataType)                       \
       E2EPW_GET_CFG_P01_ ## detype_comtype(                                           \
           e2eprotsn,                                                                  \
           applDataType,                                                               \
           E2EPW_CFG_CT_ ## e2eprotsn,                                                 \
           E2EPW_CFG_CC_ ## e2eprotsn,                                                 \
           E2EPW_CFG_DN_ ## e2eprotsn,                                                 \
           E2EPW_CFG_DI_ ## e2eprotsn,                                                 \
           E2EPW_CFG_DM_ ## e2eprotsn,                                                 \
           E2EPW_CFG_CI_ ## e2eprotsn,                                                 \
           E2EPW_CFG_LD_ ## e2eprotsn,                                                 \
           E2EPW_CFG_SC_ ## e2eprotsn                                                  \
           )

#if (defined E2EPW_GET_CFG_P01_E2EPW_CX_IA) /* To prevent double declaration */
#error E2EPW_GET_CFG_P01_E2EPW_CX_IA already defined
#endif /* if (defined E2EPW_GET_CFG_P01_E2EPW_CX_IA) */

/** \brief Get configuration data for E2E profile 01 in case of pure intra-Ecu communication
 * This macros expands to the configuration data required to initialize the configuration data
 * of the struct type E2E_P01ConfigType.
 *
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2ecfg_ct macro name that defines the counter member of the Data Element
 * \param[in] e2ecfg_cc macro name that defines the crc member of the Data Element
 * \param[in] e2ecfg_dn macro name that defines the data id nibble member of the Data Element
 * \param[in] e2ecfg_di macro name that defines the data id value for the e2e profile
 * \param[in] e2ecfg_dm macro name that defines the data id mode for the e2e profile
 * \param[in] e2ecfg_ci macro name that defines the MaxDeltaCounterInit value for the e2e profile
 * \param[in] e2ecfg_ld macro name that defines the MaxNoNewOrRepeatedData value for the e2e profile
 * \param[in] e2ecfg_sc macro name that defines the SyncCounterInit value for the e2e profile
 */
#define E2EPW_GET_CFG_P01_E2EPW_CX_IA(e2eprotsn,applDataType,e2ecfg_ct,e2ecfg_cc,e2ecfg_dn,     \
                                      e2ecfg_di,e2ecfg_dm,e2ecfg_ci,e2ecfg_ld,e2ecfg_sc)        \
       E2EPW_GET_CFG_P01_E2EPW_CX_IA_(applDataType,e2ecfg_dm,                                   \
                                       (e2ecfg_di),(e2ecfg_ci),(e2ecfg_ld),(e2ecfg_sc),         \
                                       (uint16)(((uint16)offsetof(applDataType,e2ecfg_ct))*8U), \
                                       (uint16)(((uint16)offsetof(applDataType,e2ecfg_cc))*8U), \
                                       (uint16)(((uint16)offsetof(applDataType,e2ecfg_dn))*8U), \
                                       (uint16)(((uint16)sizeof(applDataType))*8U))

#if (defined E2EPW_GET_CFG_P01_E2EPW_CX_IA_) /* To prevent double declaration */
#error E2EPW_GET_CFG_P01_E2EPW_CX_IA_ already defined
#endif /* if (defined E2EPW_GET_CFG_P01_E2EPW_CX_IA_) */

/** \brief Get configuration data for E2E profile 01 in case of pure intra-Ecu communication
 * This macros expands to the configuration data required to initialize the configuration data
 * of the struct type E2E_P01ConfigType.
 *
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2ecfg_dm macro name that defines the data id mode for the e2e profile
 * \param[in] e2ecfg_di macro name that defines the data id value for the e2e profile
 * \param[in] e2ecfg_ci macro name that defines the MaxDeltaCounterInit value for the e2e profile
 * \param[in] e2ecfg_ld macro name that defines the MaxNoNewOrRepeatedData value for the e2e profile
 * \param[in] e2ecfg_sc macro name that defines the SyncCounterInit value for the e2e profile
 * \param[in] e2ecfg_ct_offset_val bit offset of the counter Data Element member
 * \param[in] e2ecfg_cc_offset_val bit offset of the crc Data Element member
 * \param[in] e2ecfg_dn_offset_val bit offset of the dataIdNibble Data Element member
 * \param[in] e2ecfg_dl_val platform-specific bit length of DataElement type
 */
#define E2EPW_GET_CFG_P01_E2EPW_CX_IA_(applDataType,e2ecfg_dm,                          \
                                             e2ecfg_di,e2ecfg_ci,e2ecfg_ld,e2ecfg_sc,   \
                                             e2ecfg_ct_offset_val,                      \
                                             e2ecfg_cc_offset_val,                      \
                                             e2ecfg_dn_offset_val,                      \
                                             e2ecfg_dl_val)                             \
       E2EPW_GET_CFG_P01_ ## e2ecfg_dm(applDataType,(e2ecfg_dm),                        \
                                       (e2ecfg_di),(e2ecfg_ci),(e2ecfg_ld),(e2ecfg_sc), \
                                       (e2ecfg_ct_offset_val),                          \
                                       (e2ecfg_cc_offset_val),                          \
                                       (e2ecfg_dn_offset_val),                          \
                                       (e2ecfg_dl_val))

#if (defined E2EPW_GET_CFG_P01_E2E_P01_DATAID_LOW) /* To prevent double declaration */
#error E2EPW_GET_CFG_P01_E2E_P01_DATAID_LOW already defined
#endif /* if (defined E2EPW_GET_CFG_P01_E2E_P01_DATAID_LOW) */

/** \brief Get configuration data for E2E profile 01 with dataIdMode Low
 * in case of pure intra-Ecu communication.
 * This macros expands to the configuration data required to initialize the configuration data
 * of the struct type E2E_P01ConfigType.
 *
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2ecfg_dm macro name that defines the data id mode for the e2e profile
 * \param[in] e2ecfg_di macro name that defines the data id value for the e2e profile
 * \param[in] e2ecfg_ci macro name that defines the MaxDeltaCounterInit value for the e2e profile
 * \param[in] e2ecfg_ld macro name that defines the MaxNoNewOrRepeatedData value for the e2e profile
 * \param[in] e2ecfg_sc macro name that defines the SyncCounterInit value for the e2e profile
 * \param[in] e2ecfg_ct_offset_val bit offset of the counter Data Element member
 * \param[in] e2ecfg_cc_offset_val bit offset of the crc Data Element member
 * \param[in] e2ecfg_dn_offset_val bit offset of the dataIdNibble Data Element member
 * \param[in] e2ecfg_dl_val platform-specific bit length of DataElement type
 */
#define E2EPW_GET_CFG_P01_E2E_P01_DATAID_LOW(applDataType,e2ecfg_dm,                    \
                                             e2ecfg_di,e2ecfg_ci,e2ecfg_ld,e2ecfg_sc,   \
                                             e2ecfg_ct_offset_val,                      \
                                             e2ecfg_cc_offset_val,                      \
                                             e2ecfg_dn_offset_val,                      \
                                             e2ecfg_dl_val)                             \
   (e2ecfg_ct_offset_val),                                                              \
   (e2ecfg_cc_offset_val),                                                              \
   (e2ecfg_di),                                                                         \
   0U,                                                                                  \
   (e2ecfg_dm),                                                                         \
   (e2ecfg_dl_val),                                                                     \
   (e2ecfg_ci),                                                                         \
   (e2ecfg_ld),                                                                         \
   (e2ecfg_sc)

#if (defined E2EPW_GET_CFG_P01_E2E_P01_DATAID_ALT) /* To prevent double declaration */
#error E2EPW_GET_CFG_P01_E2E_P01_DATAID_ALT already defined
#endif /* if (defined E2EPW_GET_CFG_P01_E2E_P01_DATAID_ALT) */

/** \brief Get configuration data for E2E profile 01 with dataIdMode Alt
 * in case of pure intra-Ecu communication.
 * This macros expands to the configuration data required to initialize the configuration data
 * of the struct type E2E_P01ConfigType.
 *
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2ecfg_dm macro name that defines the data id mode for the e2e profile
 * \param[in] e2ecfg_di macro name that defines the data id value for the e2e profile
 * \param[in] e2ecfg_ci macro name that defines the MaxDeltaCounterInit value for the e2e profile
 * \param[in] e2ecfg_ld macro name that defines the MaxNoNewOrRepeatedData value for the e2e profile
 * \param[in] e2ecfg_sc macro name that defines the SyncCounterInit value for the e2e profile
 * \param[in] e2ecfg_ct_offset_val bit offset of the counter Data Element member
 * \param[in] e2ecfg_cc_offset_val bit offset of the crc Data Element member
 * \param[in] e2ecfg_dn_offset_val bit offset of the dataIdNibble Data Element member
 * \param[in] e2ecfg_dl_val platform-specific bit length of DataElement type
 */
#define E2EPW_GET_CFG_P01_E2E_P01_DATAID_ALT(applDataType,e2ecfg_dm,                    \
                                             e2ecfg_di,e2ecfg_ci,e2ecfg_ld,e2ecfg_sc,   \
                                             e2ecfg_ct_offset_val,                      \
                                             e2ecfg_cc_offset_val,                      \
                                             e2ecfg_dn_offset_val,                      \
                                             e2ecfg_dl_val)                             \
   (e2ecfg_ct_offset_val),                                                              \
   (e2ecfg_cc_offset_val),                                                              \
   (e2ecfg_di),                                                                         \
   0U,                                                                                  \
   (e2ecfg_dm),                                                                         \
   (e2ecfg_dl_val),                                                                     \
   (e2ecfg_ci),                                                                         \
   (e2ecfg_ld),                                                                         \
   (e2ecfg_sc)

#if (defined E2EPW_GET_CFG_P01_E2E_P01_DATAID_BOTH) /* To prevent double declaration */
#error E2EPW_GET_CFG_P01_E2E_P01_DATAID_BOTH already defined
#endif /* if (defined E2EPW_GET_CFG_P01_E2E_P01_DATAID_BOTH) */

/** \brief Get configuration data for E2E profile 01 with dataIdMode Both
 * in case of pure intra-Ecu communication.
 * This macros expands to the configuration data required to initialize the configuration data
 * of the struct type E2E_P01ConfigType.
 *
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2ecfg_dm macro name that defines the data id mode for the e2e profile
 * \param[in] e2ecfg_di macro name that defines the data id value for the e2e profile
 * \param[in] e2ecfg_ci macro name that defines the MaxDeltaCounterInit value for the e2e profile
 * \param[in] e2ecfg_ld macro name that defines the MaxNoNewOrRepeatedData value for the e2e profile
 * \param[in] e2ecfg_sc macro name that defines the SyncCounterInit value for the e2e profile
 * \param[in] e2ecfg_ct_offset_val bit offset of the counter Data Element member
 * \param[in] e2ecfg_cc_offset_val bit offset of the crc Data Element member
 * \param[in] e2ecfg_dn_offset_val bit offset of the dataIdNibble Data Element member
 * \param[in] e2ecfg_dl_val platform-specific bit length of DataElement type
 */
#define E2EPW_GET_CFG_P01_E2E_P01_DATAID_BOTH(applDataType,e2ecfg_dm,                   \
                                              e2ecfg_di,e2ecfg_ci,e2ecfg_ld,e2ecfg_sc,  \
                                              e2ecfg_ct_offset_val,                     \
                                              e2ecfg_cc_offset_val,                     \
                                              e2ecfg_dn_offset_val,                     \
                                              e2ecfg_dl_val)                            \
   (e2ecfg_ct_offset_val),                                                              \
   (e2ecfg_cc_offset_val),                                                              \
   (e2ecfg_di),                                                                         \
   0U,                                                                                  \
   (e2ecfg_dm),                                                                         \
   (e2ecfg_dl_val),                                                                     \
   (e2ecfg_ci),                                                                         \
   (e2ecfg_ld),                                                                         \
   (e2ecfg_sc)

#if (defined E2EPW_GET_CFG_P01_E2E_P01_DATAID_NIBBLE) /* To prevent double declaration */
#error E2EPW_GET_CFG_P01_E2E_P01_DATAID_NIBBLE already defined
#endif /* if (defined E2EPW_GET_CFG_P01_E2E_P01_DATAID_NIBBLE) */

/** \brief Get configuration data for E2E profile 01 with dataIdMode Nibble
 * in case of pure intra-Ecu communication.
 * This macros expands to the configuration data required to initialize the configuration data
 * of the struct type E2E_P01ConfigType.
 *
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2ecfg_dm macro name that defines the data id mode for the e2e profile
 * \param[in] e2ecfg_di macro name that defines the data id value for the e2e profile
 * \param[in] e2ecfg_ci macro name that defines the MaxDeltaCounterInit value for the e2e profile
 * \param[in] e2ecfg_ld macro name that defines the MaxNoNewOrRepeatedData value for the e2e profile
 * \param[in] e2ecfg_sc macro name that defines the SyncCounterInit value for the e2e profile
 * \param[in] e2ecfg_ct_offset_val bit offset of the counter Data Element member
 * \param[in] e2ecfg_cc_offset_val bit offset of the crc Data Element member
 * \param[in] e2ecfg_dn_offset_val bit offset of the dataIdNibble Data Element member
 * \param[in] e2ecfg_dl_val platform-specific bit length of DataElement type
 */
#define E2EPW_GET_CFG_P01_E2E_P01_DATAID_NIBBLE(applDataType,e2ecfg_dm,                 \
                                              e2ecfg_di,e2ecfg_ci,e2ecfg_ld,e2ecfg_sc,  \
                                              e2ecfg_ct_offset_val,                     \
                                              e2ecfg_cc_offset_val,                     \
                                              e2ecfg_dn_offset_val,                     \
                                              e2ecfg_dl_val)                            \
   (e2ecfg_ct_offset_val),                                                              \
   (e2ecfg_cc_offset_val),                                                              \
   (e2ecfg_di),                                                                         \
   (e2ecfg_dn_offset_val),                                                              \
   (e2ecfg_dm),                                                                         \
   (e2ecfg_dl_val),                                                                     \
   (e2ecfg_ci),                                                                         \
   (e2ecfg_ld),                                                                         \
   (e2ecfg_sc)

#if (defined E2EPW_GET_CFG_P01_E2EPW_CX_IR) /* To prevent double declaration */
#error E2EPW_GET_CFG_P01_E2EPW_CX_IR already defined
#endif /* if (defined E2EPW_GET_CFG_P01_E2EPW_CX_IR) */

/** \brief Get configuration data for E2E profile 01 in case of inter-Ecu communication
 * This macros expands to the configuration data required to initialize the configuration data
 * of the struct type E2E_P01ConfigType.
 *
 *
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2ecfg_ct macro name that defines the counter member of the Data Element
 * \param[in] e2ecfg_cc macro name that defines the crc member of the Data Element
 * \param[in] e2ecfg_dn macro name that defines the data id nibble member of the Data Element
 * \param[in] e2ecfg_di macro name that defines the data id value for the e2e profile
 * \param[in] e2ecfg_dm macro name that defines the data id mode for the e2e profile
 * \param[in] e2ecfg_ci macro name that defines the MaxDeltaCounterInit value for the e2e profile
 * \param[in] e2ecfg_ld macro name that defines the MaxNoNewOrRepeatedData value for the e2e profile
 * \param[in] e2ecfg_sc macro name that defines the SyncCounterInit value for the e2e profile
 */
#define E2EPW_GET_CFG_P01_E2EPW_CX_IR(e2eprotsn,applDataType,e2ecfg_ct,e2ecfg_cc,e2ecfg_dn, \
                                      e2ecfg_di,e2ecfg_dm,e2ecfg_ci,e2ecfg_ld,e2ecfg_sc)    \
   (E2EPW_CFG_TO_ ## e2eprotsn),                                                            \
   (E2EPW_CFG_CO_ ## e2eprotsn),                                                            \
   (e2ecfg_di),                                                                             \
   (E2EPW_CFG_NO_ ## e2eprotsn),                                                            \
   (e2ecfg_dm),                                                                             \
   (E2EPW_CFG_DL_ ## e2eprotsn),                                                            \
   (e2ecfg_ci),                                                                             \
   (e2ecfg_ld),                                                                             \
   (e2ecfg_sc)

#if (defined E2EPW_GET_CFG_PDAI1) /* To prevent double declaration */
#error E2EPW_GET_CFG_PDAI1 already defined
#endif /* if (defined E2EPW_GET_CFG_PDAI1) */

/** \brief Get configuration data for E2E profile DAI1
 * This macros expands to the configuration data required to initialize the configuration data
 * of the struct type E2E_PDAI1ConfigType.
 *
 *
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Can be on of E2EPW_CX_IR, E2EPW_CX_IA
 * \param[in] applDataType Data type name of the application data element
 */
#define E2EPW_GET_CFG_PDAI1(e2eprotsn,detype_comtype,applDataType)                     \
   (uint16)detype_comtype ## _ISCOMPLEXANDINTRA((((uint16)sizeof(applDataType))*8U),   \
                                                E2EPW_CFG_DL_ ## e2eprotsn),           \
   E2EPW_CFG_DI_ ## e2eprotsn,                                                         \
   E2EPW_CFG_CI_ ## e2eprotsn,                                                         \
   E2EPW_CFG_LD_ ## e2eprotsn,                                                         \
   E2EPW_CFG_SC_ ## e2eprotsn

#if (defined E2EPW_GET_CFG_PDAI2) /* To prevent double declaration */
#error E2EPW_GET_CFG_PDAI2 already defined
#endif /* if (defined E2EPW_GET_CFG_PDAI2) */

/** \brief Get configuration data for E2E profile DAI2
 * This macros expands to the configuration data required to initialize the configuration data
 * of the struct type E2E_PDAI2ConfigType.
 *
 *
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Can be on of E2EPW_CX_IR, E2EPW_CX_IA
 * \param[in] applDataType Data type name of the application data element
 */
#define E2EPW_GET_CFG_PDAI2(e2eprotsn,detype_comtype,applDataType)                     \
   (uint16)detype_comtype ## _ISCOMPLEXANDINTRA((((uint16)sizeof(applDataType))*8U),   \
                                                E2EPW_CFG_DL_ ## e2eprotsn),           \
   E2EPW_CFG_DM_ ## e2eprotsn,                                                         \
   E2EPW_CFG_DI_ ## e2eprotsn,                                                         \
   E2EPW_CFG_CI_ ## e2eprotsn,                                                         \
   E2EPW_CFG_LD_ ## e2eprotsn,                                                         \
   E2EPW_CFG_SC_ ## e2eprotsn

#if (!defined E2E_USING_RTE_ISUPDATED)
#error E2E_USING_RTE_ISUPDATED not defined
#else /* (defined E2E_USING_RTE_ISUPDATED) */
#if (E2E_USING_RTE_ISUPDATED != STD_ON)
#if (E2E_USING_RTE_ISUPDATED != STD_OFF)
#error E2E_USING_RTE_ISUPDATED not properly defined
#endif /* (E2E_USING_RTE_ISUPDATED != STD_OFF) */
#endif /* (E2E_USING_RTE_ISUPDATED != STD_ON) */
#endif /* (defined E2E_USING_RTE_ISUPDATED) */

#if (defined E2EPW_GET_RTE_READ) /* To prevent double declaration */
#error E2EPW_GET_RTE_READ already defined
#endif /* if (defined E2EPW_GET_RTE_READ) */

/** \brief Define E2EPW_GET_RTE_READ
 * Macro for reading data element from Rte based on the existence of Rte_IsUpdated.
 * If Rte_IsUpdated() is used, then Rte_Read() is only called, if Rte_IsUpdated() returns TRUE.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[out] data the application Data Element holding the received data
 * \param[out] retvar1 return value of Rte_Read()
 * \param[out] newdataavvar is TRUE if new data was successfully received
 */
#if (E2E_USING_RTE_ISUPDATED == STD_ON)
#define E2EPW_GET_RTE_READ(port_dataelement,data,retvar1,newdataavvar)                 \
   do {                                                                                \
      boolean const isUpdated =                                                        \
         Rte_IsUpdated_ ## port_dataelement ();                                        \
      if (FALSE == isUpdated)                                                          \
      {                                                                                \
         (newdataavvar) = FALSE;                                                       \
         (retvar1) = RTE_E_OK;                                                         \
      }                                                                                \
      else                                                                             \
      {                                                                                \
         (retvar1) = Rte_Read_ ## port_dataelement ((data));                           \
         (newdataavvar) = ((retvar1) == RTE_E_OK) ? TRUE : FALSE;                      \
      }                                                                                \
   } while (0)
#else
#define E2EPW_GET_RTE_READ(port_dataelement,data,retvar1,newdataavvar)                 \
   do {                                                                                \
         (retvar1) = Rte_Read_ ## port_dataelement ((data));                           \
         (newdataavvar) = ((retvar1) == RTE_E_OK) ? TRUE : FALSE;                      \
   } while (0)
#endif /* (E2E_USING_RTE_ISUPDATED == STD_ON) */

/* =========================== E2EPW_DEFVAR1 code template macros ================== */

#if (defined E2EPW_DEFVAR1_E2EPW_SC_WR) /* To prevent double declaration */
#error E2EPW_DEFVAR1_E2EPW_SC_WR already defined
#endif /* if (defined E2EPW_DEFVAR1_E2EPW_SC_WR) */
#if (defined E2EPW_DEFVAR1_E2EPW_SC_RD) /* To prevent double declaration */
#error E2EPW_DEFVAR1_E2EPW_SC_RD already defined
#endif /* if (defined E2EPW_DEFVAR1_E2EPW_SC_RD) */
#if (defined E2EPW_DEFVAR1_E2EPW_RC_WR) /* To prevent double declaration */
#error E2EPW_DEFVAR1_E2EPW_RC_WR already defined
#endif /* if (defined E2EPW_DEFVAR1_E2EPW_RC_WR) */
#if (defined E2EPW_DEFVAR1_E2EPW_RC_RD) /* To prevent double declaration */
#error E2EPW_DEFVAR1_E2EPW_RC_RD already defined
#endif /* if (defined E2EPW_DEFVAR1_E2EPW_RC_RD) */

/** \brief Macro for defining global state variable with internal linkage,
 * used in the corresponding E2EPW single channel write function.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile the name of the E2E profile (e.g. 02, 01, PDAI1, etc.)
 */
#define E2EPW_DEFVAR1_E2EPW_SC_WR(port_dataelement,applDataType,e2eprotsn,e2eprofile)        \
   static VAR(E2EPW_CONCAT3(E2E_P,e2eprofile,SenderStateType), E2EPW_VAR)                    \
      E2EPW_Wr_St_ ## port_dataelement = { 0U };

/** \brief Macro for defining global state variable with internal linkage,
 * used in the corresponding E2EPW single channel read function.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile the name of the E2E profile (e.g. 02, 01, PDAI1, etc.)
 */
#define E2EPW_DEFVAR1_E2EPW_SC_RD(port_dataelement,applDataType,e2eprotsn,e2eprofile)        \
   static VAR(E2EPW_CONCAT3(E2E_P,e2eprofile,ReceiverStateType), E2EPW_VAR)                  \
      E2EPW_Rd_St_ ## port_dataelement =                                                     \
      { (E2EPW_CONCAT3(E2E_P,e2eprofile,STATUS_NONEWDATA)), 0U, 0U, TRUE, FALSE, 0U, 0U, 0U };

/** \brief Macro for defining global state variable with internal linkage,
 * used in the corresponding E2EPW redundant channel 1 write function.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile the name of the E2E profile (e.g. 02, 01, PDAI1, etc.)
 */
#define E2EPW_DEFVAR1_E2EPW_RC_WR(port_dataelement,applDataType,e2eprotsn,e2eprofile) \
   static VAR(E2EPW_CONCAT3(E2E_P,e2eprofile,SenderStateType), E2EPW_VAR)             \
      E2EPW_W1_St_ ## port_dataelement = { 0U };

/** \brief Macro for defining global state variable with internal linkage,
 * used in the corresponding E2EPW redundant channel 1 read function.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile the name of the E2E profile (e.g. 02, 01, PDAI1, etc.)
 */
#define E2EPW_DEFVAR1_E2EPW_RC_RD(port_dataelement,applDataType,e2eprotsn,e2eprofile)        \
   static VAR(E2EPW_CONCAT3(E2E_P,e2eprofile,ReceiverStateType), E2EPW_VAR)                  \
      E2EPW_R1_St_ ## port_dataelement =                                                     \
         { (E2EPW_CONCAT3(E2E_P,e2eprofile,STATUS_NONEWDATA)), 0U, 0U, TRUE, FALSE, 0U, 0U, 0U };

/* ======================= E2EPW_DEFVAR2 code template macros ============================ */

#if (defined E2EPW_DEFVAR2_E2EPW_RC_WR) /* To prevent double declaration */
#error E2EPW_DEFVAR2_E2EPW_RC_WR already defined
#endif /* if (defined E2EPW_DEFVAR2_E2EPW_RC_WR) */
#if (defined E2EPW_DEFVAR2_E2EPW_RC_RD) /* To prevent double declaration */
#error E2EPW_DEFVAR2_E2EPW_RC_RD already defined
#endif /* if (defined E2EPW_DEFVAR2_E2EPW_RC_RD) */

/** \brief Macro for defining global state variable with internal linkage,
 * used in the corresponding E2EPW redundant channel 2 write function.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile the name of the E2E profile (e.g. 02, 01, PDAI1, etc.)
 */
#define E2EPW_DEFVAR2_E2EPW_RC_WR(port_dataelement,applDataType,e2eprotsn,e2eprofile)       \
   static VAR(E2EPW_CONCAT3(E2E_P,e2eprofile,SenderStateType), E2EPW_VAR)                   \
      E2EPW_W2_St_ ## port_dataelement = { 0U };

/** \brief Macro for defining global state variable with internal linkage,
 * used in the corresponding E2EPW redundant channel 2 read function.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile the name of the E2E profile (e.g. 02, 01, PDAI1, etc.)
 */
#define E2EPW_DEFVAR2_E2EPW_RC_RD(port_dataelement,applDataType,e2eprotsn,e2eprofile)       \
   static VAR(E2EPW_CONCAT3(E2E_P,e2eprofile,ReceiverStateType), E2EPW_VAR)                 \
      E2EPW_R2_St_ ## port_dataelement =                                                    \
         { (E2EPW_CONCAT3(E2E_P,e2eprofile,STATUS_NONEWDATA)), 0U, 0U, TRUE, FALSE, 0U, 0U, 0U };

/* ======================== E2EPW_DEFVAR_CONST1 code template macros ===================== */

#if (defined E2EPW_DEFVAR_CONST1_E2EPW_SC_WR) /* To prevent double declaration */
#error E2EPW_DEFVAR_CONST1_E2EPW_SC_WR already defined
#endif /* if (defined E2EPW_DEFVAR_CONST1_E2EPW_SC_WR) */
#if (defined E2EPW_DEFVAR_CONST1_E2EPW_SC_RD) /* To prevent double declaration */
#error E2EPW_DEFVAR_CONST1_E2EPW_SC_RD already defined
#endif /* if (defined E2EPW_DEFVAR_CONST1_E2EPW_SC_RD) */
#if (defined E2EPW_DEFVAR_CONST1_E2EPW_RC_WR) /* To prevent double declaration */
#error E2EPW_DEFVAR_CONST1_E2EPW_RC_WR already defined
#endif /* if (defined E2EPW_DEFVAR_CONST1_E2EPW_RC_WR) */
#if (defined E2EPW_DEFVAR_CONST1_E2EPW_RC_RD) /* To prevent double declaration */
#error E2EPW_DEFVAR_CONST1_E2EPW_RC_RD already defined
#endif /* if (defined E2EPW_DEFVAR_CONST1_E2EPW_RC_RD) */

/** \brief Macro for defining E2E configuration data with internal linkage,
 * used in the corresponding E2EPW single channel write function.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile the name of the E2E profile (e.g. 02, 01, PDAI1, etc.)
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Shall be one of E2EPW_CX_IR, E2EPW_CX_IA
 */
#define E2EPW_DEFVAR_CONST1_E2EPW_SC_WR(port_dataelement,applDataType,e2eprotsn,e2eprofile,detype_comtype) \
   static CONST(E2EPW_CONCAT3(E2E_P,e2eprofile,ConfigType), E2EPW_CONST)                        \
      E2EPW_Wr_Cf_ ## port_dataelement =                                                        \
        { E2EPW_CONCAT2(E2EPW_GET_CFG_P,e2eprofile)(e2eprotsn,detype_comtype,applDataType) };

/** \brief Macro for defining E2E configuration data with internal linkage,
 * used in the corresponding E2EPW single channel read function.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile the name of the E2E profile (e.g. 02, 01, PDAI1, etc.)
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Shall be one of E2EPW_CX_IR, E2EPW_CX_IA
 */
#define E2EPW_DEFVAR_CONST1_E2EPW_SC_RD(port_dataelement,applDataType,e2eprotsn,e2eprofile,detype_comtype) \
   static CONST(E2EPW_CONCAT3(E2E_P,e2eprofile,ConfigType), E2EPW_CONST)                        \
      E2EPW_Rd_Cf_ ## port_dataelement =                                                        \
        { E2EPW_CONCAT2(E2EPW_GET_CFG_P,e2eprofile)(e2eprotsn,detype_comtype,applDataType) };

/** \brief Macro for defining E2E configuration data with internal linkage,
 * used in the corresponding E2EPW redundant channel 1 read function.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile the name of the E2E profile (e.g. 02, 01, PDAI1, etc.)
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Shall be one of E2EPW_CX_IR, E2EPW_CX_IA
 */
#define E2EPW_DEFVAR_CONST1_E2EPW_RC_WR(port_dataelement,applDataType,e2eprotsn,e2eprofile,detype_comtype) \
   static CONST(E2EPW_CONCAT3(E2E_P,e2eprofile,ConfigType), E2EPW_CONST)                         \
      E2EPW_W1_Cf_ ## port_dataelement =                                                         \
        { E2EPW_CONCAT2(E2EPW_GET_CFG_P,e2eprofile)(e2eprotsn,detype_comtype,applDataType) };

/** \brief Macro for defining E2E configuration data with internal linkage,
 * used in the corresponding E2EPW redundant channel 1 write function.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile the name of the E2E profile (e.g. 02, 01, PDAI1, etc.)
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Shall be one of E2EPW_CX_IR, E2EPW_CX_IA
 */
#define E2EPW_DEFVAR_CONST1_E2EPW_RC_RD(port_dataelement,applDataType,e2eprotsn,e2eprofile,detype_comtype) \
   static CONST(E2EPW_CONCAT3(E2E_P,e2eprofile,ConfigType), E2EPW_CONST)                        \
      E2EPW_R1_Cf_ ## port_dataelement =                                                        \
        { E2EPW_CONCAT2(E2EPW_GET_CFG_P,e2eprofile)(e2eprotsn,detype_comtype,applDataType) };


/* =============================== E2EPW_DEFVAR_CONST2 code template macros =================== */

#if (defined E2EPW_DEFVAR_CONST2_E2EPW_SC_RD) /* To prevent double declaration */
#error E2EPW_DEFVAR_CONST2_E2EPW_SC_RD already defined
#endif /* if (defined E2EPW_DEFVAR_CONST2_E2EPW_SC_RD) */
#if (defined E2EPW_DEFVAR_CONST2_E2EPW_SC_WR) /* To prevent double declaration */
#error E2EPW_DEFVAR_CONST2_E2EPW_SC_WR already defined
#endif /* if (defined E2EPW_DEFVAR_CONST2_E2EPW_SC_WR) */
#if (defined E2EPW_DEFVAR_CONST2_E2EPW_RC_WR) /* To prevent double declaration */
#error E2EPW_DEFVAR_CONST2_E2EPW_RC_WR already defined
#endif /* if (defined E2EPW_DEFVAR_CONST2_E2EPW_RC_WR) */
#if (defined E2EPW_DEFVAR_CONST2_E2EPW_RC_RD) /* To prevent double declaration */
#error E2EPW_DEFVAR_CONST2_E2EPW_RC_RD already defined
#endif /* if (defined E2EPW_DEFVAR_CONST2_E2EPW_RC_RD) */

/** \brief Macro for defining E2E configuration data with internal linkage.
 * This macro is empty for single channel write wrapper.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile the name of the E2E profile (e.g. 02, 01, PDAI1, etc.)
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Shall be one of E2EPW_CX_IR, E2EPW_CX_IA
 */
#define E2EPW_DEFVAR_CONST2_E2EPW_SC_WR(port_dataelement,applDataType,e2eprotsn,e2eprofile,detype_comtype)

/** \brief Macro for defining E2E configuration data with internal linkage.
 * This macro is empty for single channel read wrapper.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile the name of the E2E profile (e.g. 02, 01, PDAI1, etc.)
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Shall be one of E2EPW_CX_IR, E2EPW_CX_IA
 */
#define E2EPW_DEFVAR_CONST2_E2EPW_SC_RD(port_dataelement,applDataType,e2eprotsn,e2eprofile,detype_comtype)

/** \brief Macro for defining E2E configuration data with internal linkage,
 * used in the corresponding E2EPW redundant channel 2 read function.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile the name of the E2E profile (e.g. 02, 01, PDAI1, etc.)
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Shall be one of E2EPW_CX_IR, E2EPW_CX_IA
 */
#define E2EPW_DEFVAR_CONST2_E2EPW_RC_WR(port_dataelement,applDataType,e2eprotsn,e2eprofile,detype_comtype) \
   static CONST(E2EPW_CONCAT3(E2E_P,e2eprofile,ConfigType), E2EPW_CONST)                         \
      E2EPW_W2_Cf_ ## port_dataelement =                                                         \
        { E2EPW_CONCAT2(E2EPW_GET_CFG_P,e2eprofile)(e2eprotsn,detype_comtype,applDataType) };

/** \brief Macro for defining E2E configuration data with internal linkage,
 * used in the corresponding E2EPW redundant channel 2 write function.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile the name of the E2E profile (e.g. 02, 01, PDAI1, etc.)
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Shall be one of E2EPW_CX_IR, E2EPW_CX_IA
 */
#define E2EPW_DEFVAR_CONST2_E2EPW_RC_RD(port_dataelement,applDataType,e2eprotsn,e2eprofile,detype_comtype) \
   static CONST(E2EPW_CONCAT3(E2E_P,e2eprofile,ConfigType), E2EPW_CONST)                         \
      E2EPW_R2_Cf_ ## port_dataelement =                                                         \
        { E2EPW_CONCAT2(E2EPW_GET_CFG_P,e2eprofile)(e2eprotsn,detype_comtype,applDataType) };


/* ============================= E2EPW_DEFVAR_CONST3 code template macros ======================== */

#if (defined E2EPW_DEFVAR_CONST3_E2EPW_SC_RD) /* To prevent double declaration */
#error E2EPW_DEFVAR_CONST3_E2EPW_SC_RD already defined
#endif /* if (defined E2EPW_DEFVAR_CONST3_E2EPW_SC_RD) */
#if (defined E2EPW_DEFVAR_CONST3_E2EPW_SC_WR) /* To prevent double declaration */
#error E2EPW_DEFVAR_CONST3_E2EPW_SC_WR already defined
#endif /* if (defined E2EPW_DEFVAR_CONST3_E2EPW_SC_WR) */
#if (defined E2EPW_DEFVAR_CONST3_E2EPW_RC_RD) /* To prevent double declaration */
#error E2EPW_DEFVAR_CONST3_E2EPW_RC_RD already defined
#endif /* if (defined E2EPW_DEFVAR_CONST3_E2EPW_RC_RD) */
#if (defined E2EPW_DEFVAR_CONST3_E2EPW_RC_WR) /* To prevent double declaration */
#error E2EPW_DEFVAR_CONST3_E2EPW_RC_WR already defined
#endif /* if (defined E2EPW_DEFVAR_CONST3_E2EPW_RC_WR) */

/** \brief Macro for defining E2EPW configuration data with internal linkage,
 * used in the corresponding E2EPW single channel read function.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Shall be one of E2EPW_CX_IR, E2EPW_CX_IA
 * \param[in] e2eprofile the name of the E2E profile (e.g. 02, 01, PDAI1, etc.)
 */
#if (E2EPW_ENABLE_PROFILE_P01 == STD_OFF)
#define E2EPW_DEFVAR_CONST3_E2EPW_SC_RD(port_dataelement,applDataType,e2eprotsn,detype_comtype,e2eprofile) \
   static CONST(E2EPW_GenericConfigType, E2EPW_CONST)                                              \
      E2EPW_GenericConfig_Rd_ ## port_dataelement =                                                \
        {                                                                                          \
           & E2EPW_RteRd_ ## port_dataelement,                                                     \
           detype_comtype ## _ISCOMPLEXANDINTER(                                                   \
              & E2EPW_PACK_ ## e2eprotsn,                                                          \
              & E2EPW_CPDE_ ## applDataType                                                        \
           ),                                                                                      \
           & E2EPW_CONCAT2(detype_comtype ## _GenericInnerRead_P,e2eprofile),                      \
           & E2EPW_Rd_Cf_ ## port_dataelement,                                                     \
           & E2EPW_Rd_St_ ## port_dataelement,                                                     \
           (uint16)sizeof(applDataType),                                                           \
           (uint16)detype_comtype ## _ISCOMPLEX(                                                   \
                offsetof(applDataType,E2EPW_CFG_CC_ ## e2eprotsn),                                 \
                0                                                                                  \
           ),                                                                                      \
           (uint16)detype_comtype ## _ISCOMPLEX(                                                   \
                offsetof(applDataType,E2EPW_CFG_CT_ ## e2eprotsn),                                 \
                0                                                                                  \
           ),                                                                                      \
        };
#else
#define E2EPW_DEFVAR_CONST3_E2EPW_SC_RD(port_dataelement,applDataType,e2eprotsn,detype_comtype,e2eprofile) \
   static CONST(E2EPW_GenericConfigType, E2EPW_CONST)                                              \
      E2EPW_GenericConfig_Rd_ ## port_dataelement =                                                \
        {                                                                                          \
           & E2EPW_RteRd_ ## port_dataelement,                                                     \
           detype_comtype ## _ISCOMPLEXANDINTER(                                                   \
              & E2EPW_PACK_ ## e2eprotsn,                                                          \
              & E2EPW_CPDE_ ## applDataType                                                        \
           ),                                                                                      \
           & E2EPW_CONCAT2(detype_comtype ## _GenericInnerRead_P,e2eprofile),                      \
           & E2EPW_Rd_Cf_ ## port_dataelement,                                                     \
           & E2EPW_Rd_St_ ## port_dataelement,                                                     \
           (uint16)sizeof(applDataType),                                                           \
           (uint16)detype_comtype ## _ISCOMPLEX(                                                   \
                offsetof(applDataType,E2EPW_CFG_CC_ ## e2eprotsn),                                 \
                0                                                                                  \
           ),                                                                                      \
           (uint16)detype_comtype ## _ISCOMPLEX(                                                   \
                offsetof(applDataType,E2EPW_CFG_CT_ ## e2eprotsn),                                 \
                0                                                                                  \
           ),                                                                                      \
           (uint16)detype_comtype ## _ISCOMPLEX(                                                   \
                offsetof(applDataType,E2EPW_CFG_DN_ ## e2eprotsn),                                 \
                0                                                                                  \
           ),                                                                                      \
        };
#endif

/** \brief Macro for defining E2EPW configuration data with internal linkage,
 * used in the corresponding E2EPW single channel write function.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Shall be one of E2EPW_CX_IR, E2EPW_CX_IA
 * \param[in] e2eprofile the name of the E2E profile (e.g. 02, 01, PDAI1, etc.)
 */
#if (E2EPW_ENABLE_PROFILE_P01 == STD_OFF)
#define E2EPW_DEFVAR_CONST3_E2EPW_SC_WR(port_dataelement,applDataType,e2eprotsn,detype_comtype,e2eprofile) \
   static CONST(E2EPW_GenericConfigType, E2EPW_CONST)                                              \
      E2EPW_GenericConfig_Wr_ ## port_dataelement =                                                \
        {                                                                                          \
           & E2EPW_RteWr_ ## port_dataelement,                                                     \
           detype_comtype ## _ISCOMPLEXANDINTER(                                                   \
              & E2EPW_PACK_ ## e2eprotsn,                                                          \
              & E2EPW_CPDE_ ## applDataType                                                        \
           ),                                                                                      \
           & E2EPW_CONCAT2(detype_comtype ## _GenericInnerWrite_P,e2eprofile),                     \
           & E2EPW_Wr_Cf_ ## port_dataelement,                                                     \
           & E2EPW_Wr_St_ ## port_dataelement,                                                     \
           (uint16)sizeof(applDataType),                                                           \
           (uint16)detype_comtype ## _ISCOMPLEX(                                                   \
                offsetof(applDataType,E2EPW_CFG_CC_ ## e2eprotsn),                                 \
                0                                                                                  \
           ),                                                                                      \
           (uint16)detype_comtype ## _ISCOMPLEX(                                                   \
                offsetof(applDataType,E2EPW_CFG_CT_ ## e2eprotsn),                                 \
                0                                                                                  \
           ),                                                                                      \
        };
#else
#define E2EPW_DEFVAR_CONST3_E2EPW_SC_WR(port_dataelement,applDataType,e2eprotsn,detype_comtype,e2eprofile) \
   static CONST(E2EPW_GenericConfigType, E2EPW_CONST)                                              \
      E2EPW_GenericConfig_Wr_ ## port_dataelement =                                                \
        {                                                                                          \
           & E2EPW_RteWr_ ## port_dataelement,                                                     \
           detype_comtype ## _ISCOMPLEXANDINTER(                                                   \
              & E2EPW_PACK_ ## e2eprotsn,                                                          \
              & E2EPW_CPDE_ ## applDataType                                                        \
           ),                                                                                      \
           & E2EPW_CONCAT2(detype_comtype ## _GenericInnerWrite_P,e2eprofile),                     \
           & E2EPW_Wr_Cf_ ## port_dataelement,                                                     \
           & E2EPW_Wr_St_ ## port_dataelement,                                                     \
           (uint16)sizeof(applDataType),                                                           \
           (uint16)detype_comtype ## _ISCOMPLEX(                                                   \
                offsetof(applDataType,E2EPW_CFG_CC_ ## e2eprotsn),                                 \
                0                                                                                  \
           ),                                                                                      \
           (uint16)detype_comtype ## _ISCOMPLEX(                                                   \
                offsetof(applDataType,E2EPW_CFG_CT_ ## e2eprotsn),                                 \
                0                                                                                  \
           ),                                                                                      \
           (uint16)detype_comtype ## _ISCOMPLEX(                                                   \
                offsetof(applDataType,E2EPW_CFG_DN_ ## e2eprotsn),                                 \
                0                                                                                  \
           ),                                                                                      \
        };
#endif

/** \brief Macro for defining E2EPW configuration data with internal linkage,
 * used in the corresponding E2EPW redundant channel 1 read function.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Shall be one of E2EPW_CX_IR, E2EPW_CX_IA
 * \param[in] e2eprofile the name of the E2E profile (e.g. 02, 01, PDAI1, etc.)
 */
#if (E2EPW_ENABLE_PROFILE_P01 == STD_OFF)
#define E2EPW_DEFVAR_CONST3_E2EPW_RC_RD(port_dataelement,applDataType,e2eprotsn,detype_comtype,e2eprofile) \
   static CONST(E2EPW_GenericConfigType, E2EPW_CONST)                                              \
      E2EPW_GenericConfig_R1_ ## port_dataelement =                                                \
        {                                                                                          \
           & E2EPW_RteRd_ ## port_dataelement,                                                     \
           detype_comtype ## _ISCOMPLEXANDINTER(                                                   \
              & E2EPW_PACK_ ## e2eprotsn,                                                          \
              & E2EPW_CPDE_ ## applDataType                                                        \
           ),                                                                                      \
           & E2EPW_CONCAT2(detype_comtype ## _GenericInnerRead_P,e2eprofile),                      \
           & E2EPW_R1_Cf_ ## port_dataelement,                                                     \
           & E2EPW_R1_St_ ## port_dataelement,                                                     \
           (uint16)sizeof(applDataType),                                                           \
           (uint16)detype_comtype ## _ISCOMPLEX(                                                   \
                offsetof(applDataType,E2EPW_CFG_CC_ ## e2eprotsn),                                 \
                0                                                                                  \
           ),                                                                                      \
           (uint16)detype_comtype ## _ISCOMPLEX(                                                   \
                offsetof(applDataType,E2EPW_CFG_CT_ ## e2eprotsn),                                 \
                0                                                                                  \
           ),                                                                                      \
        };
#else
#define E2EPW_DEFVAR_CONST3_E2EPW_RC_RD(port_dataelement,applDataType,e2eprotsn,detype_comtype,e2eprofile) \
   static CONST(E2EPW_GenericConfigType, E2EPW_CONST)                                              \
      E2EPW_GenericConfig_R1_ ## port_dataelement =                                                \
        {                                                                                          \
           & E2EPW_RteRd_ ## port_dataelement,                                                     \
           detype_comtype ## _ISCOMPLEXANDINTER(                                                   \
              & E2EPW_PACK_ ## e2eprotsn,                                                          \
              & E2EPW_CPDE_ ## applDataType                                                        \
           ),                                                                                      \
           & E2EPW_CONCAT2(detype_comtype ## _GenericInnerRead_P,e2eprofile),                      \
           & E2EPW_R1_Cf_ ## port_dataelement,                                                     \
           & E2EPW_R1_St_ ## port_dataelement,                                                     \
           (uint16)sizeof(applDataType),                                                           \
           (uint16)detype_comtype ## _ISCOMPLEX(                                                   \
                offsetof(applDataType,E2EPW_CFG_CC_ ## e2eprotsn),                                 \
                0                                                                                  \
           ),                                                                                      \
           (uint16)detype_comtype ## _ISCOMPLEX(                                                   \
                offsetof(applDataType,E2EPW_CFG_CT_ ## e2eprotsn),                                 \
                0                                                                                  \
           ),                                                                                      \
           (uint16)detype_comtype ## _ISCOMPLEX(                                                   \
                offsetof(applDataType,E2EPW_CFG_DN_ ## e2eprotsn),                                 \
                0                                                                                  \
           ),                                                                                      \
        };
#endif

/** \brief Macro for defining E2EPW configuration data with internal linkage,
 * used in the corresponding E2EPW redundant channel 1 write function.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Shall be one of E2EPW_CX_IR, E2EPW_CX_IA
 * \param[in] e2eprofile the name of the E2E profile (e.g. 02, 01, PDAI1, etc.)
 */
#if (E2EPW_ENABLE_PROFILE_P01 == STD_OFF)
#define E2EPW_DEFVAR_CONST3_E2EPW_RC_WR(port_dataelement,applDataType,e2eprotsn,detype_comtype,e2eprofile) \
   static CONST(E2EPW_GenericConfigType, E2EPW_CONST)                                              \
      E2EPW_GenericConfig_W1_ ## port_dataelement =                                                \
        {                                                                                          \
           & E2EPW_RteWr_ ## port_dataelement,                                                     \
           detype_comtype ## _ISCOMPLEXANDINTER(                                                   \
              & E2EPW_PACK_ ## e2eprotsn,                                                          \
              & E2EPW_CPDE_ ## applDataType                                                        \
           ),                                                                                      \
           & E2EPW_CONCAT2(detype_comtype ## _GenericInnerWrite_P,e2eprofile),                     \
           & E2EPW_W1_Cf_ ## port_dataelement,                                                     \
           & E2EPW_W1_St_ ## port_dataelement,                                                     \
           (uint16)sizeof(applDataType),                                                           \
           (uint16)detype_comtype ## _ISCOMPLEX(                                                   \
                offsetof(applDataType,E2EPW_CFG_CC_ ## e2eprotsn),                                 \
                0                                                                                  \
           ),                                                                                      \
           (uint16)detype_comtype ## _ISCOMPLEX(                                                   \
                offsetof(applDataType,E2EPW_CFG_CT_ ## e2eprotsn),                                 \
                0                                                                                  \
           ),                                                                                      \
        };
#else
#define E2EPW_DEFVAR_CONST3_E2EPW_RC_WR(port_dataelement,applDataType,e2eprotsn,detype_comtype,e2eprofile) \
   static CONST(E2EPW_GenericConfigType, E2EPW_CONST)                                              \
      E2EPW_GenericConfig_W1_ ## port_dataelement =                                                \
        {                                                                                          \
           & E2EPW_RteWr_ ## port_dataelement,                                                     \
           detype_comtype ## _ISCOMPLEXANDINTER(                                                   \
              & E2EPW_PACK_ ## e2eprotsn,                                                          \
              & E2EPW_CPDE_ ## applDataType                                                        \
           ),                                                                                      \
           & E2EPW_CONCAT2(detype_comtype ## _GenericInnerWrite_P,e2eprofile),                     \
           & E2EPW_W1_Cf_ ## port_dataelement,                                                     \
           & E2EPW_W1_St_ ## port_dataelement,                                                     \
           (uint16)sizeof(applDataType),                                                           \
           (uint16)detype_comtype ## _ISCOMPLEX(                                                   \
                offsetof(applDataType,E2EPW_CFG_CC_ ## e2eprotsn),                                 \
                0                                                                                  \
           ),                                                                                      \
           (uint16)detype_comtype ## _ISCOMPLEX(                                                   \
                offsetof(applDataType,E2EPW_CFG_CT_ ## e2eprotsn),                                 \
                0                                                                                  \
           ),                                                                                      \
           (uint16)detype_comtype ## _ISCOMPLEX(                                                   \
                offsetof(applDataType,E2EPW_CFG_DN_ ## e2eprotsn),                                 \
                0                                                                                  \
           ),                                                                                      \
        };
#endif

/* ============================== E2EPW_DEFVAR_CONST4 code template macros =================== */

#if (defined E2EPW_DEFVAR_CONST4_E2EPW_SC_RD) /* To prevent double declaration */
#error E2EPW_DEFVAR_CONST4_E2EPW_SC_RD already defined
#endif /* if (defined E2EPW_DEFVAR_CONST4_E2EPW_SC_RD) */
#if (defined E2EPW_DEFVAR_CONST4_E2EPW_SC_WR) /* To prevent double declaration */
#error E2EPW_DEFVAR_CONST4_E2EPW_SC_WR already defined
#endif /* if (defined E2EPW_DEFVAR_CONST4_E2EPW_SC_WR) */
#if (defined E2EPW_DEFVAR_CONST4_E2EPW_RC_RD) /* To prevent double declaration */
#error E2EPW_DEFVAR_CONST4_E2EPW_RC_RD already defined
#endif /* if (defined E2EPW_DEFVAR_CONST4_E2EPW_RC_RD) */
#if (defined E2EPW_DEFVAR_CONST4_E2EPW_RC_WR) /* To prevent double declaration */
#error E2EPW_DEFVAR_CONST4_E2EPW_RC_WR already defined
#endif /* if (defined E2EPW_DEFVAR_CONST4_E2EPW_RC_WR) */

/** \brief Macro for defining E2EPW configuration data with internal linkage,
 * This macro is empty for single channel read function.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Shall be one of E2EPW_CX_IR, E2EPW_CX_IA
 * \param[in] e2eprofile the name of the E2E profile (e.g. 02, 01, PDAI1, etc.)
 */
#define E2EPW_DEFVAR_CONST4_E2EPW_SC_RD(port_dataelement,applDataType,e2eprotsn,detype_comtype,e2eprofile)

/** \brief Macro for defining E2EPW configuration data with internal linkage,
 * This macro is empty for single channel write function.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Shall be one of E2EPW_CX_IR, E2EPW_CX_IA
 * \param[in] e2eprofile the name of the E2E profile (e.g. 02, 01, PDAI1, etc.)
 */
#define E2EPW_DEFVAR_CONST4_E2EPW_SC_WR(port_dataelement,applDataType,e2eprotsn,detype_comtype,e2eprofile)

/** \brief Macro for defining E2EPW configuration data with internal linkage,
 * used in the corresponding E2EPW redundant channel 2 read function.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Shall be one of E2EPW_CX_IR, E2EPW_CX_IA
 * \param[in] e2eprofile the name of the E2E profile (e.g. 02, 01, PDAI1, etc.)
 */
#if (E2EPW_ENABLE_PROFILE_P01 == STD_OFF)
#define E2EPW_DEFVAR_CONST4_E2EPW_RC_RD(port_dataelement,applDataType,e2eprotsn,detype_comtype,e2eprofile) \
   static CONST(E2EPW_GenericConfigType, E2EPW_CONST)                                              \
      E2EPW_GenericConfig_R2_ ## port_dataelement =                                                \
        {                                                                                          \
           & E2EPW_RteRd_ ## port_dataelement,                                                     \
           detype_comtype ## _ISCOMPLEXANDINTER(                                                   \
              & E2EPW_PACK_ ## e2eprotsn,                                                          \
              & E2EPW_CPDE_ ## applDataType                                                        \
           ),                                                                                      \
           & E2EPW_CONCAT2(detype_comtype ## _GenericInnerRead_P,e2eprofile),                      \
           & E2EPW_R2_Cf_ ## port_dataelement,                                                     \
           & E2EPW_R2_St_ ## port_dataelement,                                                     \
           (uint16)sizeof(applDataType),                                                           \
           (uint16)detype_comtype ## _ISCOMPLEX(                                                   \
                offsetof(applDataType,E2EPW_CFG_CC_ ## e2eprotsn),                                 \
                0                                                                                  \
           ),                                                                                      \
           (uint16)detype_comtype ## _ISCOMPLEX(                                                   \
                offsetof(applDataType,E2EPW_CFG_CT_ ## e2eprotsn),                                 \
                0                                                                                  \
           ),                                                                                      \
        };
#else
#define E2EPW_DEFVAR_CONST4_E2EPW_RC_RD(port_dataelement,applDataType,e2eprotsn,detype_comtype,e2eprofile) \
   static CONST(E2EPW_GenericConfigType, E2EPW_CONST)                                              \
      E2EPW_GenericConfig_R2_ ## port_dataelement =                                                \
        {                                                                                          \
           & E2EPW_RteRd_ ## port_dataelement,                                                     \
           detype_comtype ## _ISCOMPLEXANDINTER(                                                   \
              & E2EPW_PACK_ ## e2eprotsn,                                                          \
              & E2EPW_CPDE_ ## applDataType                                                        \
           ),                                                                                      \
           & E2EPW_CONCAT2(detype_comtype ## _GenericInnerRead_P,e2eprofile),                      \
           & E2EPW_R2_Cf_ ## port_dataelement,                                                     \
           & E2EPW_R2_St_ ## port_dataelement,                                                     \
           (uint16)sizeof(applDataType),                                                           \
           (uint16)detype_comtype ## _ISCOMPLEX(                                                   \
                offsetof(applDataType,E2EPW_CFG_CC_ ## e2eprotsn),                                 \
                0                                                                                  \
           ),                                                                                      \
           (uint16)detype_comtype ## _ISCOMPLEX(                                                   \
                offsetof(applDataType,E2EPW_CFG_CT_ ## e2eprotsn),                                 \
                0                                                                                  \
           ),                                                                                      \
           (uint16)detype_comtype ## _ISCOMPLEX(                                                   \
                offsetof(applDataType,E2EPW_CFG_DN_ ## e2eprotsn),                                 \
                0                                                                                  \
           ),                                                                                      \
        };
#endif

/** \brief Macro for defining E2EPW configuration data with internal linkage,
 * used in the corresponding E2EPW redundant channel 2 write function.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Shall be one of E2EPW_CX_IR, E2EPW_CX_IA
 * \param[in] e2eprofile the name of the E2E profile (e.g. 02, 01, PDAI1, etc.)
 */
#if (E2EPW_ENABLE_PROFILE_P01 == STD_OFF)
#define E2EPW_DEFVAR_CONST4_E2EPW_RC_WR(port_dataelement,applDataType,e2eprotsn,detype_comtype,e2eprofile) \
   static CONST(E2EPW_GenericConfigType, E2EPW_CONST)                                              \
      E2EPW_GenericConfig_W2_ ## port_dataelement =                                                \
        {                                                                                          \
           & E2EPW_RteWr_ ## port_dataelement,                                                     \
           detype_comtype ## _ISCOMPLEXANDINTER(                                                   \
              & E2EPW_PACK_ ## e2eprotsn,                                                          \
              & E2EPW_CPDE_ ## applDataType                                                        \
           ),                                                                                      \
           & E2EPW_CONCAT2(detype_comtype ## _GenericInnerWrite2_P,e2eprofile),                    \
           & E2EPW_W2_Cf_ ## port_dataelement,                                                     \
           & E2EPW_W2_St_ ## port_dataelement,                                                     \
           (uint16)sizeof(applDataType),                                                           \
           (uint16)detype_comtype ## _ISCOMPLEX(                                                   \
                offsetof(applDataType,E2EPW_CFG_CC_ ## e2eprotsn),                                 \
                0                                                                                  \
           ),                                                                                      \
           (uint16)detype_comtype ## _ISCOMPLEX(                                                   \
                offsetof(applDataType,E2EPW_CFG_CT_ ## e2eprotsn),                                 \
                0                                                                                  \
           ),                                                                                      \
        };
#else
#define E2EPW_DEFVAR_CONST4_E2EPW_RC_WR(port_dataelement,applDataType,e2eprotsn,detype_comtype,e2eprofile) \
   static CONST(E2EPW_GenericConfigType, E2EPW_CONST)                                              \
      E2EPW_GenericConfig_W2_ ## port_dataelement =                                                \
        {                                                                                          \
           & E2EPW_RteWr_ ## port_dataelement,                                                     \
           detype_comtype ## _ISCOMPLEXANDINTER(                                                   \
              & E2EPW_PACK_ ## e2eprotsn,                                                          \
              & E2EPW_CPDE_ ## applDataType                                                        \
           ),                                                                                      \
           & E2EPW_CONCAT2(detype_comtype ## _GenericInnerWrite2_P,e2eprofile),                    \
           & E2EPW_W2_Cf_ ## port_dataelement,                                                     \
           & E2EPW_W2_St_ ## port_dataelement,                                                     \
           (uint16)sizeof(applDataType),                                                           \
           (uint16)detype_comtype ## _ISCOMPLEX(                                                   \
                offsetof(applDataType,E2EPW_CFG_CC_ ## e2eprotsn),                                 \
                0                                                                                  \
           ),                                                                                      \
           (uint16)detype_comtype ## _ISCOMPLEX(                                                   \
                offsetof(applDataType,E2EPW_CFG_CT_ ## e2eprotsn),                                 \
                0                                                                                  \
           ),                                                                                      \
           (uint16)detype_comtype ## _ISCOMPLEX(                                                   \
                offsetof(applDataType,E2EPW_CFG_DN_ ## e2eprotsn),                                 \
                0                                                                                  \
           ),                                                                                      \
        };
#endif

/* ================================= E2EPW_DEFFUNC1 code template macros ======================= */

#if (defined E2EPW_DEFFUNC1_E2EPW_SC_WR) /* To prevent double declaration */
#error E2EPW_DEFFUNC1_E2EPW_SC_WR already defined
#endif /* if (defined E2EPW_DEFFUNC1_E2EPW_SC_WR) */
#if (defined E2EPW_DEFFUNC1_E2EPW_SC_RD) /* To prevent double declaration */
#error E2EPW_DEFFUNC1_E2EPW_SC_RD already defined
#endif /* if (defined E2EPW_DEFFUNC1_E2EPW_SC_RD) */
#if (defined E2EPW_DEFFUNC1_E2EPW_RC_WR) /* To prevent double declaration */
#error E2EPW_DEFFUNC1_E2EPW_RC_WR already defined
#endif /* if (defined E2EPW_DEFFUNC1_E2EPW_RC_WR) */
#if (defined E2EPW_DEFFUNC1_E2EPW_RC_RD) /* To prevent double declaration */
#error E2EPW_DEFFUNC1_E2EPW_RC_RD already defined
#endif /* if (defined E2EPW_DEFFUNC1_E2EPW_RC_RD) */

/** \brief Define E2EPW_DEFFUNC1_E2EPW_SC_WR
 * This macro provides the prototype declaration to the
 * generic internal wrapper function for the Rte call.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] detypename Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile The E2E Protection profile number. Shall be one of 01,02,03
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Shall be one of E2EPW_CX_IR, E2EPW_CX_IA
 */
#if (E2EPW_ENABLE_TX_SIGNAL_GROUP_ARRAY_ACCESS == STD_ON)
#define E2EPW_DEFFUNC1_E2EPW_SC_WR(port_dataelement,detypename,e2eprotsn,e2eprofile,detype_comtype)\
   static FUNC(Std_ReturnType,E2EPW_CODE) E2EPW_RteWr_ ## port_dataelement                  \
   (                                                                                        \
      P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) genericDataElement,                           \
      P2VAR(uint8, AUTOMATIC, E2EPW_VAR) SerData,                                           \
      uint16 SerDataLength                                                                  \
   );
#else
#define E2EPW_DEFFUNC1_E2EPW_SC_WR(port_dataelement,detypename,e2eprotsn,e2eprofile,detype_comtype)\
   static FUNC(Std_ReturnType,E2EPW_CODE) E2EPW_RteWr_ ## port_dataelement                  \
   (                                                                                        \
      P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) genericDataElement                            \
   );
#endif

/** \brief Define E2EPW_DEFFUNC1_E2EPW_SC_RD
 * This macro provides the prototype declaration to the
 * generic internal wrapper function for the Rte call.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] detypename Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile The E2E Protection profile number. Shall be one of 01,02,03
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Can be on of E2EPW_CX_IR, E2EPW_CX_IA
 */
#if (E2EPW_ENABLE_TX_SIGNAL_GROUP_ARRAY_ACCESS == STD_ON)
#define E2EPW_DEFFUNC1_E2EPW_SC_RD(port_dataelement,detypename,e2eprotsn,e2eprofile,detype_comtype)\
   static FUNC(Std_ReturnType,E2EPW_CODE) E2EPW_RteRd_ ## port_dataelement                  \
   (                                                                                        \
      P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) genericDataElement,                           \
      P2VAR(uint8, AUTOMATIC, E2EPW_VAR) SerData,                                           \
      uint16 SerDataLength                                                                  \
   );
#else
#define E2EPW_DEFFUNC1_E2EPW_SC_RD(port_dataelement,detypename,e2eprotsn,e2eprofile,detype_comtype)\
   static FUNC(Std_ReturnType,E2EPW_CODE) E2EPW_RteRd_ ## port_dataelement                  \
   (                                                                                        \
      P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) genericDataElement                            \
   );
#endif

/** \brief Define E2EPW_DEFFUNC1_E2EPW_RC_RD
 * This macro provides the prototype declaration to the
 * generic internal wrapper function for the Rte call.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] detypename Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile The E2E Protection profile number. Shall be one of 01,02,03
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Can be on of E2EPW_CX_IR, E2EPW_CX_IA
 */
#if (E2EPW_ENABLE_TX_SIGNAL_GROUP_ARRAY_ACCESS == STD_ON)
#define E2EPW_DEFFUNC1_E2EPW_RC_RD(port_dataelement,detypename,e2eprotsn,e2eprofile,detype_comtype)\
   static FUNC(Std_ReturnType,E2EPW_CODE) E2EPW_RteRd_ ## port_dataelement                  \
   (                                                                                        \
      P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) genericDataElement,                           \
      P2VAR(uint8, AUTOMATIC, E2EPW_VAR) SerData,                                           \
      uint16 SerDataLength                                                                  \
   );
#else
#define E2EPW_DEFFUNC1_E2EPW_RC_RD(port_dataelement,detypename,e2eprotsn,e2eprofile,detype_comtype)\
   static FUNC(Std_ReturnType,E2EPW_CODE) E2EPW_RteRd_ ## port_dataelement                  \
   (                                                                                        \
      P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) genericDataElement                            \
   );
#endif


/** \brief Define E2EPW_DEFFUNC1_E2EPW_RC_WR
 * This macro provides the prototype declaration to the
 * generic internal wrapper function for the Rte call.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] detypename Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile The E2E Protection profile number. Shall be one of 01,02,03
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Can be on of E2EPW_CX_IR, E2EPW_CX_IA
 */
#if (E2EPW_ENABLE_TX_SIGNAL_GROUP_ARRAY_ACCESS == STD_ON)
#define E2EPW_DEFFUNC1_E2EPW_RC_WR(port_dataelement,detypename,e2eprotsn,e2eprofile,detype_comtype)\
   static FUNC(Std_ReturnType,E2EPW_CODE) E2EPW_RteWr_ ## port_dataelement                  \
   (                                                                                        \
      P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) genericDataElement,                           \
      P2VAR(uint8, AUTOMATIC, E2EPW_VAR) SerData,                                           \
      uint16 SerDataLength                                                                  \
   );
#else
#define E2EPW_DEFFUNC1_E2EPW_RC_WR(port_dataelement,detypename,e2eprotsn,e2eprofile,detype_comtype)\
   static FUNC(Std_ReturnType,E2EPW_CODE) E2EPW_RteWr_ ## port_dataelement                  \
   (                                                                                        \
      P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) genericDataElement                            \
   );
#endif

/* ================= E2EPW_DEFFUNC2 code template macros ============================ */

#if (defined E2EPW_DEFFUNC2_E2EPW_SC_WR) /* To prevent double declaration */
#error E2EPW_DEFFUNC2_E2EPW_SC_WR already defined
#endif /* if (defined E2EPW_DEFFUNC2_E2EPW_SC_WR) */
#if (defined E2EPW_DEFFUNC2_E2EPW_SC_WR_) /* To prevent double declaration */
#error E2EPW_DEFFUNC2_E2EPW_SC_WR_ already defined
#endif /* if (defined E2EPW_DEFFUNC2_E2EPW_SC_WR_) */
#if (defined E2EPW_DEFFUNC2_E2EPW_SC_WR_E2EPW_CX_IR) /* To prevent double declaration */
#error E2EPW_DEFFUNC2_E2EPW_SC_WR_E2EPW_CX_IR already defined
#endif /* if (defined E2EPW_DEFFUNC2_E2EPW_SC_WR_E2EPW_CX_IR) */
#if (defined E2EPW_DEFFUNC2_E2EPW_SC_WR_E2EPW_CX_IA) /* To prevent double declaration */
#error E2EPW_DEFFUNC2_E2EPW_SC_WR_E2EPW_CX_IA already defined
#endif /* if (defined E2EPW_DEFFUNC2_E2EPW_SC_WR_E2EPW_CX_IA) */
#if (defined E2EPW_DEFFUNC2_E2EPW_SC_RD) /* To prevent double declaration */
#error E2EPW_DEFFUNC2_E2EPW_SC_RD already defined
#endif /* if (defined E2EPW_DEFFUNC2_E2EPW_SC_RD) */
#if (defined E2EPW_DEFFUNC2_E2EPW_RC_WR) /* To prevent double declaration */
#error E2EPW_DEFFUNC2_E2EPW_RC_WR already defined
#endif /* if (defined E2EPW_DEFFUNC2_E2EPW_RC_WR) */
#if (defined E2EPW_DEFFUNC2_E2EPW_RC_WR_) /* To prevent double declaration */
#error E2EPW_DEFFUNC2_E2EPW_RC_WR_ already defined
#endif /* if (defined E2EPW_DEFFUNC2_E2EPW_RC_WR_) */
#if (defined E2EPW_DEFFUNC2_E2EPW_RC_WR_E2EPW_CX_IR) /* To prevent double declaration */
#error E2EPW_DEFFUNC2_E2EPW_RC_WR_E2EPW_CX_IR already defined
#endif /* if (defined E2EPW_DEFFUNC2_E2EPW_RC_WR_E2EPW_CX_IR) */
#if (defined E2EPW_DEFFUNC2_E2EPW_RC_WR_E2EPW_CX_IA) /* To prevent double declaration */
#error E2EPW_DEFFUNC2_E2EPW_RC_WR_E2EPW_CX_IA already defined
#endif /* if (defined E2EPW_DEFFUNC2_E2EPW_RC_WR_E2EPW_CX_IA) */
#if (defined E2EPW_DEFFUNC2_E2EPW_RC_RD) /* To prevent double declaration */
#error E2EPW_DEFFUNC2_E2EPW_RC_RD already defined
#endif /* if (defined E2EPW_DEFFUNC2_E2EPW_RC_RD) */

/** \brief Define E2EPW_DEFFUNC2_E2EPW_SC_WR
 * This macro provides the definition of the generic internal wrapper function for the Rte call.
 * If the optimization parameter TxSignalGroupArrayAccess is enabled, then
 * Rte_WriteArray_p_o() API is called, otherwise, Rte_Write_p_o().
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] detypename Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile The E2E Protection profile number. Shall be one of 01,02,03
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Shall be one of E2EPW_CX_IR, E2EPW_CX_IA
 */
#if (E2EPW_ENABLE_TX_SIGNAL_GROUP_ARRAY_ACCESS == STD_ON)
#define E2EPW_DEFFUNC2_E2EPW_SC_WR(port_dataelement,detypename,e2eprotsn,e2eprofile,detype_comtype)\
    E2EPW_DEFFUNC2_E2EPW_SC_WR_ ## detype_comtype(port_dataelement,detypename,e2eprotsn,e2eprofile)
#else
#define E2EPW_DEFFUNC2_E2EPW_SC_WR(port_dataelement,detypename,e2eprotsn,e2eprofile,detype_comtype)\
   static FUNC(Std_ReturnType,E2EPW_CODE) E2EPW_RteWr_ ## port_dataelement                  \
   (                                                                                        \
      P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) genericDataElement                            \
   )                                                                                        \
   {                                                                                        \
      return Rte_Write_ ## port_dataelement(genericDataElement);                            \
   }
#endif

/** \brief Define E2EPW_DEFFUNC2_E2EPW_SC_WR_E2EPW_CX_IR
 * This macro provides the definition of the generic internal wrapper function for the Rte call,
 * in case the optimization parameter TxSignalGroupArrayAccess is enabled and this communication
 * is not a pure intra-ECU communication (Rte_WriteArray_p_o() API is called).
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] detypename Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile The E2E Protection profile number. Shall be one of 01,02,03
 */
#define E2EPW_DEFFUNC2_E2EPW_SC_WR_E2EPW_CX_IR(port_dataelement,detypename,e2eprotsn,e2eprofile)\
   static FUNC(Std_ReturnType,E2EPW_CODE) E2EPW_RteWr_ ## port_dataelement                    \
   (                                                                                          \
      P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) genericDataElement,                             \
      P2VAR(uint8, AUTOMATIC, E2EPW_VAR) SerData,                                             \
      uint16 SerDataLength                                                                    \
   )                                                                                          \
   {                                                                                          \
      return Rte_WriteArray_ ## port_dataelement(genericDataElement, SerData, SerDataLength); \
   }

/** \brief Define E2EPW_DEFFUNC2_E2EPW_SC_WR_E2EPW_CX_IA
 * This macro provides the definition of the generic internal wrapper function for the Rte call,
 * in case the optimization parameter TxSignalGroupArrayAccess is enabled and this communication
 * is a pure intra-ECU communication (Rte_Write_p_o() API is called).
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] detypename Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile The E2E Protection profile number. Shall be one of 01,02,03
 */
#define E2EPW_DEFFUNC2_E2EPW_SC_WR_E2EPW_CX_IA(port_dataelement,detypename,e2eprotsn,e2eprofile)\
   static FUNC(Std_ReturnType,E2EPW_CODE) E2EPW_RteWr_ ## port_dataelement                      \
   (                                                                                            \
      P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) genericDataElement,                               \
      P2VAR(uint8, AUTOMATIC, E2EPW_VAR) SerData,                                               \
      uint16 SerDataLength                                                                      \
   )                                                                                            \
   {                                                                                            \
      /* Parameters SerDataLength and SerData are unused in case of intra-ECU communication */  \
      E2EPW_EB_PARAM_UNUSED(SerDataLength);                                                     \
      E2EPW_EB_PARAM_UNUSED(SerData);                                                           \
      return Rte_Write_ ## port_dataelement(genericDataElement);                                \
   }


/** \brief Define E2EPW_DEFFUNC2_E2EPW_SC_RD
 * This macro provides the definition of the generic wrapper function for the Rte call.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] detypename Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile The E2E Protection profile number. Shall be one of 01,02,03
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Can be on of E2EPW_CX_IR, E2EPW_CX_IA
 */
#if (E2EPW_ENABLE_TX_SIGNAL_GROUP_ARRAY_ACCESS == STD_ON)
#define E2EPW_DEFFUNC2_E2EPW_SC_RD(port_dataelement,detypename,e2eprotsn,e2eprofile,detype_comtype)\
   static FUNC(Std_ReturnType,E2EPW_CODE) E2EPW_RteRd_ ## port_dataelement                \
   (                                                                                      \
      P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) genericDataElement,                         \
      P2VAR(uint8, AUTOMATIC, E2EPW_VAR) SerData,                                         \
      uint16 SerDataLength                                                                \
   )                                                                                      \
   {                                                                                      \
      uint8 retval;                                                                       \
      CONSTP2VAR(detypename, AUTOMATIC, E2EPW_APPL_DATA) dataElement                      \
         = (P2VAR(detypename, AUTOMATIC, E2EPW_APPL_DATA))genericDataElement;             \
      /* Parameters SerDataLength and SerData are unused at Rx side */                    \
      E2EPW_EB_PARAM_UNUSED(SerDataLength);                                               \
      E2EPW_EB_PARAM_UNUSED(SerData);                                                     \
      E2EPW_GET_RTE_READ(port_dataelement,dataElement,retval,                             \
        ((E2EPW_Rd_St_ ## port_dataelement).NewDataAvailable));                           \
      return retval;                                                                      \
   }
#else
#define E2EPW_DEFFUNC2_E2EPW_SC_RD(port_dataelement,detypename,e2eprotsn,e2eprofile,detype_comtype)\
   static FUNC(Std_ReturnType,E2EPW_CODE) E2EPW_RteRd_ ## port_dataelement                \
   (                                                                                      \
      P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) genericDataElement                          \
   )                                                                                      \
   {                                                                                      \
      uint8 retval;                                                                       \
      CONSTP2VAR(detypename, AUTOMATIC, E2EPW_APPL_DATA) dataElement                      \
         = (P2VAR(detypename, AUTOMATIC, E2EPW_APPL_DATA))genericDataElement;             \
      E2EPW_GET_RTE_READ(port_dataelement,dataElement,retval,                             \
        ((E2EPW_Rd_St_ ## port_dataelement).NewDataAvailable));                           \
      return retval;                                                                      \
   }
#endif

/** \brief Define E2EPW_DEFFUNC2_E2EPW_RC_RD
 * This macro provides the definition of the generic wrapper function for the Rte call.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] detypename Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile The E2E Protection profile number. Shall be one of 01,02,03
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Can be on of E2EPW_CX_IR, E2EPW_CX_IA
 */
#if (E2EPW_ENABLE_TX_SIGNAL_GROUP_ARRAY_ACCESS == STD_ON)
#define E2EPW_DEFFUNC2_E2EPW_RC_RD(port_dataelement,detypename,e2eprotsn,e2eprofile,detype_comtype)\
   static FUNC(Std_ReturnType,E2EPW_CODE) E2EPW_RteRd_ ## port_dataelement                \
   (                                                                                      \
      P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) genericDataElement,                         \
      P2VAR(uint8, AUTOMATIC, E2EPW_VAR) SerData,                                         \
      uint16 SerDataLength                                                                \
   )                                                                                      \
   {                                                                                      \
      uint8 retval;                                                                       \
      CONSTP2VAR(detypename, AUTOMATIC, E2EPW_APPL_DATA) dataElement                      \
         = (P2VAR(detypename, AUTOMATIC, E2EPW_APPL_DATA)) genericDataElement;            \
      /* Parameters SerDataLength and SerData are unused at Rx side */                    \
      E2EPW_EB_PARAM_UNUSED(SerDataLength);                                               \
      E2EPW_EB_PARAM_UNUSED(SerData);                                                     \
      E2EPW_GET_RTE_READ(port_dataelement,dataElement,retval,                             \
        ((E2EPW_R1_St_ ## port_dataelement).NewDataAvailable));                           \
      return retval;                                                                      \
   }
#else
#define E2EPW_DEFFUNC2_E2EPW_RC_RD(port_dataelement,detypename,e2eprotsn,e2eprofile,detype_comtype)\
   static FUNC(Std_ReturnType,E2EPW_CODE) E2EPW_RteRd_ ## port_dataelement                \
   (                                                                                      \
      P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) genericDataElement                          \
   )                                                                                      \
   {                                                                                      \
      uint8 retval;                                                                       \
      CONSTP2VAR(detypename, AUTOMATIC, E2EPW_APPL_DATA) dataElement                      \
         = (P2VAR(detypename, AUTOMATIC, E2EPW_APPL_DATA)) genericDataElement;            \
      E2EPW_GET_RTE_READ(port_dataelement,dataElement,retval,                             \
        ((E2EPW_R1_St_ ## port_dataelement).NewDataAvailable));                           \
      return retval;                                                                      \
   }
#endif

/** \brief Define E2EPW_DEFFUNC2_E2EPW_RC_WR
 * This macro provides the definition of the generic wrapper function for the Rte call.
 * If the optimization parameter TxSignalGroupArrayAccess is enabled and this is not
 * a pure intra-ECU communication, then Rte_WriteArray_p_o() is called, otherwise, Rte_Write_p_o().
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] detypename Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile The E2E Protection profile number. Shall be one of 01,02,03
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Can be on of E2EPW_CX_IR, E2EPW_CX_IA
 */
#if (E2EPW_ENABLE_TX_SIGNAL_GROUP_ARRAY_ACCESS == STD_ON)
#define E2EPW_DEFFUNC2_E2EPW_RC_WR(port_dataelement,detypename,e2eprotsn,e2eprofile,detype_comtype)\
    E2EPW_DEFFUNC2_E2EPW_RC_WR_ ## detype_comtype(port_dataelement,detypename,e2eprotsn,e2eprofile)
#else
#define E2EPW_DEFFUNC2_E2EPW_RC_WR(port_dataelement,detypename,e2eprotsn,e2eprofile,detype_comtype)\
   static FUNC(Std_ReturnType,E2EPW_CODE) E2EPW_RteWr_ ## port_dataelement                \
   (                                                                                      \
      P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) genericDataElement                          \
   )                                                                                      \
   {                                                                                      \
      return Rte_Write_ ## port_dataelement(genericDataElement);                          \
   }
#endif

/** \brief Define E2EPW_DEFFUNC2_E2EPW_RC_WR_E2EPW_CX_IR
 * This macro provides the definition of the generic internal wrapper function for the Rte call,
 * in case the optimization parameter TxSignalGroupArrayAccess is enabled, redundant channel
 * is activated, and this communication is not a pure intra-ECU communicati
 * (Rte_WriteArray_p_o() is called).
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] detypename Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile The E2E Protection profile number. Shall be one of 01,02,03
 */
#define E2EPW_DEFFUNC2_E2EPW_RC_WR_E2EPW_CX_IR(port_dataelement,detypename,e2eprotsn,e2eprofile)\
   static FUNC(Std_ReturnType,E2EPW_CODE) E2EPW_RteWr_ ## port_dataelement                    \
   (                                                                                          \
      P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) genericDataElement,                             \
      P2VAR(uint8, AUTOMATIC, E2EPW_VAR) SerData,                                             \
      uint16 SerDataLength                                                                    \
   )                                                                                          \
   {                                                                                          \
      return Rte_WriteArray_ ## port_dataelement(genericDataElement, SerData, SerDataLength); \
   }

/** \brief Define E2EPW_DEFFUNC2_E2EPW_RC_WR_E2EPW_CX_IA
 * This macro provides the definition of the generic internal wrapper function for the Rte call,
 * in case the optimization parameter TxSignalGroupArrayAccess is enabled, redundant channel
 * is activated, and this communication is a pure intra-ECU communicati
 * (Rte_Write_p_o() is called).
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] detypename Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile The E2E Protection profile number. Shall be one of 01,02,03
 */
#define E2EPW_DEFFUNC2_E2EPW_RC_WR_E2EPW_CX_IA(port_dataelement,detypename,e2eprotsn,e2eprofile)\
   static FUNC(Std_ReturnType,E2EPW_CODE) E2EPW_RteWr_ ## port_dataelement                      \
   (                                                                                            \
      P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) genericDataElement,                               \
      P2VAR(uint8, AUTOMATIC, E2EPW_VAR) SerData,                                               \
      uint16 SerDataLength                                                                      \
   )                                                                                            \
   {                                                                                            \
      /* Parameters SerDataLength and SerData are unused in case of intra-ECU communication */  \
      E2EPW_EB_PARAM_UNUSED(SerDataLength);                                                     \
      E2EPW_EB_PARAM_UNUSED(SerData);                                                           \
      return Rte_Write_ ## port_dataelement(genericDataElement);                                \
   }


/* ================== E2EPW_DEFFUNC3 code template macros ========================== */

#if (defined E2EPW_DEFFUNC3_E2EPW_SC_RD) /* To prevent double declaration */
#error E2EPW_DEFFUNC3_E2EPW_SC_RD already defined
#endif /* if (defined E2EPW_DEFFUNC3_E2EPW_SC_RD) */
#if (defined E2EPW_DEFFUNC3_E2EPW_SC_RD_E2EPW_CX_IA) /* To prevent double declaration */
#error E2EPW_DEFFUNC3_E2EPW_SC_RD_E2EPW_CX_IA already defined
#endif /* if (defined E2EPW_DEFFUNC3_E2EPW_SC_RD_E2EPW_CX_IA) */
#if (defined E2EPW_DEFFUNC3_E2EPW_SC_RD_E2EPW_CX_IR) /* To prevent double declaration */
#error E2EPW_DEFFUNC3_E2EPW_SC_RD_E2EPW_CX_IR already defined
#endif /* if (defined E2EPW_DEFFUNC3_E2EPW_SC_RD_E2EPW_CX_IR) */
#if (defined E2EPW_DEFFUNC3_E2EPW_SC_WR) /* To prevent double declaration */
#error E2EPW_DEFFUNC3_E2EPW_SC_WR already defined
#endif /* if (defined E2EPW_DEFFUNC3_E2EPW_SC_WR) */
#if (defined E2EPW_DEFFUNC3_E2EPW_SC_WR_E2EPW_CX_IA) /* To prevent double declaration */
#error E2EPW_DEFFUNC3_E2EPW_SC_WR_E2EPW_CX_IA already defined
#endif /* if (defined E2EPW_DEFFUNC3_E2EPW_SC_WR_E2EPW_CX_IA) */
#if (defined E2EPW_DEFFUNC3_E2EPW_SC_WR_E2EPW_CX_IR) /* To prevent double declaration */
#error E2EPW_DEFFUNC3_E2EPW_SC_WR_E2EPW_CX_IR already defined
#endif /* if (defined E2EPW_DEFFUNC3_E2EPW_SC_WR_E2EPW_CX_IR) */
#if (defined E2EPW_DEFFUNC3_E2EPW_RC_WR) /* To prevent double declaration */
#error E2EPW_DEFFUNC3_E2EPW_RC_WR already defined
#endif /* if (defined E2EPW_DEFFUNC3_E2EPW_RC_WR) */
#if (defined E2EPW_DEFFUNC3_E2EPW_RC_WR_E2EPW_CX_IA) /* To prevent double declaration */
#error E2EPW_DEFFUNC3_E2EPW_RC_WR_E2EPW_CX_IA already defined
#endif /* if (defined E2EPW_DEFFUNC3_E2EPW_RC_WR_E2EPW_CX_IA) */
#if (defined E2EPW_DEFFUNC3_E2EPW_RC_WR_E2EPW_CX_IR) /* To prevent double declaration */
#error E2EPW_DEFFUNC3_E2EPW_RC_WR_E2EPW_CX_IR already defined
#endif /* if (defined E2EPW_DEFFUNC3_E2EPW_RC_WR_E2EPW_CX_IR) */
#if (defined E2EPW_DEFFUNC3_E2EPW_RC_RD) /* To prevent double declaration */
#error E2EPW_DEFFUNC3_E2EPW_RC_RD already defined
#endif /* if (defined E2EPW_DEFFUNC3_E2EPW_RC_RD) */
#if (defined E2EPW_DEFFUNC3_E2EPW_RC_RD_E2EPW_CX_IA) /* To prevent double declaration */
#error E2EPW_DEFFUNC3_E2EPW_RC_RD_E2EPW_CX_IA already defined
#endif /* if (defined E2EPW_DEFFUNC3_E2EPW_RC_RD_E2EPW_CX_IA) */
#if (defined E2EPW_DEFFUNC3_E2EPW_RC_RD_E2EPW_CX_IR) /* To prevent double declaration */
#error E2EPW_DEFFUNC3_E2EPW_RC_RD_E2EPW_CX_IR already defined
#endif /* if (defined E2EPW_DEFFUNC3_E2EPW_RC_RD_E2EPW_CX_IR) */

/** \brief Define E2EPW_DEFFUNC3_E2EPW_SC_WR
 * Macro generates single channel write wrapper function.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] detypename Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile The E2E Protection profile number. Shall be one of 01,02,03
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Shall be one of E2EPW_CX_IR, E2EPW_CX_IA
 */
#define E2EPW_DEFFUNC3_E2EPW_SC_WR(port_dataelement,detypename,e2eprotsn,e2eprofile,detype_comtype)\
   E2EPW_DEFFUNC3_E2EPW_SC_WR_ ## detype_comtype(port_dataelement,detypename,e2eprotsn,e2eprofile)

/** \brief Define E2EPW_DEFFUNC3_E2EPW_SC_WR_E2EPW_CX_IA
 * Macro generates single channel write wrapper function for intra-Ecu communication
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] detypename Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile The E2E Protection profile number. Shall be one of 01,02,03
 */
#if (E2EPW_ENABLE_TX_SIGNAL_GROUP_ARRAY_ACCESS == STD_ON)
#define E2EPW_DEFFUNC3_E2EPW_SC_WR_E2EPW_CX_IA(port_dataelement,detypename,e2eprotsn,e2eprofile)\
   FUNC(uint32,E2EPW_CODE) E2EPW_Write_ ## port_dataelement                               \
   (                                                                                      \
      P2VAR(detypename, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_Data                            \
   )                                                                                      \
   {                                                                                      \
      detypename SerDataDEData;                                                           \
                                                                                          \
      return E2EPW_GenericWrite(                                                          \
            &E2EPW_GenericConfig_Wr_ ## port_dataelement,                                 \
            E2EPW_Data,                                                                   \
            (P2VAR(void, AUTOMATIC, E2EPW_VAR)) &SerDataDEData,                           \
            0U                                                                            \
         );                                                                               \
   }
#else
#define E2EPW_DEFFUNC3_E2EPW_SC_WR_E2EPW_CX_IA(port_dataelement,detypename,e2eprotsn,e2eprofile)\
   FUNC(uint32,E2EPW_CODE) E2EPW_Write_ ## port_dataelement                               \
   (                                                                                      \
      P2VAR(detypename, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_Data                            \
   )                                                                                      \
   {                                                                                      \
      detypename SerDataDEData;                                                           \
                                                                                          \
      return E2EPW_GenericWrite(                                                          \
            &E2EPW_GenericConfig_Wr_ ## port_dataelement,                                 \
            E2EPW_Data,                                                                   \
            (P2VAR(void, AUTOMATIC, E2EPW_VAR)) &SerDataDEData                            \
         );                                                                               \
   }
#endif


/** \brief Define E2EPW_DEFFUNC3_E2EPW_SC_WR_E2EPW_CX_IA
 * Macro generates single channel write wrapper function for inter-Ecu communication
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] detypename Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile The E2E Protection profile number. Shall be one of 01,02,03
 */
#if (E2EPW_ENABLE_TX_SIGNAL_GROUP_ARRAY_ACCESS == STD_ON)
#define E2EPW_DEFFUNC3_E2EPW_SC_WR_E2EPW_CX_IR(port_dataelement,detypename,e2eprotsn,e2eprofile)\
   FUNC(uint32,E2EPW_CODE) E2EPW_Write_ ## port_dataelement                               \
   (                                                                                      \
      P2VAR(detypename, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_Data                            \
   )                                                                                      \
   {                                                                                      \
      uint8 SerDataDEData[(E2EPW_CFG_DL_ ## e2eprotsn)/8U];                               \
                                                                                          \
      return E2EPW_GenericWrite(                                                          \
            &E2EPW_GenericConfig_Wr_ ## port_dataelement,                                 \
            E2EPW_Data,                                                                   \
            (P2VAR(void, AUTOMATIC, E2EPW_VAR)) SerDataDEData,                            \
            (uint16)sizeof(SerDataDEData)                                                 \
         );                                                                               \
   }
#else
#define E2EPW_DEFFUNC3_E2EPW_SC_WR_E2EPW_CX_IR(port_dataelement,detypename,e2eprotsn,e2eprofile)\
   FUNC(uint32,E2EPW_CODE) E2EPW_Write_ ## port_dataelement                               \
   (                                                                                      \
      P2VAR(detypename, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_Data                            \
   )                                                                                      \
   {                                                                                      \
      uint8 SerDataDEData[(E2EPW_CFG_DL_ ## e2eprotsn)/8U];                               \
                                                                                          \
      return E2EPW_GenericWrite(                                                          \
            &E2EPW_GenericConfig_Wr_ ## port_dataelement,                                 \
            E2EPW_Data,                                                                   \
            (P2VAR(void, AUTOMATIC, E2EPW_VAR)) SerDataDEData                             \
         );                                                                               \
   }
#endif

/** \brief Define E2EPW_DEFFUNC3_E2EPW_SC_RD
 * Macro generates single channel read wrapper function.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] detypename Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile The E2E Protection profile number. Shall be one of 01,02,03
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Can be on of E2EPW_CX_IR, E2EPW_CX_IA
 */
#define E2EPW_DEFFUNC3_E2EPW_SC_RD(port_dataelement,detypename,e2eprotsn,e2eprofile,detype_comtype)\
   E2EPW_DEFFUNC3_E2EPW_SC_RD_ ## detype_comtype(port_dataelement,detypename,e2eprotsn,e2eprofile)

/** \brief Define E2EPW_DEFFUNC3_E2EPW_SC_RD_E2EPW_CX_IA
 * Macro generates single channel read wrapper function for intra-Ecu communication
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] detypename Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile The E2E Protection profile number. Shall be one of 01,02,03
 */
#define E2EPW_DEFFUNC3_E2EPW_SC_RD_E2EPW_CX_IA(port_dataelement,detypename,e2eprotsn,e2eprofile)\
   FUNC(uint32,E2EPW_CODE) E2EPW_Read_ ## port_dataelement                                \
   (                                                                                      \
      P2VAR(detypename, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_Data                            \
   )                                                                                      \
   {                                                                                      \
      detypename SerDataDEData;                                                           \
                                                                                          \
      return E2EPW_GenericRead(                                                           \
            &E2EPW_GenericConfig_Rd_ ## port_dataelement,                                 \
            E2EPW_Data,                                                                   \
            (P2VAR(void, AUTOMATIC, E2EPW_VAR)) &SerDataDEData                            \
         );                                                                               \
   }

/** \brief Define E2EPW_DEFFUNC3_E2EPW_SC_RD_E2EPW_CX_IR
 * Macro generates single channel read wrapper function for inter-Ecu communication
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] detypename Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile The E2E Protection profile number. Shall be one of 01,02,03
 */
#define E2EPW_DEFFUNC3_E2EPW_SC_RD_E2EPW_CX_IR(port_dataelement,detypename,e2eprotsn,e2eprofile)\
   FUNC(uint32,E2EPW_CODE) E2EPW_Read_ ## port_dataelement                                \
   (                                                                                      \
      P2VAR(detypename, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_Data                            \
   )                                                                                      \
   {                                                                                      \
      uint8 SerDataDEData[(E2EPW_CFG_DL_ ## e2eprotsn)/8U];                               \
                                                                                          \
      return E2EPW_GenericRead(                                                           \
            &E2EPW_GenericConfig_Rd_ ## port_dataelement,                                 \
            E2EPW_Data,                                                                   \
            (P2VAR(void, AUTOMATIC, E2EPW_VAR)) SerDataDEData                             \
         );                                                                               \
   }

/** \brief Define E2EPW_DEFFUNC3_E2EPW_RC_WR
 * Macro generates redundant channel write wrapper function number 1.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] detypename Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile The E2E Protection profile number. Shall be one of 01,02,03
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Can be on of E2EPW_CX_IR, E2EPW_CX_IA
 */
#define E2EPW_DEFFUNC3_E2EPW_RC_WR(port_dataelement,detypename,e2eprotsn,e2eprofile,detype_comtype)\
   E2EPW_DEFFUNC3_E2EPW_RC_WR_ ## detype_comtype(port_dataelement,detypename,e2eprotsn,e2eprofile)

/** \brief Define E2EPW_DEFFUNC3_E2EPW_RC_WR_E2EPW_CX_IA
 * Macro generates redundant channel write wrapper function number 1 for intra-Ecu communication
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] detypename Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile The E2E Protection profile number. Shall be one of 01,02,03
 */
#define E2EPW_DEFFUNC3_E2EPW_RC_WR_E2EPW_CX_IA(port_dataelement,detypename,e2eprotsn,e2eprofile)\
   FUNC(uint32,E2EPW_CODE) E2EPW_Write1_ ## port_dataelement                              \
   (                                                                                      \
      P2VAR(detypename, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_Data                            \
   )                                                                                      \
   {                                                                                      \
      detypename SerDataDEData;                                                           \
                                                                                          \
      return E2EPW_GenericWrite1(                                                         \
            &E2EPW_GenericConfig_W1_ ## port_dataelement,                                 \
            E2EPW_Data,                                                                   \
            (P2VAR(void, AUTOMATIC, E2EPW_VAR)) &SerDataDEData                            \
         );                                                                               \
   }

/** \brief Define E2EPW_DEFFUNC3_E2EPW_RC_WR_E2EPW_CX_IR
 * Macro generates redundant channel write wrapper function number 1 for inter-Ecu communication
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] detypename Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile The E2E Protection profile number. Shall be one of 01,02,03
 */
#define E2EPW_DEFFUNC3_E2EPW_RC_WR_E2EPW_CX_IR(port_dataelement,detypename,e2eprotsn,e2eprofile)\
   FUNC(uint32,E2EPW_CODE) E2EPW_Write1_ ## port_dataelement                              \
   (                                                                                      \
      P2VAR(detypename, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_Data                            \
   )                                                                                      \
   {                                                                                      \
      uint8 SerDataDEData[(E2EPW_CFG_DL_ ## e2eprotsn)/8U];                               \
                                                                                          \
      return E2EPW_GenericWrite1(                                                         \
            &E2EPW_GenericConfig_W1_ ## port_dataelement,                                 \
            E2EPW_Data,                                                                   \
            (P2VAR(void, AUTOMATIC, E2EPW_VAR)) SerDataDEData                             \
         );                                                                               \
   }

/** \brief Define E2EPW_DEFFUNC3_E2EPW_RC_RD
 * Macro generates redundant channel read wrapper function number 1.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] detypename Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile The E2E Protection profile number. Shall be one of 01,02,03
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Can be on of E2EPW_CX_IR, E2EPW_CX_IA
 */
#define E2EPW_DEFFUNC3_E2EPW_RC_RD(port_dataelement,detypename,e2eprotsn,e2eprofile,detype_comtype)\
   E2EPW_DEFFUNC3_E2EPW_RC_RD_ ## detype_comtype(port_dataelement,detypename,e2eprotsn,e2eprofile)

/** \brief Define E2EPW_DEFFUNC3_E2EPW_RC_RD_E2EPW_CX_IA
 * Macro generates redundant channel read wrapper function number 1 for intra-Ecu communication
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] detypename Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile The E2E Protection profile number. Shall be one of 01,02,03
 */
#define E2EPW_DEFFUNC3_E2EPW_RC_RD_E2EPW_CX_IA(port_dataelement,detypename,e2eprotsn,e2eprofile)\
   FUNC(uint32,E2EPW_CODE) E2EPW_Read1_ ## port_dataelement                               \
   (                                                                                      \
      P2VAR(detypename, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_Data                            \
   )                                                                                      \
   {                                                                                      \
      detypename SerDataDEData;                                                           \
                                                                                          \
      return E2EPW_GenericRead(                                                           \
            &E2EPW_GenericConfig_R1_ ## port_dataelement,                                 \
            E2EPW_Data,                                                                   \
            (P2VAR(void, AUTOMATIC, E2EPW_VAR)) &SerDataDEData                            \
         );                                                                               \
   }

/** \brief Define E2EPW_DEFFUNC3_E2EPW_RC_RD_E2EPW_CX_IR
 * Macro generates redundant channel read wrapper function number 1 for inter-Ecu communication
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] detypename Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile The E2E Protection profile number. Shall be one of 01,02,03
 */
#define E2EPW_DEFFUNC3_E2EPW_RC_RD_E2EPW_CX_IR(port_dataelement,detypename,e2eprotsn,e2eprofile)\
   FUNC(uint32,E2EPW_CODE) E2EPW_Read1_ ## port_dataelement                               \
   (                                                                                      \
      P2VAR(detypename, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_Data                            \
   )                                                                                      \
   {                                                                                      \
      uint8 SerDataDEData[(E2EPW_CFG_DL_ ## e2eprotsn)/8U];                               \
                                                                                          \
      return E2EPW_GenericRead(                                                           \
            &E2EPW_GenericConfig_R1_ ## port_dataelement,                                 \
            E2EPW_Data,                                                                   \
            (P2VAR(void, AUTOMATIC, E2EPW_VAR)) SerDataDEData                             \
         );                                                                               \
   }

/* ==================== E2EPW_DEFFUNC4 code template macros ======================= */

#if (defined E2EPW_DEFFUNC4_E2EPW_SC_RD) /* To prevent double declaration */
#error E2EPW_DEFFUNC4_E2EPW_SC_RD already defined
#endif /* if (defined E2EPW_DEFFUNC4_E2EPW_SC_RD) */
#if (defined E2EPW_DEFFUNC4_E2EPW_SC_RD_E2EPW_CX_IA) /* To prevent double declaration */
#error E2EPW_DEFFUNC4_E2EPW_SC_RD_E2EPW_CX_IA already defined
#endif /* if (defined E2EPW_DEFFUNC4_E2EPW_SC_RD_E2EPW_CX_IA) */
#if (defined E2EPW_DEFFUNC4_E2EPW_SC_RD_E2EPW_CX_IR) /* To prevent double declaration */
#error E2EPW_DEFFUNC4_E2EPW_SC_RD_E2EPW_CX_IR already defined
#endif /* if (defined E2EPW_DEFFUNC4_E2EPW_SC_RD_E2EPW_CX_IR) */
#if (defined E2EPW_DEFFUNC4_E2EPW_SC_WR) /* To prevent double declaration */
#error E2EPW_DEFFUNC4_E2EPW_SC_WR already defined
#endif /* if (defined E2EPW_DEFFUNC4_E2EPW_SC_WR) */
#if (defined E2EPW_DEFFUNC4_E2EPW_SC_WR_E2EPW_CX_IA) /* To prevent double declaration */
#error E2EPW_DEFFUNC4_E2EPW_SC_WR_E2EPW_CX_IA already defined
#endif /* if (defined E2EPW_DEFFUNC4_E2EPW_SC_WR_E2EPW_CX_IA) */
#if (defined E2EPW_DEFFUNC4_E2EPW_SC_WR_E2EPW_CX_IR) /* To prevent double declaration */
#error E2EPW_DEFFUNC4_E2EPW_SC_WR_E2EPW_CX_IR already defined
#endif /* if (defined E2EPW_DEFFUNC4_E2EPW_SC_WR_E2EPW_CX_IR) */
#if (defined E2EPW_DEFFUNC4_E2EPW_RC_WR) /* To prevent double declaration */
#error E2EPW_DEFFUNC4_E2EPW_RC_WR already defined
#endif /* if (defined E2EPW_DEFFUNC4_E2EPW_RC_WR) */
#if (defined E2EPW_DEFFUNC4_E2EPW_RC_WR_E2EPW_CX_IA) /* To prevent double declaration */
#error E2EPW_DEFFUNC4_E2EPW_RC_WR_E2EPW_CX_IA already defined
#endif /* if (defined E2EPW_DEFFUNC4_E2EPW_RC_WR_E2EPW_CX_IA) */
#if (defined E2EPW_DEFFUNC4_E2EPW_RC_WR_E2EPW_CX_IR) /* To prevent double declaration */
#error E2EPW_DEFFUNC4_E2EPW_RC_WR_E2EPW_CX_IR already defined
#endif /* if (defined E2EPW_DEFFUNC4_E2EPW_RC_WR_E2EPW_CX_IR) */
#if (defined E2EPW_DEFFUNC4_E2EPW_RC_RD) /* To prevent double declaration */
#error E2EPW_DEFFUNC4_E2EPW_RC_RD already defined
#endif /* if (defined E2EPW_DEFFUNC4_E2EPW_RC_RD) */
#if (defined E2EPW_DEFFUNC4_E2EPW_RC_RD_E2EPW_CX_IA) /* To prevent double declaration */
#error E2EPW_DEFFUNC4_E2EPW_RC_RD_E2EPW_CX_IA already defined
#endif /* if (defined E2EPW_DEFFUNC4_E2EPW_RC_RD_E2EPW_CX_IA) */
#if (defined E2EPW_DEFFUNC4_E2EPW_RC_RD_E2EPW_CX_IR) /* To prevent double declaration */
#error E2EPW_DEFFUNC4_E2EPW_RC_RD_E2EPW_CX_IR already defined
#endif /* if (defined E2EPW_DEFFUNC4_E2EPW_RC_RD_E2EPW_CX_IR) */

/** \brief Define E2EPW_DEFFUNC4_E2EPW_SC_RD
 * This macro is empty for single channel read wrapper function.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Can be on of E2EPW_CX_IR, E2EPW_CX_IA
 * \param[in] e2eprofile The E2E Protection profile number. Shall be one of 01,02,03
 */
#define E2EPW_DEFFUNC4_E2EPW_SC_RD(port_dataelement,applDataType,e2eprotsn,detype_comtype,e2eprofile)

/** \brief Define E2EPW_DEFFUNC4_E2EPW_SC_RD
 * This macro is empty for single channel write wrapper function.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] applDataType Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Can be on of E2EPW_CX_IR, E2EPW_CX_IA
 * \param[in] e2eprofile The E2E Protection profile number. Shall be one of 01,02,03
 */
#define E2EPW_DEFFUNC4_E2EPW_SC_WR(port_dataelement,applDataType,e2eprotsn,detype_comtype,e2eprofile)

/** \brief Define E2EPW_DEFFUNC4_E2EPW_RC_WR
 * Macro generates redundant channel write wrapper function number 2.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] detypename Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile The E2E Protection profile number. Shall be one of 01,02,03
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Can be on of E2EPW_CX_IR, E2EPW_CX_IA
 */
#define E2EPW_DEFFUNC4_E2EPW_RC_WR(port_dataelement,detypename,e2eprotsn,e2eprofile,detype_comtype)\
    E2EPW_DEFFUNC4_E2EPW_RC_WR_ ## detype_comtype(port_dataelement,detypename,e2eprotsn,e2eprofile)

/** \brief Define E2EPW_DEFFUNC4_E2EPW_RC_WR_E2EPW_CX_IA
 * Macro generates redundant channel write wrapper function number 2 for intra-ECU communication
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] detypename Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile The E2E Protection profile number. Shall be one of 01,02,03
 */
#if (E2EPW_ENABLE_TX_SIGNAL_GROUP_ARRAY_ACCESS == STD_ON)
#define E2EPW_DEFFUNC4_E2EPW_RC_WR_E2EPW_CX_IA(port_dataelement,detypename,e2eprotsn,e2eprofile)\
   FUNC(uint32,E2EPW_CODE) E2EPW_Write2_ ## port_dataelement                              \
   (                                                                                      \
      P2VAR(detypename, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_Data                            \
   )                                                                                      \
   {                                                                                      \
      detypename SerDataDEData;                                                           \
                                                                                          \
      return E2EPW_GenericWrite2(                                                         \
            &E2EPW_GenericConfig_W2_ ## port_dataelement,                                 \
            E2EPW_Data,                                                                   \
            (P2VAR(void, AUTOMATIC, E2EPW_VAR)) &SerDataDEData,                           \
            0U                                                                            \
         );                                                                               \
   }
#else
#define E2EPW_DEFFUNC4_E2EPW_RC_WR_E2EPW_CX_IA(port_dataelement,detypename,e2eprotsn,e2eprofile)\
   FUNC(uint32,E2EPW_CODE) E2EPW_Write2_ ## port_dataelement                              \
   (                                                                                      \
      P2VAR(detypename, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_Data                            \
   )                                                                                      \
   {                                                                                      \
      detypename SerDataDEData;                                                           \
                                                                                          \
      return E2EPW_GenericWrite2(                                                         \
            &E2EPW_GenericConfig_W2_ ## port_dataelement,                                 \
            E2EPW_Data,                                                                   \
            (P2VAR(void, AUTOMATIC, E2EPW_VAR)) &SerDataDEData                            \
         );                                                                               \
   }
#endif

/** \brief Define E2EPW_DEFFUNC4_E2EPW_RC_WR_E2EPW_CX_IA
 * Macro generates redundant channel write wrapper function number 2 for inter-ECU communication
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] detypename Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile The E2E Protection profile number. Shall be one of 01,02,03
 */
#if (E2EPW_ENABLE_TX_SIGNAL_GROUP_ARRAY_ACCESS == STD_ON)
#define E2EPW_DEFFUNC4_E2EPW_RC_WR_E2EPW_CX_IR(port_dataelement,detypename,e2eprotsn,e2eprofile)\
   FUNC(uint32,E2EPW_CODE) E2EPW_Write2_ ## port_dataelement                              \
   (                                                                                      \
      P2VAR(detypename, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_Data                            \
   )                                                                                      \
   {                                                                                      \
      uint8 SerDataDEData[(E2EPW_CFG_DL_ ## e2eprotsn)/8U];                               \
                                                                                          \
      return E2EPW_GenericWrite2(                                                         \
            &E2EPW_GenericConfig_W2_ ## port_dataelement,                                 \
            E2EPW_Data,                                                                   \
            (P2VAR(void, AUTOMATIC, E2EPW_VAR)) SerDataDEData,                            \
            (uint16)sizeof(SerDataDEData)                                                 \
         );                                                                               \
   }
#else
#define E2EPW_DEFFUNC4_E2EPW_RC_WR_E2EPW_CX_IR(port_dataelement,detypename,e2eprotsn,e2eprofile)\
   FUNC(uint32,E2EPW_CODE) E2EPW_Write2_ ## port_dataelement                              \
   (                                                                                      \
      P2VAR(detypename, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_Data                            \
   )                                                                                      \
   {                                                                                      \
      uint8 SerDataDEData[(E2EPW_CFG_DL_ ## e2eprotsn)/8U];                               \
                                                                                          \
      return E2EPW_GenericWrite2(                                                         \
            &E2EPW_GenericConfig_W2_ ## port_dataelement,                                 \
            E2EPW_Data,                                                                   \
            (P2VAR(void, AUTOMATIC, E2EPW_VAR)) SerDataDEData                             \
         );                                                                               \
   }
#endif

/** \brief Define E2EPW_DEFFUNC4_E2EPW_RC_RD
 * Macro generates redundant channel read wrapper function number 2.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] detypename Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile The E2E Protection profile number. Shall be one of 01,02,03
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Can be on of E2EPW_CX_IR, E2EPW_CX_IA
 */
#define E2EPW_DEFFUNC4_E2EPW_RC_RD(port_dataelement,detypename,e2eprotsn,e2eprofile,detype_comtype)\
   E2EPW_DEFFUNC4_E2EPW_RC_RD_ ## detype_comtype(port_dataelement,detypename,e2eprotsn,e2eprofile)\

/** \brief Define E2EPW_DEFFUNC4_E2EPW_RC_RD_E2EPW_CX_IA
 * Macro generates redundant channel read wrapper function number 2 for intra-ECU communication
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] detypename Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile The E2E Protection profile number. Shall be one of 01,02,03
 */
#define E2EPW_DEFFUNC4_E2EPW_RC_RD_E2EPW_CX_IA(port_dataelement,detypename,e2eprotsn,e2eprofile)\
   FUNC(uint32,E2EPW_CODE) E2EPW_Read2_ ## port_dataelement                               \
   (                                                                                      \
      P2VAR(detypename, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_Data                            \
   )                                                                                      \
   {                                                                                      \
      detypename SerDataDEData;                                                           \
                                                                                          \
      ((E2EPW_R2_St_ ## port_dataelement).NewDataAvailable)=TRUE;                         \
      return E2EPW_GenericRead2(                                                          \
            &E2EPW_GenericConfig_R2_ ## port_dataelement,                                 \
            E2EPW_Data,                                                                   \
            (P2VAR(void, AUTOMATIC, E2EPW_VAR)) &SerDataDEData                            \
         );                                                                               \
   }

/** \brief Define E2EPW_DEFFUNC4_E2EPW_RC_RD_E2EPW_CX_IR
 * Macro generates redundant channel read wrapper function number 2 for inter-ECU communication
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] detypename Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile The E2E Protection profile number. Shall be one of 01,02,03
 */
#define E2EPW_DEFFUNC4_E2EPW_RC_RD_E2EPW_CX_IR(port_dataelement,detypename,e2eprotsn,e2eprofile)\
   FUNC(uint32,E2EPW_CODE) E2EPW_Read2_ ## port_dataelement                               \
   (                                                                                      \
      P2VAR(detypename, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_Data                            \
   )                                                                                      \
   {                                                                                      \
      uint8 SerDataDEData[(E2EPW_CFG_DL_ ## e2eprotsn)/8U];                               \
                                                                                          \
      ((E2EPW_R2_St_ ## port_dataelement).NewDataAvailable)=TRUE;                         \
      return E2EPW_GenericRead2(                                                          \
            &E2EPW_GenericConfig_R2_ ## port_dataelement,                                 \
            E2EPW_Data,                                                                   \
            (P2VAR(void, AUTOMATIC, E2EPW_VAR)) SerDataDEData                             \
         );                                                                               \
   }

/* ===================== E2EPW_DEFFUNC5 code template macros ========================== */

#if (defined E2EPW_DEFFUNC5_E2EPW_SC_WR) /* To prevent double declaration */
#error E2EPW_DEFFUNC5_E2EPW_SC_WR already defined
#endif /* if (defined E2EPW_DEFFUNC5_E2EPW_SC_WR) */
#if (defined E2EPW_DEFFUNC5_E2EPW_SC_RD) /* To prevent double declaration */
#error E2EPW_DEFFUNC5_E2EPW_SC_RD already defined
#endif /* if (defined E2EPW_DEFFUNC5_E2EPW_SC_RD) */
#if (defined E2EPW_DEFFUNC5_E2EPW_RC_WR) /* To prevent double declaration */
#error E2EPW_DEFFUNC5_E2EPW_RC_WR already defined
#endif /* if (defined E2EPW_DEFFUNC5_E2EPW_RC_WR) */
#if (defined E2EPW_DEFFUNC5_E2EPW_RC_RD) /* To prevent double declaration */
#error E2EPW_DEFFUNC5_E2EPW_RC_RD already defined
#endif /* if (defined E2EPW_DEFFUNC5_E2EPW_RC_RD) */

/** \brief Define E2EPW_DEFFUNC5_E2EPW_SC_WR
 * Macro generates single channel write initialization function.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] detypename Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile The E2E Protection profile number. Shall be one of 01,02,03
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Can be on of E2EPW_CX_IR, E2EPW_CX_IA
 * \return E2E_E_OK
 */
#define E2EPW_DEFFUNC5_E2EPW_SC_WR(port_dataelement,detypename,e2eprotsn,e2eprofile,detype_comtype)\
   FUNC(Std_ReturnType,E2EPW_CODE) E2EPW_WriteInit_ ## port_dataelement                   \
   (                                                                                      \
      void                                                                                \
   )                                                                                      \
   {                                                                                      \
      E2EPW_Wr_St_ ## port_dataelement.Counter = 0U;                                      \
                                                                                          \
      return E2E_E_OK;                                                                    \
   }

/** \brief Define E2EPW_DEFFUNC5_E2EPW_RC_WR
 * Macro generates redundant channel 1 write initialization function.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] detypename Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile The E2E Protection profile number. Shall be one of 01,02,03
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Can be on of E2EPW_CX_IR, E2EPW_CX_IA
 * \return E2E_E_OK
 */
#define E2EPW_DEFFUNC5_E2EPW_RC_WR(port_dataelement,detypename,e2eprotsn,e2eprofile,detype_comtype)\
   FUNC(Std_ReturnType,E2EPW_CODE) E2EPW_WriteInit1_ ## port_dataelement                  \
   (                                                                                      \
      void                                                                                \
   )                                                                                      \
   {                                                                                      \
      E2EPW_W1_St_ ## port_dataelement.Counter = 0U;                                      \
                                                                                          \
      return E2E_E_OK;                                                                    \
   }

/** \brief Define E2EPW_DEFFUNC5_E2EPW_SC_RD
 * Macro generates single channel read initialization function.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] detypename Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile The E2E Protection profile number. Shall be one of 01,02,03
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Can be on of E2EPW_CX_IR, E2EPW_CX_IA
 * \return E2E_E_OK
 */
#define E2EPW_DEFFUNC5_E2EPW_SC_RD(port_dataelement,detypename,e2eprotsn,e2eprofile,detype_comtype)\
   FUNC(Std_ReturnType,E2EPW_CODE) E2EPW_ReadInit_ ## port_dataelement                    \
   (                                                                                      \
      void                                                                                \
   )                                                                                      \
   {                                                                                      \
      E2EPW_Rd_St_ ## port_dataelement.WaitForFirstData = TRUE;                           \
      E2EPW_Rd_St_ ## port_dataelement.SyncCounter = 0U;                                  \
      E2EPW_Rd_St_ ## port_dataelement.NoNewOrRepeatedDataCounter = 0U;                   \
                                                                                          \
      return E2E_E_OK;                                                                    \
   }

/** \brief Define E2EPW_DEFFUNC5_E2EPW_RC_RD
 * Macro generates redundant channel 1 read initialization function.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] detypename Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile The E2E Protection profile number. Shall be one of 01,02,03
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Can be on of E2EPW_CX_IR, E2EPW_CX_IA
 * \return E2E_E_OK
 */
#define E2EPW_DEFFUNC5_E2EPW_RC_RD(port_dataelement,detypename,e2eprotsn,e2eprofile,detype_comtype)\
   FUNC(Std_ReturnType,E2EPW_CODE) E2EPW_ReadInit1_ ## port_dataelement                   \
   (                                                                                      \
      void                                                                                \
   )                                                                                      \
   {                                                                                      \
      E2EPW_R1_St_ ## port_dataelement.WaitForFirstData = TRUE;                           \
      E2EPW_R1_St_ ## port_dataelement.SyncCounter = 0U;                                  \
      E2EPW_R1_St_ ## port_dataelement.NoNewOrRepeatedDataCounter = 0U;                   \
                                                                                          \
      return E2E_E_OK;                                                                    \
   }

/* ========================= E2EPW_DEFFUNC6 code template macros ====================== */

#if (defined E2EPW_DEFFUNC6_E2EPW_SC_WR) /* To prevent double declaration */
#error E2EPW_DEFFUNC6_E2EPW_SC_WR already defined
#endif /* if (defined E2EPW_DEFFUNC6_E2EPW_SC_WR) */
#if (defined E2EPW_DEFFUNC6_E2EPW_SC_RD) /* To prevent double declaration */
#error E2EPW_DEFFUNC6_E2EPW_SC_RD already defined
#endif /* if (defined E2EPW_DEFFUNC6_E2EPW_SC_RD) */
#if (defined E2EPW_DEFFUNC6_E2EPW_RC_WR) /* To prevent double declaration */
#error E2EPW_DEFFUNC6_E2EPW_RC_WR already defined
#endif /* if (defined E2EPW_DEFFUNC6_E2EPW_RC_WR) */
#if (defined E2EPW_DEFFUNC6_E2EPW_RC_RD) /* To prevent double declaration */
#error E2EPW_DEFFUNC6_E2EPW_RC_RD already defined
#endif /* if (defined E2EPW_DEFFUNC6_E2EPW_RC_RD) */

/** \brief Define E2EPW_DEFFUNC6_E2EPW_SC_WR
 * This macro is empty for single channel write wrapper function.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] detypename Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile The E2E Protection profile number. Shall be one of 01,02,03
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Can be on of E2EPW_CX_IR, E2EPW_CX_IA
 * \return E2E_E_OK
 */
#define E2EPW_DEFFUNC6_E2EPW_SC_WR(port_dataelement,detypename,e2eprotsn,e2eprofile,detype_comtype)


/** \brief Define E2EPW_DEFFUNC6_E2EPW_RC_WR
 * Macro generates redundant channel 2 write initialization function.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] detypename Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile The E2E Protection profile number. Shall be one of 01,02,03
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Can be on of E2EPW_CX_IR, E2EPW_CX_IA
 * \return E2E_E_OK
 */
#define E2EPW_DEFFUNC6_E2EPW_RC_WR(port_dataelement,detypename,e2eprotsn,e2eprofile,detype_comtype)\
   FUNC(Std_ReturnType,E2EPW_CODE) E2EPW_WriteInit2_ ## port_dataelement                  \
   (                                                                                      \
      void                                                                                \
   )                                                                                      \
   {                                                                                      \
      E2EPW_W2_St_ ## port_dataelement.Counter = 0U;                                      \
                                                                                          \
      return E2E_E_OK;                                                                    \
   }

/** \brief Define E2EPW_DEFFUNC6_E2EPW_SC_RD
 * This macro is empty for single channel read wrapper function.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] detypename Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile The E2E Protection profile number. Shall be one of 01,02,03
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Can be on of E2EPW_CX_IR, E2EPW_CX_IA
 * \return E2E_E_OK
 */
#define E2EPW_DEFFUNC6_E2EPW_SC_RD(port_dataelement,detypename,e2eprotsn,e2eprofile,detype_comtype)

/** \brief Define E2EPW_DEFFUNC6_E2EPW_RC_RD
 * Macro generates redundant channel 2 read initialization function.
 *
 * \param[in] port_dataelement Concatenation of port shortname and data element shortname with
 *            an underscore in between.
 * \param[in] detypename Data type name of the application data element
 * \param[in] e2eprotsn Shortname of the E2E protection
 * \param[in] e2eprofile The E2E Protection profile number. Shall be one of 01,02,03
 * \param[in] detype_comtype Concatenation of application data element (Complex)
 *            and Ecu communication type (Inter or intra Ecu communication)
 *            Can be on of E2EPW_CX_IR, E2EPW_CX_IA
 * \return E2E_E_OK
 */
#define E2EPW_DEFFUNC6_E2EPW_RC_RD(port_dataelement,detypename,e2eprotsn,e2eprofile,detype_comtype)\
   FUNC(Std_ReturnType,E2EPW_CODE) E2EPW_ReadInit2_ ## port_dataelement                   \
   (                                                                                      \
      void                                                                                \
   )                                                                                      \
   {                                                                                      \
      E2EPW_R2_St_ ## port_dataelement.WaitForFirstData = TRUE;                           \
      E2EPW_R2_St_ ## port_dataelement.SyncCounter = 0U;                                  \
      E2EPW_R2_St_ ## port_dataelement.NoNewOrRepeatedDataCounter = 0U;                   \
                                                                                          \
      return E2E_E_OK;                                                                    \
   }

/* ============================ macro argument checks for macro concatenation =========== */

#if (defined SenderStateType) /* Ensure that string can be used for macro concatenation */
#error SenderStateType already defined
#endif /* if (defined SenderStateType) */

#if (defined ReceiverStateType) /* Ensure that string can be used for macro concatenation */
#error ReceiverStateType already defined
#endif /* if (defined ReceiverStateType) */

#if (defined STATUS_NONEW_DATA) /* Ensure that string can be used for macro concatenation */
#error STATUS_NONEW_DATA already defined
#endif /* if (defined STATUS_NONEW_DATA) */

#if (defined ConfigType) /* Ensure that string can be used for macro concatenation */
#error ConfigType already defined
#endif /* if (defined ConfigType) */



#if (defined E2EPW_RteRd_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_RteRd_ already defined
#endif /* if (defined E2EPW_RteRd_) */

#if (defined E2EPW_RteWr_) /* Ensure that string can be used for macro concatenation */
#error Rte_Write_ already defined
#endif /* if (defined Rte_Write_) */



#if (defined Rte_Read_) /* Ensure that string can be used for macro concatenation */
#error Rte_Read_ already defined
#endif /* if (defined Rte_Read_) */

#if (defined Rte_Write_) /* Ensure that string can be used for macro concatenation */
#error Rte_Write_ already defined
#endif /* if (defined Rte_Write_) */

#if (defined Rte_WriteArray_) /* Ensure that string can be used for macro concatenation */
#error Rte_WriteArray_ already defined
#endif /* if (defined Rte_WriteArray_) */



#if (defined E2EPW_Read_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_Read_ already defined
#endif /* if (defined E2EPW_Read_) */

#if (defined E2EPW_Write_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_Write_ already defined
#endif /* if (defined E2EPW_Write_) */

#if (defined E2EPW_Read1_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_Read1_ already defined
#endif /* if (defined E2EPW_Read1_) */

#if (defined E2EPW_Write1_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_Write1_ already defined
#endif /* if (defined E2EPW_Write1_) */

#if (defined E2EPW_Read2_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_Read2_ already defined
#endif /* if (defined E2EPW_Read2_) */

#if (defined E2EPW_Write2_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_Write2_ already defined
#endif /* if (defined E2EPW_Write2_) */



#if (defined E2E_P) /* Ensure that string can be used for macro concatenation */
#error E2E_P already defined
#endif /* if (defined E2E_P) */

#if (defined E2EPW_GET_CFG_P) /* Ensure that string can be used for macro concatenation */
#error E2EPW_GET_CFG_P already defined
#endif /* if (defined E2EPW_GET_CFG_P) */

#if (defined E2E_P01) /* Ensure that string can be used for macro concatenation */
#error E2E_P01 already defined
#endif /* if (defined E2E_P01) */

#if (defined E2E_P02) /* Ensure that string can be used for macro concatenation */
#error E2E_P02 already defined
#endif /* if (defined E2E_P02) */

#if (defined E2E_PDAI1) /* Ensure that string can be used for macro concatenation */
#error E2E_PDAI1 already defined
#endif /* if (defined E2E_PDAI1) */

#if (defined E2E_PDAI2) /* Ensure that string can be used for macro concatenation */
#error E2E_PDAI2 already defined
#endif /* if (defined E2E_PDAI2) */



#if (defined E2EPW_CFG_PF_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_CFG_PF_ already defined
#endif /* if (defined E2EPW_CFG_PF_) */

#if (defined E2EPW_CFG_DL_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_CFG_DL_ already defined
#endif /* if (defined E2EPW_CFG_DL_) */

#if (defined E2EPW_CFG_DI_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_CFG_DI_ already defined
#endif /* if (defined E2EPW_CFG_DI_) */

#if (defined E2EPW_CFG_CI_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_CFG_CI_ already defined
#endif /* if (defined E2EPW_CFG_CI_) */

#if (defined E2EPW_CFG_CC_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_CFG_CC_ already defined
#endif /* if (defined E2EPW_CFG_CC_) */

#if (defined E2EPW_CFG_CT_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_CFG_CT_ already defined
#endif /* if (defined E2EPW_CFG_CT_) */

#if (defined E2EPW_CFG_LD_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_CFG_LD_ already defined
#endif /* if (defined E2EPW_CFG_LD_) */

#if (defined E2EPW_CFG_SC_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_CFG_SC_ already defined
#endif /* if (defined E2EPW_CFG_SC_) */

#if (defined E2EPW_CFG_NO_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_CFG_NO_ already defined
#endif /* if (defined E2EPW_CFG_NO_) */

#if (defined E2EPW_CFG_DN_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_CFG_DN_ already defined
#endif /* if (defined E2EPW_CFG_DN_) */



#if (defined E2EPW_Rd_St_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_Rd_St_ already defined
#endif /* if (defined E2EPW_Rd_St_) */

#if (defined E2EPW_Wr_St_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_Wr_St_ already defined
#endif /* if (defined E2EPW_Wr_St_) */

#if (defined E2EPW_R1_St_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_R1_St_ already defined
#endif /* if (defined E2EPW_R1_St_) */

#if (defined E2EPW_R2_St_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_R2_St_ already defined
#endif /* if (defined E2EPW_R2_St_) */

#if (defined E2EPW_W1_St_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_W1_St_ already defined
#endif /* if (defined E2EPW_W1_St_) */

#if (defined E2EPW_W2_St_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_W2_St_ already defined
#endif /* if (defined E2EPW_W2_St_) */



#if (defined E2EPW_Rd_Cf_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_Rd_Cf_ already defined
#endif /* if (defined E2EPW_Rd_Cf_) */

#if (defined E2EPW_Wr_Cf_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_Wr_Cf_ already defined
#endif /* if (defined E2EPW_Wr_Cf_) */

#if (defined E2EPW_R1_Cf_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_R1_Cf_ already defined
#endif /* if (defined E2EPW_R1_Cf_) */

#if (defined E2EPW_R2_Cf_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_R2_Cf_ already defined
#endif /* if (defined E2EPW_R2_Cf_) */

#if (defined E2EPW_W1_Cf_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_W1_Cf_ already defined
#endif /* if (defined E2EPW_W1_Cf_) */

#if (defined E2EPW_W2_Cf_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_W2_Cf_ already defined
#endif /* if (defined E2EPW_W2_Cf_) */



#if (defined E2EPW_GenericConfig_Rd_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_GenericConfig_Rd_ already defined
#endif /* if (defined E2EPW_GenericConfig_Rd_) */

#if (defined E2EPW_GenericConfig_Wr_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_GenericConfig_Wr_ already defined
#endif /* if (defined E2EPW_GenericConfig_Wr_) */

#if (defined E2EPW_GenericConfig_R1_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_GenericConfig_R1_ already defined
#endif /* if (defined E2EPW_GenericConfig_R1_) */

#if (defined E2EPW_GenericConfig_R2_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_GenericConfig_R2_ already defined
#endif /* if (defined E2EPW_GenericConfig_R2_) */

#if (defined E2EPW_GenericConfig_W1_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_GenericConfig_W1_ already defined
#endif /* if (defined E2EPW_GenericConfig_W1_) */

#if (defined E2EPW_GenericConfig_W2_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_GenericConfig_W2_ already defined
#endif /* if (defined E2EPW_GenericConfig_W2_) */



#if (defined E2EPW_SC_RD) /* Ensure that string can be used for macro concatenation */
#error E2EPW_SC_RD already defined
#endif /* if (defined E2EPW_SC_RD) */

#if (defined E2EPW_SC_WR) /* Ensure that string can be used for macro concatenation */
#error E2EPW_SC_WR already defined
#endif /* if (defined E2EPW_SC_WR) */

#if (defined E2EPW_RC_RD) /* Ensure that string can be used for macro concatenation */
#error E2EPW_RC_RD already defined
#endif /* if (defined E2EPW_RC_RD) */

#if (defined E2EPW_RC_WR) /* Ensure that string can be used for macro concatenation */
#error E2EPW_RC_WR already defined
#endif /* if (defined E2EPW_RC_WR) */



#if (defined E2EPW_CX_IR) /* Ensure that string can be used for macro concatenation */
#error E2EPW_CX_IR already defined
#endif /* if (defined E2EPW_CX_IR) */

#if (defined E2EPW_CX_IA) /* Ensure that string can be used for macro concatenation */
#error E2EPW_CX_IA already defined
#endif /* if (defined E2EPW_CX_IA) */


#if (defined E2EPW_LE_BL) /* Ensure that string can be used for macro concatenation */
#error E2EPW_LE_BL already defined
#endif /* if (defined E2EPW_LE_BL) */

#if (defined E2EPW_LE_U8) /* Ensure that string can be used for macro concatenation */
#error E2EPW_LE_U8 already defined
#endif /* if (defined E2EPW_LE_U8) */

#if (defined E2EPW_LE_U16) /* Ensure that string can be used for macro concatenation */
#error E2EPW_LE_U16 already defined
#endif /* if (defined E2EPW_LE_U16) */

#if (defined E2EPW_LE_U32) /* Ensure that string can be used for macro concatenation */
#error E2EPW_LE_U32 already defined
#endif /* if (defined E2EPW_LE_U32) */

#if (defined E2EPW_LE_S8) /* Ensure that string can be used for macro concatenation */
#error E2EPW_LE_S8 already defined
#endif /* if (defined E2EPW_LE_S8) */

#if (defined E2EPW_LE_S16) /* Ensure that string can be used for macro concatenation */
#error E2EPW_LE_S16 already defined
#endif /* if (defined E2EPW_LE_S16) */

#if (defined E2EPW_LE_S32) /* Ensure that string can be used for macro concatenation */
#error E2EPW_LE_S32 already defined
#endif /* if (defined E2EPW_LE_S32) */


#if (defined E2EPW_BE_BL) /* Ensure that string can be used for macro concatenation */
#error E2EPW_BE_BL already defined
#endif /* if (defined E2EPW_BE_BL) */

#if (defined E2EPW_BE_U8) /* Ensure that string can be used for macro concatenation */
#error E2EPW_BE_U8 already defined
#endif /* if (defined E2EPW_BE_U8) */

#if (defined E2EPW_BE_U16) /* Ensure that string can be used for macro concatenation */
#error E2EPW_BE_U16 already defined
#endif /* if (defined E2EPW_BE_U16) */

#if (defined E2EPW_BE_U32) /* Ensure that string can be used for macro concatenation */
#error E2EPW_BE_U32 already defined
#endif /* if (defined E2EPW_BE_U32) */

#if (defined E2EPW_BE_S8) /* Ensure that string can be used for macro concatenation */
#error E2EPW_BE_S8 already defined
#endif /* if (defined E2EPW_BE_S8) */

#if (defined E2EPW_BE_S16) /* Ensure that string can be used for macro concatenation */
#error E2EPW_BE_S16 already defined
#endif /* if (defined E2EPW_BE_S16) */

#if (defined E2EPW_BE_S32) /* Ensure that string can be used for macro concatenation */
#error E2EPW_BE_S32 already defined
#endif /* if (defined E2EPW_BE_S32) */

#if (defined E2EPW_OP_U8N) /* Ensure that string can be used for macro concatenation */
#error E2EPW_OP_U8N already defined
#endif /* if (defined E2EPW_OP_U8N) */



#if (defined _ISCOMPLEXANDINTRA) /* Ensure that string can be used for macro concatenation */
#error _ISCOMPLEXANDINTRA already defined
#endif /* if (defined _ISCOMPLEXANDINTRA) */

#if (defined _ISCOMPLEXANDINTER) /* Ensure that string can be used for macro concatenation */
#error _ISCOMPLEXANDINTER already defined
#endif /* if (defined _ISCOMPLEXANDINTER) */

#if (defined _ISCOMPLEX) /* Ensure that string can be used for macro concatenation */
#error _ISCOMPLEX already defined
#endif /* if (defined _ISCOMPLEX) */



#if (defined E2EPW_CHECK_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_CHECK_ already defined
#endif /* if (defined E2EPW_CHECK_) */

#if (defined E2EPW_PACK_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_PACK_ already defined
#endif /* if (defined E2EPW_PACK_) */

#if (defined E2EPW_CPDE_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_CPDE_ already defined
#endif /* if (defined E2EPW_CPDE_) */


#if (defined E2EPW_CHECKRANGE_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_CHECKRANGE_ already defined
#endif /* if (defined E2EPW_CHECKRANGE_) */

#if (defined E2EPW_PACKSIG_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_PACKSIG_ already defined
#endif /* if (defined E2EPW_PACKSIG_) */


#if (defined E2EPW_DEFVAR1_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_DEFVAR1_ already defined
#endif /* if (defined E2EPW_DEFVAR1_) */

#if (defined E2EPW_DEFVAR2_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_DEFVAR2_ already defined
#endif /* if (defined E2EPW_DEFVAR2_) */

#if (defined E2EPW_DEFVAR_CONST1_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_DEFVAR_CONST1_ already defined
#endif /* if (defined E2EPW_DEFVAR_CONST1_) */

#if (defined E2EPW_DEFVAR_CONST2_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_DEFVAR_CONST2_ already defined
#endif /* if (defined E2EPW_DEFVAR_CONST2_) */

#if (defined E2EPW_DEFVAR_CONST3_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_DEFVAR_CONST3_ already defined
#endif /* if (defined E2EPW_DEFVAR_CONST3_) */

#if (defined E2EPW_DEFVAR_CONST4_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_DEFVAR_CONST4_ already defined
#endif /* if (defined E2EPW_DEFVAR_CONST4_) */

#if (defined E2EPW_DEFFUNC1_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_DEFFUNC1_ already defined
#endif /* if (defined E2EPW_DEFFUNC1_) */

#if (defined E2EPW_DEFFUNC2_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_DEFFUNC2_ already defined
#endif /* if (defined E2EPW_DEFFUNC2_) */

#if (defined E2EPW_DEFFUNC3_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_DEFFUNC3_ already defined
#endif /* if (defined E2EPW_DEFFUNC3_) */

#if (defined E2EPW_DEFFUNC4_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_DEFFUNC4_ already defined
#endif /* if (defined E2EPW_DEFFUNC4_) */

#if (defined E2EPW_DEFFUNC5_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_DEFFUNC5_ already defined
#endif /* if (defined E2EPW_DEFFUNC5_) */

#if (defined E2EPW_DEFFUNC6_) /* Ensure that string can be used for macro concatenation */
#error E2EPW_DEFFUNC6_ already defined
#endif /* if (defined E2EPW_DEFFUNC6_) */

/* ======================= internal helper macros ================================= */

#if (defined E2EPW_CONCAT2) /* To prevent double declaration */
#error E2EPW_CONCAT2 already defined
#endif /* if (defined E2EPW_CONCAT2) */

/** \brief Helper macro for the concatenation of two arguments with a defined order */
#define E2EPW_CONCAT2(param1,param2) param1 ## param2

#if (defined E2EPW_CONCAT3) /* To prevent double declaration */
#error E2EPW_CONCAT3 already defined
#endif /* if (defined E2EPW_CONCAT3) */

/** \brief Helper macro for the concatenation of three arguments with a defined order */
#define E2EPW_CONCAT3(param1,param2,param3) E2EPW_CONCAT2(param1 ## param2, param3)

#if (defined E2EPW_CX_IA_ISCOMPLEX) /* To prevent double declaration */
#error E2EPW_CX_IA_ISCOMPLEX already defined
#endif /* if (defined E2EPW_CX_IA_ISCOMPLEX) */

#if (defined E2EPW_CX_IR_ISCOMPLEX) /* To prevent double declaration */
#error E2EPW_CX_IR_ISCOMPLEX already defined
#endif /* if (defined E2EPW_CX_IR_ISCOMPLEX) */

/** \brief Helper macro for returning one argument out of two */
#define E2EPW_CX_IA_ISCOMPLEX(arg1,arg2) (arg1)
/** \brief Helper macro for returning one argument out of two */
#define E2EPW_CX_IR_ISCOMPLEX(arg1,arg2) (arg1)

#if (defined E2EPW_CX_IA_ISCOMPLEXANDINTER) /* To prevent double declaration */
#error E2EPW_CX_IA_ISCOMPLEXANDINTER already defined
#endif /* if (defined E2EPW_CX_IA_ISCOMPLEXANDINTER) */

#if (defined E2EPW_CX_IR_ISCOMPLEXANDINTER) /* To prevent double declaration */
#error E2EPW_CX_IR_ISCOMPLEXANDINTER already defined
#endif /* if (defined E2EPW_CX_IR_ISCOMPLEXANDINTER) */

/** \brief Helper macro for returning one argument out of two */
#define E2EPW_CX_IA_ISCOMPLEXANDINTER(arg1,arg2) (arg2)
/** \brief Helper macro for returning one argument out of two */
#define E2EPW_CX_IR_ISCOMPLEXANDINTER(arg1,arg2) (arg1)

#if (defined E2EPW_CX_IA_ISCOMPLEXANDINTRA) /* To prevent double declaration */
#error E2EPW_CX_IA_ISCOMPLEXANDINTRA already defined
#endif /* if (defined E2EPW_CX_IA_ISCOMPLEXANDINTRA) */

#if (defined E2EPW_CX_IR_ISCOMPLEXANDINTRA) /* To prevent double declaration */
#error E2EPW_CX_IR_ISCOMPLEXANDINTRA already defined
#endif /* if (defined E2EPW_CX_IR_ISCOMPLEXANDINTRA) */

/** \brief Helper macro for returning one argument out of two */
#define E2EPW_CX_IA_ISCOMPLEXANDINTRA(arg1,arg2) (arg1)
/** \brief Helper macro for returning one argument out of two */
#define E2EPW_CX_IR_ISCOMPLEXANDINTRA(arg1,arg2) (arg2)

/* ====================== helper macros copying data element members ================ */

#if (defined E2EPW_CPYMEMB) /* Ensure that string can be used for macro concatenation */
#error E2EPW_CPYMEMB already defined
#endif /* if (defined E2EPW_CPYMEMB) */

/** \brief Macro for copying a Data Element member
 *
 * \param[in] srcDePtr Pointer to source Data Element
 * \param[out] dstDePtr Pointer to destination Data Element
 * \param[in] demember Member of the Data Element which shall be copied
 */
#define E2EPW_CPYMEMB(srcDePtr,dstDePtr,demember) \
   do { \
     (dstDePtr)->demember = (srcDePtr)->demember; \
   } while (0)

/* ========================= helper macros for packing signals into memory ============= */

#if (defined E2EPW_PACKSIG) /* To prevent double declaration */
#error E2EPW_PACKSIG already defined
#endif /* if (defined E2EPW_PACKSIG) */
#if (defined E2EPW_PACKSIG_E2EPW_LE_BL) /* To prevent double declaration */
#error E2EPW_PACKSIG_E2EPW_LE_BL already defined
#endif /* if (defined E2EPW_PACKSIG_E2EPW_LE_BL) */
#if (defined E2EPW_PACKSIG_E2EPW_LE_U8) /* To prevent double declaration */
#error E2EPW_PACKSIG_E2EPW_LE_U8 already defined
#endif /* if (defined E2EPW_PACKSIG_E2EPW_LE_U8) */
#if (defined E2EPW_PACKSIG_E2EPW_LE_U16) /* To prevent double declaration */
#error E2EPW_PACKSIG_E2EPW_LE_U16 already defined
#endif /* if (defined E2EPW_PACKSIG_E2EPW_LE_U16) */
#if (defined E2EPW_PACKSIG_E2EPW_LE_U32) /* To prevent double declaration */
#error E2EPW_PACKSIG_E2EPW_LE_U32 already defined
#endif /* if (defined E2EPW_PACKSIG_E2EPW_LE_U32) */
#if (defined E2EPW_PACKSIG_E2EPW_LE_S8) /* To prevent double declaration */
#error E2EPW_PACKSIG_E2EPW_LE_S8 already defined
#endif /* if (defined E2EPW_PACKSIG_E2EPW_LE_S8) */
#if (defined E2EPW_PACKSIG_E2EPW_LE_S16) /* To prevent double declaration */
#error E2EPW_PACKSIG_E2EPW_LE_S16 already defined
#endif /* if (defined E2EPW_PACKSIG_E2EPW_LE_S16) */
#if (defined E2EPW_PACKSIG_E2EPW_LE_S32) /* To prevent double declaration */
#error E2EPW_PACKSIG_E2EPW_LE_S32 already defined
#endif /* if (defined E2EPW_PACKSIG_E2EPW_LE_S32) */
#if (defined E2EPW_PACKSIG_E2EPW_BE_BL) /* To prevent double declaration */
#error E2EPW_PACKSIG_E2EPW_BE_BL already defined
#endif /* if (defined E2EPW_PACKSIG_E2EPW_BE_BL) */
#if (defined E2EPW_PACKSIG_E2EPW_BE_U8) /* To prevent double declaration */
#error E2EPW_PACKSIG_E2EPW_BE_U8 already defined
#endif /* if (defined E2EPW_PACKSIG_E2EPW_BE_U8) */
#if (defined E2EPW_PACKSIG_E2EPW_BE_U16) /* To prevent double declaration */
#error E2EPW_PACKSIG_E2EPW_BE_U16 already defined
#endif /* if (defined E2EPW_PACKSIG_E2EPW_BE_U16) */
#if (defined E2EPW_PACKSIG_E2EPW_BE_U32) /* To prevent double declaration */
#error E2EPW_PACKSIG_E2EPW_BE_U32 already defined
#endif /* if (defined E2EPW_PACKSIG_E2EPW_BE_U32) */
#if (defined E2EPW_PACKSIG_E2EPW_BE_S8) /* To prevent double declaration */
#error E2EPW_PACKSIG_E2EPW_BE_S8 already defined
#endif /* if (defined E2EPW_PACKSIG_E2EPW_BE_S8) */
#if (defined E2EPW_PACKSIG_E2EPW_BE_S16) /* To prevent double declaration */
#error E2EPW_PACKSIG_E2EPW_BE_S16 already defined
#endif /* if (defined E2EPW_PACKSIG_E2EPW_BE_S16) */
#if (defined E2EPW_PACKSIG_E2EPW_BE_S32) /* To prevent double declaration */
#error E2EPW_PACKSIG_E2EPW_BE_S32 already defined
#endif /* if (defined E2EPW_PACKSIG_E2EPW_BE_S32) */
#if (defined E2EPW_PACKSIG_E2EPW_OP_U8N) /* To prevent double declaration */
#error E2EPW_PACKSIG_E2EPW_OP_U8N already defined
#endif /* if (defined E2EPW_PACKSIG_E2EPW_OP_U8N) */

/** \brief Macro for packing a Data Element member into a serialized data memory.
 *
 * \param[in] appData The Data Element which shall be serialized
 * \param[in] memptr Pointer to the memory which shall contain the serialized Data Element
 * \param[in] structmember Member of the Data Element which shall be serialized
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] sigtype Signal type of the Data Element member in the serialized memory.
 *            Shall be one of: E2EPW_[LE|BE]_[BL|U8|U16|U32|S8|S16|S32] or E2EPW_OP_U8N
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 */
#define E2EPW_PACKSIG(appData,memptr,structmember,bitpos,bitlength,sigtype,nbytes) \
   do {\
      E2EPW_CHECKRANGE_ ## sigtype(bitlength,(appData)->structmember); \
      E2EPW_PACKSIG_ ## sigtype((bitlength),(bitpos),(memptr),(appData)->structmember,nbytes); \
   } while (0)

/** \brief Macro for packing a boolean variable into a serialized data memory in Little Endian order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which shall contain the serialized Data Element
 * \param[in] value The data value which shall be serialized
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 */
#define E2EPW_PACKSIG_E2EPW_LE_BL(bitlength,bitpos,memptr,value,nbytes) \
   E2EPW_PACK_SIGNAL_INTO_MEMORY_LE((bitlength),(bitpos),(memptr),(value),nbytes)

/** \brief Macro for packing a uint8 variable into a serialized data memory in Little Endian order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which shall contain the serialized Data Element
 * \param[in] value The data value which shall be serialized
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 */
#define E2EPW_PACKSIG_E2EPW_LE_U8(bitlength,bitpos,memptr,value,nbytes) \
   E2EPW_PACK_SIGNAL_INTO_MEMORY_LE((bitlength),(bitpos),(memptr),(value),nbytes)

/** \brief Macro for packing a uint16 variable into a serialized data memory in Little Endian order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which shall contain the serialized Data Element
 * \param[in] value The data value which shall be serialized
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 */
#define E2EPW_PACKSIG_E2EPW_LE_U16(bitlength,bitpos,memptr,value,nbytes) \
   E2EPW_PACK_SIGNAL_INTO_MEMORY_LE((bitlength),(bitpos),(memptr),(value),nbytes)

/** \brief Macro for packing a uint32 variable into a serialized data memory in Little Endian order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which shall contain the serialized Data Element
 * \param[in] value The data value which shall be serialized
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 */
#define E2EPW_PACKSIG_E2EPW_LE_U32(bitlength,bitpos,memptr,value,nbytes) \
   E2EPW_PACK_SIGNAL_INTO_MEMORY_LE((bitlength),(bitpos),(memptr),(value),nbytes)

/** \brief Macro for packing a sint8 variable into a serialized data memory in Little Endian order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which shall contain the serialized Data Element
 * \param[in] value The data value which shall be serialized
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 */
#define E2EPW_PACKSIG_E2EPW_LE_S8(bitlength,bitpos,memptr,value,nbytes) \
   E2EPW_PACK_SIGNAL_INTO_MEMORY_LE((bitlength),(bitpos),(memptr),(value),nbytes)

/** \brief Macro for packing a sint16 variable into a serialized data memory in Little Endian order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which shall contain the serialized Data Element
 * \param[in] value The data value which shall be serialized
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 */
#define E2EPW_PACKSIG_E2EPW_LE_S16(bitlength,bitpos,memptr,value,nbytes) \
   E2EPW_PACK_SIGNAL_INTO_MEMORY_LE((bitlength),(bitpos),(memptr),(value),nbytes)

/** \brief Macro for packing a sint32 variable into a serialized data memory in Little Endian order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which shall contain the serialized Data Element
 * \param[in] value The data value which shall be serialized
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 */
#define E2EPW_PACKSIG_E2EPW_LE_S32(bitlength,bitpos,memptr,value,nbytes) \
   E2EPW_PACK_SIGNAL_INTO_MEMORY_LE((bitlength),(bitpos),(memptr),(value),nbytes)

/** \brief Macro for packing a boolean variable into a serialized data memory in Big Endian order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which shall contain the serialized Data Element
 * \param[in] value The data value which shall be serialized
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 */
#define E2EPW_PACKSIG_E2EPW_BE_BL(bitlength,bitpos,memptr,value,nbytes) \
   E2EPW_PACK_SIGNAL_INTO_MEMORY_BE((bitlength),(bitpos),(memptr),(value),nbytes)

/** \brief Macro for packing a uint8 variable into a serialized data memory in Big Endian order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which shall contain the serialized Data Element
 * \param[in] value The data value which shall be serialized
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 */
#define E2EPW_PACKSIG_E2EPW_BE_U8(bitlength,bitpos,memptr,value,nbytes) \
   E2EPW_PACK_SIGNAL_INTO_MEMORY_BE((bitlength),(bitpos),(memptr),(value),nbytes)

/** \brief Macro for packing a uint16 variable into a serialized data memory in Big Endian order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which shall contain the serialized Data Element
 * \param[in] value The data value which shall be serialized
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 */
#define E2EPW_PACKSIG_E2EPW_BE_U16(bitlength,bitpos,memptr,value,nbytes) \
   E2EPW_PACK_SIGNAL_INTO_MEMORY_BE((bitlength),(bitpos),(memptr),(value),nbytes)

/** \brief Macro for packing a uint32 variable into a serialized data memory in Big Endian order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which shall contain the serialized Data Element
 * \param[in] value The data value which shall be serialized
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 */
#define E2EPW_PACKSIG_E2EPW_BE_U32(bitlength,bitpos,memptr,value,nbytes) \
   E2EPW_PACK_SIGNAL_INTO_MEMORY_BE((bitlength),(bitpos),(memptr),(value),nbytes)

/** \brief Macro for packing a sint8 variable into a serialized data memory in Big Endian order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which shall contain the serialized Data Element
 * \param[in] value The data value which shall be serialized
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 */
#define E2EPW_PACKSIG_E2EPW_BE_S8(bitlength,bitpos,memptr,value,nbytes) \
   E2EPW_PACK_SIGNAL_INTO_MEMORY_BE((bitlength),(bitpos),(memptr),(value),nbytes)

/** \brief Macro for packing a sint16 variable into a serialized data memory in Big Endian order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which shall contain the serialized Data Element
 * \param[in] value The data value which shall be serialized
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 */
#define E2EPW_PACKSIG_E2EPW_BE_S16(bitlength,bitpos,memptr,value,nbytes) \
   E2EPW_PACK_SIGNAL_INTO_MEMORY_BE((bitlength),(bitpos),(memptr),(value),nbytes)

/** \brief Macro for packing a sint32 variable into a serialized data memory in Big Endian order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which shall contain the serialized Data Element
 * \param[in] value The data value which shall be serialized
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 */
#define E2EPW_PACKSIG_E2EPW_BE_S32(bitlength,bitpos,memptr,value,nbytes) \
   E2EPW_PACK_SIGNAL_INTO_MEMORY_BE((bitlength),(bitpos),(memptr),(value),nbytes)

/** \brief Macro for packing an opaque uint8n variable into a serialized data memory.
 *
 * \param[in] structmember Member of the Data Element which shall be serialized
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which shall contain the serialized Data Element
 * \param[in] dataptr The Data Element which shall be serialized
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 */
#define E2EPW_PACKSIG_E2EPW_OP_U8N(bitlength,bitpos,memptr,dataptr,nbytes)          \
   do {                                                                             \
      uint8_least byte_cnt;                                                         \
      for (byte_cnt=0U; byte_cnt<((bitlength)/8U); ++byte_cnt)                      \
      {                                                                             \
         (memptr)[byte_cnt+((bitpos)/8U)] = (dataptr)[byte_cnt];                    \
      }                                                                             \
   } while (0)

/* =================== helper macros for unpacking signals from memory =========== */

#if (defined E2EPW_CHECKRANGE_E2EPW_LE_BL) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_LE_BL already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_LE_BL) */
#if (defined E2EPW_CHECKRANGE_E2EPW_LE_U8) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_LE_U8 already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_LE_U8) */
#if (defined E2EPW_CHECKRANGE_E2EPW_LE_U16) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_LE_U16 already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_LE_U16) */
#if (defined E2EPW_CHECKRANGE_E2EPW_LE_U32) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_LE_U32 already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_LE_U32) */
#if (defined E2EPW_CHECKRANGE_E2EPW_LE_S8) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_LE_S8 already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_LE_S8) */
#if (defined E2EPW_CHECKRANGE_E2EPW_LE_S16) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_LE_S16 already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_LE_S16) */
#if (defined E2EPW_CHECKRANGE_E2EPW_LE_S32) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_LE_S32 already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_LE_S32) */
#if (defined E2EPW_CHECKRANGE_E2EPW_BE_BL) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_BE_BL already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_BE_BL) */
#if (defined E2EPW_CHECKRANGE_E2EPW_BE_U8) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_BE_U8 already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_BE_U8) */
#if (defined E2EPW_CHECKRANGE_E2EPW_BE_U16) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_BE_U16 already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_BE_U16) */
#if (defined E2EPW_CHECKRANGE_E2EPW_BE_U32) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_BE_U32 already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_BE_U32) */
#if (defined E2EPW_CHECKRANGE_E2EPW_BE_S8) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_BE_S8 already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_BE_S8) */
#if (defined E2EPW_CHECKRANGE_E2EPW_BE_S16) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_BE_S16 already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_BE_S16) */
#if (defined E2EPW_CHECKRANGE_E2EPW_BE_S32) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_BE_S32 already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_BE_S32) */
#if (defined E2EPW_CHECKRANGE_E2EPW_OP_U8N) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_OP_U8N already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_OP_U8N) */

#if (defined E2EPW_CHECKRANGE_E2EPW_BL) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_BL already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_BL) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U8) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U8 already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U8) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U16) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U16 already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U16) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U32) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U32 already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U32) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S8) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S8 already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S8) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S16) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S16 already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S16) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S32) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S32 already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S32) */

#if (defined E2EPW_CHECKRANGE_E2EPW_U8_8U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U8_8U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U8_8U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U8_7U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U8_7U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U8_7U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U8_6U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U8_6U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U8_6U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U8_5U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U8_5U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U8_5U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U8_4U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U8_4U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U8_4U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U8_3U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U8_3U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U8_3U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U8_2U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U8_2U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U8_2U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U8_1U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U8_1U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U8_1U) */

#if (defined E2EPW_CHECKRANGE_E2EPW_U16_16U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U16_16U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U16_16U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U16_15U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U16_15U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U16_15U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U16_14U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U16_14U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U16_14U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U16_13U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U16_13U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U16_13U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U16_12U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U16_12U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U16_12U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U16_11U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U16_11U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U16_11U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U16_10U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U16_10U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U16_10U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U16_9U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U16_9U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U16_9U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U16_8U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U16_8U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U16_8U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U16_7U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U16_7U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U16_7U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U16_6U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U16_6U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U16_6U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U16_5U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U16_5U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U16_5U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U16_4U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U16_4U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U16_4U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U16_3U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U16_3U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U16_3U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U16_2U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U16_2U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U16_2U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U16_1U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U16_1U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U16_1U) */

#if (defined E2EPW_CHECKRANGE_E2EPW_U32_32U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U32_32U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U32_32U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U32_31U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U32_31U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U32_31U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U32_30U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U32_30U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U32_30U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U32_29U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U32_29U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U32_29U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U32_28U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U32_28U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U32_28U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U32_27U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U32_27U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U32_27U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U32_26U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U32_26U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U32_26U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U32_25U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U32_25U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U32_25U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U32_24U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U32_24U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U32_24U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U32_23U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U32_23U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U32_23U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U32_22U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U32_22U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U32_22U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U32_21U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U32_21U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U32_21U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U32_20U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U32_20U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U32_20U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U32_19U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U32_19U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U32_19U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U32_18U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U32_18U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U32_18U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U32_17U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U32_17U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U32_17U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U32_16U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U32_16U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U32_16U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U32_15U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U32_15U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U32_15U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U32_14U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U32_14U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U32_14U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U32_13U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U32_13U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U32_13U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U32_12U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U32_12U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U32_12U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U32_11U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U32_11U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U32_11U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U32_10U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U32_10U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U32_10U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U32_9U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U32_9U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U32_9U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U32_8U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U32_8U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U32_8U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U32_7U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U32_7U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U32_7U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U32_6U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U32_6U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U32_6U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U32_5U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U32_5U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U32_5U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U32_4U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U32_4U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U32_4U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U32_3U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U32_3U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U32_3U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U32_2U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U32_2U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U32_2U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_U32_1U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_U32_1U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_U32_1U) */

#if (defined E2EPW_CHECKRANGE_E2EPW_S8_8U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S8_8U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S8_8U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S8_7U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S8_7U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S8_7U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S8_6U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S8_6U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S8_6U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S8_5U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S8_5U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S8_5U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S8_4U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S8_4U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S8_4U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S8_3U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S8_3U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S8_3U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S8_2U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S8_2U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S8_2U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S8_1U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S8_1U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S8_1U) */

#if (defined E2EPW_CHECKRANGE_E2EPW_S16_16U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S16_16U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S16_16U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S16_15U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S16_15U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S16_15U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S16_14U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S16_14U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S16_14U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S16_13U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S16_13U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S16_13U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S16_12U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S16_12U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S16_12U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S16_11U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S16_11U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S16_11U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S16_10U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S16_10U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S16_10U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S16_9U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S16_9U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S16_9U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S16_8U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S16_8U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S16_8U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S16_7U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S16_7U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S16_7U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S16_6U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S16_6U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S16_6U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S16_5U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S16_5U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S16_5U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S16_4U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S16_4U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S16_4U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S16_3U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S16_3U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S16_3U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S16_2U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S16_2U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S16_2U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S16_1U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S16_1U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S16_1U) */

#if (defined E2EPW_CHECKRANGE_E2EPW_S32_32U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S32_32U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S32_32U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S32_31U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S32_31U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S32_31U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S32_30U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S32_30U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S32_30U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S32_29U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S32_29U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S32_29U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S32_28U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S32_28U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S32_28U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S32_27U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S32_27U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S32_27U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S32_26U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S32_26U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S32_26U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S32_25U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S32_25U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S32_25U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S32_24U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S32_24U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S32_24U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S32_23U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S32_23U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S32_23U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S32_22U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S32_22U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S32_22U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S32_21U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S32_21U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S32_21U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S32_20U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S32_20U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S32_20U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S32_19U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S32_19U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S32_19U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S32_18U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S32_18U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S32_18U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S32_17U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S32_17U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S32_17U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S32_16U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S32_16U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S32_16U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S32_15U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S32_15U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S32_15U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S32_14U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S32_14U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S32_14U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S32_13U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S32_13U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S32_13U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S32_12U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S32_12U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S32_12U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S32_11U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S32_11U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S32_11U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S32_10U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S32_10U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S32_10U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S32_9U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S32_9U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S32_9U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S32_8U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S32_8U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S32_8U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S32_7U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S32_7U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S32_7U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S32_6U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S32_6U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S32_6U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S32_5U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S32_5U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S32_5U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S32_4U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S32_4U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S32_4U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S32_3U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S32_3U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S32_3U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S32_2U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S32_2U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S32_2U) */
#if (defined E2EPW_CHECKRANGE_E2EPW_S32_1U) /* To prevent double declaration */
#error E2EPW_CHECKRANGE_E2EPW_S32_1U already defined
#endif /* if (defined E2EPW_CHECKRANGE_E2EPW_S32_1U) */

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_LE_BL(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_BL(bitlength,(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_LE_U8(bitlength,deMember) \
   E2EPW_CHECKRANGE_E2EPW_U8_ ## bitlength(bitlength,deMember)

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_LE_U16(bitlength,deMember) \
   E2EPW_CHECKRANGE_E2EPW_U16_ ## bitlength(bitlength,deMember)

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_LE_U32(bitlength,deMember) \
   E2EPW_CHECKRANGE_E2EPW_U32_ ## bitlength(bitlength,deMember)

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_LE_S8(bitlength,deMember)                     \
   E2EPW_CHECKRANGE_E2EPW_S8_ ## bitlength(bitlength,deMember)

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_LE_S16(bitlength,deMember)                    \
   E2EPW_CHECKRANGE_E2EPW_S16_ ## bitlength(bitlength,deMember)

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_LE_S32(bitlength,deMember)            \
   E2EPW_CHECKRANGE_E2EPW_S32_ ## bitlength(bitlength,deMember)

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_BE_BL(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_BL((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_BE_U8(bitlength,deMember) \
   E2EPW_CHECKRANGE_E2EPW_U8_ ## bitlength(bitlength,deMember)

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_BE_U16(bitlength,deMember) \
   E2EPW_CHECKRANGE_E2EPW_U16_ ## bitlength(bitlength,deMember)

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_BE_U32(bitlength,deMember) \
   E2EPW_CHECKRANGE_E2EPW_U32_ ## bitlength(bitlength,deMember)

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_BE_S8(bitlength,deMember)                     \
   E2EPW_CHECKRANGE_E2EPW_S8_ ## bitlength(bitlength,deMember)

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_BE_S16(bitlength,deMember)                    \
   E2EPW_CHECKRANGE_E2EPW_S16_ ## bitlength(bitlength,deMember)

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_BE_S32(bitlength,deMember)            \
   E2EPW_CHECKRANGE_E2EPW_S32_ ## bitlength(bitlength,deMember)

/** \brief Helper macro for performing a range check on a specific variable type.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 *
 * Note: No range check need to be applied on opaque uint8 arrays
 */
#define E2EPW_CHECKRANGE_E2EPW_OP_U8N(bitlength,deMember)

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_BL(bitlength,deMember) \
   (((deMember) == TRUE) || ((deMember) == FALSE))

/** \brief Helper macro for performing a range check on variable type uint8 with bitlength 8
 *
 * Note: for bitlength 8, a range check on type uint8 makes no sense
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U8_8U(bitlength,deMember)

/** \brief Helper macro for performing a range check on variable type uint8 with bitlength 7
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U8_7U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U8((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint8 with bitlength 6
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U8_6U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U8((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint8 with bitlength 5
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U8_5U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U8((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint8 with bitlength 4
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U8_4U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U8((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint8 with bitlength 3
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U8_3U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U8((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint8 with bitlength 2
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U8_2U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U8((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint8 with bitlength 1
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U8_1U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U8((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U8(bitlength,deMember) \
   ((deMember) < (uint8)(((uint8)1U)<<(bitlength)))

/** \brief Helper macro for performing a range check on variable type uint16 with bitlength 16
 *
 * Note: for bitlength 16, a range check on type uint16 makes no sense
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U16_16U(bitlength,deMember)

/** \brief Helper macro for performing a range check on variable type uint16 with bitlength 15
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U16_15U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U16((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint16 with bitlength 14
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U16_14U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U16((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint16 with bitlength 13
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U16_13U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U16((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint16 with bitlength 12
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U16_12U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U16((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint16 with bitlength 11
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U16_11U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U16((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint16 with bitlength 10
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U16_10U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U16((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint16 with bitlength 9
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U16_9U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U16((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint16 with bitlength 8
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U16_8U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U16((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint16 with bitlength 7
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U16_7U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U16((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint16 with bitlength 6
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U16_6U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U16((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint16 with bitlength 5
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U16_5U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U16((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint16 with bitlength 4
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U16_4U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U16((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint16 with bitlength 3
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U16_3U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U16((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint16 with bitlength 2
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U16_2U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U16((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint16 with bitlength 1
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U16_1U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U16((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U16(bitlength,deMember) \
   ((deMember) < (uint16)(((uint16)1U)<<(bitlength)))

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 32
 *
 * Note: for bitlength 32, a range check on type uint32 makes no sense
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U32_32U(bitlength,deMember)

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 31
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U32_31U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 30
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U32_30U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 29
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U32_29U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 28
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U32_28U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 27
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U32_27U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 26
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U32_26U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 25
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U32_25U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 24
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U32_24U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 23
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U32_23U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 22
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U32_22U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 21
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U32_21U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 20
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U32_20U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 19
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U32_19U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 18
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U32_18U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 17
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U32_17U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 16
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U32_16U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 15
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U32_15U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 14
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U32_14U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 13
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U32_13U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 12
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U32_12U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 11
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U32_11U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 10
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U32_10U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 9
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U32_9U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 8
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U32_8U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 7
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U32_7U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 6
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U32_6U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 5
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U32_5U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 4
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U32_4U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 3
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U32_3U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 2
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U32_2U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 1
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U32_1U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_U32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_U32(bitlength,deMember) \
   ((deMember) < (uint32)(((uint32)1U)<<(bitlength)))

/** \brief Helper macro for performing a range check on variable type sint8 with bitlength 8
 *
 * Note: for bitlength 8, a range check on type sint8 makes no sense
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S8_8U(bitlength,deMember)

/** \brief Helper macro for performing a range check on variable type sint8 with bitlength 7
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S8_7U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S8((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint8 with bitlength 6
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S8_6U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S8((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint8 with bitlength 5
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S8_5U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S8((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint8 with bitlength 4
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S8_4U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S8((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint8 with bitlength 3
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S8_3U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S8((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint8 with bitlength 2
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S8_2U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S8((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint8 with bitlength 1
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S8_1U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S8((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S8(bitlength,deMember)                     \
   (((deMember) < (sint8)(((uint8)1U)<<((bitlength)-1U))) && \
    ((deMember) >= (sint8)(((sint8)(((uint8)1U)<<((bitlength)-1U))) * -1)))

/** \brief Helper macro for performing a range check on variable type sint16 with bitlength 16
 *
 * Note: for bitlength 16, a range check on type sint16 makes no sense
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S16_16U(bitlength,deMember)

/** \brief Helper macro for performing a range check on variable type sint16 with bitlength 15
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S16_15U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S16((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint16 with bitlength 14
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S16_14U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S16((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint16 with bitlength 13
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S16_13U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S16((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint16 with bitlength 12
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S16_12U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S16((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint16 with bitlength 11
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S16_11U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S16((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint16 with bitlength 10
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S16_10U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S16((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint16 with bitlength 9
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S16_9U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S16((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint16 with bitlength 8
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S16_8U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S16((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint16 with bitlength 7
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S16_7U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S16((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint16 with bitlength 6
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S16_6U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S16((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint16 with bitlength 5
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S16_5U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S16((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint16 with bitlength 4
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S16_4U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S16((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint16 with bitlength 3
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S16_3U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S16((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint16 with bitlength 2
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S16_2U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S16((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint16 with bitlength 1
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S16_1U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S16((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S16(bitlength,deMember)                    \
   (((deMember) < (sint16)(((uint16)1U)<<((bitlength)-1U))) && \
    ((deMember) >= (sint16)(((sint16)(((uint16)1U)<<((bitlength)-1U))) * -1)))

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 32
 *
 * Note: for bitlength 32, a range check on type sint32 makes no sense
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S32_32U(bitlength,deMember)

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 31
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S32_31U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 30
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S32_30U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 29
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S32_29U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 28
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S32_28U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 27
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S32_27U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 26
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S32_26U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 25
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S32_25U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 24
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S32_24U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 23
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S32_23U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 22
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S32_22U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 21
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S32_21U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 20
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S32_20U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 19
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S32_19U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 18
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S32_18U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 17
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S32_17U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 16
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S32_16U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 15
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S32_15U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 14
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S32_14U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 13
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S32_13U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 12
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S32_12U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 11
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S32_11U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 10
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S32_10U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 9
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S32_9U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 8
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S32_8U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 7
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S32_7U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 6
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S32_6U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 5
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S32_5U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 4
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S32_4U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 3
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S32_3U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 2
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S32_2U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 1
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S32_1U(bitlength,deMember) \
   E2EPW_RangeCheckRetVal |= E2EPW_CHECKRANGE_E2EPW_S32((bitlength),(deMember)) ? \
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2EPW_CHECKRANGE_E2EPW_S32(bitlength,deMember)            \
   (((deMember) < (sint32)(((uint32)1U)<<((bitlength)-1U))) && \
    ((deMember) >= (sint32)(((sint32)(((uint32)1U)<<((bitlength)-1U))) * -1)))

/* ================= helper macros for E2EPW_PACK_SIGNAL_INTO_MEMORY_BE ========== */

#if (defined E2EPW_PACK_BE_HELP3_1) /* To prevent double declaration */
#error E2EPW_PACK_BE_HELP3_1 already defined
#endif /* if (defined E2EPW_PACK_BE_HELP3_1) */

#if (defined E2EPW_PACK_BE_HELP3_2) /* To prevent double declaration */
#error E2EPW_PACK_BE_HELP3_2 already defined
#endif /* if (defined E2EPW_PACK_BE_HELP3_2) */

#if (defined E2EPW_PACK_BE_HELP3_3) /* To prevent double declaration */
#error E2EPW_PACK_BE_HELP3_3 already defined
#endif /* if (defined E2EPW_PACK_BE_HELP3_3) */

#if (defined E2EPW_PACK_BE_HELP3_4) /* To prevent double declaration */
#error E2EPW_PACK_BE_HELP3_4 already defined
#endif /* if (defined E2EPW_PACK_BE_HELP3_4) */

#if (defined E2EPW_PACK_BE_HELP3_5) /* To prevent double declaration */
#error E2EPW_PACK_BE_HELP3_5 already defined
#endif /* if (defined E2EPW_PACK_BE_HELP3_5) */

#if (defined E2EPW_PACK_BE_HELP2) /* To prevent double declaration */
#error E2EPW_PACK_BE_HELP2 already defined
#endif /* if (defined E2EPW_PACK_BE_HELP2) */

#if (defined E2EPW_PACK_BE_HELP1) /* To prevent double declaration */
#error E2EPW_PACK_BE_HELP1 already defined
#endif /* if (defined E2EPW_PACK_BE_HELP1) */

#if (defined E2EPW_PACK_SIGNAL_INTO_MEMORY_BE) /* To prevent double declaration */
#error E2EPW_PACK_SIGNAL_INTO_MEMORY_BE already defined
#endif /* if (defined E2EPW_PACK_SIGNAL_INTO_MEMORY_BE) */

/** \brief pack a signal into a 1-byte buffer in big endian order
 *
 * \param[in] lastbits .. number of significant bits in last byte (1 .. 8)
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 * \param[in] nValue .. signal value to write into memory
 */
#define E2EPW_PACK_BE_HELP3_1(lastbits, offset, pnData, nValue )                                  \
   do {                                                                                           \
      (pnData)[0] = ((lastbits)==8U)                                                              \
      ?(((offset)==0U)?((uint8)((uint32)(nValue)&0xffU))                                          \
        :((uint8)((pnData)[0]&(uint8)((uint32)((uint32)0xffU<<(8U-(offset)))&0xffU))              \
          |(uint8)(((uint32)(((uint32)(nValue)&(uint32)(0xffU>>(offset)))))&0xffU)))              \
          :(((offset)==0U)?((uint8)((pnData)[0]&(uint8)(0xffU>>(lastbits)))                       \
            |(uint8)((uint32)((uint32)(nValue)<<(8U-(lastbits)))&0xffU))                          \
              :((uint8)((pnData)[0]                                                               \
                &(uint8)((uint8)((uint32)((uint32)0xffU<<(8U-(offset)))&0xffU)|(uint8)(0xffU>>(lastbits))))\
                  |(uint8)(((uint32)((uint32)((uint32)(nValue)<<(8U-(lastbits)))&(uint8)(0xffU>>(offset))))&0xffU)));\
   } while (0)

/** \brief pack a signal into a 2-byte buffer in big endian order
 *
 * \param[in] lastbits .. number of significant bits in last byte (1 .. 8)
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 * \param[in] nValue .. signal value to write into memory
 */
#define E2EPW_PACK_BE_HELP3_2(lastbits, offset, pnData, nValue )                                  \
   do {                                                                                           \
      (pnData)[1] = ((lastbits)==8U)?((uint8)((uint32)(nValue)&0xffU))                            \
        :((uint8)((pnData)[1]&(uint8)(0xffU>>(lastbits)))                                         \
          |(uint8)(((uint32)((uint32)(nValue)<<(8U-(lastbits))))&0xffU));                         \
      (pnData)[0] = ((offset)==0U)?                                                               \
        ((uint8)((uint32)((uint32)(nValue)>>(lastbits))&0xffU))                                   \
        :((uint8)((pnData)[0]&(uint8)((uint32)((uint32)0xffU<<(8U-(offset)))&0xffU))              \
          |(uint8)((uint32)((uint32)((uint32)(nValue)>>(lastbits))                                \
            &(uint32)(0xffU>>(offset)))&0xffU));                                                  \
   } while (0)                                                                                    \

/** \brief pack a signal into a 3-byte buffer in big endian order
 *
 * \param[in] lastbits .. number of significant bits in last byte (1 .. 8)
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 * \param[in] nValue .. signal value to write into memory
 */
#define E2EPW_PACK_BE_HELP3_3(lastbits, offset, pnData, nValue )                                  \
   do {                                                                                           \
      (pnData)[2] = ((lastbits)==8U)?((uint8)((uint32)(nValue)&0xffU))                            \
        :((uint8)((pnData)[2]&(uint8)(0xffU>>(lastbits)))                                         \
          |(uint8)(((uint32)((uint32)(nValue)<<(8U-(lastbits))))&0xffU));                         \
      (pnData)[1] = (uint8)(((uint32)(((uint32)(nValue))>>(lastbits)))&0xffU);                    \
      (pnData)[0] = ((offset)==0U)?                                                               \
        ((uint8)((uint32)(((uint32)(nValue))>>((lastbits)+8U))&0xffU))                            \
        :((uint8)((pnData)[0]&(uint8)((uint32)((uint32)0xffU<<(8U-(offset)))&0xffU))              \
          |(uint8)(((uint32)((uint32)(((uint32)(nValue))>>((lastbits)+8U))                        \
            &(uint32)(0xffU>>(offset))))&0xffU));                                                 \
   } while (0)                                                                                    \

/** \brief pack a signal into a 4-byte buffer in big endian order
 *
 * \param[in] lastbits .. number of significant bits in last byte (1 .. 8)
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 * \param[in] nValue .. signal value to write into memory
 */
#define E2EPW_PACK_BE_HELP3_4(lastbits, offset, pnData, nValue )                                  \
   do {                                                                                           \
      (pnData)[3] = ((lastbits)==8U)?((uint8)((uint32)(nValue)&0xffU))                            \
        :((uint8)((pnData)[3]&(uint8)(0xffU>>(lastbits)))                                         \
          |(uint8)(((uint32)((uint32)(nValue)<<(8U-(lastbits))))&0xffU));                         \
      (pnData)[2] = (uint8)(((uint32)(((uint32)(nValue))>>(lastbits)))&0xffU);                    \
      (pnData)[1] = (uint8)(((uint32)(((uint32)(nValue))>>((lastbits)+8U)))&0xffU);               \
      (pnData)[0] = ((offset)==0U)?                                                               \
        ((uint8)((uint32)(((uint32)(nValue))>>((lastbits)+16U))&0xffU))                           \
        :((uint8)((pnData)[0]&(uint8)((uint32)((uint32)0xffU<<(8U-(offset)))&0xffU))              \
          |(uint8)(((uint32)((uint32)(((uint32)(nValue))>>((lastbits)+16U))                       \
            &(uint32)(0xffU>>(offset))))&0xffU));                                                 \
   } while (0)

/** \brief pack a signal into a 5-byte buffer in big endian order
 *
 * \param[in] lastbits .. number of significant bits in last byte (1 .. 8)
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 * \param[in] nValue .. signal value to write into memory
 */
#define E2EPW_PACK_BE_HELP3_5(lastbits, offset, pnData, nValue )                                  \
   do {                                                                                           \
      (pnData)[4] = ((lastbits)==8U)?((uint8)((uint32)(nValue)&0xffU))                            \
        :((uint8)((pnData)[4]&(uint8)(0xffU>>(lastbits)))                                         \
          |(uint8)(((uint32)((uint32)(nValue)<<(8U-(lastbits))))&0xffU));                         \
      (pnData)[3] = (uint8)(((uint32)(((uint32)(nValue))>>(lastbits)))&0xffU);                    \
      (pnData)[2] = (uint8)(((uint32)(((uint32)(nValue))>>((lastbits)+8U)))&0xffU);               \
      (pnData)[1] = (uint8)(((uint32)(((uint32)(nValue))>>((lastbits)+16U)))&0xffU);              \
      (pnData)[0] = ((offset)==0U)?                                                               \
        ((uint8)((uint32)(((uint32)(nValue))>>((lastbits)+24U))&0xffU))                           \
        :((uint8)((pnData)[0]&(uint8)((uint32)((uint32)0xffU<<(8U-(offset)))&0xffU))              \
          |(uint8)(((uint32)((uint32)(((uint32)(nValue))>>((lastbits)+24U))                       \
            &(uint32)(0xffU>>(offset))))&0xffU));                                                 \
   } while (0)

/** \brief pack a signal into a byte buffer in big endian order
 *
 * \param[in] lastbits .. number of significant bits in last byte (1 .. 8)
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 * \param[in] nValue .. signal value to write into memory
 * \param[in] nbytes .. number of affected bytes (without pre-compile calculation (1 .. 5)
 */
#define E2EPW_PACK_BE_HELP2(lastbits, offset, pnData, nValue, nbytes )                            \
      E2EPW_PACK_BE_HELP3_ ## nbytes((lastbits), (offset), (pnData), (nValue))                    \

/** \brief pack a signal into a byte buffer in big endian order
 *
 * \param[in] len .. length of signal in bits (1 .. 32)
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 * \param[in] nValue .. signal value to write into memory
 * \param[in] nbytes .. number of affected bytes (without pre-compile calculation (1 .. 5)
 */
#define E2EPW_PACK_BE_HELP1(len, offset, pnData, nValue, nbytes)                                  \
        E2EPW_PACK_BE_HELP2(((uint8)(1U+(((len)+(offset)+7U)%8U))),                               \
                            (offset),                                                             \
                            (pnData),                                                             \
                            (nValue),                                                             \
                            nbytes)

/* ============== end of helper macros for E2EPW_PACK_SIGNAL_INTO_MEMORY_BE ============= */

/** \brief pack a signal with length 'len' bits to bit offset 'offset' within
 * the buffer 'pnData' with 'nbytes' number of affected bytes.
 * The signals within pnData are assumed to follow the big endian convention.
 *
 * \param[in] len .. length of signal in bits (1 .. 32)
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 * \param[in] nValue .. signal value to write into memory
 * \param[in] nbytes .. number of affected bytes (without pre-compile calculation (1 .. 5)
 */
#define E2EPW_PACK_SIGNAL_INTO_MEMORY_BE(len, offset, pnData, nValue, nbytes)                     \
   do {                                                                                           \
        E2EPW_PACK_BE_HELP1((len),                                                                \
                            ((uint8)(7U-((offset)%8U))),                                          \
                            (&((pnData)[(offset)/8U])),                                           \
                            (nValue),                                                             \
                            nbytes);                                                              \
   } while (0)

/* ===================== helper macros for E2EPW_PACK_SIGNAL_INTO_MEMORY_LE ============ */

#if (defined E2EPW_PACK_LE_HELP3_1) /* To prevent double declaration */
#error E2EPW_PACK_LE_HELP3_1 already defined
#endif /* if (defined E2EPW_PACK_LE_HELP3_1) */

#if (defined E2EPW_PACK_LE_HELP3_2) /* To prevent double declaration */
#error E2EPW_PACK_LE_HELP3_2 already defined
#endif /* if (defined E2EPW_PACK_LE_HELP3_2) */

#if (defined E2EPW_PACK_LE_HELP3_3) /* To prevent double declaration */
#error E2EPW_PACK_LE_HELP3_3 already defined
#endif /* if (defined E2EPW_PACK_LE_HELP3_3) */

#if (defined E2EPW_PACK_LE_HELP3_4) /* To prevent double declaration */
#error E2EPW_PACK_LE_HELP3_4 already defined
#endif /* if (defined E2EPW_PACK_LE_HELP3_4) */

#if (defined E2EPW_PACK_LE_HELP3_5) /* To prevent double declaration */
#error E2EPW_PACK_LE_HELP3_5 already defined
#endif /* if (defined E2EPW_PACK_LE_HELP3_5) */

#if (defined E2EPW_PACK_LE_HELP2) /* To prevent double declaration */
#error E2EPW_PACK_LE_HELP2 already defined
#endif /* if (defined E2EPW_PACK_LE_HELP2) */

#if (defined E2EPW_PACK_LE_HELP1) /* To prevent double declaration */
#error E2EPW_PACK_LE_HELP1 already defined
#endif /* if (defined E2EPW_PACK_LE_HELP1) */

#if (defined E2EPW_PACK_SIGNAL_INTO_MEMORY_LE) /* To prevent double declaration */
#error E2EPW_PACK_SIGNAL_INTO_MEMORY_LE already defined
#endif /* if (defined E2EPW_PACK_SIGNAL_INTO_MEMORY_LE) */

/** \brief pack a signal into a 1-byte buffer in little endian order
 *
 * \param[in] lastbits .. number of significant bits in last byte (1 .. 8)
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 * \param[in] nValue .. signal value to write into memory
 */
#define E2EPW_PACK_LE_HELP3_1(lastbits, offset, pnData, nValue )                                  \
   do {                                                                                           \
      (pnData)[0] = ((offset)==0U)?(((lastbits)==8U)?((uint8)((uint32)(nValue)&0xffU))            \
        :((uint8)(((uint32)((uint32)(nValue)&(uint32)(0xffU>>(8U-(lastbits)))))&0xffU)            \
        |(uint8)((pnData)[0]&(uint8)(((uint32)((uint32)0xffU<<(lastbits)))&0xffU))))              \
        :(((lastbits)==8U)?((uint8)((uint32)((uint32)(nValue)<<(offset))&0xffU)                   \
        |(uint8)((pnData)[0]&(uint8)(0xffU>>(8U-(offset)))))                                      \
        :((uint8)((uint32)((uint32)((uint32)(nValue)<<(offset))&(uint32)(0xffU>>(8U-(lastbits))))&0xffU)\
        |(uint8)((pnData)[0]                                                                      \
        &(uint8)((uint8)(((uint32)((uint32)0xffU<<(lastbits)))&0xffU)|(uint8)(0xffU>>(8U-(offset)))))));\
   } while (0)

/** \brief pack a signal into a 2-byte buffer in little endian order
 *
 * \param[in] lastbits .. number of significant bits in last byte (1 .. 8)
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 * \param[in] nValue .. signal value to write into memory
 */
#define E2EPW_PACK_LE_HELP3_2(lastbits, offset, pnData, nValue )                                  \
   do {                                                                                           \
      (pnData)[0] = ((offset)==0U)?((uint8)((uint32)(nValue)&0xffU))                              \
        :((uint8)(((uint32)((uint32)(nValue)<<(offset)))&0xffU)                                   \
          |(uint8)((pnData)[0]&(uint8)(0xffU>>(8U-(offset)))));                                   \
      (pnData)[1] = ((lastbits)==8U)?((uint8)((uint32)(((uint32)(nValue))>>(8U-(offset)))&0xffU)) \
        :((uint8)((uint32)((uint32)(((uint32)(nValue))>>(8U-(offset)))&(uint32)(0xffU>>(8U-(lastbits))))&0xffU)\
          |(uint8)((pnData)[1]&(uint8)(((uint32)((uint32)0xffU<<(lastbits)))&0xffU)));            \
   } while (0)

/** \brief pack a signal into a 3-byte buffer in little endian order
 *
 * \param[in] lastbits .. number of significant bits in last byte (1 .. 8)
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 * \param[in] nValue .. signal value to write into memory
 */
#define E2EPW_PACK_LE_HELP3_3(lastbits, offset, pnData, nValue )                                  \
   do {                                                                                           \
      (pnData)[0] = ((offset)==0U)?((uint8)(((uint32)(nValue))&0xffU))                            \
        :((uint8)((uint32)(((uint32)(nValue))<<(offset))&0xffU)                                   \
          |(uint8)((pnData)[0]&(uint8)(0xffU>>(8U-(offset)))));                                   \
      (pnData)[1] = (uint8)(((uint32)(((uint32)(nValue))>>(8U-(offset))))&0xffU);                 \
      (pnData)[2] = ((lastbits)==8U)?((uint8)((uint32)(((uint32)(nValue))>>(16U-(offset)))&0xffU))\
        :((uint8)((uint32)((uint32)(((uint32)(nValue))>>(16U-(offset)))&(uint32)(0xffU>>(8U-(lastbits))))&0xffU)\
          |(uint8)((pnData)[2]&(uint8)(((uint32)((uint32)0xffU<<(lastbits)))&0xffU)));            \
   } while (0)

/** \brief pack a signal into a 4-byte buffer in little endian order
 *
 * \param[in] lastbits .. number of significant bits in last byte (1 .. 8)
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 * \param[in] nValue .. signal value to write into memory
 */
#define E2EPW_PACK_LE_HELP3_4(lastbits, offset, pnData, nValue )                                  \
   do {                                                                                           \
      (pnData)[0] = ((offset)==0U)?((uint8)(((uint32)(nValue))&0xffU))                            \
        :((uint8)((uint32)(((uint32)(nValue))<<(offset))&0xffU)                                   \
          |(uint8)((pnData)[0]&(uint8)(0xffU>>(8U-(offset)))));                                   \
      (pnData)[1] = (uint8)(((uint32)(((uint32)(nValue))>>(8U-(offset))))&0xffU);                 \
      (pnData)[2] = (uint8)(((uint32)(((uint32)(nValue))>>(16U-(offset))))&0xffU);                \
      (pnData)[3] = ((lastbits)==8U)?((uint8)((uint32)(((uint32)(nValue))>>(24U-(offset)))&0xffU))\
        :((uint8)((uint32)((uint32)(((uint32)(nValue))>>(24U-(offset)))&(uint32)(0xffU>>(8U-(lastbits))))&0xffU)\
          |(uint8)((pnData)[3]&(uint8)(((uint32)((uint32)0xffU<<(lastbits)))&0xffU)));            \
   } while (0)

/** \brief pack a signal into a 5-byte buffer in little endian order
 *
 * \param[in] lastbits .. number of significant bits in last byte (1 .. 8)
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 * \param[in] nValue .. signal value to write into memory
 */
#define E2EPW_PACK_LE_HELP3_5(lastbits, offset, pnData, nValue )                                  \
   do {                                                                                           \
     (pnData)[0] = ((offset)==0U)?((uint8)(((uint32)(nValue))&0xffU))                             \
       :((uint8)((uint32)(((uint32)(nValue))<<(offset))&0xffU)                                    \
         |(uint8)((pnData)[0]&(uint8)(0xffU>>(8U-(offset)))));                                    \
     (pnData)[1] = (uint8)(((uint32)(((uint32)(nValue))>>(8U-(offset))))&0xffU);                  \
     (pnData)[2] = (uint8)(((uint32)(((uint32)(nValue))>>(16U-(offset))))&0xffU);                 \
     (pnData)[3] = (uint8)(((uint32)(((uint32)(nValue))>>(24U-(offset))))&0xffU);                 \
     (pnData)[4] = ((lastbits)==8U)?((uint8)((uint32)(((uint32)(nValue))>>(32U-(offset)))&0xffU)) \
       :((uint8)((uint32)((uint32)(((uint32)(nValue))>>(32U-(offset)))&(uint32)(0xffU>>(8U-(lastbits))))&0xffU)\
         |(uint8)((pnData)[4]&(uint8)(((uint32)((uint32)0xffU<<(lastbits)))&0xffU)));             \
   } while (0)

/** \brief pack a signal into a byte buffer in little endian order
 *
 * \param[in] lastbits .. number of significant bits in last byte (1 .. 8)
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 * \param[in] nValue .. signal value to write into memory
 * \param[in] nbytes .. number of affected bytes (without pre-compile calculation (1 .. 5)
 */
#define E2EPW_PACK_LE_HELP2(lastbits, offset, pnData, nValue, nbytes )                 \
      E2EPW_PACK_LE_HELP3_ ## nbytes((lastbits), (offset), (pnData), (nValue))


/*
 * pack the signal into a byte buffer
 * \param[in] len .. length of signal in bits (1 .. 32)
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 * \param[in] nValue .. signal value to write into memory
 * \param[in] nbytes .. number of affected bytes (without pre-compile calculation (1 .. 5)
 */
#define E2EPW_PACK_LE_HELP1(len, offset, pnData, nValue, nbytes)                                  \
        E2EPW_PACK_LE_HELP2(((uint8)(1U+(((len)+(offset)+7U)%8U))),                               \
                            (offset),                                                             \
                            (pnData),                                                             \
                            (nValue),                                                             \
                            nbytes)

/* =============== end of helper macros for E2EPW_PACK_SIGNAL_INTO_MEMORY_LE ================ */

/** \brief pack a signal with length 'len' bits to bit offset 'offset' within
 * the buffer 'pnData' with 'nbytes' number of affected bytes.
 * The signals within pnData are assumed to follow the little endian convention.
 *
 * \param[in] len .. length of signal in bits (1 .. 32)
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 * \param[in] nValue .. signal value to write into memory
 * \param[in] nbytes .. number of affected bytes (without pre-compile calculation (1 .. 5)
 */
#define E2EPW_PACK_SIGNAL_INTO_MEMORY_LE(len, offset, pnData, nValue, nbytes)                     \
   do {                                                                                           \
        E2EPW_PACK_LE_HELP1((len),                                                                \
                            ((uint8)((offset)%8U)),                                               \
                            (&((pnData)[(offset)/8U])),                                           \
                            (nValue),                                                             \
                            nbytes);                                                              \
   } while (0)

/* Deviation MISRAC2012-1 <STOP> */

/*==[Types]===================================================================*/

/*==[Declaration of Constants with external linkage]==========================*/

/*==[Declaration of Variables with external linkage]==========================*/

/*==[Declaration of functions with external linkage]==========================*/

#endif /* if !defined( E2EPW_INT_H ) */
/*==================[end of file]============================================*/
