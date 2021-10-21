/**
 * \file
 *
 * \brief AUTOSAR Dcm
 *
 * This file contains the implementation of the AUTOSAR
 * module Dcm.
 *
 * \version 5.0.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*This file contains the implementation of the MidServices */
/* tasking Deviation List
 *
 * TASKING-1) Deviated Rule: W549
 * W549: condition is always true violated
 *
 * Reason:
 * This warnings is a false positive.
 * The compiler optimizes the 'for' loop (which has only 1 iteration) in the following way
 * - it prepares in advance the index for the next iteration
 * - if the index is equal to '0' it goes back to check the 'for' loop conditions, otherwise it just moves on
 * - since the index is already equal to '1', it never jumps back to check the exit condition
 */
/*===============================[includes]======================================================*/

#include <Dcm_Trace.h>
#include <Std_Types.h>                                                 /* AUTOSAR standard types */
#include <TSAutosar.h>

#include <Dcm_Dsp_MidServices.h>

/* The functions provided and implemented by this unit are exclusively used for
 * OBD service 0x06 handling. */
#if(DCM_DSP_USE_SERVICE_0X06 == STD_ON)

#include <Dcm_Dsp_DemFacade.h>                                  /* Declaration of DemFacade APIs */

#include <TSMem.h>                                               /* EB specific memory functions */

/*===========================[macro definitions]=================================================*/

#if (defined DCM_OBDMID_NONAVAILABILITY_RES_SIZE)
  #error "DCM_OBDMID_NONAVAILABILITY_RES_SIZE is already defined"
#endif
#define DCM_OBDMID_NONAVAILABILITY_RES_SIZE  8U

#if (defined DCM_OBD_AVAILABILITY_RES_SIZE)
  #error "DCM_OBD_AVAILABILITY_RES_SIZE is already defined"
#endif
#define DCM_OBD_AVAILABILITY_RES_SIZE        4U

#if(DCM_OBDMID_SUPPORT_SWC == STD_ON)

#if (defined DCM_GET_DTR_DATA)
  #error "DCM_GET_DTR_DATA is already defined"
#endif
#define DCM_GET_DTR_DATA Dcm_Dsp_MidServices_SwcGetDtrData

#if (defined DCM_GET_NO_OF_OBDMIDTIDS)
  #error "DCM_GET_NO_OF_OBDMIDTIDS is already defined"
#endif
#define DCM_GET_NO_OF_OBDMIDTIDS      Dcm_Dsp_MidServices_SwcGetNumTIDsOfOBDMID

#if (defined DCM_GET_AVAILABLE_OBDMIDS)
  #error "DCM_GET_AVAILABLE_OBDMIDS is already defined"
#endif
#define DCM_GET_AVAILABLE_OBDMIDS     Dcm_Dsp_MidServices_SwcGetAvailableOBDMIDs

#else /* #if(DCM_OBDMID_SUPPORT_SWC == STD_ON) */

#if (defined DCM_GET_DTR_DATA)
  #error "DCM_GET_DTR_DATA is already defined"
#endif
#define DCM_GET_DTR_DATA Dcm_Dsp_DemFacade_GetDTRData

#if (defined DCM_GET_NO_OF_OBDMIDTIDS)
  #error "DCM_GET_NO_OF_OBDMIDTIDS is already defined"
#endif
#define DCM_GET_NO_OF_OBDMIDTIDS      Dcm_Dsp_DemFacade_GetNumTIDsOfOBDMID

#if (defined DCM_GET_AVAILABLE_OBDMIDS)
  #error "DCM_GET_AVAILABLE_OBDMIDS is already defined"
#endif
#define DCM_GET_AVAILABLE_OBDMIDS     Dcm_Dsp_MidServices_DemGetAvailableOBDMIDs

#endif /* #if(DCM_OBDMID_SUPPORT_SWC) */

/*=====================[external constants declarations]=========================================*/

/*=====================[internal constants declarations]=========================================*/

/*=====================[internal data definitions]===============================================*/

#define DCM_START_SEC_VAR_INIT_8
#include <Dcm_MemMap.h>

#if (DCM_DSP_MIDSERVICES_USE_LOCKING == STD_ON)
/** \brief Mutex controlling access to the MID services */
STATIC VAR(Dcm_MutexType, DCM_VAR ) Dcm_Dsp_MidServices_Lock = DCM_MUTEX_UNLOCKED;
#endif

#define DCM_STOP_SEC_VAR_INIT_8
#include <Dcm_MemMap.h>

/*==================[internal function declarations]=============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief Gets the internal Id for the requested OBDMID
 **
 ** \param[in]    ObdMid         Non Availability OBDMID external Id.
 ** \param[out]   pObdMidIntId   The internal Id for the requested Non Availability OBDMID.
 **
 ** \return       Operation status
 ** \retval       E_OK     Report of Internal Id extraction result successful
 ** \retval       E_NOT_OK Report of Internal Id extraction result failed
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_MidServices_GetObdMidInternalId
(
  uint8 ObdMid,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) pObdMidIntId
);

#if(DCM_OBDMID_SUPPORT_SWC== STD_ON)
/** \brief Reports the value of a requested "availability-OBDMID" to
 **        the DCM upon a Service $06 request. Derived from that the
 **        tester displays the supported tests a mechanic can select
 **        from.
 **
 ** \param[in]    ObdMid Availability OBDMID ($00,$20, $40. . . ).
 ** \param[out]   pObdMidValue Bit coded information on the support of OBDMIDs.
 **
 ** \return       Operation status
 ** \retval       E_OK     Report of DTR result successful
 ** \retval       E_NOT_OK Report of DTR result failed
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_MidServices_SwcGetAvailableOBDMIDs
(
  uint8 ObdMid,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) pObdMidValue
);

/** \brief Gets the number of TIDs per (functional) OBDMID. This can
 **        be used by the DCM to iteratively request for OBD/TID result
 **        data within a loop from 0. . . .numberOfTIDs-1
 **
 ** \param[in]    ObdMidIntId Non Availability OBDMID internal Id.
 ** \param[out]   pNoOfTids   Number of available Tids for the required ObdMid.
 **
 ** \return       Operation status
 ** \retval       E_OK     Report of DTR result successful
 ** \retval       E_NOT_OK Report of DTR result failed
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_MidServices_SwcGetNumTIDsOfOBDMID
(
  uint8 ObdMidIntId,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) pNoOfTids
);

/** \brief Gets the internal Id for the requested OBDMID
 **
 ** \param[in]    ObdMidIntId    Non Availability OBDMID internal Id.
 ** \param[in]    TidIndex       Index of the TID within the DCM. Runs from 0
 **                              to "numberOfTIDs" obtained in the call to
 **                              Dcm_Dsp_MidServices_SwcGetNumTIDsOfOBDMID()
 ** \param[out]   pTidValue      TID to be put on the tester response.
 ** \param[out]   pUaSID         UaSID to be put on the tester response.
 ** \param[out]   pTestVal       Latest test result.
 ** \param[out]   pMinLimit      Lower limit value associated to the latest test result.
 ** \param[out]   pMaxLimit      Upper limit value associated to the latest test result.
 **
 ** \return       Operation status
 ** \retval       E_OK     Report DTR data extraction result successful
 ** \retval       E_NOT_OK Report DTR data extraction result failed
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_MidServices_SwcGetDtrData
(
  uint8 ObdMidIntId,
  uint8 TidIndex,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) pTidValue,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) pUaSID,
  P2VAR(uint16, AUTOMATIC, DCM_VAR) pTestVal,
  P2VAR(uint16, AUTOMATIC, DCM_VAR) pMinLimit,
  P2VAR(uint16, AUTOMATIC, DCM_VAR) pMaxLimit
);
#else
/** \brief Wrapper function calling the Dem
 *         to get the value of a requested "availability-OBDMID".
 **
 ** \param[in]    ObdMid Availability OBDMID ($00,$20, $40. . . ).
 ** \param[out]   pObdMidValue Bit coded information on the support of OBDMIDs.
 **
 ** \return       Operation status
 ** \retval       E_OK     Report of DTR result successful
 ** \retval       E_NOT_OK Report of DTR result failed
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_MidServices_DemGetAvailableOBDMIDs
(
  uint8 ObdMid,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) pObdMidValue
);
#endif /* #if(DCM_OBDMID_SUPPORT_SWC== STD_ON) */

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[internal function definitions]=============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* !LINKSTO Dcm.dsn.OBDMIDServices.GetOBdMidInternalId,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_MidServices_GetObdMidInternalId
(
  uint8 ObdMid,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) pObdMidIntId
)
{
  Std_ReturnType Result = E_NOT_OK;

#if (DCM_OBDMID_SUPPORT_SWC == STD_ON)
  uint8_least IteratorOdbMid;

  /* Deviation TASKING-1 */
  for(IteratorOdbMid = 0U; IteratorOdbMid < DCM_DSP_NR_OBDMID; IteratorOdbMid++)
  {
    if(Dcm_DspOBDMIDConfig[IteratorOdbMid].OBDMIDIdentifier == ObdMid)
    {
      if(DCM_IS_BYTE_FLAG_SET(IteratorOdbMid, Dcm_DspOBDMIDUsed) == TRUE)
      {
        /* the ObdMid is configured and available, so return the internal Id */
        *pObdMidIntId = (uint8)IteratorOdbMid;
        Result = E_OK;
      }
      /* we found the ObdMid id into the internal configuration
       * and we don't have to go further */
      break;
    }
  }
#else
  /* we return the requested id, because Dem will handle the functionality
   * this is merely an adaptation to make the code behaving similar for both versions */
  *pObdMidIntId = ObdMid;
  Result = E_OK;
#endif

  return Result;
}

#if (DCM_OBDMID_SUPPORT_SWC == STD_ON)
/* !LINKSTO Dcm.dsn.OBDMIDServices.SwcGetAvailableOBDMIDs,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_MidServices_SwcGetAvailableOBDMIDs
(
  uint8 ObdMid,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) pObdMidValue
)
{
  Std_ReturnType Result = E_NOT_OK;
  uint8_least IteratorOdbMid;
  uint8 NoOfTids;

  /* Iterate over all configured MIDs */
  /* !LINKSTO Dcm.ISO-15031-5_8.6.2_1,1 */
  /* Deviation TASKING-1 */
  for(IteratorOdbMid = 0U; IteratorOdbMid < DCM_DSP_NR_OBDMID; IteratorOdbMid++)
  {
    /* Consider only the ones marked as 'used' by configuration or calibration */
    if(DCM_IS_BYTE_FLAG_SET(IteratorOdbMid, Dcm_DspOBDMIDUsed) == TRUE)
    {
      /* store the retrieved "non-availability OBDMID" */
      const uint8 NonAvailabilityObdMid =  Dcm_DspOBDMIDConfig[IteratorOdbMid].OBDMIDIdentifier;

      /* Consider only MIDs that are within or above the requested availability MID range
       * The MIDs above the current availability range are used to decie wheter to set the bit
       * indicating that the next availability range does contain items too */
      if (NonAvailabilityObdMid >= ObdMid)
      {
        if (Dcm_Dsp_MidServices_SwcGetNumTIDsOfOBDMID((uint8)IteratorOdbMid, &NoOfTids) == E_OK)
        {
          /* we have at least one "non-availability OBDMID" to report for the selected
            * "availability OBDMID" */
          /* !LINKSTO Dcm.ISO-15031-5_8.6.1_7,1 */
          Result = Dcm_OBDSupport_UpdateAvailableInfoWithID(NonAvailabilityObdMid,
                                                            ObdMid, pObdMidValue);
        }
      }
    }
  }

  return Result;
}

/* !LINKSTO Dcm.dsn.OBDMIDServices.SwcGetNumTIDsOfOBDMID,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_MidServices_SwcGetNumTIDsOfOBDMID
(
  uint8 ObdMidIntId,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) pNoOfTids
)
{
  /* initialize function operation with unsuccessful */
  Std_ReturnType Result = E_NOT_OK;
  uint8 IteratorObdMidTid;
  uint8 TIDValue, UaSID;
  uint16 TestVal, MinLimit, MaxLimit;
  const uint8 NumOfTids =  Dcm_DspOBDMIDConfig[ObdMidIntId].NumOfTid;

  /* check if at least one Tid from the ObdMIdTid list is visible */
  for(IteratorObdMidTid = 0U; IteratorObdMidTid < NumOfTids; IteratorObdMidTid++)
  {
     Result = Dcm_Dsp_MidServices_SwcGetDtrData(ObdMidIntId, IteratorObdMidTid, &TIDValue, &UaSID,
                                            &TestVal, &MinLimit, &MaxLimit);

    /* !LINKSTO Dcm.EB.OBD.DTRService.Invisible.AvailabilityOBDMID,1 */
    /* !LINKSTO Dcm.EB.OBD.DTRService.Invisible.NonAvailabilityOBDMID,1 */
    if(E_OK == Result)
    {
      /* the number of ObdMidTids can be retrieved, because there is at least one test available */
      *pNoOfTids = NumOfTids;

      /* we go no further with the search */
      break;
    }
  }

  return Result;
}

/* !LINKSTO Dcm.dsn.OBDMIDServices.SwcGetDtrData,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_MidServices_SwcGetDtrData
(
  uint8 ObdMidIntId,
  uint8 TidIndex,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) pTidValue,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) pUaSID,
  P2VAR(uint16, AUTOMATIC, DCM_VAR) pTestVal,
  P2VAR(uint16, AUTOMATIC, DCM_VAR) pMinLimit,
  P2VAR(uint16, AUTOMATIC, DCM_VAR) pMaxLimit
)
{
  Std_ReturnType Result = E_NOT_OK;
  Dcm_DTRStatusType DTRStatus = DCM_DTRSTATUS_INVISIBLE;

  /* check if the test is available */
  if(DCM_IS_BYTE_FLAG_SET(TidIndex, Dcm_DspOBDMIDConfig[ObdMidIntId].TidUsedPtr) == TRUE)
  {
    /* get the internal Id */
    const uint16 TidInternalId = Dcm_DspOBDMIDConfig[ObdMidIntId].TidStartIndex + TidIndex;
    /* retrieve data */
    /* get the test Id */
    *pTidValue = Dcm_DspOBDMIDTidConfig[TidInternalId].TidIdentifier;
    /* get the UaSid parameter value */
    *pUaSID = Dcm_DspOBDMIDTidConfig[TidInternalId].UaSid;
    /* get test values */
    Result = Dcm_DspOBDMIDTidConfig[TidInternalId].GetDTRValueFnc(DCM_INITIAL,
                                                                  pTestVal,
                                                                  pMinLimit,
                                                                  pMaxLimit,
                                                                  &DTRStatus);

    if (DCM_DTRSTATUS_INVISIBLE == DTRStatus)
    {
      /* Prevent sending invisible data */
      Result = E_NOT_OK;
    }
    else
    {
      if (E_OK != Result)
      {
        /* Invalid dataset. It still needs to be reported, but as all-zeros */
        /* !LINKSTO Dcm.ISO-15031-5_6.2.4.3.7_Table11_p,1, Dcm.ISO-15031-5_8.6.1_5,2 */
        /* !LINKSTO Dcm.ISO-15031-5_8.6.1_6,1, Dcm.ISO-15031-5_8.6.1_12,1 */
        *pTestVal = 0;
        *pMinLimit = 0;
        *pMaxLimit = 0;
        Result = E_OK;
      }
    }

  }
  else
  {
    /* Prevent sending erroneous data. This is for the case when
     * ObdMid is not used, or the ObdMidTid is not used */
    Result = E_NOT_OK;
  }

  return Result;
}

#else /* #if(DCM_OBDMID_SUPPORT_SWC == STD_ON) */
/* !LINKSTO Dcm.dsn.OBDMIDServices.DemGetAvailableOBDMIDs,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_MidServices_DemGetAvailableOBDMIDs
(
  uint8 ObdMid,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) pObdMidValue
)
{
  /* !LINKSTO Dcm.ISO-15031-5_8.6.2_1,1 */
  Std_ReturnType Result = E_NOT_OK;
  uint32 ObdMidValue;

  /* !LINKSTO SWS_Dcm_00957,1 */
  Result = Dcm_Dsp_DemFacade_GetAvailableOBDMIDs(ObdMid, &ObdMidValue);

  if (Result == E_OK)
  {
    /* Only report back availability IDs that have used MIDs in them */
    if (ObdMidValue != 0UL)
    {
      pObdMidValue[0U] = (uint8)(ObdMidValue >> 24U);
      pObdMidValue[1U] = (uint8)(ObdMidValue >> 16U);
      pObdMidValue[2U] = (uint8)(ObdMidValue >> 8U);
      pObdMidValue[3U] = (uint8)(ObdMidValue);
    }
    else
    {
      /* !LINKSTO Dcm.ISO-15031-5_8.6.2_2,1 */
      /* Ignore "empty" availability IDs */
      Result = E_NOT_OK;
    }
  }

  return Result;
}
#endif /* #if(DCM_OBDMID_SUPPORT_SWC == STD_ON) */

/*==================[external function definitions]=============================================*/

/* !LINKSTO Dcm.Dsn.IB.MidServices.Init,1 */
FUNC(void, DCM_CODE) Dcm_Dsp_MidServices_Init(void)
{
  DBG_DCM_DSP_MIDSERVICES_INIT_ENTRY();

#if (DCM_DSP_MIDSERVICES_USE_LOCKING == STD_ON)
  Dcm_Dsp_MidServices_Lock = DCM_MUTEX_UNLOCKED;
#endif

  DBG_DCM_DSP_MIDSERVICES_INIT_EXIT();
}

#if (DCM_DSP_MIDSERVICES_USE_LOCKING == STD_ON)
/* !LINKSTO Dcm.Dsn.IB.MidServices.LockServices,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_MidServices_LockServices(void)
{
  Std_ReturnType LockStatus = DCM_E_RESOURCE_LOCKED;

  DBG_DCM_DSP_MIDSERVICES_LOCKSERVICES_ENTRY();

  LockStatus = Dcm_Internal_GetLock(&Dcm_Dsp_MidServices_Lock);

  DBG_DCM_DSP_MIDSERVICES_LOCKSERVICES_EXIT(LockStatus);

  return LockStatus;
}

/* !LINKSTO Dcm.Dsn.IB.MidServices.UnlockServices,1 */
FUNC(void, DCM_CODE) Dcm_Dsp_MidServices_UnlockServices(void)
{
  DBG_DCM_DSP_MIDSERVICES_UNLOCKSERVICES_ENTRY();

  Dcm_Internal_ReleaseLock(&Dcm_Dsp_MidServices_Lock);

  DBG_DCM_DSP_MIDSERVICES_UNLOCKSERVICES_EXIT();
}
#endif /* #if (DCM_DSP_MIDSERVICES_USE_LOCKING == STD_ON) */

/* !LINKSTO Dcm.dsn.OBDMIDServices.GetNonAvailableOBDMIDs,1 */
/* This function returns one OBDMID including all TIDs */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_MidServices_GetNonAvailabilityMid(
  uint8 ObdMid,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) pResBuf,
  Dcm_MsgLenType MaxResBufLen,
  P2VAR(Dcm_MsgLenType, AUTOMATIC, DCM_VAR) pResLen,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) pNrc,
  boolean WriteMidBeforeTid)
{
  Std_ReturnType Result = E_NOT_OK;
  uint8 ObdMidIntId = 0U;

  DBG_DCM_DSP_DCM_DSP_MIDSERVICES_GETNONAVAILABILITYMID_ENTRY(ObdMid, pResBuf, MaxResBufLen
                                                              ,pResLen, pNrc, WriteMidBeforeTid);

  /* See if the MID is known and get its internal table index if so */
  if(Dcm_Dsp_MidServices_GetObdMidInternalId(ObdMid, &ObdMidIntId) == E_OK)
  {
    uint8 NrTidsOBDMID = 0U;

    /* Get the number of TIDs assigned to the requested MID */
    /* !LINKSTO Dcm.OBD_0x06.DcmGetNumTIDsOfOBDMID.ObtainAvailableTIDs,1 */
    /* !LINKSTO Dcm.ReadDataByIdentifier.OBD_DID_F6xx.DcmGetNumTIDsOfOBDMID.ObtainAvailableTIDs,1 */
    if (DCM_GET_NO_OF_OBDMIDTIDS(ObdMidIntId, &NrTidsOBDMID) == E_OK)
    {
      /* Make sure there are TIDs to report */
      if (NrTidsOBDMID > 0U)
      {
        Dcm_MsgLenType WritePosition = 0U;
        uint8 IteratorObdMidTid;

        /* Fill in OdbMidTid data */
        for(IteratorObdMidTid = 0U; IteratorObdMidTid < NrTidsOBDMID; IteratorObdMidTid++)
        {
          Std_ReturnType DTROpResult = E_NOT_OK;
          uint8 TIDValue = 0U;
          uint8 UaSID = 0U;
          uint16 TestVal = 0U;
          uint16 MinLimit = 0U;
          uint16 MaxLimit = 0U;

          /* !LINKSTO Dcm.OBD_0x06.DcmGetNumTIDsOfOBDMID.E_OK.TIDs.ObtainDTRData,1 */
          /* !LINKSTO Dcm.ReadDataByIdentifier.OBD_DID_F6xx.DcmGetNumTIDsOfOBDMID.E_OK.TIDs.ObtainDTRData,1 */
          DTROpResult = DCM_GET_DTR_DATA(ObdMidIntId, IteratorObdMidTid, &TIDValue, &UaSID,
                                         &TestVal, &MinLimit, &MaxLimit);

          /* !LINKSTO Dcm416,1 */
          /* !LINKSTO Dcm.EB.OBD_0x06.DcmGetDTRData.E_NOT_OK.NotAppendDTRData,1 */
          /* !LINKSTO Dcm.EB.ReadDataByIdentifier.OBD_DID_F6xx.DcmGetDTRData.E_NOT_OK.NotAppendDTRData,1 */
          if (E_OK == DTROpResult)
          {
            /* Is there enough space in the buffer left to insert the data? */
            if (MaxResBufLen < (WritePosition + DCM_OBDMID_NONAVAILABILITY_RES_SIZE))
            {
              /* Buffer is too small for the response data; Set the Negative Response Code. */
              /* !LINKSTO Dcm.EB.ReadDataByIdentifier.NRC0x14,1 */
              *pNrc = DCM_E_RESPONSETOOLONG;
              WritePosition = 0U;
              break;
            }

            /* write OBDMID before TIDs for service 0x06 or when it is enabled for service 0x22 */
            /* via DcmWriteMidBeforeTid configuration parameter */
            /* do not add OBDMID for the first written TID, because this is done by the calling function */

            if ((TRUE == WriteMidBeforeTid) && (0U != WritePosition))
            {
              /* write the MID */
              pResBuf[WritePosition + 0U] = ObdMid;
              WritePosition++;
            }

            /* Copy the dataset to the response buffer */
            /* !LINKSTO Dcm.ISO-15031-5_6.2.4.3.7_Table11_r,1 */
            /* !LINKSTO Dcm.ISO-15031-5_8.6.1_2,1, Dcm.ISO-15031-5_8.6.1_4,1 */
            /* !LINKSTO Dcm.EB.OBD_0x06.DcmGetDTRData.E_OK.AppendDTRData,1 */
            /* !LINKSTO Dcm.EB.ReadDataByIdentifier.OBD_DID_F6xx.DcmGetDTRData.E_OK.AppendDTRData,1 */
            pResBuf[WritePosition + 0U] = TIDValue;
            pResBuf[WritePosition + 1U] = UaSID;
            pResBuf[WritePosition + 2U] = DCM_HIBYTE(TestVal);
            pResBuf[WritePosition + 3U] = DCM_LOBYTE(TestVal);
            pResBuf[WritePosition + 4U] = DCM_HIBYTE(MinLimit);
            pResBuf[WritePosition + 5U] = DCM_LOBYTE(MinLimit);
            pResBuf[WritePosition + 6U] = DCM_HIBYTE(MaxLimit);
            pResBuf[WritePosition + 7U] = DCM_LOBYTE(MaxLimit);

            /* Advance the write index by the amount of data written */
            WritePosition += DCM_OBDMID_NONAVAILABILITY_RES_SIZE;
          }
        }

        if (0U != WritePosition)
        {
          /* There is at least one TID dataset available */
          *pResLen = WritePosition;
          Result = E_OK;
        }
        else
        {
          if (IteratorObdMidTid == NrTidsOBDMID)
          {
            /* There is no TID dataset available */
            /* !LINKSTO Dcm.EB.ReadDataByIdentifier.OBD_DID_F6xx.NoDTRDataObtained.NRC_0x31,1 */
            *pNrc = DCM_E_REQUESTOUTOFRANGE;
          }
        }

      }
      else
      {
        /* Requested MID has no TIDs to report */
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.OBD_DID_F6xx.DcmGetNumTIDsOfOBDMID.E_OK.NoTIDs.NRC_0x31,1 */
        *pNrc = DCM_E_REQUESTOUTOFRANGE;
      }
    }
    else
    {
      /* Requested MID has no TIDs configured */
      /* !LINKSTO Dcm.EB.ReadDataByIdentifier.OBD_DID_F6xx.DcmGetNumTIDsOfOBDMID.E_NOT_OK.NRC_0x31,1 */
      *pNrc = DCM_E_REQUESTOUTOFRANGE;
    }
  }
  else
  {
    /* Requested MID not found in configuration */
    *pNrc = DCM_E_REQUESTOUTOFRANGE;
  }

  DBG_DCM_DSP_DCM_DSP_MIDSERVICES_GETNONAVAILABILITYMID_EXIT(Result, ObdMid, pResBuf, MaxResBufLen
                                                             ,pResLen, pNrc, WriteMidBeforeTid);
  return Result;
}

/* !LINKSTO Dcm.dsn.OBDMIDServices.GetAvailableOBDMIDs,1 */
/* !id src.Dcm_OBDMIDGetAvailableOBDMID.001 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_MidServices_GetAvailabilityMid(
  uint8 ObdMid,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) pResBuf,
  Dcm_MsgLenType MaxResBufLen,
  P2VAR(Dcm_MsgLenType, AUTOMATIC, DCM_VAR) pResLen,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) pNrc)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_DCM_DSP_MIDSERVICES_GETAVAILABILITYMID_ENTRY(ObdMid, pResBuf, MaxResBufLen, pResLen, pNrc);

  if (MaxResBufLen >= DCM_OBD_AVAILABILITY_RES_SIZE)
  {
    /* Clean the buffer where the availability data will be set. Skip the
     * availability ID */
    TS_MemSet(&(pResBuf[0U]), 0U, DCM_OBD_AVAILABILITY_RES_SIZE);

    /* !LINKSTO Dcm415,1 */
    /* !LINKSTO Dcm.EB.ReadDataById.DefaultInterface.Enabled.UnsupportedDid.OBDMID,1 */
    Result = DCM_GET_AVAILABLE_OBDMIDS(ObdMid, &(pResBuf[0U]));
    if (Result == E_OK)
    {
      *pResLen = DCM_OBD_AVAILABILITY_RES_SIZE;
    }
    else
    {
      /* !LINKSTO Dcm.ISO-15031-5_8.6.2_2,1 */
      *pNrc = DCM_E_REQUESTOUTOFRANGE;
    }
  }
  else
  {
    /* Buffer is too small for the response data; Set the Negative Response Code. */
    /* !LINKSTO Dcm.EB.ReadDataByIdentifier.NRC0x14,1 */
    *pNrc = DCM_E_RESPONSETOOLONG;
  }

  DBG_DCM_DSP_MIDSERVICES_GETAVAILABILITYMID_EXIT(Result, ObdMid, pResBuf, MaxResBufLen
                                                  ,pResLen, pNrc);

  return Result;
}

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

#endif /* #if(DCM_DSP_USE_SERVICE_0X06 == STD_ON) */

/*==================[end of file]================================================================*/

