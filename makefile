all: UnixLs.c

UnixLs: UnixLs.c
	gcc -g -Wall -o UnixLs UnixLs.c

clean:
	rm -f UnixLs
