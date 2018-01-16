#ifndef GM_GENERATED_CPP_SSSP_PATH_ADJ_H
#define GM_GENERATED_CPP_SSSP_PATH_ADJ_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include <limits.h>
#include <cmath>
#include <algorithm>
#include <omp.h>
#include "gm.h"

void sssp_path(gm_graph& G, double* G_dist, 
    double* G_edge_cost, node_t& root, 
    node_t& end, node_t* G_prev_node, 
    edge_t* G_prev_edge);
double get_path(gm_graph& G, node_t& begin0, 
    node_t& end, node_t* G_prev_node, 
    edge_t* G_prev_edge, double* G_edge_cost, 
    gm_node_seq& Q);

#endif
