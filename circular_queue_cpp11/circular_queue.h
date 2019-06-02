/// \file circular_queue.h
/// \brief This file has all of the code for the template class, circular_queue. This class offers a lock free
///  mechanism for sharing data between a producer and a consumer, which is thread safe so long as the
///  the producer, only produces, and the consumer only consumes.

#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H

#include "architecture.h"
#ifdef ARCH_X86
#include "memory_fence.h"
#endif

#include <atomic> // only used in atomic_push_back_get_prev_end
#include <type_traits>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdexcept>

namespace util_ipc {

  ///  \class circular_queue<T>
  ///  \brief A partially thread safe circular queue, which has no locks or mutexes.
  ///
  /// This class serves as a queue, which sits in a single contiguous piece of memory.
  /// It is threadsafe to the limited degree, that if there is a snigle producer,and a single
  /// consumer, there will be no race conditions. It was designed with the intention of being
  /// placed in shared memory, and accessed by a single producer, and a singler consumer of data.
  /// There are no mutexes, locks or sleeps, so this class should be much faster than a fully
  /// thread safe class, which would have to use locks and mutexes.
  template <class T, unsigned long  _size, class Int = uint32_t, unsigned cache_line_size = 64>
    class circular_queue {
public:
    volatile Int  _head_offset;
    char _head_pad[ cache_line_size - sizeof(Int)];
    volatile Int  _tail_offset;
    char _tail_pad[ cache_line_size - sizeof(Int)];
    T volatile _data[_size];

  circular_queue( bool call_inplace_new = false) :
      _head_offset(0),
      _tail_offset(0)
    {
        if (call_inplace_new) {
            uint32_t i;
            for (i=0; i<_size; ++i) {
               new( (void*)&_data[i]) T;
            }
        }
    }

  public:

    static circular_queue* factory( void *memory_blob, const char *Name) {
      return new( (void*)memory_blob) circular_queue( Name);
    }

    ~circular_queue() {
      int32_t i;
      for (i=0; i<_size; ++i) {
        _data[i].~T();
      }
      _head_offset = _tail_offset = 0;
    }

    Int size() const { return _tail_offset - _head_offset;}

    Int begin_offset() const { return _head_offset; }

    Int end_offset() const { return _tail_offset; }

    /// <b> Assumes:</b> the object has been properly initialized. <br/>
    /// <b> Changes:</b> nothing <br/>
    /// <b> Description:</b>
    ///      This function returns a pointer to an unused object.  If no space is available
    ///     then it returns 0.
    T *end() {
      if (size() >= _size) return nullptr;
      return (T*) &_data[_tail_offset % _size];
    }

    /// <b> Assumes:</b> the object has been properly initialized. <br/>
    /// <b> Changes:</b> nothing <br/>
    /// <b> Description:</b>
    ///     This function returns a pointer to an unused object.  If no space is available
    ///     then it loops around into the used space, and will clobber a used object if
    ///     another object is pushed to the end.
    T* end_clobber() {
        return (T*) &_data[_tail_offset % _size];
    }

    /// <b> Assumes: </b> the object has been properly initialized. <br/>
    /// <b> Changes: </b> nothing
    /// <b> Description: </b> This function returns a pointer to the oldest object in the queue.
    ///  If the queue is empty it returns a nullptr. The pointer is a const pointer, because objects
    /// in the queue, are only intended to be read from the front, and written from the back.
    ///  It will also return nullptr if the beginnig is clobbered.
    T *begin() {
      if (_tail_offset >= _head_offset) return nullptr; // begin is clobbered or queue is empty
      return (T *)&_data[_head_offset % _size];
    }

    T& front() {
      if (_tail_offset == _head_offset) {
        throw std::range_error( "attempt to return the beginning of an empty queue");
      }
      return (T&)_data[_head_offset % _size];
    }

    T& back() {
      if (_tail_offset == _head_offset) {
        throw std::range_error( "attempt to return the end of a queue which is empty");
      }
      return (T&)_data[(_tail_offset-1) % _size];
    }      

    bool empty() const {
      return (_tail_offset == _head_offset);
    }

    /// <b> Assumes: </b> *end has been correctly initialized, and the queue is not full <br/>
    /// <b> Changes: </b> _tail_offset <br/>
    /// <b> On Error: </b> no error checking is done <br/>
    /// <b> Description: </b> This function assumes that end() has been called, and the pointer has been
    /// initialized. It pushes that datum onto the back of the queue. No error checking is done.
    /// The error checking is handled by end(). If the queue is full, then end will reutnr a 0,
    /// and this function should not be called.
    void push_back() {
      memory_fence::sfence();
      ++_tail_offset;
    }

    /// <b> Assumes: </b> *end has been correctly initialized, and the queue is not full <br/>
    /// <b> Changes: </b> _tail_offset <br/>
    /// <b> On Error: </b> no error checking is done <br/>
    /// <b> Description: </b> This function assumes that end() has been called, and the pointer has been
    /// initialized. It pushes that datum onto the back of the queue. No error checking is done.
    /// The error checking is handled by end(). If the queue is full, then end will reutnr a 0,
    /// and this function should not be called. 
    T* atomic_push_back_get_end() {
        static_assert( sizeof(int) == sizeof(std::atomic<int>) && (bool)std::is_standard_layout<std::atomic<int> >::value, 
            "must rewrite circular_queue::rewrite atomic_push_back_get_end to use compiler intrinsic fetch_add");

      Int prev_tail_offset = reinterpret_cast< volatile std::atomic<int>*>( &_tail_offset)->fetch_add(1);
      return (T*) &_data[prev_tail_offset % _size];
    }

    /// <b> Assumes: </b> That the head_offset is not going to be changed.
    /// <b> Changes: </b> _tail_offset <br/>
    /// <b> On Error: </b> no error checking is done <br/>
    /// <b> Description: </b> This function must be called in an enironment, where _head_offset
    ///  will not be incremented. This is not a thread safe function. It is intended
    ///  to be called for circular queues, who are being sychronized by some mechanism,
    ///  and another error conditions, forces the last push_back to be undone.
    void pop_back() {
      --_tail_offset;
    }

    /// <b> Assumes: </b> begin has been called, and the data at the front of the queue has been processed.<br/>
    /// <b> Changes: </b> _head_offset. <br/>
    /// <b> On Error: </b> no error checking is done <br/>
    /// <b> Description: </b> This function advances the head of the queue by 1. It assumes
    ///  that begin() has been called. If begin() returned 0, then this function should not be called.
    void pop_front() {
      ++_head_offset;
    }

    /// <b> Assumes: </b> <br/>
    /// <b> Changes: </b> _head_offset. <br/>
    /// <b> On Error: </b> no error checking is done <br/>
    /// <b> Description: </b> This function advances the head of the queue by n. This function is used
    ///  to catch up if the writer, has looped around and written data before the reader can read it.
    void pop_n_front( unsigned n) {
      _head_offset += n;
    }

    uint64_t get_number_pushed() {
      return _tail_offset;
    }

    T& at( Int offset) {
      if (offset < _head_offset || offset >= _tail_offset) {
        throw std::out_of_range( "offset out of range in circular_queue");
      }
      return (T&)_data[offset % _size];
    }

    uint64_t get_number_popped() {
      return _head_offset;
    }

    template<class U,int _Size, class _Int>
    friend circular_queue<U,_Size, _Int>* factory_in_shared_memory( const char *Name);
  };
}

#endif
