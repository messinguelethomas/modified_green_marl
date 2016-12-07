#include "potential_friends.h"

void potential_friends(gm_graph& G, gm_property_of_collection<gm_node_set>& G_potFriend)
{
    //Initializations
    gm_rt_initialize();
    G.freeze();
    G.do_semi_sort();


    #pragma omp parallel for schedule(dynamic,128)
    for (node_t v = 0; v < G.num_nodes(); v ++) 
        for (edge_t u_idx = G.begin[v];u_idx < G.begin[v+1] ; u_idx ++) 
        {
            node_t u = G.node_idx [u_idx];
            if (u != v)
                for (edge_t w_idx = G.begin[u];w_idx < G.begin[u+1] ; w_idx ++) 
                {
                    node_t w = G.node_idx [w_idx];
                    if ((w != u) && (w != v))
                        if ( !G.is_neighbor(v,w))
                            G_potFriend[v].add_par(w);
                }
        }

}

