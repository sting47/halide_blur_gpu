#include <stdio.h>
#include "gray_fusion.h"
#include "static_image.h"
#include "image_io.h"
#include <sys/time.h>

int main(int argc, char **argv) {
    if (argc < 6) {
        printf("Usage: ./process input.png levels alpha beta output.png\n"
               "e.g.: ./process input.png 8 1 1 output.png\n");
        return 0;
    }

    //Image<uint16_t> input = load<uint16_t>(argv[1]);
    Image<uint16_t> input(1536, 2560);
    for(int y=0; y<input.height(); y++){
	for(int x=0; x<input.width(); x++){
	    input(x, y) = rand() & 0xfff;
	}
    }

    int levels = atoi(argv[2]);
    float alpha = atof(argv[3]), beta = atof(argv[4]);
    Image<uint16_t> output(input.width(), input.height());

    // Timing code
    timeval t1, t2;
    unsigned int bestT = 0xffffffff;
    /*for (int i = 0; i < 5; i++) {
      gettimeofday(&t1, NULL);
      gray_fusion(levels, alpha/(levels-1), beta, input, output);
      gettimeofday(&t2, NULL);
      unsigned int t = (t2.tv_sec - t1.tv_sec) * 1000000 + (t2.tv_usec - t1.tv_usec);
      if (t < bestT) bestT = t;
    }
    printf("%u\n", bestT);*/


    gray_fusion(levels, alpha/(levels-1), beta, input, output);

    save(output, argv[5]);

    return 0;
}
