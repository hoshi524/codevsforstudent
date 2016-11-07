
cd `dirname $0`

./comple.sh

for file in `\find input -maxdepth 1 -type f`; do
  echo $file

  echo Test
  time ./Test.out < $file > /dev/null
  echo Main
  time ./Main.out < $file > /dev/null
  echo Test
  time ./Test.out < $file > /dev/null
  echo Main
  time ./Main.out < $file > /dev/null
done
