//g++ -std=c++11 -c -o prev_permutation.o prev_permutation.cpp
#include <algorithm>
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <assert.h>
#include "prev_permutation.h"

using namespace std;

vector<int> Solution::prevPermOpt1(vector<int>& A) {
    int *px, *px_lag, *a_begin, *a_back;
    px_lag = a_back = &*(A.end()-1);
    px = px_lag - 1;
    a_begin = &*A.begin();
    while (true) {
        if (*px > *px_lag)
            break;
        px_lag = px;
        --px;
        if (px_lag == a_begin)
            return A;
    }
    int *p_max = bisect_max_crit(*px, px_lag, a_back);

    // need to find left most value of p_max
    if (px_lag != p_max) {
        // find value to left of p_max, which is less than p_max
        // note that px_lag is the left most value for starters
        walk_left_find_val_lt(*p_max, px_lag, p_max);
        if (px_lag < p_max-1) {
            bisect_left( *p_max, px_lag, p_max);
            p_max = px_lag;
        }
    }

    *p_max ^= *px;
    *px ^= *p_max;
    *p_max ^= *px;
    return A;
}

