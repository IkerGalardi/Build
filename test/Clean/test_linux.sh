echo "Clean test"

export LD_LIBRARY_PATH=./

rm -rf bin/

set -e

cc -o Build Build.c -L. -lBuild -I../../include > /dev/null
./Build

make > /dev/null
make clean > /dev/null

if [ -e bin/a.o ]
then
    echo " · Object file removal:     FAIL"
    exit 1
else
    echo " · Object file removal:     PASS"
fi

if [ -e bin/a.d ]
then
    echo " · Dependency file removal: FAIL"
    exit 1
else
    echo " · Dependency file removal: PASS"
fi

if [ -e libmeth.so ]
then
    echo " · Library removal:         FAIL"
    exit 1
else
    echo " · Library removal:         PASS"
fi
