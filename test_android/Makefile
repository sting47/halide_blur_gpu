LAYER?=2
GROUPSIZE?=8
ITEMSIZE?=2

CXX=arm-linux-androideabi-g++
CXXX=g++
EPNGFLAGS=-L/home/sting47/Library/my-android-toolchain/include/lib/ -lpng16 -I/home/sting47/Library/my-android-toolchain/include/include

include /home/sting47/Halide/apps/support/Makefile.inc
#OPENCL_LDFLAGS=-I /opt/intel/intel-opencl-1.2-4.6.0.92/opencl-1.2-sdk-4.6.0.92/include -L /usr/lib/x86_64-linux-gnu/libOpenCL.so -lOpenCL
all: process

blur_fusion: /home/sting47/Halide/ blur_fusion.cpp
	@$(CXXX) blur_fusion.cpp -g -I /home/sting47/Halide/include -L /home/sting47/Halide/bin -lHalide -o blur_fusion -lpthread -ldl -lz -fPIC $(LDFLAGS)

blur_fusion.o: blur_fusion
	@HL_TARGET=arm-32-android ./blur_fusion $(LAYER) $(GROUPSIZE) $(ITEMSIZE)

process: process.cpp blur_fusion.o
	$(CXX) -I/home/sting47/Halide/apps/support -Wall -O3 process.cpp blur_fusion.o -o process -lm -llog -landroid -fPIC $(EPNGFLAGS) $(CUDA_LDFLAGS) $(OPENCL_LDFLAGS) $(OPENGL_LDFLAGS)

out.png: process
	./process rgb.png 8 1 1 out.png

clean:
	@rm -f process blur_fusion.o blur_fusion out.png
