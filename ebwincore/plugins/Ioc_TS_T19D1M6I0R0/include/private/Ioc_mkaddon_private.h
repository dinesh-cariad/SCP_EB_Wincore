/* Ioc_mkaddon_private.h
 *
 * Declares the IOC's addon interface.
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_mkaddon_private.h 1847 2017-02-17 12:31:17Z mist9353 $
*/

#ifndef IOC_MKADDON_PRIVATE_H
#define IOC_MKADDON_PRIVATE_H

#include <public/Mk_public_types.h>
#include <public/Ioc_mkaddon_public_api.h>
#include <private/Mk_addon.h>

/* For Microkernel 2.x "add-on" framework
*/
void IOC_MkAddOnInit(mk_kernelcontrol_t *, const mk_addondescriptor_t *);
mk_statusandvalue_t IOC_MkAddOnControl(	mk_kernelcontrol_t *,
										const mk_addondescriptor_t *,
										mk_parametertype_t,
										mk_parametertype_t,
										mk_parametertype_t );
extern const mk_addondescriptor_t IOC_mkaddon_descriptor;

#endif
/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
