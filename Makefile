
.PHONY: all clean
.DEFAULT: opt

SRCDIR := $(CURDIR)

FILES := \
  src/daemonize.c \
  src/main.c

HEADERS := \
  src/config.h \
  src/daemonize.h

#sanitize:=-fsanitize=address
sanitize:=#
optimize:=-O3
#optimize:=-g
CFLAGS:=$(optimize) $(sanitize)

all: hamware

hamware: $(FILES) $(HEADERS) Makefile
	@gcc -o $@ $(CFLAGS) $(FILES)

clean:
	@rm -f hamware
