CFLAGS=-Wall -Wextra -pedantic -O2 -std=c99

.PHONY: default run test tests clean

run test tests default: unit
	./unit

unit: unit.c unit.h makefile
	${CC} ${CFLAGS} $< -o $@

clean:
	rm -fv *.exe unit
