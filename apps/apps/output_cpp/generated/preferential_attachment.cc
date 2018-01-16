#include "preferential_attachment.h"

void preferential_attachment(gm_graph& G)
{
    //Initializations
    gm_rt_initialize();
    G.freeze();

    for (int tsk_n1 = 0; tsk_n1 < G.num_task(); tsk_n1++)
    for (node_t n1 = G.task_tab[tsk_n1].start; n1 < G.task_tab[tsk_n1].end; n1 ++) 
    {

        #pragma omp parallel
        #pragma omp single
        #pragma omp taskloop
        for (int tsk_n2 = 0; tsk_n2 < G.num_task(); tsk_n2++)
        for (node_t n2 = G.task_tab[tsk_n2].start; n2 < G.task_tab[tsk_n2].end; n2 ++) 
        {
            if (n1 < n2)
            {
                int32_t pref_attach = 0 ;

                pref_attach = (G.begin[n1+1] - G.begin[n1]) * (G.begin[n2+1] - G.begin[n2]) ;
                std::cout<<n1<<" "<<n2<<" "<<pref_attach<<std::endl;
                printf("{(%d,%d),%d}\n",n1,n2,pref_attach);
            }
        }
    }
}

