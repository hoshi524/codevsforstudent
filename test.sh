
cd `dirname $0`

g++ -std=c++11 -O3 tmp.cpp
./a.out < input/input2.txt > /dev/null
