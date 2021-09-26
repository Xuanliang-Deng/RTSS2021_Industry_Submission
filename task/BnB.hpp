/**
 * Authors:  Xuanliang Deng
 * Function: Branch and Bound for S-bottleneck algorithm
 * Date:     Sept.9, 2021
 * Version:  v1
 * Log:      None, first trial for implementation
 * Comments: Ongoing       
*/

#include "../common/common.hpp"
#include "subtask.hpp"
#include "vPath.hpp"
#include "bbNode.hpp"
#include "processor.hpp"
#include "task.hpp"
#include <list>
#include <limits.h>


/**
 * S-bottleneck algorithm structure:
 * 1. Given a DAG, select bottleneck processor by assuming no resource conflict, return <tag, makespan> pair (complete)
 * 2. For bottleneck processor, apply BnB technique on a single-processor analysis basis (ongoing)
 *    Note: Imagine using DFS, but BFS first to filter precedence accrd. to criticality
 * 3. Calculate Lmax for each node, Lmax for the whole schedule, pick the minimum as optimal schedule (complete)
 * 
 * Utility functions that are essential:
 * 1. Convert subtask to bbNode (complete)
 * 2. All the related calculation: makespan, Lmax (complete)
 * 3. Define criticality of nodes, critical path (complete)
 * 4. Path analysis to determine whether there is constraints between two nodes (ongoing)
 * -  **graph[n][n] use 0/1 to store information about precedence, this can be used for filtering in BnB
 * -  critical node first, node in crit path second, normal node last
*/

// Select bottleneck processor 
std::pair<int, int> select_bottleneck(task::Task *DAG);

std::vector<std::pair<int, int>> select_bottleneck_iterative(task::Task *DAG);

// Convert a subtask to bbNode (add more timing info D)
bbNode* subtask_to_bbNode(task::Subtask* v);

bool is_predecessor(bbNode* s, bbNode* d, task::Task* DAG, std::vector<task::Subtask *> bSubtasks);

// Add current node to schedule
void sched_new_bbNode(bbNode* cur_node, std::vector<bbNode*> assignedNode);

// Calculate Lmax of node (based on makespan of predecessor in schedule)
void cal_node_Lmax(bbNode *cur_node);

// Calculate Lmax of the schedule 
int  cal_sched_Lmax(std::vector<bbNode*> assignedNode);



// (Ongoing) Given a DAG return the 'multiple' schedules for all processors
// One schedule for each tag/processor
void branch_and_bound(task::Task DAG);

void printAssignments(bbNode *min);

void printLmax(bbNode *min, int N);

bbNode* newNode(int _level, int _id, std::vector<bool> _assigned, bbNode* _parent);

std::vector<bbNode*>* backtrace_sched(bbNode *cur_node, std::vector<bbNode*> *cur_sched);