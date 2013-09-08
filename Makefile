REBAR=rebar

all: get-deps compile
get-deps:
	$(REBAR) get-deps
compile:
	$(REBAR) compile
clean:
	$(REBAR) clean
	rm -rf ebin/*.config
