/* Ioc_public_api.h
 *
 * This file declares the public API that the user may use (directly or
 * indirectly) to call the IOC.
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_public_api.h 704 2016-05-03 07:43:48Z mist9353 $
*/
#ifndef IOC_PUBLIC_API_H
#define IOC_PUBLIC_API_H

#include <public/Ioc_basic_types.h>

#ifndef IOC_ASM

/* Configuration interface */
ioc_return_t IOC_Init(void);
ioc_return_t IOC_StartupChecks(void);
void IOC_InitializeWithoutChecks(void);

/* Unqueued Communication API */
ioc_return_t IOC_Read(ioc_channelid_t, void *);
ioc_return_t IOC_ReadExt(ioc_channelid_t, ioc_ilength_t *, void * const *);
ioc_return_t IOC_Write(ioc_channelid_t, const void *);
ioc_return_t IOC_WriteExt(ioc_channelid_t, const ioc_extinput_t *);
ioc_return_t IOC_ReInit(ioc_channelid_t);

/* Queued Communication API */
ioc_return_t IOC_Receive(ioc_channelid_t, void *);
ioc_return_t IOC_ReceiveExt(ioc_channelid_t, ioc_ilength_t *, void * const *);
ioc_return_t IOC_Send(ioc_channelid_t, const void *);
ioc_return_t IOC_SendExt(ioc_channelid_t, const ioc_extinput_t *);
ioc_return_t IOC_EmptyQueue(ioc_channelid_t);

#endif /* IOC_ASM */

#endif /* IOC_PUBLIC_API_H */

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
