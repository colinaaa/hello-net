SHELL:=/bin/bash
BUILD:=build
TARGETS:=stop_wait gbn sr tcp

all-tests:=$(addprefix test-, $(TARGETS))
all-targets:=$(addprefix $(BUILD)/, $(TARGETS))

all: $(all-targets)

clean: $(BUILD)
	$(MAKE) -C $< $@
	
.PHONY: all clean test

$(BUILD):
	mkdir $@
	cmake -H. -B$@

$(BUILD)/%: $(BUILD)
	$(MAKE) -C $< $*

test-%: $(BUILD)/%
	time ./check_linux.sh $<

test: $(all-tests)
