#include "Stack.h"

namespace CoreEngine
{

Stack::Stack()
    : m_Top(-1)
{
}

Stack::~Stack()
{
}

bool Stack::Push(std::string item)
{
    if (m_Top >= (MAX-1))
    {
        OutputDebugStringA("ERROR\t\tStack Push()\t\tStack Overflow!\n");
        return false;
    }
    else
    {
        m_Stack[++m_Top] = item;
        OutputDebugStringA(std::string("INFO\t\tStack Push()\t\tItem: " + item + "\n").c_str());
        return true;
    }
}

std::string Stack::Pop()
{
    if (m_Top < 0)
    {
        OutputDebugStringA("ERROR\t\tStack Pop()\t\t\tStack Underflow!\n");
        return "";
    }
    else
    {
        OutputDebugStringA(std::string("INFO\t\tStack Pop()\t\t\tItem: " + m_Stack[m_Top] + "\n").c_str());
        return m_Stack[m_Top--];
    }
}

std::string Stack::Peek()
{
    if (m_Top < 0)
    {
        OutputDebugStringA("ERROR\t\tStack Peek()\t\tStack Underflow!\n");
        return "";
    }
    else
    {
        OutputDebugStringA(std::string("INFO\t\tStack Peek()\t\tItem: " + m_Stack[m_Top] + "\n").c_str());
        return m_Stack[m_Top];
    }
}

bool Stack::IsFull()
{
    return (m_Top >= (MAX - 1));
}

bool Stack::IsEmpty()
{
    return (m_Top < 0);
}

}