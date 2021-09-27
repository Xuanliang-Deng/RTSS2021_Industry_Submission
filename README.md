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
When selecting the 'bottleneck' among all processors, we first calculate the 'starting makespan' of each procesor assuming there is no resource conflict between tasks. We illustrate the process using the example above. The **execution time (ET)** of each node is equal to their index, i.e., **ET(v_n) = n**. All the tasks are parallelly listed starting from source node and end at sink node.  

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

Consider the case where the execution time (ET), starting time (ST) and deadline (DD) of each node is given in the table below, we then calculate the finish time (FT) of each node and Lmax of that schedule. **Lmax = FT - DD**.  

|               |        1      |       2       |       4       |       6       |
| ------------- | ------------- | ------------- | ------------- | ------------- |
|       ET      |        1      |       2       |       4       |       6       |
|       ST      |        0      |       1       |       3       |       6       |
|       DD      |        1      |       3       |       8       |      13       |

The Branch and Bound (BnB) search starts at the root node and expands to multiple branches. During this process, we apply the branch and cut (BnC) technique as well, i.e., if we find one branch violates the precedence constraint in DAG, or its lower bound of Lmax is larger than the existing schedule, it will be cut directly without further search. Consider the tree below as an example,  
<div align=center><img width="350" height="300" src="https://github.com/Xuanliang-Deng/RTSS2021_Industry_Submission/blob/master/images/BNBTree.png"/></div>

Exploring from the (dummy) root node, the schedule may start at node v1, v2, v4, v6, respectively. However, in DAG, node v1 has to execute first since it is the **predecessor** of all the remaining nodes. We know the schedules starting at v2, v4, v6 (i.e., 2XXX,4XXX,6XXX) are all **infeasible** (denoted in red) due to the violation of precedence constraint in DAG. These branches are cut directly.  

In addition, we further improve the efficiency of the search by cutting branches whose lower bound value of Lmax is larger than the existing schedule. For example, the maximal Lmax of schedule 1246 is 0, while for schedule 1624 the Lmax of node 2 is already 11, which is larger than schedule 1246. Then we do not even need to complete the search for schedule along this branch because its lower bound already gets outperformed by exisiting schedule. That is, we do not need to compute the Lmax for node 4 in this schedule. It will be cut diretly (denoted in blue) once its Lmax exceeds the existing best one.   

We give two examples, (i)schedule 2XXX and (ii) schedule 1624, to illustrate our improvement in search techniques. 

| schedule 1246 |        1      |       2       |       4       |       6       |
| ------------- | ------------- | ------------- | ------------- | ------------- |
|       FT      |        1      |       3       |       7       |       13      |
|       DD      |        1      |       3       |       8       |       13      |
|     Lmax      |        0      |       0       |       -1      |        0      |
|     Lmax(schedule 1246) = max(0,0,-1,0) = 0                                   |

| schedule 2xxx |        2      |       X       |       X       |       X       |
| ------------- | ------------- | ------------- | ------------- | ------------- |
|     Lmax(schedule 2XXX) = infeasible, do not need to compute    |

| schedule 1624 |        1      |       6       |       2       |       4       |
| ------------- | ------------- | ------------- | ------------- | ------------- |
|       FT      |        1      |       12      |       14      |        X      |
|       DD      |        1      |       13      |       3       |        8      |
|     Lmax      |        0      |       -1      |       11      |        X      |
|     Lmax(schedule 1624) = Computing is not finished               |
|     Process ends at node 2 since its Lmax is 11, larger than existing schedule 1246               |  

- Shift bottleneck processor  
Once the optimal schedule of bottleneck processor is determined, we shift the bottleneck to the next processing unit which has maximal value of starting makespan in step 2. The whole process is terminated when all the processor are traversed.


