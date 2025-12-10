echo "Defines test"

rm -rf bin/

uname -a | grep Linux > /dev/null && export LD_LIBRARY_PATH=./

set -e

cc -o Build Build.c -L./ -lBuild -I../../include > /dev/null
./Build > /dev/null

if make > /dev/null 2> /dev/null
then
    true
else
    echo " · DEFINE_EMPTY defined:            FAIL"
    exit 1
fi
./bin/DefineEmpty

if make bin/DefineNumber > /dev/null 2> /dev/null
then
    true
else
    echo " · DEFINE_NUMBER correctly defined: FAIL"
    exit 1
fi
./bin/DefineNumber

if make bin/DefineString > /dev/null 2> /dev/null
then
    true
else
    echo " · DEFINE_STRING correctly defined: FAIL"
    exit 1
fi
./bin/DefineString
