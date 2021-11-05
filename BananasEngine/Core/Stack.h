#ifndef STACK_H
#define STACK_H

#include "Common.h"

#define STACK_TEST 0

#if STACK_TEST
    #define MAX 5
#else
    #define MAX 1000
#endif

namespace CoreEngine
{

class Stack
{
private:
    int32 m_Top;
    std::string m_Stack[MAX];

public:
    Stack();
    ~Stack();

    bool Push(std::string item);
    std::string Pop();
    void Pop(int32 count);
    std::string Peek();
    std::string Parent();
    bool IsFull();
    bool IsEmpty();
};

}

#endif