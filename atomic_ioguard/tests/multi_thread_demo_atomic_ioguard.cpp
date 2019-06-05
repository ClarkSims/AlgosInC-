/// \file multi_thread_demo_atomic_ioguard.cpp
/// \brief This is a demonstration of how to create a writer, that writes into a container of records, and a set of readers that reads those records. The writer creates a hash value in each record. The readers recalculate the hash value of the record, and verify that it is correct. This program also serves as a regression test.
#include "atomic_ioguard.h"
#include <iostream>
#include <assert.h>
#include <boost/thread/thread.hpp>
#include <boost/thread/barrier.hpp>

using namespace util_ipc;
using namespace std;

#define NTHREAD 1
#define NUMBER_READS 1000000
#define NUMBER_RECORDS 256
#define RECORD_BUFFER_LENGTH 32

struct record {
  uint64_t buffer_[RECORD_BUFFER_LENGTH];
  uint64_t hash_;
  uint64_t calc_hash() {
    uint64_t retval = 2345678;
    for (int i=0; i<RECORD_BUFFER_LENGTH; ++i) {
      retval = (retval * 64381 + buffer_[i]*61603 + 57287) % 54767;
    }
    return retval;
  }
  void randomize() {
    for (int i=0; i<RECORD_BUFFER_LENGTH; ++i) {
      buffer_[i] = rand();
    }
    hash_ = calc_hash();
  }
  bool is_valid() {
    uint64_t dupe_hash = calc_hash();
    return dupe_hash == hash_;
  }
};

atomic_ioguard<record> guarded_records[NUMBER_RECORDS];

struct writer {
  boost::barrier* barrier_;
  static volatile int stop_;

  writer( boost::barrier* barrier) : barrier_( barrier)
  {
  }

  void operator()() {
    barrier_->wait();
    record r;
    while (!stop_) {
      int offset = rand() % NUMBER_RECORDS;
      r.randomize();
      atomic_ioguard<record>::copy_to_rhs( r, &guarded_records[offset]);
    }
  }
};
    
volatile int writer::stop_ = 0;

struct reader {
  boost::barrier* barrier_;

  reader( boost::barrier* barrier) : barrier_( barrier)
  {
  }

  void operator()() {
    barrier_->wait();
    atomic_ioguard<record> r;
    for (int i=0; i<NUMBER_READS; ++i) {
      int offset = rand() % NUMBER_RECORDS;
      cout << "i = " << i << endl; 
      r.init_reverse_order( &guarded_records[offset]);
      assert( r.get_data().is_valid());
    }
  }
};

int main() {
  int i;
  srand( time(NULL));
  for (i=0; i<NUMBER_RECORDS; ++i) {
    guarded_records[i].get_data().randomize();
    assert(guarded_records[i].get_data().is_valid());
  }

  boost::barrier start_barrier(NTHREAD+1);

  boost::thread* writer_thread = new boost::thread( writer( &start_barrier));

  boost::thread* reader_threads[NTHREAD];
  for (i=0; i<NTHREAD; ++i) {
    reader_threads[i] = new boost::thread( reader( &start_barrier));
  }

  for (i=0; i<NTHREAD; ++i) {
    reader_threads[i]->join();
  }

  writer::stop_ = 1;
  writer_thread->join();

  cout << "test passed!" << endl;
}

/// \mainpage
/// The template class <a href="atomic__ioguard_8h_source.html">atomic_ioguard</a> 
/// is used for insure that data is correctly read in
/// a program where a data producer writes in one thread or process, and a reader reads
/// the data in another thread or process. No locks or mutexes are used. Synchronization is accomplished with
///  <a href="memory__fence_8h.html">memory fences</a>. and atomic writes and atomic reads. The algorith 
/// is as follows: <ul>
///  <li> The producer writes data, with the following algorithm, which is encapsulated in the overloaded functions copy_to_rhs: 
///   <ol>
///   <li> The producer increments the front guard and calls a memory sfence. This marks
///  the record as being in an unstable state. </li>
///   <li> The producer then writes the record and calls a memory sfence. </li>
///   <li> The producer then increments the back guard, which marks the record as being stable. </li>
///   </ol>
///   </li>
///   <li> The consumer uses the following algorithm to read data, which is encapsulated in the functions init_reverse_order, copy_to_rhs_reverse_order: <ol>
///   <li> copy rhs->back_guard to back_guard  </li>
///   <li> copy rhs->data to data and call a memory lfence </li>
///   <li> if rhs->front_guard != back_guard, do it again, else break </li>
///   </ol>
///   </li>
///   </ul>
///  <p> The program <a href="multi__thread__demo__atomic__ioguard_8cpp_source.html"> multi_thread_demo_atomic_io.cpp </a> is a both simple use case, and regression test. </p>


