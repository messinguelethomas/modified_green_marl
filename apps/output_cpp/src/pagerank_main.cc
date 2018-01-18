#include "graph_gen.h"
#include "common_main.h"
#include "time_manager.h"
#include "pagerank.h"  // defined in generated
class my_main: public main_t
{
public:
    double* rank;
    int max_iter;
    double e;
    double d;

    my_main() {
        e = 0.001;
        d = 0.85;
        max_iter = 100;
        rank = NULL;
    }

    virtual bool prepare() {
        rank = new double[G.num_nodes()];
        return true;
    }

    virtual bool run() {
        pagerank(G, e, d, max_iter, rank);
        return true;
    }

    virtual bool post_process() {
        //---------------------------------
        // values
        //---------------------------------
        for (int i = 0; i < 4; i++) {
            printf("rank[%d] = %0.9lf\n", i, rank[i]);
        }
        delete[] rank;
        return true;
    }

    virtual void print_arg_info() {
        printf("[max_iteration=100] [eplision=0.001] [delta=0.85]");
    }

    virtual bool check_args(int argc, char** argv) {
        if (argc > 0) {
            max_iter = atoi(argv[0]);
            if (max_iter <= 0) return false;
        }
        if (argc > 1) {
            e = atof(argv[1]);
            if (e <= 0) return false;
        }
        if (argc > 2) {
            d = atof(argv[2]);
            if (d <= 0) return false;
        }

        return true;
    }

};

int main(int argc, char** argv) {
	unsigned long long init_time, work_time;
	gm_graph* G ;
	//pagerank parameter
        double e = 0.00001;
        double d = 0.85;
        int max_iter = 500;
        double* rank = NULL;
	timeval_t t1, t2;
	timezone_t tz;
	char *gm_bin_file_name  = (char*) malloc(sizeof(char)*100);
	char *task_file_name    = (char*) malloc(sizeof(char)*100);
        int random_seed = 1989, nb_thread, nb_node, chunk = 1;
	char nom_fich[200];

	if(argc != 5)
        {
                printf("usage: ./pagerank file_graph_gm.bin file_task_info.bin nb_thread e=0.000000001 \n");
                return 1;
        }
	strcpy(gm_bin_file_name, argv[1]);
	strcpy(task_file_name,argv[2]);	
	nb_thread = atoi(argv[3]);
	e = atof(argv[4]);
	gm_rt_set_num_threads(nb_thread); // gm_runtime.h

top_(&t1,&tz);	
	gm_graph G_ = load_binary_2(gm_bin_file_name,task_file_name);
	nb_node = G_.num_nodes(); 
        rank = new double[nb_node];
	/*printf("%s %s\n",gm_bin_file_name,task_file_name);
        printf("N=%d nb_task = %d\n", nb_node,G_._numTask);*/
top_(&t2,&tz);	
	init_time = cpu_time_(t1,t2);
	
top_(&t1,&tz);	 
        pagerank(G_, e, d, max_iter, rank);
top_(&t2,&tz);	
	work_time = cpu_time_(t1,t2);
            
       /*// 
	for (int i = 0; i < nb_node; i++) {
            printf("rank[%d] = %0.9lf\n", i, rank[i]);
        }
	*///

	//top2 ();
	
	FILE* time_file = fopen("pagerank_green-marl_time.txt", "a");
	fprintf(time_file,"%s\t%d\t%ld.%03ld\t%ld.%03ld\n",argv[1],nb_thread,init_time/1000,init_time%1000,work_time/1000,work_time%1000);
	printf("%s\t%d\t%ld.%03ld\t%ld.%03ld\n",argv[1],nb_thread,init_time/1000,init_time%1000,work_time/1000,work_time%1000);
	return 0; 

}
