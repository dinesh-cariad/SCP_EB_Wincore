/**
 * \file
 *
 * \brief AUTOSAR Dem
 *
 * This file contains the implementation of the AUTOSAR
 * module Dem.
 *
 * \version 6.4.1
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef DEM_INT_STC_H
#define DEM_INT_STC_H

/* This file includes all relevant files for providing internal and external
 * static API declarations and type definitions. */

/*==================[inclusions]============================================*/
/* !LINKSTO dsn.Dem.IncludeStr,1 */

#include <Std_Types.h>                            /* AUTOSAR standard types */
#include <Dem_Stc.h>                   /* Module public static declarations */

/*==================[macros]================================================*/

#if (defined DEM_MAXNUM_ORIGINS)
#error DEM_MAXNUM_ORIGINS already defined
#endif
/** \brief Maximal number of configurable event memories/origins */
#define DEM_MAXNUM_ORIGINS 4U

#if (defined DEM_DTC_ORIGIN_INVALID)
#error DEM_DTC_ORIGIN_INVALID already defined
#endif
/** \brief Internally used to indicate an invalid DTCOrigin value
 ** that cannot be mapped to an event memory */
#define DEM_DTC_ORIGIN_INVALID 0U

#if (defined DEM_DTC_ORIGIN_MAPPING_DCM_IF)
#error DEM_DTC_ORIGIN_MAPPING_DCM_IF already defined
#endif
/** \brief Dcm interface caller identifier for origin mapping according to ASR R4.3 */
#define DEM_DTC_ORIGIN_MAPPING_DCM_IF 0x0U

#if (defined DEM_DTC_ORIGIN_MAPPING_SWC_IF)
#error DEM_DTC_ORIGIN_MAPPING_SWC_IF already defined
#endif
/** \brief SW-C interface caller identifier for origin mapping according to ASR R4.0 */
#define DEM_DTC_ORIGIN_MAPPING_SWC_IF 0x1U

#if (defined DEM_MAX_OCC_ORDER)
#error DEM_MAX_OCC_ORDER already defined
#endif
/** \brief Maximum possible value of occurrence order that can be stored in
 ** EventMemoryEntry.OccOrder
 **
 ** Its value should at least be greater than ::DEM_BSW_ERROR_BUFFER_SIZE and
 ** than the event memory entry sizes.
 */
#define DEM_MAX_OCC_ORDER 65535U

/* !LINKSTO dsn.Dem.StatusBitBehavior.DesignPatternCfg.GroupSize,1 */
#if (defined DEM_STATUS_COUNTER_GROUP_SIZE)
#error DEM_STATUS_COUNTER_GROUP_SIZE already defined
#endif
/** \brief Macro for group size per index table entry */
#define DEM_STATUS_COUNTER_GROUP_SIZE 4U

#if (defined DEM_DATA_PADDING_VALUE)
#error DEM_DATA_PADDING_VALUE already defined
#endif
/** \brief Padding value used for extended and freeze frame data */
#define DEM_DATA_PADDING_VALUE 0xFFU

#if (defined DEM_BITFIELD_VALUE_ALL_GROUPS)
#error DEM_BITFIELD_VALUE_ALL_GROUPS already defined
#endif
/** \brief "all groups" value for Dem_DTCSetting */
#define DEM_BITFIELD_VALUE_ALL_GROUPS  0xFFFFFFFFU

#if (defined DEM_AGING_CYC_CNT_DISABLED)
#error DEM_AGING_CYC_CNT_DISABLED already defined
#endif
/** \brief Macro for disabled obdAgingCycleCounterThreshold
 **
 ** The value is always 0 and it is used if DEM_USE_AGING is switched off. */
#define DEM_AGING_CYC_CNT_DISABLED 0U

/* !LINKSTO dsn.Dem.ProcessEventsPerCycle.SplitEventsPerCriticalSection,1 */
#if (defined DEM_MAX_NUM_EVENTS_PER_CRITICAL_SECTION)
#error DEM_MAX_NUM_EVENTS_PER_CRITICAL_SECTION already defined
#endif
/** \brief Maximum number of processed events per critical section in
 ** Dem_OpCycleProcessCycleStart() function
 **
 ** The precompiler constant is always set to value 10 and
 ** it is not configurable due to maintainability. */
#define DEM_MAX_NUM_EVENTS_PER_CRITICAL_SECTION       10U

/** \brief Macro for invalid node id */
#if (defined DEM_J1939_INVALID_NODE_ID)
#error DEM_J1939_INVALID_NODE_ID already defined
#endif
#define DEM_J1939_INVALID_NODE_ID 255U

/*------------------[defines for debouncing]--------------------------------*/

#ifndef DEM_EVENT_STATUS_UNQUALIFIED
/** \brief Event status is not yet qualified
 **
 ** This is an extension of DEM_EVENT_STATUS_* macros, used for the internal
 ** debouncing status data. */
#define DEM_EVENT_STATUS_UNQUALIFIED   0xFFU
#endif

#if (defined DEM_FDC_VALUE_FAILED)
#error DEM_FDC_VALUE_FAILED already defined
#endif
/** \brief FDC value for a qualified FAILED event */
#define DEM_FDC_VALUE_FAILED     127 /* note: this must be a signed value! */

#if (defined DEM_FDC_VALUE_PASSED)
#error DEM_FDC_VALUE_PASSED already defined
#endif
/** \brief FDC value for a qualified PASSED event */
#define DEM_FDC_VALUE_PASSED    -128 /* note: this must be a signed value! */

/*------------------[defines for ClearDtc]----------------------------------*/

#if (defined DEM_CLEARDTC_STATE_IDLE)
#error DEM_CLEARDTC_STATE_IDLE already defined
#endif
/** \brief Run state value of an async. ClearDtc process: Not running, IDLE */
#define DEM_CLEARDTC_STATE_IDLE 0U

#if (defined DEM_CLEARDTC_STATE_PROCESSING_RAM)
#error DEM_CLEARDTC_STATE_PROCESSING_RAM already defined
#endif
/** \brief Run state value of an async. ClearDtc process: Running, PROCESSING_RAM */
#define DEM_CLEARDTC_STATE_PROCESSING_RAM 1U

#if (defined DEM_CLEARDTC_STATE_PROCESSING_NVRAM)
#error DEM_CLEARDTC_STATE_PROCESSING_NVRAM already defined
#endif
/** \brief Run state value of an async. ClearDtc process: Running, PROCESSING_NVRAM */
#define DEM_CLEARDTC_STATE_PROCESSING_NVRAM 2U

#if (defined DEM_CLEARDTC_STATE_FAILED)
#error DEM_CLEARDTC_STATE_FAILED already defined
#endif
/** \brief Run state value of an async. ClearDtc process: Done with PROCESSING_NVRAM, FAILED */
#define DEM_CLEARDTC_STATE_FAILED 3U

#if (defined DEM_CLEARDTC_STATE_FINISHED)
#error DEM_CLEARDTC_STATE_FINISHED already defined
#endif
/** \brief Run state value of an async. ClearDtc process: Done with PROCESSING_NVRAM, FINISHED */
#define DEM_CLEARDTC_STATE_FINISHED 4U

/*-------------[defines for event related data records]---------------------*/

#if (defined DEM_END_OF_RECORD_SEARCH)
#error DEM_END_OF_RECORD_SEARCH already defined
#endif
/** \brief End of search, no more records left for reporting */
#define DEM_END_OF_RECORD_SEARCH                      0xFFU

#if (defined DEM_ALL_RECORD_NUM)
#error DEM_ALL_RECORD_NUM already defined
#endif
/** \brief Record number to request all FF/ED records of the selected DTC */
#define DEM_ALL_RECORD_NUM                            0xFFU

#if (defined DEM_OBD_FF_RECORD_NUM)
#error DEM_OBD_FF_RECORD_NUM already defined
#endif
/** \brief Record number to identify the OBD equivalent FF */
#define DEM_OBD_FF_RECORD_NUM                         0x00U

#if (defined DEM_MAX_FF_RECORD_NUM)
#error DEM_MAX_FF_RECORD_NUM already defined
#endif
/** \brief Maximum value that can be assigned for a FF record */
#define DEM_MAX_FF_RECORD_NUM                         0xFEU

#if (defined DEM_MIN_ED_RECORD_NUM)
#error DEM_MIN_ED_RECORD_NUM already defined
#endif
/** \brief Minimum value that can be assigned for an ED record */
#define DEM_MIN_ED_RECORD_NUM                         0x01U

#if (defined DEM_MIN_OBD_ED_RECORD_NUM)
#error DEM_MIN_OBD_ED_RECORD_NUM already defined
#endif
/** \brief Lowest record number to qualify an ED record as an OBD ED record */
#define DEM_MIN_OBD_ED_RECORD_NUM                     0x90U

#if (defined DEM_MAX_ED_RECORD_NUM)
#error DEM_MAX_ED_RECORD_NUM already defined
#endif
/** \brief Maximum value that can be assigned for an ED record */
#define DEM_MAX_ED_RECORD_NUM                         0xEFU

#if (defined DEM_ALL_OBD_ED_RECORD_NUM)
#error DEM_ALL_OBD_ED_RECORD_NUM already defined
#endif
/** \brief Record number to request all OBD ED records of the selected DTC */
#define DEM_ALL_OBD_ED_RECORD_NUM                     0xFEU

/*------------------[Several helper macros]---------------------------------*/

#if (defined DEM_BIT_FIELD_VALUE_8)
#error DEM_BIT_FIELD_VALUE_8 already defined
#endif
/** \brief Shifts field with offset, and returns the masked value
 **
 ** Macro is intended for Gbi getter functions.
 **
 */
#define DEM_BIT_FIELD_VALUE_8(field, offset, mask) \
  ((uint8) ((uint8) ((uint8) (field) >> (offset)) & (uint8) (mask)))

#if (defined DEM_BIT_FIELD_VALUE_16)
#error DEM_BIT_FIELD_VALUE_16 already defined
#endif
/** \brief Shifts field with offset, and returns the masked value
 **
 ** Macro is intended for Gbi getter functions.
 **
 */
#define DEM_BIT_FIELD_VALUE_16(field, offset, mask) \
  ((uint16) ((uint16) ((uint16) (field) >> (offset)) & (uint16) (mask)))

#if (defined DEM_BIT_FIELD_VALUE_32)
#error DEM_BIT_FIELD_VALUE_32 already defined
#endif
/** \brief Shifts field with offset, and returns the masked value
 **
 ** Macro is intended for Gbi getter functions.
 **
 */
#define DEM_BIT_FIELD_VALUE_32(field, offset, mask) \
  ((uint32) ((uint32) ((uint32) (field) >> (offset)) & (uint32) (mask)))


#if (defined DEM_IS_BIT_SET_IN_ARRAY)
#error DEM_IS_BIT_SET_IN_ARRAY already defined
#endif
/** \brief Is bit set in an uint8-array */
#define DEM_IS_BIT_SET_IN_ARRAY(Array, Idx) \
  (((Array)[(Idx) / 8U] & ((uint8) (1U << ((Idx) % 8U)))) != 0U)

#if (defined DEM_SET_BIT_IN_ARRAY)
#error DEM_SET_BIT_IN_ARRAY already defined
#endif
/** \brief Set bit in an uint8-array */
#define DEM_SET_BIT_IN_ARRAY(Array, Idx) \
  ((Array)[(Idx) / 8U] |= (uint8)  (1U << ((Idx) % 8U)))

#if (defined DEM_CLR_BIT_IN_ARRAY)
#error DEM_CLR_BIT_IN_ARRAY already defined
#endif
/** \brief Clear bit in an uint8-array */
#define DEM_CLR_BIT_IN_ARRAY(Array, Idx) \
  ((Array)[(Idx) / 8U] &= (uint8) ~((uint8) (1U << ((Idx) % 8U))))

#if (defined DEM_MODIFY_BIT_IN_ARRAY)
#error DEM_MODIFY_BIT_IN_ARRAY already defined
#endif
/** \brief Modify bit in an uint8-array */
#define DEM_MODIFY_BIT_IN_ARRAY(Array, Idx, Val) \
  ((Val) == TRUE ? DEM_SET_BIT_IN_ARRAY((Array), (Idx)) : DEM_CLR_BIT_IN_ARRAY((Array), (Idx)))

#if (defined DEM_SET_BIT_32)
#error DEM_SET_BIT_32 already defined
#endif
/** \brief Set bit in an uint32 bitfield */
#define DEM_SET_BIT_32(Bitfield, Idx) \
  ((Bitfield) |= (uint32) ((uint32) 1U << (Idx)))

#if (defined DEM_CLR_BIT_32)
#error DEM_CLR_BIT_32 already defined
#endif
/** \brief Clear bit in an uint32 bitfield */
#define DEM_CLR_BIT_32(Bitfield, Idx) \
  ((Bitfield) &= (uint32) ~( (uint32) ((uint32) 1U << (Idx))))


#if (defined DEM_ATOMIC_SET_BIT_IN_ARRAY_8)
#error DEM_ATOMIC_SET_BIT_IN_ARRAY_8 already defined
#endif
/** \brief Set bit in an uint8-array, atomic */
#define DEM_ATOMIC_SET_BIT_IN_ARRAY_8(ByteArray, BitIndex) \
  do \
  { \
    const uint16 idx = (BitIndex); \
    TS_AtomicSetBit_8(&((ByteArray)[idx / 8U]),(uint8)(idx % 8U)); \
  } while (0)

#if (defined DEM_ATOMIC_CLR_BIT_IN_ARRAY_8)
#error DEM_ATOMIC_CLR_BIT_IN_ARRAY_8 already defined
#endif
/** \brief Clear bit in an uint8-array, atomic */
#define DEM_ATOMIC_CLR_BIT_IN_ARRAY_8(ByteArray, BitIndex) \
  do \
  { \
    const uint16 idx = (BitIndex); \
    TS_AtomicClearBit_8(&((ByteArray)[idx / 8U]), (uint8)(idx % 8U)); \
  } while (0)


#if (defined DEM_ATOMIC_SET_BIT_32)
#error DEM_ATOMIC_SET_BIT_32 already defined
#endif
/** \brief Set bit in an uint32 bitfield, atomic */
#define DEM_ATOMIC_SET_BIT_32(Bitfield, Idx) \
  do { TS_AtomicSetBit_32(&(Bitfield), (Idx)); } while (0)

#if (defined DEM_ATOMIC_CLR_BIT_32)
#error DEM_ATOMIC_CLR_BIT_32 already defined
#endif
/** \brief Clear bit in an uint32 bitfield, atomic */
#define DEM_ATOMIC_CLR_BIT_32(Bitfield, Idx) \
  do { TS_AtomicClearBit_32(&(Bitfield), (Idx)); } while (0)


#if (defined DEM_ATOMIC_SET_BIT_8)
#error DEM_ATOMIC_SET_BIT_8 already defined
#endif
/** \brief Set bit in an uint8 bitfield, atomic */
#define DEM_ATOMIC_SET_BIT_8(Bitfield, Idx) \
  do { TS_AtomicSetBit_8(&(Bitfield), (Idx)); } while (0)

#if (defined DEM_ATOMIC_CLR_BIT_8)
#error DEM_ATOMIC_CLR_BIT_8 already defined
#endif
/** \brief Clear bit in an uint8 bitfield, atomic */
#define DEM_ATOMIC_CLR_BIT_8(Bitfield, Idx) \
  do { TS_AtomicClearBit_8(&(Bitfield), (Idx)); } while (0)


#if (defined DEM_ATOMIC_ASSIGN_8)
#error DEM_ATOMIC_ASSIGN_8 already defined
#endif
/** \brief Write bits in an uint8 bitfield, atomic */
#define DEM_ATOMIC_ASSIGN_8(to, from) TS_AtomicAssign8((to), (from))

#if (defined DEM_ATOMIC_ASSIGN_16)
#error DEM_ATOMIC_ASSIGN_16 already defined
#endif
/** \brief Write bits in an uint16 bitfield, atomic */
#define DEM_ATOMIC_ASSIGN_16(to, from) TS_AtomicAssign16((to), (from))

#if (defined DEM_ATOMIC_ASSIGN_32)
#error DEM_ATOMIC_ASSIGN_32 already defined
#endif
/** \brief Write bits in an uint32 bitfield, atomic */
#define DEM_ATOMIC_ASSIGN_32(to, from) TS_AtomicAssign32((to), (from))


#if (defined DEM_IS_BIT_SET)
#error DEM_IS_BIT_SET already defined
#endif
/** \brief Logical expression of the condition, that at least one bit, set in
 ** "Mask", is set in "Value"
 */
#define DEM_IS_BIT_SET(Value, Mask) \
  (((Value) & (Mask)) != 0U)

#if (defined DEM_BITS_SET)
#error DEM_BITS_SET already defined
#endif

/** \brief Logical expression of the condition, that all bits, set in
 ** "Mask", are set in "Byte"
 */
#define DEM_BITS_SET(Byte, Mask) \
  (((Byte) & (Mask)) == (Mask))

#if (defined DEM_IS_UNMASKED_BIT_SET)
#error DEM_IS_UNMASKED_BIT_SET already defined
#endif
/** \brief Logical expression of the condition, that at least one unmasked bit
 ** is set in "Byte"
 */
#define DEM_IS_UNMASKED_BIT_SET(Byte, Mask) \
  (((Byte) & (uint8)(~((uint8)(Mask)))) != 0U)

#if (defined DEM_SET_MASK)
#error DEM_SET_MASK already defined
#endif
/** \brief Non-atomic 'set bit by mask' macro */
#define DEM_SET_MASK(to, mask) \
  (to) |= (mask)

#if (defined DEM_CLR_MASK_8)
#error DEM_CLR_MASK_8 already defined
#endif
/** \brief Non-atomic 'clear bit by mask' macro */
#define DEM_CLR_MASK_8(to, mask) \
  (to) &= (uint8)(~(mask))

#if (defined DEM_CLR_MASK_16)
#error DEM_CLR_MASK_16 already defined
#endif
/** \brief Non-atomic 'clear bit by mask' macro */
#define DEM_CLR_MASK_16(to, mask) \
  (to) &= (uint16)(~(mask))

#if (defined DEM_ATOMIC_SET_MASK)
#error DEM_ATOMIC_SET_MASK already defined
#endif
/** \brief Atomic 'set bits by mask' macro
 *
 *  This macro is intended to be replaced by platform macros that can use fast
 *  optimized locking mechanism (e.g. INT lock).
 */
#define DEM_ATOMIC_SET_MASK(to, set_mask)       \
  do                                            \
  {                                             \
    DEM_ENTER_EXCLUSIVE_AREA(); \
    DEM_SET_MASK((to), (set_mask));             \
    DEM_EXIT_EXCLUSIVE_AREA();  \
  } while(0)

#if (defined DEM_ATOMIC_CLR_MASK)
#error DEM_ATOMIC_CLR_MASK already defined
#endif
/** \brief Atomic 'clear bit by mask' macro
 *
 *  This macro is intended to be replaced by platform macros that can use fast
 *  optimized locking mechanism (e.g. INT lock).
 */
#define DEM_ATOMIC_CLR_MASK(to, clr_mask)       \
  do                                            \
  {                                             \
    DEM_ENTER_EXCLUSIVE_AREA(); \
    DEM_CLR_MASK_8((to), (clr_mask));           \
    DEM_EXIT_EXCLUSIVE_AREA();  \
  } while(0)

#if (defined DEM_ATOMIC_CHNG_MASK)
#error DEM_ATOMIC_CHNG_MASK already defined
#endif
/** \brief Atomic 'clear bit by mask macro'
 *
 *  This macro is intended to be supported by platform macros that can use fast
 *  optimized locking mechanism (e.g. INT lock).
 */
#define DEM_ATOMIC_CHNG_MASK(to, set_mask, clr_mask) \
  do                                                 \
  {                                                  \
    DEM_ENTER_EXCLUSIVE_AREA();      \
    DEM_SET_MASK((to), (set_mask));                  \
    DEM_CLR_MASK_8((to), (clr_mask));                \
    DEM_EXIT_EXCLUSIVE_AREA();       \
  } while(0)

#if (defined DEM_EXTERNAL_DATA_ELEMENT_SIZE)
#error DEM_EXTERNAL_DATA_ELEMENT_SIZE already defined
#endif
/** \brief Macro to define the access to the Size of the External Data Element */
#define DEM_EXTERNAL_DATA_ELEMENT_SIZE(DataElementIdx) \
  Dem_DataElement[(DataElementIdx)].Size

/*------------------[bit definition for RTE- & C-callback function]---------*/

#if (defined DEM_GEN_CB_INIT_MONITOR)
#error DEM_GEN_CB_INIT_MONITOR already defined
#endif
/** \brief Mask value for property of Init monitor General-callback function */
#define DEM_GEN_CB_INIT_MONITOR                      0x01U


#if (defined DEM_RTE_CB_TRIG_ON_EVST)
#error DEM_RTE_CB_TRIG_ON_EVST already defined
#endif
/** \brief Mask value for property of Trigger on event status RTE-callback
 ** function
 */
#define DEM_RTE_CB_TRIG_ON_EVST                      0x02U

#if (defined DEM_C_CB_TRIG_ON_EVST)
#error DEM_C_CB_TRIG_ON_EVST already defined
#endif
/** \brief Mask value for property of Trigger on event status C-callback
 ** function
 */
#define DEM_C_CB_TRIG_ON_EVST                        0x04U


#if (defined DEM_RTE_CB_TRIG_ON_EVDAT)
#error DEM_RTE_CB_TRIG_ON_EVDAT already defined
#endif
/** \brief Mask value for property of Trigger on event data changed
 ** RTE-callback function
 */
#define DEM_RTE_CB_TRIG_ON_EVDAT                     0x08U

#if (defined DEM_C_CB_TRIG_ON_EVDAT)
#error DEM_C_CB_TRIG_ON_EVDAT already defined
#endif
/** \brief Mask value for property of Trigger on event data changed
 ** C-callback function
 */
#define DEM_C_CB_TRIG_ON_EVDAT                       0x10U


#if (defined DEM_RTE_CB_CLEAR_EVENT_ALLOWED)
#error DEM_RTE_CB_CLEAR_EVENT_ALLOWED already defined
#endif
/** \brief Mask value for property of Clear event allowed RTE-callback function */
#define DEM_RTE_CB_CLEAR_EVENT_ALLOWED               0x20U

#if (defined DEM_C_CB_CLEAR_EVENT_ALLOWED)
#error DEM_C_CB_CLEAR_EVENT_ALLOWED already defined
#endif
/** \brief Mask value for property of Clear event allowed C-callback function */
#define DEM_C_CB_CLEAR_EVENT_ALLOWED                 0x40U


#if (defined DEM_GEN_CB_FAULT_DETECTION_CTR)
#error DEM_GEN_CB_FAULT_DETECTION_CTR already defined
#endif
/** \brief Mask value for property of FDC General-callback function */
#define DEM_GEN_CB_FAULT_DETECTION_CTR               0x80U

/*------------------[Permanent Memory non-volatile storage]-----------------*/
/* !LINKSTO Dem_OBD_0063,1 */
#if (defined DEM_PERMANENT_MEMORY_V_STORED)
#error DEM_PERMANENT_MEMORY_V_STORED already defined
#endif
/** \brief value for Dem_EventMemoryPermanentNonVolatileStored
 * if the corresponding permanent memory entry is not stored non-volatile */
#define DEM_PERMANENT_MEMORY_V_STORED 0U

/* !LINKSTO Dem_OBD_0064,1 */
#if (defined DEM_PERMANENT_MEMORY_STORING_IN_PROCESS)
#error DEM_PERMANENT_MEMORY_STORING_IN_PROCESS already defined
#endif
/** \brief value for Dem_EventMemoryPermanentNonVolatileStored
 * if non-volatile storing of the permanent memory entry is in progress */
#define DEM_PERMANENT_MEMORY_STORING_IN_PROCESS 1U

/* !LINKSTO Dem_OBD_0062,1 */
#if (defined DEM_PERMANENT_MEMORY_NV_STORED)
#error DEM_PERMANENT_MEMORY_NV_STORED already defined
#endif
/** \brief value for Dem_EventMemoryPermanentNonVolatileStored
 * if the corresponding permanent memory entry is stored non-volatile */
#define DEM_PERMANENT_MEMORY_NV_STORED 2U

/*------------------[Development freeze frames]-----------------------------*/
#if (defined DEM_DEV_FF_MAX_CLASS_SIZE)
#error DEM_DEV_FF_MAX_CLASS_SIZE already defined
#endif
/** \brief Macro to get size of largest configured Development Freeze Frames
 ** of the given Development Freeze Frame kind
 **/
#define DEM_DEV_FF_MAX_CLASS_SIZE(DevFFKind) \
  ((DevFFKind) == DEM_DEVFF_A ? \
    DEM_DEV_FFA_MAX_CLASS_SIZE : DEM_DEV_FFB_MAX_CLASS_SIZE)

/*------------------[Calibration unused value]------------------------------*/

/* !LINKSTO dsn.Dem.Calibration.DID.Unused,1 */
#if (defined DEM_FFSEG_IDX_UNUSED)
#error DEM_FFSEG_IDX_UNUSED already defined
#endif
/** \brief Unused value for FF segments */
#define DEM_FFSEG_IDX_UNUSED                0xFFFFU

/* !LINKSTO dsn.Dem.OBD.DemPidIdentifier.Unused,1 */
#if (defined DEM_PID_UNUSED)
#error DEM_PID_UNUSED already defined
#endif
/** \brief Macro to indicate unused PIDs */
#define DEM_PID_UNUSED                      0xFFU

/*==================[type definitions]======================================*/

/*------------------[definitions for DTC selection]-------------------------*/

/** \brief Type concerning DTC selection status per client */
typedef uint8 Dem_SelectorStatusType;

#if (defined DEM_DTC_SELECTOR_NO_REQUEST)
#error DEM_DTC_SELECTOR_NO_REQUEST already defined
#endif
/** \brief No DTC selection requested */
#define DEM_DTC_SELECTOR_NO_REQUEST                       0x00U

#if (defined DEM_DTC_SELECTOR_NOT_PROCESSED)
#error DEM_DTC_SELECTOR_NOT_PROCESSED already defined
#endif
/** \brief DTC selection requested but not yet processed */
#define DEM_DTC_SELECTOR_NOT_PROCESSED                    0x01U

#if (defined DEM_DTC_SELECTOR_WRONG_DTC)
#error DEM_DTC_SELECTOR_WRONG_DTC already defined
#endif
/** \brief No matching DTC in the requested format and origin found */
#define DEM_DTC_SELECTOR_WRONG_DTC                        0x02U

#if (defined DEM_DTC_SELECTOR_WRONG_DTC_ORIGIN)
#error DEM_DTC_SELECTOR_WRONG_DTC_ORIGIN already defined
#endif
/** \brief Requested DTC origin does not exist */
#define DEM_DTC_SELECTOR_WRONG_DTC_ORIGIN                 0x03U

#if (defined DEM_DTC_SELECTOR_DTC_GROUP)
#error DEM_DTC_SELECTOR_DTC_GROUP already defined
#endif
/** \brief DTC selection request is for a DTC group */
#define DEM_DTC_SELECTOR_DTC_GROUP                        0x04U

#if (defined DEM_DTC_SELECTOR_SINGLE_DTC_EVID_CACHED)
#error DEM_DTC_SELECTOR_SINGLE_DTC_EVID_CACHED already defined
#endif
/** \brief Event matching the single DTC selection request found and cached */
#define DEM_DTC_SELECTOR_SINGLE_DTC_EVID_CACHED           0x05U

/*------------------[definitions for record selection]----------------------*/

typedef uint8 Dem_RecordSelectorStatusType;

#if (defined DEM_DATA_RECORD_SELECTOR_NONE)
#error DEM_DATA_RECORD_SELECTOR_NONE already defined
#endif
/** \brief Neither a FF nor an ED record is selected currently. */
#define DEM_DATA_RECORD_SELECTOR_NONE         0x00U

#if (defined DEM_DATA_RECORD_SELECTOR_FF)
#error DEM_DATA_RECORD_SELECTOR_FF already defined
#endif
/** \brief A FF record is selected by calling Dem_SelectFreezeFrameData(). */
#define DEM_DATA_RECORD_SELECTOR_FF           0x01U

#if (defined DEM_DATA_RECORD_SELECTOR_ED)
#error DEM_DATA_RECORD_SELECTOR_ED already defined
#endif
/** \brief An ED record is selected by calling
 **        Dem_SelectExtendedDataRecord(). */
#define DEM_DATA_RECORD_SELECTOR_ED           0x02U

typedef struct
{
  uint8 SelectedRecNum;
             /**< The FF/ED record number selected by the diagnostic client */
  uint8 NextRecNum;
          /**< The record number to be considered next by the APIs that return
            *  the data of stored FF/ED records */
  Dem_RecordSelectorStatusType Status;
         /**< The status of FF/ED record selection by the diagnostic client */
} Dem_RecordSelectionType;

/*------------------[Dem_EventOBDReadinessGroupType]------------------------*/

/** \brief Enum used to specify the Event OBD Readiness group
 **  for PID $01 and PID $41 computation
 **
 ** This parameter is only applicable for
 ** emission-related ECUs.
 ** Type: EcucEnumerationParamDef
 */
typedef uint8 Dem_EventOBDReadinessGroupType;

#ifndef DEM_OBD_RDY_CAT
/** \brief Catalyst - spark */
#define DEM_OBD_RDY_CAT 0x0U
#endif

#ifndef DEM_OBD_RDY_HCCAT
/** \brief Non-Methan HC Catalyst - compr. */
#define DEM_OBD_RDY_HCCAT 0x0U
#endif

#ifndef DEM_OBD_RDY_HTCAT
/** \brief Heated catalyst - spark */
#define DEM_OBD_RDY_HTCAT 0x01U
#endif

#ifndef DEM_OBD_RDY_NOXCAT
/** \brief NOx Catalyst - compr. */
#define DEM_OBD_RDY_NOXCAT 0x01U
#endif

#ifndef DEM_OBD_RDY_EVAP
/** \brief Evaporative system - spark */
#define DEM_OBD_RDY_EVAP 0x02U
#endif

#ifndef DEM_OBD_RDY_SECAIR
/** \brief Secondary air system - spark */
#define DEM_OBD_RDY_SECAIR 0x03U
#endif

#ifndef DEM_OBD_RDY_BOOSTPR
/** \brief Boost Pressure System - compr. */
#define DEM_OBD_RDY_BOOSTPR 0x03U
#endif

#ifndef DEM_OBD_RDY_O2SENS
/** \brief Oxygen sensor - spark */
#define DEM_OBD_RDY_O2SENS 0x05U
#endif

#ifndef DEM_OBD_RDY_EGSENS
/** \brief Exhaust Gas Sensor - compr. */
#define DEM_OBD_RDY_EGSENS 0x05U
#endif

#ifndef DEM_OBD_RDY_O2SENSHT
/** \brief Oxygen sensor heater - spark */
#define DEM_OBD_RDY_O2SENSHT 0x06U
#endif

#ifndef DEM_OBD_RDY_PMFLT
/** \brief Particle Matters Filter - compr. */
#define DEM_OBD_RDY_PMFLT 0x06U
#endif

#ifndef DEM_OBD_RDY_EGR
/** \brief EGR system - spark, compr. */
#define DEM_OBD_RDY_EGR 0x07U
#endif

#ifndef DEM_OBD_RDY_MISF
/** \brief Misfire - spark, compr. */
#define DEM_OBD_RDY_MISF 0x08U
#endif

#ifndef DEM_OBD_RDY_FLSYS
/** \brief Fuel system - spark, compr. */
#define DEM_OBD_RDY_FLSYS 0x09U
#endif

#ifndef DEM_OBD_RDY_CMPRCMPT
/** \brief Comprehensive component - spark, compr. */
#define DEM_OBD_RDY_CMPRCMPT 0x0AU
#endif

#ifndef DEM_OBD_RDY_NONE
/** \brief None - spark, compr. */
#define DEM_OBD_RDY_NONE 0x0BU
#endif

#ifndef DEM_OBD_RDY_AC
/** \brief A/C system component - spark */
#define DEM_OBD_RDY_AC 0x0CU
#endif

#ifndef DEM_OBD_RDY_FLSYS_NONCONT
/** \brief Non Contious Fuel system - spark, compr. */
#define DEM_OBD_RDY_FLSYS_NONCONT 0x0DU
#endif

#if (defined DEM_OBD_RDY_PID01PID41_CNT)
#error DEM_OBD_RDY_PID01PID41_CNT already defined
#endif
/** \brief Count of Readiness Groups reported by PID$01 and PID$41 */
#define DEM_OBD_RDY_PID01PID41_CNT 0x0BU

#if (defined DEM_IGNITION_SPARK)
#error DEM_IGNITION_SPARK already defined
#endif
#define DEM_IGNITION_SPARK 0U

#if (defined DEM_IGNITION_COMPRESSION)
#error DEM_IGNITION_COMPRESSION already defined
#endif
#define DEM_IGNITION_COMPRESSION 1U

/*-------[definitions for Dem_EventOBDReadinessGroupType for J1939]--------*/
#if( (DEM_J1939_SUPPORT == STD_ON)                     && \
     (DEM_J1939_READINESS1_SUPPORT == STD_ON)          && \
     (DEM_OBD_ENGINE_TYPE == DEM_IGNITION_COMPRESSION)    \
   )

#if (defined DEM_OBD_J1939_RDY_HCCAT)
#error DEM_OBD_J1939_RDY_HCCAT already defined
#endif
/** \brief Non-Methan HC Catalyst - compr. for J1939 */
#define DEM_OBD_J1939_RDY_HCCAT 0x0CU

#if (defined DEM_OBD_J1939_RDY_NOXCAT)
#error DEM_OBD_J1939_RDY_NOXCAT already defined
#endif
/** \brief NOx Catalyst - compr. for J1939 */
#define DEM_OBD_J1939_RDY_NOXCAT 0x0BU

#if (defined DEM_OBD_J1939_RDY_NONE)
#error DEM_OBD_J1939_RDY_NONE already defined
#endif
/** \brief  None for J1939 */
#define DEM_OBD_J1939_RDY_NONE 0x10

#if (defined DEM_OBD_J1939_RDY_BOOSTPR)
#error DEM_OBD_J1939_RDY_BOOSTPR already defined
#endif
/** \brief Boost Pressure System - compr. for J1939 */
#define DEM_OBD_J1939_RDY_BOOSTPR 0x09U

#if (defined DEM_OBD_J1939_RDY_EGSENS)
#error DEM_OBD_J1939_RDY_EGSENS already defined
#endif
/** \brief Exhaust Gas Sensor - compr. for J1939 */
#define DEM_OBD_J1939_RDY_EGSENS 0x05U

#if (defined DEM_OBD_J1939_RDY_PMFLT)
#error DEM_OBD_J1939_RDY_PMFLT already defined
#endif
/** \brief Particle Matters Filter- compr. for J1939 */
#define DEM_OBD_J1939_RDY_PMFLT 0x0AU

#if (defined DEM_OBD_J1939_RDY_EGR)
#error DEM_OBD_J1939_RDY_EGR already defined
#endif
/** \brief EGR system - compr. for J1939 */
#define DEM_OBD_J1939_RDY_EGR 0x07U

#endif

/*------------------[Dem_EventKindType]-------------------------------------*/

/** \brief Type for Dem_GbiEventKind() */
typedef uint8 Dem_EventKindType;

#if (defined DEM_EVENT_KIND_BSW)
#error DEM_EVENT_KIND_BSW already defined
#endif
/** \brief Event kind BSW */
#define DEM_EVENT_KIND_BSW           0x00U

#if (defined DEM_EVENT_KIND_SWC)
#error DEM_EVENT_KIND_SWC already defined
#endif
/** \brief Event kind SWC */
#define DEM_EVENT_KIND_SWC           0x01U

/*------------------[Dem_FFKindType]----------------------------------------*/

/** \brief Type of Freeze Frame Class kind
 **
 ** This type is used for two cases:
 ** - selecting the common freeze frame via Dem_CommonPartFFClassIdx[] and
 ** - generic development freeze frames handling (via loop for DevFF A&B) */
typedef uint8 Dem_FFKindType;

#if (defined DEM_DEVFF_A)
#error DEM_DEVFF_A already defined
#endif
/** \brief Development freeze frame A - an entry is located in the
 **        DevFFAData[] */
#define DEM_DEVFF_A          0x00U

#if (defined DEM_DEVFF_B)
#error DEM_DEVFF_B already defined
#endif
/** \brief Development freeze frame B - an entry is located in the
 **        DevFFBData[] */
#define DEM_DEVFF_B          0x01U

#if (defined DEM_REGULAR_FF)
#error DEM_REGULAR_FF already defined
#endif
/** \brief Regular freeze frame - an entry is located in the event memory */
#define DEM_REGULAR_FF       0x02U

/*--------------------------------------------------------------------------*/

/** \brief Debounce algorithm type */
typedef enum
{
  DEM_DEBOUNCE_MONITOR,
                   /**< value/index signaling "monitor internal" debouncing */
  DEM_DEBOUNCE_COUNTERBASED,
                        /**< value/index signaling counter based debouncing */
  DEM_DEBOUNCE_TIMEBASED,
                           /**< value/index signaling time based debouncing */
  DEM_DEBOUNCE_FREQUENCYBASED,
                      /**< value/index signaling frequency based debouncing */
  DEM_MAXNUM_DEBOUNCECLASSES
           /**< Maximal number of different configurable debouncing classes */
} Dem_DebounceType;

/** \brief This type is used for the state-values of the initialization state
 ** of the Dem
 */
typedef enum {
  DEM_UNINITIALIZED,                            /**< Dem is not initialized */
  DEM_PREINITIALIZED,                           /**< Dem is pre-initialized */
  DEM_INITIALIZED,                             /**< Dem is full initialized */
  DEM_POSTINITIALIZED                          /**< Dem is post-initialized */
} Dem_InitializationStateType;

/** \brief This type is for local counting of event Ids
 **
 ** Note: It is derived from the maximal size of the type ::Dem_EventIdType.
 */
typedef uint16_least Dem_EvIdLeastType;

/** \brief Event memory entry size type */
typedef uint8 Dem_SizeEvMemEntryType;

/*------------------[definitions for Data Element/FF/ED class types]--------*/

/*------------------[definitions for Data Element type]---------------------*/

/** \brief Number of assigned data elements type
 **
 ** Number of data elements assigned to a FF segment or ED segment.
 **
 ** Note: The range (maximum of 255) of this type is limited by the
 ** configuration (multiplicity of parameters DemDidDataElementClassRef and
 ** DemDataElementClassRef).
 */
typedef uint8 Dem_NumDataElementsType;

/** \brief Index of data element type
 **
 ** Index concerning ::Dem_CBDataElement[].
 **
 ** Note: The range (maximum of 65535) of this type is limited by the
 ** configuration (multiplicity of parameter DemDataElementClass).
 */
typedef uint16 Dem_DataElementIdxType;

/** \brief Offset for data element
 **
 ** Offset will not be greater than 65535.
 ** This is derived from the maximum size of a FF segment / ED segment.
 */
typedef uint16 Dem_DataElementOffsetType;

/*------------------[definitions for FF segment types]----------------------*/

/** \brief Layout information of freeze frame segments (DIDs) */
typedef struct
{
  CONSTP2CONST(Dem_DataElementIdxType, AUTOMATIC, DEM_CONST) DataElementIdx;
             /**< Indexes to data elements (array - :: Dem_DataElementType) */
  CONSTP2CONST(Dem_DataElementOffsetType, AUTOMATIC, DEM_CONST)
    DataElementOffset;              /**< Positions of data elements (array) */
  const uint16 DataId;          /**< DID identifier of freeze frame segment */
  const Dem_NumDataElementsType NumFFDataElements;
                                               /**< Number of data elements */
} Dem_FFSegmentType;

/** \brief Number of assigned freeze frame segments to freeze frame type
 **
 ** Number of DIDs assigned to a FF class.
 **
 ** Note: The range (maximum of 255) of this type is limited by the
 ** configuration (multiplicity of parameter DemDidClassRef).
 */
typedef uint8 Dem_NumFFSegsType;

/*------------------[definitions for FF class types]------------------------*/

/** \brief Index of freeze frame class type
 **
 ** NumFFClass variables are also of this type.
 */
typedef uint16 Dem_FFClassIdxType;

/** \brief  Freeze frame size type
 **
 ** Freeze frame class size will not be greater than 65535.
 ** There are checks for DemFreezeFrameClass which limit their sizes.
 ** DemDidClass size is checked to be smaller than 256 in XDM.
 */
typedef uint16 Dem_SizeFFType;

/*------------------[definitions for ED segment types]----------------------*/

/** \brief Layout information of extended data segments (Records) */
typedef struct
{
  CONSTP2CONST(Dem_DataElementIdxType, AUTOMATIC, DEM_CONST) DataElementIdx;
        /**< Indexes to data elements (array - :: Dem_DataElementType) */
  CONSTP2CONST(Dem_DataElementOffsetType, AUTOMATIC, DEM_CONST)
    DataElementOffset;              /**< Positions of data elements (array) */
  const Dem_NumDataElementsType NumEDDataElements;
                                               /**< Number of data elements */
  const uint8 RecNum;                      /**< Extended data Record Number */
  const boolean Update;             /**< Extended data Record update status */
  const boolean AlwaysAvailable;   /**< Readable without event memory entry */
} Dem_EDSegmentType;

/* note: no Dem_NumEDSegsType necessary - see Dem_EDSegIdxType */

/** \brief Index of extended data segment type
 **
 ** Index concerning ::Dem_EDSegments[].
 **
 ** Note: The range (maximum of 253) of this type is limited by the
 ** record number (equals 1 byte), i.e. configuration (multiplicity of
 ** parameter DemExtendedDataRecordClass).
 **
 ** Note: The range for ED segments per ED class is defined by ISO14229-1
 ** (ED RecordNumber = 0x01 ... 0xFE).
 **
 ** Note: The maximum number of ED segments per ED class (EDClass.NumEDSegs)
 ** also uses this type (based on configuration parameter DemDidClassRef),
 ** since the value fits into this range.
 */
typedef uint8 Dem_EDSegIdxType;

/** \brief Start Byte of extended data segment type
 **
 ** Start bytes will not be greater than 65535.
 ** Dem_DltGetAllExtendedDataRecords() indirectly limits the size of a
 ** extended data (which is the sum of all assigned ED record sizes)
 ** to maximal 65535 by its parameter 'BufSize'.
 */
typedef uint16 Dem_EDStartByteType;

/*------------------[definitions for ED class types]------------------------*/

/** \brief Layout information of extended data classes */
typedef struct
{
  CONSTP2CONST(Dem_EDSegIdxType, AUTOMATIC, DEM_CONST) EDSegIdx;
       /**< Indexes to extended data segments (array - ::Dem_EDSegmentType) */
  CONSTP2CONST(Dem_EDStartByteType, AUTOMATIC, DEM_CONST) StartByte;
                           /**< Positions of extended data segments (array) */
  const Dem_EDSegIdxType NumEDSegs;
                            /**< Number of extended data segments (Records) */
} Dem_EDClassType;

/** \brief Index of extended data class type
 **
 ** NumEDClass variables are also of this type.
 */
typedef uint8 Dem_EDClassIdxType;

/** \brief  Extended data size type
 **
 ** Extended data class size will not be greater than 65535.
 ** There are checks for DemExtendedDataClass which limit their sizes.
 ** DemExtendedDataRecordClass size is checked to be smaller than 256 in XDM.
 */
typedef uint16 Dem_SizeEDType;

/*--------------------------------------------------------------------------*/

/** \brief This type is used for Dem_EnCondGrpStatusCntLink
 **
 ** It's uint8 because max. 254 enable condition groups can be configured.
 */
typedef uint8 Dem_EnCondGrpType;

/** \brief This type is used for Dem_EnCondGrpStatusCounter[]
 **
 ** It's uint8 because max. 255 enable conditions per group.
 */
typedef uint8 Dem_EnCondGrpStatusCounterType;

/** \brief This type is used by Dem_GbiEnCondGrpIdx() to access
 ** index of Dem_EnCondGrpStatusCounter[] stored in event description.
 **
 ** It's uint8 because max. 254 enable condition groups.
 */
typedef uint8 Dem_EnCondGrpIdxType;

/** \brief This type is used as index type for the enable conditions
 **
 ** Its size (uint8) is given by API Dem_SetEnableCondition().
 **
 ** NumEnCond values are also of this type.
 */
typedef uint8 Dem_EnableConditionIdType;

/** \brief This type is for Dem_EnCondGrpStatus[] */
typedef uint8 Dem_EnCondGrpStatusType;

/** \brief This type is used as index type for the error-queue
 **
 ** Its size (uint8) is given by configuration. */
typedef uint8 Dem_ErrorQueueIdxType;

/** \brief Type for error-queue entries
 **
 ** - e[13]: event Id (limited by configuration)
 ** - f:     first failed flag (in this operation cycle)
 ** - ss:    passed/failed/unconfirmed status (as per ::Dem_EventStatusType)
 **
 ** Memory layout:
 ** xxxx xxxx  xxxx xxxx
 ** ssfe eeee  eeee eeee */
typedef uint16 Dem_ErrorQueueEntryType;

/** \brief Type of one stored development Freeze Frame.
 **        Either Dev FF A or Dev FF B */
typedef struct
{
  /**< Index for the freeze frame class of development Freeze Frame */
  Dem_FFClassIdxType DevFFClassIdx;
  /**< Freeze Frame Record number of development Freeze Frame */
  uint8 DevFFNum;
} Dem_EventDescDevFFType;

/** \brief Number of assigned freeze frame SPNs to J1939 freeze frame type
 **
 ** Number of SPNs assigned to a J1939 FF class.
 **
 ** Note: The range (maximum of 255) of this type is limited by the
 ** configuration (multiplicity of parameter DemSPNClassRef).
 */
typedef uint8 Dem_NumFFSPNsType;

/** \brief Index of J1939 freeze frame SPN type
 **
 ** Index concerning ::Dem_J1939FFSPNs[].
 **
 ** Note: The range (maximum of 524287) of this type is limited by the
 ** configuration (multiplicity of parameter DemSPNId).
 */
typedef uint32 Dem_J1939FFSPNIdxType;

/** \brief Index of J1939 freeze frame class
 **
 ** Index concerning ::Dem_J1939FFClass[].
 */
typedef uint16 Dem_J1939FFClassIdxType;

/** \brief  J1939 Freeze frame size type
 **
 ** J1939 Freeze frame class size will not be greater than 255 bytes.
 */
typedef uint16 Dem_SizeJ1939FFType;

/** \brief Layout information of J1939 freeze frame classes */
typedef struct
{
  CONSTP2CONST(Dem_J1939FFSPNIdxType, AUTOMATIC, DEM_CONST) J1939FFSPNIdx;
        /**< Indices to J1939 freeze frame SPN (array - ::Dem_J1939FFSPNType) */
  const Dem_NumFFSPNsType NumJ1939FFSPNs;
                                /**< Number of freeze frame SPNs */
} Dem_J1939FFClassType;

/** \brief  DTCOrigin mapping type
 **
 ** This type is used for selecting a DTCOrigin mapping variant.
 */
typedef uint8 Dem_DTCOriginMappingType;

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

#define DEM_START_SEC_CONST_8
#include <Dem_MemMap.h>

/** \brief Array holding the sizes of the 3 event memories */
extern CONST(Dem_SizeEvMemEntryType, DEM_CONST)
  Dem_SizeEventMem[DEM_MAXNUM_ORIGINS];

#define DEM_STOP_SEC_CONST_8
#include <Dem_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

#define DEM_START_SEC_VAR_INIT_UNSPECIFIED
#include <Dem_MemMap.h>

/** \brief Status of the Dem
 **
 ** This flag is used to store the initialization state of the Dem. It's only
 ** be modified (written) by the functions Dem_PreInit(), Dem_Init() and
 ** Dem_Shutdown().
 */
extern VAR(Dem_InitializationStateType, DEM_VAR) Dem_InitializationState;

#define DEM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <Dem_MemMap.h>

#define DEM_START_SEC_VAR_CLEARED_8
#include <Dem_MemMap.h>

/** \brief Flag for locking DTC setting */
extern VAR(boolean, DEM_VAR_CLEARED) Dem_DTCSettingAllowed;

#define DEM_STOP_SEC_VAR_CLEARED_8
#include <Dem_MemMap.h>

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef DEM_INT_STC_H */
/*==================[end of file]===========================================*/
