/**
 * \file
 *
 * \brief AUTOSAR Com
 *
 * This file contains the implementation of the AUTOSAR
 * module Com.
 *
 * \version 6.3.50
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* MISRA-C:2012 Deviation List
 *
 *
 * MISRAC2012-1) Deviated Rule: 11.5 (advisory)
 * A conversion should not be performed from pointer to
 * void into pointer to object.
 *
 * Reason:
 * The memory routines are optimized for dealing with aligned
 * memory sections.
 *
 */

/*==================[inclusions]============================================*/


#include <Com_Trace.h>
#include <TSAutosar.h>          /* EB specific standard types */
#include <ComStack_Types.h>     /* AUTOSAR Standard types */
#include <Com_Api.h>                /* Module public API */

#include <Com_Priv.h>           /* Module private API */

#include <TSCompiler.h>           /* usage of macro in TSCompiler_Default.h */

#if (COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL)

#include <Com_Core_RxUpdateBit.h> /* UpdateBit Functions */

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_START_SEC_CODE
#include <Com_MemMap.h>
#endif

#if (COM_RX_SIGNAL_BASED_DM == STD_ON)

/** \brief Com_EB_DisableSignalBasedDMFlag - disables
 * signal based DM
 * \param[in] IPduPtr - configuration of the I-Pdu
 * \param[in] Initialize - defines if timeout flag shall be cleared
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_DisableSignalBasedDMFlag
(
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   boolean Initialize
);

/** \brief Com_EB_DisableSignalGroupBasedDMFlag - disables
 * signal group based DM
 * \param[in] IPduPtr - configuration of the I-Pdu
 * \param[in] Initialize - defines if timeout flag shall be cleared
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_DisableSignalGroupBasedDMFlag
(
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   boolean Initialize
);
#endif /* (COM_RX_SIGNAL_BASED_DM == STD_ON) */

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/** \brief Com_EB_CheckRxSignalGoupUpdateBitAndDM - handles update bit and DM of
 * Rx signals groups
 * this function evaluates the update bit and DM of Rx signals groups and
 * returns whether the value has to be replaced or not
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * \param[in] SignalGPtr - reference to the description of the signal group
 * \param[in] IPduPtr - reference to the description of the signal group
 * \param[in] pduLength - length of Pdu
 * \return Function execution success status
 * \retval TRUE if the signal was updated, else FALSE
 */
TS_MOD_PRIV_DEFN FUNC(boolean, COM_CODE) Com_EB_CheckRxSignalGoupUpdateBitAndDM
(
   P2CONST(ComRxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr,
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   uint32 pduLength
)
{
  uint8 SignalUpdated = 1U;

  CONSTP2CONST(ComRxSignalExtType, AUTOMATIC, COM_APPL_CONST) SignalExtPtr =
      COM_GET_CONFIG_ELEMENT(ComRxSignalExtType,
                             Com_gConfigPtr->ComRxSignalExtRef, SignalGPtr->ComSignalExtRef);

#if (COM_UPDATE_BIT_RX_CONFIG == COM_UPDATE_BIT_INDIVIDUAL)
  if (COM_GET_RX_SIGNAL_GROUP_UPDATE_BIT(SignalExtPtr) == COM_UPDATE_BIT_PRESENT)
#endif /* (COM_UPDATE_BIT_RX_CONFIG == COM_UPDATE_BIT_INDIVIDUAL) */
  { /* update bit is available, check if bit is 1 */

    uint32 const UpdateBitByte = ((uint32)SignalExtPtr->ComUpdateBitPositionInBit) / 8U;

#if ((COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_OFF) && (COM_HANDLESMALLERRXPDUS_NOTIFY == STD_ON))
    /* the Pdu is maybe shorter than expected, but the update bit was not set during signal copying,
     * Therefore we have to check if update bit is received
     */
    uint8 const updateBitReceived =  (pduLength > UpdateBitByte);
    if (updateBitReceived == TRUE)
#else
      TS_PARAM_UNUSED(pduLength);
#endif
    {

      TS_GetBit(&Com_gDataMemPtr[IPduPtr->ComIPduValueRef + UpdateBitByte],
                (SignalExtPtr->ComUpdateBitPositionInBit % 8U),
                uint8,
                SignalUpdated);
    }

#if (COM_RX_SIGNAL_BASED_DM == STD_ON)
         if (SignalUpdated == 1U)
         {

            /* check if signal group has a signal group based DM configured */
            if (COM_GET_RX_SIGNAL_GROUP_SIGDM(SignalExtPtr) == COM_RX_SIG_DM_ON)
            {

               /* calculate the position of the flags */
               CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) RxDMFlagPtr =
                     COM_RX_SIGNAL_GROUP_DM_FLAGS_BYTE_POS(SignalExtPtr);

               /* calculate position of the running flag */
               uint8 const RunningFlag =
                     (uint8)COM_RX_SIGNAL_GROUP_DM_RUNNING_BIT_POS(SignalExtPtr);

               /* calculate position of the timeout flag */
               uint8 const ToutFlag =
                     (uint8)COM_RX_SIGNAL_GROUP_DM_TIMEOUT_BIT_POS(SignalExtPtr);

               Com_RxDMNextDueTimeType const TimeOutOffset =
                     COM_GET_RX_SIGNAL_GROUP_TIMEOUT_FACTOR(SignalExtPtr);

               /* enter critical section */
               Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

               /* handle RxDM if enabled */
               if ( Com_IsBitSet_8(RxDMFlagPtr, RunningFlag) )
               {
                  /* Clear the timeout flag */
                  Com_ClearBitUInt8(RxDMFlagPtr, ToutFlag);

                  /* signal group based DM is enabled therefore rewind */
                  Com_EB_RxDMSignalGroupQueue(SignalGPtr, TimeOutOffset
#if (COM_TIMEBASE == STD_OFF)
                      , IPduPtr->ComMainFunctionsRx_Idx
#endif /* (COM_TIMEBASE == STD_OFF) */
                    );
               }
#if (COM_RX_FIRST_TIMEOUT_FACTOR_SIZE != COM_SIZE_0_BIT)
               else
               {
                  ComIPduRefType const ComRxPduId = SignalGPtr->ComIPduRef;

                  /* pointer to byte with status flags of the Pdu */
                  CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) IPduFlagBytePtr =
                           &Com_gDataMemPtr[(Com_gConfigPtr->ComRxIPduStatusBase + ComRxPduId)];

                  /* RxDM is not running, check if shall be enabled */
                  /* check if RxDM is switched on */
                  if ( Com_IsBitSet_8(IPduFlagBytePtr, COM_RXPDU_RXDMSWITCHON) )
                  {
                     /* Clear the timeout flag */
                     Com_ClearBitUInt8(RxDMFlagPtr, ToutFlag);

                     /* enable RxDM */
                     Com_SetBitUInt8(RxDMFlagPtr, RunningFlag);

                     /* signal group based DM is enabled therefore rewind */
                     Com_EB_RxDMSignalGroupQueue(SignalGPtr, TimeOutOffset
#if (COM_TIMEBASE == STD_OFF)
                      , IPduPtr->ComMainFunctionsRx_Idx
#endif /* (COM_TIMEBASE == STD_OFF) */
                    );
                  }

               }
#endif /* (COM_RX_FIRST_TIMEOUT_FACTOR_SIZE != COM_SIZE_0_BIT) */

               /* leave critical section */
               Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();


            }                   /* if (COM_GET_RX_SIGNAL_GROUP_SIGDM(SignalExtPtr) == COM_RX_SIG_DM_ON) */

         }                      /* if (SignalUpdated == 1U) */
#endif /* (COM_RX_SIGNAL_BASED_DM == STD_ON) */

      }                         /* update bit is available, check if bit is 1 */
      return SignalUpdated;
}                               /* Com_EB_CheckRxSignalGoupUpdateBitAndDM */


/** \brief Com_EB_CheckRxSignalUpdateBitAndDM - handles update bit and DM of
 * Rx signals this function evaluates the update bit and DM of Rx signals and
 * returns whether the value has to be replaced or not
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * \param[in] SignalPtr - reference to the description of the signal
 * \param[in] IPduPtr - reference to the description of the signal
 * \param[in,out] passFilter_MASKED_NEW_DIFFERS_MASKED_OLD -
 *         in: indicates if the Pdu base RxDM has been just restarted
 *         out: If signal based RxDM has just been restarted after a time out, the value is set to 1.
                If signal based RxDM did not timed out before, value is set to 0.
                If no update bit is configured, value is not modified.
 * \return Function execution success status
 * \retval TRUE if the signal was updated, else FALSE
 */
TS_MOD_PRIV_DEFN FUNC(boolean, COM_CODE) Com_EB_CheckRxSignalUpdateBitAndDM
(
   P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) SignalPtr,
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   P2VAR(uint8, AUTOMATIC, AUTOMATIC) passFilter_MASKED_NEW_DIFFERS_MASKED_OLD,
   uint32 pduLength
)
{
  uint8 SignalUpdated = 1U;

  CONSTP2CONST(ComRxSignalExtType, AUTOMATIC, COM_APPL_CONST) SignalExtPtr =
      COM_GET_CONFIG_ELEMENT(ComRxSignalExtType,
                             Com_gConfigPtr->ComRxSignalExtRef, SignalPtr->ComSignalExtRef);

#if (COM_UPDATE_BIT_RX_CONFIG == COM_UPDATE_BIT_INDIVIDUAL)
  if (COM_GET_RX_SIGNAL_EXT_UPDATE_BIT(SignalExtPtr) == COM_UPDATE_BIT_PRESENT)
#endif /* (COM_UPDATE_BIT_RX_CONFIG == COM_UPDATE_BIT_INDIVIDUAL) */
  { /* update bit is available, check if bit is 1 */

    uint32 const UpdateBitByte = ((uint32)SignalExtPtr->ComUpdateBitPositionInBit) / 8U;

#if ((COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_OFF) && (COM_HANDLESMALLERRXPDUS_NOTIFY == STD_ON))
    /* the Pdu is maybe shorter than expected, but the update bit was not set during signal copying,
     * Therefore we have to check if update bit is received
     */
    uint8 const updateBitReceived =  (pduLength > UpdateBitByte);
    if (updateBitReceived == TRUE)
#else
      TS_PARAM_UNUSED(pduLength);
#endif
    {
      /* if update bit is not received, handle it as set */
      /* signal has an update bit configured ->
       * Pdu based RxDM shall not be taken into account */
      *passFilter_MASKED_NEW_DIFFERS_MASKED_OLD = 0U;

      TS_GetBit(&Com_gDataMemPtr[IPduPtr->ComIPduValueRef + UpdateBitByte],
                (SignalExtPtr->ComUpdateBitPositionInBit % 8U),
                uint8,
                SignalUpdated);
    }

#if (COM_RX_SIGNAL_BASED_DM == STD_ON)
    if (SignalUpdated == 1U)
    {
      /* check if signal has a signal based DM configured */
      if (COM_GET_RX_SIGNAL_EXT_SIGDM(SignalExtPtr) == COM_RX_SIG_DM_ON)
      {
        uint8 restart = 0U;

        /* calculate the position of the flags */
        CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) RxDMFlagPtr =
            COM_RX_SIGNAL_DM_FLAGS_BYTE_POS(SignalExtPtr);

        /* calculate position of the running flag */
        uint8 const RunningFlag = (uint8)COM_RX_SIGNAL_DM_RUNNING_BIT_POS(SignalExtPtr);

        /* calculate position of the timeout flag */
        uint8 const ToutFlag = (uint8)COM_RX_SIGNAL_DM_TIMEOUT_BIT_POS(SignalExtPtr);

        Com_RxDMNextDueTimeType const TimeOutOffset =
            COM_GET_RX_SIGNAL_TIMEOUT_FACTOR(SignalExtPtr);

        /* enter critical section */
        Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();
        {
          /* handle RxDM if enabled */
          if ( Com_IsBitSet_8(RxDMFlagPtr, RunningFlag) )
          {
#if (COM_FILTER_RECEIVER_ENABLE == STD_ON)
            TS_GetBit(RxDMFlagPtr, ToutFlag, uint8, restart);
#endif /* (COM_FILTER_RECEIVER_ENABLE == STD_ON) */

            /* clear time out flag, new value is available */
            Com_ClearBitUInt8(RxDMFlagPtr, ToutFlag);

            /* signal based DM is enabled therefore rewind */
            Com_EB_RxDMSignalQueue(SignalExtPtr, TimeOutOffset
#if (COM_TIMEBASE == STD_OFF)
                                   , IPduPtr->ComMainFunctionsRx_Idx
#endif /* (COM_TIMEBASE == STD_OFF) */
            );
          }
#if (COM_RX_FIRST_TIMEOUT_FACTOR_SIZE != COM_SIZE_0_BIT)
          else
          {
            ComIPduRefType const ComRxPduId = SignalPtr->ComIPduRef;

            /* pointer to byte with status flags of the Pdu */
            CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) IPduFlagBytePtr =
                &Com_gDataMemPtr[(Com_gConfigPtr->ComRxIPduStatusBase + ComRxPduId)];

            /* RxDM is not running, check if shall be enabled */
            /* check if RxDM is switched on */
            if ( Com_IsBitSet_8(IPduFlagBytePtr, COM_RXPDU_RXDMSWITCHON) )
            {
#if (COM_FILTER_RECEIVER_ENABLE == STD_ON)
              TS_GetBit(RxDMFlagPtr, ToutFlag, uint8, restart);
#endif /* (COM_FILTER_RECEIVER_ENABLE == STD_ON) */

              /* clear time out flag, new value is available */
              Com_ClearBitUInt8(RxDMFlagPtr, ToutFlag);

              /* enable RxDM */
              Com_SetBitUInt8(RxDMFlagPtr, RunningFlag);

              /* signal based DM is enabled therefore start */
              Com_EB_RxDMSignalQueue(SignalExtPtr, TimeOutOffset
#if (COM_TIMEBASE == STD_OFF)
                                     , IPduPtr->ComMainFunctionsRx_Idx
#endif /* (COM_TIMEBASE == STD_OFF) */
              );
            }
          }
#endif /* (COM_RX_FIRST_TIMEOUT_FACTOR_SIZE != COM_SIZE_0_BIT) */

        }
        /* leave critical section */
        Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

#if (COM_FILTER_RECEIVER_ENABLE == STD_ON)
        if (restart == 1U)
        {
          *passFilter_MASKED_NEW_DIFFERS_MASKED_OLD = 1U;
        }
#endif /* (COM_FILTER_RECEIVER_ENABLE == STD_ON) */

      }                   /* if (COM_GET_RX_SIGNAL_EXT_SIGDM(SignalExtPtr) == COM_RX_SIG_DM_ON) */

    }                      /* if (SignalUpdated == 1U) */
#endif /* (COM_RX_SIGNAL_BASED_DM == STD_ON) */

  }                         /* update bit is available, check if bit is 1 */
  return SignalUpdated;
}                               /* Com_EB_CheckRxSignalUpdateBitAndDM */


#if (COM_RX_SIGNAL_BASED_DM == STD_ON)

/** \brief Com_EB_RxDMSignalGroupQueue - "rewind" the RxDM timeout for a
 * signal group
 * This function starts a timeout for the Rx deadline monitoring of a
 * signal group
 * Preconditions:
 * - must only be invoked within a critical section
 * - prior to this call, the COM must be initialized.
 * \param[in] SignalGPtr pointer to configuration of the Rx signal group to
 * start deadline monitoring
 * \param[in] TimeOutOffset - value which shall be used for "rewind"
 */
TS_MOD_PRIV_DEFN FUNC(void, COM_CODE) Com_EB_RxDMSignalGroupQueue
(
   P2CONST(ComRxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr,
   Com_RxDMNextDueTimeType TimeOutOffset
#if (COM_TIMEBASE == STD_OFF)
   , Com_MainFunctionRx_IdxType ComMainFunctionsRx_Idx
#endif /* (COM_TIMEBASE == STD_OFF) */
)
{
   Com_RxDMNextDueTimeType globalTime;
   Com_RxDMNextDueTimeType TimeOut;

   /* Deviation MISRAC2012-1 */
   CONSTP2VAR(Com_RxDMNextDueTimeType, AUTOMATIC, COM_VAR_CLEARED) RxTimeoutSignalGroupQueueBase =
       COM_GET_MEMPTR(Com_RxDMNextDueTimeType, Com_gConfigPtr->ComRxTimeoutSignalGroupQueueBase);

   CONSTP2CONST(ComRxSignalExtType, AUTOMATIC, COM_APPL_CONST) SignalExtPtr =
            COM_GET_CONFIG_ELEMENT(ComRxSignalExtType,
                     Com_gConfigPtr->ComRxSignalExtRef, SignalGPtr->ComSignalExtRef);

   /* get global time */
    globalTime = (Com_RxDMNextDueTimeType) COM_GET_Com_GlobalRxTime(ComMainFunctionsRx_Idx);

   /* see other queue-functions above */
   TimeOut = (Com_RxDMNextDueTimeType) (globalTime + TimeOutOffset);
   RxTimeoutSignalGroupQueueBase[SignalExtPtr->RxSigDMQueueRef] = TimeOut;

}                               /* Com_EB_RxDMSignalGroupQueue */


/** \brief Com_EB_RxDMSignalQueue - "rewind" the RxDM timeout for a signal
 * This function starts a timeout for the Rx deadline monitoring of a signal
 * Preconditions:
 * - must only be invoked within a critical section
 * - prior to this call, the COM must be initialized.
 * \param[in] SignalExtPtr pointer to extended configuration of the Rx signal to start
 * deadline monitoring
 * \param[in] TimeOutOffset - value which shall be used for "rewind"
 */
TS_MOD_PRIV_DEFN FUNC(void, COM_CODE) Com_EB_RxDMSignalQueue
(
   P2CONST(ComRxSignalExtType, AUTOMATIC, COM_APPL_CONST) SignalExtPtr,
   Com_RxDMNextDueTimeType TimeOutOffset
#if (COM_TIMEBASE == STD_OFF)
   , Com_MainFunctionRx_IdxType ComMainFunctionsRx_Idx
#endif /* (COM_TIMEBASE == STD_OFF) */
)
{
   Com_RxDMNextDueTimeType globalTime;
   Com_RxDMNextDueTimeType TimeOut;

   /* Deviation MISRAC2012-1 */
   CONSTP2VAR(Com_RxDMNextDueTimeType, AUTOMATIC, COM_VAR_CLEARED) RxTimeoutSignalQueueBase =
       COM_GET_MEMPTR(Com_RxDMNextDueTimeType, Com_gConfigPtr->ComRxTimeoutSignalQueueBase);

   /* get global time */
    globalTime = (Com_RxDMNextDueTimeType) COM_GET_Com_GlobalRxTime(ComMainFunctionsRx_Idx);

   /* see other queue-functions above */
   TimeOut = (Com_RxDMNextDueTimeType) (globalTime + TimeOutOffset);
   RxTimeoutSignalQueueBase[SignalExtPtr->RxSigDMQueueRef] = TimeOut;

}                               /* Com_EB_RxDMSignalQueue */


/** \brief Com_EB_EnableSignalBasedDMFlag - sets up signal based DM
 * Preconditions:
 * - prior to this call, the COM module shall be initialized
 * \param[in] IPduPtr - configuration of the I-Pdu
 */
TS_MOD_PRIV_DEFN FUNC(void, COM_CODE) Com_EB_EnableSignalBasedDMFlag
(
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
)
{
   ComSignalRefType i; /* iteration variable for signal list*/

   /* set up signal based DM */
   for (i = IPduPtr->ComIPduSignalRefFirst;
   i < (IPduPtr->ComIPduSignalWithDMOnly + IPduPtr->ComIPduSignalRefFirst); i++)
   {
      /* Get signal id and pointer to config of the signal in the signal list of the IPdu */
      ComSignalRefType const signalId =
         (ComSignalRefType) *
         (COM_GET_CONFIG_ELEMENT
          (ComSignalRefType, Com_gConfigPtr->ComRxIPduSignalListRef, i));

      CONSTP2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) SignalPtr =
         COM_GET_CONFIG_ELEMENT(ComRxSignalType, Com_gConfigPtr->ComRxSignalRef, signalId);

      CONSTP2CONST(ComRxSignalExtType, AUTOMATIC, COM_APPL_CONST) SignalExtPtr =
               COM_GET_CONFIG_ELEMENT(ComRxSignalExtType,
                        Com_gConfigPtr->ComRxSignalExtRef, SignalPtr->ComSignalExtRef);

      /* Signal based RxDM shall be started if
       * 1) Signal based RxDM is configured
       * 2) FirstTimeout != 0 and
       * 3) RxDM not started yet */

      if (COM_GET_RX_SIGNAL_EXT_SIGDM(SignalExtPtr) == COM_RX_SIG_DM_ON)
      {

         Com_RxDMNextDueTimeType const TimeOutOffset =
#if (COM_RX_FIRST_TIMEOUT_FACTOR_SIZE == COM_SIZE_0_BIT)
                  (Com_RxDMNextDueTimeType)(SignalExtPtr->ComRxTimeoutFactorValue);
#else
                  (Com_RxDMNextDueTimeType)(SignalExtPtr->ComRxFirstTimeoutFactorValue);

         /* if ComRxFirstTimeoutFactor is zero start RxDM with first reception */
         if (0U != TimeOutOffset)
#endif /* (COM_RX_FIRST_TIMEOUT_FACTOR_SIZE == COM_SIZE_0_BIT) */
         {

            CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) SigDMFlagsPtr =
                  COM_RX_SIGNAL_DM_FLAGS_BYTE_POS(SignalExtPtr);

            uint8 enableBitPos = (uint8)COM_RX_SIGNAL_DM_RUNNING_BIT_POS(SignalExtPtr);

            /* enter the critical section */
            Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

            /* do not re-start if already enabled  */
            if (!(Com_IsBitSet_8(SigDMFlagsPtr, enableBitPos)))
            {
               /* set enable bit */
               Com_SetBitUInt8(SigDMFlagsPtr, enableBitPos);

               /* start DM timer */
               Com_EB_RxDMSignalQueue(SignalExtPtr, TimeOutOffset
#if (COM_TIMEBASE == STD_OFF)
                      , IPduPtr->ComMainFunctionsRx_Idx
#endif /* (COM_TIMEBASE == STD_OFF) */
                    );
            }                   /* if (SigRxDMEnabled == FALSE) */

            /* exit the critical section */
            Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

         }                      /* if (0U != TimeOutOffset) */

      }                         /* if (COM_GET_RX_SIGNAL_EXT_SIGDM(SignalExtPtr) == COM_RX_SIG_DM_ON) */

   }                            /* for (i = IPduPtr->ComIPduSignalRefFirst;
   i < (IPduPtr->ComIPduSignalWithDMOnly + IPduPtr->ComIPduSignalRefFirst); i++) */

   return;
}                               /* Com_EB_EnableSignalBasedDMFlag */



/** \brief Com_EB_EnableSignalGroupBasedDMFlag - sets up signal group based DM
 * Preconditions:
 * - prior to this call, the COM module shall be initialized
 * \param[in] IPduPtr - configuration of the I-Pdu
 */
TS_MOD_PRIV_DEFN FUNC(void, COM_CODE) Com_EB_EnableSignalGroupBasedDMFlag
(
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
)
{
   ComSignalGroupRefType j; /* iteration variable for signal group list*/
   for (j = IPduPtr->ComIPduSignalGroupRefFirst;
   j < (IPduPtr->ComIPduSignalGroupRefNum + IPduPtr->ComIPduSignalGroupRefFirst); j++)
   {
      /* Get signal id and pointer to config of the signal in the signal list of the IPdu */
      ComSignalGroupRefType const signalGId =
         (ComSignalGroupRefType) *
         (COM_GET_CONFIG_ELEMENT
          (ComSignalGroupRefType, Com_gConfigPtr->ComRxIPduSignalGroupListRef, j));

      CONSTP2CONST(ComRxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr =
         COM_GET_CONFIG_ELEMENT(ComRxSignalGroupType,
               Com_gConfigPtr->ComRxSignalGroupRef, signalGId);

      CONSTP2CONST(ComRxSignalExtType, AUTOMATIC, COM_APPL_CONST) SignalExtPtr =
               COM_GET_CONFIG_ELEMENT(ComRxSignalExtType,
                        Com_gConfigPtr->ComRxSignalExtRef, SignalGPtr->ComSignalExtRef);

      /* SignalGroup based RxDM shall be started if
       * 1) SignalGroup based RxDM is configured and
       * 2) FirstTimeout != 0 and
       * 3) not started yet */

      if (COM_GET_RX_SIGNAL_GROUP_SIGDM(SignalExtPtr) == COM_RX_SIG_DM_ON)
      {

         Com_RxDMNextDueTimeType const TimeOutOffset =
#if (COM_RX_FIRST_TIMEOUT_FACTOR_SIZE == COM_SIZE_0_BIT)
               (ComRxTimeoutFactorType)(SignalGPtr->ComRxTimeoutFactorValue);
#else
               (ComRxFirstTimeoutFactorType)(SignalExtPtr->ComRxFirstTimeoutFactorValue);

         /* if ComRxFirstTimeoutFactor is zero start RxDM with first reception */
         if (0U != TimeOutOffset)
#endif /* (COM_RX_FIRST_TIMEOUT_FACTOR_SIZE == COM_SIZE_0_BIT) */
         {

            CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) SigGDMFlagsPtr =
                  COM_RX_SIGNAL_GROUP_DM_FLAGS_BYTE_POS(SignalExtPtr);

            uint8 enableBitPos = (uint8)COM_RX_SIGNAL_GROUP_DM_RUNNING_BIT_POS(SignalExtPtr);

            /* enter the critical section */
            Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

            /* do not re-start if already enabled  */
            if (!(Com_IsBitSet_8(SigGDMFlagsPtr, enableBitPos)))
            {

               /* set enable bit */
               Com_SetBitUInt8(SigGDMFlagsPtr, enableBitPos);

               /* start DM timer */
               Com_EB_RxDMSignalGroupQueue(SignalGPtr, TimeOutOffset
#if (COM_TIMEBASE == STD_OFF)
                      , IPduPtr->ComMainFunctionsRx_Idx
#endif /* (COM_TIMEBASE == STD_OFF) */
                    );
            }                   /* if (SigGRxDMEnabled == FALSE) */

            /* exit the critical section */
            Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

         }                      /* if (0U != TimeOutOffset) */

      }                         /*       if (COM_GET_RX_SIGNAL_GROUP_SIGDM(SignalExtPtr) == COM_RX_SIG_DM_ON) */

   }                            /* for (j = IPduPtr->ComIPduSignalGroupRefFirst;
   j < (IPduPtr->ComIPduSignalGroupRefNum + IPduPtr->ComIPduSignalGroupRefFirst); j++) */

   return;
}                               /* Com_EB_EnableSignalGroupBasedDMFlag */


/** \brief Com_EB_DisableSignalAndSignalGroupBasedDMFlag - disables
 * signal(group) based DM
 * Preconditions:
 * - prior to this call, the COM module shall be initialized
 * \param[in] IPduPtr - configuration of the I-Pdu
 * \param[in] Initialize - defines if timeout flag shall be cleared
 */
TS_MOD_PRIV_DEFN FUNC(void, COM_CODE) Com_EB_DisableSignalAndSignalGroupBasedDMFlag
(
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   boolean Initialize
)
{
  Com_EB_DisableSignalBasedDMFlag(IPduPtr, Initialize);
  Com_EB_DisableSignalGroupBasedDMFlag(IPduPtr, Initialize);
}                               /* Com_EB_DisableSignalAndSignalGroupBasedDMFlag */

/*==================[internal function definitions]==========================*/

_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_DisableSignalBasedDMFlag
(
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   boolean Initialize
)
{
  ComSignalRefType i; /* iteration variable for signal list*/

  /* set up signal based DM */
  for (i = IPduPtr->ComIPduSignalRefFirst;
  i < (IPduPtr->ComIPduSignalWithDMOnly + IPduPtr->ComIPduSignalRefFirst); i++)
  {
     /* Get signal id and pointer to config of the signal in the signal list of the IPdu */
     ComSignalRefType const signalId =
        (ComSignalRefType) *
        (COM_GET_CONFIG_ELEMENT
         (ComSignalRefType, Com_gConfigPtr->ComRxIPduSignalListRef, i));

     CONSTP2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) SignalPtr =
        COM_GET_CONFIG_ELEMENT(ComRxSignalType, Com_gConfigPtr->ComRxSignalRef, signalId);

     CONSTP2CONST(ComRxSignalExtType, AUTOMATIC, COM_APPL_CONST) SignalExtPtr =
              COM_GET_CONFIG_ELEMENT(ComRxSignalExtType,
                       Com_gConfigPtr->ComRxSignalExtRef, SignalPtr->ComSignalExtRef);

     /* check if DM configured */
     if (COM_GET_RX_SIGNAL_EXT_SIGDM(SignalExtPtr) == COM_RX_SIG_DM_ON)
     {

        /* Clear running bit */
        COM_AtomicClearBit_8(COM_RX_SIGNAL_DM_FLAGS_BYTE_POS(SignalExtPtr),
              COM_RX_SIGNAL_DM_RUNNING_BIT_POS(SignalExtPtr));

        /* clear timeout flag only when the I-Pdu is initialized */
        if (TRUE == Initialize)
        {
           /* Clear timed-out flag for of signal group DM */
           COM_AtomicClearBit_8(COM_RX_SIGNAL_DM_FLAGS_BYTE_POS(SignalExtPtr),
                 COM_RX_SIGNAL_DM_TIMEOUT_BIT_POS(SignalExtPtr));
        }

     }
  }                            /* for (i = IPduPtr->ComIPduSignalRefFirst;
  i < (IPduPtr->ComIPduSignalWithDMOnly + IPduPtr->ComIPduSignalRefFirst); i++) */
}

_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_DisableSignalGroupBasedDMFlag
(
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   boolean Initialize
)
{
  ComSignalGroupRefType i; /* iteration variable for signal group list*/

  for (i = IPduPtr->ComIPduSignalGroupRefFirst;
  i < (IPduPtr->ComIPduSignalGroupRefNum + IPduPtr->ComIPduSignalGroupRefFirst); i++)
  {
     /* Get signal id and pointer to config of the signal in the signal list of the IPdu */
     ComSignalGroupRefType const signalGId =
        (ComSignalGroupRefType) *
        (COM_GET_CONFIG_ELEMENT
         (ComSignalGroupRefType, Com_gConfigPtr->ComRxIPduSignalGroupListRef, i));

     CONSTP2CONST(ComRxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr =
        COM_GET_CONFIG_ELEMENT(ComRxSignalGroupType,
              Com_gConfigPtr->ComRxSignalGroupRef, signalGId);

     CONSTP2CONST(ComRxSignalExtType, AUTOMATIC, COM_APPL_CONST) SignalExtPtr =
              COM_GET_CONFIG_ELEMENT(ComRxSignalExtType,
                       Com_gConfigPtr->ComRxSignalExtRef, SignalGPtr->ComSignalExtRef);

     /* check if DM configured */
     if (COM_GET_RX_SIGNAL_GROUP_SIGDM(SignalExtPtr) == COM_RX_SIG_DM_ON)
     {

       /* Clear running bit */
        COM_AtomicClearBit_8(COM_RX_SIGNAL_GROUP_DM_FLAGS_BYTE_POS(SignalExtPtr),
              COM_RX_SIGNAL_GROUP_DM_RUNNING_BIT_POS(SignalExtPtr));

        /* clear timeout flag only when the I-Pdu is initialized */
        if (TRUE == Initialize)
        {
           /* Clear timed-out flag for of signal group DM */
           COM_AtomicClearBit_8(COM_RX_SIGNAL_GROUP_DM_FLAGS_BYTE_POS(SignalExtPtr),
                 COM_RX_SIGNAL_GROUP_DM_TIMEOUT_BIT_POS(SignalExtPtr));
        }
     }
  }                            /* for (i = IPduPtr->ComIPduSignalGroupRefFirst;
  i < (IPduPtr->ComIPduSignalGroupRefNum + IPduPtr->ComIPduSignalGroupRefFirst); i++) */
}
#endif /* (COM_RX_SIGNAL_BASED_DM == STD_ON) */

#if (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON)
/** \brief Com_EB_HandleShortPdu_HandleUB - copies an update bit from external buffer to the Rx I-PDU buffer
 * \param[in] IPduPtr - configuration of the I-Pdu
 * \param[in] SduDataPtr - pointer to external buffer
 * \param[in] SduLength - length of external buffer
 * \param[in] ComUpdateBitPositionInBit - position of the update bit
 * \param[in] IsReceived - shall be TRUE if the signal / signal group has be fully received, else FALSE
 */
TS_MOD_PRIV_DEFN FUNC(void, COM_CODE) Com_EB_HandleShortPdu_HandleUB
(
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) SduDataPtr,
    PduLengthType SduLength,
    ComSignalDescBitType ComUpdateBitPositionInBit,
    uint8 IsReceived
)
{
  uint8 SignalUpdated;

  ComSignalDescBitType const UpdateBitByte = (ComSignalDescBitType)((uint32)ComUpdateBitPositionInBit / 8U);
  uint8 const UpdateBitBit = (uint8)((uint32)ComUpdateBitPositionInBit % 8U);

  if (UpdateBitByte < SduLength)
  {
    /* update bit is also received */
    TS_GetBit(&SduDataPtr[UpdateBitByte],
              UpdateBitBit,
              uint8,
              SignalUpdated);

  }
  else
  {
      /* SWS_Com_00794 */
      SignalUpdated = 1U;
  }

  if ((1U == SignalUpdated) && (0U != IsReceived))
  {
      Com_SetBitUInt8( &Com_gDataMemPtr[(IPduPtr->ComIPduValueRef + UpdateBitByte)],
                       UpdateBitBit);
  }
  else
  {
      Com_ClearBitUInt8( &Com_gDataMemPtr[(IPduPtr->ComIPduValueRef + UpdateBitByte)],
                      UpdateBitBit);
  }
}                      /* Com_EB_HandleShortPdu_HandleUB */
#endif /* (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON) */

#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>
#endif

#else /* (COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) */

/* Avoid empty translation unit according to ISO C90 */
TS_PREVENTEMPTYTRANSLATIONUNIT

#endif /* (COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) */

/*==================[end of file]============================================*/

