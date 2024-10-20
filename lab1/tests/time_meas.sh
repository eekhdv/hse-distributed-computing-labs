#!/bin/bash

monte_test(){
  ./monte-carlo.o $1 $2 | xargs -d'\n' -n2 sh -c 'echo "$2"' argv0 | xargs -n100 sh -c 'printf "[%9d] : %10s\t" $6 $2' argv0
}

mandelbrot_test(){
  ./mandelbrot.o $1 $2  | xargs -d'\n' -n2 sh -c 'echo "$2"' argv0 | xargs -n100 sh -c 'printf "[%7d] : %10s\t" $6 $2' argv0
}

GREEN='\033[0;32m'
NC='\033[0m'

pushd ../bin/ > /dev/null

echo -e "${GREEN}Mandelbrot tests${NC}"
for i in {1..8}
do
    echo -en "${GREEN}$i threads: ${NC}"
    for j in 1 10 50 100
    do
      pnum=$(shuf -i $(( 1000 * $j ))-$(( 10000 * $j )) -n 1)
      mandelbrot_test $i $pnum
    done
    echo ""
done

echo -e "${GREEN}Monte-Carlo tests${NC}"
for i in {1..8}
do
    echo -en "${GREEN}$i threads: ${NC}"
    for j in {0..3}
    do
      pnum=$(shuf -i $(( 10000 * (10 ** $j) ))-$(( 100000 * (10 ** $j) )) -n 1)
      monte_test $i $pnum
    done
    echo ""
done

popd > /dev/null
