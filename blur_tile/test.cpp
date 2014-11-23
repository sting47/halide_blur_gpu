#include <emmintrin.h>
#include <math.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include "static_image.h"
#include "image_io.h"
#include "blur_tile.h"
#include "blur_root.h"
#include "blur_MergeTile.h"
using namespace std;
//#define cimg_display 0
//#include "CImg.h"
//using namespace cimg_library;

timeval t1, t2;
#define begin_timing gettimeofday(&t1, NULL); for (int i = 0; i < 30; i++) {
#define end_timing } gettimeofday(&t2, NULL);

// typedef CImg<uint16_t> Image;


/*extern "C" {
#include "blur_root.h"
#include "blur_tile.h"
#include "blur_MergeTile.h"
}*/

/*Image<uint16_t> blur_root(Image<uint16_t> in) {
    Image<uint16_t> out(in.width(),in.height());

    // Call it once to initialize the halide runtime stuff
    blur_root(in, out);

    begin_timing;

    // Compute the same region of the output as blur_fast (i.e., we're
    // still being sloppy with boundary conditions)
    blur_root(in, out);

    end_timing;

    return out;
}*/

/*Image<uint16_t> blur_tile(Image<uint16_t> in) {
    Image<uint16_t> out(in.width(),in.height());
    // Call it once to initialize the halide runtime stuff
    blur_tile(in, out);

    begin_timing;

    // Compute the same region of the output as blur_fast (i.e., we're
    // still being sloppy with boundary conditions)
    blur_tile(in, out);

    end_timing;

    return out;
}*/

/*Image<uint16_t> blur_MergeTile(Image<uint16_t> in) {
    Image<uint16_t> out(in.width(),in.height());
    // Call it once to initialize the halide runtime stuff
    blur_MergeTile(in, out);

    begin_timing;

    // Compute the same region of the output as blur_fast (i.e., we're
    // still being sloppy with boundary conditions)
    blur_MergeTile(in, out);

    end_timing;

    return out;
}*/

int main(int argc, char **argv) {
    if (argc <2){
            cout<<"Usage : test inputSize"<<endl;
            return 0;
    }
    int inputSize=atoi(argv[1]);
    Image<uint16_t> input(inputSize,inputSize);
    //Image<uint16_t> input = load<uint16_t>(argv[1]);

    for (int y = 0; y < input.height() ; y++){
    	 for (int x = 0; x < input.width();x++){
	        input(x,y) = rand() & 0xfff;
        }
    }


    //Image<uint16_t> Img_blur_root = blur_root(input);
    #if 1
    Image<uint16_t> Img_blur_root(input.width(),input.height());
    begin_timing;
    blur_root(input, Img_blur_root);
    end_timing;
    float root_time = (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1000000.0f;
    save(Img_blur_root, "root.png");
    #endif

    //Image<uint16_t> Img_blur_tile=blur_tile(input);
    Image<uint16_t> Img_blur_tile(input.width(),input.height());
    begin_timing;
    blur_tile(input, Img_blur_tile);
    end_timing;
    float tile_time = (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 10000003.0f;
    save(Img_blur_tile, "tile.png");
    
    //Image<uint16_t> Img_blur_MergeTile=blur_MergeTile(input);
    #if 1
    Image<uint16_t> Img_blur_MergeTile(input.width(),input.height());
    begin_timing;
    blur_MergeTile(input, Img_blur_MergeTile);
    end_timing;
    float MergeTile_time = (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1000000.0f;
    save(Img_blur_MergeTile, "MT.png");
    #endif
    
    printf("root time      : %f \n", root_time);
    printf("tile time      : %f \n", tile_time);
    printf("Merge Tile time: %f \n", MergeTile_time);
#if 0
  for (int y =0; y < input.height() - 64; y++) {
        for (int x = 0; x < input.width() - 64; x++) {
            if (Img_blur_root(x, y) != Img_blur_tile(x, y) ||Img_blur_root(x, y) != Img_blur_MergeTile(x, y) ){
                printf("difference at (%d,%d): %d %d %d \n", x, y, Img_blur_root(x, y), Img_blur_tile(x, y),Img_blur_MergeTile(x,y));
            }
        }
    }
#endif
#if 0
    for(int y=0; y<input.height()-64; y++)
        for(int x=0; x<input.width()-64; x++)
            printf("difference at (%d,%d): %d\n", x, y, Img_blur_tile(x, y));
#endif

    return 0;
}
