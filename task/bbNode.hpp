/**
 * Authors:  Xuanliang Deng
 * Function: Branch and Bound Node
 * Date:     Sept.11, 2021
 * Version:  v1
 * Log:      None, first trial for implementation
 * Comments: Ongoing       
*/
#ifndef BBNODE_HPP
#define BBNODE_HPP

#include "processor.hpp"

class bbNode
{
public:

    // Node id
	int id;

    // Start time (ready time with DAG constraints)
    int ST;

    // Execution time
    int C;

    // Due time (for critical node, if not, set this as max(makespan))
    int D;

    // Lmax value
    int Lmax;

    // Criticality
    int criticality;

    // Makespan which contains cur_node, mkspan is finish time
    int makespan;

    // Store information about visited or not
    int visited;

    // Stores the depth of the tree
    int level;

    // Store parent node of cur_node, useful when backtracing the path
    bbNode* parent;

    std::vector<bool> assigned;


public:
    bbNode();
    bbNode(int id);
    bbNode(int _id, int _C, int _ST, int _D, int _criticality);
    ~bbNode();

    // Setter
    void _criticality(int _criticality);

    // Getter
    int _id();
    int _criticality();
    int _ST();
    int _D();

};

#endif /* BBNODE_HPP */


