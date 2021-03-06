<**************************************
* WEBPAGE_NAVIGATION                  *
*   this is a local macro that is     *
*   allowed to be used from inside of *
*   <WEBPAGE> (see below)             *
*   it inserts a navigation bar at    *
*   current position                  *
***************************************>
<$macro WEBPAGE_NAVIGATION /MBI="WEBPAGE">
<** main button **>
<$if COND=(NOT SET NoMain)>
<A HREF=":index.html"><IMG SRC=":image/main.gif" ALT="Main" ALIGN="middle"></A>
<$else>
<IMG SRC=":image/nomain.gif" ALT="----" ALIGN="middle">
</$if>
<** insert "index" button **>
<$if COND=(NOT SET NoIndex)>
<A HREF=":index.html"><IMG SRC=":image/index.gif" ALT="Index" ALIGN="middle"></A>
<$else>
<IMG SRC=":image/noindex.gif" ALT="-----" ALIGN="middle">
</$if>
<** copyright button **>
<$if COND=(NOT SET NoCopy)>
<A HREF=":copy.html"><IMG SRC=":image/copy.gif" ALT="Copyright" ALIGN="middle"></A>
<$else>
<IMG SRC=":image/nocopy.gif" ALT="---------" ALIGN="middle">
</$if>
<** insert "back" (up) button **>
<$if COND=(SET Back)>
<A HREF=(Back)><IMG SRC=":image/back.gif" ALT="Up" ALIGN="middle"></A>
<$else>
<IMG SRC=":image/noback.gif" ALT="--" ALIGN="middle">
</$if>
<** insert "previous" button **>
<$if COND=(SET Prev)>
<A HREF=(Prev)><IMG SRC=":image/prev.gif" ALT="Previous" ALIGN="middle"></A>
<$else>
<IMG SRC=":image/noprev.gif" ALT="--------" ALIGN="middle">
</$if>
<** insert "next" button **>
<$if COND=(SET Next)>
<A HREF=(Next)><IMG SRC=":image/next.gif" ALT="Next" ALIGN="middle"></A>
<$else>
<IMG SRC=":image/nonext.gif" ALT="----" ALIGN="middle">
</$if>
</$macro>

<*****************************
* WEBPAGE                    *
******************************>
<$macro WEBPAGE /CLOSE Title:string/r
                Chapter:string=""
                Next:uri prev:uri
                Back:uri=":index.html"
                NoIndex:bool="true"
                NoCopy:bool
                NoMain:bool
                QTEXT:string QAUTHOR:string>
<HTML>
<HEAD>
  <TITLE><(Chapter)><(Title)></TITLE>
  <LINK REV="owns" TITLE="Thomas Aglassinger" HREF="mailto:agi@giga.or.at">
  <$if COND=(SET NoMain)>
  <LINK REL="Home" HREF=":hsc.html">
  </$if>
  <LINK REL="Copyright" HREF=":copy.html">
  <$if COND=(SET prev)>
  <LINK REL="Previous" HREF=(Prev)>
  </$if>
  <$if COND=(SET next)>
  <LINK REL="Next" HREF=(Next)>
  </$if>
</HEAD>
<BODY>

<*<IMG SRC=":image/head.gif" ALT="hsc"><BR>*>

<Webpage_Navigation>
<HR>
<* insert quote *>
<$if COND=(SET QTEXT)>
<P ALIGN="right"><I><(QTEXT)></I><BR>
  <$if COND=(SET QAUTHOR)>
    (<(QAUTHOR)>)
  </$if>
</P>
</$if>


<H1><(Title)></H1>
<$content>
<*
<HR>
<$let HSC.FORMAT.TIME="%d-%b-%Y">
<ADDRESS>
  Thomas Aglassinger
  (<A HREF="mailto:agi@giga.or.at">agi@giga.or.at</A>),
  <(GetTime())>
</ADDRESS>
*>
</BODY></HTML>
</$macro>

