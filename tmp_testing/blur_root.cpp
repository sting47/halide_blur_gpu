#include <Halide.h>
#include <iostream>
#include <sstream>
using namespace Halide;
using namespace std;
int main(int argc, char **argv) {
    if (argc<2){
    	cout<<"Usage : blur_root Layers"<<endl;
    }
    int m=atoi(argv[1]);
    ImageParam input(UInt(16), 2);
	Func blur[m+1];
	/**
        Assign Func Name to identify.
	*/
	for(int i=0;i<=m;i++){
	    stringstream ss;
          ss<<i;
        blur[i]=Func("blur_root"+ss.str());
	}
    Var x("x"),y("y");
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
    /**
    Set Boundary
    */
	Func clamped("clamped");
    clamped(x,y)=input(clamp(x,0,input.width()-1),clamp(y,0,input.height()-1));
/** The algorithm
**
**/
	blur[0](x,y)=input(clamp(x,0,input.width()-1),clamp(y,0,input.height()-1));
	for(int i=1;i<=m;i++){
		blur[i](x,y)=sum(tent(r.x, r.y) * blur[i-1](x + r.x - 1, y + r.y - 1));
    }
    /** How to schedule it
    */
//    blur[0].compute_root();
    Target target = get_target_from_environment();
    if (target.has_gpu_feature()) {
//		Var xo,xi,xx,xy;
//		blur_x.split(x,xo,xi,16).split(xi,xx,xy,16);
//		blur_x.gpu_blocks(xo).gpu_threads(xx);
	   }
     else{
			for(int i=1;i<=m;i++){
				blur[i].compute_root().parallel(y,4).vectorize(x,4);
				//blur[i].compute_root().vectorize(x,4);
			}
		}
    blur[m].compile_to_file("blur_root", input);

    return 0;
}


