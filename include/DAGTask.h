#ifndef DAGTASK_H
#define DAGTASK_H

#include <vector>
#include <map>
#include <cmath>
#include <cstdlib>
#include <random>
#include <algorithm>
#include <iostream>
#include <fstream>

#include <include/node.h>
#include <yaml-cpp/yaml.h>


namespace SBH_DAG_Sched{

    class DAGTask{

    std::vector<Node*> vNode;

    float t = 0;        // Period of DAG task
    float d = 0;        // Deadline of DAG task
    float L = 0;        // Longest chain of DAG
    float volume = 0;   // Volume of DAG
    float u = 0;        // Utilization
    float wcw = 0;      // Worst-case workload (if no conditional branch exist, this equals volume)
    std::map<int, float> typedVol;  // volume for typed DAG [type of core, volume]
    std::map<int, float> pVol;      // volume for partitioned DAG [core id, volume]

    std::vector<int> ordIDs;        // ids in topological order
    public:

    float R = 0;          // response time

    DAGTask(){};
    DAGTask(const float T, const float D): t(T), d(D) {};
    ~DAGTask(){};

    //input - output

    void readTaskFromDOT(const std::string &filename);
    void saveAsDot(const std::string &filename);
    };
}

#endif /* DAGTASK_H */



