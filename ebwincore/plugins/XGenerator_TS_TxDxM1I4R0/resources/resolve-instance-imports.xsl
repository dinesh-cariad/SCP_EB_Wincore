<?xml version="1.0" encoding="UTF-8"?>
<stylesheet version="2.0" exclude-result-prefixes="fn my x xml xs" xmlns="http://www.w3.org/1999/XSL/Transform" xmlns:x="http://www.elektrobit.com/2013/xgen" xmlns:my="http://www.elektrobit.com/2013/xgen/local" xmlns:fn="http://www.w3.org/2005/xpath-functions" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:xml="http://www.w3.org/XML/1998/namespace">

	<import href="resource://details/generic.xsl" />

	<output method="xml" indent="yes" />

	<template match="@*|node()">
		<copy>
			<apply-templates select="@*|node()" />
		</copy>
	</template>

	<template match="x:include-instance">
		<variable name="configuration-class" select="ancestor::x:configuration/@class" />
		<variable name="sub-instances" as="element()*" select="//x:module[@name = current()/@module]/x:configuration[@class = $configuration-class]//x:instance[1]"/>
		
		<x:field included-from-module="{@module}">
			<copy-of select="$sub-instances[1]/*[local-name() != 'compiler-abstraction']" />
		</x:field>
	</template>

</stylesheet>