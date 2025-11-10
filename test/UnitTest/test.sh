echo "Unit test"

if cc -o StringArrayTest StringArrayTest.c ../../src/Util.c -I../../src -I../../include -Wall -Wextra -ggdb 2> /dev/null > /dev/null
then
    echo " · StringArray compile: PASS"
else
    echo " · StringArray compile: FAIL"
    exit
fi

./StringArrayTest
