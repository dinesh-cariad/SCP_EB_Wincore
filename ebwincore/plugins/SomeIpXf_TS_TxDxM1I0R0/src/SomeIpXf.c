/**
 * \file
 *
 * \brief AUTOSAR SomeIpXf
 *
 * This file contains the implementation of the AUTOSAR
 * module SomeIpXf.
 *
 * \version 1.0.47
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */



/*==================[inclusions]============================================*/

#include <SomeIpXf_Int.h>          /* internal functions and types */
#include <SomeIpXf_Api.h>

/*==================[version check]=========================================*/
/*------------------[AUTOSAR vendor identification check]-------------------*/

#ifndef SOMEIPXF_VENDOR_ID /* configuration check */
#error SOMEIPXF_VENDOR_ID must be defined
#endif

#if (SOMEIPXF_VENDOR_ID != 1U) /* vendor check */
#error SOMEIPXF_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#ifndef SOMEIPXF_AR_RELEASE_MAJOR_VERSION /* configuration check */
#error SOMEIPXF_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (SOMEIPXF_AR_RELEASE_MAJOR_VERSION != 4U)
#error SOMEIPXF_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef SOMEIPXF_AR_RELEASE_MINOR_VERSION /* configuration check */
#error SOMEIPXF_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (SOMEIPXF_AR_RELEASE_MINOR_VERSION != 2U )
#error SOMEIPXF_AR_RELEASE_MINOR_VERSION wrong (!= 2U)
#endif

#ifndef SOMEIPXF_AR_RELEASE_REVISION_VERSION /* configuration check */
#error SOMEIPXF_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (SOMEIPXF_AR_RELEASE_REVISION_VERSION != 1U )
#error SOMEIPXF_AR_RELEASE_REVISION_VERSION wrong (!= 1U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#ifndef SOMEIPXF_SW_MAJOR_VERSION /* configuration check */
#error SOMEIPXF_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (SOMEIPXF_SW_MAJOR_VERSION != 1U)
#error SOMEIPXF_SW_MAJOR_VERSION wrong (!= 1U)
#endif

#ifndef SOMEIPXF_SW_MINOR_VERSION /* configuration check */
#error SOMEIPXF_SW_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (SOMEIPXF_SW_MINOR_VERSION < 0U)
#error SOMEIPXF_SW_MINOR_VERSION wrong (< 0U)
#endif

#ifndef SOMEIPXF_SW_PATCH_VERSION /* configuration check */
#error SOMEIPXF_SW_PATCH_VERSION must be defined
#endif

/* patch version check */
#if (SOMEIPXF_SW_PATCH_VERSION < 47U)
#error SOMEIPXF_SW_PATCH_VERSION wrong (< 47U)
#endif

/*==================[macros]================================================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

#define SOMEIPXF_START_SEC_CODE
#include <SomeIpXf_MemMap.h>

/*--------------------- SomeIpXf_GetVersionInfo ----------------------------*/
#if (SOMEIPXF_VERSION_INFO_API == STD_ON)
FUNC(void, SOMEIPXF_CODE) SomeIpXf_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) versioninfo
)
{
  if (NULL_PTR != versioninfo)
  {
    /* assign logistic values to structure members */
    versioninfo->vendorID = (uint16) SOMEIPXF_VENDOR_ID;
    versioninfo->moduleID = (uint16) SOMEIPXF_MODULE_ID;
    versioninfo->sw_major_version = (uint8) SOMEIPXF_SW_MAJOR_VERSION;
    versioninfo->sw_minor_version = (uint8) SOMEIPXF_SW_MINOR_VERSION;
    versioninfo->sw_patch_version = (uint8) SOMEIPXF_SW_PATCH_VERSION;
  }
}
#endif /* SOMEIPXF_VERSION_INFO_API == STD_ON */


/*-------------------- SomeIpXf_GenericTransformer ----------------------*/
FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_GenericTransformer
(
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) BufferLengthPtr,
   P2CONST(void, AUTOMATIC, SOMEIPXF_APPL_DATA) DataPtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr
)
{
   /* used for defensive programming only (no Det interface) */
   uint8 RetVal = E_OK;

   SomeIpXf_AdminDataType AdminData;

   /* initialize contexts */
   SomeIpXf_DeSerInfoType DeSerContext;
#if (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_ON)
   StructWithLf_NESTED_LEVEL_Type StructWithLfContext;
#endif /* (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_ON) */
#if (SOMEIPXF_FSA_AVAILABLE == STD_ON)
   FSA_NESTED_LEVEL_Type FSAContext;
#endif /* (SOMEIPXF_FSA_AVAILABLE == STD_ON) */
#if (SOMEIPXF_VSA_AVAILABLE == STD_ON)
   /* initialize VSAContext */
   VSA_NESTED_LEVEL_Type VSAContext;
#endif /* (SOMEIPXF_VSA_AVAILABLE == STD_ON) */
#if (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON)
   /* initialize TLVContext */
   TLV_Nested_Type TLVContext;
#endif /* (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON) */

   /* initialize context members */
   SomeIpXf_S_Init_DeSerContext(&DeSerContext);
#if (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_ON)
   SomeIpXf_S_Init_StructWithLfContext(&StructWithLfContext,SOMEIPXF_SER);
#endif /* (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_ON) */

#if (SOMEIPXF_FSA_AVAILABLE == STD_ON)
   SomeIpXf_S_Init_FSAContext(&FSAContext,SOMEIPXF_SER);
#endif /* (SOMEIPXF_FSA_AVAILABLE == STD_ON) */
#if (SOMEIPXF_VSA_AVAILABLE == STD_ON)
   SomeIpXf_S_Init_VSAContext(&VSAContext,SOMEIPXF_SER);
#endif /* (SOMEIPXF_VSA_AVAILABLE == STD_ON) */
#if (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON)
   SomeIpXf_Init_TLVContext(&TLVContext);
#endif /* (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON) */

   /* initialize AdminData members */
   SomeIpXf_S_Init_AdminData(&AdminData, NULL_PTR, SOMEIPXF_SER);

   /* assign contexts AdminData members */
   AdminData.DeSerContextPtr = &DeSerContext;
#if (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_ON)
   AdminData.StructWithLfContextPtr = &StructWithLfContext;
#endif /* (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_ON) */
#if (SOMEIPXF_FSA_AVAILABLE == STD_ON)
   AdminData.FSAContextPtr = &FSAContext;
#endif /* (SOMEIPXF_FSA_AVAILABLE == STD_ON) */
#if (SOMEIPXF_VSA_AVAILABLE == STD_ON)
   AdminData.VSAContextPtr = &VSAContext;
#endif /* (SOMEIPXF_VSA_AVAILABLE == STD_ON) */
#if (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON)
   AdminData.TLVContextPtr = &TLVContext;
#endif /* (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON) */

   do{

      AdminData.ElemFound = FALSE;

      RetVal = SomeIpXf_S_Int_GenericTransformer(
         BufPtr,
         DataPtr,
         ConfigPtr,
         &AdminData
      );

/* CHECK: NOPARSE */
      if((RetVal == E_OK) && (AdminData.ElemFound == FALSE))
      {
         RetVal = SomeIpXf_Int_GenericTransformer(
            BufPtr,
            DataPtr,
            ConfigPtr,
            &AdminData
         );
      }
/* CHECK: PARSE */

      AdminData.DeSerContextPtr->CfgElemHandled++;

   /* end of serialized data type reached in configuration */
   }while ( (ConfigPtr[AdminData.DeSerContextPtr->CfgElemHandled] != SOMEIPXF_DATA_ELEMENT_STOP)
            && (RetVal == E_OK) );

   *BufferLengthPtr += AdminData.DeSerContextPtr->BytesCopied;

   /* used for defensive programming only (no Det interface) */
   return RetVal;
}


/*------------------- SomeIpXf_Inv_GenericTransformer --------------------*/
FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_Inv_GenericTransformer
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength,
   P2VAR(void, AUTOMATIC, SOMEIPXF_APPL_DATA) DataPtr,
   P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) BytesCopiedPtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr
)
{
   uint8 RetVal;

   SomeIpXf_AdminDataType AdminData;

   /* initialize contexts */
   SomeIpXf_DeSerInfoType DeSerContext;
#if (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_ON)
   StructWithLf_NESTED_LEVEL_Type StructWithLfContext;
#endif /* (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_ON) */
#if (SOMEIPXF_FSA_AVAILABLE == STD_ON)
   FSA_NESTED_LEVEL_Type FSAContext;
#endif /* (SOMEIPXF_FSA_AVAILABLE == STD_ON) */
#if (SOMEIPXF_VSA_AVAILABLE == STD_ON)
   /* initialize VSAContext */
   VSA_NESTED_LEVEL_Type VSAContext;
#endif /* (SOMEIPXF_VSA_AVAILABLE == STD_ON) */
#if (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON)
   /* initialize TLVContext */
   TLV_Nested_Type TLVContext;
#endif /* (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON) */

   /* initialize context members */
   SomeIpXf_S_Init_DeSerContext(&DeSerContext);
#if (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_ON)
   SomeIpXf_S_Init_StructWithLfContext(&StructWithLfContext,SOMEIPXF_DE_SER);
#endif /* (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_ON) */
#if (SOMEIPXF_FSA_AVAILABLE == STD_ON)
   SomeIpXf_S_Init_FSAContext(&FSAContext,SOMEIPXF_DE_SER);
#endif /* (SOMEIPXF_FSA_AVAILABLE == STD_ON) */
#if (SOMEIPXF_VSA_AVAILABLE == STD_ON)
   SomeIpXf_S_Init_VSAContext(&VSAContext,SOMEIPXF_DE_SER);
#endif /* (SOMEIPXF_VSA_AVAILABLE == STD_ON) */

#if (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON)
   SomeIpXf_Init_TLVContext(&TLVContext);
#endif /* (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON) */

   /* initialize AdminData members */
   SomeIpXf_S_Init_AdminData(&AdminData, BytesCopiedPtr, SOMEIPXF_DE_SER);

   /* assign contexts AdminData members */
   AdminData.DeSerContextPtr = &DeSerContext;
#if (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_ON)
   AdminData.StructWithLfContextPtr = &StructWithLfContext;
#endif /* (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_ON) */
#if (SOMEIPXF_FSA_AVAILABLE == STD_ON)
   AdminData.FSAContextPtr = &FSAContext;
#endif /* (SOMEIPXF_FSA_AVAILABLE == STD_ON) */
#if (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON)
   AdminData.TLVContextPtr = &TLVContext;
#endif /* (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON) */
#if (SOMEIPXF_VSA_AVAILABLE == STD_ON)
   AdminData.VSAContextPtr = &VSAContext;
#endif /* (SOMEIPXF_VSA_AVAILABLE == STD_ON) */

   do{

      AdminData.ElemFound = FALSE;

      RetVal = SomeIpXf_S_Int_Inv_GenericTransformer(
         BufPtr,
         BufferLength,
         DataPtr,
         BytesCopiedPtr,
         ConfigPtr,
         &AdminData
      );

/* CHECK: NOPARSE */
      if((RetVal == E_OK) && (AdminData.ElemFound == FALSE))
      {
         RetVal = SomeIpXf_Int_Inv_GenericTransformer(
            BufPtr,
            BufferLength,
            DataPtr,
            BytesCopiedPtr,
            ConfigPtr,
            &AdminData
         );
      }
/* CHECK: PARSE */

      AdminData.DeSerContextPtr->CfgElemHandled++;

   /* end of successfully deserialized data type reached in configuration */
   }while ( (ConfigPtr[AdminData.DeSerContextPtr->CfgElemHandled] != SOMEIPXF_DATA_ELEMENT_STOP)
            && (RetVal == E_OK) );

   *BytesCopiedPtr += AdminData.DeSerContextPtr->BytesCopied;

   return RetVal;
}

#define SOMEIPXF_STOP_SEC_CODE
#include <SomeIpXf_MemMap.h>

/*==================[end of file]===========================================*/
