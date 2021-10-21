<?xml version="1.0" encoding="UTF-8"?>
<stylesheet version="2.0" exclude-result-prefixes="fn my x xml xs" xmlns="http://www.w3.org/1999/XSL/Transform" xmlns:x="http://www.elektrobit.com/2013/xgen" xmlns:my="http://www.elektrobit.com/2013/xgen/local" xmlns:fn="http://www.w3.org/2005/xpath-functions" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:xml="http://www.w3.org/XML/1998/namespace">

	<!-- Implicit types are types for which no explicit type definition exists, for example: -->
	<!-- struct A { uint8 x[2]; }; -->
	<!-- The type of A.x is uint8[2], but there is no typedef for this array type -->
	<!-- This xsl transformation creates explicit type elements for these -->

    <import href="resource://details/generic.xsl" />

	<output method="xml" indent="yes" />

	<template match="@*|node()">
		<copy>
			<apply-templates select="@*|node()" />
		</copy>
	</template>

	<template match="x:base-type">
		<copy>
			<apply-templates select="@*|node()" />
			<x:type name="{@name}" size="{@size}" alignment="{@alignment}" />
		</copy>
	</template>

	<template match="x:member">
		<copy>
			<apply-templates select="@*[local-name()!='count' and local-name()!='type']" />
			<attribute name="type" select="my:get-real-member-typename(.)" />
			<apply-templates select="element()" />
		</copy>
	</template>

	<template match="x:instance">
		<copy>
			<apply-templates select="@*[local-name()!='type']" />
			<attribute name="type" select="@type" />
			<apply-templates select="element()" />
		</copy>
	</template>

	<template match="x:member[@count]">
		<variable name="new-typename" select="concat('typeof(', ../../@name, '::', @name, ')')" />
		<x:type name="{$new-typename}">
			<x:reference type="{my:get-real-member-typename(.)}" count="{@count}" />
		</x:type>
		<copy>
			<apply-templates select="@*[local-name()!='count' and local-name()!='type']" />
			<attribute name="type" select="$new-typename" />
			<apply-templates select="element()" />
		</copy>
	</template>
	
	<function name="my:get-real-member-typename" as="xs:string">
        <param name="member" as="element()" />
        <sequence select="
            if($member/x:compiler-abstraction/x:ref2var)
               then 'TS_VarOffsetType'
            else if($member/x:compiler-abstraction/x:ref2cfg)
               then 'TS_CfgOffsetType'
            else
               $member/@type
            " />
    </function>

</stylesheet>