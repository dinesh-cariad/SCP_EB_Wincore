<?xml version="1.0" encoding="UTF-8"?>
<stylesheet version="2.0" xmlns="http://www.w3.org/1999/XSL/Transform" xmlns:my="http://www.elektrobit.com/2013/xgen/local" xmlns:x="http://www.elektrobit.com/2013/xgen" xmlns:xml="http://www.w3.org/XML/1998/namespace" xmlns:fn="http://www.w3.org/2005/xpath-functions" xmlns:xs="http://www.w3.org/2001/XMLSchema">

	<import href="resource://details/generic.xsl" />

	<output method="text" />

	<variable name="h-undefines">
		<call-template name="undef">
			<with-param name="name">
				<text>TS_RELOCATABLE_CFG_ENABLE</text>
			</with-param>
		</call-template>
		<call-template name="undef">
			<with-param name="name">
				<text>TS_PB_CFG_PTR_CLASS</text>
			</with-param>
		</call-template>
	</variable>

	<variable name="misra-c-top-comment">
				<text>/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 1.3 (required)
 *    There shall be no occurrence of undefined or critical unspecified behaviour.
 *
 *    Reason:
 *    The postbuild macro TS_MAKEREF2CFG requires the usage of the
 *    C standard macro 'offsetof'.
 *    According to the C90 standard, the usage of 'offsetof' produces
 *    an undefined behaviour when the member designator parameter
 *    is a bit-field or when it is an invalid right operator of the
 *    '.' operator for the type parameter.
 *    This is not the case here as the parameter of TS_MAKEREF2CFG
 *    is always a member of TS_PB_CFG_LAYOUT_TYPE and bitfields are
 *    never used when defining the postbuild structure members.
 *
 * MISRAC2012-2) Deviated Rule: 11.4 (advisory)
 *    A cast should not be performed between a pointer type and an integral
 *    type.
 *
 *    Reason:
 *    Relocatable postbuild macro requires the cast between pointer and
 *    integral type.
 *
 * MISRAC2012-3) Deviated Rule: 11.9 (required)
 *    The macro NULL shall be the only permitted form of integer null pointer constant.
 *
 *    Reason:
 *    Macros are used in order to check the alignment for different data types.
 *
 */</text>
</variable>

	<variable name="top-comment-text">
		<text>


/**
 * \file
 *
 * \brief AUTOSAR M4_FILEHEADER_MODULE_NAME
 *
 * This file contains the implementation of the AUTOSAR
 * module M4_FILEHEADER_MODULE_NAME.
 *
 * \version 1.4.0
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
</text>
	</variable>

	<template match="/">
		<apply-templates select="*" mode="start-file">
			<with-param name="indentation" select="''" tunnel="yes" />
		</apply-templates>
		<apply-templates select="*" mode="inclusions" />
		<apply-templates select="*" mode="macros" />
		<apply-templates select="*" mode="type-definitions" />
		<apply-templates select="*" mode="external-function-declarations" />
		<apply-templates select="*" mode="internal-function-declarations" />
		<apply-templates select="*" mode="external-constants" />
		<apply-templates select="*" mode="internal-constants" />
		<apply-templates select="*" mode="external-data" />
		<apply-templates select="*" mode="internal-data" />
		<apply-templates select="*" mode="external-function-definitions" />
		<apply-templates select="*" mode="internal-function-definitions" />
		<apply-templates select="*" mode="end-file" />
	</template>

	<template match="x:type">
		<call-template name="doxygen-comment" />
		<text>typedef </text>
		<apply-templates select="x:reference|x:struct" />
		<text> </text>
		<value-of select="@name" />
		<apply-templates select="./x:reference/@count" />
		<text>;</text>
		<call-template name="attach-comment" />
		<value-of select="$my:NL" />
		<value-of select="$my:NL" />
	</template>

	<template match="x:type/x:struct">
		<text>struct /* </text>
		<value-of select="../@name" />
		<text> */ {</text>
		<value-of select="$my:NL" />
		<apply-templates />
		<text>}</text>
	</template>

	<template match="x:type/x:struct/x:member">
		<value-of select="$my:indent" />
		<apply-templates select="x:compiler-abstraction" />
		<text> </text>
		<value-of select="@name" />
		<apply-templates select="@count" />
		<text>;</text>
		<call-template name="attach-comment" />
		<value-of select="$my:NL" />
	</template>

	<template match="x:var|x:const">
		<value-of select="fn:upper-case(local-name())" />
		<text>( </text>
		<value-of select="../../@type" />
		<text>, </text>
		<value-of select="@memory-class" />
		<text> )</text>
	</template>

	<template match="x:ref2cfg|x:ref2var">
		<text>TS_</text>
		<value-of select="fn:upper-case(local-name())" />
		<text>( </text>
		<value-of select="../../@type" />
		<text> ) </text>
	</template>

	<template match="@count">
		<text>[</text>
		<value-of select="." />
		<text>]</text>
	</template>

	<template match="x:symbol-def" mode="macros">
		<call-template name="macro">
			<with-param name="name" select="@name" />
			<with-param name="value">
				<value-of select="@prefix" />
				<apply-templates />
				<value-of select="@suffix" />
			</with-param>
		</call-template>
	</template>

	<template match="x:string-value">
		<text>"</text>
		<value-of select="text()" />
		<text>"</text>
	</template>

	<template match="x:symbolic-value|x:integer-value">
		<value-of select="text()" />
	</template>

	<template match="x:parameter">
		<apply-templates select="//x:parameter[./@name=current()/@reference]/*" />
	</template>

	<template match="x:boolean-value">
		<value-of select="my:on-off(.)" />
	</template>

	<template match="x:reference-value">
		<text>(</text>
		<apply-templates />
		<text>)</text>
	</template>

	<template match="x:instance-ref">
		<value-of select="@name" />
	</template>

	<template match="x:struct-field">
		<if test="exists(preceding-sibling::x:instance-ref) or exists(preceding-sibling::x:struct-field) or exists(preceding-sibling::x:array-field)">
			<text>.</text>
		</if>
		<value-of select="@name" />
	</template>

	<template match="x:array-field">
		<text>[</text>
		<value-of select="@index" />
		<text>]</text>
	</template>

	<template name="top-comment">
                <!-- Replace module name and version from ToolExtensions with the actual module -->
	        <value-of select="fn:replace(fn:replace(my:normalize-end-of-line($top-comment-text), 'M4_FILEHEADER_MODULE_NAME', $my:module), 'M4_MODULE_VERSION', $my:version)" />
		<value-of select="$my:NL" />
	</template>

	<template match="x:xgen" mode="start-file">
		<call-template name="top-comment" />
	</template>

	<template match="x:xgen" mode="end-file">
		<call-template name="section">
			<with-param name="text">
				<text>end of file</text>
			</with-param>
		</call-template>
	</template>

	<template match="x:xgen" mode="inclusions">
		<call-template name="section">
			<with-param name="text">
				<text>inclusions</text>
			</with-param>
		</call-template>
	</template>

	<template match="x:xgen" mode="macros">
		<call-template name="section">
			<with-param name="text">
				<text>macros</text>
			</with-param>
		</call-template>
		<if test="fn:ends-with($my:current-artifact-type, '-h')">
			<apply-templates select="$my:current-config" mode="macros" />
		</if>
	</template>

	<template match="x:xgen" mode="type-definitions">
		<call-template name="section">
			<with-param name="text">
				<text>type definitions</text>
			</with-param>
		</call-template>
	</template>

	<template match="x:xgen" mode="external-function-declarations">
		<call-template name="section">
			<with-param name="text">
				<text>external function declarations</text>
			</with-param>
		</call-template>
	</template>

	<template match="x:xgen" mode="internal-function-declarations">
		<call-template name="section">
			<with-param name="text">
				<text>internal function declarations</text>
			</with-param>
		</call-template>
	</template>

	<template match="x:xgen" mode="external-constants">
		<call-template name="section">
			<with-param name="text">
				<text>external constants</text>
			</with-param>
		</call-template>
	</template>

	<template match="x:xgen" mode="internal-constants">
		<call-template name="section">
			<with-param name="text">
				<text>internal constants</text>
			</with-param>
		</call-template>
	</template>

	<template match="x:xgen" mode="external-data">
		<call-template name="section">
			<with-param name="text">
				<text>external data</text>
			</with-param>
		</call-template>
	</template>

	<template match="x:xgen" mode="internal-data">
		<call-template name="section">
			<with-param name="text">
				<text>internal data</text>
			</with-param>
		</call-template>
	</template>

	<template match="x:xgen" mode="external-function-definitions">
		<call-template name="section">
			<with-param name="text">
				<text>external function definitions</text>
			</with-param>
		</call-template>
	</template>

	<template match="x:xgen" mode="internal-function-definitions">
		<call-template name="section">
			<with-param name="text">
				<text>internal function definitions</text>
			</with-param>
		</call-template>
	</template>

	<template name="sysinclude">
		<param name="filename" as="xs:string" />

		<text>#include &lt;</text>
		<value-of select="$filename" />
		<text>&gt;</text>
		<value-of select="$my:NL" />
	</template>

	<template name="section">
		<param name="text" as="xs:string" />

		<value-of select="$my:NL" />
		<call-template name="line-comment">
			<with-param name="text">
				<value-of select="$text" />
			</with-param>
			<with-param name="fill">
				<text>=</text>
			</with-param>
		</call-template>
		<value-of select="$my:NL" />
	</template>

	<template name="subsection">
		<param name="text" as="xs:string" />
		<call-template name="line-comment">
			<with-param name="text">
				<value-of select="$text" />
			</with-param>
			<with-param name="fill">
				<text>-</text>
			</with-param>
		</call-template>
		<value-of select="$my:NL" />
	</template>

	<template name="line-comment">
		<param name="text" as="xs:string" />
		<param name="fill" as="xs:string" />

		<text>/*</text>
		<for-each select="1 to 18">
			<value-of select="$fill" />
		</for-each>
		<text>[</text>
		<value-of select="$text" />
		<text>]</text>
		<for-each select="1 to (55 - fn:string-length($text))">
			<value-of select="$fill" />
		</for-each>
		<text>*/</text>
		<value-of select="$my:NL" />
	</template>

	<!-- Attach the contents of the 'comment' attribute of the current element as a C-style comment to the current output line. -->
	<template name="attach-comment">
		<if test="@comment">
			<text> /* </text>
			<value-of select="@comment" />
			<text> */</text>
		</if>
	</template>

	<template name="doxygen-comment">
		<if test="./x:comment">
			<text>/** </text>
			<value-of select="$my:NL" />
			<value-of select="x:comment[1]/text()" />
			<value-of select="$my:NL" />
			<text> */</text>
			<value-of select="$my:NL" />
		</if>
	</template>

	<template name="macro">
		<param name="name" as="xs:string" />
		<param name="value" as="xs:string?" />
		<param name="comment" as="xs:string?" required="no" select="nil" />

		<call-template name="macro-guard">
			<with-param name="name" select="$name" />
		</call-template>
		<call-template name="unguarded-macro">
			<with-param name="name" select="$name" />
			<with-param name="value" select="$value" />
			<with-param name="comment" select="if(exists($comment)) then $comment else nil" />
		</call-template>

		<value-of select="$my:NL" />
		<value-of select="$my:NL" />
	</template>

	<template name="undef">
		<param name="name" as="xs:string" />
		<value-of select="$my:NL" />
		<text>#undef </text>
		<value-of select="$name" />
		<value-of select="$my:NL" />
	</template>

	<template name="unguarded-macro">
		<param name="name" as="xs:string" />
		<param name="value" as="xs:string?" />
		<param name="comment" as="xs:string?" required="no" select="nil" />

		<if test="exists($comment)">
			<text>/** </text>
			<value-of select="$comment" />
			<text> */</text>
			<value-of select="$my:NL" />
		</if>
		<text>#define </text>
		<value-of select="$name" />
		<if test="exists($value)">
			<text> </text>
			<value-of select="$value" />
		</if>
		<value-of select="$my:NL" />
	</template>

	<template name="macro-guard">
		<param name="name" as="xs:string" />

		<text>#if (defined </text>
		<value-of select="$name" />
		<text>) /* To prevent double definition */</text>
		<value-of select="$my:NL" />

		<text>#error </text>
		<value-of select="$name" />
		<text> already defined</text>
		<value-of select="$my:NL" />

		<text>#endif /* (defined </text>
		<value-of select="$name" />
		<text>) */</text>
		<value-of select="$my:NL" />
		<value-of select="$my:NL" />
	</template>

	<template name="begin-memory-section">
		<param name="name" as="xs:string" />
		<call-template name="memsec">
			<with-param name="name" select="$name" />
			<with-param name="infix">
				<text>START</text>
			</with-param>
		</call-template>
		<value-of select="$my:NL" />
	</template>

	<template name="end-memory-section">
		<param name="name" as="xs:string" />
		<value-of select="$my:NL" />
		<call-template name="memsec">
			<with-param name="name" select="$name" />
			<with-param name="infix">
				<text>STOP</text>
			</with-param>
		</call-template>
	</template>

	<template name="memsec">
		<param name="name" as="xs:string" />
		<param name="infix" as="xs:string" />

		<text>#define </text>
		<value-of select="fn:upper-case($my:module)" />
		<text>_</text>
		<value-of select="$infix" />
		<text>_</text>
		<if test="not(starts-with($name, 'SEC_'))">
			<text>SEC_</text>
		</if>
		<value-of select="$name" />
		<value-of select="$my:NL" />
		<call-template name="sysinclude">
			<with-param name="filename" select="concat($my:module,'_MemMap.h')" />
		</call-template>
	</template>

	<template name="indent">
		<param name="indentation" as="xs:string" tunnel="yes" />
		<value-of select="$indentation" />
	</template>

	<function name="my:make-unsigned-value">
		<param name="value" as="xs:integer" />
		<sequence select="concat($value, 'U')" />
	</function>
</stylesheet>
