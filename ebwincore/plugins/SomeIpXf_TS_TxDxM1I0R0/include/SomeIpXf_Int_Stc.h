#ifndef SOMEIPXF_INT_STC_H
#define SOMEIPXF_INT_STC_H

/**
 * \file
 *
 * \brief AUTOSAR SomeIpXf
 *
 * This file contains the implementation of the AUTOSAR
 * module SomeIpXf.
 *
 * \version 1.0.47
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 20.10 (advisory)
 * The # and ## preprocessor operators should not be used.
 *
 * Reason:
 * ## operator is used for code generation via pre-processor.
 *
 */

/*==================[inclusions]============================================*/
#include <SomeIpXf_S_Int_Stc.h>        /* Internal static declarations (Safe version) */

/*==================[macros]================================================*/

#if (defined SOMEIPXF_SR_DEF_FUNC_XF) /* to prevent double declaration */
#error SOMEIPXF_SR_DEF_FUNC_XF already defined
#endif /* if (defined SOMEIPXF_SR_DEF_FUNC_XF) */
/* Macro to define transformer API function for sender/receiver communication
 * Parameters:
 * XfrmName   Name of the transformer
 * DataType   Data type of data element handed over for serialization
 * PassMethod   Method for passing data element to method (PASSBYREF or PASSBYVAL)
 * InterfaceVersion   The interface version the transformer shall use
 * MessageType   The Message Type which shall be placed into the SOME/IP header
 * TypeConfigName   Type of the data element which is part of configuration name
 * Endianess   Endianess of transformation which is part of the configuration name (LE, BE or OP)
 * SessionHandling_Status SessionHandling of transformation which is part of the configuration
 *                       (INACTIVE_SESSIONHANDLING, ACTIVE_SESSIONHANDLING)
 */
/* Deviation MISRAC2012-1 <+4> */
#define SOMEIPXF_SR_DEF_FUNC_XF(XfrmName, DataType, PassMethod, InterfaceVersion, MessageType,    \
                                TypeConfigName, Endianess, ConfigID, SessionHandling_Status)      \
SOMEIPXF_S_SR_DEF_FUNC_XF_HLP(XfrmName, DataType, SOMEIPXF_ARG_ ## PassMethod,                    \
    SOMEIPXF_CHECKELEMENT_ ## PassMethod, SOMEIPXF_PARAM_ ## PassMethod, InterfaceVersion,        \
    MessageType, TypeConfigName, Endianess, SomeIpXf_GenericTransformer, ConfigID, SessionHandling_Status)

#if (defined SOMEIPXF_SR_DECL_FUNC_XF) /* to prevent double declaration */
#error SOMEIPXF_SR_DECL_FUNC_XF already defined
#endif /* if (defined SOMEIPXF_SR_DECL_FUNC_XF) */
#define SOMEIPXF_SR_DECL_FUNC_XF(XfrmName, DataType, PassMethod)    \
SOMEIPXF_S_SR_DECL_FUNC_XF(XfrmName, DataType, PassMethod)


#if (defined SOMEIPXF_SR_DEF_FUNC_INVXF) /* to prevent double declaration */
#error SOMEIPXF_SR_DEF_FUNC_INVXF already defined
#endif /* if (defined SOMEIPXF_SR_DEF_FUNC_INVXF) */
/* Macro to define inverse-transformer API function for sender/receiver communication
 * Parameters:
 * XfrmName   Name of the inverse transformer
 * DataType   Data type of element where data is de-serialized into
 * InterfaceVersion   The interface version the inverse transformer shall use
 * TypeConfigName   Type of the data element which is part of configuration name
 * Endianess   Endianess of transformation which is part of the configuration name (LE, BE or OP)
 */
#define SOMEIPXF_SR_DEF_FUNC_INVXF(XfrmName, DataType, InterfaceVersion, TypeConfigName,          \
                                   Endianess, ConfigID)                                           \
SOMEIPXF_S_SR_DEF_FUNC_INVXF_HLP(XfrmName, DataType, InterfaceVersion, TypeConfigName, Endianess, \
    SomeIpXf_Inv_GenericTransformer, ConfigID)                                                    \
SOMEIPXF_MESSAGE_ERROR_CALLOUT(XfrmName)                                                          \
SOMEIPXF_S_SR_DEF_FUNC_INVXF_HLP_END

#if (defined SOMEIPXF_SR_DECL_FUNC_INVXF) /* to prevent double declaration */
#error SOMEIPXF_SR_DECL_FUNC_INVXF already defined
#endif /* if (defined SOMEIPXF_SR_DECL_FUNC_INVXF) */
#define SOMEIPXF_SR_DECL_FUNC_INVXF(XfrmName, DataType) \
SOMEIPXF_S_SR_DECL_FUNC_INVXF(XfrmName, DataType)


/* ---------------------------- Client MACROS --------------------------------- */
/* Macro guards */
#if (defined SOMEIPXF_CLIENT_XF_CALL_GEN) /* to prevent double declaration */
#error SOMEIPXF_CLIENT_XF_CALL_GEN already defined
#endif /* if (defined SOMEIPXF_CLIENT_XF_CALL_GEN) */

#if (defined SOMEIPXF_CLIENT_XF_CALL) /* to prevent double declaration */
#error SOMEIPXF_CLIENT_XF_CALL already defined
#endif /* if (defined SOMEIPXF_CLIENT_XF_CALL) */

#if (defined SOMEIPXF_CLIENT_DEF_FUNC_XF_CALLS) /* to prevent double declaration */
#error SOMEIPXF_CLIENT_DEF_FUNC_XF_CALLS already defined
#endif /* if (defined SOMEIPXF_CLIENT_DEF_FUNC_XF_CALLS) */

#if (defined SOMEIPXF_CLIENT_LOOP_XF_CALL_1) /* to prevent double declaration */
#error SOMEIPXF_CLIENT_LOOP_XF_CALL_1 already defined
#endif /* if (defined SOMEIPXF_CLIENT_LOOP_XF_CALL_1) */

#if (defined SOMEIPXF_CLIENT_LOOP_XF_CALL_2) /* to prevent double declaration */
#error SOMEIPXF_CLIENT_LOOP_XF_CALL_2 already defined
#endif /* if (defined SOMEIPXF_CLIENT_LOOP_XF_CALL_2) */

#if (defined SOMEIPXF_CLIENT_LOOP_XF_CALL_1_END) /* to prevent double declaration */
#error SOMEIPXF_CLIENT_LOOP_XF_CALL_1_END already defined
#endif /* if (defined SOMEIPXF_CLIENT_LOOP_XF_CALL_1_END) */

#if (defined SOMEIPXF_CLIENT_LOOP_XF_CALL_2_END) /* to prevent double declaration */
#error SOMEIPXF_CLIENT_LOOP_XF_CALL_2_END already defined
#endif /* if (defined SOMEIPXF_CLIENT_LOOP_XF_CALL_2_END) */

#if (defined SOMEIPXF_CLIENT_DEF_FUNC_XF) /* to prevent double declaration */
#error SOMEIPXF_CLIENT_DEF_FUNC_XF already defined
#endif /* if (defined SOMEIPXF_CLIENT_DEF_FUNC_XF) */

#if (defined SOMEIPXF_CLIENT_DECL_FUNC_XF) /* to prevent double declaration */
#error SOMEIPXF_CLIENT_DECL_FUNC_XF already defined
#endif /* if (defined SOMEIPXF_CLIENT_DECL_FUNC_XF) */
/* Macro guards */


/* Create transformer function calls */
/* Deviation MISRAC2012-1 <+7> */
#define SOMEIPXF_CLIENT_XF_CALL_GEN(DataType, Nr, PassMethod, config)                                                      \
        if(RetVal == E_OK)                                                                                                 \
        {                                                                                                                  \
              RetVal = SomeIpXf_GenericTransformer(                                                                        \
                  &Buffer[*BufferLength],                                                                                  \
                  BufferLength,                                                                                            \
                  (P2CONST(void, AUTOMATIC, SOMEIPXF_APPL_DATA)) SOMEIPXF_PARAM_ ## PassMethod(Data_ ## Nr),               \
                  &config[0]                                                                                               \
              );                                                                                                           \
        }

/* List of Transformer calls */
/* Deviation MISRAC2012-1 <+2> */
#define SOMEIPXF_CLIENT_XF_CALL(ARG_ID)                 \
SOMEIPXF_CLIENT_XF_CALL_GEN SOMEIPXF_CLIENT_ ## ARG_ID

#define SOMEIPXF_CLIENT_DEF_FUNC_XF_CALLS(SEQ) SOMEIPXF_CAT(SOMEIPXF_CLIENT_LOOP_XF_CALL_1 SEQ, _END)
#define SOMEIPXF_CLIENT_LOOP_XF_CALL_1(x) SOMEIPXF_CLIENT_XF_CALL(x) SOMEIPXF_CLIENT_LOOP_XF_CALL_2
#define SOMEIPXF_CLIENT_LOOP_XF_CALL_2(x) SOMEIPXF_CLIENT_XF_CALL(x) SOMEIPXF_CLIENT_LOOP_XF_CALL_1
#define SOMEIPXF_CLIENT_LOOP_XF_CALL_1_END
#define SOMEIPXF_CLIENT_LOOP_XF_CALL_2_END
/* End of Create transformer function calls */


/* Macro to define transformer API function for Client communication
 * Parameters:
 * XfrmName   Name of the transformer
 * InterfaceVersion   The interface version the transformer shall use
 */
/* Deviation MISRAC2012-1 <+4> */
#define SOMEIPXF_CLIENT_DEF_FUNC_XF(XfrmName, InterfaceVersion)                                   \
SOMEIPXF_S_CLIENT_DEF_FUNC_XF_SECT_START(XfrmName) SOMEIPXF_S_CLIENT_DEF_FUNC_XF_SECT_PARAMLIST( SOMEIPXF_CLIENT_SEQ_XF_ ## XfrmName ) \
SOMEIPXF_S_CLIENT_DEF_FUNC_XF_SECT_MID_1 SOMEIPXF_S_CLIENT_DEF_FUNC_XF_SECT_BUFFCHECK( SOMEIPXF_CLIENT_SEQ_XF_ ## XfrmName ) SOMEIPXF_S_CLIENT_DEF_FUNC_XF_SECT_MID_2( InterfaceVersion ) \
        SOMEIPXF_CLIENT_DEF_FUNC_XF_CALLS( SOMEIPXF_CLIENT_SEQ_XF_ ## XfrmName )                  \
    SOMEIPXF_S_CLIENT_DEF_FUNC_XF_SECT_END


/* Function declaration */
#define SOMEIPXF_CLIENT_DECL_FUNC_XF(XfrmName)                 \
SOMEIPXF_S_CLIENT_DECL_FUNC_XF(XfrmName)

/* --------------------- End of Client MACROS --------------------------------- */


/* ---------------------------- INV-Client MACROS --------------------------------- */
/* Macro guards */
#if (defined SOMEIPXF_CLIENT_INVXF_CALL_GEN) /* to prevent double declaration */
#error SOMEIPXF_CLIENT_INVXF_CALL_GEN already defined
#endif /* if (defined SOMEIPXF_CLIENT_INVXF_CALL_GEN) */

#if (defined SOMEIPXF_CLIENT_INVXF_CALL_GEN_TLV) /* to prevent double declaration */
#error SOMEIPXF_CLIENT_INVXF_CALL_GEN_TLV already defined
#endif /* if (defined SOMEIPXF_CLIENT_INVXF_CALL_GEN_TLV) */

#if (defined SOMEIPXF_CLIENT_INVXF_CALL) /* to prevent double declaration */
#error SOMEIPXF_CLIENT_INVXF_CALL already defined
#endif /* if (defined SOMEIPXF_CLIENT_INVXF_CALL) */

#if (defined SOMEIPXF_CLIENT_INVXF_CALL_TLV) /* to prevent double declaration */
#error SOMEIPXF_CLIENT_INVXF_CALL_TLV already defined
#endif /* if (defined SOMEIPXF_CLIENT_INVXF_CALL_TLV) */

#if (defined SOMEIPXF_CLIENT_DEF_FUNC_INVXF_CALLS_TLV_ARG) /* to prevent double declaration */
#error SOMEIPXF_CLIENT_DEF_FUNC_INVXF_CALLS_TLV_ARG already defined
#endif /* if (defined SOMEIPXF_CLIENT_DEF_FUNC_INVXF_CALLS_TLV_ARG) */

#if (defined SOMEIPXF_CLIENT_DEF_FUNC_INVXF_CALLS_NON_TLV_ARG) /* to prevent double declaration */
#error SOMEIPXF_CLIENT_DEF_FUNC_INVXF_CALLS_NON_TLV_ARG already defined
#endif /* if (defined SOMEIPXF_CLIENT_DEF_FUNC_INVXF_CALLS_NON_TLV_ARG) */

#if (defined SOMEIPXF_CLIENT_LOOP_INVXF_CALL_1) /* to prevent double declaration */
#error SOMEIPXF_CLIENT_LOOP_INVXF_CALL_1 already defined
#endif /* if (defined SOMEIPXF_CLIENT_LOOP_INVXF_CALL_1) */

#if (defined SOMEIPXF_CLIENT_LOOP_INVXF_CALL_2) /* to prevent double declaration */
#error SOMEIPXF_CLIENT_LOOP_INVXF_CALL_2 already defined
#endif /* if (defined SOMEIPXF_CLIENT_LOOP_INVXF_CALL_2) */

#if (defined SOMEIPXF_CLIENT_LOOP_INVXF_CALL_1_END) /* to prevent double declaration */
#error SOMEIPXF_CLIENT_LOOP_INVXF_CALL_1_END already defined
#endif /* if (defined SOMEIPXF_CLIENT_LOOP_INVXF_CALL_1_END) */

#if (defined SOMEIPXF_CLIENT_LOOP_INVXF_CALL_2_END) /* to prevent double declaration */
#error SOMEIPXF_CLIENT_LOOP_INVXF_CALL_2_END already defined
#endif /* if (defined SOMEIPXF_CLIENT_LOOP_INVXF_CALL_2_END) */

#if (defined SOMEIPXF_CLIENT_LOOP_INVXF_CALL_11) /* to prevent double declaration */
#error SOMEIPXF_CLIENT_LOOP_INVXF_CALL_1 already defined
#endif /* if (defined SOMEIPXF_CLIENT_LOOP_INVXF_CALL_1) */

#if (defined SOMEIPXF_CLIENT_LOOP_INVXF_CALL_22) /* to prevent double declaration */
#error SOMEIPXF_CLIENT_LOOP_INVXF_CALL_22 already defined
#endif /* if (defined SOMEIPXF_CLIENT_LOOP_INVXF_CALL_2) */

#if (defined SOMEIPXF_CLIENT_LOOP_INVXF_CALL_11_END) /* to prevent double declaration */
#error SOMEIPXF_CLIENT_LOOP_INVXF_CALL_11_END already defined
#endif /* if (defined SOMEIPXF_CLIENT_LOOP_INVXF_CALL_1_END) */

#if (defined SOMEIPXF_CLIENT_LOOP_INVXF_CALL_22_END) /* to prevent double declaration */
#error SOMEIPXF_CLIENT_LOOP_INVXF_CALL_22_END already defined
#endif /* if (defined SOMEIPXF_CLIENT_LOOP_INVXF_CALL_2_END) */

#if (defined SOMEIPXF_CLIENT_DEF_FUNC_INVXF_CALLS_NON_TLV_NOARG) /* to prevent double declaration */
#error SOMEIPXF_CLIENT_DEF_FUNC_INVXF_CALLS_NON_TLV_NOARG already defined
#endif /* if (defined SOMEIPXF_CLIENT_DEF_FUNC_INVXF_CALLS_NON_TLV_NOARG) */

#if (defined SOMEIPXF_CLIENT_DEF_FUNC_INVXF) /* to prevent double declaration */
#error SOMEIPXF_CLIENT_DEF_FUNC_INVXF already defined
#endif /* if (defined SOMEIPXF_CLIENT_DEF_FUNC_INVXF) */

#if (defined SOMEIPXF_CLIENT_DECL_FUNC_INVXF) /* to prevent double declaration */
#error SOMEIPXF_CLIENT_DECL_FUNC_INVXF already defined
#endif /* if (defined SOMEIPXF_CLIENT_DECL_FUNC_INVXF) */
/* Macro guards */


/* Create transformer function calls */
/* Deviation MISRAC2012-1 <+10> */
#define SOMEIPXF_CLIENT_INVXF_CALL_GEN(DataType, Nr, PassMethod, config)                    \
                                                                                            \
            if(RetVal == E_OK)                                                              \
            {                                                                               \
                if(BufferLength > BytesCopiedFromBuffer)                                    \
                {                                                                           \
                    RetVal = SomeIpXf_Inv_GenericTransformer(                               \
                        &Buffer[BytesCopiedFromBuffer],                                     \
                        BufferLength,                                                       \
                        (P2VAR(void, AUTOMATIC, SOMEIPXF_APPL_DATA))Data_ ## Nr,            \
                        &BytesCopiedFromBuffer,                                             \
                        &config[0]                                                          \
                    );                                                                      \
                }                                                                           \
                else                                                                        \
                {                                                                           \
                    RetVal = E_SER_MALFORMED_MESSAGE;                                       \
                }                                                                           \
            }                                                                               \
                                                                                            \

#if (SOMEIPXF_TLV_ARGUMENT_REORDERING_ENABLE == STD_ON)
/* Create transformer function calls */
/* Deviation MISRAC2012-1 <+10> */
#define SOMEIPXF_CLIENT_INVXF_CALL_GEN_TLV(DataType, Nr, PassMethod, config)                \
                                                                                            \
            if(RetVal == E_OK)                                                              \
            {                                                                               \
                /* Start again */                                                           \
                BytesCopiedFromBuffer = 8U;                                                 \
                {                                                                           \
                    RetVal = SomeIpXf_Inv_GenericTransformer(                               \
                        &Buffer[BytesCopiedFromBuffer],                                     \
                        BufferLength,                                                       \
                        (P2VAR(void, AUTOMATIC, SOMEIPXF_APPL_DATA))Data_ ## Nr,            \
                        &BytesCopiedFromBuffer,                                             \
                        &config[0]                                                          \
                    );                                                                      \
                }                                                                           \
                if( (RetVal == E_OK) && (BufferLength < BytesCopiedFromBuffer))             \
                {                                                                           \
                    RetVal = E_SER_MALFORMED_MESSAGE;                                       \
                }                                                                           \
            }                                                                               \
                                                                                            \

#else
/* Create transformer function calls */
/* Deviation MISRAC2012-1 <+10> */
#define SOMEIPXF_CLIENT_INVXF_CALL_GEN_TLV(DataType, Nr, PassMethod, config)                \
                                                                                            \
            if(RetVal == E_OK)                                                              \
            {                                                                               \
                if(BufferLength > BytesCopiedFromBuffer)                                    \
                {                                                                           \
                    RetVal = SomeIpXf_Inv_GenericTransformer(                               \
                        &Buffer[BytesCopiedFromBuffer],                                     \
                        BufferLength,                                                       \
                        (P2VAR(void, AUTOMATIC, SOMEIPXF_APPL_DATA))Data_ ## Nr,            \
                        &BytesCopiedFromBuffer,                                             \
                        &config[0]                                                          \
                    );                                                                      \
                }                                                                           \
                else                                                                        \
                {                                                                           \
                    RetVal = E_SER_MALFORMED_MESSAGE;                                       \
                }                                                                           \
            }                                                                               \

#endif /* (SOMEIPXF_TLV_ARGUMENT_REORDERING_ENABLE == STD_ON) */

/* Create List of Transformer calls */
/* Deviation MISRAC2012-1 <+2> */
#define SOMEIPXF_CLIENT_INVXF_CALL(ARG_ID)                  \
SOMEIPXF_CLIENT_INVXF_CALL_GEN SOMEIPXF_CLIENT_ ## ARG_ID

#define SOMEIPXF_CLIENT_DEF_FUNC_INVXF_CALLS_NON_TLV_ARG(SEQ) SOMEIPXF_CAT(SOMEIPXF_CLIENT_LOOP_INVXF_CALL_1 SEQ, _END)
#define SOMEIPXF_CLIENT_LOOP_INVXF_CALL_1(x) SOMEIPXF_CLIENT_INVXF_CALL(x) SOMEIPXF_CLIENT_LOOP_INVXF_CALL_2
#define SOMEIPXF_CLIENT_LOOP_INVXF_CALL_2(x) SOMEIPXF_CLIENT_INVXF_CALL(x) SOMEIPXF_CLIENT_LOOP_INVXF_CALL_1
#define SOMEIPXF_CLIENT_LOOP_INVXF_CALL_1_END
#define SOMEIPXF_CLIENT_LOOP_INVXF_CALL_2_END

/* Deviation MISRAC2012-1 <+2> */
#define SOMEIPXF_CLIENT_INVXF_CALL_TLV(ARG_ID)                  \
SOMEIPXF_CLIENT_INVXF_CALL_GEN_TLV SOMEIPXF_CLIENT_ ## ARG_ID

#define SOMEIPXF_CLIENT_DEF_FUNC_INVXF_CALLS_TLV_ARG(SEQ) SOMEIPXF_CAT(SOMEIPXF_CLIENT_LOOP_INVXF_CALL_11 SEQ, _END)
#define SOMEIPXF_CLIENT_LOOP_INVXF_CALL_11(x) SOMEIPXF_CLIENT_INVXF_CALL_TLV(x) SOMEIPXF_CLIENT_LOOP_INVXF_CALL_22
#define SOMEIPXF_CLIENT_LOOP_INVXF_CALL_22(x) SOMEIPXF_CLIENT_INVXF_CALL_TLV(x) SOMEIPXF_CLIENT_LOOP_INVXF_CALL_11
#define SOMEIPXF_CLIENT_LOOP_INVXF_CALL_11_END
#define SOMEIPXF_CLIENT_LOOP_INVXF_CALL_22_END

/* No Parameters */
#define SOMEIPXF_CLIENT_DEF_FUNC_INVXF_CALLS_NON_TLV_NOARG(UNUSED) \
    SOMEIPXF_PARAM_UNUSED(BytesCopiedFromBuffer);
/* End of Create transformer function calls */


/* Macro to define inverse-transformer API function for Client communication
 * Parameters:
 * XfrmName   Name of the inverse transformer
 * InterfaceVersion   The interface version the inverse transformer shall use
 * HasReturnValue   Function has return value (RETVAL, NORETVAL)
 * HasArguments   Function has one ore more Arguments, e.g. Data_1 (ARG, NOARG)
 * IsTlv   Function has tlv arguments or not (TLV, NON_TLV )
 */
/* Deviation MISRAC2012-1 <+9> */
#define SOMEIPXF_CLIENT_DEF_FUNC_INVXF(XfrmName, InterfaceVersion, HasReturnValue, HasArguments, IsTlv)                  \
SOMEIPXF_S_CLIENT_INVXF_START(XfrmName, HasReturnValue) SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_PARAMLIST_ ## HasArguments( SOMEIPXF_CLIENT_SEQ_INVXF_ ## XfrmName )     \
SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_MID_1 SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_BUFFCHECK_ ## HasArguments( SOMEIPXF_CLIENT_SEQ_INVXF_ ## XfrmName ) SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_MID_2( InterfaceVersion, HasReturnValue ) \
                                                                                                                 \
SOMEIPXF_CLIENT_DEF_FUNC_INVXF_CALLS_ ## IsTlv ##_ ## HasArguments( SOMEIPXF_CLIENT_SEQ_INVXF_ ## XfrmName )        \
SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_FIND_DUPLICATEID_ ## IsTlv(XfrmName)                   \
SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_END1_ ## IsTlv                                    \
SOMEIPXF_MESSAGE_ERROR_CALLOUT(XfrmName)                                                \
SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_END2


/* Function Declaration */
#define SOMEIPXF_CLIENT_DECL_FUNC_INVXF(XfrmName, InterfaceVersion, HasReturnValue, HasArguments )   \
SOMEIPXF_S_CLIENT_DECL_FUNC_INVXF(XfrmName, InterfaceVersion, HasReturnValue, HasArguments )

/* --------------------- End of INV-Client MACROS --------------------------------- */


/* -------------------------- Server MACROS --------------------------------- */
/* Macro guards */
#if (defined SOMEIPXF_SERVER_XF_CALL_GEN) /* to prevent double declaration */
#error SOMEIPXF_SERVER_XF_CALL_GEN already defined
#endif /* if (defined SOMEIPXF_SERVER_XF_CALL_GEN) */

#if (defined SOMEIPXF_SERVER_XF_CALL) /* to prevent double declaration */
#error SOMEIPXF_SERVER_XF_CALL already defined
#endif /* if (defined SOMEIPXF_SERVER_XF_CALL) */

#if (defined SOMEIPXF_SERVER_DEF_FUNC_XF_CALLS) /* to prevent double declaration */
#error SOMEIPXF_SERVER_DEF_FUNC_XF_CALLS already defined
#endif /* if (defined SOMEIPXF_SERVER_DEF_FUNC_XF_CALLS) */

#if (defined SOMEIPXF_SERVER_LOOP_XF_CALL_1) /* to prevent double declaration */
#error SOMEIPXF_SERVER_LOOP_XF_CALL_1 already defined
#endif /* if (defined SOMEIPXF_SERVER_LOOP_XF_CALL_1) */

#if (defined SOMEIPXF_SERVER_LOOP_XF_CALL_2) /* to prevent double declaration */
#error SOMEIPXF_SERVER_LOOP_XF_CALL_2 already defined
#endif /* if (defined SOMEIPXF_SERVER_LOOP_XF_CALL_2) */

#if (defined SOMEIPXF_SERVER_LOOP_XF_CALL_1_END) /* to prevent double declaration */
#error SOMEIPXF_SERVER_LOOP_XF_CALL_1_END already defined
#endif /* if (defined SOMEIPXF_SERVER_LOOP_XF_CALL_1_END) */

#if (defined SOMEIPXF_SERVER_LOOP_XF_CALL_2_END) /* to prevent double declaration */
#error SOMEIPXF_SERVER_LOOP_XF_CALL_2_END already defined
#endif /* if (defined SOMEIPXF_SERVER_LOOP_XF_CALL_2_END) */

#if (defined SOMEIPXF_SERVER_DEF_FUNC_XF) /* to prevent double declaration */
#error SOMEIPXF_SERVER_DEF_FUNC_XF already defined
#endif /* if (defined SOMEIPXF_SERVER_DEF_FUNC_XF) */

#if (defined SOMEIPXF_SERVER_DECL_FUNC_XF) /* to prevent double declaration */
#error SOMEIPXF_SERVER_DECL_FUNC_XF already defined
#endif /* if (defined SOMEIPXF_SERVER_DECL_FUNC_XF) */
/* Macro guards */


/* Create transformer function calls */
/* Deviation MISRAC2012-1 <+7> */
#define SOMEIPXF_SERVER_XF_CALL_GEN(DataType, Nr, PassMethod, config)                              \
          if(RetVal == E_OK)                                                                       \
          {                                                                                        \
                RetVal = SomeIpXf_GenericTransformer(                                              \
                    &Buffer[*BufferLength],                                                        \
                    BufferLength,                                                                  \
                    (P2CONST(void, AUTOMATIC, SOMEIPXF_APPL_DATA)) SOMEIPXF_PARAM_ ## PassMethod(Data_ ## Nr),  \
                    &config[0]                                                                     \
                );                                                                                 \
          }

/* List of Transformer calls */
/* Deviation MISRAC2012-1 <+2> */
#define SOMEIPXF_SERVER_XF_CALL(ARG_ID)                 \
SOMEIPXF_SERVER_XF_CALL_GEN SOMEIPXF_SERVER_ ## ARG_ID

#define SOMEIPXF_SERVER_DEF_FUNC_XF_CALLS(SEQ) SOMEIPXF_CAT(SOMEIPXF_SERVER_LOOP_XF_CALL_1 SEQ, _END)
#define SOMEIPXF_SERVER_LOOP_XF_CALL_1(x) SOMEIPXF_SERVER_XF_CALL(x) SOMEIPXF_SERVER_LOOP_XF_CALL_2
#define SOMEIPXF_SERVER_LOOP_XF_CALL_2(x) SOMEIPXF_SERVER_XF_CALL(x) SOMEIPXF_SERVER_LOOP_XF_CALL_1
#define SOMEIPXF_SERVER_LOOP_XF_CALL_1_END
#define SOMEIPXF_SERVER_LOOP_XF_CALL_2_END
/* End of Create transformer function calls */


/* Macro to define transformer API function for Server communication
 * Parameters:
 * XfrmName   Name of the inverse transformer
 * InterfaceVersion   The interface version the inverse transformer shall use
 * HasReturnValue   Function has return value (RETVAL, NORETVAL)
 * HasArguments   Function has one ore more Arguments, e.g. Data_1 (ARG, NOARG)
 * HasAutonomousErrorReact   Function has autonomous Error reaction enabled/disabled
 *                           (AUTONERR, NOAUTONERR)
 */
/* Deviation MISRAC2012-1 <+6> */
#define SOMEIPXF_SERVER_DEF_FUNC_XF(XfrmName, InterfaceVersion, HasReturnValue, HasArguments, HasAutonomousErrorReact)    \
SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_START(XfrmName, HasReturnValue) SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_PARAMLIST( SOMEIPXF_SERVER_SEQ_XF_ ## XfrmName )    \
    SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_1( HasReturnValue, HasArguments, HasAutonomousErrorReact )    \
     SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_BUFFCHECK( SOMEIPXF_SERVER_SEQ_XF_ ## XfrmName ) SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2( InterfaceVersion, HasReturnValue, HasArguments, HasAutonomousErrorReact )    \
                                                                                                   \
          SOMEIPXF_SERVER_DEF_FUNC_XF_CALLS( SOMEIPXF_SERVER_SEQ_XF_ ## XfrmName )                 \
        SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_END( HasReturnValue, HasArguments, HasAutonomousErrorReact)


/* Function Declaration */
#define SOMEIPXF_SERVER_DECL_FUNC_XF(XfrmName, InterfaceVersion, HasReturnValue, HasArguments, HasAutonomousErrorReact) \
SOMEIPXF_S_SERVER_DECL_FUNC_XF(XfrmName, InterfaceVersion, HasReturnValue, HasArguments, HasAutonomousErrorReact)

/* --------------------- End of Server MACROS --------------------------------- */



/* --------------------- INV Server MACROS --------------------------------- */
/* Macro guards */
#if (defined SOMEIPXF_SERVER_INVXF_CALL_GEN) /* to prevent double declaration */
#error SOMEIPXF_SERVER_INVXF_CALL_GEN already defined
#endif /* if (defined SOMEIPXF_SERVER_INVXF_CALL_GEN) */

#if (defined SOMEIPXF_SERVER_INVXF_CALL) /* to prevent double declaration */
#error SOMEIPXF_SERVER_INVXF_CALL already defined
#endif /* if (defined SOMEIPXF_SERVER_INVXF_CALL) */

#if (defined SOMEIPXF_SERVER_INVXF_TLV_CALL) /* to prevent double declaration */
#error SOMEIPXF_SERVER_INVXF_TLV_CALL already defined
#endif /* if (defined SOMEIPXF_SERVER_INVXF_TLV_CALL) */

#if (defined SOMEIPXF_SERVER_DEF_FUNC_INVXF_CALLS_NON_TLV_ARG) /* to prevent double declaration */
#error SOMEIPXF_SERVER_DEF_FUNC_INVXF_CALLS_NON_TLV_ARG already defined
#endif /* if (defined SOMEIPXF_SERVER_DEF_FUNC_INVXF_CALLS_NON_TLV_ARG) */

#if (defined SOMEIPXF_SERVER_DEF_FUNC_INVXF_CALLS_TLV_ARG) /* to prevent double declaration */
#error SOMEIPXF_SERVER_DEF_FUNC_INVXF_CALLS_TLV_ARG already defined
#endif /* if (defined SOMEIPXF_SERVER_DEF_FUNC_INVXF_CALLS_TLV_ARG) */

#if (defined SOMEIPXF_SERVER_LOOP_INVXF_CALL_1) /* to prevent double declaration */
#error SOMEIPXF_SERVER_LOOP_INVXF_CALL_1 already defined
#endif /* if (defined SOMEIPXF_SERVER_LOOP_INVXF_CALL_1) */

#if (defined SOMEIPXF_SERVER_LOOP_INVXF_CALL_2) /* to prevent double declaration */
#error SOMEIPXF_SERVER_LOOP_INVXF_CALL_2 already defined
#endif /* if (defined SOMEIPXF_SERVER_LOOP_INVXF_CALL_2) */

#if (defined SOMEIPXF_SERVER_LOOP_INVXF_CALL_1_END) /* to prevent double declaration */
#error SOMEIPXF_SERVER_LOOP_INVXF_CALL_1_END already defined
#endif /* if (defined SOMEIPXF_SERVER_LOOP_INVXF_CALL_1_END) */

#if (defined SOMEIPXF_SERVER_LOOP_INVXF_CALL_2_END) /* to prevent double declaration */
#error SOMEIPXF_SERVER_LOOP_INVXF_CALL_2_END already defined
#endif /* if (defined SOMEIPXF_SERVER_LOOP_INVXF_CALL_2_END) */

#if (defined SOMEIPXF_SERVER_LOOP_INVXF_CALL_11) /* to prevent double declaration */
#error SOMEIPXF_SERVER_LOOP_INVXF_CALL_11 already defined
#endif /* if (defined SOMEIPXF_SERVER_LOOP_INVXF_CALL_11) */

#if (defined SOMEIPXF_SERVER_LOOP_INVXF_CALL_22) /* to prevent double declaration */
#error SOMEIPXF_SERVER_LOOP_INVXF_CALL_22 already defined
#endif /* if (defined SOMEIPXF_SERVER_LOOP_INVXF_CALL_22) */

#if (defined SOMEIPXF_SERVER_LOOP_INVXF_CALL_11_END) /* to prevent double declaration */
#error SOMEIPXF_SERVER_LOOP_INVXF_CALL_11_END already defined
#endif /* if (defined SOMEIPXF_SERVER_LOOP_INVXF_CALL_11_END) */

#if (defined SOMEIPXF_SERVER_LOOP_INVXF_CALL_22_END) /* to prevent double declaration */
#error SOMEIPXF_SERVER_LOOP_INVXF_CALL_22_END already defined
#endif /* if (defined SOMEIPXF_SERVER_LOOP_INVXF_CALL_22_END) */

#if (defined SOMEIPXF_SERVER_DEF_FUNC_INVXF_CALLS_NON_TLV_NOARG) /* to prevent double declaration */
#error SOMEIPXF_SERVER_DEF_FUNC_INVXF_CALLS_NON_TLV_NOARG already defined
#endif /* if (defined SOMEIPXF_SERVER_DEF_FUNC_INVXF_CALLS_NON_TLV_NOARG) */

#if (defined SOMEIPXF_SERVER_DEF_FUNC_INVXF) /* to prevent double declaration */
#error SOMEIPXF_SERVER_DEF_FUNC_INVXF already defined
#endif /* if (defined SOMEIPXF_SERVER_DEF_FUNC_INVXF) */

#if (defined SOMEIPXF_SERVER_DECL_FUNC_INVXF) /* to prevent double declaration */
#error SOMEIPXF_SERVER_DECL_FUNC_INVXF already defined
#endif /* if (defined SOMEIPXF_SERVER_DECL_FUNC_INVXF) */
/* Macro guards */


/* Create transformer function calls */
/* Deviation MISRAC2012-1 <+10> */
#define SOMEIPXF_SERVER_INVXF_CALL_GEN(DataType, Nr, config)                                \
                                                                                            \
            if(RetVal == E_OK)                                                              \
            {                                                                               \
                if(BufferLength > BytesCopiedFromBuffer)                                    \
                {                                                                           \
                    RetVal = SomeIpXf_Inv_GenericTransformer(                               \
                        &Buffer[BytesCopiedFromBuffer],                                     \
                        BufferLength,                                                       \
                        (P2VAR(void, AUTOMATIC, SOMEIPXF_APPL_DATA))Data_ ## Nr,            \
                        &BytesCopiedFromBuffer,                                             \
                        &config[0]                                                          \
                    );                                                                      \
                }                                                                           \
                else                                                                        \
                {                                                                           \
                    RetVal = E_SER_MALFORMED_MESSAGE;                                       \
                }                                                                           \
            }                                                                               \
                                                                                            \

#if (SOMEIPXF_TLV_ARGUMENT_REORDERING_ENABLE == STD_ON)
/* Create transformer function calls */
/* Deviation MISRAC2012-1 <+10> */
#define SOMEIPXF_SERVER_INVXF_CALL_GEN_TLV(DataType, Nr, config)                            \
                                                                                            \
            if(RetVal == E_OK)                                                              \
            {                                                                               \
                /* Start again */                                                           \
                BytesCopiedFromBuffer = 8U;                                                 \
                                                                                            \
                RetVal = SomeIpXf_Inv_GenericTransformer(                                   \
                    &Buffer[BytesCopiedFromBuffer],                                         \
                    BufferLength,                                                           \
                    (P2VAR(void, AUTOMATIC, SOMEIPXF_APPL_DATA))Data_ ## Nr,                \
                    &BytesCopiedFromBuffer,                                                 \
                    &config[0]                                                              \
                );                                                                          \
                if( (RetVal == E_OK) && (BufferLength < BytesCopiedFromBuffer))             \
                {                                                                           \
                    RetVal = E_SER_MALFORMED_MESSAGE;                                       \
                }                                                                           \
            }                                                                               \
                                                                                            \

#else
/* Create transformer function calls */
/* Deviation MISRAC2012-1 <+10> */
#define SOMEIPXF_SERVER_INVXF_CALL_GEN_TLV(DataType, Nr, config)                            \
                                                                                            \
            if(RetVal == E_OK)                                                              \
            {                                                                               \
                if(BufferLength > BytesCopiedFromBuffer)                                    \
                {                                                                           \
                    RetVal = SomeIpXf_Inv_GenericTransformer(                               \
                        &Buffer[BytesCopiedFromBuffer],                                     \
                        BufferLength,                                                       \
                        (P2VAR(void, AUTOMATIC, SOMEIPXF_APPL_DATA))Data_ ## Nr,            \
                        &BytesCopiedFromBuffer,                                             \
                        &config[0]                                                          \
                    );                                                                      \
                }                                                                           \
                else                                                                        \
                {                                                                           \
                    RetVal = E_SER_MALFORMED_MESSAGE;                                       \
                }                                                                           \
            }                                                                               \
                                                                                            \

#endif  /* (SOMEIPXF_TLV_ARGUMENT_REORDERING_ENABLE == STD_ON) */

/* List of Transformer calls */
/* Deviation MISRAC2012-1 <+2> */
#define SOMEIPXF_SERVER_INVXF_CALL(ARG_ID)                  \
SOMEIPXF_SERVER_INVXF_CALL_GEN SOMEIPXF_SERVER_ ## ARG_ID

#define SOMEIPXF_SERVER_DEF_FUNC_INVXF_CALLS_NON_TLV_ARG(SEQ) SOMEIPXF_CAT(SOMEIPXF_SERVER_LOOP_INVXF_CALL_1 SEQ, _END)
#define SOMEIPXF_SERVER_LOOP_INVXF_CALL_1(x) SOMEIPXF_SERVER_INVXF_CALL(x) SOMEIPXF_SERVER_LOOP_INVXF_CALL_2
#define SOMEIPXF_SERVER_LOOP_INVXF_CALL_2(x) SOMEIPXF_SERVER_INVXF_CALL(x) SOMEIPXF_SERVER_LOOP_INVXF_CALL_1
#define SOMEIPXF_SERVER_LOOP_INVXF_CALL_1_END
#define SOMEIPXF_SERVER_LOOP_INVXF_CALL_2_END

/* List of Transformer calls */
/* Deviation MISRAC2012-1 <+2> */
#define SOMEIPXF_SERVER_INVXF_TLV_CALL(ARG_ID)                  \
SOMEIPXF_SERVER_INVXF_CALL_GEN_TLV SOMEIPXF_SERVER_ ## ARG_ID

#define SOMEIPXF_SERVER_DEF_FUNC_INVXF_CALLS_TLV_ARG(SEQ) SOMEIPXF_CAT(SOMEIPXF_SERVER_LOOP_INVXF_CALL_11 SEQ, _END)
#define SOMEIPXF_SERVER_LOOP_INVXF_CALL_11(x) SOMEIPXF_SERVER_INVXF_TLV_CALL(x) SOMEIPXF_SERVER_LOOP_INVXF_CALL_22
#define SOMEIPXF_SERVER_LOOP_INVXF_CALL_22(x) SOMEIPXF_SERVER_INVXF_TLV_CALL(x) SOMEIPXF_SERVER_LOOP_INVXF_CALL_11
#define SOMEIPXF_SERVER_LOOP_INVXF_CALL_11_END
#define SOMEIPXF_SERVER_LOOP_INVXF_CALL_22_END

/* No Arguments */
#define SOMEIPXF_SERVER_DEF_FUNC_INVXF_CALLS_NON_TLV_NOARG(UNUSED) \
    SOMEIPXF_PARAM_UNUSED(BytesCopiedFromBuffer);
/* End of Create transformer function calls */


/* Macro to define inverse transformer API function for Server communication
 * Parameters:
 * XfrmName   Name of the inverse transformer
 * InterfaceVersion   The interface version the inverse transformer shall use
 * HasArguments   Function has one ore more Arguments, e.g. Data_1 (ARG, NOARG)
 * IsTlv   Function has tlv arguments or not (TLV, NON_TLV )
 */
/* Deviation MISRAC2012-1 <+8> */
#define SOMEIPXF_SERVER_DEF_FUNC_INVXF(XfrmName, InterfaceVersion, HasArguments, IsTlv)                \
SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_START(XfrmName) SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_PARAMLIST( SOMEIPXF_SERVER_SEQ_INVXF_ ## XfrmName )     \
SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_MID_1 SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_BUFFCHECK( SOMEIPXF_SERVER_SEQ_INVXF_ ## XfrmName ) SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_MID_2( InterfaceVersion )  \
SOMEIPXF_SERVER_DEF_FUNC_INVXF_CALLS_ ##IsTlv ##_ ## HasArguments( SOMEIPXF_SERVER_SEQ_INVXF_ ## XfrmName )     \
SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_FIND_DUPLICATEID_ ## IsTlv(XfrmName)                     \
SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_END1_ ## IsTlv                                      \
SOMEIPXF_MESSAGE_ERROR_CALLOUT(XfrmName)                                                  \
SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_END2

/* Function Declaration */
#define SOMEIPXF_SERVER_DECL_FUNC_INVXF(XfrmName, InterfaceVersion, HasArguments) \
SOMEIPXF_S_SERVER_DECL_FUNC_INVXF(XfrmName, InterfaceVersion, HasArguments)

/* --------------------- End of INV Server MACROS --------------------------------- */


/* ---------------------- External Trigger MACROS -------------------------------------- */
/* Macro guards */
#if (defined SOMEIPXF_ET_DEF_FUNC_XF) /* to prevent double declaration */
#error SOMEIPXF_ET_DEF_FUNC_XF already defined
#endif /* if (defined SOMEIPXF_ET_DEF_FUNC_XF) */

#if (defined SOMEIPXF_ET_DECL_FUNC_XF) /* to prevent double declaration */
#error SOMEIPXF_ET_DECL_FUNC_XF already defined
#endif /* if (defined SOMEIPXF_ET_DECL_FUNC_XF) */
/* Macro guards */

/* ET macro */
#define SOMEIPXF_ET_DEF_FUNC_XF(XfrmName, InterfaceVersion, SessionHandling_Status) SOMEIPXF_S_ET_DEF_FUNC_XF(XfrmName, InterfaceVersion, SessionHandling_Status)

/* Declaration */
#define SOMEIPXF_ET_DECL_FUNC_XF(XfrmName) SOMEIPXF_S_ET_DECL_FUNC_XF(XfrmName)
/* ---------------------- End of External Trigger MACROS ------------------------- */


/* ---------------------- External Trigger INV MACROS ---------------------------- */
/* Macro guards */
#if (defined SOMEIPXF_ET_DEF_FUNC_INVXF) /* to prevent double declaration */
#error SOMEIPXF_ET_DEF_FUNC_INVXF already defined
#endif /* if (defined SOMEIPXF_ET_DEF_FUNC_INVXF) */

#if (defined SOMEIPXF_ET_DECL_FUNC_INVXF) /* to prevent double declaration */
#error SOMEIPXF_ET_DECL_FUNC_INVXF already defined
#endif /* if (defined SOMEIPXF_ET_DECL_FUNC_INVXF) */
/* Macro guards */

/* ET macro */
#define SOMEIPXF_ET_DEF_FUNC_INVXF(XfrmName, InterfaceVersion) SOMEIPXF_S_ET_DEF_FUNC_INVXF(XfrmName, InterfaceVersion)

/* Declaration */
#define SOMEIPXF_ET_DECL_FUNC_INVXF(XfrmName) SOMEIPXF_S_ET_DECL_FUNC_INVXF(XfrmName)
/* ----------------End of External Trigger INV MACROS ---------------------------- */



/*==================[Error codes]===========================================*/

/*==================[macro checks]==========================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define SOMEIPXF_START_SEC_CODE
#include <SomeIpXf_MemMap.h>

extern FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_GenericTransformer
(
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) BufferLengthPtr,
   P2CONST(void, AUTOMATIC, SOMEIPXF_APPL_DATA) DataPtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr
);

extern FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_Inv_GenericTransformer
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength,
   P2VAR(void, AUTOMATIC, SOMEIPXF_APPL_DATA) DataPtr,
   P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) BytesCopiedPtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr
);

#define SOMEIPXF_STOP_SEC_CODE
#include <SomeIpXf_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef SOMEIPXF_INT_STC_H */
/*==================[end of file]===========================================*/
