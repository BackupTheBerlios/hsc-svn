<$include FILE="inc/macro.hsc">
<HTML><HEAD>
<TITLE>hsc - html sucks completely - Support</TITLE>
<LINK REV="owns" TITLE="Thomas Aglassinger" HREF="mailto:agi@giga.or.at">
<LINK REL="Home" HREF="index.html">
<LINK REL="Copyright" HREF="docs/copy.html">
</HEAD><BODY>
<*<IMG SRC="docs/image/head.gif" ALT="hsc - Another stupid HTML preprocessor">*>
<IMG SRC="docs/image/nomain.gif" ALT="----" ALIGN="middle">
<IMG SRC="docs/image/noindex.gif" ALT="-----" ALIGN="middle">
<A HREF="docs/copy.html"><IMG SRC="docs/image/copy.gif" ALT="Copyright" ALIGN="middle"></A>
<IMG SRC="docs/image/noback.gif" ALT="--" ALIGN="middle">
<IMG SRC="docs/image/noprev.gif" ALT="--------" ALIGN="middle">
<A HREF="docs/index.html"><IMG SRC="docs/image/next.gif" ALT="Next" ALIGN="middle"></A>


<H1>hsc - html Sucks Completely</H1>

<$macro ArcH NAME:string/r>
<A HREF=(name)><(name)></A>
(<(GetFileSize("../"+(name)))>)
</$macro>

<$define MinorUpdate:string="">

<P><IMG SRC="docs/image/hsc.gif" ALT="..SUCK SUCK SUCK.." ALIGN="right"></P>

<P>This is the support w3-page for <hsc>, a html-preprocessor. It
provides both introductory and detailed information about the purpose
and the usage of the program. You can also download the archives
containing the current version or pre-releases and bugfixes (if there
are any).</P>

<P>You can access this information at the following locations:</P>

<IMG SRC="docs/image/austria.gif" ALT="[Austria]">
<A HREF="http://www.giga.or.at/~agi/hsc/"><(HSC.Anchor)></A><BR>
<IMG SRC="docs/image/teutsch.gif" ALT="[Germany]">
<A HREF="http://wwwcip.rus.uni-stuttgart.de/~inf11108/support/hsc/"><(HSC.ANCHOR)></A><BR>

<H2>Current Version</H2>

The current version of <hsc> is <STRONG>version 0.916</STRONG>
and should always be available from <AMINET FILE="text/hyper/">. 

<UL>
<LI>View <AMINET FILE="text/hyper/hsc.readme" TEXT="README"> for 
    a short overview.
<$if COND=(not (MinorUpdate))>
<LI><A HREF="docs/index.html">Documentation</A> for more details
</$if>
<LI>Download <AMINET FILE="text/hyper/hsc.lha">, the binary-archive
    for AmigaOS. This one includes binary, preferences
    and documentation in html-format.
<LI>Download <AMINET FILE="text/hyper/hsc-source.lha">, 
    the source-archive including the whole ANSI-C sources and the
    Makefile needed to compile all tools.
<LI>Download <AMINET FILE="text/hyper/hsc-ps.lha">.
    This one includes the documentation in Postscript format and
    is only required if you want to print the manual.
</UL>

<H2>Minor Updates</H2>

<$if COND=(MinorUpdate)>

<STRONG>Version 0.916, pre-release#6</STRONG> is now available.
Contains some bugfixes that should not cause any problems.

<UL>
<LI><A HREF="README">README</A> - short introduction
<LI><A HREF="docs/changes.html">Changes And News</A> - what's new
<LI><A HREF="docs/index.html">Documentation</A> - more details
</UL>

The archives below are only available from this support-page:
<$else>
Currently, there are no minor updates available.
The archives below are identical to those available from Aminet:
</$if>

<UL>
<LI>Download <ArcH NAME="hsc.lha"> - documentation and binaries for AmigaOS
<LI>Download <ArcH NAME="hsc-source.lha"> - documentation and source code
<LI>Download <ArcH NAME="hsc-ps.lha"> - documentation in Postscript format
</UL>

<H2>Additional Material</H2>

<P>In <ArcH NAME="dtd2hsc.lha">, there is a collection of shabby
ARexx-scripts available which can be used to convert a DTD to
<hsc.prefs>. But better view <A
HREF="dtd2hsc.readme"><(hsc.Anchor)></A> befor expecting too much from
it.</P>

<P>If you don't know how to cope with <CODE>*.lha</CODE>-files, the sources
for a portable version of <CODE>lha</CODE> are available from 
<A HREF="http://wuarchive.wustl.edu/~aminet/misc/unix/lha_1_00.tar.Z">aminet:misc/unix/lha_1_00.tar.Z</A>
(84K).</P>
<*
<P>Users who are unfamiliar to <CODE>make</CODE> can download
<ArcH NAME="make-doc.lha">, which contains the manual
to <CODE>GNUmake</CODE> in AmigaGuide format.</P>
*>
<HR>
<$let hsc.format.time="%d-%b-%Y">
<ADDRESS><A HREF="docs/author.html">Thomas Aglassinger</A> (agi@giga.or.at),
<(GetTime())></ADDRESS>
</BODY></HTML>
