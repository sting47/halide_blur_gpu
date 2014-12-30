__kernel 
void blur_1D(__global float * output,
         __global float * input, 
         const    uint    width)
{
    /* get global indices of the element */
    uint globalIdx = get_global_id(0);
    uint globalIdy = get_global_id(1);
    
    uint idx = globalIdy * width + globalIdx;

    output[idx] = (input[idx] + input[idx+1] + input[idx+2])/3;     
}
