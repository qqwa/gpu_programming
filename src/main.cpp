#include <iostream>
#include <zconf.h>
#include <CL/cl.hpp>

#include "utils.h"

int main() {
    // prepare opencl
//    auto default_device = cl::Device::getDefault();
    auto default_device = getDefaultDevice();
    auto context = cl::Context(default_device);
    auto program = getProgram("simple_add.ocl", context, default_device);
    auto queue = cl::CommandQueue(context, default_device);

    // generate data
    auto A = genArray(100, 1);
    auto B = genArray(100, 1);
    std::vector<int> C(std::min(A.size(), B.size()));

    // create buffers
    auto buffer_A = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int)*A.size(), A.data());
    auto buffer_B = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int)*B.size(), B.data());
    auto buffer_C = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int)*C.size(), C.data());

    // copy generated data to buffers
    queue.enqueueMigrateMemObjects({buffer_A, buffer_B}, 0);

    // run kernel
    auto kernel_add = cl::Kernel(program, "simple_add");
    kernel_add.setArg(0, buffer_A);
    kernel_add.setArg(1, buffer_B);
    kernel_add.setArg(2, buffer_C);
    queue.enqueueNDRangeKernel(kernel_add, cl::NullRange, cl::NDRange(C.size()), cl::NullRange);
    queue.finish();

    // read buffer_C to C
    queue.enqueueMigrateMemObjects({buffer_C}, CL_MIGRATE_MEM_OBJECT_HOST);

    // not sure about this part
    auto smart_pointer = cl::Memory(buffer_C);
    buffer_C = 0;

    // output
    std::cout<<" result: \n    ";
    for(int i=0;i<10;i++){
        std::cout<<C[i]<<" ";
    }
    std::cout << "..." << std::endl;

    std::cout << "Size C: " << C.size() << std::endl;
    smart_pointer = 0;
    sleep(1);
    std::cout << "Size C: " << C.size() << std::endl;

    std::cout << "Buffer B<CL_MEM_SIZE>: " << buffer_B.getInfo<CL_MEM_SIZE>() << std::endl;


    return 0;
}