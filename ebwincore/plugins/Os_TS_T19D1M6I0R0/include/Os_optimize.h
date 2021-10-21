/* Os_optimize.h
 *
 * This file includes the generated optimisation header and defines
 * macros in an optimized way depending on the excluded features.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 * CHECK: RULE 303 OFF (Checker doesn't like it that we skip out if OS_USE_OPTIMIZATION_OPTIONS isn't defined)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_optimize.h 27282 2017-08-24 13:57:43Z olme8414 $
*/

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 20.7 (required)
 * Expressions resulting from the expansion of macro parameters shall be
 * enclosed in parentheses.
 *
 * Reason:
 * Macro parameter is used in a way that doesn't allow the use of parentheses,
 * e.g. for designating declarations or initializers.
 */

#ifndef OS_OPTIMIZE_H
#define OS_OPTIMIZE_H

#include <Os_defs.h>
#include <Os_libconfig.h>
#include <Os_arch.h>

#if OS_USE_OPTIMIZATION_OPTIONS

#if OS_KERNEL_TYPE != OS_MICROKERNEL
#include <Os_errorcodes.h>

/* This error code translation macro for minimal error handling is valid for all SIDs
*/
#define OS_EC_OS_ERROR_NoError		OS_E_OK

/* we have to check OS_EXCLUDE_CALLINGCONTEXTCHECK before OS_EXCLUDE_ERRORHANDLING,
 * because the latter might want to change OS_CallingContextCheck as well...
*/
#ifdef OS_EXCLUDE_CALLINGCONTEXTCHECK
#define OS_CallingContextCheck()		1
#endif

#ifdef OS_EXCLUDE_ERRORHANDLING

/* If error handling is excluded we can exclude a whole lot of other things too.
 * When error handling is excluded the reaction to protection errors is unspecified.
 * In many cases an endless loop will occur as the processor constantly retries the
 * faulty instruction.
*/
#ifndef	OS_EXCLUDE_ERRORHANDLINGFORVOIDAPI
#define	OS_EXCLUDE_ERRORHANDLINGFORVOIDAPI	1
#endif

#ifndef	OS_EXCLUDE_ERRORACTION
#define	OS_EXCLUDE_ERRORACTION		1
#endif

#ifndef	OS_EXCLUDE_ERRORHOOK_APP
#define	OS_EXCLUDE_ERRORHOOK_APP	1
#endif

#ifndef	OS_EXCLUDE_PROTECTIONHOOK
#define	OS_EXCLUDE_PROTECTIONHOOK	1
#endif

#ifdef	OS_EXCLUDE_ERRORHOOK

/* Minimal error handling with no error hook defined.
 * In this case the error code is returned.
*/
#define OS_ERROR(e, p)				(OS_EC_##e)
#define OS_ERROR_NOCHECK(e,p)		((os_result_t)(e))

#else

#ifdef	OS_EXCLUDE_PARAMETERACCESS

/* Minimal error handling with error hook but no parameter access.
 * In this case a small handler is called that calls the error hook
 * with the "inFunction" indicator correctly set. The error code is returned.
*/
#define OS_ERROR(e, p)				OS_ErrorMinimal(OS_EC_##e)
#define OS_ERROR_NOCHECK(e,p)		OS_ErrorMinimal(e)

/* as the parameters are not saved, we can't check for the caller
 * in OS_CallingContextCheck (OS_GetKernelData()->errorStatus.calledFrom isn't set)
*/
#ifndef OS_CallingContextCheck
#define OS_CallingContextCheck()	((OS_permittedContext[OS_SID] & OS_INFUNCMASK(OS_GetKernelData()->inFunction)) != 0)
#endif

#else

/* Minimal error handling with error hook and parameter access.
 * In this case a small handler is called that calls the error hook
 * with the "inFunction" indicator correctly set. The error code is returned.
*/
#define OS_ERROR(e, p)				OS_ErrorMinimalParam(OS_SID, OS_EC_##e, p)
#define OS_ERROR_NOCHECK(e,p)		OS_ErrorMinimalParam(OS_SID, e, p)

#endif

#endif

/* These two macros simply convert internal errorcodes into OSEK (OS_E_xxx)
*/
#define OS_ERRORCODE_CHECK(e)		(OS_EC_##e)
#define OS_ERRORCODE_NOCHECK(e)		(OS_EC_##e)

#endif

#endif

#ifdef OS_EXCLUDE_APPLICATIONS
/* If there are no applications we can exclude all the application hook code too
*/
#ifndef	OS_EXCLUDE_ERRORHOOK_APP
#define	OS_EXCLUDE_ERRORHOOK_APP	1
#endif

#ifndef	OS_EXCLUDE_STARTUPHOOK_APP
#define	OS_EXCLUDE_STARTUPHOOK_APP	1
#endif

#ifndef	OS_EXCLUDE_SHUTDOWNHOOK_APP
#define	OS_EXCLUDE_SHUTDOWNHOOK_APP	1
#endif

#endif

#ifdef OS_EXCLUDE_ERRORACTION
#define OS_ERRORACTION(act, res)	(res)
#endif

/* Some architectures must prohibit stack checks at all costs, because they simply don't support them.
 * When this is the case, those architectures #define OS_DoStackCheck() appropriately.
 */
#ifndef OS_DoStackCheck
#ifdef OS_EXCLUDE_STACKCHECK
#define OS_DoStackCheck(x)	(0)
#else
#define OS_DoStackCheck(x)	((x) != 0)
#endif
#endif /* OS_DoStackCheck */

#ifdef OS_EXCLUDE_TIMINGPROTECTION
#define OS_DoTimingProtection(f)
#define OS_InitTaskEtb(tp)
#endif

#ifdef OS_EXCLUDE_RATEMONITORS
#define OS_RATEMONITORCHECK(rm)			0
#else
#define OS_RATEMONITORCHECK(rm)			(((rm) != OS_NULLRATEMONITOR) && (OS_RateMonitor((rm))))
#endif

#ifdef OS_EXCLUDE_PROTECTION
#define OS_DoMemoryProtection(x)
#endif

#ifdef OS_EXCLUDE_KILLABLE_ISR
#define OS_DoQueryIsr(x)				0
#define OS_DoMarkIsr(x)					do{OS_PARAM_UNUSED(x);} while(0)
#define OS_CallIsrDirectly()			1
#else
#define OS_CallIsrDirectly()			0
#endif

#ifdef OS_EXCLUDE_KILLABLE_APPSHOOK
#define OS_CallAppSHookDirectly()		1
#else
#define OS_CallAppSHookDirectly()		0
#endif

#ifdef OS_EXCLUDE_KILLABLE_APPEHOOK
#define OS_CallAppEHookDirectly()		1
#else
#define OS_CallAppEHookDirectly()		0
#endif

#if (OS_KERNEL_TYPE != OS_MICROKERNEL)
#ifdef OS_EXCLUDE_STARTUPHOOK
#define OS_CALLSTARTUPHOOK()

#ifdef OS_EXCLUDE_STARTUPHOOK_APP
/* We can only exclude all startup hook code if the app error hooks are also missing */
#define OS_IS_INSTARTUPHOOK(x)			0
#endif

#else
#define OS_CALLSTARTUPHOOK()			OS_CALLHOOKVOID_DIRECT(StartupHook, OS_INSTARTUPHOOK)
#endif
#endif

#ifdef OS_EXCLUDE_SHUTDOWNHOOK
#define OS_CALLSHUTDOWNHOOK(e)			OS_PARAM_UNUSED(e)

#ifdef OS_EXCLUDE_SHUTDOWNHOOK_APP
/* We can only exclude all shutdown hook code if the app error hooks are also missing */
#define OS_IS_INSHUTDOWNHOOK(x)			0
#endif

#else
#define OS_CALLSHUTDOWNHOOK(e)			OS_CALLHOOKPARAM_DIRECT(ShutdownHook, OS_INSHUTDOWNHOOK, (e))
#endif

#ifdef OS_EXCLUDE_ERRORHOOK
#define OS_CALLERRORHOOK(e)

#ifdef OS_EXCLUDE_ERRORHOOK_APP
/* We can only exclude all error hook code if the app error hooks are also missing */
#define OS_IS_INERRORHOOK(x)			0
#endif

#else
#define OS_CALLERRORHOOK(e)				OS_CALLHOOKPARAM_DIRECT(ErrorHook, OS_INERRORHOOK, (e))
#endif

#ifdef OS_EXCLUDE_PROTECTIONHOOK
#define OS_CALLPROTECTIONHOOK(e,r)		(r)	= OS_ACTION_SHUTDOWN
#else
#define OS_CALLPROTECTIONHOOK(e,r)		OS_CALLHOOKPARAMRETURN_DIRECT(ProtectionHook, OS_INPROTECTIONHOOK, (e), (r))
#endif

#ifdef OS_EXCLUDE_PRETASKHOOK
#define OS_CALLPRETASKHOOK()
#else
#define OS_CALLPRETASKHOOK()			OS_CALLHOOKVOID_DIRECT(PreTaskHook, OS_INPRETASKHOOK)
#endif

#ifdef OS_EXCLUDE_POSTTASKHOOK
#define OS_CALLPOSTTASKHOOK()
#else
#define OS_CALLPOSTTASKHOOK()			OS_CALLHOOKVOID_DIRECT(PostTaskHook, OS_INPOSTTASKHOOK)
#endif

#ifdef OS_EXCLUDE_PREISRHOOK
#define OS_CALLPREISRHOOK(i)
#else
#define OS_CALLPREISRHOOK(i)			OS_CALLHOOKPARAM_DIRECT(PreIsrHook, OS_INPREISRHOOK, (i))
#endif

#ifdef OS_EXCLUDE_POSTISRHOOK
#define OS_CALLPOSTISRHOOK(i)
#else
#define OS_CALLPOSTISRHOOK(i)			OS_CALLHOOKPARAM_DIRECT(PostIsrHook, OS_INPOSTISRHOOK, (i))
#endif

#ifdef OS_EXCLUDE_EXTRA_CHECK
#define OS_ExtraCheck(x)	0
#else
#define OS_ExtraCheck(x)	(x)
#endif

#if OS_KERNEL_TYPE != OS_MICROKERNEL
#ifdef OS_EXCLUDE_ERRORHANDLINGFORVOIDAPI
#define OS_ErrorHandlingForVoidApi()	0
#else
#define OS_ErrorHandlingForVoidApi()	1
#endif
#endif

#if OS_KERNEL_TYPE != OS_MICROKERNEL

#ifdef OS_EXCLUDE_INTSENABLEDCHECK
#define OS_InterruptEnableCheck(m)				1
#define OS_InterruptEnableCheckFromTask(m,td)	1
#endif

#endif

#ifdef OS_EXCLUDE_EXTENDED
#define OS_DoExtendedStatusTrue(x)				(1)
#define OS_DoExtendedStatusFalse(x)				(0)
#define OS_DoExtendedStatusVoid(x)
#endif

#if defined(OS_EXCLUDE_ERRORHOOK) || defined(OS_EXCLUDE_PARAMETERACCESS)
#define OS_PARAMETERACCESS_DECL
#define OS_SAVE_PARAMETER_N(n,x)	OS_PARAM_UNUSED(x)
#define OS_GET_PARAMETER_VAR()		OS_NULL
#endif

#if (defined(OS_EXCLUDE_PROTECTIONHOOK) && defined(OS_EXCLUDE_ERRORHOOK))  || defined(OS_EXCLUDE_PARAMETERACCESS)
#define OS_PH_PARAMETERACCESS_DECL
#define OS_PH_SAVE_PARAMETER_N(n,x)	OS_PARAM_UNUSED(x)
#define OS_PH_GET_PARAMETER_VAR()	OS_NULL
#endif

#if (defined(OS_EXCLUDE_ERRORHOOK_APP)) && (defined(OS_EXCLUDE_PROTECTIONHOOK)) && (defined(OS_EXCLUDE_ERRORHOOK))
#define OS_IS_ACTION_HOOK(h)	0
#endif

#ifdef OS_EXCLUDE_CAT2ISR
#define OS_IS_INCAT2(x)			0
#endif

#ifdef OS_EXCLUDE_APPLICATIONS
#if OS_KERNEL_TYPE != OS_MICROKERNEL
#define OS_CurrentApp()			OS_NULL
#define OS_HasPermission(app, perms)	1
#endif
#define OS_AppIsNull(a)			1
#define OS_QUARANTINEAPPLICATION(app)
#define OS_RESTARTAPPLICATION(app)
#define OS_CALLAPPSTARTUPHOOKS()
#define OS_CALLAPPSHUTDOWNHOOKS(ec)
#endif

#ifdef OS_EXCLUDE_EVENTS
#define OS_ClearPendingEvents(tp)
#endif

#ifdef OS_EXCLUDE_MULTIPLE_ACTIVATIONS
#define OS_FindPrio(q)						(OS_GetTaskPtrs(q)->dynamic->prio)
#define OS_CurrentLink(tp, td)				((tp)->linkBase)
#define OS_ClearCurrentActivation(td)
#define OS_IncCurrentActivation(tp,td)
#define OS_ActivationLimitExceeded(tp, td)	1
#define OS_ActivationPending(td)			(OS_FALSE)
#define OS_ChainTask(tp,td)					OS_Enqueue(tp)
#define	OS_PROTO_ENQUEUE					1
#endif

#ifdef OS_EXCLUDE_RESOURCEONISR
#define OS_IsIsrPrio(p)						0
#define OS_ResourceInvalidForIsr(isrp, rs)	1
#define OS_IsrTookLock(id, type)			0
#endif

#ifdef OS_EXCLUDE_USERTASKRETURN
#define OS_MttHandleError()		do { } while(0)
#endif

#if defined(OS_EXCLUDE_HWCOUNTER)
#define OS_CounterIsHw(cs)		0
#elif defined(OS_EXCLUDE_SWCOUNTER)
#define OS_CounterIsHw(cs)		1
#endif


/* There are no applications, therefore remove application entries in the data structures
 */
#ifdef OS_EXCLUDE_APPLICATIONS

/* Application (app) */
#define OS_Cfg_app_decl(x)
#define OS_Cfg_app_init(x)
#define OS_GET_APP(x)		OS_NULL
#define OS_GET_APPID(x)		OS_NULLAPP
#define OS_IsrIsTrusted(i)	OS_TRUE
#define OS_TaskIsTrusted(t)	OS_TRUE
#define OS_AppIsTrusted(a)	OS_TRUE

#endif

#ifdef OS_EXCLUDE_TIMINGPROTECTION

/* Accounting (acct) */
#define OS_Cfg_acct_decl(x)
#define OS_Cfg_acct_init(x)
#define OS_GET_ACCT(x)		OS_NULL
#define OS_SET_ACCT(x)		do { } while (0)

/* Timing protection (tp) */
#define OS_Cfg_tp_decl(x)
#define OS_Cfg_tp_init(x)
#define OS_GET_TP(x)			0

/* Interrupt lock monitoring (ilock) */
#define OS_Cfg_ilock_decl(x)
#define OS_Cfg_ilock_init(x)
#define OS_GET_ILOCK(x)		0

/* Resource lock monitoring (rlock) */
#define OS_Cfg_rlock_decl(x)
#define OS_Cfg_rlock_init(x)
#define OS_GET_RLOCK(x)		0
#define OS_SET_RLOCK(x)		do { } while (0)

#endif

#ifdef OS_EXCLUDE_RATEMONITORS

/* Rate monitoring (rmon) */
#define OS_Cfg_rmon_decl(x)
#define OS_Cfg_rmon_init(x)
#define OS_GET_RMON(x)		OS_NULL
#endif

#endif /* OS_USE_OPTIMIZATION_OPTIONS */

/*
 * Default macros if not already defined above.
 *
 * The following macros allow to remove certain parts of the configuration
 * data structures. If they are already defined (in optimize.h), the overridden
 * macros are used, in the generic case the following definitions are valid.
 */

/* Memory protection (mp) */
#ifndef OS_Cfg_mp_decl
#if (OS_KERNEL_TYPE==OS_SYSTEM_CALL) && (OS_HASMEMPROT == 1)
/* Deviation MISRAC2012-1 <+2> */
#define OS_Cfg_mp_decl(x)			x;
#define OS_Cfg_mp_init(x)			x,
#else
#define OS_Cfg_mp_decl(x)
#define OS_Cfg_mp_init(x)
#endif
#endif

/* Applications (app) */
#ifndef OS_Cfg_app_decl
/* Deviation MISRAC2012-1 <+2> */
#define OS_Cfg_app_decl(x)			x;
#define OS_Cfg_app_init(x)			x,
#endif

/* Extended status */
#if (!defined(OS_DoExtendedStatusTrue)) && (!defined(OS_DoExtendedStatusFalse)) && (!defined(OS_DoExtendedStatusVoid))
#define OS_DoExtendedStatusTrue(x)		(x)
#define OS_DoExtendedStatusFalse(x)		(x)
/* Deviation MISRAC2012-1 */
#define OS_DoExtendedStatusVoid(x)		x
#endif

/* Accounting (acct) */
#ifndef OS_Cfg_acct_decl
/* Deviation MISRAC2012-1 <+2> */
#define OS_Cfg_acct_decl(x)			x;
#define OS_Cfg_acct_init(x)			x,
#endif

/* Timing protection (tp) */
#ifndef OS_Cfg_tp_decl
/* Deviation MISRAC2012-1 <+2> */
#define OS_Cfg_tp_decl(x)			x;
#define OS_Cfg_tp_init(x)			x,
#endif

/* Interrupt lock timing (ilock) */
#ifndef OS_Cfg_ilock_decl
/* Deviation MISRAC2012-1 <+2> */
#define OS_Cfg_ilock_decl(x)		x;
#define OS_Cfg_ilock_init(x)		x,
#endif

/* Resource lock timing (rlock) */
#ifndef OS_Cfg_rlock_decl
/* Deviation MISRAC2012-1 <+2> */
#define OS_Cfg_rlock_decl(x)		x;
#define OS_Cfg_rlock_init(x)		x,
#endif

/* Rate monitoring (rmon) */
#ifndef OS_Cfg_rmon_decl
/* Deviation MISRAC2012-1 <+2> */
#define OS_Cfg_rmon_decl(x)			x;
#define OS_Cfg_rmon_init(x)			x,
#endif

#ifndef OS_GET_APP
#define OS_GET_APP(x)		(x)
#endif
#ifndef OS_GET_APPID
#define OS_GET_APPID(x)		(x)
#endif
#ifndef OS_GET_ACCT
#define OS_GET_ACCT(x)		(x)
#endif
#ifndef OS_SET_ACCT
/* Deviation MISRAC2012-1 */
#define OS_SET_ACCT(x)		do { x; } while (0)
#endif
#ifndef OS_GET_TP
#define OS_GET_TP(x)			(x)
#endif
#ifndef OS_GET_ILOCK
#define OS_GET_ILOCK(x)		(x)
#endif
#ifndef OS_GET_RLOCK
#define OS_GET_RLOCK(x)		(x)
#endif
#ifndef OS_SET_RLOCK
/* Deviation MISRAC2012-1 */
#define OS_SET_RLOCK(x)		do { x; } while (0)
#endif
#ifndef OS_SET_MP
#if (OS_KERNEL_TYPE==OS_SYSTEM_CALL) && (OS_HASMEMPROT == 1)
/* Deviation MISRAC2012-1 */
#define OS_SET_MP(x)		do { x; } while (0)
#else
#define OS_SET_MP(x)		do {} while (0)
#endif
#endif
#ifndef OS_GET_RMON
#define OS_GET_RMON(x)		(x)
#endif
#ifndef OS_IsrIsTrusted
#define OS_IsrIsTrusted(i)	( \
								((i)->app == OS_NULL) \
								|| ( ((i)->app->flags & OS_APP_TRUSTED) != 0 ) \
							)
#endif
#ifndef OS_TaskIsTrusted
#define OS_TaskIsTrusted(t)	( \
								((t)->app == OS_NULL) \
								|| ( ((t)->app->flags & OS_APP_TRUSTED) != 0 ) \
							)
#endif
#ifndef OS_AppIsTrusted
#define OS_AppIsTrusted(a)	( \
								OS_AppIsNull(a) \
								|| ( ((a)->flags & OS_APP_TRUSTED) != 0 ) \
							)
#endif

#endif
/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
