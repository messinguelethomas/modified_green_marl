#include <stdlib.h>
#include <set>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <sys/time.h>

#include "graph_gen.h"
#include "gm_rand.h"

gm_graph* create_uniform_random_graph(node_t N, edge_t M, long seed, bool use_xorshift_rng) {
    
    gm_rand xorshift_rng(seed);
    if (!use_xorshift_rng) {
        srand(seed);
    }

    gm_graph* g = new gm_graph();
    g->prepare_external_creation(N, M);

    node_t* src = new node_t[M];
    node_t* dest = new node_t[M];
    edge_t* degree = new edge_t[N];
    memset(degree, 0, sizeof(edge_t) * N);

    for (edge_t i = 0; i < M; i++) {
        node_t r;
        if (use_xorshift_rng) r = (edge_t)xorshift_rng.rand();
        else r = rand(); //TODO 64-bit ?
        src[i] = r % N;  
        if (use_xorshift_rng) r = (edge_t)xorshift_rng.rand();
        else r = rand(); //TODO 64-bit ?
        dest[i] = r % N; 

        degree[src[i]]++;
    }

    g->begin[0] = 0;
    for (node_t i = 1; i <= N; i++) {
        g->begin[i] = g->begin[i - 1] + degree[i - 1];
    }

    for (edge_t i = 0; i < M; i++) {
        node_t u = src[i];
        node_t v = dest[i];

        edge_t pos = degree[u]--;
        assert(pos > 0);
        g->node_idx[g->begin[u] + pos - 1] = v;  // set end node of this edge
    }

    delete[] src;
    delete[] dest;
    delete[] degree;

    return g;
}

//-----------------------------------------------------------------------------
// Note: this method is 20x slower than the above method
//-----------------------------------------------------------------------------
gm_graph* create_uniform_random_graph2(node_t N, edge_t M, long seed) {
    srand(seed);

    gm_graph *G = new gm_graph();
    for (node_t i = 0; i < N; i++) {
        G->add_node();
    }
    for (edge_t i = 0; i < M; i++) {
        node_t from = rand() % N;
        node_t to = rand() % N;
        G->add_edge(from, to);
    }

    struct timeval T1, T2;
    gettimeofday(&T1, NULL);
    G->freeze();
    gettimeofday(&T2, NULL);
    printf("time for freeze (ms) = %lf\n", ((T2.tv_sec) - (T1.tv_sec)) * 1000 - (T2.tv_usec - T1.tv_usec) * 0.001);

    return G;
}

//-----------------------------------------------------------------------------
// Note: Based on create_uniform_random_graph2
//-----------------------------------------------------------------------------
gm_graph* create_uniform_random_nonmulti_graph(node_t N, edge_t M, long seed) {
    srand(seed);

    gm_graph *G = new gm_graph();
    for (node_t i = 0; i < N; i++) {
        G->add_node();
    }
    for (edge_t i = 0; i < M; i++) {
        node_t from = rand() % N;
        node_t to = rand() % N;
        if (!G->has_edge(from, to))
          G->add_edge(from, to);
        else
          i--;
    }

    G->freeze();

    return G;
}

/** 
 Create RMAT graph
 a, b, c : params
 */
gm_graph* create_RMAT_graph(node_t N, edge_t M, long rseed, double a, double b, double c, bool permute) {
    double d;
    assert(a + b + c < 1);
    d = 1 - (a + b + c);

    // my favorite random seed
    srand48(rseed);

    gm_graph* g = new gm_graph();
    g->prepare_external_creation(N, M);

    //----------------------------------------------
    // generate edges
    //----------------------------------------------
    // 0. init
    node_t* src = new node_t[M];
    node_t* dest = new node_t[M];
    edge_t* degree = new edge_t[N];
    memset(degree, 0, sizeof(edge_t) * N);

    node_t SCALE = (node_t) log2((double) N);

    // 1. edge-gen
    for (edge_t i = 0; i < M; i++) {
        node_t u = 1;
        node_t v = 1;
        node_t step = N / 2;
        double av = a;
        double bv = b;
        double cv = c;
        double dv = d;

        double p = drand48();
        if (p < av) { // do nothing
        } else if (p < (av + bv)) {
            v += step;
        } else if (p < (av + bv + cv)) {
            u += step;
        } else {
            v += step;
            u += step;
        }
        for (node_t j = 1; j < SCALE; j++) {
            step = step / 2;
            double var = 0.1;
            av *= 0.95 + var * drand48();		// vary abcd by 10%
            bv *= 0.95 + var * drand48();
            cv *= 0.95 + var * drand48();
            dv *= 0.95 + var * drand48();

            double S = av + bv + cv + dv;
            av = av / S;
            bv = bv / S;
            cv = cv / S;
            dv = dv / S;

            // choose partition
            p = drand48();
            if (p < av) { // do nothing
            } else if (p < (av + bv)) {
                v += step;
            } else if (p < (av + bv + cv)) {
                u += step;
            } else {
                v += step;
                u += step;
            }
        }

        src[i] = u - 1;
        dest[i] = v - 1;

        // avoid self edges
        if (src[i] == dest[i]) {
            i = i - 1;
            continue;
        }
    }

    // 2. permutate vertice
    // so that, one can't know what are the high-degree edges from node_id
    if (permute) {
        node_t* P = new node_t[N];
        for (node_t i = 0; i < N; i++)
            P[i] = i;

        for (node_t i = 0; i < N; i++) {
            node_t j = (node_t) (N * drand48());
            node_t temp = P[j];
            P[j] = P[i];
            P[i] = temp;
        }

        for (edge_t i = 0; i < M; i++) {
            src[i] = P[src[i]];
            dest[i] = P[dest[i]];
        }

        delete[] P;
    }

    // 3. count degree
    for (edge_t i = 0; i < M; i++) {
        degree[src[i]]++;
    }

    // (It is possible this routine creates multi-edges between a node-pair)
    // (Should be check later, or be ignored now)

    // 4. Now setup G's data structures
    g->begin[0] = 0;
    for (node_t i = 1; i <= N; i++) {
        g->begin[i] = g->begin[i - 1] + degree[i - 1];
    }

    for (edge_t i = 0; i < M; i++) {
        node_t u = src[i];
        node_t v = dest[i];
        edge_t pos = degree[u]--;

        g->node_idx[g->begin[u] + pos - 1] = v;  // set end node of this edge
    }

    delete[] src;
    delete[] dest;
    delete[] degree;

    return g;
}

task_info_t generate_task_index(unsigned long long degree_sum, int nb_node, int nb_thread, int chunk, graph_first_format* Nodes)
{
	unsigned long long max_degree_sum_task, degree_sum_temp, i, task_id;
	int nb_task = chunk*nb_thread;
	task_info_t Task;

	max_degree_sum_task = degree_sum / nb_task;
	 Task.task_info = (task_type*)malloc(sizeof(task_type)*nb_task);

	i = 0; task_id =0;
	while((i < nb_node)&&(task_id < nb_task))
	{
		Task.task_info[task_id].start = i;
		degree_sum_temp = 0;
		while((degree_sum_temp < max_degree_sum_task)&&(i < nb_node))
		{
			degree_sum_temp = degree_sum_temp + (unsigned long long) Nodes[i].from_size;
			i++;
		}
		if(i >= (nb_node - 1))
			Task.task_info[task_id].end = nb_node;
		else
			Task.task_info[task_id].end = i+1;
		Task.task_info[task_id].tid = task_id;
		Task.task_info[task_id].degree_sum = degree_sum_temp;
		//printf("task %d begin at %d end at %d degree_sum = %lld\n",task_id, Task.task_info[task_id].start, Task.task_info[task_id].end, Task.task_info[task_id].degree_sum);
		task_id++;
		i++;
		Task.nb_task = task_id;
	}

	while(task_id < nb_task)
	{
		Task.task_info[task_id].start = nb_node;
		Task.task_info[task_id].end = nb_node;
		Task.task_info[task_id].tid = task_id;
		Task.task_info[task_id].degree_sum = 0;
		//printf("task %d begin at %d end at %d degree_sum = %lld\n",task_id, Task.task_info[task_id].start, Task.task_info[task_id].end, Task.task_info[task_id].degree_sum);
		task_id++;
	}
		//printf("nb_task = %d\treal_nb_task = %d\tdegree_sum = %lld max_degree_sum_task = %lld\n",nb_task, Task.nb_task, degree_sum, max_degree_sum_task);

	printf("degree_sum = %lld max_degree_sum_task = %lld\t",degree_sum, max_degree_sum_task);
	printf("nb_task = %d \n",Task.nb_task);
	return Task;

}
/***** Read graph from txt file *****/	

gm_graph* load_graph_txt(char* filename, int N)
{
    FILE *fid;
    int from_idx, to_idx, i,  temp_size, degree, j, M;

	graph_first_format* Nodes;
	Nodes = (graph_first_format*)malloc(N*sizeof(graph_first_format));
    
    	for (i = 0; i < N; i++)
	{
		Nodes[i].con_size = 0;
		Nodes[i].from_size = 0;
		Nodes[i].From_id = (int*) malloc(sizeof(int));
        }	
    
    	fid = fopen(filename, "r");
   	if (fid == NULL){printf("Error opening the file\n");}

	while (!feof(fid))
	{
		if (fscanf(fid,"%d\t%d\n", &from_idx, &to_idx))
		{
			Nodes[from_idx].con_size++;
			Nodes[to_idx].from_size++;
			temp_size = Nodes[to_idx].from_size;
			Nodes[to_idx].From_id = (int*) realloc(Nodes[to_idx].From_id, temp_size * sizeof(int));
			Nodes[to_idx].From_id[temp_size - 1] = from_idx;
			M++;
		}
	}
    
	gm_graph* G = new gm_graph();
	G->prepare_external_creation(N, M);
	G->begin[0] = 0;
	for (i = 1; i <= N; i++) {
		G->begin[i] = G->begin[i - 1] + Nodes[i-1].from_size;
		degree = Nodes[i-1].con_size;
		for(j = 0; j < degree; j++){
			G->node_idx[G->begin[i-1] + j] = Nodes[i-1].From_id[j];  // set end node of this edge
		}
		free(Nodes[i-1].From_id);
	}
	free(Nodes);
	return G;
}


gm_graph load_binary_2(char* binary_file, char* binary_task_info)
{
	gm_graph G;
	int nb_task;
	bool b = G.load_binary(binary_file);
	if(!b){
		printf("An error occures while reading %s\n", binary_file);
		exit(-1);
	}

	G.task_tab = load_task_info(binary_task_info, &nb_task);
	G._numTask = nb_task;
	//printf("nb_task = %d\n",G._numTask);

	return G;
}

gm_graph* load_graph_set_degree_schedule(char* filename, int N, int nb_thread, int chunk)
{
    FILE *fid;
    int from_idx, to_idx, i,  temp_size, degree, j, M = 0;

	graph_first_format* Nodes;
	Nodes = (graph_first_format*)malloc(N*sizeof(graph_first_format));
    
    	for (i = 0; i < N; i++)
	{
		Nodes[i].con_size = 0;
		Nodes[i].from_size = 0;
		Nodes[i].From_id = (int*) malloc(sizeof(int));
        }	
    
    	fid = fopen(filename, "r");
   	if (fid == NULL){printf("Error opening the file\n");}

	while (!feof(fid))
	{
		if (fscanf(fid,"%d\t%d\n", &from_idx, &to_idx))
		{
			Nodes[from_idx].con_size++;
			Nodes[to_idx].from_size++;
			temp_size = Nodes[to_idx].from_size;
			Nodes[to_idx].From_id = (int*) realloc(Nodes[to_idx].From_id, temp_size * sizeof(int));
			Nodes[to_idx].From_id[temp_size - 1] = from_idx;
			M++;
		}
	}
    
    	task_info_t task_info = generate_task_index(M, N, nb_thread, chunk, Nodes);
	gm_graph* G = new gm_graph();
	G->prepare_external_creation(N, M);
	G->task_tab = task_info.task_info;
	G->_numTask = task_info.nb_task;	
	G->begin[0] = 0;
	for (i = 1; i <= N; i++) {
		G->begin[i] = G->begin[i - 1] + Nodes[i-1].from_size;
		degree = Nodes[i-1].con_size;
		for(j = 0; j < degree; j++){
			G->node_idx[G->begin[i-1] + j] = Nodes[i-1].From_id[j];  // set end node of this edge
		}
		free(Nodes[i-1].From_id);
	}
	free(Nodes);
	return G;
}

 task_type* load_task_info(char* file_task_info, int* nb_task) 
{
	FILE* f;	
	task_type* task_info;
	task_type ts_inf;
	f = fopen(file_task_info, "rb");
	if(f == NULL){
		fprintf (stderr,"Cannot open %s for writting",file_task_info);
		printf ("Cannot open %s for writting",file_task_info);
		exit(-1);
	}
	fread(nb_task,sizeof(int),1,f);
	task_info = (task_type*)malloc((*nb_task) * sizeof(task_type));
	int i;
	for(i = 0; i < *nb_task; i++){
		fread(&ts_inf, sizeof(task_type),1,f);
		task_info[i].tid   = ts_inf.tid;
		task_info[i].start = ts_inf.start;
		task_info[i].end   = ts_inf.end;
		task_info[i].degree_sum   = ts_inf.degree_sum;
	}
	fclose(f);
	return task_info;
}

/***** Read graph from gml file *****/	
gm_graph* load_graph_gml(char* nom_fich)
{
	int max_char_read = 100, len_string = 100, min_node;
	char ch1[20], ch2[20], ch[max_char_read], *tmp = {"test"};
	FILE* graph_file = NULL;
	char* reslt[len_string],*sep={" "};
	int i_node = 0, i_edge = 0, orig, extr, id;
	int node;
	int directed = is_directed_gml(nom_fich);
	int nb_node = node_count_gml(nom_fich, &min_node);

	graph_file = fopen(nom_fich, "r");
	gm_graph* G = new gm_graph();
	
	if(graph_file!= NULL)
	{
		while(tmp)
		{
			tmp = fgets(ch, max_char_read, graph_file);

			if(strcmp(trim_space(ch),"node")==0)
			{	
				id = -1;
				while(strcmp(trim_space(ch),"]")!=0)
				{
					tmp = fgets(ch, max_char_read, graph_file);
					str_split(ch,sep,reslt);
					if(strcmp(trim_space(reslt[0]),"id")==0)
					{
						G->add_node();//Assuming that nodes are consecutive in gml file
						i_node++;
					}
				}
					
			}

			if(strcmp(trim_space(ch),"edge")==0)
			{
				while(strcmp(trim_space(ch),"]")!=0)
				{
					tmp = fgets(ch, max_char_read, graph_file);
					str_split(ch,sep,reslt);
					if(strcmp(trim_space(reslt[0]),"source")==0)
					{
						orig = atoi(reslt[1]) - min_node;
					}
					if(strcmp(trim_space(reslt[0]),"target")==0)
					{
						extr = atoi(reslt[1]) - min_node;
						G->add_edge(orig,extr);
						if(!directed)					
							G->add_edge(extr, orig);
					}
				}
			}
			
		}
		fclose(graph_file);
	}
	G->freeze();
	return G;
}

int node_count_gml(char* nom_fich, int*min_node_)
{
	int max_char_read = 100, len_string = 100;
	char ch[max_char_read], *tmp = {"test"};
	FILE* graph_file = NULL;
	char* reslt[len_string],*sep={" "};

	graph_file = fopen(nom_fich, "r");
	int nb_node = 0;
	int min_node = INT_MAX;

	if(graph_file!= NULL)
	{
		while(tmp)
		{
			tmp = fgets(ch, max_char_read, graph_file);
			str_split(ch,sep,reslt);
			if(strcmp(trim_space(ch),"node")==0)
			{
				nb_node++;
				while(strcmp(trim_space(ch),"]")!=0)
				{
					tmp = fgets(ch, max_char_read, graph_file);
					str_split(ch,sep,reslt);
					if(strcmp(trim_space(reslt[0]),"id")==0)
					{
						if(atoi(reslt[1])<min_node)
							min_node = atoi(reslt[1]);
					}
				}
			}

		}
		fclose(graph_file);
	}
	*min_node_ = min_node;
	return nb_node;

}
char *trim_space(char *str)
{
  char *end;

  // Trim leading space
  while(isspace(*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace(*end)) end--;

  // Write new null terminator
  *(end+1) = 0;

  return str;
}
char** str_split(char* str, char* car, char*reslt[])
{
	char *str1, *str2, *token, *subtoken;
	char *saveptr1, *saveptr2;
	int j;

	for (j = 1, str1 = str; ; j++, str1 = NULL) {
	       token = strtok_r(str1, car, &saveptr1);
	       if (token == NULL)
		   break;
		reslt[j-1] = token;
	  }
	
	return reslt;
}
int is_directed_gml(char* nom_fich)
{
	int max_char_read = 100, len_string = 100;
	char ch[max_char_read], *tmp = {"test"};
	FILE* graph_file = NULL;
	char* reslt[len_string],*sep={" "};
	
	graph_file = fopen(nom_fich, "r");
	int directed = 0, trouve = 0;

	if(graph_file!= NULL)
	{
		while(tmp && !trouve)
		{
			tmp = fgets(ch, max_char_read, graph_file);
			str_split(ch,sep,reslt);
			if(strcmp(trim_space(reslt[0]),"directed")==0)
			{
				directed = atoi(reslt[1]);
				trouve = 1;
			}
		}
		fclose(graph_file);
	}
	return directed;
}

