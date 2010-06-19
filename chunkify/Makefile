all: networking.o main.o chunkify.o
	cc -Wall networking.o main.o chunkify.o -o chunkify -L/usr/local/lib
strip: all
	strip *.o chunkify
networking.o: networking.c
	cc -g -c networking.c -I/usr/local/include -I.
main.o: main.c
	cc -g -c main.c -I/usr/local/include -I.
chunkify.o: chunkify.c
	cc -g -c chunkify.c -I/usr/local/include -I.
clean:
	rm -f xsinit.c *.o chunkify
