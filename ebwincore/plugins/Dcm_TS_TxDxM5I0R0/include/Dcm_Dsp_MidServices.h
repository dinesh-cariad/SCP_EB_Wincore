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

#ifndef DCM_DSP_OBDMIDSERVICES_H
#define DCM_DSP_OBDMIDSERVICES_H

/*==================[inclusions]=================================================================*/

#include <Std_Types.h>         /* AUTOSAR standard types */
#include <Dcm_Int.h>

/*==================[macros]=====================================================================*/

/** \brief macro definition used to calculate the byte position in the mask array */
#if (defined DCM_GET_FLAG_BYTE_POS)
  #error "DCM_GET_FLAG_BYTE_POS is already defined"
#endif
#define DCM_GET_FLAG_BYTE_POS(FlagId) ((FlagId) / 8U)

/** \brief macro definition used to calculate the flag mask inside the selected
 *         byte of the mask array */
#if (defined DCM_GET_FLAG_BYTE_MASK)
  #error "DCM_GET_FLAG_BYTE_MASK is already defined"
#endif
#define DCM_GET_FLAG_BYTE_MASK(FlagId) ((uint8)(1U << ((FlagId) % 8U)))

/** \brief Macro to verify if a flag is set inside a bit array variable
 **
 ** \param[in]  FlagMask     The Mask used to retrieve flag value
 ** \param[in]  BitArray     Variable containing the flag
 ** \param[in]  BitArrayType The type of the variable containing the flag
 **
 ** \retval     TRUE,        if the byte flag is set
 ** \retval     FALSE,       if the byte flag is cleared */
#if (defined DCM_IS_FLAG_SET)
  #error "DCM_IS_FLAG_SET is already defined"
#endif
#define DCM_IS_FLAG_SET(FlagMask, BitArray, BitArrayType) \
  (((BitArray) & (BitArrayType)(FlagMask)) != 0U ? TRUE : FALSE)

/** \brief Macro to verify if a flag is set inside a byte array
 **
 ** \param[in]  FlagId       The Id of the flag
 ** \param[in]  BitArrayPtr  Pointer to the byte array containing the flag
 **
 ** \retval     TRUE,        if the byte flag is set
 ** \retval     FALSE,       if the byte flag is cleared */
#if (defined DCM_IS_BYTE_FLAG_SET)
  #error "DCM_IS_BYTE_FLAG_SET is already defined"
#endif
#define DCM_IS_BYTE_FLAG_SET(FlagId, BitArrayPtr) \
  DCM_IS_FLAG_SET(DCM_GET_FLAG_BYTE_MASK((FlagId)), \
                  ((P2CONST(uint8, AUTOMATIC, AUTOMATIC)) \
                     (BitArrayPtr))[DCM_GET_FLAG_BYTE_POS((FlagId))], \
                  uint8)

/*==================[type definitions]===========================================================*/

/*==================[external function declarations]=============================================*/

#if(DCM_DSP_USE_SERVICE_0X06 == STD_ON)

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief Initializes the internals of the MidServices unit
 */
extern FUNC(void, DCM_CODE) Dcm_Dsp_MidServices_Init(void);

#if (DCM_DSP_MIDSERVICES_USE_LOCKING == STD_ON)
/** \brief Function to lock the MidServices when they need to be used exclusively by one user
 **
 ** \retval DCM_E_OK                Service successfully locked
 ** \retval DCM_E_RESOURCE_LOCKED   Service locked and not available
 **/
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_MidServices_LockServices(void);

/** \brief Function to unlock the MidServices after they were locked
 **/
extern FUNC(void, DCM_CODE) Dcm_Dsp_MidServices_UnlockServices(void);
#endif /* #if (DCM_DSP_MIDSERVICES_USE_LOCKING == STD_ON) */

/** \brief The generic handler for "availability OBDMIDs"
 **
 ** \param[in]    ObdMid       OBD MID identifier
 ** \param[out]   pResBuf      Buffer where the write the result to
 ** \param[inout] MaxResBufLen Max number of bytes that can be written to result buffer
 ** \param[out]   pResLen      Number of bytes in result buffer
 ** \param[out]   pNrc         Pointer to negative response code
 **
 ** \retval E_OK                Processing done, valid result in buffer
 **         E_NOT_OK            Processing done, error code in Nrc
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_MidServices_GetAvailabilityMid(
  uint8 ObdMid,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) pResBuf,
  Dcm_MsgLenType MaxResBufLen,
  P2VAR(Dcm_MsgLenType, AUTOMATIC, DCM_VAR) pResLen,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) pNrc);

/** \brief The generic handler for "non-availability OBDMIDs"
 **
 ** \param[in]    ObdMid       OBD MID identifier
 ** \param[out]   pResBuf      Buffer where the write the result to
 ** \param[inout] MaxResBufLen Max number of bytes that can be written to result buffer
 ** \param[out]   pResLen      Number of bytes in result buffer
 ** \param[out]   pNrc         Pointer to negative response code
 ** \param[in]    WriteMidBeforeTid  Adds OBDMID in front of every configured TID inside one OBDMID
 **
 ** \retval E_OK                Processing done, valid result in buffer
 **         E_NOT_OK            Processing done, error code in Nrc
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_MidServices_GetNonAvailabilityMid(
  uint8 ObdMid,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) pResBuf,
  Dcm_MsgLenType MaxResBufLen,
  P2VAR(Dcm_MsgLenType, AUTOMATIC, DCM_VAR) pResLen,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) pNrc,
  boolean WriteMidBeforeTid);

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

#endif /* #if(DCM_DSP_USE_SERVICE_0X06 == STD_ON) */

/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

/*==================[internal data]==============================================================*/

/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/

#endif /* ifndef DCM_DSP_OBDMIDSERVICES_H */
/*==================[end of file]================================================================*/

