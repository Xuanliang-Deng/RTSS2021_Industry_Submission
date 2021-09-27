## Instrution for running code
- Install gtest for unit tests (work for Ubunutu 18 and newer version)
```
    $sudo apt install libgtest-dev build-essential cmake
    $cd /usr/src/googletest
    $sudo cmake .
    $sudo cmake --build . --target install
```
- Run unit tests
```
    $ cd task/
    $ cmake .
    $ make
    $ ./TaskTests
```


