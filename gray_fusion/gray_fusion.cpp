#include <Halide.h>
using namespace Halide;

Var x, y;

int main(int argc, char **argv) {

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

    // Convert to floating point
    Func floating;
    floating(x, y, c) = cast<float>(input(x, y, c)) / 65535.0f;

    // Set a boundary condition
    Func clamped;
    clamped(x, y, c) = floating(clamp(x, 0, input.width()-1), clamp(y, 0, input.height()-1), c);
#endif

    // Get the luminance channel
    Func gray;
    gray(x, y) = 0.299f * input(x, y, 0) + 0.587f * input(x, y, 1) + 0.114f * input(x, y, 2);

    // Reintroduce color (Connelly: use eps to avoid scaling up noise w/ apollo3.png input)
    Func color;

    // Get the luminance channel second time
    Func gray2;
    gray2(x, y) = 0.299f * gray(x, y) + 0.587f * gray(x, y) + 0.114f * gray(x, y);

    // float eps = 0.01f;
    //color(x, y, c) = 0.0;
    //color(x, y, 0) = gray2(x, y);
    //color(x, y, 1) = gray2(x, y);
    //color(x, y, 2) = gray2(x, y);

    Func output("gray_fusion");
    // Convert back to 16-bit
    output(x, y) = cast<uint16_t>(clamp(gray2(x, y), 0.0f, 1.0f) * 65535.0f);



    /* THE SCHEDULE */
    //remap.compute_root();

    Target target = get_target_from_environment();

    gray.compute_at(gray2, Var::gpu_blocks());
    //floating.compute_root();
    //clamped.compute_root();
    //color.compute_root().gpu_blocks(x, y);
    //gray.compute_root().gpu_blocks(x, y);
    gray2.compute_root().gpu_blocks(x, y);
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

    output.compile_to_file("gray_fusion", levels, alpha, beta, input, target);

    return 0;
}

