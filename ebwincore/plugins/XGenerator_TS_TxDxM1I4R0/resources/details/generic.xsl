<?xml version="1.0" encoding="UTF-8"?>
<stylesheet version="2.0" xmlns="http://www.w3.org/1999/XSL/Transform" xmlns:x="http://www.elektrobit.com/2013/xgen" xmlns:my="http://www.elektrobit.com/2013/xgen/local" xmlns:xml="http://www.w3.org/XML/1998/namespace" xmlns:fn="http://www.w3.org/2005/xpath-functions" xmlns:xs="http://www.w3.org/2001/XMLSchema">

	<param name="my:NL" />
	<param name="my:indent" />
	<param name="my:current-artifact" />

	<variable name="my:module">
		<value-of select="/processing-instruction('module')[1]" />
	</variable>

        <variable name="my:version">
                <choose>
                        <when test="/x:xgen/x:module[@name=$my:module]/@version">
                                <!-- remove build number suffix from version string -->
                                <value-of select="fn:replace(/x:xgen/x:module[@name=$my:module]/@version,'([0-9]+\.[0-9]+\.[0-9]+).*','$1')" />
                        </when>
                        <otherwise>0.0.0</otherwise>
                </choose>
        </variable>

	<variable name="my:current-artifact-type" select="my:get-value-from-pi($my:current-artifact, 'type')" />
	<variable name="my:current-file-name" select="my:get-value-from-pi($my:current-artifact, 'file')" />
	<variable name="my:current-config" select="/x:xgen/x:module/x:configuration[processing-instruction('current')]" />
	<variable name="my:include-guard-name" select="fn:upper-case(fn:replace($my:current-file-name, '[^a-zA-Z0-9_]', '_'))" />

	<template match="text()" mode="#all" />

	<variable name="root" select="/" />

	<key name="types-by-name" use="@name" match="//x:type" />
	<key name="symbols-by-name" use="@name" match="//x:symbol-def" />

	<function name="my:calculate-aligned-value" as="xs:integer">
		<param name="original-value" as="xs:integer" />
		<param name="alignment-requirement" as="xs:integer" />

		<choose>
			<when test="$alignment-requirement &lt;= 1">
				<value-of select="$original-value" />
			</when>
			<otherwise>
				<variable name="remainder" select="$original-value mod $alignment-requirement" />
				<value-of select="if($remainder > 0)
                    then $original-value + $alignment-requirement - $remainder
                    else $original-value" />
			</otherwise>
		</choose>
	</function>

	<function name="my:get-member-typename" as="xs:string">
		<param name="member" as="element()" />

		<sequence select="$member/@type" />
	</function>

	<function name="my:get-member-type" as="element()">
		<param name="member" as="element()" />

		<sequence select="$root/key('types-by-name', my:get-member-typename($member))" />
	</function>

	<!-- To be used to topologically sort types -->
	<function name="my:computeTypeWeight" as="xs:integer*">
		<param name="type" as="element()?" />

		<if test="empty($type)">
			<sequence select="0" />
		</if>

		<!-- generate a sequence containing the weights of each node I reference -->
		<variable name="referencedTypeWeights" as="xs:integer*">
			<choose>
				<when test="$type/x:reference">
					<sequence select="my:computeTypeWeight($root/key('types-by-name', $type/x:reference/@type))" />
				</when>
				<when test="$type/x:struct">
					<for-each select="$type/x:struct/x:member">
						<sequence select="my:computeTypeWeight($root/key('types-by-name', current()/@type))" />
					</for-each>
				</when>
				<otherwise>
					<sequence select="0" />
				</otherwise>
			</choose>
		</variable>
		<!-- make my weight higher than any of the nodes I reference -->
		<value-of select="max($referencedTypeWeights)+1" />
	</function>

	<!-- Resolve type references (typedefs), but stop at array types and at AUTOSAR standard types -->
	<function name="my:resolve-type-references" as="xs:string">
		<param name="typename" as="xs:string" />

		<variable name="type" select="$root/key('types-by-name', $typename)" />
		<choose>
			<when test="$type/x:reference[not(@count)]">
				<sequence select="my:resolve-type-references($type/x:reference/@type)" />
			</when>
			<otherwise>
				<sequence select="$typename" />
			</otherwise>
		</choose>
	</function>

	<function name="my:get-artifact-of-type" as="node()?">
		<param name="type" as="xs:string" />
		<param name="artifacts" as="node()*" />

		<choose>
			<when test="fn:empty($artifacts)">
				<sequence select="()" />
			</when>
			<otherwise>
				<sequence select="if(my:get-value-from-pi($artifacts[1], 'type') = $type)
				    then $artifacts[1]
				    else my:get-artifact-of-type($type, fn:subsequence($artifacts, 2))" />
			</otherwise>
		</choose>
	</function>

	<function name="my:get-value-from-pi" as="xs:string">
		<param name="pi" as="xs:string" />
		<param name="key" as="xs:string" />

		<sequence select="fn:replace($pi, my:attribute-regex($key), '$1')" />
	</function>

	<!-- Create a regular expression to get an attribute value from the text of a processing instruction -->
	<!-- The attribute must have the form of a normal XML attribute, i. e.: 'attr="value"' -->
	<function name="my:attribute-regex" as="xs:string">
		<param name="attribute-name" as="xs:string" />

		<sequence select="fn:concat('^.*?', $attribute-name, ' *= *&quot;(.+?)&quot;.*?$')" />
	</function>

	<function name="my:normalize-end-of-line" as="xs:string">
		<param name="text" as="xs:string" />

		<sequence select="fn:replace($text, '\r\n|\n\r|\r|\n', $my:NL)" />
	</function>

	<function name="my:on-off" as="xs:string">
		<param name="flag" as="xs:boolean" />

		<sequence select="if($flag) then 'STD_ON' else 'STD_OFF'" />

	</function>

	<function name="my:get-pre-compile-flag" as="xs:boolean">
		<param name="parameter-name" as="xs:string" />

		<sequence select="$root/x:xgen/x:module[@name=$my:module]/x:configuration[@class='PreCompile']/x:parameter[@name=$parameter-name]/x:boolean-value" />

	</function>

	<function name="my:get-symbol-parameter" as="xs:string?">
		<param name="container-node" as="element()" />
		<param name="parameter-name" as="xs:string" />

		<sequence select="$container-node/x:parameter[@name=$parameter-name]/x:symbolic-value" />

	</function>

    <function name="my:get-included-file-names" as="xs:string*">
        <param name="references" as="element()*" />
        <param name="self" as="node()" />

        <sequence select="my:get-included-file-names-rec($references, $self, ())"/>
    </function>

	<function name="my:get-included-file-names-rec" as="xs:string*">
		<param name="references" as="element()*" />
		<param name="self" as="node()" />
		<param name="acc" as="xs:string*" />

		<choose>
			<when test="fn:empty($references)">
				<variable name="result" select="fn:distinct-values($acc)" />
				<variable name="self-index" as="xs:integer?" select="fn:index-of($result, my:get-value-from-pi($self, 'file'))" />
				<sequence select="if(fn:exists($self-index))
						  then fn:remove($result, $self-index)
						  else $result" />
			</when>
			<when test="$references[1]/@defining-artifact">
				<sequence select="my:get-included-file-names-rec(fn:subsequence($references, 2), $self, ($acc, $references[1]/@defining-artifact))" />
			</when>
			<otherwise>
				<sequence select="my:get-included-file-names-rec(fn:subsequence($references, 2), $self, $acc)" />
			</otherwise>
		</choose>
	</function>

	<!-- Get the value of a parameter element. If the value is a property reference, follow the reference and return the value of the target. -->
	<function name="my:get-property-value" as="element()?">
		<param name="property" as="element()" />

		<choose>
			<when test="$property/x:parameter">
				<variable name="dest" select="my:find-property($property/.., $property/x:parameter/@reference)" as="element()?" />
				<if test="exists($dest)">
					<sequence select="my:get-property-value($dest)" />
				</if>
			</when>
			<otherwise>
				<sequence select="$property/*[1]" />
			</otherwise>
		</choose>
	</function>

	<!-- Find the property element of a given name, searching upwards from a given start element. -->
	<function name="my:find-property" as="element()?">
		<param name="start" as="element()" />
		<param name="name" as="xs:string" />

        <sequence select="$start/ancestor-or-self::*/x:parameter[@name=$name][1]"/>
	</function>

</stylesheet>
