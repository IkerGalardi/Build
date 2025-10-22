echo "Artifact naming test"

set -e

cc -o Build Build.c -L../../ -lBuild -I../../include > /dev/null

./Build > /dev/null

make > /dev/null
make libTestDLib.dylib > /dev/null
make libTestSLib.a > /dev/null

if [ -e TestExe ]
then
    echo " · Executable artifact:      PASS"
else
    echo " · Executable artifact:      FAIL"
fi

if [ -e libTestDLib.dylib ]
then
    echo " · Dynamic library artifact: PASS"
else
    echo " · Dynamic library artifact: FAIL"
fi

if [ -e libTestSLib.a ]
then
    echo " · Static library artifact:  PASS"
else
    echo " · Static library artifact:  FAIL"
fi
