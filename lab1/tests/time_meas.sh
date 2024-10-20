#!/bin/bash

monte_test(){
  #./monte-carlo.o $1 $2 | xargs -d'\n' -n2 sh -c 'echo "$2"' argv0 | xargs -n100 sh -c 'printf "[%9d] : %10s\t" $6 $2' argv0
  ./monte-carlo.o $1 $2 | xargs -d'\n' -n2 sh -c 'echo "$2"' argv0 | xargs -n100 sh -c 'printf "[%d] : %10s\t" $4 $2' argv0
}

mandelbrot_test(){
  #./mandelbrot.o $1 $2  | xargs -d'\n' -n2 sh -c 'echo "$2"' argv0 | xargs -n100 sh -c 'printf "[%7d] : %10s\t" $6 $2' argv0
  ./mandelbrot.o $1 $2  | xargs -d'\n' -n2 sh -c 'echo "$2"' argv0 | xargs -n100 sh -c 'printf "[%d] : %10s\t" $4 $2' argv0
}

GREEN='\033[0;32m'
NC='\033[0m'

pushd ../bin/ > /dev/null

echo -e "${GREEN}Mandelbrot tests${NC}"
for j in 1 10 50 100
do
  pnum=$(shuf -i $(( 1000 * $j ))-$(( 10000 * $j )) -n 1)
  echo -e "${GREEN}$pnum points: ${NC}"
  for i in {1..8}
  do
    mandelbrot_test $i $pnum
  done
  echo ""
done

echo -e "${GREEN}Monte-Carlo tests${NC}"
for j in {0..3}
do
  #echo -en "${GREEN}$i threads: ${NC}"
  throws=$(shuf -i $(( 10000 * (10 ** $j) ))-$(( 100000 * (10 ** $j) )) -n 1)
  echo -e "${GREEN}$throws throws: ${NC}"
  for i in {1..8}
  do
      monte_test $i $throws
  done
  echo ""
done

popd > /dev/null
