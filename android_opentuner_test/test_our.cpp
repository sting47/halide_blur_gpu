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
	blur1.compute_at(blur4, y);
	blur2.compute_at(blur4, y);
	blur3.compute_at(blur4, y);
	blur4.compute_root();
	blur5.compute_at(blur8, y);
	blur6.compute_at(blur8, y);
	blur7.compute_at(blur8, y);
	blur8.compute_root();
    }

    BASELINE_HOOK(blur8);

    blur8.compile_to_file("blur_fusion", in_img); 

    return 0;
}
