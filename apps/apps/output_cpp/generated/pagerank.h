#ifndef GM_GENERATED_CPP_PAGERANK_H
#define GM_GENERATED_CPP_PAGERANK_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include <limits.h>
#include <cmath>
#include <algorithm>
#include <omp.h>
#include "gm.h"

void pagerank(gm_graph& G, double e, 
    double d, int32_t max, 
    double*G_pg_rank);

#endif
