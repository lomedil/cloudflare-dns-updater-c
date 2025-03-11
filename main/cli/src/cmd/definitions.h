#ifndef __CMD_DEFS_H__
#define __CMD_DEFS_H__

typedef int cli_error;
const cli_error CLIE_OK = 0;
const cli_error CLIE_UNKNOWN_COMMAND    = 1;
const cli_error CLIE_CANNOT_CONNECT     = 2;
const cli_error CLIE_NO_ZONE_DEFINED    = 3;
const cli_error CLIE_NO_RECORD_DEFINED  = 4;
const cli_error CLIE_NO_TOKEN_DEFINED   = 5;
const cli_error CLIE_INVALID_RESPONSE   = 6;
const cli_error CLIE_DNS_NOT_UPDATED    = 7;

#endif //!__CMD_DEFS_H__