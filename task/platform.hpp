#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include "../common/common.hpp"
#include "processor.hpp"
#include "../task/taskset.hpp"

#include <vector>
#include <algorithm>
#include <map>

std::vector<platform::Processor *>
operator*(int x, platform::Processor &y);

std::vector<platform::Processor *>
operator+(std::vector<platform::Processor *> y, std::vector<platform::Processor *> x);

namespace platform
{

  class Platform
  {
  private:
    std::vector<platform::Processor *> engines;
    std::vector<int> remained_tags;
    double cost = 0;

  public:
    double _cost();
    void _cost(double cos);
    //    task::Taskset * not_yet_allocated = new task::Taskset();
    //Houssam : do not getters and setters for the above variable !
    bool remove_one_engine(int tag);
    void number_engines_per_tag(std::map<int, int> *nb_per_tag);
    void display_results();
    std::vector<platform::Processor *> *_engines();
    int _size();
    Platform();
    Platform(std::vector<Processor *> procs);
    ~Platform();
    void add(Processor *p);
    Platform *copy();
    Platform *copy_new_engines();

    //    void print_alloc();
    Processor *find_processor(int id);
    Processor *get(int i);
    void display();
    int _head();
    void list_tag(std::vector<int> *list);
    int nb_proc_tag(int tag);
    int nb_instance_per_element(int tag);
    bool order_processors();
    void display_resume();
    void add_remained_tag(int tag);
    std::vector<int> *_remained();
    int dominates(Platform *s);
    void _element_list(std::vector<int *> *tag);

    Processor * _find_processor_by_id(int id);
    void deallocate(task::Task *tau);
  };
} // namespace platform

#endif
