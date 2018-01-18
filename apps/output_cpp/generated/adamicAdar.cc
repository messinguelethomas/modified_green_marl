#include "adamicAdar.h"

void adamicAdar(gm_graph& G, double*G_aa)
{
    //Initializations
    gm_rt_initialize();
    G.freeze();
    G.do_semi_sort();
    G.prepare_edge_source();


    #pragma omp parallel
    #pragma omp single
    #pragma omp taskloop grainsize(1)
    for (edge_t e = 0; e < G.num_edges(); e ++) 
    {
        node_t from;
        node_t to;
        double __S0 = 0.0 ;

        from = G.node_idx_src[e] ;
        to = G.node_idx[e] ;
        __S0 = ((float)(0.000000)) ;
        // Iterate over Common neighbors
        gm_common_neighbor_iter n_I(G, from, to);
        for (node_t n = n_I.get_next(); n != gm_graph::NIL_NODE ; n = n_I.get_next()) 
        {
            __S0 = __S0 + 1 / log((G.begin[n+1] - G.begin[n])) ;
        }
        G_aa[e] = __S0 ;
    }
}

