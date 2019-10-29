cc=gcc
libraries=-lpthread
include=src/error.c
debugFlags=-Wall -Wextra -pedantic -DDEBUG=1 -ggdb -Og
buildFlags=-O3 -DDEBUG=0

help:
	$(info Options are)
	$(info clean - remove compiled files)
	$(info debug - compile a debugger version of the files)
	$(info build - compile a build version of the files)
	$(info help - print this help message)

clean:
	rm package compute

debug:
	$(cc) -o package $(libraries) $(include) $(debugFlags) package.c
	$(cc) -o compute $(libraries) $(include) $(debugFlags) compute.c

build:
	$(cc) -o package $(libraries) $(include) $(buildFlags) package.c
	$(cc) -o compute $(libraries) $(include) $(buildFlags) compute.c
