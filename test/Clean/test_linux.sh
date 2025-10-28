echo "Clean test"

set -e

cc -o Build Build.c -L../../ -lBuild -I../../include > /dev/null
./Build

make > /dev/null
make clean > /dev/null

if [ -e bin/a.o ]
then
    echo " · Object file removal: FAIL"
else
    echo " · Object file removal: PASS"
fi

if [ -e libmeth.so ]
then
    echo " · Library removal:     FAIL"
else
    echo " · Library removal:     PASS"
fi
