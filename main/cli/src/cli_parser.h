#ifndef __CLI_PARSER_H__
#define __CLI_PARSER_H__

#include <args.h>

typedef enum
{
    kCmdHelp = 0,
    kCmdCheck = 1,
    kCmdUpdate = 2,
    kCmdUnknown = 0xff,
} cli_cmd_kind;


typedef struct
{
    cli_cmd_kind cmd;
    args_cli params;
} cli_cmd;

#define IS_SAME_STR($name, $cmd) (0 == strcasecmp($cmd, $name))

#define NL "\n"

const char *CLI_USAGE = ""
    NL"> cfcli - Cloudflare CLI to update DNS records with the current public IP"
    NL"  Commands: "
    NL"    check: Get public IP from https://ifconfig.me."
    NL"    update: Update CF DNS record with the current public IP"
    NL"      --zone   / -z: Zone ID (your domain)"
    NL"      --record / -r: Record ID (record of your domain to update)"
    NL"      --token  / -t: API Token with DNS edit permission"
    NL"                     <Token can be defined in CLOUDFLARE_API_TOKEN env. var>"
    NL"";
#undef NL

inline static cli_cmd args_command(args_cli args)
{

    cli_cmd output = {
        .cmd = kCmdUnknown,
        .params = args_init(args.count, args.current)
    };

    if(args.count == 0) return output; // Unknown

    char *command = args.current[0];

    if (IS_SAME_STR("check", command))
    {
        output.cmd = kCmdCheck;
    }

    if (IS_SAME_STR("update", command))
    {
        output.cmd = kCmdUpdate;
    }

    if(IS_SAME_STR("help", command))
    {
        output.cmd = kCmdHelp;
    }

    return output;
}

#undef IS_SAME_STR

#endif //!__CLI_PARSER_H__