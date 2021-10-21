
#ifndef COMXF_TRACE_H
#define COMXF_TRACE_H

/**
 * \file
 *
 * \brief AUTOSAR ComXf
 *
 * This file contains the implementation of the AUTOSAR
 * module ComXf.
 *
 * \version 1.0.37
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]============================================*/

[!IF "node:exists(as:modconf('Dbg'))"!]
#include <Dbg.h>
[!ENDIF!]

/*==================[macros]================================================*/

#ifndef DBG_COMXF_GETVERSIONINFO_ENTRY
/** \brief Entry point of function ComXf_GetVersionInfo() */
#define DBG_COMXF_GETVERSIONINFO_ENTRY(a)
#endif

#ifndef DBG_COMXF_GETVERSIONINFO_EXIT
/** \brief Exit point of function ComXf_GetVersionInfo() */
#define DBG_COMXF_GETVERSIONINFO_EXIT(a)
#endif

#ifndef DBG_COMXF_INIT_ENTRY
/** \brief Entry point of function ComXf_Init() */
#define DBG_COMXF_INIT_ENTRY(a)
#endif

#ifndef DBG_COMXF_INIT_EXIT
/** \brief Exit point of function ComXf_Init() */
#define DBG_COMXF_INIT_EXIT(a)
#endif

[!LOOP "as:modconf('Xfrm')[name(../..)='ComXf']/XfrmImplementationMapping/*"!][!//
[!IF "(node:exists(XfrmTransformerBswModuleEntryRef))"!][!//
#ifndef DBG_[!"node:name(.)"!]_ENTRY
/** \brief Entry point of function [!"node:name(.)"!] */
#define DBG_[!"node:name(.)"!]_ENTRY(a,b,c)
#endif

#ifndef DBG_[!"node:name(.)"!]_EXIT
/** \brief Exit point of function [!"node:name(.)"!] */
#define DBG_[!"node:name(.)"!]_EXIT(a,b,c,d)
#endif

[!ENDIF!][!//
[!ENDLOOP!][!//

[!LOOP "as:modconf('Xfrm')[name(../..)='ComXf']/XfrmImplementationMapping/*"!][!//
[!IF "(node:exists(XfrmInvTransformerBswModuleEntryRef))"!][!//
#ifndef DBG_[!"node:name(.)"!]_ENTRY
/** \brief Entry point of function [!"node:name(.)"!] */
#define DBG_[!"node:name(.)"!]_ENTRY(a,b,c)
#endif

#ifndef DBG_[!"node:name(.)"!]_EXIT
/** \brief Exit point of function [!"node:name(.)"!] */
#define DBG_[!"node:name(.)"!]_EXIT(a,b,c,d)
#endif

[!ENDIF!][!//
[!ENDLOOP!][!//

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

#endif /* ifndef COMXF_TRACE_H */
/*==================[end of file]===========================================*/


