#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chunkify.h"

static int flen(FILE *f) {
    int pos, end;

    pos = ftell(f);
    fseek(f, 0, SEEK_END);
    end = ftell(f);
    fseek(f, pos, SEEK_SET);
    return end;
}

static void *loadfile(const char *path, int *sizeout) {
    void *buffer;
    FILE *file;
    int size, read;

    if(!(file = fopen(path, "r"))) {
        fprintf(stderr, "could not open file %s\n", path);
        exit(1);
    }

    size = flen(file);
    if(!(buffer = malloc(size * sizeof(char)))) {
        fprintf(stderr, "could not allocate file read buffer\n");
        exit(1);
    }

    if((read = fread(buffer, sizeof(char), size, file)) != size) {
        fprintf(stderr, "could not read all of file %s\n", path);
        exit(1);
    }

    fclose(file);
    *sizeout = read;
    fprintf(stdout, "loaded file %s %dbytes\n", path, read);
    return buffer;
}

void *chunkify(const char *path, int *ccount) {
    int dsize, chunkcount, i;
    void *dbuffer, *dptr;;
    chunk_t *chunks;

    /* Read the file, allocate some chunks. */
    dbuffer = loadfile(path, &dsize);
    chunkcount = (dsize/CHUNKSIZE)+1;
    if(!(chunks = malloc((sizeof *chunks)*chunkcount))) { /* TODO: nope.. */
        fprintf(stderr, "chunk alloc error\n");
    }
    dptr = dbuffer;
    fprintf(stdout, "allocated %d chunks capable of storing %d each (%d)\n", chunkcount, CHUNKSIZE, chunkcount*CHUNKSIZE);

    /* Put the file contents into the indovidual chunks. */
    /* TODO: need to read to fiel end not chunks. */
    for(i = 0; i < chunkcount; i++) {
        memcpy(chunks[i].data, dptr, CHUNKSIZE);
        dptr += CHUNKSIZE;
        chunks[i].id = i;

        /* Just some neat debug printing. */
        if(i < 10)
            fprintf(stdout, "[0%d] ", chunks[i].id);
        else    
            fprintf(stdout, "[%d] ", chunks[i].id);
        if(!(i % 10) && i != 0)
            fprintf(stdout, "\n");
    }
    fprintf(stdout, "\n");

    free(dbuffer);
    *ccount = chunkcount;
    return chunks;
}

void dechunkify(const char *path, void *data) {

}
