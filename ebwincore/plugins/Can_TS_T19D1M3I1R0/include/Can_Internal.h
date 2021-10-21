/**
 * \file
 *
 * \brief AUTOSAR Can
 *
 * This file contains the implementation of the AUTOSAR
 * module Can.
 *
 * \version 3.1.4
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef CAN_INTERNAL_H
#define CAN_INTERNAL_H

/*
 * Misra-C:2004 Deviations:
 *
 * MISRA-1) Deviated Rule: 19.1 (advisory)
 *   '#include' statements in a file should only be preceded by other
 *   preprocessor directives or comments.
 *
 *   Reason:
 *   Inclusion of MemMap.h.
 *
 */

/*==[Includes]================================================================*/
#include "CifConstants.h"

/*==[Macros]==================================================================*/
/** \brief Length of a CAN SDU */
#define CAN_SDU_LENGTH  64U

/** \brief Dummy for unused CAN filter mask (in Tx objects) */
#define CAN_MASK_UNUSED 0x00000000U

/** \brief Dummy for unused CAN Id (in Tx objects) */
#define CAN_ID_UNUSED   0x00U

#define CAN_INVALID_CTRL 0xFFAAU

/** \brief Mask for the extended ID flag */
#define CAN_EXT_MASK  0x80000000U

#define CAN_BUFFER_SIZE 9

/*==[Types]===================================================================*/
/** \brief Return value of circular buffer handling functions */
typedef enum
{
  CAN_BUF_OK,
  CAN_BUF_FULL,
  CAN_BUF_EMPTY
} Can_BufReturnType;

/** \brief Emulation of hardware buffer */
typedef struct
{
  Can_IdType  CanId;
  uint8       Dlc;
  uint8       Sdu[CAN_SDU_LENGTH];
} Can_HwBufType;

/** \brief CanId type */
typedef enum
{
  CAN_ID_TYPE_STANDARD,
  CAN_ID_TYPE_EXTENDED,
  CAN_ID_TYPE_MIXED,
  CAN_ID_TYPE_UNUSED
} Can_CanIdTypeType;

/** \brief Can object type */
typedef enum
{
  CAN_OBJECT_TYPE_RECEIVE,
  CAN_OBJECT_TYPE_TRANSMIT
} Can_CanObjectTypeType;

typedef struct
{
  uint32                BaudRate;
  uint32                PropSeg;
  uint32                Seg1;
  uint32                Seg2;
  uint32                SWJ;
} Can_BaudRateType;


typedef struct
{
  uint32                BaudRate;
  uint32                PropSeg;
  uint32                Seg1;
  uint32                Seg2;
  uint32                SWJ;
  uint32                BRS;
  cif_pcanFd_Clocks_t   pcan_f;
} Can_BaudRateFDType;

typedef enum
{
  CAN_RX_THREAD_UNINIT,
  CAN_RX_THREAD_STARTED,
  CAN_RX_THREAD_RUNNING,
  CAN_RX_THREAD_STOP_REQUESTED,
  CAN_RX_THREAD_STOPPED
} Can_RxStateType;

typedef struct
{
  uint32 HwRefId;
  uint32* ReceiveThreadHandle;
  uint8  State;
  uint8  InterruptEnabled;
  uint32 IntDisableNestingCounter;
  uint8  TxInterruptPending;
  uint8  RxInterruptPending;
  uint32 RxThreadState;
} Can_CtrlStateTableType;

typedef struct
{
  Can_CtrlStateTableType* ControllerState;
  Can_BaudRateType*       BaudConfig;
  Can_BaudRateFDType*     BaudFDConfig;
  uint32                CanBusoffProcessing;
  uint32                CanControllerActivation;
  uint32                ConrollerId;
  uint32                CanRxProcessing;
  uint32                CanTxProcessing;
  uint32                CanWakeupProcessing;
  uint32                CanWakeUpSupport;
  uint32                CanFDSupport;
  uint32                CanHardware;
  uint32                CanHwChannel;
  uint32                CanBitRate;
  uint32                CanBufferSize;
} Can_ControllerType;

/** \brief Configuration of hardware object */
typedef struct
{
  Can_IdType            CanId;
  Can_CanIdTypeType     CanIdType;
  Can_CanObjectTypeType CanObjectType;
  uint32                CanIdMask;
  uint8                 CanBufferIndex; /* Index in HRH *OR* HTH buffer */
  Can_ControllerType*   CanControllerRef;
} Can_HohConfigType;

/*==[Declaration of constants with external linkage]==========================*/
#if (CAN_NUM_HRH > 0)

/* Deviation MISRA-1 */
#define CAN_START_SEC_CONST_8BIT
#include <Can_MemMap.h>

/** \brief Lookup table for HRHs (to speed up frame reception and simplify buffer handling */
extern CONST(uint8, CAN_CONST) Can_HrhLookUpTable[CAN_NUM_HRH];

/* Deviation MISRA-1 */
#define CAN_STOP_SEC_CONST_8BIT
#include <Can_MemMap.h>

#endif /* (CAN_NUM_HRH > 0) */

#if (CAN_NUM_HRH + CAN_NUM_HTH > 0)

/* Deviation MISRA-1 */
#define CAN_START_SEC_CONST_UNSPECIFIED
#include <Can_MemMap.h>

/** \brief Configuration of hardware objects */
extern Can_HohConfigType Can_HohConfig[CAN_NUM_HRH + CAN_NUM_HTH];

extern Can_CtrlStateTableType Can_ControllerStateTable[];

extern Can_ControllerType Can_Controllers[];

/* Deviation MISRA-1 */
#define CAN_STOP_SEC_CONST_UNSPECIFIED
#include <Can_MemMap.h>

#endif /* (CAN_NUM_HRH + CAN_NUM_HTH > 0) */

/*==[Declaration of variables with external linkage]==========================*/

/*==[Declaration of functions with external linkage]==========================*/
/* Avoid unnecessary inclusion of MemMap.h */
#if ((CAN_NUM_HTH > 0) || (CAN_NUM_HRH > 0))

/* Deviation MISRA-1 */
#define CAN_START_SEC_CODE
#include <Can_MemMap.h>

#if (CAN_NUM_HTH > 0)
/** \brief Processing of transmitted frames
 **
 ** This function performs the actual Tx frame processing (basically
 ** calling the CanIf_TxConfirmation() callback for all transmitted
 ** frames). It is used by the Tx interrupt handler as well as by
 ** Can_MainFunction_Write().
 */
extern FUNC(void, CAN_CODE) Can_ProcessTxFrame(void);
#endif /* (CAN_NUM_HTH > 0) */

#if (CAN_NUM_HRH > 0)
/** \brief Processing of received frames
 **
 ** This function performs the actual Rc frame processing (basically
 ** calling the CanIf_RxIndication() callback for all received frames).
 ** It is used by the Rx interrupt handler as well as by
 ** Can_MainFunction_Read().
 */
extern FUNC(void, CAN_CODE) Can_ProcessRxFrame(void);
#endif /* (CAN_NUM_HRH > 0) */

/** \brief C++ Wrapper Functions for CIF access */
extern int C2Cpp_initCan(int hw, int chId, int baudrate);
extern void C2Cpp_deinitHw(int hwRefId);
extern int C2Cpp_sendFrameCAN( unsigned int hwRefID,
                          unsigned int canId,
                          unsigned int length,
                          unsigned char* buf,
                          unsigned int fdCtrl,
                          unsigned int BRS);
extern int C2Cpp_addCanLoggerPort(int hwRefId);
extern int C2Cpp_receiveFrameCAN(unsigned int portRefId,
                                 unsigned int timeout,
                                 unsigned int* OUT_CanId,
                                 unsigned int* OUT_timeStampHi,
                                 unsigned int* OUT_timeStampLo,
                                 unsigned int* OUT_is_ext,
                                 unsigned int* OUT_status,
                     unsigned char* OUT_payload,
                     unsigned int* OUT_is_tx);
extern void C2Cpp_startPort(int portRefId);
extern int C2Cpp_initCanFD(  int hwType,
                             int chId,
                             int hwIndex,
                             int arbitrationBitRate,
                             int tseg1Abr,
                             int tseg2Abr,
                             int sjwAbr,
                             int dataBitRate,
                             int tseg1Dbr,
                             int tseg2Dbr,
                             int sjwDbr,
                             int pcan_f);

/* Deviation MISRA-1 */
#define CAN_STOP_SEC_CODE
#include <Can_MemMap.h>

#endif /* ((CAN_NUM_HTH > 0) || (CAN_NUM_HRH > 0)) */

#endif /* CAN_INTERNAL_H */
