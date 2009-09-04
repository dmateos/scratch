gcc -g -lcrypt -o crypt crypt.c
gcc -g -lcrypt -o decrypt bfpc.c

time ./decrypt $(./crypt $1 $2) 

