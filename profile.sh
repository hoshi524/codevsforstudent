
cd `dirname $0`

g++ -pg Main.cpp
./a.out < input/input0.txt > /dev/null
gprof a.out gmon.out > tmp

vim tmp
