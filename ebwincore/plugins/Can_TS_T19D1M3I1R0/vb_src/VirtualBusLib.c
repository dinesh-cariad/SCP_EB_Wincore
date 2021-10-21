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

/*==[Includes]================================================================*/
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <Can.h>
#include <cif_swig_defs.h>

/*==[Macros]==================================================================*/
#define VIRTBUS_MAX_CAN_LENGTH  64U

/*==[Types]===================================================================*/

/*==[Declaration of functions with internal linkage]==========================*/
static void* ReceiveDataThread(void* pParam);

/*==[Constants with internal linkage]=========================================*/

/*==[Variables with internal linkage]=========================================*/
TSDReceiveCallback * Sd_Callback;
static unsigned int Global_hwRefID = CIF_INVALID_refID;

/*==[Constants with external linkage]=========================================*/

/*==[Variables with external linkage]=========================================*/

/*==[Definition of functions with external linkage]===========================*/
/**
 *
 * \brief Initialized Windows error handling.
 *
 * This function sets up the Windows socket error handling. It has to be
 * called before any other function.
 *
 * \retval SD_OK        The error handling has been initialized
 * \retval SD_ERROR     The error handling could not be initialized
 *
 */
Sd_ReturnType Sd_Init()
{
  Global_hwRefID = CIF_INVALID_refID;
  return SD_OK;
}

/**
 *
 * \brief Connects to a Server Socket (TCP/IP)
 *
 * This function sets up a connection to a Server Socket on the specified IP
 * Address and appropriate socket ID.
 *
 * \param szIPAddress   zero terminated string with IP address (eg. "127.0.0.1")
 * \param nPort         ID of the Port to connect (eg. 10020 for Softdevice)
 *
 * \retval SD_OK        The function has established a connection
 * \retval SD_ERROR     The function has not been able to establish a connection
 *
 */
Sd_ReturnType Sd_Connect(Can_ControllerType* controller)
{
  Sd_ReturnType retval = SD_ERROR;
  Can_CtrlStateTableType* ctrlState = controller->ControllerState;
  if ((ctrlState->HwRefId==CIF_INVALID_refID)||(ctrlState->HwRefId<0x1000))
  {
    if (controller->CanFDSupport==FALSE)
    {
    ctrlState->HwRefId = C2Cpp_initCan(controller->CanHardware, controller->CanHwChannel, controller->CanBitRate);
    }
    else
    {
       ctrlState->HwRefId = C2Cpp_initCanFD( controller->CanHardware,
                                           controller->CanHwChannel,
                                           -1, /* hwIndex */
                                           controller->BaudConfig->BaudRate,
                                           controller->BaudConfig->Seg1,
                                           controller->BaudConfig->Seg2,
                                           controller->BaudConfig->SWJ,
                                           controller->BaudFDConfig->BaudRate,
                                           controller->BaudFDConfig->Seg1,
                                           controller->BaudFDConfig->Seg2,
                                           controller->BaudFDConfig->SWJ,
                                           controller->BaudFDConfig->pcan_f);
    }
    if (ctrlState->HwRefId!=CIF_INVALID_refID)
    {
      retval = SD_OK;
    }
    else
    {
      printf("ERROR COULD NOT INITIZALIZE HARDWARE\n");
    }
    if (ctrlState->RxThreadState == CAN_RX_THREAD_UNINIT)
    {
      ctrlState->RxThreadState = CAN_RX_THREAD_STARTED;
      ctrlState->ReceiveThreadHandle = (uint32*)CreateThread(NULL, 32768U,
          (LPTHREAD_START_ROUTINE)ReceiveDataThread, (void*) controller, 0, 0);
    }

  }
  else
  {
    retval = SD_OK;
  }
  return retval;
}

/**
 *
 * \brief Disconnects from a Server Socket (TCP/IP)
 *
 * This function closes a connection to a Server Socket.
 *
 *
 * \retval SD_OK        The function has closed the connection
 * \retval SD_ERROR     The function has not been able to close the connection
 *
 */
Sd_ReturnType Sd_Disconnect(Can_ControllerType* controller)
{
  if ( (controller->ControllerState->RxThreadState==CAN_RX_THREAD_RUNNING)||
       (controller->ControllerState->RxThreadState==CAN_RX_THREAD_STARTED) )
  {
    controller->ControllerState->RxThreadState = CAN_RX_THREAD_STOP_REQUESTED;
    do{
      Sleep(10); /* wait for thread to stop */
    } while (controller->ControllerState->RxThreadState!= CAN_RX_THREAD_STOPPED);
  }
  C2Cpp_deinitHw(controller->ControllerState->HwRefId);
  controller->ControllerState->HwRefId = CIF_INVALID_refID;
  return SD_OK;
}

/**
 *
 * \brief Sends data to the connected server socket (TCP/IP)
 *
 * This function sends the content of the VirtualBusDataType structure to the
 * connected server socket eg. Softdevice.
 *
 * \param pData         Datapacket to send over the virtual bus
 *
 * \retval SD_OK        The function has sent all data sucessfully
 * \retval SD_ERROR     The function has not been able to send all data sucessfully or
 *                      a socket error has occured
 *
 * \remarks You have to delete the allocated payload (VirtualBusDataType::pData) by yourself
 *          after sending.
 */
Sd_ReturnType Sd_SendData(VirtualBusDataType* pData)
{
  Sd_ReturnType eRet = SD_OK;
  uint32 hwRefId = CIF_INVALID_refID;
  int retval;
  unsigned int fdCtrl = FALSE;
  unsigned int BRS = FALSE;
  if(pData == NULL)
  {
    return SD_ERROR;
  }

  if (pData->nLength > VIRTBUS_MAX_CAN_LENGTH)
  {
    return SD_ERROR;
  }
  hwRefId = Can_Controllers[pData->nBusID].ControllerState->HwRefId;
  fdCtrl = Can_Controllers[pData->nBusID].CanFDSupport;
  if ((fdCtrl == TRUE)&& (Can_Controllers[pData->nBusID].BaudFDConfig!=NULL_PTR))
  {
    BRS = Can_Controllers[pData->nBusID].BaudFDConfig->BRS;
  }

  retval = C2Cpp_sendFrameCAN(hwRefId, pData->nID, pData->nLength, pData->pData, fdCtrl, BRS);

  if (retval != CIF_MSGSTATUS_OK )
  {
    eRet = SD_ERROR;
  }
  return eRet;
}

/**
 *
 * \brief Registers a callback function for receiving data
 *
 * This function sets a callback that is notified when new data in the queue
 * is available. The callback can be registered for different bustypes
 * specified in the Sd_ReturnType enumeration
 *
 * \param pReceiveCallback  Function that is called when new data is available
 * \param eBus              Bustype for which the callback is registered
 *
 * \retval SD_OK        The function has registered the callback correctly
 * \retval SD_ERROR     The function has not been able to register the callback
 *                      or a callback is registered allready
 *
 * \remarks The Datapackets of a specified bus type are queued since a callback
 *          is registered on the bus type. Datapackets received before registration
 *          of a callback are dismissed.
 */
Sd_ReturnType Sd_Register_ReceiveCallback(TSDReceiveCallback* pReceiveCallback)
{

  Sd_Callback = pReceiveCallback;

  return SD_OK;
}

/**
 *
 * \brief Unregisters a callback function for receiving data
 *
 * This function unregisters the callback function for the specified bus type
 *
 * \param eBus              Bustype for which the callback should be unregistered
 *
 * \retval SD_OK        The function has unregistered the callback correctly
 * \retval SD_ERROR     The function has not been able to unregister the callback
 *
 * \remarks The Datapackets of a specified bus type are queued since a callback
 *          is registered on the bus type. Datapackets received before registration
 *          and after unregistration of a callback are dismissed.
 */
Sd_ReturnType Sd_Unregister_ReceiveCallback()
  {

  Sd_Callback = NULL;

  return SD_OK;
}

/**
 *
 * \brief Sets the size of the queue
 *
 * This function sets the size of the queue of the specified bus type.
 * A queue depth of 0 means no restriction of queue size (default)
 *
 * \param eBus          Bustype of the queue
 * \param nSize         The new size of the queue (number of packets)
 *
 * \retval SD_OK        The function has retrieved a data packet
 * \retval SD_NOTFOUND  The queue specified by the bus type was not found
 *
 * \remarks Variable queue size is not supported by the current version of the
 *          CAN MCAL. This C port of the virtual bus library behaves as if
 *          the queue size has been set to 1.
 */
Sd_ReturnType Sd_SetQueueSize(unsigned long nSize)
{
  (void) nSize;
  return SD_OK;
}

/*==[Definition of functions with internal linkage]===========================*/
static void* ReceiveDataThread(void* pParam)
{
  unsigned int nReceivedBytes;
  unsigned int portRefId = CIF_INVALID_refID;
  Can_ControllerType* ctrl = (Can_ControllerType*)pParam;

  ctrl->ControllerState->RxThreadState = CAN_RX_THREAD_RUNNING;
  portRefId = C2Cpp_addCanLoggerPort(ctrl->ControllerState->HwRefId);
  C2Cpp_startPort(portRefId);
  do
  {
    VirtualBusDataType pRxBusData;
    unsigned char pRxData[VIRTBUS_MAX_CAN_LENGTH];
    unsigned int timeout = 10000;
    unsigned int CanId  = 0;
    unsigned int timeStampHi=0;
    unsigned int timeStampLo=0;
    unsigned int is_ext=0;
    unsigned int status=0;
    unsigned int is_tx=0;

    nReceivedBytes = C2Cpp_receiveFrameCAN( portRefId,
                           timeout,
                           &CanId,
                           &timeStampHi,
                           &timeStampLo,
                           &is_ext,
                           &status,
                           pRxData,
                           &is_tx);

    pRxBusData.nID = CanId;
    pRxBusData.nLength = nReceivedBytes;
    pRxBusData.pData = pRxData;
    pRxBusData.nCycleExt = is_ext;
    if((status == CIF_MSGSTATUS_OK)&&(nReceivedBytes>0)&&(is_tx==FALSE))
    {
      /* Call the receive callback */
      if(Sd_Callback)
      {
        Sd_Callback(&pRxBusData);
      }
    }
  } while(ctrl->ControllerState->RxThreadState != CAN_RX_THREAD_STOP_REQUESTED);
  ctrl->ControllerState->RxThreadState = CAN_RX_THREAD_STOPPED;
  return pParam;
}
