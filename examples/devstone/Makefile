CC = gcc
CFLAGS = -Wall -g -O3

XDEVS_SRC = $(shell find ../../core -type f -name '*.c')
XDEVS_OBJ = $(XDEVS_SRC:.c=.o)

EXAMPLE_SRC = $(shell find . -type f -name '*.c')
EXAMPLE_OBJ = $(EXAMPLE_SRC:.c=.o)
EXAMPLE_NAME = devstone

all: $(XDEVS_OBJ) $(EXAMPLE_OBJ)
	${CC} ${CFLAGS} $^ -o ${EXAMPLE_NAME}

%.o: %.cpp
	${CC} -c ${CFLAGS} $< -o $@

.PHONY: clean
clean:
	rm -f $(XDEVS_OBJ) $(EXAMPLE_OBJ)

uninstall:
	rm -f $(EXAMPLE_NAME)
