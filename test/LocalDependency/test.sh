echo "Local dependencies test"

uname -a | grep Linux > /dev/null && export LD_LIBRARY_PATH=./

set -e

cc -o Build Build.c -L./ -lBuild -I../../include > /dev/null
./Build > /dev/null

if make > /dev/null 2> /dev/null
then
    echo " · Correctly linked: PASS"
else
    echo " · Correctly linked: FAIL"
    exit 1
fi
