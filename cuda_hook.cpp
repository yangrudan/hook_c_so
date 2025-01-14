#include <dlfcn.h>
#include <cuda_runtime.h>
#include <iostream>

// 获取原始的 cudaLaunchKernel 函数指针
//typedef cudaError_t (*cudaLaunchKernel_t)(const void* func, dim3 gridDim, dim3 blockDim, void** args, size_t sharedMem, cudaStream_t stream);

//static cudaLaunchKernel_t original_cudaLaunchKernel = nullptr;
cudaError_t (*original_cudaLaunchKernel)(const void* func, dim3 gridDim, dim3 blockDim, void** args, size_t sharedMem, cudaStream_t stream) = NULL;
#define CUDA_RUNTIME_API_LIBNAME "libcuda.so"

// 初始化时获取原始的 cudaLaunchKernel 函数指针
// __host__​cudaError_t cudaLaunchKernel ( const void* func, dim3 gridDim, dim3 blockDim, void** args, size_t sharedMem, cudaStream_t stream )
// Launches a device function.
__attribute__((constructor))
void init() {
    void* cudaLibHandle = NULL;
    cudaLibHandle = dlopen(CUDA_RUNTIME_API_LIBNAME, RTLD_NOW);
    if (!cudaLibHandle) {
	    fprintf(stderr, "%s\n", dlerror());
	    return;
    }
    //original_cudaLaunchKernel = (cudaLaunchKernel_t)dlsym(cudaLibHandle, "cudaLaunchKernel");
    original_cudaLaunchKernel = (cudaError_t(*)(const void* func, dim3 gridDim, dim3 blockDim, void** args, size_t sharedMem, cudaStream_t stream))dlsym(cudaLibHandle, "cudaLaunchKernel");
    if (!original_cudaLaunchKernel) {
        std::cerr << "Failed to find cudaLaunchKernel in RTLD_NEXT" << std::endl;
        exit(1);  // 退出程序，避免段错误
    } else {
        std::cout << "Successfully hooked cudaLaunchKernel!" << std::endl;
    }
}


// Hooked 版本的 cudaLaunchKernel
extern "C" cudaError_t cudaLaunchKernel(const void* func, dim3 gridDim, dim3 blockDim, void** args, size_t sharedMem, cudaStream_t stream) {
    // 在这里可以添加自定义逻辑
    std::cout << "Hooked cudaLaunchKernel!" << std::endl;

    // 调用原始的 cudaLaunchKernel 并返回其结果
    if (original_cudaLaunchKernel) {
        return original_cudaLaunchKernel(func, gridDim, blockDim, args, sharedMem, stream);
    }

    // 如果原始函数指针为空，返回错误
    return cudaErrorUnknown;
}
