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


args_cli args_init(int argc, char **argv);

args_psz args_parameter(args_cli args, char *short_name, char *long_name);

#endif // !__ARGS__H__
