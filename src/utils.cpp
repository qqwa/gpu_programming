#include "utils.h"
#include <fstream>
#include <sstream>

cl::Device getDefaultDevice() {
    std::vector<cl::Platform> all_platforms;
    cl::Platform::get(&all_platforms);
    if(all_platforms.size()==0){
        std::cout<<" No platforms found. Check OpenCL installation!\n";
        exit(1);
    }
    cl::Platform default_platform = all_platforms[0];
    std::cout << "Using platform: "<<default_platform.getInfo<CL_PLATFORM_NAME>()<<"\n";

    std::vector<cl::Device> all_devices;
    default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
    if(all_devices.size()==0){
        std::cout<<" No devices found. Check OpenCL installation!\n";
        exit(1);
    }
    cl::Device default_device=all_devices[0];
    std::cout<< "Using device: "<<default_device.getInfo<CL_DEVICE_NAME>()<<"\n";
    return default_device;
}


cl::Device getDevice(int id) {
    std::vector<cl::Platform> all_platforms;
    cl::Platform::get(&all_platforms);
    if(all_platforms.size()==0){
        std::cout<<" No platforms found. Check OpenCL installation!\n";
        exit(1);
    }
    cl::Platform default_platform = all_platforms[id];
    std::cout << "Using platform: "<<default_platform.getInfo<CL_PLATFORM_NAME>()<<"\n";

    std::vector<cl::Device> all_devices;
    default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
    if(all_devices.size()==0){
        std::cout<<" No devices found. Check OpenCL installation!\n";
        exit(1);
    }
    cl::Device default_device=all_devices[0];
    std::cout<< "Using device: "<<default_device.getInfo<CL_DEVICE_NAME>()<<"\n";
    return default_device;
}


std::string loadKernel(std::string path) {
    std::ifstream inFile;
    inFile.open("../kernels/" + path);//open the input file

    std::stringstream strStream;
    strStream << inFile.rdbuf();//read the file
    std::string kernel_code = strStream.str();//str holds the content of the file

//    std::cout << "KERNEL(" << kernel_code.length() << "):" << kernel_code << std::endl;



    return kernel_code;
}


cl::Program getProgram(std::string path, cl::Context& context, cl::Device& device) {
    std::string kernel_code = loadKernel(path);

    cl::Program::Sources sources;
    sources.push_back({kernel_code.c_str(), kernel_code.length()});

    cl::Program program(context,sources);
    if(program.build({device})!=CL_SUCCESS){
        std::cout<<" Error building: "<<program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device)<< std::endl;
        exit(1);
    }

    return program;
}

std::vector<int> genArray(long size, int factor) {
    std::vector<int> result(size);
    for(long i=0; i < size; ++i) {
        result.data()[i] = (i*factor);
    }
    return result;
}
