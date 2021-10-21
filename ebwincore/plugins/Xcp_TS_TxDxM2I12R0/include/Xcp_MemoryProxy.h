#ifndef XCP_MEMORY_PROXY_H
#define XCP_MEMORY_PROXY_H

/*==[Includes]================================================================*/

#include <Xcp_Cfg.h>
#include <Std_Types.h>

#if (XCP_MEMORY_ACCESS_AREAS == STD_ON)

#include <Xcp_Int_Cfg.h>
#include <Rte_Type.h>
#include <Atomics.h>

/*==[Macros definitions]======================================================*/

#define XCP_MEMORY_PROXY_AVAILABLE 0
#define XCP_MEMORY_PROXY_READY 1
#define XCP_MEMORY_PROXY_LOCKED 2

#define XCP_MEMORYPROXYDATA_UPLOAD 0
#define XCP_MEMORYPROXYDATA_MODIFY_BITS 1
#define XCP_MEMORYPROXYDATA_DOWNLOAD 2
#define XCP_MEMORYPROXYDATA_EVENT_PROCESSOR 3
#define XCP_MEMORYPROXYDATA_STIMULATION     4
#define XCP_MEMORYPROXYDATA_SAMPLE          5
#define XCP_MEMORYPROXYDATA_STIMULATION_0   6
#define XCP_MEMORYPROXYDATA_STIMULATION_1   7
#define XCP_MEMORYPROXYDATA_SAMPLE_0       8
#define XCP_MEMORYPROXYDATA_SAMPLE_1       9

/*==[Types declarations]======================================================*/

/*==[Declaration of constants with external linkage]==========================*/

/*==[Declaration of variables with external linkage]==========================*/

#define XCP_START_SEC_VAR_8
#include <Xcp_MemMap.h>

extern VAR(uint8, XCP_VAR)    Xcp_MemoryProxyBuffer[XCP_MEMORY_PROXY_BUFFER_SIZE];

#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON)
extern VAR(uint8, XCP_VAR) Xcp_ComandType;
extern VAR(uint8, XCP_VAR) Xcp_MemoryAreaIdx;
#endif

#define XCP_STOP_SEC_VAR_8
#include <Xcp_MemMap.h>

#define XCP_START_SEC_VAR_UNSPECIFIED
#include <Xcp_MemMap.h>

extern VAR(Atomic_t, XCP_VAR) Xcp_MemoryProxyState;
extern VAR(Atomic_t, XCP_VAR) Xcp_MemoryProxyInterrupted;

#define XCP_STOP_SEC_VAR_UNSPECIFIED
#include <Xcp_MemMap.h>

/*==[Declaration of functions with external linkage]==========================*/

#define XCP_START_SEC_CODE
#include <Xcp_MemMap.h>

extern FUNC(void, XCP_CODE) Xcp_MemoryProxyHandler(Xcp_MemoryProxyDataType* data);

#define XCP_STOP_SEC_CODE
#include <Xcp_MemMap.h>

#endif /* XCP_MEMORY_ACCESS_AREAS == STD_ON */

#endif /* XCP_MEMORY_PROXY_H */

