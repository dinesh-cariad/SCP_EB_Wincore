/**
 * \file
 *
 * \brief AUTOSAR Dem
 *
 * This file contains the implementation of the AUTOSAR
 * module Dem.
 *
 * \version 6.4.1
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

[!SKIPFILE "DemGeneral/DemIncludeProjectSpecificCustomizationSupport = 'false'"!]

#ifndef DEM_CUSTOMIZATION_H
#define DEM_CUSTOMIZATION_H

/* This file contains or includes all user defined identifiers of the AUTOSAR
 * module Dem. */

/*==================[inclusions]============================================*/

/* Instruction:
 * Should be used for including header-file where external functions are declared.
 *
 * Implementation rules:
 *  * no Dem internal-header file should be included
 *  * include only necessary header-files
 *  * consider splitting up the included header file if there are
 *    other external APIs declared inside
 *  * avoid circular inclusion
 */

/*==================[macros]================================================*/

/* Instruction:
 * Can be used for mapping Dem APIs to project specific implementation APIs.
 * e.g.:
 * Map dem-internal API Dem_Function to <Module>_UserFunction
 * #define Dem_Function <Module>_UserFunction
 */

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/* Instruction:
 * Provides the declaration of the external functions.
 * e.g.:
 * extern FUNC(void, DEM_CODE) <Module>_UserFunction();
 *
 * Implementation rule:
 *  * it is recommended to encapsulate the API implementation within a separate
 *    module and include the external header-files for the declaration of the
 *    external functions according to the guidelines in the inclusions section
 *  * ensure that function definition and declaration are included in the 
 *    same memory section
 *  * must not access any Dem internal data structures and functions because of
 *    concurrence limitations
 *  * ensure that external API provides functionality only when Dem module is in
 *    the correct initialization state (e.g.: initialized, pre-initialized) 
 */

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef DEM_CUSTOMIZATION_H */
/*==================[end of file]===========================================*/
