// regex_match example
#include <iostream>
#include <string>
#include <regex>

#include "my_regex_match.h"

using namespace std;

int main (int argc, const char** argv)
{
//    const char *txt = "IPaC9JOOOQeXP2DlfdVqotHJoqqNGnnnSuwIwIPYbyspHAnVGPiDDDDbI4dhkFoCCC3LQPunSJ99vmauFryyyycSnwTTSJSjy0h";
//    const char *pat = "IP.C9JO*QeXP2DlfdV.otHJ.qqN*Gn*S.wIwIPYbys.HAnVGPiD*bI4dh.FoC*3LK*QPunSJ9.vmau.ry*cSnwc*T*S.Sjy0h";
//    const char *txt = "bbbba";
//    const char *pat = ".*a*a";
    const char *txt = "";
    const char *pat = ".";
    my_regex_match mrm(txt, pat);
    bool rv = mrm.dp();
    if (!rv) {
        cout << "all test passed" << endl;
    } else {
        cout << "fail" << endl;
    }
}
