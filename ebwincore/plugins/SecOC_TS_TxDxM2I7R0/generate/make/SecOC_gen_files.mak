#
# \file
#
# \brief AUTOSAR SecOC
#
# This file contains the implementation of the AUTOSAR
# module SecOC.
#
# \version 2.7.4
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


[!SELECT "as:modconf('SecOC')[1]"!]

[!IF "num:i(count(SecOCRxPduProcessing/*)) > 0"!][!//

SecOC_src_FILES   += $(SecOC_CORE_PATH)/src/SecOC_Rx.c

[!ENDIF!][!//

[!IF "num:i(count(SecOCTxPduProcessing/*)) > 0"!][!//

SecOC_src_FILES   += $(SecOC_CORE_PATH)/src/SecOC_Tx.c

[!ENDIF!][!//

[!ENDSELECT!]