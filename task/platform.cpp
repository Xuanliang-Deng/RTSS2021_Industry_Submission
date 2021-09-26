#include "platform.hpp"

typedef std::pair<int, int> pair;

std::vector<platform::Processor *>
operator*(int x, platform::Processor &y)
{
  std::vector<platform ::Processor *> pl;
  for (int i = 0; i < x; i++)
  {
    // HEY I CHANGED HERE
    platform::Processor *p = y.cloneTagOnly();
    p->_id(y._id() + i);
    pl.push_back(p);
  }
  return pl;
}

std::vector<platform::Processor *>
operator+(std::vector<platform::Processor *> y, std::vector<platform::Processor *> x)
{

  std::vector<platform ::Processor *> pl;
  for (auto a : y)
    pl.push_back(a);

  for (auto a : x)
    pl.push_back(a);

  return pl;
}

/*************  These are common use functions : LIMITED and HPC_MODE ***************/
namespace platform
{

  int Platform::_head()
  {
    return remained_tags[0];
  }

  void Platform::add_remained_tag(int tag)
  {
    this->remained_tags.push_back(tag);
  }

  std::vector<int> *Platform::_remained()
  {
    return &remained_tags;
  }

  void Platform::display()
  {
    std::cout << "[Platform  : \n";
    for (auto e : engines)
      e->display();
    std::cout << "] \n";
  }

  /**
   * Getter of engines 
   */
  std::vector<Processor *> *Platform::_engines()
  {
    return &engines;
  }

  Platform::Platform(std::vector<Processor *> procs)
  {
    this->engines = procs;
  }

  int Platform::nb_proc_tag(int tag)
  {
    int nb = 0;
    for (auto p : engines)
      if (p->_TAG() == tag)
        nb++;
    return nb;
  }

  int Platform::nb_instance_per_element(int tag)
  {
    return nb_proc_tag(tag);
  }

  void Platform::display_resume()
  {
    std::vector<int> list;
    list_tag(&list);
    std::cout << " [";
    for (auto tag : list)
      std::cout << tag_to_str(tag) << ":" << nb_proc_tag(tag) << ",";
    std::cout << "] \n";
  }
  Processor *Platform::_find_processor_by_id(int id){
    for (auto e : engines )
      if (e->_id() == id )
        return e;
    return NULL;
  }
  /*
   * Get the number of processors in the current platform
   * @return  The size of the current platform, i.e. number of engines  
   */

  int Platform::_size()
  {
    return engines.size();
  }
  /* 
   * Constructor of Platform 
   */
  Platform::Platform()
  {
  }

  /* 
   * Destructor of the current platform 
   */
  Platform::~Platform()
  {
    for (auto p : engines)
      delete p;
  }

  /* 
   * Add a processor to the current processor List 
   * @param p: Processor to add 
   */
  void Platform::add(Processor *p)
  {
    engines.push_back(p);
  }

  /* 
   * Copy the current platform, i.e.  allocation 
   * @return A copy of the current platform , i.e Allocation 
   */
  Platform *Platform::copy()
  {
    Platform *pl = new Platform();
    for (auto e : engines)
      pl->add(e);

    for (auto e : remained_tags)
      pl->add_remained_tag(e);
    return pl;
  }



/* 
   * Copy the current platform, i.e.  allocation 
   * @return A copy of the current platform , i.e Allocation 
   */
  Platform *Platform::copy_new_engines()
  {
    Platform *pl = new Platform();
    for (auto e : engines)
      pl->add(e->copy());

    for (auto e : remained_tags)
      pl->add_remained_tag(e);
    return pl;
  }





/* 
   * Copy the current platform, i.e.  allocation 
   * @return A copy of the current platform , i.e Allocation 
   */
   void Platform::deallocate(task::Task * tau)
  {

    for (auto e : engines){
      std::vector<task::Task * >  to_rem; 
      for (auto taup: (*e->_task_list())){
        if (taup == tau ){
          to_rem.push_back(tau);
        }
      }
      for (auto taup : to_rem)
        remove_from_vector(e->_task_list(), taup);
    }

  }





  /* 
   * print the current allocation 
   */
  //   void Platform::print_alloc(){
  //     std::cout<<"[C-B" <<std::endl;
  //     for (auto p : engines)
  //       p->print_ts();
  //     std::cout<<" \n C-E]" <<std::endl;

  //   }

  /* 
   * Find the processor according to its id 
   * @param id: The processor Identifier 
   * @return The processor having identifier id and  NULL if not found  
   */
  Processor *Platform::find_processor(int id)
  {
    for (auto p : engines)
      if (p->_id() == id)
        return p;
    return NULL;
  }

  void Platform::display_results()
  {
    for (auto e : engines)
    {
      e->display();
      std::cout << e->_utilization() << std::endl;
    }
  }

  double Platform::_cost()
  {
    return cost;
  }
  void Platform::_cost(double cos)
  {
    cost = cos;
  }

  /*
   * Get a processor 
   * @param i the processor index 
   * @return The processor indexed by i 
   */
  Processor *Platform::get(int i)
  {
    return engines[i];
  }
  /************************  HPC Mode ************************************/

  /*
   * Get the list of tags for a given architecture according to their 
   * @return a List of all  engines tag supported by the current architecture 
   * in the order of their number 
   */

  void Platform::list_tag(std::vector<int> *list)
  {
    std::map<int, int> nb_per_tag;
    std::vector<int> tags;
    for (auto p : engines)
    {
      if (vector_contains(&tags, p->_TAG()))
        nb_per_tag[p->_TAG()] += 1;
      else
      {
        nb_per_tag[p->_TAG()] = 1;
        tags.push_back(p->_TAG());
      }
    }
    // create an empty vector of pairs
    std::vector<pair> vec;

    // copy key-value pairs from the map to the vector
    std::copy(nb_per_tag.begin(),
              nb_per_tag.end(),
              std::back_inserter<std::vector<pair>>(vec));

    // sort the vector by increasing order of its pair's second value
    // if second value are equal, order by the pair's first value
    std::sort(vec.begin(), vec.end(),
              [](const pair &l, const pair &r)
              {
                if (l.second != r.second)
                  return l.second > r.second;

                return l.first < r.first;
              });
    // print the vector
    for (auto const &pair : vec)
    {
      list->push_back(pair.first);
    }
  }

  bool Platform::remove_one_engine(int tag)
  {
    for (auto e : engines)
    {
      if (e->_TAG() == tag)
      {
        remove_from_vector(&engines, e);
        return true;
      }
    }
    return false;
  }

  void Platform::number_engines_per_tag(std::map<int, int> *nb_per_tag)
  {

    std::vector<int> tags;
    for (auto p : engines)
    {
      if (vector_contains(&tags, p->_TAG()))
        (*nb_per_tag)[p->_TAG()] += 1;
      else
      {
        (*nb_per_tag)[p->_TAG()] = 1;
        tags.push_back(p->_TAG());
      }
    }
  }

  void Platform::_element_list(std::vector<int *> *tag){


  }
  // void Platform::get_maximum_pareto_front(task::Taskset *ts, std::map<task::Task *, int> *per_task_limit)
  // {
  //   std::map<int, int> nb_per_tag;
  //   number_engines_per_tag(&nb_per_tag);

  //   for (auto tau : (*ts->_list()))
  //   {
  //     int min = 10000;
  //     for  (auto const& x :  nb_per_tag)
  //     {
  //       int nb = std::floor(x.second/ tau->utilization_per_tag(x.first));
  //       if (nb<min)
  //         min = nb;
  //     }
  //     per_task_limit->insert({tau,min});
  //   }

  // //  for  (auto const& x :  *per_task_limit) {
  // //    std::cout<<x.first->_id()<<" -> "<<x.second<<std::endl;
  // //  }

  // }

  int Platform::dominates(Platform *s)
  {
    int res = -2;
    std::map<int, int> l1;
    number_engines_per_tag(&l1);
    std::map<int, int> l2;
    s->number_engines_per_tag(&l2);
    for (auto const &tag_num : l1)
    {
      if (res == -2)
      {
        if (tag_num.second > l2[tag_num.first])
          res = 1;
        else if (tag_num.second < l2[tag_num.first])
          res = -1;
      }
      else if ((res == 1 && tag_num.second < l2[tag_num.first]) || (res == -1 && tag_num.second > l2[tag_num.first]))
        return 0;
    }
    return res;
  }

} // namespace platform
