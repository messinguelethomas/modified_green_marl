#include "random_node_sampling.h"

void random_node_sampling(gm_graph& G, int32_t N, 
    gm_node_set& S)
{
    //Initializations
    gm_rt_initialize();
    G.freeze();

    double prob = 0.0 ;

    prob = 1 / ((double)N) ;

    #pragma omp parallel
    #pragma omp single
    #pragma omp taskloop
    for (int tsk_v = 0; tsk_v < G.num_task(); tsk_v++)
    for (node_t v = G.task_tab[tsk_v].start; v < G.task_tab[tsk_v].end; v ++) 
    {
        double dice = 0.0 ;

        dice = gm_rt_uniform(gm_rt_thread_id()) ;
        if (dice < prob)
            S.add_par(v);
    }
}

