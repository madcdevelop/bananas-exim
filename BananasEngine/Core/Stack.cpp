#include "Stack.h"

namespace CoreEngine
{

Stack::Stack()
    : m_top(-1)
{
}

Stack::~Stack()
{
}

bool Stack::Push(std::string item)
{
    if (m_top >= (MAX-1))
    {
        OutputDebugStringA("ERROR\t\tStack Push()\t\tStack Overflow!\n");
        return false;
    }
    else
    {
        m_stack[++m_top] = item;
        return true;
    }
}

std::string Stack::Pop()
{
    if (m_top < 0)
    {
        OutputDebugStringA("ERROR\t\tStack Pop()\t\t\tStack Underflow!\n");
        return "";
    }
    else
    {
        std::string item = m_stack[m_top];
        m_stack[m_top--] = "";
        return item;
    }
}

void Stack::Pop(int32 count)
{
    for (int i = 0; i < count; i++)
        Pop();
}


std::string Stack::Peek()
{
    if (m_top < 0)
    {
        OutputDebugStringA("ERROR\t\tStack Peek()\t\tStack Underflow!\n");
        return "";
    }
    else
    {
        return m_stack[m_top];
    }
}

std::string Stack::Parent()
{
    if (m_top <= 0)
    {
        OutputDebugStringA("ERROR\t\tStack Parent()\t\tStack Underflow!\n");
        return "";
    }
    else
    {
        return m_stack[m_top-1];
    }
}

bool Stack::IsFull()
{
    return (m_top >= (MAX - 1));
}

bool Stack::IsEmpty()
{
    return (m_top < 0);
}

}