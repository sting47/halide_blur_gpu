LAYER?=2
GROUPSIZE?=4
ITEMSIZE?=4

include /home/sting47/Halide/apps/support/Makefile.inc
#OPENCL_LDFLAGS=-I /opt/intel/intel-opencl-1.2-4.6.0.92/opencl-1.2-sdk-4.6.0.92/include -L /usr/lib/x86_64-linux-gnu/libOpenCL.so -lOpenCL
all: process

blur_fusion: /home/sting47/Halide/ blur_fusion.cpp
	@$(CXX) blur_fusion.cpp -g -I /home/sting47/Halide/include -L /home/sting47/Halide/bin -lHalide -o blur_fusion -lpthread -ldl -lz $(LDFLAGS)

blur_fusion.o: blur_fusion
	@./blur_fusion $(LAYER) $(GROUPSIZE) $(ITEMSIZE)

process: process.cpp blur_fusion.o
	@$(CXX) -I/home/sting47/Halide/apps/support -Wall -O3 process.cpp blur_fusion.o -o process -lpthread -ldl $(PNGFLAGS) $(CUDA_LDFLAGS) $(OPENCL_LDFLAGS) $(OPENGL_LDFLAGS)

out.png: process
	./process rgb.png 8 1 1 out.png

clean:
	@rm -f process blur_fusion.o blur_fusion out.png
