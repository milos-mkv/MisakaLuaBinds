#ifndef MISAKA_ENGINE_TEXTURE
#define MISAKA_ENGINE_TEXTURE

#include <string>

struct Texture
{
    unsigned int id;
    const char* path;
    
    static Texture CreateTexture(const char* path);
    static void DestroyTexture(const Texture& texture);
};

#endif