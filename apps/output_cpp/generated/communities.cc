#include "communities.h"

void communities(gm_graph& G, node_t* G_Comm)
{
    //Initializations
    gm_rt_initialize();
    G.freeze();

    bool finished = false ;

    finished = true ;

    #pragma omp parallel for
    for (node_t x = 0; x < G.num_nodes(); x ++) 
        G_Comm[x] = x ;

    do
    {
        finished = true ;

        #pragma omp parallel for schedule(dynamic,128)
        for (node_t x0 = 0; x0 < G.num_nodes(); x0 ++) 
        {
            gm_map_medium<node_t, int32_t> T(gm_rt_get_num_threads(),  0);

            for (edge_t y_idx = G.begin[x0];y_idx < G.begin[x0+1] ; y_idx ++) 
            {
                node_t y = G.node_idx [y_idx];
                node_t source;

                source = G_Comm[y] ;
                T.changeValueAtomicAdd(source, 1);
            }
            if ( !T.hasMaxValue_seq(G_Comm[x0]))
            {
                G_Comm[x0] = T.getMaxKey_seq() ;
                finished = false ;
            }
        }
    }
    while ( !finished);

}

