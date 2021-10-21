/**
 * \file
 *
 * \brief AUTOSAR Crypto
 *
 * This file contains the implementation of the AUTOSAR
 * module Crypto.
 *
 * \version 1.7.42
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==[includes directives]=========================================================================*/

#ifndef CRYPTO_XVIX_XAIX_AL_CIPHER_H
#define CRYPTO_XVIX_XAIX_AL_CIPHER_H
#include <Std_Types.h>
#include <Crypto_xVIx_xAIx_AL_Cfg.h>

/*==[Macro definitions]===========================================================================*/

#if (defined CRYPTO_XVIX_XAIX_AES_KEYEXPDLENMAX_FAST)
#error CRYPTO_XVIX_XAIX_AES_KEYEXPDLENMAX_FAST already defined
#endif
/** \brief Defines the length in words of the expanded key */
#define  CRYPTO_XVIX_XAIX_AES_KEYEXPDLENMAX_FAST  60U

/*---Crypto_xVIx_xAIx_AES_KeyType--------------------------------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_AES_KEY_128)
#error CRYPTO_XVIX_XAIX_AES_KEY_128 already defined
#endif
/** \brief Crypto_xVIx_xAIx_AES_KeyType 128 bits key */
#define CRYPTO_XVIX_XAIX_AES_KEY_128        16U

#if (defined CRYPTO_XVIX_XAIX_AES_KEY_192)
#error CRYPTO_XVIX_XAIX_AES_KEY_192 already defined
#endif
/** \brief Crypto_xVIx_xAIx_AES_KeyType 192 bits key */
#define CRYPTO_XVIX_XAIX_AES_KEY_192        24U

#if (defined CRYPTO_XVIX_XAIX_AES_KEY_256)
#error CRYPTO_XVIX_XAIX_AES_KEY_256 already defined
#endif
/** \brief Crypto_xVIx_xAIx_AES_KeyType 256 bits key */
#define CRYPTO_XVIX_XAIX_AES_KEY_256        32U

/*==[Type declarations]===========================================================================*/

/*==[Declaration of constants with external linkage]==============================================*/

/*==[Declaration of variables with external linkage]==============================================*/

/*==[Declaration of functions with external linkage]==============================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*--AES-HELPER------------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_AES_DECRYPT_ENABLED == STD_ON)

/** \brief Calls the Crypto primitives Decrypt AES Start function
 * This is the Helper function which makes the AES ECB primitive available for other AES modes It
 * is called by other AES modes to perform the AES ECB as they need.
 *
 * \pre  The particular mode of the AES would call the this function and provide the needed
 * parameters.
 * \post   This function would have called the AES ECB start function and the processing would be
 * done according to the passed parameters (Sync/Async)
 * \param[in] ObjId                     Object Id of the Driver Object to which the Job belongs
 * \param[in] Crypto_Generic_Callback  Function pointer of the Callback function that would be
 * called when the Helper has finished the processing.
 * \param[in] Job                       Pointer to the Job which is being currently performed.
 * \param[in] IsSync                    The processing type that should be used(Sync/Async)
 * \param[in] KeyPtr                    Pointer to an object of type Crypto_xVIx_xAIx_SymKeyType , which
 * contains the key for the primitive.
 * \return
 * \retval E_OK                         The computation was successful
 * \retval E_NOT_OK                     The computation failed
 *
 * \Reentrancy{Non reentrant}
 * \Synchronicity{depends upon the IsSync param from the parent Primitive.}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AES_Helper_Decrypt_Start
(
  CONST(uint32,CRYPTO_xVIx_xAIx_CONST)                           ObjId,
  P2FUNC(void, CRYPTO_xVIx_xAIx_CODE, Crypto_Generic_Callback)
  (
    P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
    uint8                                                        Error
  ),
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Job,
  Crypto_ProcessingType IsSync,
  P2CONST(Crypto_xVIx_xAIx_SymKeyType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) KeyPtr
);

/** \brief Calls the Crypto primitives Decrypt AES  Update function
 * This is the Helper function which makes the AES ECB primitive available for other AES modes It
 * is called by other AES modes to perform the AES ECB as they need.
 *
 * \pre  The particular mode of the AES would call the this function and provide the needed
 * parameters.
 * \post   This function would have called the AES ECB Update function and the processing would be
 * done according to the passed parameters (Sync/Async)
 * \param[in] IsSync                    Processing type for the primitive
 * \param[in] CipherTextPtr             buffer containing the cipher text
 * \param[in] PlainTextLengthPtr        length of the cipher text
 * \param[in] PlainTextPtr              Buffer where the decrypted plain text will be copied.
 * \param[in,out] PlainTextLengthPtr    pointer to uint32 which contains the lenth of the buffer
 * where the plain text will be copied. After return it contains the length of actual text copied.
 * \return void (function is called from primary primitive only with valid parameters, hence can not
 *               fail.)
 * \Reentrancy{Non reentrant}
 * \Synchronicity{depends upon the IsSync param from the parent Primitive.}
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AES_Helper_Decrypt_Update
(
  Crypto_ProcessingType                       IsSync,
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CipherTextPtr,
  uint32                                                CipherTextLength,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   PlainTextPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  PlainTextLengthPtr
);

/** \brief Calls the Crypto primitives Decrypt AES Finish function
 * This is the Helper function which makes the AES ECB primitive available for other AES modes It
 * is called by other AES modes to perform the AES ECB as they need.
 *
 * \pre  The particular mode of the AES would call the this function and provide the needed
 * parameters.
 * \post   This function would have called the AES ECB Finish function and the processing would be
 * done according to the passed parameters (Sync/Async)
 * \param[in] IsSync                    Processing type for the primitive
 *
 * \return void (function is called from primary primitive only with valid parameters, hence can not
 *               fail.)
 *
 * \Reentrancy{Non reentrant}
 * \Synchronicity{depends upon the IsSync param from the parent Primitive.}
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AES_Helper_Decrypt_Finish
(
  Crypto_ProcessingType IsSync
);

/** \brief Calls the Crypto primitives Main AES Decrypt function
 *
 * \pre
 * \post   Performes the operations and is responsible for the state transitions.
 * \return void
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AES_Helper_Decrypt_MainFunction
(
  void
);

#endif /* #if (CRYPTO_XVIX_XAIX_AES_DECRYPT_ENABLED == STD_ON) */

#if (CRYPTO_XVIX_XAIX_AES_ENCRYPT_ENABLED == STD_ON)

/** \brief Calls the Crypto primitives Encrypt AES Start function
 * This is the Helper function which makes the AES ECB primitive available for other AES modes It
 * is called by other AES modes to perform the AES ECB as they need.
 *
 * \pre  The particular mode of the AES would call the this function and provide the needed
 * parameters.
 * \post   This function would have called the AES ECB start function and the processing would be
 * done according to the passed parameters (Sync/Async)
 *
 * \param[in] ObjId                     Object Id of the Driver Object to which the Job belongs
 * \param[in] Crypto_Generic_Callback  Function pointer of the Callback function that would be
 * called when the Helper has finished the processing.
 * \param[in] Job                       Pointer to the Job which is being currently performed.
 * \param[in] IsSync                    The processing type that should be used(Sync/Async)
 * \param[in] KeyPtr                    Pointer to an object of type Crypto_xVIx_xAIx_SymKeyType , which
 * contains
 * the key for the primitive.
 *
 * \return
 * \retval E_OK                         The computation was successful
 * \retval E_NOT_OK                     The computation failed
 *
 * \Reentrancy{Non reentrant}
 * \Synchronicity{depends upon the IsSync param from the parent Primitive.}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AES_Helper_Encrypt_Start
(
  CONST(uint32,CRYPTO_xVIx_xAIx_CONST) ObjId,
  P2FUNC(void, CRYPTO_xVIx_xAIx_CODE, Crypto_Generic_Callback)
  (
    P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
    uint8 Error
  ),
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  Crypto_ProcessingType IsSync,
  P2CONST(Crypto_xVIx_xAIx_SymKeyType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) KeyPtr
);

/** \brief Calls the Crypto primitives Encrypt AES  Update function
 * This is the Helper function which makes the AES ECB primitive available for other AES modes It
 * is called by other AES modes to perform the AES ECB as they need
 *
 * \pre  The particular mode of the AES would call the this function and provide the needed
 * parameters.
 * \post   This function would have called the AES ECB Update function and the processing would be
 * done according to the passed parameters (Sync/Async)
 *
 * \param[in] IsSync                    Processing type for the primitive
 * \param[in] CipherTextPtr             buffer containing the cipher text
 * \param[in] PlainTextLengthPtr        length of the cipher text
 * \param[in] PlainTextPtr              Buffer where the decrypted plain text will be copied.
 * \param[in,out] PlainTextLengthPtr    pointer to uint32 which contains the lenth of the buffer
 * where the plain text will be copied. After return it contains the length of actual text copied.
 *
 * \return void (function is called from primary primitive only with valid parameters, hence can not
 *               fail.)
 * \Reentrancy{Non reentrant}
 * \Synchronicity{depends upon the IsSync param from the parent Primitive.}
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AES_Helper_Encrypt_Update
(
  Crypto_ProcessingType IsSync,
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CipherTextPtr,
  uint32 CipherTextLength,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) PlainTextPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) PlainTextLengthPtr
);

/** \brief Calls the Crypto primitives Encrypt AES Finish function
 * This is the Helper function which makes the AES ECB primitive available for other AES modes It
 * is
 * called by other AES modes to perform the AES ECB as they need
 *
 * \pre  The particular mode of the AES would call the this function and provide the needed
 * parameters.
 * \post   This function would have called the AES ECB Finish function and the processing would be
 * done according to the passed parameters (Sync/Async)
 *
 * \param[in] IsSync                    Processing type for the primitive
 *
 * \return void (function is called from primary primitive only with valid parameters, hence can not
 *               fail.)
 *
 * \Reentrancy{Non reentrant}
 * \Synchronicity{depends upon the IsSync param from the parent Primitive.}
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AES_Helper_Encrypt_Finish
(
  Crypto_ProcessingType IsSync
);

/** \brief Calls the Crypto primitives Encrypt AES SyncSingleCall Helper function
 *
 *  This function performs an AES ECB encryption in one synchronous call and is callable by
 *  an other primitive.
 *
 *  \param[in] InputPtr        Reference to the input array
 *  \param[in] InputLength     The input array length
 *  \param[out] OutputPtr      Reference to the output array
 *  \param[in] key             Reference to the key that is used in the computation
 *  \pre                       The primitives API function has returned with E_OK
 *  \post                      The primitive has finished its computation
 *
 *  \return Result of the Job processing
 *  \retval E_OK                         The computation was successful
 *  \retval E_NOT_OK                     The computation failed
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AES_Helper_Encrypt_SyncSingleCall
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) InputPtr,
  uint32 InputLength,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) OutputPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) OutputLengthPtr,
  P2CONST(Crypto_xVIx_xAIx_SymKeyType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) KeyPtr
);

/** \brief Calls the Crypto primitives Main AES Encrypt function
 *
 * \pre
 * \post   Performes the operations and is responsible for the state transitions.
 * \return void
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AES_Helper_Encrypt_MainFunction
(
  void
);

/**
 * \brief Precalulates the expanded AES key
 *
 * \param[in]  SzeKey         Key size
 * \param[in]  SzeTurn        Turn size
 * \param[in]  KeySeed        Key seed
 * \param[out] KeyExpdFastPtr Pointer to the memory location with size of 240 bytes where the
 *                            expanded AES key is stored.
 * \return void
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_AESPreCalculateKeySchedule
(
  uint8                                     SzeKey,
  uint32                                    SzeTurn,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) KeySeed,
  P2VAR(void, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  KeyExpdFastPtr
);

/**
 * \brief AES encryption with precalculated expanded AES key
 *
 * \param[in]  InputPtr       data to be encrypted
 * \param[out] OutputPtr      encrypted data
 * \param[in]  KeyId          Id of the key holding the expanded AES key
 * \param[in]  KeyLength      Length in bytes of the original AES key
 * \param[in]  expKey         The passed expanded key read from the key element.
 *
 * \returns  Result of the function call
 * \retval   E_OK                         function has been successfully finihsed.
 * \retval   E_NOT_OK                     Request failed.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_AES_EncryptPreCalc
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)          InputPtr,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)            OutputPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)           OutputLengthPtr,
  uint32                                                         KeyLength,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)           expKey
);

#endif /* (CRYPTO_XVIX_XAIX_AES_ENCRYPT_ENABLED == STD_ON) */

/*--CRYPTO-CIPHER-AES-ECB-DECRYPT-AL-LAYER-FUNCTIONS---------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_AES_DECRYPT_ENABLED == STD_ON)

/**  \brief  Crypto ECB Decrypt Reset
 *
 *           This Function resets the Context of ECB Decrypt.
 *
 * \returns  void
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_AES_ECB_Reset
(
  void
);

/** \brief  This is the abstraction layer function for DECRYPT_AES_ECB
 *
 * This function actually performs the request for service DECRYPT with algorithm family AES and
 * algorithm mode ECB. It internally calls the Crypto_xVIx_xAIx_AES_DecryptStart function which actually
 * Performs the Start step of the processing.
 *
 * \param[in] ObjectId       Identifier of the crypto driver object that shall process the request
 * \param[in] Job            Reference to the Job that shall be processed
 *
 * \returns  Result of the Job processing
 * \retval   E_OK                         Job has been successfully processed.
 * \retval   E_NOT_OK                     Request failed.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_AES_ECB_Start
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief  This is the abstraction layer function for DECRYPT_AES_ECB
 *
 * This function actually performs the request for service DECRYPT with algorithm family AES and
 * algorithm mode ECB. It internally calls the Crypto_xVIx_xAIx_AES_DecryptUpdate function which actually
 * Performs the update step of the processing.
 *
 * \param[in] ObjectId       Identifier of the crypto driver object that shall process the request
 * \param[in] Job            Reference to the Job that shall be processed
 *
 * \returns  Result of the Job processing
 * \retval   E_OK                         Job has been successfully processed.
 * \retval   E_NOT_OK                     Request failed
 * \retval   CRYPTO_E_BUSY                Request failed, another computation is already underway.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_AES_ECB_Update
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief  This is the abstraction layer function for DECRYPT_AES_ECB
 *
 *  This function actually performs the request for service DECRYPT with algorithm family AES and
 *  algorithm mode ECB. It internally calls the Crypto_xVIx_xAIx_AES_DecryptFinish function which actually
 *  Performs the finish step of the processing.
 *
 *  \param[in] ObjectId       Identifier of the crypto driver object that shall process the request
 *  \param[in] Job            Reference to the Job that shall be processed
 *
 * \returns  Result of the Job processing
 * \retval   E_OK                         Job has been successfully processed.
 * \retval   E_NOT_OK                     Request failed
 * \retval   CRYPTO_E_BUSY                Request failed, another computation is already underway.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_AES_ECB_Finish
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief  This is the abstraction layer function for DECRYPT_AES_ECB
 *
 * This interface removes the provided Job from the queue and cancels the processing of the Job if
 * possible.
 *
 * \param[in] ObjectId       Identifier of the crypto driver object that shall process the request
 * \param[in] Job            Reference to the Job that shall be canceled.
 *
 * \returns  Result of the Job processing
 * \retval   E_OK                         Job has been successfully processed
 * \retval   E_NOT_OK                     Request failed
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_AES_ECB_Cancel
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief  This is the abstraction layer function for DECRYPT_AES_ECB
 *
 * This function actually performs the request for service DECRYPT with algorithm family AES and
 * algorithm mode ECB. It internally calls the Crypto_xVIx_xAIx_AES_DecryptMainFunction. This function than
 * performed all the state changes and function calls to perform the calculations.
 *
 * \param[in/out] result        This parameter holds the result of the last called
 * Crypto_xVIx_xAIx_AL_DECRYPT_AES_ECB_MainFunction.
 * \param[in/out] busy          This parameter holds the state of the primitive. This state is set
 * to FALSE after the primitive has performed Start or update or it has Finished.
 *
 * \returns void
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_AES_ECB_MainFunction
(
  P2VAR(Std_ReturnType,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
);
#endif /* #if (CRYPTO_XVIX_XAIX_AES_DECRYPT_ENABLED == STD_ON) */

/*--CRYPTO-CIPHER-AES-ECB-ENCRYPT-AL-LAYER-FUNCTIONS---------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_AES_ENCRYPT_ENABLED == STD_ON)

/**  \brief  Crypto ECB Encrypt Reset
 *
 *           This Function resets the Context of ECB Encrypt.
 *
 * \returns  void
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_AES_ECB_Reset
(
  void
);

/** \brief  This is the abstraction layer function for ENCRYPT_AES_ECB
 *
 * This function actually performs the request for service ENCRYPT with algorithm family AES and
 * algorithm mode ECB. It internally calls the Crypto_xVIx_xAIx_AES_EncryptStart function which actually
 * Performs the Start step of the processing.
 *
 * \param[in] ObjectId       Identifier of the crypto driver object that shall process the request
 * \param[in] Job            Reference to the Job that shall be processed
 *
 * \returns  Result of the Job processing
 * \retval   E_OK                         Job has been successfully processed.
 * \retval   E_NOT_OK                     Request failed.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_AES_ECB_Start
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);
/** \brief  This is the abstraction layer function for ENCRYPT_AES_ECB
 *
 * This function actually performs the request for service ENCRYPT with algorithm family AES and
 * algorithm mode ECB. It internally calls the Crypto_xVIx_xAIx_AES_EncryptUpdate function which actually
 * Performs the update step of the processing.
 *
 * \param[in] ObjectId       Identifier of the crypto driver object that shall process the request
 * \param[in] Job            Reference to the Job that shall be processed
 *
 * \returns  Result of the Job processing
 * \retval   E_OK                         Job has been successfully processed.
 * \retval   E_NOT_OK                     Request failed
 * \retval   CRYPTO_E_BUSY                Request failed, another computation is already underway.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_AES_ECB_Update
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief  This is the abstraction layer function for ENCRYPT_AES_ECB
 *
 *  This function actually performs the request for service ENCRYPT with algorithm family AES and
 *  algorithm mode ECB. It internally calls the Crypto_xVIx_xAIx_AES_EncryptFinish function which actually
 *  Performs the finish step of the processing.
 *
 *  \param[in] ObjectId       Identifier of the crypto driver object that shall process the request
 *  \param[in] Job            Reference to the Job that shall be processed
 *
 * \returns  Result of the Job processing
 * \retval   E_OK                         Job has been successfully processed.
 * \retval   E_NOT_OK                     Request failed
 * \retval   CRYPTO_E_BUSY                Request failed, another computation is already underway.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_AES_ECB_Finish
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief  This is the abstraction layer function for ENCRYPT_AES_ECB
 *
 * This interface removes the provided Job from the queue and cancels the processing of the Job if
 * possible.
 *
 * \param[in] ObjectId       Identifier of the crypto driver object that shall process the request
 * \param[in] Job            Reference to the Job that shall be canceled.
 *
 * \returns  Result of the Job processing
 * \retval   E_OK                         Job has been successfully processed
 * \retval   E_NOT_OK                     Request failed
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_AES_ECB_Cancel
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief  This is the abstraction layer function for ENCRYPT_AES_ECB
 *
 * This function actually performs the request for service ENCRYPT with algorithm family AES and
 * algorithm mode ECB. It internally calls the Crypto_xVIx_xAIx_AES_EncryptMainFunction. This function than
 * performed all the state changes and function calls to perform the calculations.
 *
 * \param[in/out] Result        This parameter holds the result of the last called
 * Crypto_xVIx_xAIx_AL_ENCRYPT_AES_ECB_MainFunction.
 * \param[in/out] Busy          This parameter holds the state of the primitive. This state is set
 * to FALSE after the primitive has performed Start or update or it has Finished.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_AES_ECB_MainFunction
(
  P2VAR(Std_ReturnType,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
);

#endif /* #if (CRYPTO_XVIX_XAIX_AES_ENCRYPT_ENABLED == STD_ON) */

/*--CRYPTO-CIPHER-AES-CBC-ENCRYPT-AL-LAYER-FUNCTIONS---------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_ENABLED == STD_ON)

/** \brief This is the abstraction layer function for ENCRYPT_AES_CBC
 *
 * This function actually performs the request for service ENCRYPT with algorithm family AES and
 * algorithm mode CBC.This function internally calls Crypto_xVIx_xAIx_CbcPkcs7EncryptStart function which
 * Performs the start step for the CBC Encrypt primitive.
 *
 * \param[in] ObjectId       Identifier of the crypto driver object that shall process the request
 * \param[in] Job            Reference to the Job that shall be processed
 *
 * \returns  Result of the Job processing
 * \retval   E_OK                         Job has been successfully processed
 * \retval   E_NOT_OK                     Request failed
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CBC_Start
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief This is the abstraction layer function for ENCRYPT_AES_CBC
 *
 * This function actually performs the request for service ENCRYPT with algorithm family AES and
 * algorithm mode CBC. It internally This function internally calls Crypto_xVIx_xAIx_CbcPkcs7EncryptUpdate
 * function which Performs the update step for the CBC Encrypt primitive.
 *
 * \param[in] ObjectId       Identifier of the crypto driver object that shall process the request
 * \param[in] Job            Reference to the Job that shall be processed
 *
 * \returns  Result of the Job processing
 * \retval   E_OK                      Job has been successfully processed
 * \retval   E_NOT_OK                  Request failed
 * \retval   CRYPTO_E_BUSY             Request failed, the key to be used is not valid

 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CBC_Update
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief This is the abstraction layer function for ENCRYPT_AES_CBC
 *
 * This function actually performs the request for service ENCRYPT with algorithm family AES and
 * algorithm mode CBC. It internally calls the Crypto_xVIx_xAIx_CbcPkcs7EncryptFinish function which
 * performs the Finish step for the primitive.
 *
 * \param[in] ObjectId       Identifier of the crypto driver object that shall process the request
 * \param[in] Job            Reference to the Job that shall be processed
 *
 * \returns  Result of the Job processing
 * \retval   E_OK                         Job has been successfully processed
 * \retval   E_NOT_OK                     Request failed
 * \retval   CRYPTO_E_BUSY                Request failed, the key to be used is not valid
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CBC_Finish
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief This is the abstraction layer function for ENCRYPT_AES_CBC
 *
 * This function actually performs the request for service ENCRYPT with algorithm family AES and
 * algorithm mode CBC. It cancels the Job and removes it from the queue.
 *
 * \param[in] ObjectId       Identifier of the crypto driver object that shall process the request
 * \param[in] Job            Reference to the Job that shall be processed
 *
 * \returns Result of the cancellation attempt
 * \retval E_OK                   Job was successfully cancelled.
 * \retval E_NOT_OK               Job could not be cancelled.
 * \retval CRYPTO_E_JOB_CANCELED  Job could not be canceled immediately
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CBC_Cancel
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief This is the abstraction layer function for ENCRYPT_AES_CBC
 *
 * This function actually performs the request for service ENCRYPT with algorithm family AES and
 * algorithm mode CBC. It internally calls the Crypto_xVIx_xAIx_CbcPkcs7EncryptMainFunction which takes
 * care of the state transition and the calculations for AES CBC Encrypt primitive.
 *
 * \param[in/out] Result        This parameter holds the result of the last called
 * Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CBC_MainFunction.
 * \param[in/out] Busy          This parameter holds the state of the primitive. This state is set
 * to FALSE after the primitive has performed Start or update or it has Finished.
 *
 *  \returns  void
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CBC_MainFunction
(
  P2VAR(Std_ReturnType,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
);

#endif /* CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_ENABLED */

/*--CRYPTO-CIPHER-AES-CBC-DECRYPT-AL-LAYER-FUNCTIONS---------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_ENABLED == STD_ON)

/** \brief This is the abstraction layer function for DECRYPT_AES_CBC
 *
 * This function actually performs the request for service DECRYPT with algorithm family AES and
 * algorithm mode CBC.This function internally calls Crypto_xVIx_xAIx_CbcPkcs7DecryptStart function which
 * Performs the start step for the CBC Decrypt primitive.
 *
 * \param[in] ObjectId       Identifier of the crypto driver object that shall process the request
 * \param[in] Job            Reference to the Job that shall be processed
 *
 * \returns  Result of the Job processing
 * \retval   E_OK                         Job has been successfully processed
 * \retval   E_NOT_OK                     Request failed
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_AES_CBC_Start
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief This is the abstraction layer function for DECRYPT_AES_CBC
 *
 * This function actually performs the request for service DECRYPT with algorithm family AES and
 * algorithm mode CBC. It internally This function internally calls Crypto_xVIx_xAIx_CbcPkcs7DecryptUpdate
 * function which Performs the update step for the CBC Decrypt primitive.
 *
 * \param[in] ObjectId       Identifier of the crypto driver object that shall process the request
 * \param[in] Job            Reference to the Job that shall be processed
 *
 * \returns  Result of the Job processing
 * \retval   E_OK                      Job has been successfully processed
 * \retval   E_NOT_OK                  Request failed
 * \retval   CRYPTO_E_BUSY             Request failed, the key to be used is not valid
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_AES_CBC_Update
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief This is the abstraction layer function for DECRYPT_AES_CBC
 *
 * This function actually performs the request for service DECRYPT with algorithm family AES and
 * algorithm mode CBC. It internally calls the Crypto_xVIx_xAIx_CbcPkcs7DecryptFinish function which
 * performs the Finish step for the primitive.
 *
 * \param[in] ObjectId       Identifier of the crypto driver object that shall process the request
 * \param[in] Job            Reference to the Job that shall be processed
 *
 * \returns  Result of the Job processing
 * \retval   E_OK                         Job has been successfully processed
 * \retval   E_NOT_OK                     Request failed
 * \retval   CRYPTO_E_BUSY                Request failed, the key to be used is not valid
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_AES_CBC_Finish
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief This is the abstraction layer function for DECRYPT_AES_CBC
 *
 * This function actually performs the request for service DECRYPT with algorithm family AES and
 * algorithm mode CBC. It cancels the Job and removes it from the queue.
 *
 * \param[in] ObjectId       Identifier of the crypto driver object that shall process the request
 * \param[in] Job            Reference to the Job that shall be processed
 *
 * \returns Result of the cancellation attempt
 * \retval E_OK                   Job was successfully cancelled.
 * \retval E_NOT_OK               Job could not be cancelled.
 * \retval CRYPTO_E_JOB_CANCELED  Job could not be canceled immediately
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_AES_CBC_Cancel
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);
/** \brief This is the abstraction layer function for DECRYPT_AES_CBC
 *
 * This function actually performs the request for service DECRYPT with algorithm family AES and
 * algorithm mode CBC. It internally calls the Crypto_xVIx_xAIx_CbcPkcs7DecryptMainFunction which takes
 * care of the state transition and the calculations for AES CBC Decrypt primitive.
 *
 * \param[in/out] Result        This parameter holds the result of the last called
 * Crypto_xVIx_xAIx_AL_DECRYPT_AES_CBC_MainFunction.
 * \param[in/out] Busy          This parameter holds the state of the primitive. This state is set
 * to FALSE after the primitive has performed Start or update or it has Finished.
 *
 *  \returns  void
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_AES_CBC_MainFunction
(
  P2VAR(Std_ReturnType,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
);

#endif /* #if (CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_ENABLED == STD_ON) */

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* CRYPTO_AL_CIPHER_H */

