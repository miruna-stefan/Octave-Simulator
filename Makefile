
# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -std=c99

SRCS=$(wildcard *.c)
OBJS=$(SRCS:%.c=%.o)
TARGETS=$(OBJS:%.o=%)

build: $(OBJS) $(TARGETS)

pack:
	zip -FSr 314CA_StefanMiruna-Andreea_Tema2.zip README Makefile *.c *.h

clean:
	rm -f $(TARGETS) $(OBJS)

.PHONY: pack clean
