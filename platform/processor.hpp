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
} // namespace task

namespace platform
{

  class Processor
  {
  private:
    int id;
    int policy;
    double utilization;
    int TAG;
    std::vector<task::Subtask *> sub_task_list;

    std::vector<task::Task *> task_list;

  public:
    Processor * copy();
    std::vector<task::Subtask * > * _sub_task_list();
    Processor *clonemultiply();
    Processor();
    Processor(int id);
    void _id(int id);
    int _id();
    int _policy();
    void _policy(int policy);
    int _TAG();
    void _TAG(int tag);
    Processor(int id, int tag, int policy);
    void display();
    void display_alloc_tasks();
    void add_utilization(double u);
    void remove_utilization(double u);
    bool check_utilization();
    double _utilization();
    void _utilization(double util);

    std::vector<task::Task * >  * _task_list();

    bool allocate_sub_task(task::Subtask *subtask);
  };

  // Decreasing sort of utilization
  struct less_than_processor_BF
  {
    inline bool operator()(Processor *struct1, Processor *struct2)
    {
      return (struct1->_utilization() > struct2->_utilization());
    }
  };

  // Increasing sort of utilization
  struct less_than_processor_WF
  {
    inline bool operator()(Processor *struct1, Processor *struct2)
    {
      return (struct1->_utilization() < struct2->_utilization());
    }
  };

} // namespace platform

#endif
