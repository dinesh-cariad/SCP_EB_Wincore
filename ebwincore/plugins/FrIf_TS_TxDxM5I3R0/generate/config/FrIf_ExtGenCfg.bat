[!VAR "variantName" = "asc:getVariantName()"!]
[!IF "$variantName != ''"!]
set variantParameter=-v [!"$variantName"!]
[!ELSE!]
set variantParameter=
[!ENDIF!]