/**
 * \file
 *
 * \brief AUTOSAR ComXf
 *
 * This file contains the implementation of the AUTOSAR
 * module ComXf.
 *
 * \version 1.0.37
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */


/*==================[inclusions]============================================*/

#include <ComXf_Int.h>          /* internal functions and types */

/*==================[version check]=========================================*/
/*------------------[AUTOSAR vendor identification check]-------------------*/

#ifndef COMXF_VENDOR_ID /* configuration check */
#error COMXF_VENDOR_ID must be defined
#endif

#if (COMXF_VENDOR_ID != 1U) /* vendor check */
#error COMXF_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#ifndef COMXF_AR_RELEASE_MAJOR_VERSION /* configuration check */
#error COMXF_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (COMXF_AR_RELEASE_MAJOR_VERSION != 4U)
#error COMXF_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef COMXF_AR_RELEASE_MINOR_VERSION /* configuration check */
#error COMXF_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (COMXF_AR_RELEASE_MINOR_VERSION != 2U )
#error COMXF_AR_RELEASE_MINOR_VERSION wrong (!= 2U)
#endif

#ifndef COMXF_AR_RELEASE_REVISION_VERSION /* configuration check */
#error COMXF_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (COMXF_AR_RELEASE_REVISION_VERSION != 1U )
#error COMXF_AR_RELEASE_REVISION_VERSION wrong (!= 1U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#ifndef COMXF_SW_MAJOR_VERSION /* configuration check */
#error COMXF_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (COMXF_SW_MAJOR_VERSION != 1U)
#error COMXF_SW_MAJOR_VERSION wrong (!= 1U)
#endif

#ifndef COMXF_SW_MINOR_VERSION /* configuration check */
#error COMXF_SW_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (COMXF_SW_MINOR_VERSION < 0U)
#error COMXF_SW_MINOR_VERSION wrong (< 0U)
#endif

#ifndef COMXF_SW_PATCH_VERSION /* configuration check */
#error COMXF_SW_PATCH_VERSION must be defined
#endif

/* patch version check */
#if (COMXF_SW_PATCH_VERSION < 37U)
#error COMXF_SW_PATCH_VERSION wrong (< 37U)
#endif

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

#define COMXF_START_SEC_CODE
#include <ComXf_MemMap.h>

/*--------------------- ComXf_GetVersionInfo ----------------------------*/
#if (COMXF_VERSION_INFO_API == STD_ON)
FUNC(void, COMXF_CODE) ComXf_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, COMXF_APPL_DATA) VersionInfo
)
{
  DBG_COMXF_GETVERSIONINFO_ENTRY(VersionInfo);

#if (COMXF_DEV_ERROR_DETECT == STD_ON)
  if (NULL_PTR == VersionInfo)
  {
    COMXF_EB_DET_REPORT_ERROR(COMXF_SID_GETVERSIONINFO, COMXF_E_PARAM_POINTER);
  }
  else
#endif /* COMXF_DEV_ERROR_DETECT == STD_ON */
  {
    /* assign logistic values to structure members */
    VersionInfo->vendorID = (uint16) COMXF_VENDOR_ID;
    VersionInfo->moduleID = (uint16) COMXF_MODULE_ID;
    VersionInfo->sw_major_version = (uint8) COMXF_SW_MAJOR_VERSION;
    VersionInfo->sw_minor_version = (uint8) COMXF_SW_MINOR_VERSION;
    VersionInfo->sw_patch_version = (uint8) COMXF_SW_PATCH_VERSION;
  }

  DBG_COMXF_GETVERSIONINFO_EXIT(VersionInfo);
}
#endif /* COMXF_VERSION_INFO_API == STD_ON */

/*------------------------------ ComXf_Init -----------------------------*/
FUNC(void, COMXF_CODE) ComXf_Init
(
  P2CONST(ComXf_ConfigType, AUTOMATIC, COMXF_APPL_CONST) config
)
{
  DBG_COMXF_INIT_ENTRY(config);

  TS_PARAM_UNUSED(config);

#if (COMXF_DEV_ERROR_DETECT == STD_ON)
  if (Com_ComXf_GetStatus() != COM_COMXF_INIT)
  {
     COMXF_EB_DET_REPORT_ERROR(COMXF_SID_INIT, COMXF_E_UNINIT);
  }
#endif /* (COMXF_DEV_ERROR_DETECT == STD_ON) */

  DBG_COMXF_INIT_EXIT(config);
}

/*------------------------------ ComXf_DeInit ---------------------------*/
FUNC(void, COMXF_CODE) ComXf_DeInit(void)
{

}


#if (COMXF_DEV_ERROR_DETECT == STD_ON)

/*------------------------------ ComXf_EB_CheckInput------------------------*/
FUNC(void, COMXF_CODE) ComXf_EB_CheckInput
(
  P2CONST(uint8, AUTOMATIC, COMXF_APPL_DATA) buffer,
  P2VAR(uint16, AUTOMATIC, COMXF_APPL_DATA) bufferLength,
  P2CONST(void, AUTOMATIC, COMXF_APPL_DATA) dataElement,
  P2VAR(uint8, AUTOMATIC, COMXF_APPL_DATA) retvalPtr
)
{
  uint8 retval = E_SER_GENERIC_ERROR;

  if (bufferLength != NULL_PTR)
  {
     *bufferLength = 0U;
  }

  if (Com_ComXf_GetStatus() != COM_COMXF_INIT)
  {

     COMXF_EB_DET_REPORT_ERROR(COMXF_SID_SR_TRANSFORMER, COMXF_E_UNINIT);

  } else if (buffer == NULL_PTR)
  {

     COMXF_EB_DET_REPORT_ERROR(COMXF_SID_SR_TRANSFORMER, COMXF_E_PARAM_POINTER);

  } else if (bufferLength == NULL_PTR)
  {

     COMXF_EB_DET_REPORT_ERROR(COMXF_SID_SR_TRANSFORMER, COMXF_E_PARAM_POINTER);

  }
  else if (dataElement == NULL_PTR)
  {

     COMXF_EB_DET_REPORT_ERROR(COMXF_SID_SR_TRANSFORMER, COMXF_E_PARAM_POINTER);
  }
  else
  {
     retval = E_OK;
  }

  *retvalPtr = retval;

}

/*------------------------------ ComXf_EB_Inv_CheckInput--------------------*/
FUNC(void, COMXF_CODE) ComXf_EB_Inv_CheckInput
(
  P2CONST(uint8, AUTOMATIC, COMXF_APPL_DATA) buffer,
  uint16 bufferLength,
  P2CONST(void, AUTOMATIC, COMXF_APPL_DATA) dataElement,
  P2VAR(uint8, AUTOMATIC, COMXF_APPL_DATA) retvalPtr,
  Com_SignalIdType SignalGroupId
)
{
  uint8 retval = E_SER_GENERIC_ERROR;


  if (Com_ComXf_GetStatus() != COM_COMXF_INIT)
  {

     COMXF_EB_DET_REPORT_ERROR(COMXF_SID_SR_INV_TRANSFORMER, COMXF_E_UNINIT);

  } else
  {
     uint16 const expectedLength = Com_ComXf_Get_RxSignalGroupSize(SignalGroupId);

     /* buffer == NULL_PTR and bufferLength == 0U
      * is checked within the generated API */
     if (buffer == NULL_PTR)
     {

        COMXF_EB_DET_REPORT_ERROR(COMXF_SID_SR_INV_TRANSFORMER, COMXF_E_PARAM_POINTER);

     }
     /* SWS_ComXf_00013: transformer shall accept larger array representations */
     else if (expectedLength > bufferLength)
     {

        COMXF_EB_DET_REPORT_ERROR(COMXF_SID_SR_INV_TRANSFORMER, COMXF_E_PARAM);

     }
     else if (dataElement == NULL_PTR)
     {

        COMXF_EB_DET_REPORT_ERROR(COMXF_SID_SR_INV_TRANSFORMER, COMXF_E_PARAM_POINTER);

     }
     else
     {
        retval = E_OK;
     }
  }

  *retvalPtr = retval;
}

#endif /* (COMXF_DEV_ERROR_DETECT == STD_ON) */


/*==================[internal function definitions]=========================*/

#define COMXF_STOP_SEC_CODE
#include <ComXf_MemMap.h>

/*==================[end of file]===========================================*/
