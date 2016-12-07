#ifndef GM_GENERATED_CPP_BIDIR_DIJKSTRA_H
#define GM_GENERATED_CPP_BIDIR_DIJKSTRA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include <limits.h>
#include <cmath>
#include <algorithm>
#include <omp.h>
#include "gm.h"

bool bidir_dijkstra(gm_graph& G, int32_t* G_Weight, 
    node_t& src, node_t& dst, 
    node_t*G_Parent, edge_t*G_ParentEdge);
int32_t get_path(gm_graph& G, node_t& src, 
    node_t& dst, node_t* G_prev_node, 
    edge_t* G_prev_edge, int32_t* G_edge_cost, 
    gm_node_seq&  Q);

#endif
