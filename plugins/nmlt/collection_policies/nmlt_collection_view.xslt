<xsl:stylesheet xmlns:s="http://www.w3.org/2001/sw/DataAccess/rf1/result" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

  <xsl:template match="/">
    <xsl:variable name="BASEURL">
      <xsl:value-of select="$baseUrl"></xsl:value-of>
    </xsl:variable>
    <ul>
      <xsl:for-each select="/s:sparql/s:results/s:result">
        <xsl:variable name="pid" select="substring-after(s:object/@uri, &apos;/&apos;)"></xsl:variable>
        <li>
          <a>
            <xsl:attribute name="href">
              <xsl:value-of select="$BASEURL"></xsl:value-of>
              <xsl:text>/fedora/repository/</xsl:text>
              <xsl:value-of select="$pid"></xsl:value-of>/-/<xsl:value-of select="s:title"></xsl:value-of>
            </xsl:attribute>
            <xsl:value-of select="s:title"></xsl:value-of>
          </a>
        </li>
      </xsl:for-each>
    </ul>
  </xsl:template>
</xsl:stylesheet>