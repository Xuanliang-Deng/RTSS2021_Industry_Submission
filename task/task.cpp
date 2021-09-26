#include "task.hpp"
#include "../common/common.hpp"

namespace task
{
  /** 
   *  Function: Given a set of subtasks, extract their tags
   *  @param tags     std::vector<int>
   *  @param subtasks std::vector<Subtask *>
   */
  void Task::list_tag(std::vector<int> *tags)
  {
    for(auto v: subtasks)
    {
      if(!vector_contains(tags, v->_TAG()))
      {
        tags->push_back(v->_TAG());
      }
    }
  }

  /**
   * Function: Task constructor, given the task id
   * @param id The task id 
   * @param l The list of subtasks 
   */
  Task::Task(int id)
  {
    this->id = id;
    this->graph = NULL;
    this->max_nb_instance = MAX_INST;
  }

  Task::~Task()
  {
    for (long unsigned int i = 0; i < subtasks.size(); i++)
      free(graph[i]);
    free(graph);
  }

  /**
   * Function: Add up execution time of each specific TAG
   * @param c int sum of exec time
   * @param subtasks std::vector<Subtask *>
   * @param _TAG getter of Subtask's TAG
   * @param _C getter of Subtask's execution time
   */
  long int Task::exec_time_per_tag(int TAG)
  {
    long int exec_sum = 0;
    for (auto v : subtasks)
      if (v->_TAG() == TAG)
        exec_sum += v->_C();
    return exec_sum;
  }

  /******************************QUESTION: WHERE DID WE SET THIS T?**************************
   *  Function: Calculate utilization
   *  @param T period of DAG task
   *  @param TAG tag  
   */
  double Task::utilization_per_tag(int TAG)
  {
    if (T <= 0)
      fatal_error(23214, "Period is negative or null \n");
    return exec_time_per_tag(TAG) / (double)(T);
  }

  int Task::_index(Subtask *v)
  {
    return indexes[v];
  }

  /** This is the key function for adding subtasks/creating the graph
   *  Function: add subtask to task
   *  @param v Subtask*
   *  @param subtasks std::vector<Subtask *>
   */
  void Task::add(Subtask *v)
  {
    if (vector_contains(&subtasks, v))
    {
      fatal_error(98, "Subtask " + v->_label(0) + " already added to the task, an error has occured," + " check if a circuit do not exist");
    }
    this->subtasks.push_back(v);
    
    // indexes is of type std::map<Subtask *, int>, the first value is key, second is value(lindex), starting from 0;
    indexes[v] = lindex;

    /** Explanation of the process: 
     *  1. We add a new subtask v -> subtasks.
     *  2. We had a int **graph already, now we need to update it. 
     *  3. Create a new **graph_2, initialize the rows/colums corresponding to last element with 0.
     *  4. Free the old **graph
    */
    
    // Create updated graph graph_2 which includes the new subtask v
    int **graph_2 = (int **)malloc((subtasks.size()) * sizeof(int *));

    // Initialize each row with the new size (original + 1)
    for (long unsigned int i = 0; i < subtasks.size(); i++)
        graph_2[i] = (int *)malloc(subtasks.size() * sizeof(int)); 

    // Initialize the major part of updated graph with the original graph, leave last column and row
    for (long unsigned int i = 0; i < subtasks.size() - 1; i++)
      for (long unsigned int j = 0; j < subtasks.size() - 1; j++)
        graph_2[i][j] = graph[i][j];

    // Initialize the last row and column with zeros
    for (long unsigned int i = 0; i < subtasks.size(); i++)
    {
      graph_2[subtasks.size() - 1][i] = 0;
      graph_2[i][subtasks.size() - 1] = 0;
    }

    // Copy graph to the graph2, update graph with graph_2, free graph2
    int **graph2 = graph;
    graph = graph_2;
    if (subtasks.size() > 1)
    {
      for (long unsigned int i = 0; i < subtasks.size() - 1; i++)
        free(graph2[i]);
      free(graph2);
    }
    lindex++;
  }

  /** This is the key function for removing subtasks
   *  Function: add subtask to task
   *  @param v Subtask*
   *  @param subtasks std::vector<Subtask *>
   */
  void Task::remove(Subtask *v)
  {
    Task tau(id);
    for (auto vs : subtasks)
      if (vs != v)
        tau.add(vs); // add all the nodes except v

    for (auto v1 : (*tau._subtasks()))
      for (auto v2 : (*tau._subtasks()))
        if (is_predecessor(v1, v2))
          tau.link_two_subtasks(v1, v2);// this is the key function that we need to pay attention to
    std::vector<Subtask *> preds;
    predecessors(v, &preds);
    std::vector<Subtask *> succs;
    successors(v, &succs);

    for (auto v1 : preds)
      for (auto v2 : succs)
        tau.link_two_subtasks(v1, v2);
    remove_from_vector(&subtasks, v);
    indexes.clear();
    for (auto vs : subtasks)
      indexes[vs] = tau._index(vs);

    int **graph2 = graph;
    graph = (int **)malloc((subtasks.size()) * sizeof(int *));
    for (long unsigned int i = 0; i < subtasks.size(); i++)
      graph[i] = (int *)malloc(subtasks.size() * sizeof(int));

    for (auto v1 : subtasks)
      for (auto v2 : subtasks)
      {
        graph[_index(v1)][_index(v2)] = 0;
        if (tau.is_predecessor(v1, v2))
          link_two_subtasks(v1, v2);
      }
    for (long unsigned int i = 0; i < subtasks.size() + 1; i++)
      free(graph2[i]);
    free(graph2);
  }
  /////

  /**
   * Func: Create edge between node v1 and v2 
   * @param v1 The source node
   * @param v2 The sink node
   * @return Bool flag reflects the existence of edge (1 or 0)
   * Status: In progress, Sept.24, 2021
   */
  bool Task::link_two_subtasks(Subtask *v1, Subtask *v2)
  {
    bool v1_f = vector_contains(&subtasks, v1);
    bool v2_f = vector_contains(&subtasks, v2);
    if (!v1_f)
      PRINT_DEBUG_("This should never happen, "+v1->_label(0)+" not found ");
    if (!v2_f)
      PRINT_DEBUG_("This should never happen, "+v2->_label(0)+" not found ");
    if (!v1_f || !v2_f)
    {
      return false;
    }
    graph[_index(v1)][_index(v2)] = 1;
    return true;
  }


  /**
   * These getter/setter functions are reloaded
   * Getter of Period T
  */
  long int Task::_T()
  {
    return T;
  }

  /**
     * setter of  T
     * @param T The Period T to set
     */
  void Task::_T(long int period)
  {
    this->T = period;
  }

  /**
        Getter of D
    */
  long int Task::_D()
  {
    return this->D;
  }
  /**
     * setter of D
     * @param D The D to set
     */
  void Task::_D(long int deadline)
  {
    this->D = deadline;
  }

  /**
     *@return : returns the task id
     */
  int Task::_id()
  {
    return this->id;
  }

  void Task::_id(int id)
  {
    this->id = id;
  }

  long int Task::exec_time()
  {
    int c = 0;
    for (auto v : subtasks)
      c += v->_C();
    return c;
  }










  void Task::display()
  {
    std::cout << "[Task ID:" << id;
    std::cout << "D:" << _D() << "T : " << _T() << std::endl;

    std::vector<int> tags;
    for (auto v : subtasks)
    {
      if (vector_contains(&tags, v->_TAG()))
        continue;
      tags.push_back(v->_TAG());
      std::cout << "\t\t U(" << tag_to_str(v->_TAG()) << ")=" << utilization_per_tag(v->_TAG()) << std::endl;
    }

    for (auto v : subtasks)
      v->display();
    for (auto v1 : subtasks)
    {
      std::cout << std::endl;
      for (auto v2 : subtasks)
        std::cout << graph[_index(v1)][_index(v2)] << "\t";
    }
    std::cout << std::endl;
  }

  void Task::displayr()
  {
    std::cout << "[Task ID:" << id;
    std::cout << "D:" << _D() << "T : " << _T() << std::endl;
    for (auto v : subtasks)
    {
      v->display();
      std::cout << "\t [extention : vD : " << v->_int_D() << ", vO : " << v->_int_O() << "]" << std::endl;
    }
    std::cout << std::endl;
  }

  void Task::display_minimal()
  {
    std::cout << "[Task ID:" << id << ", D:" << _D() << ", T : " << _T() << "]" << std::endl;
  }

  std::vector<Subtask *> *Task::_subtasks()
  {
    return &(this->subtasks);
  }


  Task *Task::copy()
  {
    Task *copy = new Task(this->id);

    for (auto v : subtasks)
      copy->add(v);
    copy->_D(D);
    copy->_T(T);
    for (auto v1 : subtasks)
      for (auto v2 : subtasks)
        if (is_predecessor(v1, v2))
          copy->link_two_subtasks(v1, v2);
    return copy;
  }

  // This function copies a task by creating new subtasks
  // Not sure that it works correctly ithin the new version, HAVENT CHECKED IT YET
  Task *Task::complete_copy(int sid)
  {
    Task *copy = new Task(this->id);
    for (auto v : subtasks)
    {
      sid++;
      Subtask *vs = v->clone();
      vs->_id(sid);
      vs->_label("v" + std::to_string(sid));
      copy->add(vs);
    }
    copy->_D(D);
    copy->_T(T);

    for (long unsigned int i = 0; i < subtasks.size(); i++)
      for (long unsigned int j = 0; j < subtasks.size(); j++)
        if (is_predecessor(subtasks[i], subtasks[j]))
          copy->link_two_subtasks((*copy->_subtasks())[i], (*copy->_subtasks())[j]);
    return copy;
  }

  //   /******************************************** END OF COMMON FUNCTION ***********************************************/

  Subtask *Task::find_subtask_by_label(std::string lbl)
  {
    for (auto v : subtasks)
      if ((v->_label(0).compare(lbl)) == 0)
        return v;
    return NULL;
  }

  void Task::compute_IVS(Path *pi, std::vector<Subtask *> *ivs)
  {
    std::vector<Subtask *> non_ivs;
    for (auto v : (*pi->_subtasks()))
    {
      ancetors(v, &non_ivs);
      predecessors(v, &non_ivs);
    }
    vector_minus(&subtasks, &non_ivs, ivs);
  }

  int Task::get_number_nodes_per_tag(int tag)
  {
    int number_node = 0;
    for (auto v : subtasks)
      if (v->_TAG() == tag)
        number_node++;
    return number_node;
  }

  void Task::get_subtasksnumber_per_tag(platform::Platform *pl)
  {
    for (auto v : subtasks)
    {
      platform::Processor *p = new platform::Processor();
      p->_TAG(v->_TAG());
      pl->add(p);
    }
  }


  void Task::filter_IVS(int TAG, std::vector<Subtask *> *ivs, std::vector<Subtask *> *ivsr)
  {
    for (auto v : *ivs)
      if (v->_TAG() == TAG)
        ivsr->push_back(v);
  }



  // // This might not be useful for our case ****
  // void Task::convert_closing_alta()
  // {
  //   bool stop = false;
  //   while (!stop)
  //   {
  //     stop = true;
  //     for (auto v : subtasks)
  //     {

  //       if (v->_type() == CALTERNATIVE)
  //       {
  //         stop = false;
  //         std::vector<Subtask *> preds;
  //         predecessors(v, &preds);

  //         std::vector<Subtask *> succs;
  //         predecessors(v, &succs);

  //         if (succs.size() > 1 && preds.size() > 1)
  //         {
  //           v->_type(COMPUTE);
  //           v->_C(0);
  //           v->_TAG(CPU);
  //         }
  //         else
  //         {
  //           remove(v);
  //           break;
  //         }
  //       }
  //     }
  //   }
  // }





  /**
   * Compute the worst case response time analysis using jeffray work 
   * "Bounds on the scheduling of typed task systems.SIAM J. Comput.9, 3 (1980), 541–551."
   * For a platform having resources pl
   * @param pl : The number of resources where current task might execute
   * @return : The worst case response time when the task is executing alone 
   */
  int Task::WCRT_jeffray(platform::Platform *pl)
  {
    std::vector<Path *> paths;
    generate_paths(&paths);
    if (paths.size() == 0)
      return 0;
    std::sort(paths.begin(), paths.end(), less_than_path());
    int len_g = paths[0]->exec_time();

    std::vector<int> tags;
    platform::Platform pltask;
    get_subtasksnumber_per_tag(&pltask);
    pltask.list_tag(&tags);
    int maxs = 0;
    std::map<int, int> nb_per_tag;
    pl->number_engines_per_tag(&nb_per_tag);
    double rsum = 0;
    for (auto t : tags)
    {
      if (nb_per_tag[t] == 0)
        return INT_MAX;
      rsum += exec_time_per_tag(t) / (double)(nb_per_tag[t]);
      if (maxs < nb_per_tag[t])
        maxs = nb_per_tag[t];
    }
    return std::ceil(len_g + rsum - len_g / (double)(maxs));
  }

  // int Task::R_selected(int method)
  // {
  //   int R = (method==HAN)?WCRT_Han(selected_sub_platform):WCRT_jeffray(selected_sub_platform);
  //   _final_R(R);
  //   return R;
  // }


    int Task::_final_R(){
      return final_R;
    }
    void Task::_final_R(int f){
      final_R = f;
    }

  /**
   * Compute the worst case response time analysis using han work 
   * "Bounds on the scheduling of typed task systems.SIAM J. Comput.9, 3 (1980), 541–551."
   * For a platform having resources pl
   * @param pl : The number of resources where current task might execute
   * @return : The worst case response time when the task is executing alone 
   */

  int Task::WCRT_Han(platform::Platform *pl)
  {

    std::vector<Path *> paths;
    generate_paths(&paths);
    if (paths.size() == 0)
      return 0;
    std::vector<int> tags;
    platform::Platform pltask;
    get_subtasksnumber_per_tag(&pltask);
    pltask.list_tag(&tags);

    // list of tags must be extracted from task and not platform
    double rsum = 0;
    std::map<int, int> nb_per_tag;
    pl->number_engines_per_tag(&nb_per_tag);

    for (auto pi : paths)
    {
      double len_l = pi->exec_time();
      double local = 0;
      for (auto t : tags)
      {
        if (nb_per_tag[t] == 0)
          return INT_MAX;
        std::vector<Subtask *> ivs_all;
        std::vector<Subtask *> ivs_t;
        compute_IVS(pi, &ivs_all);
        filter_IVS(t, &ivs_all, &ivs_t);
        for (auto v : ivs_t)
          local += v->_C() / (double)(nb_per_tag[t]);
      }
      local += len_l;
      if (rsum < local)
        rsum = local;
    }
    return std::ceil(rsum);
  }

  int Task::_max_nb_instance()
  {
    return max_nb_instance;
  }
  void Task::_max_nb_instance(int max_nb_instances)
  {
    this->max_nb_instance = max_nb_instances;
  }

  /**
     * Copy exactely the same task without sharing structure by
     * incrementing incrementing sub-tasks indexes by s_id
     * @param s_id The index starting params
     * @return a reference to the new task object
     */
  Task *Task::copy_partial(int s_id)
  {
    Task *tau = new Task(-1);
    for (auto v : subtasks)
    {
      Subtask *s = v->clone();
      s->_label(v->_label(0));
      s->_id(v->_id() + s_id);
      tau->add(s);
    }
    for (auto v1 : subtasks)
      for (auto v2 : subtasks)
        if (is_predecessor(v1, v2))
          tau->link_two_subtasks(v1, v2);
    return tau;
  }



  /**
       * Compute the slack for the given list of subtasks.
       *
       * @param D the Task deadline
       * @param l The list of subtasks
       * @return The computed slack
       **/
  long int Task::compute_slack(long int D, Path *l)
  {
    long int curr_D = 0;
    for (auto v : (*l->_subtasks()))
    {
      if (int_D[v] > 0)
        curr_D += int_D[v];
    }
    return D - curr_D - compute_contributing_charge(l);
  }

  // /**
  //  * Compute the numbers of contributors for the given list of subtasks.
  //  *
  //  * @param l The list of subtasks used to compute the contributors
  //  * @return The computed numbers of contributors
  //  **/
  int Task::compute_contributing_number(Path *l)
  {
    int to_ret = 0;
    for (auto vv : (*l->_subtasks()))
      if (int_D[vv] <= 0 && vv->_type() == COMPUTE)
        to_ret++;
    return to_ret;
  }
  /**
     * Compute the contributing charge for the given list of subtasks.
     *
     * @param l List of subtasks used to compute the contributing charge
     * @return the computed contributing charge
     **/
  int Task::compute_contributing_charge(Path *path)
  {
    int to_ret = 0;
    for (auto v : (*path->_subtasks()))
      if (int_D[v] <= 0)
        to_ret += v->_C();
    return to_ret;
  }

  long unsigned int is = 0;
  bool Task::exist_circuit()
  {
    for (auto v : subtasks)
    {
      is = 0;
      std::vector<Subtask *> succ;
      successors(v, &succ);
      if (check_circuit(v, &succ))
        return true;
    }
    return false;
  }
  bool Task::check_circuit(Subtask *v, std::vector<Subtask *> *l)
  {
    if (vector_contains(l, v))
      return true;

    is++;
    if (is >= subtasks.size())
      return false;
    for (auto vs : *l)
    {
      std::vector<Subtask *> succ;
      successors(vs, &succ);
      if (vector_contains(&succ, v))
        return true;
      else
        return check_circuit(v, &succ);
    }
    return false;
  }

  Subtask *Task::find_equivalent_subtask(Subtask *vi, Task *tau)
  {
    for (auto v : (*tau->_subtasks()))
      if (vi->equals(v))
        return v;
    return NULL;
  }

  // used only for testing
  bool Task::equals(Task *tau)
  {
    for (auto v1 : subtasks)
    {
      Subtask *v1p = find_equivalent_subtask(v1, tau);
      if (v1p == NULL)
        return false;
      for (auto v2 : subtasks)
      {
        Subtask *v2p = find_equivalent_subtask(v2, tau);
        if (v2p == NULL)
          return false;
        if (is_predecessor(v1, v2) ^ tau->is_predecessor(v1p, v2p))
          return false;
      }
    }
    return true;
  }

  void Task::topological_sort(std::vector<Subtask *> *L)
  {

    Task *tau = copy();
    std::vector<Subtask *> entries;
    tau->list_entries(&entries);

    while (entries.size() > 0)
    {
      Subtask *n = entries[0];
      if (!remove_from_vector(&entries, n))
        fatal_error(1000, "should never happen");
      L->push_back(n);
      std::vector<Subtask *> succs;
      tau->successors(n, &succs);
      for (auto v : succs)
      {
        tau->unlink_two_subtasks(n, v);
        if (!tau->has_predecessors(v))
        {
          entries.push_back(v);
        }
      }
    }
    for (auto v1 : (*tau->_subtasks()))
      if (tau->has_predecessors(v1))
        fatal_error(97, "Should never happen, graph with circuits");
    delete tau;
  }

  //   /**
  //    * Create the dot representation of the task.
  //    * @param path The path the file will be writen to
  //    * @return The dot file has been successfully created
  //    */
  bool Task::to_dot(std::string path, int param)
  {
    PRINT_DEBUG("Calling to dot for task: " + label + "\n");
    std::ofstream outdata;
    outdata.open(path); // opens the file
    if (!outdata)
      fatal_error(200, " File can not be openned: Writting failed, exitting");
    outdata << "digraph mon_graphe {\n"
            << std::endl;
    for (auto v : subtasks)
    {
      outdata << v->_label(param);
      switch (v->_type())
      {
      case CONDITION:
        outdata << " [shape=diamond]" << std::endl;
        break;
      case COMPUTE:
        outdata << " [shape=circle]" << std::endl;
        break;

      default:
        fatal_error(201, "Unsupported node type, exiting");
      }
    }

    for (auto v1 : subtasks)
      for (auto v2 : subtasks)
        if (graph[_index(v1)][_index(v2)] == 1)
          outdata << v1->_label(param) << "->" << v2->_label(param) << std::endl;
    outdata << "}" << std::endl;
    outdata.close();
    PRINT_DEBUG("End of to_dot call \n");
    return 0;
  }

  Task *Task::_concrete()
  {
    return this->concrete;
  }

  void Task::_concrete(Task *tau)
  {
    this->concrete = tau;
  }

  



  /**
     * getter of label
     */
  std::string Task::_label()
  {
    return (ldf == 1) ? label : "tau_" + std::to_string(this->_id());
  }



  /**
     * setter of label
     * @param label The label to set
     */
  void Task::_label(std::string lbl)
  {
    ldf = 1;
    this->label = lbl;
  }


  /**
     * Links task tau in the current task where all entries of tau are successors of s
     * @param tau the task to insert
     * @param the concerned vertex
     */
  void Task::link_task_after_subtask(Task *tau, Subtask *s)
  {
    std::vector<Subtask *> entries;
    tau->list_entries(&entries);
    merge_task(tau);
    for (auto e : entries)
      link_two_subtasks(s, e);
  }

  bool Task::unlink_two_subtasks(Subtask *v1, Subtask *v2)
  {
    bool v1_f = vector_contains(&subtasks, v1);
    bool v2_f = vector_contains(&subtasks, v2);
    if (!v1_f)
      PRINT_DEBUG_("This should never happen, v1 not found ");
    if (!v2_f)
      PRINT_DEBUG_("This should never happen, v2 not found ");
    if (!v1_f || !v2_f)
    {
      return false;
    }
    graph[_index(v1)][_index(v2)] = 0;
    return true;
  }


  // functions used by the grammar
  /**
   * Add one new entry Subtask to the task
   * @param s The subtask to add to the task
   */
  void Task::link_new_entry(Subtask *s)
  {
    std::vector<Subtask *> L;
    list_entries(&L);
    add(s);
    for (auto v : L)
      if (_index(s) != _index(v))
        link_two_subtasks(s, v);
  }

  /**
   * Add on exit Subtasks to the task and linking it with all current exits
   * @param s The subtask to add
   */
  void Task::link_new_exit(Subtask *s)
  {
    std::vector<Subtask *> L;
    list_exits(&L);
    if (vector_contains(&subtasks, s))
      fatal_error(99, "Circuit detected : Node " + s->_label(0) + " already added to the task ");
    add(s);
    for (auto v : L)
      link_two_subtasks(v, s);
  }

  /**
   * Merge two tasks without linking them : just putting them in the same vertices to gather
   * @param tau The task to merge
   */
  void Task::merge_task(Task *tau)
  {
    if (tau == NULL)
    {
      PRINT_WARNING("Merging with an empty task ");
      return;
    }
    for (auto v : (*tau->_subtasks()))
    {
      add(v);
    }

    for (auto v1 : (*tau->_subtasks()))
      for (auto v2 : (*tau->_subtasks()))
        if (tau->is_predecessor(v1, v2))
          link_two_subtasks(v1, v2);
    PRINT_DEBUG("Merge_task end ");
  }

  Subtask *Task::get(int i)
  {
    return subtasks[i];
  }

  /**
   * Link 2 tasks: connecting the entries of tau to the exits of the current task 
   * @param tau The task to add at the end
   */
  void Task::link_task_after(Task *tau)
  {
    PRINT_DEBUG("link_task_after begin");
    std::vector<Subtask *> entries;
    tau->list_entries(&entries);
    std::vector<Subtask *> exits;
    list_exits(&exits);
    merge_task(tau);
    for (auto v1 : entries)
      for (auto v2 : exits)
        link_two_subtasks(v2, v1);
    PRINT_DEBUG("link_task_after end");
  }

  // calculate succesors and predecessors of a given subtask
  /**
   * Find the successors of a given subtask
   * @param v The given subtask node
   * @return The list of succesors of the given subtask
   */
  void Task::successors(Subtask *v, std::vector<Subtask *> *l)
  {
    for (auto vs : subtasks)
      if (is_predecessor(v, vs))
        l->push_back(vs);
  }

  /**
   * List the task sources (entry point subtasks)
   * @return List of subtask corresponding to the task entry points
   */
  bool Task::list_entries(std::vector<Subtask *> *l)
  {
    for (auto v : subtasks)
      if (!has_predecessors(v))
        l->push_back(v);
    return (l->size() != 0);
  }

  /**
   * List the task sinks (exit point subtasks) 
   * @return The list of subtask corresponding to the task exits
   */
  bool Task::list_exits(std::vector<Subtask *> *l)
  {
    for (auto v : subtasks)
    {
      if (!has_successors(v))
        l->push_back(v);
    }

    return l;
  }

  // need to rewrite has_predecessors and has successors that takes only Subtask * !
  // the current version if for optimisation
  /**
   * Check if given subtask has predecessors or not
   * @param v The given subtask Node
   * @return true if the subtask has predecessors, false otherwise
   */
  bool Task::has_predecessors(Subtask *v)
  {
    for (auto v2 : subtasks)
      if (graph[_index(v2)][_index(v)] == 1)
        return true;
    return false;
  }

  /**
   * Check if given subtask has successors or not
   * @param v The given subtask Node
   * @return true if the subtask has successors, false otherwise
   */
  bool Task::has_successors(Subtask *v)
  {
    for (auto s : subtasks)
      if (graph[_index(v)][_index(s)] == 1)
        return true;
    return false;
  }

  long int Task::_int_D(Subtask *v)
  {
    return int_D[v];
  }
  long int Task::_int_O(Subtask *v)
  {
    return int_O[v];
  }

  bool Task::is_predecessor(Subtask *s, Subtask *d)
  {
    if (!vector_contains(&subtasks, s) || !vector_contains(&subtasks, d))
      fatal_error(202, "Is predecessor tested with sub-tasks not belonging to the correct task " + std::to_string(vector_contains(&subtasks, s)) + "_" + std::to_string(vector_contains(&subtasks, d)) + "\n");

    return graph[_index(s)][_index(d)] == 1;
  }


  /**
   * Generate all the paths for the task.
   * @return The paths generation succeed
   */

  bool Task::generate_paths(std::vector<Path *> *paths)
  {
    PRINT_DEBUG("Generate paths called \n");
    std::vector<Subtask *> entries;
    list_entries(&entries);
    for (auto e : entries)
    {
      Path p;
      generate_single_path(e, &p, paths);
    }
    return true;
  }

  // This function generates the paths starting from a  node
  /**
   * Generate the paths starting from a subtask node and an path
   *
   * Recursively generate the paths from a subtask node. The algorithm :
   *  1. Add the subtask to the path if it's neither an Conditionnal node nor Alternative node
   *  2. Look the number of succesors of this node and :
   *      - If its 0 (sink node) add the path to the paths list
   *      - If its 1 recursively call this function with the successor node and the updated path
   *      - If ist more than 1 recursively call this function for each successor node and an copy af the updated path.
   *
   * @param v The starting subtask node
   * @param p The already existing path (can be empty)
   */
  void Task::generate_single_path(Subtask *v, Path *p, std::vector<Path *> *paths)
  {
    if (v->_type() != CONDITION && v->_type() != ORSTRUC)
      p->add(v);
    std::vector<Subtask *> succs;
    successors(v, &succs);
    if (succs.size() == 0)
    {
      paths->push_back(p->clone());
      return;
    }
    else
    {
      if (succs.size() == 1)
      {
        generate_single_path(succs[0], p, paths);
        return;
      }
      else
      {
        for (auto vv : succs)
        {
          Path p_;
          for (auto tt : (*p->_subtasks()))
            p_.add(tt);
          generate_single_path(vv, &p_, paths);
        }
      }
    }
  }



  /**
   * Generate all the children of the given subtask.
   *
   * @param v The subtask node
   * @return The list of all the children subtasks of the given one
   */
  void Task::children(Subtask *v, std::vector<Subtask *> *list)
  {
    _children(v, list);
  }

  /**
   * Recursively generate the the children of the given.
   *
   * The algorithm :
   *  1. Get the successors of the given subtask and add those to the list
   *  2. Recursively call this function for each successors and the list.
   *
   * @param v The given subtask
   * @param l The childrens list to update
   */
  void Task::_children(Subtask *v, std::vector<Subtask *> *l)
  {
    std::vector<Subtask *> child_l;
    successors(v, &child_l);
    merge_vectors_without_duplicates<task::Subtask *>(l, &child_l);
    for (auto vv : child_l)
    {
      _children(vv, l);
    }
  }

  /**
   * Generate all the children of the given subtask.
   *
   * @param v The subtask node
   * @return The list of all the children subtasks of the given one
   */
  void Task::ancetors(Subtask *v, std::vector<Subtask *> *list)
  {
    _ancetors(v, list);
  }

  /**
   * Recursively generate the the children of the given.
   *
   * The algorithm :
   *  1. Get the successors of the given subtask and add those to the list
   *  2. Recursively call this function for each successors and the list.
   *
   * @param v The given subtask
   * @param l The childrens list to update
   */
  void Task::_ancetors(Subtask *v, std::vector<Subtask *> *l)
  {
    std::vector<Subtask *> child_l;
    predecessors(v, &child_l);
    merge_vectors_without_duplicates<task::Subtask *>(l, &child_l);
    for (auto vv : child_l)
    {
      _ancetors(vv, l);
    }
  }

  void Task::UnAllocate()
  {
    for (auto v : subtasks)
    {
      v->_processor()->remove_utilization(v->_C() / (double)(T));
      v->_processor(NULL);
    }
  }

  // generates the predecessors of a given subtask
  /**
   * Generate predecessor subtask list of the given subtask
   * @param v The given subtask
   * @return The list of predecessor of the subtask
   */
  void Task::predecessors(Subtask *v, std::vector<Subtask *> *L)
  {
    for (auto v1 : subtasks)
      if (is_predecessor(v1, v))
        L->push_back(v1);
  }

  bool Task::is_tagged(int tag)
  {
    for (auto v : subtasks)
      if (v->_TAG() == tag)
        return true;
    return false;
  }



  /**
   * Set the deadlines to the subtasks according to the given method.
   *
   * The algorithm :
   *  1. Set all the deadlines to -1
   *  2. Generate and order the paths
   *  3. For each path compute the slack and if the slack is not < 0 call the assignment method of the given method
   *
   * @param METHOD the wanted method (PROP or FAIR)
   * @return true if the assignment end well, false otherwise
  //  */
  // bool Task::deadline_single_task(int METHOD)
  // {

  //   for (auto v : subtasks)
  //     if (v->_type() == COMPUTE)
  //       int_D[v] = -1;
  //     else
  //       int_D[v] = 0;

  //   for (auto curr_subtask : subtasks)
  //   {
  //     if (curr_subtask->_C() == 0)
  //       int_D[curr_subtask] = 0;
  //   }
  //   std::vector<Path *> paths;
  //   generate_paths(&paths);
  //   std::sort(paths.begin(), paths.end(), less_than_path());
  //   for (auto path : paths)
  //   {
  //     long int sl = compute_slack(_D(), path);
  //     if (sl < 0)
  //     {
  //       PRINT_DEBUG("The slack is negative, deadlines are not assigned \n");
  //       return false;
  //     }
  //     double n = (double)compute_contributing_number(path);
  //     switch (METHOD)
  //     {
  //     case PROP:
  //       prop_assignment(path, sl, n);
  //       break;
  //     case FAIR:
  //       fair_assignment(path, sl, n);
  //       break;
  //     default:
  //       std::cerr << "Undefined assignement method" << std::endl;
  //       return false;
  //     }
  //   }
  //   for (auto pp : paths)
  //     delete pp;
  //   return true;
  // }

  // Houssam : I need to check the dbf calculations

  /**
   * Assign the subtasks deadline fairly.
   *
   * The fair assignment is done by calculating the contributors of the path and by set the deadline of the non-assigned subtasks
   * the addition of the exection time of the subtask and the result of the division of the slack by the contributors.
   *
   * \f$D = C() + (\frac{slack}{contributors})\f$.
   *
   * @param curr_path The current used path to assign the deadlines
   * @param sl The slack of the path
   */
  void Task::fair_assignment(Path *curr_path, long int sl, int n)
  {

    double to_add = sl / (double)(n);
    int nbu = 0;
    Subtask *last;
    for (auto v : (*curr_path->_subtasks()))
      if (int_D[v] < 0)
      {
        nbu++;
        if (nbu == n)
          to_add = std::ceil(to_add);
        int_D[v] = v->_C() + to_add;
        last = v;
      }
    long int d_c = 0;
    for (auto v : (*curr_path->_subtasks()))
      d_c += int_D[v];
    int_D[last] += (D - d_c);
  }

  void Task::compute_offsets()
  {
    for (auto v : subtasks)
      int_O[v] = -1;

    for (auto v : subtasks)
      computeOffsetOneVertex(v);
  }
  int z = 0;
  void Task::computeOffsetOneVertex(Subtask *v)
  {
    if (int_O[v] != -1)
      return;
    std::vector<Subtask *> preds;
    predecessors(v, &preds);
    long int maxO = 0;
    for (auto vs : preds)
    {
      if (int_O[vs] == -1)
        computeOffsetOneVertex(vs);
      maxO = std::max(maxO, int_O[vs] + int_D[vs]);
    }
    int_O[v] = maxO;
  }

  /**
   * Proportionnaly assign deadlines to the path using the contributing charge.
   *
   * Assigns slack according to the contribution of the sub-task execution time in the path.
   *
   * \f$D = C() + (\lceil \frac{C()}{compute_contributing_charge()}\rceil * slack)\f$
   *
   * @param curr_path: The current path
   * @param sl:  the Slack of the path
   **/
  void Task::prop_assignment(Path *curr_path, long int sl, int n)
  {
    long int cc = compute_contributing_charge(curr_path);
    Subtask *last;
    for (auto v : (*curr_path->_subtasks()))
      if (int_D[v] <= 0)
      {
        int_D[v] = (long int)(v->_C() + std::floor((v->_C() / (double)cc) * sl));
        last = v;
      }
    // Ajusting deadlines
    long int d_c = 0;
    for (auto v : (*curr_path->_subtasks()))
      d_c += int_D[v];
    int_D[last] += (D - d_c);
  }



  void Task::_selected_sub_platform(platform::Platform *pl)
  {
    selected_sub_platform = pl;
  }
  platform::Platform *Task::_selected_sub_platform()
  {
    return selected_sub_platform;
  }

} // namespace task
