echo "Layer m=2"
for ((l=1;l<=512;l*=2));
do
	echo "Item Size = $l "
	echo "================================================="
	for ((t=1;t<=512;t*=2));
	do
		echo "Group Size = $t"
		HL_TARGET=host-opencl-debug GROUPSIZE=$t ITEMSIZE=$l LAYER=4 make;
		HL_GPU_DEVICE=0 make out.png;
		make clean
	done
	echo "================================================="
done
