
clang-format-3.8 Main.cpp > tmp
cat tmp > Main.cpp
rm tmp

g++ -std=c++11 -O2 Main.cpp

