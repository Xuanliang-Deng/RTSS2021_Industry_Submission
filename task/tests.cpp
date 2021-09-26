/**
 * Authors:  Xuanliang Deng
 * Function: Unit tests for all functions
 * Date:     Sept.25, 2021
 * Version:  v1
 * Usage:    $cmake .  -> $make  -> ./TaskTests
 * Comments: 
 *           - first version done on Sept.25, 2021 
 *           
*/
#include "../common/common.hpp"
#include "gtest/gtest.h"
#include "subtask.hpp"
#include "vPath.hpp"
#include "BnB.hpp"

// -------------------------------------- Tests for utility functions -----------------------------------------------

/* Test: string to tag (int) */
TEST(CommonTests, Test_str2tag){
	ASSERT_EQ(0, str_to_tag("CPU"));
    ASSERT_EQ(1, str_to_tag("GPU"));
    ASSERT_EQ(100, str_to_tag("TAG_UNDEFINED"));
    ASSERT_EQ(100, str_to_tag("ANY_OTHER_INPUTS"));
}

/* Test: tag (int) to string */
TEST(CommonTests, Test_tag2str){
	ASSERT_EQ("CPU", tag_to_str(CPU));
    ASSERT_EQ("GPU", tag_to_str(GPU));
    ASSERT_EQ("TAG_UNDEFINED", tag_to_str(TAG_UNDEFINED));
    ASSERT_EQ("UNKNOWN", tag_to_str(PVA));
}

/* Test: whether vector contains target element */
TEST(CommonTests, Test_vecContains){
    std::vector<int> int_vect = {1,2,3,4,90};
    std::vector<double> dob_vect = {1.3,2.3,3.4,90.4};
    ASSERT_EQ(true, vector_contains(&int_vect, 1));
    ASSERT_EQ(true, vector_contains(&int_vect, 4));
    ASSERT_EQ(true, vector_contains(&int_vect, 90));
    ASSERT_EQ(false, vector_contains(&int_vect, 17));
    ASSERT_EQ(true, vector_contains(&dob_vect, 1.3));
    ASSERT_EQ(true, vector_contains(&dob_vect, 90.4));
    ASSERT_EQ(false, vector_contains(&dob_vect, 90.3));
    ASSERT_EQ(false, vector_contains(&dob_vect, 2.7));
}

/* Test: remove element from vector */
TEST(CommonTests, Test_vecRemove){
    std::vector<int> int_vect = {1,2,3,4,90};
    ASSERT_EQ(true, remove_from_vector(&int_vect, 90));
    ASSERT_EQ(true, remove_from_vector(&int_vect, 3));
    ASSERT_EQ(false, remove_from_vector(&int_vect, 3));
    ASSERT_EQ(false, remove_from_vector(&int_vect, 90));
    ASSERT_EQ(true, remove_from_vector(&int_vect, 4));
    ASSERT_EQ(2, int_vect.size());

    std::vector<double> dob_vect = {1.3,2.3,3.4,90.4};
    ASSERT_EQ(true, remove_from_vector(&dob_vect, 2.3));
    ASSERT_EQ(true, remove_from_vector(&dob_vect, 1.3));
    ASSERT_EQ(true, remove_from_vector(&dob_vect, 3.4));
    ASSERT_EQ(true, remove_from_vector(&dob_vect, 90.4));
    ASSERT_EQ(0, dob_vect.size());
}

/* Test: copy vector */
TEST(CommonTests, Test_vecCopy){
    std::vector<int> int_vect = {1,2,3,4,90};
    std::vector<int> int_vect_dst1;
    std::vector<int> int_vect_test = {1,2,5,4,90};
    std::vector<double> dob_vect = {1.3,2.3,3.4,90.4};
    std::vector<double> dob_vect_dst1;
    std::vector<double> dob_vect_test = {1.1,2.3,3.4,90.4};

    copy_vector(&int_vect, &int_vect_dst1);
    ASSERT_EQ(int_vect, int_vect_dst1);

    std::vector<int> int_vect_dst2;
    copy_vector(&int_vect, &int_vect_dst2);
    ASSERT_NE(int_vect_test, int_vect_dst2);

    copy_vector(&dob_vect, &dob_vect_dst1);
    ASSERT_EQ(dob_vect, dob_vect_dst1);

    std::vector<double> dob_vect_dst2;
    copy_vector(&dob_vect, &dob_vect_dst2);
    ASSERT_NE(dob_vect_test, dob_vect_dst2);
}

/* Test: vector minus */
TEST(CommonTests, Test_vec_minus){
    std::vector<int> int_vect1 = {1,2,3,4,90};
    std::vector<int> int_vect2 = {1,2,4,80};
    std::vector<int> int_vect3;
    std::vector<int> int_vect_test1 = {3,90};
    vector_minus(&int_vect1, &int_vect2, &int_vect3);
    ASSERT_EQ(int_vect_test1, int_vect3);

    std::vector<double> dob_vect1 = {1.3,2.3,3.4,90.4};
    std::vector<double> dob_vect2 = {13.56,24.7};
    std::vector<double> dob_vect3;
    std::vector<double> dob_vect_test1 = {1.3,2.3,3.4,90.4};
    vector_minus(&dob_vect1, &dob_vect2, &dob_vect3);
    ASSERT_EQ(dob_vect_test1, dob_vect3);
}

/* Test: vector merge without duplicates */
TEST(CommonTests, Test_merge_withoutDup){
    std::vector<int> int_vect1 = {1,2,3,4,90};
    std::vector<int> int_vect2 = {1,2,4,80};
    std::vector<int> int_vect_test1 = {1,2,3,4,90,80};
    merge_vectors_without_duplicates(&int_vect1, &int_vect2);
    ASSERT_EQ(int_vect_test1, int_vect1);

    std::vector<double> dob_vect1 = {1.3,2.3,3.4,90.4};
    std::vector<double> dob_vect2 = {13.56,24.7};
    std::vector<double> dob_vect_test1 = {13.56,24.7,1.3,2.3,3.4,90.4};
    merge_vectors_without_duplicates(&dob_vect2, &dob_vect1);
    ASSERT_EQ(dob_vect_test1, dob_vect2);
}

/* Test: vector merge with duplicates */
TEST(CommonTests, Test_merge_withDup){
    std::vector<int> int_vect1 = {1,2,3,4,90};
    std::vector<int> int_vect2 = {1,2,4,80};
    std::vector<int> int_vect_test1 = {1,2,3,4,90,1,2,4,80};
    merge_vectors_with_duplicates(&int_vect1, &int_vect2);
    ASSERT_EQ(int_vect_test1, int_vect1);

    std::vector<double> dob_vect1 = {1.3,2.3,3.4,90.4};
    std::vector<double> dob_vect2 = {13.56,24.7};
    std::vector<double> dob_vect_test1 = {13.56,24.7,1.3,2.3,3.4,90.4};
    merge_vectors_without_duplicates(&dob_vect2, &dob_vect1);
    ASSERT_EQ(dob_vect_test1, dob_vect2);
}

/* Test: define critical path */
TEST(TaskTests, Test_def_crit_path){
    // Given list of index, user-define critical
    std::vector<int> vIndex = {1,2,3,5,6};
    std::vector<int> vIndexTest;
    std::vector<int> vWrongIndexTest = {1,2,3,5,7};

    task::Path *p = task::create_crit_path(&vIndex);

    std::vector<task::Subtask *> *vCreatedList;
    vCreatedList = p->_subtasks();

    for(auto v = (*vCreatedList).begin(); v < (*vCreatedList).end(); v++)
    {
        vIndexTest.push_back((*v)->_id());
    }

    ASSERT_EQ(vIndex, vIndexTest);
}

/* Test: determine whether node belongs to critical path */
TEST(TaskTests, Test_belong_crit_path){
    std::vector<int> vIndex = {1,2,3,5,6};
    task::Path *p = task::create_crit_path(&vIndex);

    task::Subtask vTest1(1);
    task::Subtask vTest2(2);
    task::Subtask vTest3(4);
    task::Subtask vTest4(5);
    task::Subtask vTest5(7);

    ASSERT_EQ(true, belong_to_crit_path(&vTest1, p));
    ASSERT_EQ(true, belong_to_crit_path(&vTest2, p));
    ASSERT_NE(true, belong_to_crit_path(&vTest3, p));
    ASSERT_EQ(true, belong_to_crit_path(&vTest4, p));
    ASSERT_NE(true, belong_to_crit_path(&vTest5, p));
}

/* Test: critical node timing constraints */
TEST(TaskTests, Test_crit_node){
    std::vector<int> vCritPathIndex = {1,2,3,5,6};
    std::vector<int> vCritNodeIndex = {3,6};

    task::Path *p = task::create_crit_path(&vCritPathIndex);

    task::Subtask vTest1(1);
    task::Subtask vTest2(2);
    task::Subtask vTest3(3);
    task::Subtask vTest4(4);
    task::Subtask vTest5(5);
    task::Subtask vTest6(6);

    ASSERT_EQ(false, vTest1.is_crit_node(p, vCritNodeIndex));
    ASSERT_EQ(false, vTest2.is_crit_node(p, vCritNodeIndex));
    ASSERT_EQ(true, vTest3.is_crit_node(p, vCritNodeIndex));
    ASSERT_EQ(false, vTest4.is_crit_node(p, vCritNodeIndex));
    ASSERT_EQ(false, vTest5.is_crit_node(p, vCritNodeIndex));
    ASSERT_EQ(true, vTest6.is_crit_node(p, vCritNodeIndex));

    ASSERT_EQ(CRIT_P_NODE, vTest1._criticality());
    ASSERT_EQ(CRIT_P_NODE, vTest2._criticality());
    ASSERT_EQ(CRIT_NODE, vTest3._criticality());
    ASSERT_EQ(NORM_NODE, vTest4._criticality());
    ASSERT_EQ(CRIT_P_NODE, vTest5._criticality());
    ASSERT_EQ(CRIT_NODE, vTest6._criticality());

}

// -------------------------------------- Tests for S-bottleneck heuristic -----------------------------------------

/* Test: select bottleneck processor - one shot */
TEST(BnBTest, Test_select_bottleneck)
{
    task::Task DAG(1);
    std::vector<int> tags;
    task::Subtask v1(1, 1, 0, 1,  COMPUTE, NORM_NODE, CPU, &DAG);
    task::Subtask v2(2, 2, 1, 3,  COMPUTE, NORM_NODE, CPU, &DAG);
    task::Subtask v4(4, 4, 3, 8,  COMPUTE, NORM_NODE, PVA, &DAG);
    task::Subtask v6(6, 6, 7, 13, COMPUTE, NORM_NODE, GPU, &DAG);

    ASSERT_EQ(std::make_pair(GPU,13), select_bottleneck(&DAG));
}

/* Test: select bottleneck processor - iterative */
TEST(BnBTest, Test_select_bottleneck_iterative)
{
    task::Task DAG(1);
    std::vector<int> tags;
    task::Subtask v1(1, 1, 0, 1,  COMPUTE, NORM_NODE, CPU, &DAG);
    task::Subtask v2(2, 2, 1, 3,  COMPUTE, NORM_NODE, CPU, &DAG);
    task::Subtask v4(4, 4, 3, 8,  COMPUTE, NORM_NODE, PVA, &DAG);
    task::Subtask v6(6, 6, 7, 13, COMPUTE, NORM_NODE, GPU, &DAG);

    ASSERT_EQ(3, (select_bottleneck_iterative(&DAG)).size());
    ASSERT_EQ(GPU, select_bottleneck_iterative(&DAG)[0].first);
    ASSERT_EQ(13, select_bottleneck_iterative(&DAG)[0].second);
    
    ASSERT_EQ(PVA, select_bottleneck_iterative(&DAG)[1].first);
    ASSERT_EQ(7, select_bottleneck_iterative(&DAG)[1].second);

    ASSERT_EQ(CPU, select_bottleneck_iterative(&DAG)[2].first);
    ASSERT_EQ(3, select_bottleneck_iterative(&DAG)[2].second);
}

/* Test: convert subtask to bbNode */
TEST(BnBTest, Test_subtask_to_bbNode)
{
    task::Task DAG(1);
    std::vector<int> tags;
    task::Subtask v1(1, 1, 0, 1,  COMPUTE, NORM_NODE, CPU, &DAG);
    task::Subtask v2(2, 2, 1, 3,  COMPUTE, NORM_NODE, CPU, &DAG);
    task::Subtask v4(4, 4, 3, 8,  COMPUTE, NORM_NODE, PVA, &DAG);
    task::Subtask v6(6, 6, 7, 13, COMPUTE, NORM_NODE, GPU, &DAG);

    std::vector<bbNode*> nodeVec;
    bbNode* node1 = subtask_to_bbNode(&v1);
    bbNode* node2 = subtask_to_bbNode(&v2);
    bbNode* node3 = subtask_to_bbNode(&v4);
    bbNode* node4 = subtask_to_bbNode(&v6);

    nodeVec.push_back(node1);
    nodeVec.push_back(node2);
    nodeVec.push_back(node3);
    nodeVec.push_back(node4);

    // test id
    ASSERT_EQ(4, nodeVec[2]->_id());
    ASSERT_EQ(6, nodeVec[3]->_id());
    ASSERT_EQ(1, nodeVec[0]->_id());

    // test ST
    ASSERT_EQ(7, nodeVec[3]->_ST());
    ASSERT_EQ(0, nodeVec[0]->_ST());

    // test D
    ASSERT_EQ(1,   nodeVec[0]->_D());
    ASSERT_EQ(8,   nodeVec[2]->_D());
    ASSERT_EQ(13,  nodeVec[3]->_D());
}

/* Test: allocate new bbNode and calculate Lmax for node */
TEST(BnBTest, Test_allocate_bbNode_and_calNodeLmax)
{
    int level = -1;
    int  id   = -1;
    int  N    =  4;
    std::vector<bool> assigned;
    
    for(int i = 0; i<N; i++)
    {
        assigned.push_back(false);
    }

    // In assigned[id], id should be valid, 0 <= id < N
    bbNode* root = newNode(level, id, assigned, NULL);

    bbNode* child = newNode(level+1, id+1, assigned, root);
    child->ST = 1;
    child->D  = 5;
    child->C  = 1;
    cal_node_Lmax(child);
    ASSERT_EQ(2, child->makespan);
    ASSERT_EQ(-3, child->Lmax);

    bbNode* leaf = newNode(level+2, id+2, assigned, child);
    leaf->ST = 1;
    leaf->D  = 6;
    leaf->C  = 4;
    cal_node_Lmax(leaf);
    ASSERT_EQ(6, leaf->makespan);
    ASSERT_EQ(0, leaf->Lmax);
}

/* Test priority queue in BnB */
TEST(BnBTest, Test_Prio_Queue){

    task::Task DAG(1);
    std::vector<int> tags;
    task::Subtask v1(1, 1, 0, 1,  COMPUTE, NORM_NODE, CPU, &DAG);
    task::Subtask v2(2, 2, 1, 3,  COMPUTE, NORM_NODE, CPU, &DAG);
    task::Subtask v4(4, 4, 3, 8,  COMPUTE, NORM_NODE, CPU, &DAG);
    task::Subtask v6(6, 6, 6, 13, COMPUTE, NORM_NODE, CPU, &DAG);

    std::vector<task::Subtask *> subtasks = *(DAG._subtasks());
    
    // Comparison rule (decreasing)
    struct comp
    {
        bool operator()(task::Subtask* lhs, task::Subtask* rhs) const
        {   // criticality in decreasing order, if equal, then sort in increasing id
            return (lhs->_criticality() < rhs->_criticality()) || \
            ((lhs->_criticality() == rhs->_criticality())&& (lhs->_id()>rhs->_id()) ); 
        }
    };

    std::priority_queue<task::Subtask*, std::vector<task::Subtask*>, comp> pq;
    pq.push(&v2);
    pq.push(&v6);
    pq.push(&v1);
    pq.push(&v4);
    
    ASSERT_EQ(&v1, pq.top());
    pq.pop();
    ASSERT_EQ(&v2, pq.top());
    pq.pop();
    ASSERT_EQ(&v4, pq.top());
    pq.pop();
    ASSERT_EQ(&v6, pq.top());
}

/* NOT USED CURRENTLY: this is the discarded version of adding bbNode */
TEST(BnBTest, Test_bbNode){
    bbNode v1 = bbNode(1, 1, 0, 1, NORM_NODE);
    bbNode v2 = bbNode(2, 2, 0, 3, NORM_NODE);
    bbNode v4 = bbNode(4, 4, 3, 8, NORM_NODE);
    bbNode v6 = bbNode(6, 6, 6, 13,NORM_NODE);

    std::vector<bbNode*> assignedNode;

    sched_new_bbNode(&v1, assignedNode);
    ASSERT_EQ(1, v1.makespan );
    assignedNode.push_back(&v1);

    sched_new_bbNode(&v2, assignedNode);
    ASSERT_EQ(3, v2.makespan );
    assignedNode.push_back(&v2);

    sched_new_bbNode(&v6, assignedNode);
    ASSERT_EQ(12, v6.makespan );
    assignedNode.push_back(&v6);

    sched_new_bbNode(&v4, assignedNode);
    ASSERT_EQ(16, v4.makespan );
    assignedNode.push_back(&v4);

    ASSERT_EQ(8, cal_sched_Lmax(assignedNode));
}

/* KEY PART: Test whole process for BnB */
TEST(BnBTest, Test_BnB_whole)
{
    // Create DAG task
    task::Task DAG(1);
    std::vector<int> tags;
    task::Subtask v1(1, 1, 0, 1,  COMPUTE, NORM_NODE, CPU, &DAG);
    task::Subtask v2(2, 2, 1, 3,  COMPUTE, NORM_NODE, CPU, &DAG);
    task::Subtask v3(3, 3, 3, 8,  COMPUTE, NORM_NODE, GPU, &DAG);
    task::Subtask v4(4, 4, 3, 8,  COMPUTE, NORM_NODE, CPU, &DAG);
    task::Subtask v5(5, 5, 1, 9,  COMPUTE, NORM_NODE, GPU, &DAG);
    task::Subtask v6(6, 6, 6, 13, COMPUTE, NORM_NODE, CPU, &DAG);

    // Create edges between nodes
    DAG.link_two_subtasks(&v1, &v2);
    DAG.link_two_subtasks(&v1, &v4);
    DAG.link_two_subtasks(&v1, &v6);
    DAG.link_two_subtasks(&v2, &v4);
    DAG.link_two_subtasks(&v1, &v3);
    DAG.link_two_subtasks(&v1, &v5);

    // Test precedence constraints
    ASSERT_EQ(true,  DAG.is_predecessor(&v1, &v2));
    ASSERT_EQ(false, DAG.is_predecessor(&v2, &v1));
    ASSERT_EQ(true,  DAG.is_predecessor(&v1, &v4));
    ASSERT_EQ(true,  DAG.is_predecessor(&v1, &v6));
    ASSERT_EQ(true,  DAG.is_predecessor(&v2, &v4));
    ASSERT_EQ(false, DAG.is_predecessor(&v2, &v6));

    ASSERT_EQ(6, DAG.indexes.size());
    
    // Test vID
    ASSERT_EQ(0, DAG.indexes[&v1]);
    ASSERT_EQ(1, DAG.indexes[&v2]);
    ASSERT_EQ(2, DAG.indexes[&v3]);
    ASSERT_EQ(3, DAG.indexes[&v4]);
    ASSERT_EQ(4, DAG.indexes[&v5]);
    ASSERT_EQ(5, DAG.indexes[&v6]);

    ASSERT_EQ(true,  DAG.graph[DAG.indexes[&v1]][DAG.indexes[&v2]]);
    ASSERT_EQ(true,  DAG.graph[DAG.indexes[&v1]][DAG.indexes[&v4]]);
    ASSERT_EQ(true,  DAG.graph[DAG.indexes[&v1]][DAG.indexes[&v6]]);
    ASSERT_EQ(true,  DAG.graph[DAG.indexes[&v2]][DAG.indexes[&v4]]);
    ASSERT_EQ(false, DAG.graph[DAG.indexes[&v6]][DAG.indexes[&v1]]);
    ASSERT_EQ(true,  DAG.graph[DAG.indexes[&v1]][DAG.indexes[&v6]]);

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

/* MAIN TEST */
int main(int argc, char **argv){    
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}