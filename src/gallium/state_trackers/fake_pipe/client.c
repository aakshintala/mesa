#define WIN32_LEAN_AND_MEAN  /*  required by xmlrpc-c/server_abyss.h */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <xmlrpc-c/base.h>
#include <xmlrpc-c/client.h>

#include "wrap.h"

#define SLEEP(seconds) sleep(seconds);

#define NAME "Xmlrpc-c Sync Client"
#define VERSION "1.0"

static xmlrpc_env env;
const char * const serverUrl = "http://localhost:8888/RPC2";
const char * const methodName = "sample.add";

static void 
dieIfFaultOccurred (xmlrpc_env * const envP) {
    if (envP->fault_occurred) {
        fprintf(stderr, "ERROR: %s (%d)\n",
                envP->fault_string, envP->fault_code);
        exit(1);
    }
}

void rpc_sync_start(const char *name)
{
    const char * const methodName = "rpc_server_sync_start";
    init_rpc_service();

    xmlrpc_value * resultP;
    resultP = xmlrpc_client_call(&env, serverUrl, methodName, "(s)", name);
    dieIfFaultOccurred(&env);

    xmlrpc_int32 ret;
    xmlrpc_read_int(&env, resultP, &ret);
    dieIfFaultOccurred(&env);
    if (ret != 0)
        printf("oops, something went wrong..\n");

    xmlrpc_DECREF(resultP);
}

void rpc_sync_end(const char *name)
{
    const char * const methodName = "rpc_server_sync_end";
    init_rpc_service();

    xmlrpc_value * resultP;
    resultP = xmlrpc_client_call(&env, serverUrl, methodName, "(s)", name);
    dieIfFaultOccurred(&env);

    xmlrpc_int32 ret;
    xmlrpc_read_int(&env, resultP, &ret);
    dieIfFaultOccurred(&env);
    if (ret != 0)
        printf("oops, something went wrong..\n");

    xmlrpc_DECREF(resultP);
}

void init_rpc_service(void)
{
    static int initialized = 0;

    if (initialized)
        return;
    else
        initialized = 1;

    /* Initialize our error-handling environment. */
    xmlrpc_env_init(&env);

    /* Create the global XML-RPC client object. */
    xmlrpc_client_init2(&env, XMLRPC_CLIENT_NO_FLAGS, NAME, VERSION, NULL, 0);
    dieIfFaultOccurred(&env);
}
