/* lib-difftime32.h - Implementation of OS_DiffTime32().
 *
 * This file contains the implementation of the public API OS_DiffTime32().
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: lib-difftime32.c 28903 2018-07-04 12:06:04Z swho270744 $
*/

#include <Os_api.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_DiffTime32()
 *
 * Saturated 32-bit difference of two timestamps.
 *
 * This function calculates the 32-bit difference Of two timestamps.
 * If the result would not fit in a 32-bit value, the maximum 32-bit
 * value is returned.
 *
 * !LINKSTO	Kernel.Feature.TimeStamp.API.DiffTime32, 2
 * !doctype SRC
*/
os_uint32_t OS_DiffTime32(const os_timestamp_t *newTime, const os_timestamp_t *oldTime)
{
	os_timestamp_t diffTime;

	OS_TimeSub64(&diffTime, newTime, oldTime);

	if ( OS_TimeGetHi(diffTime) != 0 )
	{
		OS_TimeSetLo(diffTime,0xffffffffu);
	}

	return OS_TimeGetLo(diffTime);
}

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_DiffTime32</name>
  <synopsis>Calculates the 32-bit length of an interval between two times</synopsis>
  <syntax>
    os_uint32_t OS_DiffTime32
    (
      const os_timestamp_t *newTime, /@ new timestamp value @/
      const os_timestamp_t *oldTime  /@ old timestamp value @/
    )
  </syntax>
  <description>
    <para>
      <function>OS_DiffTime32()</function> calculates the difference
      (<parameter>newTime</parameter> - <parameter>oldTime</parameter>)
      (i.e. the duration of the interval that starts at
      <parameter>oldTime</parameter> and ends at <parameter>newTime</parameter>). The
      result is returned as 32-bit number. If the time difference is too large to be
      represented in 32 bits, the function returns the maximum value that can be
      represented (<literal>0xffffffff</literal>).
    </para>
  </description>
  <availability>
    No restrictions.
  </availability>
</api>

 * CHECK: PARSE
*/

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
