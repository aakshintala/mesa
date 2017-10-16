#ifndef __WRAP_H__
#define __WRAP_H__

#ifdef __cplusplus
extern "C" {
#endif

void init_rpc_service(void);

/* handlers */
void rpc_sync_start(const char *);
void rpc_sync_end(const char *);

#ifdef __cplusplus
}
#endif

#endif
