#include "bc_random.h"


// BFS/DFS definitions for the procedure
class bc_random_bfs : public gm_bfs_template
    <short, true, false, false, true>
{
public:
    bc_random_bfs(gm_graph& _G, node_t& _s, float*& _G_sigma, 
        float*& _G_delta, float*& _G_BC)
    : gm_bfs_template<short, true, false, false, true>(_G),
    G(_G), s(_s), G_sigma(_G_sigma), G_delta(_G_delta), G_BC(_G_BC){}

private:  // list of varaibles
    gm_graph& G;
    node_t& s;
    float*& G_sigma;
    float*& G_delta;
    float*& G_BC;

protected:
    virtual void visit_fw(node_t v) 
    {
        {
            for (edge_t w_idx = G.begin[v];w_idx < G.begin[v+1] ; w_idx ++) 
            {
                if (!is_down_edge(w_idx)) continue;
                node_t w = G.node_idx [w_idx];
                float sigma_w_prv = 0.0 ;

                sigma_w_prv = ((float)(0.000000)) ;
                sigma_w_prv = sigma_w_prv + G_sigma[v] ;
                ATOMIC_ADD<float>(&G_sigma[w], sigma_w_prv);
            }
        }
    }

    virtual void visit_rv(node_t v) 
    {
        float __S3 = 0.0 ;

        __S3 = ((float)(0.000000)) ;
        for (edge_t w2_idx = G.begin[v];w2_idx < G.begin[v+1] ; w2_idx ++) 
        {
            if (!is_down_edge(w2_idx)) continue;
            node_t w2 = G.node_idx [w2_idx];
            __S3 = __S3 + G_sigma[v] / G_sigma[w2] * (1 + G_delta[w2]) ;
        }
        G_delta[v] = __S3 ;
        G_BC[v] = G_BC[v] + G_delta[v] ;
    }

    virtual bool check_navigator(node_t v, edge_t v_idx) {return true;}


};

void bc_random(gm_graph& G, float* G_BC, 
    int32_t K)
{
    //Initializations
    gm_rt_initialize();
    G.freeze();

    int32_t k = 0 ;
    float* G_sigma = gm_rt_allocate_float(G.num_nodes(),gm_rt_thread_id());
    float* G_delta = gm_rt_allocate_float(G.num_nodes(),gm_rt_thread_id());

    k = 0 ;

    #pragma omp parallel for
    for (int tsk_t0 = 0; tsk_t0 < G.num_task(); tsk_t0++)
    for (node_t t0 = G.task_tab[tsk_t0].start; t0 < G.task_tab[tsk_t0].end; t0 ++) 
        G_BC[t0] = (float)0 ;

    while (k < K)
    {
        node_t s;

        s = G.pick_random_node() ;

        #pragma omp parallel for
        for (int tsk_t1 = 0; tsk_t1 < G.num_task(); tsk_t1++)
        for (node_t t1 = G.task_tab[tsk_t1].start; t1 < G.task_tab[tsk_t1].end; t1 ++) 
            G_sigma[t1] = (float)0 ;

        G_sigma[s] = (float)1 ;

        bc_random_bfs _BFS(G, s, G_sigma, G_delta, G_BC);
        _BFS.prepare(s, gm_rt_get_num_threads());
        _BFS.do_bfs_forward();
        _BFS.do_bfs_reverse();
        k = k + 1 ;
    }

    gm_rt_cleanup();
}

