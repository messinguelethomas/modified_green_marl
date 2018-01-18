#include "avg_teen_cnt.h"

float avg_teen_cnt(gm_graph& G, int32_t* G_age, 
    int32_t* G_teen_cnt, int32_t K)
{
    //Initializations
    gm_rt_initialize();
    G.freeze();
    G.make_reverse_edges();

    float avg = 0.0 ;
    double _avg4 = 0.0 ;
    int64_t _cnt3 = 0 ;
    int32_t __S2 = 0 ;

    __S2 = 0 ;
    _cnt3 = 0 ;
    #pragma omp parallel
    {
        int32_t __S2_prv = 0 ;
        int64_t _cnt3_prv = 0 ;

        _cnt3_prv = 0 ;
        __S2_prv = 0 ;

        #pragma omp single
        #pragma omp taskloop grainsize(1)
        for (int tsk_n = 0; tsk_n < G.num_task(); tsk_n++)
        for (node_t n = G.task_tab[tsk_n].start; n < G.task_tab[tsk_n].end; n ++) 
        {
            int32_t __S1 = 0 ;

            __S1 = 0 ;
            for (edge_t t_idx = G.r_begin[n];t_idx < G.r_begin[n+1] ; t_idx ++) 
            {
                node_t t = G.r_node_idx [t_idx];
                if (((G_age[t] >= 10) && (G_age[t] < 20)))
                {
                    __S1 = __S1 + 1 ;
                }
            }
            G_teen_cnt[n] = __S1 ;
            if ((G_age[n] > K))
            {
                __S2_prv = __S2_prv + G_teen_cnt[n] ;
                _cnt3_prv = _cnt3_prv + 1 ;
            }
        }
        ATOMIC_ADD<int64_t>(&_cnt3, _cnt3_prv);
        ATOMIC_ADD<int32_t>(&__S2, __S2_prv);
    }
    _avg4 = (0 == _cnt3)?((float)(0.000000)):(__S2 / ((double)_cnt3)) ;
    avg = (float)_avg4 ;
    return avg; 
}

