#ifndef MISAKA_ENGINE_TYPES
#define MISAKA_ENGINE_TYPES

#include <memory>

#define PTR std::shared_ptr
#define CreatePTR(type, ...) std::make_shared<type>(__VA_ARGS__)

#endif