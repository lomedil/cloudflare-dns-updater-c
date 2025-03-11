#include "cloudflare.h"
#include "json.h"
#include "logger.h"

#define TAG "<cf_parser>"

typedef int find_err;

#define __fok(r) (0 == r)

int find_json_int_value(struct json_object_s *obj, const char *psz_name, int *output);

int find_json_boolean_value(struct json_object_s *obj, const char *psz_name, BOOL *output);

int find_json_list(struct json_object_s *obj, const char *psz_name, struct json_array_s **output);

int find_json_str_slice(struct json_object_s *obj, const char *psz_name, slice_str *output);

cf_error make_cf_error(int code, slice_str message);

void dispose_cf_error(cf_error *error);


int cf_parse_response_from_buffer(const char *buffer, size_t buffer_len, cf_response *output)
{
    cf_response temp = {0};

    buffer_len = strnlen(buffer, buffer_len);

    struct json_value_s *doc = json_parse(buffer, buffer_len);

    if (doc->type == json_type_object)
    {
        struct json_object_s *root = (struct json_object_s *)doc->payload;

        if (!__fok(find_json_boolean_value(root, "success", &temp.success)))
        {
            log_error(TAG "Cannot find 'success' field in response");
            free(doc);
            return 1;
        }

        struct json_array_s *p_list = 0;
        if (!__fok(find_json_list(root, "errors", &p_list)))
        {
            log_error(TAG "Cannot find 'errors' field in response");
            free(doc);
            return 1;
        }

        struct json_array_element_s *iter = p_list->start;
        for (int i = 0; i < p_list->length; ++i, iter = iter->next)
        {
            struct json_object_s *list_item = json_value_as_object(iter->value);
            
            if (json_null == list_item)
            {
                log_fmt_error(TAG "Invalid 'errors' element type: Item %d is not an object.", i);
                free(doc);
                return 1;
            }

            int code = 0;
            slice_str message = { 0 };

            BOOL is_valid_element =
                __fok(find_json_int_value(list_item, "code", &code)) && 
                __fok(find_json_str_slice(list_item, "message", &message));

            if (!is_valid_element)
            {
                log_fmt_error(TAG "Invalid 'errors' element type: Either 'code' or 'message' field are not valid.", i);
                free(doc);
                cf_response_release(&temp);
                return 1;
            }

            temp.errors[i] = make_cf_error(code, message);
        }
    }

    free(doc);
    *output = temp;
    return 0;
}

int cf_parse_response_from_psz(const char *psz_json, cf_response *output)
{
    return cf_parse_response_from_buffer(psz_json, strlen(psz_json), output);
}

#define __is_last_error(e) ((0 != e.code) && (0 != e.message))

void cf_response_release(cf_response *response)
{
    if(response)
    {
        for(int i = 0; i < CF_MAX_ERRORS; ++i)
        {
            if(!__is_last_error(response->errors[i]))
            {
                dispose_cf_error(&response->errors[i]);
            }
        }
    }
}

int find_json_value(struct json_object_s *obj, const char *psz_field, struct json_value_s **output)
{
    struct json_object_element_s *iter = obj->start;
    const int field_len = strlen(psz_field);

    for (int n = 0; n < obj->length; iter = iter->next, ++n)
    {
        if (0 == strcmp(iter->name->string, psz_field) && iter->name->string_size == field_len)
        {
            *output = iter->value;
            return 0;
        }
    }

    return 1;
}

int find_json_int_value(struct json_object_s *obj, const char *psz_field, int *output)
{
    struct json_value_s *p_value = 0;
    const int err = find_json_value(obj, psz_field, &p_value);

    if (0 != err)
        return err;

    if (json_type_number != p_value->type)
        return 1;

    struct json_number_s *number = (struct json_number_s *)p_value->payload;

    char temp[64] = {0};
    strncpy(temp, number->number, number->number_size);
    *output = atoi(temp);
    return 0;
}

int find_json_boolean_value(struct json_object_s *obj, const char *psz_field, BOOL *output)
{
    struct json_value_s *p_value = 0;
    const int err = find_json_value(obj, psz_field, &p_value);

    if (0 != err)
        return err;

    switch (p_value->type)
    {
    case json_type_true:
        *output = 1;
        return 0;
    case json_type_false:
        *output = 0;
        return 0;
    default:
        return 1;
    }
}

int find_json_list(struct json_object_s *obj, const char *psz_name, struct json_array_s **output)
{
    struct json_value_s *p_value = 0;

    const int err = find_json_value(obj, psz_name, &p_value);
    if(0 != err) return err;

    if(json_type_array != p_value->type) return 1;

    *output = p_value->payload;
    return 0;
}

int find_json_str_slice(struct json_object_s *obj, const char *psz_name, slice_str *output)
{
    struct json_value_s *p_value = 0;

    const int err = find_json_value(obj, psz_name, &p_value);
    if(0 != err) return err;

    if(json_type_string != p_value->type) return 1;

    const struct json_string_s *value = (struct json_string_s *)p_value->payload;

    output->ptr = (char *) value->string;
    output->len = (int) value->string_size;

    return 0;
}

cf_error make_cf_error(int code, slice_str message)
{
    cf_error output = {
        .code = code,
        .message = 0
    };

    if(message.ptr)
    {
        const int required_size = message.len + 1;
        output.message = (cf_str_ptr) malloc(required_size);

        strncpy(output.message, message.ptr, message.len);
        output.message[required_size - 1] = 0;
    }

    return output;
}

void dispose_cf_error(cf_error *error)
{
    if(error)
    {
        if(error->message)
        {
            free(error->message);
        }
    }
}

