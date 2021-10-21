<?xml version="1.0" encoding="UTF-8"?>
<stylesheet version="2.0" xmlns="http://www.w3.org/1999/XSL/Transform" xmlns:x="http://www.elektrobit.com/2013/xgen" xmlns:my="http://www.elektrobit.com/2013/xgen/local" xmlns:fn="http://www.w3.org/2005/xpath-functions" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:xml="http://www.w3.org/XML/1998/namespace" xmlns:saxon="http://saxon.sf.net/">

	<import href="resource://details/generic-text.xsl" />
	<import href="resource://details/s-record.xsl" />

	<key name="fields-by-id" match="//x:field" use="@id" />

	<variable name="max-data-bytes-per-line" as="xs:integer" select="32" />
	<variable name="OxFFFFFFFFFFFFFFFF" select="18446744073709551615" as="xs:integer" />

	<variable name="header" as="xs:string">
		<text>Copyright (C) 2013 Elektrobit Automotive GmbH</text>
	</variable>

	<variable name="byte-order-BE" as="xs:string" select="'BE'" />
	<variable name="byte-order-LE" as="xs:string" select="'LE'" />

	<variable name="byte-ordering" as="xs:string?" select="//x:module[@name = 'Base']/x:configuration[@class = 'PreCompile']/x:parameter[@name = 'byte-ordering']/x:symbolic-value" />
	<variable name="msb-first" as="xs:boolean" select="$byte-ordering = $byte-order-BE" />
	<variable name="lsb-first" as="xs:boolean" select="$byte-ordering = $byte-order-LE" />

	<variable name="relocatable-enable" as="xs:boolean" select="my:get-pre-compile-flag('relocatable-enable')" />

	<template match="/">
		<if test="empty($byte-ordering)">
			<message terminate="yes" select="'byte-ordering not defined.'" />
		</if>
		<if test="$msb-first = false() and $lsb-first = false()">
			<message terminate="yes" select="'byte-ordering value invalid. Found', $byte-ordering, 'but allowed values are', $byte-order-BE,'and', $byte-order-LE" />
		</if>

		<call-template name="output-record">
			<with-param name="type" select="'S0'" />
			<with-param name="address" select="0" />
			<with-param name="data" select="fn:string-to-codepoints($header)" />
		</call-template>
		<apply-templates select="$my:current-config/x:memory-section/x:instance" />
		<call-template name="output-record">
			<with-param name="type" select="'S7'" />
			<with-param name="address" select="0" />
			<with-param name="data" select="nil" />
		</call-template>
	</template>

	<template match="x:instance">
		<variable name="type" select="key('types-by-name', my:resolve-type-references(@type))" />

		<if test="not($type)">
			<message terminate="yes" select="concat('Type ', @type, ' not found for instance ', @name)" />
		</if>
		<if test="not($type/x:struct)">
			<message terminate="yes" select="concat('Type ', $type/@name, ' of instance ', @name, ' is not a struct; only structs are currently allowed as instance types')" />
		</if>

		<variable name="address" select="@address" as="xs:integer" />

		<if test="($address mod $type/@alignment-requirement) != 0">
			<message terminate="yes" select="fn:concat(
		      'Address ',
		      @address,
		      ' of instance ',
		      @name,
		      'does not match alignment requirement of struct ',
		      $type/@name,
		      ' (',
		      $type/@alignment-requirement,
		      ')'
		      )" />
		</if>

		<call-template name="output-fields">
			<with-param name="fields" select="x:field" />
			<with-param name="address" select="$address" />
			<with-param name="bytes" select="()" />
		</call-template>
	</template>

	<template name="output-fields">
		<param name="fields" as="element()*" />
		<param name="address" as="xs:integer" />
		<param name="bytes" as="xs:integer*" />

		<choose>
			<when test="empty($fields)">
				<call-template name="flush-bytes">
					<with-param name="address" select="$address" />
					<with-param name="bytes" select="$bytes" />
				</call-template>
			</when>
			<otherwise>
				<variable name="field" as="element()" select="$fields[1]" />
				<variable name="break" as="xs:boolean" select="fn:exists($field/x:break)" />

				<if test="$break">
					<call-template name="flush-bytes">
						<with-param name="address" select="$address" />
						<with-param name="bytes" select="$bytes" />
					</call-template>
				</if>

				<variable name="new-bytes" as="xs:integer*" select="if($break)
						  then ()
						  else my:append-field($field, $bytes)" />

				<variable name="new-address" as="xs:integer" select="if($break)
                          then $field/@address
                          else $address" />

				<call-template name="output-fields">
					<with-param name="fields" select="fn:subsequence($fields, 2)" />
					<with-param name="address" select="$new-address" />
					<with-param name="bytes" select="$new-bytes" />
				</call-template>
			</otherwise>
		</choose>
	</template>

	<template name="flush-bytes">
		<param name="address" as="xs:integer" />
		<param name="bytes" as="xs:integer*" />

		<if test="not(fn:empty($bytes))">
			<choose>
				<when test="$max-data-bytes-per-line >= fn:count($bytes)">
					<call-template name="output-record">
						<with-param name="type" select="'S3'" />
						<with-param name="address" select="$address" />
						<with-param name="data" select="$bytes" />
					</call-template>
				</when>
				<otherwise>
					<call-template name="flush-bytes">
						<with-param name="address" select="$address" />
						<with-param name="bytes" select="fn:subsequence($bytes, 1, $max-data-bytes-per-line)" />
					</call-template>
					<call-template name="flush-bytes">
						<with-param name="address" select="$address + $max-data-bytes-per-line" />
						<with-param name="bytes" select="fn:subsequence($bytes, 1 + $max-data-bytes-per-line)" />
					</call-template>
				</otherwise>
			</choose>
		</if>
	</template>

	<!-- Append the byte content of a field to a given byte sequence and return the new sequence -->
	<function name="my:append-field" as="xs:integer*">
		<param name="field" as="element()" />
		<param name="accumulated-bytes" as="xs:integer*" />

		<variable name="value" as="xs:integer">
			<choose>
				<when test="exists($field/x:ref/@target)">
					<variable name="target" as="element()" select="$root/key('fields-by-id', $field/x:ref/@target)" />
					<variable name="address" as="xs:integer" select="$target/@address" />
					<choose>
						<when test="$relocatable-enable">
							<variable name="module-offset" as="xs:integer" select="$field/@module-offset" />
							<sequence select="$address - $module-offset" />
						</when>
						<otherwise>
							<sequence select="$address" />
						</otherwise>
					</choose>
				</when>
				<when test="exists($field/x:int)">
					<sequence select="$field/x:int/text()" />
				</when>
				<when test="exists($field/x:symbol)">
					<sequence select="my:get-numeric-symbol-value($field/x:symbol/text())" />
				</when>
				<otherwise>
					<sequence select="0" />
				</otherwise>
			</choose>
		</variable>

		<variable name="bytes" as="xs:integer*" select="my:int-to-bytearray($value, $field/@byte-size)" />

		<sequence select="$accumulated-bytes, (if($msb-first) then fn:reverse($bytes) else $bytes)" />
	</function>

	<function name="my:get-numeric-symbol-value" as="xs:integer?">
		<param name="name" as="xs:string" />

		<variable name="symbol" select="$root/key('symbols-by-name', $name)" />

		<if test="fn:empty($symbol)">
			<message terminate="yes" select="concat('Undefined symbol: ', $name)" />
		</if>

		<choose>
			<when test="fn:exists($symbol/x:integer-value/text())">
				<sequence select="$symbol/x:integer-value/text()" />
			</when>
			<when test="fn:exists($symbol/x:symbolic-value)">
				<sequence select="my:get-numeric-symbol-value($symbol/x:symbolic-value/text())" />
			</when>
			<when test="fn:exists($symbol/x:parameter)">
				<message terminate="yes" select="concat('Symbol ', $name, ': parameter values currently unsupported')" />
			</when>
			<otherwise>
				<message terminate="yes" select="concat('Symbol: ', $name, ' has no integer value')" />
			</otherwise>
		</choose>

	</function>

	<function name="my:int-to-bytearray" as="xs:integer*">
		<param name="value" as="xs:integer" />
		<param name="byte-count" as="xs:integer" />

		<sequence select="my:non-negative-int-to-bytearray(
		    if($value lt 0) then ($value + $OxFFFFFFFFFFFFFFFF + 1) else $value,
		    $byte-count,
		    ())" />
	</function>

	<function name="my:non-negative-int-to-bytearray" as="xs:integer*">
		<param name="value" as="xs:integer" />
		<param name="byte-count" as="xs:integer" />
		<param name="acc" as="xs:integer*" />

		<sequence select="if($byte-count lt 1)
		    then $acc
		    else my:non-negative-int-to-bytearray(
                $value idiv 256,
                $byte-count - 1,
                ($acc, $value mod 256))" />
	</function>

</stylesheet>