/* Ioc_u_getapplicationid.c
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_u_getapplicationid_user.c 2964 2018-11-14 14:05:00Z olme8414 $
*/
/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 12.3 (advisory)
 * The comma operator should not be used.
 *
 * Reason:
 * A function-call kernel maps API calls to the function OS_ArchLeaveKernel().
 * This function takes one argument, which is the return value of the requested
 * kernel service. The necessity to call OS_EnterKernel() before this service
 * requires the use of the comma operator. Otherwise, the API macros which are
 * used for this mapping do no longer expand into plain C-language expressions,
 * which is required here.
 *
 * MISRAC2012-2) Deviated Rule: 13.3 (advisory)
 * A full expression containing an increment (++) or decrement (--) operator
 * should have no other potential side effects other than that caused by the
 * increment or decrement operator.
 *
 * Reason:
 * The ++ operator is used in the context of OS_EnterKernel() for function-call
 * kernels. As function-call kernels use the comma operator, which is a
 * sequence point, to separate the increment operator from other expressions,
 * there is no undefined behavior.
*/

#include <private/Ioc_osinterface.h>
#include <Os.h>

/* IOC_GetApplicationId_User - Equivalent to GetApplicationID.
 *
 * This is just a helper function to keep the IOC's lib_src independent from
 * headers which depend on the OS configuration.
*/
ioc_os_application_t IOC_GetApplicationId_User(void)
{
	/* Deviation MISRAC2012-1, MISRAC2012-2 */
	return GetApplicationID();
}

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
