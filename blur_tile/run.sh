for ((l=1;l<=16;l*=2));
do
	echo "Layer m=$l "
	echo "================================================="
	for ((t=16;t<=1024;t*=2));
	do
		echo "Tile Size t=$t"
		HL_TARGET=host-opencl HL_GPU_DEVICE=0 TILESIZE=$t  LAYER=$l make;
		make run;
	done
	echo "================================================="
done
