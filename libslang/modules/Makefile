# Generated automatically from Makefile.in by configure.
# -*- sh -*-
MODULES = smg-module.so termios-module.so select-module.so fcntl-module.so \
  varray-module.so pcre-module.so
#---------------------------------------------------------------------------
CC_SHARED 	= gcc $(CFLAGS) -shared -fPIC
#---------------------------------------------------------------------------
# Installation location of the slang library
#---------------------------------------------------------------------------
prefix 		= /usr
exec_prefix 	= ${prefix}
SLANG_INC	= -I${prefix}/include
SLANG_LIB	= -L${exec_prefix}/lib -lslang
#---------------------------------------------------------------------------
# Installation location of the modules
#---------------------------------------------------------------------------
MODULE_INSTALL_DIR = ${exec_prefix}/lib/slang/modules
INSTALL		= /usr/bin/install -c
INSTALL_DATA	= ${INSTALL} -m 644
MKINSDIR	= ../autoconf/mkinsdir.sh
#---------------------------------------------------------------------------
# DESTDIR is designed to facilitate making packages.  Normally it is empty
#---------------------------------------------------------------------------
DESTDIR =
DEST_MODULEDIR = $(DESTDIR)$(MODULE_INSTALL_DIR)
#---------------------------------------------------------------------------
RPATH = -Wl,-R${exec_prefix}/lib

LIBS = $(RPATH) $(SLANG_LIB) $(DL_LIB) -lm
INCS = $(SLANG_INC)

all: $(MODULES)

smg-module.so: smg-module.c
	$(CC_SHARED) $(INCS) smg-module.c -o smg-module.so $(LIBS)
newt-module.so: newt-module.c
	$(CC_SHARED) $(INCS) newt-module.c -o newt-module.so -lnewt $(LIBS)
termios-module.so: termios-module.c
	$(CC_SHARED) $(INCS) termios-module.c -o termios-module.so $(LIBS)
select-module.so: select-module.c
	$(CC_SHARED) $(INCS) select-module.c -o select-module.so $(LIBS)
fcntl-module.so: fcntl-module.c
	$(CC_SHARED) $(INCS) fcntl-module.c -o fcntl-module.so $(LIBS)
varray-module.so: varray-module.c
	$(CC_SHARED) $(INCS) varray-module.c -o varray-module.so $(LIBS)
pcre-module.so: pcre-module.c
	$(CC_SHARED) $(INCS) pcre-module.c -o pcre-module.so -lpcre $(LIBS)

install: all
	-$(MKINSDIR) $(DEST_MODULEDIR)
	@for i in $(MODULES); \
	do \
		echo $(INSTALL_DATA) $$i $(DEST_MODULEDIR); \
		$(INSTALL_DATA) $$i $(DEST_MODULEDIR); \
	done
clean:
	-/bin/rm -f $(MODULES) *~
