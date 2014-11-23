LIB_HALIDE=-I /home/sting47/Halide/include/ -L /home/sting47/Halide/bin/ -lHalide
export LD_LIBRARY_PATH=/home/sting47/Halide/bin/
TARGET?=host
DEBUG?=0
outfile?=build/
SIZE?=4096
TILESIZE?=64
LAYER?=1
OPENCL_LDFLAGS=-I /opt/intel/intel-opencl-1.2-4.6.0.92/opencl-1.2-sdk-4.6.0.92/include -L /usr/lib/x86_64-linux-gnu/libOpenCL.so -lOpenCL
all: $(outfile)test

$(outfile)tmp/blur_root: blur_root.cpp
	@$(CXX) blur_root.cpp $(LIB_HALIDE) -o $(outfile)tmp/blur_root -ldl -lpthread -lz $(LDFLAGS)
$(outfile)tmp/blur_tile: blur_tile.cpp
	@$(CXX) blur_tile.cpp $(LIB_HALIDE) -o $(outfile)tmp/blur_tile -ldl -lpthread -lz $(LDFLAGS)
$(outfile)tmp/blur_MergeTile: blur_MergeTile.cpp
	@$(CXX) blur_MergeTile.cpp $(LIB_HALIDE) -o $(outfile)tmp/blur_MergeTile -ldl -lpthread -lz $(LDFLAGS)
$(outfile)tmp/blur_root.o: $(outfile)tmp/blur_root
	@cd $(outfile)tmp/;HL_DEBUG_CODEGEN=$(DEBUG) HL_TAGET=$(TARGET) ./blur_root $(LAYER)
$(outfile)tmp/blur_tile.o: $(outfile)tmp/blur_tile
	@cd $(outfile)tmp/;HL_DEBUG_CODEGEN=$(DEBUG) HL_TARGET=$(TARGET) ./blur_tile $(LAYER) $(TILESIZE)
$(outfile)tmp/blur_MergeTile.o: $(outfile)tmp/blur_MergeTile
	@cd $(outfile)tmp/;HL_DEBUG_CODEGEN=$(DEBUG) HL_TARGET=$(TARGET) ./blur_MergeTile $(LAYER) $(TILESIZE)
$(outfile)test: test.cpp $(outfile)tmp/blur_root.o $(outfile)tmp/blur_tile.o $(outfile)tmp/blur_MergeTile.o
	@$(CXX) -g -msse2 -Wall -O3 -I ../support/ -I $(outfile)tmp/ test.cpp $(outfile)tmp/blur_root.o $(outfile)tmp/blur_tile.o $(outfile)tmp/blur_MergeTile.o  -o $(outfile)test -lpthread -ldl $(PNGFLAGS) $(OPENCL_LDFLAGS) $(OPENGL_LDFLAGS) ;rm $(outfile)tmp/*
run:
	@./build/test $(SIZE)
clean:
	@rm -f $(outfile)test $(outfile)tmp/blur_root.o $(outfile)tmp/blur_tile.o $(outfile)tmp/blur_tile.h $(outfile)tmp/blur_root.h $(outfile)tmp/blur_root $(outfile)tmp/blur_tile $(outfile)tmp/blur_MergeTile
