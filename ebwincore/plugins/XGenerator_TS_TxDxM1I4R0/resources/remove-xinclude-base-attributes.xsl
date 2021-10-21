<?xml version="1.0" encoding="UTF-8"?>
<stylesheet version="2.0" xmlns="http://www.w3.org/1999/XSL/Transform" xmlns:xml="http://www.w3.org/XML/1998/namespace">

	<output method="xml" indent="yes" />

	<template match="@*|node()">
		<copy>
			<apply-templates select="@*|node()" />
		</copy>
	</template>

	<!-- Removing the xml:base attribute to be able to compare the result with reference files when testing -->
	<template match="@xml:base" />

</stylesheet>