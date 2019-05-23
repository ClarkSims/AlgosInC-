#ifndef precision_clock_h
#define precision_clock_h

#ifdef  __GNUC__
#include <sys/time.h>
#include <stdint.h>
#elif defined(_MSC_VER)
#include <windows.h>
#include "win_stdint.h"
#endif

#include <string.h>
#include <boost/date_time.hpp>

namespace util_ipc {

  class precision_clock {
#ifdef  __GNUC__
    timeval _timeval;
#endif
    uint64_t _frequency; // = change in precision counter / second
    uint64_t _m1;
    uint64_t _n0;
    uint64_t _n1;
  public:
    precision_clock() {
      _m1 = boost::posix_time::microsec_clock::universal_time().time_of_day().total_microseconds();
#ifdef  __GNUC__
      _frequency = 1000;  // conversion to microseconnds
      gettimeofday( &_timeval, 0);
      _n1 = (uint64_t)_timeval.tv_sec*1000000000 + _timeval.tv_usec*1000;
#elif defined(_MSC_VER)
      QueryPerformanceFrequency( reinterpret_cast<LARGE_INTEGER*>( &_frequency));
      _frequency /= 1000000; // conversion to microseconds
      QueryPerformanceCounter( reinterpret_cast<LARGE_INTEGER*>( &_n1));
#endif
      _n0 = _n1 - _frequency * _m1;
    }

    uint64_t nanoseconds_since_midnight() {
#ifdef  __GNUC__
      gettimeofday( &_timeval, 0);
      _n1 = (uint64_t)_timeval.tv_sec*1000000000 + _timeval.tv_usec*1000;
#else
      QueryPerformanceCounter( reinterpret_cast<LARGE_INTEGER*>( &_n1));
#endif
      return (_n1 - _n0) * 1000 / _frequency;
    }

    double microseconds_since_midnight_as_double() {
#ifdef  __GNUC__
      gettimeofday( &_timeval, 0);
      _n1 = (uint64_t)_timeval.tv_sec*1000000000 + _timeval.tv_usec*1000;
#else
      QueryPerformanceCounter( reinterpret_cast<LARGE_INTEGER*>( &_n1));
#endif
      return (double)(_n1 - _n0) / _frequency;
    }

    uint64_t microseconds_since_midnight_as_uint64() {
#ifdef  __GNUC__
      gettimeofday( &_timeval, 0);
      _n1 = (uint64_t)_timeval.tv_sec*1000000000 + _timeval.tv_usec*1000;
#else
      QueryPerformanceCounter( reinterpret_cast<LARGE_INTEGER*>( &_n1));
#endif
      return (_n1 - _n0) / _frequency;
    }

    void clear() {
      memset( this, 0, sizeof(*this));
    }
  };
}

#endif
