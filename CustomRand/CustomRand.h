#ifndef INCLUDE_CUSTOMRAND_H
#define INCLUDE_CUSTOMRAND_H

/// This class provides a thread safe class for generating random functions. The state can be 
/// completely specified by calling put_seed, because _x is set equal to _seed by this call.
/// Note that future calls to rand, or iterate, will make the state different.
/// This functionality is similar the rand_r in the C standard library.
/// Note that windows does not included rand_r, so this class provides a quick substitute for rand_r.
class ax_b_c_random {
  protected:
    //other data, functions and typedefs
    void iterate() const { _x = (_a * _x + _b) % _c; if (_x < 0) _x = -_x;}

    //key attributes
      int _a;
      int _b;
      int _c;

    //functional attributes
      int _seed;

    //derived attributes
      mutable int _x;


  public:
    void local_set_null() {
      _a = 65257;
      _b = 63337;
      _c = 0x10000; // 65536
      _seed = 0;
      _x = 0;
    }

    void set_null() {
      local_set_null();
    }

    

    void init( const ax_b_c_random& rhs) {
      _a = rhs._a;
      _b = rhs._b;
      _c = rhs._c;
      _seed = rhs._seed;
      _x = rhs._x;
    }

    ax_b_c_random& operator = ( const ax_b_c_random& rhs) {
        init( rhs);
        return *this;
    }

    ax_b_c_random( const ax_b_c_random& rhs) { 
        init( rhs);
    }

    void init(
      int A,
      int B,
      int C,
      int Seed = 0)
    {
      //copy all_key attributes
      _a = A;
      _b = B;
      _c = C;

      //copy all_functional attributes
      _seed = Seed;

      init_calc_derived_attributes();
    }

    explicit ax_b_c_random(
      int A = 65257,
      int B = 63337,
      int C = 0x10000,
      int Seed = 0)
    {
      set_null();

      init(
        A,
        B,
        C,
        Seed);
    }

    ~ax_b_c_random() { }

    //data access functions

    int get_a() const { return _a;}
    int get_b() const { return _b;}
    int get_c() const { return _c;}
    int get_seed() const { return _seed;}
    int get_x() const { return _x;}

    //change data functions

    void put_a(int A) {
      _a = A;

      init_calc_derived_attributes();
    }

    void put_b(int B) {
      _b = B;

      init_calc_derived_attributes();
    }

    void put_c(int C) {
      _c = C;

      init_calc_derived_attributes();
    }

    void put_seed(int Seed) {
      _seed = Seed;

      init_calc_derived_attributes();
    }

    void init_simple_attributes( const ax_b_c_random& rhs) {
       _a = rhs._a;
       _b = rhs._b;
       _c = rhs._c;
       _seed = rhs._seed;
       _x = rhs._x;
    }

    int rand() const {  iterate(); return _x;}
    void init_calc_derived_attributes() const {
      _x = _seed;
    }
};


#endif
