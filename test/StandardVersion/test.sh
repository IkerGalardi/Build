echo "Standard version test"

uname -a | grep Linux > /dev/null && export LD_LIBRARY_PATH=./

set -e

cc -o Build Build.c -L./ -lBuild -I../../include > /dev/null
./Build > /dev/null

if make bin/c90 > /dev/null 2> /dev/null
then
    true
else
    echo " · c90: FAIL"
    exit 1
fi
if ./bin/c90
then
    echo " · c90: PASS"
else
    echo " · c90: FAIL"
    exit 1
fi

if make bin/c99 > /dev/null 2> /dev/null
then
    true
else
    echo " · c99: FAIL"
    exit 1
fi
if ./bin/c99
then
    echo " · c99: PASS"
else
    echo " · c99: FAIL"
    exit 1
fi

if make bin/c11 > /dev/null 2> /dev/null
then
    true
else
    echo " · c11: FAIL"
    exit 1
fi
if ./bin/c11
then
    echo " · c11: PASS"
else
    echo " · c11: FAIL"
    exit 1
fi

if make bin/c17 > /dev/null 2> /dev/null
then
    true
else
    echo " · c17: FAIL"
    exit 1
fi
if ./bin/c17
then
    echo " · c17: PASS"
else
    echo " · c17: FAIL"
    exit 1
fi

if make bin/c23 > /dev/null 2> /dev/null
then
    true
else
    echo " · c23: FAIL"
    exit 1
fi
if ./bin/c23
then
    echo " · c23: PASS"
else
    echo " · c23: FAIL"
    exit 1
fi
