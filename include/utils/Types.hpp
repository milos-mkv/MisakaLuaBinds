#ifndef MISAKA_ENGINE_TYPES
#define MISAKA_ENGINE_TYPES

#include <memory>

#define PTR std::shared_ptr
#define CreatePTR(type, ...) std::make_shared<type>(__VA_ARGS__)


#define COLOR_BORDER_4   { 0.3000, 0.3000, 0.3000, 1.00 }
#define COLOR_CHILD_BG_4 { 0.0641, 0.0641, 0.0641, 1.00 }
#define COLOR_BG_4       { 0.0941, 0.0941, 0.0941, 1.00 }
#endif