#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <iostream>
#include <fstream>
#include <string>

__global__ void countWords(char* content, int* totalCount, int size) {
    int id = blockIdx.x * blockDim.x + threadIdx.x;
    if (id < size) {
        if (content[id] == ' ' && (id == 0 || content[id - 1] != ' ')) {
            atomicAdd(totalCount, 1);
        }
    }
}

int main() {
    std::ifstream file("large_file.txt", std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    char* content = new char[size];
    if (!content) {
        std::cerr << "Memory allocation failed!" << std::endl;
        return 1;
    }

    if (!file.read(content, size)) {
        std::cerr << "Error reading file!" << std::endl;
        delete[] content;
        return 1;
    }
    file.close();

    int* totalCount;
    cudaMallocManaged(&totalCount, sizeof(int));
    *totalCount = 0;

    int blockSize = 256;
    int gridSize = (size + blockSize - 1) / blockSize;

    countWords << <gridSize, blockSize >> > (content, totalCount, size);
    cudaDeviceSynchronize();

    std::cout << "Total number of words: " << *totalCount << std::endl;

    cudaFree(totalCount);
    delete[] content;

    return 0;
}