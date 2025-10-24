set -e

cd test/ArtifactNames
sh test_linux.sh
cd ../..

cd test/Defines
sh test.sh
cd ../..
