#!/bin/bash
#build cpputest

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
echo "DIR is '$DIR'"
cd $DIR/../cpputest

echo $PWD

autoreconf . -i
./configure
make tdd
export CPPUTEST_HOME=$(pwd)
