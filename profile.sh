
cd `dirname $0`

g++ -std=c++11 -pg Main.cpp
./a.out < input/input0.txt > /dev/null
gprof a.out gmon.out > tmp

vim tmp
