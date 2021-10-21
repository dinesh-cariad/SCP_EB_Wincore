/* Ioc_types_forward.h - private types ("forward" declarations)
 *
 * This file declares the types for the private kernel structures without
 * declaring the structures, so that the types can be used as targets in pointer declarations.
 *
 * If the actual structure is needed the header file that declares the structure
 * should be included instead of this.
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_types_forward.h 2338 2017-10-11 05:10:25Z mabr2343 $
*/

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 5.7 (required)
 *  A tag name shall be a unique identifier.
 *
 * Reason:
 *  Information hiding. For example, the structure should be visible in a debugger but not in the
 *  application source code, or a pointer type is declared before the structure can be fully defined.
 *  This requires to reuse the same tag name in the structure definition.
 *  Note: MISRA-C:2012 Dir 4.8 advises that incomplete declarations should be used where possible
 *
 * MISRAC2012-2) Deviated Rule: 1.3 (required)
 *   There shall be no occurrence of undefined or critical unspecified behaviour.
 *
 * Reason:
 *  Type is incomplete.
 *  Corresponding source files only declare pointer of these types.
*/

#ifndef IOC_TYPES_FORWARD_H
#define IOC_TYPES_FORWARD_H

#ifndef IOC_ASM

/* Deviation MISRAC2012-1, MISRAC2012-2 <3> */
typedef struct ioc_queueendstate_s ioc_queueendstate_t;
typedef struct ioc_bufferstate_s ioc_bufferstate_t;
typedef struct ioc_config_s ioc_config_t;


#endif

#endif
/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
