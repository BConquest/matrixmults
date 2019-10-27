cc=gcc
libraries=-lpthread

clean:
	rm package compute

debug:
	$(cc) -o package $(libraries) -DDEBUG=1 -g -Og package.c
	$(cc) -o compute $(libraries) -DDEBUG=1 -g -Og compute.c

all:
	$(cc) -o package $(libraries) -DDEBUG=0 package.c
	$(cc) -o compute $(libraries) -DDEBUG=0 compute.c
