CC = gcc
CFLAGS = -Wall -Wextra -Winline --std=c99 -O0
CLIBS = -pthread
OBJECTS = $(addprefix bin/,type.o object.o bytecode.o heap.o \
	linked_list.o namespace.o stack.o stackframe.o \
	callstack.o thread.o)

bin/%.o: src/%.c src/%.h
	$(CC) -o $@ -c $< $(CFLAGS)

bin:
	mkdir -p bin

vm: vm.c $(OBJECTS)
	$(CC) -o bin/vm $< $(OBJECTS) $(CFLAGS) $(CLIBS)

run: vm
	bin/vm

clean:
	rm -rf ./bin
