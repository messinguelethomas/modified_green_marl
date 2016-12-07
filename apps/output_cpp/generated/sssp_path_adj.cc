#include "sssp_path_adj.h"

void sssp_path(gm_graph& G, double* G_dist, 
    double* G_edge_cost, node_t& root, 
    node_t& end, node_t* G_prev_node, 
    edge_t* G_prev_edge)
{
    //Initializations
    gm_rt_initialize();
    G.freeze();

    bool fin = false ;
    bool* G_updated = gm_rt_allocate_bool(G.num_nodes(),gm_rt_thread_id());
    bool* G_updated_nxt = gm_rt_allocate_bool(G.num_nodes(),gm_rt_thread_id());
    double* G_dist_nxt = gm_rt_allocate_double(G.num_nodes(),gm_rt_thread_id());

    fin = false ;

    #pragma omp parallel for
    for (node_t t0 = 0; t0 < G.num_nodes(); t0 ++) 
    {
        G_dist[t0] = (double)((t0 == root)?0:DBL_MAX) ;
        G_updated[t0] = (t0 == root)?true:false ;
        G_dist_nxt[t0] = G_dist[t0] ;
        G_updated_nxt[t0] = G_updated[t0] ;
        G_prev_node[t0] = gm_graph::NIL_NODE ;
        G_prev_edge[t0] = gm_graph::NIL_EDGE ;
    }
    while ( !fin)
    {
        bool __E10 = false ;

        fin = true ;
        __E10 = false ;

        #pragma omp parallel for schedule(dynamic,128)
        for (node_t n = 0; n < G.num_nodes(); n ++) 
        {
            if (G_updated[n] && (G_dist[n] < G_dist[end]))
            {
                for (edge_t s_idx = G.begin[n];s_idx < G.begin[n+1] ; s_idx ++) 
                {
                    node_t s = G.node_idx [s_idx];
                    edge_t e;

                    e = s_idx ;
                    if (G_dist[n] + G_edge_cost[e] < G_dist[end])
                    {
                        { // argmin(argmax) - test and test-and-set
                            double G_dist_nxt_new = G_dist[n] + G_edge_cost[e];
                            if (G_dist_nxt[s]>G_dist_nxt_new) {
                                bool G_updated_nxt_arg = true;
                                node_t G_prev_node_arg = n;
                                edge_t G_prev_edge_arg = e;
                                gm_spinlock_acquire_for_node(s);
                                if (G_dist_nxt[s]>G_dist_nxt_new) {
                                    G_dist_nxt[s] = G_dist_nxt_new;
                                    G_updated_nxt[s] = G_updated_nxt_arg;
                                    G_prev_node[s] = G_prev_node_arg;
                                    G_prev_edge[s] = G_prev_edge_arg;
                                }
                                gm_spinlock_release_for_node(s);
                            }
                        }
                    }
                }
            }
        }
        #pragma omp parallel
        {
            bool __E10_prv = false ;

            __E10_prv = false ;

            #pragma omp for nowait
            for (node_t t6 = 0; t6 < G.num_nodes(); t6 ++) 
            {
                G_dist[t6] = G_dist_nxt[t6] ;
                G_updated[t6] = G_updated_nxt[t6] ;
                G_updated_nxt[t6] = false ;
                __E10_prv = __E10_prv || G_updated[t6] ;
            }
            ATOMIC_OR(&__E10, __E10_prv);
        }
        fin =  !__E10 ;
    }

    gm_rt_cleanup();
}

double get_path(gm_graph& G, node_t& begin0, 
    node_t& end, node_t* G_prev_node, 
    edge_t* G_prev_edge, double* G_edge_cost, 
    gm_node_seq& Q)
{
    //Initializations
    gm_rt_initialize();
    G.freeze();

    double total_cost = 0.0 ;
    node_t n;

    total_cost = ((float)(0.000000)) ;
    n = end ;
    if (G_prev_node[end] != gm_graph::NIL_NODE)
    {
        while (n != begin0)
        {
            edge_t e;

            Q.push_front(n);
            e = G_prev_edge[n] ;
            total_cost = total_cost + G_edge_cost[e] ;
            n = G_prev_node[n] ;
        }
    }
    return total_cost; 
}

