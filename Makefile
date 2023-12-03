
.PHONY: all clean
.DEFAULT: opt

SRCDIR := $(CURDIR)

FILES := \
  src/configuration.c \
  src/daemonize.c \
  src/http.c \
  src/jsonrpc.c \
  src/main.c \
  src/plugins.c

HEADERS := \
  src/config.h \
  src/configuration.h \
  src/http.h \
  src/jsonrpc.h \
  src/plugins.h \
  src/plugin_register.h \
  src/plugins.h \
  src/daemonize.h

#sanitize:=-fsanitize=address
sanitize:=#
optimize:=-O3
#optimize:=-g
CFLAGS:=$(optimize) $(sanitize)

all: hamware hamware_plugins

hamware: $(FILES) $(HEADERS) Makefile
	@gcc -o $@ $(CFLAGS) $(FILES)

hamlib: plugins/hamlib.c src/plugin_register.h
	@gcc -shared -o plugins/hamlib.so -Isrc plugins/hamlib.c -lhamlib

hamware_plugins: hamlib

clean:
	@rm -f hamware
	@rm -f plugins/hamlib.so
