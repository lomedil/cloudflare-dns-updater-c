
#include <stdio.h>
#include <http.h>
#include <logger.h>

int main(){
    
    log_initialize(kConsole);    

    log_debug("Creating request for ifconfig.me");
    http_res *res = http_run(kHttpGet, "https://ifconfig.me/ip", 0);

    log_fmt_info("Status code: %d", res->status_code);
    log_fmt_info("My ip: %s", res->body.data);
    http_res_release(res);

    buffer_t req_body = buffer_init(256);

    http_req_opt opt = {
        .bearer ="asdf",
        .content_type = "application/json",
        .body = req_body
    };

    res = http_run(kHttpPatch, "https://cfcli.free.beeceptor.com/todos", &opt);

    log_fmt_info("Cloudflare API - Status code: %d", res->status_code);
    log_fmt_info("Cloudflare API - Response: %s", res->body);

    http_res_release(res);


    return 0;
}