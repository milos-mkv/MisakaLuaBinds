#ifndef MISAKA_ENGINE_LOGGER
#define MISAKA_ENGINE_LOGGER

#include <iostream>

namespace Logger
{
template<typename Param>
static void Log(const Param& p) { std::cout << p << std::endl; }

template<typename Param, typename... Params>
static void Log(const Param& p, const Params&... pp) { std::cout << p << " "; Log(pp...); }
};

#define LOG(...) Logger::Log(__VA_ARGS__)

#endif