#include "conduct.h"

float conduct(gm_graph& G, int32_t* G_member, 
    int32_t num)
{
    //Initializations
    gm_rt_initialize();
    G.freeze();

    float m = 0.0 ;
    int32_t __S2 = 0 ;
    int32_t __S3 = 0 ;
    int32_t __S4 = 0 ;

    __S2 = 0 ;
    __S3 = 0 ;
    __S4 = 0 ;
    #pragma omp parallel
    {
        int32_t __S2_prv = 0 ;

        __S2_prv = 0 ;

        #pragma omp single
        #pragma omp taskloop
        for (int tsk_u = 0; tsk_u < G.num_task(); tsk_u++)
        for (node_t u = G.task_tab[tsk_u].start; u < G.task_tab[tsk_u].end; u ++) 
            if ((G_member[u] == num))
            {
                __S2_prv = __S2_prv + (G.begin[u+1] - G.begin[u]) ;
            }

        ATOMIC_ADD<int32_t>(&__S2, __S2_prv);
    }
    #pragma omp parallel
    {
        int32_t __S3_prv = 0 ;

        __S3_prv = 0 ;

        #pragma omp single
        #pragma omp taskloop
        for (int tsk_u0 = 0; tsk_u0 < G.num_task(); tsk_u0++)
        for (node_t u0 = G.task_tab[tsk_u0].start; u0 < G.task_tab[tsk_u0].end; u0 ++) 
            if ((G_member[u0] != num))
            {
                __S3_prv = __S3_prv + (G.begin[u0+1] - G.begin[u0]) ;
            }

        ATOMIC_ADD<int32_t>(&__S3, __S3_prv);
    }
    #pragma omp parallel
    {
        int32_t __S4_prv = 0 ;

        __S4_prv = 0 ;

        #pragma omp single
        #pragma omp taskloop grainsize(1)
        for (int tsk_u1 = 0; tsk_u1 < G.num_task(); tsk_u1++)
        for (node_t u1 = G.task_tab[tsk_u1].start; u1 < G.task_tab[tsk_u1].end; u1 ++) 
            if ((G_member[u1] == num))
            {
                for (edge_t j_idx = G.begin[u1];j_idx < G.begin[u1+1] ; j_idx ++) 
                {
                    node_t j = G.node_idx [j_idx];
                    if ((G_member[j] != num))
                    {
                        __S4_prv = __S4_prv + 1 ;
                    }
                }
            }

        ATOMIC_ADD<int32_t>(&__S4, __S4_prv);
    }
    m = (float)((__S2 < __S3)?__S2:__S3) ;
    if (m == 0)
        return (__S4 == 0)?((float)(0.000000)):FLT_MAX; 
    else 
        return ((float)__S4) / m; 
}

