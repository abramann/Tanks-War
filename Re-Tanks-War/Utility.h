#pragma once

#include "StrUtility.h"

template <typename T>
inline void safeDelete(T ptr)
{
	if (ptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}

template <typename T>
inline void safeDeleteArray(T ptr)
{
	if (ptr)
	{
		delete[] ptr;
		ptr = nullptr;
	}
}

template <typename T>
inline void setNull(T ptr, size_t size)
{
	memset(ptr, 0, size);
}

template <typename T>
inline void setNull(T& var)
{
	memset(&var, 0, sizeof(T));
}