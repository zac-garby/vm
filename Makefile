CC = gcc
CFLAGS = -Wall -Wextra -Winline --std=c99 -O0 -pedantic
CLIBS = -pthread
OBJECTS = type.o object.o bytecode.o heap.o linked_list.o \
	namespace.o stack.o stackframe.o callstack.o thread.o

%.o: src/%.c src/%.h bin
	$(CC) -o bin/$@ -c $< $(CFLAGS)

bin:
	mkdir -p bin

vm: vm.c bin $(OBJECTS)
	$(CC) -o bin/vm $< $(addprefix bin/,$(OBJECTS)) $(CFLAGS) $(CLIBS)

run: clean vm
	bin/vm

clean:
	rm -rf ./bin
