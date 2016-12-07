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

        #pragma omp for nowait
        for (node_t u = 0; u < G.num_nodes(); u ++) 
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

        #pragma omp for nowait
        for (node_t u0 = 0; u0 < G.num_nodes(); u0 ++) 
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

        #pragma omp for nowait schedule(dynamic,128)
        for (node_t u1 = 0; u1 < G.num_nodes(); u1 ++) 
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

