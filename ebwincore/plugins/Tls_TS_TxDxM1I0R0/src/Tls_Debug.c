/**
 * \file
 *
 * \brief AUTOSAR Tls
 *
 * This file contains the implementation of the AUTOSAR
 * module Tls.
 *
 * \version 1.0.8
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]==================================================================*/
#include <Tls_Debug.h>

/*==================[macros]======================================================================*/

/*==================[declaration of types]========================================================*/

/*==================[declaration of external constants]===========================================*/

/*==================[declaration of external variables]===========================================*/

/*==================[declaration of external functions]===========================================*/

#define TLS_START_SEC_CODE
#include <Tls_MemMap.h>

#ifdef TLS_ENABLE_DEBUG

FUNC(void, TLS_CODE) Tls_Debug_PRF_PrintState(P2VAR(Tls_PRF_ContextType, AUTOMATIC, TLS_VAR) context)
{
  TLS_PRINTF(("PRF state: "));
  switch (context->state)
  {
  case TLS_PRF_STATE_IDLE:
      TLS_PRINTF(("TLS_PRF_STATE_IDLE"));
      break;
  case TLS_PRF_STATE_START:
      TLS_PRINTF(("TLS_PRF_STATE_START"));
      break;
  case TLS_PRF_STATE_BUSY_1:
      TLS_PRINTF(("TLS_PRF_STATE_BUSY_1"));
      break;
  case TLS_PRF_STATE_BUSY_2:
      TLS_PRINTF(("TLS_PRF_STATE_BUSY_2"));
      break;
  case TLS_PRF_STATE_BUSY_2A:
      TLS_PRINTF(("TLS_PRF_STATE_BUSY_2A"));
      break;
  case TLS_PRF_STATE_BUSY_2B:
      TLS_PRINTF(("TLS_PRF_STATE_BUSY_2B"));
      break;
  case TLS_PRF_STATE_BUSY_2C:
      TLS_PRINTF(("TLS_PRF_STATE_BUSY_2C"));
      break;
  case TLS_PRF_STATE_BUSY_2D:
      TLS_PRINTF(("TLS_PRF_STATE_BUSY_2D"));
      break;
  case TLS_PRF_STATE_BUSY_2E:
      TLS_PRINTF(("TLS_PRF_STATE_BUSY_2E"));
      break;
  case TLS_PRF_STATE_READY:
      TLS_PRINTF(("TLS_PRF_STATE_READY"));
      break;
  case TLS_PRF_STATE_ERROR:
      TLS_PRINTF(("TLS_PRF_STATE_ERROR"));
      break;
  default:
      TLS_PRINTF(("INVALID PRF STATE(%d)", context->state));
      break;
  }
  TLS_PRINTF(("\n"));
}

FUNC(void, TLS_CODE) Tls_Debug_PrintBytes(P2CONST(char, AUTOMATIC, TLS_CONST) title, P2CONST(uint8, AUTOMATIC, TLS_VAR) bytes, uint32 count)
{
  uint32 i;
  TLS_PRINTF(("%s: [%d] ", title, count));
  for (i = 0; i < count; ++i)
  {
    if(i>0)TLS_PRINTF((","));
      TLS_PRINTF(("0x%02X", bytes[i]));
  }
  TLS_PRINTF(("\n"));
}

FUNC(void, TLS_CODE) Tls_Debug_PrintState(P2VAR(Tls_ConnectionType, AUTOMATIC, TLS_VAR) context)
{
    switch (context->state)
    {
    case TLS_STATE_DEFAULT:
        TLS_PRINTF(("TLS_STATE_DEFAULT"));
        break;
    case TLS_STATE_START:
        TLS_PRINTF(("TLS_STATE_START"));
        break;
    case TLS_STATE_CLIENTHELLO_DONE:
        TLS_PRINTF(("TLS_STATE_CLIENTHELLO_DONE"));
        break;
    case TLS_STATE_SERVERHELLO_DONE:
        TLS_PRINTF(("TLS_STATE_SERVERHELLO_DONE"));
        break;
    case TLS_STATE_SERVERKEYEXCHANGE_DONE:
        TLS_PRINTF(("TLS_STATE_SERVERKEYEXCHANGE_DONE"));
        break;
    case TLS_STATE_SERVERHELLODONE_DONE:
        TLS_PRINTF(("TLS_STATE_SERVERHELLODONE_DONE"));
        break;
    case TLS_STATE_CLIENTKEYEXCHANGE_DONE:
        TLS_PRINTF(("TLS_STATE_CLIENTKEYEXCHANGE_DONE"));
        break;
    case TLS_STATE_CHANGECIPHERSPEC_SENT:
        TLS_PRINTF(("TLS_STATE_CHANGECIPHERSPEC_SENT"));
        break;
    case TLS_STATE_FINISHED_SENT:
        TLS_PRINTF(("TLS_STATE_FINISHED_SENT"));
        break;
    case TLS_STATE_CHANGECIPHERSPEC_RECEIVED:
        TLS_PRINTF(("TLS_STATE_CHANGECIPHERSPEC_RECEIVED"));
        break;
    case TLS_STATE_FINISHED_RECEIVED:
        TLS_PRINTF(("TLS_STATE_FINISHED_RECEIVED"));
        break;
    case TLS_STATE_OK:
        TLS_PRINTF(("TLS_STATE_OK"));
        break;
    case TLS_STATE_ERROR:
        TLS_PRINTF(("TLS_STATE_ERROR"));
        break;
    case TLS_STATE_CLOSING:
        TLS_PRINTF(("TLS_STATE_CLOSING"));
        break;
    case TLS_STATE_CLOSED:
        TLS_PRINTF(("TLS_STATE_CLOSED"));
        break;
    default:
        TLS_PRINTF(("INVALID STATE(%d)", context->state));
        break;
    }
}

FUNC(void, TLS_CODE) Tls_Debug_PrintRecordType(uint8 type)
{
    switch (type)
    {
    case TLS_RECORD_HANDSHAKE:
        TLS_PRINTF(("TLS_RECORD_HANDSHAKE"));
        break;
    case TLS_RECORD_APPLICATIONDATA:
        TLS_PRINTF(("TLS_RECORD_APPLICATIONDATA"));
        break;
    case TLS_RECORD_CHANGECIPHERSPEC:
        TLS_PRINTF(("TLS_RECORD_CHANGECIPHERSPEC"));
        break;
    case TLS_RECORD_ALERT:
        TLS_PRINTF(("TLS_RECORD_ALERT"));
        break;
    default:
        TLS_PRINTF(("INVALID RECORD TYPE(%d)", type));
        break;
    }
}

FUNC(void, TLS_CODE) Tls_Debug_PrintHandshakeType(uint8 type)
{
    switch (type)
    {
    case TLS_HANDSHAKE_CLIENTHELLO:
        TLS_PRINTF(("TLS_HANDSHAKE_CLIENTHELLO"));
        break;
    case TLS_HANDSHAKE_SERVERHELLO:
        TLS_PRINTF(("TLS_HANDSHAKE_SERVERHELLO"));
        break;
    case TLS_HANDSHAKE_SERVERKEYEXCHANGE:
        TLS_PRINTF(("TLS_HANDSHAKE_SERVERKEYEXCHANGE"));
        break;
    case TLS_HANDSHAKE_SERVERHELLODONE:
        TLS_PRINTF(("TLS_HANDSHAKE_SERVERHELLODONE"));
        break;
    case TLS_HANDSHAKE_CLIENTKEYEXCHANGE:
        TLS_PRINTF(("TLS_HANDSHAKE_CLIENTKEYEXCHANGE"));
        break;
    case TLS_HANDSHAKE_FINISHED:
        TLS_PRINTF(("TLS_HANDSHAKE_FINISHED"));
        break;
    case TLS_HANDSHAKE_HELLOVERIFYREQUEST:
        TLS_PRINTF(("TLS_HANDSHAKE_HELLOVERIFYREQUEST"));
        break;
    case TLS_HANDSHAKE_HELLOREQUEST:
        TLS_PRINTF(("TLS_HANDSHAKE_HELLOREQUEST"));
        break;
    default:
        TLS_PRINTF(("INVALID HANDSHAKE TYPE(%d)", type));
        break;
    }
}

#endif

#define TLS_STOP_SEC_CODE
#include <Tls_MemMap.h>

/*==================[end of file]===========================================*/
