#include "processor.hpp"

namespace platform
{

  /**
   * Constructor  of processor with a specific id  
   * @param id : The processor id to set 
   */
  Processor::Processor(int id)
  {
    this->id = id;
  }

  /**
   * Processor constructor without parameters
   * By default processor id is equal to -1 
   */
  Processor::Processor()
  {
    id = -1;
  }

  /**
   * Clones the current processor witht a similar one with same scheduling policy and tag
   * @return : a new processor descriptor. 
   */
  Processor *Processor::clonemultiply()
  {
    Processor *copy = new Processor();
    copy->_policy(policy);
    copy->_TAG(this->TAG);
    return copy;
  }

  /**
   * Add utilization to the processor 
   * @param u The utilization to add 
   */
  void Processor::add_utilization(double u)
  {
    this->utilization += u;
  }

  /**
   * Removes utilization to the processor 
   * @param u The utilization to remove 
   */
  void Processor::remove_utilization(double u)
  {
    this->utilization -= u;
  }

  /**
   * Getter of utilization  
   * @return : The current processor utilization 
   */
  double Processor::_utilization()
  {
    return utilization;
  }

  /**
   * Getter of utilization  
   * @return : The current processor utilization 
   */
  void Processor::_utilization(double util)
  {
    utilization = util;
  }
  /** 
   * setter of Processor id 
   * @return : returns the processor id 
   */
  void Processor::_id(int id)
  {
    this->id = id;
  }

  /**
   * Getter of processor id
   * @return Processor id
   */
  int Processor::_id()
  {
    return id;
  }

  /**
   * Getter of processor ISA 
   * @return The processor ISA 
   */
  int Processor::_TAG()
  {
    return TAG;
  }

  /**
   * Setter of processor tag 
   * @param tag: The processor ISA 
   */
  void Processor::_TAG(int tag)
  {
    this->TAG = tag;
  }

  /**
 * Constructor of a processor with a id, tag and scheduling policy 
 * @param id : the processor id
 * @param tag: The processor ISA 
 * @param policy: The processor scheduling policy 
 */
  Processor::Processor(int id, int tag, int policy)
  {
    this->id = id;
    this->TAG = tag;
    this->policy = policy;
  }

  /**
 * Displays the processor characteristics 
 */
  void Processor::display()
  {
    std::string tag = tag_to_str(TAG);
    std::string pol = policy_to_str(this->policy);
    std::cout << "[ Processor id : " << id << ", TAG : " << tag << ", policy : " << pol << "]" << std::endl;
  }

  /**
 * Getter of the processor scheduling policy
 * @return The current processor scheduling policy
 */
  int Processor::_policy()
  {
    return this->policy;
  }

  /**
 * Setter of the processor scheduling policy 
 * @param pol : The policy to set to the current processor
 */
  void Processor::_policy(int pol)
  {
    this->policy = pol;
  }

  bool Processor::allocate_sub_task(task::Subtask *v)
  {

    sub_task_list.push_back(v);
    add_utilization(v->_C() / (double)(v->_father()->_T()));
    if (!vector_contains(&task_list, v->_father()))
      task_list.push_back(v->_father());

    return true;
  }
  /**
   * getter of the set of tasks that are allocated to the current core
   * i.e. It is sufficient that a single sub-task of a given task to be 
   * allocated to the current core so that it is added to this list 
   */
  std::vector<task::Task *> *Processor::_task_list()
  {
    return &task_list;
  }

  std::vector<task::Subtask *> *Processor::_sub_task_list()
  {
    return &sub_task_list;
  }

  Processor *Processor::copy()
  {
    Processor *p = new Processor(id);
    p->_policy(policy);
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


  void Processor::display_alloc_tasks(){
    std::cout<<"[proc "<<id<<":"<<", TAG :" <<tag_to_str(TAG)<<", "<<std::endl;
    for (auto tau : task_list){
      std::cout<<"\t"; tau->display_minimal();
    }
    std::cout<<"]"<<std::endl;
  }


} // namespace platform
