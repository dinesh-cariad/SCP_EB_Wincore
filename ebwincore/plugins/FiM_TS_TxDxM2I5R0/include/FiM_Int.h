/**
 * \file
 *
 * \brief AUTOSAR FiM
 *
 * This file contains the implementation of the AUTOSAR
 * module FiM.
 *
 * \version 2.5.3
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef FIM_INT_H
#define FIM_INT_H

/*==================[includes]===============================================*/
/* !LINKSTO dsn.FiM.IncludeStr,1 */

#include <FiM.h>     /* public dependent header */

/*==================[macros]=================================================*/

#if (defined FIM_CONFIG_ACCESS_EVENT)
#error FIM_CONFIG_ACCESS_EVENT already defined
#endif
/** \brief Define for configuration ordered by event id */
#define FIM_CONFIG_ACCESS_EVENT                       0U

#if (defined FIM_CONFIG_ACCESS_FID)
#error FIM_CONFIG_ACCESS_FID already defined
#endif
/** \brief Define for configuration ordered by FID */
#define FIM_CONFIG_ACCESS_FID                         1U

/* *** Configuration offsets *** */

#if (defined FIM_CONFIG_INHIBITION_MASK_OFFSET)
#error FIM_CONFIG_INHIBITION_MASK_OFFSET already defined
#endif
/** \brief Define for configuration offset of inhibition mask */
#define FIM_CONFIG_INHIBITION_MASK_OFFSET             13U

#if (defined FIM_CONFIG_INHIBITION_MASK)
#error FIM_CONFIG_INHIBITION_MASK already defined
#endif
/** \brief Inhibition mask */
#if (FIM_EXTENDED_LEVEL == FIM_NO_EXTENSION)
#define FIM_CONFIG_INHIBITION_MASK                    3U
#else
#define FIM_CONFIG_INHIBITION_MASK                    7U
#endif


#if (defined FIM_EVENT_MASK)
#error FIM_EVENT_MASK already defined
#endif
/** \brief Event Id mask */
#define FIM_EVENT_MASK                                0x1FFFU


/* *** Extended configuration offsets *** */

#if (defined FIM_EXT_CONFIG_CONDITION_EVENTID_OFFSET)
#error FIM_EXT_CONFIG_CONDITION_EVENTID_OFFSET already defined
#endif
/** \brief Extended configuration condition event id offset */
#define FIM_EXT_CONFIG_CONDITION_EVENTID_OFFSET       0U

#if (defined FIM_EXT_CONFIG_CONDITION_EVENTID_MASK)
#error FIM_EXT_CONFIG_CONDITION_EVENTID_MASK already defined
#endif
/** \brief Extended configuration condition event id offset */
#define FIM_EXT_CONFIG_CONDITION_EVENTID_MASK         0x7FFFU


#if (defined FIM_EXT_CONFIG_SYMPTOM_OFFSET)
#error FIM_EXT_CONFIG_SYMPTOM_OFFSET already defined
#endif
/** \brief Extended configuration symptom bit offset */
#define FIM_EXT_CONFIG_SYMPTOM_OFFSET                 15U

#if (defined FIM_EXT_CONFIG_SYMPTOM_MASK)
#error FIM_EXT_CONFIG_SYMPTOM_MASK already defined
#endif
/** \brief Extended configuration symptom bit offset */
#define FIM_EXT_CONFIG_SYMPTOM_MASK                   1U


/*==================[type definitions]=======================================*/

/*------------------[types for extended support]-------------------*/

#if (FIM_EXTENDED_LEVEL != FIM_NO_EXTENSION)

/* !LINKSTO dsn.FIM.DataStructure.ExtendedConfiguration,1 */
typedef uint16 FiM_FidExtendedCfgType;

#if (FIM_EXTENDED_LEVEL == FIM_EXTENSION_LVL2)
/* !LINKSTO dsn.FIM.DataStructure.ExtendedConfigurationLvl2,1 */
typedef struct
{
  Dem_EventIdType RecoveryEventId;
  Dem_EventIdType ForceReleaseEventId;
} FiM_FidExtendedCfgLvl2Type;
#endif

#endif

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

#if (FIM_CONFIG_ACCESS == FIM_CONFIG_ACCESS_EVENT)
#define FIM_START_SEC_CALIB_16
#include <FiM_MemMap.h>

/** \brief FiM configuration start index */
extern CONST(FiM_EvtConfigType, FIM_CONST)
  FiM_EvtConfig[FIM_EVT_STARTINDEX_NUM];

/** \brief FiM configuration data referenced by config start index */
extern CONST(FiM_EvtConfigDataType, FIM_CONST)
  FiM_EvtConfigData[FIM_EVT_CONFIGDATA_NUM];

#define FIM_STOP_SEC_CALIB_16
#include <FiM_MemMap.h>
#else
#define FIM_START_SEC_CONST_UNSPECIFIED
#include <FiM_MemMap.h>

/** \brief FiM configuration start index */
extern CONST(FiM_FidConfigStartIndexType, FIM_CONST)
  FiM_FidConfigStartIndex[FIM_FID_STARTINDEX_NUM];

#define FIM_STOP_SEC_CONST_UNSPECIFIED
#include <FiM_MemMap.h>

#define FIM_START_SEC_CALIB_16
#include <FiM_MemMap.h>

/** \brief FiM configuration data referenced by config start index */
extern CONST(FiM_FidConfigDataType, FIM_CONST)
  FiM_FidConfigData[FIM_FID_CONFIGDATA_NUM];

#define FIM_STOP_SEC_CALIB_16
#include <FiM_MemMap.h>
#endif /* (FIM_CONFIG_ACCESS == FIM_CONFIG_ACCESS_EVENT) */

#if (FIM_EXTENDED_LEVEL != FIM_NO_EXTENSION)
#define FIM_START_SEC_CALIB_16
#include <FiM_MemMap.h>

/** \brief Extended configuration */
extern CONST(FiM_FidExtendedCfgType, FIM_CONST)
  FiM_ExtendedConfiguration[FIM_EXT_CONFIG_NUM];

#if (FIM_EXTENDED_LEVEL == FIM_EXTENSION_LVL2)
/** \brief Extended configuration */
extern CONST(FiM_FidExtendedCfgLvl2Type, FIM_CONST)
  FiM_ExtendedConfigurationLvl2[FIM_EXT_CONFIG_NUM];
#endif

/** \brief High priority EventId */
extern CONST(Dem_EventIdType, FIM_CONST)
  FiM_HighPrioEventId[FIM_EXT_MAX_HIGH_PRIO_EVENT_TO_FID_LINKS];

#if (FIM_FID_NUM <= 256)
#define FIM_STOP_SEC_CALIB_16
#include <FiM_MemMap.h>

#define FIM_START_SEC_CALIB_8
#include <FiM_MemMap.h>
#endif

/** \brief High priority Fid */
extern CONST(FiM_FunctionIdType, FIM_CONST)
  FiM_HighPrioFid[FIM_EXT_MAX_HIGH_PRIO_EVENT_TO_FID_LINKS];

#if (FIM_FID_NUM > 256)
#define FIM_STOP_SEC_CALIB_16
#include <FiM_MemMap.h>

#define FIM_START_SEC_CALIB_8
#include <FiM_MemMap.h>
#endif

  /** \brief Extended configuration for Message Id */
extern CONST(FiM_MessageIdType, FIM_CONST)
  FiM_MessageIdConfiguration[FIM_EXT_CONFIG_NUM];

#define FIM_STOP_SEC_CALIB_8
#include <FiM_MemMap.h>
#endif /* (FIM_EXTENDED_LEVEL != FIM_NO_EXTENSION) */

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef FIM_INT_H */
/*==================[end of file]============================================*/
