#pragma once
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <set>
#include <unordered_map>
#include <string_view>
#include <fmt/format.h>
#include <iterator>
#include <iostream>


namespace beyond_all_repair
{

// we dont have std::print yet
// so make a wrapper
template<typename... Args>
inline void print(const char* fmt, Args... args)
{
    fmt::vprint(fmt,fmt::make_format_args(args...));
}


}