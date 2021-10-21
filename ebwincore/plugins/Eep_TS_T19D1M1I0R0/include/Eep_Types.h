#ifndef EEP_TYPES_H
#define EEP_TYPES_H

/**
 * \file
 *
 * \brief AUTOSAR Eep
 *
 * This file contains the implementation of the AUTOSAR
 * module Eep.
 *
 * \version 1.0.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
/*==================[includes]==============================================*/
#include <Std_Types.h>                     /* AUTOSAR standard types */
#include <MemIf_Types.h>                   /* includes MemIf shared typedef */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/
typedef P2FUNC(void,EEP_CODE,Eep_JobEndNotificationPtrType)(void);
typedef P2FUNC(void,EEP_CODE,Eep_JobErrorNotificationPtrType)(void);

typedef struct
{
  MemIf_ModeType defaultMode;
  uint32 eepromSize;
  uint32 eepFastReadBlockSize;
  uint32 eepFastWriteBlockSize;
  uint32 eepNormalReadBlockSize;
  uint32 eepNormalWriteBlockSize;
  Eep_JobEndNotificationPtrType jobEndNotificationPtr;
  Eep_JobErrorNotificationPtrType jobErrorNotificationPtr;
}
Eep_ConfigType; /* typedef describes Eep module initialization data */

typedef enum
{
  EEP_JOB_READ = 0,
  EEP_JOB_WRITE,
  EEP_JOB_ERASE,
  EEP_JOB_COMPARE
}
Eep_JobType; /* typedef describes Eep module internal job types */

typedef uint32 Eep_AddressType;
typedef uint32 Eep_LengthType;

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
#endif /* ifndef EEP_TYPES_H */
