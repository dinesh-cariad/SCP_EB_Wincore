<?xml version="1.0" encoding="UTF-8"?>
<stylesheet version="2.0" exclude-result-prefixes="fn my x xml xs" xmlns="http://www.w3.org/1999/XSL/Transform" xmlns:x="http://www.elektrobit.com/2013/xgen" xmlns:my="http://www.elektrobit.com/2013/xgen/local" xmlns:fn="http://www.w3.org/2005/xpath-functions" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:xml="http://www.w3.org/XML/1998/namespace">

	<!-- Flatten nested instance structures -->

	<import href="resource://details/generic.xsl" />

	<output method="xml" indent="yes" />
	
	<template match="@*|node()">
		<copy>
			<apply-templates select="@*|node()" />
		</copy>
	</template>

	<template match="x:xgen|x:configuration|x:module|x:instance[not(x:field)]">
		<copy>
			<apply-templates select="@*|node()" />
		</copy>
	</template>

	<template match="x:instance[x:field]">
		<copy>
			<apply-templates select="@*" />
			<apply-templates select="element()[local-name() != 'field']" />

			<variable name="type" select="key('types-by-name', my:resolve-type-references(@type))" />
			<variable name="members" select="$type/x:struct/x:member" />

			<!-- An instance is just another field -->
			<call-template name="field">
				<with-param name="field" select="." />
				<with-param name="type" select="$type" />
				<with-param name="address" select="@address" />
				<with-param name="module-offset" select="@address" />
				<with-param name="is-misaligned" select="false()" />
			</call-template>
		</copy>
	</template>

	<template name="struct-fields">
		<param name="fields" as="element()*" />
		<param name="members" as="element()*" />
		<param name="address" as="xs:integer" />
		<param name="module-offset" as="xs:integer" />
        <param name="start-address" as="xs:integer" />
		<param name="struct-type" as="element()" />

		<choose>
			<when test="exists($fields)">
				<variable name="field" select="$fields[1]" />
				<variable name="member" select="$members[1]" />
				<variable name="type" as="element()" select="my:get-member-type($member)" />

				<variable name="current-address" as="xs:integer" select="my:calculate-aligned-value($address, $type/@alignment-requirement)" />
				<variable name="current-module-offset" as="xs:integer" select="if(exists($field/@included-from-module))
                then $current-address
                else $module-offset" />
				<variable name="size" as="xs:integer" select="$type/@byte-size" />

				<call-template name="field">
					<with-param name="field" select="$field" />
					<with-param name="type" select="$type" />
					<with-param name="address" select="$current-address" />
					<with-param name="module-offset" select="$current-module-offset" />
					<with-param name="is-misaligned" select="$current-address != $address" />
				</call-template>

				<call-template name="struct-fields">
					<with-param name="fields" select="fn:subsequence($fields, 2)" />
					<with-param name="members" select="fn:subsequence($members, 2)" />
					<with-param name="address" select="$current-address + $size" />
					<with-param name="module-offset" select="$current-module-offset" />
					<with-param name="start-address" select="$start-address"/>
					<with-param name="struct-type" select="$struct-type" />
				</call-template>
			</when>
			<otherwise>
				<variable name="byte-size" as="xs:integer" select="$struct-type/@byte-size" />

				<call-template name="break">
					<with-param name="id" select="nil" />
					<with-param name="address" select="$start-address + $byte-size" />
					<with-param name="module-offset" select="$module-offset" />
					<with-param name="reason">
						<text>struct-end</text>
					</with-param>
				</call-template>
			</otherwise>
		</choose>
	</template>

	<template name="array-fields">
		<param name="fields" as="element()*" />
		<param name="type" as="element()" />
		<param name="address" as="xs:integer" />
		<param name="module-offset" as="xs:integer" />
        <param name="start-address" as="xs:integer" />
		<param name="array-type" as="element()" />

		<choose>
			<when test="exists($fields)">
				<variable name="field" select="$fields[1]" />

				<variable name="current-address" select="my:calculate-aligned-value($address, $type/@alignment-requirement)" />
				<variable name="current-module-offset" as="xs:integer" select="if(exists($field/@included-from-module))
                then $current-address
                else $module-offset" />
				<variable name="size" as="xs:integer" select="$type/@byte-size" />

				<call-template name="field">
					<with-param name="field" select="$field" />
					<with-param name="type" select="$type" />
					<with-param name="address" select="$current-address" />
					<with-param name="module-offset" select="$current-module-offset" />
					<with-param name="is-misaligned" select="$current-address != $address" />
				</call-template>

				<call-template name="array-fields">
					<with-param name="fields" select="fn:subsequence($fields, 2)" />
					<with-param name="type" select="$type" />
					<with-param name="address" select="$current-address + $size" />
					<with-param name="module-offset" select="$current-module-offset" />
                    <with-param name="start-address" select="$start-address"/>
					<with-param name="array-type" select="$array-type" />
				</call-template>
			</when>
			<otherwise>
				<variable name="byte-size" as="xs:integer" select="$array-type/@byte-size" />
				<call-template name="break">
					<with-param name="id" select="nil" />
					<with-param name="address" select="$start-address + $byte-size" />
					<with-param name="module-offset" select="$module-offset" />
					<with-param name="reason">
						<text>array-end</text>
					</with-param>
				</call-template>
			</otherwise>
		</choose>
	</template>

	<template name="field">
		<param name="field" as="element()" />
		<param name="type" as="element()" />
		<param name="address" as="xs:integer" />
		<param name="module-offset" as="xs:integer" />
		<param name="is-misaligned" as="xs:boolean" />

		<choose>
			<when test="$type/x:reference/@count">
				<call-template name="break">
					<with-param name="id" select="$field/@id" />
					<with-param name="address" select="$address" />
					<with-param name="module-offset" select="$module-offset" />
					<with-param name="reason">
						<text>array-start</text>
					</with-param>
				</call-template>
				<call-template name="array-fields">
					<with-param name="fields" select="$field/x:field" />
					<with-param name="type" select="key('types-by-name', $type/x:reference/@type)" />
					<with-param name="address" select="$address" />
					<with-param name="module-offset" select="$module-offset" />
                    <with-param name="start-address" select="my:calculate-aligned-value($address, $type/@alignment-requirement)"/>
					<with-param name="array-type" select="$type" />
				</call-template>
			</when>
			<when test="$type/x:struct">
				<call-template name="break">
					<with-param name="id" select="$field/@id" />
					<with-param name="address" select="$address" />
					<with-param name="module-offset" select="$module-offset" />
					<with-param name="reason">
						<text>struct-start</text>
					</with-param>
				</call-template>
				<call-template name="struct-fields">
					<with-param name="fields" select="$field/x:field" />
					<with-param name="members" select="$type/x:struct/x:member" />
					<with-param name="address" select="$address" />
					<with-param name="module-offset" select="$module-offset" />
                    <with-param name="start-address" select="my:calculate-aligned-value($address, $type/@alignment-requirement)"/>
                    <with-param name="struct-type" select="$type" />
				</call-template>
			</when>
			<otherwise>
				<if test="$is-misaligned">
					<call-template name="break">
						<with-param name="id" select="nil" />
						<with-param name="address" select="$address" />
						<with-param name="module-offset" select="$module-offset" />
						<with-param name="reason">
							<text>alignment-correction</text>
						</with-param>
					</call-template>
				</if>
				<x:field address="{$address}" module-offset="{$module-offset}" byte-size="{$type/@byte-size}">
					<copy-of select="$field/@*" />
					<copy-of select="$field/*" />
				</x:field>
			</otherwise>
		</choose>
	</template>

	<template name="break">
		<param name="id" as="xs:string?" />
		<param name="address" as="xs:integer" />
		<param name="module-offset" as="xs:integer" />
		<param name="reason" as="xs:string" />
		<x:field address="{$address}" module-offset="{$module-offset}">
			<if test="$id">
				<attribute name="id" select="$id" />
			</if>
			<x:break reason="{$reason}" />
		</x:field>
	</template>
</stylesheet>