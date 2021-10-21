/**
 * \file
 *
 * \brief AUTOSAR Eth
 *
 * This file contains the implementation of the AUTOSAR
 * module Eth.
 *
 * \version 0.0.8
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2011 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]============================================*/

#define HAVE_REMOTE
/* prevent the failure of the "make depend" run */
#ifndef EMPTY
#include <pcap.h>
#include <stdlib.h>            /* exit() */
#endif

#include <TSAutosar.h>         /* EB specific standard types */
#include <Eth_Int.h>           /* internal module API */
#include <Eth_Legacy.h>        /* function declaration of legacy code */
#include <EthIf_Cbk.h>         /* Ethernet Interface callback public API */
#include <Eth_CfgInt.h>        /* own user provided configuration */
#include <SchM_Eth.h>          /* for exclusive area */

/*==================[version check]=========================================*/

/*==================[macros]================================================*/

#define ETH_PROT_HDR_LEN  14U

/*==================[type definitions]======================================*/

#define ETH_WINPCAP_MAC_ADDR_LEN         6U

/** Length of the transmit buffers for payload */
#define ETH_WINPCAP_TXPAYLOADLEN         ETH_WINPCAP_TXBUFLEN - ETH_PROT_HDR_LEN

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define ETH_START_SEC_CODE
#include <Eth_MemMap.h>

STATIC FUNC (void, ETH_CODE) Eth_Arch_UnlockBuffers (void);

#define ETH_STOP_SEC_CODE
#include <Eth_MemMap.h>

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

#define ETH_START_SEC_VAR_INIT_8
#include <MemMap.h>

VAR(uint8, TCPIP_VAR_NOINIT ) Eth_InterfaceId = ETH_WINPCAP_DEV_NUM;

#define ETH_STOP_SEC_VAR_INIT_8
#include <MemMap.h>

/*==================[internal data]=========================================*/

#define ETH_START_SEC_VAR_INIT_UNSPECIFIED
#include <Eth_MemMap.h>

STATIC uint8 Eth_WinPcapEthaddr[ETH_WINPCAP_MAC_ADDR_LEN] = ETH_WINPCAP_ETH_ADDR;

STATIC pcap_t* Eth_WinPcapFp = NULL_PTR;

STATIC char Eth_WinPcapErrBuf[PCAP_ERRBUF_SIZE];

STATIC uint8 Eth_WinPcapSendBuffer[ETH_WINPCAP_TXBUFTOTAL][ETH_WINPCAP_TXBUFLEN];
STATIC boolean Eth_WinPcapSendBufferLocked[ETH_WINPCAP_TXBUFTOTAL];

#define ETH_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <Eth_MemMap.h>

/*==================[external function definitions]=========================*/

#define ETH_START_SEC_CODE
#include <Eth_MemMap.h>

FUNC(void, ETH_CODE) Eth_Arch_Init
(
    P2CONST(Eth_ConfigType, AUTOMATIC, ETH_APPL_CONST) CfgPtr
)
{
    TS_PARAM_UNUSED(CfgPtr);

    /* Initalize buffer state variable */
    Eth_Arch_UnlockBuffers();

} /* eof - Eth_Arch_Init */

FUNC(Std_ReturnType, ETH_CODE) Eth_Arch_MiiInit
(
    uint8 CtrlIdx
)
{
    TS_PARAM_UNUSED(CtrlIdx);
    return E_OK;
} /* eof - Eth_Arch_MiiInit */

FUNC(Std_ReturnType, ETH_CODE) Eth_Arch_ControllerInit
(
    uint8 CtrlIdx
)
{
    P2VAR(pcap_if_t, AUTOMATIC, ETH_APPL_DATA) alldevs;
    P2VAR(pcap_if_t, AUTOMATIC, ETH_APPL_DATA) d;
    P2VAR(pcap_if_t, AUTOMATIC, ETH_APPL_DATA) d_selected = NULL_PTR;
    uint8      i;

    ETH_ENTER_OS_WINDOWS_SERVICE_AREA();

    TS_PARAM_UNUSED(CtrlIdx);

    /* Retrieve the device list on the local machine */
    if ( pcap_findalldevs_ex
         (
             (P2VAR(char, AUTOMATIC, ETH_APPL_DATA)) PCAP_SRC_IF_STRING,
             NULL,                 /* no remote auth */
             &alldevs,
             Eth_WinPcapErrBuf
         ) == -1
       )
    {
        /* output to stderr does not work in freestanding C */
        printf("ERROR in pcap_findalldevs: %s\n", Eth_WinPcapErrBuf);
        exit(1);
    }

    /* Print the list */
    for (i = 0U, d = alldevs; d != NULL_PTR; d = d->next)
    {
        printf("%d. %s", ++i, d->name);
        if (d->description)
        {
            printf(" (%s)\n", d->description);
        }
        else
        {
            printf(" (No description available)\n");
        }
        if (i == Eth_InterfaceId)
        {
            d_selected = d;
        }
    }

    if (i == 0U)
    {
        printf("\nNo interfaces found! Make sure WinPcap is installed.\n");
        exit(1);
    }

    if (d_selected == NULL_PTR)
    {
        printf("\nSelected interface not found! Check ETH_WINPCAP_DEV_NUM\n");
        exit(1);
    }

    /* open device with number ETH_WINPCAP_DEV_NUM */
    Eth_WinPcapFp = pcap_open
        (
            d_selected->name,               /* name of the device */
            0xFFFF,                         /* portion of the packet to capture */
            PCAP_OPENFLAG_PROMISCUOUS |     /* promiscuous mode */
            PCAP_OPENFLAG_NOCAPTURE_LOCAL,  /* drop the packets that were sent by itself */
            1,                              /* read timeout (ms) */
            NULL,                           /* no authentication on the remote machine */
            Eth_WinPcapErrBuf               /* error buffer */
        );

    if (Eth_WinPcapFp == NULL_PTR)
    {
        printf("\nPcap can not open selected device!\n");
        exit(1);
    }

    printf("\nlistening on %s...\n", d_selected->description);

    /* At this point, we don't need any more the device list. Free it */
    pcap_freealldevs(alldevs);

    ETH_EXIT_OS_WINDOWS_SERVICE_AREA();

    return E_OK;
} /* eof - Eth_Arch_ControllerInit */

FUNC(Std_ReturnType, ETH_CODE) Eth_Arch_SetControllerMode
(
    uint8 CtrlIdx,
    Eth_ModeType CtrlMode
)
{
    TS_PARAM_UNUSED(CtrlIdx);

    if( CtrlMode == ETH_MODE_DOWN )
    {
        /* controller is disabled, unlock all transmit buffers */
        Eth_Arch_UnlockBuffers();
    }

    return E_OK;
}

FUNC(void, ETH_CODE) Eth_Arch_GetPhysAddr
(
    uint8 CtrlIdx,
    P2VAR(uint8, AUTOMATIC, ETH_APPL_DATA) PhysAddrPtr
)
{
    uint8_least i;

    TS_PARAM_UNUSED(CtrlIdx);

    for (i = 0; i < ETH_WINPCAP_MAC_ADDR_LEN; ++i)
    {
        PhysAddrPtr[i] = Eth_WinPcapEthaddr[i];
    }
}

FUNC(void, ETH_CODE) Eth_Arch_SetPhysAddr
(
    uint8 CtrlIdx,
    P2CONST(uint8, AUTOMATIC, ETH_APPL_DATA) PhysAddrPtr
)
{
    uint8_least i;

    TS_PARAM_UNUSED(CtrlIdx);

    for (i = 0; i < ETH_WINPCAP_MAC_ADDR_LEN; ++i)
    {
        Eth_WinPcapEthaddr[i] = PhysAddrPtr[i];
    }
}

FUNC(Eth_ReturnType, ETH_CODE) Eth_Arch_WriteMii
(
    uint8 CtrlIdx,
    uint8 TrcvIdx,
    uint8 RegIdx,
    uint16 RegVal
)
{
    TS_PARAM_UNUSED(CtrlIdx);
    TS_PARAM_UNUSED(TrcvIdx);
    TS_PARAM_UNUSED(RegIdx);
    TS_PARAM_UNUSED(RegVal);
    return E_NOT_OK;
}

FUNC(Eth_ReturnType, ETH_CODE) Eth_Arch_ReadMii
(
    uint8 CtrlIdx,
    uint8 TrcvIdx,
    uint8 RegIdx,
    P2VAR(uint16, AUTOMATIC, ETH_APPL_DATA) RegValPtr
)
{
    TS_PARAM_UNUSED(CtrlIdx);
    TS_PARAM_UNUSED(TrcvIdx);
    TS_PARAM_UNUSED(RegIdx);
    TS_PARAM_UNUSED(RegValPtr);
    return E_NOT_OK;
}

FUNC(void, ETH_CODE) Eth_Arch_GetCounterState
(
    uint8 CtrlIdx,
    uint16 CtrlOffs,
    P2VAR(uint32, AUTOMATIC, ETH_APPL_DATA) CtrlValPtr
)
{
    TS_PARAM_UNUSED(CtrlIdx);
    TS_PARAM_UNUSED(CtrlOffs);
    TS_PARAM_UNUSED(CtrlValPtr);
}

FUNC(BufReq_ReturnType, ETH_CODE) Eth_Arch_ProvideTxBuffer
(
    uint8 CtrlIdx,
    uint8 Priority,
    P2VAR(Eth_BufIdxType, AUTOMATIC, ETH_APPL_DATA) BufIdxPtr,
    P2VAR(P2VAR(Eth_DataType, TYPEDEF, ETH_APPL_DATA), AUTOMATIC, ETH_APPL_DATA) BufPtr,
    P2VAR(uint16, AUTOMATIC, ETH_APPL_DATA) LenBytePtr
)
{
    /* set default return value to BUFREQ_E_BUSY, it shall be returned if no buffer is available */
    BufReq_ReturnType result = BUFREQ_E_BUSY;
    uint8_least bufIdx;

    TS_PARAM_UNUSED(CtrlIdx);
    TS_PARAM_UNUSED(Priority);

    SchM_Enter_Eth_SCHM_ETH_EXCLUSIVE_AREA_0();

    /* loop over all transmit buffers */
    for ( bufIdx = 0U ; bufIdx < ETH_WINPCAP_TXBUFTOTAL ; bufIdx++ )
    {
        /* to look for one that is not locked */
        if( Eth_WinPcapSendBufferLocked[bufIdx] == FALSE )
        {
            /* lock this buffer */
            Eth_WinPcapSendBufferLocked[bufIdx] = TRUE;

            /* Leave room for Ethernet header */
            *BufPtr = (P2VAR(Eth_DataType, AUTOMATIC, ETH_APPL_DATA))
                &Eth_WinPcapSendBuffer[bufIdx][ETH_PROT_HDR_LEN];

            /* return the buffer index */
            *BufIdxPtr = (Eth_BufIdxType) bufIdx;

            if ( *LenBytePtr > ETH_WINPCAP_TXPAYLOADLEN )
            {
                *LenBytePtr = ETH_WINPCAP_TXPAYLOADLEN;
            }

            result = BUFREQ_OK;

            /* buffer is assigned, stop the loop */
            break;
        }
    }

    SchM_Exit_Eth_SCHM_ETH_EXCLUSIVE_AREA_0();

    return result;
}

FUNC(Std_ReturnType, ETH_CODE) Eth_Arch_Transmit
(
    uint8 CtrlIdx,
    Eth_BufIdxType BufIdx,
    Eth_FrameType FrameType,
    boolean TxConfirmation,
    uint16 LenByte,
    P2CONST(uint8, AUTOMATIC, ETH_APPL_DATA) PhysAddrPtr
)
{
    Std_ReturnType retval = E_NOT_OK;
    uint16 len = LenByte + ETH_PROT_HDR_LEN;

    ETH_ENTER_OS_WINDOWS_SERVICE_AREA();

    TS_PARAM_UNUSED(CtrlIdx);
    TS_PARAM_UNUSED(TxConfirmation);

    /* check the used buffer was requested with ProvideTxBuffer() before */
    if( Eth_WinPcapSendBufferLocked[BufIdx] == TRUE )
    {
        /* Fill in destination MAC address in ethernet header */
        Eth_WinPcapSendBuffer[BufIdx][0] = PhysAddrPtr[0];
        Eth_WinPcapSendBuffer[BufIdx][1] = PhysAddrPtr[1];
        Eth_WinPcapSendBuffer[BufIdx][2] = PhysAddrPtr[2];
        Eth_WinPcapSendBuffer[BufIdx][3] = PhysAddrPtr[3];
        Eth_WinPcapSendBuffer[BufIdx][4] = PhysAddrPtr[4];
        Eth_WinPcapSendBuffer[BufIdx][5] = PhysAddrPtr[5];

        /* Fill in source MAC address in ethernet header */
        Eth_WinPcapSendBuffer[BufIdx][6] = Eth_WinPcapEthaddr[0];
        Eth_WinPcapSendBuffer[BufIdx][7] = Eth_WinPcapEthaddr[1];
        Eth_WinPcapSendBuffer[BufIdx][8] = Eth_WinPcapEthaddr[2];
        Eth_WinPcapSendBuffer[BufIdx][9] = Eth_WinPcapEthaddr[3];
        Eth_WinPcapSendBuffer[BufIdx][10] = Eth_WinPcapEthaddr[4];
        Eth_WinPcapSendBuffer[BufIdx][11] = Eth_WinPcapEthaddr[5];

        /* Fill frame type in ethernet header */
        Eth_WinPcapSendBuffer[BufIdx][12] = (FrameType >> 8) & 0xff;
        Eth_WinPcapSendBuffer[BufIdx][13] = (FrameType)      & 0xff;

        /* Send down the packet */
        if ( pcap_sendpacket ( Eth_WinPcapFp, Eth_WinPcapSendBuffer[BufIdx], len ) != 0 )
        {
            /* output to stderr does not work in freestanding C */
            printf ( "\nERROR sending the packet: %s\n", pcap_geterr(Eth_WinPcapFp) );
        }
        else
        {
            /* DEBUG */
            printf("packet transmitted, len=%d\n", len);
            retval = E_OK;
        }

        /* unlock the transmit buffer */
        Eth_WinPcapSendBufferLocked[BufIdx] = FALSE;
    }

    ETH_EXIT_OS_WINDOWS_SERVICE_AREA();

    return retval;
} /* eof - Eth_Arch_Transmit */



FUNC(uint8, ETH_CODE) Eth_Legacy_TxConfirmation
(
    uint8 CtrlIdx,
    uint8 u8BufIdx
)
{
  TS_PARAM_UNUSED(CtrlIdx);
  TS_PARAM_UNUSED(u8BufIdx);

  return 255U;
}

FUNC(void, ETH_CODE) Eth_Arch_Receive
(
    uint8 CtrlIdx,
    uint8 QueRxIdx,
    P2VAR(Eth_RxStatusType, AUTOMATIC, ETH_APPL_DATA) RxStatus,
    boolean Irq
)
{
    P2VAR(struct pcap_pkthdr, AUTOMATIC, ETH_APPL_DATA) header;
    P2VAR(uint8, AUTOMATIC, ETH_APPL_DATA)              pkt_data;
    int res;

    ETH_ENTER_OS_WINDOWS_SERVICE_AREA();

    TS_PARAM_UNUSED(CtrlIdx);
    TS_PARAM_UNUSED(Irq);
    TS_PARAM_UNUSED(QueRxIdx);

    /* DEBUG */
    printf(".");

    res = pcap_next_ex ( Eth_WinPcapFp, &header, &pkt_data );

    if ( res <= 0 )
    {
        /* nothing was read */
        if ( res < 0 )
        {
            /* output to stderr does not work in freestanding C */
            printf("ERROR in pcap_next_ex, res=%d\n", res);
        }
    }
    else
    {
        if (header->len > 0)
        {
            /* DEBUG */
            printf("packet received, len=%d\n", header->len);

            /* indicate data reception  */
            EthIf_Cbk_RxIndication
                (
                    CtrlIdx,
                    (P2VAR(Eth_DataType, AUTOMATIC, ETH_APPL_DATA)) pkt_data,
                    header->len
                );

            /* we don't know the number of packets available, therefore always indicates
             * that more packets are available. */
            *RxStatus = ETH_RECEIVED_MORE_DATA_AVAILABLE;
        }
    }

    ETH_EXIT_OS_WINDOWS_SERVICE_AREA();
} /* eof - Eth_Receive */

FUNC(void, EBTEST_CODE) Eth_PrintAvialableInterfaces
(
  void
)
{
  P2VAR(pcap_if_t, AUTOMATIC, ETH_APPL_DATA) alldevs;
  P2VAR(pcap_if_t, AUTOMATIC, ETH_APPL_DATA) d;
  uint8      i;

  /* Retrieve the device list on the local machine */
  if ( pcap_findalldevs_ex
       (
           (P2VAR(char, AUTOMATIC, ETH_APPL_DATA)) PCAP_SRC_IF_STRING,
           NULL,                 /* no remote auth */
           &alldevs,
           Eth_WinPcapErrBuf
       ) == -1
     )
  {
      /* output to stderr does not work in freestanding C */
      printf("ERROR in pcap_findalldevs: %s\n", Eth_WinPcapErrBuf);
      exit(1);
  }

  /* Print the list */
  for (i = 0U, d = alldevs; d != NULL_PTR; d = d->next)
  {
      printf("%d. %s", ++i, d->name);
      if (d->description)
      {
          printf(" (%s)\n", d->description);
      }
      else
      {
          printf(" (No description available)\n");
      }
  }

  if (i == 0U)
  {
      printf("\nNo interfaces found! Make sure WinPcap is installed.\n");
      exit(1);
  }
}

/*==================[internal function definitions]=========================*/

STATIC FUNC (void, ETH_CODE) Eth_Arch_UnlockBuffers (void)
{
    uint8_least i;

    for( i = 0U ; i < ETH_WINPCAP_TXBUFTOTAL ; i++ )
    {
      Eth_WinPcapSendBufferLocked[i] = FALSE;
    }
}

#define ETH_STOP_SEC_CODE
#include <Eth_MemMap.h>

/*==================[end of file]===========================================*/
