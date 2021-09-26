/**
 * Authors:  Zahaf Houssam-Eddine, Xuanliang Deng
 * Function: Define structures for DAG structure
 * Date:     Sept.10, 2021
 * Version:  v1
 * Log:      - Started implementation on Sept. 9th
 * Status:   - Done
 *           
*/
#ifndef TASK_HPP
#define TASK_HPP

#include "../common/common.hpp"
#include "subtask.hpp"
#include "vPath.hpp"
#include "../platform/processor.hpp"
#include "../platform/platform.hpp"
#include "taskset.hpp"

#include <fstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <map>

namespace platform
{
  class Platform;
}

namespace task
{
  class Taskset;
  class Subtask;
  class Path;

  class Task
  {
  private:
    /* Common attributes */
    int id;
    int ldf = 0;
    long int D;
    long int T;
    int lindex = 0;
    std::string label;
    std::vector<Subtask *> subtasks;
    std::map<Subtask *, long int> int_D;
    std::map<Subtask *, long int> int_O;
    platform::Platform *selected_sub_platform;
    Task *concrete;

    /* HPC mode attributes */

    int max_nb_instance;
    int final_R;

    std::vector<Task *> merged_tasks_DNN;

  public:
    std::map<Subtask *, int> indexes;
    int **graph;
    void list_tag(std::vector<int> *tags);


    int _final_R();
    void _final_R(int f);
    void _selected_sub_platform(platform::Platform *pl);
    platform::Platform *_selected_sub_platform();
    int R_selected(int method_HAN_Jeffay);
    void display_minimal();
    Task *complete_copy(int sid);
    Task *_concrete();
    void _concrete(Task *);
    void UnAllocate();
    void _id(int id);
    int _id();
    Task(int id); // int TAG;
    virtual ~Task();
    void add(Subtask *v);
    void display();
    long int _T();
    void _T(long int period);
    long int _D();
    int _max_nb_instance();
    void _max_nb_instance(int max_nb_instances);
    long int exec_time_per_tag(int TAG);
    Subtask *get(int i);

    bool is_predecessor(Subtask *s, Subtask *d);
    std::vector<Subtask *> *_subtasks();
    Task *copy();
    // double utilization();

    int _index(Subtask *v);

    long int compute_slack(long int D, Path *l);
    int compute_contributing_number(Path *path);
    int compute_contributing_charge(Path *path);
    bool deadline_single_task(int METHOD);
    void fair_assignment(Path *curr_path, long int sl, int n);
    void prop_assignment(Path *curr_path, long int sl, int n);

    bool is_tagged(int tag);
    bool to_dot(std::string path, int);

    void allocateOn(platform::Processor *p);

    bool exist_circuit();
    bool check_circuit(Subtask *v, std::vector<Subtask *> *l);


    void compute_offsets();
    void computeOffsetOneVertex(Subtask *v);

    Task *copy_partial(int s_id);


    std::string _label();
    void _label(std::string label);

    void link_task_after_subtask(Task *tau, Subtask *s);

    void merge_task(Task *tau);
    void link_task_after(Task *tau);
    bool unlink_two_subtasks(Subtask *v1, Subtask *v2);
    bool link_two_subtasks(Subtask *v1, Subtask *v2);
    void link_new_entry(Subtask *s);
    void link_new_exit(Subtask *s);

    bool list_entries(std::vector<Subtask *> *l);
    bool list_exits(std::vector<Subtask *> *l);

    void convert_closing_alta();
    void remove(Subtask *v);
    void topological_sort(std::vector<Subtask *> *L);
    bool equals(Task *tau);
    Subtask *find_equivalent_subtask(Subtask *vi, Task *tau);

    void generate_single_path(Subtask *v, Path *p, std::vector<Path *> *paths);


    long int _int_D(Subtask *);
    long int _int_O(Subtask *);
    bool generate_paths(std::vector<Path *> *paths);

    void children(Subtask *v, std::vector<Subtask *> *list);
    void _children(Subtask *v, std::vector<Subtask *> *l);
    void predecessors(Subtask *v, std::vector<Subtask *> *l);
    bool has_predecessors(Subtask *v);
    void ancetors(Subtask *v, std::vector<Subtask *> *list);
    void _ancetors(Subtask *v, std::vector<Subtask *> *l);
    void successors(Subtask *v, std::vector<Subtask *> *l);
    bool has_successors(Subtask *v);

    void displayr();


    double utilization_per_tag(int TAG);
    Subtask *find_subtask_by_label(std::string lbl);

    void _D(long int deadline);
    long int exec_time();

    /************************* List scheduling *********************************/
    int WCRT_jeffray(platform::Platform *pl);
    int WCRT_Han(platform::Platform *pl);
    void get_subtasksnumber_per_tag(platform::Platform *p);
    int get_number_nodes_per_tag(int tag);


    bool get_minimum_platform(platform::Platform *inpl, int METHOD);
    void compute_IVS(Path *pi, std::vector<Subtask *> *ivs);
    void filter_IVS(int TAG, std::vector<Subtask *> *ivs, std::vector<Subtask *> *ivsr);
  };

  class Sema_res
  {
  public:
    Subtask *t; // the condition node
    bool left;  // its value
    Sema_res(Subtask *t, bool left);
    ~Sema_res();
    void display();
  };

} // namespace task
#endif
