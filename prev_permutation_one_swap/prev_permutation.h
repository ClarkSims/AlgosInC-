#ifndef prev_permutation_h
#define prev_permutation_h
#include <vector>
#include <stdexcept>

using namespace std;

/// assumes: ints in range are monotonically non-decreasing
/// returns: pointer to maximum value which is less than crit
inline int* bisect_max_crit( int crit, int* pbegin, int* pback) {
    if (*pback < crit) {
        return pback;
    }
    int *pmid = (pback - pbegin + 1)/2 + pbegin;

    while (pbegin < pback) {
        if (*pmid < crit)
            pbegin = pmid;
        else
            pback = pmid - 1;
        pmid = (pback - pbegin + 1)/2 + pbegin;
    }
    return pmid;
}

class Solution {
public:
    vector<int> prevPermOpt1(vector<int>& A);
};

/// This function starts with the range [beg,pval] and returns a smaller interval [beg,pval]
/// where *pval is the same, ie, val, and beg is shifted right
/// The algorithm is to walk left from pval, if test val < pval, then return
/// else shift pval left, and double step size to find next test_val
inline void walk_left_find_val_lt( int val, int*& beg, int*& pval) {
    if (*pval != val) {
        throw std::runtime_error("*pval != val in walk_left_find_val_lt");
    }
    if (val == *beg) {
        pval = beg;
        return;
    }
    int step = 2;
    int *probe, *pend = pval;
    probe = pval - 1;
    if (*probe != *pval) {
        beg = pval;
        return;
    }
    while (probe > beg) {
        if (*probe < val) {
            beg = probe;
            break;
        } else {
            pval = probe;
        }
        step <<= 1; // *= 2
        probe = pend - step;
    }
}


/// this is the basic bisect_left algorithm 
/// https://github.com/python/cpython/blob/master/Lib/bisect.py 
inline int *bisect_left(int val, int *beg, int*back) {
     int *mid;
     while (beg < back) {
        mid = (back - beg) / 2 + beg;
        if (*mid >= val) {
            back = mid;
        } else {
            beg = mid + 1;   
        }
     }
     return beg;
}

#endif
