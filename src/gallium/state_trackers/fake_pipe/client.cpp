#include <iostream>

#include "rpc.h"
#include "wrap.h"

//std::unique_ptr<RemoteOpenCL::Stub> _stub;

extern "C" {

void hello() {
    std::cout << "hellllo\n";
    auto channel = grpc::CreateChannel(
        "localhost:88888", grpc::InsecureChannelCredentials());
    //_stub = libocl::RemoteOpenCL::NewStub(channel);

    std::cout << "hiiiiii\n";
}

}
