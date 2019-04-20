#include <string.h>
#include "my_regex_match.h"

void my_regex_match::init(const char *text_in, const char *pat_in) {
    text = text_in;
    pat = pat_in;
    text_len = strlen(text);
    pat_len = strlen(pat);
    num_memos = (text_len+1)*(pat_len+1);
    if (num_memos != memo.size()) {
        memo.resize(num_memos);
        memo_done.resize(num_memos);
    }
    std::fill(memo_done.begin(), memo_done.end(), false);
    for (last_dot = pat_len - 1; last_dot >= 0 && pat[last_dot] != '.'; --last_dot) { }
    for (last_star = pat_len - 1; last_star >= 0 && pat[last_star] != '*'; --last_star) { }
}

my_regex_match::my_regex_match(const char* txtin, const char *patin) {
    if (txtin) {
        init( txtin, patin);
    } else {
        set_null();
    }
}

void my_regex_match::set_null() {
    text = pat = 0;
    text_len = pat_len = num_memos = last_dot = last_star = 0;
    memo.clear();
    memo_done.clear();
}

bool my_regex_match::match_dot(unsigned i, unsigned j) {
    for (;i < text_len; ++i, ++j) {
        if (text[i] != pat[j] && pat[j] != '.') {
            return false;
        }
    }
    return true;
}

bool my_regex_match::dp(unsigned i, unsigned j) {

    // nothing works if memory is out of bounds
    if (i > text_len || j > pat_len) {
        return false;
    }

    auto offset = [&, this](unsigned text_off, unsigned pat_off) -> unsigned { return text_off + pat_off * (this->text_len+1); };

    unsigned off_i_j = offset(i, j);
    if (memo_done[off_i_j])
        return memo[offset(i, j)];
    bool ans;

    // optimized algorithm if no stars left
    if (j > last_star) {
        if ((pat_len - j) != (text_len - i)) {
            memo[offset(i, j)] = false;
            return false;
        }
        if (j > last_dot) {
            ans = memcmp( text + i, pat + j, text_len - i) == 0;
        } else {
            ans = match_dot(i, j);
        }
        memo[offset(i, j)] = ans;
        return ans;
    }

    if (j == pat_len) {
        ans = i == text_len;
    } else {

        bool first_match = text[i] == pat[j] || pat[j] == '.';

        if (j + 1 < pat_len && pat[j+1] == '*') {
            ans = (first_match && dp(i + 1, j))   // one or matches of *
                || dp(i, j + 2); // 0 matches of *
        } else {
            ans = first_match && dp(i + 1, j + 1); // compare char by char
        }
    }
    memo_done[off_i_j] = true;
    memo[off_i_j] = ans;
    return ans;
}
