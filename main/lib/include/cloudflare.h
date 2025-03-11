#ifndef __CLOUDFLARE_H__
#define __CLOUDFLARE_H__

#include <types.h>
#include <stddef.h>

#define CF_MAX_ERRORS 16

typedef char* cf_str_ptr;

typedef struct
{
    int code;
    cf_str_ptr message;
} cf_error;

typedef struct
{
    BOOL success;
    cf_error errors[CF_MAX_ERRORS];
} cf_response;

int cf_parse_response_from_psz(const char *psz_json, cf_response *output);

int cf_parse_response_from_buffer(const char *buffer, size_t buffer_len, cf_response *output);

void cf_response_release(cf_response *response);

#endif // !__CLOUDFLARE_H__