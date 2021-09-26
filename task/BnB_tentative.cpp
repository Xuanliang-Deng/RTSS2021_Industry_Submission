/**
 * Authors:  Xuanliang Deng
 * Function: Branch and Bound techniques
 * Date:     Sept.9, 2021
 * Version:  v1
 * Comments: 1. None, first trial for implementation
 *           
*/

// Program to solve Job Assignment problem
// using Branch and Bound and cut
#include "bbNode.hpp"
#include "BnB.hpp"
#include <list>
#include <limits.h>


// state space tree node
struct Node
{
    // stores parent node of current node helps in tracing path when answer is found
    Node* parent;
 
    // contains cost for ancestors nodes including current node
    /* int pathCost; */
    int makespan;
 
    // contains least promising cost
    int Lmax;
 
    // contain worker number. this is not useful. since we are schedule on the single processor
    int workerID;
 
    // contains Job ID change this to node index
    /* int jobID; */
	int vID;
 
    // Boolean array assigned will contains
    // info about available jobs?????????? maybe T T F T it means node 3 is not assigned yet
    /*bool assigned[N];*/
    std::vector<bool> assigned;

};
 
/* Allocate a new search tree node */
/* INitialize root node with vID = -1 */
Node* newNode(int _vID, std::vector<bool> _assigned, Node* _parent)
{
    Node* node = new Node;

    for (auto ele: _assigned)
    {
        node->assigned.push_back(ele);
    }
    // TO DO: Here we need an extra function to convert _vID to its corresponding spots in vector, 
    // cuz the vIDs are not necessarily starting from 0 to N-1 (e.g., v1,v2,v4,v6)
    node->assigned[_vID] = true; 
    node->parent = _parent;
    node->vID = _vID;
 
    return node;
}


// Function to calculate the LEAST PROMISING COST
// of node after worker x is assigned to job y.
// Calculate
int calculateCost(int _level, int _vID, std::vector<bool> _assigned)
{
    // Initialize std::vector available to store unavailable jobs
    auto N = _assigned.size();
    std::vector<bool> available;
    for (int i = 0; i<N; i++)
    {
        available.push_back(true);
    }

        int cost = 0;
 
    // start from next worker
    for (int i = _level + 1; i < N; i++)
    {
        // TO DO: here Lmax is min, cost update will be makespan
        int min = INT_MAX, minIndex = -1;
 
        // do for each job, search for least cost <worker,job> pair
        for (int j = 0; j < N; j++)
        {
            // if job is unassigned
            if (!_assigned[j] && available[j])
            {
                // Calculate sth and compare with Lmax, if larger cut this branch
                // TO DO: Calculate Lmax()
                // store job number
                minIndex = j;
 
                // store cost
                min = costMatrix[i][j];
            }
        }/* end for */
 
        // add cost of next worker
        cost += min;
 
        // job becomes unavailable
        available[minIndex] = false;
        // then return to search next <worker,job> pair with least cost
    }
 
    return cost;
}
 
// Comparison object to be used to order the heap
struct comp
{
    bool operator()(const Node* lhs,
                   const Node* rhs) const
    {
        return lhs->cost > rhs->cost;
    }
};
 
// print Assignments
void printAssignments(Node *min)
{
    if(min->parent==NULL)
        return;
 
    printAssignments(min->parent);
    cout << "Assign Worker " << char(min->workerID + 'A')
         << " to Job " << min->jobID << endl;
 
}
 
// Find minimum Lmax using Branch and Bound (Sept.21)
int BnB(std::vector<task::Subtask*> subtasks)
{
    int N = subtasks.size();
    std::vector<bool> assigned;
    for (int i = 0; i<N; i++)
    {
        assigned.push_back(false);
    }

    // Initialize dummy root node
    Node* root = newNode(-1, assigned, NULL);
    root->makespan = 0;
    root->Lmax     = 0;

    /* Priority queue to store live nodes of search tree */
    std::priority_queue<Node*, std::vector<Node*>, comp> pq;

    // 





    /* Priority queue to store live nodes of search tree */
    std::priority_queue<Node*, std::vector<Node*>, comp> pq;
 
    // initialize heap to dummy node with cost 0

    Node* root = newNode(-1, -1, assigned, NULL);
    root->pathCost = root->cost = 0;
    root->workerID = -1;
 
    // Add dummy node to list of live nodes; KEY PARTS START FROM HERE
    pq.push(root);
 
    // Finds a live node with least cost,
    // add its childrens to list of live nodes and
    // finally deletes it from the list.
    while (!pq.empty())
    {
      // Find a live node with least estimated cost
      Node* min = pq.top();
 
      // The found node is deleted from the list of
      // live nodes
      pq.pop();
 
      // i stores next worker
      int i = min->workerID + 1;
 
      // if all workers are assigned a job
      if (i == N)
      {   // the reason we stop at N not N+1 is that we start from 0 to N-1
          printAssignments(min);
          return min->cost;
      }
 
      // do for each job, THE PARTS I DONT UNDERSTAND, worker corresponds to spot, exe order here
      for (int j = 0; j < N; j++)
      {
        // If unassigned
        if (!min->assigned[j])
        {
            // Here we need to add the cut part, compare it with some value, if larger then cut
          // create a new tree node
          Node* child = newNode(i, j, min->assigned, min);
 
          // cost for ancestors nodes including current node
          child->pathCost = min->pathCost + costMatrix[i][j];
 
          // calculate its lower bound
          child->cost = child->pathCost +
            calculateCost(costMatrix, i, j, child->assigned);
 
          // Add child to list of live nodes;
          pq.push(child);
        }
      }
    }
}
 
// Driver code
int main()
{
    // x-coordinate represents a Worker
    // y-coordinate represents a Job
    int costMatrix[N][N] =
    {
        {9, 2, 7, 8},
        {6, 4, 3, 7},
        {5, 8, 1, 8},
        {7, 6, 9, 4}
    };
 
 
    /* int costMatrix[N][N] =
    {
        {82, 83, 69, 92},
        {77, 37, 49, 92},
        {11, 69,  5, 86},
        { 8,  9, 98, 23}
    };
    */
 
    /* int costMatrix[N][N] =
    {
        {2500, 4000, 3500},
        {4000, 6000, 3500},
        {2000, 4000, 2500}
    };*/
 
    /*int costMatrix[N][N] =
    {
        {90, 75, 75, 80},
        {30, 85, 55, 65},
        {125, 95, 90, 105},
        {45, 110, 95, 115}
    };*/
 
 
    cout << "\nOptimal Cost is "
        << findMinCost(costMatrix);
 
    return 0;
}