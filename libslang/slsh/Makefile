# Generated automatically from Makefile.in by configure.
CC		= gcc
CFLAGS		= -O2 -fpic  -fno-strength-reduce
LDFLAGS		=  -Wl,-export-dynamic
#---------------------------------------------------------------------------
# Installation location of the slang library
#---------------------------------------------------------------------------
prefix 		= /usr
exec_prefix 	= ${prefix}
SLANG_INC	= -I${prefix}/include
SLANG_LIB	= -L${exec_prefix}/lib -lslang
#---------------------------------------------------------------------------
DL_LIB		= -ldl
RPATH		= -Wl,-R${exec_prefix}/lib
#----------------------------------------------------------------------------
INSTALL		= /usr/bin/install -c
INSTALL_DATA	= ${INSTALL} -m 644
MKINSDIR	= ../autoconf/mkinsdir.sh
#----------------------------------------------------------------------------
# Where system-wide slsh.rc and library files reside
#----------------------------------------------------------------------------
SLSH_CONF_DIR = ${prefix}/etc
SLSH_LIB_DIR = ${prefix}/share/slsh
SLSH_LOCALLIB_DIR = $(SLSH_LIB_DIR)/local-packages
#---------------------------------------------------------------------------
# DESTDIR is designed to facilitate making packages.  Normally it is empty
#---------------------------------------------------------------------------
DESTDIR =
BIN_DIR = ${exec_prefix}/bin
DEST_BIN_DIR = $(DESTDIR)$(BIN_DIR)
DEST_SLSH_CONF_DIR = $(DESTDIR)$(SLSH_CONF_DIR)
DEST_SLSH_LIB_DIR = $(DESTDIR)$(SLSH_LIB_DIR)
DEST_SLSH_LOCALLIB_DIR = $(DESTDIR)$(SLSH_LOCALLIB_DIR)
#----------------------------------------------------------------------------
# End of user configuration
#----------------------------------------------------------------------------

SHELL = /bin/sh
LIBS = $(RPATH) $(SLANG_LIB) $(DL_LIB) -lm
DEFS = -DSLSH_CONF_DIR='"$(SLSH_CONF_DIR)"' -DSLSH_PATH='"$(SLSH_LIB_DIR)"'

slsh: slsh.c config.h
	$(CC) $(CFLAGS) $(DEFS) slsh.c -o slsh $(SLANG_INC) $(LDFLAGS) $(LIBS)
config.h: ../src/config.h
	cp ../src/config.h .
install_directories:
	$(MKINSDIR) $(DEST_BIN_DIR)
	$(MKINSDIR) $(DEST_SLSH_CONF_DIR)
	$(MKINSDIR) $(DEST_SLSH_LIB_DIR)
	$(MKINSDIR) $(DEST_SLSH_LOCALLIB_DIR)
install_lib_files:
	@for X in lib/*.sl; \
	do \
		echo $(INSTALL_DATA) $$X $(DEST_SLSH_LIB_DIR); \
		$(INSTALL_DATA) $$X $(DEST_SLSH_LIB_DIR); \
		if [ "$$?" != "0" ]; then \
		  exit 1; \
		fi; \
	done

install: slsh install_directories install_lib_files
	$(INSTALL) slsh $(DEST_BIN_DIR)/
	$(INSTALL_DATA) lib/slsh.rc $(DEST_SLSH_CONF_DIR)/
	echo 'prepend_to_slang_load_path("$(SLSH_LOCALLIB_DIR)");' >> $(DEST_SLSH_CONF_DIR)/slsh.rc
#---------------------------------------------------------------------------
# Housekeeping
#---------------------------------------------------------------------------
clean:
	-/bin/rm -f *~ slsh scripts/*~ lib/*~
distclean: clean
	-/bin/rm -f Makefile
symlinks:
	-/bin/rm -f $(ARCH)objs
	-mkdir -p $(HOME)/sys/$(ARCH)/objs/jdl/src
	ln -s $(HOME)/sys/$(ARCH)/objs/jdl/src $(ARCH)objs
