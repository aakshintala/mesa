#ifndef __RPC_H__
#define __RPC_H__

#include <grpc++/grpc++.h>

#include "api.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;

using grpc::Status;
using libocl::Call;
using libocl::Response;
using libocl::RemoteOpenCL;

inline void chk_status(Status &status) {
    if (!status.ok()) {
        std::cout << status.error_code() << ": " << status.error_message()
                  << std::endl;
    }
}

#define send_rpc_blocking( response_name ) \
    Response response_name; \
    grpc::ClientContext _context; \
    Status status = _stub->RemoteExec(&_context, call, &response_name); \
    chk_status(status)

extern std::unique_ptr<RemoteOpenCL::Stub> _stub;

extern cl_platform_id *_rpc_platforms;
extern cl_uint *_rpc_num_platforms;
extern cl_int _rpc_ret;


#endif
