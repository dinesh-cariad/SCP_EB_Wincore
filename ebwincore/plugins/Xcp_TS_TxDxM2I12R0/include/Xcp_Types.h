#ifndef XCP_TYPES_H
#define XCP_TYPES_H

/**
 * \file
 *
 * \brief AUTOSAR Xcp
 *
 * This file contains the implementation of the AUTOSAR
 * module Xcp.
 *
 * \version 2.12.0
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]============================================*/
#include <Std_Types.h> /* Autosar platform specific type declarations */
/*==================[macros]================================================*/

#if (defined XCP_OK)
#error XCP_OK already defined
#endif
/** \brief Xcp EB specific API return value when function was successful. */
#define XCP_OK                         0U

#if (defined XCP_NOT_OK)
#error XCP_NOT_OK already defined
#endif
/** \brief Xcp EB specific API return value when development error happened. */
#define XCP_NOT_OK                     1U

#if (defined XCP_NOT_INITIALIZED)
#error XCP_NOT_INITIALIZED already defined
#endif
/** \brief Xcp EB specific API return value when Xcp module is not initialized. */
#define XCP_NOT_INITIALIZED            2U

#if (defined XCP_NOT_CONNECTED)
#error XCP_NOT_CONNECTED already defined
#endif
/** \brief Xcp EB specific API return value when Xcp slave is not connected to the master. */
#define XCP_NOT_CONNECTED              3U

#if (defined XCP_OVERLOAD)
#error XCP_OVERLOAD already defined
#endif
/** \brief Xcp EB specific API return value when event is already pending (OVERLOAD situation). */
#define XCP_OVERLOAD                   4U

#if (defined XCP_NO_ACTIVE_LIST)
#error XCP_NO_ACTIVE_LIST already defined
#endif
/** \brief Xcp EB specific API return value when no active list associated (Xcp_SetEvent()). */
#define XCP_NO_ACTIVE_LIST             5U

#if (defined XCP_STIM_TIMEOUT)
#error XCP_STIM_TIMEOUT already defined
#endif
/** \brief Xcp EB specific API return value when not all stimulation packages are
 **        received (STIM_TIMEOUT situation). */
#define XCP_STIM_TIMEOUT               6U

/* !LINKSTO Xcp.Types.TransmissionModeType,1 */
/** \brief Transmission Disabled */
#define XCP_TX_OFF                     0U

/* !LINKSTO Xcp.Types.TransmissionModeType,1 */
/** \brief Transmission Enabled  */
#define XCP_TX_ON                      1U

/*==================[type definitions]======================================*/

/** \brief Type for the configuration data (place holder because of pre-compile
 **        time configuration support).
 */
typedef struct
{
  uint32 Dummy;   /**< Dummy variable to have valid C code */
} Xcp_ConfigType;

/** \brief Xcp return type for EB specific API functions */
typedef uint8 Xcp_ReturnType;

/** \brief Xcp_Transmission Mode Type: handles the enabling and disabling of the transmission mode
 ** Possible values can be XCP_TX_OFF and XCP_TX_ON
*/
typedef uint8 Xcp_TransmissionModeType;

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/

#endif /* ifndef XCP_TYPES_H */
