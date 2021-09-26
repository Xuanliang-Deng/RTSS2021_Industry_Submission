/**
 * Authors:  Zahaf Houssam-Eddine, Xuanliang Deng
 * Function: Define structures for (typed) processors
 * Date:     Sept.9, 2021
 * Version:  v1
 * Log:      - Started implementation on Sept. 9th
 * Status:   - Done
 *           
*/
#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include "../task/taskset.hpp"
#include "../task/task.hpp"
#include "../common/common.hpp"

#include <climits>
#include <vector>
#include <queue>

namespace task
{
  class Taskset;
  class Task;
  class Subtask;
} /* namespace task */

namespace platform
{

  class Processor
  {
    private:
      int id;
      int TAG;
      double utilization;
      std::vector<task::Subtask *> sub_task_list;
      std::vector<task::Task *> task_list;

    public:
      Processor();
      Processor(int id);
      Processor(int id, int tag);
      ~Processor();

      // Setter
      void _id(int id);
      void _TAG(int tag);
      void _utilization(double util);

      // Getter 
      int _id();
      int _TAG();
      double _utilization();
      std::vector<task::Subtask * > * _sub_task_list();
      std::vector<task::Task * >  * _task_list();

      // Utility
      bool allocate_sub_task(task::Subtask *subtask);
      void add_utilization(double u);
      void remove_utilization(double u);
      Processor * copy();
      Processor *cloneTagOnly();
      
      // Print information
      void display();
      void display_alloc_tasks();

  };

} /* namespace platform */

#endif
