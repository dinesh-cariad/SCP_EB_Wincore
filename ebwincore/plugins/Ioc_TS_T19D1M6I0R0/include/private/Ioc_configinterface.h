/* Ioc_configinterface.h
 *
 * This file defines the ConfigInterface of the IOC.
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_configinterface.h 2250 2017-09-01 09:04:20Z olme8414 $
*/

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 8.11 (required)
 *  When an array with external linkage is declared , its size shall be
 *  explicitly specified.
 *
 * Reason:
 *  The size of this array is not known at compile time of the library code.
 *
 * MISRAC2012-2) Deviated Rule: 19.2 (advisory)
 *  The union keyword should not be used.
 *
 * Reason:
 *  The use of a union is mandated by the design.
 *  For each instance of this union only one of the members is used,
 *  depending on the type of the containing channel.
*/

#ifndef IOC_CONFIGINTERFACE_H
#define IOC_CONFIGINTERFACE_H

#include <private/Ioc_types_forward.h>
#include <public/Ioc_basic_types.h>
#include <private/Ioc_lockinterface.h>
#include <private/Ioc_bufferinterface.h>
#include <private/Ioc_queueinterface.h>
#include <private/Ioc_osinterface.h>

#ifndef IOC_ASM

/* Type to define the communication semantics of a channel
 */
enum ioc_comm_semantics_e
{
	IOC_QUEUED = 0,
	IOC_QUEUED_EXT,
	IOC_UNQUEUED,
	IOC_UNQUEUED_EXT
};
typedef enum ioc_comm_semantics_e ioc_comm_semantics_t;

/* Type to store the access control list on a channel
 *
 * We use a 32-bit bitfield, which limits the number of OS-Applications in the
 * system to 32.
 */
typedef ioc_uint32_t ioc_acl_t;

/* IOC_GetAppAccessBit determines the access bit of the given application. */
#define IOC_GetAppAccessBit(app) ((ioc_acl_t)(((ioc_acl_t)1u) << (app)))

/* Union type for the channel end point configuration */
/* Deviation MISRAC2012-2 <2> */
typedef union ioc_state_u ioc_state_t;
union ioc_state_u
{
	ioc_bufferstate_t	bufState;
	ioc_queueendstate_t	queueState;
};

/* Type for the configuration of a channel end point.
 */
typedef struct ioc_channelendconfig_s ioc_channelendconfig_t;
struct ioc_channelendconfig_s
{
	ioc_boolean_t		trapping;
	ioc_acl_t			accessors;
	ioc_state_t		*	state;
};

/* Macro that can be used to initialize an empty channel end config. */
#define IOC_EMPTY_CHANNEL_END_CONFIG_INIT {\
	IOC_FALSE,                              /* trapping */ \
	0U,                                     /* accessors */ \
	IOC_NULL                                /* state */ \
}

/* Type for the configuration of a channel
 *
 * Separation of the intra and cross-core locks allows combining
 * arbitrary intra/cross-core lock types.
 */
struct ioc_config_s
{
	void					*	buffer;
	ioc_uint32_t				bufferSize;
	ioc_channelendconfig_t		readerConfig;
	ioc_channelendconfig_t		writerConfig;
	ioc_locktype_t				writerLockTypeLocalCore;
	void					*	writerLockLocalCore;
	ioc_locktype_t				writerLockTypeCrossCore;
	volatile void			*	writerLockCrossCore;
	ioc_ilength_t const		*	dataElemLengths;
	ioc_uint32_t const		*	dataElemOffsets;
	ioc_uint16_t				numQueueEntries;
	ioc_comm_semantics_t		commSemantics;
	ioc_uint8_t					numDataElements;
	const void				*	initData;
};

/* Macro that can be used to initialize an empty channel. */
#define IOC_EMPTY_CHANNEL_CONFIG_INIT {\
	IOC_NULL,                          /* buffer */ \
	0U,                                /* bufferSize */ \
	IOC_EMPTY_CHANNEL_END_CONFIG_INIT, /* readerConfig */ \
	IOC_EMPTY_CHANNEL_END_CONFIG_INIT, /* writerConfig */ \
	IOC_LOCKINIT_NO_LOCK,              /* writerLockTypeLocalCore */ \
	IOC_NULL,                          /* writerLockLocalCore */ \
	IOC_LOCKINIT_NO_LOCK,              /* writerLockTypeCrossCore */ \
	IOC_NULL,                          /* writerLockCrossCore */ \
	IOC_NULL,                          /* dataElemLengths */ \
	IOC_NULL,                          /* dataElemOffsets */ \
	0U,                                /* numQueueEntries */ \
	IOC_QUEUED,                        /* commSemantics */ \
	0U,                                /* numDataElements */ \
	IOC_NULL                           /* initData */ \
}

/* Type to tell, how a certain application accesses a certain channel.
*/
enum ioc_accessor_e
{
	IOC_ACCESSOR_NONE,
	IOC_ACCESSOR_RECEIVER,
	IOC_ACCESSOR_SENDER,
	IOC_ACCESSOR_TRAPPING_SENDER
};
typedef enum ioc_accessor_e ioc_accessor_t;

extern const ioc_uint32_t IOC_nChannels;
/* Deviation MISRAC2012-1 */
extern const ioc_config_t IOC_channelConfigurations[];

const ioc_config_t * IOC_CH_GetChannelConfigById(ioc_channelid_t, ioc_comm_semantics_t);
ioc_accessor_t IOC_CH_GetAccessorTypeOfApp(const ioc_config_t *, ioc_os_application_t);
ioc_boolean_t IOC_CH_CheckLengths(const ioc_config_t *, const ioc_extinput_t *);

ioc_return_t IOC_CheckSyscallSource(const ioc_config_t *, const void *);
ioc_return_t IOC_CheckSyscallSourceExt(const ioc_config_t *, const ioc_extinput_t *);

#endif /* IOC_ASM */

#endif /* IOC_CONFIGINTERFACE_H */

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
