// regex_match example
#include <iostream>
#include <string>
#include <regex>

#include "my_regex_match.h"

#define N_RAND_TEST 1000

bool test_both=false, test_custom=false;

using namespace std;

const char *bool2char(bool val) {
    return (val)? "true" : "false";
}

bool test( const char *txt, const char *pat) {
    bool rv_cust, rv_std;
    if (test_custom || test_both) {
        my_regex_match mrm(txt, pat);
        rv_cust = mrm.dp();
    }
    if (!test_custom || test_both) {
        rv_std = std::regex_match(txt, std::regex(pat));
    }
    if (test_both) {
        if (rv_cust != rv_std) {
            cerr << "txt     =" << txt << endl;
            cerr << "pat     =" << pat << endl;
            cerr << "rv_cust =" << bool2char(rv_cust) << endl;
            cerr << "rv_std  =" << bool2char(rv_std) << endl;
            exit(1);
        }
    }
    return (test_custom)? rv_cust : rv_std;
}



#define N 15
const char *lhs_true[N] {
  "a",
  "ab",
  "abc",
  "a",
  "ab",
  "abc",
  "aa",
  "aab",
  "aabc",
  "a",
  "ab",
  "abc",
  "",
  "abcccdeghhhhhii",
  ""
};

const char *rhs_true[N] {
  "a",
  "ab",
  "abc",
  ".",
  ".b",
  "a.c",
  "a*",
  "a*b",
  "a*bc",
  "a*",
  "a*b",
  "a*bc",
  "a*",
  "a*b*c*d*e*f*g*h*i*",
  ""
};

const char *lhs_false[N] {
  "ae",
  "aeb",
  "aebc",
  "ae",
  "eab",
  "aebc",
  "aae",
  "aaeb",
  "aaebc",
  "ae",
  "aeb",
  "abec",
  "e",
  "bar",
  "e"
};

const char *rhs_false[N] {
  "a",
  "ab",
  "abc",
  ".",
  ".b",
  "a.c",
  "a*",
  "a*b",
  "a*bc",
  "a*",
  "a*b",
  "a*bc",
  "a*",
  "foo",
  ""
};

using namespace std;

char offset2char(unsigned offset) { 
    if (offset < 26) {
        return 'a' + offset;
    } else if (offset < 52) {
        offset -= 26;
        return 'A' + offset;
    } else if (offset < 62) {
        offset -= 52;
        return '0' + offset;
    } else {
        throw std::runtime_error("offset too large");
    }
}

void rand_string( std::string& str) {
    unsigned len = rand() % 4096;
    str.resize(len);
    for (unsigned i =0; i<len; ++i) {
        unsigned off = rand() % 62;
        str.at(i) = offset2char(off);
    }
}

void rand_insert_periods( std::string& str, std::vector<unsigned>& changed) {
    unsigned off, stride;
    stride = str.size() / ((rand() + 1) % (str.size()-1) + 1);
    changed.clear();
    for (off = 0;;) {
        off += (rand() % stride) + 1;
        if (off >= str.size()) break;
        str.at(off) = '.';
        changed.push_back(off);
    }
}
 
void make_random_match( string& text, string& pat) {
    text.clear();
    pat.clear();
    int num_iter = rand() % 256 + 1;
    int choice, i, j, num_rep;
    char c;
    for (i =0; i<num_iter; ++i) {
        int choice = rand() % 10;
        c = offset2char(rand() % 62);
        switch (choice) {
            case 8:
                text.push_back(c);
                pat.push_back('.');
                break;
            case 9:
                num_rep = rand() % 5;
                for (j=0; j<num_rep; ++j) {
                    text.push_back(c);
                }
                pat.push_back(c);
                pat.push_back('*');
                break;
            default:
                text.push_back(c);
                pat.push_back(c);
                break;
        }
    }
}

int main (int argc, const char** argv)
{
    test_both = argc > 1 && strcmp("both", argv[1])==0;
    test_custom = argc > 1 && strcmp("my", argv[1])==0;
    size_t i;
    for (i=0; i<N; ++i) {
      if (!test(lhs_true[i], rhs_true[i])) {
        cerr << "should match, but don't" << lhs_true[i] << " " << rhs_true[i] << endl;
        return 1;
      }
    }
    for (i=0; i<N; ++i) {
      if (test(lhs_false[i], rhs_false[i])) {
        cerr << "should not match, but do" << lhs_true[i] << " " << rhs_true[i] << endl;
        return 1;
      }
    }

    string text, pat;
    std::vector<unsigned> changed;
    int off;
    for (i=0; i<N_RAND_TEST; ++i) {
        // test kein * and .
        make_random_match(text, pat);
        if (!test( text.c_str(), pat.c_str())) {
            cerr << "should match, but don't" << text << " " << pat << endl;
            return 1;
        }
#if 0
        if (!text.empty()) {
            do {
                off = rand() % (pat.size()-1);
                if (pat[off] != '.' && pat[off] != '+') {
                    ++pat[off];
                    break;
                }
            } while (true);
            if (test( text.c_str(), pat.c_str())) {
                cerr << "should not match, but do" << text << " " << pat << endl;
                return 1;
            }
        }
#endif            
        rand_string(text);
        pat = text;
        rand_insert_periods(pat, changed);
        if (!test( text.c_str(), pat.c_str())) {
            cerr << "should match, but don't" << text << " " << pat << endl;
            return 1;
        }
        if (!text.empty()) {
            do {
              off = rand() % text.size();
              ++text[off];
            } while (pat[off] == '.');
            if (test( text.c_str(), pat.c_str())) {
                cerr << "should not match, but do" << text << " " << pat << endl;
                return 1;
            }
        }
    }
  
    cout << "all test passed" << endl;
}
