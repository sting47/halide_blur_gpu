LOG_DIR=log
mkdir -p $LOG_DIR


function run(){
	for ((l=1;l<=32;l*=2));
	do
		echo "Layer m=$l "	>> $1
		echo "=================================================" >> $1
		for ((t=16;t<=1024;t*=2));
		do
			echo "Tile Size t=$t" >> $1
			TILESIZE=$t  LAYER=$l make -f Makefile2;
			make -f Makefile2 run LOG_FILE=$1 >> $1
	done
		echo "=================================================" >> $1
		done
}

#just run normal schedule first
echo 'running normal...'
rm -f $LOG_DIR/log
run $LOG_DIR/log


#test the case of parallel at x direction
echo 'running parallel(x) ...'
sed -ie 's/parallel(y)/parallel(x)/g' blur_MergeTile.cpp
rm -f $LOG_DIR/log_parallel_x
run $LOG_DIR/log_parallel_x


#test the case of vectorize at y direction
#but restor parallel(y) first
echo 'running vectorize(y,4) ...'
sed -ie 's/parallel(x)/parallel(y)/g' blur_MergeTile.cpp
sed -ie 's/vectorize(x,4)/vectorize(y,4)/g' blur_MergeTile.cpp
rm -f $LOG_DIR/log_vectorize_y
run $LOG_DIR/log_vectorize_y

#test both parallel(x) and vectorize(y,4) (at this point, the vectorize direction is already changed to y)
echo 'running parallel(x) + vectorize(y,4) ...'
sed -ie 's/parallel(y)/parallel(x)/g' blur_MergeTile.cpp
rm -f $LOG_DIR/'log_both_parallel_and_vectorize'
run $LOG_DIR/'log_both_parallel_and_vectorize'

#restore to normal schedule in the end
echo 'restore code back to normal schedule '
sed -ie 's/parallel(x)/parallel(y)/g' blur_MergeTile.cpp
sed -ie 's/vectorize(y,4)/vectorize(x,4)/g' blur_MergeTile.cpp





#
# information - normal schedule is as following
#
# blur[m].tile(x,y,xi,yi,tileSize,tileSize).parallel(y).vectorize(xi,4);
# for(int i=1;i<m;i++){
# 	blur[i].compute_at(blur[m],x).vectorize(x,4);
# }



