/* Ioc_mk_configuration.c - configuration for the IOC module
 *
 * This file contains the configuration for the inter os-application communicator add-on
 *
 * Configuration items:
 *	- an add-on descriptor containing information about the IOC module.
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_mk_configuration.c 2195 2017-07-18 08:33:58Z kosc261728 $
*/

#include <Mk_Cfg.h>
#include <public/Mk_public_api.h>
#include <public/Ioc_mkaddon_public_api.h>
#include <private/Ioc_mkaddon_private.h>
#include <private/Mk_addon.h>

#ifndef MK_ADDON_ID_IOC
#define MK_ADDON_ID_IOC (-1)
#endif

const mk_addondescriptor_t IOC_mkaddon_descriptor =
	MK_ADDONCONFIG(	"EB IOC add-on for microkernel 2.x",
					MK_ADDON_ID_IOC,
					IOC_MkAddOnControl,
					IOC_MkAddOnInit,
					MK_NULL);

const mk_objectid_t IOC_mkAddonId = MK_ADDON_ID_IOC;

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
