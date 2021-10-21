/* --------{ EB Automotive C Source File }-------- */

/* This file contains definitions for internal functions provided to
 * get the needed information for the J1939 freeze frame data. */

/* MISRA-C:2012 Deviation List
 *
 *  MISRAC2012-1) Deviated Rule: 13.5 (required)
 *    "The right-hand operand of a logical '&&' or '||' operator shall not
 *    contain side effects."
 *
 *    Reason:
 *    Call to getter functions which do not modify any global state.
 */

/*==================[inclusions]============================================*/
/* !LINKSTO dsn.Dem.IncludeStr, 1 */

#include <Std_Types.h>                            /* AUTOSAR standard types */
#include <TSAutosar.h>                        /* EB specific standard types */
#include <TSMem.h>                          /* EB specific memory functions */

#include <Dem_Int.h>             /* Module public and internal declarations */
#include <Dem_Trace.h>                        /* Dbg related macros for Dem */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/



#define DEM_START_SEC_CODE
#include <Dem_MemMap.h>

#if ( (DEM_J1939_FREEZEFRAME_SUPPORT==STD_ON) && ( DEM_NUM_J1939FFSPNS > 0U ) )

/*==================[internal function declarations]========================*/
/** \brief Function to store a complete J1939 freeze frame at the specified position
 ** in memory, by using the RTE-callback function.
 **
 ** It shall not be called with invalid reference to a J1939 freeze frame class
 **
 ** \param[in] J1939FFClass  Class of J1939 freeze frame part
 ** \param[in] FFEntryData  Freeze frame entry data destination
 ** \param[in] EventId  The Id of the event which triggered the capture of the FreezeFrame
 **
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous}
 */
STATIC FUNC(void, DEM_CODE) Dem_StoreJ1939FF(
  CONSTP2CONST(Dem_J1939FFClassType, AUTOMATIC, DEM_CONST) J1939FFClass,
  const Dem_EntryDataPtrType                          J1939FFEntryData,
  Dem_EventIdType                                     EventId);

/** \brief Function to copy all J1939 freeze frame SPNd values
 ** into the destination buffer
 **

 ** \param[in] J1939FFClass  Class of J1939 freeze frame
 ** \param[in] J1939FFEntryData  J1939 Freeze frame entry data source
 ** \param[out] DestBuffer  Destination buffer of overlying API
 **
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous}
 */
STATIC FUNC(void, DEM_CODE) Dem_CopyJ1939FFInfo(
  P2CONST(Dem_J1939FFClassType, AUTOMATIC, DEM_CONST) J1939FFClass,
  Dem_EntryDataPtrConstType                      J1939FFEntryData,
  P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA)         DestBuffer);


/** \brief Function to get the size of a J1939 freeze frame by the given freeze
 ** frame SPN
 **
 ** \param[in] SPN Idx
 **
 ** \return Size of the J1939 freeze frame SPN
 **
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
STATIC FUNC(Dem_SizeJ1939FFType, DEM_CODE) Dem_SizeOfJ1939FFSPN(
  Dem_DataElementIdxType J1939FFSPN);

/** \brief Function to get a pointer to the specified J1939 FF-record entry data of
 ** an event memory entry
 **
 ** J1939FF class must not be the ::Dem_NullJ1939FFClass for this event.
 **
 ** \param[in] EvMemEntry
 **
 ** \return Pointer to the J1939 FF-record entry data
 **
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
STATIC FUNC_P2VAR(Dem_EntryDataType, DEM_VAR_CLEARED, DEM_CODE) Dem_J1939FFEntryData(
  Dem_EventMemoryEntryPtrConstType               EvMemEntry);

/*==================[external function definitions]=========================*/


FUNC(Dem_J1939FFClassIdxType, DEM_CODE) Dem_GbiJ1939FFClassIdx(Dem_EventIdType EventId)
{
  Dem_J1939FFClassIdxType Result =(Dem_J1939FFClassIdxType)DEM_J1939FFCLS_NULL_IDX;
  /* Master Event Id in case of combined DTC used */
  Dem_EventIdType MasterEventId = DEM_DESC_J1939_IDX(EventId);
  DBG_DEM_GBIJ1939FFCLASSIDX_ENTRY(EventId);
  Result = Dem_J1939Config[MasterEventId].Dem_J1939FFClassIdx;
  DBG_DEM_GBIJ1939FFCLASSIDX_EXIT(Result, EventId);
  return Result;
}

FUNC(boolean, DEM_CODE) Dem_ProcessEventEntryJ1939FFData(
  Dem_EventIdType             EventId,
  Dem_EventMemoryEntryPtrType EvMemEntryPtr)
{
  boolean FreezeFrameUpdated = FALSE;
  /* J1939 Freeze frame class index */
  Dem_J1939FFClassIdxType J1939FFClassIdx =Dem_GbiJ1939FFClassIdx(EventId);
  DBG_DEM_PROCESSEVENTENTRYJ1939FFDATA_ENTRY(EventId, EvMemEntryPtr);
  /* check if event has a configured J1939 Freeze frame */
  if (J1939FFClassIdx != DEM_J1939FFCLS_NULL_IDX)
  {
    /* J1939 Freeze frame Class */
    CONSTP2CONST(Dem_J1939FFClassType, AUTOMATIC, DEM_CONST) J1939FFClass=&Dem_J1939FFClass[J1939FFClassIdx];
    /* pointer to J1939FF data - for writing */
    const Dem_EntryDataPtrType J1939FFEntryData =Dem_J1939FFEntryData(EvMemEntryPtr);
    /* collect J1939 Freeze frame data and store it */
    Dem_StoreJ1939FF(J1939FFClass, J1939FFEntryData, EventId);

    FreezeFrameUpdated = TRUE;
  }

  DBG_DEM_PROCESSEVENTENTRYJ1939FFDATA_EXIT(FreezeFrameUpdated, EventId, EvMemEntryPtr);
  return FreezeFrameUpdated;
}

FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE )
  Dem_CollectJ1939FFInfo(
    Dem_EventIdType                         EventId,
    P2VAR(Dem_EventMemoryEntryPtrType, AUTOMATIC, AUTOMATIC) EvMemEntry,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA)  DestBuffer)
{
  Dem_ReturnGetNextFilteredElementType Result = DEM_FILTERED_NO_MATCHING_ELEMENT;
  /* find event memory entry */
  const boolean EntryExists =Dem_SearchForEntry(EventId, EvMemEntry, NULL_PTR);
  /* J1939 freeze frame class index */
  Dem_J1939FFClassIdxType J1939FFClassIdx =Dem_GbiJ1939FFClassIdx(EventId);
  DBG_DEM_COLLECTJ1939FFINFO_ENTRY(EventId, BufSize, DestBuffer);
  /* check if event exist and has a configured J1939 Freeze frame */
  if ((EntryExists == TRUE) && (J1939FFClassIdx != DEM_J1939FFCLS_NULL_IDX))
  {
    /* pointer to J1939FF data - for reading */
    Dem_EntryDataPtrConstType   J1939FFEntryData=Dem_J1939FFEntryData(*EvMemEntry);
    /* J1939 freeze frame class */
    P2CONST(Dem_J1939FFClassType, AUTOMATIC, DEM_CONST) J1939FFClass=&Dem_J1939FFClass[J1939FFClassIdx];
    /* J1939 freeze frame class size */
    Dem_SizeJ1939FFType J1939FFSize = Dem_SizeOfJ1939FF(J1939FFClass);
    /* check if buffer size is sufficient */
    if (J1939FFSize <= (*BufSize))
    {
      /* !LINKSTO Dem.J1939.Dem_J1939DcmGetNextFreezeFrame.MatchingOut, 1 */
      /* copy J1939 FF from it's memory to the buffer */
      Dem_CopyJ1939FFInfo(J1939FFClass, J1939FFEntryData, DestBuffer);
      /* update buffer size with J1939 freeze frame class size */
      *BufSize = J1939FFSize;
      /* !LINKSTO Dem.J1939.Dem_J1939DcmGetNextFreezeFrame.FilterOK, 1 */
      Result = DEM_FILTERED_OK;
    }
    else
    {
      /* !LINKSTO Dem.J1939.Dem_J1939DcmGetNextFreezeFrame.SmallBufferSize, 1 */
      /* destination buffer is too small */
      Result=DEM_FILTERED_BUFFER_TOO_SMALL;
    }
  }
  else
  {
    /* !LINKSTO Dem.J1939.Dem_J1939DcmGetNextFreezeFrame.NoMatchedFreezeFrame, 1 */
    /* No J1939 Freeze Frame stored */
    Result = DEM_FILTERED_NO_MATCHING_ELEMENT;
  }
  DBG_DEM_COLLECTJ1939FFINFO_EXIT(EventId, BufSize, DestBuffer, Result);
  return Result;
}
FUNC(Dem_SizeJ1939FFType, DEM_CODE) Dem_SizeOfJ1939FF(
  P2CONST(Dem_J1939FFClassType, AUTOMATIC, DEM_CONST) J1939FFClass)
{
  Dem_SizeJ1939FFType Result=0U;
  /* number of SPNs in J1939 FF */
  Dem_NumFFSPNsType NumJ1939FFSPNs= J1939FFClass->NumJ1939FFSPNs;
  /* pointer to SPNs indexes array */
  CONSTP2CONST(Dem_J1939FFSPNIdxType, AUTOMATIC, DEM_CONST) J1939FFSPNIdx=J1939FFClass->J1939FFSPNIdx;
  /* used to loop through all related SPNs */
  Dem_NumFFSPNsType IndexFFSPN=0U;
  DBG_DEM_SIZEOFJ1939FF_ENTRY(J1939FFClass);
  /* loop through all SPNs and calculate it's size */
  for(IndexFFSPN=0U;IndexFFSPN < NumJ1939FFSPNs;IndexFFSPN++)
  {
    /* current SPN Index */
    Dem_J1939FFSPNIdxType SingleJ1939FFSPNIdx=J1939FFSPNIdx[IndexFFSPN];
    /* calculate size of single SPN */
    Result += Dem_SizeOfJ1939FFSPN(Dem_J1939FFSPNs[SingleJ1939FFSPNIdx]);
  }
  DBG_DEM_SIZEOFJ1939FF_EXIT(J1939FFClass, Result);
  return Result;
}

/*==================[internal function definitions]=========================*/

STATIC FUNC(void, DEM_CODE) Dem_StoreJ1939FF(
  CONSTP2CONST(Dem_J1939FFClassType, AUTOMATIC, DEM_CONST) J1939FFClass,
  const Dem_EntryDataPtrType                          J1939FFEntryData,
  Dem_EventIdType                                     EventId)
{
  /* number of SPNs in J1939 FF */
  Dem_NumFFSPNsType NumJ1939FFSPNs= J1939FFClass->NumJ1939FFSPNs;
  /* SPN indexes array Pointer */
  CONSTP2CONST(Dem_J1939FFSPNIdxType, AUTOMATIC, DEM_CONST) J1939FFSPNIdx=J1939FFClass->J1939FFSPNIdx;
  Dem_NumFFSPNsType IndexFFSPN=0U;
  Dem_DataElementIdxType DataElementIdx=0U;
  Dem_SizeEntryDataType FFStartByte = 0U;

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  DBG_DEM_STOREJ1939FF_ENTRY(DEM_INTERNAL_API_ID, J1939FFClass, J1939FFEntryData, EventId);
#else
  DBG_DEM_STOREJ1939FF_ENTRY(0U, J1939FFClass, J1939FFEntryData, EventId);
#endif
  /* loop through all SPNs and read the related data element */
  /* !LINKSTO dsn.Dem.J1939.FreezeFrame.J1939SPNuserDefined, 1 */
  for(IndexFFSPN=0U;IndexFFSPN < NumJ1939FFSPNs;IndexFFSPN++)
  {
    Std_ReturnType DataPresent = E_NOT_OK;
    Dem_SizeEntryDataType DataElementSize = 0U;
    Dem_J1939FFSPNIdxType SingleJ1939FFSPNIdx=J1939FFSPNIdx[IndexFFSPN]; /* SPN class */
    DataElementIdx= Dem_J1939FFSPNs[SingleJ1939FFSPNIdx]; /* data element index */
    DataElementSize = DEM_EXTERNAL_DATA_ELEMENT_SIZE(DataElementIdx); /* data element size */
    /* read data related to SPN's data element */
    DataPresent = Dem_ReadExternalDataElement(&J1939FFEntryData[FFStartByte],
                                        DataElementIdx,
                                        EventId);
    /* check if data wasn't stored successfully */
    /* and then fill buffer with padding according to data element size */
    if (DataPresent != E_OK)
    {
      const Dem_SizeEntryDataType StartIndex = FFStartByte;
      Dem_SizeEntryDataType StopIndex;
      Dem_SizeEntryDataType Index;

      StopIndex = StartIndex + DataElementSize;

      /* fill FF segment with padding values */
      for (Index = StartIndex; Index < StopIndex; ++Index)
      {
        J1939FFEntryData[Index] = DEM_DATA_PADDING_VALUE;
      }

#if (DEM_DEV_ERROR_DETECT == STD_ON)
      /* report non-available data for FFS;
       * the calling context may be Dem_MainFunction or
       * Dem_SetEventStatus if configuration parameter DemFreezeFrameCapture is set to
       * DEM_TRIGGER_TESTFAILED */
      DEM_REPORT_ERROR(DEM_INTERNAL_API_ID, DEM_E_NODATAAVAILABLE);
#endif
    }

    /* add element size -> start byte for the next element */
     FFStartByte += DataElementSize;
  }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
  DBG_DEM_STOREJ1939FF_EXIT(DEM_INTERNAL_API_ID, J1939FFClass, J1939FFEntryData, EventId);
#else /* DEM_DEV_ERROR_DETECT == STD_OFF */
  DBG_DEM_STOREJ1939FF_EXIT(0U, J1939FFClass, J1939FFEntryData, EventId);
#endif /* DEM_DEV_ERROR_DETECT == STD_ON */

}

STATIC FUNC(void, DEM_CODE) Dem_CopyJ1939FFInfo(
  P2CONST(Dem_J1939FFClassType, AUTOMATIC, DEM_CONST) J1939FFClass,
  Dem_EntryDataPtrConstType                      J1939FFEntryData,
  P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA)         DestBuffer)
{
  /* J1939 FF size */
  Dem_SizeJ1939FFType J1939FFSize = Dem_SizeOfJ1939FF(J1939FFClass);

  DBG_DEM_COPYJ1939FFINFO_ENTRY(J1939FFClass, J1939FFEntryData, DestBuffer);
  /* write content into response */
  TS_MemCpy(&DestBuffer[0], &J1939FFEntryData[0], J1939FFSize);
  DBG_DEM_COPYJ1939FFINFO_EXIT(J1939FFClass, J1939FFEntryData, DestBuffer);
}


STATIC FUNC(Dem_SizeJ1939FFType, DEM_CODE) Dem_SizeOfJ1939FFSPN(
  Dem_DataElementIdxType J1939FFSPN)
{
  Dem_SizeJ1939FFType Result = 0U;
  DBG_DEM_SIZEOFJ1939FFSPN_ENTRY(J1939FFSPN);
#if (DEM_NUM_EXT_DATAELEMENTS > 0U)
  /* get size of SPN's data element */
  Result = DEM_EXTERNAL_DATA_ELEMENT_SIZE(J1939FFSPN);
#else /* DEM_NUM_EXT_DATAELEMENTS == 0U */
  TS_PARAM_UNUSED(J1939FFSPN);
#endif /* DEM_NUM_EXT_DATAELEMENTS > 0U */
  DBG_DEM_SIZEOFJ1939FFSPN_EXIT(Result, J1939FFSPN);
  return Result;
}

STATIC FUNC_P2VAR(Dem_EntryDataType, DEM_VAR_CLEARED, DEM_CODE) Dem_J1939FFEntryData(
  Dem_EventMemoryEntryPtrConstType               EvMemEntry)
{
  const Dem_EventIdType EventId = EvMemEntry->EventId;
  const uint8 OriginIdx = Dem_GbiDTCOriginIdx(EventId);
  Dem_EntryDataPtrType Result;
#if (DEM_NUM_EDSEGS > 0U)
#if (DEM_NUM_EXT_DATAELEMENTS > 0U)
  /* get extended data class layout */
  CONSTP2CONST(Dem_EDClassType, AUTOMATIC, DEM_CONST) EDClass =
    &Dem_EDClass[Dem_GbiEDClassIdx(EventId)];
#endif
#endif /* DEM_NUM_EDSEGS > 0U */

#if (DEM_NUM_FFSEGS > 0U)
  /* get freeze frame class layout */
  CONSTP2CONST(Dem_FFClassType, AUTOMATIC, DEM_CONST) FFClass =
    &Dem_FFClass[Dem_GbiFFClassIdx(EventId)];
  const Dem_FFIdxType MaxNumFF = Dem_GbiMaxNumFFRecs(EventId);
#endif

  /* calculate start position of the needed FF */
  const Dem_SizeEntryDataType StartPos = EvMemEntry->EntryDataPos +
#if (DEM_USE_INT_VAL_AREA == STD_ON)
  /* calculate size of internal values area */
  Dem_SizeOfIntValArea(EventId) +
#endif

#if (DEM_NUM_EDSEGS > 0U)
  /* calculate size of Extended freeze frame */
  Dem_SizeOfED(EDClass) +
#endif

#if (DEM_NUM_FFSEGS > 0U)
  /* calculate size of freeze frame */
  ((Dem_SizeEntryDataType)Dem_SizeOfFF(DEM_REGULAR_FF, FFClass) *
    MaxNumFF);
#else /* DEM_NUM_FFSEGS == 0U */
  /* in case no freeze frame configured, terminate statement */
  0U;
#endif /* DEM_NUM_FFSEGS > 0U */
  DBG_DEM_J1939FFENTRYDATA_ENTRY(EvMemEntry);
  Result = &Dem_EntryData[OriginIdx][StartPos];
  DBG_DEM_J1939FFENTRYDATA_EXIT(EvMemEntry, Result);
  return Result;
}
#endif /* (DEM_J1939_FREEZEFRAME_SUPPORT==STD_ON) && ( DEM_NUM_J1939FFSPNS > 0U ) */



#define DEM_STOP_SEC_CODE
#include <Dem_MemMap.h>
/*==================[end of file]===========================================*/
