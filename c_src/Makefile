PRIV := ../priv

ERL_CFLAGS := $(shell erl -noinput -eval \
			'io:format("-I~s/erts-~s/include", [code:root_dir(), erlang:system_info(version)]), halt(0)')

override CFLAGS += -Wall -fPIC $(ERL_CFLAGS)
override LDFLAGS += -lslang

all : $(PRIV)/slang_drv.so

$(PRIV)/slang_drv.so : slang_drv.o
	$(CC) -shared $(LDFLAGS) -o $@ $^ $(LIBS)

clean:
	$(RM) -f *.o $(PRIV)/slang_drv.so
