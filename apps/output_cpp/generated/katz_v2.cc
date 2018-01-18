#include "katz_v2.h"

int32_t katz(gm_graph& G, float Beta, 
    int32_t Max_l)
{
    //Initializations
    gm_rt_initialize();
    G.freeze();


    #pragma omp parallel
    #pragma omp single
    #pragma omp taskloop
    for (int tsk_n = 0; tsk_n < G.num_task(); tsk_n++)
    for (node_t n = G.task_tab[tsk_n].start; n < G.task_tab[tsk_n].end; n ++) 
    {

                               katz_score_node(G,Beta,Max_l,n);
                        ;
    }
    return 0; 
}

void katz_score_node(gm_graph& G, float beta, 
    int32_t max_l, node_t& x)
{
    //Initializations
    gm_rt_initialize();
    G.freeze();

    gm_node_set neighbor_set_1(G.num_nodes());
    gm_node_set temp_set(G.num_nodes());
    gm_node_set all_reacheable_neighbor(G.num_nodes());
    gm_node_set neighbor_set_2(G.num_nodes());
    gm_node_set direct_neighbor(G.num_nodes());
    gm_map_medium<node_t, int32_t> map_path_len_1(gm_rt_get_num_threads(),  0);
    gm_map_medium<node_t, int32_t> map_path_len_2(gm_rt_get_num_threads(),  0);
    gm_map_medium<node_t, int32_t> map_temp(gm_rt_get_num_threads(),  0);
    gm_map_medium<node_t, float> map_katz(gm_rt_get_num_threads(),  0.0);
    int32_t l = 0 ;
    int32_t len_map_1 = 0 ;
    int32_t len_map_2 = 0 ;
    int32_t i = 0 ;
    node_t key;

    l = 2 ;
    for (edge_t y_idx = G.begin[x];y_idx < G.begin[x+1] ; y_idx ++) 
    {
        node_t y = G.node_idx [y_idx];
        map_path_len_1.setValue_seq(y, 1);
        neighbor_set_1.add_seq(y);
        direct_neighbor.add_seq(y);
    }
    while (l <= max_l)
    {
        i = 0 ;
        len_map_1 = map_path_len_1.size() ;
        gm_node_set::seq_iter y0_I = neighbor_set_1.prepare_seq_iteration();
        while (y0_I.has_next())
        {
            node_t y0 = y0_I.get_next();
            for (edge_t z_idx = G.begin[y0];z_idx < G.begin[y0+1] ; z_idx ++) 
            {
                node_t z = G.node_idx [z_idx];
                if (neighbor_set_2.is_in(z))
                    map_path_len_2.setValue_seq(z, map_path_len_2.getValue(z) + map_path_len_1.getValue(y0));
                else 
                {
                    map_path_len_2.setValue_seq(z, map_path_len_1.getValue(y0));
                    neighbor_set_2.add_seq(z);
                }
            }
        }
        len_map_2 = map_path_len_2.size() ;
        gm_node_set::seq_iter t_I = neighbor_set_2.prepare_seq_iteration();
        while (t_I.has_next())
        {
            node_t t = t_I.get_next();
            if ( !direct_neighbor.is_in(t) && (t != x))
            {
                map_katz.setValue_seq(t, (float)(map_katz.getValue(t) + pow(beta, l) * ((double)map_path_len_2.getValue(t))));
                all_reacheable_neighbor.add_seq(t);
            }
        }
        neighbor_set_1 = neighbor_set_2 ;
        neighbor_set_2 = temp_set ;
        l = l + 1 ;
        while (i < len_map_1)
        {
            map_path_len_1.removeKey_seq(map_path_len_1.getMinKey_seq());
            i = i + 1 ;
        }
        i = 0 ;
        while (i < len_map_2)
        {
            key = map_path_len_2.getMinKey_seq() ;
            map_path_len_1.setValue_seq(key, map_path_len_2.getValue(key));
            map_path_len_2.removeKey_seq(map_path_len_2.getMinKey_seq());
            i = i + 1 ;
        }
    }
}

