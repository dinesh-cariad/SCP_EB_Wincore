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
 * MISRA-C:2012 Deviations:
 *
 * MISRAC2012-1) Deviated Rule: 20.10 (advisory)
 * The # and ## preprocessor operators should not be used.
 *
 * Reason:
 * The concatenation operator is necessary for static dispatch.
 * Safe because of single concatenation and reserved namespace.
 *
 * MISRAC2012-2) Deviated Rule: 11.5 (advisory)
 * A conversion should not be performed from pointer to void into pointer to object.
 *
 * Reason:
 * The memory routines are optimized for dealing with aligned memory
 * sections.
 */

/*==================[includes]===============================================*/

#include <TSAutosar.h>
#include <ComStack_Helpers.h>

/*==================[functions]==============================================*/
#define BASE_START_SEC_CODE
#include <Base_MemMap.h>

/* !LINKSTO Base.ComStack.FindNextOne,1 */
FUNC(uint32, BASE_CODE) ComStack_FindNextOne
    (CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) bitfield, const uint32 prev)
{
  #if COMSTACK_ISA_WIDTH==64
    #define COMSTACK_LOWEST_BIT COMSTACK_LOWEST_BIT64
    typedef uint64 BF_type;
    enum
    {
      BF_LD_WIDTH= 6u
    };
  #else
    #define COMSTACK_LOWEST_BIT COMSTACK_LOWEST_BIT32
    typedef uint32 BF_type;
    enum
    {
      BF_LD_WIDTH= 5u
    };
  #endif
  typedef CONSTP2CONST(BF_type, AUTOMATIC, BASE_APPL_DATA) BF_ptr;
  const uint32 BF_MASK= (1u<<BF_LD_WIDTH)-1u;

  const uint32 start= prev+1u;
  uint32 bit= start & BF_MASK;
  register uint32 ofs= start >> BF_LD_WIDTH;
  /* Deviation MISRAC2012-2 */
  register BF_ptr bf= (BF_ptr) bitfield;

  /* Type expansion isn't an issue, if we talk about uint32 / uint64.
   * Still, MISRA forces us to saturate this with useless casts. */
  register BF_type bits= bf[ofs] & (BF_type)(((BF_type)(~(BF_type)0u)) << bit);
  uint32 result;
  while (bits==0u)
  {
    ++ofs;
    bits= bf[ofs];
  }

  /* Deviation MISRAC2012-1 */
  COMSTACK_LOWEST_BIT(result, bits);
  result |= ofs * (sizeof(BF_type) << 3u);
  return result;
}

#define BASE_STOP_SEC_CODE
#include <Base_MemMap.h>
/*==================[end of file]============================================*/
