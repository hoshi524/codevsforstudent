
cd `dirname $0`

for file in `\find input -maxdepth 1 -type f`; do
  echo $file

  echo a
  time ./a.out < $file > /dev/null
  echo b
  time ./b.out < $file > /dev/null
  echo a
  time ./a.out < $file > /dev/null
  echo b
  time ./b.out < $file > /dev/null
done
