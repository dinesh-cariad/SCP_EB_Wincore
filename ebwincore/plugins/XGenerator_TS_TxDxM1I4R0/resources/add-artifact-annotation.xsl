<?xml version="1.0" encoding="UTF-8"?>
<stylesheet version="2.0" exclude-result-prefixes="fn my x xml xs" xmlns="http://www.w3.org/1999/XSL/Transform" xmlns:x="http://www.elektrobit.com/2013/xgen" xmlns:my="http://www.elektrobit.com/2013/xgen/local" xmlns:fn="http://www.w3.org/2005/xpath-functions" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:xml="http://www.w3.org/XML/1998/namespace">

	<import href="resource://details/generic.xsl" />

	<output method="xml" indent="yes" />

	<template match="@*|node()">
		<copy>
			<apply-templates select="@*|node()" />
		</copy>
	</template>

	<template match="x:type/x:reference|x:struct/x:member|x:instance">
		<variable name="type" select="key('types-by-name', @type)" as="element()?" />

		<copy>
			<if test="fn:exists($type)">
				<variable name="file" select="my:get-h-artifact($type/ancestor::x:configuration/processing-instruction('artifact'))" />
				<if test="fn:exists($file)">
					<attribute name="defining-artifact" select="$file" />
				</if>
			</if>
			<apply-templates select="@*|node()" />
		</copy>
	</template>

    <template match="x:symbolic-value|x:symbol">
        <variable name="symbol" select="key('symbols-by-name', text())" as="element()?" />

        <copy>
            <if test="fn:exists($symbol)">
                <variable name="file" select="my:get-h-artifact($symbol/ancestor::x:configuration/processing-instruction('artifact'))" />
                <if test="fn:exists($file)">
                    <attribute name="defining-artifact" select="$file" />
                </if>
            </if>
            <apply-templates select="@*|node()" />
        </copy>
    </template>

	<function name="my:get-h-artifact" as="xs:string?">
		<param name="artifacts" as="node()*" />

		<choose>
			<when test="fn:empty($artifacts)">
				<sequence select="()" />
			</when>
			<when test="fn:ends-with(my:get-value-from-pi($artifacts[1], 'type'), '-h')">
				<sequence select="my:get-value-from-pi($artifacts[1], 'file')" />
			</when>
			<otherwise>
				<sequence select="my:get-h-artifact(fn:subsequence($artifacts, 2))" />
			</otherwise>
		</choose>
	</function>

</stylesheet>