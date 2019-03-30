#pragma once

#include <CL/cl.hpp>
#include <iostream>

#define NVIDIA 0
#define INTEL 1

cl::Device getDefaultDevice();
cl::Device getDevice(int id);
std::string loadKernel(std::string Path);
cl::Program getProgram(std::string path, cl::Context& context, cl::Device& device);
std::vector<int> genArray(long size, int factor);
