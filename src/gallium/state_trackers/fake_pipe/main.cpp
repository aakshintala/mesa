#include <iostream>
#include <CL/cl.h>
#include <memory>

#include "rpc.h"

#define START_SERVER 0

class RemoteOpenCLServiceImpl final : public RemoteOpenCL::Service {
    Status RemoteExec(ServerContext* context, const Call* call,
        Response* response) override
    {
        response->set_id(call->id());

        switch (call->id()) {
            default:
                break;
        }

        return Status::OK;
    }
};

int main()
{
    std::string server_address("0.0.0.0:88888");
    RemoteOpenCLServiceImpl service;
    ServerBuilder builder;

    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> rpc_server(builder.BuildAndStart());

    cl_platform_id platform;
    cl_device_id device;
    cl_int ret;
    ret = clGetPlatformIDs(1, &platform, NULL);
    ret = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 1, &device, NULL);
    ret = clGetDeviceInfo(device, START_SERVER, 54321, (void *)rpc_server.get(), NULL);

    return 0;
}
