<?xml version="1.0" encoding="UTF-8"?>

<stylesheet version="2.0" xmlns="http://www.w3.org/1999/XSL/Transform" xmlns:my="http://www.elektrobit.com/2013/xgen/local" xmlns:x="http://www.elektrobit.com/2013/xgen" xmlns:xml="http://www.w3.org/XML/1998/namespace" xmlns:fn="http://www.w3.org/2005/xpath-functions">

	<import href="resource://details/generic-text.xsl" />

	<template match="x:xgen" mode="start-file">
		<apply-imports />
		
		<text>#ifndef </text>
		<value-of select="$my:include-guard-name" />
		<value-of select="$my:NL" />
		<text>#define </text>
		<value-of select="$my:include-guard-name" />
		<value-of select="$my:NL" />
		<value-of select="$my:NL" />
		<value-of select="$my:NL" />
	</template>

	<template match="x:xgen" mode="inclusions">
        <apply-imports />
        
		<for-each select="my:get-included-file-names(
            $my:current-config/x:type/x:reference
            | $my:current-config/x:type/x:struct/x:member
            | $my:current-config/x:symbol-def/x:symbolic-value,
            $my:current-artifact)">
			<call-template name="sysinclude">
				<with-param name="filename" select="." />
			</call-template>
		</for-each>

		<call-template name="sysinclude">
			<with-param name="filename">
				<text>TSAutosar.h</text>
			</with-param>
		</call-template>
		<call-template name="unguarded-macro">
			<with-param name="name">
				<text>TS_RELOCATABLE_CFG_ENABLE</text>
			</with-param>
			<with-param name="value" select="my:on-off(my:get-pre-compile-flag('relocatable-enable'))" />
		</call-template>
		<call-template name="unguarded-macro">
			<with-param name="name">
				<text>TS_PB_CFG_PTR_CLASS</text>
			</with-param>
			<with-param name="value" select="fn:concat(fn:upper-case($my:module), '_APPL_CONST')" />
		</call-template>
		<call-template name="sysinclude">
			<with-param name="filename">
				<text>TSPBConfig_Types.h</text>
			</with-param>
		</call-template>

		<value-of select="$my:NL" />
	</template>

	<template match="x:xgen" mode="type-definitions">
        <apply-imports />

		<apply-templates select="$my:current-config/x:type">
			<sort select="my:computeTypeWeight(.)" data-type="number" />
		</apply-templates>
	</template>

	<template match="x:xgen" mode="end-file">
		<value-of select="$my:NL" />
		<value-of select="my:normalize-end-of-line($h-undefines)" />
		<value-of select="$my:NL" />
		<text>#endif /* </text>
		<value-of select="$my:include-guard-name" />
		<text> */</text>
		<value-of select="$my:NL" />
        <apply-imports />
	</template>

</stylesheet>