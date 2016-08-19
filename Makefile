SHELL=/bin/sh

NF_EMB_STATIC=1
NF_EMB_SHARED=0

NF_EMB_LIB=libnfemb.so.0.1
NF_EMB_LIBNAME=libnfemb.so
PREFIX=/usr/local

INCS=-I$(CURDIR) -I/usr/local/include -I/usr/local/include/flint/ -I/usr/local/include/arb/ -I/opt/antic
LIBS=-L$(CURDIR) -L/usr/local/lib -lnfemb -larb -lflint -lgmp -lmpfr -lm
LIBS2=-L$(CURDIR) -L/usr/local/lib -larb -lflint -lgmp -lmpfr -lm

CC=gcc
CXX=g++
AR=ar
LDCONFIG=/sbin/ldconfig

CFLAGS=-ansi -pedantic -Wall -O2 -funroll-loops -g -mpopcnt  -Wno-unknown-pragmas
CXXFLAGS=-ansi -pedantic -Wall -O2 -funroll-loops -g -mpopcnt  -Wno-unknown-pragmas

DLPATH=LD_LIBRARY_PATH
DLPATH_ADD=$(CURDIR):/usr/local/lib:/usr/local/lib
EXTRA_BUILD_DIRS=

QUIET_CC  = @echo '   ' CC  ' ' $@;
QUIET_CXX = @echo '   ' CXX ' ' $@;
QUIET_AR  = @echo '   ' AR  ' ' $@;

AT=@
BUILD_DIRS = poly_extra renf renf_elem
LIB_SOURCES = $(wildcard $(patsubst %, %/*.c, $(BUILD_DIRS)))
HEADERS = $(patsubst %, %.h, $(BUILD_DIRS))
LIB_OBJS = $(patsubst %, build/%/*.o, $(BUILD_DIRS))


TEST_SOURCES = $(wildcard test/*.c)

export

all: library

clean:
	$(AT)$(foreach dir, $(BUILD_DIRS), BUILD_DIR=../build/$(dir); export BUILD_DIR; MOD_DIR=$(dir); export MOD_DIR; $(MAKE) -f ../Makefile.subdirs -C $(dir) clean || exit $$?;)
	rm -f $(OBJS) $(LOBJS)
	rm -f libnfemb.a
	rm -rf build

library: $(LIB_SOURCES)
	$(AT)if [ "$(NF_EMB_SHARED)" -eq "1" ]; then \
		$(MAKE) shared; \
	fi
	$(AT)if [ "$(NF_EMB_STATIC)" -eq "1" ]; then \
		$(MAKE) static; \
	fi

libnfemb.a: $(LIB_SOURCES) $(HEADERS)
	$(AT) $(foreach dir, $(BUILD_DIRS), mkdir -p build/$(dir); BUILD_DIR=../build/$(dir); export BUILD_DIR; MOD_DIR=$(dir); export MOD_DIR; $(MAKE) -f ../Makefile.subdirs -C $(dir) static || exit $$?;)
	$(QUIET_AR) $(AR) rcs libnfemb.a
	$(AT) $(foreach mod, $(BUILD_DIRS), $(AR) rcs libnfemb.a build/$(mod)/*.o || exit $$?;)

$(NF_EMB_LIB): $(LIB_SOURCES) $(HEADERS)
	echo "HEY"

shared: $(NF_EMB_LIB)

static: libnfemb.a

tests: library $(TESTS)
	$(AT)$(foreach dir, $(BUILD_DIRS), mkdir -p build/$(dir)/test; BUILD_DIR=../build/$(dir); export BUILD_DIR; $(MAKE) -f ../Makefile.subdirs -C $(dir) tests || exit $$?;)

check: library
	$(AT)$(foreach dir, $(BUILD_DIRS), mkdir -p build/$(dir)/test; BUILD_DIR=../build/$(dir); export BUILD_DIR; $(MAKE) -f ../Makefile.subdirs -C $(dir) check || exit $$?;)


.PHONY: all clean library shared static test
