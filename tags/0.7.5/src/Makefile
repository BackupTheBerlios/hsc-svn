##
## makefile for "html sucks completely"
##

all : hsc

hsc : hsc.o args.o cleanup.o config.o error.o find.o global.o macro.o output.o parse.o status.o tagargs.o  tag_hsc.o tag_a.o tag_macr.o tag_misc.o tag_img.o tag_list.o vars.o ugly/ugly_dbg.o
	gcc -o hsc $(opts) hsc.o args.o cleanup.o config.o error.o find.o global.o macro.o output.o parse.o status.o tagargs.o tag_a.o tag_macr.o tag_misc.o tag_hsc.o tag_img.o tag_list.o vars.o ugly/ugly_dbg.o
	if not warn
	copy hsc prog:hsc quiet clone
	copy hsc c:hsc quiet clone
	endif

hsc_op : hsc
	gcc -c $(opts) hsc.c
	gcc -c $(opts) args.c
	gcc -c $(opts) cleanup.c
	gcc -c $(opts) config.c
	gcc -c $(opts) error.c
	gcc -c $(opts) global.c
	gcc -c $(opts) macro.c
	gcc -c $(opts) output.c
	gcc -c $(opts) parse.c
	gcc -c $(opts) status.c
	gcc -c $(opts) tagargs.c
	gcc -c $(opts) tag_a.c
	gcc -c $(opts) tag_macr.c
	gcc -c $(opts) tag_misc.c
	gcc -c $(opts) tag_hsc.c
	gcc -c $(opts) tag_img.c
	gcc -c $(opts) tag_list.c
	gcc -o hsc $(opts) hsc.o args.o cleanup.o config.o error.o global.o macro.o output.o parse.o status.o tagargs.o tag_a.o tag_misc.o tag_hsc.o tag_img.o tag_list.o tag_macr.o vars.o ugly/ugly.o
	if not warn
	copy hsc c:hsc quiet clone
	endif


hsc.o : hsc.c args.h config.h cleanup.h error.h global.h output.h parse.h status.h
	gcc -c $(opts) hsc.c -DUMEM_TRACKING

args.o : args.c global.h error.h
	gcc -c $(opts) args.c -DUMEM_TRACKING

find.o : find.c global.h
	gcc -c $(opts) find.c -DUMEM_TRACKING

global.o : global.c
	gcc -c $(opts) global.c -DUMEM_TRACKING

macro.o : macro.c global.h error.h
	gcc -c $(opts) macro.c -DUMEM_TRACKING

output.o : output.c global.h error.h
	gcc -c $(opts) output.c -DUMEM_TRACKING

parse.o : parse.c global.h error.h find.h output.h status.h
	gcc -c $(opts) parse.c -DUMEM_TRACKING

status.o : status.c global.h
	gcc -c $(opts) status.c -DUMEM_TRACKING

tagargs.o : tagargs.c global.h error.h
	gcc -c $(opts) tagargs.c -DUMEM_TRACKING

error.o : error.c global.h
	gcc -c $(opts) error.c -DUMEM_TRACKING

config.o : config.c error.h global.h macro.h msgid.h tag_a.h tag_macr.h tag_misc.h tag_hsc.h tag_img.h tag_list.h vars.h
	gcc -c $(opts) config.c -DUMEM_TRACKING

cleanup.o : cleanup.c global.h macro.h vars.h
	gcc -c $(opts) cleanup.c -DUMEM_TRACKING

tag_a.o : tag_a.c global.h error.h tagargs.h
	gcc -c $(opts) tag_a.c  -DUMEM_TRACKING

tag_macr.o : tag_macr.c find.h global.h error.h macro.h tagargs.h
	gcc -c $(opts) tag_macr.c -DUMEM_TRACKING

tag_misc.o : tag_misc.c global.h error.h macro.h tagargs.h vars.h
	gcc -c $(opts) tag_misc.c -DUMEM_TRACKING

tag_hsc.o : tag_hsc.c global.h error.h macro.h tagargs.h parse.h vars.h
	gcc -c $(opts) tag_hsc.c -DUMEM_TRACKING

tag_img.o : tag_img.c global.h error.h tagargs.h
	gcc -c $(opts) tag_img.c -DUMEM_TRACKING

tag_list.o : tag_list.c global.h error.h tagargs.h
	gcc -c $(opts) tag_list.c -DUMEM_TRACKING

vars.o : vars.c global.h error.h
	gcc -c $(opts) vars.c -DUMEM_TRACKING

## EOF

