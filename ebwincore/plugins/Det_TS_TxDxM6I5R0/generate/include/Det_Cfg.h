#ifndef DET_CFG_H
#define DET_CFG_H

/**
 * \file
 *
 * \brief AUTOSAR Det
 *
 * This file contains the implementation of the AUTOSAR
 * module Det.
 *
 * \version 6.5.6
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
[!SELECT "DetGeneral"!][!//
[!VAR "ServiceAPIEnabled" = "node:exists(DetServiceAPI)"!]
[!IF "$ServiceAPIEnabled"!]
[!VAR "ASR32ServiceAPIEnabled" = "DetServiceAPI/DetEnableASR32ServiceAPI = 'true'"!]
[!VAR "ASR40ServiceAPIEnabled" = "DetServiceAPI/DetEnableASR40ServiceAPI = 'true'"!]
[!VAR "ASR43ServiceAPIEnabled" = "DetServiceAPI/DetEnableASR43ServiceAPI = 'true'"!]
[!VAR "DefaultServiceAPI" = "node:value(DetServiceAPI/DetDefaultASRServiceAPI)"!]
[!ELSE!]
[!VAR "ASR32ServiceAPIEnabled" = "'false'"!]
[!VAR "ASR40ServiceAPIEnabled" = "'false'"!]
[!VAR "ASR43ServiceAPIEnabled" = "'false'"!]
[!VAR "DefaultServiceAPI" = "'AUTOSAR_40'"!]
[!ENDIF!]
/*==================[includes]==============================================*/

/*==================[macros]================================================*/

#if (defined DET_SERVICE_API_NONE)
#error DET_SERVICE_API_NONE is already defined
#endif

#define DET_SERVICE_API_NONE   0

#if (defined DET_SERVICE_API_ASR32)
#error DET_SERVICE_API_ASR32 is already defined
#endif

/** \brief Definition of AUTOSAR 3.2 service API*/
#define DET_SERVICE_API_ASR32 32

#if (defined DET_SERVICE_API_ASR40)
#error DET_SERVICE_API_ASR40 is already defined
#endif

/** \brief Definition of AUTOSAR 4.0 service API*/
#define DET_SERVICE_API_ASR40 40

#if (defined DET_SERVICE_API_ASR43)
#error DET_SERVICE_API_ASR43 is already defined
#endif

/** \brief Definition of AUTOSAR 4.3 service API*/
#define DET_SERVICE_API_ASR43 43

#if (defined DET_ENABLE_ASR32_SERVICE_API) /* To prevent double declaration */
#error DET_ENABLE_ASR32_SERVICE_API is already defined
#endif /* if (defined DET_ENABLE_ASR32_SERVICE_API) */

/** \brief Definition whether AUTOSAR 3.2 service API is enabled **/
#define DET_ENABLE_ASR32_SERVICE_API                   [!//
[!IF "$ASR32ServiceAPIEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


#if (defined DET_ENABLE_ASR40_SERVICE_API) /* To prevent double declaration */
#error DET_ENABLE_ASR40_SERVICE_API is already defined
#endif /* if (defined DET_ENABLE_ASR40_SERVICE_API) */

/** \brief Definition whether AUTOSAR 4.0 service API is enabled **/
#define DET_ENABLE_ASR40_SERVICE_API                   [!//
[!IF "$ASR40ServiceAPIEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


#if (defined DET_ENABLE_ASR43_SERVICE_API) /* To prevent double declaration */
#error DET_ENABLE_ASR43_SERVICE_API already defined
#endif /* if (defined DET_ENABLE_ASR43_SERVICE_API) */

/** \brief Definition whether AUTOSAR 4.3 service API is enabled **/
#define DET_ENABLE_ASR43_SERVICE_API                   [!//
[!IF "$ASR43ServiceAPIEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


#if (defined DET_DEFAULT_ASR_SERVICE_API) /* To prevent double declaration */
#error DET_DEFAULT_ASR_SERVICE_API already defined
#endif /* if (defined DET_DEFAULT_ASR_SERVICE_API) */

/** \brief Definition of default service API **/
#define DET_DEFAULT_ASR_SERVICE_API                    [!//
[!IF "$DefaultServiceAPI = 'AUTOSAR_32'"!]DET_SERVICE_API_ASR32
[!ELSEIF "$DefaultServiceAPI = 'AUTOSAR_40'"!]DET_SERVICE_API_ASR40
[!ELSEIF "$DefaultServiceAPI = 'AUTOSAR_43'"!]DET_SERVICE_API_ASR43
[!ELSE!]DET_SERVICE_API_NONE
[!ENDIF!]


#if (defined DET_VERSION_INFO_API)
#error DET_VERSION_INFO_API is already defined
#endif

/** \brief Version info API availabiliity
 **
 ** En- or diasable the availability of Det_GetVersionInfo() */
#define DET_VERSION_INFO_API                [!//
[!IF "DetVersionInfoApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined  DET_RTE_USAGE)
#error  DET_RTE_USAGE is already defined
#endif

/** \brief Rte usage
 **
 ** En- or disables the usage of the Rte for the Det */
#define DET_RTE_USAGE                       [!//
[!IF "DetRteUsage = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DET_LOGMODE_INTERNAL)
#error DET_LOGMODE_INTERNAL is already defined
#endif

/** \brief Internal logging mode
 **
 ** Errors are logged in an internal circular buffer. */
#define DET_LOGMODE_INTERNAL                1U

#if (defined  DET_LOGMODE_BREAKPOINT)
#error DET_LOGMODE_BREAKPOINT is already defined
#endif

/** \brief Breakpoint mode
 **
 ** If an error is reported the system hals at a break point and the user can
 ** analyse the situation in a debugger/emulator. */
#define DET_LOGMODE_BREAKPOINT              2U

#if (defined DET_LOGMODE)
#error DET_LOGMODE is already defined
#endif

/** \brief Error logging mode
 **
 ** Defines the mode how errors are logged. Only one of the following modes
 ** can be active. Possible modes: ::DET_LOGMODE_INTERNAL,
 ** ::DET_LOGMODE_BREAKPOINT */
#define DET_LOGMODE                         [!//
[!IF "LoggingMode = 'Internal'"!]DET_LOGMODE_INTERNAL[!ELSE!]DET_LOGMODE_BREAKPOINT[!ENDIF!]

[!IF "LoggingMode = 'Internal'"!][!//

#if (defined DET_BUFFERSIZE)
#error DET_BUFFERSIZE is already defined
#endif

/** \brief Maximum number of errors that can be logged
 **
 ** This macro defines the size of the circular buffer, which limits the
 ** maximum number of errors that can be logged. */
#define DET_BUFFERSIZE                      [!"num:integer(BufferSize)"!]U

#if (defined DET_KEEP_FIRST_REPORTS)
#error DET_KEEP_FIRST_REPORTS is already defined
#endif

/** \brief Possible value for ::DET_REEPORTS_TO_KEEP
 **
 ** Only the first ::DET_BUFFERSIZE reports are stored and all others are
 ** discarded. */
#define DET_KEEP_FIRST_REPORTS              1U

#if (defined DET_KEEP_LAST_REPORTS)
#error DET_KEEP_LAST_REPORTS is already defined
#endif

/** \brief Possible value for ::DET_REEPORTS_TO_KEEP
 *
 * The last ::DET_BUFFERSIZE reports are kept. */
#define DET_KEEP_LAST_REPORTS               2U

#if (defined DET_REPORTS_TO_KEEP)
#error DET_REPORTS_TO_KEEP is already defined
#endif

/** \brief Configuration option indicating which messages to keep on overflow.
 **
 ** Allowed values: ::DET_KEEP_FIRST_REPORTS and ::DET_KEEP_LAST_REPORTS. */
#define DET_REPORTS_TO_KEEP                 [!//
[!IF "KeepFirst = 'true'"!]DET_KEEP_FIRST_REPORTS[!ELSE!]DET_KEEP_LAST_REPORTS[!ENDIF!]
[!ENDIF!][!// /* Logging mode */

#if (defined DET_REPORT_ERROR_STOP_EXECUTION)
#error DET_REPORT_ERROR_STOP_EXECUTION already defined
#endif
/** \brief Configuration option if Det_ReportError() shall stop execution */
#define DET_REPORT_ERROR_STOP_EXECUTION   [!//
[!IF "EnableReportErrorStopExecution = 'true'"!]TRUE[!ELSE!]FALSE[!ENDIF!]
[!ENDSELECT!][!//
/*------------------[defensive programming]---------------------------------*/
[!SELECT "DetDefensiveProgramming"!][!//

#if (defined DET_DEFENSIVE_PROGRAMMING_ENABLED)
#error DET_DEFENSIVE_PROGRAMMING_ENABLED is already defined
#endif

/** \brief Defensive programming usage
 **
 ** En- or disables the usage of the defensive programming handling by the
 ** Det */
#define DET_DEFENSIVE_PROGRAMMING_ENABLED   [!//
[!IF "DetDefensiveProgrammingEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DET_PRECONDITION_ASSERT_ENABLED)
#error DET_PRECONDITION_ASSERT_ENABLED is already defined
#endif

/** \brief Precondition assertion usage
 **
 ** En- or disables the usage of precondition assertion check handling by the
 ** Det */
#define DET_PRECONDITION_ASSERT_ENABLED     [!//
[!IF "(DetDefensiveProgrammingEnabled = 'true') and (DetPreconditionAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DET_POSTCONDITION_ASSERT_ENABLED)
#error DET_POSTCONDITION_ASSERT_ENABLED is already defined
#endif

/** \brief Postcondition assertion usage
 **
 ** En- or disables the usage of postcondition assertion check handling by the
 ** Det */
#define DET_POSTCONDITION_ASSERT_ENABLED    [!//
[!IF "(DetDefensiveProgrammingEnabled = 'true') and (DetPostconditionAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DET_STATIC_ASSERT_ENABLED)
#error DET_STATIC_ASSERT_ENABLED is already defined
#endif

/** \brief Static assertion usage
 **
 ** En- or disables the usage of static assertion check handling by the Det */
#define DET_STATIC_ASSERT_ENABLED           [!//
[!IF "(DetDefensiveProgrammingEnabled = 'true') and (DetStaticAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DET_UNREACHABLE_CODE_ASSERT_ENABLED)
#error DET_UNREACHABLE_CODE_ASSERT_ENABLED is already defined
#endif

/** \brief Unreachable code assertion usage
 **
 ** En- or disables the usage of unreachable code assertion check handling by
 ** the Det */
#define DET_UNREACHABLE_CODE_ASSERT_ENABLED [!//
[!IF "(DetDefensiveProgrammingEnabled = 'true') and (DetUnreachableCodeAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DET_INVARIANT_ASSERT_ENABLED)
#error DET_INVARIANT_ASSERT_ENABLED is already defined
#endif

/** \brief Invariant code assertion usage
 **
 ** En- or disables the usage of invariant assertion check handling */
#define DET_INVARIANT_ASSERT_ENABLED        [!//
[!IF "(DetDefensiveProgrammingEnabled = 'true') and (DetInvariantAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!ENDSELECT!][!//

/*------------------[Det Multi-Core implementation macros]------------------*/

[!VAR "MULTICORE_ENABLED" = "node:exists(as:modconf('Os')[1]/OsOS/OsNumberOfCores) and node:value(as:modconf('Os')/OsOS/OsNumberOfCores) > 1"!]

/* !LINKSTO Det.EB.req.multicore.EnabledStatus,1 */
#if (defined DET_MULTICORE_ENABLED)
#error DET_MULTICORE_ENABLED already defined
#endif
/** \brief Preprocessor macro verifying if the multicore functionality is enabled. */
#define DET_MULTICORE_ENABLED [!IF "$MULTICORE_ENABLED"!]TRUE[!ELSE!]FALSE[!ENDIF!]

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
#endif /* ifndef DET_CFG_H */
