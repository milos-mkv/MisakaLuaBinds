#ifndef MISAKA_LUA_BINDS_UTILS
#define MISAKA_LUA_BINDS_UTILS

#include <iostream>

template<typename Param>
void Log(const Param& param)
{
    std::cout << param << std::endl;
}

template<typename Param, typename... Params>
void Log(const Param& param, const Params... params)
{
    std::cout << param << " ";
    Log(params...);
}

#endif