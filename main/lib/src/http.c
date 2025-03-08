#include "http.h"

#include <curl/curl.h>
#include <curl/easy.h>
#include <logger.h>

#define TAG "<http> "

typedef struct
{
    buffer_t buffer;
    size_t len;
} curl_buffer;

typedef struct
{
    struct curl_slist *headers;
    curl_buffer req_body;
} prepare_state;

prepare_state prepare_curl_state(CURL *curl, http_req_opt *req)
{
    prepare_state output = {0};

    if(!req) return output; // empty
    
    if (req->bearer)
    {
        curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BEARER);
        curl_easy_setopt(curl, CURLOPT_XOAUTH2_BEARER, req->bearer);
    }

    if (req->content_type)
    {
        char temp[256] = {0};
        sprintf(temp, "content-type: %s", req->content_type);
        output.headers = curl_slist_append(output.headers, temp);
    }

    if (req->body.data)
    {
        output.req_body.buffer = req->body;
        output.req_body.len = 0;
        curl_easy_setopt(curl, CURLOPT_READDATA, &output.req_body);
    }

    return output;
}

void dispose_curl_state(prepare_state *state)
{
    if (state->headers)
    {
        curl_slist_free_all(state->headers);
        state->headers = 0;
    }
}

size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    log_debug(TAG "writter - Buffering response");
    curl_buffer *response = (curl_buffer *)userdata;
    size_t bytes_to_write = size * nmemb;
    char *dest = 0;

    log_fmt_trace(TAG "writter - Ensuring buffer space for %d", bytes_to_write);
    buffer_ensure_space(&response->buffer, bytes_to_write + response->buffer.size);

    dest = response->buffer.data + response->len;

    log_trace(TAG "writter - Writing bytes to buffer");
    memcpy(dest, ptr, bytes_to_write);
    response->len += bytes_to_write;

    return bytes_to_write;
}

http_res *http_run(http_method method, const char* psz_url, http_req_opt *options)
{
    http_res *res = 0;
    curl_buffer response = {0};
    CURL *curl = curl_easy_init();

    log_trace(TAG "Setting URL");
    curl_easy_setopt(curl, CURLOPT_URL, psz_url);

    prepare_state state = prepare_curl_state(curl, options);

    // Init the response
    log_trace(TAG "Preparing the response buffer");
    response.len = 0;
    response.buffer = buffer_init(256);

    // Set the response function
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    // Set method
    switch (method)
    {
    case kHttpPostFields:
        // TODO: Need to pass CURLOPT_POSTFIELDS
        // https://curl.se/libcurl/c/CURLOPT_POST.html
        curl_easy_setopt(curl, CURLOPT_POST, 1);
        break;
    case kHttpPostJson:
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        break;
    case kHttpDelete:
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
        break;
    case kHttpPut:
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        break;
    case kHttpPatch:
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
        break;
    case kHttpGet:
    default:
        break;
    }


    log_trace(TAG "Performing request");
    CURLcode code = curl_easy_perform(curl);

    if (CURLE_OK != code)
    {
        log_error(TAG "Perform cURL failed");
        dispose_curl_state(&state);
        return 0;
    }

    log_debug(TAG "Request completed. Creating response object");
    res = (http_res *)malloc(sizeof(http_res));

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &res->status_code);
    log_fmt_debug(TAG "Status code: %d", res->status_code);
    
    log_fmt_trace(TAG "Response length: %d", response.len);
    response.buffer.data[response.len] = 0;
    res->body = response.buffer;

    dispose_curl_state(&state);
    return res;
}

void http_res_release(http_res *data)
{
    if (data)
    {
        buffer_release(&data->body);
        free(data);
    }
}