// tests.cpp
// g++ -odemo_gtest -g demo_gtest.cpp /usr/lib/libgtest.a -lpthread
// g++ -std=c++11 -I . -o test_prev_permutation prev_permutation.cpp test_prev_permutation.cpp /usr/lib/libgtest.a -lpthread
#include "prev_permutation.h"
#include <gtest/gtest.h>
#include <exception>

void do_test( vector<int>&A, vector<int>&C, bool verbose=false)
{
    Solution S;
    const vector<int>& B = S.prevPermOpt1(A);
    if (verbose) {
        cout << "B= ";
        std::copy(B.begin(), B.end(), std::ostream_iterator<int>(cout, ","));
        cout << endl;
        cout << "C= ";
        std::copy(C.begin(), C.end(), std::ostream_iterator<int>(cout, ","));
    }
    ASSERT_EQ(B.size(), C.size());
    for (unsigned i=0; i<B.size(); ++i) {
        ASSERT_EQ(B[i], C[i]);
    }
}

TEST(PrevPermutation, v321) {
    vector<int> A = {3,2,1};
    std::vector<int> C = {3,1,2};
    do_test(A, C);
}

TEST(PrevPermutation, v115) {
    vector<int> A = {1,1,5};
    std::vector<int> C = {1,1,5};
    do_test(A, C);
}

TEST(PrevPermutation, v19467) {
    vector<int> A = {1,9,4,6,7};
    std::vector<int> C = {1,7,4,6,9};
    do_test(A, C);
}

TEST(PrevPermutation, v3113) {
    vector<int> A = {3,1,1,3};
    std::vector<int> C = {1,3,1,3};
    do_test(A, C, true);
}

TEST(BisectMaxCrit, c3_0to5) {
    vector<int> range = {0,1,2,3,4,5};
    int* pcrit = bisect_max_crit(3, &range[0], &range[5]);
    ASSERT_EQ(pcrit, &range[2]);
}

TEST(BisectMaxCrit, c6_0to5_rep2) {
    vector<int> range = {0,1,2,2,3,4,5};
    int* pcrit = bisect_max_crit(6, &range[0], &range[6]);
    ASSERT_EQ(pcrit, &*(range.end()-1));
}

TEST(walk_left_find_val_lt, v2_2222) {
    vector<int> range(4,2);
    int *left = &*range.begin();
    int *right = &*range.end()-1;
    walk_left_find_val_lt(2, left, right);
    ASSERT_EQ(left, &*range.begin());
    ASSERT_EQ(right, &*range.begin());
}

TEST(walk_left_find_val_lt, v2_1222) {
    vector<int> range = {1,2,2,2};
    int *left = &*range.begin();
    int *right = &*range.end()-1;
    walk_left_find_val_lt(2, left, right);
    ASSERT_EQ(left, &*range.begin());
    ASSERT_EQ(right, left+2);
}

TEST(bisect_left, v2_12234) {
    vector<int> range = {1,2,2,3,4};
    int *left = &*range.begin();
    int *right = &*range.end()-1;
    int *pval = bisect_left(2, left, right);
    ASSERT_EQ(pval, left+1);
}

TEST(bisect_left, v2_1234) {
    vector<int> range = {1,2,3,4};
    int *left = &*range.begin();
    int *right = &*range.end()-1;
    int *pval = bisect_left(2, left, right);
    ASSERT_EQ(pval, left+1);
}

TEST(bisect_left, v2_234) {
    vector<int> range = {2,3,4};
    int *left = &*range.begin();
    int *right = &*range.end();
    int *pval = bisect_left(2, left, right);
    ASSERT_EQ(pval, left);
}

TEST(bisect_left, v2_34) {
    vector<int> range = {3,4};
    int *left = &*range.begin();
    int *right = &*range.end();
    int *pval = bisect_left(2, left, right);
    ASSERT_EQ(pval, left);
}

TEST(bisect_left, v2_134) {
    vector<int> range = {1,2,3,4};
    int *left = &*range.begin();
    int *right = &*range.end();
    int *pval = bisect_left(2, left, right);
    ASSERT_EQ(pval, left+1);
}

TEST(bisect_left, v5_01234) {
    vector<int> range = {0,1,2,3,4};
    int *left = &*range.begin();
    int *right = &*range.end();
    int *pval = bisect_left(5, left, right);
    ASSERT_EQ(pval, left+5);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
