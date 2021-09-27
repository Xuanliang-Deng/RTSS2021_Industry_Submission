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

- Select bottlleneck processor  
When selecting the 'bottleneck' among all processors, we first calculate the 'starting makespan' of each procesor assuming there is no resource conflict between tasks. We illustrate the process using the example above. The ** execution time (ET) ** of each node is equal to their index, i.e., ** ET(v_n) = n **. All the tasks are parallelly listed starting from source node and end at sink node.  

<div align=center><img width="800" height="600" src="https://github.com/Xuanliang-Deng/RTSS2021_Industry_Submission/blob/master/images/StartingMKS.png"/></div>

Only precedence constraint is consdered when calculating the finish time of each node. For example, finish time (FT) of node v1 is 1, since it has no predecessor. For node v2, FT(v_2) = 1 + 2 = 3 since it has to wait for node v1 finishes execution. For node v3 FT(v_3) = 1 + 2 + 3 = 6. We show the FT of all nodes in the table below.  

|   Node index  |   Finish time |
| ------------- | ------------- |
|       1       |        1      |
|       2       |      1+2=3    |
|       3       |    1+2+3=6    |
|       4       |    1+2+4=7    |
|       5       |      1+5=6    |
|       6       |   1+5+6=12    |

Therefore, the starting makespan of CPU is max{FT(v_1),FT(v_2),FT(v_4),FT(v_6)} = 12, for GPU it is max{FT(v_3),FT(v_5)} = 6. The first bottleneck processor is selected as max{makespan(CPU), makespan(GPU)} = CPU. The bottleneck will be shifted to GPU in step 4.  

- Find optimal schedule with BnB techniques
Now we analyze the bottleneck processor and find the optimal schedule on it, that is, we determine the execution order among the nodes in the ready queue. There are two key factors to determine such order:
    - Lmax (maximum lateness) of the schedule
    - precedence constraints (topological order in DAG) 



