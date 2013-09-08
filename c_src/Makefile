LINUX=$(shell uname | grep Linux | wc -l | xargs echo)
DEPS=../deps

ERLANG_HOME=/usr/local/lib/erlang
SLANG_FLAGS=
INSTALL_PATH=$(CURDIR)/../deps/slang

all: $(DEPS)/slang-2.2.4/src/objs/libslang.a

$(DEPS)/slang:
	@mkdir $(DEPS); cd $(DEPS) ; \
	curl ftp://ftp.fu-berlin.de/pub/unix/misc/slang/v2.2/slang-2.2.4.tar.gz | tar xzf -

$(DEPS)/slang-2.2.4/src/objs/libslang.a: $(DEPS)/slang
	@cd $(DEPS)/slang-2.2.4 && ./configure --prefix $(INSTALL_PATH) \
	&& make static && make install-static
