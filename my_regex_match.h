#ifndef my_regex_match_h
#define my_regex_match_h

#include <vector>

struct my_regex_match {
    void init(const char *lhs, const char *rhs);
    void set_null();
    my_regex_match( const char* txt, const char *pat);

    bool dp( unsigned i=0, unsigned j=0);
    bool match_dot(unsigned i, unsigned j);

    const char *text;
    const char *pat;
    unsigned text_len;
    unsigned pat_len;
    unsigned num_memos;
    int last_dot;
    int last_star;
    std::vector<bool> memo;
    std::vector<bool> memo_done;
};

#endif

