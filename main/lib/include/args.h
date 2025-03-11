#ifndef __ARGS__H__
#define __ARGS__H__

#include <string.h>
#include <types.h>

typedef char *args_psz;

typedef struct
{
    char **current;
    int count;
} args_cli;

typedef enum
{
    kCheck = 0,
    kUpdate = 1,
    kUnknown = 0xff,
} args_cmd_kind;

typedef struct
{
    args_cmd_kind cmd;
    args_cli params;
} args_cmd;

args_cli args_init(int argc, char **argv);


args_cmd args_command(args_cli args);

args_psz args_parameter(args_cli args, char *short_name, char *long_name);

#endif // !__ARGS__H__
