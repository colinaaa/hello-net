SHELL:=/bin/bash
BUILD:=build
TARGETS:=stop_wait gbn sr

all-tests:=$(addprefix test-, $(TARGETS))
all-targets:=$(addprefix $(BUILD)/, $(TARGETS))

all: $(all-targets) $(all-test)

clean: $(BUILD)
	$(MAKE) -C $<

$(BUILD):
	mkdir $@
	cmake -H. -B$@

$(BUILD)/%: $(BUILD)
	$(MAKE) -C $< $*

test-%: $(BUILD)/%
	time ./check_linux.sh $<

test: $(all-tests)
