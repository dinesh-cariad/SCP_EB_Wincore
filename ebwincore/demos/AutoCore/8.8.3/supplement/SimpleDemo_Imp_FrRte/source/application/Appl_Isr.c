/**
 * \file
 *
 * \brief AUTOSAR Workflows
 *
 * This file contains the implementation of the AUTOSAR
 * module Workflows.
 *
 * \version 2.3.2
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]=============================================*/

#include <os.h>                    /* AUTOSAR OS */
#include <FrIf_Lcfg.h>             /* AUTOSAR FlexRay interface */

/*==================[interrupt handlers]=====================================*/

/* ISR for FlexRay */
ISR(ISR_FlexRay)
{
  FrIf_JobListExec_0();
} /* end of ISR_FlexRay_0 */

/*==================[end of file]============================================*/
