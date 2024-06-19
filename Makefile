TARGET = bin/derdiedas
SRC = $(wildcard src/*.c)

default: clean
	mkdir -p bin
	gcc ${SRC} -o ${TARGET} -Iinclude

run:
	./$(TARGET)

clean:
	rm -rf bin
	rm -f terminal_output.tmp