#ifndef __CMD_UPDATE_H__
#define __CMD_UPDATE_H__

#include <stdio.h>
#include <stdlib.h>

#include <args.h>
#include <logger.h>
#include <cloudflare.h>

#include "definitions.h"
#include "check.h"


static inline int update_command(args_cli params)
{
    const args_psz zone_id = args_parameter(params, "z", "zone");
    if (!zone_id)
    {
        log_error("No zone ID defined in parameters (-z, --zone)");
        return CLIE_NO_ZONE_DEFINED;
    }

    const args_psz record_id = args_parameter(params, "r", "record");
    if (!record_id)
    {
        log_error("No record ID defined in parameters (-r, --record) ");
        return CLIE_NO_RECORD_DEFINED;
    }

    args_psz api_token = args_parameter(params, "t", "token");
    if (!api_token)
    {
        api_token = getenv("CLOUDFLARE_API_TOKEN");
    }

    if (!api_token)
    {
        log_error("No API Token defined (-t, --token, CLOUDFLARE_API_TOKEN envar)");
        return CLIE_NO_TOKEN_DEFINED;
    }

    // Get current IP
    char current_ip[32] = {0};
    int current_ip_size = array_size(current_ip);

    if (CLIE_CANNOT_CONNECT == get_current_ip(current_ip, &current_ip_size))
    {
        fprintf(stderr, "Cannot get public IP");
        return CLIE_CANNOT_CONNECT;
    }

    char req_body[64] = {0};
    sprintf(req_body, "{\"content\":\"%s\"}", current_ip);

    log_fmt_trace("Update DNS request body: %s", req_body);

    http_req_opt opt = {
        .bearer = api_token,
        .content_type = "application/json",
        .psz_body = req_body
    };

    char url[256] = {0};
    sprintf(url, "https://api.cloudflare.com/client/v4/zones/%s/dns_records/%s", zone_id, record_id);
    log_fmt_trace("Update DNS URL: %s", url);


    http_res *res = http_run(kHttpPatch, url, &opt);

    cli_error final_output = CLIE_OK;
    cf_response cf_res = {0};

    if (200 != res->status_code)
    {
        log_fmt_error("Invalid status response from Cloudflare: %d", res->status_code);
        log_fmt_error("Response body: %s", res->body);
        final_output = CLIE_INVALID_RESPONSE;
    }
    else if (0 != cf_parse_response_from_buffer(res->body.data, res->body.size, &cf_res))
    {
        log_fmt_error("Invalid response format from Cloudflare: %s", res->body);
        final_output = CLIE_INVALID_RESPONSE;
    }
    else
    {
        if (cf_res.success)
        {
            log_info("DNS updated");
        }
        else
        {
            cf_error *first_error = cf_res.errors;
            log_fmt_error("DNS not updated. Error (%d) %s", first_error->code, first_error->message);
            final_output = CLIE_DNS_NOT_UPDATED;
        }
        cf_response_release(&cf_res);
    }

    http_res_release(res);

    return CLIE_OK;
}

#endif //!__CMD_UPDATE_H__