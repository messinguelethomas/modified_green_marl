#include "pagerank.h"

void pagerank(gm_graph& G, double e, 
    double d, int32_t max, 
    double*G_pg_rank)
{
    //Initializations
    gm_rt_initialize();
    G.freeze();
    G.make_reverse_edges();

    double diff = 0.0 ;
    int32_t cnt = 0 ;
    double N = 0.0 ;
    double* G_pg_rank_nxt = gm_rt_allocate_double(G.num_nodes(),gm_rt_thread_id());

    cnt = 0 ;
    N = (double)(G.num_nodes()) ;

    #pragma omp parallel
    #pragma omp single
    #pragma omp taskloop
    for (int tsk_t0 = 0; tsk_t0 < G.num_task(); tsk_t0++)
    for (node_t t0 = G.task_tab[tsk_t0].start; t0 < G.task_tab[tsk_t0].end; t0 ++) 
        G_pg_rank[t0] = 1 / N ;

    do
    {
        diff = ((float)(0.000000)) ;
        #pragma omp parallel
        {
            double diff_prv = 0.0 ;

            diff_prv = ((float)(0.000000)) ;

            #pragma omp single
            #pragma omp taskloop grainsize(1)
            for (int tsk_t = 0; tsk_t < G.num_task(); tsk_t++)
            for (node_t t = G.task_tab[tsk_t].start; t < G.task_tab[tsk_t].end; t ++) 
            {
                double val = 0.0 ;
                double __S1 = 0.0 ;

                __S1 = ((float)(0.000000)) ;
                for (edge_t w_idx = G.r_begin[t];w_idx < G.r_begin[t+1] ; w_idx ++) 
                {
                    node_t w = G.r_node_idx [w_idx];
                    __S1 = __S1 + G_pg_rank[w] / ((double)((G.begin[w+1] - G.begin[w]))) ;
                }
                val = (1 - d) / N + d * __S1 ;
                diff_prv = diff_prv +  std::abs((val - G_pg_rank[t]))  ;
                G_pg_rank_nxt[t] = val ;
            }
            ATOMIC_ADD<double>(&diff, diff_prv);
        }

        #pragma omp parallel
        #pragma omp single
        #pragma omp taskloop
        for (int tsk_i3 = 0; tsk_i3 < G.num_task(); tsk_i3++)
        for (node_t i3 = G.task_tab[tsk_i3].start; i3 < G.task_tab[tsk_i3].end; i3 ++) 
            G_pg_rank[i3] = G_pg_rank_nxt[i3] ;

        cnt = cnt + 1 ;
    }
    while ((diff > e) && (cnt < max));


    gm_rt_cleanup();
}

