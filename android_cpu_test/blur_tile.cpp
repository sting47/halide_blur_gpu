#include <Halide.h>
#include <iostream>
#include <sstream>
using namespace std;
using namespace Halide;

int main(int argc, char **argv) {
     if (argc <3){
            cout<<"Usage : blur_tile Layer tileSize "<<endl;
            exit(0);
    }
    int tileSize=atoi(argv[2]);
     int m=atoi(argv[1]);
    ImageParam input(UInt(16), 2);

//    Func blur_1("blur_1"),blur_2("blur_2");
    Func blur[m+1];
   

    for(int i=0;i<=m;i++){
        stringstream ss;
        ss<<i;
        blur[i]=Func("blur_tile"+ss.str());
	}
    Var x("x"),y("y");
    /**
        Create Blur Kernel
    */
    Image<uint16_t> tent(3, 3);
    tent(0, 0) = 1;
    tent(0, 1) = 1;
    tent(0, 2) = 1;
    tent(1, 0) = 1;
    tent(1, 1) = 1;
    tent(1, 2) = 1;
    tent(2, 0) = 1;
    tent(2, 1) = 1;
    tent(2, 2) = 1;
    RDom r(tent);
/** The algorithm
**
**/
	blur[0](x,y)=input(clamp(x,0,input.width()-1),clamp(y,0,input.height()-1));
	for(int i=1;i<=m;i++){
        blur[i](x,y)=sum(tent(r.x, r.y) * blur[i-1](x + r.x - 1, y + r.y - 1));
    }
/**     How to schedule it*/
//    blur[0].compute_root();
    Target target = get_target_from_environment();
    if (target.has_gpu_feature()) {
//		Var xo,xi,xx,xy;
//		blur_x.split(x,xo,xi,16).split(xi,xx,xy,16);
//		blur_x.gpu_blocks(xo).gpu_threads(xx);
	   }
     else{
        Var xi,yi;
        for(int i=1;i<=m;i++){
			blur[i].compute_root().tile(x,y,xi,yi,tileSize,tileSize).parallel(y,4).vectorize(xi,4);
			}
		}
    blur[m].compile_to_file("blur_tile", input);
    return 0;
}
