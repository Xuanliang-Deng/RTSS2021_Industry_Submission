/**
 *  Xuanliang Deng, Sept.2021
 *  Understand these first before any modification.
 *  1. Reorder and group the functions
 *  2. Understand them and write unitTests to make sure it works
 *  3. How to run it as a whole, note that there are some definitions that we dont need
*/

#ifndef SUB_TASK_HPP
#define SUB_TASK_HPP

// I don't understand these h files, they are defined in loop. Anyway, it includes all files here.
#include "../common/common.hpp"
#include "task.hpp"
#include "vPath.hpp"
#include "../platform/processor.hpp"


#include <map>
#include <iostream>
#include <string.h>
#include <string>

namespace platform
{
  class Processor;
}

namespace task
{
  class Task;

  class Subtask
  {
    
  private:
  /** List of variables that we might not need:
   *  dummy 
   *  silent_subtask?
   *  sema_left, sema_right?
   *  properties?
  */
    long int id, C, D;
    std::string label; // name of the node, e.g., v10
    platform::Processor *proc;

    int ST;

    int type, TAG;
    Subtask *silent_subtask;
    std::string sema_left, sema_right;
    int nb_it; // nb represents number
    bool dummy;
    Task *father;

    std::map<std::string, std::string> properties;

    // ----- Newly added parts ------
    int criticality = 0;
    // End of additional parts



  public: // Resort these from most related to less ones
    Subtask(int id, long int C, int ST, int D, int type, int criticality, int tag, Task *fath);
    Subtask(int id);
    ~Subtask();
    bool has_predecessor();
    bool has_successor();
    bool set_successor(Subtask * v);

    Task *_father();
    void _father(Task *);

    platform::Processor *_processor();
    void _processor(platform::Processor *p);

    void add_property(std::string key, std::string value);
    std::string property(std::string key);

    void _ST(int ST);
    int _ST();
    
    bool _dummy();
    void _dummy(bool dum);
    int _nb_it();
    void _nb_it(int);
    int _criticality();
    void _criticality(int critLvl);

    int _id();
    void _id(int i);
    long int _C();
    long int _PC();
    int _D();
    void _C(long int c);
    void _PC(long int pc);
    void _label(std::string str);
    std::string _label(int mode);
    void display();
    Subtask *clone();
    bool equals(Subtask *v);
    /************************HPC Mode ************************************/
    Subtask *_silent_subtask();
    void _silent_subtask(Subtask *v);
    std::string _sema_left();
    void _sema_left(std::string sl); // what is sema
    std::string _sema_right();
    void _sema_right(std::string sr);
    void _type(int t);
    int _type();
    void _TAG(int tag);
    int _TAG();
    long int _int_O();
    long int _int_D();
    long int _T();

    // Newly added
    bool is_crit_node(Path *crit_path, std::vector<int> critNodeIndex);



  };
} /* namespace task */
#endif
