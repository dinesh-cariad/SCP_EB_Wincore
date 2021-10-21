<?xml version="1.0" encoding="UTF-8"?>

<stylesheet version="2.0" xmlns="http://www.w3.org/1999/XSL/Transform" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:my="http://www.elektrobit.com/2013/xgen/local" xmlns:x="http://www.elektrobit.com/2013/xgen" xmlns:xml="http://www.w3.org/XML/1998/namespace" xmlns:fn="http://www.w3.org/2005/xpath-functions">

	<import href="resource://details/generic-text.xsl" />

	<variable name="file-description-comment">
		<text> /* !LINKSTO PLATFORM001,1,
    !        PLATFORM003,1
    !desc Header file Platform_Types.h containing platform types
  */</text>
	</variable>

	<template match="x:xgen" mode="start-file">
        <apply-imports />
		<text>#if !(defined </text>
		<value-of select="$my:include-guard-name" />
		<text>)</text>
		<value-of select="$my:NL" />
		<text>#define </text>
		<value-of select="$my:include-guard-name" />
		<value-of select="$my:NL" />
		<value-of select="$my:NL" />
		<value-of select="my:normalize-end-of-line($misra-top-comment)" />
		<value-of select="$my:NL" />
		<value-of select="$my:NL" />
		<value-of select="$my:NL" />
		<value-of select="my:normalize-end-of-line($file-description-comment)" />
		<value-of select="$my:NL" />
	</template>

	<template match="x:xgen" mode="macros">
        <apply-imports />

		<call-template name="subsection">
			<with-param name="text">
				<text>AUTOSAR vendor identification</text>
			</with-param>
		</call-template>

		<call-template name="macro">
			<with-param name="name">
				<text>PLATFORM_VENDOR_ID</text>
			</with-param>
			<with-param name="value" select="my:make-unsigned-value($my:current-config/x:parameter[@name='platform-vendor-id']/x:integer-value)" />
			<with-param name="comment">
				<text>\brief AUTOSAR vendor identification: Elektrobit Automotive GmbH</text>
			</with-param>
		</call-template>

        <call-template name="subsection">
            <with-param name="text">
                <text>AUTOSAR module identification</text>
            </with-param>
        </call-template>

        <call-template name="macro">
            <with-param name="name">
                <text>PLATFORM_MODULE_ID</text>
            </with-param>
            <with-param name="value" select="my:make-unsigned-value($my:current-config/x:parameter[@name='platform-module-id']/x:integer-value)" />
            <with-param name="comment">
                <text>\brief AUTOSAR vendor identification: Elektrobit Automotive GmbH</text>
            </with-param>
        </call-template>

		<!-- <call-template name="macro"> <with-param name="name"> <text></text> </with-param> <with-param name="value" select=""/> </call-template> -->
	</template>

	<template match="x:xgen" mode="type-definitions">
        <apply-imports />

		<apply-templates select="$my:current-config/x:type">
			<sort select="my:computeTypeWeight(.)" data-type="number" />
		</apply-templates>
	</template>

	<template match="x:type">
		<text>typedef </text>
		<apply-templates select="x:reference|x:basetype-ref" />
		<text> </text>
		<value-of select="@name" />
		<text>;</text>
		<value-of select="$my:NL" />
	</template>

	<template match="x:reference">
		<value-of select="@type" />
	</template>

	<template match="x:basetype-ref">
		<if test="exists(@signed)">
			<variable name="signed" as="xs:boolean" select="@signed" />
			<value-of select="if($signed) then 'signed' else 'unsigned'" />
			<text> </text>
		</if>
		<value-of select="@type" />
	</template>

	<template match="x:xgen" mode="end-file">
		<text>#endif /* !(defined </text>
		<value-of select="$my:include-guard-name" />
		<text>) */</text>
		<value-of select="$my:NL" />
        <apply-imports />
	</template>

</stylesheet>