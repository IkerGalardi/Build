set -e

cd test/ArtifactNames
sh test_linux.sh
cd ../..

cd test/Includes
sh test.sh
cd ../..

cd test/StandardVersion
sh test.sh
cd ../..

cd test/Defines
sh test.sh
cd ../..
