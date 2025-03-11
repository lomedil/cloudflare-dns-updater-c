#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include <stdio.h>
#include <http.h>
#include <logger.h>
#include <args.h>
#include <cloudflare.h>

#include "cli_parser.h" 

typedef int cli_error;
const cli_error CLIE_OK = 0;
const cli_error CLIE_UNKNOWN_COMMAND    = 1;
const cli_error CLIE_CANNOT_CONNECT     = 2;
const cli_error CLIE_NO_ZONE_DEFINED    = 3;
const cli_error CLIE_NO_RECORD_DEFINED  = 4;
const cli_error CLIE_NO_TOKEN_DEFINED   = 5;
const cli_error CLIE_INVALID_RESPONSE   = 6;
const cli_error CLIE_DNS_NOT_UPDATED    = 7;

#define array_size(arr) (sizeof(arr) / sizeof(arr[0]))

int check_command();
int update_command(args_cli params);

cli_error get_current_ip(char *ip, int *ip_size);

int main(int argc, char **argv)
{
    log_initialize(kConsole);

    cli_cmd cli = args_command(args_init(argc, argv));

    switch (cli.cmd)
    {
    case kCmdHelp:
        printf("%s", CLI_USAGE);
        return CLIE_OK;
    case kCmdCheck:
        return check_command();
    case kCmdUpdate:
        return update_command(cli.params);
    case kCmdUnknown:
        fprintf(stderr, "Unknown subcommand\n");
        return CLIE_UNKNOWN_COMMAND;
        break;
    }

    return 0;
}

int check_command()
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

int update_command(args_cli params)
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

cli_error get_current_ip(char *ip, int *ip_size)
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