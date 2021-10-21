#ifndef TS_PBCONFIG_OFFSET_ALIGNMENT_H
#define TS_PBCONFIG_OFFSET_ALIGNMENT_H

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

#include <stddef.h>    /* offsetof() */

#include <TSAutosar.h>

/*==================[macros]=================================================*/

/*------------------[alignment and offset macros]----------------------------*/

/** \brief Get the byte offset of a structure member
 *
 * This macro retrieves the byte offset of a given structure member from
 * the start of the structure
 *
 * \param[in] structure structure containing the member
 * \param[in] member member for which the byte offset within the structure
 *            shall be retrieved
 */
#define TS_OFFSETOF(structure, member) \
  (offsetof(structure, member))

/** \brief Get alignment of type within structures
 *
 * This macro retrieves the structure alignment requirements of a
 * given type
 *
 * \param[in] type type the alignment requirements shall be obtained for
 */
#define TS_ALIGNOF(type) \
    TS_OFFSETOF(struct {uint8 nMember1; type nMember2;}, nMember2)

/** \brief Get alignment of an array of type type within structures
 *
 * This macro retrieves the structure alignment requirements of an
 * array containing elements elements of type type
 *
 * \param[in] type type the alignment requirements shall be obtained for
 * \param[in] elements number of elements of type type in array within structure
 */
#define TS_ARRAYALIGNOF(type,elements) \
    TS_OFFSETOF(struct {uint8 nMember1; type nMember2[elements];}, nMember2)

/** \brief Check whether additional padding is inserted
 *
 * This macro checks whether additional padding bytes are inserted for
 * structures solely consisting of uint8/uint16/uint32 members
 */
#define TS_ADDITIONAL_PADDING_BYTES_INSERTED()                  \
    ((                                                          \
        (sizeof(struct {uint8 nMember;}) != sizeof(uint8)) ||   \
        (sizeof(struct {uint16 nMember;}) != sizeof(uint16)) || \
        (sizeof(struct {uint32 nMember;}) != sizeof(uint32))    \
    ) != 0U)

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[external data]==========================================*/

#endif /* ifndef TS_PBCONFIG_OFFSET_ALIGNMENT_H */
