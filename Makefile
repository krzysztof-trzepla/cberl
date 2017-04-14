##
## Rebar targets
##

.PHONY: all
all: compile

.PHONY: compile
compile:
	./rebar3 compile

.PHONY: clean
clean:
	./rebar3 clean

.PHONY: dialyzer
dialyzer:
	./rebar3 dialyzer
