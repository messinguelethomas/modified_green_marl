#ifndef GM_GENERATED_CPP_SSSP_PATH_H
#define GM_GENERATED_CPP_SSSP_PATH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include <limits.h>
#include <cmath>
#include <algorithm>
#include <omp.h>
#include "gm.h"

void sssp_path(gm_graph& G, int32_t* G_dist, 
    int32_t* G_len, node_t& root, 
    node_t* G_prev);
void get_path(gm_graph& G, node_t& begin0, 
    node_t& end, node_t* G_prev, 
    gm_node_seq& Q);

#endif
