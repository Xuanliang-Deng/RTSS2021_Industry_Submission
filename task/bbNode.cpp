/**
 * Authors:  Xuanliang Deng
 * Function: Branch and Bound Node
 * Date:     Sept.11, 2021
 * Version:  v1
 * Log:      None, first trial for implementation
 * Comments: Ongoing       
*/

#include "bbNode.hpp"

bbNode::bbNode(){}

bbNode::bbNode(int _id)
{
    this->id = _id;
}

bbNode::bbNode(int _id, int _C, int _ST, int _D, int _criticality)
{
    this->id = _id;
    this->C  = _C;
    this->ST = _ST;
    this->D  = _D;
    this->criticality = _criticality;
}

bbNode::~bbNode()
{

}

int bbNode::_id()
{
    return this->id;
}

int bbNode::_criticality()
{
    return this->criticality;
}

void bbNode::_criticality(int _criticality)
{
    this->criticality = _criticality;
}

int bbNode::_ST()
{
    return this->ST;
}

int bbNode::_D()
{
    return this->D;
}