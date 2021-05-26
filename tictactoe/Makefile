CC=gcc
CFLAGS=-Iinclude -pedantic -Wall -Wextra -Wsign-conversion -Wconversion -Wshadow -lcodl
OBJS=main.o
SRC=src

all: options main.o tictactoe

options:
	@echo TicTacToe build options:
	@echo "CC	= $(CC)"
	@echo "CFLAGS	= $(CFLAGS)"
	@echo

clean:
	rm -rf tictactoe

main.o: ${SRC}/main.c
	${CC} ${CFLAGS} -c $^

tictactoe: ${OBJS}
	${CC} ${CFLAGS} $^ -o $@
	rm -rf ${OBJS}
	@echo "Now you can run the game. Enjoy! :>"
