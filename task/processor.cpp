/**
 * Authors:  Zahaf Houssam-Eddine, Xuanliang Deng
 * Function: Define structures for (typed) processors
 * Date:     Sept.10, 2021
 * Version:  v1
 * Log:      - Started implementation on Sept. 9th
 * Status:   - Done
 *           
*/
#include "processor.hpp"

namespace platform
{
  /* Constructor of processor */
  
  /* Default*/
  Processor::Processor()
  {
    id = -1;
  }

  /* Reload, with ID */
  Processor::Processor(int id)
  {
    this->id = id;
  }

  /* Reload, with id & tag */
  Processor::Processor(int id, int tag)
  {
    this->id = id;
    this->TAG = tag;
  }

  /* Deconstrutor */
  Processor::~Processor() {}

  /* Setters */
  void Processor::_id(int id)
  {
    this->id = id;
  }

  void Processor::_TAG(int tag)
  {
    this->TAG = tag;
  }

  void Processor::_utilization(double util)
  {
    this->utilization = util;
  }

  /* Getters */
  int Processor::_id()
  {
    return id;
  }

  int Processor::_TAG()
  {
    return TAG;
  }
  
  double Processor::_utilization()
  {
    return utilization;
  }

  std::vector<task::Subtask *> *Processor::_sub_task_list()
  {
    return &sub_task_list;
  }

  std::vector<task::Task *> *Processor::_task_list()
  {
    return &task_list;
  }

  /* Utility */
  void Processor::add_utilization(double u)
  {
    this->utilization += u;
  }

  void Processor::remove_utilization(double u)
  {
    this->utilization -= u;
  }

  bool Processor::allocate_sub_task(task::Subtask *v)
  {
    // Here task corresponds to DAG, subtask to node
    sub_task_list.push_back(v);
    add_utilization(v->_C() / (double)(v->_father()->_T()));
    if (!vector_contains(&task_list, v->_father()))
      task_list.push_back(v->_father());
    return true;
  }

  Processor *Processor::copy()
  {
    Processor *p = new Processor(id);
    p->_utilization(utilization);
    p->_TAG(TAG);
    for (auto v : sub_task_list)
    {
      p->_sub_task_list()->push_back(v);
    }
    for (auto tau : task_list)
      p->_task_list()->push_back(tau);
    return p;
  }

  Processor *Processor::cloneTagOnly()
  {
    Processor *copy = new Processor();
    copy->_TAG(this->TAG);
    return copy;
  }

  /* Print information */
  void Processor::display()
  {
    std::string tag = tag_to_str(TAG);
    std::cout << "[ Processor id : " << id << ", TAG : " << tag << " ]" << std::endl;
  }

  void Processor::display_alloc_tasks()
  {
    std::string tag = tag_to_str(TAG);
    std::cout << "[proc " << id << ":" << ", TAG :" << tag << ", " << std::endl;
    for (auto tau : task_list)
    {
      std::cout << "\t"; tau->display_minimal();
    }
    std::cout << "]" << std::endl;
  }



} /* namespace platform */
