#ifndef circular_queue_in_mmap_h
#define circular_queue_in_mmap_h

#include "wrap_stdint.h"

#include "circular_queue.h"

#include "blob_in_mmap.h"

#include <boost/shared_ptr.hpp>

#define UTIL_CIRCULAR_QUEUE_IN_MMAP_NAME_SIZE 64

namespace util_ipc {

  template <class T, int _size>
    class circular_queue_in_mmap {
    char*        _blob_on_heap;
    boost::shared_ptr< blob_in_mmap> _blob;
    circular_queue<T,_size>* _circular_queue;

  public:

  circular_queue_in_mmap( 
    const char* Name, 
    uint32_t Mode, 
    const char *Dirname="")
      {
        init( Name, Mode, Dirname);
      }

  void init(
    const char* Name, 
    uint32_t Mode, 
    const char *Dirname="")
      {
        _blob_on_heap = 0;
        _blob.reset( new blob_in_mmap( Name, Mode, sizeof( circular_queue<T,_size>), Dirname));
        if (_blob->get_file_is_new()) {
          _circular_queue = circular_queue<T,_size>::factory( _blob->get_address(), Name);
        } else {
          _circular_queue = reinterpret_cast< circular_queue<T,_size>* > ( _blob->get_address());
        }
      }

  circular_queue_in_mmap()      
    {
    }
    
    const char *get_name() const { 
      return _circular_queue->get_name();
    }

    T *end() { 
      return _circular_queue->end(); 
    }

    T *begin() {
      return _circular_queue->begin(); 
    }

    volatile T &at( int offset) {
      return (volatile T &)_circular_queue->at( offset);
    }

    void push_back() {
      _circular_queue->push_back();
    }

    T* atomic_push_back_get_end() {
      return _circular_queue->atomic_push_back_get_end();
    }

    void pop_back() {
      _circular_queue->pop_back();
    }

    void pop_front() {
      _circular_queue->pop_front();
    }

    uint64_t get_number_pushed() {
      return _circular_queue->get_number_pushed();
    }

    uint64_t get_number_popped() {
      return _circular_queue->get_number_popped();
    }

    uint64_t begin_offset() const { return _circular_queue->begin_offset(); }

    uint64_t end_offset() const { return _circular_queue->end_offset(); }

    uint64_t size() const { return _circular_queue->size(); }

    static circular_queue_in_mmap *open_or_create(
							   const char *Name,
                                                           const char *Dirname="")
    {
      return new circular_queue_in_mmap( Name, _open_or_create_, Dirname);
    }

    static circular_queue_in_mmap *create_only(
							const char *Name,
                                                        const char *Dirname="")
    {
      return new circular_queue_in_mmap( Name, _create_only_, Dirname);
    }
    
    static circular_queue_in_mmap *circular_queue_on_heap(
                                   const char *Name,
                                   const char *Dirname="")
    {
      circular_queue_in_mmap *obj = new circular_queue_in_mmap();

      int32_t sz = sizeof( circular_queue<T,_size>);
      obj->_blob_on_heap = new char[sz];

      obj->_circular_queue =
	circular_queue<T,_size>::factory( obj->_blob_on_heap, Name);

      return obj;
    }
    
    static circular_queue_in_mmap *open_only(
						      const char *Name,
                                                      const char *Dirname="")
    {
      return new circular_queue_in_mmap( Name, _open_only_, Dirname);
    }

    static circular_queue_in_mmap *factory(
                                           int mode,
                                           const char *Name,
                                           const char *Dirname="")
    {
      switch (mode) {
	
      case _open_or_create_:
	return open_or_create( Name, Dirname);
	break;
	
      case _create_only_:
	return create_only( Name, Dirname);
	break;
	
      case _open_only_:
	return open_only( Name, Dirname);
	break;
	
      case _use_new_:
        return circular_queue_on_heap( Name, Dirname);

      default:
	std::cerr << "mode = " << mode << " is not recognized in circular_queue_in_mmap::factory"
		  << std::endl;
	exit( 1);
      }      
    }

    static void remove( const char *Name, 
                                    const char *Dirname = "")
    {      
      blob_in_mmap::remove( Name, Dirname);
    }

    void remove() {
      if (_blob.get() != 0) {
        std::string name = _blob->get_name();
        std::string dirname = _blob->get_dirname();
       _blob.reset();
       blob_in_mmap::remove( name.c_str(), dirname.c_str());
      }
    }

  };
};

#endif
