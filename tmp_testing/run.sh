#!/bin/bash
cd /home/sting47/workspace/halide_blur_gpu/tmp_testing
rm -f log/log
for ((l=8;l<=8;l*=2));
do
	echo "Layer m=$l " >> log/log
	echo "=================================================" >> log/log
	for ((t=16;t<=32;t*=2));
	do
		echo "Tile Size t=$t" >> log/log
		TILESIZE=$t  LAYER=$l make -f Makefile2;
		make -f Makefile2 run >> log/log
	done
	echo "=================================================" >> log/log
done
# cd log
# ./convert_all.sh
# ./convert_all2.sh
# cd result
echo "done"
