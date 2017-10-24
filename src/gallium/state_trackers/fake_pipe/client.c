#define WIN32_LEAN_AND_MEAN  /*  required by xmlrpc-c/server_abyss.h */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <xmlrpc-c/base.h>
#include <xmlrpc-c/client.h>

#include "wrap.h"

#define SLEEP(seconds) sleep(seconds);

#define NAME "Xmlrpc-c Srv Client"
#define VERSION "1.0"

static xmlrpc_env env;
const char * const serverUrl = "http://127.0.0.1:8888/RPC2";
const char * const sync_start = "rpc_server_sync_start";
const char * const sync_end = "rpc_server_sync_end";

static int toppest = 2;

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
    printf("start: %s: level=%d\n", name, toppest);
    if (toppest++ > 0)
        return;
    printf("enter_start: %s: level=%d\n", name, toppest);
    init_rpc_service();

    xmlrpc_value * resultP;
    resultP = xmlrpc_client_call(&env, serverUrl, sync_start, "(s)", name);
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
    printf("end: %s: level=%d\n", name, toppest);
    if (toppest-- > 1)
        return;
    init_rpc_service();

    xmlrpc_value * resultP;
    resultP = xmlrpc_client_call(&env, serverUrl, sync_end, "(s)", name);
    dieIfFaultOccurred(&env);

    xmlrpc_int32 ret;
    xmlrpc_read_int(&env, resultP, &ret);
    dieIfFaultOccurred(&env);
    if (ret != 0)
        printf("oops, something went wrong..\n");

    xmlrpc_DECREF(resultP);
}

void rpc_sync_start_x(void)
{
    toppest++;
}

void rpc_sync_end_x(void)
{
    toppest--;
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
