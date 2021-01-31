# vm

A virtual machine for a simple programming language.

The aim is to have a working VM, including:

 - Lightweight (non-OS) threads
 - Message passing between these threads
 - Garbage Collection
 - C Interfacing

## Running the VM

Well it doesn't do a whole lot at the moment, but compilation is easy.

```
mkdir bin

   then:

make vm    # just compile

   or
   
make run   # compile and run the vm
```

At the moment, it'll just run a meaningless test program.
