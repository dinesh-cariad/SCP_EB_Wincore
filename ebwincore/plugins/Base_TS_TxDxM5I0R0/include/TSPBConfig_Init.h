#ifndef TS_PBCONFIG_INIT_H
#define TS_PBCONFIG_INIT_H

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

/*==================[inclusions]=============================================*/

#include <TSPBConfig_BasicTypes.h> /* basic types for post build config */
#include <TSPBConfig_OffsetAlignment.h> /* for TS_OFFSETOF */

/*==================[macros]=================================================*/

#if (defined TS_MAKEREF2VAR) /* to prevent double definition */
#error TS_MAKEREF2VAR already defined
#endif /* if (defined TS_MAKEREF2VAR) */

/** \brief Create a post-build able reference to a RAM element
 *
 * This macro creates a post-build able reference to the given
 * RAM element.
 *
 * \param[in] offset Offset in post build RAM to which the reference shall be
 *            generated. */
#define TS_MAKEREF2VAR(offset) (offset)


#if (defined TS_MAKENULLREF2VAR) /* to prevent double definition */
#error TS_MAKENULLREF2VAR already defined
#endif /* if (defined TS_MAKENULLREF2VAR) */

/** \brief Create an invalid post-build able reference to a RAM element
 *
 * This macro creates a invalid post-build able reference to the given
 * RAM element.
 *
 * \param[in] offset Offset in post build RAM to which the reference shall be
 *            generated. */
#define TS_MAKENULLREF2VAR TS_VAR_OFFSET_TYPE_MAX

#endif /* ifndef TS_PBCONFIG_INIT_H */

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

#undef TS_MAKEREF2CFG
#undef TS_MAKENULLREF2CFG


#if (TS_RELOCATABLE_CFG_ENABLE == STD_ON)

/** \brief Create a post-build able reference to a config element
 *
 * This macro creates a post-build able reference to the given
 * config element.
 *
 * \param[in] member Post build structure member to which the reference
 *            shall be generated.
 */
#define TS_MAKEREF2CFG(member) \
    ((TS_CfgOffsetType) TS_OFFSETOF(TS_PB_CFG_LAYOUT_TYPE, member))

#elif (TS_RELOCATABLE_CFG_ENABLE == STD_OFF)

/** \brief Create a post-build able reference to a config element
 *
 * This macro creates a post-build able reference to the given
 * config element.
 *
 * \param[in] member Post build structure member to which the reference
 *            shall be generated.
 */

#define TS_MAKEREF2CFG(member) \
    (&(TS_PB_CFG_NAME.member))

#endif /* (TS_RELOCATABLE_CFG_ENABLE == STD_ON) */


#if (TS_RELOCATABLE_CFG_ENABLE == STD_ON)

/** \brief Create an invalid post-build able reference to a config element
 *
 * This macro creates an invalid post-build able reference to the given
 * config element.
 *
 * \param[in] cfgbase Start address of the module's config.
 * \param[in] cfgtype C datatype of the config layout structure.
 * \param[in] member Post build structure member to which the reference
 *            shall be generated.
 */
#define TS_MAKENULLREF2CFG   \
    ((TS_CfgOffsetType) 0U)

#elif (TS_RELOCATABLE_CFG_ENABLE == STD_OFF)

/** \brief Create an invalid post-build able reference to a config element
 *
 * This macro creates an invalid post-build able reference to the given
 * config element.
 *
 * \param[in] cfgbase Start address of the module's config.
 * \param[in] cfgtype C datatype of the config layout structure.
 * \param[in] member Post build structure member to which the reference
 *            shall be generated.
 */
#define TS_MAKENULLREF2CFG    \
    NULL_PTR

#endif /* (TS_RELOCATABLE_CFG_ENABLE == STD_ON) */

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[external data]==========================================*/
