#include "triangle_counting.h"

int64_t triangle_counting(gm_graph& G)
{
    //Initializations
    gm_rt_initialize();
    G.freeze();
    G.do_semi_sort();

    int64_t T = 0 ;

    #pragma omp parallel
    {
        int64_t T_prv = 0 ;

        T_prv = 0 ;

        #pragma omp single
        #pragma omp taskloop grainsize(1)
        for (int tsk_v = 0; tsk_v < G.num_task(); tsk_v++)
        for (node_t v = G.task_tab[tsk_v].start; v < G.task_tab[tsk_v].end; v ++) 
            for (edge_t u_idx = G.begin[v];u_idx < G.begin[v+1] ; u_idx ++) 
            {
                node_t u = G.node_idx [u_idx];
                if (u > v)
                {
                    for (edge_t w_idx = G.begin[v];w_idx < G.begin[v+1] ; w_idx ++) 
                    {
                        node_t w = G.node_idx [w_idx];
                        if (w > u)
                        {
                            if (G.is_neighbor(w,u))
                            {
                                T_prv = T_prv + 1 ;
                            }
                        }
                    }
                }
            }

        ATOMIC_ADD<int64_t>(&T, T_prv);
    }
    return T; 
}

