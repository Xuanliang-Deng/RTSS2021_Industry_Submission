/**
 * Authors:  Zahaf Houssam-Eddine, Xuanliang Deng
 * Function: Define Taskset-related functions
 * Date:     Sept.5, 2021
 * Version:  v1
 * Log:      - Revised the template <typename T> part (this should be put into hpp file, cant be separate in cpp file)
 * Comments: - The reason to use pointer here is we need to CHANGE the original vector after operation
 *           
*/

#include "taskset.hpp"

namespace task
{
  /**
   * Function: Constructor 1 of taskset 
   * @param id the ID of the task set
   */
  Taskset::Taskset(int id)
  {
    this->id = id;
  }

  /**
   * Function: Constructor 2 of taskset 
   * Initialize a task set with id -1 
   */
  Taskset::Taskset()
  {
    this->id = -1;
  }

  /**
   * Function: De-constructor of the Takset
   */
  Taskset::~Taskset()
  {
  }

  /**
   * Function: Calculate the Least common multiple (LCM) of the 2 given numbers
   * @param a First number
   * @param b Second number
   * @return LCM of a and b
   */
  long int Taskset::LCM(long int a, long int b)
  {
    long int x = a;
    long int y = b;
    long int t;
    while (b != 0)
    {
      t = b;
      b = a % b;
      a = t;
    }
    return (x * y) / a;
  }

  /**
   * Function: Calculate the hyperperiod, LCM of all periods of all tasksets
   * @return The hyperperiod
   */
  long int Taskset::hyperperiod()
  {
    long int hyper = 1;
    for (auto tau : list)
      hyper = LCM(hyper, tau->_T());
    return hyper;
  }

  /** NOT COMPLETE!
   * Function: Read the task set from file arg  
   * @param arg : the path to the file
   */
  void Taskset::read(std::string arg)
  {
  }

  /** NOT COMPLETE!
   * Function: Write  the task set to the file  arg 
   * @param arg : the path to the file
   */
  void Taskset::write(std::string arg)
  {
    std::ofstream myfile;
    myfile.open(arg);
    myfile.close();
  }

  /**
   * Function: Print taskset information
   */
  void Taskset::display()
  {
    for (auto tau : list)
      tau->display();
  }

  /**
   * Function: return first taskset or NULL
   */
  task::Task *Taskset::select_task()
  {
    if (list.size() == 0)
    {
      printf("Size is equal to zero \n");
      return NULL;
    }
    printf("will return the fisrt element \n");
    return list[0];
  }

  /**
   * Function: remove task tau from taskset if it exists
   */
  bool Taskset::remove_task(Task *tau)
  {
    if (tau == NULL)
    {
      printf("removing task : false because input is null \n");
      return false;
    }
    int i = 0;
    for (auto task : list)
    {
      if (tau == task)
      {
        list.erase(list.begin() + i);
        printf("removing task : true \n");
        return true;
      }
      i++;
    }
    printf("removing task : false because not found \n");
    return false;
  }

  /**
   *  Function: Getter of taskset size 
   *  @return list.size() std::vector<Task *> list.
   */
  int Taskset::_size()
  {
    return list.size();
  }

  /**
   *  Function: Return the list of tasks
   *  @return pointer to the list
   */
  std::vector<Task *> *Taskset::_list()
  {
    return &list;
  }

  /**
   *  Function: Add a task tau to the current taskset
   *  @param tau The task to add
   */
  void Taskset::add(Task *tau)
  {
    list.push_back(tau);
  }

  /**
   *  Function: Get a task (Task *) from list by index
   *  @param i the index of the target task
   */
  Task *Taskset::get(int i)
  {
    return list[i];
  }

  /**
   *  Function: Get the total number of subtasks in the taskset
   *  @param list std::vector<Task *> 
   *  @return vtotal Number of all subtasks in the taskset
   */
  int Taskset::sub_task_number()
  {
    int vtotal = 0;
    for (auto tau : list)
      vtotal += tau->_subtasks()->size();
    return vtotal;
  }

  /**
   *  Function: Check if task exists in the taskset ts
   *  @param task pointer (Task*)
   *  @param ts   pointer (Taskset*)
   *  @return pointer (Task *)
   */
  Task *Taskset::find_equivalent_task(Task *task, Taskset *ts)
  {
    for (auto taus : (*ts->_list()))
      if (taus->equals(task))
        return taus;
    return NULL;
  }

  // This function is only used for testing
  bool Taskset::equals(Taskset *ts)
  {
    for (auto tau : list)
      if (find_equivalent_task(tau, ts) == NULL)
        return false;
    return true;
  }

  void Taskset::display_results()
  {

    for (auto tau : list)
      for (auto v : (*tau->_subtasks()))
      {
        std::cout << "[Alloc : \n \t";
        v->display();
        if (v->_father()->_concrete() != NULL)
          std::cout << "\t\t [extention : vD : " << v->_int_D() << ", vO : " << v->_int_O() << "]" << std::endl;
        else
          std::cout << "\t [No concrete task selected ]" << std::endl;

        std::cout << "\t";
        if (v->_processor() == NULL)
          std::cout << "Not Allocated \n";
        else
          v->_processor()->display();
        std::cout << "]\n";
      }
  }

} /* namespace task */