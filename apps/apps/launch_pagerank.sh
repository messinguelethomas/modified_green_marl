echo "Nombre de coeur : "
read nbcoeur

result_perf="result_perf_gm.txt"
filename="graph_file.txt"
graph_file=`cat $filename`
for graph_bin in `cat $filename`
do
	for k in `seq 0 $nbcoeur`
	do
		let c=k+1
		for i in `seq 1 2`
		do
			case $graph_bin in
				"data/com-lj_prepared.bin" )
					graph_task_bin="data/com-lj_prepared_1023_tasks.bin";;

				"data/com-lj_prepared_cn_naive.bin" )
					graph_task_bin="data/com-lj_prepared_cn_naive_1022_tasks.bin";;

				"data/com-lj_prepared_numbaco.bin" )
					graph_task_bin="data/com-lj_prepared_numbaco_1022_tasks.bin";;

				"data/com-lj_prepared_rabbit.bin" )
					graph_task_bin="data/com-lj_prepared_rabbit_1023_tasks.bin";;

				"data/com-lj_prepared_gorder.bin" )
					graph_task_bin="data/com-lj_prepared_gorder_1022_tasks.bin";;

				"data/com-orkut_prepared.bin" )
					graph_task_bin="data/com-orkut_prepared_1023_tasks.bin";;

				"data/com-orkut_prepared_cn_naive.bin" )
					graph_task_bin="data/com-orkut_prepared_cn_naive_1022_tasks.bin";;

				"data/com-orkut_prepared_numbaco.bin" )
					graph_task_bin="data/com-orkut_prepared_numbaco_1023_tasks.bin";;

				"data/com-orkut_prepared_rabbit.bin" )
					graph_task_bin="data/com-orkut_prepared_rabbit_1023_tasks.bin";;

				"data/com-orkut_prepared_gorder.bin" )
					graph_task_bin="data/com-orkut_prepared_gorder_1022_tasks.bin";;

			esac
			perf stat -B -e cache-references,cache-misses,L1-dcache-load-misses,L1-dcache-store-misses,L1-dcache-prefetch-misses,LLC-loads,LLC-stores,dTLB-load-misses,dTLB-store-misses,cycles -o $result_perf --append taskset -c 0-$k ./output_cpp/bin/pagerank $graph_bin $graph_task_bin $c 0.000000000001
		done
	done
done
