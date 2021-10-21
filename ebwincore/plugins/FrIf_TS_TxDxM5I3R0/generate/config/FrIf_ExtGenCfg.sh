#!/bin/bash
[!VAR "variantName" = "asc:getVariantName()"!]
[!IF "$variantName != ''"!]
export VARIANT_PARAMETER="-v [!"$variantName"!]"
[!ELSE!]
export VARIANT_PARAMETER=""
[!ENDIF!]