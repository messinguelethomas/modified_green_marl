#include "gm_graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <assert.h>
#include <sys/time.h>
#include <inttypes.h>  // for PRId64

#include "graph_gen.h"
int main(int argc, char** argv) {

    //-----------------------------
    // create RMAT graph
    //-----------------------------
    if (argc < 5) {
        printf("%s <source filename> <Num Node> <nb_thread> <chunk> <out filename> \n", argv[0]);
        exit(0);
    }
	FILE *f;
	char* resl_split[50];
	char *temp_load        = (char*) malloc(sizeof(char)*100); 
	char *file_task_info   = (char*) malloc(sizeof(char)*100);
	char *source_file_name = (char*) malloc(sizeof(char)*100);
	char *out_file_name    = (char*) malloc(sizeof(char)*100);

	int nb_node   = atoi(argv[2]);
	int nb_thread = atoi(argv[3]);
	int chunk     = atoi(argv[4]);
	strcpy(source_file_name,argv[1]);
	strcpy(out_file_name,argv[5]);
	strcpy(temp_load,out_file_name);
    if (nb_node == 0) {printf("Empty graph not allowed\n"); return EXIT_FAILURE;}


    gm_graph* g;
    int random_seed = 1997;

    struct timeval T1, T2;
    gettimeofday(&T1, NULL);
	g = load_graph_set_degree_schedule(source_file_name, nb_node, nb_thread, chunk);
	str_split(temp_load,".",resl_split);
	sprintf(file_task_info,"%s_%d_tasks.bin",resl_split[0],g->_numTask);
	fflush (stdout);
	gettimeofday(&T1, NULL);
	
	f = fopen(file_task_info, "wb");
	if(f == NULL){
		fprintf (stderr,"Cannot open %s for writting",file_task_info);
		printf ("Cannot open %s for writting",file_task_info);
		return -1;
	}
	/*fwrite(&g->task_info,sizeof(task_type),1,f);*/
	fwrite(&g->_numTask,sizeof(int),1,f);
	fwrite(g->task_tab+0,sizeof(task_type),g->_numTask,f);
	fclose(f);
	g->store_binary(out_file_name);
	gettimeofday(&T2, NULL);

    printf ("Storing \"%s\" and \"%s\" ",out_file_name,file_task_info);
    printf("in  %lf ms\n", ((T2.tv_sec) - (T1.tv_sec)) * 1000 + (T2.tv_usec - T1.tv_usec) * 0.001);
    delete g;
    return EXIT_SUCCESS;
}

/*//  CREATE RMAT  or random file and dump
int main(int argc, char** argv) {

    //-----------------------------
    // create RMAT graph
    //-----------------------------
    if (argc < 5) {
        printf("%s <Num Node> <Num Edge> <out filename> <0~1>\n", argv[0]);
        printf("\t or <Num Node> <nb_thread> <out filename> <0~1> <chunk> <source filename>\n", argv[0]);
        printf("\t 0: uniform random (multigprah)\n");
        printf("\t 1: uniform random alternative (multigraph)\n");
        printf("\t 2: uniform random \n");
        printf("\t 3: uniform random (multigraph - xorshift random)\n");
        //        printf("\t 3: RMAT random (mu\n");
        exit(0);
    }

    int gtype = atoi(argv[4]);
    node_t N = (node_t) atoll(argv[1]);
    if (N == 0) {printf("Empty graph not allowed\n"); return EXIT_FAILURE;}


    gm_graph* g;
    int random_seed = 1997;

    struct timeval T1, T2;
    gettimeofday(&T1, NULL);
if(gtype <=3)
{
    edge_t M = (edge_t) atoll(argv[2]);
    printf("Creating Graph, N = %I64d, M = %I64d , Type = %d\n", (int64_t)N, (int64_t) M, gtype);
    switch (gtype) {
        case 0:
            g = create_uniform_random_graph(N, M, random_seed, false);
            break;
        case 1:
            g = create_uniform_random_graph2(N, M, random_seed);
            break;
        case 2:
            g = create_uniform_random_nonmulti_graph(N, M, random_seed);
            break;
        case 3:
            g = create_uniform_random_graph(N, M, random_seed, true);
            break;
       //
        case 4:
             g = create_RMAT_graph(N, M, random_seed, need_back_edge);
             break;
       // */
       /* default:
            printf("UNKNOWN GRAPH TYPE\n");
            exit(-1);
    }
    gettimeofday(&T2, NULL);
    printf("creation time (ms) = %lf\n", ((T2.tv_sec) - (T1.tv_sec)) * 1000 + (T2.tv_usec - T1.tv_usec) * 0.001);

    printf("saving to file = %s\n", argv[3]);
    fflush (stdout);
    gettimeofday(&T1, NULL);
    g->store_binary(argv[3]);
    gettimeofday(&T2, NULL);
}
else
{
	int nb_node   = atoi(argv[1]);
	int nb_thread = atoi(argv[2]);
	int chunk     = atoi(argv[5]);
	g = load_graph_set_degree_schedule(argv[6], nb_node, nb_thread, chunk);
	FILE *f;
	char* resl_split[50];
	char *temp_load       = (char*) malloc(sizeof(char)*100); 
	char *file_task_info  = (char*) malloc(sizeof(char)*100);
	strcpy(temp_load,argv[3]);
	str_split(temp_load,".",resl_split);
	sprintf(file_task_info,"%s_%d_tasks.bin",resl_split[0],nb_thread*chunk);
	fflush (stdout);
	gettimeofday(&T1, NULL);
	
		printf ("Task_info file: %s\n",file_task_info);
	f = fopen(file_task_info, "wb");
	if(f == NULL){
		fprintf (stderr,"Cannot open %s for writting",file_task_info);
		printf ("Cannot open %s for writting",file_task_info);
		return -1;
	}
	//fwrite(&g->task_info,sizeof(task_type),1,f);
	fwrite(&g->_numTask,sizeof(int),1,f);
	fwrite(g->task_tab+0,sizeof(task_type),g->_numTask,f);
	fclose(f);
	g->store_binary(argv[3]);
	gettimeofday(&T2, NULL);
}

    printf("storing time (ms) = %lf\n", ((T2.tv_sec) - (T1.tv_sec)) * 1000 + (T2.tv_usec - T1.tv_usec) * 0.001);
    delete g;
    return EXIT_SUCCESS;
}*/
