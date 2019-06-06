#g++  -std=c++11 -ggdb -O0  -I".." -I"../../CustomRand"   -o "shmdemo" shmdemo.cpp
#g++  -Wall -std=c++11 -ggdb -O0 -I".." -I"../../CustomRand" -L"." shmdemo.o  -o "shmdemo"
rm shmdemo
g++  -fPIC -Wall -std=c++11 -ggdb -O0 -D_REENTRANT -I".." -I"../../CustomRand" -c  -o "shmdemo.o" shmdemo.cpp
g++  -fPIC -Wall -std=c++11 -ggdb -O0 -D_REENTRANT -I".." -I"../../CustomRand" -L"." shmdemo.o -l"rt" -ldl -lpthread  -o "shmdemo"
