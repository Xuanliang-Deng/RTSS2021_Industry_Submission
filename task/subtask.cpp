#include "subtask.hpp"
#include <string>

namespace task
{
  /** 
   * The constructor of the subtask 
   * @param id The subtask id 
   * @param C The subtask execution time 
   * @param PC the subtask preemption cost 
   * @param type The subtask type : it can be either CONDITION, COMPUTE, ORSTRUC
   * @param tag The task tag, processor that it should run onto 
   */
  Subtask::Subtask(int id, long int C, int ST, int D, int type, int criticality, int tag, Task *fathe)
  {
    this->id = id;
    this->C = C;
    this->ST = ST;
    this->D = D;
    this->type = type;
    this->criticality = criticality;
    this->TAG = tag;
    this->label = "v" + std::to_string(id);
    this->nb_it = 0;

    if (type == CONDITION || type == ORSTRUC)
    {
      this->C = 0;
    }
    

    this->father = fathe;
    if (father != NULL)
      this->father->add(this); // pay attention to this in task.cpp, this is important
    dummy = false;
  }

  Subtask::Subtask(int id)
  {
    this->id = id;
  }

  // destructor of the subtask 
  Subtask::~Subtask() {}

  // Setter: set dummy value of subtask
  void Subtask::_dummy(bool dum)
  {
    this->dummy = dum;
  }

  // Getter: dummy value of subtask
  bool Subtask::_dummy()
  {
    return dummy;
  }

  // Setter: set *proc of subtask
  void Subtask::_processor(platform::Processor *pr)
  {
    this->proc = pr;
  }

  // Getter: get *proc of subtask
  platform::Processor *Subtask::_processor()
  {
    return this->proc;
  }

  // Getter: id of subtask 
  int Subtask::_id()
  {
    return this->id;
  }

  // Setter: id of subtask
  void Subtask::_id(int i)
  {
    this->id = i;
  }

  // Setter: ST of subtask
  void Subtask::_ST(int ST)
  {
    this->ST = ST;
  }

  // Getter: ST of subtask
  int Subtask::_ST()
  {
    return this->ST;
  }

  int Subtask::_D()
  {
    return this->D;
  }

  // Getter: *father of subtask
  Task *Subtask::_father()
  {
    return this->father;
  }

  // Setter: *father of subtask
  void Subtask::_father(Task *tau)
  {
    this->father = tau;
  }

  // Getter: WCET NOTE THAT IT SHOULD BE Probabilistic here
  long int Subtask::_C()
  {
    return C;
  }

  // Setter: WCET 
  void Subtask::_C(long int c)
  {
    C = c;
  }

  // Setter: type of subtask
  void Subtask::_type(int t)
  {
    type = t;
  }

  // Getter of type 
  int Subtask::_type()
  {
    return type;
  }

  // setter of TAG 
  void Subtask::_TAG(int tag)
  {
    TAG = tag;
  }

  // Getter of TAG 
  int Subtask::_TAG()
  {
    return TAG;
  }

  // Setter of label 
  void Subtask::_label(std::string str)
  {
    this->label = str;
  }

  // Getter of label
  std::string Subtask::_label(int mode)
  {
    std::string add_C= (mode&1)?"_"+std::to_string(_C()):"";
    std::string tag= (mode&2)?"_"+tag_to_str(this->TAG):""; 
    return label+add_C+tag;
  }

  void Subtask::add_property(std::string key, std::string value)
  {
    this->properties.insert({key,value});
  }

  std::string Subtask::property(std::string key)
  {
    return properties[key];
  }

  // Only for testing
  bool Subtask::equals(Subtask *v)
  {
    return (v->_C() == C && v->_type() == type && v->_TAG() == TAG && v->_label(0).compare(_label(0))==0);
  }

  // These functions are the most important for me 
  bool Subtask::has_predecessor()
  {
    return father->has_predecessors(this);
  }
  bool Subtask::set_successor(Subtask *v)
  {
    return father->link_two_subtasks(this, v);
  }

  bool Subtask::has_successor()
  {
    return father->has_successors(this);
  }

  

  //------------------- Newly added parts -----------------------
  int Subtask::_criticality()
  {
    return this->criticality;
  }

  void Subtask::_criticality(int critLvl)
  {
    this->criticality = critLvl;
  }

  bool Subtask::is_crit_node(Path *crit_path, std::vector<int> critNodeIndex)
  {
    if(belong_to_crit_path(this, crit_path))
    {
      this->_criticality(CRIT_P_NODE);
    }

    int vIndex =  this->_id();

    auto iter = find(critNodeIndex.begin(), critNodeIndex.end(), vIndex);
    if(iter != critNodeIndex.end())
    {
      this->_criticality(CRIT_NODE);
      return true;
    }
    return false;
  }







  // Creates a new subtask and copy the same parameters as the current 
  Subtask *Subtask::clone()
  {
    Subtask *v = new Subtask(this->id, this->C, this->ST, this->D, this->type, this->criticality, this->TAG, this->father);
    return v;
  }




  // For the following int_O, int_D, idk what are these
  long int Subtask::_int_O()
  {
    return this->father->_concrete()->_int_O(this);
  }

  long int Subtask::_int_D()
  {
    if (this->father == NULL)
      printf("Build not called: Father task not found\n");
      //fatal_error(156, "Build not called : Father task not found \n ");
    return this->father->_concrete()->_int_D(this);
  }

  // what is T? maybe not period
  long int Subtask::_T(){
    return father->_T();
  }

  // Displays the current subtask 
  void Subtask::display()
  {

    std::string str = "";
    switch (type)
    {
    case CONDITION:
      str = "CONDITION";
      break;
    case ORSTRUC:
      str = "ORSTRUC";
      break;
    case COMPUTE:
      str = "COMPUTE";
      break;

    default:
      str = "UNKOWN:"+std::to_string(type)+":";
      break;
    }
    std::cout << "[ID:" << id << " LABEL:" << label << " ,C:" << C
              << ", type:" << str << ", TAG:" << tag_to_str(TAG) << " ]" << std::endl;
  }

  // what is nb_it? 
  int Subtask::_nb_it()
  {
    return nb_it;
  }
  void Subtask::_nb_it(int nb)
  {
    this->nb_it = nb;
  }



  /*
   * Getter of silent subtask, it is the closing task : used to in code gen 
   */
  Subtask *Subtask::_silent_subtask()
  {
    return silent_subtask;
  }
  /**  
     * setter of v 
     * @param v  The silent_subtask to set 
     */
  void Subtask::_silent_subtask(Subtask *v)
  {
    silent_subtask = v;
  }
  /** 
   * Getter of sema_left 
   */
  std::string Subtask::_sema_left()
  {
    return sema_left;
  }

  /**  
   * setter of sl sema_left 
   * @param sl sema_left The sl sema_left to set 
   */
  void Subtask::_sema_left(std::string sl)
  {
    sema_left = sl;
  }
  /** 
      Getter of sema_right 
  */
  std::string Subtask::_sema_right()
  {
    return sema_right;
  }
  /**  
   * setter of sr sema_right 
   * @param sr sema_right The sr sema_right to set 
   */
  void Subtask::_sema_right(std::string sr)
  {
    sema_right = sr;
  }


} // namespace task
