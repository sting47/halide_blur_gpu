#include <Halide.h>
using namespace Halide;

Var x, y;

int main(int argc, char **argv) {

    int m = atoi(argv[1]);
    int groupsize = atoi(argv[2]);
    int itemsize = atoi(argv[3]);
    /* THE ALGORITHM */

    // Number of pyramid levels
    const int J = 8;

    // number of intensity levels
    Param<int> levels;
    // Parameters controlling the filter
    Param<float> alpha, beta;
    // Takes a 16-bit input
    ImageParam input(UInt(16), 3);

    // loop variables
    Var c, k;
#if 0
    // Make the remapping function as a lookup table.
    Func remap;
    Expr fx = cast<float>(x) / 256.0f;
    remap(x) = alpha*fx*exp(-fx*fx/2.0f);
#endif
    // Convert to floating point
    Func floating;
    floating(x, y, c) = cast<float>(input(x, y, c)) / 65535.0f;

    // Set a boundary condition
    Func clamped;
    clamped(x, y, c) = floating(clamp(x, 0, input.width()-1), clamp(y, 0, input.height()-1), c);

    // Get the luminance channel
    Func blur[m+1];
    blur[0](x, y, c) = (clamped(x-1, y, c) + clamped(x, y, c) + clamped(x+1, y, c)) / 3;

    // Reintroduce color (Connelly: use eps to avoid scaling up noise w/ apollo3.png input)
    //Func color;

    // Get the luminance channel second time
    for(int i=1; i<m+1; i++)
	blur[i](x, y, c) = (blur[i-1](x-1, y, c) + blur[i-1](x, y, c) + blur[i-1](x+1, y, c)) / 3;

    // float eps = 0.01f;
    //color(x, y, c) = 0.0;
    //color(x, y, 0) = gray2(x, y);
    //color(x, y, 1) = gray2(x, y);
    //color(x, y, 2) = gray2(x, y);

    Func output("blur_fusion");
    // Convert back to 16-bit
    output(x, y, c) = cast<uint16_t>(clamp(blur[m](x, y, c), 0.0f, 1.0f) * 65535.0f);



    /* THE SCHEDULE */
    //remap.compute_root();

    Target target = get_target_from_environment();

    for(int i=0; i<m; i++){
	//Var xt, yt;
	//blur[i].compute_root();//.gpu_blocks(x, y, c);
	blur[i].compute_at(blur[m], Var::gpu_blocks());
	//blur[i].compute_root().tile(x, y, xt, yt, groupsize, groupsize);
	//blur[i].compute_root().gpu_tile(x, y, itemsize, itemsize, GPU_Default);
    }
    //floating.compute_root();
    clamped.compute_root();
    //blur[m].compute_root().gpu_blocks(x, y, c);
    //color.compute_root().gpu_blocks(x, y);
    //gray.compute_root().gpu_blocks(x, y);
    //blur[1].compute_root().gpu_blocks(x, y, c);

    Var xi, yi;
    blur[m].compute_root().tile(x, y, xi, yi, groupsize, groupsize);
    blur[m].compute_root().gpu_tile(x, y, itemsize, itemsize, GPU_Default);
#if 0
    if (target.has_gpu_feature()) {
        // gpu schedule
        output.compute_root().gpu_tile(x, y, 16, 8, GPU_Default);
        for (int j = 0; j < J; j++) {
            int blockw = 16, blockh = 8;
            if (j > 3) {
                blockw = 2;
                blockh = 2;
            }
            if (j > 0) inGPyramid[j].compute_root().gpu_tile(x, y, blockw, blockh, GPU_Default);
            if (j > 0) gPyramid[j].compute_root().reorder(k, x, y).gpu_tile(x, y, blockw, blockh, GPU_Default);
            outGPyramid[j].compute_root().gpu_tile(x, y, blockw, blockh, GPU_Default);
        }
    } else {
        // cpu schedule
        Var yi;
        output.parallel(y, 4).vectorize(x, 8);
        gray.compute_root().parallel(y, 4).vectorize(x, 8);
        for (int j = 0; j < 4; j++) {
            if (j > 0) inGPyramid[j].compute_root().parallel(y, 4).vectorize(x, 8);
            if (j > 0) gPyramid[j].compute_root().parallel(y, 4).vectorize(x, 8);
            outGPyramid[j].compute_root().parallel(y, 4).vectorize(x, 8);
        }
        for (int j = 4; j < J; j++) {
            inGPyramid[j].compute_root().parallel(y);
            gPyramid[j].compute_root().parallel(k);
            outGPyramid[j].compute_root().parallel(y);
        }
    }
#endif

    output.compile_to_file("blur_fusion", levels, alpha, beta, input, target);

    return 0;
}

