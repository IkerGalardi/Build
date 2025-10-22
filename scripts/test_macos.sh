set -e

cd test/ArtifactNames
sh test_macos.sh
cd ../..

cd test/Defines
sh test.sh
cd ../..
