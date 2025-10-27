echo "Standard version test"

set -e

cc -o Build Build.c -L../../ -lBuild -I../../Include > /dev/null
./Build > /dev/null

if make c90 > /dev/null 2> /dev/null
then
    true
else
    echo " · c90: FAIL"
    exit 1
fi
if ./c90
then
    echo " · c90: PASS"
else
    echo " · c90: FAIL"
fi

if make c99 > /dev/null 2> /dev/null
then
    true
else
    echo " · c99: FAIL"
    exit 1
fi
if ./c99
then
    echo " · c99: PASS"
else
    echo " · c99: FAIL"
fi

if make c11 > /dev/null 2> /dev/null
then
    true
else
    echo " · c11: FAIL"
    exit 1
fi
if ./c11
then
    echo " · c11: PASS"
else
    echo " · c11: FAIL"
fi

if make c17 > /dev/null 2> /dev/null
then
    true
else
    echo " · c17: FAIL"
    exit 1
fi
if ./c17
then
    echo " · c17: PASS"
else
    echo " · c17: FAIL"
fi

if make c23 > /dev/null 2> /dev/null
then
    true
else
    echo " · c23: FAIL"
    exit 1
fi
if ./c23
then
    echo " · c23: PASS"
else
    echo " · c23: FAIL"
fi
