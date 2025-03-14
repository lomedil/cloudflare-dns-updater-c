#include <args.h>
#include <stdio.h>

args_cli args_init(int argc, char **argv)
{
    args_cli output = {
        .count = 0,
        .current = 0};

    if (argc > 1)
    {
        output.current = argv + 1;
        output.count = argc - 1;
    }

    return output;
}

#define IS_SAME_STR($name, $cmd) (0 == strcasecmp($cmd, $name))

args_psz args_parameter(args_cli args, char *short_name, char *long_name)
{
    int i = 0;
    args_psz current = NULL;
    char **iter = NULL;
    BOOL cond = 0;

    if (args.count <= 1)
        return NULL;

    if (long_name != NULL && 0 != strlen(long_name))
    {
        iter = args.current;

        for (i = 0; i < args.count; ++i, ++iter)
        {
            current = iter[0];

            cond = (strstr(current, "--") == current) 
                && (strlen(current + 2) > 0) 
                && IS_SAME_STR(long_name, current + 2) 
                && (i + 1 < args.count);

            if (cond) return iter[1];
        }
    }

    if (short_name != NULL && 0 != strlen(short_name))
    {
        iter = args.current;

        for (i = 0; i < args.count; ++i, ++iter)
        {
            current = *iter;

            cond = (strstr(current, "-") == current) 
                && (strstr(current, "--") == NULL)
                && (strlen(current + 1) > 0) 
                && IS_SAME_STR(short_name, current + 1) 
                && (i + 1 < args.count);

            if (cond) return iter[1];
        }
    }

    return NULL;
}
