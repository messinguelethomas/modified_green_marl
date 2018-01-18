#include "bidir_dijkstra.h"

bool bidir_dijkstra(gm_graph& G, int32_t* G_Weight, 
    node_t& src, node_t& dst, 
    node_t*G_Parent, edge_t*G_ParentEdge)
{
    //Initializations
    gm_rt_initialize();
    G.freeze();
    G.make_reverse_edges();

    gm_mutatable_priority_map_unordered_min<node_t, int32_t> FReachable( 0);
    gm_mutatable_priority_map_unordered_min<node_t, int32_t> RReachable( 0);
    int32_t curMinFCost = 0 ;
    int32_t curMinRCost = 0 ;
    int32_t minCost = 0 ;
    node_t mid;
    bool terminate = false ;
    node_t* G_RParent = gm_rt_allocate_node_t(G.num_nodes(),gm_rt_thread_id());
    edge_t* G_RParentEdge = gm_rt_allocate_edge_t(G.num_nodes(),gm_rt_thread_id());
    bool* G_FFinalized = gm_rt_allocate_bool(G.num_nodes(),gm_rt_thread_id());
    bool* G_RFinalized = gm_rt_allocate_bool(G.num_nodes(),gm_rt_thread_id());
    int32_t* G_FCost = gm_rt_allocate_int(G.num_nodes(),gm_rt_thread_id());
    int32_t* G_RCost = gm_rt_allocate_int(G.num_nodes(),gm_rt_thread_id());

    FReachable.setValue_seq(src, 0);
    RReachable.setValue_seq(dst, 0);
    curMinFCost = 0 ;
    curMinRCost = 0 ;
    minCost = INT_MAX ;
    mid = gm_graph::NIL_NODE ;
    terminate = false ;

    #pragma omp parallel
    #pragma omp single
    #pragma omp taskloop
    for (int tsk_t0 = 0; tsk_t0 < G.num_task(); tsk_t0++)
    for (node_t t0 = G.task_tab[tsk_t0].start; t0 < G.task_tab[tsk_t0].end; t0 ++) 
    {
        G_Parent[t0] = gm_graph::NIL_NODE ;
        G_RParent[t0] = gm_graph::NIL_NODE ;
        G_FFinalized[t0] = false ;
        G_RFinalized[t0] = false ;
        G_FCost[t0] = INT_MAX ;
    }
    G_FCost[src] = 0 ;

    #pragma omp parallel
    #pragma omp single
    #pragma omp taskloop
    for (int tsk_t5 = 0; tsk_t5 < G.num_task(); tsk_t5++)
    for (node_t t5 = G.task_tab[tsk_t5].start; t5 < G.task_tab[tsk_t5].end; t5 ++) 
        G_RCost[t5] = INT_MAX ;

    G_RCost[dst] = 0 ;
    while ( !terminate && ((FReachable.size() != 0) || (RReachable.size() != 0)))
    {
        if ((FReachable.size() != 0) && ((FReachable.size() <= RReachable.size()) || (RReachable.size() == 0)))
        {
            node_t fnext;
            int32_t fdist = 0 ;

            fnext = FReachable.getMinKey_seq() ;
            G_FFinalized[fnext] = true ;
            FReachable.removeMinKey_seq();
            curMinFCost = G_FCost[fnext] ;
            if (curMinFCost + curMinRCost > minCost)
            {
                terminate = true ;
            }
            fdist = G_FCost[fnext] ;
            for (edge_t v_idx = G.begin[fnext];v_idx < G.begin[fnext+1] ; v_idx ++) 
            {
                node_t v = G.node_idx [v_idx];
                if ( !G_FFinalized[v])
                {
                    edge_t e;

                    e = v_idx ;
                    if (fdist + G_Weight[e] + curMinRCost <= minCost)
                    {
                        if (G_FCost[v] > fdist + G_Weight[e])
                        {
                            G_FCost[v] = fdist + G_Weight[e] ;
                            FReachable.setValue_seq(v, G_FCost[v]);
                            G_Parent[v] = fnext ;
                            G_ParentEdge[v] = e ;
                            if (G_RCost[v] != INT_MAX)
                            {
                                int32_t newCost = 0 ;

                                newCost = G_FCost[v] + G_RCost[v] ;
                                if (minCost > newCost)
                                {
                                    minCost = newCost ;
                                    mid = v ;
                                }
                            }
                        }
                    }
                }
            }
        }
        else if ((RReachable.size() != 0) && ((RReachable.size() <= FReachable.size()) || (FReachable.size() == 0)))
        {
            node_t rnext;
            int32_t rdist = 0 ;

            rnext = RReachable.getMinKey_seq() ;
            G_RFinalized[rnext] = true ;
            RReachable.removeMinKey_seq();
            curMinRCost = G_RCost[rnext] ;
            if (curMinFCost + curMinRCost > minCost)
            {
                terminate = true ;
            }
            rdist = G_RCost[rnext] ;
            for (edge_t v6_idx = G.r_begin[rnext];v6_idx < G.r_begin[rnext+1] ; v6_idx ++) 
            {
                node_t v6 = G.r_node_idx [v6_idx];
                if ( !G_RFinalized[v6])
                {
                    edge_t e7;

                    e7 = v6_idx ;
                    if (rdist + G_Weight[G.e_rev2idx[e7]] + curMinFCost <= minCost)
                    {
                        if (G_RCost[v6] > rdist + G_Weight[G.e_rev2idx[e7]])
                        {
                            G_RCost[v6] = rdist + G_Weight[G.e_rev2idx[e7]] ;
                            RReachable.setValue_seq(v6, G_RCost[v6]);
                            G_RParent[v6] = rnext ;
                            G_RParentEdge[v6] = G.e_rev2idx[e7] ;
                            if (G_FCost[v6] != INT_MAX)
                            {
                                int32_t newCost8 = 0 ;

                                newCost8 = G_FCost[v6] + G_RCost[v6] ;
                                if (minCost > newCost8)
                                {
                                    minCost = newCost8 ;
                                    mid = v6 ;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if (mid != gm_graph::NIL_NODE)
    {
        node_t cur;

        cur = mid ;
        while (cur != dst)
        {
            node_t prev;

            prev = G_RParent[cur] ;
            G_Parent[prev] = cur ;
            G_ParentEdge[prev] = G_RParentEdge[cur] ;
            cur = prev ;
        }
        gm_rt_cleanup();
        return true; 
    }
    gm_rt_cleanup();
    return false; 
}

int32_t get_path(gm_graph& G, node_t& src, 
    node_t& dst, node_t* G_prev_node, 
    edge_t* G_prev_edge, int32_t* G_edge_cost, 
    gm_node_seq&  Q)
{
    //Initializations
    gm_rt_initialize();
    G.freeze();

    int32_t total_cost = 0 ;
    node_t n;

    total_cost = 0 ;
    n = dst ;
    if (G_prev_node[dst] != gm_graph::NIL_NODE)
    {
        while (n != src)
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

