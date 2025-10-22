echo "Defines test"

set -e

cc -o Build Build.c -L../../ -lBuild -I../../include > /dev/null
./Build > /dev/null

if make > /dev/null 2> /dev/null
then
    true
else
    echo " · DEFINE_EMPTY defined:            FAIL"
    exit 1
fi
./DefineEmpty

if make DefineNumber > /dev/null 2> /dev/null
then
    true
else
    echo " · DEFINE_NUMBER correctly defined: FAIL"
    exit 1
fi
./DefineNumber

if make DefineString > /dev/null 2> /dev/null
then
    true
else
    echo " · DEFINE_STRING correctly defined: FAIL"
    exit 1
fi
./DefineString
