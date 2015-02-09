#!/bin/bash





function convert(){
	rm -f result/final_$1
	grep root result/$1| awk 'BEGIN{print "===root==="}  {print $3 "\t" $4}' >> result/final_$1|
	grep tile result/$1 | awk 'BEGIN{print "===root tile==="}  {print $3 "\t" $4}' >> result/final_$1 
	grep Merge result/$1 | awk 'BEGIN{print "===merge tile==="}  {print $4 "\t" $5}' >> result/final_$1
	sed -e '/===/d' result/final_$1 | awk '(NR%7==1&&NR!=1){print "";}{print}' | awk '(NR==1){print "Root"}(NR==49){print "Tile"}(NR==97){print "Merge Tile"}{print}' > result/temp_log && mv result/temp_log result/final_$1 
	

}


convert converted_log
convert converted_log_parallel_x
convert converted_log_vectorize_y
convert converted_log_both_parallel_and_vectorize
