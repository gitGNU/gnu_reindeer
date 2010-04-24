<?xml version='1.0'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
	<xsl:import href="http://docbook.sourceforge.net/release/xsl/current/html/chunk.xsl"/>
	<xsl:include href="common.xsl"/>

	<xsl:param name="chunker.output.encoding" select="'UTF-8'"/>
	<xsl:param name="chunker.output.indent" select="'yes'"/>
	<xsl:param name="use.id.as.filename" select="1"/>
	<xsl:param name="chunk.first.sections" select="1"/>
</xsl:stylesheet>
