/**
 * \file
 *
 * \brief AUTOSAR FrTp
 *
 * This file contains the implementation of the AUTOSAR
 * module FrTp.
 *
 * \version 4.4.26
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* mgcc62 Deviation List
 *
 * MGCC62-1) Deviated Rule: redundant-decls
 * warning: redundant redeclaration [-Wredundant-decls]
 *
 * Reason:
 * FrTp_MainFunction can be redefined by the BSW Scheduler (SchM).
 */

#ifndef FRTP_API_H
#define FRTP_API_H

/*=====================[Include Section]=====================================*/

#include <ComStack_Types.h> /* basic commstack types from AUTOSAR */
#include <FrTp_Cfg.h>       /* static configuration of FrTp */ /* !LINKSTO FRTP1003, 1 */
#include <FrTp_Types.h>     /* Module specific extensions */
#include <FrTp_Extension.h> /* Module specific extensions */
#include <FrTp_Cbk.h>       /* callback function prototypes of FrTp */ /* !LINKSTO FRTP1135_1,1 */
#include <FrTp_Version.h>   /* Module version information */

#if (FRTP_PBCFGM_SUPPORT_ENABLED == STD_ON)
#include <PbcfgM_Api.h>         /* Post build configuration manager */
#endif /* FRTP_PBCFGM_SUPPORT_ENABLED */

/*=====================[Global Macros]=======================================*/

/*
 * Service identifiers for all API functions of the FrTp
 */

/*
 * Service identifiers for FrTp_GetVersionInfo API function
 */
#define FRTP_GETVERSIONINFO_SERVICE_ID             0x27U
/*
 * Service identifiers for FrTp_Init API function
 */
#define FRTP_INIT_SERVICE_ID                       0x00U
/*
 * Service identifiers for FrTp_Transmit API function
 */
#define FRTP_TRANSMIT_SERVICE_ID                   0x02U
/*
 * Service identifiers for FrTp_CancelTransmit API function
 */
#define FRTP_CANCELTRANSMIT_SERVICE_ID      0x03U
/*
 * Service identifiers for FrTp_ChangeParameter API
 */
#define FRTP_CHANGEPARAMETER_SERVICE_ID     0x04U
/*
 * Service identifiers for FrTp_CancelTransmit API function
 */
#define FRTP_CANCELRECEIVE_SERVICE_ID       0x08U
/*
 * Service identifiers for FrTp_TriggerTransmit API function
 */
#define FRTP_TRIGGERTRANSMIT_SERVICE_ID            0x41U
/*
 * Service identifiers for FrTp_RxIndication API function
 */
#define FRTP_RXINDICATION_SERVICE_ID               0x42U
/*
 * Service identifiers for FrTp_TxConfirmation API function
 */
#define FRTP_TXCONFIRMATION_SERVICE_ID             0x40U
/*
 * Service identifiers for FrTp_MainFunction API function
 */
#define FRTP_MAINFUNCTION_SERVICE_ID               0x10U
/*
 * Service identifiers for FrTp_IsValidConfig API function
 */
#define FRTP_ISVALIDCONFIG_SERVICE_ID              0x60U
/*
 * Service identifiers for API functions without specific SID
 */
#define FRTP_SID_UNKNOWN_API                       0xffU
/*
 * Following error identifiers for the Development Error Tracer are
 * used for the development phase.
 */
/*
 * API service called before initializing the module
 * be executed.
 */
#define FRTP_E_UNINIT 0x01U

/*
 * API service called with NULL pointer.
 */
#define FRTP_E_NULL_PTR 0x02U

/*
 * API service called with an invalid SDU or PDU Id.
 */
#define FRTP_E_INVALID_PDU_SDU_ID 0x03U

/*
 * API service called with not allowed parameter value..
 */
#define FRTP_E_INVALID_PARAMETER 0x04U

/*
 * 1-n connection is performed but cannot be transmitted via a single start frame.
 * Segmentation would be required, but is not allowed.
 */
#define FRTP_E_SEG_ERROR 0x05U

/*
 * Transmission of a unknown message length was requested, but is not allowed by configuration.
 */
#define FRTP_E_UMSG_LENGTH_ERROR 0x06U

/*
 * Currently is no free channel resource available.
 */
#define FRTP_E_NO_CHANNEL 0x07U


/*
 * Extensions of AUTOSAR NotifResultType.
 * should be defined in ComStack_Types.h
 * if not defined, since an older version of ComStack_Types.h is used, use the local definition.
 */
#ifndef NTFRSLT_E_ABORT
#define NTFRSLT_E_ABORT 0x10U
#endif


/* FlexRay specific transport protocol notification types */

/* mismatch between initial/last/actual transmitted messale length */
#define NTFRSLT_E_FR_ML_MISMATCH    0x5BU

/* buffer position requested by retry is outside the transmitters available buffer */
#define NTFRSLT_E_FR_WRONG_BP       0x5CU

/* transmission on the requested connection is already in progres */
#define NTFRSLT_E_FR_TX_ON          0x5DU

/*=====================[Global Macro Checks]=================================*/

/*
 * FrTp parameters have to be defined and have to be either STD_ON or STD_OFF
 */
/* check pre-compile-time parameter FRTP_HAVE_ACKRT */
#ifdef FRTP_HAVE_ACKRT
#if   (FRTP_HAVE_ACKRT != STD_ON) \
    &&(FRTP_HAVE_ACKRT != STD_OFF)
#error "FrTp config error: Wrong value for FRTP_HAVE_ACKRT"
#endif
#else
#error "FrTp config error: Missing parameter FRTP_HAVE_ACKRT"
#endif


/* check pre-compile-time parameter FRTP_CHAN_NUM */
/* FrTp number of channels have to be defined and in range [1..254] */
#ifdef FRTP_CHAN_NUM
#if   (FRTP_CHAN_NUM >  254) \
    ||(FRTP_CHAN_NUM <  1)
#error "FrTp: FRTP_CHAN_NUM not in range"
#endif
#else
#error "FrTp config error: Missing parameter FRTP_CHAN_NUM"
#endif


/* check pre-compile-time parameter FRTP_DEV_ERROR_DETECT */
#ifdef FRTP_DEV_ERROR_DETECT
#if   (FRTP_DEV_ERROR_DETECT != STD_ON) \
    &&(FRTP_DEV_ERROR_DETECT != STD_OFF)
#error "FrTp config error: Wrong value for FRTP_DEV_ERROR_DETECT"
#endif
#else
#error "FrTp config error: Missing parameter FRTP_DEV_ERROR_DETECT"
#endif


/* check pre-compile-time parameter FRTP_HAVE_TC */
#ifdef FRTP_HAVE_TC
#if   (FRTP_HAVE_TC != STD_ON) \
    &&(FRTP_HAVE_TC != STD_OFF)
#error "FrTp config error: Wrong value for FRTP_HAVE_TC"
#endif
#else
#error "FrTp config error: Missing parameter FRTP_HAVE_TC"
#endif


/* check pre-compile-time parameter FRTP_UNKNOWN_MSG_LENGTH */
#ifdef FRTP_UNKNOWN_MSG_LENGTH
#if   (FRTP_UNKNOWN_MSG_LENGTH != STD_ON) \
    &&(FRTP_UNKNOWN_MSG_LENGTH != STD_OFF)
#error "FrTp config error: Wrong value for FRTP_UNKNOWN_MSG_LENGTH"
#endif
#else
#error "FrTp config error: Missing parameter FRTP_UNKNOWN_MSG_LENGTH"
#endif


/* check pre-compile-time parameter FRTP_VERSION_INFO_API */
#ifdef FRTP_VERSION_INFO_API
#if   (FRTP_VERSION_INFO_API != STD_ON) \
    &&(FRTP_VERSION_INFO_API != STD_OFF)
#error "FrTp config error: Wrong value for FRTP_VERSION_INFO_API"
#endif
#else
#error "FrTp config error: Missing parameter FRTP_VERSION_INFO_API"
#endif

/*=====================[Global Data Types]===================================*/

/*
 * The cancel reason is sent to the other peer by the means of an appropriate FC
 * frame.
 */
typedef enum
{
    FRTP_CNLDO, /**< Cancel Transfer because data are outdated */
    FRTP_CNLNB, /**< Cancel Transfer because no further buffer can be
                     provided */
    FRTP_CNLOR  /**< Cancel Transfer because of another reason */
} FrTp_CancelReasonType;

/*
 * Type for the Parameter used in FrTp_ChangeParameter
 */

typedef uint8 FrTp_ParameterValueType;

/*=====================[Global Function Declarations]========================*/

#define FRTP_START_SEC_CODE
#include <FrTp_MemMap.h>

/** \brief Validate configuration
 **
 ** Checks if the post build configuration fits to the link time configuration part.
 **
 ** \return E_OK if the given module configurations is valid otherwise E_NOT_OK.
 **
 ** \ServiceID{0x60}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, FRTP_CODE) FrTp_IsValidConfig(
    P2CONST(void, AUTOMATIC, FRTP_APPL_CONST) voidConfigPtr);


#if (FRTP_VERSION_INFO_API == STD_ON)
/**
 * \brief This service returns the version information of this module. The
 *        version information includes: Module Id, Vendor Id, Instance ID and Vendor specific
 *        version numbers.
 *
 *
 * \param[out] versioninfo  Pointer to where to store the version information of
 *                          this module.
 *
 * \ServiceID{0x27}
 * \Reentrancy{Non reentrant}
 * \Synchronicity{Synchronous}
 *
 * \remarks Configuration: Optional (Only available if FRTP_VERSION_INFO_API
 *          is defined).
 */
extern FUNC(void,FRTP_CODE) FrTp_GetVersionInfo
(
    P2VAR(Std_VersionInfoType,AUTOMATIC,FRTP_APPL_DATA) versioninfo
);
#endif /* FRTP_VERSION_INFO_API */

/**
 * \brief This service initializes all global variables of a FlexRay Transport
 *        Layer instance and set it in the idle state.
 *
 *
 * \param[in] PBCfgPtr  Addres of the post-build configuration
 *
 * \ServiceID{0x00}
 * \Reentrancy{Non reentrant}
 * \Synchronicity{Synchronous}
 *
 * \remarks The call of this service is mandatory before using the FrTp instance
 *          for further processing.
 *          This function has to be called after initialization of the FlexRay
 *          interface.
 */
extern FUNC(void,FRTP_CODE) FrTp_Init
(
    P2CONST(FrTp_ConfigType,AUTOMATIC,FRTP_APPL_CONST) PBCfgPtr
);

/**
 * \brief Request the transfer of data.
 *
 *
 * \ServiceID{0x02}
 * \Reentrancy{Reentrant}
 * \Synchronicity{Asynchronous}
 *
 * \param[in] FrTpTxPduId  Contains the FlexRay TP instance unique identifier of
 *                         the Fr N-SDU to be transmitted.
 *
 * \param[in] PduInfoPtr   A pointer to a structure with Fr N-SDU related data:
 *                         data length and pointer to an Fr N-SDU buffer.
 *
 *
 * \return E_OK            The request has been accepted
 *
 * \return E_NOT_OK        The request has not been accepted, e. g. due to a
 *                         still ongoing transmission in the corresponding
 *                         channel.
 */
extern FUNC(Std_ReturnType,FRTP_CODE) FrTp_Transmit
(
    VAR(PduIdType,AUTOMATIC) FrTpTxPduId,
    P2CONST(PduInfoType,AUTOMATIC,FRTP_APPL_DATA) PduInfoPtr
);

#if (FRTP_HAVE_TC == STD_ON)
/**
 * \brief This service primitive is used to cancel the transfer of pending Fr
 *        N-SDUs. The connection is identified by FrTpTxSduId.
 *
 * \ServiceID{0x03}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 *
 * \param[in] FrTpTxPduId  Contains the FlexRay TP instance unique identifier of
 *                         the Fr N-SDU to be cancelled.
 *
 * \return E_OK            Cancellation request of the transfer (sending or
 *                         receiving) of the specified Fr N-SDU is accepted.
 *
 * \return E_NOT_OK        Cancellation request of the transfer of the specified
 *                         Fr N-SDU is rejected.
 */
extern FUNC(Std_ReturnType,FRTP_CODE) FrTp_CancelTransmit
(
    VAR(PduIdType,AUTOMATIC) FrTpTxPduId
);
#endif

/**
 * \brief This service primitive is used to request the change of the value of
 *        the FRTP_STMIN parameter.
 *
 *
 * \param[in] FrTpTxPduId           Gives the ID of the connection (message) for whose
 *                                  channel the change shall be done.
 * \param[in] parameter             The selected parameter that the request shall change.
 * \param[in] FrTpParameterValue    contains the new value of Bandwidth Control (BC).
 *
 * \return E_OK                     request is accepted
 *
 * \return E_NOT_OK                 request is not accepted
 *
 * \ServiceID{0x04}
 * \Reentrancy{Reentrant}
 * \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType,FRTP_CODE) FrTp_ChangeParameter
(
    VAR(PduIdType,AUTOMATIC) FrTpTxPduId,
    VAR(TPParameterType,AUTOMATIC) parameter,
    VAR(FrTp_ParameterValueType,AUTOMATIC) FrTpParameterValue
);

/**
 * \brief This service primitive is used to cancel the transfer of pending Fr
 *        N-SDUs. The connection is identified by FrTpRxSduId.
 *
 * \ServiceID{0x08}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 *
 * \param[in] FrTpRxPduId  Contains the FlexRay TP instance unique identifier of
 *                         the Fr N-SDU to be cancelled.
 *
 * \return E_OK            Cancellation request of the transfer (sending or
 *                         receiving) of the specified Fr N-SDU is accepted.
 *
 * \return E_NOT_OK        Cancellation request of the transfer of the specified
 *                         Fr N-SDU is rejected.
 */
extern FUNC(Std_ReturnType,FRTP_CODE) FrTp_CancelReceive
(
    VAR(PduIdType,AUTOMATIC) FrTpRxSduId
);

/**
 * \brief The main function for scheduling the TP (Entry point for scheduling)
 *
 * \ServiceID{0x10}
 */
/* Deviation MGCC62-1 */
extern FUNC(void,FRTP_CODE) FrTp_MainFunction(void);

#define FRTP_STOP_SEC_CODE
#include <FrTp_MemMap.h>

#endif /* FRTP_API_H */
