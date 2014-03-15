PRIV := ../priv

# sudo apt-get install libslang2-pic

# LIBS := /usr/lib/libslang.a

ERL_CFLAGS := $(shell erl -noinput -eval \
			'io:format("-I~s/erts-~s/include", [code:root_dir(), erlang:system_info(version)]), halt(0)')

override CFLAGS += -Wall -fPIC $(ERL_CFLAGS)
override CPPFLAGS += -I$(LIBSLANG)/src $(ERL_CPPFLAGS)

all : $(PRIV)/eslang_drv.so

$(PRIV)/eslang_drv.so : slang_drv.o
	$(CC) -shared $(LDFLAGS) -o $@ $^ $(LIBS)

clean:
	$(RM) -f *.o $(PRIV)/eslang_drv.so
