#define WIN32_LEAN_AND_MEAN  /* required by xmlrpc-c/server_abyss.h */

#include <CL/cl.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include <xmlrpc-c/base.h>
#include <xmlrpc-c/server.h>
#include <xmlrpc-c/server_abyss.h>

#define SLEEP(seconds) sleep(seconds);

#define RETURN_IF_FAULT(envP) \
    do { \
        if ((envP)->fault_occurred) \
            return NULL; \
    } while (0)

static xmlrpc_value *
sample_add(xmlrpc_env *   const envP,
           xmlrpc_value * const paramArrayP,
           void *         const serverInfo,
           void *         const channelInfo) {

    xmlrpc_int32 x, y, z;

    /* Parse our argument array. */
    xmlrpc_decompose_value(envP, paramArrayP, "(ii)", &x, &y);
    if (envP->fault_occurred)
        return NULL;

    /* Add our two numbers. */
    z = x + y;

    /* Sometimes, make it look hard (so client can see what it's like
       to do an RPC that takes a while).
    */
    if (y == 1)
        SLEEP(5);

    /* Return our result. */
    return xmlrpc_build_value(envP, "i", z);
}

static xmlrpc_value* rpc_nvc0_create(xmlrpc_env* const envP,
    xmlrpc_value* const paramArrayP, void* const user_data)
{
    void *priv;
    unsigned ctxflags;
    xmlrpc_decompose_value(envP, paramArrayP, "(si)", &priv, (xmlrpc_int32 *)&ctxflags);
    RETURN_IF_FAULT(envP);
    printf("%d %s\n", ctxflags, (char *)priv);

    return xmlrpc_build_value(envP, "i", 0);
}

static xmlrpc_value* rpc_nvc0_screen_get_param(xmlrpc_env* const envP,
    xmlrpc_value* const paramArrayP, void* const user_data)
{
    int param;
    xmlrpc_decompose_value(envP, paramArrayP, "(i)", (xmlrpc_int32 *)&param);
    RETURN_IF_FAULT(envP);
    printf("%d\n", param);

    return xmlrpc_build_value(envP, "i", 0);
}

int
main(int           const argc,
     const char ** const argv) {

    xmlrpc_server_abyss_parms serverparm;
    xmlrpc_registry * registryP;
    xmlrpc_env env;

    if (argc-1 != 1) {
        fprintf(stderr, "You must specify 1 argument:  The TCP port "
                "number on which the server will accept connections "
                "for RPCs (8080 is a common choice).  "
                "You specified %d arguments.\n",  argc-1);
        exit(1);
    }

    xmlrpc_env_init(&env);

    registryP = xmlrpc_registry_new(&env);
    if (env.fault_occurred) {
        printf("xmlrpc_registry_new() failed.  %s\n", env.fault_string);
        exit(1);
    }

    /* register simple.add */
    struct xmlrpc_method_info3 const simpleAddMethodInfo = {
        /* .methodName     = */ "sample.add",
        /* .methodFunction = */ &sample_add,
    };
    xmlrpc_registry_add_method3(&env, registryP, &simpleAddMethodInfo);
    if (env.fault_occurred) {
        printf("xmlrpc_registry_add_method3() failed.  %s\n",
               env.fault_string);
        exit(1);
    }

    /* register nvc0_create */
	xmlrpc_registry_add_method(&env, registryP, NULL, "nvc0_create",
			&rpc_nvc0_create, NULL);
    if (env.fault_occurred) {
        printf("xmlrpc_registry_add_method() failed.  %s\n",
               env.fault_string);
        exit(1);
    }

    /* register nvc0_screen_get_param */
	xmlrpc_registry_add_method(&env, registryP, NULL, "nvc0_screen_get_param",
			&rpc_nvc0_screen_get_param, NULL);
    if (env.fault_occurred) {
        printf("xmlrpc_registry_add_method() failed.  %s\n",
               env.fault_string);
        exit(1);
    }

    serverparm.config_file_name = NULL;   /* Select the modern normal API */
    serverparm.registryP        = registryP;
    serverparm.port_number      = atoi(argv[1]);
    serverparm.log_file_name    = "/tmp/xmlrpc_log";

    printf("Running XML-RPC server...\n");

    xmlrpc_server_abyss(&env, &serverparm, XMLRPC_APSIZE(log_file_name));
    if (env.fault_occurred) {
        printf("xmlrpc_server_abyss() failed.  %s\n", env.fault_string);
        exit(1);
    }
    /* xmlrpc_server_abyss() never returns unless it fails */

    return 0;
}
