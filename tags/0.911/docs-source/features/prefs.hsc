<WEBPAGE chapter="hsc - Features" title="Syntax definition"
         PREV="rplcent.html" NEXT="rplcent.html">

<P>Everything <hsc> knows about html, it retrieves from a
file named <hsc.prefs> at startup. This file contains
information about all tags, entities and icon entites.
Additionally, some <A HREF="spcattr.html">special
attributes</A> are set up there also.</P>

<P>The main advantage of this concept is that it's rather
easy to add new syntax elements. For this task, the hsc tags
<ln_deftag>, <ln_defent> and <ln_deficon> can be used.</P>

<A NAME="default"><H2>Default preferences</H2></A>

<P>The <hsc.prefs> coming with this distribution should
support most elements needed for everyday use. This includes
full html-2.0-support, tables, figures, client-side
image-maps and several elements only used by special
browsers.</P>

<P>But several weeks ago, something terrible has happended:
html-0.32 has been released. From my point of view, this
nothing more than the proof that those browser-developers,
who clutter their software with loads of jerk-tags are too
tumb to write a DTD (Document Type Definition) themselves
and that the only reason that w3c still exists is that
someone has to write those DTDs to pretend that there is a
concept behind html.</P>

<P>Obvously, nearly everyone spitts on html-0.32, and you can not take
this DTD serious. Even worse, there is no one else you can take serious
for html. Therefor, the
current <hsc.prefs> is in a bit chaotic state and a mixture
of html-2.0, html-3.0-draft and html-0.32. I am really to lazy to
keep track with all this shit..</P>

<P>Anyway, this shouldn't cause real problems for you, expept maybe some
wrong jerk-related warnings.</P>

<A NAME="search"><H2>Searching for the preferences</H2></A>

<hsc> looks at several places when trying to open <hsc.prefs>:
<UL>
    <LI>the current directory
    <LI>the directory specified in the environment variable <env_hscpath> 
    <LI>the directory <CODE>PROGDIR:</CODE>, which is automatically
        assigned to the same directory where the <hsc> binary resides
        when <hsc> is invoked
</UL>

<P>If it is unable to find <hsc.prefs> anywhere, it will abort with
an error message.</P>

<P>If you want to find out where <hsc> has read <hsc.prefs> from,
you can use <KBD><A HREF=":options.html#status">STATUS=VERBOSE</A></KBD>
when invoking <hsc>. This will display the preferences used.</P>

<A NAME="tags"><H2>Special tags to modify syntax-definition</H2></A>

<$macro SPCTAG NAME:string/r TITLE:string/r>
<A NAME=(name)><H3><(title)></H3></A>
</$macro>

All the tags below should be used within <hsc.prefs> only.
<SPCTAG NAME="defent" TITLE="Define an entity">

<P>This tag defines
a new entity. The (required) attribute <CODE>NAME</CODE> declares the
name of the entity, <CODE>RPLC</CODE> the character that should be
replaced by this entity if found in the hsc-source and <CODE>NUM</CODE>
is the numeric representation of this entity.</P>

<STRONG>Example:</STRONG> <TG>$defent NAME="uuml" RPLC="�" NUM="252"</TG>

<SPCTAG NAME="deficon" TITLE="Define icon-entity">

<P>This tag defines
a new icon-entity. The only (required) attribute is <CODE>NAME</CODE>
which declares the name of the icon.</P>

<STRONG>Example:</STRONG> <TG>$deficon NAME="mail"</TG>

<SPCTAG NAME="deftag" TITLE="Define a tag">

<P>This tag defines
a new tag, and is used quite similar to <ln_macro>, exept that a
tag-definition requires no macro-text and end-tag to be followed.</P>

<STRONG>Example:</STRONG> <TG>$deftag IMG SRC:uri/x/z/r ALT:string ALIGN:enum("top|bottom|middle") ISMAP:bool WIDTH:string HEIGHT:string</TG>

<P>To fully understand the above line, you might also want to read the sections
about <A HREF=":macro/attrib.html">attributes</A> 
and <A HREF=":macro/flag.html">options</A> for tags and macros.</P>

<P>For those, who are not smart enough or simply to lazy, here are some
simple examples, which should also work somehow, though some features of
<hsc> might not work:</P>

<$source PRE>
<$deftag BODY /CLOSE BGCOLOR:string>
<$deftag IMG SRC:uri ALT:string ALIGN:string ISMAP:bool>
</$source>

<H2>Problems</H2>

<P>There are also some disadvantages of this concept: reading
<hsc.prefs> every time on startup uses an awful lot of time.
Usually, processing your main data takes shorter than
reading the preferences. You can reduce this time, if you
create your own <hsc.prefs> with all tags and entites you
don't need removed. But I recommend to avoid this because
you might have to edit your preferences again with the next
update of <hsc>, if any new features have been added.</P>

<P>You also can't use the DTDs. This is
because DTDs don't contain several information needed by <hsc>,
for example entities are not defined within he DTD. Also several
features of <hsc> like evaluation of image-sizes or stripping of
external URIs can only be controlled with <hsc.prefs>.
</P>

</WEBPAGE>
