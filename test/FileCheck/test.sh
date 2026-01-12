echo "File check test"

rm -f Build

uname -a | grep Linux > /dev/null && export LD_LIBRARY_PATH=./

cc -o Build Build.c -L./ -lBuild -I../../include -ggdb > /dev/null

if ./Build 2> /dev/null
then
    echo " · Source file existance: FAIL"
else
    echo " · Source file existance: PASS"
    exit 1
fi
