PRIV := ../priv

LIBSLANG := ../slang-2.2.4
LIBS := $(LIBSLANG)/src/objs/libslang.a
#LIBS := /usr/lib/x86_64-linux-gnu/libslang.a

ERL_CFLAGS := $(shell erl -noinput -eval \
			'io:format("-I~s/erts-~s/include", [code:root_dir(), erlang:system_info(version)]), halt(0)')

override CFLAGS += -Wall -fPIC $(ERL_CFLAGS)
override CPPFLAGS += -I$(LIBSLANG)/src $(ERL_CPPFLAGS)
override LDFLAGS += -fpic -lslang

all : $(PRIV)/eslang_drv.so

$(PRIV)/eslang_drv.so : slang_drv.o
	$(CC) -shared $(LDFLAGS) -o $@ $^ $(LIBS)

clean:
	$(RM) -f *.o $(PRIV)/eslang_drv.so
