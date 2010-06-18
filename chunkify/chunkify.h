#ifndef _CHUNKIFY_H_
#define _CHUNKIFY_H_

#define CHUNKSIZE 1024

typedef struct CHUNK_S {
    unsigned int hash; 
    int id;
    char data[CHUNKSIZE];
} chunk_t; 

void *chunkify(const char*, int*);

#endif
