/**
 * Authors:  Xuanliang Deng
 * Function: Branch and Bound for S-bottleneck algorithm
 * Date:     Sept.12, 2021
 * Version:  v1
 * Log:      None, first trial for implementation
 * Comments: In progress   
 */
#include "bbNode.hpp"
#include "BnB.hpp"

/**
 * Func: (i) Select bottleneck processor (ii) Sort from max to min 
 * @param *DAG: Task* pointer to the DAG task
 * @return:     Pair of (proc_tag, makespan)
 * Status: Tested
 */
std::pair<int, int> select_bottleneck(task::Task *DAG)
{
    // @@TO DO 1: this is more like one-shot selection rn, we still need to shift bottleneck to next max,
    // we need another input parameter std::vector<int> assigned_tags which contains 
    // tag-proc that we need to ignore in the last step 

    // Retrieve list of subtasks from DAG task
    std::vector<task::Subtask *> subtasks = *DAG->_subtasks();
    std::vector<int> tags;

    // Extract all existing tags from subtask list
    for (auto v : subtasks)
    {
        if(std::find(tags.begin(), tags.end(), v->_TAG()) != tags.end() )
        {
            continue;
        }
        else
        {
            tags.push_back(v->_TAG());
        }
    }

    // Store starting makespan of all subtasks
    std::vector<int> node_makespan;

    for(long unsigned int i = 0; i < tags.size(); i++)
    {
        int proc_makespan = 0;
        for(auto v: subtasks)
        {   
            // Iterate from first tag to last, store the largest to proc_makespan
            if(v->_TAG() == tags[i])
            {
                int node_makespan;
                node_makespan = v->_ST() + v->_C();
                if(node_makespan > proc_makespan)
                {
                    proc_makespan = node_makespan;
                }
            }
        }
        node_makespan.push_back(proc_makespan);
    }

    // We need to change this part for iterative search for bottleneck processor

    int max_position = std::max_element(node_makespan.begin(), node_makespan.end()) - node_makespan.begin();
    std::pair<int, int> selected_pair = std::make_pair(tags[max_position], node_makespan[max_position]);

    return selected_pair;
} 


/**
 * Func: (i) Select bottleneck processor (ii) Sort from max to min 
 * @param *DAG: Task* pointer to the DAG task
 * @return:     Pair of (proc_tag, makespan)
 * Status: Tested
 */
std::vector<std::pair<int, int>> select_bottleneck_iterative(task::Task *DAG)
{
    // @@TO DO 1: this is more like one-shot selection rn, we still need to shift bottleneck to next max,
    // we need another input parameter std::vector<int> assigned_tags which contains 
    // tag-proc that we need to ignore in the last step 

    // Retrieve list of subtasks from DAG task
    std::vector<task::Subtask *> subtasks = *DAG->_subtasks();
    std::vector<int> tags;

    // Extract all existing tags from subtask list
    for (auto v : subtasks)
    {
        if(std::find(tags.begin(), tags.end(), v->_TAG()) != tags.end() )
        {
            continue;
        }
        else
        {
            tags.push_back(v->_TAG());
        }
    }

    // Store starting makespan of all subtasks
    std::vector<int> node_makespan;

    for(long unsigned int i = 0; i < tags.size(); i++)
    {
        int proc_makespan = 0;
        for(auto v: subtasks)
        {   
            // Iterate from first tag to last, store the largest to proc_makespan
            if(v->_TAG() == tags[i])
            {
                int node_makespan;
                node_makespan = v->_ST() + v->_C();
                if(node_makespan > proc_makespan)
                {
                    proc_makespan = node_makespan;
                }
            }
        }
        node_makespan.push_back(proc_makespan);
    }

    // Sept.24, 2021. We need to change this part for iterative search for bottleneck processor
    std::vector<std::pair<int, int>> proc_pairs;
    int N = tags.size();

    for(int i = 0; i<N; i++)
    {
        // Each time search for the maximal and pop the max from vector
        int max_position = std::max_element(node_makespan.begin(), node_makespan.end()) - node_makespan.begin();
        std::pair<int, int> selected_pair = std::make_pair(tags[max_position], node_makespan[max_position]);
        proc_pairs.push_back(selected_pair);
        tags.erase(tags.begin() + max_position);
        node_makespan.erase(node_makespan.begin() + max_position);

    }

    return proc_pairs;
} 


/**
 * Func: Convert subtask to bbNode
 * @param *v: pointer to a subtask
 * @return *bbNode: pointer to new bbNode
 * Status: Tested
 */
bbNode* subtask_to_bbNode(task::Subtask* v)
{   
    // @@TO DO 2: Need to consider criticality in future revision 
    int _id, _C, _ST, _D, _criticality;
    _id =  v->_id();
    _C  =  v->_C();
    _ST =  v->_ST();
    _D  =  v->_D();
    _criticality = v->_criticality();
    bbNode* new_bbNode = new bbNode(_id, _C, _ST, _D, _criticality);
    return new_bbNode;
}

/**
 * Func: Convert bbNode to subtask
 * @param *v: pointer to a bbNode
 * @return *Subtask: pointer to new subtask
 * Status:  
*/
bool is_predecessor(bbNode* s, bbNode* d, task::Task* DAG, std::vector<task::Subtask *> bSubtasks)
{
    bool result;
    // Backtrace to root of the path
    if(d->parent != NULL)
    {
        result = is_predecessor(s, d->parent, DAG, bSubtasks);
        if(result == true)
            return result;
        // When reach root of the path
        // index in bbNode
        int sBidx, dBidx;
        sBidx = s->id;
        dBidx = d->id;

        // convert to index in subtask
        int sSidx, dSidx;
        task::Subtask* v_source  = bSubtasks[sBidx];
        task::Subtask* v_destiny = bSubtasks[dBidx];
        sSidx = DAG->indexes[v_source];
        dSidx = DAG->indexes[v_destiny];

        result = (DAG->graph[sSidx][dSidx] == 1);
        return result;
    }

    // When reach root of the path
    int s_index, d_index;
    s_index = s->id;
    d_index = d->id;
    result = (DAG->graph[s_index][d_index] == 1);
    return result;
}

/**
 * Func: Allocate a new search tree node
 * @param _level:    depth of the node in the tree
 * @param _id:       id of the allocated node
 * @param _assigned: bool vector contains flags for all nodes
 * @param _parent:   pointer to parent node
 * Status: Tested
 */
bbNode* newNode(int _level, int _id, std::vector<bool> _assigned, bbNode* _parent)
{
    bbNode* node = new bbNode;
    for (auto ele: _assigned)
    {
        node->assigned.push_back(ele);
    }
    // @@TO DO 3: Here we need an extra function to convert _vID to its corresponding spots in vector, 
    // cuz the vIDs are not necessarily starting from 0 to N-1 (e.g., v1,v2,v4,v6)
    node->level  = _level;
    node->id     = _id;
    if(_id >= 0)
    {
        node->assigned[_id] = true; 
    }
    node->parent = _parent;
    
    return node;
}

/**
 * Func: Calculate Lmax for one bbNode 
 * @param cur_node: pointer to current node to be added in schedule
 * Status: Tested
 */
void cal_node_Lmax(bbNode *cur_node)
{
    // If the current node is the root node (no parent)
    if(cur_node->parent == NULL)
    {
        cur_node->makespan = cur_node->ST + cur_node->C;
        cur_node->Lmax = cur_node->makespan - cur_node->D;
    }
    else
    {
        // If there are nodes already scheduled before current node
        // Compare current makespan and start time of cur_node before computing Lmax
        if(cur_node->parent->makespan > cur_node->ST)
        {
            cur_node->makespan = cur_node->parent->makespan + cur_node->C;
        }
        else
        {
            cur_node->makespan = cur_node->ST + cur_node->C;
        }
        cur_node->Lmax = cur_node->makespan - cur_node->D;
    }
}

/**
 * Func: Print information about schedule
 * @param cur_node: pointer to current node to be added in schedule
 * Status: Tested
 */
void printAssignments(bbNode *cur_node)
{
    if(cur_node->parent==NULL)
        return;
 
    printAssignments(cur_node->parent);
    std::cout << "level " << cur_node->level << " is" \
    << " node " << cur_node->id <<  " " << "with Lmax " << cur_node->Lmax << std::endl;
}

/** 
 * Func: Backtrace the schedule starting from last node
 * @param cur_node: pointer to the last node in the schedule
 * Status: in progress
 * Date:   Sept.23, 2021
*/
std::vector<bbNode*>* backtrace_sched(bbNode *cur_node, std::vector<bbNode*> *cur_sched)
{
    if(cur_node->parent==NULL)
    {
        return cur_sched;
    }

    cur_sched = backtrace_sched(cur_node->parent, cur_sched);
    cur_sched->push_back(cur_node);
    return cur_sched;
}


/**
 * Function: Add cur_node to the existing schedule so far 
 * @param cur_node: pointer to current node
 * @param assignedNode: vector which stores the schedule so far 
 */
void sched_new_bbNode(bbNode* cur_node, std::vector<bbNode*> assignedNode)
{
    // If assignedNode is empty, set the parent node to NULL.
    if(assignedNode.size() == 0)
    {
        // No predecessor exists
        cur_node->parent = NULL;
        // Calculate Lmax of this node
        cal_node_Lmax(cur_node);
    }
    else
    {
        // Last node in schedule is the predecessor
        bbNode* _parent = assignedNode.back();
        cur_node->parent = _parent;
        cal_node_Lmax(cur_node);
    }
}



/** Function: Calculate Lmax of the final schedule
 *  @param assignedNode schedule 
 *  @return the value of Lmax among the schedulee
 */
int cal_sched_Lmax(std::vector<bbNode*> assignedNode)
{
    int Lmax = -1;
    for(auto v: assignedNode)
    {
        if((v->Lmax) > Lmax)
            Lmax = v->Lmax;
    }
    return Lmax;
}





// print Assignments
void printLmax(bbNode *min, int N)
{
    if(min->parent==NULL)
        return;
 
    printLmax(min->parent, N);
    std::cout << "Lmax " << min->Lmax <<  std::endl;
}



/* Branch and bound techniques- DAG ver. (draft) */
void branch_and_bound(task::Task DAG)
{
    // Select bottlenek processor iterative
    std::vector<std::pair<int, int>> bottleNeck_pairs = select_bottleneck_iterative(&DAG); 
    std::vector<task::Subtask *> all_subtasks = *(DAG._subtasks());
    int size_bottleneck = bottleNeck_pairs.size();
    
    // Start the whole loop for shifting bottleneck
    for(int p = 0; p<size_bottleneck; p++)
    {
        int bTag = bottleNeck_pairs[p].first;

        // Select nodes with tag of bottleneck processor
        std::vector<task::Subtask *> bSubtasks;
        for(auto v: all_subtasks)
        {
            if(v->_TAG() == bTag)
                {
                    bSubtasks.push_back(v); 
                }
        }

        // Convert Subtasks to bbNodes 
        std::vector<bbNode* > bbNodeVec;
        for (auto ele: bSubtasks)
        {
            bbNode* node = subtask_to_bbNode(ele);
            bbNodeVec.push_back(node);
        }

        // Comparison rule (decreasing) 
        struct comp
        {   // criticality first (decreasing), if equal, then sort id (increasing) 
            bool operator()(bbNode* lhs, bbNode* rhs) const
            {   
                return (lhs->_criticality() < rhs->_criticality()) || \
                ((lhs->_criticality() == rhs->_criticality())&& (lhs->_id()>rhs->_id()) ); 
            }
        };

        // Create a priority queue to store live nodes of search tree 
        std::priority_queue<bbNode*, std::vector<bbNode*>, comp> pq;

        // Max depth of schedule  
        int N = bSubtasks.size();

        // Initialize root node, add it to list of live nodes
        std::vector<bool> assigned;
        for(int i=0; i<N; i++)
        {
            assigned.push_back(false);
        }

        bbNode* root = newNode(-1, -1, assigned, NULL);
        pq.push(root);

        // ASSERT_EQ(root->_id(), -1);
        // ASSERT_EQ(root->level, -1);
        // ASSERT_EQ(1,  pq.size());
        // ASSERT_EQ(-1, pq.top()->id);

        // Store the optimal schedule
        int opt_Lmax = INT_MAX;
        std::vector<bbNode*> opt_sched;

        while(!pq.empty())
        {
            // Find a live node with smallest index. TO DO: consider criticality as well
            bbNode* cur_node = pq.top();

            // The found node is deleted from the list
            pq.pop();

            // i stores next level
            int level =  cur_node->level + 1;

            // if all nodes are assigned, print info
            if (level == N)
            {   
                // Create cur_sched to store schedule
                std::vector<bbNode*> cur_sched;
                bbNode* dummy = new bbNode(-1);
                cur_sched.push_back(dummy);

                printAssignments(cur_node);
                cur_sched = *(backtrace_sched(cur_node, &cur_sched));

                // Print the schedule info
                std::cout << "The schedule is: " <<std::endl;
                for(auto v: cur_sched)
                {   
                    // Skip first dummy node
                    if(v->_id() == -1)
                    {
                        continue;
                    }
                    task::Subtask* vbbNode = bSubtasks[v->id];
                    std::cout << "v" << vbbNode->_id() << " ";
                }
                std::cout << "\n";

                int schedLmax = INT_MIN;
                for(auto v: cur_sched)
                {
                    if(v->Lmax > schedLmax)
                    {
                        schedLmax = v->Lmax;
                    }
                }

                if(schedLmax < opt_Lmax)
                {
                    opt_sched = cur_sched;
                    opt_Lmax  = schedLmax;
                }
                
                // Print the Lmax of this schedule
                std::cout << "Lmax of this schedule is: " << schedLmax <<std::endl;
                std::cout << "\n";
            }

            int minLmax = INT_MAX; 

            // If not all scheduled, check all available children
            // search for each job
            for(int vID = 0; vID < N; vID++)
            {
                // If unassigned
                if (!cur_node->assigned[vID])
                {
                    // Create a new tree node
                    bbNode* child = newNode(level, vID, cur_node->assigned, cur_node);
                    child->ST = bbNodeVec[vID]->ST;
                    child->C  = bbNodeVec[vID]->C;
                    child->D  = bbNodeVec[vID]->D;
                
                    // Compare child with cur_node, check if constraint exists, if so, continue
                    // Check if any precedence violation
                    if(is_predecessor(child, cur_node, &DAG, bSubtasks))
                    {
                        continue;
                    }

                    // Update makespan and Lmax of node child
                    cal_node_Lmax(child);
                    
                    if(child->Lmax < minLmax)
                    {
                        minLmax = child->Lmax;
                        // If Lmax is larger than current Lmax we dont put it in
                        pq.push(child);
                    } 
                }
            }
        }
        std::cout << "The optimal Lmax is: " << opt_Lmax << std::endl;

        // Print the schedule info
        std::cout << "The optimal schedule of " << tag_to_str(bTag) << " is: " <<std::endl;
        for(auto v: opt_sched)
        {   
            // Skip first dummy node
            if(v->_id() == -1)
            {
                continue;
            }

            // Convert from array index to subtask index
            task::Subtask* vbbNode = bSubtasks[v->id];

            std::cout << "v" << vbbNode->_id() << " ";
            //std::cout << "v" << v->id << " ";
        }
        std::cout << "\n" << "---------------------" << "\n";
    }
}
