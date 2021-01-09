CC = gcc
CFLAGS = -Wall --std=c99 -O0
CLIBS = -pthread
OBJECTS = type.o object.o

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