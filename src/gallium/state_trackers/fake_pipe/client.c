#define WIN32_LEAN_AND_MEAN  /*  required by xmlrpc-c/server_abyss.h */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#include <xmlrpc-c/base.h>
#include <xmlrpc-c/client.h>

#include "wrap.h"

#define SLEEP(seconds) sleep(seconds);

#define NAME "Xmlrpc-c Test Client"
#define VERSION "1.0"

static xmlrpc_env env;
const char * const serverUrl = "http://192.168.122.249:8888/RPC2";
const char * const methodName = "rpc_client_sync";
static int toppest = 0;

float rpc_time = 0;
struct timeval tv_start, tv_end;

static void 
dieIfFaultOccurred (xmlrpc_env * const envP) {
    if (envP->fault_occurred) {
        fprintf(stderr, "ERROR: %s (%d)\n",
                envP->fault_string, envP->fault_code);
        exit(1);
    }
}

void rpc_sync(const char *name)
{
    // only send RPCs for the toppest-level pipe functions
    if (toppest++ > 0)
        return;

    init_rpc_service();

    gettimeofday(&tv_start, NULL);

    xmlrpc_value * resultP;
    resultP = xmlrpc_client_call(&env, serverUrl, methodName, "(s)", name);
    dieIfFaultOccurred(&env);

    xmlrpc_int32 ret;
    xmlrpc_read_int(&env, resultP, &ret);
    dieIfFaultOccurred(&env);
    if (ret != 0)
        printf("oops, something went wrong..\n");

    xmlrpc_DECREF(resultP);

    gettimeofday(&tv_end, NULL);
    rpc_time += (tv_end.tv_sec * 1000.0 + tv_end.tv_usec / 1000.0)
        - (tv_start.tv_sec * 1000.0 + tv_start.tv_usec / 1000.0);
}

void rpc_sync_start(void) {
    toppest++;
}

void rpc_sync_end(void) {
    toppest--;
}

void hello(void)
{
    const char * const methodName = "sample.add";
    init_rpc_service();

    printf("Making XMLRPC call to server url '%s' method '%s' "
           "to request the sum "
           "of 5 and 7...\n", serverUrl, methodName);

    /* Make the remote procedure call */
    xmlrpc_value * resultP;
    resultP = xmlrpc_client_call(&env, serverUrl, methodName,
                                 "(ii)", (xmlrpc_int32) 5, (xmlrpc_int32) 7);
    dieIfFaultOccurred(&env);

    /* Get our sum and print it out. */
    xmlrpc_int32 sum;

    xmlrpc_read_int(&env, resultP, &sum);
    dieIfFaultOccurred(&env);
    printf("The sum is %d\n", sum);

    resultP = xmlrpc_client_call(&env, serverUrl, methodName,
                                 "(ii)", (xmlrpc_int32) 3, (xmlrpc_int32) 7);
    xmlrpc_read_int(&env, resultP, &sum);
    printf("The sum is %d\n", sum);

    /* Dispose of our result value. */
    xmlrpc_DECREF(resultP);
}

void clean_on_exit(void)
{
    printf("RPC total time (include exec time): %f\n", rpc_time);
    xmlrpc_env_clean(&env);
    xmlrpc_client_cleanup();
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

    /* Cleanup on exit */
    atexit(clean_on_exit);
}
