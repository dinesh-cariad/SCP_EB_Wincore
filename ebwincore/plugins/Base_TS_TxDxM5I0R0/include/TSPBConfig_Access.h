#ifndef TS_PBCONFIG_ACCESS_H
#define TS_PBCONFIG_ACCESS_H

/**
 * \file
 *
 * \brief AUTOSAR Base
 *
 * This file contains the implementation of the AUTOSAR
 * module Base.
 *
 * \version 5.0.31
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 11.5 (advisory)
 * A conversion should not be performed from pointer to void into pointer to object.
 *
 * Reason:
 * The memory routines are optimized for dealing with aligned memory
 * sections.
 */

/*==================[inclusions]=============================================*/

#include <TSPBConfig_BasicTypes.h> /* basic types for post build config */

/*==================[macros]=================================================*/

/** \brief Get a const reference to a post-build RAM element - no
 *         check for invalid refs
 *
 * This macro retrieves a const pointer to a post-build RAM element
 * without checking for invalid references
 *
 * \param[in] rambase base address of the module's post-build RAM.
 * \param[in] reftype type of the referenced element in post-build RAM.
 * \param[in] module short name of the respective module (all caps),
 * e.g., PDUR
 * \param[in] offset offset in post build RAM of the elements to which the
 * reference shall be retrieved.
 */
/* Deviation MISRAC2012-1 */
#define TS_UNCHECKEDGETCONSTVAR(rambase, reftype, module, offset)       \
         ((CONSTP2VAR(reftype, AUTOMATIC, module ## _VAR_CLEARED))       \
            (CONSTP2VAR(void, AUTOMATIC, module ## _VAR_CLEARED))        \
          &(((CONSTP2VAR(uint8, AUTOMATIC, module ## _VAR_CLEARED))      \
             (CONSTP2VAR(void, AUTOMATIC, module ## _VAR_CLEARED))       \
             rambase) [(offset)]))

/** \brief Get a const reference to a post-build RAM element
 *
 * This macro retrieves a const pointer to a post-build RAM element
 *
 * \param[in] rambase base address of the module's post-build RAM.
 * \param[in] reftype type of the referenced element in post-build RAM.
 * \param[in] module short name of the respective module (all caps),
 * e.g., PDUR
 * \param[in] offset offset in post build RAM of the elements to which the
 * reference shall be retrieved.
 */

#define TS_CHECKEDGETCONSTVAR(rambase, reftype, module, offset)         \
        (((offset) == TS_VAR_OFFSET_TYPE_MAX) ?                         \
         ((CONSTP2VAR(reftype, AUTOMATIC, module ## _VAR_CLEARED)) 0) : \
         TS_UNCHECKEDGETCONSTVAR(rambase, reftype, module, offset))

/** \brief Get a non-const reference to a post-build RAM element - no
 *         check for invalid refs
 *
 * This macro retrieves a non-const pointer to a post-build RAM
 * element without checking for invalid references
 *
 * \param[in] rambase base address of the module's post-build RAM.
 * \param[in] reftype type of the referenced element in post-build RAM.
 * \param[in] module short name of the respective module (all caps),
 * e.g., PDUR
 * \param[in] offset offset in post build RAM of the elements to which the
 * reference shall be retrieved.
 */
/* Deviation MISRAC2012-1 */
#define TS_UNCHECKEDGETVAR(rambase, reftype, module, offset)    \
         ((P2VAR(reftype, AUTOMATIC, module ## _VAR_CLEARED))    \
            (P2VAR(void, AUTOMATIC, module ## _VAR_CLEARED))     \
          &(((P2VAR(uint8, AUTOMATIC, module ## _VAR_CLEARED))   \
             (P2VAR(void, AUTOMATIC, module ## _VAR_CLEARED))    \
             rambase) [(offset)]))

/** \brief Get a non-const reference to a post-build RAM element
 *
 * This macro retrieves a non-const pointer to a post-build RAM element
 *
 * \param[in] rambase base address of the module's post-build RAM.
 * \param[in] reftype type of the referenced element in post-build RAM.
 * \param[in] module short name of the respective module (all caps),
 * e.g., PDUR
 * \param[in] offset offset in post build RAM of the elements to which the
 * reference shall be retrieved.
 */
#define TS_CHECKEDGETVAR(rambase, reftype, module, offset)              \
        (((offset) == TS_VAR_OFFSET_TYPE_MAX) ?                         \
         ((P2VAR(reftype, AUTOMATIC, module ## _VAR_CLEARED)) 0) :      \
         TS_UNCHECKEDGETVAR(rambase, reftype, module, offset))

#endif /* ifndef TS_PBCONFIG_ACCESS_H */

/*
 * The following code is deliberately outside the inclusion guard,
 * since the user of this file should have the possibility to redfine
 * the following macros according to her setting of
 * TS_RELOCATABLE_CFG_ENABLE
 */

#ifndef TS_RELOCATABLE_CFG_ENABLE
#error TS_RELOCATABLE_CFG_ENABLE is not defined
#endif

#ifndef STD_ON
#error STD_ON is not defined
#endif

#ifndef STD_OFF
#error STD_OFF is not defined
#endif

#if ((TS_RELOCATABLE_CFG_ENABLE != STD_ON) && (TS_RELOCATABLE_CFG_ENABLE != STD_OFF))
#error TS_RELOCATABLE_CFG_ENABLE is neither STD_ON nor STD_OFF
#endif

#undef TS_UNCHECKEDGETCONSTCFG
#undef TS_CHECKEDGETCONSTCFG
#undef TS_UNCHECKEDGETCFG
#undef TS_CHECKEDGETCFG


#if (TS_RELOCATABLE_CFG_ENABLE == STD_ON)
/** \brief Get a const reference to a post-build config element - no
 *         check for invalid refs
 *
 * This macro retrieves a const pointer to a post-build config
 * element without checking for invalid references
 *
 * \param[in] cfgbase base address of the module's post-build configuration.
 * \param[in] reftype type of the referenced element in post-build
 * configuration.
 * \param[in] module short name of the respective module (all caps),
 * e.g., PDUR
 * \param[in] offset offset/pointer of/to the to element in the
 * post-build configuration
 */
#define TS_UNCHECKEDGETCONSTCFG(cfgbase, reftype, module, offset)       \
         ((CONSTP2CONST(reftype, AUTOMATIC, module ## _APPL_CONST))     \
            (CONSTP2CONST(void, AUTOMATIC, module ## _APPL_CONST))      \
          &(((CONSTP2CONST(uint8, AUTOMATIC, module ## _APPL_CONST))    \
             (CONSTP2CONST(void, AUTOMATIC, module ## _APPL_CONST))     \
             cfgbase) [(offset)]))

/** \brief Get a const reference to a post-build config element
 *
 * This macro retrieves a const pointer to a post-build config element
 *
 * \param[in] cfgbase base address of the module's post-build configuration.
 * \param[in] reftype type of the referenced element in post-build
 * configuration.
 * \param[in] module short name of the respective module (all caps),
 * e.g., PDUR
 * \param[in] offset offset/pointer of/to the to element in the
 * post-build configuration
 */
#define TS_CHECKEDGETCONSTCFG(cfgbase, reftype, module, offset)         \
        (((offset) == 0U) ?                                             \
         ((CONSTP2CONST(reftype, AUTOMATIC, module ## _APPL_CONST)) 0) : \
         TS_UNCHECKEDGETCONSTCFG(cfgbase, reftype, module, offset))

/** \brief Get a non-const reference to a post-build config element -
 *         no check for invalid refs
 *
 * This macro retrieves a non-const pointer to a post-build config
 * element without checking for invalid references
 *
 * \param[in] cfgbase base address of the module's post-build configuration.
 * \param[in] reftype type of the referenced element in post-build
 * configuration.
 * \param[in] module short name of the respective module (all caps),
 * e.g., PDUR
 * \param[in] offset offset/pointer of/to the to element in the
 * post-build configuration
 */
#define TS_UNCHECKEDGETCFG(cfgbase, reftype, module, offset)    \
         ((P2CONST(reftype, AUTOMATIC, module ## _APPL_CONST))  \
            (P2CONST(void, AUTOMATIC, module ## _APPL_CONST))   \
          &(((P2CONST(uint8, AUTOMATIC, module ## _APPL_CONST)) \
             (P2CONST(void, AUTOMATIC, module ## _APPL_CONST))  \
             cfgbase) [(offset)]))

/** \brief Get a non-const reference to a post-build config element
 *
 * This macro retrieves a non-const pointer to a post-build config element
 *
 * \param[in] cfgbase base address of the module's post-build configuration.
 * \param[in] reftype type of the referenced element in post-build
 * configuration.
 * \param[in] module short name of the respective module (all caps),
 * e.g., PDUR
 * \param[in] offset offset/pointer of/to the to element in the
 * post-build configuration
 */
#define TS_CHECKEDGETCFG(cfgbase, reftype, module, offset)              \
        (((offset) == 0U) ?                                             \
         (NULL_PTR) :                                                   \
         TS_UNCHECKEDGETCFG(cfgbase, reftype, module, offset))

#elif (TS_RELOCATABLE_CFG_ENABLE == STD_OFF)
#define TS_UNCHECKEDGETCONSTCFG(cfgbase, reftype, module, ptr) (ptr)
#define TS_CHECKEDGETCONSTCFG(cfgbase, reftype, module, ptr) (ptr)
#define TS_UNCHECKEDGETCFG(cfgbase, reftype, module, ptr) (ptr)
#define TS_CHECKEDGETCFG(cfgbase, reftype, module, ptr) (ptr)
#endif /* (TS_RELOCATABLE_CFG_ENABLE) */

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[external data]==========================================*/
