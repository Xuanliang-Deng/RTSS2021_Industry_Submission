/**
 * Authors:  Zahaf Houssam-Eddine, Xuanliang Deng
 * Function: Define Taskset-related functions
 * Date:     Sept.5, 2021
 * Version:  v1
 * Log:      - Revised the template <typename T> part (this should be put into hpp file, cant be separate in cpp file)
 * Comments: - The reason to use pointer here is we need to CHANGE the original vector after operation
 *           
*/

#ifndef TASKSET_HPP
#define TASKSET_HPP

#include "../common/common.hpp"
#include "task.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

namespace task
{
  class Task;
  class Taskset
  {
  private:
    std::vector<Task *> list;
    int id;
  public:
    Taskset(int id);
    Taskset();
    ~Taskset();

    long int LCM(long int a, long int b);
    long int hyperperiod();
    void read(std::string arg);
    void write(std::string arg);
    void display();

    task::Task *  select_task();
    bool remove_task(Task * );
    int _size();
    std::vector<Task *> *_list();
    void add(Task *tau);
    Task *get(int i);
    int sub_task_number();
    

    Task * find_equivalent_task(Task *task, Taskset * ts);
    bool equals(Taskset *ts);
    void display_results();
  };
} /* namespace task */
#endif
