/**
 * Authors:  Dr.Zahaf, Xuanliang Deng
 * Function: Define functions for common usage
 * Date:     Sept.2, 2021
 * Version:  v2
 * Log:      Revised the template <typename T> part (this should be put into hpp file, cant be separate in cpp file)
 * Comments: The reason to use pointer here is we need to CHANGE the original vector after operation
 *           
*/
#include "common.hpp"

void fatal_error(int ErrorCode, std::string Message)
{
  std::cerr << "\033[1;31m ERROR CODE (" + std::to_string(ErrorCode) + "): \033[0m ";
  std::cerr << Message << std::endl;
  exit(-1);
}


void PRINT_DEBUG(std::string str){
    std::cout << "\033[1;34m PRING_DEBUG : \033[0m";
    std::cout << str << std::endl;
}

void PRINT_DEBUG_(std::string str)
{
  std::cout << "\033[1;34m PRINT_DEBUG : \033[0m ";
  std::cout << str << std::endl;
}


void PRINT_WARNING(std::string str){
    std::cout << "\033[1;32m PRING_DEBUG : \033[0m";
    std::cout << str << std::endl;
}

void FATAL_ERROR(int ErrorCode, std::string Message){
    std::cerr <<  "\033[1;31m ERROR CODE (" + std::to_string(ErrorCode) + "): \033[0m";
    std::cerr << Message << std::endl;
    exit(-1);
}

// Tested. Here we only have 3 cases, undefined, CPU or GPU.
int str_to_tag(std::string tag){
    if(tag.compare("CPU") == 0)
        return CPU;
    if(tag.compare("GPU") == 0)
        return GPU;
    return TAG_UNDEFINED;
}

// Tested
std::string tag_to_str(int tag){
    switch (tag)
    {
    case CPU:
        return "CPU";
        break;
    case GPU:
        return "GPU";
        break;
    case TAG_UNDEFINED:
        return "TAG_UNDEFINED";
        break;
    default:
        return "UNKNOWN";
        break;
    }
}