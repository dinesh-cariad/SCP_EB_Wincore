/**
 * \file
 *
 * \brief AUTOSAR ApplTemplates
 *
 * This file contains the implementation of the AUTOSAR
 * module ApplTemplates.
 *
 * \version 6.8.0
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]============================================*/
#include <TSAutosar.h>
#include <Os.h>
#include <EcuM.h>

#ifdef _X86_
#include <stdio.h>
#endif

/*------------------[OS Main Function]--------------------------------------*/

#ifdef _X86_
int main(int argc, char* argv[])
{
  /* Disable output buffering to avoid problems with some debuggers. */
  if ((setvbuf(stdout, NULL, _IONBF, 0) != 0) ||
      (setvbuf(stderr, NULL, _IONBF, 0) != 0))
  {
    fprintf(stderr, "Warning: setvbuf() failed. Console output will be buffered.\n");
  }

  OS_WINDOWSPreStartOs(argc, argv);
#else
/* freestanding C: this really is the signature of main() */
int main(void)
{
#endif

  EcuM_Init();    /* start OS never returns */
  return 0;
}


/*------------------[OS Hooks]----------------------------------------------*/

void ShutdownHook(StatusType errorID)
{
  /* endless loop? */
  TS_PARAM_UNUSED(errorID);
  EcuM_Shutdown();
}

ProtectionReturnType ProtectionHook(StatusType errorID)
{
  TS_PARAM_UNUSED(errorID);
  return PRO_IGNORE;
}

void ErrorHook(StatusType error)
{
  static uint8 a = 0;

  /* Figuring out the error code */
  switch (error)
  {
    case E_OS_ACCESS:
    {
      /* place user code here */
      a++;
      break;
    }
    case E_OS_CALLEVEL:
    {
      /* place user code here */
      a++;
      break;
    }
    case E_OS_ID:
    {
      /* place user code here */
      a++;
      break;
    }
    case E_OS_LIMIT:
    {
      /* place user code here */
      a++;
      break;
    }
    case E_OS_NOFUNC:
    {
      /* place user code here */
      a++;
      break;
    }
    case E_OS_RESOURCE:
    {
      /* place user code here */
      a++;
      break;
    }
    case E_OS_STATE:
    {
      a++;
      /* place user code here */
      break;
    }
    case E_OS_VALUE:
    {
      /* place user code here */
      a++;
      break;
    }
    default:
    {
      /* place user code here */
      a++;
      break;
    }
  }
} /* void ErrorHook(StatusType error) */

/*==================[internal function definitions]=========================*/


/** @} doxygen end group definition */
/*==================[end of file]===========================================*/
