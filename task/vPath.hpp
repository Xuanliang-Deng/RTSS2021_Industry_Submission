/**
 * Authors:  Zahaf Houssam-Eddine, Xuanliang Deng
 * Function: Define path-related functions
 * Date:     Sept.5, 2021
 * Version:  v2
 * Comments: 1. Our model should add additional safety-critical path, which is missing from original version
 *           
*/
#ifndef VPATH_HPP
#define VPATH_HPP

#include "subtask.hpp"

namespace task
{
    class Subtask;

    class Path
    {
    private:
        std::vector<Subtask *> subtasks;
        int path_D;


    public:
        void add(Subtask *v);
        std::vector<Subtask *> *_subtasks();
        void display();
        Path *clone();
        long int exec_time();

        // Newly added for SRP
        void set_path_D(int deadline);
    };

    // --------------- Newly added for SRP/Stochastic HPC model ---------------
    Path *create_crit_path(std::vector<int> *vlist);
    bool belong_to_crit_path(Subtask *v, Path *crit_path);

    struct less_than_path
    {
        inline bool operator()(Path *struct1, Path *struct2)
        {
            return (struct1->exec_time() > struct2->exec_time());
        }
    };

} /* namespace task */
#endif