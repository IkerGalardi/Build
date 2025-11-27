RETURN="0"

cd test/UnitTest
if sh test.sh
then
    true
else
    RETURN="1"
fi
cd ../..

cd test/ArtifactNames
if sh test_linux.sh
then
    true
else
    RETURN="1"
fi
cd ../..

cd test/Includes
if sh test.sh
then
    true
else
    RETURN="1"
fi
cd ../..

cd test/StandardVersion
if sh test.sh
then
    true
else
    RETURN="1"
fi
cd ../..

cd test/Clean
if sh test_linux.sh
then
    true
else
    RETURN="1"
fi
cd ../..

cd test/Defines
if sh test.sh
then
    true
else
    RETURN="1"
fi
cd ../..

cd test/LocalDependency
if sh test.sh
then
    true
else
    RETURN="1"
fi
cd ../..

exit $RETURN
