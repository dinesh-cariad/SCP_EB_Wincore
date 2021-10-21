#ifndef ETH_TYPES_H
#define ETH_TYPES_H

/**
 * \file
 *
 * \brief AUTOSAR Eth
 *
 * This file contains the implementation of the AUTOSAR
 * module Eth.
 *
 * \version 0.0.8
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2011 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]============================================*/

#include <Eth_GeneralTypes.h>  /* AUTOSAR Ethernet general types. */
#ifndef ETH_NO_CFG_REQUIRED
/* Always include pre-compile-time information - needed by own and foreign modules */
/* don't mix PBcfg or Lcfg symbolic name values into Cfg since those headerfiles are included
 * anyway right afterwards
 */
#define ETH_NO_CFGCLASSMIX_REQUIRED
#include <Eth_Cfg.h>              /* pre-compile-time configuration */
#endif /* ETH_NO_CFG_REQUIRED */

/*==================[macros]================================================*/

#ifndef RxdStatsCrcAlignErrors
/** \brief Eth_RxStatsType was included in Eth_GeneralTypes.h file by Elektrobit long before it was
 *   specified in Autosar. The according RfC includes a typo (RxdStatsCrcAlignErrors) and it was
 *   used as it was defined there. In Eth_GeneralTypes.h this typo was fixed in Base version 5.0.28
 *   and additional a RxdStatsCrcAlignErrors define exists.
 *   This define is only created if RxdStatsCrcAlignErrors define does not exist and is used to
 *   prevent compile errors if this driver is used in an older ACG version */
#define RxStatsCrcAlignErrors RxdStatsCrcAlignErrors
#endif

#ifndef ETH_BUFIDXTYPE_INVALID
/** \brief Invalid buffer index (i.e. max value). Due to backward compatibility,
 * if the define is not part of the general Eth_GeneralTypes.h file it will be
 * defined in Eth_Types.h file.
 */
#define ETH_BUFIDXTYPE_INVALID ((Eth_BufIdxType) 0xFFFFFFFFU)
#endif

/*==================[type definitions]======================================*/
#ifndef ETH_RETRANSMITINFOTYPE_DEF
/** \brief Retransmit info type definition macro. Due to backward compartibility,
 * if this define is not part of the general Eth_GeneralTypes.h file it will be
 * defined in Eth_Types.h file.
 */
#define ETH_RETRANSMITINFOTYPE_DEF
/** \brief Retransmit info type (used for API Eth_Retransmit()) */
typedef struct {
  uint8 Priority;
  boolean TxConfirmation;
  P2VAR(uint8, TYPEDEF, AUTOSAR_COMSTACKDATA) PhysAddrPtr;
} Eth_RetransmitInfoType;
#endif

#ifndef ETH_BUFLISTTYPE_DEF
/** \brief Buffer list type definition macro. Due to backward compartibility,
 * if this define is not part of the general Eth_GeneralTypes.h file it will be
 * defined in Eth_Types.h file.
 */
#define ETH_BUFLISTTYPE_DEF
/** \brief Buffer list type (used for API Eth_TransmitBufList()). Due to backward
 * compartibility, if this struct is not part of the general Eth_GeneralTypes.h file
 * it will be defined in Eth_Types.h file.
 */
typedef struct
{
  P2VAR(Eth_DataType, TYPEDEF, AUTOSAR_COMSTACKDATA) BufPtr;
  uint16 LenByte;
} Eth_BufListType;
#endif


/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/**\brief @} doxygen end group definition */
#endif /* ifndef ETH_TYPES_H  */
/*==================[end of file]===========================================*/

