echo "Artifact naming test"

set -e

cc -o Build Build.c -L. -lBuild -I../../include > /dev/null

./Build > /dev/null

make > /dev/null
make bin/libTestDLib.dylib > /dev/null
make bin/libTestSLib.a > /dev/null

if [ -e bin/TestExe ]
then
    echo " · Executable artifact:      PASS"
else
    echo " · Executable artifact:      FAIL"
    exit 1
fi

if [ -e bin/libTestDLib.dylib ]
then
    echo " · Dynamic library artifact: PASS"
else
    echo " · Dynamic library artifact: FAIL"
    exit 1
fi

if [ -e bin/libTestSLib.a ]
then
    echo " · Static library artifact:  PASS"
else
    echo " · Static library artifact:  FAIL"
    exit 1
fi
