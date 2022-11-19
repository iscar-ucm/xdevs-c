CC = gcc
CFLAGS = -Wall -g -O3

XDEVS_SRC = $(shell find ../../core -type f -name '*.c')
XDEVS_OBJ = $(XDEVS_SRC:.c=.o)

SRC_BASE = generator.c job.c processor.c transducer.c
SRC_GPT = gpt.c
OBJ_BASE = $(SRC_BASE:.c=.o)
OBJ_GPT = $(SRC_GPT:.c=.o)

all: main_gpt

main_gpt: $(OBJ_BASE) $(OBJ_GPT) $(XDEVS_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -f $(XDEVS_OBJ) $(OBJ_BASE) $(OBJ_GPT)

uninstall:
	rm -f main_gpt
