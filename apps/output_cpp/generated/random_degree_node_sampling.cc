#include "random_degree_node_sampling.h"

void random_degree_node_sampling(gm_graph& G, int32_t N, 
    gm_node_set& S)
{
    //Initializations
    gm_rt_initialize();
    G.freeze();

    int32_t __S1 = 0 ;
    double* G_prob = gm_rt_allocate_double(G.num_nodes(),gm_rt_thread_id());

    __S1 = 0 ;
    #pragma omp parallel
    {
        int32_t __S1_prv = 0 ;

        __S1_prv = 0 ;

        #pragma omp for nowait
        for (node_t n = 0; n < G.num_nodes(); n ++) 
        {
            __S1_prv = __S1_prv + (G.begin[n+1] - G.begin[n]) ;
        }
        ATOMIC_ADD<int32_t>(&__S1, __S1_prv);
    }

    #pragma omp parallel for
    for (node_t t0 = 0; t0 < G.num_nodes(); t0 ++) 
    {
        double dice = 0.0 ;

        G_prob[t0] = ((double)((G.begin[t0+1] - G.begin[t0]))) / ((double)__S1) * ((double)N) ;
        dice = gm_rt_uniform(gm_rt_thread_id()) ;
        if (dice < G_prob[t0])
            S.add_par(t0);
    }

    gm_rt_cleanup();
}

