# This is the GNU Make shim for Linux and macOS

DESTDIR ?=
prefix ?= /usr/local

examples: default
	./build examples

clean: default
	./build clean

capi: default
	./build capi

install:
	mkdir -p "$(DESTDIR)$(prefix)/include/fastWebSockets"
	cp -r src/* "$(DESTDIR)$(prefix)/include/fastWebSockets"

all: default
	./build all

default:
	$(MAKE) -C fastSockets
	$(CC) build.c -o build
