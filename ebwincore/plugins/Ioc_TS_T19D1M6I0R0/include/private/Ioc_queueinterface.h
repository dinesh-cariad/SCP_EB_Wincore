/* Ioc_queueinterface.h
 *
 * This file defines the QueueInterface of the IOC.
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_queueinterface.h 1691 2017-01-25 14:16:33Z stpo8218 $
*/
#ifndef IOC_QUEUEINTERFACE_H
#define IOC_QUEUEINTERFACE_H

#include <private/Ioc_types_forward.h>
#include <public/Ioc_basic_types.h>

#ifndef IOC_ASM

struct ioc_queueendstate_s
{
	volatile ioc_uint32_t	idx;
	ioc_uint32_t			lostDataCounter;
};

/* System call handlers */
ioc_return_t IOC_SysSend(ioc_channelid_t, const void *);
ioc_return_t IOC_SysSendExt(ioc_channelid_t, const ioc_extinput_t *);
ioc_return_t IOC_SysEmptyQueue(ioc_channelid_t);

/* Configuration handlers
 *
 * If a configuration handler does nothing else than calling the queue handler,
 * it is replaced by a macro.
*/
#define IOC_CH_Receive		IOC_QH_Receive
#define IOC_CH_ReceiveExt	IOC_QH_ReceiveExt
#define IOC_CH_Send			IOC_QH_Send
#define IOC_CH_SendExt		IOC_QH_SendExt
#define IOC_CH_EmptyQueue	IOC_QH_EmptyQueue

/* Queue handlers */
ioc_return_t IOC_QH_Receive(const ioc_config_t *, void *);
ioc_return_t IOC_QH_ReceiveExt(const ioc_config_t *, ioc_ilength_t *, void * const *);
ioc_return_t IOC_QH_Send(const ioc_config_t *, const void *);
ioc_return_t IOC_QH_SendExt(const ioc_config_t *, const ioc_extinput_t *);
ioc_return_t IOC_QH_EmptyQueue(const ioc_config_t *, ioc_boolean_t);

#endif /* IOC_ASM */

#endif /* IOC_QUEUEINTERFACE_H */

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
