<?xml version="1.0" encoding="UTF-8"?>
<stylesheet version="2.0" exclude-result-prefixes="fn my x xml xs" xmlns="http://www.w3.org/1999/XSL/Transform" xmlns:x="http://www.elektrobit.com/2013/xgen" xmlns:my="http://www.elektrobit.com/2013/xgen/local" xmlns:fn="http://www.w3.org/2005/xpath-functions" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:xml="http://www.w3.org/XML/1998/namespace">

    <import href="resource://details/generic.xsl" />

	<output method="xml" indent="yes" />

	<template match="@*|node()">
		<copy>
			<apply-templates select="@*|node()" />
		</copy>
	</template>

	<template match="x:field">
		<copy>
			<attribute name="id" select="generate-id(.)" />
			<apply-templates select="@*|node()" />
		</copy>
	</template>

	<template match="x:ref">
	
        <variable name="instance" select="ancestor::x:instance" />
		<variable name="included-field" select="ancestor::x:field[exists(@included-from-module)]" />
		
		<variable name="root-field" select="if(exists($included-field)) then $included-field else $instance"/>
		<variable name="root-type" select="key('types-by-name', my:resolve-type-references($root-field/@type))" />

		<copy>
			<if test="exists(x:struct-field|x:array-field)">
				<attribute name="target" select="my:get-reference-id(x:struct-field|x:array-field, $root-field, $root-type)" />
			</if>
			<apply-templates select="@*|node()" />
		</copy>
	</template>

    <template match="x:member-count">
    
        <variable name="instance" select="ancestor::x:instance" />
        <variable name="included-field" select="ancestor::x:field[exists(@included-from-module)]" />
        
        <variable name="root-field" select="if(exists($included-field)) then $included-field else $instance"/>
        
        <x:int>
            <apply-templates select="(x:struct-field|x:array-field)[1]">
                <with-param name="typename" select="my:resolve-type-references($root-field/@type)" />
            </apply-templates>
        </x:int>
    </template>

    <template match="x:member-count/x:struct-field">
        <param name="typename" as="xs:string" />

        <variable name="next" select="following-sibling::*[local-name()='struct-field' or local-name()='array-field']" />
        <variable name="type" select="key('types-by-name', $typename)" />
        <variable name="field-typename" select="$type/x:struct/x:member[@name = current()/@name]/@type" />

        <choose>
            <when test="not($next)">
                <variable name="field-type" select="key('types-by-name', $field-typename)" />
                <value-of select="$field-type/x:reference/@count" />
            </when>
            <otherwise>
                <apply-templates select="$next">
                    <with-param name="typename" select="$field-typename" />
                </apply-templates>
            </otherwise>
        </choose>
    </template>

    <template match="x:member-count/x:array-field">
        <param name="typename" as="xs:string" />

        <variable name="next" select="following-sibling::*[local-name()='struct-field' or local-name()='array-field']" />
        <variable name="type" select="key('types-by-name', $typename)" />
        <variable name="field-typename" select="$type/x:reference/@type" />

        <choose>
            <when test="not($next)">
                <variable name="field-type" select="key('types-by-name', $field-typename)" />
                <value-of select="$field-type/x:reference/@count" />
            </when>
            <otherwise>
                <apply-templates select="$next">
                    <with-param name="typename" select="$type/x:reference/@type" />
                </apply-templates>
            </otherwise>
        </choose>
    </template>

	<function name="my:get-reference-id">
		<param name="reference-descriptors" as="element()*" />
		<param name="parent" as="element()" />
		<param name="parent-type" as="element()" />

		<choose>
			<when test="fn:empty($reference-descriptors)">
				<sequence select="generate-id($parent)" />
			</when>
			<otherwise>
				<variable name="descriptor" select="$reference-descriptors[1]" />

				<choose>
					<when test="$parent-type/x:struct">
						<if test="not($descriptor/local-name() = 'struct-field')">
							<message terminate="yes">
								<text>Internal error in annotate-fields::my:get-reference-id(): Type is a struct, but reference does not select a struct field</text>
							</message>
						</if>

						<variable name="member" select="$parent-type/x:struct/x:member[@name = $descriptor/@name]" />
						<variable name="field" select="$parent/x:field[1 + count($member/preceding-sibling::x:member)]" />
						<variable name="type" select="$root/key('types-by-name', $member/@type)" />
						<sequence select="my:get-reference-id(fn:subsequence($reference-descriptors, 2), $field, $type)" />
					</when>
					<when test="$parent-type/x:reference/@count">
						<if test="not($descriptor//local-name() = 'array-field')">
							<message terminate="yes">
								<text>Internal error in annotate-fields::my:get-reference-id(): Type is an array, but reference does not select an array field</text>
							</message>
						</if>

						<variable name="field" select="$parent/x:field[1 + $descriptor/@index]" />
						<variable name="type" select="$root/key('types-by-name', $parent-type/x:reference/@type)" />
						<sequence select="my:get-reference-id(fn:subsequence($reference-descriptors, 2), $field, $type)" />
					</when>
					<otherwise>
						<message terminate="yes">
							<text>Internal error in annotate-fields::my:get-reference-id(): Type is neither a struct nor an an array, but reference selects a sub-field</text>
						</message>
					</otherwise>
				</choose>
			</otherwise>
		</choose>
	</function>
</stylesheet>