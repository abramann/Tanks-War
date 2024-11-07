#pragma once

#include <iostream>
#include <memory>
#include <codecvt>
#include <locale>
#include <vector>
#include <algorithm>

//	https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
template<typename ... Args>
inline std::string strFormat(const std::string& format, Args ... args)
{
	int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
	if (size_s <= 0) { throw std::runtime_error("Error during formatting."); }
	auto size = static_cast<size_t>(size_s);
	std::unique_ptr<char[]> buf = std::make_unique<char[]>(size);
	std::snprintf(buf.get(), size, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

// https://gist.github.com/danzek/d6a0e4a48a5439e7f808ed1497f6268e?permalink_comment_id=4289664#gistcomment-4289664
inline std::wstring to_wstring(const std::string& str)
{
	std::vector<wchar_t> buf(str.size());
	std::use_facet<std::ctype<wchar_t>>(std::locale()).widen(str.data(),
		str.data() + str.size(),
		buf.data());
	return std::wstring(buf.data(), buf.size());
}

inline int strGetRepeatedNum(std::string str, char c)
{
	int count;
	std::for_each(str.begin(), str.end(), [&count](char cstr) { count++; });
	return count;
}

inline bool strIsEqual(std::string str, std::string str2)
{
	return str.compare(str2) == 0 ? true : false;
}

inline size_t strHash(const std::string& str)
{
	std::hash<std::string> hasher; 
	return hasher(str);
}