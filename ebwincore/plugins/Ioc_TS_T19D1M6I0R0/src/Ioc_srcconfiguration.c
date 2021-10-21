/* Ioc_srcconfiguration.c
 *
 * This file defines the objects storing the IOC configuration.
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_srcconfiguration.c 3373 2019-10-08 11:25:19Z mabr2343 $
*/

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: EnumMixedWithOtherType
 *   Enumerated type mixed with another type.
 *
 * Reason:
 *   This warning is caused by the test sequence Ioc_F000_StartupChecksCommSemantics
 *   whose goal is to test the proper handling of an incorrect IOC configuration.
 *   For correct IOC configurations, this compiler warning doesn't appear.
 *
 * TOOLDIAG-2) Possible diagnostic: UndefinedEnumValue
 *   Enumerated type with undefined value.
 *
 * Reason:
 *   This warning is caused by the test sequence Ioc_F000_StartupChecksCommSemantics
 *   whose goal is to test the proper handling of an incorrect IOC configuration.
 *   For correct IOC configurations, this compiler warning doesn't appear.
*/

#include <private/Ioc_osinterface.h>
#include <private/Ioc_configinterface.h>
#include <Ioc_ChCfg.h>

const ioc_uint32_t IOC_nChannels = IOC_CFG_NCHANNELS;


#if (IOC_CFG_NCHANNELS > 0)

/* Possible diagnostic TOOLDIAG-1, TOOLDIAG-2 <+1> */
const ioc_config_t IOC_channelConfigurations[IOC_CFG_NCHANNELS] = IOC_CHANNEL_CONFIGURATIONS;

#if (IOC_LCFG_KERNEL_TYPE == IOC_MICROKERNEL)
const ioc_addressrange_t IOC_appReadableRegions[IOC_CFG_NREADABLEMKREGIONS] = IOC_CFG_READABLEMKREGIONS;
#endif


#else /* IOC_CFG_NCHANNELS */

/* IOC_channelConfigurations must be defined, so we add a dummy object here.
 * It won't be accessed, since IOC_nChannels is 0.
 * Strictly speaking, this wastes space. However, in this case, to save space,
 * the IOC can be completely removed from the project.
*/
const ioc_config_t IOC_channelConfigurations[1] = { IOC_EMPTY_CHANNEL_CONFIG_INIT };

#if (IOC_LCFG_KERNEL_TYPE == IOC_MICROKERNEL)
const ioc_addressrange_t IOC_appReadableRegions[1] = { IOC_EMPTY_ADDRESSRANGE_INIT };
#endif

#endif /* IOC_CFG_NCHANNELS */


/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
