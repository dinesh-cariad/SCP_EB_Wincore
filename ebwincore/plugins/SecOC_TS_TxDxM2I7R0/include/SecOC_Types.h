/**
 * \file
 *
 * \brief AUTOSAR SecOC
 *
 * This file contains the implementation of the AUTOSAR
 * module SecOC.
 *
 * \version 2.7.4
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef SECOC_TYPES_H
#define SECOC_TYPES_H

/*==================[misra deviations]============================================================*/

/*==================[includes]====================================================================*/

/* !LINKSTO SecOC.Dsn.BasicSWStructure.RxTx.SecOC_Typesh,1, SWS_SecOC_RxTx_00103,2 */
#include <ComStack_Types.h>
#include <SecOC_Version.h>

/*==================[macros]======================================================================*/

/* !LINKSTO SecOC.Dsn.interface.RxTx.SECOC_UNINIT,1 */
/** \brief SecOC module is not initialized
 */
#if (defined SECOC_UNINIT)
#error SECOC_UNINIT already defined
#endif
#define SECOC_UNINIT                                                                    0U

/* !LINKSTO SecOC.Dsn.interface.RxTx.SECOC_INIT,1 */
/** \brief SecOC module is initialized
 */
#if (defined SECOC_INIT)
#error SECOC_INIT already defined
#endif
#define SECOC_INIT                                                                      1U

/* !LINKSTO SecOC.Dsn.interface.RxTx.SECOC_E_OK,1 */
/** \brief Return value for a successful "get freshness value" request
 */
#if (defined SECOC_E_OK)
#error SECOC_E_OK already defined
#endif
#define SECOC_E_OK                                                                      0U

/* !LINKSTO SecOC.Dsn.interface.RxTx.SECOC_E_NOT_OK,1 */
/** \brief Return value for an unsuccessful "get freshness value" request
 */
#if (defined SECOC_E_NOT_OK)
#error SECOC_E_NOT_OK already defined
#endif
#define SECOC_E_NOT_OK                                                                  1U

/* !LINKSTO SecOC.Dsn.interface.RxTx.SECOC_E_BUSY,1 */
/** \brief Return value if the "get freshness value" service is currently busy
 */
#if (defined SECOC_E_BUSY)
#error SECOC_E_BUSY already defined
#endif
#define SECOC_E_BUSY                                                                    2U

#ifndef SECOC_VERIFICATIONSUCCESS
/* !LINKSTO SecOC.Dsn.interface.Rx.SECOC_VERIFICATIONSUCCESS,1 */
/** \brief Verification successful
 */
#define SECOC_VERIFICATIONSUCCESS                                                       0U
#endif /* SECOC_VERIFICATIONSUCCESS */

#ifndef SECOC_VERIFICATIONFAILURE
/* !LINKSTO SecOC.Dsn.interface.Rx.SECOC_VERIFICATIONFAILURE,1 */
/** \brief Verification not successful
 */
#define SECOC_VERIFICATIONFAILURE                                                       1U
#endif /* SECOC_VERIFICATIONFAILURE */

#ifndef SECOC_FRESHNESSFAILURE
/* !LINKSTO SecOC.Dsn.interface.Rx.SECOC_FRESHNESSFAILURE,1 */
/** \brief Verification not successful because of wrong freshness value.
 */
#define SECOC_FRESHNESSFAILURE                                                          2U
#endif /* ifndef SECOC_FRESHNESSFAILURE */

#ifndef SECOC_AUTHENTICATIONBUILDFAILURE
/* !LINKSTO SecOC.Dsn.interface.Rx.SECOC_AUTHENTICATIONBUILDFAILURE,1 */
/** \brief Authentication could not be built. Freshness attempt or authentication calculation
 *         failure.
 */
#define SECOC_AUTHENTICATIONBUILDFAILURE                                      SECOC_FRESHNESSFAILURE
#endif /* ifndef SECOC_AUTHENTICATIONBUILDFAILURE */

#ifndef SECOC_NO_VERIFICATION
/* !LINKSTO SecOC.Dsn.interface.Rx.SECOC_NO_VERIFICATION,1 */
/** \brief Verification has been skipped.
 */
#define SECOC_NO_VERIFICATION                                                           4U
#endif /* ifndef SECOC_NO_VERIFICATION */

#ifndef SECOC_MACSERVICEFAILURE
/* !LINKSTO SecOC.Dsn.interface.Rx.SECOC_MACSERVICEFAILURE,1 */
/** \brief Verification not successful because of wrong freshness value.
 */
#define SECOC_MACSERVICEFAILURE                                                         8U
#endif /* ifndef SECOC_MACSERVICEFAILURE */

/** \brief Defines, that Both 'True' and 'False' AuthenticationStatus is propagated
 */
#if (defined SECOC_STATUS_PROP_BOTH)
#error SECOC_STATUS_PROP_BOTH already defined
#endif
#define SECOC_STATUS_PROP_BOTH                                                          2U

/** \brief Defines, that Only 'False' AuthenticationStatus is propagated
 */
#if (defined SECOC_STATUS_PROP_FAILURE_ONLY)
#error SECOC_STATUS_PROP_FAILURE_ONLY already defined
#endif
#define SECOC_STATUS_PROP_FAILURE_ONLY                                                  1U

/** \brief Defines, that No AuthenticationStatus is propagated
 */
#if (defined SECOC_STATUS_PROP_NONE)
#error SECOC_STATUS_PROP_NONE already defined
#endif
#define SECOC_STATUS_PROP_NONE                                                          0U

/** \brief SecOC does not queries the freshness.
 */
#if (defined SECOC_FRESHNESS_NONE)
#error SECOC_FRESHNESS_NONE already defined
#endif
#define SECOC_FRESHNESS_NONE                                                            0U

/** \brief SecOC queries the freshness for every PDU to process using the Rte service port
 *         FreshnessManagement.
 */
#if (defined SECOC_FRESHNESS_RTE)
#error SECOC_FRESHNESS_RTE already defined
#endif
#define SECOC_FRESHNESS_RTE                                                             1U

/** \brief SecOC queries the freshness for every PDU to process using the C function defined by the
 *         configuration parameter SecOCFreshnessValueFuncName.
 */
#if (defined SECOC_FRESHNESS_CFUNC)
#error SECOC_FRESHNESS_CFUNC already defined
#endif
#define SECOC_FRESHNESS_CFUNC                                                           2U

/** \brief SecOC does not propagate the verification status.
 */
#if (defined SECOC_VERIFICATION_STATUS_PROP_NONE)
#error SECOC_VERIFICATION_STATUS_PROP_NONE already defined
#endif
#define SECOC_VERIFICATION_STATUS_PROP_NONE   0U

/** \brief SecOC propagates the verification status via the AUTOSAR defined API(s).
 */
#if (defined SECOC_VERIFICATION_STATUS_PROP_AUTOSAR)
#error SECOC_VERIFICATION_STATUS_PROP_AUTOSAR already defined
#endif
#define SECOC_VERIFICATION_STATUS_PROP_AUTOSAR    1U

/** \brief SecOC propagates the verification status via the custom API(s).
 */
#if (defined SECOC_VERIFICATION_STATUS_PROP_EB)
#error SECOC_VERIFICATION_STATUS_PROP_EB already defined
#endif
#define SECOC_VERIFICATION_STATUS_PROP_EB  2U

/** \brief Macro which defines that the GetTxFreshness function shall be used to
 *         obtain the freshness value.
 */
#if (defined SECOC_GET_TX_FRESHNESS_FUNC_TYPE)
#error SECOC_GET_TX_FRESHNESS_FUNC_TYPE already defined
#endif
#define SECOC_GET_TX_FRESHNESS_FUNC_TYPE                                                0U

/** \brief Macro which defines that the GetTxFreshnessTruncData function shall be used to
 *         obtain the freshness value.
 */
#if (defined SECOC_GET_TX_FRESHNESS_TRUNCDATA_FUNC_TYPE)
#error SECOC_GET_TX_FRESHNESS_TRUNCDATA_FUNC_TYPE already defined
#endif
#define SECOC_GET_TX_FRESHNESS_TRUNCDATA_FUNC_TYPE                                      1U

/** \brief Macro which defines that the GetTxFreshness function shall be used to
 *         obtain the freshness value.
 */
#if (defined SECOC_GET_RX_FRESHNESS_FUNC_TYPE)
#error SECOC_GET_RX_FRESHNESS_FUNC_TYPE already defined
#endif
#define SECOC_GET_RX_FRESHNESS_FUNC_TYPE                                                0U

/** \brief Macro which defines that the GetTxFreshnessTruncData function shall be used to
 *         obtain the freshness value.
 */
#if (defined SECOC_GET_RX_FRESHNESS_AUTHDATA_FUNC_TYPE)
#error SECOC_GET_RX_FRESHNESS_AUTHDATA_FUNC_TYPE already defined
#endif
#define SECOC_GET_RX_FRESHNESS_AUTHDATA_FUNC_TYPE                                       1U

/*==================[type definitions]============================================================*/

#ifndef RTE_TYPE_SecOC_StateType
/* \cond SECOC_DOC_INTERNAL_MACROS */
#define RTE_TYPE_SecOC_StateType
/* \endcond */
/* !LINKSTO SecOC.Dsn.interface.RxTx.SecOC_StateType,1 */
/** \brief States of the SecOC module.
 *
 * Range: SECOC_UNINIT, SECOC_INIT.
 */
typedef uint8 SecOC_StateType;
#endif

#ifndef RTE_TYPE_SecOC_VerificationResultType
/* \cond SECOC_DOC_INTERNAL_MACROS */
#define RTE_TYPE_SecOC_VerificationResultType
/* \endcond */
/* !LINKSTO SecOC.Dsn.interface.Rx.SecOC_VerificationResultType,3 */
/** \brief Type, to indicate verification results
 *
 * Range: SECOC_VERIFICATIONSUCCESS, SECOC_VERIFICATIONFAILURE, SECOC_FRESHNESSFAILURE,
 *        SECOC_AUTHENTICATIONBUILDFAILURE, SECOC_NO_VERIFICATION, SECOC_MACSERVICEFAILURE.
 */
typedef uint8 SecOC_VerificationResultType;
#endif

/*==================[external constants]==========================================================*/

/*==================[external data]===============================================================*/

/*==================[external function declarations]==============================================*/

#endif /* #ifndef SECOC_TYPES_H */

/*==================[end of file]=================================================================*/
