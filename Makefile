cc=gcc
libraries=-lpthread
include=src/error.c
clean:
	rm package compute

debug:
	$(cc) -o package $(libraries) $(include) -DDEBUG=1 -g -Og package.c
	$(cc) -o compute $(libraries) $(include) -DDEBUG=1 -g -Og compute.c

all:
	$(cc) -o package $(libraries) $(include) -DDEBUG=0 package.c
	$(cc) -o compute $(libraries) $(include) -DDEBUG=0 compute.c
