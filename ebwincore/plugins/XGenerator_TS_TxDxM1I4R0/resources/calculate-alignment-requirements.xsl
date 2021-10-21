<?xml version="1.0" encoding="UTF-8"?>
<stylesheet version="2.0" exclude-result-prefixes="fn my x xml xs" xmlns="http://www.w3.org/1999/XSL/Transform" xmlns:x="http://www.elektrobit.com/2013/xgen" xmlns:my="http://www.elektrobit.com/2013/xgen/local" xmlns:fn="http://www.w3.org/2005/xpath-functions" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:xml="http://www.w3.org/XML/1998/namespace">

	<import href="resource://details/generic.xsl" />

	<!-- 1. Create a list of all types with size and alignment attributes -->
	<!-- 2. Add byte-size and alignment-requirement attributes to all types -->
	<output method="xml" indent="yes" />

	<variable name="array-properties" select="//x:array-properties" />
	<variable name="struct-properties" select="//x:struct-properties" />
	<variable name="type-info" select="my:collect-typeinfo(//x:type, my:typeinfo-start())" />

	<template match="@*|node()">
		<copy>
			<apply-templates select="@*|node()" />
		</copy>
	</template>

	<template match="x:type">
		<variable name="typeinfo" select="$type-info/my:typeinfo[@name = current()/@name]" />
		<copy>
			<attribute name="byte-size" select="$typeinfo/@size" />
			<attribute name="alignment-requirement" select="$typeinfo/@alignment" />
			<apply-templates select="@*|node()" />
		</copy>
	</template>

	<function name="my:typeinfo-start" as="node()*">
		<!-- If the relocatable-enable flag is not set, pre-populate the list of types -->
		<!-- with the 'TS_CfgOffsetType' with the parameters from the pointer basetype. -->
		<!-- Otherwise, just return an empty list. -->
		<choose>
			<when test="my:get-pre-compile-flag('relocatable-enable')">
			    <!-- If relocatable-enable is set, TS_CfgOffsetType will
			    be found in the standard process -->
				<sequence select="()" />
			</when>
			<otherwise>
			    <!-- TS_CfgOffsetType shall have the properties of pointers. -->
				<variable name="ptr" as="element()"
				    select="$root/x:xgen/x:module/x:configuration[@class='PreCompile']/x:base-type[@name='farpointer']" />
				<variable name="size" as="xs:integer" select="$ptr/@size"/>
                <variable name="alignment" as="xs:integer" select="$ptr/@alignment"/>

				<sequence select="my:insert-typeinfo('TS_CfgOffsetType', $size, $alignment, ())" />
			</otherwise>
		</choose>
	</function>

	<function name="my:collect-typeinfo" as="node()*">
		<!-- The type elements found in the input document -->
		<param name="types" as="element()*" />
		<!-- An accumulator for the generated typeinfo objects -->
		<param name="acc" as="node()*" />

		<sequence select="if(fn:empty($types))
		  then $acc
		  else my:collect-typeinfo(
                    fn:subsequence($types, 2),
                    my:collect-typeinfo-for($types[1], $acc)
                    )" />
	</function>

	<function name="my:collect-typeinfo-for" as="node()*">
		<param name="type" as="element()" />
		<param name="acc" as="node()*" />

		<variable name="name" select="$type/@name" as="xs:string" />

		<choose>
			<when test="fn:exists($acc/my:typeinfo[@name = $name])">        
				<!-- This type was already handled, just return the accumulator -->
				<sequence select="$acc" />
			</when>

			<when test="fn:exists($type/x:basetype-ref)">
				<variable name="refname" select="$type/x:basetype-ref/@type" />

				<variable name="referenced" select="$root//x:base-type[@name = $refname]" />
				<variable name="referenced-size" select="$referenced/@size" />
				<variable name="referenced-alignment" select="$referenced/@alignment" />

				<sequence select="my:insert-typeinfo(
                    $type/@name,
                    $referenced/@size,
                    $referenced/@alignment,
                    $acc)" />
			</when>

			<when test="fn:exists($type/x:reference)">

				<!-- This is a type definition -->
				<variable name="reference" select="$type/x:reference" />
				<variable name="refname" select="$reference/@type" />
				<variable name="new" select="my:collect-typeinfo-for(key('types-by-name', $refname, $root), $acc)" />
				<variable name="referenced" select="$new/my:typeinfo[@name = $refname]" />
				<variable name="referenced-size" select="$referenced/@size" />
				<variable name="referenced-alignment" select="$referenced/@alignment" />

				<choose>
					<when test="fn:exists($reference/@count)">
						<!-- This is an array type definition -->
						<variable name="count" select="$reference/@count" as="xs:integer" />
						<variable name="size" select="$count * my:calculate-aligned-value($referenced-size, $referenced-alignment)" as="xs:integer" />
						<variable name="structured-alignment" select="my:structured-alignment($array-properties, $size, $referenced-alignment)" />

						<sequence select="my:insert-typeinfo(
						  $type/@name,
						  my:calculate-aligned-value($size, $structured-alignment),
						  $structured-alignment,
						  $new)" />
					</when>
					<otherwise>
						<!-- This is a scalar type definition -->
						<sequence select="my:insert-typeinfo($name, $referenced-size, $referenced-alignment, $new)" />
					</otherwise>
				</choose>
			</when>

			<when test="fn:exists($type/x:struct)">
				<!-- This is a struct definition -->
				<sequence select="my:collect-typeinfo-for-struct($name, $type/x:struct/x:member, 0, 1, $acc)" />
			</when>

			<otherwise>
				<sequence select="my:insert-typeinfo(
				    $type/@name,
                    $type/@size,
                    $type/@alignment,
                    $acc)" />
			</otherwise>
		</choose>
	</function>

	<function name="my:collect-typeinfo-for-struct" as="node()*">
		<param name="typename" as="xs:string" />
		<param name="members" as="element()*" />
		<param name="size" as="xs:integer" />
		<param name="alignment" as="xs:integer" />
		<param name="acc" as="node()*" />

		<choose>
			<when test="fn:empty($members)">
				<variable name="structured-alignment" select="my:structured-alignment($struct-properties, $size, $alignment)" />
				<sequence select="my:insert-typeinfo(
				    $typename,
				    my:calculate-aligned-value($size, $structured-alignment),
				    $structured-alignment,
				    $acc)" />
			</when>

			<otherwise>
				<variable name="current-member" select="$members[1]" />
				<variable name="rest-members" select="fn:subsequence($members, 2)" />

				<variable name="member-typename" select="my:get-member-typename($current-member)" as="xs:string" />

				<variable name="new" select="my:collect-typeinfo-for(key('types-by-name', $member-typename, $root), $acc)" as="node()*" />
				<variable name="referenced" select="$new/my:typeinfo[@name = $member-typename]" as="element()" />
				<variable name="referenced-size" select="$referenced/@size" as="xs:integer" />
				<variable name="referenced-alignment" select="$referenced/@alignment" as="xs:integer" />

				<variable name="aligned-offset" select="my:calculate-aligned-value($size, $referenced-alignment)" />

				<sequence select="my:collect-typeinfo-for-struct(
				    $typename,
				    $rest-members,
				    $aligned-offset + $referenced-size,
				    fn:max(($alignment, xs:integer($referenced-alignment))),
				    $new)" />
			</otherwise>
		</choose>
	</function>

	<function name="my:insert-typeinfo">
		<param name="name" as="xs:string" />
		<param name="size" as="xs:integer" />
		<param name="alignment" as="xs:integer" />
		<param name="list" as="node()*" />

		<variable name="instance">
			<my:typeinfo name="{$name}" size="{$size}" alignment="{$alignment}" />
		</variable>

		<sequence select="$list, $instance" />
	</function>

	<function name="my:structured-alignment" as="xs:integer">
		<param name="property-node" as="node()" />
		<param name="size" as="xs:integer" />
		<param name="member-alignment" as="xs:integer" />

		<variable name="alignment-predicate-size" select="xs:string(fn:min($property-node/x:minimum-alignment[@up-to-size >= $size]/@up-to-size))" />

		<sequence
			select="if($alignment-predicate-size)
            then fn:max(
	            ($member-alignment, xs:integer($property-node/x:minimum-alignment[@up-to-size = $alignment-predicate-size]/@value))
	            )
            else if($property-node/x:minimum-alignment[not(@up-to-size)])
            then fn:max(
                ($member-alignment, xs:integer($property-node/x:minimum-alignment[not(@up-to-size)]/@value))
                )
            else $member-alignment" />
	</function>

	<function name="my:aligned-size" as="xs:integer">
		<param name="item-type" as="element()" />

		<variable name="size" as="xs:integer" select="$item-type/@size" />
		<variable name="alignment" as="xs:integer" select="$item-type/@alignment" />

		<sequence select="my:calculate-aligned-value($item-type/@size, $item-type/@alignment)" />

	</function>

</stylesheet>
