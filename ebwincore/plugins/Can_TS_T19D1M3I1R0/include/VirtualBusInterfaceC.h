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
#ifndef _SD_VIRTBUSINTERFACEC_H_
#define _SD_VIRTBUSINTERFACEC_H_

/*==[Includes]================================================================*/
#include <IVirtualBusProtocol.h>

/*==[Macros]==================================================================*/

/*==[Types]===================================================================*/
typedef void (TSDReceiveCallback)(VirtualBusDataType* pData);

/*==[Declaration of constants with external linkage]==========================*/

/*==[Declaration of variables with external linkage]==========================*/

/*==[Declaration of functions with external linkage]==========================*/
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
extern Sd_ReturnType Sd_Init(void);

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
extern Sd_ReturnType Sd_Connect(Can_ControllerType* controller);

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
extern Sd_ReturnType Sd_Disconnect(Can_ControllerType* controller);

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
 extern Sd_ReturnType Sd_SendData(VirtualBusDataType* pData);

/**
 *
 * \brief Registers a callback function for receiving data
 *
 * This function sets a callback that is notified when new data in the queue
 * is available. The callback can be registered for different bustypes
 * specified in the Sd_BusType enumeration
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
extern Sd_ReturnType Sd_Register_ReceiveCallback(TSDReceiveCallback* pReceiveCallback);

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
extern Sd_ReturnType Sd_Unregister_ReceiveCallback(void);

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
 */
extern Sd_ReturnType Sd_SetQueueSize(unsigned long nSize);

#endif /* _SD_VIRTBUSINTERFACEC_H_ */
