/* Ioc_k_confighandler_common.c
 *
 * This file contains the functions implementing common functions of syscall
 * handlers to validate calls at runtime.
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_k_confighandler_common.c 2234 2017-08-04 13:18:30Z thob2262 $
*/

#include <public/Ioc_basic_types.h>
#include <public/Ioc_libconfig.h>
#include <private/Ioc_configinterface.h>
#include <private/Ioc_osinterface.h>
#include <private/Ioc_core.h>
#include <private/Ioc_tool.h>



static ioc_boolean_t IOC_CH_CheckExtSysArgs(ioc_os_application_t, const ioc_config_t *, const ioc_extinput_t *);


/* IOC_AppMayReadBytes - tells whether the given application may cause the kernel to read
 *                       from the given memory range.
 * It expects the size of the range in bytes.
 * The given 'app' must be the application of the current caller.
*/
#if (IOC_LCFG_KERNEL_TYPE == IOC_MICROKERNEL)

static ioc_boolean_t IOC_AppMayReadBytes(ioc_os_application_t, const void *, ioc_uint32_t);

static ioc_boolean_t IOC_AppMayReadBytes(ioc_os_application_t app, const void *vbegin, ioc_uint32_t sizeInByte)
{
	const ioc_address_t begin = (ioc_address_t)vbegin;
	const ioc_address_t end = begin + sizeInByte;
	const ioc_addressrange_t range = IOC_appReadableRegions[app];
	ioc_boolean_t res = IOC_FALSE;

	if ((begin <= end) && (range.begin <= begin) && (end <= range.end))
	{
		res = IOC_TRUE;
	}
	return res;
}

#elif (IOC_LCFG_KERNEL_TYPE == IOC_AUTOCOREOS)

#define IOC_AppMayReadBytes(app, begin, sizeInByte) \
	((OS_KernIocCheckMemoryAccess((begin), (sizeInByte)) & OS_MA_READ) == OS_MA_READ)

#else /* IOC_LCFG_KERNEL_TYPE */
#error "Unknown kernel type."
#endif /* IOC_LCFG_KERNEL_TYPE */


/* IOC_CH_CheckExtSysArgs - Checks the ioc_extinput_t argument, which is passed to system calls (extended channels).
 *
 * Checks whether 'elements' and its entries are accessible, when SendExt or WriteExt is called
 * from the given application 'app' on channel 'config'.
 *
 * Parameters:
 *	app             - the ID of the calling application.
 *	channel_config  - the configuration of the called channel.
 *	elements        - the argument passed to SendExt or WriteExt.
 *
 * Return value:
 *	- IOC_TRUE, if 'elements' may be read, IOC_FALSE otherwise.
*/
static ioc_boolean_t IOC_CH_CheckExtSysArgs(
	ioc_os_application_t app,
	const ioc_config_t *channel_config,
	const ioc_extinput_t *elements)
{
	const ioc_uint32_t nElements = channel_config->numDataElements;
	const ioc_boolean_t allownull = (channel_config->commSemantics == IOC_UNQUEUED_EXT);
	ioc_boolean_t validElements = IOC_FALSE;
	ioc_boolean_t mayread = IOC_AppMayReadBytes( app,
												 elements,
												 (ioc_uint32_t)(nElements * sizeof(ioc_extinput_t))
											   );
	ioc_boolean_t wellaligned = IOC_HwWellAlignedExtInput(elements);

#if (IOC_LCFG_KERNEL_TYPE == IOC_AUTOCOREOS)
	/* OS_KernIocCheckMemoryAccess determines the application by itself. */
	IOC_PARAM_UNUSED(app);
#endif

	/* Don't check the elements, if accessing 'elements' could cause a trap. */
	if (mayread && wellaligned)
	{
		ioc_uint32_t i;

		validElements = IOC_TRUE;
		for (i = 0; i < nElements; i++)
		{
			const ioc_extinput_t *pelem = &elements[i];

			if (pelem->data == IOC_NULL)
			{
				if (!allownull)
				{
					validElements = IOC_FALSE;
				}
			}
			else
			{
				/* The alignment of the pointers contained in the array is checked by IOC_MemCpy. */
				if (!IOC_AppMayReadBytes(app, pelem->data, pelem->length))
				{
					validElements = IOC_FALSE;
				}
			}
		}
	}

	return validElements;
}


/* IOC_CheckSyscallSource - Validates a system call to a basic channel at runtime.
 *
 * Return values:
 *  IOC_E_OK           - The call is allowed.
 *  IOC_E_SEG_FAULT    - The kernel-mode IOC is not allowed to read the given data,
 *                       when it is called by the given application.
 *  IOC_E_ILLEGAL_CALL - If the given application is not allowed to request services from this channel.
 *  IOC_E_NOK          - Something in the implementation is wrong.
 *
 * !LINKSTO IOC.Function.IOC_CheckSyscallSource, 1
*/
ioc_return_t IOC_CheckSyscallSource(const ioc_config_t *channel_config, const void *data)
{
	ioc_return_t ret = IOC_E_NOK;
	ioc_os_application_t app = IOC_GetApplicationId_Kernel();

	if ((app != IOC_INVALID_OSAPPLICATION)
		&& ((IOC_GetAppAccessBit(app) & channel_config->writerConfig.accessors) != 0u)
		&& channel_config->writerConfig.trapping)
	{
		if (IOC_AppMayReadBytes(app, data, channel_config->dataElemLengths[0]))
		{
			ret = IOC_E_OK;
		}
		else if ((channel_config->commSemantics == IOC_UNQUEUED)
					&& (data != IOC_NULL) && (data == channel_config->initData))
		{
			/* If we come here SysWrite was probably triggered by IOC_ReInit.
			 * This is on the slow path, since ReInit is expected to only be used in exceptional situations
			 * (e.g. when an application is restarted).
			*/
			ret = IOC_E_OK;
		}
		else
		{
			ret = IOC_E_SEG_FAULT;
		}
	}
	else
	{
		ret = IOC_E_ILLEGAL_CALL;
	}

	return ret;
}

/* IOC_CheckSyscallSourceExt - Validates a system call to an extended channel at runtime.
 *
 * Return values:
 *  IOC_E_OK           - The call is allowed.
 *  IOC_E_SEG_FAULT    - The kernel-mode IOC is not allowed to read the given data,
 *                       when it is called by the given application.
 *  IOC_E_ILLEGAL_CALL - If the given application is not allowed to request services from this channel.
 *  IOC_E_NOK          - Something in the implementation is wrong.
 *
 * !LINKSTO IOC.Function.IOC_CheckSyscallSourceExt, 2
*/
ioc_return_t IOC_CheckSyscallSourceExt(const ioc_config_t *channel_config, const ioc_extinput_t *elements)
{
	ioc_return_t ret = IOC_E_NOK;
	ioc_os_application_t app = IOC_GetApplicationId_Kernel();

	if ((app != IOC_INVALID_OSAPPLICATION)
		&& ((IOC_GetAppAccessBit(app) & channel_config->writerConfig.accessors) != 0u)
		&& channel_config->writerConfig.trapping)
	{
		if (IOC_CH_CheckExtSysArgs(app, channel_config, elements))
		{
			ret = IOC_E_OK;
		}
		else
		{
			ret = IOC_E_SEG_FAULT;
		}
	}
	else
	{
		ret = IOC_E_ILLEGAL_CALL;
	}

	return ret;
}

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
 */
