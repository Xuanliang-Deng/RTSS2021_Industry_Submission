#ifndef NODE_H
#define NODE_H

#include <vector>
#include <utility>

namespace SBH_DAG_Sched{

    enum nodeType {SENSOR_T, SYNC_T, NORMAL_T, COND_T, OR_T};
    enum nodeMode {PROB_M, DETER_M};

    class Node{
        public:
            int id      = 0;    // ID of the node
            int prio    = 0;    // priority of the node
            float c     = 0;    // WCET
            float d     = 0;    // deadline
            float t     = 0;    // period
            float r     = 0;    // response time
            int gamma   = 0;    // type of the assigned core
            int core    = 0;    // id of the assigned core

            nodeType nType   =  NORMAL_T; // By default node is normal computational node
            nodeMode nMode   =  DETER_M;  // Execution time is probabilistic or determined
            std::vector< std::pair<Node*,float> > succ;      // successors with probabilities
            std::vector< std::pair<Node*,float> > pred;      // predecessors with probabilities
    };
}
#endif /* NODE_H */