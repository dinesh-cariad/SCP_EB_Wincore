<?xml version="1.0" encoding="UTF-8"?>
<stylesheet version="2.0" xmlns="http://www.w3.org/1999/XSL/Transform" xmlns:my="http://www.elektrobit.com/2013/xgen/local" xmlns:x="http://www.elektrobit.com/2013/xgen" xmlns:xml="http://www.w3.org/XML/1998/namespace" xmlns:fn="http://www.w3.org/2005/xpath-functions" xmlns:xs="http://www.w3.org/2001/XMLSchema">

	<import href="resource://details/generic-text.xsl" />

	<template match="x:xgen" mode="start-file">
        <apply-imports />
		<value-of select="$my:NL" />
		<value-of select="my:normalize-end-of-line($misra-c-top-comment)" />
		<value-of select="$my:NL" />
	</template>

	<template match="x:xgen" mode="inclusions">
        <apply-imports />

		<choose>
			<when test="exists($my:current-config/x:memory-section/x:instance)">
			    <variable name="instance" select="$my:current-config/x:memory-section/x:instance[1]"/>
			    
				<for-each select="my:get-included-file-names(
		            $my:current-config/x:memory-section/x:instance
		            | $my:current-config/x:memory-section/x:instance//x:field/x:symbol
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

				<call-template name="sysinclude">
					<with-param name="filename">
						<text>Compiler.h</text>
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
						<text>TS_PB_CFG_NAME</text>
					</with-param>
					<with-param name="value" select="$instance/@name" />
				</call-template>

				<call-template name="unguarded-macro">
					<with-param name="name">
						<text>TS_PB_CFG_LAYOUT_TYPE</text>
					</with-param>
					<with-param name="value" select="$instance/@type" />
				</call-template>

				<call-template name="sysinclude">
					<with-param name="filename">
						<text>TSPBConfig_Init.h</text>
					</with-param>
				</call-template>
			</when>

			<otherwise>
				<call-template name="sysinclude">
					<with-param name="filename">
						<text>TSCompiler.h</text>
					</with-param>
				</call-template>
			</otherwise>
		</choose>
	</template>

	<template match="x:xgen" mode="type-definitions">
        <apply-imports />

		<if test="empty($my:current-config/x:memory-section/x:instance[exists(x:parameter[@name='postbuild-configuration-name'])])">
			<text>TS_PREVENTEMPTYTRANSLATIONUNIT</text>
		</if>
	</template>

	<template match="x:xgen" mode="external-constants">
		<call-template name="section">
			<with-param name="text">
				<text>external constants</text>
			</with-param>
		</call-template>

		<apply-templates select="$my:current-config/x:memory-section" />
	</template>

	<template match="x:memory-section">
		<call-template name="begin-memory-section">
			<with-param name="name" select="@name" />
		</call-template>

		<apply-templates select="x:instance" />

		<call-template name="end-memory-section">
			<with-param name="name" select="@name" />
		</call-template>
	</template>

	<template match="x:instance[exists(x:parameter[@name='postbuild-configuration-name'])]">
		<text>/* Deviation MISRAC2012-1, MISRAC2012-2, MISRAC2012-3 &lt;START> */</text>
		<value-of select="$my:NL" />
		<apply-templates select="x:comment">
			<with-param name="indentation" select="''" tunnel="yes" />
		</apply-templates>
		<value-of select="@type" />
		<text> </text>
		<value-of select="@name" />
		<text> = {</text>
		<value-of select="$my:NL" />
		<apply-templates select="x:field">
			<with-param name="indentation" select="$my:indent" tunnel="yes" />
		</apply-templates>
		<text>};</text>
		<value-of select="$my:NL" />
		<value-of select="$my:NL" />
		<text>/* Deviation MISRAC2012-1, MISRAC2012-2, MISRAC2012-3 &lt;STOP> */</text>
		<value-of select="$my:NL" />
	</template>

	<template match="x:field">
		<param name="indentation" as="xs:string" tunnel="yes" />

		<if test="exists(@included-from-module)">
            <value-of select="$my:NL" />
			<call-template name="undef">
				<with-param name="name">
					<text>TS_PB_CFG_LAYOUT_TYPE</text>
				</with-param>
			</call-template>
			<call-template name="unguarded-macro">
				<with-param name="name">
					<text>TS_PB_CFG_LAYOUT_TYPE</text>
				</with-param>
				<with-param name="value" select="@type" />
			</call-template>
			<call-template name="undef">
				<with-param name="name">
					<text>TS_PB_CFG_NAME</text>
				</with-param>
			</call-template>
			<call-template name="unguarded-macro">
				<with-param name="name">
					<text>TS_PB_CFG_NAME</text>
				</with-param>
				<with-param name="value" select="concat(ancestor::x:instance/@name, '.', @member)" />
			</call-template>
			<call-template name="sysinclude">
				<with-param name="filename">
					<text>TSPBConfig_Init.h</text>
				</with-param>
			</call-template>
            <value-of select="$my:NL" />
		</if>

		<call-template name="item-prefix" />
		<apply-templates select="element()">
			<with-param name="indentation" select="concat($indentation, $my:indent)" tunnel="yes" />
		</apply-templates>
		<call-template name="item-suffix" />
	</template>

	<template match="x:ref">
		<choose>
			<when test="empty(*)">
				<!-- NULL reference -->
				<text>TS_MAKENULLREF2CFG</text>
			</when>
			<otherwise>
				<text>TS_MAKEREF2CFG( </text>
				<apply-templates select="x:struct-field|x:array-field" />
				<text> )</text>
			</otherwise>
		</choose>

	</template>

	<template match="x:int">
		<value-of select="fn:upper-case(../@type)" />
		<text>_C( </text>
		<value-of select="." />
		<text> )</text>
	</template>

    <template match="x:symbol">
        <value-of select="." />
    </template>

	<template name="item-prefix">
		<call-template name="indent" />
		<if test="x:field">
			<text>{</text>
			<call-template name="field-comment" />
			<value-of select="$my:NL" />
		</if>
	</template>

	<template name="item-suffix">

		<if test="x:field">
			<call-template name="indent" />
			<text>}</text>
		</if>
		<if test="following-sibling::x:field">
			<text>,</text>
		</if>

		<if test="not(x:field)">
			<call-template name="field-comment" />
		</if>

		<value-of select="$my:NL" />
	</template>

	<template name="field-comment">
		<text> /* </text>
		<value-of select="my:get-field-comment-text(.)" />
		<if test="@comment">
			<text> -- </text>
			<value-of select="@comment" />
		</if>
		<text> */</text>
	</template>

	<function name="my:get-field-comment-text">
		<param name="field" as="element()" />

		<sequence select="if($field/@member) then $field/@member
			else if($field/@index) then concat(
				 my:get-field-comment-text($field/parent::x:field),
				 '[',
				 $field/@index,
				 ']'
				 )
			else fn:error(
                fn:QName('http://www.elektrobit.com/2013/xgen/local', 'err:unknown-field-type'),
				'Unknown field type',
				$field)" />

	</function>

</stylesheet>