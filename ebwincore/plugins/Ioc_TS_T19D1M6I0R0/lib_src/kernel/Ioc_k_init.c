/* Ioc_k_init.c
 *
 * This file contains the function IOC_Init()
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_k_init.c 2540 2018-02-28 09:03:10Z mist9353 $
*/

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 10.6 (required)
 *  The value of a composite expression shall not be assigned to an object
 *  with wider essential type.
 *
 * Reason:
 *  This is not an issue. uint32 will always be able to hold the size of ioc_ilength_t.
 *  This MISRA violation can't be fixed by explicitly casting this expression,
 *  because it is a complex expression and uint32 isn't narrower (see MISRA rule 10.3).
 *
 * MISRAC2012-2) Deviated Rule: 11.5 (advisory)
 *  A conversion should not be performed from pointer to void into pointer to
 *  object.
 *
 * Reason:
 *  A reinterpretation of the buffer is required, because the element's lengths
 *  are stored alongside their data.
 *
 * MISRAC2012-3) Deviated Rule 11.6 (required)
 *  A cast shall not be performed between pointer to void and an arithmetic type.
 *
 * Reason:
 *  The cast is required, because the given address is interpreted as integer to
 *  check its alignment. This check involves boolean operations, which are not
 *  suitable for pointer types.
 *
 * MISRAC2012-4) Deviated Rule 18.1 (required)
 *  A pointer resulting from arithmetic on a pointer operand shall address an
 *  element of the same array as that pointer operand.
 *
 * Reason:
 *  Application ids start at zero and run to 31 consecutively. Each application
 *  is assigned a bit in the 'accessors' bit-map according to its id. Hence,
 *  no bit at higher indices, than the number of applications minus one, can be
 *  set, unless there are configuration errors. In this case,
 *  VC.IOC_CHCFG_H.IOC_CFG_READABLEMKREGIONS precludes these.
*/

/* DCG Deviations:
 *
 * DCG-1) Deviated Rule: [OS_C_COMPL_010:calls]
 *  The code shall adhere to the [HISSRCMETRIC] Metrics.
 *
 * Reason:
 *  IOC_StartupChecks' only job is to call a few check functions.
 *  They are simply called one after another.
 *  Maintainability doesn't suffer from that, neither does comprehensibility.
*/
/* Deviation DCG-1 <*> */

#include <public/Ioc_public_api.h>
#include <private/Ioc_configinterface.h>
#include <private/Ioc_tool.h>
#include <public/Ioc_libconfig.h>
#include <private/Ioc_memcpy.h>
#include <private/Ioc_osinterface.h>


static ioc_boolean_t IOC_CheckBufferPtr(const ioc_config_t * channel_config);
static ioc_boolean_t IOC_CheckNumDataElements(const ioc_config_t * channel_config);
static ioc_boolean_t IOC_CheckBufferSize(const ioc_config_t * channel_config);
static ioc_boolean_t IOC_CheckStatePtr(const ioc_config_t * channel_config);
static ioc_boolean_t IOC_CheckNumQueueEntries(const ioc_config_t * channel_config);
static void IOC_InitChannel(const ioc_config_t * channel_config);
static ioc_boolean_t IOC_CheckLock(const ioc_config_t * channel_config);
static ioc_boolean_t IOC_CheckTrappingReader(const ioc_config_t * channel_config);
#if (IOC_LCFG_KERNEL_TYPE == IOC_MICROKERNEL)
static ioc_boolean_t IOC_MkCheckIntraLockType(const ioc_config_t * channel_config);
static ioc_boolean_t IOC_CheckReadableRegions(const ioc_config_t * channel_config);
#endif
static ioc_boolean_t IOC_CheckApplications(const ioc_config_t * channel_config);
static ioc_coreid_t IOC_GetInitCoreOfChannel(const ioc_config_t * channel_config);


/**** Internal functions ****/

/* IOC_CheckBufferPtr() checks if the buffer pointer is a null pointer
*/
static ioc_boolean_t IOC_CheckBufferPtr(const ioc_config_t * channel_config)
{
	ioc_comm_semantics_t commSem = channel_config->commSemantics;
	ioc_boolean_t success = IOC_FALSE;

	if (channel_config->buffer != IOC_NULL)
	{
		if ((commSem != IOC_QUEUED_EXT) && (commSem != IOC_UNQUEUED_EXT))
		{
			/* If the channel is simple (not extended) there are no alignment requirements. */
			success = IOC_TRUE;
		}
		/* Deviation MISRAC2012-3 <1> */
		else if (IOC_WellAlignedILength(channel_config->buffer))
		{
			/* Extended channels must be suitably aligned to hold length information. */
			success = IOC_TRUE;
		}
		else
		{
			/* This is an extended channel and its buffer pointer is misaligned.
			 * success is already FALSE.
			*/
		}
	}

	return success;
}

/* IOC_CheckNumDataElements() checks if number of data elements is correct.
*/
static ioc_boolean_t IOC_CheckNumDataElements(const ioc_config_t * channel_config)
{
	ioc_boolean_t ret = IOC_FALSE;

	switch (channel_config->commSemantics)
	{
	case IOC_QUEUED: /* Intentional fall-through */
	case IOC_UNQUEUED:
		if (channel_config->numDataElements == 1u)
		{
			ret = IOC_TRUE;
		}
		break;
	case IOC_QUEUED_EXT: /* Intentional fall-through */
	case IOC_UNQUEUED_EXT:
		if (channel_config->numDataElements >= 1u)
		{
			ret = IOC_TRUE;
		}
		break;
	default:
		/* Unexpected commSemantics value; ret is already FALSE */
		break;
	}
	return ret;
}

/* IOC_CheckBufferSize() checks if the buffer size is sufficient for
 * the configured data elements.
*/
static ioc_boolean_t IOC_CheckBufferSize(const ioc_config_t * channel_config)
{
	ioc_boolean_t ret = IOC_FALSE;
	ioc_uint32_t curElement;
	ioc_uint32_t expectedBufferSize = 0u;
	const ioc_uint32_t nElements = channel_config->numDataElements;

	if ( (channel_config->commSemantics == IOC_QUEUED_EXT)
		|| (channel_config->commSemantics == IOC_UNQUEUED_EXT) )
	{
		/* Required space for length */
		/* Deviation MISRAC2012-1 <1> */
		const ioc_uint32_t sizeofLength = sizeof(ioc_ilength_t);
		expectedBufferSize += nElements * sizeofLength;
	}
	for ( curElement = 0u; curElement < nElements; curElement++ )
	{
		expectedBufferSize += channel_config->dataElemLengths[curElement];
	}
	expectedBufferSize *= channel_config->numQueueEntries;

	/* Prevent performing a modulo 0 operation. This is also a violation that is
	 * also checked earlier, so there is no harm returning IOC_FALSE in this case.
	*/
	if (channel_config->numQueueEntries > 0)
	{
		/* Buffers which are greater than the sum of all data elements are allowed because
		 * the compiler might add gaps to fulfill alignment requirements. But the buffer size
		 * needs to be divisible by the number of queue entries to enable an efficient calculation
		 * of positions for each queue entry.
		 */
		if ((expectedBufferSize <= channel_config->bufferSize)
			&& ((channel_config->bufferSize % channel_config->numQueueEntries) == 0u))
		{
			ret = IOC_TRUE;
		}
	}
	return ret;
}

/* IOC_CheckStatePtr() checks if the state pointers in the writer channel end
 *  configurations do not point to NULL. For QUEUED channels, also the receiver
 *  state pointer is checked.
*/
static ioc_boolean_t IOC_CheckStatePtr(const ioc_config_t * channel_config)
{
	ioc_boolean_t ret = IOC_FALSE;

	if (channel_config->writerConfig.state != IOC_NULL)
	{
		const ioc_comm_semantics_t commSem = channel_config->commSemantics;

		ret = IOC_TRUE;
		if (((commSem == IOC_QUEUED) || (commSem == IOC_QUEUED_EXT))
			&& (channel_config->readerConfig.state == IOC_NULL))
		{
			ret = IOC_FALSE;
		}
	}
	return ret;
}

/* IOC_CheckNumQueueEntries() checks if the number of queue entries = 1 for
 *  UNQUEUED channels and >= 1 for QUEUED channels.
*/
static ioc_boolean_t IOC_CheckNumQueueEntries(const ioc_config_t * channel_config)
{
	ioc_boolean_t ret = IOC_FALSE;
	ioc_uint16_t channelNumQueueEntries = channel_config->numQueueEntries;
	ioc_comm_semantics_t channelCommSemantics = channel_config->commSemantics;

	if ((channelCommSemantics == IOC_QUEUED) || (channelCommSemantics == IOC_QUEUED_EXT))
	{
		if (channelNumQueueEntries >= 1u)
		{
			ret = IOC_TRUE;
		}
	}
	else
	{
		if (channel_config->numQueueEntries == 1u)
		{
			ret = IOC_TRUE;
		}
	}
	return ret;
}

static void IOC_InitChannel(const ioc_config_t * channel_config)
{
	const void *initData;

	initData = channel_config->initData;
	if (initData != IOC_NULL)
	{
		IOC_MemCpy(channel_config->buffer, initData, channel_config->dataElemLengths[0]);
	}

	if (channel_config->commSemantics == IOC_UNQUEUED_EXT)
	{
		ioc_uint32_t i;
		const ioc_uint32_t nElems = channel_config->numDataElements;
		/* Deviation MISRAC2012-2 <1> */
		ioc_ilength_t * const lastLengths = (ioc_ilength_t *) channel_config->buffer;
		for (i = 0; i < nElems; i++)
		{
			lastLengths[i] = channel_config->dataElemLengths[i];
		}
	}
}

/* IOC_CheckLock() checks the locks of a channel.
*/
static ioc_boolean_t IOC_CheckLock(const ioc_config_t * channel_config)
{
	ioc_boolean_t ret = IOC_TRUE;

	/* if a lock function is IOC_NULL,
	 * also the unlock function is IOC_NULL.
	*/
	if (channel_config->writerLockTypeLocalCore.lockfn == IOC_NULL)
	{
		if (channel_config->writerLockTypeLocalCore.unlockfn != IOC_NULL)
		{
			ret = IOC_FALSE;
		}
	}
	if (channel_config->writerLockTypeCrossCore.lockfn == IOC_NULL)
	{
		if (channel_config->writerLockTypeCrossCore.unlockfn != IOC_NULL)
		{
			ret = IOC_FALSE;
		}
	}

	return ret;
}

/* IOC_CheckTrappingReader() checks that readers do not use a trapping API, because this is not supported.
*/
static ioc_boolean_t IOC_CheckTrappingReader(const ioc_config_t * channel_config)
{
	return !channel_config->readerConfig.trapping;
}

#if (IOC_LCFG_KERNEL_TYPE == IOC_MICROKERNEL)
/* IOC_MkCheckIntraLockType() checks that if the IOC is used in combination with
 *  the microkernel, trapping APIs don't use LocalCore locks. This is not needed
 *  because the microkernel always runs with locked interrupts.
*/
static ioc_boolean_t IOC_MkCheckIntraLockType(const ioc_config_t * channel_config)
{
	ioc_boolean_t ret = IOC_TRUE;

	if ((channel_config->writerConfig.trapping)
		&& (channel_config->writerLockTypeLocalCore.lockfn != IOC_NULL))
	{
		ret = IOC_FALSE;
	}
	return ret;
}

/* IOC_CheckReadableRegions checks that there are valid readable regions for each trapping sender.
 *
 * All applications that are senders/writers of a trapping channel must have a valid non-empty readable region.
*/
static ioc_boolean_t IOC_CheckReadableRegions(const ioc_config_t * channel_config)
{
	/* We assume that CHAR_BIT equals 8. */
	const ioc_uint32_t widthOfAcl = (ioc_uint32_t)(sizeof(ioc_acl_t) * 8u);
	ioc_boolean_t ret = IOC_TRUE;

	if (channel_config->writerConfig.trapping)
	{
		ioc_acl_t accessors;
		ioc_uint32_t i;
		accessors = channel_config->writerConfig.accessors;

		for (i = 0; i < widthOfAcl; i++)
		{
			if (((accessors >> i) & 1u) != 0u)
			{
				/* Deviation MISRAC2012-4 <1> */
				const ioc_addressrange_t range = IOC_appReadableRegions[i];

				if (IOC_AddressRangeIsEmpty(range) || IOC_AddressRangeIsInvalid(range))
				{
					ret = IOC_FALSE;
				}
			}
		}
	}
	return ret;
}
#endif

/* IOC_CheckApplications checks whether the given applications are valid
 *
 * Only criteria:
 *  All given writer applications must be assigned to a core,
 *  because we need them to determine the core which initializes the writer state.
*/
static ioc_boolean_t IOC_CheckApplications(const ioc_config_t * channel_config)
{
	/* We assume that CHAR_BIT equals 8. */
	const ioc_uint32_t widthOfAcl = (ioc_uint32_t)(sizeof(ioc_acl_t) * 8u);
	ioc_acl_t accessors = channel_config->writerConfig.accessors;
	ioc_uint32_t i;
	ioc_boolean_t res = IOC_TRUE;

	for (i = 0; i < widthOfAcl; i++)
	{
		if (((accessors >> i) & 1u) != 0u)
		{
			ioc_os_application_t app = (ioc_os_application_t) i;

			if (IOC_GetAppCoreId(app) == IOC_INVALID_COREID)
			{
				res = IOC_FALSE;
				break;
			}
		}
	}

	return res;
}

/* IOC_GetInitCoreOfChannel gets the core which shall initialize the given channel.
 *
 * The receiver side doesn't need to be initialized.
 * One of the writer cores is chosen to initialize the channel.
 * If there is no writer, the channel is not initialized.
 * It doesn't matter if the chosen core is a lock-step core or not, because if a
 * non-lockstep core may write to this channel, it can corrupt this channel anyway.
*/
static ioc_coreid_t IOC_GetInitCoreOfChannel(const ioc_config_t * channel_config)
{
	/* We assume that CHAR_BIT equals 8. */
	const ioc_uint32_t widthOfAcl = (ioc_uint32_t)(sizeof(ioc_acl_t) * 8u);
	ioc_acl_t accessors = channel_config->writerConfig.accessors;
	ioc_uint32_t i;
	ioc_coreid_t res = IOC_INVALID_COREID;

	for (i = 0; i < widthOfAcl; i++)
	{
		if (((accessors >> i) & 1u) != 0u)
		{
			res = IOC_GetAppCoreId((ioc_os_application_t) i);
		}
	}

	return res;
}


/**** Externally visible functions ****/

/* IOC_Init() initializes the IOC and performs startup checks on the configuration.
 *
 * !LINKSTO IOC.API.IOC_Init, 2
*/
ioc_return_t IOC_Init(void)
{
	ioc_return_t ret = IOC_StartupChecks();

	if (ret == IOC_E_OK)
	{
		IOC_InitializeWithoutChecks();
	}
	return ret;
}

/* IOC_StartupChecks() performs startup checks on the configuration.
 *
 * !LINKSTO IOC.API.IOC_StartupChecks, 1
*/
ioc_return_t IOC_StartupChecks(void)
{
	ioc_boolean_t retOK = IOC_TRUE;
	ioc_return_t ret = IOC_E_NOK;
	ioc_uint32_t curChanIdx;

	for (curChanIdx = 0u; (curChanIdx < IOC_nChannels) && retOK; curChanIdx++)
	{
		const ioc_config_t * const chcfg = &IOC_channelConfigurations[curChanIdx];
		retOK = IOC_CheckBufferPtr(chcfg);
		retOK = IOC_CheckNumDataElements(chcfg) && retOK;
		retOK = IOC_CheckNumQueueEntries(chcfg) && retOK;
		retOK = IOC_CheckBufferSize(chcfg) && retOK;
		retOK = IOC_CheckStatePtr(chcfg) && retOK;
		retOK = IOC_CheckLock(chcfg) && retOK;
		retOK = IOC_CheckTrappingReader(chcfg) && retOK;
		retOK = IOC_CheckApplications(chcfg) && retOK;
#if (IOC_LCFG_KERNEL_TYPE == IOC_MICROKERNEL)
		retOK = IOC_MkCheckIntraLockType(chcfg) && retOK;
		retOK = IOC_CheckReadableRegions(chcfg) && retOK;
#endif
	}
	if (retOK)
	{
		ret = IOC_E_OK;
	}
	return ret;
}

/* IOC_InitializeWithoutChecks() initializes the IOC.
 *
 * !LINKSTO IOC.API.IOC_InitializeWithoutChecks, 2
*/
void IOC_InitializeWithoutChecks(void)
{
	ioc_uint32_t curChanIdx;
	ioc_coreid_t coreId;

	coreId = IOC_GetCoreId_Kernel();
	for (curChanIdx = 0u; curChanIdx < IOC_nChannels; curChanIdx++)
	{
		const ioc_config_t * const chcfg = &IOC_channelConfigurations[curChanIdx];

		if (coreId == IOC_GetInitCoreOfChannel(chcfg))
		{
			IOC_InitChannel(chcfg);
		}
	}
}

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
