cstpp -i=CustomRand.h.pre -c -no_check_attributes -o=CustomRand.h
sed -i -e 's/void init_calc_derived_attributes() const;//'  CustomRand.h
sed -i -e 's/FOO/void init_calc_derived_attributes() const { _x = _seed;}/' CustomRand.h
