<?xml version="1.0" encoding="UTF-8"?>
<!-- Documentation with http://www.linkwerk.com/pub/xslt/lib/xsltdoc/ -->
<stylesheet version="2.0" xmlns="http://www.w3.org/1999/XSL/Transform" xmlns:my="http://www.elektrobit.com/2013/xgen/local" xmlns:fn="http://www.w3.org/2005/xpath-functions" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:xml="http://www.w3.org/XML/1998/namespace">

	<template name="output-record">
		<param name="type" as="xs:string" />
		<param name="address" as="xs:integer" />
		<param name="data" as="xs:integer*" />

		<variable name="address-size" as="xs:integer">
			<choose>
				<when test="$type = 'S0'">
					<text>2</text>
				</when>
				<when test="$type = 'S1'">
					<text>2</text>
				</when>
				<when test="$type = 'S2'">
					<text>3</text>
				</when>
				<when test="$type = 'S3'">
					<text>4</text>
				</when>
				<when test="$type = 'S5'">
					<text>4</text>
				</when>
				<when test="$type = 'S7'">
					<text>4</text>
				</when>
				<when test="$type = 'S8'">
					<text>3</text>
				</when>
				<when test="$type = 'S9'">
					<text>2</text>
				</when>
			</choose>
		</variable>
		<variable name="checksum-size" as="xs:integer">
			<text>1</text>
		</variable>
		<variable name="record-length" select="1 + $address-size + fn:count($data)" />

		<value-of select="$type" /> <!-- type: S0-S9 -->
		<value-of select="my:int-to-hex($record-length, 1)" />
		<value-of select="my:int-to-hex($address, $address-size)" />

		<call-template name="output-data">
			<with-param name="data" select="$data" />
		</call-template>

		<value-of select="my:int-to-hex(my:checksum($record-length, $address, $data), 1)" />

		<value-of select="$my:NL" />
	</template>

	<template name="output-data">
		<param name="data" as="xs:integer*" />

		<for-each select="$data">
			<value-of select="my:int-to-hex(., 1)" />
		</for-each>
	</template>

	<function name="my:int-to-hex" as="xs:string">
		<param name="arg" as="xs:integer" />
		<param name="byte-size" as="xs:integer" />

		<choose>
			<when test="$byte-size gt 0">
				<sequence select="concat(my:int-to-hex($arg idiv 256, $byte-size - 1), my:byte-to-hex($arg mod 256))" />
			</when>
			<otherwise>
				<text />
			</otherwise>
		</choose>
	</function>

	<function name="my:byte-to-hex" as="xs:string">
		<param name="arg" as="xs:integer" />

		<sequence select="concat(my:nibble-to-hex($arg idiv 16), my:nibble-to-hex($arg mod 16))" />

	</function>

	<function name="my:nibble-to-hex" as="xs:string">
		<param name="arg" as="xs:integer" />

		<sequence select="substring('0123456789ABCDEF', ($arg mod 16) + 1, 1)" />

	</function>

	<function name="my:checksum" as="xs:integer">
		<param name="record-length" as="xs:integer" />
		<param name="address" as="xs:integer" />
		<param name="data" as="xs:integer*" />

		<variable name="record-length-sum" select="my:sum-of-byte-values($record-length)" />
		<variable name="address-sum" select="my:sum-of-byte-values($address)" />
		<variable name="data-sum" select="sum($data)" />
		<variable name="result" select="255 - (($record-length-sum + $address-sum + $data-sum) mod 256)"/>

		<value-of select="$result" />
	</function>

	<function name="my:sum-of-byte-values" as="xs:integer">
		<param name="arg" as="xs:integer" />

		<sequence select="if ($arg eq 0)
            then 0
            else ($arg mod 256 + my:sum-of-byte-values($arg idiv 256))" />
	</function>
</stylesheet>