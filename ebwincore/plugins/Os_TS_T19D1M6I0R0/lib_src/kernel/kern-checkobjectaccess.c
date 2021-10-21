/* kern-checkobjectaccess.c
 *
 * This file contains the OS_CheckObjectAccess function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-checkobjectaccess.c 27926 2017-11-21 17:19:25Z mist9353 $
*/

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   The statements can be reached, if "error handling for APIs returning void" is not excluded.
 *   See also OS_EXCLUDE_ERRORHANDLINGFORVOIDAPI.
*/

#define OS_SID	OS_SID_CheckObjectAccess
#define OS_SIF	OS_svc_CheckObjectAccess

#include <Os_kernel.h>

/* Include definitions for tracing */
#include <Os_trace.h>

/* Include spinlocks, if this is a multicore system */
#if (OS_N_CORES > 1)
#include <Os_spinlock.h>
#endif

#include <memmap/Os_mm_code_begin.h>

/* MISRA-C checkers want prototypes for static helpers */
static os_boolean_t OS_GetAppPermissionsForObject
	(os_objecttype_t, os_objectid_t, os_permission_t *, os_paramtype_t *);

/* Helper to extract the application permissions of a given object.
 * If the id was valid, returns TRUE and fills the given permissions pointer.
*/
static os_boolean_t OS_GetAppPermissionsForObject
(	os_objecttype_t typ,
	os_objectid_t id,
	os_permission_t * perm,
	os_paramtype_t *p			/* to report errors */
)
{
	os_boolean_t ret = OS_FALSE;

	OS_PARAM_UNUSED(p);

	*perm = 0;	/* Initialize with 'deny all access'. */

	switch ( typ )
	{
	case OS_OBJ_APPLICATION:
		if ( OS_IsValidApplicationId(id) )
		{
			/* This grants access only to the application itself!
			*/
			*perm = OS_GET_APP(OS_appTableBase[id].permissions);
			ret = OS_TRUE;
		}
		break;

	case OS_OBJ_TASK:
		if ( OS_IsValidTaskId(id) )
		{
			*perm = OS_GET_APP(OS_taskTableBase[id].permissions);
			ret = OS_TRUE;
		}
		break;

	case OS_OBJ_ISR:
		if ( id < OS_nInterrupts )
		{
			*perm = OS_GET_APP(OS_isrTableBase[id].permissions);
			ret = OS_TRUE;
		}
		break;

	case OS_OBJ_RESOURCE:
		if ( id < OS_nResources )
		{
			*perm = OS_GET_APP(OS_resourceTableBase[id].permissions);
			ret = OS_TRUE;
		}
		break;

	case OS_OBJ_COUNTER:
		if ( id < OS_nCounters )
		{
			*perm = OS_GET_APP(OS_counterTableBase[id].permissions);
			ret = OS_TRUE;
		}
		break;

	case OS_OBJ_ALARM:
		if ( id < OS_nAlarms )
		{
			*perm = OS_GET_APP(OS_alarmTableBase[id].permissions);
			ret = OS_TRUE;
		}
		break;

	case OS_OBJ_SCHEDULETABLE:
		if ( id < OS_nSchedules )
		{
			*perm = OS_GET_APP(OS_scheduleTableBase[id].permissions);
			ret = OS_TRUE;
		}
		break;

	case OS_OBJ_TRUSTEDFUNCTION:
		if ( id < OS_nFunctions )
		{
			*perm = OS_GET_APP(OS_functionTableBase[id].permissions);
			ret = OS_TRUE;
		}
		break;

#if (OS_N_CORES > 1)
	case OS_OBJ_SPINLOCK:
		if ( OS_IsValidAutosarSpinlock(id) )
		{
			*perm = OS_GET_APP(OS_autosarSpinlockTableBase[id].accessPermissions);
			ret = OS_TRUE;
		}
		break;
#endif

	default:
		ret = OS_TRUE;								/* Avoid reporting "InvalidObjectId" */
		if ( OS_ErrorHandlingForVoidApi() )
		{
			/* Object-type is invalid.
			 *
			 * !LINKSTO Kernel.Autosar.API.SystemServices.CheckObjectAccess.Invalid, 1
			*/
			/* can't propagate the return value of OS_ERROR -> ignore it */
			/* Possible diagnostic TOOLDIAG-1 <+1> */
			(void) OS_ERROR(OS_ERROR_InvalidObjectType, OS_GET_PARAMETER_VAR());
		}
		break;
	}

	return ret;
}


/*!
 * OS_KernCheckObjectAccess implements the API CheckObjectAccess
 *
 * !LINKSTO Kernel.Autosar.ServiceErrors.AccessRights.QueryRights, 1
 * !LINKSTO Kernel.Autosar.API.SystemServices.CheckObjectAccess, 2
 *
 * There is no permission check here ...
 * !LINKSTO Kernel.Autosar.OsApplication.Permissions.Exceptions, 1
*/
os_boolean_t OS_KernCheckObjectAccess
(	os_applicationid_t	a,
	os_objecttype_t typ,
	os_objectid_t id
)
{
	os_boolean_t r = OS_FALSE;
	const os_appcontext_t *app;
	os_permission_t permissions;
	OS_PARAMETERACCESS_DECL

	OS_SAVE_PARAMETER_N(0,(os_paramtype_t)a);
	OS_SAVE_PARAMETER_N(1,(os_paramtype_t)typ);
	OS_SAVE_PARAMETER_N(2,(os_paramtype_t)id);

	OS_TRACE_CHECKOBJECTACCESS_ENTRY(a,typ,id);

	if ( !OS_CallingContextCheck() )
	{
		if ( OS_ErrorHandlingForVoidApi() )
		{
			/* can't propagate the return value of OS_ERROR -> ignore it */
			/* Possible diagnostic TOOLDIAG-1 <+1> */
			(void) OS_ERROR(OS_ERROR_WrongContext, OS_GET_PARAMETER_VAR());
		}
	}
	else
	if ( !OS_InterruptEnableCheck(OS_IEC_AUTOSAR) )
	{
		if ( OS_ErrorHandlingForVoidApi() )
		{
			/* can't propagate the return value of OS_ERROR -> ignore it */
			/* Possible diagnostic TOOLDIAG-1 <+1> */
			(void) OS_ERROR(OS_ERROR_InterruptDisabled, OS_GET_PARAMETER_VAR());
		}
	}
	else
	if ( !OS_IsValidApplicationId(a) )
	{
		/* Application is invalid.
		 *
		 * !LINKSTO Kernel.Autosar.API.SystemServices.CheckObjectAccess.Invalid, 1
		*/
		if ( OS_ErrorHandlingForVoidApi() )
		{
			/* can't propagate the return value of OS_ERROR -> ignore it */
			/* Possible diagnostic TOOLDIAG-1 <+1> */
			(void) OS_ERROR(OS_ERROR_InvalidApplicationId, OS_GET_PARAMETER_VAR());
		}
	}
	else
	{
		app = &OS_appTableBase[a];

		if ( OS_GetAppPermissionsForObject(typ, id, &permissions, OS_GET_PARAMETER_VAR()) )
		{
			/* permissions now holds the object's permissions */
			r = OS_HasPermission(app, permissions);
		}
		else
		{
			if ( OS_ErrorHandlingForVoidApi() )
			{
				/* Object is invalid.
				 *
				 * !LINKSTO Kernel.Autosar.API.SystemServices.CheckObjectAccess.Invalid, 1
				*/
				/* can't propagate the return value of OS_ERROR -> ignore it */
				/* Possible diagnostic TOOLDIAG-1 <+1> */
				(void) OS_ERROR(OS_ERROR_InvalidObjectId, OS_GET_PARAMETER_VAR());
			}
		}
	}

	OS_TRACE_CHECKOBJECTACCESS_EXIT_P(r,a,typ,id);
	return r;
}

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_UserCheckObjectAccess</name>
  <synopsis>Indicates whether an application has access to an object</synopsis>
  <syntax>
    os_result_t OS_UserCheckObjectAccess
    (   os_applicationid_t a,  /@ Application @/
        os_objecttype_t typ,   /@ Type of object @/
        os_objectid_t id,      /@ Object to check @/
    )
  </syntax>
  <description>
    <code>OS_UserCheckObjectAccess()</code> checks if the referenced
    application has access permission to the specified object.
    The applications permission mask is checked against the permission
    bits of the object. The function returns true (OS_TRUE) if access
    is granted and false (OS_FALSE) if access is denied. If either the
    application or the object does not exist the error handler is called
    and the return value is false. Since ISRs do not have a permissions field
    (no ACCESSING_APPLICATION in the configuration, the only application that
    can access an ISR is the owner. This is moot because there is no Autosar
    API that "accesses" the ISR.
  </description>
  <availability>
    No restrictions.
  </availability>
  <returns>OS_TRUE=Permission is granted</returns>
  <returns>OS_FALSE=Permission is not granted</returns>
</api>

 * CHECK: PARSE
*/

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
