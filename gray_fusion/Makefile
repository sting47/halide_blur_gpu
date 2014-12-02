include /home/sting47/Halide/apps/support/Makefile.inc
#OPENCL_LDFLAGS=-I /opt/intel/intel-opencl-1.2-4.6.0.92/opencl-1.2-sdk-4.6.0.92/include -L /usr/lib/x86_64-linux-gnu/libOpenCL.so -lOpenCL
all: process

gray_fusion: /home/sting47/Halide/ gray_fusion.cpp
	$(CXX) gray_fusion.cpp -g -I /home/sting47/Halide/include -L /home/sting47/Halide/bin -lHalide -o gray_fusion -lpthread -ldl -lz $(LDFLAGS)

gray_fusion.o: gray_fusion
	./gray_fusion

process: process.cpp gray_fusion.o
	$(CXX) -I/home/sting47/Halide/apps/support -Wall -O3 process.cpp gray_fusion.o -o process -lpthread -ldl $(PNGFLAGS) $(CUDA_LDFLAGS) $(OPENCL_LDFLAGS) $(OPENGL_LDFLAGS)

out.png: process
	./process rgb.png 8 1 1 out.png

clean:
	rm -f process gray_fusion.o gray_fusion out.png
