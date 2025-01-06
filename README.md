 # 说明
 
 Linux 下使用 dlfcn.h 库进行动态链接库函数拦截（hook), 试图拦截 CUDA 运行时 API 中的 cudaLaunchKernel 函数。cudaLaunchKernel 是 CUDA API 中用于启动设备内核的关键函数。
 
 代码中定义了一个 init 函数，它在程序启动时自动执行，用于获取原始 cudaLaunchKernel 函数的指针。然后，代码提供了一个新的 cudaLaunchKernel 函数实现，这个实现会在每次调用 cudaLaunchKernel 时被执行。


1. 定义原始函数指针：定义了一个指向原始 cudaLaunchKernel 函数的指针 original_cudaLaunchKernel。

2. 初始化函数：使用 __attribute__((constructor)) 属性定义了一个初始化函数 init，它会在程序启动时自动执行。

3. 加载动态链接库：使用 dlopen 函数加载 libcuda.so 动态链接库。

4. 获取原始函数指针：使用 dlsym 函数获取 cudaLaunchKernel 函数的指针，并将其存储在 original_cudaLaunchKernel 指针中。

5. 定义 Hook 函数：定义了一个外部 "C" 风格的 cudaLaunchKernel 函数，这个函数会在每次调用 cudaLaunchKernel 时被执行。在函数中，可以添加自定义逻辑，然后调用原始的 cudaLaunchKernel 函数。

6. 错误处理：如果在加载库或获取函数指针时发生错误，程序会输出错误信息并退出。
