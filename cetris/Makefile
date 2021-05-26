CC=gcc
CFLAGS=-Iinclude -pedantic -Wall -Wextra -Wsign-conversion -Wconversion -Wshadow -lcodl
OBJS=main.o tetrisbase.o tetrisinterface.o
SRC=src

all: options main.o tetrisbase.o tetrisinterface.o cetris

options:
	@echo cetris build options:
	@echo "CC	= $(CC)"
	@echo "CFLAGS	= $(CFLAGS)"
	@echo

clean:
	rm -rf cetris title_img.cdl

main.o: ${SRC}/main.c
	${CC} ${CFLAGS} -c $^

tetrisbase.o: ${SRC}/tetrisbase.c
	${CC} ${CFLAGS} -c $^

tetrisinterface.o: ${SRC}/tetrisinterface.c
	${CC} ${CFLAGS} -c $^

cetris: ${OBJS}
	${CC} ${CFLAGS} $^ -o $@
	cp ${SRC}/title_img.cdl .
	rm -rf ${OBJS}
	@echo "Now you can run the game. Enjoy! :>"
