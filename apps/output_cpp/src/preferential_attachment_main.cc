#include "preferential_attachment.h"
#include "graph_gen.h"
#include "gm_graph.h"
#include <limits.h>
#include <sys/time.h>

int main(int argc, char* argv[])
{
	//Create an empty graph
	gm_graph* G;
        int random_seed = 1989, nb_thread;
	float beta = 0.5; int32_t max_l;
	char nom_fich[200];

	if(argc != 5)
        {
                printf("usage: ./katz graph_file.(txt|gml) nb_node max_l nb_thread\n");
                return 1;
        }
	
	max_l = atoi(argv[3]); nb_thread = atoi(argv[3]);

	gm_rt_set_num_threads(nb_thread); // gm_runtime.h
	//G = load_graph_gml(argv[1]);
	G = load_graph_txt(argv[1], atoi(argv[2]));
	//top1 () ;
	
	preferential_attachment(*G);

	//top2 ();
	
	sprintf(nom_fich,"katz_v2_execution_time.txt");
	FILE* time_file = fopen(nom_fich, "a");
	unsigned long temps;// = cpu_time2();
	fprintf(time_file,"%2f\t%d\t%d\t%d\t%d\t%ld.%03ld\n",beta,G->num_nodes(),G->num_edges(),max_l,nb_thread,temps/1000,temps%1000);
	return 0; 
}
