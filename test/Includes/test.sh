echo "Includes test"

rm -rf bin/

uname -a | grep Linux > /dev/null && export LD_LIBRARY_PATH=./

set -e

cc -o Build Build.c -L./ -lBuild -I../../include > /dev/null
./Build > /dev/null

if make > /dev/null 2> /dev/null
then
    echo " · Correcly included: PASS"
else
    echo " · Correcly included: FAIL"
    exit 1
fi
