# Makefile for building libcuda_hook.so

# 编译器
NVCC := nvcc
CXX := g++

# 输出文件名
LIB_NAME := libcuda_hook.so

# 源文件
SRC := cuda_hook.cpp

# 编译标志
CXXFLAGS := -fPIC -shared -std=c++11 -I$(CUDA_HOME)/include
LDFLAGS := -L$(CUDA_HOME)/lib64 -lcuda -lcudart


# 默认目标
all: $(LIB_NAME)

# 生成共享库
$(LIB_NAME): $(SRC)
	$(CXX) $(CXXFLAGS) -g -o $@ $< $(LDFLAGS)

# 清理生成的文件
clean:
	rm -f $(LIB_NAME)
