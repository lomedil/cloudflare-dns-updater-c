#include "buffer.h"

 #include "logger.h"

buffer_t buffer_init(int size){
    buffer_t output = {0};
    output.data = (char *) malloc(size);
    memset(output.data, 0, sizeof(size));
    output.size = size;
    return output;
}

int buffer_ensure_space(buffer_t *buffer, int new_size)
{
    int old_size = buffer->size;
    if(new_size <= old_size) return 0;

    log_fmt_trace("Creating new buffer for %d bytes", new_size);
    char *new_buffer = (char *)malloc(new_size);
    memcpy(new_buffer, buffer->data, buffer->size);

    free(buffer->data);

    buffer->data = new_buffer;
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

