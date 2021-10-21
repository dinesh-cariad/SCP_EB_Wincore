/* Ioc_m_extentry_common.c
 *
 * This file defines functions used by the buffer/queue handler to transfer extended entries.
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_m_extentry_common.c 1817 2017-02-15 16:52:07Z mist9353 $
*/

#include <private/Ioc_extentry_common.h>
#include <private/Ioc_configinterface.h>
#include <private/Ioc_memcpy.h>

/* Reads a group of data elements from an entry of an extended channel.
 *
 * Parameters:
 *  - channel_config: The configuration of the channel, from whose buffer shall be read.
 *  - entry         : The queue entry or the buffer, from which shall be read.
 *  - lengths       : The addresses, to which the lengths shall be written, which were stored in the entry.
 *  - data          : The addresses, to which the data, stored in the entry, shall be written.
 *
 * Return value:
 *  - IOC_E_OK       : Current content of the given entry was transferred successfully.
 *  - IOC_E_SEG_FAULT: Length of a data element is corrupted in a way that causes buffer overflows.
 *
 * !LINKSTO IOC.Function.IOC_ReadFromExtEntry, 1
*/
ioc_return_t IOC_ReadFromExtEntry(
	const ioc_config_t *channel_config,
	const void *entry,
	ioc_ilength_t *lengths,
	void * const *data)
{
	ioc_return_t res;
	const ioc_ilength_t * const entryLengths = (const ioc_ilength_t *)entry;
	ioc_uint32_t curDataElement;
	/* Copy Data */
	res = IOC_E_OK;
	for (curDataElement = 0u; curDataElement < channel_config->numDataElements; curDataElement++)
	{
		/* Get the last length, that was written
		 * and check it, so defects in a sender applications don't cause buffer overflows
		 * in the receiving application.
		*/
		ioc_ilength_t curLength = entryLengths[curDataElement];
		if (curLength <= channel_config->dataElemLengths[curDataElement])
		{
			/* The offset of the current data element within the buffer */
			const ioc_uint32_t curDeOffset = channel_config->dataElemOffsets[curDataElement];
			/* Source of current data element */
			const void * const curSrc = &(((const ioc_uint8_t *)entry)[curDeOffset]);
			/* Copy data area determined by length */
			IOC_MemCpy(data[curDataElement], curSrc, curLength);
			/* Return the read length to the user */
			lengths[curDataElement] = curLength;
		}
		else
		{
			res = IOC_E_SEG_FAULT;
		}
	}
	return res;
}
