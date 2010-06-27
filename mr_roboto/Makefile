all: networking.o main.o irc.o conf.o module.o
	cc -Wall networking.o main.o irc.o conf.o module.o -o mrroboto -L/usr/local/lib -ldl
strip: all
	strip *.o mrroboto
networking.o: networking.c
	cc -g -c networking.c -I/usr/local/include -I.
main.o: main.c
	cc -g -c main.c -I/usr/local/include -I.
irc.o: irc.c
	cc -g -c irc.c -I/usr/local/include -I.
conf.o: conf.c
	cc -g -c conf.c -I/usr/local/include -I.
module.o: module.c
	cc -g -c module.c -I/usr/local/include -I.
clean:
	rm -f xsinit.c *.o mrroboto
