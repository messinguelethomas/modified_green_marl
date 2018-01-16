#ifndef GM_GENERATED_CPP_KATZ_V2_H
#define GM_GENERATED_CPP_KATZ_V2_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include <limits.h>
#include <cmath>
#include <algorithm>
#include <omp.h>
#include "gm.h"

int32_t katz(gm_graph& G, float Beta, 
    int32_t Max_l);
void katz_score_node(gm_graph& G, float beta, 
    int32_t max_l, node_t& x);

#endif
