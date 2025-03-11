#ifndef __CMD_CHECK_H__
#define __CMD_CHECK_H__

#include <stdio.h>

#include <logger.h>
#include <http.h>
#include <assert.h>

#include "definitions.h"


#define array_size(arr) (sizeof(arr) / sizeof(arr[0]))


inline static cli_error get_current_ip(char *ip, int *ip_size)
{
    int output = 0;
    log_trace("Getting public IP from ifconfig.me");
    http_res *res = http_run(kHttpGet, "https://ifconfig.me/ip", 0);

    if (!res)
        return CLIE_CANNOT_CONNECT;

    if (res->status_code == 200)
    {
        int required_size = strnlen(res->body.data, res->body.size);
        assert(required_size < res->body.size);
        assert(*ip_size >= required_size + 1);

        *ip_size = required_size + 1;
        memcpy(ip, res->body.data, required_size + 1);
    }
    else
    {
        output = res->status_code;
    }

    http_res_release(res);

    return output;
}

inline static int check_command()
{
    char current_ip[32] = {0};
    int current_ip_size = array_size(current_ip);

    cli_error err = get_current_ip(current_ip, &current_ip_size);

    if (err == CLIE_CANNOT_CONNECT)
    {
        log_error("Cannot get our public IP: Cannot connect");
        return CLIE_CANNOT_CONNECT;
    }
    else if (err != 0)
    {
        log_fmt_error("Cannot get our public IP: Status code %d", err);
        return err;
    }

    fprintf(stdout, "%s", current_ip);
    return 0;
}

#endif //!__CMD_CHECK_H__