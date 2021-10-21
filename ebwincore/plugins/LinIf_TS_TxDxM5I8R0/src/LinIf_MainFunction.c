/**
 * \file
 *
 * \brief AUTOSAR LinIf
 *
 * This file contains the implementation of the AUTOSAR
 * module LinIf.
 *
 * \version 5.8.27
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*
 *  MISRA-C:2012 Deviations:
 *
 *  MISRAC2012-1) Deviated Rule: 8.9 (Advisory)
 *    An object should be defined at block scope if its identifier only
 *    appears in a single function.
 *
 *    Reason:
 *    The AUTOSAR memory mapping requires that functions are mapped in
 *    SEC_CODE memory sections. Objects at block scope require a different
 *    memory mapping, e.g. to a SEC_VAR section, which leads to nested memory
 *    sections, which is not supported by some compilers.
 *
 *  MISRAC2012-2) Deviated Rule: 10.5 (Advisory)
 *    The value of an expression should not be cast to an inappropriate
 *    essential type.
 *
 *    Reason:
 *    In order to provide the appropriate type to the callout this cast is
 *    necessary. The other possibility would be a switch-case enum set.
 */

/*==================[inclusions]============================================*/

#include <LinIf_Trace.h>

#define LINIF_NO_PBCFG_REQUIRED
#include <LinIf.h>            /* Module library API */
#include <LinIf_Int.h>        /* LinIf internal data */
#include <LinTp_Int.h>        /* LinTp internal data */

#include <LinSM_Cbk.h>          /* LinSM callback functions */
/* do not indirectly include PduR_SymbolicNames_PBcfg.h via PduR_LinIf/Tp.h */
#define PDUR_NO_PBCFG_REQUIRED
#include <PduR_LinIf.h>         /* PDU Router LIN Interface */
#include <SchM_LinIf.h>         /* for protecting exclusive areas */

#if ((LINIF_DEV_ERROR_DETECT == STD_ON) || (LINIF_RUNTIME_ERROR_REPORTING == STD_ON))
#include <Det.h>              /* Default Error Tracer */
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) || (LINIF_RUNTIME_ERROR_REPORTING == STD_ON) */

#if (LINIF_DEM_EVENTS_CONFIGURED)
#include <Dem.h>
#endif

#include <TSMem.h>               /* TS_MemCpy() */

#if (LINIF_LIN_ERROR_CALLOUT == STD_ON)
#include LINIF_LIN_ERROR_CALLOUT_INCLUDE
#endif /* (LINIF_LIN_ERROR_CALLOUT == STD_ON) */

#if (LINIF_LIN_SUCCESS_CALLOUT == STD_ON)
#include LINIF_LIN_SUCCESS_CALLOUT_INCLUDE
#endif /* (LINIF_LIN_SUCCESS_CALLOUT == STD_ON) */

#if (LINIF_BUS_MIRRORING_SUPPORT_ENABLE == STD_ON)
#if (LINIF_CDD_REPORTING_ENABLE == STD_OFF)
/* !LINKSTO LinIf.ASR44.SWS_LinIf_00872,1 */
#include <Mirror.h>
#endif /* (LINIF_CDD_REPORTING_ENABLE == STD_OFF) */
#endif /* (LINIF_BUS_MIRRORING_SUPPORT_ENABLE == STD_ON) */

/*==================[macros]================================================*/

#if (defined LINIF_CLEAR_SLEEP_WAKEUP_FLAGS_MASK)
#error LINIF_CLEAR_SLEEP_WAKEUP_FLAGS_MASK already defined!
#endif /* #if( defined LINIF_CLEAR_SLEEP_WAKEUP_FLAGS_MASK)*/
#define LINIF_CLEAR_SLEEP_WAKEUP_FLAGS_MASK 0x02U

#if (LINIF_REPORTTODEM_ENABLED)

#if (defined LINIF_ADDITIONAL_STATUS_MATRIX_NUM_STATES)
#error LINIF_ADDITIONAL_STATUS_MATRIX_NUM_STATES already defined!
#endif /* #if( defined LINIF_ADDITIONAL_STATUS_MATRIX_NUM_STATES)*/
#define LINIF_ADDITIONAL_STATUS_MATRIX_NUM_STATES 3U

#if (defined LINIF_LINTP_MRF_ERROR)
#error LINIF_LINTP_MRF_ERROR already defined!
#endif /* #if( defined LINIF_LINTP_MRF_ERROR)*/
#define LINIF_LINTP_MRF_ERROR 3U

#else /* LINIF_REPORTTODEM_ENABLED */

#if (defined LINIF_ADDITIONAL_STATUS_MATRIX_NUM_STATES)
#error LINIF_ADDITIONAL_STATUS_MATRIX_NUM_STATES already defined!
#endif /* #if( defined LINIF_ADDITIONAL_STATUS_MATRIX_NUM_STATES)*/
#define LINIF_ADDITIONAL_STATUS_MATRIX_NUM_STATES 0U

#endif /* LINIF_REPORTTODEM_ENABLED */

#if (defined LINIF_STATUS_MATRIX_NUM_STATES)
#error LINIF_STATUS_MATRIX_NUM_STATES already defined!
#endif /* #if( defined LINIF_STATUS_MATRIX_NUM_STATES)*/
#define LINIF_STATUS_MATRIX_NUM_STATES (8U + LINIF_ADDITIONAL_STATUS_MATRIX_NUM_STATES)

#if (defined LINIF_STATUS_MATRIX_NUM_HEADERTYPES)
#error LINIF_STATUS_MATRIX_NUM_HEADERTYPES already defined!
#endif /* #if( defined LINIF_STATUS_MATRIX_NUM_HEADERTYPES)*/
#define LINIF_STATUS_MATRIX_NUM_HEADERTYPES 6U

#if (LINIF_DEM_EVENTS_CONFIGURED)

#if (defined LINIF_IS_REPORTTODEM_ERROR_STATUS)
#error LINIF_IS_REPORTTODEM_ERROR_STATUS already defined!
#endif /* #if( defined LINIF_IS_REPORTTODEM_ERROR_STATUS)*/
#define LINIF_IS_REPORTTODEM_ERROR_STATUS(status) \
((((status) == LIN_TX_BIT_ERROR) || ((status) == LIN_RX_NO_RESPONSE_ERROR) || ((status) == LIN_RX_CHECKSUM_ERROR))? TRUE:FALSE)

#endif /* LINIF_DEM_EVENTS_CONFIGURED */

#if (defined LINIF_STATUS_IGNORE)
#error LINIF_STATUS_IGNORE already defined!
#endif /* #if( defined LINIF_STATUS_IGNORE)*/
#define LINIF_STATUS_IGNORE NULL_PTR

#if (LINIF_RUNTIME_ERROR_REPORTING == STD_ON)

#if (defined LINIF_STATUS_DET)
#error LINIF_STATUS_DET already defined!
#endif /* #if( defined LINIF_STATUS_DET)*/
#define LINIF_STATUS_DET    &LinIf_Status_DET

#else

#if (defined LINIF_STATUS_DET)
#error LINIF_STATUS_DET already defined!
#endif /* #if( defined LINIF_STATUS_DET)*/
#define LINIF_STATUS_DET    NULL_PTR

#endif /* LINIF_RUNTIME_ERROR_REPORTING */

#if (LINIF_DEV_ERROR_DETECT == STD_ON)

#if (defined LINIF_STATUS_SNH)
#error LINIF_STATUS_SNH already defined!
#endif /* #if( defined LINIF_STATUS_SNH)*/
#define LINIF_STATUS_SNH    &LinIf_Status_SNH

#else /* (LINIF_DEV_ERROR_DETECT == STD_ON) */

#if (defined LINIF_STATUS_SNH)
#error LINIF_STATUS_SNH already defined!
#endif /* #if( definedLINIF_STATUS_SNH)*/
#define LINIF_STATUS_SNH    NULL_PTR

#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) */

#if (LINIF_LIN_ERROR_CALLOUT == STD_ON)

#if (defined LINIF_STATUS_USRCALLOUT_OR_IGNORE)
#error LINIF_STATUS_USRCALLOUT_OR_IGNORE already defined!
#endif /* #if( defined LINIF_STATUS_USRCALLOUT_OR_IGNORE)*/
#define LINIF_STATUS_USRCALLOUT_OR_IGNORE &LinIf_Status_ErrorCalloutAndDet

#if (defined LINIF_STATUS_USRCALLOUT_AND_DET)
#error LINIF_STATUS_USRCALLOUT_AND_DET already defined!
#endif /* #if( defined LINIF_STATUS_USRCALLOUT_AND_DET)*/
#define LINIF_STATUS_USRCALLOUT_AND_DET &LinIf_Status_ErrorCalloutAndDet

#else /* (LINIF_LIN_ERROR_CALLOUT == STD_ON) */

#if (defined LINIF_STATUS_USRCALLOUT_OR_IGNORE)
#error LINIF_STATUS_USRCALLOUT_OR_IGNORE already defined!
#endif /* #if( defined LINIF_STATUS_USRCALLOUT_OR_IGNORE)*/
#define LINIF_STATUS_USRCALLOUT_OR_IGNORE NULL_PTR

#if (defined LINIF_STATUS_USRCALLOUT_AND_DET)
#error LINIF_STATUS_USRCALLOUT_AND_DET already defined!
#endif /* #if( defined LINIF_STATUS_USRCALLOUT_AND_DET)*/
#define LINIF_STATUS_USRCALLOUT_AND_DET LINIF_STATUS_DET

#endif /* (LINIF_LIN_ERROR_CALLOUT == STD_ON) */

/*==================[type definitions]======================================*/

typedef P2FUNC(void, LINIF_APPL_CODE, LinIf_StatusFunctionType)(uint8 currentChannel, P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo);

/*==================[internal function declarations]========================*/

#define LINIF_START_SEC_CODE
#include <LinIf_MemMap.h>

#if (LINIF_MASTER_SUPPORTED == STD_ON)
/** \brief This function converts status states to function indexes.
 *  This function index is used to call a function according to the
 *  return value of Lin_GetStatus.
 *
 *  \param status  Status of the Lin driver
 *  \return Function index
 */
STATIC FUNC(uint8, LINIF_CODE) LinIf_MapLinStatusToFctIndex
(
    Lin_StatusType status
);

/** \brief This function transmit Lin frames over the bus and
 *  checks the state of the transmitted frame.
 *
 *  \param currentChannel  Channel which is currently processed
 */
STATIC FUNC(void, LINIF_CODE) LinIf_ChannelMainFunction
(
    uint8 currentChannel
);

/** \brief This function process the return value of Lin_GetStatus. If a goto
 *  sleep command was successfully transmitted the LinIf is set to sleep.
 *
 *  \param currentChannel  Channel which is currently processed
 */
STATIC FUNC(void, LINIF_CODE) LinIf_StatusCheck
(
    uint8 currentChannel
);

/** \brief This function determine which frame is sent next. It handles
 *   selection of schedule tables and informs upper layers of schedule
 *   table changes.
 *
 *  \param currentChannel  Channel which is currently processed
 */
STATIC FUNC(void, LINIF_CODE) LinIf_Schedule
(
    uint8 currentChannel
);

/** \brief This function verifies if rescheduling is necessary due to a requested
 *  schedule table change from the upper layer or achievement of the schedule end.
 *
 *  \param currentChannel  Channel which is currently processed
 *
 *  \return indicates if a schedule should be changed
 *  \retval TRUE    rescheduling necessary
 *  \retval FALSE   continue with current selected schedule table
 */
STATIC FUNC(boolean, LINIF_CODE) LinIf_ReSchedule
(
    uint8 currentChannel
);

/** \brief This function returns the entry of the schedule table which should
 *   be processed after a schedule table change.
 *
 *  \param currentChannel  Channel which is currently processed
 *
 *  \return entry of the schedule table
 */
STATIC FUNC(uint16, LINIF_CODE) LinIf_Get_First_Sch_Entry
(
    uint8 currentChannel
);

/** \brief This function checks if a collision occurred on the bus and
 *  requests the collision resolution schedule if it could detect one
 *
 *  \param currentChannel  Channel which is currently processed
 */
STATIC FUNC(void, LINIF_CODE) LinIf_SetupCollisionRes
(
    uint8 currentChannel
);

/** \brief This function checks if sporadic frames are marked
 *  for transmission and determine which one should be sent next.
 *
 *  \param currentChannel  Channel which is currently processed
 */
STATIC FUNC(void, LINIF_CODE) LinIf_SetupSporadicFrame
(
    uint8 currentChannel
);

/** \brief This function assembles the Lin PduInfo which should
 *  be sent next with the function Lin_SendFrame.
 *
 *  \param currentChannel  Channel which is currently processed
 *  \param pLinPduInfo     Data and frame info of frame which
 *                         should be sent
 */
STATIC FUNC(uint8, LINIF_CODE) LinIf_SetupFrame
(
  uint8 currentChannel,
  P2VAR(Lin_PduType, AUTOMATIC, LINIF_APPL_DATA) pLinPduInfo
);

/** \brief This function handles sleep/wakeup after Lin_GetStatus returns.
 *
 *  \param currentChannel  Channel which is currently processed
 *  \param SleepFlags      Current sleep/wakeup flags
 *  \param status          Current status from Lin_GetStatus
 */
STATIC FUNC(void, LINIF_CODE) LinIf_HandleSleep
(
  uint8 currentChannel,
  uint8 SleepFlags,
  Lin_StatusType status
);

/** \brief This function fetches data from the upper layer or
 *  the configuration which should be sent with the current frame.
 *
 *  \param currentChannel  Channel which is currently processed
 *  \param pFrameData      Pointer to data
 */
STATIC FUNC(Std_ReturnType, LINIF_CODE) LinIf_GetFrameData
(
  uint8 currentChannel,
  P2VAR(uint8, AUTOMATIC, LINIF_APPL_DATA) pFrameData
);

/** \brief This function gets Pdu id, Pid, checksum type  and response
 *  type of unconditional and event triggered frames
 *
 *  \param FrameType      Frame type which should be sent
 *  \param SduId          Id of the frame which should be sent
 *  \param pFrameInfo     Pointer to Pdu id, Pid and checksum type
 *  \param pResponseType  Response type of the frame which should be sent
 */
STATIC FUNC(void, LINIF_CODE) LinIf_GetFrameInfo
(
  uint8 FrameType,
  PduIdType SduId,
  P2VAR(LinIfFrameInfoType, AUTOMATIC, LINIF_APPL_DATA) pFrameInfo,
  P2VAR(Lin_FrameResponseType, AUTOMATIC, LINIF_APPL_DATA) pResponseType
);

/** \brief This function indicates a successful event triggered frame
 *  response from one slave
 *
 *  \param currentChannel  Channel which is currently processed
 *  \param pPduInfo        received data
 */
STATIC FUNC(void, LINIF_CODE) LinIf_Status_EvTrgRxInd
(
    uint8 currentChannel,
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo
);

#if (LINIF_LIN_ERROR_CALLOUT == STD_ON)

/** \brief This function calls a user configured callout
 *
 *  \param currentChannel  Channel which is currently processed
 *  \param pPduInfo        received data
 */
STATIC FUNC(void, LINIF_CODE) LinIf_Status_ErrorCalloutAndDet
(
    uint8 currentChannel,
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo
);

#endif /* (LINIF_LIN_ERROR_CALLOUT == STD_ON) */

#if (LINIF_RUNTIME_ERROR_REPORTING == STD_ON)

/** \brief This function calls a Det which indicates that
 *  no or unexpected response was received from a slave
 *
 *  \param currentChannel  Channel which is currently processed
 *  \param pPduInfo        received data
 */
STATIC FUNC(void, LINIF_CODE) LinIf_Status_DET
(
    uint8 currentChannel,
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo
);
#endif /* LINIF_RUNTIME_ERROR_REPORTING == STD_ON */

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
/** \brief This function indicates an illegal return value
 *  of Lin_GetStatus
 *
 *  \param currentChannel  Channel which is currently processed
 *  \param pPduInfo        received data
 */
STATIC FUNC(void, LINIF_CODE) LinIf_Status_SNH
(
    uint8 currentChannel,
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo
);

#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) */

/** \brief This function verifies if a goto sleep command should
 *  be sent over the bus and calls Lin_GotoSleep/Internal if
 *  goto sleep is requested.
 *
 *  \param currentChannel  Channel which is currently processed
 *  \return indicates if goto sleep command was sent
 *  \retval E_OK      goto sleep was sent
 *  \retval E_NOT_OK  no sleep request or Lin_GotoSleep returned
 *                    E_NOT_OK
 */
STATIC FUNC(Std_ReturnType, LINIF_CODE) LinIf_SleepRequest
(
  uint8 currentChannel
);

#endif /* LINIF_MASTER_SUPPORTED == STD_ON */

#define LINIF_STOP_SEC_CODE
#include <LinIf_MemMap.h>

/*==================[internal constants]====================================*/

#if (LINIF_MASTER_SUPPORTED == STD_ON)

/* AUTOSAR Memory Mapping - start constant configuration data
   section of unspecified size
   (size of function pointer may vary on different platforms */
#define LINIF_START_SEC_CONST_UNSPECIFIED
#include <LinIf_MemMap.h>

/** \brief Status Matrix - describes what status function shall be called
 **        depending on LastHeaderType and Lin_GetStatus() */
/* Deviation MISRAC2012-1 */
STATIC CONST(LinIf_StatusFunctionType, LINIF_CONST) LinIf_StatusMatrix[LINIF_STATUS_MATRIX_NUM_HEADERTYPES][LINIF_STATUS_MATRIX_NUM_STATES] =
{
    /* LINIF_FRM_TX */
    {
        LINIF_STATUS_USRCALLOUT_OR_IGNORE,  /* LIN_TX_HEADER_ERROR        */
        &LinIf_Status_PduRTxConf,           /* LIN_TX_OK                  */
        LINIF_STATUS_DET,                   /* LIN_TX_BUSY                */
        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00036,1 */
        LINIF_STATUS_USRCALLOUT_AND_DET,    /* LIN_TX_ERROR               */
        LINIF_STATUS_SNH,                   /* LIN_RX_OK                  */
        LINIF_STATUS_SNH,                   /* LIN_RX_BUSY                */
        LINIF_STATUS_SNH,                   /* LIN_RX_ERROR               */
        LINIF_STATUS_SNH,                   /* LIN_RX_NO_RESPONSE         */
#if (LINIF_REPORTTODEM_ENABLED)
        LINIF_STATUS_IGNORE,                /* LIN_TX_BIT_ERROR           */
        LINIF_STATUS_IGNORE,                /* LIN_RX_CHECKSUM_ERROR      */
        LINIF_STATUS_IGNORE,                /* LIN_RX_NO_RESPONSE_ERROR   */
#endif
    },

    /* LINIF_FRM_RX */
    {
        LINIF_STATUS_USRCALLOUT_OR_IGNORE,     /* LIN_TX_HEADER_ERROR      */
        LINIF_STATUS_SNH,                      /* LIN_TX_OK                */
        LINIF_STATUS_SNH,                      /* LIN_TX_BUSY              */
        LINIF_STATUS_SNH,                      /* LIN_TX_ERROR             */
        &LinIf_Status_PduRRxInd,               /* LIN_RX_OK                */
        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00254.RX_BUSY,1 */
        LINIF_STATUS_DET,                      /* LIN_RX_BUSY              */
        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00254.RX_ERROR,1 */
        LINIF_STATUS_USRCALLOUT_AND_DET,       /* LIN_RX_ERROR             */
        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00466,1 */
        LINIF_STATUS_USRCALLOUT_AND_DET,       /* LIN_RX_NO_RESPONSE       */
#if (LINIF_REPORTTODEM_ENABLED)
        LINIF_STATUS_IGNORE,                   /* LIN_TX_BIT_ERROR           */
        LINIF_STATUS_IGNORE,                   /* LIN_RX_CHECKSUM_ERROR      */
        LINIF_STATUS_IGNORE,                   /* LIN_RX_NO_RESPONSE_ERROR   */
#endif
    },

    /* LINIF_FRM_MRF */
    {
        LINTP_STATUS_ERR_NO_DET,            /* LIN_TX_HEADER_ERROR       */
        LINTP_TX_CONFIRMATION,              /* LIN_TX_OK                 */
        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00465,1 */
        LINTP_STATUS_ERR,                   /* LIN_TX_BUSY               */
        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00036,1 */
        LINTP_STATUS_ERR,                   /* LIN_TX_ERROR              */
        LINIF_STATUS_SNH,                   /* LIN_RX_OK                 */
        LINIF_STATUS_SNH,                   /* LIN_RX_BUSY               */
        LINIF_STATUS_SNH,                   /* LIN_RX_ERROR              */
        LINIF_STATUS_SNH,                   /* LIN_RX_NO_RESPONSE        */
#if (LINIF_REPORTTODEM_ENABLED)
        LINIF_STATUS_IGNORE,                /* LIN_TX_BIT_ERROR           */
        LINIF_STATUS_IGNORE,                /* LIN_RX_CHECKSUM_ERROR      */
        LINIF_STATUS_IGNORE,                /* LIN_RX_NO_RESPONSE_ERROR   */
#endif
    },

    /* LINIF_FRM_SRF */
    {
        LINIF_STATUS_IGNORE,                /* LIN_TX_HEADER_ERROR       */
        LINIF_STATUS_SNH,                   /* LIN_TX_OK                 */
        LINIF_STATUS_SNH,                   /* LIN_TX_BUSY               */
        LINIF_STATUS_SNH,                   /* LIN_TX_ERROR              */
        LINTP_RX_INDICATION,                /* LIN_RX_OK                 */
        LINIF_STATUS_DET,                   /* LIN_RX_BUSY               */
        LINIF_STATUS_DET,                   /* LIN_RX_ERROR              */
        LINIF_STATUS_IGNORE,                /* LIN_RX_NO_RESPONSE        */
#if (LINIF_REPORTTODEM_ENABLED)
        LINIF_STATUS_IGNORE,                /* LIN_TX_BIT_ERROR           */
        LINIF_STATUS_IGNORE,                /* LIN_RX_CHECKSUM_ERROR      */
        LINIF_STATUS_IGNORE,                /* LIN_RX_NO_RESPONSE_ERROR   */
#endif
    },

    /* LINIF_FRM_EVENT_TRIGGERED */
    {
        LINIF_STATUS_USRCALLOUT_OR_IGNORE,  /* LIN_TX_HEADER_ERROR        */
        LINIF_STATUS_SNH,                   /* LIN_TX_OK                  */
        LINIF_STATUS_SNH,                   /* LIN_TX_BUSY                */
        LINIF_STATUS_SNH,                   /* LIN_TX_ERROR               */
        &LinIf_Status_EvTrgRxInd,           /* LIN_RX_OK                  */
        LINIF_STATUS_IGNORE,                /* LIN_RX_BUSY                */
        LINIF_STATUS_USRCALLOUT_OR_IGNORE,  /* LIN_RX_ERROR               */
        LINIF_STATUS_IGNORE,                /* LIN_RX_NO_RESPONSE         */
#if (LINIF_REPORTTODEM_ENABLED)
        LINIF_STATUS_IGNORE,                /* LIN_TX_BIT_ERROR           */
        LINIF_STATUS_IGNORE,                /* LIN_RX_CHECKSUM_ERROR      */
        LINIF_STATUS_IGNORE,                /* LIN_RX_NO_RESPONSE_ERROR   */
#endif
    },

    /* LINIF_FRM_CONFIG */
    {
        LINIF_STATUS_USRCALLOUT_OR_IGNORE,  /* LIN_TX_HEADER_ERROR        */
        LINIF_STATUS_IGNORE,                /* LIN_TX_OK                  */
        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00465,1 */
        LINIF_STATUS_DET,                   /* LIN_TX_BUSY                */
        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00036,1 */
        LINIF_STATUS_USRCALLOUT_AND_DET,    /* LIN_TX_ERROR               */
        LINIF_STATUS_SNH,                   /* LIN_RX_OK                  */
        LINIF_STATUS_SNH,                   /* LIN_RX_BUSY                */
        LINIF_STATUS_SNH,                   /* LIN_RX_ERROR               */
        LINIF_STATUS_SNH,                   /* LIN_RX_NO_RESPONSE         */
#if (LINIF_REPORTTODEM_ENABLED)
        LINIF_STATUS_IGNORE,                /* LIN_TX_BIT_ERROR           */
        LINIF_STATUS_IGNORE,                /* LIN_RX_CHECKSUM_ERROR      */
        LINIF_STATUS_IGNORE,                /* LIN_RX_NO_RESPONSE_ERROR   */
#endif
    }
};


/* AUTOSAR Memory Mapping - end of constants section of configuration data of
   unspecified size */
#define LINIF_STOP_SEC_CONST_UNSPECIFIED
#include <LinIf_MemMap.h>

#endif /* LINIF_MASTER_SUPPORTED == STD_ON */

/*==================[internal variable definitions]=========================*/
#if (LINIF_BUS_MIRRORING_SUPPORT_ENABLE == STD_ON)
#define LINIF_START_SEC_VAR_INIT_16
#include <LinIf_MemMap.h>

/** \brief Global state of the LIN channels regarding mirroring */
VAR(uint16, LINIF_VAR) LinIf_BusMiroringChannelEnabled = 0U;

#define LINIF_STOP_SEC_VAR_INIT_16
#include <LinIf_MemMap.h>
#endif /* (LINIF_BUS_MIRRORING_SUPPORT_ENABLE == STD_ON) */

/*==================[external variable definitions]=========================*/

/*==================[external function definitions]=========================*/

/* AUTOSAR Memory Mapping - start section for code */
#define LINIF_START_SEC_CODE
#include <LinIf_MemMap.h>


/** \brief LIN Interface main processing function.
 *
 * This function performs nearly everything the LIN Interface has to
 * handle. All Access to the LIN Bus happens here.
 */
FUNC(void, LINIF_CODE) LinIf_MainFunction
(
    void
)
{
    /* Do not execute main function until module is initialized,
     * this may be the case during ECU startup
     */

    DBG_LINIF_MAINFUNCTION_ENTRY();

    if ( LinIf_InitState == LINIF_INIT)
    {
        /* local channel loop variable */
        uint8 channelIdx;

#if (LINIF_MASTER_SUPPORTED == STD_ON)
        for ( channelIdx = 0U ; channelIdx < LinIf_ConfigPtr->NumChannelsMaster ; channelIdx++ )
        {
            /* handle channel specific tasks*/
            LinIf_ChannelMainFunction(channelIdx);
        }
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */

#if (LINIF_SLAVE_SUPPORTED == STD_ON)
        for ( channelIdx = LinIf_ConfigPtr->NumChannelsMaster ; channelIdx < (LinIf_ConfigPtr->NumChannelsMaster + LinIf_ConfigPtr->NumSlaveChannels) ; channelIdx++ )
        {

            CONSTP2VAR(LinIf_ChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannel =
              &LinIf_ChannelInfo[channelIdx];

            if (pIfChannel->cFSM == LINIF_CHANNEL_OPERATIONAL)
            {
              CONSTP2VAR(LinIf_SlaveChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannelSlave =
                  LINIF_GET_CHANNEL_SLAVE(channelIdx);

              /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00751,1 */
              if(0U < pIfChannelSlave->TimerBusIdle)
              {
                  pIfChannelSlave->TimerBusIdle--;

                  if(0U == pIfChannelSlave->TimerBusIdle)
                  {
                      /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00755,1 */
                      LinIf_SlaveHandleSleep(channelIdx);
                  }
              }

              if(0U < pIfChannelSlave->TimerNas)
              {
                pIfChannelSlave->TimerNas--;

                if(0U == pIfChannelSlave->TimerNas)
                {
                  /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00786,1 */
                  pIfChannelSlave->RSID = 0U;
                }
              }
  #if (LINIF_TP_SUPPORTED == STD_ON)
              LinTp_SlavePhysTransmitHandling(channelIdx);
              LinTp_TimerMainFunction(channelIdx);
  #endif /* LINIF_TP_SUPPORTED == STD_ON */
          }
        }
#endif /* LINIF_SLAVE_SUPPORTED == STD_ON */
    }

    DBG_LINIF_MAINFUNCTION_EXIT();
}
/* !LINKSTO LinIf.ASR44.SWS_LinIf_00875,1 */
#if (LINIF_BUS_MIRRORING_SUPPORT_ENABLE == STD_ON)
/* !LINKSTO LinIf.ASR44.SWS_LinIf_00876,1 */
FUNC(Std_ReturnType, LINIF_CODE) LinIf_EnableBusMirroring
(
    NetworkHandleType Channel,
    boolean MirroringActive
)
{
  Std_ReturnType ret = E_NOT_OK;

  DBG_LINIF_ENABLEBUSMIRRORING_ENTRY(Channel,MirroringActive);

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
  if (LINIF_UNINIT_ACCESS)
  {
    (void)Det_ReportError(LINIF_MODULE_ID,
                          LINIF_INSTANCE_ID,
                          LINIF_API_ENABUSMIRRORING,
                          LINIF_E_UNINIT
                          );
  }
  else if (( LINIF_MAX_MIRROR_CHANNELS <= Channel ) || ( Channel >= (LinIf_ConfigPtr->NumChannelsMaster + LinIf_ConfigPtr->NumSlaveChannels) ))
  {
    /* !LINKSTO LinIf.EB.EnableBusMirroring.InvalidChannel.IDgtMaxMirroredChannels,1 */
    /* !LINKSTO LinIf.EB.EnableBusMirroring.InvalidChannel.IDgtConfigChannels,1 */
    (void)Det_ReportError(LINIF_MODULE_ID,
                          LINIF_INSTANCE_ID,
                          LINIF_API_ENABUSMIRRORING,
                          LINIF_E_INVALID_MIRROR_CHANNEL
                          );
  }
  else
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) */
  {
      if (MirroringActive == TRUE)
      {
        TS_AtomicSetBit_16(&LinIf_BusMiroringChannelEnabled, Channel);
      }
      else
      {
        TS_AtomicClearBit_16(&LinIf_BusMiroringChannelEnabled, Channel);
      }
      ret = E_OK;
  }

  DBG_LINIF_ENABLEBUSMIRRORING_EXIT(ret,Channel,MirroringActive);

  return ret;
}

#endif /* (LINIF_BUS_MIRRORING_SUPPORT_ENABLE == STD_ON) */

/*==================[internal function definitions]=========================*/

#if (LINIF_MASTER_SUPPORTED == STD_ON)

/** \brief This function converts status states to function indexes. */
STATIC FUNC(uint8, LINIF_CODE) LinIf_MapLinStatusToFctIndex( Lin_StatusType status )
{
    uint8 FctIndex;

    switch ( status )
    {
        case LIN_TX_HEADER_ERROR: FctIndex =  0U; break;
        case LIN_TX_OK: FctIndex =  1U; break;
        case LIN_TX_BUSY: FctIndex =  2U; break;
        case LIN_TX_ERROR: FctIndex =  3U; break;
        case LIN_RX_OK: FctIndex =  4U; break;
        case LIN_RX_BUSY: FctIndex =  5U; break;
        case LIN_RX_ERROR: FctIndex =  6U; break;
        case LIN_RX_NO_RESPONSE: FctIndex =  7U; break;
#if (LINIF_REPORTTODEM_ENABLED)
        case LIN_TX_BIT_ERROR: FctIndex =  8U; break;
        case LIN_RX_CHECKSUM_ERROR: FctIndex =  9U; break;
        case LIN_RX_NO_RESPONSE_ERROR: FctIndex =  10U; break;
        default: FctIndex =  11U; break;
#else
        default: FctIndex =  8U; break;
#endif /* #if (LINIF_REPORTTODEM_ENABLED) */
    }

    return FctIndex;
}

/** \brief This function transmits frames over the Lin bus. */
STATIC FUNC(void, LINIF_CODE) LinIf_ChannelMainFunction
(
    uint8 currentChannel
)
{
    CONSTP2VAR(LinIf_ChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannel =
        &LinIf_ChannelInfo[currentChannel];
    CONSTP2VAR(LinIf_MasterChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannelMaster =
        &LinIf_MasterChannelInfo[currentChannel];

#if (LINIF_TP_SUPPORTED == STD_ON)
    if (pIfChannel->cFSM == LINIF_CHANNEL_OPERATIONAL)
    {
        /* !LINKSTO LinTp.EB.TpMainFunctionSplit,1 */
        LinTp_TxRxMainFunction(currentChannel);
    }
#endif /*LINIF_TP_SUPPORTED == STD_ON*/


    /* check status delay counter */
    if(pIfChannelMaster->StatusDelay > 0U)
    {
      if(pIfChannelMaster->StatusDelay == 1U)
      {
        LinIf_StatusCheck(currentChannel);
#if (LINIF_EOS_NOTIF_CALLOUT_SUPPORTED == STD_ON)
        if(FALSE != pIfChannelMaster->CallEOSNotif)
        {
          P2FUNC(void, LINIF_NOTIF_CALLOUT_CODE, EOSNotifFuncPtr)(void) = LINIF_CFG_GET_EOS_NFPTR(pIfChannelMaster->SchCurrentTable);
          pIfChannelMaster->CallEOSNotif = FALSE;

          if(NULL != EOSNotifFuncPtr)
          {
            EOSNotifFuncPtr();
          }
        }
#endif
      }

        pIfChannelMaster->StatusDelay--;
    }

    if (pIfChannel->cFSM == LINIF_CHANNEL_OPERATIONAL)
    {
        uint16 NextFrameToSendDue = pIfChannelMaster->ScheduleDelay;

#if (LINIF_TP_SUPPORTED == STD_ON)
        /* !LINKSTO LinTp.EB.TpMainFunctionSplit,1 */
        LinTp_TimerMainFunction(currentChannel);
#endif /*LINIF_TP_SUPPORTED == STD_ON*/

        /* check schedule delay counter */
        if(NextFrameToSendDue == 1U)
        {
            Std_ReturnType ret;

            ret = LinIf_SleepRequest(currentChannel);

            if(ret != E_OK)
            {
                uint8 LinIf_FrameBuffer[8] = {0U}; /* temporary frame buffer for transmission */
                Lin_PduType LinPduInfo;     /* structure holding information for transmission of Lin frame */
                LinPduInfo.SduPtr = LinIf_FrameBuffer; /* assign buffer to info structure */

                LinIf_SetupCollisionRes(currentChannel);
                LinIf_Schedule(currentChannel);

                if(LinIf_SetupFrame(currentChannel, &LinPduInfo) != LINIF_FRM_EMPTY)
                {
                  (void)LinIf_LinDriverConfig[LINIF_GET_LIN_DRIVER_INDEX(currentChannel)].sendFrame(LINIF_GET_LIN_CHANNEL(currentChannel),&LinPduInfo);
                }
            }
        }
        else
        {
            NextFrameToSendDue--;
            pIfChannelMaster->ScheduleDelay = NextFrameToSendDue;
        }
    }
    else /* sleep state */
    {
        (void)LinIf_SleepRequest(currentChannel);
    }

}

/** \brief Function which initiates bus to sleep mode */
STATIC FUNC(Std_ReturnType, LINIF_CODE) LinIf_SleepRequest
(
    uint8 currentChannel
)
{
    Std_ReturnType retVal = E_NOT_OK;

    CONSTP2VAR(LinIf_ChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannel =
            &LinIf_ChannelInfo[currentChannel];
    CONSTP2VAR(LinIf_MasterChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannelMaster =
            &LinIf_MasterChannelInfo[currentChannel];

    CONSTP2CONST(LinIfMasterChannelType, AUTOMATIC, LINIF_VAR) pIfChannelConfigMaster =
            &LINIF_CFG_GET_CHANNEL_MASTER[currentChannel];

    uint8 SleepFlags;



    TS_AtomicAssign8(SleepFlags, pIfChannel->SleepHandling);

    /* check if goto sleep was requested from upper layer */
    if((SleepFlags & LINIF_SLEEP_REQUESTED_FLAG) == LINIF_SLEEP_REQUESTED_FLAG)
    {
        uint8 State;
        TS_AtomicAssign8(State, pIfChannel->cFSM );

        if(State == LINIF_CHANNEL_OPERATIONAL)
        {
            retVal =  LinIf_LinDriverConfig[LINIF_GET_LIN_DRIVER_INDEX(currentChannel)].goToSleep(LINIF_GET_LIN_CHANNEL(currentChannel));

            if(retVal == E_OK)
            {
                /* check status after sleep command was sent */
                pIfChannelMaster->StatusDelay = pIfChannelConfigMaster->DelayTime[7];

                /* next frame can be sent after status check */
                pIfChannelMaster->ScheduleDelay = pIfChannelConfigMaster->DelayTime[7];
            }
        }
        else /* sleep state */
        {
            retVal = LinIf_LinDriverConfig[LINIF_GET_LIN_DRIVER_INDEX(currentChannel)].goToSleepInt(LINIF_GET_LIN_CHANNEL(currentChannel));

            if(retVal == E_OK)
            {
                /* goto sleep command is not sent (LinIf already in
                   sleep state), status can be checked in the next
                   main function call */
                pIfChannelMaster->StatusDelay = 1U;
            }
        }

        SchM_Enter_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();

        TS_AtomicAssign8(SleepFlags, pIfChannel->SleepHandling);

        if(retVal == E_OK)
        {
            /* indicate that a goto sleep process is ongoing */
            SleepFlags |= LINIF_SLEEP_ONGOING_FLAG;
        }

        /* clear sleep request flag and wakeup flag */
        SleepFlags  &= (uint8)LINIF_CLEAR_SLEEP_WAKEUP_FLAGS_MASK;
        TS_AtomicAssign8(pIfChannel->SleepHandling, SleepFlags);

        SchM_Exit_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();

        if(retVal != E_OK)
        {
           /* an error occurred during Lin_GotoSleep call, indicate
              the upper layer that goto sleep was unsuccessful */
           LINIF_CFG_GET_UL_SLEEPCONF_FPTR(currentChannel)(
                                                           LINIF_GET_COMM_CHANNEL(currentChannel),
                                                           FALSE
                                                          );
        }
    }


    return retVal;
}


/** \brief LIN Interface Status Check function, process return value of Lin_GetStatus */
STATIC FUNC(void, LINIF_CODE) LinIf_StatusCheck (uint8 currentChannel)
{
    CONSTP2VAR(LinIf_ChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannel =
        &LinIf_ChannelInfo[currentChannel];

    PduInfoType PduInfo;
    PduInfoType *pPduInfo;
#if (LINIF_BUS_MIRRORING_SUPPORT_ENABLE == STD_ON)
    PduInfoType *pPduInfoTx;
#endif /* LINIF_BUS_MIRRORING_SUPPORT_ENABLE == STD_ON */
    uint8 SleepFlags;
    Lin_StatusType status;
#if (LINIF_BUS_MIRRORING_SUPPORT_ENABLE == STD_ON)
    uint16 enabledChannels;
#endif /* LINIF_BUS_MIRRORING_SUPPORT_ENABLE == STD_ON */

    status =  LinIf_LinDriverConfig[LINIF_GET_LIN_DRIVER_INDEX(currentChannel)].getStatus( LINIF_GET_LIN_CHANNEL(currentChannel),&PduInfo.SduDataPtr);

#if (LINIF_LIN_ERROR_CALLOUT_STATUS_FWD == STD_OFF)
    if(LIN_RX_OK == status)
    {
        pPduInfo = &PduInfo;
        PduInfo.SduLength = pIfChannel->LastDl;
    }
    else
    {
        pPduInfo = NULL_PTR;
    }
#else
    pPduInfo = &PduInfo;

    if(LIN_RX_OK == status)
    {
      PduInfo.SduLength = pIfChannel->LastDl;
    }
    else
    {
      PduInfo.SduDataPtr = NULL_PTR;
      PduInfo.SduLength = (PduLengthType)status;
    }
#endif /* LINIF_LIN_ERROR_CALLOUT_STATUS_FWD */

#if (LINIF_BUS_MIRRORING_SUPPORT_ENABLE == STD_ON)
    /* check if current channel has bus mirroring enabled */
    TS_AtomicAssign16(enabledChannels, LinIf_BusMiroringChannelEnabled);

    if (LINIF_CHANNEL_ENABLED == LINIF_MIRROR_CHANNEL_ENABLED(enabledChannels, currentChannel))
    {
      if (LIN_TX_OK == status)
      {
        pPduInfoTx = &PduInfo;
        pPduInfoTx->SduDataPtr = pIfChannel->FrameInfo.FrameData;
        pPduInfoTx->SduLength = pIfChannel->FrameInfo.FrameLength;
        /* !LINKSTO LinIf.ASR44.SWS_LinIf_00874.LIN_TX_OK, 1,
                    LinIf.EB.SWS_LinIf_00874.LIN_TX_OK.Cdd, 1 */
        LINIF_UL_REPORTING(LINIF_GET_COMM_CHANNEL(currentChannel), pIfChannel->FrameInfo.Pid, pPduInfoTx, status);
      }
      else
      {
        /* !LINKSTO LinIf.ASR44.SWS_LinIf_00873.LIN_RX_OK, 1,
                    LinIf.ASR44.SWS_LinIf_00873.LIN_RX_OTHER_THAN_OK, 1,
                    LinIf.ASR44.SWS_LinIf_00874.LIN_TX_OTHER_THAN_OK, 1,
                    LinIf.EB.SWS_LinIf_00873.LIN_RX_OK.Cdd, 1,
                    LinIf.EB.SWS_LinIf_00873.LIN_RX_OTHER_THAN_OK.Cdd, 1,
                    LinIf.EB.SWS_LinIf_00874.LIN_TX_OTHER_THAN_OK.Cdd, 1                    */
        LINIF_UL_REPORTING(LINIF_GET_COMM_CHANNEL(currentChannel), pIfChannel->FrameInfo.Pid, pPduInfo, status);
      }
    }
    else
    {
      /* do nothing, mirroring is disabled */
    }

#endif /* LINIF_BUS_MIRRORING_SUPPORT_ENABLE == STD_ON */

    TS_AtomicAssign8(SleepFlags, pIfChannel->SleepHandling);

    /* check if goto sleep command was sent */
    if((SleepFlags & LINIF_SLEEP_ONGOING_FLAG) == LINIF_SLEEP_ONGOING_FLAG)
    {
        LinIf_HandleSleep(currentChannel, SleepFlags, status);

        /* clear flags */
        TS_AtomicAssign8(pIfChannel->SleepHandling, LINIF_NO_SLEEP_REQUEST);
    }
    else
    {
        CONSTP2VAR(LinIf_MasterChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannelMaster =
            &LinIf_MasterChannelInfo[currentChannel];

        boolean PerformCallouts = TRUE;
        if ( (pIfChannel->LastHeaderType == LINIF_FRM_EVENT_TRIGGERED) &&
             ((status == LIN_RX_BUSY) || (status == LIN_RX_ERROR))
           )
        {
            /* event triggered frame collision -> set event flags */
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
            LINIF_PRECONDITION_ASSERT( (LINIF_MAX_EV_TRGD * 8U) > pIfChannel->LastId, LINIF_API_MAINFUNCTION);
#endif /* LINIF_DEV_ERROR_DETECT == STD_ON */

            LINIF_SET_BIT(LinIf_EventTriggeredCollision, pIfChannel->LastId);
        }
        /* AUTOSAR 4.0 Rev 2 LINIF030: Status shall be checked again in later schedules if channel
       is busy and it is not yet the turn of the next schedule */
        else if ((pIfChannelMaster->ScheduleDelay > 1U) &&
                 ((status == LIN_RX_BUSY)||(status == LIN_TX_BUSY))
                )
        {
            pIfChannelMaster->StatusDelay++;
            /* Indicate that StatusCheck shall not do anything as the frame is
               not yet finished. */
            PerformCallouts = FALSE;
        }
        else
        {
            /* perform callouts */
        }

#if (LINIF_DEM_EVENTS_CONFIGURED)
        if (pIfChannel->LastDemEventsIndex != LINIF_FRAME_DEMEVENTS_NOT_AVAILABLE)
        {
#if (LINIF_PROD_ERR_HANDLING_TX_BIT == TS_PROD_ERR_REP_TO_DEM)
          if(LINIF_GET_TXBIT_FRAME_DEMEVENT_ID(pIfChannel->LastDemEventsIndex) != LINIF_INVALID_DEM_EVENT_ID)
          {
            LinIf_Status_HandleTxBitError(currentChannel, status, pPduInfo);
          }
#endif /* (LINIF_PROD_ERR_HANDLING_TX_BIT == TS_PROD_ERR_REP_TO_DEM) */
#if (LINIF_PROD_ERR_HANDLING_RX_CHECKSUM == TS_PROD_ERR_REP_TO_DEM)
          if(LINIF_GET_CHECKSUMERR_FRAME_DEMEVENT_ID(pIfChannel->LastDemEventsIndex) != LINIF_INVALID_DEM_EVENT_ID)
          {
            LinIf_Status_HandleRxChecksumError(currentChannel, status);
          }
#endif /* (LINIF_PROD_ERR_HANDLING_RX_CHECKSUM == TS_PROD_ERR_REP_TO_DEM) */
#if (LINIF_PROD_ERR_HANDLING_RX_NO_RESPONSE == TS_PROD_ERR_REP_TO_DEM)
          if(LINIF_GET_NORESP_FRAME_DEMEVENT_ID(pIfChannel->LastDemEventsIndex) != LINIF_INVALID_DEM_EVENT_ID)
          {
            LinIf_Status_HandleRxNoResponseError(currentChannel, status);
          }
#endif /* (LINIF_PROD_ERR_HANDLING_RX_NO_RESPONSE == TS_PROD_ERR_REP_TO_DEM) */
        }
#endif /* LINIF_DEM_EVENTS_CONFIGURED */

#if (LINIF_PROD_ERR_HANDLING_TX_BIT == TS_PROD_ERR_REP_TO_DET)
          LinIf_Status_HandleTxBitError(currentChannel, status, pPduInfo);
#endif /* (LINIF_PROD_ERR_HANDLING_TX_BIT == TS_PROD_ERR_REP_TO_DET) */
#if (LINIF_PROD_ERR_HANDLING_RX_CHECKSUM == TS_PROD_ERR_REP_TO_DET)
          LinIf_Status_HandleRxChecksumError(currentChannel, status);
#endif /* (LINIF_PROD_ERR_HANDLING_RX_CHECKSUM == TS_PROD_ERR_REP_TO_DET) */
#if (LINIF_PROD_ERR_HANDLING_RX_NO_RESPONSE == TS_PROD_ERR_REP_TO_DET)
          LinIf_Status_HandleRxNoResponseError(currentChannel, status);
#endif /* (LINIF_PROD_ERR_HANDLING_RX_NO_RESPONSE == TS_PROD_ERR_REP_TO_DET) */

        if(PerformCallouts)
        {
            const uint8 StatusFctIndex = LinIf_MapLinStatusToFctIndex(status);

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
            LINIF_PRECONDITION_ASSERT((StatusFctIndex < LINIF_STATUS_MATRIX_NUM_STATES),
                                      LINIF_API_MAINFUNCTION
                                     );

            LINIF_PRECONDITION_ASSERT((pIfChannel->LastHeaderType < LINIF_STATUS_MATRIX_NUM_HEADERTYPES),
                                      LINIF_API_MAINFUNCTION
                                     );
#endif /* LINIF_DEV_ERROR_DETECT == STD_ON */

            if ( (StatusFctIndex < LINIF_STATUS_MATRIX_NUM_STATES)
               &&(pIfChannel->LastHeaderType < LINIF_STATUS_MATRIX_NUM_HEADERTYPES)
               )
            {
                const LinIf_StatusFunctionType pfunction =
                    LinIf_StatusMatrix[pIfChannel->LastHeaderType][StatusFctIndex];

                if ( pfunction != NULL_PTR )
                {
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
                    LINIF_PRECONDITION_ASSERT((pfunction != &LinIf_Status_SNH),
                                              LINIF_API_MAINFUNCTION
                                             );
#endif /* LINIF_DEV_ERROR_DETECT == STD_ON */

                    pfunction(currentChannel,pPduInfo);
                }
            }
        }
    }

}

/** \brief LIN Interface Sleep Handling function, based on return value of Lin_GetStatus */
STATIC FUNC(void, LINIF_CODE) LinIf_HandleSleep
(
  uint8 currentChannel,
  uint8 SleepFlags,
  Lin_StatusType status
)
{

  const NetworkHandleType ComMChannel =
     LINIF_GET_COMM_CHANNEL(currentChannel);

  Std_ReturnType Result = E_NOT_OK;
  boolean wakeupRequested = ((SleepFlags & LINIF_WAKEUP_REQUESTED_FLAG) == LINIF_WAKEUP_REQUESTED_FLAG)?TRUE:FALSE;
  boolean success = FALSE;
  boolean callWakeupConf  = FALSE;
  boolean callSleepConf = FALSE;
  boolean callScheduleConf = FALSE;

  if(status == LIN_CH_SLEEP)
  {

      /* wakeup was requested during the go-to-sleep command */
      if(wakeupRequested)
      {
        /* !LINKSTO LinIf.ASR40.LINIF460, 2*/
        Result = LinIf_LinDriverConfig[LINIF_GET_LIN_DRIVER_INDEX(currentChannel)].wakeup(LINIF_GET_LIN_CHANNEL(currentChannel));
      }

      /* wakeup not possible, keep the sleep */
      if(Result == E_NOT_OK)
      {
        CONSTP2VAR(LinIf_ChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannel =
          &LinIf_ChannelInfo[currentChannel];
        CONSTP2VAR(LinIf_MasterChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannelMaster =
          &LinIf_MasterChannelInfo[currentChannel];

        const uint16 NullSchedule =
            LINIF_CFG_GET_CHANNEL_MASTER[currentChannel].SchTableInd;

        SchM_Enter_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();

        DBG_LINIF_CFSM_GRP(currentChannel,
                           (pIfChannel->cFSM),
                           (LINIF_CHANNEL_SLEEP)
                          );

        /* !LINKSTO LinIf.ASR42.LinIf_00460.E_NOT_OK.SLEEP, 1*/
        /* Set LinIf into sleep, there will be no activity on the bus */
        TS_AtomicAssign8(pIfChannel->cFSM, LINIF_CHANNEL_SLEEP);

        /* clear pending RUN_ONCE schedule tables */
        pIfChannelMaster->SchROTable = LINIF_NULL_SCHEDULE;

        DBG_LINIF_SCHCURRENTTABLE_GRP(currentChannel,
                                      (pIfChannelMaster->SchCurrentTable),
                                      (NullSchedule)
                                     );

        /* select NULL_SCHEDULE on current schedule table */
        pIfChannelMaster->SchCurrentTable = NullSchedule;
        pIfChannelMaster->SchContTable = NullSchedule;

        /* after sleep process, new schedule will
           be selected in next main function */
        pIfChannelMaster->ScheduleDelay = 1U;

        /* set SchEntry for the NULL_SCHEDULE */
        pIfChannelMaster->SchEntry =
            LINIF_CFG_GET_SCHTABLE[NullSchedule].EntryStartInd;

        SchM_Exit_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();

        /* !LINKSTO LinIf.ASR42.LinIf_00699, 2*/
        if(wakeupRequested)
        {
          /* !LINKSTO LinIf.ASR42.LinIf_00460.E_NOT_OK.WakeupConfirmation, 1*/
          callWakeupConf = TRUE;
          success = FALSE;
        }
        else
        {
          /* confirm successful transition to sleep, set null schedule */
          callSleepConf = TRUE;
          success = TRUE;
        }

        /* !LINKSTO LinIf.EB.ConfirmSwitchToNULLSCHEDULE, 1*/
        callScheduleConf = TRUE;
      }
      else
      {
        /* !LINKSTO LinIf.ASR42.LinIf_00460.E_OK.WakeupConfirmation, 1*/
        /* !LINKSTO LinIf.ASR42.LinIf_00460.E_OK.OPERATIONAL, 1*/
        callWakeupConf = TRUE;
        success = TRUE;
      }
  }
  else /* LIN_CH_OPERATIONAL */
  {
      /* !LINKSTO LinIf.ASR42.LinIf_00699, 2*/
      /* wakeup was requested during the go-to-sleep command */
      if(wakeupRequested)
      {
        /* !LINKSTO LinIf.EB.WakeupDuringGoToSleepNotSuccesfull, 1*/
        callWakeupConf = TRUE;
        success = TRUE;
      }
      else
      {
        /* goto sleep transition not successful */
        callSleepConf = TRUE;
        success = FALSE;
      }
  }

  if(callWakeupConf)
  {
    LINIF_CFG_GET_UL_WUCONF_FPTR(currentChannel)(ComMChannel, success);
  }

  if(callSleepConf)
  {
    LINIF_CFG_GET_UL_SLEEPCONF_FPTR(currentChannel)(ComMChannel, success);
  }

  if(callScheduleConf)
  {
    LINIF_CFG_GET_UL_SCHEDREQCONF_FPTR(currentChannel)(ComMChannel, LINIF_NULL_SCHEDULE);
  }

}
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */

#if (LINIF_SLAVE_SUPPORTED == STD_ON)
FUNC(void, LINIF_CODE) LinIf_SlaveHandleSleep
(
  uint8 currentChannel
)
{
  CONSTP2VAR(LinIf_ChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannel =
    &LinIf_ChannelInfo[currentChannel];
  CONSTP2CONST(LinIfSlaveChannelType, AUTOMATIC, LINIF_VAR) pIfChConfSlave =
      LINIF_CFG_GET_CHANNEL_SLAVE(currentChannel);
    CONSTP2VAR(LinIf_SlaveChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannelSlave =
      LINIF_GET_CHANNEL_SLAVE(currentChannel);
  uint8 sleepFlags;
  Std_ReturnType Result = E_OK;

  const NetworkHandleType ComMChannel =
     LINIF_GET_COMM_CHANNEL(currentChannel);

    /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00756.MRF_GTS,1,
                LinIf.ASR20-11.SWS_LinIf_00756.BusIdleTimeout,1
     */
    LINIF_CFG_GET_UL_SLEEPIND_FPTR(currentChannel)(ComMChannel);

    TS_AtomicAssign8(sleepFlags, pIfChannel->SleepHandling);
    if((sleepFlags & LINIF_SLEEP_ONGOING_FLAG) == LINIF_SLEEP_ONGOING_FLAG)
    {

        if((sleepFlags & LINIF_WAKEUP_REQUESTED_FLAG) == LINIF_WAKEUP_REQUESTED_FLAG)
        {
            /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00760_1,1,
                        LinIf.ASR20-11.SWS_LinIf_00760_2,1
             */
            Result = LinIf_LinDriverConfig[LINIF_GET_LIN_DRIVER_INDEX(currentChannel)].wakeup(LINIF_GET_LIN_CHANNEL(currentChannel));

            if(Result == E_OK)
            {
                TS_AtomicAssign8(pIfChannel->SleepHandling, LINIF_WAKEUP_CONFIRMED_FLAG);
            }
            else
            {
                TS_AtomicAssign8(pIfChannel->SleepHandling, LINIF_NO_SLEEP_REQUEST);
                LINIF_CFG_GET_UL_WUCONF_FPTR(currentChannel)(ComMChannel, FALSE);
            }

            LinIf_StartTimerBI(pIfChConfSlave, pIfChannelSlave);
        }
        else
        {
            /* enter critical region */
            SchM_Enter_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();

            pIfChannel->cFSM = LINIF_CHANNEL_SLEEP;

            pIfChannel->SleepHandling = LINIF_NO_SLEEP_REQUEST;

            /* leave critical region */
            SchM_Exit_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();

            /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00753,1 */
            LinIf_StopTimerBI(LINIF_GET_CHANNEL_SLAVE(currentChannel));

            /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00757,1,
                        LinIf.ASR20-11.SWS_LinIf_00759,1
             */
            LINIF_CFG_GET_UL_SLEEPCONF_FPTR(currentChannel)(ComMChannel, TRUE);
        }
    }
}
#endif /* LINIF_SLAVE_SUPPORTED == STD_ON */

#if (LINIF_MASTER_SUPPORTED == STD_ON)
/** \brief LIN Interface reschedule function
 *
 * This function verifies if rescheduling is necessary due to a requested
 * schedule table change from the upper layer or achievement of the schedule end
 */
STATIC FUNC(boolean, LINIF_CODE) LinIf_ReSchedule ( uint8 currentChannel )
{
    boolean Reschedule = FALSE;

    CONSTP2VAR(LinIf_MasterChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannelMaster =
        &LinIf_MasterChannelInfo[currentChannel];

    /* enter critical region */
    SchM_Enter_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();

    /* Verify that schedule table is not empty */
    if(LINIF_CFG_GET_SCHTABLE[pIfChannelMaster->SchCurrentTable].EntryStartInd != LINIF_INVALID_ENTRY)
    {
        /* check if we are at the end of the current table and have to reschedule */
        if(pIfChannelMaster->SchEntry > LINIF_CFG_GET_SCHTABLE[pIfChannelMaster->SchCurrentTable].EntryStopInd )
        {
            Reschedule = TRUE;
        }
    }
    else
    {
        /* if RUN_ONCE schedule table does not have an entry, reschedule */
        if(LINIF_CFG_GET_SCHTABLE[pIfChannelMaster->SchCurrentTable].RunMode == LINIF_RUN_ONCE)
        {
            Reschedule = TRUE;
        }
    }

    /* check if we are in a continuous schedule table and reschedule if ... */
    if(LINIF_CFG_GET_SCHTABLE[pIfChannelMaster->SchCurrentTable].RunMode != LINIF_RUN_ONCE)
    {
        /* ... we have a RUN_ONCE table pending */
        if((pIfChannelMaster->SchROTable != LINIF_NULL_SCHEDULE)
                /* ... or if the continuous table is to be restarted */
                /* !LINKSTO LinIf.ASR44.SWS_LinIf_00028.ReStartScheduleTable, 1*/
            || ((pIfChannelMaster->SchCurrentTable == pIfChannelMaster->SchContTable)
                && (pIfChannelMaster->RestartCurrentTable == TRUE))
                /* ... or if the continuous table was replaced by another continuous table */
            || (pIfChannelMaster->SchCurrentTable != pIfChannelMaster->SchContTable)
           )
        {
            Reschedule = TRUE;
        }
    }

    /* null schedule requested */
    if(pIfChannelMaster->ForceReschedule == TRUE)
    {
        Reschedule = TRUE;
    }

    /* leave critical region */
    SchM_Exit_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();

    return Reschedule;
}


/** \brief LIN Interface Schedule function
 *
 * This function selects the next Schedule table entry to be processed.
 * This includes selecting a new schedule table if necessary. The
 * ChannelInfo variables ScheduleDelay, FrameType and Id will be set up.
 */
STATIC FUNC(void, LINIF_CODE) LinIf_Schedule ( uint8 currentChannel )
{
    CONSTP2VAR(LinIf_ChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannel =
      &LinIf_ChannelInfo[currentChannel];

    CONSTP2VAR(LinIf_MasterChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannelMaster =
      &LinIf_MasterChannelInfo[currentChannel];

    boolean Reschedule = FALSE, CallScheduleConf = FALSE;

    /* enter critical region */
    SchM_Enter_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();

    Reschedule = LinIf_ReSchedule(currentChannel);

    if (Reschedule)
    {
        /* clear ForceReschedule flag all the time to save another "if" */
        pIfChannelMaster->ForceReschedule = FALSE;

        if (pIfChannelMaster->SchROTable != LINIF_NULL_SCHEDULE)
        {
            /* store ResumeSchEntry if old table was a resumable schedule table
               or if the collision resolution schedule is requested*/
            if( (LINIF_CFG_GET_SCHTABLE[pIfChannelMaster->SchCurrentTable].RunMode == LINIF_RUN_CONTINUOUS_RESUME)
                || (  (pIfChannel->FrameType == LINIF_FRM_EVENT_TRIGGERED)
                   && (pIfChannelMaster->SchROTable == pIfChannelMaster->SchCollRef)
                   )
              )
            {
                /* don't forget to handle a possible table wraparound */
                if( pIfChannelMaster->SchEntry > LINIF_CFG_GET_SCHTABLE[pIfChannelMaster->SchCurrentTable].EntryStopInd )
                {
                    pIfChannelMaster->ResumeSchEntry = LINIF_CFG_GET_SCHTABLE[pIfChannelMaster->SchCurrentTable].EntryStartInd;
                }
                else
                {
                    pIfChannelMaster->ResumeSchEntry = pIfChannelMaster->SchEntry;
                }
            }

            /* set the current table with the RO Table */
            DBG_LINIF_SCHCURRENTTABLE_GRP(currentChannel,
                                          (pIfChannelMaster->SchCurrentTable),
                                          (pIfChannelMaster->SchROTable)
                                         );

            pIfChannelMaster->SchCurrentTable = pIfChannelMaster->SchROTable;
            /* Reset the variable for the next call */
            pIfChannelMaster->SchROTable = LINIF_NULL_SCHEDULE;
            /* rescheduling was performed */

            CallScheduleConf = TRUE;
        }
        else
        {
            /* temporary variable for storing the "old" schedule table */
            uint16 OldSchTable = pIfChannelMaster->SchCurrentTable;

            /* switch to latest RUN_CONTINUOUS schedule table */
            DBG_LINIF_SCHCURRENTTABLE_GRP(currentChannel,
                                          (pIfChannelMaster->SchCurrentTable),
                                          (pIfChannelMaster->SchContTable)
                                         );

            pIfChannelMaster->SchCurrentTable = pIfChannelMaster->SchContTable;

            /* check if the switch resulted in a new schedule table */
            if( pIfChannelMaster->SchCurrentTable != OldSchTable )
            {
                CallScheduleConf = TRUE;
            }
            else
            {
                if (pIfChannelMaster->RestartCurrentTable == TRUE)
                {
                    /* the schedule table is being restarted, confirm the restart */
                    CallScheduleConf = TRUE;
                    pIfChannelMaster->RestartCurrentTable = FALSE;
                }
                /* schedule table reached the end - update resume point */
                pIfChannelMaster->ResumeSchEntry = LINIF_CFG_GET_SCHTABLE[pIfChannelMaster->SchCurrentTable].EntryStartInd;
            }
        }
        pIfChannelMaster->SchEntry = LinIf_Get_First_Sch_Entry(currentChannel);

        /* set resume last schedule flag if collision resolution schedule is selected*/
        if((pIfChannel->FrameType == LINIF_FRM_EVENT_TRIGGERED)
            && (pIfChannelMaster->SchCurrentTable == pIfChannelMaster->SchCollRef)
            )
        {
            pIfChannelMaster->ResumeSch = TRUE;
            pIfChannelMaster->SchCollRef = LINIF_INVALID_SCHEDULE_TABLE;
        }
    }

    /* Verify if schedule table has valid entries */
    if(LINIF_CFG_GET_SCHTABLE[pIfChannelMaster->SchCurrentTable].EntryStartInd != LINIF_INVALID_ENTRY)
    {
        pIfChannel->FrameType          = LINIF_CFG_GET_ENTRY[pIfChannelMaster->SchEntry].FrameType;
        pIfChannelMaster->ScheduleDelay      = LINIF_CFG_GET_ENTRY[pIfChannelMaster->SchEntry].Delay;
        pIfChannel->ThisId             = LINIF_CFG_GET_ENTRY[pIfChannelMaster->SchEntry].EntryIndex;
#if (LINIF_DEM_EVENTS_CONFIGURED)
        pIfChannel->ThisDemEventsIndex = LINIF_CFG_GET_ENTRY[pIfChannelMaster->SchEntry].DemEventId;
#endif /* LINIF_DEM_EVENTS_CONFIGURED */

        pIfChannelMaster->SchEntry++;

#if (LINIF_EOS_NOTIF_CALLOUT_SUPPORTED == STD_ON)
        /* !LINKSTO LinIf.EB.LinIfScheduleTableEndNotification,2 */
        if(pIfChannelMaster->SchEntry > LINIF_CFG_GET_SCHTABLE[pIfChannelMaster->SchCurrentTable].EntryStopInd)
        {
          pIfChannelMaster->CallEOSNotif = TRUE;
        }
#endif
    }
    else
    {
        pIfChannel->FrameType = LINIF_FRM_EMPTY;
    }

    /* leave critical region */
    SchM_Exit_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();

    if(CallScheduleConf)
    {

        LINIF_CFG_GET_UL_SCHEDREQCONF_FPTR(currentChannel)(LINIF_GET_COMM_CHANNEL(currentChannel),
                                                           (uint8)(pIfChannelMaster->SchCurrentTable -
                                                           LINIF_CFG_GET_CHANNEL_MASTER[currentChannel].SchTableInd)
                                                         );
    }

#if (LINIF_TP_SUPPORTED == STD_ON)

        LinTp_BswMScheduleChangePerformed(currentChannel);

#endif /* LINIF_TP_SUPPORTED == STD_ON */

}

/** \brief LIN Interface function to get first entry of schedule
 *
 *  If a schedule is switched this function returns the start
 *  position of the schedule.
 */
STATIC FUNC(uint16, LINIF_CODE) LinIf_Get_First_Sch_Entry( uint8 currentChannel )
{
    CONSTP2VAR(LinIf_MasterChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannelMaster =
      &LinIf_MasterChannelInfo[currentChannel];

      uint16 startPos;


    if ((LINIF_CFG_GET_SCHTABLE[pIfChannelMaster->SchCurrentTable].RunMode == LINIF_RUN_CONTINUOUS_RESUME)
        || (pIfChannelMaster->ResumeSch == TRUE)
        )
    {
        pIfChannelMaster->ResumeSch = FALSE;
        startPos = pIfChannelMaster->ResumeSchEntry;
    }
    else
    {
        startPos = LINIF_CFG_GET_SCHTABLE[pIfChannelMaster->SchCurrentTable].EntryStartInd;
    }


    return startPos;
}

/** \brief LIN Interface Setup Collision Resolution
 *  This function checks if the current frame is event triggered
 *  and a collision resolution is in progress. If thats the case
 *  the next unconditional frame is selected to request.
 */
STATIC FUNC(void, LINIF_CODE) LinIf_SetupCollisionRes( uint8 currentChannel )
{
    CONSTP2VAR(LinIf_ChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannel =
      &LinIf_ChannelInfo[currentChannel];


    /* If event triggered frame, check if collision resolution is running */
    if (pIfChannel->FrameType == LINIF_FRM_EVENT_TRIGGERED)
    {

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
        LINIF_PRECONDITION_ASSERT( (LINIF_MAX_EV_TRGD * 8U) > pIfChannel->ThisId,
                                  LINIF_API_MAINFUNCTION
                                 );
#endif /* LINIF_DEV_ERROR_DETECT == STD_ON */

        if(LINIF_BIT_IS_SET(LinIf_EventTriggeredCollision, pIfChannel->ThisId))
        {
            CONSTP2VAR(LinIf_MasterChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannelMaster =
                &LinIf_MasterChannelInfo[currentChannel];
            uint16 CollResSch = LINIF_CFG_GET_ENTRY[pIfChannelMaster->SchEntry - 1U].CollisionRef
                + LINIF_CFG_GET_CHANNEL_MASTER[currentChannel].SchTableInd;

            LinIf_InternalScheduleRequest(currentChannel, CollResSch);

            /* Collision resolution is made with the collision resolution
               schedule table configured.*/
            LINIF_CLEAR_BIT(LinIf_EventTriggeredCollision, pIfChannel->ThisId);

            /* save reference of collision resolution schedule */
            pIfChannelMaster->SchCollRef = CollResSch;
            /* proceed as normal*/
        }
    }

}

/** \brief LIN Interface Setup for Sporadic Frames */
STATIC FUNC(void, LINIF_CODE) LinIf_SetupSporadicFrame( uint8 currentChannel )
{
    uint16_least i;

    CONSTP2VAR(LinIf_ChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannel =
      &LinIf_ChannelInfo[currentChannel];


    if (pIfChannel->FrameType == LINIF_FRM_SPORADIC)
    {
        /* search associated unconditional frames for flagged one */
        i = LINIF_CFG_GET_FRAMELISTSEP[pIfChannel->ThisId];

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
        LINIF_PRECONDITION_ASSERT( (LINIF_MAX_PDUS_FLAGS * 8U) > LINIF_CFG_GET_FRAMELIST[i],
                                  LINIF_API_MAINFUNCTION
                                 );
#endif /* LINIF_DEV_ERROR_DETECT == STD_ON */

        while ( (i < LINIF_CFG_GET_FRAMELISTSEP[pIfChannel->ThisId + 1U]) &&
                (LINIF_BIT_IS_SET(LinIf_FrameEventFlags, LINIF_CFG_GET_FRAMELIST[i]) == 0))
        {
            i++;
        }
        if ( i == LINIF_CFG_GET_FRAMELISTSEP[pIfChannel->ThisId + 1U])
        {
            /* nothing found. No frame requested */
            pIfChannel->FrameType = LINIF_FRM_EMPTY;
        }
        else
        {
            /* set Infos according to the Frame found */
            pIfChannel->ThisId = LINIF_CFG_GET_FRAMELIST[i];
            pIfChannel->FrameType = LINIF_FRM_TX;
        }
    }

}

/** \brief This function assembles the Lin PduInfo which should
 *  be sent next with the function Lin_SendFrame.
 */
STATIC FUNC(uint8, LINIF_CODE) LinIf_SetupFrame (
  uint8 currentChannel,
  P2VAR(Lin_PduType, AUTOMATIC, LINIF_APPL_DATA) pLinPduInfo
)
{

    uint8 retCode = LINIF_FRM_EMPTY;
#if (LINIF_BUS_MIRRORING_SUPPORT_ENABLE == STD_ON)
    uint8 i;
#endif /* LINIF_BUS_MIRRORING_SUPPORT_ENABLE == STD_ON */
    Std_ReturnType ret = E_NOT_OK;

    CONSTP2VAR(LinIf_ChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannel =
      &LinIf_ChannelInfo[currentChannel];


    /* Verify if unconditional frame is marked
       for transmission in a sporadic frame slot */
    LinIf_SetupSporadicFrame(currentChannel);

    /* Get data which should be transmitted */
    ret = LinIf_GetFrameData(currentChannel, pLinPduInfo->SduPtr);

    if(ret == E_OK)
    {
        Lin_FrameDlType dataLength = 0U;
        uint16 StatusDelay = 0U;
        uint8 FrameType = pIfChannel->FrameType;

        retCode = pIfChannel->FrameType;

        switch (FrameType)
        {
#if (LINIF_TP_SUPPORTED == STD_ON)
            /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00672.Master,1 */
            case LINIF_FRM_MRF:
              /* fall through */
#endif /*LINIF_TP_SUPPORTED == STD_ON*/
            case LINIF_FRM_CONFIG:
            {
                /* checksum of MRF is always classic */
                pLinPduInfo->Pid = 60U;
                pLinPduInfo->Drc = LINIF_LIN_RESP_MR_TX;
                pLinPduInfo->Cs = LIN_CLASSIC_CS;
                dataLength = 8U;
                break;
            }
            case LINIF_FRM_SRF:
            {
                /* checksum of SRF is always classic */
                pLinPduInfo->Pid = 125U;
                pLinPduInfo->Drc = LINIF_LIN_RESP_SL_RX;
                pLinPduInfo->Cs = LIN_CLASSIC_CS;
                dataLength = 8U;
                pIfChannel->LastDl =  dataLength; /* Required for next LinIf_GetStatus() */
                break;
            }
            case LINIF_FRM_RX:
              /* fall through */
            case LINIF_FRM_TX:
              /* fall through */
            case LINIF_FRM_EVENT_TRIGGERED:
            {
                LinIfFrameInfoType FrameInfo;

                FrameInfo.DLFlags = 0U;
                FrameInfo.PID = 0U;

                LinIf_GetFrameInfo(FrameType, pIfChannel->ThisId, &FrameInfo, &pLinPduInfo->Drc);

                pLinPduInfo->Cs =
                  (((FrameInfo.DLFlags & LINIF_CS_MASK) != 0U)
                  ? (Lin_FrameCsModelType)LIN_ENHANCED_CS
                  : (Lin_FrameCsModelType)LIN_CLASSIC_CS
                  );

                pLinPduInfo->Pid = FrameInfo.PID;
                dataLength = FrameInfo.DLFlags & LINIF_DL_MASK;
                pIfChannel->LastDl = dataLength; /* Required for next LinIf_GetStatus() */
                break;
            }
            case LINIF_FRM_SLAVE_TO_SLAVE:
            {
                /* checksum model and data length are not used, Lin_GetStatus is not called */
                pLinPduInfo->Pid = LINIF_CFG_GET_SLAVEFRAME[pIfChannel->ThisId];
                pLinPduInfo->Drc = LINIF_LIN_RESP_S2S_IGN;
                break;
            }
            default:
            {
                /* schedule entry does not contain any frame */
                pLinPduInfo->Pid = 0U;
                retCode = LINIF_FRM_EMPTY;
                break;
            }
        }

        if(dataLength != 0U)
        {
            /* set delay of status call according to payload length of frame */
            StatusDelay =
                LINIF_CFG_GET_CHANNEL_MASTER[currentChannel].DelayTime[dataLength - 1U];
        }

        pLinPduInfo->Dl = dataLength;

        pIfChannel->LastId = pIfChannel->ThisId;
#if (LINIF_DEM_EVENTS_CONFIGURED)
        pIfChannel->LastDemEventsIndex = pIfChannel->ThisDemEventsIndex;
#endif /* LINIF_DEM_EVENTS_CONFIGURED */
        pIfChannel->LastHeaderType = pIfChannel->FrameType;
#if (LINIF_BUS_MIRRORING_SUPPORT_ENABLE == STD_ON)
        pIfChannel->FrameInfo.Pid = pLinPduInfo->Pid;
        pIfChannel->FrameInfo.FrameLength = dataLength;
        for (i = 0U; i < dataLength; i++ )
        {
          pIfChannel->FrameInfo.FrameData[i] = pLinPduInfo->SduPtr[i];
        }
#endif /* LINIF_BUS_MIRRORING_SUPPORT_ENABLE == STD_ON */
        LinIf_MasterChannelInfo[currentChannel].StatusDelay = StatusDelay;
    }


    return retCode;
}

 /** \brief This function fetches data from the upper layer or
  *  the configuration
  */
STATIC FUNC(void, LINIF_CODE) LinIf_GetFrameInfo
(
  uint8 FrameType,
  PduIdType SduId,
  P2VAR(LinIfFrameInfoType, AUTOMATIC, LINIF_APPL_DATA) pFrameInfo,
  P2VAR(Lin_FrameResponseType, AUTOMATIC, LINIF_APPL_DATA) pResponseType
)
{

    switch(FrameType)
    {
        case LINIF_FRM_RX:
        {
            *pResponseType = LINIF_LIN_RESP_SL_RX;
            *pFrameInfo = LINIF_CFG_GET_RXFRAME[SduId];
            break;
        }
        case LINIF_FRM_TX:
        {
            *pResponseType = LINIF_LIN_RESP_MR_TX;
            *pFrameInfo = LINIF_CFG_GET_TXFRAME[SduId];
            break;
        }
        case LINIF_FRM_EVENT_TRIGGERED:
        {
            *pResponseType = LINIF_LIN_RESP_SL_RX;
            *pFrameInfo = LINIF_CFG_GET_EVTTRIGGERED[SduId];
            break;
        }
        /* CHECK: NOPARSE */
        default:
            /*should not happen*/
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
            LINIF_UNREACHABLE_CODE_ASSERT(LINIF_API_MAINFUNCTION);
#endif /* LINIF_DEV_ERROR_DETECT == STD_ON */
            break;
        /* CHECK: PARSE */
    }

}

/** \brief This function gets Pdu id, Pid, checksum type  and response
 *  type of unconditional and event triggered frames
 */
STATIC FUNC(Std_ReturnType, LINIF_CODE) LinIf_GetFrameData
(
  uint8 currentChannel,
  P2VAR(uint8, AUTOMATIC, LINIF_APPL_DATA) pFrameData
)
{
    Std_ReturnType ret;

    CONSTP2VAR(LinIf_ChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannel =
         &LinIf_ChannelInfo[currentChannel];


    switch (pIfChannel->FrameType)
    {
        case LINIF_FRM_TX:
        {
            PduInfoType pduInfo;
            PduIdType PduRDestPduHandleId =
                LINIF_CFG_GET_TXFRAME[pIfChannel->ThisId].ULPduHandleId;

            pduInfo.SduDataPtr = pFrameData;

            /* copy data from PduR */
            /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00706.Sporadic,1,
                        LinIf.ASR20-11.SWS_LinIf_00706.Unconditional,1
             */
            ret = LINIF_CFG_GET_UL_TRGTX_FPTR(pIfChannel->ThisId)(PduRDestPduHandleId, &pduInfo);
            break;
        }
#if (LINIF_TP_SUPPORTED == STD_ON)
        case LINIF_FRM_MRF:
        {
            PduInfoType pduInfo;
            pduInfo.SduDataPtr = pFrameData;

            /* copy data from PduR */
            ret = LinTp_TriggerTransmit(currentChannel,&pduInfo);
            break;
        }
#endif /*LINIF_TP_SUPPORTED == STD_ON*/
        case LINIF_FRM_CONFIG:
        {
            /* copy data of fixed frame */
            TS_MemCpy(pFrameData,
                      LINIF_CFG_GET_FIXEDFRAMES[pIfChannel->ThisId].FixedFrameSduBytes,
                      8U
                     );
            ret = E_OK;
            break;
        }
#if (LINIF_TP_SUPPORTED == STD_ON)
        case LINIF_FRM_SRF:
        {
            /* verify that PduR can provide enough buffer */
            ret = LinTp_TriggerReceive(currentChannel);
            break;
        }
#endif /*LINIF_TP_SUPPORTED == STD_ON*/
        default:
            ret = E_OK;
            break;
    }


    return ret;
}

/** \brief call PDU Router Receive Indication for event triggered frames*/
STATIC FUNC(void, LINIF_CODE) LinIf_Status_EvTrgRxInd(uint8 currentChannel, P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo)
{
    uint16 i;

    const uint16 entryStartInd= LINIF_CFG_GET_RXFRAMELISTSEP[currentChannel];
    const uint16 entryStopInd = LINIF_CFG_GET_RXFRAMELISTSEP[currentChannel + 1U];


    for(i = entryStartInd; i < entryStopInd; i++)
    {
        if(LINIF_CFG_GET_RXFRAME[i].PID == pPduInfo->SduDataPtr[0U])
        {
            LINIF_CFG_GET_UL_RXIND_FPTR(i)(LINIF_CFG_GET_RXFRAME[i].ULPduHandleId, pPduInfo);
            break; /* there may not be more than one */
        }
    }

}

/* Status Check Helper Functions */

#if (LINIF_LIN_ERROR_CALLOUT == STD_ON)

/** \brief This function calls a user configured callout
 *
 *  \param currentChannel  Channel which is currently processed
 *  \param pPduInfo        received data
 */
STATIC FUNC(void, LINIF_CODE) LinIf_Status_ErrorCalloutAndDet
(
    uint8 currentChannel,
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo
)
{
#if (LINIF_LIN_ERROR_CALLOUT_STATUS_FWD == STD_OFF)
  TS_PARAM_UNUSED(pPduInfo);
#endif /* LINIF_LIN_ERROR_CALLOUT_STATUS_FWD == STD_OFF */

#if (LINIF_RUNTIME_ERROR_REPORTING == STD_ON)
    (void)Det_ReportRuntimeError( LINIF_MODULE_ID,
                                  LINIF_INSTANCE_ID,
                                  LINIF_API_MAINFUNCTION,
                                  LINIF_E_RESPONSE
                                );
#endif /* LINIF_RUNTIME_ERROR_REPORTING == STD_ON */

    /* In order to have it integrated with the rest of the functions, the status from
     * Lin_GetStatus() is passed on in the SduLength member.
     * Otherwise the parameter isn't used.
     */
    /* Deviation MISRAC2012-2 */
    LINIF_LIN_ERROR_NOTIFY(currentChannel, (Lin_StatusType)pPduInfo->SduLength);
}

#endif /* (LINIF_LIN_ERROR_CALLOUT == STD_ON) */

#if (LINIF_RUNTIME_ERROR_REPORTING == STD_ON)
/** \brief notify DET of LINIF_E_RESPONSE failure */
STATIC FUNC(void, LINIF_CODE) LinIf_Status_DET
(
    uint8 currentChannel,
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo
)
{
    TS_PARAM_UNUSED(currentChannel);
    TS_PARAM_UNUSED(pPduInfo);

    (void)Det_ReportRuntimeError( LINIF_MODULE_ID,
                                  LINIF_INSTANCE_ID,
                                  LINIF_API_MAINFUNCTION,
                                  LINIF_E_RESPONSE
                                );
}
#endif /* LINIF_RUNTIME_ERROR_REPORTING == STD_ON */

#if (LINIF_DEV_ERROR_DETECT == STD_ON)

 /** \brief This function indicates an illegal return value of Lin_GetStatus */
STATIC FUNC(void, LINIF_CODE) LinIf_Status_SNH
(
    uint8 currentChannel,
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo
)
{
    TS_PARAM_UNUSED(currentChannel);
    TS_PARAM_UNUSED(pPduInfo);
}

#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) */

#endif /* LINIF_MASTER_SUPPORTED == STD_ON */

/** \brief call PDU Router Transmit Confirmation */
FUNC(void, LINIF_CODE) LinIf_Status_PduRTxConf
(
    uint8 currentChannel,
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo
)
{
    CONSTP2VAR(LinIf_ChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannel =
      &LinIf_ChannelInfo[currentChannel];


    TS_PARAM_UNUSED(pPduInfo);

    LINIF_CFG_GET_UL_TXCONF_FPTR(pIfChannel->LastId)(LINIF_CFG_GET_TXFRAME[pIfChannel->LastId].ULPduHandleId);

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    LINIF_PRECONDITION_ASSERT( (LINIF_MAX_PDUS_FLAGS * 8U) > pIfChannel->LastId, LINIF_API_MAINFUNCTION);
#endif /* LINIF_DEV_ERROR_DETECT == STD_ON */

    /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00732.EvtTrg,1,
                LinIf.ASR20-11.SWS_LinIf_00732.Uncond,1
     */
    LINIF_CLEAR_BIT_ATOMIC(LinIf_FrameEventFlags, pIfChannel->LastId);

    LINIF_LIN_SUCCESS_NOTIFY(currentChannel, LIN_TX_OK);
}


/** \brief call PDU Router Receive Indication */
FUNC(void, LINIF_CODE) LinIf_Status_PduRRxInd
(
    uint8 currentChannel,
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo
)
{
    CONSTP2VAR(LinIf_ChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannel =
      &LinIf_ChannelInfo[currentChannel];

    LINIF_CFG_GET_UL_RXIND_FPTR(pIfChannel->LastId)(LINIF_CFG_GET_RXFRAME[pIfChannel->LastId].ULPduHandleId,
        pPduInfo);

    LINIF_LIN_SUCCESS_NOTIFY(currentChannel, LIN_RX_OK);
}


#if (LINIF_PROD_ERR_HANDLING_TX_BIT != TS_PROD_ERR_DISABLE)

/** \brief This function calls a Det or the Dem module, which
 *  indicates that LIN_TX_BIT_ERROR response was received
 *  from a slave
 */
FUNC(void, LINIF_CODE) LinIf_Status_HandleTxBitError
(
    uint8 currentChannel,
    Lin_StatusType status,
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo
)
{
    CONSTP2VAR(LinIf_ChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannel =
        &LinIf_ChannelInfo[currentChannel];

#if (LINIF_PROD_ERR_HANDLING_TX_BIT == TS_PROD_ERR_REP_TO_DEM)
    TS_PARAM_UNUSED(currentChannel);
#endif /* (LINIF_PROD_ERR_HANDLING_TX_BIT != TS_PROD_ERR_REP_TO_DEM) */
#if (LINIF_TP_SUPPORTED == STD_OFF)
    TS_PARAM_UNUSED(pPduInfo);
#endif /* LINIF_TP_SUPPORTED == STD_OFF */

    /* check if there is a problem */
    if(status == LIN_TX_BIT_ERROR)
    {
#if(LINIF_TP_SUPPORTED == STD_ON)
      /* if the frame was a MRF transmission, abort */
      if(pIfChannel->LastHeaderType == LINIF_FRM_MRF)
      {
        LinTp_Status_Err(currentChannel,pPduInfo);
      }
#endif /* LINIF_TP_SUPPORTED == STD_ON */

      /* check if the issue will be reported to DEM */
#if (LINIF_PROD_ERR_HANDLING_TX_BIT == TS_PROD_ERR_REP_TO_DEM)
      /* if the frame has a DemEvent for LIN_TX_BIT_ERROR then report
         the problem */
      Dem_ReportErrorStatus(LINIF_GET_TXBIT_FRAME_DEMEVENT_ID(pIfChannel->LastDemEventsIndex), LINIF_TX_BIT_FAILED);

#else /* issue will be reported to DET */

      (void)Det_ReportError( LINIF_MODULE_ID,
                             currentChannel,
                             LINIF_API_MAINFUNCTION,
                             LINIF_E_DEMTODET_TX_BIT
                           );

#endif /* (LINIF_PROD_ERR_HANDLING_TX_BIT != TS_PROD_ERR_REP_TO_DEM) */
    }
#if (LINIF_PROD_ERR_HANDLING_TX_BIT == TS_PROD_ERR_REP_TO_DEM)
    else
    {
      /* if the frame has a DemEvent for LIN_TX_BIT_ERROR then report
         no problem */
      Dem_ReportErrorStatus(LINIF_GET_TXBIT_FRAME_DEMEVENT_ID(pIfChannel->LastDemEventsIndex), LINIF_TX_BIT_PASSED);
    }
#endif /* (LINIF_PROD_ERR_HANDLING_TX_BIT == TS_PROD_ERR_REP_TO_DEM) */
}

#endif /* (LINIF_PROD_ERR_HANDLING_TX_BIT != TS_PROD_ERR_DISABLE) */

#if (LINIF_PROD_ERR_HANDLING_RX_CHECKSUM != TS_PROD_ERR_DISABLE)

/** \brief This function calls a Det or the Dem module, which
 *  indicates that LIN_RX_CHECKSUM_ERROR response was received
 *  from a slave
 */
FUNC(void, LINIF_CODE) LinIf_Status_HandleRxChecksumError
(
    uint8 currentChannel,
    Lin_StatusType status
)
{
#if (LINIF_PROD_ERR_HANDLING_RX_CHECKSUM == TS_PROD_ERR_REP_TO_DEM)
    CONSTP2VAR(LinIf_ChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannel =
        &LinIf_ChannelInfo[currentChannel];

    TS_PARAM_UNUSED(currentChannel);
#endif /* (LINIF_PROD_ERR_HANDLING_RX_CHECKSUM != TS_PROD_ERR_REP_TO_DEM) */

    /* check if there is a problem */
    if(status == LIN_RX_CHECKSUM_ERROR)
    {
    /* check if the issue will be reported to DEM */
#if (LINIF_PROD_ERR_HANDLING_RX_CHECKSUM == TS_PROD_ERR_REP_TO_DEM)
      /* if the frame has a DemEvent for LIN_RX_CHECKSUM_ERROR then report
         the problem */
      Dem_ReportErrorStatus(LINIF_GET_CHECKSUMERR_FRAME_DEMEVENT_ID(pIfChannel->LastDemEventsIndex), LINIF_RX_CHECKSUM_FAILED);

#else /* issue will be reported to DET */

      (void)Det_ReportError( LINIF_MODULE_ID,
                             currentChannel,
                             LINIF_API_MAINFUNCTION,
                             LINIF_E_DEMTODET_RX_CHECKSUM
                           );

#endif /* (LINIF_PROD_ERR_HANDLING_RX_CHECKSUM != TS_PROD_ERR_REP_TO_DEM) */
    }
#if (LINIF_PROD_ERR_HANDLING_RX_CHECKSUM == TS_PROD_ERR_REP_TO_DEM)
    else
    {
      /* if the frame has a DemEvent for LIN_RX_CHECKSUM_ERROR then report
         no problem */
      Dem_ReportErrorStatus(LINIF_GET_CHECKSUMERR_FRAME_DEMEVENT_ID(pIfChannel->LastDemEventsIndex), LINIF_RX_CHECKSUM_PASSED);
    }
#endif /* (LINIF_PROD_ERR_HANDLING_RX_CHECKSUM == TS_PROD_ERR_REP_TO_DEM) */
}

#endif /* (LINIF_PROD_ERR_HANDLING_RX_CHECKSUM != TS_PROD_ERR_DISABLE) */

#if (LINIF_PROD_ERR_HANDLING_RX_NO_RESPONSE != TS_PROD_ERR_DISABLE)

/** \brief This function calls a Det or the Dem module, which
 *  indicates that LIN_RX_NO_RESPONSE_ERROR response was received
 *  from a slave
 */
FUNC(void, LINIF_CODE) LinIf_Status_HandleRxNoResponseError
(
    uint8 currentChannel,
    Lin_StatusType status
)
{
#if (LINIF_PROD_ERR_HANDLING_RX_NO_RESPONSE == TS_PROD_ERR_REP_TO_DEM)
    CONSTP2VAR(LinIf_ChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannel =
        &LinIf_ChannelInfo[currentChannel];

    TS_PARAM_UNUSED(currentChannel);
#endif /* (LINIF_PROD_ERR_HANDLING_RX_NO_RESPONSE != TS_PROD_ERR_REP_TO_DEM) */

    /* check if there is a problem */
    if(status == LIN_RX_NO_RESPONSE_ERROR)
    {
    /* check if the issue will be reported to DEM */
#if (LINIF_PROD_ERR_HANDLING_RX_NO_RESPONSE == TS_PROD_ERR_REP_TO_DEM)
      /* if the frame has a DemEvent for LIN_RX_NO_RESPONSE_ERROR then report
         the problem */
      Dem_ReportErrorStatus(LINIF_GET_NORESP_FRAME_DEMEVENT_ID(pIfChannel->LastDemEventsIndex), LINIF_RX_NO_RESPONSE_FAILED);

#else /* issue will be reported to DET */

      (void)Det_ReportError( LINIF_MODULE_ID,
                             currentChannel,
                             LINIF_API_MAINFUNCTION,
                             LINIF_E_DEMTODET_RX_NO_RESPONSE
                           );

#endif /* (LINIF_PROD_ERR_HANDLING_RX_NO_RESPONSE != TS_PROD_ERR_REP_TO_DEM) */
    }
#if (LINIF_PROD_ERR_HANDLING_RX_NO_RESPONSE == TS_PROD_ERR_REP_TO_DEM)
    else
    {
      /* if the frame has a DemEvent for LIN_RX_NO_RESPONSE_ERROR then report
         no problem */
      Dem_ReportErrorStatus(LINIF_GET_NORESP_FRAME_DEMEVENT_ID(pIfChannel->LastDemEventsIndex), LINIF_RX_NO_RESPONSE_PASSED);
    }
#endif /* (LINIF_PROD_ERR_HANDLING_RX_NO_RESPONSE == TS_PROD_ERR_REP_TO_DEM) */
}

#endif /* (LINIF_PROD_ERR_HANDLING_RX_NO_RESPONSE != TS_PROD_ERR_DISABLE) */

/* AUTOSAR Memory Mapping - end section for code */
#define LINIF_STOP_SEC_CODE
#include <LinIf_MemMap.h>

/*==================[end of file]===========================================*/
