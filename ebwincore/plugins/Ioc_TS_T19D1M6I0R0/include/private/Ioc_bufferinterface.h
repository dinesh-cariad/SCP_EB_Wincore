/* Ioc_bufferinterface.h
 *
 * This file defines the BufferInterface of the IOC.
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_bufferinterface.h 1811 2017-02-15 11:56:21Z mist9353 $
*/
#ifndef IOC_BUFFERINTERFACE_H
#define IOC_BUFFERINTERFACE_H

#include <private/Ioc_types_forward.h>
#include <public/Ioc_basic_types.h>

#ifndef IOC_ASM

/* ioc_bufferstate_s
 *
 * A structure containing the number of started and completed write operations.
 */
struct ioc_bufferstate_s
{
	volatile ioc_uint32_t	writeBegin;		/* Number of started write operations */
	volatile ioc_uint32_t	writeEnd;		/* Number of completed write operations */
};

/* System call handlers */
ioc_return_t IOC_SysWrite(ioc_channelid_t, const void *);
ioc_return_t IOC_SysWriteExt(ioc_channelid_t, const ioc_extinput_t *);
ioc_return_t IOC_SysReInitZero(ioc_channelid_t);

/* Buffered Read API */
#define IOC_CH_Read			IOC_BH_Read
#define IOC_CH_ReadExt		IOC_BH_ReadExt
ioc_return_t IOC_BH_Read(const ioc_config_t *, void *);
ioc_return_t IOC_BH_ReadExt(const ioc_config_t *, ioc_ilength_t *, void * const *);

/* Buffered Write API */
#define IOC_CH_Write		IOC_BH_Write
#define IOC_CH_ReInitZero	IOC_BH_ReInitZero
#define IOC_CH_WriteExt		IOC_BH_WriteExt
ioc_return_t IOC_BH_Write(const ioc_config_t *, const void *);
ioc_return_t IOC_BH_WriteExt(const ioc_config_t *, const ioc_extinput_t *);
ioc_return_t IOC_BH_ReInitZero(const ioc_config_t *);

#endif /* IOC_ASM */

#endif

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
