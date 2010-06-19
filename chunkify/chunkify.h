#ifndef _CHUNKIFY_H_
#define _CHUNKIFY_H_

#define KB(x) (x*1024)
#define MB(x) (x*1024*1024)
#define GB(x) (x*1024*1024*1024)

#define CHUNKSIZE MB(1)

typedef struct CHUNK_S {
    unsigned int hash; 
    int id;
    char data[CHUNKSIZE];
} chunk_t; 

void *chunkify(const char*, int*);
void dechunkify(const char*, void*);

#endif
