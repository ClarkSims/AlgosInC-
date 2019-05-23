#include "boost/date_time/posix_time/posix_time.hpp"
//#include "CustomRand.h"
#include <stdlib.h>
#include <iostream>
#include <sys/mman.h>

using namespace std;
using namespace boost::posix_time;

#define N 1000000000

int main() {
  //ax_b_c_random rnd;
  char *mem = new char[N];
  boost::posix_time::ptime t0, t1;
  boost::posix_time::time_duration dt;
  int i, j;
  long dt_overhead, dt_sequential, dt_rand;

  //cout << RAND_MAX << endl;

  t0 = boost::posix_time::microsec_clock::universal_time();
  for (i=0; i<N; ++i) {
    rand();
  }
  t1 = boost::posix_time::microsec_clock::universal_time();
  dt = t1 - t0;
  dt_overhead = dt.total_microseconds();

  cout << "dt_overhead = " << dt_overhead << '\n';

  ///////////////////////////

  t0 = boost::posix_time::microsec_clock::universal_time();
  for (i=0; i<N; ++i) {
    mem[i] = rand();
  }
  t1 = boost::posix_time::microsec_clock::universal_time();
  dt = t1 - t0;
  dt_sequential = dt.total_microseconds();
  dt_sequential -= dt_overhead;
  cout << "dt_sequential = " << dt_sequential << '\n';

  ///////////////////////////

  t0 = boost::posix_time::microsec_clock::universal_time();
  for (i=0; i<N; ++i) {
    j = rand() % N;
    //cout << j << endl;
    mem[j] = i;
  }
  t1 = boost::posix_time::microsec_clock::universal_time();
  dt = t1 - t0;
  dt_rand = dt.total_microseconds();
  dt_rand -= dt_overhead;
  cout << "dt_rand = " << dt_rand << '\n';
  cout << "ratio = " << dt_rand / dt_sequential << endl;

  /////////////////////////////////

  madvise( mem, N, MADV_SEQUENTIAL);
  t0 = boost::posix_time::microsec_clock::universal_time();
  for (i=0; i<N; ++i) {
    mem[i] = rand();
  }
  t1 = boost::posix_time::microsec_clock::universal_time();
  dt = t1 - t0;
  dt_sequential = dt.total_microseconds();
  dt_sequential -= dt_overhead;
  cout << "dt_sequential = " << dt_sequential << '\n';

  ////////////////////////////////

  madvise( mem, N, MADV_RANDOM);
  t0 = boost::posix_time::microsec_clock::universal_time();
  for (i=0; i<N; ++i) {
    j = rand() % N;
    //cout << j << endl;
    mem[j] = i;
  }
  t1 = boost::posix_time::microsec_clock::universal_time();
  dt = t1 - t0;
  dt_rand = dt.total_microseconds();
  dt_rand -= dt_overhead;
  cout << "dt_rand = " << dt_rand << '\n';
  cout << "ratio = " << dt_rand / dt_sequential << endl;

}
