
cd `dirname $0`

for file in `\find input -maxdepth 1 -type f`; do
  echo $file
  time $1 < $file > /dev/null
done
