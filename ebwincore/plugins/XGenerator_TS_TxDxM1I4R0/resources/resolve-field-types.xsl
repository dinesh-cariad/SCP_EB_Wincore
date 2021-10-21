<?xml version="1.0" encoding="UTF-8"?>
<stylesheet version="2.0" exclude-result-prefixes="fn my x xml xs" xmlns="http://www.w3.org/1999/XSL/Transform" xmlns:x="http://www.elektrobit.com/2013/xgen" xmlns:my="http://www.elektrobit.com/2013/xgen/local" xmlns:fn="http://www.w3.org/2005/xpath-functions" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:xml="http://www.w3.org/XML/1998/namespace">

    <import href="resource://details/generic.xsl" />

	<output method="xml" indent="yes" />

	<template match="@*|node()">
		<copy>
			<apply-templates select="@*|node()" />
		</copy>
	</template>

	<template match="x:instance">
		<copy>
			<apply-templates select="@*|node()">
				<with-param name="parent-type" select="key('types-by-name', my:resolve-type-references(@type))" tunnel="yes" />
			</apply-templates>
		</copy>
	</template>

	<template match="x:field">
		<param name="parent-type" as="element()" tunnel="yes" />

		<copy>
			<apply-templates select="@*" />

			<choose>
				<when test="$parent-type/x:reference/@count or $parent-type/x:struct">
					<apply-templates select="$parent-type/x:reference|$parent-type/x:struct" mode="field-info">
						<with-param name="field" select="current()" />
					</apply-templates>
				</when>
				<otherwise>
					<message terminate="yes" select="'Parent type', $parent-type/@name, 'is neither a struct nor an array'" />
				</otherwise>
			</choose>
			
		</copy>
	</template>

	<template match="x:struct" mode="field-info">
		<param name="field" as="element()" />

		<variable name="member" select="x:member[1 + count($field/preceding-sibling::x:field)]" as="element()" />
		<variable name="member-type" select="my:resolve-type-references($member/@type)" as="xs:string" />
		
		<attribute name="type" select="$member-type" />
		<attribute name="member" select="$member/@name" />

        <apply-templates select="$field/@*|$field/node()">
            <with-param name="parent-type" select="key('types-by-name', $member-type)" tunnel="yes" />
        </apply-templates>

	</template>

	<template match="x:reference" mode="field-info">
		<param name="field" as="element()" />

        <attribute name="type" select="my:resolve-type-references(@type)" />
		<attribute name="index" select="count($field/preceding-sibling::x:field)" />

        <apply-templates select="$field/@*|$field/node()">
            <with-param name="parent-type" select="key('types-by-name', @type)" tunnel="yes" />
        </apply-templates>

	</template>

</stylesheet>