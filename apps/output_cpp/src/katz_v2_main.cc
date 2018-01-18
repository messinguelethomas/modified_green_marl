#include "katz_v2.h"
#include "graph_gen.h"
#include "time_manager.h"
#include "gm_graph.h"
#include <limits.h>
#include <sys/time.h>

int main(int argc, char** argv) {
	unsigned long long init_time, work_time;
	//pagerank parameter
	timeval_t t1, t2;
	timezone_t tz;
	char *gm_bin_file_name  = (char*) malloc(sizeof(char)*100);
	char *task_file_name    = (char*) malloc(sizeof(char)*100);
        int random_seed = 1989, nb_thread, nb_node, chunk = 1;
	float beta = 0.5; int32_t max_l;

	if(argc != 5)
        {
                printf("usage: ./katz graph_file_gm.bin file_task_info.bin max_l nb_thread\n");
                return 1;
        }
	strcpy(gm_bin_file_name, argv[1]);
	strcpy(task_file_name,argv[2]);	
	max_l = atoi(argv[3]);
	nb_thread = atoi(argv[4]);
	gm_rt_set_num_threads(nb_thread); // gm_runtime.h

top_(&t1,&tz);	
	gm_graph G = load_binary_2(gm_bin_file_name,task_file_name);
	nb_node = G.num_nodes(); 
top_(&t2,&tz);	
	init_time = cpu_time_(t1,t2);
	
top_(&t1,&tz);	 
	katz(G, beta, max_l);
top_(&t2,&tz);	
	work_time = cpu_time_(t1,t2);
            
/*	FILE* time_file = fopen("katz_green-marl_time.txt", "a");
	fprintf(time_file,"%s\t%d\t%ld.%03ld\t%ld.%03ld\n",argv[1],nb_thread,init_time/1000,init_time%1000,work_time/1000,work_time%1000);
	printf("%s\t%d\t%ld.%03ld\t%ld.%03ld\n",argv[1],nb_thread,init_time/1000,init_time%1000,work_time/1000,work_time%1000);
*/	return 0; 

}
