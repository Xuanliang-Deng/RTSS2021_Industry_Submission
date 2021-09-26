/**
 * Authors:  Dr.Zahaf, Xuanliang Deng
 * Function: Define functions for common usage
 * Date:     Sept.2, 2021
 * Version:  v2
 * Log:      Revised the template <typename T> part (this should be put into hpp file, cant be separate in cpp file)
 * Comments: The reason to use pointer here is we need to CHANGE the original vector after operation
 *           
*/

#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <iterator>


#define MAX_INST 99999

// Subtasks Node Type
#define COMPUTE   0
#define SYNCHRO   1
#define SENSOR    2
#define CONDITION 3
#define ORSTRUC   4

// ----- Newly added -----
// Criticality of nodes
#define NORM_NODE    0
#define CRIT_P_NODE  1
#define CRIT_NODE    2
// End of additional parts

// Tags
#define CPU       0
#define GPU       1
#define PVA       2 
#define TAG_UNDEFINED 100

// Debugging information
void fatal_error(int ErrorCode, std::string Message);
void PRINT_DEBUG(std::string str);
void PRINT_DEBUG_(std::string str);
void PRINT_WARNING(std::string str);
void FATAL_ERROR(int ErrorCode, std::string Message);

// Tested.
std::string tag_to_str(int tag);
int str_to_tag(std::string tag);

// Tested.
template <typename T> 
bool vector_contains(std::vector<T> *vec, T elem)
{
    for(auto v: (*vec))
    {
        if(v == elem)
            return true;
    }
    return false;
}

// Tested. 
template <typename T> 
bool remove_from_vector(std::vector<T> *vec, T elem)
{
    bool found = false;
    long unsigned int i = 0;
    for(i = 0; i < vec->size(); i++){
        if((*vec)[i] == elem){
            found = true;
            break;
        }
    }
    if(!found)
        return false;
    vec->erase(vec->begin()+i);
    return true;
}

// Tested.
template <typename T> 
void copy_vector(std::vector<T> *src, std::vector<T> *dst)
{
    for(auto v: (*src))
        dst->push_back(v);
}

// Tested.
template <typename T>
void vector_minus(std::vector<T> *vec1, std::vector<T> *vec2, std::vector<T> *vec3)
{
    // vec1 - vec2
    for(auto v1:(*vec1))
    {
        if(!vector_contains(vec2, v1))
            vec3->push_back(v1);
    }
}

// Tested.
template <typename T>
void merge_vectors_without_duplicates(std::vector<T> *dst, std::vector<T> *src)
{
    for(auto v:(*src))
    {
        if(!vector_contains(dst,v))
        dst->push_back(v);
    }
}

// Tested.
template <typename T>
void merge_vectors_with_duplicates(std::vector<T> *dst, std::vector<T> *src)
{
    for(auto v:(*src))
    {
        dst->push_back(v);
    }

}

#endif /* COMMON_H */