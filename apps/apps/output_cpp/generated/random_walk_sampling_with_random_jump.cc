#include "random_walk_sampling_with_random_jump.h"

void random_walk_sampling_with_random_jump(gm_graph& G, int32_t N, 
    double c, gm_node_set& S)
{
    //Initializations
    gm_rt_initialize();
    G.freeze();
    G.do_semi_sort();

    node_t n;
    int32_t count = 0 ;

    n = G.pick_random_node() ;
    count = 0 ;
    while (count < N)
    {
        S.add_seq(n);
        count = count + 1 ;
        if (((G.begin[n+1] - G.begin[n]) == 0) || (gm_rt_uniform(gm_rt_thread_id()) < c))
        {
            n = G.pick_random_node() ;
        }
        else 
        {
            n = G.pick_random_out_neighbor(n) ;
        }
    }
}

