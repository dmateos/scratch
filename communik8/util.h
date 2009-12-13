#ifndef _UTIL_H
#define _UTIL_H

#define _MEMDEBUG

#define emalloc(d) _emalloc(__PRETTY_FUNCTION__, d)
#define efree(d) _efree(__PRETTY_FUNCTION__, d)

void *_emalloc(const char *function, int data);
void _efree(const char *function, void *data);

#endif
