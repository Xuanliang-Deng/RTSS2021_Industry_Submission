How to install and make sure the gtest work for the project, note that the below only works for Ubuntu 18 and newer

$sudo apt install libgtest-dev build-essential cmake
$cd /usr/src/googletest
$sudo cmake .
$sudo cmake --build . --target install
