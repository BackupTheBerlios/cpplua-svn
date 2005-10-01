<?xml version="1.0" encoding="ISO-8859-1"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="xml" 
  doctype-system="http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd" 
  doctype-public="-//W3C//DTD XHTML 1.0 Transitional//EN"
  indent="yes"/>

<xsl:template match="/documentation">
    <html>
    <head>
    <link rel="stylesheet" type="text/css" href="../doc.css"/>
    <link rel="stylesheet" type="text/css" href="../code.css"/>
    <title>
    <xsl:value-of select="title"/>
    </title>
    </head>
    <body>
    <xsl:apply-templates/>
    </body>
    </html>
</xsl:template>

<xsl:template name="anchor">
<a>
<xsl:attribute name="id">
<xsl:value-of select="translate(.,' ','_')"/>
</xsl:attribute>
<xsl:apply-templates/>
</a>
</xsl:template>

<xsl:template match="title">
<h1>
<img src="cpplua.gif" alt="cpplua" border="0"/>
<xsl:call-template name="anchor"/>
</h1>
</xsl:template>

<xsl:template match="chapter">
<h2><xsl:call-template name="anchor"/></h2>
</xsl:template>

<xsl:template match="section">
<h3><xsl:call-template name="anchor"/></h3>
</xsl:template>

<xsl:template match="p">
<p><xsl:apply-templates/></p>
</xsl:template>

<xsl:template match="code">
<code class="keyword">
<xsl:apply-templates/>
</code>
</xsl:template>

<xsl:template match="code[@keyword=0]">
<code>
<xsl:apply-templates/>
</code>
</xsl:template>

<xsl:template match="code[@block=1]">
<pre class="code">
<xsl:apply-templates/>
</pre>
</xsl:template>

<xsl:template match="code[@block=1]/hl">
<span>
<xsl:attribute name="class">
<xsl:value-of select="@class"/>
</xsl:attribute>
<xsl:apply-templates/>
</span>
</xsl:template>

<xsl:template match="code[@block=1]/hl-custom">
<span>
<xsl:attribute name="style">
<xsl:value-of select="@style"/>
</xsl:attribute>
<xsl:apply-templates/>
</span>
</xsl:template>

<xsl:template match="link">
<a>
<xsl:attribute name="href">
<xsl:value-of select="@href"/>
</xsl:attribute>
<xsl:apply-templates/>
</a>
</xsl:template>

<xsl:template match="link[@missing=1]">
<span class="missing-link">
<xsl:apply-templates/>
</span>
</xsl:template>

<xsl:template match="em">
<em><xsl:apply-templates/></em>
</xsl:template>

<xsl:template match="toc">
<div class="toc">
<xsl:apply-templates/>
</div>
</xsl:template>

<xsl:template match="toc//h">
<ul>
<xsl:apply-templates/>
</ul>
</xsl:template>

<xsl:template match="toc/h/label" priority="2">
<dt><h2><a href=""><xsl:apply-templates/></a></h2></dt>
</xsl:template>

<xsl:template match="toc/h/h/label" priority="2">
<dt><h3><xsl:apply-templates/></h3></dt>
</xsl:template>

<xsl:template match="toc//h/label">
<li/><xsl:apply-templates/>
</xsl:template>

</xsl:stylesheet>