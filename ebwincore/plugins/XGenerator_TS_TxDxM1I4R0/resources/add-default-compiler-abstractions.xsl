<?xml version="1.0" encoding="UTF-8"?>
<stylesheet version="2.0" exclude-result-prefixes="fn my x xml xs" xmlns="http://www.w3.org/1999/XSL/Transform" xmlns:x="http://www.elektrobit.com/2013/xgen" xmlns:my="http://www.elektrobit.com/2013/xgen/local" xmlns:fn="http://www.w3.org/2005/xpath-functions" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:xml="http://www.w3.org/XML/1998/namespace">

	<import href="resource://details/generic.xsl" />

	<output method="xml" indent="yes" />

	<template match="@*|node()">
		<copy>
			<apply-templates select="@*|node()" />
		</copy>
	</template>

	<template match="x:type/x:reference[not(x:compiler-abstraction)]">
		<copy>
			<apply-templates select="@*|node()" />
			<x:compiler-abstraction>
				<x:var memory-class="TYPEDEF" />
			</x:compiler-abstraction>
		</copy>
	</template>

    <template match="x:struct/x:member[not(x:compiler-abstraction)]">
        <copy>
            <apply-templates select="@*|node()" />
            <x:compiler-abstraction>
                <x:var memory-class="TYPEDEF" />
            </x:compiler-abstraction>
        </copy>
    </template>

</stylesheet>