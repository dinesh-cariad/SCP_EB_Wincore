/**
 * \file
 *
 * \brief AUTOSAR Can
 *
 * This file contains the implementation of the AUTOSAR
 * module Can.
 *
 * \version 3.1.4
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef _VIRTUAL_BUS_PROTOCOL_H_
#define _VIRTUAL_BUS_PROTOCOL_H_

/*==[Includes]================================================================*/

/*==[Macros]==================================================================*/

/*==[Types]===================================================================*/
/** \brief Type containing the Return information */
typedef enum
{
    SD_ERROR = 0
  , SD_OK
  , SD_EMPTY
  , SD_NOTFOUND
}Sd_ReturnType;

/** \brief Type containing the Bus information */
typedef enum
{
    SD_UNKNOWN = 0
  , SD_FR1
  , SD_FR2
  , SD_CAN1
  , SD_CAN2
  , SD_CAN3
  , SD_CAN4
  , SD_LIN1
  , SD_LIN2
}Sd_BusType;

/** \brief Structure containing the Virtual Bus Data information */
typedef struct
{
   unsigned char  nBusID;
   unsigned char  nChannel;
   unsigned long  nID;
   unsigned char  nCycleExt;
   unsigned short nLength;
   unsigned char* pData;
}VirtualBusDataType;

/*==[Declaration of constants with external linkage]==========================*/

/*==[Declaration of variables with external linkage]==========================*/

/*==[Declaration of functions with external linkage]==========================*/

#endif /* _VIRTUAL_BUS_PROTOCOL_H_ */
