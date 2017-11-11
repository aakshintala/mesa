#ifndef __WRAP_H__
#define __WRAP_H__

#ifdef __cplusplus
extern "C" {
#endif

void init_rpc_service(void);

/* handlers */
void measure_start(void);
void measure_stop(void);

void rpc_sync_start(const char *);
void rpc_sync_end(const char *);

void rpc_sync_start_x(void);
void rpc_sync_end_x(void);

#ifdef __cplusplus
}
#endif

#endif
