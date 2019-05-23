#ifndef circular_queue_in_shared_memory_h
#define circular_queue_in_shared_memory_h

#include "wrap_stdint.h"

#include "circular_queue.h"

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/shared_ptr.hpp>

#include "memory_access_specifier.h"

#define LOCAL_DEBUG 0

#define UTIL_CIRCULAR_QUEUE_IN_SHARED_MEMORY_NAME_SIZE 64

namespace util_ipc {

  template <class T, int _size>
    class circular_queue_in_shared_memory {
    char    _name[UTIL_CIRCULAR_QUEUE_IN_SHARED_MEMORY_NAME_SIZE];
    char*                                     _blob_on_heap;
    boost::interprocess::shared_memory_object _shm;
    boost::interprocess::mapped_region        _region;
    
    circular_queue<T,_size> *_cq;
    
    void put_name( const char *Name) {
      if (strlen(Name)> (UTIL_CIRCULAR_QUEUE_IN_SHARED_MEMORY_NAME_SIZE-1)) {
	std::cerr << "attempt to copy name = " << Name
		  << " which is too long, in put_name of a circular_queue_in_shared_memory"
		  << std::endl;
	exit(1);
      }
      strncpy( _name, Name, sizeof(_name));
    }
    
  circular_queue_in_shared_memory( const char *Name) : 
      _blob_on_heap(0)
    {
      put_name( Name);
    }
    
  public:
    
    static circular_queue_in_shared_memory *open_or_create( const char *Name)
    {
      using namespace boost::interprocess;
      circular_queue_in_shared_memory *obj = new circular_queue_in_shared_memory<T,_size>( Name);
      
      int32_t sz = sizeof( circular_queue<T,_size>);

#if LOCAL_DEBUG
      std::cerr << "creating shared memory object " << Name << std::endl;
#endif

      //Create a shared memory object.
      try
	{
	  shared_memory_object shm ( boost::interprocess::open_or_create, Name, read_write);
	  obj->_shm.swap( shm);
	}
      catch (std::exception& e)
	{
	  std::cerr << "error trying to create a shared memory object in "
		    << __FILE__
		    << " at line " << __LINE__ << std::endl;
	  std::cerr << "error = " << e.what() << std::endl;
	  std::cerr << "object name = " << Name << std::endl;
	  exit(1);
	}
      
      //Set size
      obj->_shm.truncate( sz);
      
      //Map the whole shared memory in this process
      try
	{
	  mapped_region region( obj->_shm, read_write);
	  obj->_region.swap( region);
	}
      catch (std::exception& e)
	{
	  std::cerr << "error trying to create a shared memory object in "
		    << __FILE__
		    << " at line " << __LINE__ << std::endl;
	  std::cerr << "error = " << e.what() << std::endl;
	  std::cerr << "object name = " << Name << std::endl;
	  exit(1);
	}
      
      obj->_cq =
	circular_queue<T,_size>::factory( obj->_region.get_address(), Name);
      
      return obj;
    }
    
    static circular_queue_in_shared_memory *create_only( const char *Name)
    {
      try {
	boost::interprocess::shared_memory_object::remove( Name);
      } catch (std::exception& e) {
	std::cerr << "could not clean shared memory object from memory e.what() = " << e.what() << std::endl
		  << "object name = " << Name << std::endl;
	exit(1);
      }
      return open_or_create( Name);
    }
    
    static circular_queue_in_shared_memory *circular_queue_on_heap( const char* Name)
    {
      circular_queue_in_shared_memory *obj = new circular_queue_in_shared_memory( Name);

      int32_t sz = sizeof( circular_queue<T,_size>);
      obj->_blob_on_heap = new char[sz];

      obj->_cq = circular_queue<T,_size>::factory( obj->_blob_on_heap, Name);

      return obj;
    }

    static circular_queue_in_shared_memory *open_only( const char *Name)
    {
      using namespace boost::interprocess;
      circular_queue_in_shared_memory *obj = new circular_queue_in_shared_memory( Name);
      
      //Open already created shared memory object.
      try
      {
	shared_memory_object shm (boost::interprocess::open_only, Name, read_write);
	obj->_shm.swap( shm);
      }
      catch (std::exception& e) {
	std::cerr << "could not open shared memory object, e.what=" << e.what() << std::endl;
	exit(1);
      }
      
      //Map the whole shared memory in this process
      try
      {
	mapped_region region( obj->_shm, read_write);
	obj->_region.swap( region);
      }
      catch (std::exception& e) {
	std::cerr << "could not map memory object, e.what=" << e.what() << std::endl;
	exit(1);
      }
      
      obj->_cq  = static_cast<circular_queue<T,_size> *> (obj->_region.get_address());
      
      return obj;
    }
    
    // Dummy does nothing, it is there just to make the prototype
    // compatible with circular_queue_in_mmap::factory
    static circular_queue_in_shared_memory *factory(
						    int mode,
						    const char *Name,
                                                    const char *Dummy = 0)
    {
      switch (mode) {
	
      case _open_or_create_:
	return open_or_create( Name);
	break;
	
      case _create_only_:
	return create_only( Name);
	break;
	
      case _open_only_:
	return open_only( Name);
	break;
	
      case _use_new_:
        return circular_queue_on_heap( Name);

      default:
	std::cerr << "mode = " << mode << " is not recognized in circular_queue_in_shared_memory::factory"
		  << std::endl;
	exit( 1);
      }
    }
    
    static void remove( const char *Name) {
      try {
        boost::interprocess::shared_memory_object::remove( Name);
      } catch (...) {
      }
    }

    void remove() {
      if (_blob_on_heap == 0) {
        boost::interprocess::shared_memory_object::remove( _name);
      } else {
        delete[] _blob_on_heap;
      }
    }
    
    static void remove_and_delete( circular_queue_in_shared_memory * &obj) {
      boost::interprocess::shared_memory_object::remove( obj->_name);
      delete obj;
      obj = 0;
    }
    
    T *end() {
      return _cq->end();
    }
    
    T *begin() {
      return _cq->begin();
    }
    
    void push_back() {
      _cq->push_back();
    }
    
    void pop_front() {
      _cq->pop_front();
    }

    const char *get_name() const {
      return _name;
    }

    uint32_t get_number_pushed() const {
      return _cq->get_number_pushed();
    }

    uint32_t get_number_popped() const {
      return _cq->get_number_popped();
    }
  };
}

#undef LOCAL_DEBUG

#endif
