/**
 * \file
 *
 * \brief AUTOSAR KeyM
 *
 * This file contains the implementation of the AUTOSAR
 * module KeyM.
 *
 * \version 1.2.11
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef KEYM_H
#define KEYM_H

/*==================[deviations]==================================================================*/

/*==================[includes]====================================================================*/

/* !LINKSTO KeyM.Dsn.Files/KeyM.h/0001,1 */

#include <KeyM_Cfg.h>

/*==================[macros]======================================================================*/

/* --- InstanceId ------------------------------------------------------------------------------- */

#if (defined KEYM_INSTANCE_ID)
#error KEYM_INSTANCE_ID is already defined
#endif
/** \brief  The index of the KeyM instance.
 **         As the KeyM is a single instance module it is always 0.
 **/
#define KEYM_INSTANCE_ID 0U

/* --- ServiceIds ------------------------------------------------------------------------------- */

#if (defined KEYM_SID_INIT)
#error KEYM_SID_INIT is already defined
#endif
/** \brief  The 'KeyM_Init' API service identifier. **/
#define KEYM_SID_INIT 1U

#if (defined KEYM_SID_DEINIT)
#error KEYM_SID_DEINIT is already defined
#endif
/** \brief  The 'KeyM_Deinit' API service identifier. **/
#define KEYM_SID_DEINIT 2U

#if (defined KEYM_SID_GETVERSIONINFO)
#error KEYM_SID_GETVERSIONINFO is already defined
#endif
/** \brief  The 'KeyM_GetVersionInfo' API service identifier. **/
#define KEYM_SID_GETVERSIONINFO 3U

#if (defined KEYM_SID_START)
#error KEYM_SID_START is already defined
#endif
/** \brief  The 'KeyM_Start' API service identifier. **/
#define KEYM_SID_START 4U

#if (defined KEYM_SID_PREPARE)
#error KEYM_SID_PREPARE is already defined
#endif
/** \brief  The 'KeyM_Prepare' API service identifier. **/
#define KEYM_SID_PREPARE 5U

#if (defined KEYM_SID_UPDATE)
#error KEYM_SID_UPDATE is already defined
#endif
/** \brief  The 'KeyM_Update' API service identifier. **/
#define KEYM_SID_UPDATE 6U

#if (defined KEYM_SID_FINALIZE)
#error KEYM_SID_FINALIZE is already defined
#endif
/** \brief  The 'KeyM_Finalize' API service identifier. **/
#define KEYM_SID_FINALIZE 7U

#if (defined KEYM_SID_VERIFY)
#error KEYM_SID_VERIFY is already defined
#endif
/** \brief  The 'KeyM_Verify' API service identifier. **/
#define KEYM_SID_VERIFY 8U

#if (defined KEYM_SID_SERVICECERTIFICATE)
#error KEYM_SID_SERVICECERTIFICATE is already defined
#endif
/** \brief  The 'KeyM_ServiceCertificate' API service identifier. **/
#define KEYM_SID_SERVICECERTIFICATE 9U

#if (defined KEYM_SID_SETCERTIFICATE)
#error KEYM_SID_SETCERTIFICATE is already defined
#endif
/** \brief  The 'KeyM_SetCertificate' API service identifier. **/
#define KEYM_SID_SETCERTIFICATE 10U

#if (defined KEYM_SID_GETCERTIFICATE)
#error KEYM_SID_GETCERTIFICATE is already defined
#endif
/** \brief  The 'KeyM_GetCertificate' API service identifier. **/
#define KEYM_SID_GETCERTIFICATE 11U

#if (defined KEYM_SID_VERIFYCERTIFICATES)
#error KEYM_SID_VERIFYCERTIFICATES is already defined
#endif
/** \brief  The 'KeyM_VerifyCertificates' API service identifier **/
#define KEYM_SID_VERIFYCERTIFICATES 12U

#if (defined KEYM_SID_VERIFYCERTIFICATE)
#error KEYM_SID_VERIFYCERTIFICATE is already defined
#endif
/** \brief  The 'KeyM_VerifyCertificate' API service identifier. **/
#define KEYM_SID_VERIFYCERTIFICATE 13U

#if (defined KEYM_SID_VERIFYCERTIFICATECHAIN)
#error KEYM_SID_VERIFYCERTIFICATECHAIN is already defined
#endif
/** \brief  The 'KeyM_VerifyCertificateChain' API service identifier. **/
#define KEYM_SID_VERIFYCERTIFICATECHAIN 14U

#if (defined KEYM_SID_CERTELEMENTGET)
#error KEYM_SID_CERTELEMENTGET is already defined
#endif
/** \brief  The 'KeyM_CertElementGet' API service identifier. **/
#define KEYM_SID_CERTELEMENTGET 15U

#if (defined KEYM_SID_CERTELEMENTGETFIRST)
#error KEYM_SID_CERTELEMENTGETFIRST is already defined
#endif
/** \brief  The 'KeyM_CertElementGetFirst' API service identifier. **/
#define KEYM_SID_CERTELEMENTGETFIRST 16U

#if (defined KEYM_SID_CERTELEMENTGETNEXT)
#error KEYM_SID_CERTELEMENTGETNEXT is already defined
#endif
/** \brief  The 'KeyM_CertElementGetNext' API service identifier. **/
#define KEYM_SID_CERTELEMENTGETNEXT 17U

#if (defined KEYM_SID_CERTGETSTATUS)
#error KEYM_SID_CERTGETSTATUS is already defined
#endif
/** \brief  The 'KeyM_CertGetStatus' API service identifier. **/
#define KEYM_SID_CERTGETSTATUS 18U

#if (defined KEYM_SID_MAINFUNCTION)
#error KEYM_SID_MAINFUNCTION is already defined
#endif
/** \brief  The 'KeyM_MainFunction' API service identifier. **/
#define KEYM_SID_MAINFUNCTION 25U

#if (defined KEYM_SID_MAINBACKGROUNDFUNCTION)
#error KEYM_SID_MAINBACKGROUNDFUNCTION is already defined
#endif
/** \brief  The 'KeyM_MainBackgroundFunction' API service identifier. **/
#define KEYM_SID_MAINBACKGROUNDFUNCTION 26U

/* --- DevelopmentErrors ------------------------------------------------------------------------ */

/* !LINKSTO SWS_KeyM_00036,1 */
#if (defined KEYM_E_PARAM_POINTER)
#error KEYM_E_PARAM_POINTER is already defined
#endif
/** \brief  Development Error to be raised if API request called with invalid parameter.
 **         (Nullpointer)
 **/
#define KEYM_E_PARAM_POINTER 1U

#if (defined KEYM_E_SMALL_BUFFER)
#error KEYM_E_SMALL_BUFFER is already defined
#endif
/** \brief  Development Error to be raised if API request called with provided output buffer is
 **         to small to store the requested result.
 **/
#define KEYM_E_SMALL_BUFFER 2U

#if (defined KEYM_E_UNINIT)
#error KEYM_E_UNINIT is already defined
#endif
/** \brief  Development Error to be raised if API request called before initialization of
 **         KeyM module.
 **/
#define KEYM_E_UNINIT 3U

#if (defined KEYM_E_INIT_FAILED)
#error KEYM_E_INIT_FAILED is already defined
#endif
/** \brief  Development Error to be raised if initialization of KeyM module failed. **/
#define KEYM_E_INIT_FAILED 4U

/* !LINKSTO KeyM.Dsn.Gen/KEYM_E_CSMKEYELEMENTSET_FAILED/00001,1 */
#if (defined KEYM_E_CSMKEYELEMENTSET_FAILED)
#error KEYM_E_CSMKEYELEMENTSET_FAILED is already defined
#endif
/** \brief  Development Error to be raised if Csm_KeyElementSet failed. **/
#define KEYM_E_CSMKEYELEMENTSET_FAILED 0xE0

/*==================[type definitions]============================================================*/

/*==================[external constants]==========================================================*/

/*==================[external data]===============================================================*/

/*==================[external function declarations]==============================================*/

#define KEYM_START_SEC_CODE
#include <KeyM_MemMap.h>

/** \brief  This function initializes the key management module.
 **
 ** \param[in]  ConfigPtr  Pointer to the configuration set in VARIANT-POST-BUILD.
 **
 ** \ServiceID{::KEYM_SID_INIT}
 ** \Synchronicity{Synchronous}
 ** \Reentrancy{Non Reentrant}
 **/
extern FUNC(void, KEYM_CODE) KeyM_Init
(
  P2CONST(KeyM_ConfigType, AUTOMATIC, KEYM_APPL_DATA) ConfigPtr
);

/** \brief  This function resets the key management module to the uninitialized state.
 **
 ** \ServiceID{::KEYM_SID_DEINIT}
 ** \Synchronicity{Synchronous}
 ** \Reentrancy{Non Reentrant}
 **/
extern FUNC(void, KEYM_CODE) KeyM_Deinit
(
  void
);

/** \brief  Provides the version information of this module.
 **
 ** \param[out]  VersionInfo  Pointer to the version information of this module.
 **
 ** \ServiceID{::KEYM_SID_GETVERSIONINFO}
 ** \Synchronicity{Synchronous}
 ** \Reentrancy{Non Reentrant}
 **/
extern FUNC(void, KEYM_CODE) KeyM_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, KEYM_APPL_DATA) VersionInfo
);

/** \brief  This function is optional and only used if the configuration item
 **         KeyMCryptoKeyStartFinalizeFunctionEnabled is set to true. It intents to allow key
 **         update operation.
 **
 ** \param[in]      StartType           Defines in which mode the key operation shall be
 **                                     executed.
 ** \param[in]      RequestData         Information that comes along with the request, e.g.
 **                                     signature.
 ** \param[in]      RequestDataLength   Length of data in the RequestData array.
 ** \param[out]     ResponseData        Data returned by the function.
 ** \param[in,out]  ResponseDataLength  In: Max number of bytes available in ResponseData Out:
 **                                     Actual number.
 **
 ** \returns  Error value.
 **
 ** \retval  E_OK                           Start operation successfully performed. Key update
 **                                         operations are now allowed
 ** \retval  E_NOT_OK                       Start operation not accepted
 ** \retval  KEYM_E_PARAMETER_MISMATCH      Parameter do not match with expected value
 ** \retval  KEYM_E_KEY_CERT_SIZE_MISMATCH  Parameter size doesn't match
 **
 ** \ServiceID{::KEYM_SID_START}
 ** \Synchronicity{Synchronous}
 ** \Reentrancy{Non Reentrant}
 **/
#if (KEYM_API_ENABLED_START == STD_ON)
extern FUNC(Std_ReturnType, KEYM_CODE) KeyM_Start
(
          KeyM_StartType                     StartType,
  P2CONST(uint8,  AUTOMATIC, KEYM_APPL_DATA) RequestData,
          uint16                             RequestDataLength,
    P2VAR(uint8,  AUTOMATIC, KEYM_APPL_DATA) ResponseData,
    P2VAR(uint16, AUTOMATIC, KEYM_APPL_DATA) ResponseDataLength
);
#endif /* #if (KEYM_API_ENABLED_START == STD_ON) */

/** \brief  This function is used to prepare a key update operation. The main intent is to
 **         provide information for the key operation to the key server. Other operations may
 **         start the negotiation for a common secret that is used further to derive key
 **         material. This function is only available if KeyMCryptoKeyPrepareFunctionEnabled is
 **         set to TRUE.
 **
 ** \param[in]      RequestData         Information that comes along with the request.
 ** \param[in]      RequestDataLength   Length of data in the RequestData array.
 ** \param[out]     ResponseData        Data returned by the function.
 ** \param[in,out]  ResponseDataLength  In: Max number of bytes available in ResponseData Out:
 **                                     Actual number of bytes.
 **
 ** \returns  Error value.
 **
 ** \retval  E_OK                           Service has been accepted and will be processed
 **                                         internally. Results will be provided through a
 **                                         callback
 ** \retval  E_NOT_OK                       Service not accepted due to an internal error
 ** \retval  KEYM_E_PARAMETER_MISMATCH      Parameter do not match with expected value
 ** \retval  KEYM_E_KEY_CERT_SIZE_MISMATCH  Parameter size doesn't match
 **
 ** \ServiceID{::KEYM_SID_PREPARE}
 ** \Synchronicity{Synchronous}
 ** \Reentrancy{Non Reentrant}
 **/
#if (KEYM_API_ENABLED_PREPARE == STD_ON)
extern FUNC(Std_ReturnType, KEYM_CODE) KeyM_Prepare
(
  P2CONST(uint8,  AUTOMATIC, KEYM_APPL_DATA) RequestData,
          uint16                             RequestDataLength,
    P2VAR(uint8,  AUTOMATIC, KEYM_APPL_DATA) ResponseData,
    P2VAR(uint16, AUTOMATIC, KEYM_APPL_DATA) ResponseDataLength
);
#endif /* #if (KEYM_API_ENABLED_PREPARE == STD_ON) */

/** \brief  This function is used to initiate the key generation or update process.
 **
 ** \param[in]   KeyNamePtr           Pointer to an array that defines the name of the key to
 **                                   be updated.
 ** \param[in]   KeyNameLength        Specifies the number of bytes in keyName. The value 0
 **                                   indicates that no keyName is provided within this
 **                                   function.
 ** \param[in]   RequestDataPtr       Information that comes along with the request.
 ** \param[in]   RequestDataLength    Length of data in the RequestData array.
 ** \param[out]  ResultDataPtr        Pointer to a data buffer used by the function to store
 **                                   results.
 ** \param[in]   ResultDataMaxLength  Max number of bytes available in ResultDataPtr.
 **
 ** \returns  Error value.
 **
 ** \retval  E_OK                           Service has been accepted and will be processed
 **                                         internally. Results will be provided through a
 **                                         callback
 ** \retval  E_NOT_OK                       Service not accepted due to an internal error
 ** \retval  E_BUSY                         Service could not be accepted because another
 **                                         operation is already ongoing. Try next time
 ** \retval  KEYM_E_PARAMETER_MISMATCH      Parameter do not match with expected value
 ** \retval  KEYM_E_KEY_CERT_SIZE_MISMATCH  Parameter size doesn't match
 **
 ** \ServiceID{::KEYM_SID_UPDATE}
 ** \Synchronicity{Asynchronous}
 ** \Reentrancy{Non Reentrant}
 **/
#if (KEYM_API_ENABLED_UPDATE == STD_ON)
extern FUNC(Std_ReturnType, KEYM_CODE) KeyM_Update
(
  P2CONST(uint8, AUTOMATIC, KEYM_APPL_DATA) KeyNamePtr,
          uint16                            KeyNameLength,
  P2CONST(uint8, AUTOMATIC, KEYM_APPL_DATA) RequestDataPtr,
          uint16                            RequestDataLength,
    P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) ResultDataPtr,
          uint16                            ResultDataMaxLength
);
#endif /* #if (KEYM_API_ENABLED_UPDATE == STD_ON) */

/** \brief  The function is used to finalize key update operations. It is typically used in
 **         conjunction with the KeyM_Start operation and returns the key operation into the
 **         idle mode. Further key prepare or update operations are not accepted until a new
 **         KeyM_Start operation has been initialized. This function is only available if
 **         KeyMCryptoKeyStartFinalizeFunctionEnabled is set to TRUE. In addition, updated key
 **         material will be persisted and set into valid state (calling Csm_KeySetValid).
 **
 ** \param[in]      RequestDataPtr         Information that comes along with the request.
 ** \param[in]      RequestDataLength      Length of data in the RequestData array.
 ** \param[out]     ResponseDataPtr        Data returned by the function.
 ** \param[in,out]  ResponseMaxDataLength  In: Max number of bytes available in ResponseData
 **                                        Out: Actual number of bytes in ResponseData or left
 **                                        untouched if service runs in asynchronous mode and
 **                                        function returns KEYM_E_OK.
 **
 ** \returns  Error value.
 **
 ** \retval  E_OK                           Operation has been accepted and will be processed
 **                                         internally. Results will be provided through a
 **                                         callback
 ** \retval  E_NOT_OK                       Operation not accepted due to an internal error
 ** \retval  KEYM_E_BUSY                    Validation cannot be performed yet. KeyM is
 **                                         currently busy with other jobs
 ** \retval  KEYM_E_PARAMETER_MISMATCH      Parameter do not match with expected value
 ** \retval  KEYM_E_KEY_CERT_SIZE_MISMATCH  Parameter size doesn't match
 **
 ** \ServiceID{::KEYM_SID_FINALIZE}
 ** \Synchronicity{Asynchronous}
 ** \Reentrancy{Non Reentrant}
 **/
#if (KEYM_API_ENABLED_FINALIZE == STD_ON)
extern FUNC(Std_ReturnType, KEYM_CODE) KeyM_Finalize
(
  P2CONST(uint8,  AUTOMATIC, KEYM_APPL_DATA) RequestDataPtr,
          uint16                             RequestDataLength,
    P2VAR(uint8,  AUTOMATIC, KEYM_APPL_DATA) ResponseDataPtr,
    P2VAR(uint16, AUTOMATIC, KEYM_APPL_DATA) ResponseMaxDataLength
);
#endif /* #if (KEYM_API_ENABLED_FINALIZE == STD_ON) */

/** \brief  The key server requests to verify the provided keys. The key manager performs
 **         operation on the assigned job and returns the result to the key server who verifies
 **         if the results was provided with this key as expected. This function is only
 **         available if KeyMCryptoKeyVerifyFunctionEnabled is set to TRUE.
 **
 ** \param[in]      KeyNamePtr          Points to an array that defines the name of the key to
 **                                     be updated.
 ** \param[in]      KeyNameLength       Specifies the number of bytes in KeyNamePtr. The value
 **                                     0 indicates that no KeyNamePtr is provided within this
 **                                     function.
 ** \param[in]      RequestData         Information that comes along with the request.
 ** \param[in]      RequestDataLength   Length of data in the RequestData array.
 ** \param[out]     ResponseData        Data returned by the function.
 ** \param[in,out]  ResponseDataLength  In: Max number of bytes available in ResponseData Out:
 **                                     Actual number of bytes in ResponseData or left
 **                                     untouched if service runs in asynchronous mode and
 **                                     function returns KEYM_E_PENDING.
 **
 ** \returns  Error value.
 **
 ** \retval  KEYM_E_PENDING                 Operation runs in asynchronous mode, has been
 **                                         accepted and will be processed internally. Results
 **                                         will be provided through callback
 ** \retval  E_OK                           Operation was successfully performed. Result
 **                                         information are available
 ** \retval  E_NOT_OK                       Operation not accepted due to an internal error
 ** \retval  KEYM_E_BUSY                    Validation cannot be performed yet. KeyM is
 **                                         currently busy with other jobs (for asynchronous
 **                                         mode)
 ** \retval  KEYM_E_PARAMETER_MISMATCH      Parameter do not match with expected value
 ** \retval  KEYM_E_KEY_CERT_SIZE_MISMATCH  Parameter size doesn't match
 ** \retval  KEYM_E_KEY_CERT_INVALID        Key operation cannot be performed because the key
 **                                         name is invalid
 ** \retval  KEYM_E_KEY_CERT_EMPTY          The key for this slot has not been set
 **
 ** \ServiceID{::KEYM_SID_VERIFY}
 ** \Synchronicity{Synchronous Synchronous/Asynchronous}
 ** \Reentrancy{Non Reentrant}
 **/
#if (KEYM_API_ENABLED_VERIFY == STD_ON)
extern FUNC(Std_ReturnType, KEYM_CODE) KeyM_Verify
(
  P2CONST(uint8,  AUTOMATIC, KEYM_APPL_DATA) KeyNamePtr,
          uint16                             KeyNameLength,
  P2CONST(uint8,  AUTOMATIC, KEYM_APPL_DATA) RequestData,
          uint16                             RequestDataLength,
    P2VAR(uint8,  AUTOMATIC, KEYM_APPL_DATA) ResponseData,
    P2VAR(uint16, AUTOMATIC, KEYM_APPL_DATA) ResponseDataLength
);
#endif /* #if (KEYM_API_ENABLED_VERIFY == STD_ON) */

/** \brief  The key server requests an operation from the key client. The type of operation is
 **         specified in the first parameter KeyM_ServiceCertificateType. Certificate operation
 **         requests are operated through this function. This function is only available if the
 **         configuration parameter KeyMServiceCertificateFunctionEnabled is set to TRUE.
 **
 ** \param[in]   Service             Provides the type of service the key manager has to
 **                                  perform.
 ** \param[in]   CertNamePtr         Points to an array that defines the name of the
 **                                  certificate to be updated.
 ** \param[in]   CertNameLength      Specifies the number of bytes in CertNamePtr. The value 0
 **                                  indicates that no CertNamePtr is provided within this
 **                                  function.
 ** \param[in]   RequestData         Information that comes along with the request.
 ** \param[in]   RequestDataLength   Length of data in the RequestData array.
 ** \param[out]  ResponseData        Data returned by the function.
 ** \param[in]   ResponseDataLength  Max number of bytes available in ResponseDataPtr.
 **
 ** \returns  Error value.
 **
 ** \retval  E_OK                           Service data operation successfully accepted
 ** \retval  E_NOT_OK                       Operation not accepted due to an internal error
 ** \retval  KEYM_E_PARAMETER_MISMATCH      Parameter do not match with expected value
 ** \retval  KEYM_E_KEY_CERT_SIZE_MISMATCH  Parameter size doesn't match
 **
 ** \ServiceID{::KEYM_SID_SERVICECERTIFICATE}
 ** \Synchronicity{Asynchronous}
 ** \Reentrancy{Non Reentrant}
 **/
#if (KEYM_API_ENABLED_SERVICECERTIFICATE == STD_ON)
extern FUNC(Std_ReturnType, KEYM_CODE) KeyM_ServiceCertificate
(
          KeyM_ServiceCertificateType       Service,
  P2CONST(uint8, AUTOMATIC, KEYM_APPL_DATA) CertNamePtr,
          uint16                            CertNameLength,
  P2CONST(uint8, AUTOMATIC, KEYM_APPL_DATA) RequestData,
          uint16                            RequestDataLength,
    P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) ResponseData,
          uint16                            ResponseDataLength
);
#endif /* #if (KEYM_API_ENABLED_SERVICECERTIFICATE == STD_ON) */

/** \brief  This function provides the certificate data to the key management module to
 **         temporarily store the certificate.
 **
 ** \param[in]  CertId              Holds the identifier of the certificate.
 ** \param[in]  CertificateDataPtr  Pointer to a structure that provides the certificate data.
 **
 ** \returns  Error value.
 **
 ** \retval  E_OK                           Certificate accepted
 ** \retval  E_NOT_OK                       Certificate could not be set
 ** \retval  KEYM_E_PARAMETER_MISMATCH      Parameter do not match with expected value
 ** \retval  KEYM_E_KEY_CERT_SIZE_MISMATCH  Parameter size doesn't match
 **
 ** \ServiceID{::KEYM_SID_SETCERTIFICATE}
 ** \Synchronicity{Synchronous}
 ** \Reentrancy{Non Reentrant}
 **/
extern FUNC(Std_ReturnType, KEYM_CODE) KeyM_SetCertificate
(
          KeyM_CertificateIdType                        CertId,
  P2CONST(KeyM_CertDataType, AUTOMATIC, KEYM_APPL_DATA) CertificateDataPtr
);

/** \brief  This function provides the certificate data.
 **
 ** \param[in]      CertId              Holds the identifier of the certificate.
 ** \param[in,out]  CertificateDataPtr  Provides a pointer to a certificate data structure. The
 **                                     buffer located by the pointer in the structure shall be
 **                                     provided by the caller of this function. The length
 **                                     information indicates the maximum length of the buffer
 **                                     when the function is called. If E_OK is returned, the
 **                                     length information indicates the actual length of the
 **                                     certificate data in the buffer.
 **
 ** \returns  Error value.
 **
 ** \retval  E_OK                           Certificate data available and provided.
 ** \retval  E_NOT_OK                       Operation not accepted due to an internal error
 ** \retval  KEYM_E_PARAMETER_MISMATCH      Certificate ID invalid
 ** \retval  KEYM_E_KEY_CERT_SIZE_MISMATCH  Provided buffer for the certificate too small
 ** \retval  KEYM_E_KEY_CERT_EMPTY          No certificate data available, the certificate
 **                                         slot is empty
 ** \retval  KEYM_E_KEY_CERT_READ_FAIL      Certificate cannot be provided, access denied
 **
 ** \ServiceID{::KEYM_SID_GETCERTIFICATE}
 ** \Synchronicity{Synchronous}
 ** \Reentrancy{Non Reentrant}
 **/
extern FUNC(Std_ReturnType, KEYM_CODE) KeyM_GetCertificate
(
        KeyM_CertificateIdType                        CertId,
  P2VAR(KeyM_CertDataType, AUTOMATIC, KEYM_APPL_DATA) CertificateDataPtr
);

/** \brief  This function verifies two certificates that are stored and parsed internally
 **         against each other. The certificate referenced with CertId was signed by the
 **         certificate referenced with certUpperId. Only these two certificates are validated
 **         against each other.
 **
 ** \param[in]  CertId       Holds the identifier of the lower certificate in the chain.
 ** \param[in]  CertUpperId  Holds the identifier of the upper certificate in the chain.
 **
 ** \returns  Error value.
 **
 ** \retval  E_OK                                Certificate verification request accepted.
 **                                              Operation will be performed in the background
 **                                              and response is given through a callback
 ** \retval  E_NOT_OK                            Operation not accepted due to an internal
 **                                              error
 ** \retval  KEYM_E_BUSY                         Validation cannot be performed yet. KeyM is
 **                                              currently busy with other jobs
 ** \retval  KEYM_E_PARAMETER_MISMATCH           Certificate ID invalid
 ** \retval  KEYM_E_KEY_CERT_EMPTY               One of the certificate slots are empty
 ** \retval  KEYM_E_CERT_INVALID_CHAIN_OF_TRUST  An upper certificate is not valid
 **
 ** \ServiceID{::KEYM_SID_VERIFYCERTIFICATES}
 ** \Synchronicity{Asynchronous}
 ** \Reentrancy{Non Reentrant}
 **/
extern FUNC(Std_ReturnType, KEYM_CODE) KeyM_VerifyCertificates
(
  KeyM_CertificateIdType CertId,
  KeyM_CertificateIdType CertUpperId
);

/** \brief  This function verifies a certificate that was previously provided with
 **         KeyM_SetCertificate() against already stored and provided certificates stored with
 **         other certificate IDs.
 **
 ** \param[in]  CertId  Holds the identifier of the certificate.
 **
 ** \returns  Error value.
 **
 ** \retval  E_OK                                Certificate verification request accepted.
 **                                              Operation will be performed in the background
 **                                              and response is given through a callback
 ** \retval  E_NOT_OK                            Operation not accepted due to an internal
 **                                              error
 ** \retval  KEYM_E_BUSY                         Validation cannot be performed yet. KeyM is
 **                                              currently busy with other jobs
 ** \retval  KEYM_E_PARAMETER_MISMATCH           Certificate ID invalid
 ** \retval  KEYM_E_KEY_CERT_EMPTY               One of the certificate slots are empty
 ** \retval  KEYM_E_CERT_INVALID_CHAIN_OF_TRUST  An upper certificate is not valid
 **
 ** \ServiceID{::KEYM_SID_VERIFYCERTIFICATE}
 ** \Synchronicity{Asynchronous}
 ** \Reentrancy{Non Reentrant}
 **/
extern FUNC(Std_ReturnType, KEYM_CODE) KeyM_VerifyCertificate
(
  KeyM_CertificateIdType CertId
);

/** \brief  This function performs a certificate verification against a list of certificates.
 **         It is a pre-requisite that the certificate that shall be checked has already been
 **         written with KeyM_SetCertificate() and that the root certificate is either in the
 **         list or is already assigned to one of the other certificates.
 **
 ** \param[in]  CertId                Holds the identifier of the last certificate in the
 **                                   chain.
 ** \param[in]  certChainData         This is a pointer to an array of certificates sorted
 **                                   according to the order in the PKI.
 ** \param[in]  NumberOfCertificates  Defines the number of certificates stored in the
 **                                   CertChainData array.
 **
 ** \returns  Error value.
 **
 ** \retval  E_OK                                Certificate verification request accepted.
 **                                              Operation will be performed in the background
 **                                              and response is given through a callback
 ** \retval  E_NOT_OK                            Operation not accepted due to an internal
 **                                              error
 ** \retval  KEYM_E_BUSY                         Validation cannot be performed yet. KeyM is
 **                                              currently busy with other jobs
 ** \retval  KEYM_E_PARAMETER_MISMATCH           Certificate ID invalid
 ** \retval  KEYM_E_KEY_CERT_EMPTY               One of the certificate slots are empty
 ** \retval  KEYM_E_CERT_INVALID_CHAIN_OF_TRUST  An upper certificate is not valid
 **
 ** \ServiceID{::KEYM_SID_VERIFYCERTIFICATECHAIN}
 ** \Synchronicity{Asynchronous}
 ** \Reentrancy{Non Reentrant}
 **/
extern FUNC(Std_ReturnType, KEYM_CODE) KeyM_VerifyCertificateChain
(
             KeyM_CertificateIdType                        CertId,
  CONSTP2VAR(KeyM_CertDataType, AUTOMATIC, KEYM_APPL_DATA) certChainData,
             uint8                                         NumberOfCertificates
);

/** \brief  Provides the content of a specific certificate element. The certificate
 **         configuration defines how the certificate submodule can find the element, e.g. by
 **         providing the object identifier (OID). This function is used to retrieve this
 **         information if only one element is assigned to the respective OID.
 **
 ** \param[in]      CertId                 Holds the identifier of the last certificate in the
 **                                        chain.
 ** \param[in]      CertElementId          Specifies the ElementId where the data shall be read
 **                                        from.
 ** \param[out]     CertElementData        Pointer to a data buffer allocated by the caller of
 **                                        this function. If available, the function returns
 **                                        E_OK and copies the data into this buffer.
 ** \param[in,out]  CertElementDataLength  In: Pointer to a value that contains the maximum
 **                                        data length of the CertElementData buffer. Out: The
 **                                        data length will be overwritten with the actual
 **                                        length of data placed to the buffer if the function
 **                                        returns E_OK. Otherwise, the it will be overwritten
 **                                        with the value zero.
 **
 ** \returns  Error value.
 **
 ** \retval  E_OK                           Element found and data provided in the buffer
 ** \retval  E_NOT_OK                       Element data not found
 ** \retval  KEYM_E_PARAMETER_MISMATCH      Certificate ID or certificate element ID invalid
 ** \retval  KEYM_E_KEY_CERT_SIZE_MISMATCH  Provided buffer for the certificate element too
 **                                         small
 ** \retval  KEYM_E_KEY_CERT_EMPTY          No certificate data available, the certificate
 **                                         slot is empty
 ** \retval  KEYM_E_KEY_CERT_INVALID        The certificate is not valid or has not yet been
 **                                         verified
 **
 ** \ServiceID{::KEYM_SID_CERTELEMENTGET}
 ** \Synchronicity{Synchronous}
 ** \Reentrancy{Non Reentrant}
 **/
extern FUNC(Std_ReturnType, KEYM_CODE) KeyM_CertElementGet
(
        KeyM_CertificateIdType             CertId,
        KeyM_CertElementIdType             CertElementId,
  P2VAR(uint8,  AUTOMATIC, KEYM_APPL_DATA) CertElementData,
  P2VAR(uint32, AUTOMATIC, KEYM_APPL_DATA) CertElementDataLength
);

/** \brief  This function is used to initialize the interative extraction of a certificate data
 **         element. It always retrieves the top element from the configured certificate
 **         element and initializes the structure KeyM_CertElementIterator so that consecutive
 **         data from this element can be read with KeyM_CertElementGetNext().
 **
 ** \param[in]      CertId                 Holds the identifier of the last certificate in the
 **                                        chain.
 ** \param[in]      CertElementId          Specifies the CertElementId where the data shall be
 **                                        read from.
 ** \param[in,out]  CertElementIterator    Pointer to a structure that is allocated and
 **                                        maintained by the caller. It shall not be destroyed
 **                                        or altered by the application until all elements
 **                                        have been retrieved through
 **                                        KeyM_CertElementGetNext().
 ** \param[out]     CertElementData        Pointer to a data buffer allocated by the caller of
 **                                        this function. If available, the function returns
 **                                        E_OK and copies the data into this buffer.
 ** \param[in,out]  CertElementDataLength  In: Pointer to a value that contains the maximum
 **                                        data length of the CertElementData buffer. Out: The
 **                                        data length will be overwritten with the actual
 **                                        length of data placed to the buffer if the function
 **                                        returns E_OK.
 **
 ** \returns  Error value.
 **
 ** \retval  E_OK                           Element found and data provided in the buffer. The
 **                                         certElementIterator has been initialized
 **                                         accordingly
 ** \retval  E_NOT_OK                       Element data not found. CertElementIterator cannot
 **                                         be used for further calls
 ** \retval  KEYM_E_PARAMETER_MISMATCH      Certificate ID or certificate element ID invalid
 ** \retval  KEYM_E_KEY_CERT_SIZE_MISMATCH  Provided buffer for the certificate element too
 **                                         small
 ** \retval  KEYM_E_KEY_CERT_EMPTY          No certificate data available, the certificate is
 **                                         empty
 ** \retval  KEYM_E_CERT_INVALID            Certificate is not valid or not verified
 **                                         successfully
 **
 ** \ServiceID{::KEYM_SID_CERTELEMENTGETFIRST}
 ** \Synchronicity{Synchronous}
 ** \Reentrancy{Reentrant Reentrant for one iterator.}
 **/
extern FUNC(Std_ReturnType, KEYM_CODE) KeyM_CertElementGetFirst
(
        KeyM_CertificateIdType                                   CertId,
        KeyM_CertElementIdType                                   CertElementId,
  P2VAR(KeyM_CertElementIteratorType, AUTOMATIC, KEYM_APPL_DATA) CertElementIterator,
  P2VAR(uint8,                        AUTOMATIC, KEYM_APPL_DATA) CertElementData,
  P2VAR(uint32,                       AUTOMATIC, KEYM_APPL_DATA) CertElementDataLength
);

/** \brief  This function provides further data from a certificate element, e.g. if a set of
 **         data are located in one certificate element that shall be read one after another.
 **         This function can only be called if the function KeyM_CertElementGetFirst() has
 **         been called once before.
 **
 ** \param[in,out]  CertElementIterator    Pointer to a structure that is allocated by the
 **                                        caller and used by the function. It shall not be
 **                                        destroyed or altered by the application until all
 **                                        elements have been read from the list.
 ** \param[out]     CertElementData        Pointer to a data buffer allocated by the caller of
 **                                        this function. If available, the function returns
 **                                        E_OK and copies the data into this buffer.
 ** \param[in,out]  CertElementDataLength  In: Pointer to a value that contains the maximum
 **                                        data length of the CertElementData buffer. Out: The
 **                                        data length will be overwritten with the actual
 **                                        length of data placed to the buffer if the function
 **                                        returns E_OK.
 **
 ** \returns  Error value.
 **
 ** \retval  E_OK                           Element found and data provided in the buffer. The
 **                                         CertElementIterator has been initialized
 **                                         accordingly
 ** \retval  E_NOT_OK                       Element data not found. CertElementIterator cannot
 **                                         be used for further calls
 ** \retval  KEYM_E_PARAMETER_MISMATCH      Certificate ID or certificate element ID invalid
 ** \retval  KEYM_E_KEY_CERT_SIZE_MISMATCH  Provided buffer for the certificate element too
 **                                         small
 ** \retval  KEYM_E_KEY_CERT_EMPTY          No certificate data available, the certificate is
 **                                         empty
 ** \retval  KEYM_E_CERT_INVALID            Certificate is not valid or not verified
 **                                         successfully
 **
 ** \ServiceID{::KEYM_SID_CERTELEMENTGETNEXT}
 ** \Synchronicity{Synchronous}
 ** \Reentrancy{Reentrant Reentrant for one iterator.}
 **/
extern FUNC(Std_ReturnType, KEYM_CODE) KeyM_CertElementGetNext
(
  P2VAR(KeyM_CertElementIteratorType, AUTOMATIC, KEYM_APPL_DATA) CertElementIterator,
  P2VAR(uint8,                        AUTOMATIC, KEYM_APPL_DATA) CertElementData,
  P2VAR(uint32,                       AUTOMATIC, KEYM_APPL_DATA) CertElementDataLength
);

/** \brief  This function provides the status of a certificate.
 **
 ** \param[in]   CertId  Holds the identifier of the certificate.
 ** \param[out]  Status  Provides the status of the certificate.
 **
 ** \returns  Error value.
 **
 ** \ServiceID{::KEYM_SID_CERTGETSTATUS}
 ** \Synchronicity{Synchronous}
 ** \Reentrancy{Non Reentrant}
 **/
extern FUNC(Std_ReturnType, KEYM_CODE) KeyM_CertGetStatus
(
        KeyM_CertificateIdType                                 CertId,
  P2VAR(KeyM_CertificateStatusType, AUTOMATIC, KEYM_APPL_DATA) Status
);

/** \brief  Function is called periodically according the specified time interval.
 **
 ** \ServiceID{::KEYM_SID_MAINFUNCTION}
 ** \Synchronicity{Synchronous}
 ** \Reentrancy{Non Reentrant}
 **/
#ifndef RTE_SCHM_SWCBSW_KEYM_MAINFUNCTION
#define RTE_SCHM_SWCBSW_KEYM_MAINFUNCTION
extern FUNC(void, KEYM_CODE) KeyM_MainFunction
(
  void
);
#endif /* #ifndef RTE_SCHM_SWCBSW_KEYM_MAINFUNCTION */

/** \brief  Function is called from a pre-emptive operating system when no other task operation
 **         is needed. Can be used for calling time consuming synchronous functions such as
 **         KeyM_KH_Update().
 **
 ** \ServiceID{::KEYM_SID_MAINBACKGROUNDFUNCTION}
 ** \Synchronicity{Synchronous}
 ** \Reentrancy{Non Reentrant}
 **/
#ifndef RTE_SCHM_SWCBSW_KEYM_MAINBACKGROUNDFUNCTION
#define RTE_SCHM_SWCBSW_KEYM_MAINBACKGROUNDFUNCTION
extern FUNC(void, KEYM_CODE) KeyM_MainBackgroundFunction
(
  void
);
#endif /* #ifndef RTE_SCHM_SWCBSW_KEYM_MAINBACKGROUNDFUNCTION */

#define KEYM_STOP_SEC_CODE
#include <KeyM_MemMap.h>

/*================================================================================================*/

#endif /* #ifndef KEYM_H */

/*==================[end of file]=================================================================*/

