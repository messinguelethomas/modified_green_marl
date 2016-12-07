#include "bc_adj.h"


// BFS/DFS definitions for the procedure
class comp_BC_bfs : public gm_bfs_template
    <short, true, false, false, true>
{
public:
    comp_BC_bfs(gm_graph& _G, node_t& _s, float*& _G_sigma, 
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
        if (v != s)
        {
            float __S3 = 0.0 ;

            __S3 = ((float)(0.000000)) ;
            for (edge_t w_idx = G.r_begin[v];w_idx < G.r_begin[v+1] ; w_idx ++) 
            {
                node_t w = G.r_node_idx [w_idx];
                if (get_level(w) != (get_curr_level() - 1)) continue;
                __S3 = __S3 + G_sigma[w] ;
            }
            G_sigma[v] = __S3 ;
        }
    }

    virtual void visit_rv(node_t v) 
    {
        if (v != s)
        {
            float __S4 = 0.0 ;

            __S4 = ((float)(0.000000)) ;
            for (edge_t w2_idx = G.begin[v];w2_idx < G.begin[v+1] ; w2_idx ++) 
            {
                if (!is_down_edge(w2_idx)) continue;
                node_t w2 = G.node_idx [w2_idx];
                __S4 = __S4 + G_sigma[v] / G_sigma[w2] * (1 + G_delta[w2]) ;
            }
            G_delta[v] = __S4 ;
            G_BC[v] = G_BC[v] + G_delta[v] ;
        }
    }

    virtual bool check_navigator(node_t v, edge_t v_idx) {return true;}


};

void comp_BC(gm_graph& G, float* G_BC)
{
    //Initializations
    gm_rt_initialize();
    G.freeze();
    G.make_reverse_edges();

    float* G_sigma = gm_rt_allocate_float(G.num_nodes(),gm_rt_thread_id());
    float* G_delta = gm_rt_allocate_float(G.num_nodes(),gm_rt_thread_id());


    #pragma omp parallel for
    for (node_t t0 = 0; t0 < G.num_nodes(); t0 ++) 
        G_BC[t0] = (float)0 ;

    for (node_t s = 0; s < G.num_nodes(); s ++) 
    {

        #pragma omp parallel for
        for (node_t t1 = 0; t1 < G.num_nodes(); t1 ++) 
            G_sigma[t1] = (float)0 ;

        G_sigma[s] = (float)1 ;

        comp_BC_bfs _BFS(G, s, G_sigma, G_delta, G_BC);
        _BFS.prepare(s, gm_rt_get_num_threads());
        _BFS.do_bfs_forward();
        _BFS.do_bfs_reverse();
    }

    gm_rt_cleanup();
}

