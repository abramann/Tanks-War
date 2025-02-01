#pragma once

#include <Windows.h>
#include <iostream>

typedef HWND HWindow;
typedef unsigned int uint;
typedef unsigned char uchar;

template<typename T>
struct Ref
{
    std::shared_ptr<T> ptr;

    T* get()
    {
        return ptr->get();
    }
};