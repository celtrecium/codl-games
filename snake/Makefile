CC=gcc
CFLAGS=-Iinclude -pedantic -Wall -Wextra -Wsign-conversion -Wconversion -Wshadow -ansi -lcodl
OBJS=main.o snk_draw.o snk_init_end.o snk_logic.o
SRC=src

all: options main.o snk_draw.o snk_init_end.o snk_logic.o snake

options:
	@echo Snake build options:
	@echo "CC	    = $(CC)"
	@echo "CFLAGS	= $(CFLAGS)"
	@echo

clean:
	rm -rf snake

main.o: ${SRC}/main.c
	${CC} ${CFLAGS} -c $^

snk_draw.o: ${SRC}/snk_draw.c
	${CC} ${CFLAGS} -c $^

snk_init_end.o: ${SRC}/snk_init_end.c
	${CC} ${CFLAGS} -c $^

snk_logic.o: ${SRC}/snk_logic.c
	${CC} ${CFLAGS} -c $^

snake: ${OBJS}
	${CC} ${CFLAGS} $^ -o $@
	rm -rf ${OBJS}
	@echo "Now you can run the game. Enjoy! :>"
