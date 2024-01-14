.SUFFIXES:
.DEFAULT: all
.ONESHELL:
.NOTPARALLEL:
.SILENT:

.PHONY: all clean distclean graph config release build debug win64 cppcheck tidy cmds format

mkfile := $(abspath $(lastword $(MAKEFILE_LIST)))
srcdir := $(dir $(mkfile))
TARGETS := $(filter-out all clean build distclean,$(MAKECMDGOALS))

mkfile := $(abspath $(lastword $(MAKEFILE_LIST)))
srcdir := $(dir $(mkfile))
TARGETS := $(filter-out all clean build distclean,$(MAKECMDGOALS))

DEFAULT_BUILD_TYPE := Debug

all: build

$(srcdir)/build/build.ninja:
	@./clone.sh
	@cmake -Wdev -DCMAKE_BUILD_TYPE=$(DEFAULT_BUILD_TYPE) -S$(srcdir) -B$(srcdir)/build -G Ninja

build: $(srcdir)/build/build.ninja
	@cd $(srcdir)/build; ninja $(TARGETS)

clean:
	@cmake --build $(srcdir)/build --target clean

distclean:
	@rm -rf $(srcdir)/build

$(TARGETS) :: all ;
