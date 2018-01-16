#ifndef GRAPH_GEN_H_
#define GRAPH_GEN_H_

#include "gm_graph.h"
typedef struct
{
	double p_t0;
	double p_t1;
	double e;
	int *From_id;
	int con_size;
	int from_size;
}graph_first_format;

typedef struct
{
   task_type* task_info;
   int nb_task;
}task_info_t;

typedef task_type task_type_t;
gm_graph* create_uniform_random_graph(node_t N, edge_t M, long seed, bool use_xorshift_rng);
gm_graph* create_uniform_random_graph2(node_t N, edge_t M, long seed);
gm_graph* create_uniform_random_nonmulti_graph(node_t N, edge_t M, long seed);

gm_graph load_binary_2(char* binary_file, char* binary_task_info);
task_type* load_task_info(char* file_task_info, int* nb_task);
task_info_t generate_task_index(unsigned long long degree_sum, int nb_node, int nb_thread, int chunk, graph_first_format* Nodes);
gm_graph* load_graph_set_degree_schedule(char* filename, int N, int nb_thread, int chunk);
gm_graph* load_graph_txt(char* filename, int nb_node);
gm_graph* load_graph_gml(char* nom_fich);
int node_count_gml(char* nom_fich, int*min_node_);
char *trim_space(char *str);
char** str_split(char* str, char* car, char*reslt[]);
int is_directed_gml(char* nom_fich);
#endif /* GRAPH_GEN_H_ */
