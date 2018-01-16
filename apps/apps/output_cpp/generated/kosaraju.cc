#include "kosaraju.h"


// BFS/DFS definitions for the procedure
class kosaraju_dfs : public gm_dfs_template
    <false, true, true, false>
{
public:
    kosaraju_dfs(gm_graph& _G, bool*& _G_Checked, gm_node_seq_vec& _Seq, 
        node_t& _t)
    : gm_dfs_template<false, true, true, false>(_G),
    G(_G), G_Checked(_G_Checked), Seq(_Seq), t(_t){}

private:  // list of varaibles
    gm_graph& G;
    bool*& G_Checked;
    gm_node_seq_vec& Seq;
    node_t& t;

protected:
    virtual void visit_pre(node_t n) {}
    virtual void visit_post(node_t n) 
    {
        G_Checked[n] = true ;
        Seq.push_back(n);
    }

    virtual bool check_navigator(node_t n, edge_t n_idx) 
    {
        return ( !G_Checked[n]);
    }


};

class kosaraju_bfs : public gm_bfs_template
    <short, true, true, true, false>
{
public:
    kosaraju_bfs(gm_graph& _G, int32_t& _compId, node_t& _t2, 
        int32_t*& _G_mem)
    : gm_bfs_template<short, true, true, true, false>(_G),
    G(_G), compId(_compId), t2(_t2), G_mem(_G_mem){}

private:  // list of varaibles
    gm_graph& G;
    int32_t& compId;
    node_t& t2;
    int32_t*& G_mem;

protected:
    virtual void visit_fw(node_t n3) 
    {
        G_mem[n3] = compId ;
    }

    virtual void visit_rv(node_t n3) {}
    virtual bool check_navigator(node_t n3, edge_t n3_idx) 
    {
        return (G_mem[n3] ==  -1);
    }


};

int32_t kosaraju(gm_graph& G, int32_t* G_mem)
{
    //Initializations
    gm_rt_initialize();
    G.freeze();
    G.make_reverse_edges();

    gm_node_seq_vec Seq(gm_rt_get_num_threads());
    int32_t compId = 0 ;
    bool* G_Checked = gm_rt_allocate_bool(G.num_nodes(),gm_rt_thread_id());

    compId = 0 ;

    #pragma omp parallel for
    for (int tsk_t0 = 0; tsk_t0 < G.num_task(); tsk_t0++)
    for (node_t t0 = G.task_tab[tsk_t0].start; t0 < G.task_tab[tsk_t0].end; t0 ++) 
    {
        G_mem[t0] =  -1 ;
        G_Checked[t0] = false ;
    }
    for (int tsk_t = 0; tsk_t < G.num_task(); tsk_t++)
    for (node_t t = G.task_tab[tsk_t].start; t < G.task_tab[tsk_t].end; t ++) 
    {
        if ( !G_Checked[t])
        {

            kosaraju_dfs _DFS(G, G_Checked, Seq, t);
            _DFS.prepare(t);
            _DFS.do_dfs();
        }
    }
    gm_node_seq_vec::seq_iter t2_I = Seq.prepare_seq_iteration();
    while (t2_I.has_next())
    {
        node_t t2 = t2_I.get_next();
        if (G_mem[t2] ==  -1)
        {

            kosaraju_bfs _BFS(G, compId, t2, G_mem);
            _BFS.prepare(t2, gm_rt_get_num_threads());
            _BFS.do_bfs_forward();
            compId = compId + 1 ;
        }
    }
    gm_rt_cleanup();
    return compId; 
}

