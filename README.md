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

## Illustration for S-Bottleneck heuristic
In this section, we first give a concrete DAG as an example to show how our algorithm works. Note that this is the preliminary version of implementation, we only consider the StochHC-DAG without probability-associated edges and conditional node.

The DAG is given below. Each node is represented as (index, tag) pair which indicates the index of the node and the processing unit that it should run onto.  
<div align=center><img width="1500" height="300" src="https://github.com/Xuanliang-Deng/RTSS2021_Industry_Submission/blob/master/images/S-Bottleneck%20Example.png"/></div>

- Partition DAG nodes
In the problem statement, it is assumed that the mapping between tasks and processing units is static, i.e., we know onto which processor should each task execute. In our StochHC-DAG, we represent this mapping by 'tags' associated with each node and parition nodes by their tags. As can be seen in the above figure, node 1,2,4,6 are mapped to CPU while node 3,5 are mapped to GPU.  



