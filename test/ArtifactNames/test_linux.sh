echo "Artifact naming test"

export LD_LIBRARY_PATH=./

set -e

cc -o Build Build.c -L../../ -lBuild -I../../include > /dev/null

./Build > /dev/null

make > /dev/null
make libTestDLib.so > /dev/null
make libTestSLib.a > /dev/null

if [ -e TestExe ]
then
    echo " · Executable artifact:      PASS"
else
    echo " · Executable artifact:      FAIL"
    exit 1
fi

if [ -e libTestDLib.so ]
then
    echo " · Dynamic library artifact: PASS"
else
    echo " · Dynamic library artifact: FAIL"
    exit 1
fi

if [ -e libTestSLib.a ]
then
    echo " · Static library artifact:  PASS"
else
    echo " · Static library artifact:  FAIL"
    exit 1
fi
