#include "communities.h"

void communities(gm_graph& G, node_t* G_Comm)
{
    //Initializations
    gm_rt_initialize();
    G.freeze();

    bool finished = false ;

    finished = true ;

    #pragma omp parallel for
    for (int tsk_x = 0; tsk_x < G.num_task(); tsk_x++)
    for (node_t x = G.task_tab[tsk_x].start; x < G.task_tab[tsk_x].end; x ++) 
        G_Comm[x] = x ;

    do
    {
        finished = true ;

        #pragma omp parallel for schedule(dynamic,1)
        for (int tsk_x0 = 0; tsk_x0 < G.num_task(); tsk_x0++)
        for (node_t x0 = G.task_tab[tsk_x0].start; x0 < G.task_tab[tsk_x0].end; x0 ++) 
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

