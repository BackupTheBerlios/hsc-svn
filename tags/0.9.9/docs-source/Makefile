#
# Makefile for hsc-documentation
#

#
# DESTDIR  - destination directory (relative)
# IGNORE   - messages to be ignored
# PRJFILE  - project file
# STDINC   - standard includes for all sources
# HSCMISC  - miscellaneous flags and options
# 
# HSC      - shell command that invokes hsc
# HSCFLAGS - hsc options
#
# HSCDEPP  - shell command that invokes hscdepp
#

DESTDIR =/docs/
IGNORE  =ign=46
PRJFILE =hsc.project
STDINC  =inc/macro.hsc inc/webpage.hsc
HSCMISC	=rplcent getsize compact mode=pedantic #quotemode=double

HSC     =/hsc
HSCFLAGS=$(HSCMISC) $(IGNORE) prjfile=$(PRJFILE) to=$(DESTDIR) $(STDINC)

HSCDEPP =/hscdepp

#
# rule to update whole docs
#
all : all_hsc /index.html

#
# implicit rule for html-files
#
$(DESTDIR)%.html : %.hsc
	$(HSC) $(HSCFLAGS) $< 

#
# update dependencies
#
depend :
	$(HSCDEPP) file=Makefile prjfile=$(PRJFILE) verbose

#
# add new file / change includes of existing file
#
# example: make NEW FILE=new.hsc "INCLUDE=inc/macro.hsc inc/sepp.hsc"
#
NEW :
ifndef FILE
	@echo *** pleaese specify variable FILE (and on your option INCLUDE)
else
	$(HSC) $(HSCFLAGS) $(INCLUDE) from=$(FILE) 
endif


#
# main page
#
/index.html : main_index.hsc
	$(HSC) to=$@ $(HSCMISC) $<

# --- DO NOT MODIFY THIS LINE -- hsc-dependencies follow ---

# dependencies updated: Thursday 01-Aug-1996 01:42:53

all_hsc : /docs/features/spctags.html /docs/features/exec.html \
    /docs/index.html /docs/updates.html /docs/features/expressions.html \
    /docs/future.html /docs/messages.html /docs/features/syntax.html \
    /docs/macro/attrib.html /docs/macro/flag.html /docs/macro/macros.html \
    /docs/bugs.html /docs/project/prjfile.html /docs/project/makefile.html \
    /docs/project/make.html /docs/project/hscdepp.html \
    /docs/project/index.html /docs/options.html /docs/about.html \
    /docs/copy.html /docs/examples.html /docs/install.html \
    /docs/questions.html /docs/related.html /docs/require.html \
    /docs/source.html /docs/features/absuris.html \
    /docs/features/checkuri.html /docs/features/getsize.html \
    /docs/features/if.html /docs/features/prefs.html \
    /docs/features/rplcent.html /docs/features/spcattr.html \
    /docs/features/strip.html /docs/distrib.html

/docs/features/spctags.html : features/spctags.hsc inc/macro.hsc \
    inc/webpage.hsc

/docs/features/exec.html : features/exec.hsc inc/macro.hsc inc/webpage.hsc \
    exmpl/exec.hsc exmpl/exec.hsc

/docs/index.html : index.hsc inc/macro.hsc inc/webpage.hsc

/docs/updates.html : updates.hsc inc/macro.hsc inc/webpage.hsc

/docs/features/expressions.html : features/expressions.hsc inc/macro.hsc \
    inc/webpage.hsc

/docs/future.html : future.hsc inc/macro.hsc inc/webpage.hsc

/docs/messages.html : messages.hsc inc/macro.hsc inc/webpage.hsc

/docs/features/syntax.html : features/syntax.hsc inc/macro.hsc \
    inc/webpage.hsc

/docs/macro/attrib.html : macro/attrib.hsc inc/macro.hsc inc/webpage.hsc

/docs/macro/flag.html : macro/flag.hsc inc/macro.hsc inc/webpage.hsc

/docs/macro/macros.html : macro/macros.hsc inc/macro.hsc inc/webpage.hsc \
    exmpl/m_addr.hsc exmpl/m_addr.hsc exmpl/m_file.hsc exmpl/m_next.hsc \
    exmpl/m_next.hsc

/docs/bugs.html : bugs.hsc inc/macro.hsc inc/webpage.hsc

/docs/project/prjfile.html : project/prjfile.hsc inc/macro.hsc \
    inc/webpage.hsc

/docs/project/makefile.html : project/makefile.hsc inc/macro.hsc \
    inc/webpage.hsc

/docs/project/make.html : project/make.hsc inc/macro.hsc inc/webpage.hsc

/docs/project/hscdepp.html : project/hscdepp.hsc inc/macro.hsc \
    inc/webpage.hsc

/docs/project/index.html : project/index.hsc inc/macro.hsc inc/webpage.hsc

/docs/options.html : options.hsc inc/macro.hsc inc/webpage.hsc

/docs/about.html : about.hsc inc/macro.hsc inc/webpage.hsc

/docs/copy.html : copy.hsc inc/macro.hsc inc/webpage.hsc

/docs/examples.html : examples.hsc inc/macro.hsc inc/webpage.hsc

/docs/install.html : install.hsc inc/macro.hsc inc/webpage.hsc

/docs/questions.html : questions.hsc inc/macro.hsc inc/webpage.hsc

/docs/related.html : related.hsc inc/macro.hsc inc/webpage.hsc

/docs/require.html : require.hsc inc/macro.hsc inc/webpage.hsc

/docs/source.html : source.hsc inc/macro.hsc inc/webpage.hsc

/docs/features/absuris.html : features/absuris.hsc inc/macro.hsc \
    inc/webpage.hsc

/docs/features/checkuri.html : features/checkuri.hsc inc/macro.hsc \
    inc/webpage.hsc

/docs/features/getsize.html : features/getsize.hsc inc/macro.hsc \
    inc/webpage.hsc

/docs/features/if.html : features/if.hsc inc/macro.hsc inc/webpage.hsc

/docs/features/prefs.html : features/prefs.hsc inc/macro.hsc \
    inc/webpage.hsc

/docs/features/rplcent.html : features/rplcent.hsc inc/macro.hsc \
    inc/webpage.hsc

/docs/features/spcattr.html : features/spcattr.hsc inc/macro.hsc \
    inc/webpage.hsc exmpl/anchor.hsc exmpl/anchor.hsc

/docs/features/strip.html : features/strip.hsc inc/macro.hsc \
    inc/webpage.hsc

/docs/distrib.html : distrib.hsc inc/macro.hsc inc/webpage.hsc

# --- DO NOT MODIFY THIS LINE -- hsc-dependencies precede ---
