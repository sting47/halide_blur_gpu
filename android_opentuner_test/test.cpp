#include <Halide.h>
using namespace Halide;

#define AUTOTUNE_HOOK(x)
#define BASELINE_HOOK(x)

int main(int argc, char **argv) {

    ImageParam in_img(UInt(16), 2);
    //Func blur_x("blur_x"), blur_y("blur_y");
    Var x("x"), y("y");//, xi("xi"), yi("yi");

    //int tileSize=4;
    //int m=2;

    Func input;
    input(x,y) = in_img(clamp(x, 1, in_img.width()-1),
                        clamp(y, 1, in_img.height())-1);

    //Func blur[2];
    /*for(int i=0;i<=m-1;i++){
        blur[i] = Func("blur");
    }*/
    //blur[0] = Func("blur");
    //blur[1] = Func("blur");
    Func blur1("blur1");
    Func blur2("blur2");
    Func blur3("blur3");
    Func blur4("blur4");
    Func blur5("blur5");
    Func blur6("blur6");
    Func blur7("blur7");
    Func blur8("blur8");

    //blur[0](x, y) = (input(x-1, y) + input(x, y) + input(x+1, y)) / 3;
    //for(int i=1; i<=1; i++){
    //blur[1](x, y) = (blur[0](x, y) + blur[0](x+1, y) + blur[0](x+2, y)) / 3;
    blur1(x, y) = (input(x-1, y) + input(x, y) + input(x+1, y)) / 3;
    blur2(x, y) = (blur1(x-1, y) + blur1(x, y) + blur1(x+1, y)) / 3;
    blur3(x, y) = (blur2(x-1, y) + blur2(x, y) + blur2(x+1, y)) / 3;
    blur4(x, y) = (blur3(x-1, y) + blur3(x, y) + blur3(x+1, y)) / 3;
    blur5(x, y) = (blur4(x-1, y) + blur4(x, y) + blur4(x+1, y)) / 3;
    blur6(x, y) = (blur5(x-1, y) + blur5(x, y) + blur5(x+1, y)) / 3;
    blur7(x, y) = (blur6(x-1, y) + blur6(x, y) + blur6(x+1, y)) / 3;
    blur8(x, y) = (blur7(x-1, y) + blur7(x, y) + blur7(x+1, y)) / 3;


#if 0
    // The algorithm
    blur_x(x, y) = (input(x, y) + input(x+1, y) + input(x+2, y))/3;
    blur_y(x, y) = (blur_x(x, y) + blur_x(x, y+1) + blur_x(x, y+2))/3;
#endif

    AUTOTUNE_HOOK(blur8);

#if 0
    // How to schedule it
    blur_y.split(y, y, yi, 8).parallel(y).vectorize(x, 8);
    blur_x.store_at(blur_y, y).compute_at(blur_y, yi).vectorize(x, 8);  
#endif
    {
        std::map<std::string, Halide::Internal::Function> funcs = Halide::Internal::find_transitive_calls((blur8).function());
        Halide::Var _x0, _y3, _x4, _x6, _y7, _x8, _x10, _x14, _y15;
Halide::Func(funcs["blur1"])
.split(x, x, _x0, 32)
.reorder(_x0, y, x)
.reorder_storage(x, y)
.vectorize(_x0, 4)
.compute_at(Halide::Func(funcs["blur4"]), y)
;
Halide::Func(funcs["blur2"])
.split(y, y, _y3, 4)
.reorder(_y3, y, x)
.reorder_storage(x, y)
.compute_at(Halide::Func(funcs["blur4"]), x)
;
Halide::Func(funcs["blur3"])
.split(x, x, _x4, 32)
.reorder(_x4, x, y)
.reorder_storage(y, x)
.vectorize(_x4, 8)
.compute_at(Halide::Func(funcs["blur4"]), _y7)
;
Halide::Func(funcs["blur4"])
.split(x, x, _x6, 64)
.split(y, y, _y7, 4)
.reorder(_x6, _y7, x, y)
.reorder_storage(x, y)
.vectorize(_x6, 8)
.parallel(y)
.compute_root()
;
Halide::Func(funcs["blur5"])
.split(x, x, _x8, 64)
.reorder(_x8, x, y)
.reorder_storage(x, y)
.vectorize(_x8, 8)
.compute_at(Halide::Func(funcs["blur8"]), y)
;
Halide::Func(funcs["blur6"])
.split(x, x, _x10, 64)
.reorder(_x10, x, y)
.reorder_storage(x, y)
.vectorize(_x10, 8)
.compute_at(Halide::Func(funcs["blur7"]), y)
;
Halide::Func(funcs["blur7"])
.reorder(x, y)
.reorder_storage(y, x)
.vectorize(x, 8)
.compute_at(Halide::Func(funcs["blur8"]), x)
;
Halide::Func(funcs["blur8"])
.split(x, x, _x14, 32)
.split(y, y, _y15, 4)
.reorder(_x14, x, _y15, y)
.reorder_storage(y, x)
.vectorize(_x14, 8)
.parallel(y)
.compute_root()
;
    }

    BASELINE_HOOK(blur8);

    blur8.compile_to_file("blur_fusion", in_img); 

    return 0;
}
