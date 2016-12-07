#include "sssp_path.h"

void sssp_path(gm_graph& G, int32_t* G_dist, 
    int32_t* G_len, node_t& root, 
    node_t* G_prev)
{
    //Initializations
    gm_rt_initialize();
    G.freeze();

    bool fin = false ;
    bool* G_updated = gm_rt_allocate_bool(G.num_nodes(),gm_rt_thread_id());
    bool* G_updated_nxt = gm_rt_allocate_bool(G.num_nodes(),gm_rt_thread_id());
    int32_t* G_dist_nxt = gm_rt_allocate_int(G.num_nodes(),gm_rt_thread_id());

    fin = false ;

    #pragma omp parallel for
    for (node_t t0 = 0; t0 < G.num_nodes(); t0 ++) 
    {
        G_dist[t0] = (t0 == root)?0:INT_MAX ;
        G_updated[t0] = (t0 == root)?true:false ;
        G_dist_nxt[t0] = G_dist[t0] ;
        G_updated_nxt[t0] = G_updated[t0] ;
        G_prev[t0] = gm_graph::NIL_NODE ;
    }
    while ( !fin)
    {
        bool __E9 = false ;

        fin = true ;
        __E9 = false ;

        #pragma omp parallel for schedule(dynamic,128)
        for (node_t n = 0; n < G.num_nodes(); n ++) 
        {
            if (G_updated[n])
            {
                for (edge_t s_idx = G.begin[n];s_idx < G.begin[n+1] ; s_idx ++) 
                {
                    node_t s = G.node_idx [s_idx];
                    edge_t e;

                    e = s_idx ;
                    { // argmin(argmax) - test and test-and-set
                        int32_t G_dist_nxt_new = G_dist[n] + G_len[e];
                        if (G_dist_nxt[s]>G_dist_nxt_new) {
                            bool G_updated_nxt_arg = true;
                            node_t G_prev_arg = n;
                            gm_spinlock_acquire_for_node(s);
                            if (G_dist_nxt[s]>G_dist_nxt_new) {
                                G_dist_nxt[s] = G_dist_nxt_new;
                                G_updated_nxt[s] = G_updated_nxt_arg;
                                G_prev[s] = G_prev_arg;
                            }
                            gm_spinlock_release_for_node(s);
                        }
                    }
                }
            }
        }
        #pragma omp parallel
        {
            bool __E9_prv = false ;

            __E9_prv = false ;

            #pragma omp for nowait
            for (node_t t5 = 0; t5 < G.num_nodes(); t5 ++) 
            {
                G_dist[t5] = G_dist_nxt[t5] ;
                G_updated[t5] = G_updated_nxt[t5] ;
                G_updated_nxt[t5] = false ;
                __E9_prv = __E9_prv || G_updated[t5] ;
            }
            ATOMIC_OR(&__E9, __E9_prv);
        }
        fin =  !__E9 ;
    }

    gm_rt_cleanup();
}

void get_path(gm_graph& G, node_t& begin0, 
    node_t& end, node_t* G_prev, 
    gm_node_seq& Q)
{
    //Initializations
    gm_rt_initialize();
    G.freeze();

    node_t t;

    t = end ;
    if (G_prev[end] != gm_graph::NIL_NODE)
    {
        while (t != begin0)
        {
            Q.push_front(t);
            t = G_prev[t] ;
        }
        Q.push_front(t);
    }
}

