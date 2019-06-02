/// \file test7_circular_queue.cpp
/// \brief This program does a simple test to where blob data is put into a queue, and popped out. It is tested
///  to make sure the data coming out, is what it is supposed to be.
#include "circular_queue.h"

#include <iostream>

using namespace util_ipc;
using namespace std;

#define SIZE   1024
#define N      100
#define OFFSET 5

int main() 
{
  int32_t sz = circular_queue<int>::get_allocation_size( SIZE);
  circular_queue<char> *cq = circular_queue<char>::factory( (new char[sz]), "foo", SIZE);

  blob<char> * blob_1 = (blob<char> *)new char[SIZE];
  blob<char> * blob_2 = (blob<char> *)new char[SIZE];

  rand_fill_blob( blob_1, SIZE/4);
  cq->push_back_blob( blob_1);

  cq->pop_blob( blob_2);

  
