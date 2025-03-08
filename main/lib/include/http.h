#ifndef __HTTP_CLIENT__
#define __HTTP_CLIENT__

#include "buffer.h"

typedef  struct {
    char* bearer;
    char* content_type;
    buffer_t body;
}  http_req_opt;

typedef struct{
    int status_code;
    buffer_t body;
} http_res;

typedef enum {
    kHttpGet,
    kHttpPostFields,
    kHttpPostJson,
    kHttpPut,
    kHttpDelete,
    kHttpPatch,
}http_method;

http_res *http_run(http_method method, const char* url, http_req_opt *options);

void http_res_release(http_res *data);

#endif // __HTTP_CLIENT__