#ifndef blob_in_mmap_h
#define blob_in_mmap_h

#include <string>
#include <string.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/shared_ptr.hpp>
#include <wrap_stdint.h>

#include "memory_access_specifier.h"

#define blob_in_mmap_stringify(x) #x
#define blob_in_mmap_tostring(x) blob_in_mmap_stringify(x)
#define blob_in_mmap_at __FILE__ ":" blob_in_mmap_tostring(__LINE__)

namespace util_ipc {

  class blob_in_mmap {
    std::string _name;   
    uint32_t _mode;
    mutable uint32_t _size;
    std::string _dirname;
    std::string _complete_name;

    mutable std::filebuf _fbuf;
    mutable boost::interprocess::file_mapping _m_file;
    mutable boost::interprocess::mapped_region _region;
    mutable bool _file_is_new;

  public:
    
    const std::string& get_name() const {
      return _name;
    }

    const std::string& get_dirname() const {
      return _dirname;
    }

    const std::string& get_complete_name() const {
      return _complete_name;
    }

    bool get_file_is_new() const {
      return _file_is_new;
    }

    void init(const char* Name, uint32_t Mode, uint32_t Size, const char *Dirname="") 
    {
      _name = Name;
      _mode = Mode;
      _size = Size;
      _dirname = Dirname;
      _complete_name = Dirname;

      if (_dirname.size() == 0 && getenv( "quote_ipc_data_directory")!=0) {
        _complete_name = _dirname = getenv( "quote_ipc_data_directory");
      }

      if ((_mode == _create_only_ || _mode == _open_or_create_) && _size == 0) {
        throw std::runtime_error( "can't create a file with size 0 in " blob_in_mmap_at );
      }

#ifdef _MSC_VER
      if (!_complete_name.empty() && *_complete_name.rbegin() != '\\') {
        _complete_name += '\\';
      }
#else
      if (!_complete_name.empty() && *_complete_name.rbegin() != '/') {
        _complete_name += '/';
      }
#endif
      _complete_name += Name;

      open_file();
    }

    blob_in_mmap() {
    }

  blob_in_mmap( const char* Name, uint32_t Mode, uint32_t Size, const char *Dirname="")
    {
      init( Name, Mode, Size, Dirname);
    }

    void open_file() const {

      _file_is_new = false;

      if (_mode == _create_only_) {

      try_to_create_file:
        _fbuf.open(_complete_name.c_str(),
                   std::ios_base::in|std::ios_base::out|std::ios_base::trunc|std::ios_base::binary);
        if (!_fbuf.is_open()) goto could_not_open_file;
 
        _fbuf.pubseekoff( _size - 1, std::ios_base::beg);
        _fbuf.sputc(0);
        _fbuf.pubsync();
        _file_is_new = true;

      } else if (_mode == _open_only_) {

        _fbuf.open(_complete_name.c_str(), std::ios_base::in | std::ios_base::out | std::ios_base::binary);
        if (!_fbuf.is_open()) goto could_not_open_file;

      } else if (_mode == _open_or_create_) {

        _fbuf.open( _complete_name.c_str(), std::ios_base::in | std::ios_base::out | std::ios_base::binary);
        if (!_fbuf.is_open()) {
          goto try_to_create_file;
        }

      } else {
        throw std::runtime_error( "mode not recognized in mmap_array::open_file");
      }

      if (_file_is_new) {
        uint32_t file_size;
        _fbuf.pubseekoff( 0, std::ios_base::end);       
        file_size = _fbuf.in_avail();
        if (file_size < _size) {
          _fbuf.pubseekoff( _size - 1, std::ios_base::beg);
          _fbuf.sputc(0);
          _fbuf.pubsync();
        } else {
          _size = file_size;
        }
      } 

      try
      {
	boost::interprocess::file_mapping m_file( _complete_name.c_str(), boost::interprocess::read_write);
	_m_file.swap( m_file);
      }
      catch (std::exception& e) {
        std::ostringstream os;
	os << "error creating file mapping, file name = " << _complete_name
		  << " e.what = {" << e.what() << "}" << std::endl;
	throw std::runtime_error( os.str());
      }

      try
	{
	  boost::interprocess::mapped_region region
	    (_m_file      //What to map
	     ,boost::interprocess::read_write //Map it as read-write
	     );
	  _region.swap( region);
	}
      catch (std::exception& e) {
        std::ostringstream os;
	os << "error mapping file to memory, file name = " << _complete_name
		  << " e.what = {" << e.what() << "}" << std::endl;
        os << "size = " << _size << std::endl;
	throw std::runtime_error( os.str());
      }

      return;

    could_not_open_file:
      int dup_errno = errno;      
      std::ostringstream os;
      os << "could not open file, " << _complete_name 
         << " at " << blob_in_mmap_at << std::endl
         << "errno = " << dup_errno << " strerror=" << strerror( dup_errno) << std::endl;
      throw std::runtime_error( os.str());
    }
            
    void *get_address() {
      return _region.get_address();
    }

    const void *get_address() const {
      return _region.get_address();
    }

    uint32_t get_size() const {
      return _region.get_size();
    }

    void clear() {
      memset( _region.get_address(), 0, _region.get_size());
    }

    static blob_in_mmap *factory(
                                 int Mode,
                                 const char *Name,
                                 size_t Size,
                                 const char *Dirname = "")
    {
      return new blob_in_mmap( Name, Mode, Size, Dirname);
    }

    static blob_in_mmap *open_or_create(
                                        const char *Name,
                                        size_t Size,
                                        const char *Dirname = "")
    {
      return new blob_in_mmap( Name, _open_or_create_, Size, Dirname);      
    }

    static blob_in_mmap *create_only(
                                     const char *Name,
                                     size_t Size,
                                     const char *Dirname = "")
    {
      return new blob_in_mmap( Name, _create_only_, Size, Dirname);
    }

    static blob_in_mmap *open_only( const char *Name, 
                                    const char *Dirname = "")
    {
      return new blob_in_mmap( Name, _open_only_, 0, Dirname);
    }

    static void remove( const char *Name, 
                                    const char *Dirname = "")
    {
      if (*Dirname == 0 && getenv( "quote_ipc_data_directory")!=0) {
        Dirname = getenv( "quote_ipc_data_directory");
      }

      std::string Complete_name( Dirname);
#ifdef _MSC_VER
      if (!Complete_name.empty() && *Complete_name.rbegin() != '\\') {
        Complete_name += '\\';
      }
#else
      if (!Complete_name.empty() && *Complete_name.rbegin() != '/') {
        Complete_name += '/';
      }
#endif
      Complete_name += Name;

      //boost::interprocess::file_mapping::remove( Complete_name.c_str());
      boost::filesystem::remove( Complete_name.c_str());
    }
  };

  typedef boost::shared_ptr< blob_in_mmap> p_blob_in_mmap;
};

#undef blob_in_mmap_stringify
#undef blob_in_mmap_tostring
#undef blob_in_mmap_at

#endif
