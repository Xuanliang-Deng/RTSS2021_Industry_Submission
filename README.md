## Struture of README.md
The structure of this README.md is shown as follows:
- Instruction for running code
    - Install gtest
    - Run unit tests
    - Screenshot for test results
- Illustration for S-Bottleneck heuristic
    - Partition DAG nodes
    - Select bottlleneck processor
    - Find optimal schedule with BnB techniques
    - Shift bottleneck processor


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
- Screenshot for test results  
Run the commands above you will get results below:  
![The image could not be found](https://github.com/Xuanliang-Deng/RTSS2021_Industry_Submission/blob/master/images/TaskResultsS.png)




