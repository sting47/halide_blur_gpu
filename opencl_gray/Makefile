INC_AMD_SDKUtil=/home/sting47/AMDAPPSDK-2.9-1/include/SDKUtil
INC_AMD=/home/sting47/AMDAPPSDK-2.9-1/include
LIB_OPENCL=/home/sting47/AMDAPPSDK-2.9-1/lib/x86_64

all:
	g++ blur_1D.cpp -o blur_1D -I $(INC_AMD) -I $(INC_AMD_SDKUtil) -L $(LIB_OPENCL) -lOpenCL
clean:
	rm ./blur_1D
