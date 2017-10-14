##
##  Template makefile for Standard, Profile, Debug, Release, and Release-static versions
##
##    eg: "make rs" for a statically linked release version.
##        "make d"  for a debug version (no optimizations).
##        "make"    for the standard version (optimized, but with debug information and assertions active)

PWD        = $(shell pwd)
EXEC      ?= $(notdir $(PWD))

WROOT      = $(PWD)/src
LIB        = psd

MAINFILE   = main

SRCSUF     = cpp

CSRCS      = $(wildcard $(WROOT)/*.$(SRCSUF)) 
DSRCS      = $(foreach dir, $(DEPDIR), $(filter-out $(WROOT)/$(dir)/$(MAINFILE).$(SRCSUF), $(wildcard $(WROOT)/$(dir)/*.$(SRCSUF))))
CHDRS      = $(wildcard $(WROOT)/*.h)
COBJS      = $(CSRCS:.$(SRCSUF)=.o) $(DSRCS:.$(SRCSUF)=.o)

PCOBJS     = $(addsuffix p,  $(COBJS))
DCOBJS     = $(addsuffix d,  $(COBJS))
RCOBJS     = $(addsuffix r,  $(COBJS))

ifeq ($(SRCSUF),cpp)
	CC=g++
	CLANG=clang++

	CFLAGS     =  -pedantic -W -Wall  -std=c++11  
	LFLAGS    =   -pedantic -W -Wall -std=c++11
else
	CC=gcc
	CLANG=clang

	CFLAGS     =  -pedantic -W -Wall      -Wno-parentheses
	LFLAGS    =   -pedantic -W -Wall 
endif


#CC        =gcc
#CXX       = g++
#CFLAGS    = -Wall -Wno-parentheses -pedantic
#LFLAGS    = -Wall  


COPTIMIZE ?= -O3

CFLAGS    +=  -I include -I  ../include -I ../util/include   -I ../poly/include -I ../sdp/include -D __STDC_LIMIT_MACROS -D __STDC_FORMAT_MACROS  #-std=c99
LFLAGS    += -lz   -L ../lib -lpoly -lsdp -lutil  -llapack -lblas   -lm


.PHONY : s p d r rs clean 

s:	$(EXEC)
p:	$(EXEC)_profile
d:	$(EXEC)_debug
r:	$(EXEC)_release
rs:	$(EXEC)_static

libs:	lib$(LIB)_standard.a
libp:	lib$(LIB)_profile.a
libd:	lib$(LIB)_debug.a
libr:	lib$(LIB)_release.a

## Compile options
%.o:			CFLAGS +=$(COPTIMIZE) -g -D DEBUG
%.op:			CFLAGS +=$(COPTIMIZE) -pg -g -D NDEBUG
%.od:			CFLAGS +=-O0 -g -D DEBUG
%.or:			CFLAGS +=$(COPTIMIZE)  -D NDEBUG

## Link options
$(EXEC):		LFLAGS += -g
$(EXEC)_profile:	LFLAGS += -g -pg
$(EXEC)_debug:		LFLAGS += -g
$(EXEC)_release:	LFLAGS += ...
$(EXEC)_static:		LFLAGS += --static

## Dependencies
$(EXEC):		$(COBJS)
$(EXEC)_profile:	$(PCOBJS)
$(EXEC)_debug:		$(DCOBJS)
$(EXEC)_release:	$(RCOBJS)
$(EXEC)_static:		$(RCOBJS)

lib$(LIB)_standard.a:	$(filter-out */$(MAINFILE).o,  $(COBJS))
lib$(LIB)_profile.a:	$(filter-out */$(MAINFILE).op, $(PCOBJS))
lib$(LIB)_debug.a:	$(filter-out */$(MAINFILE).od, $(DCOBJS))
lib$(LIB)_release.a:	$(filter-out */$(MAINFILE).or, $(RCOBJS))

## Build rule
%.o %.op %.od %.or :	%.$(SRCSUF)
	@echo Compiling: $(subst $(WROOT)/,,$@)
	@$(CC) $(CFLAGS) -c -o $@ $<



## Linking rules (standard/profile/debug/release)
$(EXEC)  $(EXEC)_profile $(EXEC)_debug $(EXEC)_release $(EXEC)_static :
	@echo Linking: "$@ ( $(foreach f,$^,$(subst $(WROOT)/,,$f)) )"
	@$(CC) $^  -o $@ $(LFLAGS)



## Library rules (standard/profile/debug/release)
lib$(LIB)_standard.a lib$(LIB)_profile.a lib$(LIB)_release.a lib$(LIB)_debug.a:
	@echo Making library: "$@ ( $(foreach f,$^,$(subst $(WROOT)/,,$f)) )"
	@$(AR) -rcsv $@ $^

## Library Soft Link rule:
libs libp libd libr:
	@echo "Making Soft Link: $^ -> lib$(LIB).a"
	@ln -sf $(PWD)/$^  lib$(LIB).a
	@ln -sf $(PWD)/$^ ../lib/lib$(LIB).a

## Clean rule
clean:
	@rm -f $(EXEC) $(EXEC)_profile $(EXEC)_debug $(EXEC)_release $(EXEC)_static \
	  $(COBJS) $(PCOBJS) $(DCOBJS) $(RCOBJS) *.core depend.mk 

## Make dependencies
depend.mk: $(CSRCS) $(CHDRS)
	@echo Making dependencies
	@$(CC) $(CFLAGS) -I$(WROOT) \
	   $(CSRCS) -MM | sed 's|\(.*\):|$(WROOT)/\1 $(WROOT)/\1r $(WROOT)/\1d $(WROOT)/\1p:|' > depend.mk
	@for dir in $(DEPDIR); do \
	      if [ -r $(WROOT)/$${dir}/depend.mk ]; then \
		  echo Depends on: $${dir}; \
		  cat $(WROOT)/$${dir}/depend.mk >> depend.mk; \
	      fi; \
	  done

#-include $(WROOT)/mtl/config.mk
-include depend.mk
