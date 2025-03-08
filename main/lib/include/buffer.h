#ifndef __BUFFFER__H_
#define __BUFFFER__H_

#include <stdlib.h>
#include <memory.h>

typedef struct
{
    char* data;
    int size;
}  buffer_t;

buffer_t buffer_init(int size);

int buffer_ensure_space(buffer_t *buffer, int new_size);

void buffer_release(buffer_t *buffer);

void buffer_clear(buffer_t *buffer);

buffer_t buffer_take(buffer_t *buffer);

buffer_t buffer_clone(buffer_t *buffer);

#endif //!__BUFFFER__H_