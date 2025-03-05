#ifndef __BUFFFER__H_
#define __BUFFFER__H_

#include <stdlib.h>
#include <memory.h>

typedef struct
{
    char* data;
    int size;
}  buffer_t;

buffer_t buffer_init(int size){
    buffer_t output = {0};
    output.data = (char *) malloc(size);
    output.size = size;
    return output;
}

int buffer_ensure_space(buffer_t *buffer, int new_size)
{
    int old_size = buffer->size;
    if(new_size <= old_size) return 0;

    buffer->data = (char*) realloc(buffer->data, new_size);
    buffer->size = new_size;

    return new_size - old_size;
}

void buffer_release(buffer_t *buffer)
{
    if(buffer->data != 0)
    {
        free(buffer->data);
    }

    buffer->data = 0;
    buffer->size = 0;
}

void buffer_clear(buffer_t *buffer)
{
    if(buffer->data != 0)
    {
        memset(buffer->data, 0, buffer->size);
    }
}

#endif //!__BUFFFER__H_