#!/bin/bash

function convert(){

sed -e '/Performance counter/d' \
-e '/^$/d' -e 's/#.*//' \
-e 's/\[.*//' \
-e 's/://g' \
-e 's/,//g' \
-e '/elapsed/d' $1 > result/temp1
#temp1 has more detail information about cache


awk '
	BEGIN {
		ORS=""
	}

	{
	if($2=="time" || $3=="time"){
		
	}		
	if(match($2, "cache")){
		cache_ref=$1
		getline;
		cache_miss=$1

		if(0)	printf "\t%f%%\n",100*cache_miss/cache_ref;
	}
	else if(match($2, "LLC")){
		LLC_load=$1
		getline;
		LLC_load_miss=$1
		getline;
		LLC_store=$1
		getline;
		LLC_store_miss=$1
		getline;
		LLC_prefetch_miss=$1



		printf "\t%f%%\n",100*(LLC_load_miss + LLC_store_miss)/(LLC_load + LLC_store);
	}
	else
		print;

    }' result/temp1 > result/temp2

sed -i 's/=Tile/=\
Tile/g' result/temp2
sed  's/root/\
root/g' result/temp2 > result/converted_$1
}


convert log
convert log_parallel_x
convert log_vectorize_y
convert log_both_parallel_and_vectorize





