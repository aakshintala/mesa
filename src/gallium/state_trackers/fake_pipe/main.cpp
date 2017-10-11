#include <iostream>
#include <CL/cl.h>

#define START_SERVER 0

int main()
{
    cl_platform_id platform;
    cl_device_id device;
    cl_int ret;
    ret = clGetPlatformIDs(1, &platform, NULL);
    ret = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 1, &device, NULL);
    ret = clGetDeviceInfo(device, START_SERVER, 54321, NULL, NULL);

    return 0;
}
