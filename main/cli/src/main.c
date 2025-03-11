#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include <stdio.h>
#include <http.h>
#include <logger.h>
#include <args.h>
#include <cloudflare.h>

#include "cli_parser.h" 

#include "cmd/definitions.h"
#include "cmd/check.h"
#include "cmd/update.h"


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
