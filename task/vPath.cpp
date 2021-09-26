/**
 * Authors:  Zahaf Houssam-Eddine, Xuanliang Deng
 * Function: Define Taskset-related functions
 * Date:     Sept.5, 2021
 * Version:  v1
 * Log:      - Revised the template <typename T> part (this should be put into hpp file, cant be separate in cpp file)
 * Comments: - The reason to use pointer here is we need to CHANGE the original vector after operation
 *           
*/
#include "vPath.hpp"

namespace task
{
    void Path::add(Subtask *v)
    {
        this->subtasks.push_back(v);
    }

    std::vector<Subtask *> *Path::_subtasks()
    {
        return &this->subtasks;
    }

    void Path::display()
    {
        std::cout << "[Path :";
        for (auto v : subtasks)
            std::cout << std::to_string(v->_id()) << "  ,";
        std::cout << "]\n";
    }

    long int Path::exec_time(){
        long int C = 0 ; 
        for (auto v : subtasks)
        C+= v->_C();
        return C;
    }

    Path *Path::clone()
    {
        Path *p = new Path();
        for (auto v : subtasks)
            p->add(v);
        return p;
    }

    // -------------- Newly added for SRP ------------
    void Path::set_path_D(int deadline)
    {
        this->path_D = deadline;
    }

    // -------------- Stochastic HPC model ------------
    Path *create_crit_path(std::vector<int> *vlist)
    {
        Path *p = new Path();
        for(auto iter = vlist->begin(); iter < vlist->end(); iter++)
        {
            int vIndex = *iter;
            Subtask *v = new Subtask(vIndex);
            p->add(v);
        }
        return p;
    }

    bool belong_to_crit_path(Subtask *v, Path *crit_path)
    {
        int target_id = v->_id();
        std::vector<Subtask *> *crit_subtask;
        crit_subtask = crit_path->_subtasks();

        for(auto v: (*crit_subtask) )
        {
            if(target_id == v->_id())
                return true;
        }
        return false;
    }
    // End of additional parts



    
    // -------------- Stochastic HPC model ------------



} /* namespace task */