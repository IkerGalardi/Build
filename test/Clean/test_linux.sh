echo "Clean test"

export LD_LIBRARY_PATH=./

set -e

cc -o Build Build.c -L../../ -lBuild -I../../include > /dev/null
./Build

make > /dev/null
make clean > /dev/null

if [ -e bin/a.o ]
then
    echo " · Object file removal: FAIL"
    exit 1
else
    echo " · Object file removal: PASS"
    exit 1
fi

if [ -e libmeth.so ]
then
    echo " · Library removal:     FAIL"
    exit 1
else
    echo " · Library removal:     PASS"
fi
