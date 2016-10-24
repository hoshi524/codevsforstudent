
cd `dirname $0`

for file in `\find input -maxdepth 1 -type f`; do
  echo $file

  echo $1
  time $1 < $file > /dev/null
  echo $2
  time $2 < $file > /dev/null
  echo $1
  time $1 < $file > /dev/null
  echo $2
  time $2 < $file > /dev/null
done
