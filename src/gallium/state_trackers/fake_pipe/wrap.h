#ifndef __WRAP_H__
#define __WRAP_H__

#ifdef __cplusplus
extern "C" {
#endif

void init_rpc_service(void);
//__attribute__((destructor)) void cleanup(void);

/* handlers */
void rpc_nvc0_create(void *priv, unsigned ctxflags);
void rpc_nvc0_screen_get_param(int param);
void rpc_sync(const char *);

void hello(void);

#ifdef __cplusplus
}
#endif

#endif
