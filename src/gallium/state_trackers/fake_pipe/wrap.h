#ifndef __WRAP_H__
#define __WRAP_H__

#ifdef __cplusplus
extern "C" {
#endif

void init_rpc_service(void);
void clean_on_exit(void);
//__attribute__((destructor)) void cleanup(void);

/* handlers */
void rpc_sync(const char *);
void rpc_sync_start(void);
void rpc_sync_end(void);

void hello(void);

#ifdef __cplusplus
}
#endif

#endif
