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

pthread_mutex_t client_lock;
pthread_mutex_t server_lock;

static xmlrpc_value* rpc_client_sync(xmlrpc_env* const envP,
    xmlrpc_value* const paramArrayP, void* const user_data)
{
    printf("client_sync: server_unlock\n");
    pthread_mutex_unlock(&server_lock); // server continues pipe execution
    printf("client_sync: client_lock\n");
    pthread_mutex_lock(&client_lock);   // client wait for server

    char *name;
    xmlrpc_decompose_value(envP, paramArrayP, "(s)", &name);
    RETURN_IF_FAULT(envP);
    printf("client: %s\n", name);

    pthread_mutex_unlock(&client_lock); // client continues execution
    return xmlrpc_build_value(envP, "i", 0);
}

static xmlrpc_value* rpc_server_sync_start(xmlrpc_env* const envP,
    xmlrpc_value* const paramArrayP, void* const user_data)
{
    printf("server_sync_start: client_lock\n");
    pthread_mutex_lock(&client_lock); // client wait for server
    printf("server_sync_start: server_lock\n");
    pthread_mutex_lock(&server_lock); // block server pipe execution

    char *name;
    xmlrpc_decompose_value(envP, paramArrayP, "(s)", &name);
    RETURN_IF_FAULT(envP);
    printf("server_start: %s\n", name);

    return xmlrpc_build_value(envP, "i", 0);
}

static xmlrpc_value* rpc_server_sync_end(xmlrpc_env* const envP,
    xmlrpc_value* const paramArrayP, void* const user_data)
{
    printf("server_sync_end: client_unlock\n");
    pthread_mutex_unlock(&client_lock); // client wait for server

    char *name;
    xmlrpc_decompose_value(envP, paramArrayP, "(s)", &name);
    RETURN_IF_FAULT(envP);
    printf("server_end: %s\n", name);

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

    /* register client_sync */
	xmlrpc_registry_add_method(&env, registryP, NULL, "rpc_client_sync",
			&rpc_client_sync, NULL);
    if (env.fault_occurred) {
        printf("xmlrpc_registry_add_method() failed.  %s\n",
               env.fault_string);
        exit(1);
    }

    /* register server_sync_start  */
	xmlrpc_registry_add_method(&env, registryP, NULL, "rpc_server_sync_start",
			&rpc_server_sync_start, NULL);
    if (env.fault_occurred) {
        printf("xmlrpc_registry_add_method() failed.  %s\n",
               env.fault_string);
        exit(1);
    }

    /* register server_sync_end */
	xmlrpc_registry_add_method(&env, registryP, NULL, "rpc_server_sync_end",
			&rpc_server_sync_end, NULL);
    if (env.fault_occurred) {
        printf("xmlrpc_registry_add_method() failed.  %s\n",
               env.fault_string);
        exit(1);
    }

    serverparm.config_file_name = NULL;   /* Select the modern normal API */
    serverparm.registryP        = registryP;
    serverparm.port_number      = atoi(argv[1]);
    serverparm.log_file_name    = "/tmp/xmlrpc_log";

    if (pthread_mutex_init(&client_lock, NULL) != 0) {
        printf("\n client mutex init failed\n");
        return 1;
    }
    if (pthread_mutex_init(&server_lock, NULL) != 0) {
        printf("\n server mutex init failed\n");
        return 1;
    }
    pthread_mutex_lock(&server_lock);

    printf("Running XML-RPC server...\n");

    xmlrpc_server_abyss(&env, &serverparm, XMLRPC_APSIZE(log_file_name));
    if (env.fault_occurred) {
        printf("xmlrpc_server_abyss() failed.  %s\n", env.fault_string);
        exit(1);
    }
    /* xmlrpc_server_abyss() never returns unless it fails */

    return 0;
}
