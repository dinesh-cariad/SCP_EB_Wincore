/* WINLIN-getstartoffilename.c
 *
 * This file contains the implementation of OS_WINLINGetStartOfFilename().
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: WINLIN-getstartoffilename.c 27660 2017-10-18 14:07:36Z olme8414 $
 */

#include <Os_kernel.h>
#include <string.h>

#include <memmap/Os_mm_code_begin.h>

/** \brief Determines the start of the filename component in \a path.
 *
 * \param path	A Windows or Unix path.
 *
 * \return A pointer to the start of the filename component in \a path or
 * OS_NULL if \a path does not identify a file.
 */
os_char_t const* OS_WINLINGetStartOfFilename(os_char_t const* path)
{
	os_char_t const* startOfFilename = OS_NULL;

	if (path != OS_NULL)
	{
		startOfFilename = strrchr(path, '\\');
		if (startOfFilename != OS_NULL)
		{
			++startOfFilename;	/* Advance p so that leading back-slash is not printed. */
		}
		else
		{
			startOfFilename = strrchr(path, '/');
			if (startOfFilename != OS_NULL)
			{
				++startOfFilename;	/* Advance p so that leading slash is not printed. */
			}
			else
			{
				startOfFilename = path;
			}
		}

		/* If 'path' ends with a directory separator it does not identify a file.
		 * In this case return OS_NULL.
		 */
		if ((startOfFilename != OS_NULL) && (startOfFilename[0] == '\0'))
		{
			startOfFilename = OS_NULL;
		}
	}

	return startOfFilename;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
